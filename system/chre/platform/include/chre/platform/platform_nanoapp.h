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

#ifndef CHRE_PLATFORM_PLATFORM_NANOAPP_H_
#define CHRE_PLATFORM_PLATFORM_NANOAPP_H_

#include <cstdint>
#include <cstddef>

#include "chre/util/non_copyable.h"
#include "chre/target_platform/platform_nanoapp_base.h"

namespace chre {

/**
 * The common interface to Nanoapp functionality that has platform-specific
 * implementation but must be supported for every platform.
 */
class PlatformNanoapp : public PlatformNanoappBase, public NonCopyable {
 public:
  /**
   * Calls the start function of the nanoapp. For dynamically loaded nanoapps,
   * this must also result in calling through to any of the nanoapp's static
   * global constructors/init functions, etc., prior to invoking the
   * nanoappStart.
   *
   * @return true if the app was able to start successfully
   *
   * @see nanoappStart
   */
  bool start();

  /**
   * Passes an event to the nanoapp.
   *
   * @see nanoappHandleEvent
   */
  void handleEvent(uint32_t senderInstanceId, uint16_t eventType,
                   const void *eventData);

  /**
   * Calls the nanoapp's end callback. For dynamically loaded nanoapps, this
   * must also result in calling through to any of the nanoapp's static global
   * destructors, atexit functions, etc., after nanoappEnd returns. This is only
   * valid to call after start() has returned true.
   *
   * This function must leave the nanoapp in a state where it can be started
   * again via start().
   *
   * @see nanoappEnd
   */
  void end();

  /**
   * Retrieves the nanoapp's 64-bit identifier. This function must always return
   * a valid identifier - either the one supplied by the host via the HAL (from
   * the header), or the authoritative value inside the nanoapp binary if one
   * exists. In the event that both are available and they do not match, the
   * platform implementation must return false from start().
   */
  uint64_t getAppId() const;

  /**
   * Retrieves the nanoapp's own version number. The same restrictions apply
   * here as for getAppId().
   *
   * @see #getAppId
   */
  uint32_t getAppVersion() const;

  /**
   * Retrieves the API version that this nanoapp was compiled against. This
   * function must only be called while the nanoapp is running (i.e. between
   * calls to start() and end()).
   */
  uint32_t getTargetApiVersion() const;

  /**
   * Returns true if the nanoapp should not appear in the context hub HAL list
   * of nanoapps, e.g. because it implements some device functionality purely
   * beneath the HAL.
   */
  bool isSystemNanoapp() const;

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

 protected:
  /**
   * PlatformNanoapp's constructor is protected, as it must only exist within
   * the context of the derived class chre::Nanoapp.
   */
  PlatformNanoapp() = default;

  /**
   * Unloads the nanoapp from memory.
   */
  ~PlatformNanoapp();
};

}  // namespace chre

#endif  // CHRE_PLATFORM_PLATFORM_NANOAPP_H_
