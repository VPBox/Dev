/*
 * Copyright 2018 The Android Open Source Project
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
#include <gtest/gtest.h>
#include <utils/Log.h>

#include "AsyncCallRecorder.h"
#include "Scheduler/IdleTimer.h"

using namespace std::chrono_literals;

namespace android {
namespace scheduler {

class IdleTimerTest : public testing::Test {
protected:
    IdleTimerTest() = default;
    ~IdleTimerTest() override = default;

    // This timeout should be used when a 3ms callback is expected.
    // While the tests typically request a callback after 3ms, the scheduler
    // does not always cooperate, at it can take significantly longer (observed
    // 30ms).
    static constexpr auto waitTimeForExpected3msCallback = 100ms;

    // This timeout should be used when an 3ms callback is not expected.
    // Note that there can be false-negatives if the callback happens later.
    static constexpr auto waitTimeForUnexpected3msCallback = 6ms;

    AsyncCallRecorder<void (*)()> mResetTimerCallback;
    AsyncCallRecorder<void (*)()> mExpiredTimerCallback;

    std::unique_ptr<IdleTimer> mIdleTimer;

    void clearPendingCallbacks() {
        while (mExpiredTimerCallback.waitForCall(0us).has_value()) {
        }
    }
};

namespace {
TEST_F(IdleTimerTest, createAndDestroyTest) {
    mIdleTimer = std::make_unique<scheduler::IdleTimer>(3ms, [] {}, [] {});
}

TEST_F(IdleTimerTest, startStopTest) {
    mIdleTimer = std::make_unique<scheduler::IdleTimer>(30ms, mResetTimerCallback.getInvocable(),
                                                        mExpiredTimerCallback.getInvocable());
    auto startTime = std::chrono::steady_clock::now();
    mIdleTimer->start();
    EXPECT_TRUE(mResetTimerCallback.waitForCall().has_value());
    // The idle timer fires after 30ms, so there should be no callback within
    // 25ms (waiting for a callback for the full 30ms would be problematic).
    bool callbackCalled = mExpiredTimerCallback.waitForCall(25ms).has_value();
    // Under ideal conditions there should be no event. But occasionally
    // it is possible that the wait just prior takes more than 30ms, and
    // a callback is observed. We check the elapsed time since before the IdleTimer
    // thread was started as a sanity check to not have a flakey test.
    EXPECT_FALSE(callbackCalled && std::chrono::steady_clock::now() - startTime < 30ms);

    std::this_thread::sleep_for(std::chrono::milliseconds(25));
    EXPECT_FALSE(mResetTimerCallback.waitForCall().has_value());
    mIdleTimer->stop();
}

TEST_F(IdleTimerTest, resetTest) {
    mIdleTimer = std::make_unique<scheduler::IdleTimer>(20ms, mResetTimerCallback.getInvocable(),
                                                        mExpiredTimerCallback.getInvocable());
    mIdleTimer->start();
    EXPECT_TRUE(mResetTimerCallback.waitForCall().has_value());
    // Observe any event that happens in about 25ms. We don't care if one was
    // observed or not.
    mExpiredTimerCallback.waitForCall(25ms).has_value();
    mIdleTimer->reset();
    EXPECT_TRUE(mResetTimerCallback.waitForCall().has_value());
    // There may have been a race with the reset. Clear any callbacks we
    // received right afterwards.
    clearPendingCallbacks();
    // A single callback should be generated after 30ms
    EXPECT_TRUE(
            mExpiredTimerCallback.waitForCall(waitTimeForExpected3msCallback + 30ms).has_value());
    // After one event, it should be idle, and not generate another.
    EXPECT_FALSE(
            mExpiredTimerCallback.waitForCall(waitTimeForUnexpected3msCallback * 10).has_value());
    mIdleTimer->stop();
    // Final quick check that no more callback were observed.
    EXPECT_FALSE(mExpiredTimerCallback.waitForCall(0ms).has_value());
    EXPECT_FALSE(mResetTimerCallback.waitForCall(0ms).has_value());
}

TEST_F(IdleTimerTest, resetBackToBackTest) {
    mIdleTimer = std::make_unique<scheduler::IdleTimer>(20ms, mResetTimerCallback.getInvocable(),
                                                        mExpiredTimerCallback.getInvocable());
    mIdleTimer->start();
    EXPECT_TRUE(mResetTimerCallback.waitForCall().has_value());

    mIdleTimer->reset();
    EXPECT_FALSE(mResetTimerCallback.waitForCall(1ms).has_value());
    EXPECT_FALSE(mExpiredTimerCallback.waitForCall(waitTimeForUnexpected3msCallback).has_value());

    mIdleTimer->reset();
    EXPECT_FALSE(mResetTimerCallback.waitForCall(1ms).has_value());
    EXPECT_FALSE(mExpiredTimerCallback.waitForCall(waitTimeForUnexpected3msCallback).has_value());

    mIdleTimer->reset();
    EXPECT_FALSE(mResetTimerCallback.waitForCall(1ms).has_value());
    EXPECT_FALSE(mExpiredTimerCallback.waitForCall(waitTimeForUnexpected3msCallback).has_value());

    mIdleTimer->reset();
    EXPECT_FALSE(mResetTimerCallback.waitForCall(1ms).has_value());
    EXPECT_FALSE(mExpiredTimerCallback.waitForCall(waitTimeForUnexpected3msCallback).has_value());

    // A single callback should be generated after 30ms
    EXPECT_TRUE(
            mExpiredTimerCallback.waitForCall(waitTimeForExpected3msCallback + 30ms).has_value());
    mIdleTimer->stop();
    // Final quick check that no more callback were observed.
    EXPECT_FALSE(mExpiredTimerCallback.waitForCall(0ms).has_value());
    EXPECT_FALSE(mResetTimerCallback.waitForCall(0ms).has_value());
}

TEST_F(IdleTimerTest, startNotCalledTest) {
    mIdleTimer = std::make_unique<scheduler::IdleTimer>(3ms, mResetTimerCallback.getInvocable(),
                                                        mExpiredTimerCallback.getInvocable());
    // The start hasn't happened, so the callback does not happen.
    EXPECT_FALSE(mExpiredTimerCallback.waitForCall(waitTimeForUnexpected3msCallback).has_value());
    EXPECT_FALSE(mResetTimerCallback.waitForCall().has_value());
    mIdleTimer->stop();
    // Final quick check that no more callback were observed.
    EXPECT_FALSE(mExpiredTimerCallback.waitForCall(0ms).has_value());
    EXPECT_FALSE(mResetTimerCallback.waitForCall(0ms).has_value());
}

TEST_F(IdleTimerTest, idleTimerIdlesTest) {
    mIdleTimer = std::make_unique<scheduler::IdleTimer>(3ms, mResetTimerCallback.getInvocable(),
                                                        mExpiredTimerCallback.getInvocable());
    mIdleTimer->start();
    EXPECT_TRUE(mResetTimerCallback.waitForCall().has_value());

    // A callback should be generated after 3ms
    EXPECT_TRUE(mExpiredTimerCallback.waitForCall(waitTimeForExpected3msCallback).has_value());
    // After one event, it should be idle, and not generate another.
    EXPECT_FALSE(mExpiredTimerCallback.waitForCall(waitTimeForUnexpected3msCallback).has_value());
    // Once reset, it should generate another
    mIdleTimer->reset();
    EXPECT_TRUE(mResetTimerCallback.waitForCall().has_value());
    EXPECT_TRUE(mExpiredTimerCallback.waitForCall(waitTimeForExpected3msCallback).has_value());
    mIdleTimer->stop();
    // Final quick check that no more callback were observed.
    EXPECT_FALSE(mExpiredTimerCallback.waitForCall(0ms).has_value());
    EXPECT_FALSE(mResetTimerCallback.waitForCall(0ms).has_value());
}

TEST_F(IdleTimerTest, timeoutCallbackExecutionTest) {
    mIdleTimer = std::make_unique<scheduler::IdleTimer>(3ms, mResetTimerCallback.getInvocable(),
                                                        mExpiredTimerCallback.getInvocable());
    mIdleTimer->start();
    EXPECT_TRUE(mResetTimerCallback.waitForCall().has_value());
    EXPECT_TRUE(mExpiredTimerCallback.waitForCall(waitTimeForExpected3msCallback).has_value());
    mIdleTimer->stop();
}

TEST_F(IdleTimerTest, noCallbacksAfterStopAndResetTest) {
    mIdleTimer = std::make_unique<scheduler::IdleTimer>(3ms, mResetTimerCallback.getInvocable(),
                                                        mExpiredTimerCallback.getInvocable());
    mIdleTimer->start();
    EXPECT_TRUE(mResetTimerCallback.waitForCall().has_value());
    EXPECT_TRUE(mExpiredTimerCallback.waitForCall(waitTimeForExpected3msCallback).has_value());

    mIdleTimer->stop();
    clearPendingCallbacks();
    mIdleTimer->reset();
    EXPECT_FALSE(mExpiredTimerCallback.waitForCall(waitTimeForUnexpected3msCallback).has_value());
    EXPECT_FALSE(mResetTimerCallback.waitForCall().has_value());
}

TEST_F(IdleTimerTest, noCallbacksAfterStopTest) {
    mIdleTimer = std::make_unique<scheduler::IdleTimer>(3ms, mResetTimerCallback.getInvocable(),
                                                        mExpiredTimerCallback.getInvocable());
    mIdleTimer->start();
    EXPECT_TRUE(mResetTimerCallback.waitForCall().has_value());

    mIdleTimer->stop();
    clearPendingCallbacks();
    mIdleTimer->reset();

    // No more idle events should be observed
    EXPECT_FALSE(mExpiredTimerCallback.waitForCall(waitTimeForUnexpected3msCallback).has_value());
    EXPECT_FALSE(mResetTimerCallback.waitForCall().has_value());
}

} // namespace
} // namespace scheduler
} // namespace android
