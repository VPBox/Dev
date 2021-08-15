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

#ifndef CHRE_PLATFORM_ANDROID_LOG_H_
#define CHRE_PLATFORM_ANDROID_LOG_H_

#include <cstdio>
#include <android/log.h>

#define ANDROID_LOG_TAG "CHRE"

#ifndef __FILENAME__
#define __FILENAME__ __FILE__
#endif

#define CHRE_ANDROID_LOG(level, fmt, ...) \
    __android_log_print(level, ANDROID_LOG_TAG, \
        "%s:%d\t" fmt, __FILENAME__, __LINE__, ##__VA_ARGS__); \

#define LOGE(fmt, ...) \
    CHRE_ANDROID_LOG(ANDROID_LOG_ERROR, fmt, ##__VA_ARGS__)

#define LOGW(fmt, ...) \
    CHRE_ANDROID_LOG(ANDROID_LOG_WARN, fmt, ##__VA_ARGS__)

#define LOGI(fmt, ...) \
    CHRE_ANDROID_LOG(ANDROID_LOG_INFO, fmt, ##__VA_ARGS__)

#define LOGD(fmt, ...) \
    CHRE_ANDROID_LOG(ANDROID_LOG_DEBUG, fmt, ##__VA_ARGS__)

#endif  // CHRE_PLATFORM_ANDROID_LOG_H_
