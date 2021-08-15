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

#ifndef CHRE_PLATFORM_FATAL_ERROR_H_
#define CHRE_PLATFORM_FATAL_ERROR_H_

/**
 * @file
 * Includes the platform-specific header that supplies a fatal error macro. The
 * platform header must supply the following symbol as a macro or free function:
 *
 * FATAL_ERROR_QUIT()
 *
 * The error will be logged via the standard logging macros and then
 * FATAL_ERROR_QUIT will be invoked. The FATAL_ERROR_QUIT macro will terminate
 * the CHRE runtime.
 */

#include "chre/platform/log.h"
#include "chre/target_platform/fatal_error.h"

#ifndef FATAL_ERROR_QUIT
#error "FATAL_ERROR_QUIT must be defined"
#endif  // FATAL_ERROR_QUIT

#define FATAL_ERROR(fmt, ...) do { \
  LOGE(fmt, ##__VA_ARGS__);        \
  FATAL_ERROR_QUIT();              \
} while (0)

/**
 * Fatal error on out of memory error with file and line number.
 */
#define FATAL_ERROR_OOM() \
    FATAL_ERROR("Out of memory")

#endif  // CHRE_PLATFORM_FATAL_ERROR_H_
