/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CHRE_WIFI_OFFLOAD_CHANNEL_HISTOGRAM_H_
#define CHRE_WIFI_OFFLOAD_CHANNEL_HISTOGRAM_H_

#include "chre/apps/wifi_offload/wifi_offload.h"

#include "chre/apps/wifi_offload/flatbuffers_types_generated.h"

namespace wifi_offload {

/**
 * A class to keep scan count for each channel. It provides an indexer to access
 * each channel's scan count with channel number.
 */
class ChannelHistogram {
 public:
  static constexpr uint8_t kNumChannels = 69;

  ChannelHistogram();

  ~ChannelHistogram() = default;

  static bool IsSupportedFrequency(uint32_t frequency);

  /* Gets the scaled scan count for a given channel number. All scan counts are
   * scaled such that the largest non-zero count is always mapped to 255 */
  uint8_t GetChannelScanCount(uint8_t channel_number) const;

  bool IncrementScanCountForFrequency(uint32_t frequency);

  bool IncrementScanCountForFrequencyForTest(uint32_t frequency,
                                             uint32_t increase_count);

  bool IncrementScanCountForChannelForTest(uint8_t channel,
                                           uint32_t increase_count);

  bool operator==(const ChannelHistogram &other) const;

  flatbuffers::Offset<flatbuffers::Vector<uint8_t>> Serialize(
      flatbuffers::FlatBufferBuilder *builder) const;

  bool Deserialize(const flatbuffers::Vector<uint8_t> &fbs_scan_count);

 private:
  uint32_t scan_count_internal_high_res_[kNumChannels];
};

}  // namespace wifi_offload

#endif  // CHRE_WIFI_OFFLOAD_CHANNEL_HISTOGRAM_H_
