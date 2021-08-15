/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef CHRE_CORE_EVENT_LOOP_MANAGER_H_
#define CHRE_CORE_EVENT_LOOP_MANAGER_H_

#include "chre_api/chre/event.h"
#include "chre/core/event_loop.h"
#include "chre/core/event_loop_common.h"
#include "chre/core/gnss_manager.h"
#include "chre/core/host_comms_manager.h"
#include "chre/core/sensor_request_manager.h"
#include "chre/core/wifi_request_manager.h"
#include "chre/core/wwan_request_manager.h"
#include "chre/platform/memory_manager.h"
#include "chre/platform/mutex.h"
#include "chre/util/fixed_size_vector.h"
#include "chre/util/non_copyable.h"
#include "chre/util/singleton.h"
#include "chre/util/unique_ptr.h"

#ifdef CHRE_AUDIO_SUPPORT_ENABLED
#include "chre/core/audio_request_manager.h"
#endif  // CHRE_AUDIO_SUPPORT_ENABLED

namespace chre {

/**
 * A class that keeps track of all event loops in the system. This class
 * represents the top-level object in CHRE. It will own all resources that are
 * shared by all event loops.
 */
class EventLoopManager : public NonCopyable {
 public:
   /**
    * Validates that a CHRE API is invoked from a valid nanoapp context and
    * returns a pointer to the currently executing nanoapp. This should be
    * called by most CHRE API methods that require accessing details about the
    * event loop or the nanoapp itself. If the current event loop or nanoapp are
    * null, this is an assertion error.
    *
    * @param functionName The name of the CHRE API. This should be __func__.
    * @param eventLoop Optional output parameter, which will be populated with
    *        the EventLoop that is currently executing if this function is
    *        successful
    * @return A pointer to the currently executing nanoapp or null if outside
    *         the context of a nanoapp.
    */
  static Nanoapp *validateChreApiCall(const char *functionName);

  /**
   * Collect debugging information for this CHRE instance. Must only be called
   * from the context of the main CHRE thread.
   *
   * @return Buffer containing debugging information stored in a null-terminated
   *         string allocated on the heap (possibly nullptr if the allocation
   *         failed)
   */
  UniquePtr<char> debugDump();

  /**
   * Leverages the event queue mechanism to schedule a CHRE system callback to
   * be invoked at some point in the future from within the context of the
   * "main" EventLoop. Which EventLoop is considered to be the "main" one is
   * currently not specified, but it is required to be exactly one EventLoop
   * that does not change at runtime.
   *
   * This function is safe to call from any thread.
   *
   * @param type An identifier for the callback, which is passed through to the
   *        callback as a uint16_t, and can also be useful for debugging
   * @param data Arbitrary data to provide to the callback
   * @param callback Function to invoke from within the main CHRE event loop
   */
  void deferCallback(SystemCallbackType type, void *data,
                     SystemCallbackFunction *callback) {
    mEventLoop.postEventOrDie(static_cast<uint16_t>(type), data, callback,
                              kSystemInstanceId);
  }

  /**
   * Schedules a CHRE system callback to be invoked at some point in the future
   * after a specified amount of time, in the context of the "main" CHRE
   * EventLoop.
   *
   * This function is safe to call from any thread.
   *
   * @param type An identifier for the callback, which is passed through to the
   *        callback as a uint16_t, and can also be useful for debugging
   * @param data Arbitrary data to provide to the callback
   * @param callback Function to invoke from within the main CHRE event loop
   * @param delay The delay to postpone posting the event
   * @return TimerHandle of the requested timer.
   *
   * @see deferCallback
   */
  TimerHandle setDelayedCallback(SystemCallbackType type, void *data,
                                 SystemCallbackFunction *callback,
                                 Nanoseconds delay) {
    return mEventLoop.getTimerPool().setSystemTimer(
        delay, callback, type, data);
  }

  /**
   * Cancels a delayed callback previously scheduled by setDelayedCallback.
   *
   * This function is safe to call from any thread.
   *
   * @param timerHandle The TimerHandle returned by setDelayedCallback
   *
   * @return true if the callback was successfully cancelled
   */
  bool cancelDelayedCallback(TimerHandle timerHandle) {
    return mEventLoop.getTimerPool().cancelSystemTimer(timerHandle);
  }

  /**
   * Returns a guaranteed unique instance identifier to associate with a newly
   * constructed nanoapp.
   *
   * @return a unique instance ID
   */
  uint32_t getNextInstanceId();

#ifdef CHRE_AUDIO_SUPPORT_ENABLED
  /**
   * @return A reference to the audio request manager. This allows interacting
   *         with the audio subsystem and manages requests from various
   *         nanoapps.
   */
  AudioRequestManager& getAudioRequestManager() {
    return mAudioRequestManager;
  }
#endif  // CHRE_AUDIO_SUPPORT_ENABLED

  /**
   * @return The event loop managed by this event loop manager.
   */
  EventLoop& getEventLoop() {
    return mEventLoop;
  }

#ifdef CHRE_GNSS_SUPPORT_ENABLED
  /**
   * @return A reference to the GNSS request manager. This allows interacting
   *         with the platform GNSS subsystem and manages requests from various
   *         nanoapps.
   */
  GnssManager& getGnssManager() {
    return mGnssManager;
  }
#endif  // CHRE_GNSS_SUPPORT_ENABLED

  /**
   * @return A reference to the host communications manager that enables
   *         transferring arbitrary data between the host processor and CHRE.
   */
  HostCommsManager& getHostCommsManager() {
    return mHostCommsManager;
  }

  /**
   * @return Returns a reference to the sensor request manager. This allows
   *         interacting with the platform sensors and managing requests from
   *         various nanoapps.
   */
  SensorRequestManager& getSensorRequestManager() {
    return mSensorRequestManager;
  }

#ifdef CHRE_WIFI_SUPPORT_ENABLED
  /**
   * @return Returns a reference to the wifi request manager. This allows
   *         interacting with the platform wifi subsystem and manages the
   *         requests from various nanoapps.
   */
  WifiRequestManager& getWifiRequestManager() {
    return mWifiRequestManager;
  }
#endif  // CHRE_WIFI_SUPPORT_ENABLED

#ifdef CHRE_WWAN_SUPPORT_ENABLED
  /**
   * @return A reference to the WWAN request manager. This allows interacting
   *         with the platform WWAN subsystem and manages requests from various
   *         nanoapps.
   */
  WwanRequestManager& getWwanRequestManager() {
    return mWwanRequestManager;
  }
#endif  // CHRE_WWAN_SUPPORT_ENABLED

  /**
   * @return A reference to the memory manager. This allows central control of
   *         the heap space allocated by nanoapps.
   */
  MemoryManager& getMemoryManager() {
    return mMemoryManager;
  }

  /**
   * Performs second-stage initialization of things that are not necessarily
   * required at construction time but need to be completed prior to executing
   * any nanoapps.
   */
  void lateInit();

 private:
  //! The instance ID that was previously generated by getNextInstanceId()
  uint32_t mLastInstanceId = kSystemInstanceId;

#ifdef CHRE_AUDIO_SUPPORT_ENABLED
  //! The audio request manager handles requests for all nanoapps and manages
  //! the state of the audio subsystem that the runtime subscribes to.
  AudioRequestManager mAudioRequestManager;
#endif

  //! The event loop managed by this event loop manager.
  EventLoop mEventLoop;

#ifdef CHRE_GNSS_SUPPORT_ENABLED
  //! The GnssManager that handles requests for all nanoapps. This manages the
  //! state of the GNSS subsystem that the runtime subscribes to.
  GnssManager mGnssManager;
#endif  // CHRE_GNSS_SUPPORT_ENABLED

  //! Handles communications with the host processor.
  HostCommsManager mHostCommsManager;

  //! The SensorRequestManager that handles requests for all nanoapps. This
  //! manages the state of all sensors that runtime subscribes to.
  SensorRequestManager mSensorRequestManager;

#ifdef CHRE_WIFI_SUPPORT_ENABLED
  //! The WifiRequestManager that handles requests for nanoapps. This manages
  //! the state of the wifi subsystem that the runtime subscribes to.
  WifiRequestManager mWifiRequestManager;
#endif  // CHRE_WIFI_SUPPORT_ENABLED

#ifdef CHRE_WWAN_SUPPORT_ENABLED
  //! The WwanRequestManager that handles requests for nanoapps. This manages
  //! the state of the WWAN subsystem that the runtime subscribes to.
  WwanRequestManager mWwanRequestManager;
#endif  // CHRE_WWAN_SUPPORT_ENABLED

  //! The MemoryManager that handles malloc/free call from nanoapps and also
  //! controls upper limits on the heap allocation amount.
  MemoryManager mMemoryManager;
};

//! Provide an alias to the EventLoopManager singleton.
typedef Singleton<EventLoopManager> EventLoopManagerSingleton;

//! Extern the explicit EventLoopManagerSingleton to force non-inline method
//! calls. This reduces codesize considerably.
extern template class Singleton<EventLoopManager>;

}  // namespace chre

#endif  // CHRE_CORE_EVENT_LOOP_MANAGER_H_
