/*
 * Copyright (C) 2019 The Android Open Source Project
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

#define LOG_TAG "InputClassifier"

#include "InputClassifier.h"
#include "InputClassifierConverter.h"

#include <algorithm>
#include <android-base/stringprintf.h>
#include <cmath>
#include <inttypes.h>
#include <log/log.h>
#if defined(__linux__)
    #include <pthread.h>
#endif
#include <server_configurable_flags/get_flags.h>
#include <unordered_set>

#include <android/hardware/input/classifier/1.0/IInputClassifier.h>

#define INDENT1 "  "
#define INDENT2 "    "
#define INDENT3 "      "
#define INDENT4 "        "
#define INDENT5 "          "

using android::base::StringPrintf;
using android::hardware::hidl_bitfield;
using android::hardware::hidl_vec;
using android::hardware::Return;
using namespace android::hardware::input;

namespace android {

static constexpr bool DEBUG = false;

// Category (=namespace) name for the input settings that are applied at boot time
static const char* INPUT_NATIVE_BOOT = "input_native_boot";
// Feature flag name for the deep press feature
static const char* DEEP_PRESS_ENABLED = "deep_press_enabled";

//Max number of elements to store in mEvents.
static constexpr size_t MAX_EVENTS = 5;

template<class K, class V>
static V getValueForKey(const std::unordered_map<K, V>& map, K key, V defaultValue) {
    auto it = map.find(key);
    if (it == map.end()) {
        return defaultValue;
    }
    return it->second;
}

static MotionClassification getMotionClassification(common::V1_0::Classification classification) {
    static_assert(MotionClassification::NONE ==
            static_cast<MotionClassification>(common::V1_0::Classification::NONE));
    static_assert(MotionClassification::AMBIGUOUS_GESTURE ==
            static_cast<MotionClassification>(common::V1_0::Classification::AMBIGUOUS_GESTURE));
    static_assert(MotionClassification::DEEP_PRESS ==
            static_cast<MotionClassification>(common::V1_0::Classification::DEEP_PRESS));
    return static_cast<MotionClassification>(classification);
}

static bool isTouchEvent(const NotifyMotionArgs& args) {
    return args.source == AINPUT_SOURCE_TOUCHPAD || args.source == AINPUT_SOURCE_TOUCHSCREEN;
}

// Check if the "deep touch" feature is on.
static bool deepPressEnabled() {
    std::string flag_value = server_configurable_flags::GetServerConfigurableFlag(
            INPUT_NATIVE_BOOT, DEEP_PRESS_ENABLED, "true");
    std::transform(flag_value.begin(), flag_value.end(), flag_value.begin(), ::tolower);
    if (flag_value == "1" || flag_value == "true") {
        ALOGI("Deep press feature enabled.");
        return true;
    }
    ALOGI("Deep press feature is not enabled.");
    return false;
}


// --- ClassifierEvent ---

ClassifierEvent::ClassifierEvent(std::unique_ptr<NotifyMotionArgs> args) :
        type(ClassifierEventType::MOTION), args(std::move(args)) { };
ClassifierEvent::ClassifierEvent(std::unique_ptr<NotifyDeviceResetArgs> args) :
        type(ClassifierEventType::DEVICE_RESET), args(std::move(args)) { };
ClassifierEvent::ClassifierEvent(ClassifierEventType type, std::unique_ptr<NotifyArgs> args) :
        type(type), args(std::move(args)) { };

ClassifierEvent::ClassifierEvent(ClassifierEvent&& other) :
        type(other.type), args(std::move(other.args)) { };

ClassifierEvent& ClassifierEvent::operator=(ClassifierEvent&& other) {
    type = other.type;
    args = std::move(other.args);
    return *this;
}

ClassifierEvent ClassifierEvent::createHalResetEvent() {
    return ClassifierEvent(ClassifierEventType::HAL_RESET, nullptr);
}

ClassifierEvent ClassifierEvent::createExitEvent() {
    return ClassifierEvent(ClassifierEventType::EXIT, nullptr);
}

std::optional<int32_t> ClassifierEvent::getDeviceId() const {
    switch (type) {
        case ClassifierEventType::MOTION: {
            NotifyMotionArgs* motionArgs = static_cast<NotifyMotionArgs*>(args.get());
            return motionArgs->deviceId;
        }
        case ClassifierEventType::DEVICE_RESET: {
            NotifyDeviceResetArgs* deviceResetArgs =
                    static_cast<NotifyDeviceResetArgs*>(args.get());
            return deviceResetArgs->deviceId;
        }
        case ClassifierEventType::HAL_RESET: {
            return std::nullopt;
        }
        case ClassifierEventType::EXIT: {
            return std::nullopt;
        }
    }
}

// --- MotionClassifier ---

MotionClassifier::MotionClassifier(sp<android::hardware::hidl_death_recipient> deathRecipient) :
        mDeathRecipient(deathRecipient), mEvents(MAX_EVENTS) {
    mHalThread = std::thread(&MotionClassifier::callInputClassifierHal, this);
#if defined(__linux__)
    // Set the thread name for debugging
    pthread_setname_np(mHalThread.native_handle(), "InputClassifier");
#endif
}

/**
 * This function may block for some time to initialize the HAL, so it should only be called
 * from the "InputClassifier HAL" thread.
 */
bool MotionClassifier::init() {
    ensureHalThread(__func__);
    sp<android::hardware::input::classifier::V1_0::IInputClassifier> service =
            classifier::V1_0::IInputClassifier::getService();
    if (!service) {
        // Not really an error, maybe the device does not have this HAL,
        // but somehow the feature flag is flipped
        ALOGI("Could not obtain InputClassifier HAL");
        return false;
    }

    sp<android::hardware::hidl_death_recipient> recipient = mDeathRecipient.promote();
    if (recipient != nullptr) {
        const bool linked = service->linkToDeath(recipient, 0 /* cookie */).withDefault(false);
        if (!linked) {
            ALOGE("Could not link MotionClassifier to the HAL death");
            return false;
        }
    }

    // Under normal operation, we do not need to reset the HAL here. But in the case where system
    // crashed, but HAL didn't, we may be connecting to an existing HAL process that might already
    // have received events in the past. That means, that HAL could be in an inconsistent state
    // once it receives events from the newly created MotionClassifier.
    mEvents.push(ClassifierEvent::createHalResetEvent());

    {
        std::scoped_lock lock(mLock);
        if (mService) {
            ALOGE("MotionClassifier::%s should only be called once", __func__);
        }
        mService = service;
    }
    return true;
}

MotionClassifier::~MotionClassifier() {
    requestExit();
    mHalThread.join();
}

void MotionClassifier::ensureHalThread(const char* function) {
    if (DEBUG) {
        if (std::this_thread::get_id() != mHalThread.get_id()) {
            LOG_FATAL("Function %s should only be called from InputClassifier thread", function);
        }
    }
}

/**
 * Obtain the classification from the HAL for a given MotionEvent.
 * Should only be called from the InputClassifier thread (mHalThread).
 * Should not be called from the thread that notifyMotion runs on.
 *
 * There is no way to provide a timeout for a HAL call. So if the HAL takes too long
 * to return a classification, this would directly impact the touch latency.
 * To remove any possibility of negatively affecting the touch latency, the HAL
 * is called from a dedicated thread.
 */
void MotionClassifier::callInputClassifierHal() {
    ensureHalThread(__func__);
    const bool initialized = init();
    if (!initialized) {
        // MotionClassifier no longer useful.
        // Deliver death notification from a separate thread
        // because ~MotionClassifier may be invoked, which calls mHalThread.join()
        std::thread([deathRecipient = mDeathRecipient](){
                sp<android::hardware::hidl_death_recipient> recipient = deathRecipient.promote();
                if (recipient != nullptr) {
                    recipient->serviceDied(0 /*cookie*/, nullptr);
                }
        }).detach();
        return;
    }
    // From this point on, mService is guaranteed to be non-null.

    while (true) {
        ClassifierEvent event = mEvents.pop();
        bool halResponseOk = true;
        switch (event.type) {
            case ClassifierEventType::MOTION: {
                NotifyMotionArgs* motionArgs = static_cast<NotifyMotionArgs*>(event.args.get());
                common::V1_0::MotionEvent motionEvent =
                        notifyMotionArgsToHalMotionEvent(*motionArgs);
                Return<common::V1_0::Classification> response = mService->classify(motionEvent);
                halResponseOk = response.isOk();
                if (halResponseOk) {
                    common::V1_0::Classification halClassification = response;
                    updateClassification(motionArgs->deviceId, motionArgs->eventTime,
                            getMotionClassification(halClassification));
                }
                break;
            }
            case ClassifierEventType::DEVICE_RESET: {
                const int32_t deviceId = *(event.getDeviceId());
                halResponseOk = mService->resetDevice(deviceId).isOk();
                setClassification(deviceId, MotionClassification::NONE);
                break;
            }
            case ClassifierEventType::HAL_RESET: {
                halResponseOk = mService->reset().isOk();
                clearClassifications();
                break;
            }
            case ClassifierEventType::EXIT: {
                clearClassifications();
                return;
            }
        }
        if (!halResponseOk) {
            ALOGE("Error communicating with InputClassifier HAL. "
                    "Exiting MotionClassifier HAL thread");
            clearClassifications();
            return;
        }
    }
}

void MotionClassifier::enqueueEvent(ClassifierEvent&& event) {
    bool eventAdded = mEvents.push(std::move(event));
    if (!eventAdded) {
        // If the queue is full, suspect the HAL is slow in processing the events.
        ALOGE("Dropped event with eventTime %" PRId64, event.args->eventTime);
        reset();
    }
}

void MotionClassifier::requestExit() {
    reset();
    mEvents.push(ClassifierEvent::createExitEvent());
}

void MotionClassifier::updateClassification(int32_t deviceId, nsecs_t eventTime,
        MotionClassification classification) {
    std::scoped_lock lock(mLock);
    const nsecs_t lastDownTime = getValueForKey(mLastDownTimes, deviceId, static_cast<nsecs_t>(0));
    if (eventTime < lastDownTime) {
        // HAL just finished processing an event that belonged to an earlier gesture,
        // but new gesture is already in progress. Drop this classification.
        ALOGW("Received late classification. Late by at least %" PRId64 " ms.",
                nanoseconds_to_milliseconds(lastDownTime - eventTime));
        return;
    }
    mClassifications[deviceId] = classification;
}

void MotionClassifier::setClassification(int32_t deviceId, MotionClassification classification) {
    std::scoped_lock lock(mLock);
    mClassifications[deviceId] = classification;
}

void MotionClassifier::clearClassifications() {
    std::scoped_lock lock(mLock);
    mClassifications.clear();
}

MotionClassification MotionClassifier::getClassification(int32_t deviceId) {
    std::scoped_lock lock(mLock);
    return getValueForKey(mClassifications, deviceId, MotionClassification::NONE);
}

void MotionClassifier::updateLastDownTime(int32_t deviceId, nsecs_t downTime) {
    std::scoped_lock lock(mLock);
    mLastDownTimes[deviceId] = downTime;
    mClassifications[deviceId] = MotionClassification::NONE;
}

MotionClassification MotionClassifier::classify(const NotifyMotionArgs& args) {
    if ((args.action & AMOTION_EVENT_ACTION_MASK) == AMOTION_EVENT_ACTION_DOWN) {
        updateLastDownTime(args.deviceId, args.downTime);
    }

    ClassifierEvent event(std::make_unique<NotifyMotionArgs>(args));
    enqueueEvent(std::move(event));
    return getClassification(args.deviceId);
}

void MotionClassifier::reset() {
    mEvents.clear();
    mEvents.push(ClassifierEvent::createHalResetEvent());
}

/**
 * Per-device reset. Clear the outstanding events that are going to be sent to HAL.
 * Request InputClassifier thread to call resetDevice for this particular device.
 */
void MotionClassifier::reset(const NotifyDeviceResetArgs& args) {
    int32_t deviceId = args.deviceId;
    // Clear the pending events right away, to avoid unnecessary work done by the HAL.
    mEvents.erase([deviceId](const ClassifierEvent& event) {
            std::optional<int32_t> eventDeviceId = event.getDeviceId();
            return eventDeviceId && (*eventDeviceId == deviceId);
    });
    enqueueEvent(std::make_unique<NotifyDeviceResetArgs>(args));
}

const char* MotionClassifier::getServiceStatus() REQUIRES(mLock) {
    if (!mService) {
        return "null";
    }
    if (mService->ping().isOk()) {
        return "running";
    }
    return "not responding";
}

void MotionClassifier::dump(std::string& dump) {
    std::scoped_lock lock(mLock);
    dump += StringPrintf(INDENT2 "mService status: %s\n", getServiceStatus());
    dump += StringPrintf(INDENT2 "mEvents: %zu element(s) (max=%zu)\n",
            mEvents.size(), MAX_EVENTS);
    dump += INDENT2 "mClassifications, mLastDownTimes:\n";
    dump += INDENT3 "Device Id\tClassification\tLast down time";
    // Combine mClassifications and mLastDownTimes into a single table.
    // Create a superset of device ids.
    std::unordered_set<int32_t> deviceIds;
    std::for_each(mClassifications.begin(), mClassifications.end(),
            [&deviceIds](auto pair){ deviceIds.insert(pair.first); });
    std::for_each(mLastDownTimes.begin(), mLastDownTimes.end(),
            [&deviceIds](auto pair){ deviceIds.insert(pair.first); });
    for(int32_t deviceId : deviceIds) {
        const MotionClassification classification =
                getValueForKey(mClassifications, deviceId, MotionClassification::NONE);
        const nsecs_t downTime = getValueForKey(mLastDownTimes, deviceId, static_cast<nsecs_t>(0));
        dump += StringPrintf("\n" INDENT4 "%" PRId32 "\t%s\t%" PRId64,
                deviceId, motionClassificationToString(classification), downTime);
    }
}


// --- InputClassifier ---

InputClassifier::InputClassifier(const sp<InputListenerInterface>& listener) :
        mListener(listener) {
    // The rest of the initialization is done in onFirstRef, because we need to obtain
    // an sp to 'this' in order to register for HAL death notifications
}

void InputClassifier::onFirstRef() {
    if (!deepPressEnabled()) {
        // If feature is not enabled, MotionClassifier should stay null to avoid unnecessary work.
        // When MotionClassifier is null, InputClassifier will forward all events
        // to the next InputListener, unmodified.
        return;
    }
    std::scoped_lock lock(mLock);
    mMotionClassifier = std::make_unique<MotionClassifier>(this);
}

void InputClassifier::notifyConfigurationChanged(const NotifyConfigurationChangedArgs* args) {
    // pass through
    mListener->notifyConfigurationChanged(args);
}

void InputClassifier::notifyKey(const NotifyKeyArgs* args) {
    // pass through
    mListener->notifyKey(args);
}

void InputClassifier::notifyMotion(const NotifyMotionArgs* args) {
    std::scoped_lock lock(mLock);
    // MotionClassifier is only used for touch events, for now
    const bool sendToMotionClassifier = mMotionClassifier && isTouchEvent(*args);
    if (!sendToMotionClassifier) {
        mListener->notifyMotion(args);
        return;
    }

    NotifyMotionArgs newArgs(*args);
    newArgs.classification = mMotionClassifier->classify(newArgs);
    mListener->notifyMotion(&newArgs);
}

void InputClassifier::notifySwitch(const NotifySwitchArgs* args) {
    // pass through
    mListener->notifySwitch(args);
}

void InputClassifier::notifyDeviceReset(const NotifyDeviceResetArgs* args) {
    std::scoped_lock lock(mLock);
    if (mMotionClassifier) {
        mMotionClassifier->reset(*args);
    }
    // continue to next stage
    mListener->notifyDeviceReset(args);
}

void InputClassifier::serviceDied(uint64_t /*cookie*/,
        const wp<android::hidl::base::V1_0::IBase>& who) {
    std::scoped_lock lock(mLock);
    ALOGE("InputClassifier HAL has died. Setting mMotionClassifier to null");
    mMotionClassifier = nullptr;
    sp<android::hidl::base::V1_0::IBase> service = who.promote();
    if (service) {
        service->unlinkToDeath(this);
    }
}

void InputClassifier::dump(std::string& dump) {
    std::scoped_lock lock(mLock);
    dump += "Input Classifier State:\n";

    dump += INDENT1 "Motion Classifier:\n";
    if (mMotionClassifier) {
        mMotionClassifier->dump(dump);
    } else {
        dump += INDENT2 "<nullptr>";
    }
    dump += "\n";
}

} // namespace android
