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

#ifndef CHRE_CORE_TIMER_POOL_H_
#define CHRE_CORE_TIMER_POOL_H_

#include "chre_api/chre/re.h"

#include "chre/core/event_loop_common.h"
#include "chre/core/nanoapp.h"
#include "chre/platform/mutex.h"
#include "chre/platform/system_timer.h"
#include "chre/util/non_copyable.h"
#include "chre/util/priority_queue.h"

namespace chre {

/**
 * The type to use when referring to a timer instance.
 *
 * Note that this mirrors the CHRE API definition of a timer handle, so should
 * not be changed without appropriate consideration.
 */
typedef uint32_t TimerHandle;

/**
 * Tracks requests from CHRE apps for timed events.
 */
class TimerPool : public NonCopyable {
 public:
  /**
   * Sets up the timer instance initial conditions.
   */
  TimerPool();

  /**
   * Requests a timer for a nanoapp given a cookie to pass to the nanoapp when
   * the timer event is published.
   *
   * @param nanoapp The nanoapp for which this timer is being requested.
   * @param duration The duration of the timer.
   * @param cookie A cookie to pass to the app when the timer elapses.
   * @param isOneShot false if the timer is expected to auto-reload.
   * @return TimerHandle of the requested timer. Returns CHRE_TIMER_INVALID if
   *         not successful.
   */
  TimerHandle setNanoappTimer(
      const Nanoapp *nanoapp, Nanoseconds duration, const void *cookie,
      bool isOneShot) {
    CHRE_ASSERT(nanoapp != nullptr);
    return setTimer(nanoapp->getInstanceId(), duration, nullptr /* callback */,
                    CHRE_EVENT_TIMER, cookie, isOneShot);
  }

  /**
   * Requests a timer for a system callback. When the timer expires, the
   * specified SystemCallbackFunction will be processed in the context of the
   * main CHRE event loop. Note that it is not immediately invoked when the
   * timer expires. If no system timers are available, this method will fatally
   * error.
   *
   * TODO: Consider adding a way to directly invoke a callback after the delay
   *       rather than posting an event.
   *
   * @param duration The duration to set the timer for.
   * @param callback The callback to invoke when the timer expires.
   * @param callbackType The type of this callback.
   * @param cookie A cookie to pass to this callback.
   * @return TimerHandle of the requested timer.
   */
  TimerHandle setSystemTimer(
      Nanoseconds duration, SystemCallbackFunction *callback,
      SystemCallbackType callbackType, const void *cookie);

  /**
   * Cancels a timer given a handle.
   *
   * @param nanoapp The nanoapp requesting a timer to be cancelled.
   * @param timerHandle The handle for a timer to be cancelled.
   * @return false if the timer handle is invalid or is not owned by the nanoapp
   */
  bool cancelNanoappTimer(const Nanoapp *nanoapp, TimerHandle timerHandle) {
    CHRE_ASSERT(nanoapp != nullptr);
    return cancelTimer(nanoapp->getInstanceId(), timerHandle);
  }

  /**
   * Cancels a timer created by setSystemTimer() given a handle.
   *
   * @param timerHandle The handle for a timer to be cancelled.
   * @return false if the timer handle is invalid or is not owned by the system
   */
  bool cancelSystemTimer(TimerHandle timerHandle) {
    return cancelTimer(kSystemInstanceId, timerHandle);
  }

 private:
  /**
   * Tracks metadata associated with a request for a timed event.
   */
  struct TimerRequest {
    //! The instance ID from which this request was made.
    uint32_t instanceId;

    //! The TimerHandle assigned to this request.
    TimerHandle timerHandle;

    //! The time when the request was made.
    Nanoseconds expirationTime;

    //! The requested duration of the timer.
    Nanoseconds duration;

    //! The callback to invoked after the timer event is posted to CHRE.
    SystemCallbackFunction *callback;

    //! The cookie pointer to be passed as an event to the requesting nanoapp.
    const void *cookie;

    //! The event type to post when the timer expires.
    uint16_t eventType;

    //! Whether or not the request is a one shot or should be rescheduled.
    bool isOneShot;

    /**
     * Provides a greater than comparison of TimerRequests.
     *
     * @param request The other request to compare against.
     * @return Returns true if this request is greater than the provided
     *         request.
     */
    bool operator>(const TimerRequest& request) const;
  };

  //! The queue of outstanding timer requests.
  PriorityQueue<TimerRequest, std::greater<TimerRequest>> mTimerRequests;

  //! The underlying system timer used to schedule delayed callbacks.
  SystemTimer mSystemTimer;

  //! The next timer handle for generateTimerHandleLocked() to return.
  TimerHandle mLastTimerHandle = CHRE_TIMER_INVALID;

  //! Max number of timers that can be requested.
  static constexpr size_t kMaxTimerRequests = 64;

  //! The number of timers that must be available for all nanoapps
  //! (per CHRE API).
  static constexpr size_t kNumReservedNanoappTimers = 32;

  //! Max number of timers that can be allocated for nanoapps. Must be at least
  //! as large as kNumReservedNanoappTimers.
  static constexpr size_t kMaxNanoappTimers = 32;

  static_assert(kMaxNanoappTimers >= kNumReservedNanoappTimers,
                "Max number of nanoapp timers is too small");

  //! Whether or not the timer handle generation logic needs to perform a
  //! search for a vacant timer handle.
  bool mGenerateTimerHandleMustCheckUniqueness = false;

  //! The mutex to lock when using this class.
  Mutex mMutex;

  //! The number of active nanoapp timers.
  size_t mNumNanoappTimers = 0;

  /**
   * Requests a timer given a cookie to pass to the CHRE event loop when the
   * timer event is published.
   *
   * @param instanceId The instance ID of the caller.
   * @param duration The duration of the timer.
   * @param cookie A cookie to pass to the app when the timer elapses.
   * @param isOneShot false if the timer is expected to auto-reload.
   * @return TimerHandle of the requested timer. Returns CHRE_TIMER_INVALID if
   *         not successful.
   */
  TimerHandle setTimer(
      uint32_t instanceId, Nanoseconds duration,
      SystemCallbackFunction *callback, uint16_t eventType,
      const void *cookie, bool isOneShot);

  /**
   * Cancels a timer given a handle.
   *
   * @param instanceId The instance ID of the caller.
   * @param timerHandle The handle for a timer to be cancelled.
   * @return false if the timer handle is invalid or is not owned by the caller
   */
  bool cancelTimer(uint32_t instanceId, TimerHandle timerHandle);

  /**
   * Looks up a timer request given a timer handle. mMutex must be acquired
   * prior to calling this function.
   *
   * @param timerHandle The timer handle referring to a given request.
   * @param index A pointer to the index of the handle. If the handle is found
   *        this will be populated with the index of the request from the list
   *        of requests. This is optional and will only be populated if not
   *        nullptr.
   * @return A pointer to a TimerRequest or nullptr if no match is found.
   */
  TimerRequest *getTimerRequestByTimerHandleLocked(TimerHandle timerHandle,
      size_t *index = nullptr);

  /**
   * Obtains a unique timer handle to return to an app requesting a timer.
   * mMutex must be acquired prior to calling this function.
   *
   * @return The guaranteed unique timer handle.
   */
  TimerHandle generateTimerHandleLocked();

  /**
   * Obtains a unique timer handle by searching through the list of timer
   * requests. This is a fallback for once the timer handles have been
   * exhausted. mMutex must be acquired prior to calling this function.
   *
   * @return A guaranteed unique timer handle.
   */
  TimerHandle generateUniqueTimerHandleLocked();

  /**
   * Helper function to determine whether a new timer of the specified type
   * can be allocated. mMutex must be acquired prior to calling this function.
   *
   * @param isNanoappTimer true if invoked for a nanoapp timer.
   * @return true if a new timer of the given type is allowed to be allocated.
   */
  bool isNewTimerAllowedLocked(bool isNanoappTimer) const;

  /**
   * Inserts a TimerRequest into the list of active timer requests. The order of
   * mTimerRequests is always maintained such that the timer request with the
   * closest expiration time is at the front of the list. mMutex must be
   * acquired prior to calling this function.
   *
   * @param timerRequest The timer request being inserted into the list.
   * @return true if insertion of timer succeeds.
   */
  bool insertTimerRequestLocked(const TimerRequest& timerRequest);

  /**
   * Pops the TimerRequest at the front of the list. mMutex must be acquired
   * prior to calling this function.
   */
  void popTimerRequestLocked();

  /**
   * Removes the TimerRequest at the specified index of the list. mMutex must be
   * acquired prior to calling this function.
   *
   * @param index The index of the TimerRequest to remove.
   */
  void removeTimerRequestLocked(size_t index);

  /**
   * Sets the underlying system timer to the next timer in the timer list if
   * available.
   *
   * @return true if any timer events were posted
   */
  bool handleExpiredTimersAndScheduleNext();

  /**
   * Same as handleExpiredTimersAndScheduleNext(), except mMutex must be
   * acquired prior to calling this function.
   *
   * @return true if any timer events were posted
   */
  bool handleExpiredTimersAndScheduleNextLocked();

  /**
   * This static method handles the callback from the system timer. The data
   * pointer here is the TimerPool instance.
   *
   * @param data A pointer to the timer pool.
   */
  static void handleSystemTimerCallback(void *timerPoolPtr);
};

}  // namespace chre

#endif  // CHRE_CORE_TIMER_POOL_H_
