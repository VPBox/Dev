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

#include "chre_api/chre/wwan.h"

#include "chre/core/event_loop_manager.h"
#include "chre/util/macros.h"

using chre::EventLoopManager;
using chre::EventLoopManagerSingleton;

DLL_EXPORT uint32_t chreWwanGetCapabilities() {
#ifdef CHRE_WWAN_SUPPORT_ENABLED
  return chre::EventLoopManagerSingleton::get()->getWwanRequestManager()
      .getCapabilities();
#else
  return CHRE_WWAN_CAPABILITIES_NONE;
#endif  // CHRE_WWAN_SUPPORT_ENABLED
}

DLL_EXPORT bool chreWwanGetCellInfoAsync(const void *cookie) {
#ifdef CHRE_WWAN_SUPPORT_ENABLED
  chre::Nanoapp *nanoapp = EventLoopManager::validateChreApiCall(__func__);
  return chre::EventLoopManagerSingleton::get()->getWwanRequestManager()
      .requestCellInfo(nanoapp, cookie);
#else
  return false;
#endif  // CHRE_WWAN_SUPPORT_ENABLED
}
