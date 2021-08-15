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

#ifndef CHRE_PLATFORM_SLPI_STATIC_NANOAPP_INIT_H_
#define CHRE_PLATFORM_SLPI_STATIC_NANOAPP_INIT_H_

#include "chre/core/static_nanoapps.h"
#include "chre/platform/fatal_error.h"
#include "chre/platform/shared/nanoapp_support_lib_dso.h"
#include "chre/platform/slpi/uimg_util.h"

/**
 * Initializes a static nanoapp that is based on the SLPI implementation of
 * PlatformNanoappBase.
 *
 * @param appName the name of the nanoapp. This will be prefixed by gNanoapp
 * when creating the global instance of the nanoapp.
 * @param appId the app's unique 64-bit ID
 * @param appVersion the application-defined 32-bit version number
 */
#define CHRE_STATIC_NANOAPP_INIT(appName, appId_, appVersion_) \
namespace chre {                                               \
                                                               \
UniquePtr<Nanoapp> initializeStaticNanoapp##appName() {        \
  UniquePtr<Nanoapp> nanoapp = MakeUnique<Nanoapp>();   \
  static struct chreNslNanoappInfo appInfo;                    \
  appInfo.magic = CHRE_NSL_NANOAPP_INFO_MAGIC;                 \
  appInfo.structMinorVersion =                                 \
    CHRE_NSL_NANOAPP_INFO_STRUCT_MINOR_VERSION;                \
  appInfo.targetApiVersion = CHRE_API_VERSION;                 \
  appInfo.vendor = "Google"; /* TODO: make this configurable */\
  appInfo.name = #appName;                                     \
  appInfo.isSystemNanoapp = true;                              \
  appInfo.isTcmNanoapp = isSlpiUimgSupported();                \
  appInfo.appId = appId_;                                      \
  appInfo.appVersion = appVersion_;                            \
  appInfo.entryPoints.start = nanoappStart;                    \
  appInfo.entryPoints.handleEvent = nanoappHandleEvent;        \
  appInfo.entryPoints.end = nanoappEnd;                        \
  appInfo.appVersionString = "<undefined>";                    \
  if (nanoapp.isNull()) {                                      \
    FATAL_ERROR("Failed to allocate nanoapp " #appName);       \
  } else {                                                     \
    nanoapp->loadStatic(&appInfo);                             \
  }                                                            \
                                                               \
  return nanoapp;                                              \
}                                                              \
                                                               \
}  // namespace chre

#endif  // CHRE_PLATFORM_SLPI_STATIC_NANOAPP_INIT_H_
