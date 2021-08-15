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

// #define LOG_NDEBUG 0
#define LOG_TAG "BatteryChecker_test"
#include <utils/Log.h>

#include <gtest/gtest.h>

#include <media/stagefright/BatteryChecker.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/foundation/AHandler.h>

#include <vector>

namespace android {

static const int kBatteryTimeoutUs = 1000000ll; // 1 seconds
static const int kTestMarginUs = 50000ll; // 50ms
static const int kWaitStatusChangeUs = kBatteryTimeoutUs + kTestMarginUs;
static const int kSparseFrameIntervalUs = kBatteryTimeoutUs - kTestMarginUs;

class BatteryCheckerTestHandler : public AHandler {
    enum EventType {
        // Events simulating MediaCodec
        kWhatStart = 0,             // codec entering executing state
        kWhatStop,                  // codec exiting executing state
        kWhatActivity,        // codec queue input or dequeue output
        kWhatReleased,     // codec released
        kWhatCheckpoint,        // test checkpoing with expected values on On/Off

        // Message for battery checker monitor (not for testing through runTest())
        kWhatBatteryChecker,
    };

    struct Operation {
        int32_t event;
        int64_t delay = 0;
        uint32_t repeatCount = 0;
        int32_t expectedOnCounter = 0;
        int32_t expectedOffCounter = 0;
    };

    std::vector<Operation> mOps;
    sp<BatteryChecker> mBatteryChecker;
    int32_t mOnCounter;
    int32_t mOffCounter;
    Condition mDone;
    Mutex mLock;

    BatteryCheckerTestHandler() : mOnCounter(0), mOffCounter(0) {}

    void runTest(const std::vector<Operation> &ops, int64_t timeoutUs) {
        mOps = ops;

        mBatteryChecker = new BatteryChecker(
                new AMessage(kWhatBatteryChecker, this), kBatteryTimeoutUs);

        (new AMessage(ops[0].event, this))->post();

        // wait for done
        AutoMutex lock(mLock);
        EXPECT_NE(TIMED_OUT, mDone.waitRelative(mLock, timeoutUs * 1000ll));
    }

    virtual void onMessageReceived(const sp<AMessage> &msg);

    friend class BatteryCheckerTest;
};

class BatteryCheckerTest : public ::testing::Test {
public:
    BatteryCheckerTest()
        : mLooper(new ALooper)
        , mHandler(new BatteryCheckerTestHandler()) {
        mLooper->setName("BatterCheckerLooper");
        mLooper->start(false, false, ANDROID_PRIORITY_AUDIO);
        mLooper->registerHandler(mHandler);
    }

protected:
    using EventType = BatteryCheckerTestHandler::EventType;
    using Operation = BatteryCheckerTestHandler::Operation;

    virtual ~BatteryCheckerTest() {
        mLooper->stop();
        mLooper->unregisterHandler(mHandler->id());
    }

    void runTest(const std::vector<Operation> &ops, int64_t timeoutUs) {
        mHandler->runTest(ops, timeoutUs);
    }

    sp<ALooper> mLooper;
    sp<BatteryCheckerTestHandler> mHandler;
};

void BatteryCheckerTestHandler::onMessageReceived(const sp<AMessage> &msg) {
    switch(msg->what()) {
    case kWhatStart:
        mBatteryChecker->setExecuting(true);
        break;
    case kWhatStop:
        mBatteryChecker->setExecuting(false);
        break;
    case kWhatActivity:
        mBatteryChecker->onCodecActivity([this] () { mOnCounter++; });
        break;
    case kWhatReleased:
        mBatteryChecker->onClientRemoved();
        break;
    case kWhatBatteryChecker:
        mBatteryChecker->onCheckBatteryTimer(msg, [this] () { mOffCounter++;  });
        break;
    case kWhatCheckpoint:
        // verify ON/OFF state and total events
        EXPECT_EQ(mOnCounter, mOps[0].expectedOnCounter);
        EXPECT_EQ(mOffCounter, mOps[0].expectedOffCounter);
        break;
    default:
        TRESPASS();
    }
    if (msg->what() != kWhatBatteryChecker) {
        EXPECT_EQ(msg->what(), mOps[0].event);
        // post next message
        if (!mOps[0].repeatCount) {
            mOps.erase(mOps.begin());
        } else {
            mOps[0].repeatCount--;
        }
        int64_t duration = mOps[0].delay;
        if (!mOps.empty()) {
            (new AMessage(mOps[0].event, this))->post(duration);
        } else {
            AutoMutex lock(mLock);
            mDone.signal();
        }
    }
}

TEST_F(BatteryCheckerTest, testNormalOperations) {
    runTest({
        {EventType::kWhatStart,        0ll},
        {EventType::kWhatCheckpoint,   0ll, 0, 0, 0},
        {EventType::kWhatActivity,     33333ll},
        {EventType::kWhatCheckpoint,   0ll, 0, 1, 0},                 // ON
        {EventType::kWhatActivity,     33333ll, 2*kWaitStatusChangeUs/33333ll},
        {EventType::kWhatCheckpoint,   0ll, 0, 1, 0},
        {EventType::kWhatCheckpoint,   kWaitStatusChangeUs, 0, 1, 1}, // OFF
    }, 10000000ll);
}

TEST_F(BatteryCheckerTest, testPauseResume) {
    runTest({
        {EventType::kWhatStart,        0ll},
        {EventType::kWhatCheckpoint,   0ll, 0, 0, 0},
        {EventType::kWhatActivity,     33333ll},
        {EventType::kWhatCheckpoint,   0ll, 0, 1, 0},                 // ON
        {EventType::kWhatCheckpoint,   kWaitStatusChangeUs, 0, 1, 1}, // OFF
        {EventType::kWhatActivity,     33333ll},
        {EventType::kWhatCheckpoint,   0ll, 0, 2, 1},                 // ON
        {EventType::kWhatCheckpoint,   kWaitStatusChangeUs, 0, 2, 2}, // OFF
    }, 10000000ll);
}

TEST_F(BatteryCheckerTest, testClientRemovedAndRestart) {
    runTest({
        {EventType::kWhatStart,        0ll},
        {EventType::kWhatActivity,     33333ll, kWaitStatusChangeUs/33333ll},
        {EventType::kWhatCheckpoint,   0ll, 0, 1, 0},

        // stop executing state itself shouldn't trigger any calls
        {EventType::kWhatStop,         0ll},
        {EventType::kWhatCheckpoint,   0ll, 0, 1, 0},

        // release shouldn't trigger any calls either,
        // client resource will be removed entirely
        {EventType::kWhatReleased,     0ll},
        {EventType::kWhatCheckpoint,   0ll, 0, 1, 0},
        {EventType::kWhatCheckpoint,   kWaitStatusChangeUs, 0, 1, 0},

        // start pushing buffers again, On should be received without any Off
        {EventType::kWhatStart,        0ll},
        {EventType::kWhatActivity,     33333ll},
        {EventType::kWhatCheckpoint,   0ll, 0, 2, 0},

        // double check that only new checker msg triggers OFF,
        // left-over checker msg from stale generate discarded
        {EventType::kWhatCheckpoint,   kWaitStatusChangeUs, 0, 2, 1},
    }, 10000000ll);
}

TEST_F(BatteryCheckerTest, testActivityWhileNotExecuting) {
    runTest({
        // activity before start shouldn't trigger
        {EventType::kWhatActivity,     0ll},
        {EventType::kWhatCheckpoint,   0ll, 0, 0, 0},

        {EventType::kWhatStart,        0ll},
        {EventType::kWhatCheckpoint,   0ll, 0, 0, 0},

        // activity after start before stop should trigger
        {EventType::kWhatActivity,     33333ll},
        {EventType::kWhatCheckpoint,   0ll, 0, 1, 0},

        // stop executing state itself shouldn't trigger any calls
        {EventType::kWhatStop,         0ll},
        {EventType::kWhatCheckpoint,   0ll, 0, 1, 0},

        // keep pushing another 3 seconds after stop, expected to OFF
        {EventType::kWhatActivity,     33333ll, kWaitStatusChangeUs/33333ll},
        {EventType::kWhatCheckpoint,   0ll, 0, 1, 1},
    }, 10000000ll);
}

TEST_F(BatteryCheckerTest, testSparseActivity) {
    runTest({
        {EventType::kWhatStart,        0ll},
        {EventType::kWhatCheckpoint,   0ll, 0, 0, 0},

        // activity arrives sparsely with interval only slightly small than timeout
        // should only trigger 1 ON
        {EventType::kWhatActivity,     kSparseFrameIntervalUs, 2},
        {EventType::kWhatCheckpoint,   0ll, 0, 1, 0},
        {EventType::kWhatCheckpoint,   kSparseFrameIntervalUs, 0, 1, 0},
        {EventType::kWhatCheckpoint,   kTestMarginUs, 0, 1, 1}, // OFF
    }, 10000000ll);
}
} // namespace android
