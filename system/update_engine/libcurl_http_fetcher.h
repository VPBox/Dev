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

#ifndef UPDATE_ENGINE_LIBCURL_HTTP_FETCHER_H_
#define UPDATE_ENGINE_LIBCURL_HTTP_FETCHER_H_

#include <map>
#include <memory>
#include <string>
#include <utility>

#include <curl/curl.h>

#include <base/logging.h>
#include <base/macros.h>
#include <brillo/message_loops/message_loop.h>

#include "update_engine/certificate_checker.h"
#include "update_engine/common/hardware_interface.h"
#include "update_engine/common/http_fetcher.h"

// This is a concrete implementation of HttpFetcher that uses libcurl to do the
// http work.

namespace chromeos_update_engine {

class LibcurlHttpFetcher : public HttpFetcher {
 public:
  LibcurlHttpFetcher(ProxyResolver* proxy_resolver,
                     HardwareInterface* hardware);

  // Cleans up all internal state. Does not notify delegate
  ~LibcurlHttpFetcher() override;

  void SetOffset(off_t offset) override { bytes_downloaded_ = offset; }

  void SetLength(size_t length) override { download_length_ = length; }
  void UnsetLength() override { SetLength(0); }

  // Begins the transfer if it hasn't already begun.
  void BeginTransfer(const std::string& url) override;

  // If the transfer is in progress, aborts the transfer early. The transfer
  // cannot be resumed.
  void TerminateTransfer() override;

  // Pass the headers to libcurl.
  void SetHeader(const std::string& header_name,
                 const std::string& header_value) override;

  // Suspend the transfer by calling curl_easy_pause(CURLPAUSE_ALL).
  void Pause() override;

  // Resume the transfer by calling curl_easy_pause(CURLPAUSE_CONT).
  void Unpause() override;

  // Libcurl sometimes asks to be called back after some time while
  // leaving that time unspecified. In that case, we pick a reasonable
  // default of one second, but it can be overridden here. This is
  // primarily useful for testing.
  // From http://curl.haxx.se/libcurl/c/curl_multi_timeout.html:
  //     if libcurl returns a -1 timeout here, it just means that libcurl
  //     currently has no stored timeout value. You must not wait too long
  //     (more than a few seconds perhaps) before you call
  //     curl_multi_perform() again.
  void set_idle_seconds(int seconds) override { idle_seconds_ = seconds; }

  // Sets the retry timeout. Useful for testing.
  void set_retry_seconds(int seconds) override { retry_seconds_ = seconds; }

  void set_no_network_max_retries(int retries) {
    no_network_max_retries_ = retries;
  }

  void set_server_to_check(ServerToCheck server_to_check) {
    server_to_check_ = server_to_check;
  }

  size_t GetBytesDownloaded() override {
    return static_cast<size_t>(bytes_downloaded_);
  }

  void set_low_speed_limit(int low_speed_bps, int low_speed_sec) override {
    low_speed_limit_bps_ = low_speed_bps;
    low_speed_time_seconds_ = low_speed_sec;
  }

  void set_connect_timeout(int connect_timeout_seconds) override {
    connect_timeout_seconds_ = connect_timeout_seconds;
  }

  void set_max_retry_count(int max_retry_count) override {
    max_retry_count_ = max_retry_count;
  }

 private:
  // libcurl's CURLOPT_CLOSESOCKETFUNCTION callback function. Called when
  // closing a socket created with the CURLOPT_OPENSOCKETFUNCTION callback.
  static int LibcurlCloseSocketCallback(void* clientp, curl_socket_t item);

  // Callback for when proxy resolution has completed. This begins the
  // transfer.
  void ProxiesResolved();

  // Asks libcurl for the http response code and stores it in the object.
  void GetHttpResponseCode();

  // Checks whether stored HTTP response is within the success range.
  inline bool IsHttpResponseSuccess() {
    return (http_response_code_ >= 200 && http_response_code_ < 300);
  }

  // Checks whether stored HTTP response is within the error range. This
  // includes both errors with the request (4xx) and server errors (5xx).
  inline bool IsHttpResponseError() {
    return (http_response_code_ >= 400 && http_response_code_ < 600);
  }

  // Resumes a transfer where it left off. This will use the
  // HTTP Range: header to make a new connection from where the last
  // left off.
  virtual void ResumeTransfer(const std::string& url);

  void TimeoutCallback();
  void RetryTimeoutCallback();

  // Calls into curl_multi_perform to let libcurl do its work. Returns after
  // curl_multi_perform is finished, which may actually be after more than
  // one call to curl_multi_perform. This method will set up the message
  // loop with sources for future work that libcurl will do, if any, or complete
  // the transfer and finish the action if no work left to do.
  // This method will not block.
  void CurlPerformOnce();

  // Sets up message loop sources as needed by libcurl. This is generally
  // the file descriptor of the socket and a timer in case nothing happens
  // on the fds.
  void SetupMessageLoopSources();

  // Callback called by libcurl when new data has arrived on the transfer
  size_t LibcurlWrite(void* ptr, size_t size, size_t nmemb);
  static size_t StaticLibcurlWrite(void* ptr,
                                   size_t size,
                                   size_t nmemb,
                                   void* stream) {
    return reinterpret_cast<LibcurlHttpFetcher*>(stream)->LibcurlWrite(
        ptr, size, nmemb);
  }

  // Cleans up the following if they are non-null:
  // curl(m) handles, fd_task_maps_, timeout_id_.
  void CleanUp();

  // Force terminate the transfer. This will invoke the delegate's (if any)
  // TransferTerminated callback so, after returning, this fetcher instance may
  // be destroyed.
  void ForceTransferTermination();

  // Sets the curl options for HTTP URL.
  void SetCurlOptionsForHttp();

  // Sets the curl options for HTTPS URL.
  void SetCurlOptionsForHttps();

  // Sets the curl options for file URI.
  void SetCurlOptionsForFile();

  // Convert a proxy URL into a curl proxy type, if applicable. Returns true iff
  // conversion was successful, false otherwise (in which case nothing is
  // written to |out_type|).
  bool GetProxyType(const std::string& proxy, curl_proxytype* out_type);

  // Hardware interface used to query dev-mode and official build settings.
  HardwareInterface* hardware_;

  // Handles for the libcurl library
  CURLM* curl_multi_handle_{nullptr};
  CURL* curl_handle_{nullptr};
  struct curl_slist* curl_http_headers_{nullptr};

  // The extra headers that will be sent on each request.
  std::map<std::string, std::string> extra_headers_;

  // Lists of all read(0)/write(1) file descriptors that we're waiting on from
  // the message loop. libcurl may open/close descriptors and switch their
  // directions so maintain two separate lists so that watch conditions can be
  // set appropriately.
  std::map<int, brillo::MessageLoop::TaskId> fd_task_maps_[2];

  // The TaskId of the timer we're waiting on. kTaskIdNull if we are not waiting
  // on it.
  brillo::MessageLoop::TaskId timeout_id_{brillo::MessageLoop::kTaskIdNull};

  bool transfer_in_progress_{false};
  bool transfer_paused_{false};

  // Whether it should ignore transfer failures for the purpose of retrying the
  // connection.
  bool ignore_failure_{false};

  // Whether we should restart the transfer once Unpause() is called. This can
  // be caused because either the connection dropped while pause or the proxy
  // was resolved and we never started the transfer in the first place.
  bool restart_transfer_on_unpause_{false};

  // The transfer size. -1 if not known.
  off_t transfer_size_{0};

  // How many bytes have been downloaded and sent to the delegate.
  off_t bytes_downloaded_{0};

  // The remaining maximum number of bytes to download. Zero represents an
  // unspecified length.
  size_t download_length_{0};

  // If we resumed an earlier transfer, data offset that we used for the
  // new connection.  0 otherwise.
  // In this class, resume refers to resuming a dropped HTTP connection,
  // not to resuming an interrupted download.
  off_t resume_offset_{0};

  // Number of resumes performed so far and the max allowed.
  int retry_count_{0};
  int max_retry_count_{kDownloadMaxRetryCount};

  // Seconds to wait before retrying a resume.
  int retry_seconds_{20};

  // When waiting for a retry, the task id of the retry callback.
  brillo::MessageLoop::TaskId retry_task_id_{brillo::MessageLoop::kTaskIdNull};

  // Number of resumes due to no network (e.g., HTTP response code 0).
  int no_network_retry_count_{0};
  int no_network_max_retries_{0};

  // Seconds to wait before asking libcurl to "perform".
  int idle_seconds_{1};

  // If true, we are currently performing a write callback on the delegate.
  bool in_write_callback_{false};

  // If true, we have returned at least one byte in the write callback
  // to the delegate.
  bool sent_byte_{false};

  // We can't clean everything up while we're in a write callback, so
  // if we get a terminate request, queue it until we can handle it.
  bool terminate_requested_{false};

  // The ServerToCheck used when checking this connection's certificate. If no
  // certificate check needs to be performed, this should be set to
  // ServerToCheck::kNone.
  ServerToCheck server_to_check_{ServerToCheck::kNone};

  int low_speed_limit_bps_{kDownloadLowSpeedLimitBps};
  int low_speed_time_seconds_{kDownloadLowSpeedTimeSeconds};
  int connect_timeout_seconds_{kDownloadConnectTimeoutSeconds};

  DISALLOW_COPY_AND_ASSIGN(LibcurlHttpFetcher);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_LIBCURL_HTTP_FETCHER_H_
