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

#include "chre/apps/wifi_offload/channel_histogram.h"
#include "chre/apps/wifi_offload/utility.h"

namespace wifi_offload {
namespace {

/* Strictly increasing sequence of supported channel numbers in
 * 2.4GHz (802.11b/g/n) and 5GHz (802.11a/h/j/n/ac) */
constexpr uint8_t kAllChannels[] = {
    1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,
    16,  34,  36,  38,  40,  42,  44,  46,  48,  50,  52,  54,  56,  58,
    60,  62,  64,  100, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120,
    122, 124, 126, 128, 132, 134, 136, 138, 140, 142, 144, 149, 151, 153,
    155, 157, 159, 161, 165, 183, 184, 185, 187, 188, 189, 192, 196,
};
static_assert(sizeof(kAllChannels) / sizeof(kAllChannels[0]) ==
                  ChannelHistogram::kNumChannels,
              "some elements unspecified");

/**
 * Returns the channel number of a given frequency based on 802.11.
 *
 * @param frequency Frequncy of the channel in MHz
 *
 * @return Channel number of the given frequency. Zero if unsupported
 *         frequency or channel number. Returned value will be in the range of
 *         [0, 255]
 */
uint8_t GetChannelNumber(uint32_t frequency) {
  int channel_number =
      utility::Ieee80211FrequencyToChannel(static_cast<int>(frequency));
  if (channel_number <= 0 || channel_number > 255) {
    LOGE("Unknown channel frequency %" PRIu32 " MHz.", frequency);
    channel_number = 0;
  }
  return static_cast<uint8_t>(channel_number);
}

/**
 * Returns the index of a given channel number in kAllChannels.
 *
 * @param channel_number Channel number we want to map to an index
 *
 * @return Index of the given channel number in kAllChannels. kNumChannels if
 *         not found. Returned value will be in the range of [0, kNumChannels]
 */
size_t GetChannelIndex(uint8_t channel_number) {
  for (size_t i = 0; i < ChannelHistogram::kNumChannels; i++) {
    if (channel_number == kAllChannels[i]) {
      return i;
    }
  }

  LOGE("Unsupported channel number: %" PRIu8, channel_number);
  return ChannelHistogram::kNumChannels;
}

}  // namespace

ChannelHistogram::ChannelHistogram() {
  std::memset(scan_count_internal_high_res_, 0,
              sizeof(scan_count_internal_high_res_));
}

bool ChannelHistogram::IsSupportedFrequency(uint32_t frequency) {
  return GetChannelNumber(frequency) != 0;
}

uint8_t ChannelHistogram::GetChannelScanCount(uint8_t channel_number) const {
  size_t index = GetChannelIndex(channel_number);
  if (index == kNumChannels) {
    return 0;
  }

  if (scan_count_internal_high_res_[index] == 0) {
    return 0;
  }

  uint32_t max_count = 0;
  // since there is at least one non-zero value, max_count won't be 0
  for (const auto count : scan_count_internal_high_res_) {
    if (max_count < count) {
      max_count = count;
    }
  }

  // linearly map from [1,max_count] to [1,255]
  uint64_t scaled_value = scan_count_internal_high_res_[index];
  scaled_value = scaled_value * 254 / max_count + 1;
  return static_cast<uint8_t>(scaled_value);
}

bool ChannelHistogram::IncrementScanCountForFrequency(uint32_t frequency) {
  size_t index = GetChannelIndex(GetChannelNumber(frequency));
  if (index == kNumChannels) {
    return false;
  }

  scan_count_internal_high_res_[index]++;
  return true;
}

bool ChannelHistogram::IncrementScanCountForFrequencyForTest(
    uint32_t frequency, uint32_t increase_count) {
  return IncrementScanCountForChannelForTest(GetChannelNumber(frequency),
                                             increase_count);
}

bool ChannelHistogram::IncrementScanCountForChannelForTest(
    uint8_t channel, uint32_t increase_count) {
  size_t index = GetChannelIndex(channel);
  if (index == kNumChannels) {
    return false;
  }

  scan_count_internal_high_res_[index] += increase_count;
  return true;
}

bool ChannelHistogram::operator==(const ChannelHistogram &other) const {
  if (this == &other) {
    return true;
  }

  for (const auto channel : kAllChannels) {
    // Compare scaled values, rather than raw values
    if (GetChannelScanCount(channel) != other.GetChannelScanCount(channel)) {
      return false;
    }
  }
  return true;
}

flatbuffers::Offset<flatbuffers::Vector<uint8_t>> ChannelHistogram::Serialize(
    flatbuffers::FlatBufferBuilder *builder) const {
  uint8_t lowResScanCount[kNumChannels];
  for (size_t i = 0; i < kNumChannels; i++) {
    lowResScanCount[i] = GetChannelScanCount(kAllChannels[i]);
  }
  return builder->CreateVector(lowResScanCount, kNumChannels);
}

bool ChannelHistogram::Deserialize(
    const flatbuffers::Vector<uint8_t> &fbs_scan_count) {
  if (fbs_scan_count.size() != kNumChannels) {
    LOGE("Failed to deserialize ChannelHistogram. Null or incomplete members.");
    return false;
  }

  for (uint8_t i = 0; i < kNumChannels; i++) {
    scan_count_internal_high_res_[i] = fbs_scan_count.Get(i);
  }
  return true;
}

}  // namespace wifi_offload
