/******************************************************************************
 *
 *  Copyright 2014 Google, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#pragma once

/*
 * TODO(armansito): Work-around until we figure out a way to generate logs in a
 * platform-independent manner.
 */
#if defined(OS_GENERIC)

/* syslog didn't work well here since we would be redefining LOG_DEBUG. */
#include <stdio.h>

#define LOGWRAPPER(tag, fmt, args...) \
  fprintf(stderr, "%s: " fmt "\n", tag, ##args)

#define LOG_VERBOSE(...) LOGWRAPPER(__VA_ARGS__)
#define LOG_DEBUG(...) LOGWRAPPER(__VA_ARGS__)
#define LOG_INFO(...) LOGWRAPPER(__VA_ARGS__)
#define LOG_WARN(...) LOGWRAPPER(__VA_ARGS__)
#define LOG_ERROR(...) LOGWRAPPER(__VA_ARGS__)

#define LOG_EVENT_INT(...)

#else /* !defined(OS_GENERIC) */

#include <log/log.h>

/**
 * These log statements are effectively executing only ALOG(_________, tag, fmt,
 * ## args ).
 * fprintf is only to cause compilation error when LOG_TAG is not provided,
 * which breaks build on Linux (for OS_GENERIC).
 */

#if LOG_NDEBUG
#define LOG_VERBOSE(tag, fmt, args...)                          \
  do {                                                          \
    (true) ? ((int)0) : fprintf(stderr, "%s" fmt, tag, ##args); \
  } while (0)
#else  // LOG_NDEBUG
#define LOG_VERBOSE(tag, fmt, args...)               \
  do {                                               \
    (true) ? ALOG(LOG_VERBOSE, tag, fmt, ##args)     \
           : fprintf(stderr, "%s" fmt, tag, ##args); \
  } while (0)
#endif  // !LOG_NDEBUG

#define LOG_DEBUG(tag, fmt, args...)                 \
  do {                                               \
    (true) ? ALOG(LOG_DEBUG, tag, fmt, ##args)       \
           : fprintf(stderr, "%s" fmt, tag, ##args); \
  } while (0)
#define LOG_INFO(tag, fmt, args...)                  \
  do {                                               \
    (true) ? ALOG(LOG_INFO, tag, fmt, ##args)        \
           : fprintf(stderr, "%s" fmt, tag, ##args); \
  } while (0)
#define LOG_WARN(tag, fmt, args...)                  \
  do {                                               \
    (true) ? ALOG(LOG_WARN, tag, fmt, ##args)        \
           : fprintf(stderr, "%s" fmt, tag, ##args); \
  } while (0)
#define LOG_ERROR(tag, fmt, args...)                 \
  do {                                               \
    (true) ? ALOG(LOG_ERROR, tag, fmt, ##args)       \
           : fprintf(stderr, "%s" fmt, tag, ##args); \
  } while (0)

#endif /* defined(OS_GENERIC) */
