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

#ifndef CHRE_PLATFORM_LOG_H_
#define CHRE_PLATFORM_LOG_H_

/**
 * @file
 * Includes the appropriate platform-specific header file that supplies logging
 * capabilities. The platform header file must supply these symbols, either as
 * macros or free functions:
 *
 *   LOGE(format, ...)
 *   LOGW(format, ...)
 *   LOGI(format, ...)
 *   LOGD(format, ...)
 *
 * Where "format" is a printf-style format string, and E, W, I, D correspond to
 * the log levels Error, Warning, Informational, and Debug, respectively.
 */

#include "chre/target_platform/log.h"
#include "chre/util/log_common.h"

/*
 * Log errors if the platform does not supply logging macros.
 */

#ifndef LOGE
#error "LOGE must be defined"
#endif  // LOGE

#ifndef LOGW
#error "LOGW must be defined"
#endif  // LOGW

#ifndef LOGI
#error "LOGI must be defined"
#endif  // LOGI

#ifndef LOGD
#error "LOGD must be defined"
#endif  // LOGD

/*
 * Supply a stub implementation of the LOGx macros when the build is
 * configured with a minimum logging level that is above the requested level.
 */

#ifndef CHRE_MINIMUM_LOG_LEVEL
#error "CHRE_MINIMUM_LOG_LEVEL must be defined"
#endif  // CHRE_MINIMUM_LOG_LEVEL

#if CHRE_MINIMUM_LOG_LEVEL < CHRE_LOG_LEVEL_ERROR
#undef LOGE
#define LOGE(format, ...) chreLogNull(format, ##__VA_ARGS__)
#endif

#if CHRE_MINIMUM_LOG_LEVEL < CHRE_LOG_LEVEL_WARN
#undef LOGW
#define LOGW(format, ...) chreLogNull(format, ##__VA_ARGS__)
#endif

#if CHRE_MINIMUM_LOG_LEVEL < CHRE_LOG_LEVEL_INFO
#undef LOGI
#define LOGI(format, ...) chreLogNull(format, ##__VA_ARGS__)
#endif

#if CHRE_MINIMUM_LOG_LEVEL < CHRE_LOG_LEVEL_DEBUG
#undef LOGD
#define LOGD(format, ...) chreLogNull(format, ##__VA_ARGS__)
#endif

/**
 * Logs an out of memory error with file and line number.
 */
#define LOG_OOM() LOGE("OOM at %s:%d", CHRE_FILENAME, __LINE__)

#endif  // CHRE_PLATFORM_LOG_H_
