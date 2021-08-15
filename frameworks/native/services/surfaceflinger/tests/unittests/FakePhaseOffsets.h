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

#include "Scheduler/PhaseOffsets.h"

namespace android {
namespace scheduler {

using RefreshRateType = RefreshRateConfigs::RefreshRateType;

class FakePhaseOffsets : public android::scheduler::PhaseOffsets {
    nsecs_t FAKE_PHASE_OFFSET_NS = 0;

public:
    FakePhaseOffsets() = default;
    ~FakePhaseOffsets() = default;

    nsecs_t getCurrentAppOffset() override { return FAKE_PHASE_OFFSET_NS; }
    nsecs_t getCurrentSfOffset() override { return FAKE_PHASE_OFFSET_NS; }

    PhaseOffsets::Offsets getOffsetsForRefreshRate(
            RefreshRateType /*refreshRateType*/) const override {
        return getCurrentOffsets();
    }

    // Returns early, early GL, and late offsets for Apps and SF.
    PhaseOffsets::Offsets getCurrentOffsets() const override {
        return Offsets{{RefreshRateType::DEFAULT, FAKE_PHASE_OFFSET_NS, FAKE_PHASE_OFFSET_NS},
                       {RefreshRateType::DEFAULT, FAKE_PHASE_OFFSET_NS, FAKE_PHASE_OFFSET_NS},
                       {RefreshRateType::DEFAULT, FAKE_PHASE_OFFSET_NS, FAKE_PHASE_OFFSET_NS}};
    }

    // This function should be called when the device is switching between different
    // refresh rates, to properly update the offsets.
    void setRefreshRateType(RefreshRateType /*refreshRateType*/) override {}

    nsecs_t getOffsetThresholdForNextVsync() const override { return FAKE_PHASE_OFFSET_NS; }

    // Returns current offsets in human friendly format.
    void dump(std::string& /*result*/) const override {}
};

} // namespace scheduler
} // namespace android
