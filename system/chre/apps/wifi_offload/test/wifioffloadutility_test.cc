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

#include "chre/apps/wifi_offload/utility.h"
#include "include/utility.h"

TEST(UtilityTest, MapAllSupportedFrequenciesToAllchannel_numbers) {
  for (size_t i = 0; i < wifi_offload_test::kNumFrequencies_Test; i++) {
    int channel = wifi_offload::utility::Ieee80211FrequencyToChannel(
        static_cast<int>(wifi_offload_test::kAllFrequencies_Test[i]));
    EXPECT_EQ(wifi_offload_test::kAllChannels_Test[i],
              static_cast<uint8_t>(channel));
  }
}
