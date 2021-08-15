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

#ifndef CHRE_PLATFORM_NANOAPP_DSO_UTIL_H_
#define CHRE_PLATFORM_NANOAPP_DSO_UTIL_H_

#include "chre/platform/shared/nanoapp_support_lib_dso.h"

namespace chre {

/**
 * Performs sanity checks on the app info structure included in a dynamically
 * loaded nanoapp.
 *
 * @param expectedAppId The app ID passed alongside the binary
 * @param expectedAppVersion The app version number passed alongside the binary
 * @param appInfo App info structure included in the nanoapp binary
 *
 * @return true if validation was successful
 */
bool validateAppInfo(uint64_t expectedAppId, uint32_t expectedAppVersion,
                     const struct chreNslNanoappInfo *appInfo);

}  // namespace chre

#endif  // CHRE_PLATFORM_NANOAPP_DSO_UTIL_H_
