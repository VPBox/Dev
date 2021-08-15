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

#include "Scheduler/EventThread.h"
#include "Scheduler/MessageQueue.h"

namespace android {
namespace mock {

class MessageQueue : public android::MessageQueue {
public:
    MessageQueue();
    ~MessageQueue() override;

    MOCK_METHOD1(init, void(const sp<SurfaceFlinger>&));
    MOCK_METHOD2(setEventThread, void(android::EventThread*, ResyncCallback));
    MOCK_METHOD1(setEventConnection, void(const sp<EventThreadConnection>& connection));
    MOCK_METHOD0(waitMessage, void());
    MOCK_METHOD2(postMessage, status_t(const sp<MessageBase>&, nsecs_t));
    MOCK_METHOD0(invalidate, void());
    MOCK_METHOD0(refresh, void());
};

} // namespace mock
} // namespace android
