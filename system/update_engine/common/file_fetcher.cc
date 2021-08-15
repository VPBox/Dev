//
// Copyright (C) 2016 The Android Open Source Project
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

#include "update_engine/common/file_fetcher.h"

#include <algorithm>
#include <string>

#include <base/bind.h>
#include <base/format_macros.h>
#include <base/location.h>
#include <base/logging.h>
#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>
#include <brillo/streams/file_stream.h>

#include "update_engine/common/hardware_interface.h"
#include "update_engine/common/platform_constants.h"

using std::string;

namespace {

size_t kReadBufferSize = 16 * 1024;

}  // namespace

namespace chromeos_update_engine {

// static
bool FileFetcher::SupportedUrl(const string& url) {
  // Note that we require the file path to start with a "/".
  return base::StartsWith(
      url, "file:///", base::CompareCase::INSENSITIVE_ASCII);
}

FileFetcher::~FileFetcher() {
  LOG_IF(ERROR, transfer_in_progress_)
      << "Destroying the fetcher while a transfer is in progress.";
  CleanUp();
}

// Begins the transfer, which must not have already been started.
void FileFetcher::BeginTransfer(const string& url) {
  CHECK(!transfer_in_progress_);

  if (!SupportedUrl(url)) {
    LOG(ERROR) << "Unsupported file URL: " << url;
    // No HTTP error code when the URL is not supported.
    http_response_code_ = 0;
    CleanUp();
    if (delegate_)
      delegate_->TransferComplete(this, false);
    return;
  }

  string file_path = url.substr(strlen("file://"));
  stream_ =
      brillo::FileStream::Open(base::FilePath(file_path),
                               brillo::Stream::AccessMode::READ,
                               brillo::FileStream::Disposition::OPEN_EXISTING,
                               nullptr);

  if (!stream_) {
    LOG(ERROR) << "Couldn't open " << file_path;
    http_response_code_ = kHttpResponseNotFound;
    CleanUp();
    if (delegate_)
      delegate_->TransferComplete(this, false);
    return;
  }
  http_response_code_ = kHttpResponseOk;

  if (offset_)
    stream_->SetPosition(offset_, nullptr);
  bytes_copied_ = 0;
  transfer_in_progress_ = true;
  ScheduleRead();
}

void FileFetcher::TerminateTransfer() {
  CleanUp();
  if (delegate_) {
    // Note that after the callback returns this object may be destroyed.
    delegate_->TransferTerminated(this);
  }
}

void FileFetcher::ScheduleRead() {
  if (transfer_paused_ || ongoing_read_ || !transfer_in_progress_)
    return;

  buffer_.resize(kReadBufferSize);
  size_t bytes_to_read = buffer_.size();
  if (data_length_ >= 0) {
    bytes_to_read = std::min(static_cast<uint64_t>(bytes_to_read),
                             data_length_ - bytes_copied_);
  }

  if (!bytes_to_read) {
    OnReadDoneCallback(0);
    return;
  }

  ongoing_read_ = stream_->ReadAsync(
      buffer_.data(),
      bytes_to_read,
      base::Bind(&FileFetcher::OnReadDoneCallback, base::Unretained(this)),
      base::Bind(&FileFetcher::OnReadErrorCallback, base::Unretained(this)),
      nullptr);

  if (!ongoing_read_) {
    LOG(ERROR) << "Unable to schedule an asynchronous read from the stream.";
    CleanUp();
    if (delegate_)
      delegate_->TransferComplete(this, false);
  }
}

void FileFetcher::OnReadDoneCallback(size_t bytes_read) {
  ongoing_read_ = false;
  if (bytes_read == 0) {
    CleanUp();
    if (delegate_)
      delegate_->TransferComplete(this, true);
  } else {
    bytes_copied_ += bytes_read;
    if (delegate_ &&
        !delegate_->ReceivedBytes(this, buffer_.data(), bytes_read))
      return;
    ScheduleRead();
  }
}

void FileFetcher::OnReadErrorCallback(const brillo::Error* error) {
  LOG(ERROR) << "Asynchronous read failed: " << error->GetMessage();
  CleanUp();
  if (delegate_)
    delegate_->TransferComplete(this, false);
}

void FileFetcher::Pause() {
  if (transfer_paused_) {
    LOG(ERROR) << "Fetcher already paused.";
    return;
  }
  transfer_paused_ = true;
}

void FileFetcher::Unpause() {
  if (!transfer_paused_) {
    LOG(ERROR) << "Resume attempted when fetcher not paused.";
    return;
  }
  transfer_paused_ = false;
  ScheduleRead();
}

void FileFetcher::CleanUp() {
  if (stream_) {
    stream_->CancelPendingAsyncOperations();
    stream_->CloseBlocking(nullptr);
    stream_.reset();
  }
  // Destroying the |stream_| releases the callback, so we don't have any
  // ongoing read at this point.
  ongoing_read_ = false;
  buffer_ = brillo::Blob();

  transfer_in_progress_ = false;
  transfer_paused_ = false;
}

}  // namespace chromeos_update_engine
