//
// Copyright (C) 2010 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_COMMON_MULTI_RANGE_HTTP_FETCHER_H_
#define UPDATE_ENGINE_COMMON_MULTI_RANGE_HTTP_FETCHER_H_

#include <deque>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "update_engine/common/http_fetcher.h"

// This class is a simple wrapper around an HttpFetcher. The client
// specifies a vector of byte ranges. MultiRangeHttpFetcher will fetch bytes
// from those offsets, using the same bash fetcher for all ranges. Thus, the
// fetcher must support beginning a transfer after one has stopped. Pass -1
// as a length to specify unlimited length. It really only would make sense
// for the last range specified to have unlimited length, tho it is legal for
// other entries to have unlimited length.

// There are three states a MultiRangeHttpFetcher object will be in:
// - Stopped (start state)
// - Downloading
// - Pending transfer ended
// Various functions below that might change state indicate possible
// state changes.

namespace chromeos_update_engine {

class MultiRangeHttpFetcher : public HttpFetcher, public HttpFetcherDelegate {
 public:
  // Takes ownership of the passed in fetcher.
  explicit MultiRangeHttpFetcher(HttpFetcher* base_fetcher)
      : HttpFetcher(base_fetcher->proxy_resolver()),
        base_fetcher_(base_fetcher),
        base_fetcher_active_(false),
        pending_transfer_ended_(false),
        terminating_(false),
        current_index_(0),
        bytes_received_this_range_(0) {}
  ~MultiRangeHttpFetcher() override {}

  void ClearRanges() { ranges_.clear(); }

  void AddRange(off_t offset, size_t size) {
    CHECK_GT(size, static_cast<size_t>(0));
    ranges_.push_back(Range(offset, size));
  }

  void AddRange(off_t offset) { ranges_.push_back(Range(offset)); }

  // HttpFetcher overrides.
  void SetOffset(off_t offset) override;

  void SetLength(size_t length) override {}  // unsupported
  void UnsetLength() override {}

  // Begins the transfer to the specified URL.
  // State change: Stopped -> Downloading
  // (corner case: Stopped -> Stopped for an empty request)
  void BeginTransfer(const std::string& url) override;

  // State change: Downloading -> Pending transfer ended
  void TerminateTransfer() override;

  void SetHeader(const std::string& header_name,
                 const std::string& header_value) override {
    base_fetcher_->SetHeader(header_name, header_value);
  }

  void Pause() override { base_fetcher_->Pause(); }

  void Unpause() override { base_fetcher_->Unpause(); }

  // These functions are overloaded in LibcurlHttp fetcher for testing purposes.
  void set_idle_seconds(int seconds) override {
    base_fetcher_->set_idle_seconds(seconds);
  }
  void set_retry_seconds(int seconds) override {
    base_fetcher_->set_retry_seconds(seconds);
  }
  // TODO(deymo): Determine if this method should be virtual in HttpFetcher so
  // this call is sent to the base_fetcher_.
  virtual void SetProxies(const std::deque<std::string>& proxies) {
    base_fetcher_->SetProxies(proxies);
  }

  inline size_t GetBytesDownloaded() override {
    return base_fetcher_->GetBytesDownloaded();
  }

  void set_low_speed_limit(int low_speed_bps, int low_speed_sec) override {
    base_fetcher_->set_low_speed_limit(low_speed_bps, low_speed_sec);
  }

  void set_connect_timeout(int connect_timeout_seconds) override {
    base_fetcher_->set_connect_timeout(connect_timeout_seconds);
  }

  void set_max_retry_count(int max_retry_count) override {
    base_fetcher_->set_max_retry_count(max_retry_count);
  }

 private:
  // A range object defining the offset and length of a download chunk.  Zero
  // length indicates an unspecified end offset (note that it is impossible to
  // request a zero-length range in HTTP).
  class Range {
   public:
    Range(off_t offset, size_t length) : offset_(offset), length_(length) {}
    explicit Range(off_t offset) : offset_(offset), length_(0) {}

    inline off_t offset() const { return offset_; }
    inline size_t length() const { return length_; }

    inline bool HasLength() const { return (length_ > 0); }

    std::string ToString() const;

   private:
    off_t offset_;
    size_t length_;
  };

  typedef std::vector<Range> RangesVect;

  // State change: Stopped or Downloading -> Downloading
  void StartTransfer();

  // HttpFetcherDelegate overrides.
  // State change: Downloading -> Downloading or Pending transfer ended
  bool ReceivedBytes(HttpFetcher* fetcher,
                     const void* bytes,
                     size_t length) override;

  // State change: Pending transfer ended -> Stopped
  void TransferEnded(HttpFetcher* fetcher, bool successful);
  // These two call TransferEnded():
  void TransferComplete(HttpFetcher* fetcher, bool successful) override;
  void TransferTerminated(HttpFetcher* fetcher) override;

  void Reset();

  std::unique_ptr<HttpFetcher> base_fetcher_;

  // If true, do not send any more data or TransferComplete to the delegate.
  bool base_fetcher_active_;

  // If true, the next fetcher needs to be started when TransferTerminated is
  // received from the current fetcher.
  bool pending_transfer_ended_;

  // True if we are waiting for base fetcher to terminate b/c we are
  // ourselves terminating.
  bool terminating_;

  RangesVect ranges_;

  RangesVect::size_type current_index_;  // index into ranges_
  size_t bytes_received_this_range_;

  DISALLOW_COPY_AND_ASSIGN(MultiRangeHttpFetcher);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_MULTI_RANGE_HTTP_FETCHER_H_
