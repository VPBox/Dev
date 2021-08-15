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

#ifndef CHRE_PLATFORM_PLATFORM_LOG_H_
#define CHRE_PLATFORM_PLATFORM_LOG_H_

#include "chre/target_platform/platform_log_base.h"
#include "chre/util/singleton.h"

namespace chre {

/**
 * A logging class that platforms can supply an implementation and storage for.
 * Only one instance of this class is created by CHRE and is accessed via the
 * PlatformLogSingleton alias provided below.
 */
class PlatformLog : public PlatformLogBase {
 public:
  /**
   * Allows the platform to initialize logging infrastructure.
   */
  PlatformLog();

  /**
   * Allows the platform to deinitialize logging infrastructure.
   */
  ~PlatformLog();

  /**
   * Logs message with printf-style arguments. No trailing newline is required
   * for this method.
   */
  void log(const char *formatStr, ...);
};

//! Provides an alias to the PlatformLog singleton.
typedef Singleton<PlatformLog> PlatformLogSingleton;

}  // namespace chre

#endif  // CHRE_PLATFORM_PLATFORM_LOG_H_
