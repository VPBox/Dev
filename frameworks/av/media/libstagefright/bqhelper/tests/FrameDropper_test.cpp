/*
 * Copyright (C) 2015 The Android Open Source Project
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

//#define LOG_NDEBUG 0
#define LOG_TAG "FrameDropper_test"
#include <utils/Log.h>

#include <gtest/gtest.h>

#include <media/stagefright/bqhelper/FrameDropper.h>
#include <media/stagefright/foundation/ADebug.h>

namespace android {

struct TestFrame {
  int64_t timeUs;
  bool shouldDrop;
};

static const TestFrame testFrames20Fps[] = {
    {1000000, false}, {1050000, false}, {1100000, false}, {1150000, false},
    {1200000, false}, {1250000, false}, {1300000, false}, {1350000, false},
    {1400000, false}, {1450000, false}, {1500000, false}, {1550000, false},
    {1600000, false}, {1650000, false}, {1700000, false}, {1750000, false},
    {1800000, false}, {1850000, false}, {1900000, false}, {1950000, false},
};

static const TestFrame testFrames30Fps[] = {
    {1000000, false}, {1033333, false}, {1066667, false}, {1100000, false},
    {1133333, false}, {1166667, false}, {1200000, false}, {1233333, false},
    {1266667, false}, {1300000, false}, {1333333, false}, {1366667, false},
    {1400000, false}, {1433333, false}, {1466667, false}, {1500000, false},
    {1533333, false}, {1566667, false}, {1600000, false}, {1633333, false},
};

static const TestFrame testFrames40Fps[] = {
    {1000000, false}, {1025000, true}, {1050000, false}, {1075000, false},
    {1100000, false}, {1125000, true}, {1150000, false}, {1175000, false},
    {1200000, false}, {1225000, true}, {1250000, false}, {1275000, false},
    {1300000, false}, {1325000, true}, {1350000, false}, {1375000, false},
    {1400000, false}, {1425000, true}, {1450000, false}, {1475000, false},
};

static const TestFrame testFrames60Fps[] = {
    {1000000, false}, {1016667, true}, {1033333, false}, {1050000, true},
    {1066667, false}, {1083333, true}, {1100000, false}, {1116667, true},
    {1133333, false}, {1150000, true}, {1166667, false}, {1183333, true},
    {1200000, false}, {1216667, true}, {1233333, false}, {1250000, true},
    {1266667, false}, {1283333, true}, {1300000, false}, {1316667, true},
};

static const TestFrame testFramesVariableFps[] = {
    // 40fps
    {1000000, false}, {1025000, true}, {1050000, false}, {1075000, false},
    {1100000, false}, {1125000, true}, {1150000, false}, {1175000, false},
    {1200000, false}, {1225000, true}, {1250000, false}, {1275000, false},
    {1300000, false}, {1325000, true}, {1350000, false}, {1375000, false},
    {1400000, false}, {1425000, true}, {1450000, false}, {1475000, false},
    // a timestamp jump plus switch to 20fps
    {2000000, false}, {2050000, false}, {2100000, false}, {2150000, false},
    {2200000, false}, {2250000, false}, {2300000, false}, {2350000, false},
    {2400000, false}, {2450000, false}, {2500000, false}, {2550000, false},
    {2600000, false}, {2650000, false}, {2700000, false}, {2750000, false},
    {2800000, false}, {2850000, false}, {2900000, false}, {2950000, false},
    // 60fps
    {2966667, false}, {2983333, true}, {3000000, false}, {3016667, true},
    {3033333, false}, {3050000, true}, {3066667, false}, {3083333, true},
    {3100000, false}, {3116667, true}, {3133333, false}, {3150000, true},
    {3166667, false}, {3183333, true}, {3200000, false}, {3216667, true},
    {3233333, false}, {3250000, true}, {3266667, false}, {3283333, true},
};

static const int kMaxTestJitterUs = 2000;
// return one of 1000, 0, -1000 as jitter.
static int GetJitter(size_t i) {
    return (1 - (i % 3)) * (kMaxTestJitterUs / 2);
}

class FrameDropperTest : public ::testing::Test {
public:
    FrameDropperTest() : mFrameDropper(new FrameDropper()) {
        EXPECT_EQ(OK, mFrameDropper->setMaxFrameRate(30.0));
    }

protected:
    void RunTest(const TestFrame* frames, size_t size) {
        for (size_t i = 0; i < size; ++i) {
            int jitter = GetJitter(i);
            int64_t testTimeUs = frames[i].timeUs + jitter;
            printf("time %lld, testTime %lld, jitter %d\n",
                    (long long)frames[i].timeUs, (long long)testTimeUs, jitter);
            EXPECT_EQ(frames[i].shouldDrop, mFrameDropper->shouldDrop(testTimeUs));
        }
    }

    sp<FrameDropper> mFrameDropper;
};

TEST_F(FrameDropperTest, TestInvalidMaxFrameRate) {
    EXPECT_NE(OK, mFrameDropper->setMaxFrameRate(-1.0));
    EXPECT_NE(OK, mFrameDropper->setMaxFrameRate(0));
}

TEST_F(FrameDropperTest, Test20Fps) {
    RunTest(testFrames20Fps, ARRAY_SIZE(testFrames20Fps));
}

TEST_F(FrameDropperTest, Test30Fps) {
    RunTest(testFrames30Fps, ARRAY_SIZE(testFrames30Fps));
}

TEST_F(FrameDropperTest, Test40Fps) {
    RunTest(testFrames40Fps, ARRAY_SIZE(testFrames40Fps));
}

TEST_F(FrameDropperTest, Test60Fps) {
    RunTest(testFrames60Fps, ARRAY_SIZE(testFrames60Fps));
}

TEST_F(FrameDropperTest, TestVariableFps) {
    RunTest(testFramesVariableFps, ARRAY_SIZE(testFramesVariableFps));
}

} // namespace android
