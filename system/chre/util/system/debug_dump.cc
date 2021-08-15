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

#include "chre/util/system/debug_dump.h"

#include <cstdio>

#include "chre/platform/log.h"

namespace chre {

void debugDumpPrint(char *buffer, size_t *bufferPos, size_t bufferSize,
                    const char *formatStr, ...) {
  if (*bufferPos < bufferSize) {
    va_list argList;
    va_start(argList, formatStr);
    int strLen = vsnprintf(&buffer[*bufferPos], bufferSize - *bufferPos,
                           formatStr, argList);
    va_end(argList);

    if (strLen >= 0) {
      size_t strLenBytes = static_cast<size_t>(strLen);
      if (bufferSize < strLenBytes ||
          *bufferPos >= (bufferSize - strLenBytes)) {
        *bufferPos = bufferSize;
        buffer[bufferSize - 1] = '\0';
        LOG_OOM();
      } else {
        *bufferPos += strLenBytes;
      }
    } else {
      LOGE("Error formatting dump state");
    }
  }
}

}  // namespace chre
