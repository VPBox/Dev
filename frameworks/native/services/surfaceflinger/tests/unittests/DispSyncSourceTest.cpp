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
#define LOG_TAG "LibSurfaceFlingerUnittests"
#define LOG_NDEBUG 0

#include <inttypes.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <log/log.h>

#include "AsyncCallRecorder.h"
#include "Scheduler/DispSyncSource.h"
#include "mock/MockDispSync.h"

namespace android {
namespace {

using namespace std::chrono_literals;
using testing::Return;

class DispSyncSourceTest : public testing::Test, private VSyncSource::Callback {
protected:
    DispSyncSourceTest();
    ~DispSyncSourceTest() override;

    void createDispSync();
    void createDispSyncSource();

    void onVSyncEvent(nsecs_t when) override;

    std::unique_ptr<mock::DispSync> mDispSync;
    std::unique_ptr<DispSyncSource> mDispSyncSource;

    AsyncCallRecorder<void (*)(nsecs_t)> mVSyncEventCallRecorder;

    static constexpr std::chrono::nanoseconds mPhaseOffset = 6ms;
    static constexpr std::chrono::nanoseconds mOffsetThresholdForNextVsync = 16ms;
    static constexpr int mIterations = 100;
};

DispSyncSourceTest::DispSyncSourceTest() {
    const ::testing::TestInfo* const test_info =
            ::testing::UnitTest::GetInstance()->current_test_info();
    ALOGD("**** Setting up for %s.%s\n", test_info->test_case_name(), test_info->name());
}

DispSyncSourceTest::~DispSyncSourceTest() {
    const ::testing::TestInfo* const test_info =
            ::testing::UnitTest::GetInstance()->current_test_info();
    ALOGD("**** Tearing down after %s.%s\n", test_info->test_case_name(), test_info->name());
}

void DispSyncSourceTest::onVSyncEvent(nsecs_t when) {
    ALOGD("onVSyncEvent: %" PRId64, when);

    mVSyncEventCallRecorder.recordCall(when);
}

void DispSyncSourceTest::createDispSync() {
    mDispSync = std::make_unique<mock::DispSync>();
}

void DispSyncSourceTest::createDispSyncSource() {
    createDispSync();
    mDispSyncSource = std::make_unique<DispSyncSource>(mDispSync.get(), mPhaseOffset.count(),
                                                       mOffsetThresholdForNextVsync.count(), true,
                                                       "DispSyncSourceTest");
    mDispSyncSource->setCallback(this);
}

/* ------------------------------------------------------------------------
 * Test cases
 */

TEST_F(DispSyncSourceTest, createDispSync) {
    createDispSync();
    EXPECT_TRUE(mDispSync);
}

TEST_F(DispSyncSourceTest, createDispSyncSource) {
    createDispSyncSource();
    EXPECT_TRUE(mDispSyncSource);
}

TEST_F(DispSyncSourceTest, noCallbackAfterInit) {
    createDispSyncSource();
    EXPECT_TRUE(mDispSyncSource);

    // DispSyncSource starts with Vsync disabled
    mDispSync->triggerCallback();
    EXPECT_FALSE(mVSyncEventCallRecorder.waitForUnexpectedCall().has_value());
}

TEST_F(DispSyncSourceTest, waitForCallbacks) {
    createDispSyncSource();
    EXPECT_TRUE(mDispSyncSource);

    mDispSyncSource->setVSyncEnabled(true);
    EXPECT_EQ(mDispSync->getCallbackPhase(), mPhaseOffset.count());

    for (int i = 0; i < mIterations; i++) {
        mDispSync->triggerCallback();
        EXPECT_TRUE(mVSyncEventCallRecorder.waitForCall().has_value());
    }
}

TEST_F(DispSyncSourceTest, waitForCallbacksWithPhaseChange) {
    createDispSyncSource();
    EXPECT_TRUE(mDispSyncSource);

    mDispSyncSource->setVSyncEnabled(true);
    EXPECT_EQ(mDispSync->getCallbackPhase(), mPhaseOffset.count());

    for (int i = 0; i < mIterations; i++) {
        mDispSync->triggerCallback();
        EXPECT_TRUE(mVSyncEventCallRecorder.waitForCall().has_value());
    }

    EXPECT_CALL(*mDispSync, getPeriod()).Times(1).WillOnce(Return(16666666));
    mDispSyncSource->setPhaseOffset((mPhaseOffset / 2).count());

    EXPECT_EQ(mDispSync->getCallbackPhase(), (mPhaseOffset / 2).count());

    for (int i = 0; i < mIterations; i++) {
        mDispSync->triggerCallback();
        EXPECT_TRUE(mVSyncEventCallRecorder.waitForCall().has_value());
    }
}

} // namespace
} // namespace android
