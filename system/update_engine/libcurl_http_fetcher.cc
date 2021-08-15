//
// Copyright (C) 2009 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "update_engine/libcurl_http_fetcher.h"

#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <string>

#include <base/bind.h>
#include <base/format_macros.h>
#include <base/location.h>
#include <base/logging.h>
#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>

#ifdef __ANDROID__
#include <cutils/qtaguid.h>
#include <private/android_filesystem_config.h>
#endif  // __ANDROID__

#include "update_engine/certificate_checker.h"
#include "update_engine/common/hardware_interface.h"
#include "update_engine/common/platform_constants.h"

using base::TimeDelta;
using brillo::MessageLoop;
using std::max;
using std::string;

// This is a concrete implementation of HttpFetcher that uses libcurl to do the
// http work.

namespace chromeos_update_engine {

namespace {

const int kNoNetworkRetrySeconds = 10;

// libcurl's CURLOPT_SOCKOPTFUNCTION callback function. Called after the socket
// is created but before it is connected. This callback tags the created socket
// so the network usage can be tracked in Android.
int LibcurlSockoptCallback(void* /* clientp */,
                           curl_socket_t curlfd,
                           curlsocktype /* purpose */) {
#ifdef __ANDROID__
  // Socket tag used by all network sockets. See qtaguid kernel module for
  // stats.
  const int kUpdateEngineSocketTag = 0x55417243;  // "CrAU" in little-endian.
  qtaguid_tagSocket(curlfd, kUpdateEngineSocketTag, AID_OTA_UPDATE);
#endif  // __ANDROID__
  return CURL_SOCKOPT_OK;
}

}  // namespace

// static
int LibcurlHttpFetcher::LibcurlCloseSocketCallback(void* clientp,
                                                   curl_socket_t item) {
#ifdef __ANDROID__
  qtaguid_untagSocket(item);
#endif  // __ANDROID__
  LibcurlHttpFetcher* fetcher = static_cast<LibcurlHttpFetcher*>(clientp);
  // Stop watching the socket before closing it.
  for (size_t t = 0; t < arraysize(fetcher->fd_task_maps_); ++t) {
    const auto fd_task_pair = fetcher->fd_task_maps_[t].find(item);
    if (fd_task_pair != fetcher->fd_task_maps_[t].end()) {
      if (!MessageLoop::current()->CancelTask(fd_task_pair->second)) {
        LOG(WARNING) << "Error canceling the watch task "
                     << fd_task_pair->second << " for "
                     << (t ? "writing" : "reading") << " the fd " << item;
      }
      fetcher->fd_task_maps_[t].erase(item);
    }
  }

  // Documentation for this callback says to return 0 on success or 1 on error.
  if (!IGNORE_EINTR(close(item)))
    return 0;
  return 1;
}

LibcurlHttpFetcher::LibcurlHttpFetcher(ProxyResolver* proxy_resolver,
                                       HardwareInterface* hardware)
    : HttpFetcher(proxy_resolver), hardware_(hardware) {
  // Dev users want a longer timeout (180 seconds) because they may
  // be waiting on the dev server to build an image.
  if (!hardware_->IsOfficialBuild())
    low_speed_time_seconds_ = kDownloadDevModeLowSpeedTimeSeconds;
  if (hardware_->IsOOBEEnabled() && !hardware_->IsOOBEComplete(nullptr))
    max_retry_count_ = kDownloadMaxRetryCountOobeNotComplete;
}

LibcurlHttpFetcher::~LibcurlHttpFetcher() {
  LOG_IF(ERROR, transfer_in_progress_)
      << "Destroying the fetcher while a transfer is in progress.";
  CancelProxyResolution();
  CleanUp();
}

bool LibcurlHttpFetcher::GetProxyType(const string& proxy,
                                      curl_proxytype* out_type) {
  if (base::StartsWith(
          proxy, "socks5://", base::CompareCase::INSENSITIVE_ASCII) ||
      base::StartsWith(
          proxy, "socks://", base::CompareCase::INSENSITIVE_ASCII)) {
    *out_type = CURLPROXY_SOCKS5_HOSTNAME;
    return true;
  }
  if (base::StartsWith(
          proxy, "socks4://", base::CompareCase::INSENSITIVE_ASCII)) {
    *out_type = CURLPROXY_SOCKS4A;
    return true;
  }
  if (base::StartsWith(
          proxy, "http://", base::CompareCase::INSENSITIVE_ASCII) ||
      base::StartsWith(
          proxy, "https://", base::CompareCase::INSENSITIVE_ASCII)) {
    *out_type = CURLPROXY_HTTP;
    return true;
  }
  if (base::StartsWith(proxy, kNoProxy, base::CompareCase::INSENSITIVE_ASCII)) {
    // known failure case. don't log.
    return false;
  }
  LOG(INFO) << "Unknown proxy type: " << proxy;
  return false;
}

void LibcurlHttpFetcher::ResumeTransfer(const string& url) {
  LOG(INFO) << "Starting/Resuming transfer";
  CHECK(!transfer_in_progress_);
  url_ = url;
  curl_multi_handle_ = curl_multi_init();
  CHECK(curl_multi_handle_);

  curl_handle_ = curl_easy_init();
  CHECK(curl_handle_);
  ignore_failure_ = false;

  // Tag and untag the socket for network usage stats.
  curl_easy_setopt(
      curl_handle_, CURLOPT_SOCKOPTFUNCTION, LibcurlSockoptCallback);
  curl_easy_setopt(
      curl_handle_, CURLOPT_CLOSESOCKETFUNCTION, LibcurlCloseSocketCallback);
  curl_easy_setopt(curl_handle_, CURLOPT_CLOSESOCKETDATA, this);

  CHECK(HasProxy());
  bool is_direct = (GetCurrentProxy() == kNoProxy);
  LOG(INFO) << "Using proxy: " << (is_direct ? "no" : "yes");
  if (is_direct) {
    CHECK_EQ(curl_easy_setopt(curl_handle_, CURLOPT_PROXY, ""), CURLE_OK);
  } else {
    CHECK_EQ(curl_easy_setopt(
                 curl_handle_, CURLOPT_PROXY, GetCurrentProxy().c_str()),
             CURLE_OK);
    // Curl seems to require us to set the protocol
    curl_proxytype type;
    if (GetProxyType(GetCurrentProxy(), &type)) {
      CHECK_EQ(curl_easy_setopt(curl_handle_, CURLOPT_PROXYTYPE, type),
               CURLE_OK);
    }
  }

  if (post_data_set_) {
    CHECK_EQ(curl_easy_setopt(curl_handle_, CURLOPT_POST, 1), CURLE_OK);
    CHECK_EQ(
        curl_easy_setopt(curl_handle_, CURLOPT_POSTFIELDS, post_data_.data()),
        CURLE_OK);
    CHECK_EQ(curl_easy_setopt(
                 curl_handle_, CURLOPT_POSTFIELDSIZE, post_data_.size()),
             CURLE_OK);
  }

  // Setup extra HTTP headers.
  if (curl_http_headers_) {
    curl_slist_free_all(curl_http_headers_);
    curl_http_headers_ = nullptr;
  }
  for (const auto& header : extra_headers_) {
    // curl_slist_append() copies the string.
    curl_http_headers_ =
        curl_slist_append(curl_http_headers_, header.second.c_str());
  }
  if (post_data_set_) {
    // Set the Content-Type HTTP header, if one was specifically set.
    if (post_content_type_ != kHttpContentTypeUnspecified) {
      const string content_type_attr = base::StringPrintf(
          "Content-Type: %s", GetHttpContentTypeString(post_content_type_));
      curl_http_headers_ =
          curl_slist_append(curl_http_headers_, content_type_attr.c_str());
    } else {
      LOG(WARNING) << "no content type set, using libcurl default";
    }
  }
  CHECK_EQ(
      curl_easy_setopt(curl_handle_, CURLOPT_HTTPHEADER, curl_http_headers_),
      CURLE_OK);

  if (bytes_downloaded_ > 0 || download_length_) {
    // Resume from where we left off.
    resume_offset_ = bytes_downloaded_;
    CHECK_GE(resume_offset_, 0);

    // Compute end offset, if one is specified. As per HTTP specification, this
    // is an inclusive boundary. Make sure it doesn't overflow.
    size_t end_offset = 0;
    if (download_length_) {
      end_offset = static_cast<size_t>(resume_offset_) + download_length_ - 1;
      CHECK_LE((size_t)resume_offset_, end_offset);
    }

    // Create a string representation of the desired range.
    string range_str = base::StringPrintf(
        "%" PRIu64 "-", static_cast<uint64_t>(resume_offset_));
    if (end_offset)
      range_str += std::to_string(end_offset);
    CHECK_EQ(curl_easy_setopt(curl_handle_, CURLOPT_RANGE, range_str.c_str()),
             CURLE_OK);
  }

  CHECK_EQ(curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, this), CURLE_OK);
  CHECK_EQ(
      curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, StaticLibcurlWrite),
      CURLE_OK);
  CHECK_EQ(curl_easy_setopt(curl_handle_, CURLOPT_URL, url_.c_str()), CURLE_OK);

  // If the connection drops under |low_speed_limit_bps_| (10
  // bytes/sec by default) for |low_speed_time_seconds_| (90 seconds,
  // 180 on non-official builds), reconnect.
  CHECK_EQ(curl_easy_setopt(
               curl_handle_, CURLOPT_LOW_SPEED_LIMIT, low_speed_limit_bps_),
           CURLE_OK);
  CHECK_EQ(curl_easy_setopt(
               curl_handle_, CURLOPT_LOW_SPEED_TIME, low_speed_time_seconds_),
           CURLE_OK);
  CHECK_EQ(curl_easy_setopt(
               curl_handle_, CURLOPT_CONNECTTIMEOUT, connect_timeout_seconds_),
           CURLE_OK);

  // By default, libcurl doesn't follow redirections. Allow up to
  // |kDownloadMaxRedirects| redirections.
  CHECK_EQ(curl_easy_setopt(curl_handle_, CURLOPT_FOLLOWLOCATION, 1), CURLE_OK);
  CHECK_EQ(
      curl_easy_setopt(curl_handle_, CURLOPT_MAXREDIRS, kDownloadMaxRedirects),
      CURLE_OK);

  // Lock down the appropriate curl options for HTTP or HTTPS depending on
  // the url.
  if (hardware_->IsOfficialBuild()) {
    if (base::StartsWith(
            url_, "http://", base::CompareCase::INSENSITIVE_ASCII)) {
      SetCurlOptionsForHttp();
    } else if (base::StartsWith(
                   url_, "https://", base::CompareCase::INSENSITIVE_ASCII)) {
      SetCurlOptionsForHttps();
#if !USE_OMAHA
    } else if (base::StartsWith(
                   url_, "file://", base::CompareCase::INSENSITIVE_ASCII)) {
      SetCurlOptionsForFile();
#endif
    } else {
      LOG(ERROR) << "Received invalid URI: " << url_;
      // Lock down to no protocol supported for the transfer.
      CHECK_EQ(curl_easy_setopt(curl_handle_, CURLOPT_PROTOCOLS, 0), CURLE_OK);
    }
  } else {
    LOG(INFO) << "Not setting http(s) curl options because we are "
              << "running a dev/test image";
  }

  CHECK_EQ(curl_multi_add_handle(curl_multi_handle_, curl_handle_), CURLM_OK);
  transfer_in_progress_ = true;
}

// Lock down only the protocol in case of HTTP.
void LibcurlHttpFetcher::SetCurlOptionsForHttp() {
  LOG(INFO) << "Setting up curl options for HTTP";
  CHECK_EQ(curl_easy_setopt(curl_handle_, CURLOPT_PROTOCOLS, CURLPROTO_HTTP),
           CURLE_OK);
  CHECK_EQ(
      curl_easy_setopt(curl_handle_, CURLOPT_REDIR_PROTOCOLS, CURLPROTO_HTTP),
      CURLE_OK);
}

// Security lock-down in official builds: makes sure that peer certificate
// verification is enabled, restricts the set of trusted certificates,
// restricts protocols to HTTPS, restricts ciphers to HIGH.
void LibcurlHttpFetcher::SetCurlOptionsForHttps() {
  LOG(INFO) << "Setting up curl options for HTTPS";
  CHECK_EQ(curl_easy_setopt(curl_handle_, CURLOPT_SSL_VERIFYPEER, 1), CURLE_OK);
  CHECK_EQ(curl_easy_setopt(curl_handle_, CURLOPT_SSL_VERIFYHOST, 2), CURLE_OK);
  CHECK_EQ(curl_easy_setopt(
               curl_handle_, CURLOPT_CAPATH, constants::kCACertificatesPath),
           CURLE_OK);
  CHECK_EQ(curl_easy_setopt(curl_handle_, CURLOPT_PROTOCOLS, CURLPROTO_HTTPS),
           CURLE_OK);
  CHECK_EQ(
      curl_easy_setopt(curl_handle_, CURLOPT_REDIR_PROTOCOLS, CURLPROTO_HTTPS),
      CURLE_OK);
  CHECK_EQ(curl_easy_setopt(curl_handle_, CURLOPT_SSL_CIPHER_LIST, "HIGH:!ADH"),
           CURLE_OK);
  if (server_to_check_ != ServerToCheck::kNone) {
    CHECK_EQ(
        curl_easy_setopt(curl_handle_, CURLOPT_SSL_CTX_DATA, &server_to_check_),
        CURLE_OK);
    CHECK_EQ(curl_easy_setopt(curl_handle_,
                              CURLOPT_SSL_CTX_FUNCTION,
                              CertificateChecker::ProcessSSLContext),
             CURLE_OK);
  }
}

// Lock down only the protocol in case of a local file.
void LibcurlHttpFetcher::SetCurlOptionsForFile() {
  LOG(INFO) << "Setting up curl options for FILE";
  CHECK_EQ(curl_easy_setopt(curl_handle_, CURLOPT_PROTOCOLS, CURLPROTO_FILE),
           CURLE_OK);
  CHECK_EQ(
      curl_easy_setopt(curl_handle_, CURLOPT_REDIR_PROTOCOLS, CURLPROTO_FILE),
      CURLE_OK);
}

// Begins the transfer, which must not have already been started.
void LibcurlHttpFetcher::BeginTransfer(const string& url) {
  CHECK(!transfer_in_progress_);
  url_ = url;
  auto closure =
      base::Bind(&LibcurlHttpFetcher::ProxiesResolved, base::Unretained(this));
  ResolveProxiesForUrl(url_, closure);
}

void LibcurlHttpFetcher::ProxiesResolved() {
  transfer_size_ = -1;
  resume_offset_ = 0;
  retry_count_ = 0;
  no_network_retry_count_ = 0;
  http_response_code_ = 0;
  terminate_requested_ = false;
  sent_byte_ = false;

  // If we are paused, we delay these two operations until Unpause is called.
  if (transfer_paused_) {
    restart_transfer_on_unpause_ = true;
    return;
  }
  ResumeTransfer(url_);
  CurlPerformOnce();
}

void LibcurlHttpFetcher::ForceTransferTermination() {
  CancelProxyResolution();
  CleanUp();
  if (delegate_) {
    // Note that after the callback returns this object may be destroyed.
    delegate_->TransferTerminated(this);
  }
}

void LibcurlHttpFetcher::TerminateTransfer() {
  if (in_write_callback_) {
    terminate_requested_ = true;
  } else {
    ForceTransferTermination();
  }
}

void LibcurlHttpFetcher::SetHeader(const string& header_name,
                                   const string& header_value) {
  string header_line = header_name + ": " + header_value;
  // Avoid the space if no data on the right side of the semicolon.
  if (header_value.empty())
    header_line = header_name + ":";
  TEST_AND_RETURN(header_line.find('\n') == string::npos);
  TEST_AND_RETURN(header_name.find(':') == string::npos);
  extra_headers_[base::ToLowerASCII(header_name)] = header_line;
}

void LibcurlHttpFetcher::CurlPerformOnce() {
  CHECK(transfer_in_progress_);
  int running_handles = 0;
  CURLMcode retcode = CURLM_CALL_MULTI_PERFORM;

  // libcurl may request that we immediately call curl_multi_perform after it
  // returns, so we do. libcurl promises that curl_multi_perform will not block.
  while (CURLM_CALL_MULTI_PERFORM == retcode) {
    retcode = curl_multi_perform(curl_multi_handle_, &running_handles);
    if (terminate_requested_) {
      ForceTransferTermination();
      return;
    }
  }

  // If the transfer completes while paused, we should ignore the failure once
  // the fetcher is unpaused.
  if (running_handles == 0 && transfer_paused_ && !ignore_failure_) {
    LOG(INFO) << "Connection closed while paused, ignoring failure.";
    ignore_failure_ = true;
  }

  if (running_handles != 0 || transfer_paused_) {
    // There's either more work to do or we are paused, so we just keep the
    // file descriptors to watch up to date and exit, until we are done with the
    // work and we are not paused.
    SetupMessageLoopSources();
    return;
  }

  // At this point, the transfer was completed in some way (error, connection
  // closed or download finished).

  GetHttpResponseCode();
  if (http_response_code_) {
    LOG(INFO) << "HTTP response code: " << http_response_code_;
    no_network_retry_count_ = 0;
  } else {
    LOG(ERROR) << "Unable to get http response code.";
  }

  // we're done!
  CleanUp();

  // TODO(petkov): This temporary code tries to deal with the case where the
  // update engine performs an update check while the network is not ready
  // (e.g., right after resume). Longer term, we should check if the network
  // is online/offline and return an appropriate error code.
  if (!sent_byte_ && http_response_code_ == 0 &&
      no_network_retry_count_ < no_network_max_retries_) {
    no_network_retry_count_++;
    retry_task_id_ = MessageLoop::current()->PostDelayedTask(
        FROM_HERE,
        base::Bind(&LibcurlHttpFetcher::RetryTimeoutCallback,
                   base::Unretained(this)),
        TimeDelta::FromSeconds(kNoNetworkRetrySeconds));
    LOG(INFO) << "No HTTP response, retry " << no_network_retry_count_;
  } else if ((!sent_byte_ && !IsHttpResponseSuccess()) ||
             IsHttpResponseError()) {
    // The transfer completed w/ error and we didn't get any bytes.
    // If we have another proxy to try, try that.
    //
    // TODO(garnold) in fact there are two separate cases here: one case is an
    // other-than-success return code (including no return code) and no
    // received bytes, which is necessary due to the way callbacks are
    // currently processing error conditions;  the second is an explicit HTTP
    // error code, where some data may have been received (as in the case of a
    // semi-successful multi-chunk fetch).  This is a confusing behavior and
    // should be unified into a complete, coherent interface.
    LOG(INFO) << "Transfer resulted in an error (" << http_response_code_
              << "), " << bytes_downloaded_ << " bytes downloaded";

    PopProxy();  // Delete the proxy we just gave up on.

    if (HasProxy()) {
      // We have another proxy. Retry immediately.
      LOG(INFO) << "Retrying with next proxy setting";
      retry_task_id_ = MessageLoop::current()->PostTask(
          FROM_HERE,
          base::Bind(&LibcurlHttpFetcher::RetryTimeoutCallback,
                     base::Unretained(this)));
    } else {
      // Out of proxies. Give up.
      LOG(INFO) << "No further proxies, indicating transfer complete";
      if (delegate_)
        delegate_->TransferComplete(this, false);  // signal fail
      return;
    }
  } else if ((transfer_size_ >= 0) && (bytes_downloaded_ < transfer_size_)) {
    if (!ignore_failure_)
      retry_count_++;
    LOG(INFO) << "Transfer interrupted after downloading " << bytes_downloaded_
              << " of " << transfer_size_ << " bytes. "
              << transfer_size_ - bytes_downloaded_ << " bytes remaining "
              << "after " << retry_count_ << " attempt(s)";

    if (retry_count_ > max_retry_count_) {
      LOG(INFO) << "Reached max attempts (" << retry_count_ << ")";
      if (delegate_)
        delegate_->TransferComplete(this, false);  // signal fail
      return;
    }
    // Need to restart transfer
    LOG(INFO) << "Restarting transfer to download the remaining bytes";
    retry_task_id_ = MessageLoop::current()->PostDelayedTask(
        FROM_HERE,
        base::Bind(&LibcurlHttpFetcher::RetryTimeoutCallback,
                   base::Unretained(this)),
        TimeDelta::FromSeconds(retry_seconds_));
  } else {
    LOG(INFO) << "Transfer completed (" << http_response_code_ << "), "
              << bytes_downloaded_ << " bytes downloaded";
    if (delegate_) {
      bool success = IsHttpResponseSuccess();
      delegate_->TransferComplete(this, success);
    }
    return;
  }
  // If we reach this point is because TransferComplete() was not called in any
  // of the previous branches. The delegate is allowed to destroy the object
  // once TransferComplete is called so this would be illegal.
  ignore_failure_ = false;
}

size_t LibcurlHttpFetcher::LibcurlWrite(void* ptr, size_t size, size_t nmemb) {
  // Update HTTP response first.
  GetHttpResponseCode();
  const size_t payload_size = size * nmemb;

  // Do nothing if no payload or HTTP response is an error.
  if (payload_size == 0 || !IsHttpResponseSuccess()) {
    LOG(INFO) << "HTTP response unsuccessful (" << http_response_code_
              << ") or no payload (" << payload_size << "), nothing to do";
    return 0;
  }

  sent_byte_ = true;
  {
    double transfer_size_double;
    CHECK_EQ(curl_easy_getinfo(curl_handle_,
                               CURLINFO_CONTENT_LENGTH_DOWNLOAD,
                               &transfer_size_double),
             CURLE_OK);
    off_t new_transfer_size = static_cast<off_t>(transfer_size_double);
    if (new_transfer_size > 0) {
      transfer_size_ = resume_offset_ + new_transfer_size;
    }
  }
  bytes_downloaded_ += payload_size;
  if (delegate_) {
    in_write_callback_ = true;
    auto should_terminate = !delegate_->ReceivedBytes(this, ptr, payload_size);
    in_write_callback_ = false;
    if (should_terminate) {
      LOG(INFO) << "Requesting libcurl to terminate transfer.";
      // Returning an amount that differs from the received size signals an
      // error condition to libcurl, which will cause the transfer to be
      // aborted.
      return 0;
    }
  }
  return payload_size;
}

void LibcurlHttpFetcher::Pause() {
  if (transfer_paused_) {
    LOG(ERROR) << "Fetcher already paused.";
    return;
  }
  transfer_paused_ = true;
  if (!transfer_in_progress_) {
    // If pause before we started a connection, we don't need to notify curl
    // about that, we will simply not start the connection later.
    return;
  }
  CHECK(curl_handle_);
  CHECK_EQ(curl_easy_pause(curl_handle_, CURLPAUSE_ALL), CURLE_OK);
}

void LibcurlHttpFetcher::Unpause() {
  if (!transfer_paused_) {
    LOG(ERROR) << "Resume attempted when fetcher not paused.";
    return;
  }
  transfer_paused_ = false;
  if (restart_transfer_on_unpause_) {
    restart_transfer_on_unpause_ = false;
    ResumeTransfer(url_);
    CurlPerformOnce();
    return;
  }
  if (!transfer_in_progress_) {
    // If resumed before starting the connection, there's no need to notify
    // anybody. We will simply start the connection once it is time.
    return;
  }
  CHECK(curl_handle_);
  CHECK_EQ(curl_easy_pause(curl_handle_, CURLPAUSE_CONT), CURLE_OK);
  // Since the transfer is in progress, we need to dispatch a CurlPerformOnce()
  // now to let the connection continue, otherwise it would be called by the
  // TimeoutCallback but with a delay.
  CurlPerformOnce();
}

// This method sets up callbacks with the MessageLoop.
void LibcurlHttpFetcher::SetupMessageLoopSources() {
  fd_set fd_read;
  fd_set fd_write;
  fd_set fd_exc;

  FD_ZERO(&fd_read);
  FD_ZERO(&fd_write);
  FD_ZERO(&fd_exc);

  int fd_max = 0;

  // Ask libcurl for the set of file descriptors we should track on its
  // behalf.
  CHECK_EQ(curl_multi_fdset(
               curl_multi_handle_, &fd_read, &fd_write, &fd_exc, &fd_max),
           CURLM_OK);

  // We should iterate through all file descriptors up to libcurl's fd_max or
  // the highest one we're tracking, whichever is larger.
  for (size_t t = 0; t < arraysize(fd_task_maps_); ++t) {
    if (!fd_task_maps_[t].empty())
      fd_max = max(fd_max, fd_task_maps_[t].rbegin()->first);
  }

  // For each fd, if we're not tracking it, track it. If we are tracking it, but
  // libcurl doesn't care about it anymore, stop tracking it. After this loop,
  // there should be exactly as many tasks scheduled in fd_task_maps_[0|1] as
  // there are read/write fds that we're tracking.
  for (int fd = 0; fd <= fd_max; ++fd) {
    // Note that fd_exc is unused in the current version of libcurl so is_exc
    // should always be false.
    bool is_exc = FD_ISSET(fd, &fd_exc) != 0;
    bool must_track[2] = {
        is_exc || (FD_ISSET(fd, &fd_read) != 0),  // track 0 -- read
        is_exc || (FD_ISSET(fd, &fd_write) != 0)  // track 1 -- write
    };
    MessageLoop::WatchMode watch_modes[2] = {
        MessageLoop::WatchMode::kWatchRead,
        MessageLoop::WatchMode::kWatchWrite,
    };

    for (size_t t = 0; t < arraysize(fd_task_maps_); ++t) {
      auto fd_task_it = fd_task_maps_[t].find(fd);
      bool tracked = fd_task_it != fd_task_maps_[t].end();

      if (!must_track[t]) {
        // If we have an outstanding io_channel, remove it.
        if (tracked) {
          MessageLoop::current()->CancelTask(fd_task_it->second);
          fd_task_maps_[t].erase(fd_task_it);
        }
        continue;
      }

      // If we are already tracking this fd, continue -- nothing to do.
      if (tracked)
        continue;

      // Track a new fd.
      fd_task_maps_[t][fd] = MessageLoop::current()->WatchFileDescriptor(
          FROM_HERE,
          fd,
          watch_modes[t],
          true,  // persistent
          base::Bind(&LibcurlHttpFetcher::CurlPerformOnce,
                     base::Unretained(this)));

      static int io_counter = 0;
      io_counter++;
      if (io_counter % 50 == 0) {
        LOG(INFO) << "io_counter = " << io_counter;
      }
    }
  }

  // Set up a timeout callback for libcurl.
  if (timeout_id_ == MessageLoop::kTaskIdNull) {
    VLOG(1) << "Setting up timeout source: " << idle_seconds_ << " seconds.";
    timeout_id_ = MessageLoop::current()->PostDelayedTask(
        FROM_HERE,
        base::Bind(&LibcurlHttpFetcher::TimeoutCallback,
                   base::Unretained(this)),
        TimeDelta::FromSeconds(idle_seconds_));
  }
}

void LibcurlHttpFetcher::RetryTimeoutCallback() {
  retry_task_id_ = MessageLoop::kTaskIdNull;
  if (transfer_paused_) {
    restart_transfer_on_unpause_ = true;
    return;
  }
  ResumeTransfer(url_);
  CurlPerformOnce();
}

void LibcurlHttpFetcher::TimeoutCallback() {
  // We always re-schedule the callback, even if we don't want to be called
  // anymore. We will remove the event source separately if we don't want to
  // be called back.
  timeout_id_ = MessageLoop::current()->PostDelayedTask(
      FROM_HERE,
      base::Bind(&LibcurlHttpFetcher::TimeoutCallback, base::Unretained(this)),
      TimeDelta::FromSeconds(idle_seconds_));

  // CurlPerformOnce() may call CleanUp(), so we need to schedule our callback
  // first, since it could be canceled by this call.
  if (transfer_in_progress_)
    CurlPerformOnce();
}

void LibcurlHttpFetcher::CleanUp() {
  MessageLoop::current()->CancelTask(retry_task_id_);
  retry_task_id_ = MessageLoop::kTaskIdNull;

  MessageLoop::current()->CancelTask(timeout_id_);
  timeout_id_ = MessageLoop::kTaskIdNull;

  for (size_t t = 0; t < arraysize(fd_task_maps_); ++t) {
    for (const auto& fd_taks_pair : fd_task_maps_[t]) {
      if (!MessageLoop::current()->CancelTask(fd_taks_pair.second)) {
        LOG(WARNING) << "Error canceling the watch task " << fd_taks_pair.second
                     << " for " << (t ? "writing" : "reading") << " the fd "
                     << fd_taks_pair.first;
      }
    }
    fd_task_maps_[t].clear();
  }

  if (curl_http_headers_) {
    curl_slist_free_all(curl_http_headers_);
    curl_http_headers_ = nullptr;
  }
  if (curl_handle_) {
    if (curl_multi_handle_) {
      CHECK_EQ(curl_multi_remove_handle(curl_multi_handle_, curl_handle_),
               CURLM_OK);
    }
    curl_easy_cleanup(curl_handle_);
    curl_handle_ = nullptr;
  }
  if (curl_multi_handle_) {
    CHECK_EQ(curl_multi_cleanup(curl_multi_handle_), CURLM_OK);
    curl_multi_handle_ = nullptr;
  }
  transfer_in_progress_ = false;
  transfer_paused_ = false;
  restart_transfer_on_unpause_ = false;
}

void LibcurlHttpFetcher::GetHttpResponseCode() {
  long http_response_code = 0;  // NOLINT(runtime/int) - curl needs long.
  if (base::StartsWith(url_, "file://", base::CompareCase::INSENSITIVE_ASCII)) {
    // Fake out a valid response code for file:// URLs.
    http_response_code_ = 299;
  } else if (curl_easy_getinfo(curl_handle_,
                               CURLINFO_RESPONSE_CODE,
                               &http_response_code) == CURLE_OK) {
    http_response_code_ = static_cast<int>(http_response_code);
  }
}

}  // namespace chromeos_update_engine
