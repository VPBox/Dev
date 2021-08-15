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

// If the CHRE build variant wants to supply its own static nanoapps, include
// chre/platform/static_nanoapp_init.h in the nanoapp to include and define
// CHRE_STATIC_NANOAPP_INIT with the appropriate parameters (see examples under
// chre/apps/). Then, define the UniquePtr created by CHRE_STATIC_NANOAPP_INIT
// similar to chre/apps/apps.h and add that variable to kStaticNanoappList
// below.
#ifdef CHRE_INCLUDE_DEFAULT_STATIC_NANOAPPS
#include "chre/apps/apps.h"
#endif  // CHRE_INCLUDE_DEFAULT_STATIC_NANOAPPS
#include "chre/core/event_loop_manager.h"
#include "chre/core/static_nanoapps.h"
#include "chre/util/macros.h"

namespace chre {

// The CHRE build variant can supply this macro to override the default list of
// static nanoapps. Most production variants will supply this macro as these
// nanoapps are mostly intended for testing and evaluation purposes. This list
// is supplied empty to ensure that the symbol is avilable for platforms with
// no static nanoapps.
#ifndef CHRE_VARIANT_SUPPLIES_STATIC_NANOAPP_LIST

//! The default list of static nanoapps to load.
const StaticNanoappInitFunction kStaticNanoappList[] = {};

//! The size of the default static nanoapp list.
const size_t kStaticNanoappCount = ARRAY_SIZE(kStaticNanoappList);

#endif  // CHRE_VARIANT_SUPPLIES_STATIC_NANOAPP_LIST

void loadStaticNanoapps() {
  // Compare with zero to allow the compiler to optimize away the loop.
  // Tautological comparisons are not warnings when comparing a const with
  // another const.
  if (kStaticNanoappCount > 0) {
    // Cast the kStaticNanoappCount to size_t to avoid tautological comparison
    // warnings when the kStaticNanoappCount is zero.
    for (size_t i = 0; i < reinterpret_cast<size_t>(kStaticNanoappCount); i++) {
      UniquePtr<Nanoapp> nanoapp = kStaticNanoappList[i]();
      EventLoopManagerSingleton::get()->getEventLoop().startNanoapp(nanoapp);
    }
  }
}

}  // namespace chre
