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

#include "chre/apps/apps.h"
#include "chre/core/static_nanoapps.h"
#include "chre/util/macros.h"

namespace chre {

//! The default list of static nanoapps to load.
const StaticNanoappInitFunction kStaticNanoappList[] = {
  initializeStaticNanoappAudioWorld,
  initializeStaticNanoappGnssWorld,
  initializeStaticNanoappHelloWorld,
  initializeStaticNanoappMessageWorld,
  initializeStaticNanoappSensorWorld,
  initializeStaticNanoappSpammer,
  initializeStaticNanoappTimerWorld,
  initializeStaticNanoappUnloadTester,
  initializeStaticNanoappWifiWorld,
  initializeStaticNanoappWwanWorld,
};

//! The size of the default static nanoapp list.
const size_t kStaticNanoappCount = ARRAY_SIZE(kStaticNanoappList);

}  // namespace chre
