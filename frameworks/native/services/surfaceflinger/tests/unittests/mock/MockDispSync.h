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

#pragma once

#include <gmock/gmock.h>

#include "Scheduler/DispSync.h"

namespace android {
namespace mock {

class DispSync : public android::DispSync {
public:
    DispSync();
    ~DispSync() override;

    MOCK_METHOD0(reset, void());
    MOCK_METHOD1(addPresentFence, bool(const std::shared_ptr<FenceTime>&));
    MOCK_METHOD0(beginResync, void());
    MOCK_METHOD2(addResyncSample, bool(nsecs_t, bool*));
    MOCK_METHOD0(endResync, void());
    MOCK_METHOD1(setPeriod, void(nsecs_t));
    MOCK_METHOD0(getPeriod, nsecs_t());
    MOCK_METHOD0(getIntendedPeriod, nsecs_t());
    MOCK_METHOD1(setRefreshSkipCount, void(int));
    MOCK_CONST_METHOD1(computeNextRefresh, nsecs_t(int));
    MOCK_METHOD1(setIgnorePresentFences, void(bool));
    MOCK_METHOD0(expectedPresentTime, nsecs_t());

    MOCK_CONST_METHOD1(dump, void(std::string&));

    status_t addEventListener(const char* name, nsecs_t phase, Callback* callback,
                              nsecs_t lastCallbackTime) override;
    status_t removeEventListener(Callback* callback, nsecs_t* outLastCallback) override;
    status_t changePhaseOffset(Callback* callback, nsecs_t phase) override;

    nsecs_t getCallbackPhase() { return mCallback.phase; }

    void triggerCallback();

private:
    struct CallbackType {
        Callback* callback = nullptr;
        nsecs_t phase;
    };
    CallbackType mCallback;
};

} // namespace mock
} // namespace android
