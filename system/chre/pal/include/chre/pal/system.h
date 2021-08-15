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

#ifndef CHRE_PAL_SYSTEM_H_
#define CHRE_PAL_SYSTEM_H_

/**
 * @file
 * Defines a set of system functions implemented by the entity opening a PAL
 * interface that PAL implementations are strongly recommended to use in place
 * of calling directly into the underlying system. One of the motivations for
 * having PAL implementations use these functions rather than equivalent ones
 * exposed by the underlying platform is to provide improved debuggability of
 * the CHRE implementation as a whole.
 */

#include <stdint.h>

#include "chre_api/chre/re.h"
#include "chre/pal/version.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initial version of the CHRE PAL System API.
 */
#define CHRE_PAL_SYSTEM_API_V1_0  CHRE_PAL_CREATE_API_VERSION(1, 0)

/**
 * The version of the CHRE GNSS PAL defined in this header file.
 */
#define CHRE_PAL_SYSTEM_API_CURRENT_VERSION  CHRE_PAL_SYSTEM_API_V1_0

struct chrePalSystemApi {
    /**
     * The version of this API structure, which can be used at runtime to
     * determine if functions added in newer versions are available, etc.
     */
    uint32_t version;

    /**
     * Retrieves the current time using the same time base as supplied to the
     * nanoapp in chreGetTime(). This function should be used when populating
     * reference time fields in event structures passed by a PAL implementation
     * to CHRE.
     *
     * @return Current time since some fixed arbitrary reference point in the
     *         past, in nanoseconds
     *
     * @see chreGetTime
     */
    uint64_t (*getCurrentTime)(void);

    /**
     * Logs a message to the same messaging subsystem as used by the CHRE
     * system. Semantics are the same as chreLog, but the implementation may
     * differ.
     *
     * @param level Log level, same as defined in the CHRE API
     * @param formatStr printf-style format string, details provided in the CHRE
     *        API
     *
     * @see chreLog
     */
    void (*log)(enum chreLogLevel level, const char *formatStr, ...);

    /**
     * Dynamically allocate a block of memory. Semantics are the same as
     * chreHeapAlloc, but the implementation may differ.
     *
     * @param size Size of the allocation, in bytes
     *
     * @return Pointer to buffer that is aligned to store any kind of variable,
     *         or NULL if the allocation failed
     *
     * @see chreHeapAlloc
     */
    void *(*memoryAlloc)(size_t size);

    /**
     * Return memory allocated via memoryAlloc to the system. Semantics are the
     * same as chreHeapFree, but the implementation may differ.
     *
     * @param pointer A pointer previously returned by memoryAlloc
     *
     * @see chreHeapFree
     */
    void (*memoryFree)(void *pointer);
};

#ifdef __cplusplus
}
#endif

#endif  // CHRE_PAL_SYSTEM_H_
