/******************************************************************************
 *
 *  Copyright 2019 Google, Inc.
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

#ifndef LOG_TAG
#define LOG_TAG "bt"
#endif

#if defined(OS_ANDROID)

#include <log/log.h>

#define LOG_VERBOSE(fmt, args...) ALOGV("%s: " fmt, __PRETTY_FUNCTION__, ##args)
#define LOG_DEBUG(fmt, args...) ALOGD("%s: " fmt, __PRETTY_FUNCTION__, ##args)
#define LOG_INFO(fmt, args...) ALOGI("%s: " fmt, __PRETTY_FUNCTION__, ##args)
#define LOG_WARN(fmt, args...) ALOGW("%s: " fmt, __PRETTY_FUNCTION__, ##args)
#define LOG_ERROR(fmt, args...) ALOGE("%s: " fmt, __PRETTY_FUNCTION__, ##args)

#else

/* syslog didn't work well here since we would be redefining LOG_DEBUG. */
#include <stdio.h>

#define LOGWRAPPER(fmt, args...) \
  fprintf(stderr, "%s - %s: " fmt "\n", LOG_TAG, __PRETTY_FUNCTION__, ##args)

#define LOG_VERBOSE(...) LOGWRAPPER(__VA_ARGS__)
#define LOG_DEBUG(...) LOGWRAPPER(__VA_ARGS__)
#define LOG_INFO(...) LOGWRAPPER(__VA_ARGS__)
#define LOG_WARN(...) LOGWRAPPER(__VA_ARGS__)
#define LOG_ERROR(...) LOGWRAPPER(__VA_ARGS__)

#endif /* defined(OS_ANDROID) */

#define ASSERT(condition) \
  if (!(condition)) { \
    LOG_ERROR("%s:%d assertion '" #condition "' failed", __FILE__, __LINE__); \
    abort(); \
  }

#define ASSERT_LOG(condition, fmt, args...) \
  if (!(condition)) { \
    LOG_ERROR("%s:%d assertion '" #condition "' failed - " fmt, __FILE__, __LINE__, ##args); \
    abort(); \
  }

