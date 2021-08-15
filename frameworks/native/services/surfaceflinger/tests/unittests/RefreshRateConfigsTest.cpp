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

#include "DisplayHardware/HWC2.h"
#include "Scheduler/RefreshRateConfigs.h"
#include "mock/DisplayHardware/MockDisplay.h"

using namespace std::chrono_literals;
using testing::_;

namespace android {
namespace scheduler {

using RefreshRateType = RefreshRateConfigs::RefreshRateType;
using RefreshRate = RefreshRateConfigs::RefreshRate;

class RefreshRateConfigsTest : public testing::Test {
protected:
    static constexpr int CONFIG_ID_60 = 0;
    static constexpr hwc2_config_t HWC2_CONFIG_ID_60 = 0;
    static constexpr int CONFIG_ID_90 = 1;
    static constexpr hwc2_config_t HWC2_CONFIG_ID_90 = 1;
    static constexpr int64_t VSYNC_60 = 16666667;
    static constexpr int64_t VSYNC_90 = 11111111;

    RefreshRateConfigsTest();
    ~RefreshRateConfigsTest();

    void assertRatesEqual(const RefreshRate& left, const RefreshRate& right) {
        ASSERT_EQ(left.configId, right.configId);
        ASSERT_EQ(left.name, right.name);
        ASSERT_EQ(left.fps, right.fps);
    }

    RefreshRateConfigs mConfigs;
};

RefreshRateConfigsTest::RefreshRateConfigsTest() {
    const ::testing::TestInfo* const test_info =
            ::testing::UnitTest::GetInstance()->current_test_info();
    ALOGD("**** Setting up for %s.%s\n", test_info->test_case_name(), test_info->name());
}

RefreshRateConfigsTest::~RefreshRateConfigsTest() {
    const ::testing::TestInfo* const test_info =
            ::testing::UnitTest::GetInstance()->current_test_info();
    ALOGD("**** Tearing down after %s.%s\n", test_info->test_case_name(), test_info->name());
}

namespace {
/* ------------------------------------------------------------------------
 * Test cases
 */
TEST_F(RefreshRateConfigsTest, zeroDeviceConfigs_storesPowerSavingConfig) {
    std::vector<std::shared_ptr<const HWC2::Display::Config>> displayConfigs;
    mConfigs.populate(displayConfigs);

    // We always store a configuration for screen off.
    const auto& rates = mConfigs.getRefreshRates();
    ASSERT_EQ(1, rates.size());
    const auto& powerSavingRate = rates.find(RefreshRateType::POWER_SAVING);
    ASSERT_NE(rates.end(), powerSavingRate);
    ASSERT_EQ(rates.end(), rates.find(RefreshRateType::PERFORMANCE));
    ASSERT_EQ(rates.end(), rates.find(RefreshRateType::DEFAULT));

    RefreshRate expectedConfig =
            RefreshRate{SCREEN_OFF_CONFIG_ID, "ScreenOff", 0, HWC2_SCREEN_OFF_CONFIG_ID};
    assertRatesEqual(expectedConfig, *powerSavingRate->second);

    ASSERT_TRUE(mConfigs.getRefreshRate(RefreshRateType::POWER_SAVING));
    assertRatesEqual(expectedConfig, *mConfigs.getRefreshRate(RefreshRateType::POWER_SAVING));
    ASSERT_FALSE(mConfigs.getRefreshRate(RefreshRateType::PERFORMANCE));
    ASSERT_FALSE(mConfigs.getRefreshRate(RefreshRateType::DEFAULT));

    // Sanity check that getRefreshRate() does not modify the underlying configs.
    ASSERT_EQ(1, mConfigs.getRefreshRates().size());
}

TEST_F(RefreshRateConfigsTest, oneDeviceConfig_storesDefaultConfig) {
    auto display = new Hwc2::mock::Display();
    std::vector<std::shared_ptr<const HWC2::Display::Config>> displayConfigs;
    auto config60 = HWC2::Display::Config::Builder(*display, CONFIG_ID_60);
    config60.setVsyncPeriod(VSYNC_60);
    displayConfigs.push_back(config60.build());
    mConfigs.populate(displayConfigs);

    const auto& rates = mConfigs.getRefreshRates();
    ASSERT_EQ(2, rates.size());
    const auto& powerSavingRate = rates.find(RefreshRateType::POWER_SAVING);
    const auto& defaultRate = rates.find(RefreshRateType::DEFAULT);
    ASSERT_NE(rates.end(), powerSavingRate);
    ASSERT_NE(rates.end(), defaultRate);
    ASSERT_EQ(rates.end(), rates.find(RefreshRateType::PERFORMANCE));

    RefreshRate expectedPowerSavingConfig =
            RefreshRate{SCREEN_OFF_CONFIG_ID, "ScreenOff", 0, HWC2_SCREEN_OFF_CONFIG_ID};
    assertRatesEqual(expectedPowerSavingConfig, *powerSavingRate->second);
    RefreshRate expectedDefaultConfig = RefreshRate{CONFIG_ID_60, "60fps", 60, HWC2_CONFIG_ID_60};
    assertRatesEqual(expectedDefaultConfig, *defaultRate->second);

    ASSERT_TRUE(mConfigs.getRefreshRate(RefreshRateType::POWER_SAVING));
    assertRatesEqual(expectedPowerSavingConfig,
                     *mConfigs.getRefreshRate(RefreshRateType::POWER_SAVING));
    ASSERT_TRUE(mConfigs.getRefreshRate(RefreshRateType::DEFAULT));
    assertRatesEqual(expectedDefaultConfig, *mConfigs.getRefreshRate(RefreshRateType::DEFAULT));
    ASSERT_FALSE(mConfigs.getRefreshRate(RefreshRateType::PERFORMANCE));

    // Sanity check that getRefreshRate() does not modify the underlying configs.
    ASSERT_EQ(2, mConfigs.getRefreshRates().size());
}

TEST_F(RefreshRateConfigsTest, twoDeviceConfigs_storesPerformanceConfig) {
    auto display = new Hwc2::mock::Display();
    std::vector<std::shared_ptr<const HWC2::Display::Config>> displayConfigs;
    auto config60 = HWC2::Display::Config::Builder(*display, CONFIG_ID_60);
    config60.setVsyncPeriod(VSYNC_60);
    displayConfigs.push_back(config60.build());
    auto config90 = HWC2::Display::Config::Builder(*display, CONFIG_ID_90);
    config90.setVsyncPeriod(VSYNC_90);
    displayConfigs.push_back(config90.build());
    mConfigs.populate(displayConfigs);

    const auto& rates = mConfigs.getRefreshRates();
    ASSERT_EQ(3, rates.size());
    const auto& powerSavingRate = rates.find(RefreshRateType::POWER_SAVING);
    const auto& defaultRate = rates.find(RefreshRateType::DEFAULT);
    const auto& performanceRate = rates.find(RefreshRateType::PERFORMANCE);
    ASSERT_NE(rates.end(), powerSavingRate);
    ASSERT_NE(rates.end(), defaultRate);
    ASSERT_NE(rates.end(), performanceRate);

    RefreshRate expectedPowerSavingConfig =
            RefreshRate{SCREEN_OFF_CONFIG_ID, "ScreenOff", 0, HWC2_SCREEN_OFF_CONFIG_ID};
    assertRatesEqual(expectedPowerSavingConfig, *powerSavingRate->second);
    RefreshRate expectedDefaultConfig = RefreshRate{CONFIG_ID_60, "60fps", 60, HWC2_CONFIG_ID_60};
    assertRatesEqual(expectedDefaultConfig, *defaultRate->second);
    RefreshRate expectedPerformanceConfig =
            RefreshRate{CONFIG_ID_90, "90fps", 90, HWC2_CONFIG_ID_90};
    assertRatesEqual(expectedPerformanceConfig, *performanceRate->second);

    ASSERT_TRUE(mConfigs.getRefreshRate(RefreshRateType::POWER_SAVING));
    assertRatesEqual(expectedPowerSavingConfig,
                     *mConfigs.getRefreshRate(RefreshRateType::POWER_SAVING));
    ASSERT_TRUE(mConfigs.getRefreshRate(RefreshRateType::DEFAULT));
    assertRatesEqual(expectedDefaultConfig, *mConfigs.getRefreshRate(RefreshRateType::DEFAULT));
    ASSERT_TRUE(mConfigs.getRefreshRate(RefreshRateType::PERFORMANCE));
    assertRatesEqual(expectedPerformanceConfig,
                     *mConfigs.getRefreshRate(RefreshRateType::PERFORMANCE));
}
} // namespace
} // namespace scheduler
} // namespace android
