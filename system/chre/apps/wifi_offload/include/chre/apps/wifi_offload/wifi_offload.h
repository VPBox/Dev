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

#ifndef CHRE_WIFI_OFFLOAD_WIFI_OFFLOAD_H_
#define CHRE_WIFI_OFFLOAD_WIFI_OFFLOAD_H_

#include <cinttypes>
#include <chre/wifi.h>

#ifdef BUILD_FOR_CHRE_WIFI_OFFLOAD
#include <chre/event.h>
#include <chre/re.h>
#include <chre/version.h>

#define LOG_TAG "[WifiOffload]"

#include "chre/util/dynamic_vector.h"
#ifndef LOGE
#include "chre/util/nanoapp/log.h"
#endif  // LOGE
#include "chre/util/unique_ptr.h"

namespace wifi_offload {
template <typename T>
using Vector = chre::DynamicVector<T>;
}  // namespace wifi_offload

#else  // BUILD_FOR_CHRE_WIFI_OFFLOAD
#include <android/log.h>
#include <vector>

#ifndef LOG_TAG
#define LOG_TAG "[Offload HAL]"
#endif

// Define these to logging functions that are available for offload HAL
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace wifi_offload {
template <typename T>
using Vector = std::vector<T>;
}  // namespace wifi_offload

#endif  // BUILD_FOR_CHRE_WIFI_OFFLOAD

#endif  // CHRE_WIFI_OFFLOAD_WIFI_OFFLOAD_H_
