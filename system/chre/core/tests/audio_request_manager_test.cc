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

#include "chre/core/audio_request_manager.h"

using chre::AudioRequestManager;
using chre::Nanoseconds;

TEST(AudioDurationFromSampleCountAndRate, HalfSecond) {
  Nanoseconds duration =
      AudioRequestManager::getDurationFromSampleCountAndRate(8000, 16000);
  EXPECT_EQ(duration.toRawNanoseconds(), 500000000);
}

TEST(AudioDurationFromSampleCountAndRate, OneSecond) {
  Nanoseconds duration =
      AudioRequestManager::getDurationFromSampleCountAndRate(16000, 16000);
  EXPECT_EQ(duration.toRawNanoseconds(), 1000000000);
}

TEST(AudioDurationFromSampleCountAndRate, OneHundredSecond) {
  Nanoseconds duration =
      AudioRequestManager::getDurationFromSampleCountAndRate(1600000, 16000);
  EXPECT_EQ(duration.toRawNanoseconds(), 100000000000);
}

TEST(AudioSampleCountFromRateAndDuration, OneSample) {
  uint32_t sampleCount =
      AudioRequestManager::getSampleCountFromRateAndDuration(16000,
          Nanoseconds(62500));
  EXPECT_EQ(sampleCount, 1);
}

TEST(AudioSampleCountFromRateAndDuration, OneHundredSample) {
  uint32_t sampleCount =
      AudioRequestManager::getSampleCountFromRateAndDuration(16000,
          Nanoseconds(6250000));
  EXPECT_EQ(sampleCount, 100);
}

TEST(AudioSampleCountFromRateAndDuration, OneThousandSample) {
  uint32_t sampleCount =
      AudioRequestManager::getSampleCountFromRateAndDuration(16000,
          Nanoseconds(62500000));
  EXPECT_EQ(sampleCount, 1000);
}
