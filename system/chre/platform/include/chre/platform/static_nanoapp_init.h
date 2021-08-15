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

#ifndef CHRE_PLATFORM_STATIC_NANOAPP_INIT_H_
#define CHRE_PLATFORM_STATIC_NANOAPP_INIT_H_

/**
 * @file
 * Includes the appropriate platform-specific header file that supplies the
 * macro to initialize a static nanoapp. The platform header file must supply
 * the following macro:
 *
 * CHRE_STATIC_NANOAPP_INIT(appName, appId, appVersion)
 *
 * Where appName is the name of a global variable that will be created with type
 * PlatformNanoapp, appId is the app's 64-bit identifier, and appVersion is the
 * application-defined 32-bit version number.
 */

// Since this file is included in nanoapp code, it's likely that the nanoapp log
// macro will have been included, resulting in conflicting definitions of the
// log macros with the ones pulled in via nanoapp.h. Undefine these macros to
// allow their redefinition for CHRE system code.
#ifdef CHRE_UTIL_NANOAPP_LOG_H_
#undef LOGE
#undef LOGW
#undef LOGI
#undef LOGD
#endif // CHRE_UTIL_NANOAPP_LOG_H_

#include "chre/target_platform/static_nanoapp_init.h"

#ifndef CHRE_STATIC_NANOAPP_INIT
#error "CHRE_STATIC_NANOAPP_INIT must be defined by the target platform's static_nanoapp_init.h"
#endif

#endif  // CHRE_PLATFORM_STATIC_NANOAPP_INIT_H_
