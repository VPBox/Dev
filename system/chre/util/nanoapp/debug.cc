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

#include "chre/util/nanoapp/debug.h"

#include <cctype>
#include <cstdio>

#ifdef CHRE_NANOAPP_INTERNAL
#include "chre/platform/log.h"
#else
#define LOG_TAG "[DEBUG]"
#include "chre/util/nanoapp/log.h"
#endif

namespace chre {

void logBuffer(const uint8_t *buffer, size_t bufferSize) {
  char line[32];
  char lineChars[32];
  size_t offset = 0;
  size_t offsetChars = 0;

  if (bufferSize > 128) {
    LOGD("Dumping first 128 bytes of buffer @ %p, size %zu",
         buffer, bufferSize);
    bufferSize = 128;
  } else {
    LOGD("Dumping buffer @ %p, size %zu", buffer, bufferSize);
  }

  for (size_t i = 1; i <= bufferSize; i++) {
    // This ignores potential errors returned by snprintf. This is a relatively
    // simple case and the deliberate decision to ignore them has been made.
    offset += static_cast<size_t>(
        snprintf(&line[offset], sizeof(line) - offset, "%02x ", buffer[i - 1]));
    offsetChars += static_cast<size_t>(
        snprintf(&lineChars[offsetChars], sizeof(lineChars) - offsetChars,
                 "%c", (isprint(buffer[i - 1])) ? buffer[i - 1] : '.'));
    if ((i % 8) == 0) {
      LOGD("  %s\t%s", line, lineChars);
      offset = 0;
      offsetChars = 0;
    } else if ((i % 4) == 0) {
      offset += static_cast<size_t>(
          snprintf(&line[offset], sizeof(line) - offset, " "));
    }
  }

  if (offset > 0) {
    char tabs[8];
    char *pos = tabs;
    while (offset < 28) {
      *pos++ = '\t';
      offset += 8;
    }
    *pos = '\0';
    LOGD("  %s%s%s", line, tabs, lineChars);
  }
}

}  // namespace chre
