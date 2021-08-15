#undef LOG_TAG
#define LOG_TAG "SchedulerUnittests"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <log/log.h>

#include <mutex>

#include "Scheduler/EventControlThread.h"
#include "Scheduler/EventThread.h"
#include "Scheduler/Scheduler.h"
#include "mock/MockEventThread.h"

using testing::_;
using testing::Return;

namespace android {

constexpr PhysicalDisplayId PHYSICAL_DISPLAY_ID = 999;

class SchedulerTest : public testing::Test {
protected:
    class MockEventThreadConnection : public android::EventThreadConnection {
    public:
        explicit MockEventThreadConnection(EventThread* eventThread)
              : EventThreadConnection(eventThread, ResyncCallback(),
                                      ISurfaceComposer::eConfigChangedSuppress) {}
        ~MockEventThreadConnection() = default;

        MOCK_METHOD1(stealReceiveChannel, status_t(gui::BitTube* outChannel));
        MOCK_METHOD1(setVsyncRate, status_t(uint32_t count));
        MOCK_METHOD0(requestNextVsync, void());
    };

    scheduler::RefreshRateConfigs mRefreshRateConfigs;

    /**
     * This mock Scheduler class uses implementation of mock::EventThread but keeps everything else
     * the same.
     */
    class MockScheduler : public android::Scheduler {
    public:
        MockScheduler(const scheduler::RefreshRateConfigs& refreshRateConfigs,
                      std::unique_ptr<EventThread> eventThread)
              : Scheduler([](bool) {}, refreshRateConfigs), mEventThread(std::move(eventThread)) {}

        std::unique_ptr<EventThread> makeEventThread(
                const char* /* connectionName */, DispSync* /* dispSync */,
                nsecs_t /* phaseOffsetNs */, nsecs_t /* offsetThresholdForNextVsync */,
                impl::EventThread::InterceptVSyncsCallback /* interceptCallback */) override {
            return std::move(mEventThread);
        }

        MockScheduler() = default;
        ~MockScheduler() override = default;

        std::unique_ptr<EventThread> mEventThread;
    };

    SchedulerTest();
    ~SchedulerTest() override;

    sp<Scheduler::ConnectionHandle> mConnectionHandle;
    mock::EventThread* mEventThread;
    std::unique_ptr<MockScheduler> mScheduler;
    sp<MockEventThreadConnection> mEventThreadConnection;
};

SchedulerTest::SchedulerTest() {
    const ::testing::TestInfo* const test_info =
            ::testing::UnitTest::GetInstance()->current_test_info();
    ALOGD("**** Setting up for %s.%s\n", test_info->test_case_name(), test_info->name());

    std::unique_ptr<mock::EventThread> eventThread = std::make_unique<mock::EventThread>();
    mEventThread = eventThread.get();
    mScheduler = std::make_unique<MockScheduler>(mRefreshRateConfigs, std::move(eventThread));
    EXPECT_CALL(*mEventThread, registerDisplayEventConnection(_)).WillOnce(Return(0));

    mEventThreadConnection = new MockEventThreadConnection(mEventThread);

    // createConnection call to scheduler makes a createEventConnection call to EventThread. Make
    // sure that call gets executed and returns an EventThread::Connection object.
    EXPECT_CALL(*mEventThread, createEventConnection(_, _))
            .WillRepeatedly(Return(mEventThreadConnection));

    mConnectionHandle = mScheduler->createConnection("appConnection", 16, 16, ResyncCallback(),
                                                     impl::EventThread::InterceptVSyncsCallback());
    EXPECT_TRUE(mConnectionHandle != nullptr);
}

SchedulerTest::~SchedulerTest() {
    const ::testing::TestInfo* const test_info =
            ::testing::UnitTest::GetInstance()->current_test_info();
    ALOGD("**** Tearing down after %s.%s\n", test_info->test_case_name(), test_info->name());
}

namespace {
/* ------------------------------------------------------------------------
 * Test cases
 */

TEST_F(SchedulerTest, testNullPtr) {
    // Passing a null pointer for ConnectionHandle is a valid argument. The code doesn't throw any
    // exceptions, just gracefully continues.
    sp<IDisplayEventConnection> returnedValue;
    ASSERT_NO_FATAL_FAILURE(
            returnedValue =
                    mScheduler->createDisplayEventConnection(nullptr, ResyncCallback(),
                                                             ISurfaceComposer::
                                                                     eConfigChangedSuppress));
    EXPECT_TRUE(returnedValue == nullptr);
    EXPECT_TRUE(mScheduler->getEventThread(nullptr) == nullptr);
    EXPECT_TRUE(mScheduler->getEventConnection(nullptr) == nullptr);
    ASSERT_NO_FATAL_FAILURE(mScheduler->hotplugReceived(nullptr, PHYSICAL_DISPLAY_ID, false));
    ASSERT_NO_FATAL_FAILURE(mScheduler->onScreenAcquired(nullptr));
    ASSERT_NO_FATAL_FAILURE(mScheduler->onScreenReleased(nullptr));
    std::string testString;
    ASSERT_NO_FATAL_FAILURE(mScheduler->dump(nullptr, testString));
    EXPECT_TRUE(testString == "");
    ASSERT_NO_FATAL_FAILURE(mScheduler->setPhaseOffset(nullptr, 10));
}

TEST_F(SchedulerTest, invalidConnectionHandle) {
    // Passing an invalid ConnectionHandle is a valid argument. The code doesn't throw any
    // exceptions, just gracefully continues.
    sp<Scheduler::ConnectionHandle> connectionHandle = new Scheduler::ConnectionHandle(20);

    sp<IDisplayEventConnection> returnedValue;
    ASSERT_NO_FATAL_FAILURE(
            returnedValue =
                    mScheduler->createDisplayEventConnection(connectionHandle, ResyncCallback(),
                                                             ISurfaceComposer::
                                                                     eConfigChangedSuppress));
    EXPECT_TRUE(returnedValue == nullptr);
    EXPECT_TRUE(mScheduler->getEventThread(connectionHandle) == nullptr);
    EXPECT_TRUE(mScheduler->getEventConnection(connectionHandle) == nullptr);

    // The EXPECT_CALLS make sure we don't call the functions on the subsequent event threads.
    EXPECT_CALL(*mEventThread, onHotplugReceived(_, _)).Times(0);
    ASSERT_NO_FATAL_FAILURE(
            mScheduler->hotplugReceived(connectionHandle, PHYSICAL_DISPLAY_ID, false));

    EXPECT_CALL(*mEventThread, onScreenAcquired()).Times(0);
    ASSERT_NO_FATAL_FAILURE(mScheduler->onScreenAcquired(connectionHandle));

    EXPECT_CALL(*mEventThread, onScreenReleased()).Times(0);
    ASSERT_NO_FATAL_FAILURE(mScheduler->onScreenReleased(connectionHandle));

    std::string testString;
    EXPECT_CALL(*mEventThread, dump(_)).Times(0);
    ASSERT_NO_FATAL_FAILURE(mScheduler->dump(connectionHandle, testString));
    EXPECT_TRUE(testString == "");

    EXPECT_CALL(*mEventThread, setPhaseOffset(_)).Times(0);
    ASSERT_NO_FATAL_FAILURE(mScheduler->setPhaseOffset(connectionHandle, 10));
}

TEST_F(SchedulerTest, validConnectionHandle) {
    sp<IDisplayEventConnection> returnedValue;
    ASSERT_NO_FATAL_FAILURE(
            returnedValue =
                    mScheduler->createDisplayEventConnection(mConnectionHandle, ResyncCallback(),
                                                             ISurfaceComposer::
                                                                     eConfigChangedSuppress));
    EXPECT_TRUE(returnedValue != nullptr);
    ASSERT_EQ(returnedValue, mEventThreadConnection);

    EXPECT_TRUE(mScheduler->getEventThread(mConnectionHandle) != nullptr);
    EXPECT_TRUE(mScheduler->getEventConnection(mConnectionHandle) != nullptr);

    EXPECT_CALL(*mEventThread, onHotplugReceived(PHYSICAL_DISPLAY_ID, false)).Times(1);
    ASSERT_NO_FATAL_FAILURE(
            mScheduler->hotplugReceived(mConnectionHandle, PHYSICAL_DISPLAY_ID, false));

    EXPECT_CALL(*mEventThread, onScreenAcquired()).Times(1);
    ASSERT_NO_FATAL_FAILURE(mScheduler->onScreenAcquired(mConnectionHandle));

    EXPECT_CALL(*mEventThread, onScreenReleased()).Times(1);
    ASSERT_NO_FATAL_FAILURE(mScheduler->onScreenReleased(mConnectionHandle));

    std::string testString("dump");
    EXPECT_CALL(*mEventThread, dump(testString)).Times(1);
    ASSERT_NO_FATAL_FAILURE(mScheduler->dump(mConnectionHandle, testString));
    EXPECT_TRUE(testString != "");

    EXPECT_CALL(*mEventThread, setPhaseOffset(10)).Times(1);
    ASSERT_NO_FATAL_FAILURE(mScheduler->setPhaseOffset(mConnectionHandle, 10));
}
} // namespace
} // namespace android
