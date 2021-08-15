/*
 * Copyright (C) 2010 The Android Open Source Project
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

#include "../InputDispatcher.h"

#include <binder/Binder.h>

#include <gtest/gtest.h>
#include <linux/input.h>

namespace android {

// An arbitrary time value.
static const nsecs_t ARBITRARY_TIME = 1234;

// An arbitrary device id.
static const int32_t DEVICE_ID = 1;

// An arbitrary display id.
static const int32_t DISPLAY_ID = ADISPLAY_ID_DEFAULT;

// An arbitrary injector pid / uid pair that has permission to inject events.
static const int32_t INJECTOR_PID = 999;
static const int32_t INJECTOR_UID = 1001;


// --- FakeInputDispatcherPolicy ---

class FakeInputDispatcherPolicy : public InputDispatcherPolicyInterface {
    InputDispatcherConfiguration mConfig;

protected:
    virtual ~FakeInputDispatcherPolicy() {
    }

public:
    FakeInputDispatcherPolicy() {
        mInputEventFiltered = false;
        mTime = -1;
        mAction = -1;
        mDisplayId = -1;
        mOnPointerDownToken.clear();
    }

    void assertFilterInputEventWasCalledWithExpectedArgs(const NotifyMotionArgs* args) {
        ASSERT_TRUE(mInputEventFiltered)
                << "Expected filterInputEvent() to have been called.";

        ASSERT_EQ(mTime, args->eventTime)
                << "Expected time of filtered event was not matched";
        ASSERT_EQ(mAction, args->action)
                << "Expected action of filtered event was not matched";
        ASSERT_EQ(mDisplayId, args->displayId)
                << "Expected displayId of filtered event was not matched";

        reset();
    }

    void assertFilterInputEventWasCalledWithExpectedArgs(const NotifyKeyArgs* args) {
        ASSERT_TRUE(mInputEventFiltered)
                << "Expected filterInputEvent() to have been called.";

        ASSERT_EQ(mTime, args->eventTime)
                << "Expected time of filtered event was not matched";
        ASSERT_EQ(mAction, args->action)
                << "Expected action of filtered event was not matched";
        ASSERT_EQ(mDisplayId, args->displayId)
                << "Expected displayId of filtered event was not matched";

        reset();
    }

    void assertFilterInputEventWasNotCalled() {
        ASSERT_FALSE(mInputEventFiltered)
                << "Expected filterInputEvent() to not have been called.";
    }

    void assertOnPointerDownEquals(const sp<IBinder>& touchedToken) {
        ASSERT_EQ(mOnPointerDownToken, touchedToken)
                << "Expected token from onPointerDownOutsideFocus was not matched";
        reset();
    }

private:
    bool mInputEventFiltered;
    nsecs_t mTime;
    int32_t mAction;
    int32_t mDisplayId;
    sp<IBinder> mOnPointerDownToken;

    virtual void notifyConfigurationChanged(nsecs_t) {
    }

    virtual nsecs_t notifyANR(const sp<InputApplicationHandle>&,
            const sp<IBinder>&,
            const std::string&) {
        return 0;
    }

    virtual void notifyInputChannelBroken(const sp<IBinder>&) {
    }

    virtual void notifyFocusChanged(const sp<IBinder>&, const sp<IBinder>&) {
    }

    virtual void getDispatcherConfiguration(InputDispatcherConfiguration* outConfig) {
        *outConfig = mConfig;
    }

    virtual bool filterInputEvent(const InputEvent* inputEvent, uint32_t policyFlags) {
        switch (inputEvent->getType()) {
            case AINPUT_EVENT_TYPE_KEY: {
                const KeyEvent* keyEvent = static_cast<const KeyEvent*>(inputEvent);
                mTime = keyEvent->getEventTime();
                mAction = keyEvent->getAction();
                mDisplayId = keyEvent->getDisplayId();
                break;
            }

            case AINPUT_EVENT_TYPE_MOTION: {
                const MotionEvent* motionEvent = static_cast<const MotionEvent*>(inputEvent);
                mTime = motionEvent->getEventTime();
                mAction = motionEvent->getAction();
                mDisplayId = motionEvent->getDisplayId();
                break;
            }
        }

        mInputEventFiltered = true;
        return true;
    }

    virtual void interceptKeyBeforeQueueing(const KeyEvent*, uint32_t&) {
    }

    virtual void interceptMotionBeforeQueueing(int32_t, nsecs_t, uint32_t&) {
    }

    virtual nsecs_t interceptKeyBeforeDispatching(const sp<IBinder>&,
            const KeyEvent*, uint32_t) {
        return 0;
    }

    virtual bool dispatchUnhandledKey(const sp<IBinder>&,
            const KeyEvent*, uint32_t, KeyEvent*) {
        return false;
    }

    virtual void notifySwitch(nsecs_t, uint32_t, uint32_t, uint32_t) {
    }

    virtual void pokeUserActivity(nsecs_t, int32_t) {
    }

    virtual bool checkInjectEventsPermissionNonReentrant(int32_t, int32_t) {
        return false;
    }

    virtual void onPointerDownOutsideFocus(const sp<IBinder>& newToken) {
        mOnPointerDownToken = newToken;
    }

    void reset() {
        mInputEventFiltered = false;
        mTime = -1;
        mAction = -1;
        mDisplayId = -1;
        mOnPointerDownToken.clear();
    }
};


// --- InputDispatcherTest ---

class InputDispatcherTest : public testing::Test {
protected:
    sp<FakeInputDispatcherPolicy> mFakePolicy;
    sp<InputDispatcher> mDispatcher;
    sp<InputDispatcherThread> mDispatcherThread;

    virtual void SetUp() {
        mFakePolicy = new FakeInputDispatcherPolicy();
        mDispatcher = new InputDispatcher(mFakePolicy);
        mDispatcher->setInputDispatchMode(/*enabled*/ true, /*frozen*/ false);
        //Start InputDispatcher thread
        mDispatcherThread = new InputDispatcherThread(mDispatcher);
        mDispatcherThread->run("InputDispatcherTest", PRIORITY_URGENT_DISPLAY);
    }

    virtual void TearDown() {
        mDispatcherThread->requestExit();
        mDispatcherThread.clear();
        mFakePolicy.clear();
        mDispatcher.clear();
    }
};


TEST_F(InputDispatcherTest, InjectInputEvent_ValidatesKeyEvents) {
    KeyEvent event;

    // Rejects undefined key actions.
    event.initialize(DEVICE_ID, AINPUT_SOURCE_KEYBOARD, ADISPLAY_ID_NONE,
            /*action*/ -1, 0,
            AKEYCODE_A, KEY_A, AMETA_NONE, 0, ARBITRARY_TIME, ARBITRARY_TIME);
    ASSERT_EQ(INPUT_EVENT_INJECTION_FAILED, mDispatcher->injectInputEvent(
            &event,
            INJECTOR_PID, INJECTOR_UID, INPUT_EVENT_INJECTION_SYNC_NONE, 0, 0))
            << "Should reject key events with undefined action.";

    // Rejects ACTION_MULTIPLE since it is not supported despite being defined in the API.
    event.initialize(DEVICE_ID, AINPUT_SOURCE_KEYBOARD, ADISPLAY_ID_NONE,
            AKEY_EVENT_ACTION_MULTIPLE, 0,
            AKEYCODE_A, KEY_A, AMETA_NONE, 0, ARBITRARY_TIME, ARBITRARY_TIME);
    ASSERT_EQ(INPUT_EVENT_INJECTION_FAILED, mDispatcher->injectInputEvent(
            &event,
            INJECTOR_PID, INJECTOR_UID, INPUT_EVENT_INJECTION_SYNC_NONE, 0, 0))
            << "Should reject key events with ACTION_MULTIPLE.";
}

TEST_F(InputDispatcherTest, InjectInputEvent_ValidatesMotionEvents) {
    MotionEvent event;
    PointerProperties pointerProperties[MAX_POINTERS + 1];
    PointerCoords pointerCoords[MAX_POINTERS + 1];
    for (int i = 0; i <= MAX_POINTERS; i++) {
        pointerProperties[i].clear();
        pointerProperties[i].id = i;
        pointerCoords[i].clear();
    }

    // Some constants commonly used below
    constexpr int32_t source = AINPUT_SOURCE_TOUCHSCREEN;
    constexpr int32_t edgeFlags = AMOTION_EVENT_EDGE_FLAG_NONE;
    constexpr int32_t metaState = AMETA_NONE;
    constexpr MotionClassification classification = MotionClassification::NONE;

    // Rejects undefined motion actions.
    event.initialize(DEVICE_ID, source, DISPLAY_ID,
            /*action*/ -1, 0, 0, edgeFlags, metaState, 0, classification, 0, 0, 0, 0,
            ARBITRARY_TIME, ARBITRARY_TIME, /*pointerCount*/ 1, pointerProperties, pointerCoords);
    ASSERT_EQ(INPUT_EVENT_INJECTION_FAILED, mDispatcher->injectInputEvent(
            &event,
            INJECTOR_PID, INJECTOR_UID, INPUT_EVENT_INJECTION_SYNC_NONE, 0, 0))
            << "Should reject motion events with undefined action.";

    // Rejects pointer down with invalid index.
    event.initialize(DEVICE_ID, source, DISPLAY_ID,
            AMOTION_EVENT_ACTION_POINTER_DOWN | (1 << AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT),
            0, 0, edgeFlags, metaState, 0, classification, 0, 0, 0, 0,
            ARBITRARY_TIME, ARBITRARY_TIME, /*pointerCount*/ 1, pointerProperties, pointerCoords);
    ASSERT_EQ(INPUT_EVENT_INJECTION_FAILED, mDispatcher->injectInputEvent(
            &event,
            INJECTOR_PID, INJECTOR_UID, INPUT_EVENT_INJECTION_SYNC_NONE, 0, 0))
            << "Should reject motion events with pointer down index too large.";

    event.initialize(DEVICE_ID, source, DISPLAY_ID,
            AMOTION_EVENT_ACTION_POINTER_DOWN | (~0U << AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT),
            0, 0, edgeFlags, metaState, 0, classification, 0, 0, 0, 0,
            ARBITRARY_TIME, ARBITRARY_TIME, /*pointerCount*/ 1, pointerProperties, pointerCoords);
    ASSERT_EQ(INPUT_EVENT_INJECTION_FAILED, mDispatcher->injectInputEvent(
            &event,
            INJECTOR_PID, INJECTOR_UID, INPUT_EVENT_INJECTION_SYNC_NONE, 0, 0))
            << "Should reject motion events with pointer down index too small.";

    // Rejects pointer up with invalid index.
    event.initialize(DEVICE_ID, source, DISPLAY_ID,
            AMOTION_EVENT_ACTION_POINTER_UP | (1 << AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT),
            0, 0, edgeFlags, metaState, 0, classification, 0, 0, 0, 0,
            ARBITRARY_TIME, ARBITRARY_TIME, /*pointerCount*/ 1, pointerProperties, pointerCoords);
    ASSERT_EQ(INPUT_EVENT_INJECTION_FAILED, mDispatcher->injectInputEvent(
            &event,
            INJECTOR_PID, INJECTOR_UID, INPUT_EVENT_INJECTION_SYNC_NONE, 0, 0))
            << "Should reject motion events with pointer up index too large.";

    event.initialize(DEVICE_ID, source, DISPLAY_ID,
            AMOTION_EVENT_ACTION_POINTER_UP | (~0U << AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT),
            0, 0, edgeFlags, metaState, 0, classification, 0, 0, 0, 0,
            ARBITRARY_TIME, ARBITRARY_TIME, /*pointerCount*/ 1, pointerProperties, pointerCoords);
    ASSERT_EQ(INPUT_EVENT_INJECTION_FAILED, mDispatcher->injectInputEvent(
            &event,
            INJECTOR_PID, INJECTOR_UID, INPUT_EVENT_INJECTION_SYNC_NONE, 0, 0))
            << "Should reject motion events with pointer up index too small.";

    // Rejects motion events with invalid number of pointers.
    event.initialize(DEVICE_ID, source, DISPLAY_ID,
            AMOTION_EVENT_ACTION_DOWN, 0, 0, edgeFlags, metaState, 0, classification, 0, 0, 0, 0,
            ARBITRARY_TIME, ARBITRARY_TIME, /*pointerCount*/ 0, pointerProperties, pointerCoords);
    ASSERT_EQ(INPUT_EVENT_INJECTION_FAILED, mDispatcher->injectInputEvent(
            &event,
            INJECTOR_PID, INJECTOR_UID, INPUT_EVENT_INJECTION_SYNC_NONE, 0, 0))
            << "Should reject motion events with 0 pointers.";

    event.initialize(DEVICE_ID, source, DISPLAY_ID,
            AMOTION_EVENT_ACTION_DOWN, 0, 0, edgeFlags, metaState, 0, classification, 0, 0, 0, 0,
            ARBITRARY_TIME, ARBITRARY_TIME,
            /*pointerCount*/ MAX_POINTERS + 1, pointerProperties, pointerCoords);
    ASSERT_EQ(INPUT_EVENT_INJECTION_FAILED, mDispatcher->injectInputEvent(
            &event,
            INJECTOR_PID, INJECTOR_UID, INPUT_EVENT_INJECTION_SYNC_NONE, 0, 0))
            << "Should reject motion events with more than MAX_POINTERS pointers.";

    // Rejects motion events with invalid pointer ids.
    pointerProperties[0].id = -1;
    event.initialize(DEVICE_ID, source, DISPLAY_ID,
            AMOTION_EVENT_ACTION_DOWN, 0, 0, edgeFlags, metaState, 0, classification, 0, 0, 0, 0,
            ARBITRARY_TIME, ARBITRARY_TIME, /*pointerCount*/ 1, pointerProperties, pointerCoords);
    ASSERT_EQ(INPUT_EVENT_INJECTION_FAILED, mDispatcher->injectInputEvent(
            &event,
            INJECTOR_PID, INJECTOR_UID, INPUT_EVENT_INJECTION_SYNC_NONE, 0, 0))
            << "Should reject motion events with pointer ids less than 0.";

    pointerProperties[0].id = MAX_POINTER_ID + 1;
    event.initialize(DEVICE_ID, source, DISPLAY_ID,
            AMOTION_EVENT_ACTION_DOWN, 0, 0, edgeFlags, metaState, 0, classification, 0, 0, 0, 0,
            ARBITRARY_TIME, ARBITRARY_TIME, /*pointerCount*/ 1, pointerProperties, pointerCoords);
    ASSERT_EQ(INPUT_EVENT_INJECTION_FAILED, mDispatcher->injectInputEvent(
            &event,
            INJECTOR_PID, INJECTOR_UID, INPUT_EVENT_INJECTION_SYNC_NONE, 0, 0))
            << "Should reject motion events with pointer ids greater than MAX_POINTER_ID.";

    // Rejects motion events with duplicate pointer ids.
    pointerProperties[0].id = 1;
    pointerProperties[1].id = 1;
    event.initialize(DEVICE_ID, source, DISPLAY_ID,
            AMOTION_EVENT_ACTION_DOWN, 0, 0, edgeFlags, metaState, 0, classification, 0, 0, 0, 0,
            ARBITRARY_TIME, ARBITRARY_TIME, /*pointerCount*/ 2, pointerProperties, pointerCoords);
    ASSERT_EQ(INPUT_EVENT_INJECTION_FAILED, mDispatcher->injectInputEvent(
            &event,
            INJECTOR_PID, INJECTOR_UID, INPUT_EVENT_INJECTION_SYNC_NONE, 0, 0))
            << "Should reject motion events with duplicate pointer ids.";
}

// --- InputDispatcherTest SetInputWindowTest ---
static const int32_t INJECT_EVENT_TIMEOUT = 500;
static const int32_t DISPATCHING_TIMEOUT = 100;

class FakeApplicationHandle : public InputApplicationHandle {
public:
    FakeApplicationHandle() {}
    virtual ~FakeApplicationHandle() {}

    virtual bool updateInfo() {
        mInfo.dispatchingTimeout = DISPATCHING_TIMEOUT;
        return true;
    }
};

class FakeInputReceiver {
public:
    void consumeEvent(int32_t expectedEventType, int32_t expectedDisplayId,
            int32_t expectedFlags = 0) {
        uint32_t consumeSeq;
        InputEvent* event;
        status_t status = mConsumer->consume(&mEventFactory, false /*consumeBatches*/, -1,
            &consumeSeq, &event);

        ASSERT_EQ(OK, status)
                << mName.c_str() << ": consumer consume should return OK.";
        ASSERT_TRUE(event != nullptr)
                << mName.c_str() << ": consumer should have returned non-NULL event.";
        ASSERT_EQ(expectedEventType, event->getType())
                << mName.c_str() << ": event type should match.";

        ASSERT_EQ(expectedDisplayId, event->getDisplayId())
                << mName.c_str() << ": event displayId should be the same as expected.";

        int32_t flags;
        switch (expectedEventType) {
            case AINPUT_EVENT_TYPE_KEY: {
                KeyEvent* typedEvent = static_cast<KeyEvent*>(event);
                flags = typedEvent->getFlags();
                break;
            }
            case AINPUT_EVENT_TYPE_MOTION: {
                MotionEvent* typedEvent = static_cast<MotionEvent*>(event);
                flags = typedEvent->getFlags();
                break;
            }
            default: {
                FAIL() << mName.c_str() << ": invalid event type: " << expectedEventType;
            }
        }
        ASSERT_EQ(expectedFlags, flags)
                << mName.c_str() << ": event flags should be the same as expected.";

        status = mConsumer->sendFinishedSignal(consumeSeq, handled());
        ASSERT_EQ(OK, status)
                << mName.c_str() << ": consumer sendFinishedSignal should return OK.";
    }

    void assertNoEvents() {
        uint32_t consumeSeq;
        InputEvent* event;
        status_t status = mConsumer->consume(&mEventFactory, false /*consumeBatches*/, -1,
            &consumeSeq, &event);
        ASSERT_NE(OK, status)
                << mName.c_str()
                << ": should not have received any events, so consume(..) should not return OK.";
    }

protected:
        explicit FakeInputReceiver(const sp<InputDispatcher>& dispatcher,
            const std::string name, int32_t displayId) :
                mDispatcher(dispatcher), mName(name), mDisplayId(displayId) {
            InputChannel::openInputChannelPair(name, mServerChannel, mClientChannel);
            mConsumer = new InputConsumer(mClientChannel);
        }

        virtual ~FakeInputReceiver() {
        }

        // return true if the event has been handled.
        virtual bool handled() {
            return false;
        }

        sp<InputDispatcher> mDispatcher;
        sp<InputChannel> mServerChannel, mClientChannel;
        InputConsumer *mConsumer;
        PreallocatedInputEventFactory mEventFactory;

        std::string mName;
        int32_t mDisplayId;
};

class FakeWindowHandle : public InputWindowHandle, public FakeInputReceiver {
public:
    static const int32_t WIDTH = 600;
    static const int32_t HEIGHT = 800;

    FakeWindowHandle(const sp<InputApplicationHandle>& inputApplicationHandle,
        const sp<InputDispatcher>& dispatcher, const std::string name, int32_t displayId) :
            FakeInputReceiver(dispatcher, name, displayId),
            mFocused(false), mFrame(Rect(0, 0, WIDTH, HEIGHT)), mLayoutParamFlags(0) {
            mServerChannel->setToken(new BBinder());
            mDispatcher->registerInputChannel(mServerChannel, displayId);

            inputApplicationHandle->updateInfo();
            mInfo.applicationInfo = *inputApplicationHandle->getInfo();
    }

    virtual bool updateInfo() {
        mInfo.token = mServerChannel ? mServerChannel->getToken() : nullptr;
        mInfo.name = mName;
        mInfo.layoutParamsFlags = mLayoutParamFlags;
        mInfo.layoutParamsType = InputWindowInfo::TYPE_APPLICATION;
        mInfo.dispatchingTimeout = DISPATCHING_TIMEOUT;
        mInfo.frameLeft = mFrame.left;
        mInfo.frameTop = mFrame.top;
        mInfo.frameRight = mFrame.right;
        mInfo.frameBottom = mFrame.bottom;
        mInfo.globalScaleFactor = 1.0;
        mInfo.addTouchableRegion(mFrame);
        mInfo.visible = true;
        mInfo.canReceiveKeys = true;
        mInfo.hasFocus = mFocused;
        mInfo.hasWallpaper = false;
        mInfo.paused = false;
        mInfo.layer = 0;
        mInfo.ownerPid = INJECTOR_PID;
        mInfo.ownerUid = INJECTOR_UID;
        mInfo.inputFeatures = 0;
        mInfo.displayId = mDisplayId;

        return true;
    }

    void setFocus() {
        mFocused = true;
    }

    void setFrame(const Rect& frame) {
        mFrame.set(frame);
    }

    void setLayoutParamFlags(int32_t flags) {
        mLayoutParamFlags = flags;
    }

    void releaseChannel() {
        mServerChannel.clear();
        InputWindowHandle::releaseChannel();
    }
protected:
    virtual bool handled() {
        return true;
    }

    bool mFocused;
    Rect mFrame;
    int32_t mLayoutParamFlags;
};

static int32_t injectKeyDown(const sp<InputDispatcher>& dispatcher,
        int32_t displayId = ADISPLAY_ID_NONE) {
    KeyEvent event;
    nsecs_t currentTime = systemTime(SYSTEM_TIME_MONOTONIC);

    // Define a valid key down event.
    event.initialize(DEVICE_ID, AINPUT_SOURCE_KEYBOARD, displayId,
            AKEY_EVENT_ACTION_DOWN, /* flags */ 0,
            AKEYCODE_A, KEY_A, AMETA_NONE, /* repeatCount */ 0, currentTime, currentTime);

    // Inject event until dispatch out.
    return dispatcher->injectInputEvent(
            &event,
            INJECTOR_PID, INJECTOR_UID, INPUT_EVENT_INJECTION_SYNC_WAIT_FOR_RESULT,
            INJECT_EVENT_TIMEOUT, POLICY_FLAG_FILTERED | POLICY_FLAG_PASS_TO_USER);
}

static int32_t injectMotionDown(const sp<InputDispatcher>& dispatcher, int32_t source,
        int32_t displayId, int32_t x = 100, int32_t y = 200) {
    MotionEvent event;
    PointerProperties pointerProperties[1];
    PointerCoords pointerCoords[1];

    pointerProperties[0].clear();
    pointerProperties[0].id = 0;
    pointerProperties[0].toolType = AMOTION_EVENT_TOOL_TYPE_FINGER;

    pointerCoords[0].clear();
    pointerCoords[0].setAxisValue(AMOTION_EVENT_AXIS_X, x);
    pointerCoords[0].setAxisValue(AMOTION_EVENT_AXIS_Y, y);

    nsecs_t currentTime = systemTime(SYSTEM_TIME_MONOTONIC);
    // Define a valid motion down event.
    event.initialize(DEVICE_ID, source, displayId,
            AMOTION_EVENT_ACTION_DOWN, /* actionButton */ 0, /* flags */ 0, /* edgeFlags */ 0,
            AMETA_NONE, /* buttonState */ 0, MotionClassification::NONE,
            /* xOffset */ 0, /* yOffset */ 0, /* xPrecision */ 0,
            /* yPrecision */ 0, currentTime, currentTime, /*pointerCount*/ 1, pointerProperties,
            pointerCoords);

    // Inject event until dispatch out.
    return dispatcher->injectInputEvent(
            &event,
            INJECTOR_PID, INJECTOR_UID, INPUT_EVENT_INJECTION_SYNC_WAIT_FOR_RESULT,
            INJECT_EVENT_TIMEOUT, POLICY_FLAG_FILTERED | POLICY_FLAG_PASS_TO_USER);
}

static NotifyKeyArgs generateKeyArgs(int32_t action, int32_t displayId = ADISPLAY_ID_NONE) {
    nsecs_t currentTime = systemTime(SYSTEM_TIME_MONOTONIC);
    // Define a valid key event.
    NotifyKeyArgs args(/* sequenceNum */ 0, currentTime, DEVICE_ID, AINPUT_SOURCE_KEYBOARD,
            displayId, POLICY_FLAG_PASS_TO_USER, action, /* flags */ 0,
            AKEYCODE_A, KEY_A, AMETA_NONE, currentTime);

    return args;
}

static NotifyMotionArgs generateMotionArgs(int32_t action, int32_t source, int32_t displayId) {
    PointerProperties pointerProperties[1];
    PointerCoords pointerCoords[1];

    pointerProperties[0].clear();
    pointerProperties[0].id = 0;
    pointerProperties[0].toolType = AMOTION_EVENT_TOOL_TYPE_FINGER;

    pointerCoords[0].clear();
    pointerCoords[0].setAxisValue(AMOTION_EVENT_AXIS_X, 100);
    pointerCoords[0].setAxisValue(AMOTION_EVENT_AXIS_Y, 200);

    nsecs_t currentTime = systemTime(SYSTEM_TIME_MONOTONIC);
    // Define a valid motion event.
    NotifyMotionArgs args(/* sequenceNum */ 0, currentTime, DEVICE_ID, source, displayId,
            POLICY_FLAG_PASS_TO_USER, action, /* actionButton */ 0, /* flags */ 0,
            AMETA_NONE, /* buttonState */ 0, MotionClassification::NONE,
            AMOTION_EVENT_EDGE_FLAG_NONE, /* deviceTimestamp */ 0, 1, pointerProperties,
            pointerCoords, /* xPrecision */ 0, /* yPrecision */ 0, currentTime,
            /* videoFrames */ {});

    return args;
}

TEST_F(InputDispatcherTest, SetInputWindow_SingleWindowTouch) {
    sp<FakeApplicationHandle> application = new FakeApplicationHandle();
    sp<FakeWindowHandle> window = new FakeWindowHandle(application, mDispatcher, "Fake Window",
            ADISPLAY_ID_DEFAULT);

    std::vector<sp<InputWindowHandle>> inputWindowHandles;
    inputWindowHandles.push_back(window);

    mDispatcher->setInputWindows(inputWindowHandles, ADISPLAY_ID_DEFAULT);
    ASSERT_EQ(INPUT_EVENT_INJECTION_SUCCEEDED, injectMotionDown(mDispatcher,
            AINPUT_SOURCE_TOUCHSCREEN, ADISPLAY_ID_DEFAULT))
            << "Inject motion event should return INPUT_EVENT_INJECTION_SUCCEEDED";

    // Window should receive motion event.
    window->consumeEvent(AINPUT_EVENT_TYPE_MOTION, ADISPLAY_ID_DEFAULT);
}

// The foreground window should receive the first touch down event.
TEST_F(InputDispatcherTest, SetInputWindow_MultiWindowsTouch) {
    sp<FakeApplicationHandle> application = new FakeApplicationHandle();
    sp<FakeWindowHandle> windowTop = new FakeWindowHandle(application, mDispatcher, "Top",
            ADISPLAY_ID_DEFAULT);
    sp<FakeWindowHandle> windowSecond = new FakeWindowHandle(application, mDispatcher, "Second",
            ADISPLAY_ID_DEFAULT);

    std::vector<sp<InputWindowHandle>> inputWindowHandles;
    inputWindowHandles.push_back(windowTop);
    inputWindowHandles.push_back(windowSecond);

    mDispatcher->setInputWindows(inputWindowHandles, ADISPLAY_ID_DEFAULT);
    ASSERT_EQ(INPUT_EVENT_INJECTION_SUCCEEDED, injectMotionDown(mDispatcher,
            AINPUT_SOURCE_TOUCHSCREEN, ADISPLAY_ID_DEFAULT))
            << "Inject motion event should return INPUT_EVENT_INJECTION_SUCCEEDED";

    // Top window should receive the touch down event. Second window should not receive anything.
    windowTop->consumeEvent(AINPUT_EVENT_TYPE_MOTION, ADISPLAY_ID_DEFAULT);
    windowSecond->assertNoEvents();
}

TEST_F(InputDispatcherTest, SetInputWindow_FocusedWindow) {
    sp<FakeApplicationHandle> application = new FakeApplicationHandle();
    sp<FakeWindowHandle> windowTop = new FakeWindowHandle(application, mDispatcher, "Top",
            ADISPLAY_ID_DEFAULT);
    sp<FakeWindowHandle> windowSecond = new FakeWindowHandle(application, mDispatcher, "Second",
            ADISPLAY_ID_DEFAULT);

    // Set focused application.
    mDispatcher->setFocusedApplication(ADISPLAY_ID_DEFAULT, application);

    // Expect one focus window exist in display.
    windowSecond->setFocus();
    std::vector<sp<InputWindowHandle>> inputWindowHandles;
    inputWindowHandles.push_back(windowTop);
    inputWindowHandles.push_back(windowSecond);

    mDispatcher->setInputWindows(inputWindowHandles, ADISPLAY_ID_DEFAULT);
    ASSERT_EQ(INPUT_EVENT_INJECTION_SUCCEEDED, injectKeyDown(mDispatcher))
            << "Inject key event should return INPUT_EVENT_INJECTION_SUCCEEDED";

    // Focused window should receive event.
    windowTop->assertNoEvents();
    windowSecond->consumeEvent(AINPUT_EVENT_TYPE_KEY, ADISPLAY_ID_NONE);
}

TEST_F(InputDispatcherTest, SetInputWindow_FocusPriority) {
    sp<FakeApplicationHandle> application = new FakeApplicationHandle();
    sp<FakeWindowHandle> windowTop = new FakeWindowHandle(application, mDispatcher, "Top",
            ADISPLAY_ID_DEFAULT);
    sp<FakeWindowHandle> windowSecond = new FakeWindowHandle(application, mDispatcher, "Second",
            ADISPLAY_ID_DEFAULT);

    // Set focused application.
    mDispatcher->setFocusedApplication(ADISPLAY_ID_DEFAULT, application);

    // Display has two focused windows. Add them to inputWindowsHandles in z-order (top most first)
    windowTop->setFocus();
    windowSecond->setFocus();
    std::vector<sp<InputWindowHandle>> inputWindowHandles;
    inputWindowHandles.push_back(windowTop);
    inputWindowHandles.push_back(windowSecond);

    mDispatcher->setInputWindows(inputWindowHandles, ADISPLAY_ID_DEFAULT);
    ASSERT_EQ(INPUT_EVENT_INJECTION_SUCCEEDED, injectKeyDown(mDispatcher))
            << "Inject key event should return INPUT_EVENT_INJECTION_SUCCEEDED";

    // Top focused window should receive event.
    windowTop->consumeEvent(AINPUT_EVENT_TYPE_KEY, ADISPLAY_ID_NONE);
    windowSecond->assertNoEvents();
}

TEST_F(InputDispatcherTest, SetInputWindow_InputWindowInfo) {
    sp<FakeApplicationHandle> application = new FakeApplicationHandle();

    sp<FakeWindowHandle> windowTop = new FakeWindowHandle(application, mDispatcher, "Top",
            ADISPLAY_ID_DEFAULT);
    sp<FakeWindowHandle> windowSecond = new FakeWindowHandle(application, mDispatcher, "Second",
            ADISPLAY_ID_DEFAULT);

    // Set focused application.
    mDispatcher->setFocusedApplication(ADISPLAY_ID_DEFAULT, application);

    windowTop->setFocus();
    windowSecond->setFocus();
    std::vector<sp<InputWindowHandle>> inputWindowHandles;
    inputWindowHandles.push_back(windowTop);
    inputWindowHandles.push_back(windowSecond);
    // Release channel for window is no longer valid.
    windowTop->releaseChannel();
    mDispatcher->setInputWindows(inputWindowHandles, ADISPLAY_ID_DEFAULT);

    // Test inject a key down, should dispatch to a valid window.
    ASSERT_EQ(INPUT_EVENT_INJECTION_SUCCEEDED, injectKeyDown(mDispatcher))
            << "Inject key event should return INPUT_EVENT_INJECTION_SUCCEEDED";

    // Top window is invalid, so it should not receive any input event.
    windowTop->assertNoEvents();
    windowSecond->consumeEvent(AINPUT_EVENT_TYPE_KEY, ADISPLAY_ID_NONE);
}

/* Test InputDispatcher for MultiDisplay */
class InputDispatcherFocusOnTwoDisplaysTest : public InputDispatcherTest {
public:
    static constexpr int32_t SECOND_DISPLAY_ID = 1;
    virtual void SetUp() {
        InputDispatcherTest::SetUp();

        application1 = new FakeApplicationHandle();
        windowInPrimary = new FakeWindowHandle(application1, mDispatcher, "D_1",
                ADISPLAY_ID_DEFAULT);
        std::vector<sp<InputWindowHandle>> inputWindowHandles;
        inputWindowHandles.push_back(windowInPrimary);
        // Set focus window for primary display, but focused display would be second one.
        mDispatcher->setFocusedApplication(ADISPLAY_ID_DEFAULT, application1);
        windowInPrimary->setFocus();
        mDispatcher->setInputWindows(inputWindowHandles, ADISPLAY_ID_DEFAULT);

        application2 = new FakeApplicationHandle();
        windowInSecondary = new FakeWindowHandle(application2, mDispatcher, "D_2",
                SECOND_DISPLAY_ID);
        // Set focus to second display window.
        std::vector<sp<InputWindowHandle>> inputWindowHandles_Second;
        inputWindowHandles_Second.push_back(windowInSecondary);
        // Set focus display to second one.
        mDispatcher->setFocusedDisplay(SECOND_DISPLAY_ID);
        // Set focus window for second display.
        mDispatcher->setFocusedApplication(SECOND_DISPLAY_ID, application2);
        windowInSecondary->setFocus();
        mDispatcher->setInputWindows(inputWindowHandles_Second, SECOND_DISPLAY_ID);
    }

    virtual void TearDown() {
        InputDispatcherTest::TearDown();

        application1.clear();
        windowInPrimary.clear();
        application2.clear();
        windowInSecondary.clear();
    }

protected:
    sp<FakeApplicationHandle> application1;
    sp<FakeWindowHandle> windowInPrimary;
    sp<FakeApplicationHandle> application2;
    sp<FakeWindowHandle> windowInSecondary;
};

TEST_F(InputDispatcherFocusOnTwoDisplaysTest, SetInputWindow_MultiDisplayTouch) {
    // Test touch down on primary display.
    ASSERT_EQ(INPUT_EVENT_INJECTION_SUCCEEDED, injectMotionDown(mDispatcher,
            AINPUT_SOURCE_TOUCHSCREEN, ADISPLAY_ID_DEFAULT))
            << "Inject motion event should return INPUT_EVENT_INJECTION_SUCCEEDED";
    windowInPrimary->consumeEvent(AINPUT_EVENT_TYPE_MOTION, ADISPLAY_ID_DEFAULT);
    windowInSecondary->assertNoEvents();

    // Test touch down on second display.
    ASSERT_EQ(INPUT_EVENT_INJECTION_SUCCEEDED, injectMotionDown(mDispatcher,
            AINPUT_SOURCE_TOUCHSCREEN, SECOND_DISPLAY_ID))
            << "Inject motion event should return INPUT_EVENT_INJECTION_SUCCEEDED";
    windowInPrimary->assertNoEvents();
    windowInSecondary->consumeEvent(AINPUT_EVENT_TYPE_MOTION, SECOND_DISPLAY_ID);
}

TEST_F(InputDispatcherFocusOnTwoDisplaysTest, SetInputWindow_MultiDisplayFocus) {
    // Test inject a key down with display id specified.
    ASSERT_EQ(INPUT_EVENT_INJECTION_SUCCEEDED, injectKeyDown(mDispatcher, ADISPLAY_ID_DEFAULT))
            << "Inject key event should return INPUT_EVENT_INJECTION_SUCCEEDED";
    windowInPrimary->consumeEvent(AINPUT_EVENT_TYPE_KEY, ADISPLAY_ID_DEFAULT);
    windowInSecondary->assertNoEvents();

    // Test inject a key down without display id specified.
    ASSERT_EQ(INPUT_EVENT_INJECTION_SUCCEEDED, injectKeyDown(mDispatcher))
            << "Inject key event should return INPUT_EVENT_INJECTION_SUCCEEDED";
    windowInPrimary->assertNoEvents();
    windowInSecondary->consumeEvent(AINPUT_EVENT_TYPE_KEY, ADISPLAY_ID_NONE);

    // Remove secondary display.
    std::vector<sp<InputWindowHandle>> noWindows;
    mDispatcher->setInputWindows(noWindows, SECOND_DISPLAY_ID);

    // Expect old focus should receive a cancel event.
    windowInSecondary->consumeEvent(AINPUT_EVENT_TYPE_KEY, ADISPLAY_ID_NONE,
            AKEY_EVENT_FLAG_CANCELED);

    // Test inject a key down, should timeout because of no target window.
    ASSERT_EQ(INPUT_EVENT_INJECTION_TIMED_OUT, injectKeyDown(mDispatcher))
            << "Inject key event should return INPUT_EVENT_INJECTION_TIMED_OUT";
    windowInPrimary->assertNoEvents();
    windowInSecondary->assertNoEvents();
}

class FakeMonitorReceiver : public FakeInputReceiver, public RefBase {
public:
    FakeMonitorReceiver(const sp<InputDispatcher>& dispatcher, const std::string name,
            int32_t displayId, bool isGestureMonitor = false)
            : FakeInputReceiver(dispatcher, name, displayId) {
        mServerChannel->setToken(new BBinder());
        mDispatcher->registerInputMonitor(mServerChannel, displayId, isGestureMonitor);
    }
};

// Test per-display input monitors for motion event.
TEST_F(InputDispatcherFocusOnTwoDisplaysTest, MonitorMotionEvent_MultiDisplay) {
    sp<FakeMonitorReceiver> monitorInPrimary =
            new FakeMonitorReceiver(mDispatcher, "M_1", ADISPLAY_ID_DEFAULT);
    sp<FakeMonitorReceiver> monitorInSecondary =
            new FakeMonitorReceiver(mDispatcher, "M_2", SECOND_DISPLAY_ID);

    // Test touch down on primary display.
    ASSERT_EQ(INPUT_EVENT_INJECTION_SUCCEEDED, injectMotionDown(mDispatcher,
            AINPUT_SOURCE_TOUCHSCREEN, ADISPLAY_ID_DEFAULT))
            << "Inject motion event should return INPUT_EVENT_INJECTION_SUCCEEDED";
    windowInPrimary->consumeEvent(AINPUT_EVENT_TYPE_MOTION, ADISPLAY_ID_DEFAULT);
    monitorInPrimary->consumeEvent(AINPUT_EVENT_TYPE_MOTION, ADISPLAY_ID_DEFAULT);
    windowInSecondary->assertNoEvents();
    monitorInSecondary->assertNoEvents();

    // Test touch down on second display.
    ASSERT_EQ(INPUT_EVENT_INJECTION_SUCCEEDED, injectMotionDown(mDispatcher,
            AINPUT_SOURCE_TOUCHSCREEN, SECOND_DISPLAY_ID))
            << "Inject motion event should return INPUT_EVENT_INJECTION_SUCCEEDED";
    windowInPrimary->assertNoEvents();
    monitorInPrimary->assertNoEvents();
    windowInSecondary->consumeEvent(AINPUT_EVENT_TYPE_MOTION, SECOND_DISPLAY_ID);
    monitorInSecondary->consumeEvent(AINPUT_EVENT_TYPE_MOTION, SECOND_DISPLAY_ID);

    // Test inject a non-pointer motion event.
    // If specific a display, it will dispatch to the focused window of particular display,
    // or it will dispatch to the focused window of focused display.
    ASSERT_EQ(INPUT_EVENT_INJECTION_SUCCEEDED, injectMotionDown(mDispatcher,
        AINPUT_SOURCE_TRACKBALL, ADISPLAY_ID_NONE))
            << "Inject motion event should return INPUT_EVENT_INJECTION_SUCCEEDED";
    windowInPrimary->assertNoEvents();
    monitorInPrimary->assertNoEvents();
    windowInSecondary->consumeEvent(AINPUT_EVENT_TYPE_MOTION, ADISPLAY_ID_NONE);
    monitorInSecondary->consumeEvent(AINPUT_EVENT_TYPE_MOTION, ADISPLAY_ID_NONE);
}

// Test per-display input monitors for key event.
TEST_F(InputDispatcherFocusOnTwoDisplaysTest, MonitorKeyEvent_MultiDisplay) {
    //Input monitor per display.
    sp<FakeMonitorReceiver> monitorInPrimary =
            new FakeMonitorReceiver(mDispatcher, "M_1", ADISPLAY_ID_DEFAULT);
    sp<FakeMonitorReceiver> monitorInSecondary =
            new FakeMonitorReceiver(mDispatcher, "M_2", SECOND_DISPLAY_ID);

    // Test inject a key down.
    ASSERT_EQ(INPUT_EVENT_INJECTION_SUCCEEDED, injectKeyDown(mDispatcher))
            << "Inject key event should return INPUT_EVENT_INJECTION_SUCCEEDED";
    windowInPrimary->assertNoEvents();
    monitorInPrimary->assertNoEvents();
    windowInSecondary->consumeEvent(AINPUT_EVENT_TYPE_KEY, ADISPLAY_ID_NONE);
    monitorInSecondary->consumeEvent(AINPUT_EVENT_TYPE_KEY, ADISPLAY_ID_NONE);
}

class InputFilterTest : public InputDispatcherTest {
protected:
    static constexpr int32_t SECOND_DISPLAY_ID = 1;

    void testNotifyMotion(int32_t displayId, bool expectToBeFiltered) {
        NotifyMotionArgs motionArgs;

        motionArgs = generateMotionArgs(
                AMOTION_EVENT_ACTION_DOWN, AINPUT_SOURCE_TOUCHSCREEN, displayId);
        mDispatcher->notifyMotion(&motionArgs);
        motionArgs = generateMotionArgs(
                AMOTION_EVENT_ACTION_UP, AINPUT_SOURCE_TOUCHSCREEN, displayId);
        mDispatcher->notifyMotion(&motionArgs);

        if (expectToBeFiltered) {
            mFakePolicy->assertFilterInputEventWasCalledWithExpectedArgs(&motionArgs);
        } else {
            mFakePolicy->assertFilterInputEventWasNotCalled();
        }
    }

    void testNotifyKey(bool expectToBeFiltered) {
        NotifyKeyArgs keyArgs;

        keyArgs = generateKeyArgs(AKEY_EVENT_ACTION_DOWN);
        mDispatcher->notifyKey(&keyArgs);
        keyArgs = generateKeyArgs(AKEY_EVENT_ACTION_UP);
        mDispatcher->notifyKey(&keyArgs);

        if (expectToBeFiltered) {
            mFakePolicy->assertFilterInputEventWasCalledWithExpectedArgs(&keyArgs);
        } else {
            mFakePolicy->assertFilterInputEventWasNotCalled();
        }
    }
};

// Test InputFilter for MotionEvent
TEST_F(InputFilterTest, MotionEvent_InputFilter) {
    // Since the InputFilter is disabled by default, check if touch events aren't filtered.
    testNotifyMotion(ADISPLAY_ID_DEFAULT, /*expectToBeFiltered*/ false);
    testNotifyMotion(SECOND_DISPLAY_ID, /*expectToBeFiltered*/ false);

    // Enable InputFilter
    mDispatcher->setInputFilterEnabled(true);
    // Test touch on both primary and second display, and check if both events are filtered.
    testNotifyMotion(ADISPLAY_ID_DEFAULT, /*expectToBeFiltered*/ true);
    testNotifyMotion(SECOND_DISPLAY_ID, /*expectToBeFiltered*/ true);

    // Disable InputFilter
    mDispatcher->setInputFilterEnabled(false);
    // Test touch on both primary and second display, and check if both events aren't filtered.
    testNotifyMotion(ADISPLAY_ID_DEFAULT, /*expectToBeFiltered*/ false);
    testNotifyMotion(SECOND_DISPLAY_ID, /*expectToBeFiltered*/ false);
}

// Test InputFilter for KeyEvent
TEST_F(InputFilterTest, KeyEvent_InputFilter) {
    // Since the InputFilter is disabled by default, check if key event aren't filtered.
    testNotifyKey(/*expectToBeFiltered*/ false);

    // Enable InputFilter
    mDispatcher->setInputFilterEnabled(true);
    // Send a key event, and check if it is filtered.
    testNotifyKey(/*expectToBeFiltered*/ true);

    // Disable InputFilter
    mDispatcher->setInputFilterEnabled(false);
    // Send a key event, and check if it isn't filtered.
    testNotifyKey(/*expectToBeFiltered*/ false);
}

class InputDispatcherOnPointerDownOutsideFocus : public InputDispatcherTest {
    virtual void SetUp() {
        InputDispatcherTest::SetUp();

        sp<FakeApplicationHandle> application = new FakeApplicationHandle();
        mUnfocusedWindow = new FakeWindowHandle(application, mDispatcher, "Top",
                ADISPLAY_ID_DEFAULT);
        mUnfocusedWindow->setFrame(Rect(0, 0, 30, 30));
        // Adding FLAG_NOT_TOUCH_MODAL to ensure taps outside this window are not sent to this
        // window.
        mUnfocusedWindow->setLayoutParamFlags(InputWindowInfo::FLAG_NOT_TOUCH_MODAL);

        mWindowFocused = new FakeWindowHandle(application, mDispatcher, "Second",
                ADISPLAY_ID_DEFAULT);
        mWindowFocused->setFrame(Rect(50, 50, 100, 100));
        mWindowFocused->setLayoutParamFlags(InputWindowInfo::FLAG_NOT_TOUCH_MODAL);
        mWindowFocusedTouchPoint = 60;

        // Set focused application.
        mDispatcher->setFocusedApplication(ADISPLAY_ID_DEFAULT, application);
        mWindowFocused->setFocus();

        // Expect one focus window exist in display.
        std::vector<sp<InputWindowHandle>> inputWindowHandles;
        inputWindowHandles.push_back(mUnfocusedWindow);
        inputWindowHandles.push_back(mWindowFocused);
        mDispatcher->setInputWindows(inputWindowHandles, ADISPLAY_ID_DEFAULT);
    }

    virtual void TearDown() {
        InputDispatcherTest::TearDown();

        mUnfocusedWindow.clear();
        mWindowFocused.clear();
    }

protected:
    sp<FakeWindowHandle> mUnfocusedWindow;
    sp<FakeWindowHandle> mWindowFocused;
    int32_t mWindowFocusedTouchPoint;
};

// Have two windows, one with focus. Inject MotionEvent with source TOUCHSCREEN and action
// DOWN on the window that doesn't have focus. Ensure the window that didn't have focus received
// the onPointerDownOutsideFocus callback.
TEST_F(InputDispatcherOnPointerDownOutsideFocus, OnPointerDownOutsideFocus_Success) {
    ASSERT_EQ(INPUT_EVENT_INJECTION_SUCCEEDED, injectMotionDown(mDispatcher,
            AINPUT_SOURCE_TOUCHSCREEN, ADISPLAY_ID_DEFAULT, 20, 20))
            << "Inject motion event should return INPUT_EVENT_INJECTION_SUCCEEDED";
    // Call monitor to wait for the command queue to get flushed.
    mDispatcher->monitor();

    mFakePolicy->assertOnPointerDownEquals(mUnfocusedWindow->getToken());
}

// Have two windows, one with focus. Inject MotionEvent with source TRACKBALL and action
// DOWN on the window that doesn't have focus. Ensure no window received the
// onPointerDownOutsideFocus callback.
TEST_F(InputDispatcherOnPointerDownOutsideFocus, OnPointerDownOutsideFocus_NonPointerSource) {
    ASSERT_EQ(INPUT_EVENT_INJECTION_SUCCEEDED, injectMotionDown(mDispatcher,
            AINPUT_SOURCE_TRACKBALL, ADISPLAY_ID_DEFAULT, 20, 20))
            << "Inject motion event should return INPUT_EVENT_INJECTION_SUCCEEDED";
    // Call monitor to wait for the command queue to get flushed.
    mDispatcher->monitor();

    mFakePolicy->assertOnPointerDownEquals(nullptr);
}

// Have two windows, one with focus. Inject KeyEvent with action DOWN on the window that doesn't
// have focus. Ensure no window received the onPointerDownOutsideFocus callback.
TEST_F(InputDispatcherOnPointerDownOutsideFocus, OnPointerDownOutsideFocus_NonMotionFailure) {
    ASSERT_EQ(INPUT_EVENT_INJECTION_SUCCEEDED, injectKeyDown(mDispatcher, ADISPLAY_ID_DEFAULT))
            << "Inject key event should return INPUT_EVENT_INJECTION_SUCCEEDED";
    // Call monitor to wait for the command queue to get flushed.
    mDispatcher->monitor();

    mFakePolicy->assertOnPointerDownEquals(nullptr);
}

// Have two windows, one with focus. Inject MotionEvent with source TOUCHSCREEN and action
// DOWN on the window that already has focus. Ensure no window received the
// onPointerDownOutsideFocus callback.
TEST_F(InputDispatcherOnPointerDownOutsideFocus,
        OnPointerDownOutsideFocus_OnAlreadyFocusedWindow) {
    ASSERT_EQ(INPUT_EVENT_INJECTION_SUCCEEDED, injectMotionDown(mDispatcher,
            AINPUT_SOURCE_TOUCHSCREEN, ADISPLAY_ID_DEFAULT, mWindowFocusedTouchPoint,
            mWindowFocusedTouchPoint))
            << "Inject motion event should return INPUT_EVENT_INJECTION_SUCCEEDED";
    // Call monitor to wait for the command queue to get flushed.
    mDispatcher->monitor();

    mFakePolicy->assertOnPointerDownEquals(nullptr);
}

} // namespace android
