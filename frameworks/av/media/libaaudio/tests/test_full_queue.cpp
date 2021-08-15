/*
 * Copyright (C) 2019 The Android Open Source Project
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

// Test whether a stream dies if it is written to after a delay.
// Maybe because the message queue from the AAudio service fills up.

#include <stdio.h>
#include <unistd.h>

#include <aaudio/AAudio.h>
#include <gtest/gtest.h>

constexpr int64_t kNanosPerSecond = 1000000000;
constexpr int64_t kTimeoutNanos = kNanosPerSecond / 2;
constexpr int kNumFrames = 256;
constexpr int kChannelCount = 2;

static void checkFullQueue(aaudio_performance_mode_t perfMode,
                           int32_t sleepMillis) {
    std::unique_ptr<float[]> buffer = std::make_unique<float[]>(
            kNumFrames * kChannelCount);

    AAudioStreamBuilder *aaudioBuilder = nullptr;

    // Use an AAudioStreamBuilder to contain requested parameters.
    ASSERT_EQ(AAUDIO_OK, AAudio_createStreamBuilder(&aaudioBuilder));

    AAudioStreamBuilder_setChannelCount(aaudioBuilder, kChannelCount);

    // Request stream properties.
    AAudioStreamBuilder_setPerformanceMode(aaudioBuilder, perfMode);

    // Create an AAudioStream using the Builder.
    AAudioStream *aaudioStream = nullptr;
    ASSERT_EQ(AAUDIO_OK, AAudioStreamBuilder_openStream(aaudioBuilder,
            &aaudioStream));
    AAudioStreamBuilder_delete(aaudioBuilder);

    EXPECT_EQ(AAUDIO_OK, AAudioStream_requestStart(aaudioStream));

    // Sleep for awhile. This might kill the stream.
    usleep(sleepMillis * 1000); // 1000 millis in a microsecond

    for (int i = 0; i < 10; i++) {
        const aaudio_result_t result = AAudioStream_write(aaudioStream,
                buffer.get(),
                kNumFrames,
                kTimeoutNanos);
        EXPECT_EQ(kNumFrames, result);
        if (kNumFrames != result) break;
    }

    EXPECT_EQ(AAUDIO_OK, AAudioStream_requestStop(aaudioStream));

    EXPECT_EQ(AAUDIO_OK, AAudioStream_close(aaudioStream));
}

TEST(test_full_queue, aaudio_full_queue_perf_none_50) {
    checkFullQueue(AAUDIO_PERFORMANCE_MODE_NONE, 50 /* sleepMillis */);
}

TEST(test_full_queue, aaudio_full_queue_perf_none_200) {
    checkFullQueue(AAUDIO_PERFORMANCE_MODE_NONE, 200 /* sleepMillis */);
}

TEST(test_full_queue, aaudio_full_queue_perf_none_1000) {
    checkFullQueue(AAUDIO_PERFORMANCE_MODE_NONE, 1000 /* sleepMillis */);
}

TEST(test_full_queue, aaudio_full_queue_low_latency_50) {
    checkFullQueue(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY, 50 /* sleepMillis */);
}

TEST(test_full_queue, aaudio_full_queue_low_latency_200) {
    checkFullQueue(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY, 200 /* sleepMillis */);
}

TEST(test_full_queue, aaudio_full_queue_low_latency_1000) {
    checkFullQueue(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY, 1000 /* sleepMillis */);
}
