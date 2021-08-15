/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef CHRE_UTIL_NANOAPP_ASSERT_H_
#define CHRE_UTIL_NANOAPP_ASSERT_H_

/**
 * @file
 *
 * Suppplies a CHRE_ASSERT macro for Nanoapps to use.
 */

#ifdef CHRE_IS_NANOAPP_BUILD

#include <chre.h>

/**
 * Provides the CHRE_ASSERT macro that uses chreAbort to abort the nanoapp upon
 * failure.
 *
 * @param the condition to check for non-zero.
 */
#ifdef CHRE_ASSERTIONS_ENABLED
#define CHRE_ASSERT(condition) do {                                    \
      if (!(condition)) {                                              \
        chreLog(CHRE_LOG_ERROR, "CHRE_ASSERT at %s:%d", CHRE_FILENAME, \
                __LINE__);                                             \
        chreAbort(UINT32_MAX);                                         \
      }                                                                \
    } while (0)
#else
#define CHRE_ASSERT(condition) ((void) (condition))
#endif  // CHRE_ASSERTIONS_ENABLED

#ifdef GTEST
// Mocks are not supported in standalone mode. Just skip the statement entirely.
#define EXPECT_CHRE_ASSERT(statement)
#endif  // GTEST

#else
// When compiling as a static nanoapp, use the platform implementation of
// CHRE_ASSERT.
#include "chre/platform/assert.h"
#endif  // CHRE_IS_NANOAPP_BUILD

#endif  // CHRE_UTIL_NANOAPP_ASSERT_H_
