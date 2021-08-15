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

#include "gtest/gtest.h"

#include "chre/apps/wifi_offload/channel_histogram.h"
#include "include/random_generator.h"
#include "include/utility.h"

using wifi_offload_test::kNumFrequencies_Test;
using wifi_offload_test::kAllFrequencies_Test;
using wifi_offload_test::kAllChannels_Test;

/**
 * This file includes all the unit tests for ChannelHistogram class, except ==
 * operator and serialize/deserialize functions which have already been covered
 * in offloadtypes_test.cc.
 */

class ChannelHistogramTest : public testing::Test {
 public:
  wifi_offload_test::RandomGenerator random_gen_;
  wifi_offload::ChannelHistogram channel_histo_;
};

TEST_F(ChannelHistogramTest, UnsupportedFrequencyAndchannel_numbersAreHandled) {
  // some unsupported frequencies
  channel_histo_.IncrementScanCountForFrequency(2000);
  channel_histo_.IncrementScanCountForFrequency(42000);

  // verify no count have been increased
  for (size_t i = 0; i < kNumFrequencies_Test; i++) {
    ASSERT_EQ(0, channel_histo_.GetChannelScanCount(kAllChannels_Test[i]));
  }

  // some unsupported channel numbers
  EXPECT_EQ(0, channel_histo_.GetChannelScanCount(0));
  EXPECT_EQ(0, channel_histo_.GetChannelScanCount(15));
  EXPECT_EQ(0, channel_histo_.GetChannelScanCount(200));
}

TEST_F(ChannelHistogramTest, FrequenciesMapCorrectlyTochannel_numbers) {
  for (size_t i = 0; i < kNumFrequencies_Test; i++) {
    wifi_offload::ChannelHistogram histo;
    histo.IncrementScanCountForFrequency(kAllFrequencies_Test[i]);

    // verify only the increase channel is non-zero
    for (size_t j = 0; j < kNumFrequencies_Test; j++) {
      EXPECT_EQ(kAllChannels_Test[i] == kAllChannels_Test[j] ? 255 : 0,
                histo.GetChannelScanCount(kAllChannels_Test[j]));
    }
  }
}

TEST_F(ChannelHistogramTest, IncreaseFrequencyScanCountAndGetChannelScanCount) {
  uint32_t increase = 1;
  for (size_t i = 0; i < 12; i++) {
    increase *= 2;
    channel_histo_.IncrementScanCountForFrequencyForTest(
        kAllFrequencies_Test[i], increase);
  }

  uint32_t expected = 1;
  for (size_t i = 0; i < 12; i++) {
    uint8_t scaled_value =
        channel_histo_.GetChannelScanCount(kAllChannels_Test[i]);

    expected *= 2;
    EXPECT_EQ(expected * 254 / increase + 1, scaled_value);
  }

  for (size_t i = 12; i < 14; i++) {
    EXPECT_EQ(0, channel_histo_.GetChannelScanCount(kAllChannels_Test[i]));
  }
}

TEST_F(ChannelHistogramTest, SetScanCountToMaxUInt32) {
  // add some nice scan counts
  uint32_t increase = 1;
  for (size_t i = 0; i < 12; i++) {
    increase *= 2;
    channel_histo_.IncrementScanCountForFrequencyForTest(
        kAllFrequencies_Test[i], increase);
  }
  /* set the next scan count to be 2 ^ 32 - 1, this will force all other counts
   * to get mapped to 1 */
  channel_histo_.IncrementScanCountForFrequencyForTest(kAllFrequencies_Test[12],
                                                       0xffffffff);

  for (size_t i = 0; i < 12; i++) {
    EXPECT_EQ(1, channel_histo_.GetChannelScanCount(kAllChannels_Test[i]));
  }
  EXPECT_EQ(255, channel_histo_.GetChannelScanCount(kAllChannels_Test[12]));
}
