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

#ifndef CHRE_HOST_LOG_H_
#define CHRE_HOST_LOG_H_

#ifndef LOG_TAG
#define LOG_TAG "CHRE"
#endif

#include "chre/util/log_common.h"

#include <log/log.h>

/**
 * Logs a message to both logcat and stdout. Don't use this directly; prefer one
 * of LOGE, LOGW, etc. to populate the level.
 *
 * @param level log level to pass to ALOG (LOG_ERROR, LOG_WARN, etc.)
 * @param format printf-style format string
 */
#define CHRE_LOG(level, format, ...)                 \
    do {                                             \
        ALOG(level, LOG_TAG, format, ##__VA_ARGS__); \
        printf("%s:%d : " format "\n", __func__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define LOGE(format, ...) CHRE_LOG(LOG_ERROR,   format, ##__VA_ARGS__)
#define LOGW(format, ...) CHRE_LOG(LOG_WARN,    format, ##__VA_ARGS__)
#define LOGI(format, ...) CHRE_LOG(LOG_INFO,    format, ##__VA_ARGS__)
#define LOGD(format, ...) CHRE_LOG(LOG_DEBUG,   format, ##__VA_ARGS__)

#if LOG_NDEBUG
#define LOGV(format, ...) chreLogNull(format, ##__VA_ARGS__)
#else
#define LOGV(format, ...) CHRE_LOG(LOG_VERBOSE, format, ##__VA_ARGS__)
#endif

/**
 * Helper to log a library error with a human-readable version of the provided
 * error code.
 *
 * @param message Error message string to log
 * @param error_code Standard error code number (EINVAL, etc)
 */
#define LOG_ERROR(message, error_code)                              \
    do {                                                            \
        char error_string[64];                                      \
        strerror_r(error_code, error_string, sizeof(error_string)); \
        LOGE("%s: %s (%d)\n", message, error_string, error_code);   \
    } while (0)

#endif  // CHRE_HOST_LOG_H_
