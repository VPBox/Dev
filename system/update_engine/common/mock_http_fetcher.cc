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

#include "update_engine/common/mock_http_fetcher.h"

#include <algorithm>

#include <base/bind.h>
#include <base/logging.h>
#include <base/strings/string_util.h>
#include <base/time/time.h>
#include <gtest/gtest.h>

// This is a mock implementation of HttpFetcher which is useful for testing.

using brillo::MessageLoop;
using std::min;

namespace chromeos_update_engine {

MockHttpFetcher::~MockHttpFetcher() {
  CHECK(timeout_id_ == MessageLoop::kTaskIdNull)
      << "Call TerminateTransfer() before dtor.";
}

void MockHttpFetcher::BeginTransfer(const std::string& url) {
  EXPECT_FALSE(never_use_);
  if (fail_transfer_ || data_.empty()) {
    // No data to send, just notify of completion..
    SignalTransferComplete();
    return;
  }
  if (sent_size_ < data_.size())
    SendData(true);
}

void MockHttpFetcher::SendData(bool skip_delivery) {
  if (fail_transfer_ || sent_size_ == data_.size()) {
    SignalTransferComplete();
    return;
  }

  if (paused_) {
    // If we're paused, we should return so no callback is scheduled.
    return;
  }

  // Setup timeout callback even if the transfer is about to be completed in
  // order to get a call to |TransferComplete|.
  if (timeout_id_ == MessageLoop::kTaskIdNull) {
    timeout_id_ = MessageLoop::current()->PostDelayedTask(
        FROM_HERE,
        base::Bind(&MockHttpFetcher::TimeoutCallback, base::Unretained(this)),
        base::TimeDelta::FromMilliseconds(10));
  }

  if (!skip_delivery) {
    const size_t chunk_size =
        min(kMockHttpFetcherChunkSize, data_.size() - sent_size_);
    sent_size_ += chunk_size;
    CHECK(delegate_);
    delegate_->ReceivedBytes(this, &data_[sent_size_ - chunk_size], chunk_size);
  }
  // We may get terminated and deleted right after |ReceivedBytes| call, so we
  // should not access any class member variable after this call.
}

void MockHttpFetcher::TimeoutCallback() {
  CHECK(!paused_);
  timeout_id_ = MessageLoop::kTaskIdNull;
  CHECK_LE(sent_size_, data_.size());
  // Same here, we should not access any member variable after this call.
  SendData(false);
}

// If the transfer is in progress, aborts the transfer early.
// The transfer cannot be resumed.
void MockHttpFetcher::TerminateTransfer() {
  LOG(INFO) << "Terminating transfer.";
  // Kill any timeout, it is ok to call with kTaskIdNull.
  MessageLoop::current()->CancelTask(timeout_id_);
  timeout_id_ = MessageLoop::kTaskIdNull;
  delegate_->TransferTerminated(this);
}

void MockHttpFetcher::SetHeader(const std::string& header_name,
                                const std::string& header_value) {
  extra_headers_[base::ToLowerASCII(header_name)] = header_value;
}

std::string MockHttpFetcher::GetHeader(const std::string& header_name) const {
  const auto it = extra_headers_.find(base::ToLowerASCII(header_name));
  if (it == extra_headers_.end())
    return "";
  return it->second;
}

void MockHttpFetcher::Pause() {
  CHECK(!paused_);
  paused_ = true;
  MessageLoop::current()->CancelTask(timeout_id_);
  timeout_id_ = MessageLoop::kTaskIdNull;
}

void MockHttpFetcher::Unpause() {
  CHECK(paused_) << "You must pause before unpause.";
  paused_ = false;
  SendData(false);
}

void MockHttpFetcher::FailTransfer(int http_response_code) {
  fail_transfer_ = true;
  http_response_code_ = http_response_code;
}

void MockHttpFetcher::SignalTransferComplete() {
  // If the transfer has been failed, the HTTP response code should be set
  // already.
  if (!fail_transfer_) {
    http_response_code_ = 200;
  }
  delegate_->TransferComplete(this, !fail_transfer_);
}

}  // namespace chromeos_update_engine
