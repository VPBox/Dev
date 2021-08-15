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

#include <utils/Errors.h>

#include "AsyncCallRecorder.h"
#include "Scheduler/EventThread.h"

using namespace std::chrono_literals;
using namespace std::placeholders;

using testing::_;
using testing::Invoke;

namespace android {
namespace {

constexpr PhysicalDisplayId INTERNAL_DISPLAY_ID = 111;
constexpr PhysicalDisplayId EXTERNAL_DISPLAY_ID = 222;
constexpr PhysicalDisplayId DISPLAY_ID_64BIT = 0xabcd12349876fedcULL;

class MockVSyncSource : public VSyncSource {
public:
    MOCK_METHOD1(setVSyncEnabled, void(bool));
    MOCK_METHOD1(setCallback, void(VSyncSource::Callback*));
    MOCK_METHOD1(setPhaseOffset, void(nsecs_t));
    MOCK_METHOD1(pauseVsyncCallback, void(bool));
};

} // namespace

class EventThreadTest : public testing::Test {
protected:
    class MockEventThreadConnection : public EventThreadConnection {
    public:
        MockEventThreadConnection(android::impl::EventThread* eventThread,
                                  ResyncCallback&& resyncCallback,
                                  ISurfaceComposer::ConfigChanged configChanged)
              : EventThreadConnection(eventThread, std::move(resyncCallback), configChanged) {}
        MOCK_METHOD1(postEvent, status_t(const DisplayEventReceiver::Event& event));
    };

    using ConnectionEventRecorder =
            AsyncCallRecorderWithCannedReturn<status_t (*)(const DisplayEventReceiver::Event&)>;

    EventThreadTest();
    ~EventThreadTest() override;

    void createThread();
    sp<MockEventThreadConnection> createConnection(ConnectionEventRecorder& recorder,
                                                   ISurfaceComposer::ConfigChanged configChanged);

    void expectVSyncSetEnabledCallReceived(bool expectedState);
    void expectVSyncSetPhaseOffsetCallReceived(nsecs_t expectedPhaseOffset);
    VSyncSource::Callback* expectVSyncSetCallbackCallReceived();
    void expectInterceptCallReceived(nsecs_t expectedTimestamp);
    void expectVsyncEventReceivedByConnection(const char* name,
                                              ConnectionEventRecorder& connectionEventRecorder,
                                              nsecs_t expectedTimestamp, unsigned expectedCount);
    void expectVsyncEventReceivedByConnection(nsecs_t expectedTimestamp, unsigned expectedCount);
    void expectHotplugEventReceivedByConnection(PhysicalDisplayId expectedDisplayId,
                                                bool expectedConnected);
    void expectConfigChangedEventReceivedByConnection(PhysicalDisplayId expectedDisplayId,
                                                      int32_t expectedConfigId);

    AsyncCallRecorder<void (*)(bool)> mVSyncSetEnabledCallRecorder;
    AsyncCallRecorder<void (*)(VSyncSource::Callback*)> mVSyncSetCallbackCallRecorder;
    AsyncCallRecorder<void (*)(nsecs_t)> mVSyncSetPhaseOffsetCallRecorder;
    AsyncCallRecorder<void (*)()> mResyncCallRecorder;
    AsyncCallRecorder<void (*)(nsecs_t)> mInterceptVSyncCallRecorder;
    ConnectionEventRecorder mConnectionEventCallRecorder{0};

    MockVSyncSource mVSyncSource;
    VSyncSource::Callback* mCallback = nullptr;
    std::unique_ptr<android::impl::EventThread> mThread;
    sp<MockEventThreadConnection> mConnection;
};

EventThreadTest::EventThreadTest() {
    const ::testing::TestInfo* const test_info =
            ::testing::UnitTest::GetInstance()->current_test_info();
    ALOGD("**** Setting up for %s.%s\n", test_info->test_case_name(), test_info->name());

    EXPECT_CALL(mVSyncSource, setVSyncEnabled(_))
            .WillRepeatedly(Invoke(mVSyncSetEnabledCallRecorder.getInvocable()));

    EXPECT_CALL(mVSyncSource, setCallback(_))
            .WillRepeatedly(Invoke(mVSyncSetCallbackCallRecorder.getInvocable()));

    EXPECT_CALL(mVSyncSource, setPhaseOffset(_))
            .WillRepeatedly(Invoke(mVSyncSetPhaseOffsetCallRecorder.getInvocable()));

    createThread();
    mConnection = createConnection(mConnectionEventCallRecorder,
                                   ISurfaceComposer::eConfigChangedDispatch);

    // A display must be connected for VSYNC events to be delivered.
    mThread->onHotplugReceived(INTERNAL_DISPLAY_ID, true);
    expectHotplugEventReceivedByConnection(INTERNAL_DISPLAY_ID, true);
}

EventThreadTest::~EventThreadTest() {
    const ::testing::TestInfo* const test_info =
            ::testing::UnitTest::GetInstance()->current_test_info();
    ALOGD("**** Tearing down after %s.%s\n", test_info->test_case_name(), test_info->name());

    // EventThread should unregister itself as VSyncSource callback.
    EXPECT_TRUE(!mVSyncSetCallbackCallRecorder.waitForUnexpectedCall().has_value());
}

void EventThreadTest::createThread() {
    mThread =
            std::make_unique<android::impl::EventThread>(&mVSyncSource,
                                                         mInterceptVSyncCallRecorder.getInvocable(),
                                                         "unit-test-event-thread");

    // EventThread should register itself as VSyncSource callback.
    mCallback = expectVSyncSetCallbackCallReceived();
    ASSERT_TRUE(mCallback);
}

sp<EventThreadTest::MockEventThreadConnection> EventThreadTest::createConnection(
        ConnectionEventRecorder& recorder, ISurfaceComposer::ConfigChanged configChanged) {
    sp<MockEventThreadConnection> connection =
            new MockEventThreadConnection(mThread.get(), mResyncCallRecorder.getInvocable(),
                                          configChanged);
    EXPECT_CALL(*connection, postEvent(_)).WillRepeatedly(Invoke(recorder.getInvocable()));
    return connection;
}

void EventThreadTest::expectVSyncSetEnabledCallReceived(bool expectedState) {
    auto args = mVSyncSetEnabledCallRecorder.waitForCall();
    ASSERT_TRUE(args.has_value());
    EXPECT_EQ(expectedState, std::get<0>(args.value()));
}

void EventThreadTest::expectVSyncSetPhaseOffsetCallReceived(nsecs_t expectedPhaseOffset) {
    auto args = mVSyncSetPhaseOffsetCallRecorder.waitForCall();
    ASSERT_TRUE(args.has_value());
    EXPECT_EQ(expectedPhaseOffset, std::get<0>(args.value()));
}

VSyncSource::Callback* EventThreadTest::expectVSyncSetCallbackCallReceived() {
    auto callbackSet = mVSyncSetCallbackCallRecorder.waitForCall();
    return callbackSet.has_value() ? std::get<0>(callbackSet.value()) : nullptr;
}

void EventThreadTest::expectInterceptCallReceived(nsecs_t expectedTimestamp) {
    auto args = mInterceptVSyncCallRecorder.waitForCall();
    ASSERT_TRUE(args.has_value());
    EXPECT_EQ(expectedTimestamp, std::get<0>(args.value()));
}

void EventThreadTest::expectVsyncEventReceivedByConnection(
        const char* name, ConnectionEventRecorder& connectionEventRecorder,
        nsecs_t expectedTimestamp, unsigned expectedCount) {
    auto args = connectionEventRecorder.waitForCall();
    ASSERT_TRUE(args.has_value()) << name << " did not receive an event for timestamp "
                                  << expectedTimestamp;
    const auto& event = std::get<0>(args.value());
    EXPECT_EQ(DisplayEventReceiver::DISPLAY_EVENT_VSYNC, event.header.type)
            << name << " did not get the correct event for timestamp " << expectedTimestamp;
    EXPECT_EQ(expectedTimestamp, event.header.timestamp)
            << name << " did not get the expected timestamp for timestamp " << expectedTimestamp;
    EXPECT_EQ(expectedCount, event.vsync.count)
            << name << " did not get the expected count for timestamp " << expectedTimestamp;
}

void EventThreadTest::expectVsyncEventReceivedByConnection(nsecs_t expectedTimestamp,
                                                           unsigned expectedCount) {
    expectVsyncEventReceivedByConnection("mConnectionEventCallRecorder",
                                         mConnectionEventCallRecorder, expectedTimestamp,
                                         expectedCount);
}

void EventThreadTest::expectHotplugEventReceivedByConnection(PhysicalDisplayId expectedDisplayId,
                                                             bool expectedConnected) {
    auto args = mConnectionEventCallRecorder.waitForCall();
    ASSERT_TRUE(args.has_value());
    const auto& event = std::get<0>(args.value());
    EXPECT_EQ(DisplayEventReceiver::DISPLAY_EVENT_HOTPLUG, event.header.type);
    EXPECT_EQ(expectedDisplayId, event.header.displayId);
    EXPECT_EQ(expectedConnected, event.hotplug.connected);
}

void EventThreadTest::expectConfigChangedEventReceivedByConnection(
        PhysicalDisplayId expectedDisplayId, int32_t expectedConfigId) {
    auto args = mConnectionEventCallRecorder.waitForCall();
    ASSERT_TRUE(args.has_value());
    const auto& event = std::get<0>(args.value());
    EXPECT_EQ(DisplayEventReceiver::DISPLAY_EVENT_CONFIG_CHANGED, event.header.type);
    EXPECT_EQ(expectedDisplayId, event.header.displayId);
    EXPECT_EQ(expectedConfigId, event.config.configId);
}

namespace {

/* ------------------------------------------------------------------------
 * Test cases
 */

TEST_F(EventThreadTest, canCreateAndDestroyThreadWithNoEventsSent) {
    EXPECT_FALSE(mVSyncSetEnabledCallRecorder.waitForUnexpectedCall().has_value());
    EXPECT_FALSE(mVSyncSetCallbackCallRecorder.waitForCall(0us).has_value());
    EXPECT_FALSE(mVSyncSetPhaseOffsetCallRecorder.waitForCall(0us).has_value());
    EXPECT_FALSE(mResyncCallRecorder.waitForCall(0us).has_value());
    EXPECT_FALSE(mInterceptVSyncCallRecorder.waitForCall(0us).has_value());
    EXPECT_FALSE(mConnectionEventCallRecorder.waitForCall(0us).has_value());
}

TEST_F(EventThreadTest, vsyncRequestIsIgnoredIfDisplayIsDisconnected) {
    mThread->onHotplugReceived(INTERNAL_DISPLAY_ID, false);
    expectHotplugEventReceivedByConnection(INTERNAL_DISPLAY_ID, false);

    // Signal that we want the next vsync event to be posted to the connection.
    mThread->requestNextVsync(mConnection);

    // EventThread should not enable vsync callbacks.
    EXPECT_FALSE(mVSyncSetEnabledCallRecorder.waitForUnexpectedCall().has_value());
}

TEST_F(EventThreadTest, requestNextVsyncPostsASingleVSyncEventToTheConnection) {
    // Signal that we want the next vsync event to be posted to the connection
    mThread->requestNextVsync(mConnection);

    // EventThread should immediately request a resync.
    EXPECT_TRUE(mResyncCallRecorder.waitForCall().has_value());

    // EventThread should enable vsync callbacks.
    expectVSyncSetEnabledCallReceived(true);

    // Use the received callback to signal a first vsync event.
    // The interceptor should receive the event, as well as the connection.
    mCallback->onVSyncEvent(123);
    expectInterceptCallReceived(123);
    expectVsyncEventReceivedByConnection(123, 1u);

    // Use the received callback to signal a second vsync event.
    // The interceptor should receive the event, but the the connection should
    // not as it was only interested in the first.
    mCallback->onVSyncEvent(456);
    expectInterceptCallReceived(456);
    EXPECT_FALSE(mConnectionEventCallRecorder.waitForUnexpectedCall().has_value());

    // EventThread should also detect that at this point that it does not need
    // any more vsync events, and should disable their generation.
    expectVSyncSetEnabledCallReceived(false);
}

TEST_F(EventThreadTest, setVsyncRateZeroPostsNoVSyncEventsToThatConnection) {
    // Create a first connection, register it, and request a vsync rate of zero.
    ConnectionEventRecorder firstConnectionEventRecorder{0};
    sp<MockEventThreadConnection> firstConnection =
            createConnection(firstConnectionEventRecorder,
                             ISurfaceComposer::eConfigChangedSuppress);
    mThread->setVsyncRate(0, firstConnection);

    // By itself, this should not enable vsync events
    EXPECT_FALSE(mVSyncSetEnabledCallRecorder.waitForUnexpectedCall().has_value());
    EXPECT_FALSE(mVSyncSetCallbackCallRecorder.waitForCall(0us).has_value());

    // However if there is another connection which wants events at a nonzero rate.....
    ConnectionEventRecorder secondConnectionEventRecorder{0};
    sp<MockEventThreadConnection> secondConnection =
            createConnection(secondConnectionEventRecorder,
                             ISurfaceComposer::eConfigChangedSuppress);
    mThread->setVsyncRate(1, secondConnection);

    // EventThread should enable vsync callbacks.
    expectVSyncSetEnabledCallReceived(true);

    // Send a vsync event. EventThread should then make a call to the
    // interceptor, and the second connection. The first connection should not
    // get the event.
    mCallback->onVSyncEvent(123);
    expectInterceptCallReceived(123);
    EXPECT_FALSE(firstConnectionEventRecorder.waitForUnexpectedCall().has_value());
    expectVsyncEventReceivedByConnection("secondConnection", secondConnectionEventRecorder, 123,
                                         1u);
}

TEST_F(EventThreadTest, setVsyncRateOnePostsAllEventsToThatConnection) {
    mThread->setVsyncRate(1, mConnection);

    // EventThread should enable vsync callbacks.
    expectVSyncSetEnabledCallReceived(true);

    // Send a vsync event. EventThread should then make a call to the
    // interceptor, and the connection.
    mCallback->onVSyncEvent(123);
    expectInterceptCallReceived(123);
    expectVsyncEventReceivedByConnection(123, 1u);

    // A second event should go to the same places.
    mCallback->onVSyncEvent(456);
    expectInterceptCallReceived(456);
    expectVsyncEventReceivedByConnection(456, 2u);

    // A third event should go to the same places.
    mCallback->onVSyncEvent(789);
    expectInterceptCallReceived(789);
    expectVsyncEventReceivedByConnection(789, 3u);
}

TEST_F(EventThreadTest, setVsyncRateTwoPostsEveryOtherEventToThatConnection) {
    mThread->setVsyncRate(2, mConnection);

    // EventThread should enable vsync callbacks.
    expectVSyncSetEnabledCallReceived(true);

    // The first event will be seen by the interceptor, and not the connection.
    mCallback->onVSyncEvent(123);
    expectInterceptCallReceived(123);
    EXPECT_FALSE(mConnectionEventCallRecorder.waitForUnexpectedCall().has_value());

    // The second event will be seen by the interceptor and the connection.
    mCallback->onVSyncEvent(456);
    expectInterceptCallReceived(456);
    expectVsyncEventReceivedByConnection(456, 2u);

    // The third event will be seen by the interceptor, and not the connection.
    mCallback->onVSyncEvent(789);
    expectInterceptCallReceived(789);
    EXPECT_FALSE(mConnectionEventCallRecorder.waitForUnexpectedCall().has_value());

    // The fourth event will be seen by the interceptor and the connection.
    mCallback->onVSyncEvent(101112);
    expectInterceptCallReceived(101112);
    expectVsyncEventReceivedByConnection(101112, 4u);
}

TEST_F(EventThreadTest, connectionsRemovedIfInstanceDestroyed) {
    mThread->setVsyncRate(1, mConnection);

    // EventThread should enable vsync callbacks.
    expectVSyncSetEnabledCallReceived(true);

    // Destroy the only (strong) reference to the connection.
    mConnection = nullptr;

    // The first event will be seen by the interceptor, and not the connection.
    mCallback->onVSyncEvent(123);
    expectInterceptCallReceived(123);
    EXPECT_FALSE(mConnectionEventCallRecorder.waitForUnexpectedCall().has_value());

    // EventThread should disable vsync callbacks
    expectVSyncSetEnabledCallReceived(false);
}

TEST_F(EventThreadTest, connectionsRemovedIfEventDeliveryError) {
    ConnectionEventRecorder errorConnectionEventRecorder{NO_MEMORY};
    sp<MockEventThreadConnection> errorConnection =
            createConnection(errorConnectionEventRecorder,
                             ISurfaceComposer::eConfigChangedSuppress);
    mThread->setVsyncRate(1, errorConnection);

    // EventThread should enable vsync callbacks.
    expectVSyncSetEnabledCallReceived(true);

    // The first event will be seen by the interceptor, and by the connection,
    // which then returns an error.
    mCallback->onVSyncEvent(123);
    expectInterceptCallReceived(123);
    expectVsyncEventReceivedByConnection("errorConnection", errorConnectionEventRecorder, 123, 1u);

    // A subsequent event will be seen by the interceptor and not by the
    // connection.
    mCallback->onVSyncEvent(456);
    expectInterceptCallReceived(456);
    EXPECT_FALSE(errorConnectionEventRecorder.waitForUnexpectedCall().has_value());

    // EventThread should disable vsync callbacks with the second event
    expectVSyncSetEnabledCallReceived(false);
}

TEST_F(EventThreadTest, eventsDroppedIfNonfatalEventDeliveryError) {
    ConnectionEventRecorder errorConnectionEventRecorder{WOULD_BLOCK};
    sp<MockEventThreadConnection> errorConnection =
            createConnection(errorConnectionEventRecorder,
                             ISurfaceComposer::eConfigChangedSuppress);
    mThread->setVsyncRate(1, errorConnection);

    // EventThread should enable vsync callbacks.
    expectVSyncSetEnabledCallReceived(true);

    // The first event will be seen by the interceptor, and by the connection,
    // which then returns an non-fatal error.
    mCallback->onVSyncEvent(123);
    expectInterceptCallReceived(123);
    expectVsyncEventReceivedByConnection("errorConnection", errorConnectionEventRecorder, 123, 1u);

    // A subsequent event will be seen by the interceptor, and by the connection,
    // which still then returns an non-fatal error.
    mCallback->onVSyncEvent(456);
    expectInterceptCallReceived(456);
    expectVsyncEventReceivedByConnection("errorConnection", errorConnectionEventRecorder, 456, 2u);

    // EventThread will not disable vsync callbacks as the errors are non-fatal.
    EXPECT_FALSE(mVSyncSetEnabledCallRecorder.waitForUnexpectedCall().has_value());
}

TEST_F(EventThreadTest, setPhaseOffsetForwardsToVSyncSource) {
    mThread->setPhaseOffset(321);
    expectVSyncSetPhaseOffsetCallReceived(321);
}

TEST_F(EventThreadTest, postHotplugInternalDisconnect) {
    mThread->onHotplugReceived(INTERNAL_DISPLAY_ID, false);
    expectHotplugEventReceivedByConnection(INTERNAL_DISPLAY_ID, false);
}

TEST_F(EventThreadTest, postHotplugInternalConnect) {
    mThread->onHotplugReceived(INTERNAL_DISPLAY_ID, true);
    expectHotplugEventReceivedByConnection(INTERNAL_DISPLAY_ID, true);
}

TEST_F(EventThreadTest, postHotplugExternalDisconnect) {
    mThread->onHotplugReceived(EXTERNAL_DISPLAY_ID, false);
    expectHotplugEventReceivedByConnection(EXTERNAL_DISPLAY_ID, false);
}

TEST_F(EventThreadTest, postHotplugExternalConnect) {
    mThread->onHotplugReceived(EXTERNAL_DISPLAY_ID, true);
    expectHotplugEventReceivedByConnection(EXTERNAL_DISPLAY_ID, true);
}

TEST_F(EventThreadTest, postConfigChangedPrimary) {
    mThread->onConfigChanged(INTERNAL_DISPLAY_ID, 7);
    expectConfigChangedEventReceivedByConnection(INTERNAL_DISPLAY_ID, 7);
}

TEST_F(EventThreadTest, postConfigChangedExternal) {
    mThread->onConfigChanged(EXTERNAL_DISPLAY_ID, 5);
    expectConfigChangedEventReceivedByConnection(EXTERNAL_DISPLAY_ID, 5);
}

TEST_F(EventThreadTest, postConfigChangedPrimary64bit) {
    mThread->onConfigChanged(DISPLAY_ID_64BIT, 7);
    expectConfigChangedEventReceivedByConnection(DISPLAY_ID_64BIT, 7);
}

TEST_F(EventThreadTest, suppressConfigChanged) {
    ConnectionEventRecorder suppressConnectionEventRecorder{0};
    sp<MockEventThreadConnection> suppressConnection =
            createConnection(suppressConnectionEventRecorder,
                             ISurfaceComposer::eConfigChangedSuppress);

    mThread->onConfigChanged(INTERNAL_DISPLAY_ID, 9);
    expectConfigChangedEventReceivedByConnection(INTERNAL_DISPLAY_ID, 9);

    auto args = suppressConnectionEventRecorder.waitForCall();
    ASSERT_FALSE(args.has_value());
}

} // namespace
} // namespace android
