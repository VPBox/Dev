/*
 * Copyright 2019 The Android Open Source Project
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
#define LOG_TAG "SchedulerUnittests"

#include <gmock/gmock.h>
#include <log/log.h>
#include <thread>

#include "Scheduler/RefreshRateStats.h"
#include "mock/DisplayHardware/MockDisplay.h"
#include "mock/MockTimeStats.h"

using namespace std::chrono_literals;
using testing::_;
using testing::AtLeast;

namespace android {
namespace scheduler {

class RefreshRateStatsTest : public testing::Test {
protected:
    static constexpr int CONFIG_ID_90 = 0;
    static constexpr int CONFIG_ID_60 = 1;
    static constexpr int64_t VSYNC_90 = 11111111;
    static constexpr int64_t VSYNC_60 = 16666667;

    RefreshRateStatsTest();
    ~RefreshRateStatsTest();

    mock::TimeStats mTimeStats;
    RefreshRateConfigs mRefreshRateConfigs;
    RefreshRateStats mRefreshRateStats{mRefreshRateConfigs, mTimeStats};
};

RefreshRateStatsTest::RefreshRateStatsTest() {
    const ::testing::TestInfo* const test_info =
            ::testing::UnitTest::GetInstance()->current_test_info();
    ALOGD("**** Setting up for %s.%s\n", test_info->test_case_name(), test_info->name());
}

RefreshRateStatsTest::~RefreshRateStatsTest() {
    const ::testing::TestInfo* const test_info =
            ::testing::UnitTest::GetInstance()->current_test_info();
    ALOGD("**** Tearing down after %s.%s\n", test_info->test_case_name(), test_info->name());
}

namespace {
/* ------------------------------------------------------------------------
 * Test cases
 */
TEST_F(RefreshRateStatsTest, canCreateAndDestroyTest) {
    std::vector<std::shared_ptr<const HWC2::Display::Config>> configs;
    mRefreshRateConfigs.populate(configs);

    // There is one default config, so the refresh rates should have one item.
    EXPECT_EQ(1, mRefreshRateStats.getTotalTimes().size());
}

TEST_F(RefreshRateStatsTest, oneConfigTest) {
    auto display = new Hwc2::mock::Display();

    auto config = HWC2::Display::Config::Builder(*display, CONFIG_ID_90);
    config.setVsyncPeriod(VSYNC_90);
    std::vector<std::shared_ptr<const HWC2::Display::Config>> configs;
    configs.push_back(config.build());

    mRefreshRateConfigs.populate(configs);

    EXPECT_CALL(mTimeStats, recordRefreshRate(0, _)).Times(AtLeast(1));
    EXPECT_CALL(mTimeStats, recordRefreshRate(90, _)).Times(AtLeast(1));

    std::unordered_map<std::string, int64_t> times = mRefreshRateStats.getTotalTimes();
    EXPECT_EQ(2, times.size());
    EXPECT_NE(0u, times.count("ScreenOff"));
    EXPECT_EQ(1u, times.count("90fps"));
    EXPECT_EQ(0, times["90fps"]);
    // Setting up tests on mobile harness can be flaky with time passing, so testing for
    // exact time changes can result in flaxy numbers. To avoid that remember old
    // numbers to make sure the correct values are increasing in the next test.
    int screenOff = times["ScreenOff"];
    int ninety = times["90fps"];

    // Screen is off by default.
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    times = mRefreshRateStats.getTotalTimes();
    EXPECT_LT(screenOff, times["ScreenOff"]);
    EXPECT_EQ(0, times["90fps"]);

    mRefreshRateStats.setConfigMode(CONFIG_ID_90);
    mRefreshRateStats.setPowerMode(HWC_POWER_MODE_NORMAL);
    screenOff = mRefreshRateStats.getTotalTimes()["ScreenOff"];
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    times = mRefreshRateStats.getTotalTimes();
    EXPECT_EQ(screenOff, times["ScreenOff"]);
    EXPECT_LT(ninety, times["90fps"]);

    mRefreshRateStats.setPowerMode(HWC_POWER_MODE_DOZE);
    ninety = mRefreshRateStats.getTotalTimes()["90fps"];
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    times = mRefreshRateStats.getTotalTimes();
    EXPECT_LT(screenOff, times["ScreenOff"]);
    EXPECT_EQ(ninety, times["90fps"]);

    mRefreshRateStats.setConfigMode(CONFIG_ID_90);
    screenOff = mRefreshRateStats.getTotalTimes()["ScreenOff"];
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    times = mRefreshRateStats.getTotalTimes();
    // Because the power mode is not HWC_POWER_MODE_NORMAL, switching the config
    // does not update refresh rates that come from the config.
    EXPECT_LT(screenOff, times["ScreenOff"]);
    EXPECT_EQ(ninety, times["90fps"]);
}

TEST_F(RefreshRateStatsTest, twoConfigsTest) {
    auto display = new Hwc2::mock::Display();

    auto config90 = HWC2::Display::Config::Builder(*display, CONFIG_ID_90);
    config90.setVsyncPeriod(VSYNC_90);
    std::vector<std::shared_ptr<const HWC2::Display::Config>> configs;
    configs.push_back(config90.build());

    auto config60 = HWC2::Display::Config::Builder(*display, CONFIG_ID_60);
    config60.setVsyncPeriod(VSYNC_60);
    configs.push_back(config60.build());

    mRefreshRateConfigs.populate(configs);

    EXPECT_CALL(mTimeStats, recordRefreshRate(0, _)).Times(AtLeast(1));
    EXPECT_CALL(mTimeStats, recordRefreshRate(60, _)).Times(AtLeast(1));
    EXPECT_CALL(mTimeStats, recordRefreshRate(90, _)).Times(AtLeast(1));

    std::unordered_map<std::string, int64_t> times = mRefreshRateStats.getTotalTimes();
    EXPECT_EQ(3, times.size());
    EXPECT_NE(0u, times.count("ScreenOff"));
    EXPECT_EQ(1u, times.count("60fps"));
    EXPECT_EQ(0, times["60fps"]);
    EXPECT_EQ(1u, times.count("90fps"));
    EXPECT_EQ(0, times["90fps"]);
    // Setting up tests on mobile harness can be flaky with time passing, so testing for
    // exact time changes can result in flaxy numbers. To avoid that remember old
    // numbers to make sure the correct values are increasing in the next test.
    int screenOff = times["ScreenOff"];
    int sixty = times["60fps"];
    int ninety = times["90fps"];

    // Screen is off by default.
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    times = mRefreshRateStats.getTotalTimes();
    EXPECT_LT(screenOff, times["ScreenOff"]);
    EXPECT_EQ(sixty, times["60fps"]);
    EXPECT_EQ(ninety, times["90fps"]);

    mRefreshRateStats.setConfigMode(CONFIG_ID_90);
    mRefreshRateStats.setPowerMode(HWC_POWER_MODE_NORMAL);
    screenOff = mRefreshRateStats.getTotalTimes()["ScreenOff"];
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    times = mRefreshRateStats.getTotalTimes();
    EXPECT_EQ(screenOff, times["ScreenOff"]);
    EXPECT_EQ(sixty, times["60fps"]);
    EXPECT_LT(ninety, times["90fps"]);

    // When power mode is normal, time for configs updates.
    mRefreshRateStats.setConfigMode(CONFIG_ID_60);
    ninety = mRefreshRateStats.getTotalTimes()["90fps"];
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    times = mRefreshRateStats.getTotalTimes();
    EXPECT_EQ(screenOff, times["ScreenOff"]);
    EXPECT_EQ(ninety, times["90fps"]);
    EXPECT_LT(sixty, times["60fps"]);

    mRefreshRateStats.setConfigMode(CONFIG_ID_90);
    sixty = mRefreshRateStats.getTotalTimes()["60fps"];
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    times = mRefreshRateStats.getTotalTimes();
    EXPECT_EQ(screenOff, times["ScreenOff"]);
    EXPECT_LT(ninety, times["90fps"]);
    EXPECT_EQ(sixty, times["60fps"]);

    mRefreshRateStats.setConfigMode(CONFIG_ID_60);
    ninety = mRefreshRateStats.getTotalTimes()["90fps"];
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    times = mRefreshRateStats.getTotalTimes();
    EXPECT_EQ(screenOff, times["ScreenOff"]);
    EXPECT_EQ(ninety, times["90fps"]);
    EXPECT_LT(sixty, times["60fps"]);

    // Because the power mode is not HWC_POWER_MODE_NORMAL, switching the config
    // does not update refresh rates that come from the config.
    mRefreshRateStats.setPowerMode(HWC_POWER_MODE_DOZE);
    mRefreshRateStats.setConfigMode(CONFIG_ID_90);
    sixty = mRefreshRateStats.getTotalTimes()["60fps"];
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    times = mRefreshRateStats.getTotalTimes();
    EXPECT_LT(screenOff, times["ScreenOff"]);
    EXPECT_EQ(ninety, times["90fps"]);
    EXPECT_EQ(sixty, times["60fps"]);

    mRefreshRateStats.setConfigMode(CONFIG_ID_60);
    screenOff = mRefreshRateStats.getTotalTimes()["ScreenOff"];
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    times = mRefreshRateStats.getTotalTimes();
    EXPECT_LT(screenOff, times["ScreenOff"]);
    EXPECT_EQ(ninety, times["90fps"]);
    EXPECT_EQ(sixty, times["60fps"]);
}
} // namespace
} // namespace scheduler
} // namespace android
