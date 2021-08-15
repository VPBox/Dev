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

#undef LOG_TAG
#define LOG_TAG "LibSurfaceFlingerUnittests"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <log/log.h>

#include "AsyncCallRecorder.h"
#include "Scheduler/EventControlThread.h"

namespace android {
namespace {

using namespace std::chrono_literals;
using testing::_;

class EventControlThreadTest : public testing::Test {
protected:
    EventControlThreadTest();
    ~EventControlThreadTest() override;

    void createThread();

    void expectVSyncEnableCallbackCalled(bool enable);

    AsyncCallRecorder<void (*)(bool)> mVSyncSetEnabledCallRecorder;

    std::unique_ptr<EventControlThread> mThread;
};

EventControlThreadTest::EventControlThreadTest() {
    const ::testing::TestInfo* const test_info =
            ::testing::UnitTest::GetInstance()->current_test_info();
    ALOGD("**** Setting up for %s.%s\n", test_info->test_case_name(), test_info->name());
}

EventControlThreadTest::~EventControlThreadTest() {
    const ::testing::TestInfo* const test_info =
            ::testing::UnitTest::GetInstance()->current_test_info();
    ALOGD("**** Tearing down after %s.%s\n", test_info->test_case_name(), test_info->name());
}

void EventControlThreadTest::createThread() {
    mThread = std::make_unique<android::impl::EventControlThread>(
            mVSyncSetEnabledCallRecorder.getInvocable());
}

void EventControlThreadTest::expectVSyncEnableCallbackCalled(bool expectedEnabled) {
    auto args = mVSyncSetEnabledCallRecorder.waitForCall();
    ASSERT_TRUE(args.has_value());
    EXPECT_EQ(std::get<0>(args.value()), expectedEnabled);
}

/* ------------------------------------------------------------------------
 * Test cases
 */

TEST_F(EventControlThreadTest, signalsVSyncDisabledOnStartup) {
    createThread();

    // On thread start, there should be an automatic explicit call to disable
    // vsyncs
    expectVSyncEnableCallbackCalled(false);
}

TEST_F(EventControlThreadTest, signalsVSyncDisabledOnce) {
    createThread();
    expectVSyncEnableCallbackCalled(false);

    mThread->setVsyncEnabled(false);

    EXPECT_FALSE(mVSyncSetEnabledCallRecorder.waitForUnexpectedCall().has_value());
}

TEST_F(EventControlThreadTest, signalsVSyncEnabledThenDisabled) {
    createThread();
    expectVSyncEnableCallbackCalled(false);

    mThread->setVsyncEnabled(true);

    expectVSyncEnableCallbackCalled(true);

    mThread->setVsyncEnabled(false);

    expectVSyncEnableCallbackCalled(false);
}

TEST_F(EventControlThreadTest, signalsVSyncEnabledOnce) {
    createThread();
    expectVSyncEnableCallbackCalled(false);

    mThread->setVsyncEnabled(true);

    expectVSyncEnableCallbackCalled(true);

    mThread->setVsyncEnabled(true);

    EXPECT_FALSE(mVSyncSetEnabledCallRecorder.waitForUnexpectedCall().has_value());
}

} // namespace
} // namespace android
