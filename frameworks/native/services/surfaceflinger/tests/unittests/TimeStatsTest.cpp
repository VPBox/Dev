/*
 * Copyright 2018 The Android Open Source Project
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

#undef LOG_TAG
#define LOG_TAG "LibSurfaceFlingerUnittests"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <log/log.h>
#include <utils/String16.h>
#include <utils/Vector.h>

#include <random>
#include <unordered_set>

#include "TimeStats/TimeStats.h"

#include "libsurfaceflinger_unittest_main.h"

using namespace android::surfaceflinger;
using namespace google::protobuf;

namespace android {
namespace {

using testing::Contains;
using testing::SizeIs;
using testing::UnorderedElementsAre;

// clang-format off
#define FMT_PROTO          true
#define FMT_STRING         false
#define LAYER_ID_0         0
#define LAYER_ID_1         1
#define LAYER_ID_INVALID   -1
#define NUM_LAYERS         1
#define NUM_LAYERS_INVALID "INVALID"

enum InputCommand : int32_t {
    ENABLE                 = 0,
    DISABLE                = 1,
    CLEAR                  = 2,
    DUMP_ALL               = 3,
    DUMP_MAXLAYERS_1       = 4,
    DUMP_MAXLAYERS_INVALID = 5,
    INPUT_COMMAND_BEGIN    = ENABLE,
    INPUT_COMMAND_END      = DUMP_MAXLAYERS_INVALID,
    INPUT_COMMAND_RANGE    = INPUT_COMMAND_END - INPUT_COMMAND_BEGIN + 1,
};

enum TimeStamp : int32_t {
    POST                   = 0,
    ACQUIRE                = 1,
    ACQUIRE_FENCE          = 2,
    LATCH                  = 3,
    DESIRED                = 4,
    PRESENT                = 5,
    PRESENT_FENCE          = 6,
    TIME_STAMP_BEGIN       = POST,
    TIME_STAMP_END         = PRESENT,
    TIME_STAMP_RANGE       = TIME_STAMP_END - TIME_STAMP_BEGIN + 1,
};

static const TimeStamp NORMAL_SEQUENCE[] = {
        TimeStamp::POST,
        TimeStamp::ACQUIRE,
        TimeStamp::LATCH,
        TimeStamp::DESIRED,
        TimeStamp::PRESENT,
};

static const TimeStamp NORMAL_SEQUENCE_2[] = {
        TimeStamp::POST,
        TimeStamp::ACQUIRE_FENCE,
        TimeStamp::LATCH,
        TimeStamp::DESIRED,
        TimeStamp::PRESENT_FENCE,
};

static const TimeStamp UNORDERED_SEQUENCE[] = {
        TimeStamp::ACQUIRE,
        TimeStamp::LATCH,
        TimeStamp::POST,
        TimeStamp::DESIRED,
        TimeStamp::PRESENT,
};

static const TimeStamp INCOMPLETE_SEQUENCE[] = {
        TimeStamp::POST,
};
// clang-format on

class TimeStatsTest : public testing::Test {
public:
    TimeStatsTest() {
        const ::testing::TestInfo* const test_info =
                ::testing::UnitTest::GetInstance()->current_test_info();
        ALOGD("**** Setting up for %s.%s\n", test_info->test_case_name(), test_info->name());
    }

    ~TimeStatsTest() {
        const ::testing::TestInfo* const test_info =
                ::testing::UnitTest::GetInstance()->current_test_info();
        ALOGD("**** Tearing down after %s.%s\n", test_info->test_case_name(), test_info->name());
    }

    std::string inputCommand(InputCommand cmd, bool useProto);

    void setTimeStamp(TimeStamp type, int32_t id, uint64_t frameNumber, nsecs_t ts);

    int32_t genRandomInt32(int32_t begin, int32_t end);

    template <size_t N>
    void insertTimeRecord(const TimeStamp (&sequence)[N], int32_t id, uint64_t frameNumber,
                          nsecs_t ts) {
        for (size_t i = 0; i < N; i++, ts += 1000000) {
            setTimeStamp(sequence[i], id, frameNumber, ts);
        }
    }

    std::mt19937 mRandomEngine = std::mt19937(std::random_device()());
    std::unique_ptr<TimeStats> mTimeStats = std::make_unique<impl::TimeStats>();
};

std::string TimeStatsTest::inputCommand(InputCommand cmd, bool useProto) {
    std::string result;
    Vector<String16> args;

    switch (cmd) {
        case InputCommand::ENABLE:
            args.push_back(String16("-enable"));
            break;
        case InputCommand::DISABLE:
            args.push_back(String16("-disable"));
            break;
        case InputCommand::CLEAR:
            args.push_back(String16("-clear"));
            break;
        case InputCommand::DUMP_ALL:
            args.push_back(String16("-dump"));
            break;
        case InputCommand::DUMP_MAXLAYERS_1:
            args.push_back(String16("-dump"));
            args.push_back(String16("-maxlayers"));
            args.push_back(String16(std::to_string(NUM_LAYERS).c_str()));
            break;
        case InputCommand::DUMP_MAXLAYERS_INVALID:
            args.push_back(String16("-dump"));
            args.push_back(String16("-maxlayers"));
            args.push_back(String16(NUM_LAYERS_INVALID));
            break;
        default:
            ALOGD("Invalid control command");
    }

    EXPECT_NO_FATAL_FAILURE(mTimeStats->parseArgs(useProto, args, result));
    return result;
}

static std::string genLayerName(int32_t layerID) {
    return (layerID < 0 ? "invalid.dummy" : "com.dummy#") + std::to_string(layerID);
}

void TimeStatsTest::setTimeStamp(TimeStamp type, int32_t id, uint64_t frameNumber, nsecs_t ts) {
    switch (type) {
        case TimeStamp::POST:
            ASSERT_NO_FATAL_FAILURE(mTimeStats->setPostTime(id, frameNumber, genLayerName(id), ts));
            break;
        case TimeStamp::ACQUIRE:
            ASSERT_NO_FATAL_FAILURE(mTimeStats->setAcquireTime(id, frameNumber, ts));
            break;
        case TimeStamp::ACQUIRE_FENCE:
            ASSERT_NO_FATAL_FAILURE(
                    mTimeStats->setAcquireFence(id, frameNumber, std::make_shared<FenceTime>(ts)));
            break;
        case TimeStamp::LATCH:
            ASSERT_NO_FATAL_FAILURE(mTimeStats->setLatchTime(id, frameNumber, ts));
            break;
        case TimeStamp::DESIRED:
            ASSERT_NO_FATAL_FAILURE(mTimeStats->setDesiredTime(id, frameNumber, ts));
            break;
        case TimeStamp::PRESENT:
            ASSERT_NO_FATAL_FAILURE(mTimeStats->setPresentTime(id, frameNumber, ts));
            break;
        case TimeStamp::PRESENT_FENCE:
            ASSERT_NO_FATAL_FAILURE(
                    mTimeStats->setPresentFence(id, frameNumber, std::make_shared<FenceTime>(ts)));
            break;
        default:
            ALOGD("Invalid timestamp type");
    }
}

int32_t TimeStatsTest::genRandomInt32(int32_t begin, int32_t end) {
    std::uniform_int_distribution<int32_t> distr(begin, end);
    return distr(mRandomEngine);
}

TEST_F(TimeStatsTest, canEnableAndDisableTimeStats) {
    EXPECT_TRUE(inputCommand(InputCommand::ENABLE, FMT_STRING).empty());
    ASSERT_TRUE(mTimeStats->isEnabled());

    EXPECT_TRUE(inputCommand(InputCommand::DISABLE, FMT_STRING).empty());
    ASSERT_FALSE(mTimeStats->isEnabled());
}

TEST_F(TimeStatsTest, canIncreaseGlobalStats) {
    constexpr size_t TOTAL_FRAMES = 5;
    constexpr size_t MISSED_FRAMES = 4;
    constexpr size_t CLIENT_COMPOSITION_FRAMES = 3;

    EXPECT_TRUE(inputCommand(InputCommand::ENABLE, FMT_STRING).empty());

    for (size_t i = 0; i < TOTAL_FRAMES; i++) {
        ASSERT_NO_FATAL_FAILURE(mTimeStats->incrementTotalFrames());
    }
    for (size_t i = 0; i < MISSED_FRAMES; i++) {
        ASSERT_NO_FATAL_FAILURE(mTimeStats->incrementMissedFrames());
    }
    for (size_t i = 0; i < CLIENT_COMPOSITION_FRAMES; i++) {
        ASSERT_NO_FATAL_FAILURE(mTimeStats->incrementClientCompositionFrames());
    }

    SFTimeStatsGlobalProto globalProto;
    ASSERT_TRUE(globalProto.ParseFromString(inputCommand(InputCommand::DUMP_ALL, FMT_PROTO)));

    ASSERT_TRUE(globalProto.has_total_frames());
    EXPECT_EQ(TOTAL_FRAMES, globalProto.total_frames());
    ASSERT_TRUE(globalProto.has_missed_frames());
    EXPECT_EQ(MISSED_FRAMES, globalProto.missed_frames());
    ASSERT_TRUE(globalProto.has_client_composition_frames());
    EXPECT_EQ(CLIENT_COMPOSITION_FRAMES, globalProto.client_composition_frames());
}

TEST_F(TimeStatsTest, canInsertGlobalPresentToPresent) {
    EXPECT_TRUE(inputCommand(InputCommand::ENABLE, FMT_STRING).empty());

    ASSERT_NO_FATAL_FAILURE(
            mTimeStats->setPresentFenceGlobal(std::make_shared<FenceTime>(1000000)));
    ASSERT_NO_FATAL_FAILURE(
            mTimeStats->setPresentFenceGlobal(std::make_shared<FenceTime>(2000000)));

    ASSERT_NO_FATAL_FAILURE(mTimeStats->setPowerMode(HWC_POWER_MODE_NORMAL));
    ASSERT_NO_FATAL_FAILURE(
            mTimeStats->setPresentFenceGlobal(std::make_shared<FenceTime>(3000000)));
    ASSERT_NO_FATAL_FAILURE(
            mTimeStats->setPresentFenceGlobal(std::make_shared<FenceTime>(5000000)));

    ASSERT_NO_FATAL_FAILURE(mTimeStats->setPowerMode(HWC_POWER_MODE_OFF));
    ASSERT_NO_FATAL_FAILURE(
            mTimeStats->setPresentFenceGlobal(std::make_shared<FenceTime>(6000000)));
    ASSERT_NO_FATAL_FAILURE(
            mTimeStats->setPresentFenceGlobal(std::make_shared<FenceTime>(8000000)));

    SFTimeStatsGlobalProto globalProto;
    ASSERT_TRUE(globalProto.ParseFromString(inputCommand(InputCommand::DUMP_ALL, FMT_PROTO)));

    ASSERT_EQ(1, globalProto.present_to_present_size());
    const SFTimeStatsHistogramBucketProto& histogramProto = globalProto.present_to_present().Get(0);
    EXPECT_EQ(1, histogramProto.frame_count());
    EXPECT_EQ(2, histogramProto.time_millis());
}

TEST_F(TimeStatsTest, canInsertOneLayerTimeStats) {
    EXPECT_TRUE(inputCommand(InputCommand::ENABLE, FMT_STRING).empty());

    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_0, 1, 1000000);
    insertTimeRecord(NORMAL_SEQUENCE_2, LAYER_ID_0, 2, 2000000);

    SFTimeStatsGlobalProto globalProto;
    ASSERT_TRUE(globalProto.ParseFromString(inputCommand(InputCommand::DUMP_ALL, FMT_PROTO)));

    ASSERT_EQ(1, globalProto.stats_size());
    const SFTimeStatsLayerProto& layerProto = globalProto.stats().Get(0);
    ASSERT_TRUE(layerProto.has_layer_name());
    EXPECT_EQ(genLayerName(LAYER_ID_0), layerProto.layer_name());
    ASSERT_TRUE(layerProto.has_total_frames());
    EXPECT_EQ(1, layerProto.total_frames());
    ASSERT_EQ(6, layerProto.deltas_size());
    for (const SFTimeStatsDeltaProto& deltaProto : layerProto.deltas()) {
        ASSERT_EQ(1, deltaProto.histograms_size());
        const SFTimeStatsHistogramBucketProto& histogramProto = deltaProto.histograms().Get(0);
        EXPECT_EQ(1, histogramProto.frame_count());
        if ("post2acquire" == deltaProto.delta_name()) {
            EXPECT_EQ(1, histogramProto.time_millis());
        } else if ("post2present" == deltaProto.delta_name()) {
            EXPECT_EQ(4, histogramProto.time_millis());
        } else if ("acquire2present" == deltaProto.delta_name()) {
            EXPECT_EQ(3, histogramProto.time_millis());
        } else if ("latch2present" == deltaProto.delta_name()) {
            EXPECT_EQ(2, histogramProto.time_millis());
        } else if ("desired2present" == deltaProto.delta_name()) {
            EXPECT_EQ(1, histogramProto.time_millis());
        } else if ("present2present" == deltaProto.delta_name()) {
            EXPECT_EQ(1, histogramProto.time_millis());
        } else {
            FAIL() << "Unknown delta_name: " << deltaProto.delta_name();
        }
    }
}

TEST_F(TimeStatsTest, canNotInsertInvalidLayerNameTimeStats) {
    EXPECT_TRUE(inputCommand(InputCommand::ENABLE, FMT_STRING).empty());

    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_INVALID, 1, 1000000);
    insertTimeRecord(NORMAL_SEQUENCE_2, LAYER_ID_INVALID, 2, 2000000);

    SFTimeStatsGlobalProto globalProto;
    ASSERT_TRUE(globalProto.ParseFromString(inputCommand(InputCommand::DUMP_ALL, FMT_PROTO)));

    ASSERT_EQ(0, globalProto.stats_size());
}

TEST_F(TimeStatsTest, canInsertMultipleLayersTimeStats) {
    EXPECT_TRUE(inputCommand(InputCommand::ENABLE, FMT_STRING).empty());

    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_0, 1, 1000000);
    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_1, 1, 1000000);
    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_0, 2, 2000000);
    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_1, 2, 2000000);

    SFTimeStatsGlobalProto globalProto;
    ASSERT_TRUE(globalProto.ParseFromString(inputCommand(InputCommand::DUMP_ALL, FMT_PROTO)));

    EXPECT_EQ(2, globalProto.stats_size());
}

TEST_F(TimeStatsTest, canInsertUnorderedLayerTimeStats) {
    EXPECT_TRUE(inputCommand(InputCommand::ENABLE, FMT_STRING).empty());

    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_0, 1, 1000000);
    insertTimeRecord(UNORDERED_SEQUENCE, LAYER_ID_0, 2, 2000000);

    SFTimeStatsGlobalProto globalProto;
    ASSERT_TRUE(globalProto.ParseFromString(inputCommand(InputCommand::DUMP_ALL, FMT_PROTO)));

    ASSERT_EQ(1, globalProto.stats_size());
    const SFTimeStatsLayerProto& layerProto = globalProto.stats().Get(0);
    ASSERT_TRUE(layerProto.has_layer_name());
    EXPECT_EQ(genLayerName(LAYER_ID_0), layerProto.layer_name());
    ASSERT_TRUE(layerProto.has_total_frames());
    EXPECT_EQ(1, layerProto.total_frames());
    ASSERT_EQ(6, layerProto.deltas_size());
    for (const SFTimeStatsDeltaProto& deltaProto : layerProto.deltas()) {
        ASSERT_EQ(1, deltaProto.histograms_size());
        const SFTimeStatsHistogramBucketProto& histogramProto = deltaProto.histograms().Get(0);
        EXPECT_EQ(1, histogramProto.frame_count());
        if ("post2acquire" == deltaProto.delta_name()) {
            EXPECT_EQ(0, histogramProto.time_millis());
        } else if ("post2present" == deltaProto.delta_name()) {
            EXPECT_EQ(2, histogramProto.time_millis());
        } else if ("acquire2present" == deltaProto.delta_name()) {
            EXPECT_EQ(2, histogramProto.time_millis());
        } else if ("latch2present" == deltaProto.delta_name()) {
            EXPECT_EQ(2, histogramProto.time_millis());
        } else if ("desired2present" == deltaProto.delta_name()) {
            EXPECT_EQ(1, histogramProto.time_millis());
        } else if ("present2present" == deltaProto.delta_name()) {
            EXPECT_EQ(1, histogramProto.time_millis());
        } else {
            FAIL() << "Unknown delta_name: " << deltaProto.delta_name();
        }
    }
}

TEST_F(TimeStatsTest, recordRefreshRateNewConfigs) {
    EXPECT_TRUE(inputCommand(InputCommand::ENABLE, FMT_STRING).empty());

    uint32_t fpsOne = 30;
    uint32_t fpsTwo = 90;
    uint64_t millisOne = 5000;
    uint64_t millisTwo = 7000;

    mTimeStats->recordRefreshRate(fpsOne, ms2ns(millisOne));
    mTimeStats->recordRefreshRate(fpsTwo, ms2ns(millisTwo));

    SFTimeStatsGlobalProto globalProto;
    ASSERT_TRUE(globalProto.ParseFromString(inputCommand(InputCommand::DUMP_ALL, FMT_PROTO)));

    SFTimeStatsDisplayConfigBucketProto expectedBucketOne;
    SFTimeStatsDisplayConfigProto* expectedConfigOne = expectedBucketOne.mutable_config();
    expectedConfigOne->set_fps(fpsOne);
    expectedBucketOne.set_duration_millis(millisOne);

    SFTimeStatsDisplayConfigBucketProto expectedBucketTwo;
    SFTimeStatsDisplayConfigProto* expectedConfigTwo = expectedBucketTwo.mutable_config();
    expectedConfigTwo->set_fps(fpsTwo);
    expectedBucketTwo.set_duration_millis(millisTwo);

    EXPECT_THAT(globalProto.display_config_stats(), SizeIs(2));

    std::unordered_set<uint32_t> seen_fps;
    for (const auto& bucket : globalProto.display_config_stats()) {
        seen_fps.emplace(bucket.config().fps());
        if (fpsOne == bucket.config().fps()) {
            EXPECT_EQ(millisOne, bucket.duration_millis());
        } else if (fpsTwo == bucket.config().fps()) {
            EXPECT_EQ(millisTwo, bucket.duration_millis());
        } else {
            FAIL() << "Unknown fps: " << bucket.config().fps();
        }
    }
    EXPECT_THAT(seen_fps, UnorderedElementsAre(fpsOne, fpsTwo));
}

TEST_F(TimeStatsTest, recordRefreshRateUpdatesConfig) {
    EXPECT_TRUE(inputCommand(InputCommand::ENABLE, FMT_STRING).empty());

    uint32_t fps = 30;
    uint64_t millisOne = 5000;
    uint64_t millisTwo = 7000;

    mTimeStats->recordRefreshRate(fps, ms2ns(millisOne));
    mTimeStats->recordRefreshRate(fps, ms2ns(millisTwo));

    SFTimeStatsGlobalProto globalProto;
    ASSERT_TRUE(globalProto.ParseFromString(inputCommand(InputCommand::DUMP_ALL, FMT_PROTO)));
    EXPECT_THAT(globalProto.display_config_stats(), SizeIs(1));
    EXPECT_EQ(fps, globalProto.display_config_stats().Get(0).config().fps());
    EXPECT_EQ(millisOne + millisTwo, globalProto.display_config_stats().Get(0).duration_millis());
}

TEST_F(TimeStatsTest, canRemoveTimeRecord) {
    EXPECT_TRUE(inputCommand(InputCommand::ENABLE, FMT_STRING).empty());

    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_0, 1, 1000000);
    insertTimeRecord(INCOMPLETE_SEQUENCE, LAYER_ID_0, 2, 2000000);
    ASSERT_NO_FATAL_FAILURE(mTimeStats->removeTimeRecord(0, 2));
    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_0, 3, 3000000);

    SFTimeStatsGlobalProto globalProto;
    ASSERT_TRUE(globalProto.ParseFromString(inputCommand(InputCommand::DUMP_ALL, FMT_PROTO)));

    ASSERT_EQ(1, globalProto.stats_size());
    const SFTimeStatsLayerProto& layerProto = globalProto.stats().Get(0);
    ASSERT_TRUE(layerProto.has_total_frames());
    EXPECT_EQ(1, layerProto.total_frames());
}

TEST_F(TimeStatsTest, canRecoverFromIncompleteTimeRecordError) {
    EXPECT_TRUE(inputCommand(InputCommand::ENABLE, FMT_STRING).empty());

    uint64_t frameNumber = 1;
    nsecs_t ts = 1000000;
    insertTimeRecord(INCOMPLETE_SEQUENCE, LAYER_ID_0, 1, 1000000);
    for (size_t i = 0; i < impl::TimeStats::MAX_NUM_TIME_RECORDS + 2; i++) {
        frameNumber++;
        ts += 1000000;
        insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_0, frameNumber, ts);
    }

    SFTimeStatsGlobalProto globalProto;
    ASSERT_TRUE(globalProto.ParseFromString(inputCommand(InputCommand::DUMP_ALL, FMT_PROTO)));

    ASSERT_EQ(1, globalProto.stats_size());
    const SFTimeStatsLayerProto& layerProto = globalProto.stats().Get(0);
    ASSERT_TRUE(layerProto.has_total_frames());
    EXPECT_EQ(1, layerProto.total_frames());
}

TEST_F(TimeStatsTest, layerTimeStatsOnDestroy) {
    EXPECT_TRUE(inputCommand(InputCommand::ENABLE, FMT_STRING).empty());

    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_0, 1, 1000000);
    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_0, 2, 2000000);
    ASSERT_NO_FATAL_FAILURE(mTimeStats->onDestroy(0));
    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_0, 3, 3000000);

    SFTimeStatsGlobalProto globalProto;
    ASSERT_TRUE(globalProto.ParseFromString(inputCommand(InputCommand::DUMP_ALL, FMT_PROTO)));

    ASSERT_EQ(1, globalProto.stats_size());
    const SFTimeStatsLayerProto& layerProto = globalProto.stats().Get(0);
    ASSERT_TRUE(layerProto.has_total_frames());
    EXPECT_EQ(1, layerProto.total_frames());
}

TEST_F(TimeStatsTest, canClearTimeStats) {
    EXPECT_TRUE(inputCommand(InputCommand::ENABLE, FMT_STRING).empty());

    ASSERT_NO_FATAL_FAILURE(mTimeStats->incrementTotalFrames());
    ASSERT_NO_FATAL_FAILURE(mTimeStats->incrementMissedFrames());
    ASSERT_NO_FATAL_FAILURE(mTimeStats->incrementClientCompositionFrames());
    ASSERT_NO_FATAL_FAILURE(mTimeStats->setPowerMode(HWC_POWER_MODE_NORMAL));
    ASSERT_NO_FATAL_FAILURE(
            mTimeStats->setPresentFenceGlobal(std::make_shared<FenceTime>(1000000)));
    ASSERT_NO_FATAL_FAILURE(
            mTimeStats->setPresentFenceGlobal(std::make_shared<FenceTime>(2000000)));
    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_0, 1, 1000000);
    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_0, 2, 2000000);

    EXPECT_TRUE(inputCommand(InputCommand::CLEAR, FMT_STRING).empty());

    SFTimeStatsGlobalProto globalProto;
    ASSERT_TRUE(globalProto.ParseFromString(inputCommand(InputCommand::DUMP_ALL, FMT_PROTO)));

    EXPECT_EQ(0, globalProto.total_frames());
    EXPECT_EQ(0, globalProto.missed_frames());
    EXPECT_EQ(0, globalProto.client_composition_frames());
    EXPECT_EQ(0, globalProto.present_to_present_size());
    EXPECT_EQ(0, globalProto.stats_size());
}

TEST_F(TimeStatsTest, canDumpWithMaxLayers) {
    EXPECT_TRUE(inputCommand(InputCommand::ENABLE, FMT_STRING).empty());

    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_0, 1, 1000000);
    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_1, 1, 1000000);
    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_0, 2, 2000000);
    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_1, 2, 2000000);
    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_1, 3, 2000000);

    SFTimeStatsGlobalProto globalProto;
    ASSERT_TRUE(
            globalProto.ParseFromString(inputCommand(InputCommand::DUMP_MAXLAYERS_1, FMT_PROTO)));

    ASSERT_EQ(1, globalProto.stats_size());
    const SFTimeStatsLayerProto& layerProto = globalProto.stats().Get(0);
    ASSERT_TRUE(layerProto.has_layer_name());
    EXPECT_EQ(genLayerName(LAYER_ID_1), layerProto.layer_name());
    ASSERT_TRUE(layerProto.has_total_frames());
    EXPECT_EQ(2, layerProto.total_frames());
}

TEST_F(TimeStatsTest, canDumpWithInvalidMaxLayers) {
    EXPECT_TRUE(inputCommand(InputCommand::ENABLE, FMT_STRING).empty());

    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_0, 1, 1000000);
    insertTimeRecord(NORMAL_SEQUENCE, LAYER_ID_0, 2, 2000000);

    SFTimeStatsGlobalProto globalProto;
    ASSERT_TRUE(globalProto.ParseFromString(
            inputCommand(InputCommand::DUMP_MAXLAYERS_INVALID, FMT_PROTO)));

    ASSERT_EQ(0, globalProto.stats_size());
}

TEST_F(TimeStatsTest, canSurviveMonkey) {
    if (g_noSlowTests) {
        GTEST_SKIP();
    }

    EXPECT_TRUE(inputCommand(InputCommand::ENABLE, FMT_STRING).empty());

    for (size_t i = 0; i < 10000000; ++i) {
        const int32_t layerID = genRandomInt32(-1, 10);
        const int32_t frameNumber = genRandomInt32(1, 10);
        switch (genRandomInt32(0, 100)) {
            case 0:
                ALOGV("removeTimeRecord");
                ASSERT_NO_FATAL_FAILURE(mTimeStats->removeTimeRecord(layerID, frameNumber));
                continue;
            case 1:
                ALOGV("onDestroy");
                ASSERT_NO_FATAL_FAILURE(mTimeStats->onDestroy(layerID));
                continue;
        }
        TimeStamp type = static_cast<TimeStamp>(genRandomInt32(TIME_STAMP_BEGIN, TIME_STAMP_END));
        const int32_t ts = genRandomInt32(1, 1000000000);
        ALOGV("type[%d], layerID[%d], frameNumber[%d], ts[%d]", type, layerID, frameNumber, ts);
        setTimeStamp(type, layerID, frameNumber, ts);
    }
}

} // namespace
} // namespace android
