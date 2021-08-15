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

#include "chre/platform/shared/pal_system_api.h"

#include <cstdarg>
#include <cstdio>

#include "chre/pal/system.h"
#include "chre/platform/log.h"
#include "chre/platform/memory.h"
#include "chre/platform/system_time.h"

//! Define a format string for PAL logs. This is defined as a macro so that it
//! can be used as a string literal by platform implementations of the logging
//! macros.
#define PAL_LOG_FORMAT_STR "PAL: %s"

namespace chre {

uint64_t palSystemApiGetCurrentTime() {
  return SystemTime::getMonotonicTime().toRawNanoseconds();
}

void palSystemApiLog(enum chreLogLevel level, const char *formatStr, ...) {
  char logBuf[512];
  va_list args;

  va_start(args, formatStr);
  vsnprintf(logBuf, sizeof(logBuf), formatStr, args);
  va_end(args);

  switch (level) {
    case CHRE_LOG_ERROR:
      LOGE(PAL_LOG_FORMAT_STR, logBuf);
      break;
    case CHRE_LOG_WARN:
      LOGW(PAL_LOG_FORMAT_STR, logBuf);
      break;
    case CHRE_LOG_INFO:
      LOGI(PAL_LOG_FORMAT_STR, logBuf);
      break;
    case CHRE_LOG_DEBUG:
    default:
      LOGD(PAL_LOG_FORMAT_STR, logBuf);
  }
}

// Initialize the CHRE System API with function implementations provided above.
const chrePalSystemApi gChrePalSystemApi = {
  CHRE_PAL_SYSTEM_API_CURRENT_VERSION, /* version */
  palSystemApiGetCurrentTime,          /* getCurrentTime */
  palSystemApiLog,                     /* log */
  palSystemApiMemoryAlloc,             /* memoryAlloc */
  palSystemApiMemoryFree,              /* memoryFree */
};

}  // namespace chre
