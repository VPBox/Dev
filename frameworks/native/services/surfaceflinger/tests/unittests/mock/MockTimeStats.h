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

#include <gmock/gmock.h>

#include "TimeStats/TimeStats.h"

namespace android {
namespace mock {

class TimeStats : public android::TimeStats {
public:
    TimeStats();
    ~TimeStats() override;

    MOCK_METHOD3(parseArgs, void(bool, const Vector<String16>&, std::string&));
    MOCK_METHOD0(isEnabled, bool());
    MOCK_METHOD0(miniDump, std::string());
    MOCK_METHOD0(incrementTotalFrames, void());
    MOCK_METHOD0(incrementMissedFrames, void());
    MOCK_METHOD0(incrementClientCompositionFrames, void());
    MOCK_METHOD4(setPostTime, void(int32_t, uint64_t, const std::string&, nsecs_t));
    MOCK_METHOD3(setLatchTime, void(int32_t, uint64_t, nsecs_t));
    MOCK_METHOD3(setDesiredTime, void(int32_t, uint64_t, nsecs_t));
    MOCK_METHOD3(setAcquireTime, void(int32_t, uint64_t, nsecs_t));
    MOCK_METHOD3(setAcquireFence, void(int32_t, uint64_t, const std::shared_ptr<FenceTime>&));
    MOCK_METHOD3(setPresentTime, void(int32_t, uint64_t, nsecs_t));
    MOCK_METHOD3(setPresentFence, void(int32_t, uint64_t, const std::shared_ptr<FenceTime>&));
    MOCK_METHOD1(onDestroy, void(int32_t));
    MOCK_METHOD2(removeTimeRecord, void(int32_t, uint64_t));
    MOCK_METHOD1(setPowerMode, void(int32_t));
    MOCK_METHOD2(recordRefreshRate, void(uint32_t, nsecs_t));
    MOCK_METHOD1(setPresentFenceGlobal, void(const std::shared_ptr<FenceTime>&));
};

} // namespace mock
} // namespace android
