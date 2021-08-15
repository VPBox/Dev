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

#ifndef NVRAM_CORE_LOGGER_H_
#define NVRAM_CORE_LOGGER_H_

// Numeric constants for log levels.
#define NVRAM_LOG_LEVEL_ERR 1
#define NVRAM_LOG_LEVEL_WARN 2
#define NVRAM_LOG_LEVEL_INFO 3
#define NVRAM_LOG_LEVEL_DEBUG 4

#if !defined(NVRAM_LOG_LEVEL)
// By default, log only warnings and errors.
#define NVRAM_LOG_LEVEL NVRAM_LOG_LEVEL_WARN
#endif

#ifdef __ANDROID__

#define LOG_TAG "NVRAM"
#include <log/log.h>

// Maps NVRAM log levels to Android log priorities.
#define NVRAM_ANDROID_LOG_PRI_ERR ANDROID_LOG_ERROR
#define NVRAM_ANDROID_LOG_PRI_WARN ANDROID_LOG_WARN
#define NVRAM_ANDROID_LOG_PRI_INFO ANDROID_LOG_INFO
#define NVRAM_ANDROID_LOG_PRI_DEBUG ANDROID_LOG_DEBUG

// Send log output to Android's logging system.
#define NVRAM_LOG_EMIT(level, fmt, ...) \
  LOG_PRI(NVRAM_ANDROID_LOG_PRI_##level, LOG_TAG, fmt, ##__VA_ARGS__)

#else  // __ANDROID__

extern "C" {
#include <stdio.h>
}

// By default, send log output to stderr.
#define NVRAM_LOG_EMIT(level, fmt, ...) \
  fprintf(stderr, "NVRAM: " fmt "\n", ##__VA_ARGS__)

#endif  // !__ANDROID__

// NVRAM_LOG is the central log macro. It checks whether the log level is
// effective, adds file and line information and calls the platform-specific
// NVRAM_LOG_EMIT.
#define NVRAM_STR(arg) #arg
#define NVRAM_STRINGIFY(arg) NVRAM_STR(arg)
#define NVRAM_LOG(level, fmt, ...)                                           \
  do {                                                                       \
    if (NVRAM_LOG_LEVEL_##level <= NVRAM_LOG_LEVEL) {                        \
      NVRAM_LOG_EMIT(level, __FILE__ ":" NVRAM_STRINGIFY(__LINE__) ": " fmt, \
                     ##__VA_ARGS__);                                         \
    }                                                                        \
  } while (0)

// Convenience logging macros.
#define NVRAM_LOG_ERR(fmt, ...) NVRAM_LOG(ERR, fmt, ##__VA_ARGS__)
#define NVRAM_LOG_WARN(fmt, ...) NVRAM_LOG(WARN, fmt, ##__VA_ARGS__)
#define NVRAM_LOG_INFO(fmt, ...) NVRAM_LOG(INFO, fmt, ##__VA_ARGS__)
#define NVRAM_LOG_DEBUG(fmt, ...) NVRAM_LOG(DEBUG, fmt, ##__VA_ARGS__)

#endif  // NVRAM_CORE_LOGGER_H_
