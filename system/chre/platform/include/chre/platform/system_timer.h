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

#ifndef CHRE_PLATFORM_SYSTEM_TIMER_H_
#define CHRE_PLATFORM_SYSTEM_TIMER_H_

#include <cstdint>

#include "chre/target_platform/system_timer_base.h"
#include "chre/util/non_copyable.h"
#include "chre/util/time.h"

namespace chre {

/**
 * The function signature of a timer callback.
 *
 * @param The data pointer here is passed in by the entity that requested the
 *        timer and is used to provided a context in the callback.
 */
typedef void (SystemTimerCallback)(void *data);

/**
 * Abstracts a system timer from the underlying platform, which will invoke the
 * supplied callback after at least the given amount of time has passed. The
 * calling context for the callback is undefined, and may be inside an
 * interrupt, or in a different thread, etc. Therefore, the callback is
 * responsible for ensuring that it handles this potential concurrency
 * appropriately.
 */
class SystemTimer : public SystemTimerBase,
                    public NonCopyable {
 public:
  /**
   * Allows the platform to construct a timer.
   */
  SystemTimer();

  /**
   * Cleans up a timer when it goes out of scope.
   */
  ~SystemTimer();

  /**
   * Initializes the timer. This must be called before other methods in this
   * class are called.
   *
   * @return true on successful, false on failure
   */
  bool init();

  /**
   * Sets the timer to expire after the given delay. If the timer was already
   * running, its expiry time is updated to this value.
   *
   * Note that it is possible for the timer to fire before this function
   * returns.
   *
   * @param callback The callback to invoke when the timer has elapsed.
   * @param data The data to pass to the callback when it is invoked.
   * @param delay The minimum delay until the first firing of the timer.
   * @return true on success, false on failure
   */
  bool set(SystemTimerCallback *callback, void *data, Nanoseconds delay);

  /**
   * Disarms the timer. If it was armed and is not currently in the process of
   * firing, this prevents the callback from being invoked until the timer is
   * restarted by a subsequent call to set().
   *
   * @return Whether or not the timer was cancelled successfully.
   */
  bool cancel();

  /**
   * Determines whether or not the timer is currently timing.
   *
   * @return true if the timer is currently active and false if it is idle.
   */
  bool isActive();

 private:
  // We make SystemTimerBase a friend to allow the base platform class to
  // access the members of this class.
  friend class SystemTimerBase;

  //! The callback to invoke when the timer has elapsed.
  SystemTimerCallback *mCallback;

  //! The data to pass to the callback when invoked.
  void *mData;
};

}  // namespace chre

#endif  // CHRE_PLATFORM_SYSTEM_TIMER_H_
