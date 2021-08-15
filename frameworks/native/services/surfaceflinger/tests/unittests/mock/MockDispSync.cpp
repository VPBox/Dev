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

#include "mock/MockDispSync.h"
#include <thread>

namespace android {
namespace mock {

// Explicit default instantiation is recommended.
DispSync::DispSync() = default;
DispSync::~DispSync() = default;

status_t DispSync::addEventListener(const char* /*name*/, nsecs_t phase, Callback* callback,
                                    nsecs_t /*lastCallbackTime*/) {
    if (mCallback.callback != nullptr) {
        return BAD_VALUE;
    }

    mCallback = {callback, phase};
    return NO_ERROR;
}
status_t DispSync::removeEventListener(Callback* callback, nsecs_t* /*outLastCallback*/) {
    if (mCallback.callback != callback) {
        return BAD_VALUE;
    }

    mCallback = {nullptr, 0};
    return NO_ERROR;
}

status_t DispSync::changePhaseOffset(Callback* callback, nsecs_t phase) {
    if (mCallback.callback != callback) {
        return BAD_VALUE;
    }

    mCallback.phase = phase;
    return NO_ERROR;
}

void DispSync::triggerCallback() {
    if (mCallback.callback == nullptr) return;

    mCallback.callback->onDispSyncEvent(
            std::chrono::steady_clock::now().time_since_epoch().count());
}

} // namespace mock
} // namespace android
