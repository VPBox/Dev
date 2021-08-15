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

#ifndef UPDATE_ENGINE_COMMON_MOCK_HTTP_FETCHER_H_
#define UPDATE_ENGINE_COMMON_MOCK_HTTP_FETCHER_H_

#include <map>
#include <string>
#include <vector>

#include <base/logging.h>
#include <brillo/message_loops/message_loop.h>

#include "update_engine/common/http_fetcher.h"

// This is a mock implementation of HttpFetcher which is useful for testing.
// All data must be passed into the ctor. When started, MockHttpFetcher will
// deliver the data in chunks of size kMockHttpFetcherChunkSize. To simulate
// a network failure, you can call FailTransfer().

namespace chromeos_update_engine {

// MockHttpFetcher will send a chunk of data down in each call to BeginTransfer
// and Unpause. For the other chunks of data, a callback is put on the run
// loop and when that's called, another chunk is sent down.
const size_t kMockHttpFetcherChunkSize(65536);

class MockHttpFetcher : public HttpFetcher {
 public:
  // The data passed in here is copied and then passed to the delegate after
  // the transfer begins.
  MockHttpFetcher(const uint8_t* data,
                  size_t size,
                  ProxyResolver* proxy_resolver)
      : HttpFetcher(proxy_resolver),
        sent_size_(0),
        timeout_id_(brillo::MessageLoop::kTaskIdNull),
        paused_(false),
        fail_transfer_(false),
        never_use_(false) {
    data_.insert(data_.end(), data, data + size);
  }

  // Constructor overload for string data.
  MockHttpFetcher(const char* data, size_t size, ProxyResolver* proxy_resolver)
      : MockHttpFetcher(
            reinterpret_cast<const uint8_t*>(data), size, proxy_resolver) {}

  // Cleans up all internal state. Does not notify delegate
  ~MockHttpFetcher() override;

  // Ignores this.
  void SetOffset(off_t offset) override {
    sent_size_ = offset;
    if (delegate_)
      delegate_->SeekToOffset(offset);
  }

  // Do nothing.
  void SetLength(size_t length) override {}
  void UnsetLength() override {}
  void set_low_speed_limit(int low_speed_bps, int low_speed_sec) override {}
  void set_connect_timeout(int connect_timeout_seconds) override {}
  void set_max_retry_count(int max_retry_count) override {}

  // Dummy: no bytes were downloaded.
  size_t GetBytesDownloaded() override { return sent_size_; }

  // Begins the transfer if it hasn't already begun.
  void BeginTransfer(const std::string& url) override;

  // If the transfer is in progress, aborts the transfer early.
  // The transfer cannot be resumed.
  void TerminateTransfer() override;

  void SetHeader(const std::string& header_name,
                 const std::string& header_value) override;

  // Return the value of the header |header_name| or the empty string if not
  // set.
  std::string GetHeader(const std::string& header_name) const;

  // Suspend the mock transfer.
  void Pause() override;

  // Resume the mock transfer.
  void Unpause() override;

  // Fail the transfer. This simulates a network failure.
  void FailTransfer(int http_response_code);

  // If set to true, this will EXPECT fail on BeginTransfer
  void set_never_use(bool never_use) { never_use_ = never_use; }

  const brillo::Blob& post_data() const { return post_data_; }

 private:
  // Sends data to the delegate and sets up a timeout callback if needed. There
  // must be a delegate. If |skip_delivery| is true, no bytes will be delivered,
  // but the callbacks still be set if needed.
  void SendData(bool skip_delivery);

  // Callback for when our message loop timeout expires.
  void TimeoutCallback();

  // Sets the HTTP response code and signals to the delegate that the transfer
  // is complete.
  void SignalTransferComplete();

  // A full copy of the data we'll return to the delegate
  brillo::Blob data_;

  // The number of bytes we've sent so far
  size_t sent_size_;

  // The extra headers set.
  std::map<std::string, std::string> extra_headers_;

  // The TaskId of the timeout callback. After each chunk of data sent, we
  // time out for 0s just to make sure that run loop services other clients.
  brillo::MessageLoop::TaskId timeout_id_;

  // True iff the fetcher is paused.
  bool paused_;

  // Set to true if the transfer should fail.
  bool fail_transfer_;

  // Set to true if BeginTransfer should EXPECT fail.
  bool never_use_;

  DISALLOW_COPY_AND_ASSIGN(MockHttpFetcher);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_MOCK_HTTP_FETCHER_H_
