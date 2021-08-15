/*
 * Copyright (C) 2018 The Android Open Source Project
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

// Unit tests for Isochronous Clock Model

#include <math.h>
#include <stdlib.h>


#include <aaudio/AAudio.h>
#include <audio_utils/clock.h>
#include <client/IsochronousClockModel.h>
#include <gtest/gtest.h>

using namespace aaudio;

// We can use arbitrary values here because we are not opening a real audio stream.
#define SAMPLE_RATE             48000
#define HW_FRAMES_PER_BURST     48
#define NANOS_PER_BURST         (NANOS_PER_SECOND * HW_FRAMES_PER_BURST / SAMPLE_RATE)

class ClockModelTestFixture: public ::testing::Test {
public:
    ClockModelTestFixture() {
    }

    void SetUp() {
        model.setSampleRate(SAMPLE_RATE);
        model.setFramesPerBurst(HW_FRAMES_PER_BURST);
    }

    void TearDown() {
    }

    ~ClockModelTestFixture()  {
        // cleanup any pending stuff, but no exceptions allowed
    }

    // Test processing of timestamps when the hardware may be slightly off from
    // the expected sample rate.
    void checkDriftingClock(double hardwareFramesPerSecond, int numLoops) {
        const int64_t startTimeNanos = 500000000; // arbitrary
        model.start(startTimeNanos);

        const int64_t startPositionFrames = HW_FRAMES_PER_BURST; // hardware
        // arbitrary time for first burst
        const int64_t markerTime = startTimeNanos + NANOS_PER_MILLISECOND
                + (200 * NANOS_PER_MICROSECOND);

        // Should set initial marker.
        model.processTimestamp(startPositionFrames, markerTime);
        ASSERT_EQ(startPositionFrames, model.convertTimeToPosition(markerTime));

        double elapsedTimeSeconds = startTimeNanos / (double) NANOS_PER_SECOND;
        for (int i = 0; i < numLoops; i++) {
            // Calculate random delay over several bursts.
            const double timeDelaySeconds = 10.0 * drand48() * NANOS_PER_BURST / NANOS_PER_SECOND;
            elapsedTimeSeconds += timeDelaySeconds;
            const int64_t elapsedTimeNanos = (int64_t)(elapsedTimeSeconds * NANOS_PER_SECOND);
            const int64_t currentTimeNanos = startTimeNanos + elapsedTimeNanos;
            // Simulate DSP running at the specified rate.
            const int64_t currentTimeFrames = startPositionFrames +
                                        (int64_t)(hardwareFramesPerSecond * elapsedTimeSeconds);
            const int64_t numBursts = currentTimeFrames / HW_FRAMES_PER_BURST;
            const int64_t alignedPosition = startPositionFrames + (numBursts * HW_FRAMES_PER_BURST);

            // Apply drifting timestamp.
            model.processTimestamp(alignedPosition, currentTimeNanos);

            ASSERT_EQ(alignedPosition, model.convertTimeToPosition(currentTimeNanos));
        }
    }

    IsochronousClockModel model;
};

// Check default setup.
TEST_F(ClockModelTestFixture, clock_setup) {
    ASSERT_EQ(SAMPLE_RATE, model.getSampleRate());
    ASSERT_EQ(HW_FRAMES_PER_BURST, model.getFramesPerBurst());
}

// Test delta calculations.
TEST_F(ClockModelTestFixture, clock_deltas) {
    int64_t position = model.convertDeltaTimeToPosition(NANOS_PER_SECOND);
    ASSERT_EQ(SAMPLE_RATE, position);

    // Deltas are not quantized.
    // Compare time to the equivalent position in frames.
    constexpr int64_t kNanosPerBurst = HW_FRAMES_PER_BURST * NANOS_PER_SECOND / SAMPLE_RATE;
    position = model.convertDeltaTimeToPosition(NANOS_PER_SECOND + (kNanosPerBurst / 2));
    ASSERT_EQ(SAMPLE_RATE + (HW_FRAMES_PER_BURST / 2), position);

    int64_t time = model.convertDeltaPositionToTime(SAMPLE_RATE);
    ASSERT_EQ(NANOS_PER_SECOND, time);

    // Compare position in frames to the equivalent time.
    time = model.convertDeltaPositionToTime(SAMPLE_RATE + (HW_FRAMES_PER_BURST / 2));
    ASSERT_EQ(NANOS_PER_SECOND + (kNanosPerBurst / 2), time);
}

// start() should force the internal markers
TEST_F(ClockModelTestFixture, clock_start) {
    const int64_t startTime = 100000;
    model.start(startTime);

    int64_t position = model.convertTimeToPosition(startTime);
    EXPECT_EQ(0, position);

    int64_t time = model.convertPositionToTime(position);
    EXPECT_EQ(startTime, time);

    time = startTime + (500 * NANOS_PER_MICROSECOND);
    position = model.convertTimeToPosition(time);
    EXPECT_EQ(0, position);
}

// timestamps moves the window if outside the bounds
TEST_F(ClockModelTestFixture, clock_timestamp) {
    const int64_t startTime = 100000000;
    model.start(startTime);

    const int64_t position = HW_FRAMES_PER_BURST; // hardware
    int64_t markerTime = startTime + NANOS_PER_MILLISECOND + (200 * NANOS_PER_MICROSECOND);

    // Should set marker.
    model.processTimestamp(position, markerTime);
    EXPECT_EQ(position, model.convertTimeToPosition(markerTime));

    // convertTimeToPosition rounds down
    EXPECT_EQ(position, model.convertTimeToPosition(markerTime + (73 * NANOS_PER_MICROSECOND)));

    // convertPositionToTime rounds up
    EXPECT_EQ(markerTime + NANOS_PER_BURST, model.convertPositionToTime(position + 17));
}

#define NUM_LOOPS_DRIFT   10000

// test nudging the window by using a drifting HW clock
TEST_F(ClockModelTestFixture, clock_no_drift) {
    checkDriftingClock(SAMPLE_RATE, NUM_LOOPS_DRIFT);
}

// These slow drift rates caused errors when I disabled the code that handles
// drifting in the clock model. So I think the test is valid.
// It is unlikely that real hardware would be off by more than this amount.
TEST_F(ClockModelTestFixture, clock_slow_drift) {
    checkDriftingClock(0.998 * SAMPLE_RATE, NUM_LOOPS_DRIFT);
}

TEST_F(ClockModelTestFixture, clock_fast_drift) {
    checkDriftingClock(1.002 * SAMPLE_RATE, NUM_LOOPS_DRIFT);
}