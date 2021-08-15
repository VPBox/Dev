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

#ifndef CHRE_UTIL_SYSTEM_DEBUG_DUMP_H_
#define CHRE_UTIL_SYSTEM_DEBUG_DUMP_H_

#include <cstddef>
#include <stdarg.h>

namespace chre {

/**
 * Prints a formatted string into a string buffer. If the log message does not
 * fit into the remaining space of the buffer, the error is logged and the
 * buffer is null-terminated. Subsequent calls to this function will do nothing.
 *
 * @param buffer Pointer to the start of the buffer.
 * @param bufferPos Pointer to buffer position to start the print (in-out).
 * @param bufferSize Size of the buffer in bytes.
 * @param formatStr Formatted string.
 */
void debugDumpPrint(char *buffer, size_t *bufferPos, size_t bufferSize,
                    const char *formatStr, ...);

}  // namespace chre

#endif  // CHRE_UTIL_SYSTEM_DEBUG_DUMP_H_
