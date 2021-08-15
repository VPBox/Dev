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

#pragma once

#include <algorithm>
#include <numeric>

#include "android-base/stringprintf.h"

#include "DisplayHardware/HWComposer.h"
#include "Scheduler/SchedulerUtils.h"

namespace android {
namespace scheduler {

/**
 * This class is used to encapsulate configuration for refresh rates. It holds information
 * about available refresh rates on the device, and the mapping between the numbers and human
 * readable names.
 */
class RefreshRateConfigs {
public:
    // Enum to indicate which vsync rate to run at. Power saving is intended to be the lowest
    // (eg. when the screen is in AOD mode or off), default is the old 60Hz, and performance
    // is the new 90Hz. Eventually we want to have a way for vendors to map these in the configs.
    enum class RefreshRateType { POWER_SAVING, DEFAULT, PERFORMANCE };

    struct RefreshRate {
        // This config ID corresponds to the position of the config in the vector that is stored
        // on the device.
        int configId;
        // Human readable name of the refresh rate.
        std::string name;
        // Refresh rate in frames per second, rounded to the nearest integer.
        uint32_t fps = 0;
        // config Id (returned from HWC2::Display::Config::getId())
        hwc2_config_t id;
    };

    // TODO(b/122916473): Get this information from configs prepared by vendors, instead of
    // baking them in.
    const std::map<RefreshRateType, std::shared_ptr<RefreshRate>>& getRefreshRates() const {
        return mRefreshRates;
    }
    std::shared_ptr<RefreshRate> getRefreshRate(RefreshRateType type) const {
        const auto& refreshRate = mRefreshRates.find(type);
        if (refreshRate != mRefreshRates.end()) {
            return refreshRate->second;
        }
        return nullptr;
    }

    RefreshRateType getRefreshRateType(hwc2_config_t id) const {
        for (const auto& [type, refreshRate] : mRefreshRates) {
            if (refreshRate->id == id) {
                return type;
            }
        }

        return RefreshRateType::DEFAULT;
    }

    void populate(const std::vector<std::shared_ptr<const HWC2::Display::Config>>& configs) {
        mRefreshRates.clear();

        // This is the rate that HWC encapsulates right now when the device is in DOZE mode.
        mRefreshRates.emplace(RefreshRateType::POWER_SAVING,
                              std::make_shared<RefreshRate>(
                                      RefreshRate{SCREEN_OFF_CONFIG_ID, "ScreenOff", 0,
                                                  HWC2_SCREEN_OFF_CONFIG_ID}));

        if (configs.size() < 1) {
            ALOGE("Device does not have valid configs. Config size is 0.");
            return;
        }

        // Create a map between config index and vsync period. This is all the info we need
        // from the configs.
        std::vector<std::pair<int, nsecs_t>> configIdToVsyncPeriod;
        for (int i = 0; i < configs.size(); ++i) {
            configIdToVsyncPeriod.emplace_back(i, configs.at(i)->getVsyncPeriod());
        }

        std::sort(configIdToVsyncPeriod.begin(), configIdToVsyncPeriod.end(),
                  [](const std::pair<int, nsecs_t>& a, const std::pair<int, nsecs_t>& b) {
                      return a.second > b.second;
                  });

        // When the configs are ordered by the resync rate. We assume that the first one is DEFAULT.
        nsecs_t vsyncPeriod = configIdToVsyncPeriod[0].second;
        if (vsyncPeriod != 0) {
            const float fps = 1e9 / vsyncPeriod;
            const int configId = configIdToVsyncPeriod[0].first;
            mRefreshRates.emplace(RefreshRateType::DEFAULT,
                                  std::make_shared<RefreshRate>(
                                          RefreshRate{configId, base::StringPrintf("%2.ffps", fps),
                                                      static_cast<uint32_t>(fps),
                                                      configs.at(configId)->getId()}));
        }

        if (configs.size() < 2) {
            return;
        }

        // When the configs are ordered by the resync rate. We assume that the second one is
        // PERFORMANCE, eg. the higher rate.
        vsyncPeriod = configIdToVsyncPeriod[1].second;
        if (vsyncPeriod != 0) {
            const float fps = 1e9 / vsyncPeriod;
            const int configId = configIdToVsyncPeriod[1].first;
            mRefreshRates.emplace(RefreshRateType::PERFORMANCE,
                                  std::make_shared<RefreshRate>(
                                          RefreshRate{configId, base::StringPrintf("%2.ffps", fps),
                                                      static_cast<uint32_t>(fps),
                                                      configs.at(configId)->getId()}));
        }
    }

private:
    std::map<RefreshRateType, std::shared_ptr<RefreshRate>> mRefreshRates;
};

} // namespace scheduler
} // namespace android
