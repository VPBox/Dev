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

#include "chre/core/init.h"

#include "chre/core/event_loop_manager.h"
#include "chre/platform/system_time.h"
#include "chre/util/singleton.h"

#ifndef CHRE_VERSION_STRING
#define CHRE_VERSION_STRING "undefined"
#endif  // CHRE_VERSION_STRING

//! The CHRE version string. Placed in a variable in the global namespace for
//! easy access with debugging tools.
static const char kChreVersionString[] = CHRE_VERSION_STRING;

namespace chre {

void init() {
  LOGI("CHRE init, version: %s", kChreVersionString);

  SystemTime::init();
  EventLoopManagerSingleton::init();
}

void deinit() {
  EventLoopManagerSingleton::deinit();

  LOGD("CHRE deinit");
}

}  // namespace chre
