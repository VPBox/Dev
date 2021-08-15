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
#include <gui/ISurfaceComposer.h>

#include "Scheduler/EventThread.h"
#include "Scheduler/RefreshRateConfigs.h"
#include "Scheduler/Scheduler.h"

namespace android {

class TestableScheduler : public Scheduler {
public:
    TestableScheduler(const scheduler::RefreshRateConfigs& refreshRateConfig)
          : Scheduler([](bool) {}, refreshRateConfig) {}

    // Creates EventThreadConnection with the given eventThread. Creates Scheduler::Connection
    // and adds it to the list of connectins. Returns the ConnectionHandle for the
    // Scheduler::Connection. This allows plugging in mock::EventThread.
    sp<Scheduler::ConnectionHandle> addConnection(std::unique_ptr<EventThread> eventThread) {
        sp<EventThreadConnection> eventThreadConnection =
                new EventThreadConnection(eventThread.get(), ResyncCallback(),
                                          ISurfaceComposer::eConfigChangedSuppress);
        const int64_t id = sNextId++;
        mConnections.emplace(id,
                             std::make_unique<Scheduler::Connection>(new ConnectionHandle(id),
                                                                     eventThreadConnection,
                                                                     std::move(eventThread)));
        return mConnections[id]->handle;
    }

    /* ------------------------------------------------------------------------
     * Read-write access to private data to set up preconditions and assert
     * post-conditions.
     */
    auto& mutablePrimaryHWVsyncEnabled() { return mPrimaryHWVsyncEnabled; }
    auto& mutableEventControlThread() { return mEventControlThread; }
    auto& mutablePrimaryDispSync() { return mPrimaryDispSync; }
    auto& mutableHWVsyncAvailable() { return mHWVsyncAvailable; }

    ~TestableScheduler() {
        // All these pointer and container clears help ensure that GMock does
        // not report a leaked object, since the Scheduler instance may
        // still be referenced by something despite our best efforts to destroy
        // it after each test is done.
        mutableEventControlThread().reset();
        mutablePrimaryDispSync().reset();
        mConnections.clear();
    };
};

} // namespace android
