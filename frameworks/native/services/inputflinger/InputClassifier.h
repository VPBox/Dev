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

#ifndef _UI_INPUT_CLASSIFIER_H
#define _UI_INPUT_CLASSIFIER_H

#include <android-base/thread_annotations.h>
#include <utils/RefBase.h>
#include <unordered_map>
#include <thread>

#include "BlockingQueue.h"
#include "InputListener.h"
#include <android/hardware/input/classifier/1.0/IInputClassifier.h>

namespace android {

enum class ClassifierEventType : uint8_t {
    MOTION = 0,
    DEVICE_RESET = 1,
    HAL_RESET = 2,
    EXIT = 3,
};

struct ClassifierEvent {
    ClassifierEventType type;
    std::unique_ptr<NotifyArgs> args;

    ClassifierEvent(ClassifierEventType type, std::unique_ptr<NotifyArgs> args);
    ClassifierEvent(std::unique_ptr<NotifyMotionArgs> args);
    ClassifierEvent(std::unique_ptr<NotifyDeviceResetArgs> args);
    ClassifierEvent(ClassifierEvent&& other);
    ClassifierEvent& operator=(ClassifierEvent&& other);

    // Convenience function to create a HAL_RESET event
    static ClassifierEvent createHalResetEvent();
    // Convenience function to create an EXIT event
    static ClassifierEvent createExitEvent();

    std::optional<int32_t> getDeviceId() const;
};

// --- Interfaces ---

/**
 * Interface for adding a MotionClassification to NotifyMotionArgs.
 *
 * To implement, override the classify function.
 */
class MotionClassifierInterface {
public:
    MotionClassifierInterface() { }
    virtual ~MotionClassifierInterface() { }
    /**
     * Based on the motion event described by NotifyMotionArgs,
     * provide a MotionClassification for the current gesture.
     */
    virtual MotionClassification classify(const NotifyMotionArgs& args) = 0;
    /**
     * Reset all internal HAL state.
     */
    virtual void reset() = 0;
    /**
     * Reset HAL state for a specific device.
     */
    virtual void reset(const NotifyDeviceResetArgs& args) = 0;

    /**
     * Dump the state of the motion classifier
     */
    virtual void dump(std::string& dump) = 0;
};

/**
 * Base interface for an InputListener stage.
 * Provides classification to events.
 */
class InputClassifierInterface : public virtual RefBase, public InputListenerInterface {
public:
    /**
     * Dump the state of the input classifier.
     * This method may be called on any thread (usually by the input manager).
     */
    virtual void dump(std::string& dump) = 0;
protected:
    InputClassifierInterface() { }
    virtual ~InputClassifierInterface() { }
};

// --- Implementations ---

/**
 * Implementation of MotionClassifierInterface that calls the InputClassifier HAL
 * in order to determine the classification for the current gesture.
 *
 * The InputClassifier HAL may keep track of the entire gesture in order to determine
 * the classification, and may be hardware-specific. It may use the data in
 * NotifyMotionArgs::videoFrames field to drive the classification decisions.
 * The HAL is called from a separate thread.
 */
class MotionClassifier final : public MotionClassifierInterface {
public:
    /**
     * The deathRecipient will be subscribed to the HAL death. If the death recipient
     * owns MotionClassifier and receives HAL death, it should delete its copy of it.
     * The callback serviceDied will also be sent if the MotionClassifier itself fails
     * to initialize. If the MotionClassifier fails to initialize, it is not useful, and
     * should be deleted.
     * If no death recipient is supplied, then the registration step will be skipped, so there will
     * be no listeners registered for the HAL death. This is useful for testing
     * MotionClassifier in isolation.
     */
    explicit MotionClassifier(sp<android::hardware::hidl_death_recipient> deathRecipient = nullptr);
    ~MotionClassifier();

    /**
     * Classifies events asynchronously; that is, it doesn't block events on a classification,
     * but instead sends them over to the classifier HAL and after a classification is
     * determined, it then marks the next event it sees in the stream with it.
     *
     * Therefore, it is acceptable to have the classifications be delayed by 1-2 events
     * in a particular gesture.
     */
    virtual MotionClassification classify(const NotifyMotionArgs& args) override;
    virtual void reset() override;
    virtual void reset(const NotifyDeviceResetArgs& args) override;

    virtual void dump(std::string& dump) override;

private:
    /**
     * Initialize MotionClassifier.
     * Return true if initializaion is successful.
     */
    bool init();
    /**
     * Entity that will be notified of the HAL death (most likely InputClassifier).
     */
    wp<android::hardware::hidl_death_recipient> mDeathRecipient;

    // The events that need to be sent to the HAL.
    BlockingQueue<ClassifierEvent> mEvents;
    /**
     * Add an event to the queue mEvents.
     */
    void enqueueEvent(ClassifierEvent&& event);
    /**
     * Thread that will communicate with InputClassifier HAL.
     * This should be the only thread that communicates with InputClassifier HAL,
     * because this thread is allowed to block on the HAL calls.
     */
    std::thread mHalThread;
    /**
     * Print an error message if the caller is not on the InputClassifier thread.
     * Caller must supply the name of the calling function as __func__
     */
    void ensureHalThread(const char* function);
    /**
     * Call the InputClassifier HAL
     */
    void callInputClassifierHal();
    /**
     * Access to the InputClassifier HAL. May be null if init() hasn't completed yet.
     * When init() successfully completes, mService is guaranteed to remain non-null and to not
     * change its value until MotionClassifier is destroyed.
     * This variable is *not* guarded by mLock in the InputClassifier thread, because
     * that thread knows exactly when this variable is initialized.
     * When accessed in any other thread, mService is checked for nullness with a lock.
     */
    sp<android::hardware::input::classifier::V1_0::IInputClassifier> mService;
    std::mutex mLock;
    /**
     * Per-device input classifications. Should only be accessed using the
     * getClassification / setClassification methods.
     */
    std::unordered_map<int32_t /*deviceId*/, MotionClassification>
            mClassifications GUARDED_BY(mLock);
    /**
     * Set the current classification for a given device.
     */
    void setClassification(int32_t deviceId, MotionClassification classification);
    /**
     * Get the current classification for a given device.
     */
    MotionClassification getClassification(int32_t deviceId);
    void updateClassification(int32_t deviceId, nsecs_t eventTime,
            MotionClassification classification);
    /**
     * Clear all current classifications
     */
    void clearClassifications();
    /**
     * Per-device times when the last ACTION_DOWN was received.
     * Used to reject late classifications that do not belong to the current gesture.
     *
     * Accessed indirectly by both InputClassifier thread and the thread that receives notifyMotion.
     */
    std::unordered_map<int32_t /*deviceId*/, nsecs_t /*downTime*/> mLastDownTimes GUARDED_BY(mLock);

    void updateLastDownTime(int32_t deviceId, nsecs_t downTime);

    /**
     * Exit the InputClassifier HAL thread.
     * Useful for tests to ensure proper cleanup.
     */
    void requestExit();
    /**
     * Return string status of mService
     */
    const char* getServiceStatus() REQUIRES(mLock);
};


/**
 * Implementation of the InputClassifierInterface.
 * Represents a separate stage of input processing. All of the input events go through this stage.
 * Acts as a passthrough for all input events except for motion events.
 * The events of motion type are sent to MotionClassifier.
 */
class InputClassifier : public InputClassifierInterface,
        public android::hardware::hidl_death_recipient {
public:
    explicit InputClassifier(const sp<InputListenerInterface>& listener);
    // Some of the constructor logic is finished in onFirstRef
    virtual void onFirstRef() override;

    virtual void notifyConfigurationChanged(const NotifyConfigurationChangedArgs* args) override;
    virtual void notifyKey(const NotifyKeyArgs* args) override;
    virtual void notifyMotion(const NotifyMotionArgs* args) override;
    virtual void notifySwitch(const NotifySwitchArgs* args) override;
    virtual void notifyDeviceReset(const NotifyDeviceResetArgs* args) override;

    virtual void serviceDied(uint64_t cookie,
            const wp<android::hidl::base::V1_0::IBase>& who) override;

    virtual void dump(std::string& dump) override;

private:
    // Protect access to mMotionClassifier, since it may become null via a hidl callback
    std::mutex mLock;
    std::unique_ptr<MotionClassifierInterface> mMotionClassifier GUARDED_BY(mLock);
    // The next stage to pass input events to
    sp<InputListenerInterface> mListener;
};

} // namespace android
#endif
