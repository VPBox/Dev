/******************************************************************************
 *
 *  Copyright 2017 The Android Open Source Project
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#include <gtest/gtest.h>

#include "audio_a2dp_hw/include/audio_a2dp_hw.h"

namespace {
static uint32_t codec_sample_rate2value(
    btav_a2dp_codec_sample_rate_t codec_sample_rate) {
  switch (codec_sample_rate) {
    case BTAV_A2DP_CODEC_SAMPLE_RATE_44100:
      return 44100;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_48000:
      return 48000;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_88200:
      return 88200;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_96000:
      return 96000;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_176400:
      return 176400;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_192000:
      return 192000;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_16000:
      return 16000;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_24000:
      return 24000;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_NONE:
      break;
  }
  return 0;
}

static uint32_t codec_bits_per_sample2value(
    btav_a2dp_codec_bits_per_sample_t codec_bits_per_sample) {
  switch (codec_bits_per_sample) {
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16:
      return 16;
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_24:
      return 24;
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_32:
      return 32;
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE:
      break;
  }
  return 0;
}

static uint32_t codec_channel_mode2value(
    btav_a2dp_codec_channel_mode_t codec_channel_mode) {
  switch (codec_channel_mode) {
    case BTAV_A2DP_CODEC_CHANNEL_MODE_MONO:
      return 1;
    case BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO:
      return 2;
    case BTAV_A2DP_CODEC_CHANNEL_MODE_NONE:
      break;
  }
  return 0;
}

}  // namespace

class AudioA2dpHwTest : public ::testing::Test {
 protected:
  AudioA2dpHwTest() {}

 private:
};

TEST_F(AudioA2dpHwTest, test_compute_buffer_size) {
  const btav_a2dp_codec_sample_rate_t codec_sample_rate_array[] = {
      BTAV_A2DP_CODEC_SAMPLE_RATE_NONE,  BTAV_A2DP_CODEC_SAMPLE_RATE_44100,
      BTAV_A2DP_CODEC_SAMPLE_RATE_48000, BTAV_A2DP_CODEC_SAMPLE_RATE_88200,
      BTAV_A2DP_CODEC_SAMPLE_RATE_96000, BTAV_A2DP_CODEC_SAMPLE_RATE_176400,
      BTAV_A2DP_CODEC_SAMPLE_RATE_192000};

  const btav_a2dp_codec_bits_per_sample_t codec_bits_per_sample_array[] = {
      BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE, BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16,
      BTAV_A2DP_CODEC_BITS_PER_SAMPLE_24, BTAV_A2DP_CODEC_BITS_PER_SAMPLE_32};

  const btav_a2dp_codec_channel_mode_t codec_channel_mode_array[] = {
      BTAV_A2DP_CODEC_CHANNEL_MODE_NONE, BTAV_A2DP_CODEC_CHANNEL_MODE_MONO,
      BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO};

  for (const auto codec_sample_rate : codec_sample_rate_array) {
    for (const auto codec_bits_per_sample : codec_bits_per_sample_array) {
      for (const auto codec_channel_mode : codec_channel_mode_array) {
        size_t buffer_size = audio_a2dp_hw_stream_compute_buffer_size(
            codec_sample_rate, codec_bits_per_sample, codec_channel_mode);

        // Check for invalid input
        if ((codec_sample_rate == BTAV_A2DP_CODEC_SAMPLE_RATE_NONE) ||
            (codec_bits_per_sample == BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE) ||
            (codec_channel_mode == BTAV_A2DP_CODEC_CHANNEL_MODE_NONE)) {
          EXPECT_EQ(buffer_size,
                    static_cast<size_t>(AUDIO_STREAM_OUTPUT_BUFFER_SZ));
          continue;
        }

        uint32_t sample_rate = codec_sample_rate2value(codec_sample_rate);
        EXPECT_TRUE(sample_rate != 0);

        uint32_t bits_per_sample =
            codec_bits_per_sample2value(codec_bits_per_sample);
        EXPECT_TRUE(bits_per_sample != 0);

        uint32_t number_of_channels =
            codec_channel_mode2value(codec_channel_mode);
        EXPECT_TRUE(number_of_channels != 0);

        const uint64_t time_period_ms = 20;  // TODO: Must be a parameter
        size_t expected_buffer_size =
            (time_period_ms * AUDIO_STREAM_OUTPUT_BUFFER_PERIODS * sample_rate *
             number_of_channels * (bits_per_sample / 8)) /
            1000;

        // Compute the divisor and adjust the buffer size
        const size_t divisor = (AUDIO_STREAM_OUTPUT_BUFFER_PERIODS * 16 *
                                number_of_channels * bits_per_sample) /
                               8;
        const size_t remainder = expected_buffer_size % divisor;
        if (remainder != 0) {
          expected_buffer_size += divisor - remainder;
        }

        EXPECT_EQ(buffer_size, expected_buffer_size);
      }
    }
  }
}
