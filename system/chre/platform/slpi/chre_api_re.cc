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

#include "chre_api/chre/re.h"
#include "chre/platform/log.h"
#include "chre/platform/slpi/debug_dump.h"
#include "chre/util/macros.h"

#ifdef CHRE_USE_FARF_LOGGING
inline void logToFarf(enum chreLogLevel level, const char *logStr) {
  switch (level) {
    case CHRE_LOG_ERROR:
      LOGE(logStr);
      break;
    case CHRE_LOG_WARN:
      LOGW(logStr);
      break;
    case CHRE_LOG_INFO:
      LOGI(logStr);
      break;
    case CHRE_LOG_DEBUG:
    default:
      LOGD(logStr);
  }
}

#else  // CHRE_USE_FARF_LOGGING

inline ashLogLevel chreLogLevelToAshLogLevel(enum chreLogLevel level) {
  enum ashLogLevel ashLevel;
  switch (level) {
    case CHRE_LOG_ERROR:
      ashLevel = ASH_LOG_ERROR;
      break;
    case CHRE_LOG_WARN:
      ashLevel = ASH_LOG_WARN;
      break;
    case CHRE_LOG_INFO:
      ashLevel = ASH_LOG_INFO;
      break;
    case CHRE_LOG_DEBUG:
    default:
      ashLevel = ASH_LOG_DEBUG;
  }
  return ashLevel;
}

#endif  // CHRE_USE_FARF_LOGGING

DLL_EXPORT void chreLog(enum chreLogLevel level, const char *formatStr, ...) {
  va_list args;

  va_start(args, formatStr);
#ifdef CHRE_USE_FARF_LOGGING
  // The same size is used in the implmentation of ashVaLog().
  constexpr size_t kDebugMaxLogEntrySize = 128;
  // FARF doesn't provide a method that takes va_list as an input so write the
  // string to a buffer before logging so it can be passed to the LOGX methods.
  char logBuf[kDebugMaxLogEntrySize];
  vsnprintf(logBuf, sizeof(logBuf), formatStr, args);
  logToFarf(level, logBuf);
#else  // CHRE_USE_FARF_LOGGING
  ashVaLog(ASH_SOURCE_CHRE, chreLogLevelToAshLogLevel(level), formatStr, args);
#endif  // CHRE_USE_FARF_LOGGING
  va_end(args);
}
