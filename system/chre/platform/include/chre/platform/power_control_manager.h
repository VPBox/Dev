/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef CHRE_PLATFORM_POWER_CONTROL_MANAGER_H
#define CHRE_PLATFORM_POWER_CONTROL_MANAGER_H

#include <cstddef>

#include "chre/target_platform/power_control_manager_base.h"
#include "chre/util/non_copyable.h"

namespace chre {

/**
 * An abstraction for an entity that performs power-related controls for the
 * underlying platform.
 */
class PowerControlManager : public PowerControlManagerBase,
                            public NonCopyable {
 public:
  /**
   * Perform power-related control after a single process of the event loop.
   *
   * @param numPendingEvents The current size of the event queue.
   */
  void postEventLoopProcess(size_t numPendingEvents);

  /**
   * @return true if the host is awake, false otherwise.
   */
  bool hostIsAwake();
};

} // namespace chre

#endif // CHRE_PLATFORM_POWER_CONTROL_MANAGER_H
