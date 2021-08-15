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

#ifndef CHRE_CORE_EVENT_LOOP_H_
#define CHRE_CORE_EVENT_LOOP_H_

#include "chre/core/event.h"
#include "chre/core/nanoapp.h"
#include "chre/core/timer_pool.h"
#include "chre/platform/atomic.h"
#include "chre/platform/mutex.h"
#include "chre/platform/platform_nanoapp.h"
#include "chre/platform/power_control_manager.h"
#include "chre/util/dynamic_vector.h"
#include "chre/util/fixed_size_blocking_queue.h"
#include "chre/util/non_copyable.h"
#include "chre/util/synchronized_memory_pool.h"
#include "chre/util/unique_ptr.h"
#include "chre_api/chre/event.h"

// These default values can be overridden in the variant-specific makefile.
#ifndef CHRE_MAX_EVENT_COUNT
#define CHRE_MAX_EVENT_COUNT 96
#endif

#ifndef CHRE_MAX_UNSCHEDULED_EVENT_COUNT
#define CHRE_MAX_UNSCHEDULED_EVENT_COUNT 96
#endif

namespace chre {

/**
 * The EventLoop represents a single thread of execution that is shared among
 * zero or more nanoapps. As the name implies, the EventLoop is built around a
 * loop that delivers events to the nanoapps managed within for processing.
 */
class EventLoop : public NonCopyable {
 public:
  EventLoop() : mRunning(true) {}

  /**
   * Synchronous callback used with forEachNanoapp
   */
  typedef void (NanoappCallbackFunction)(const Nanoapp *nanoapp, void *data);

  /**
   * Searches the set of nanoapps managed by this EventLoop for one with the
   * given app ID. If found, provides its instance ID, which can be used to send
   * events to the app.
   *
   * This function is safe to call from any thread.
   *
   * @param appId The nanoapp identifier to search for.
   * @param instanceId If this function returns true, will be populated with the
   *        instanceId associated with the given appId; otherwise unmodified.
   *        Must not be null.
   * @return true if the given app ID was found and instanceId was populated
   */
  bool findNanoappInstanceIdByAppId(uint64_t appId, uint32_t *instanceId) const;

  /**
   * Iterates over the list of Nanoapps managed by this EventLoop, and invokes
   * the supplied callback for each one. This holds a lock if necessary, so it
   * is safe to call from any thread.
   *
   * @param callback Function to invoke on each Nanoapp (synchronously)
   * @param data Arbitrary data to pass to the callback
   */
  void forEachNanoapp(NanoappCallbackFunction *callback, void *data);

  /**
   * Invokes a message to host free callback supplied by the given nanoapp
   * (identified by app ID). Ensures that the calling context is updated
   * appropriately.
   *
   * @param appId Identifies the nanoapp that sent this message and supplied the
   *        free callback
   * @param freeFunction The non-null message free callback given by the nanoapp
   * @param message Pointer to the message data
   * @param messageSize Size of the message
   */
  void invokeMessageFreeFunction(
      uint64_t appId, chreMessageFreeFunction *freeFunction, void *message,
      size_t messageSize);

  /**
   * Invokes the Nanoapp's start callback, and if successful, adds it to the
   * set of Nanoapps managed by this EventLoop. This function must only be
   * called from the context of the thread that runs this event loop (i.e. from
   * the same thread that will call run() or from a callback invoked within
   * run()).
   *
   * @param nanoapp The nanoapp that will be started. Upon success, this
   *        UniquePtr will become invalid, as the underlying Nanoapp instance
   *        will have been transferred to be managed by this EventLoop.
   * @return true if the app was started successfully
   */
  bool startNanoapp(UniquePtr<Nanoapp>& nanoapp);

  /**
   * Stops and unloads a nanoapp identified by its instance ID. The end entry
   * point will be invoked, and the chre::Nanoapp instance will be destroyed.
   * After this function returns, all references to the Nanoapp instance are
   * invalidated.
   *
   * @param instanceId The nanoapp's unique instance identifier
   * @param allowSystemNanoappUnload If false, this function will reject
   *        attempts to unload a system nanoapp
   *
   * @return true if the nanoapp with the given instance ID was found & unloaded
   */
  bool unloadNanoapp(uint32_t instanceId, bool allowSystemNanoappUnload);

  /**
   * Executes the loop that blocks on the event queue and delivers received
   * events to nanoapps. Only returns after stop() is called (from another
   * context).
   */
  void run();

  /**
   * Signals the event loop currently executing in run() to exit gracefully at
   * the next available opportunity. This function is thread-safe.
   */
  void stop();

  /**
   * Posts an event to a nanoapp that is currently running (or all nanoapps if
   * the target instance ID is kBroadcastInstanceId). A senderInstanceId cannot
   * be provided to this method because it should only be used to post events
   * sent by the system. If the event fails to post, this is considered a fatal
   * error.
   *
   * @see postLowPriorityEventOrFree
   */
  bool postEventOrDie(uint16_t eventType, void *eventData,
                      chreEventCompleteFunction *freeCallback,
                      uint32_t targetInstanceId = kBroadcastInstanceId);

  /**
   * Posts an event to a nanoapp that is currently running (or all nanoapps if
   * the target instance ID is kBroadcastInstanceId). If the event fails to
   * post, it is freed with freeCallback.
   *
   * This function is safe to call from any thread.
   *
   * @param eventType The type of data being posted.
   * @param eventData The data being posted.
   * @param freeCallback The callback to invoke when the event is no longer
   *        needed.
   * @param senderInstanceId The instance ID of the sender of this event.
   * @param targetInstanceId The instance ID of the destination of this event.
   *
   * @return true if the event was successfully added to the queue.
   *
   * @see chreSendEvent
   */
  bool postLowPriorityEventOrFree(
      uint16_t eventType, void *eventData,
      chreEventCompleteFunction *freeCallback,
      uint32_t senderInstanceId = kSystemInstanceId,
      uint32_t targetInstanceId = kBroadcastInstanceId);

  /**
   * Returns a pointer to the currently executing Nanoapp, or nullptr if none is
   * currently executing. Must only be called from within the thread context
   * associated with this EventLoop.
   *
   * @return the currently executing nanoapp, or nullptr
   */
  Nanoapp *getCurrentNanoapp() const {
    return mCurrentApp;
  }

  /**
   * Gets the number of nanoapps currently associated with this event loop. Must
   * only be called within the context of this EventLoop.
   *
   * @return The number of nanoapps managed by this event loop
   */
  size_t getNanoappCount() const {
    return mNanoapps.size();
  }

  /**
   * Obtains the TimerPool associated with this event loop.
   *
   * @return The timer pool owned by this event loop.
   */
  TimerPool& getTimerPool() {
    return mTimerPool;
  }

  /**
   * Searches the set of nanoapps managed by this EventLoop for one with the
   * given instance ID.
   *
   * This function is safe to call from any thread.
   *
   * @param instanceId The nanoapp instance ID to search for.
   * @return a pointer to the found nanoapp or nullptr if no match was found.
   */
  Nanoapp *findNanoappByInstanceId(uint32_t instanceId) const;

  /**
   * Looks for an app with the given ID and if found, populates info with its
   * metadata. Safe to call from any thread.
   *
   * @see chreGetNanoappInfoByAppId
   */
  bool populateNanoappInfoForAppId(uint64_t appId,
                                   struct chreNanoappInfo *info) const;

  /**
   * Looks for an app with the given instance ID and if found, populates info
   * with its metadata. Safe to call from any thread.
   *
   * @see chreGetNanoappInfoByInstanceId
   */
  bool populateNanoappInfoForInstanceId(uint32_t instanceId,
                                        struct chreNanoappInfo *info) const;

  /**
   * @return true if the current Nanoapp (or entire CHRE) is being unloaded, and
   *         therefore it should not be allowed to send events or messages, etc.
   */
  bool currentNanoappIsStopping() const;

  /**
   * Prints state in a string buffer. Must only be called from the context of
   * the main CHRE thread.
   *
   * @param buffer Pointer to the start of the buffer.
   * @param bufferPos Pointer to buffer position to start the print (in-out).
   * @param size Size of the buffer in bytes.
   */
  void logStateToBuffer(char *buffer, size_t *bufferPos,
                        size_t bufferSize) const;


  /**
   * Returns a reference to the power control manager. This allows power
   * controls from subsystems outside the event loops.
   */
  PowerControlManager& getPowerControlManager() {
    return mPowerControlManager;
  }

 private:
  //! The maximum number of events that can be active in the system.
  static constexpr size_t kMaxEventCount = CHRE_MAX_EVENT_COUNT;

  //! The minimum number of events to reserve in the event pool for high
  //! priority events.
  static constexpr size_t kMinReservedHighPriorityEventCount = 16;

  //! The maximum number of events that are awaiting to be scheduled. These
  //! events are in a queue to be distributed to apps.
  static constexpr size_t kMaxUnscheduledEventCount =
      CHRE_MAX_UNSCHEDULED_EVENT_COUNT;

  //! The memory pool to allocate incoming events from.
  SynchronizedMemoryPool<Event, kMaxEventCount> mEventPool;

  //! The timer used schedule timed events for tasks running in this event loop.
  TimerPool mTimerPool;

  //! The list of nanoapps managed by this event loop.
  DynamicVector<UniquePtr<Nanoapp>> mNanoapps;

  //! This lock *must* be held whenever we:
  //!   (1) make changes to the mNanoapps vector, or
  //!   (2) read the mNanoapps vector from a thread other than the one
  //!       associated with this EventLoop
  //! It is not necessary to acquire the lock when reading mNanoapps from within
  //! the thread context of this EventLoop.
  mutable Mutex mNanoappsLock;

  //! The blocking queue of incoming events from the system that have not been
  //! distributed out to apps yet.
  FixedSizeBlockingQueue<Event *, kMaxUnscheduledEventCount> mEvents;

  //! Indicates whether the event loop is running.
  AtomicBool mRunning;

  //! The nanoapp that is currently executing - must be set any time we call
  //! into the nanoapp's entry points or callbacks
  Nanoapp *mCurrentApp = nullptr;

  //! Set to the nanoapp we are in the process of unloading in unloadNanoapp()
  Nanoapp *mStoppingNanoapp = nullptr;

  //! The object which manages power related controls.
  PowerControlManager mPowerControlManager;

  //! The maximum number of events ever waiting in the event pool.
  size_t mMaxEventPoolUsage = 0;

  /**
   * Modifies the run loop state so it no longer iterates on new events. This
   * should only be invoked by the event loop when it is ready to stop
   * processing new events.
   */
  void onStopComplete();

  /**
   * Allocates an event from the event pool and post it.
   *
   * @return true if the event has been successfully allocated and posted.
   *
   * @see postEventOrDie and postLowPriorityEventOrFree
   */
  bool allocateAndPostEvent(uint16_t eventType, void *eventData,
    chreEventCompleteFunction *freeCallback, uint32_t senderInstanceId,
    uint32_t targetInstanceId);

  /**
   * Do one round of Nanoapp event delivery, only considering events in
   * Nanoapps' own queues (not mEvents).
   *
   * @return true if there are more events pending in Nanoapps' own queues
   */
  bool deliverEvents();

  /**
   * Delivers the next event pending in the Nanoapp's queue, and takes care of
   * freeing events once they have been delivered to all nanoapps. Must only be
   * called after confirming that the app has at least 1 pending event.
   *
   * @return true if the nanoapp has another event pending in its queue
   */
  bool deliverNextEvent(const UniquePtr<Nanoapp>& app);

  /**
   * Given an event pulled from the main incoming event queue (mEvents), deliver
   * it to all Nanoapps that should receive the event, or free the event if
   * there are no valid recipients.
   *
   * @param event The Event to distribute to Nanoapps
   */
  void distributeEvent(Event *event);

  /**
   * Distribute all events pending in the inbound event queue. Note that this
   * function only guarantees that any events in the inbound queue at the time
   * it is called will be distributed to Nanoapp event queues - new events may
   * still be posted during or after this function call from other threads as
   * long as postEvent() will accept them.
   */
  void flushInboundEventQueue();

  /**
   * Delivers events pending in Nanoapps' own queues until they are all empty.
   */
  void flushNanoappEventQueues();

  /**
   * Call after when an Event has been delivered to all intended recipients.
   * Invokes the event's free callback (if given) and releases resources.
   *
   * @param event The event to be freed
   */
  void freeEvent(Event *event);

  /**
   * Finds a Nanoapp with the given 64-bit appId.
   *
   * Only safe to call within this EventLoop's thread, or if mNanoappsLock is
   * held.
   *
   * @param appId Nanoapp ID
   * @return Pointer to Nanoapp instance in this EventLoop with the given app
   *         ID, or nullptr if not found
   */
  Nanoapp *lookupAppByAppId(uint64_t appId) const;

  /**
   * Finds a Nanoapp with the given instanceId.
   *
   * Only safe to call within this EventLoop's thread, or if mNanoappsLock is
   * held.
   *
   * @param instanceId Nanoapp instance identifier
   * @return Nanoapp with the given instanceId, or nullptr if not found
   */
  Nanoapp *lookupAppByInstanceId(uint32_t instanceId) const;

  /**
   * Sends an event with payload struct chreNanoappInfo populated from the given
   * Nanoapp instance to inform other nanoapps about it starting/stopping.
   *
   * @param eventType Should be one of CHRE_EVENT_NANOAPP_{STARTED, STOPPED}
   * @param nanoapp The nanoapp instance whose status has changed
   */
  void notifyAppStatusChange(uint16_t eventType, const Nanoapp& nanoapp);

  /**
   * Stops and unloads the Nanoapp at the given index in mNanoapps.
   *
   * IMPORTANT: prior to calling this function, the event queues must be in a
   * safe condition for removal of this nanoapp. This means that there must not
   * be any pending events in this nanoapp's queue, and there must not be any
   * outstanding events sent by this nanoapp, as they may reference the
   * nanoapp's own memory (even if there is no free callback).
   */
  void unloadNanoappAtIndex(size_t index);
};

}  // namespace chre

#endif  // CHRE_CORE_EVENT_LOOP_H_
