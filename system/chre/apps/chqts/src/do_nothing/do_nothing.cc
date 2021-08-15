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

/**
 * The most trivial possible nanoapp.
 *
 * This successfully loads, and does nothing else.  If a CHRE implementation
 * can't load and unload this, there's no point in testing anything else yet.
 */

#include <cstdint>

#include <chre.h>

extern "C" void nanoappHandleEvent(uint32_t /* senderInstanceId */,
                                   uint16_t /* eventType */,
                                   const void* /* eventData */) {
  // Intentionally do nothing.
}

extern "C" bool nanoappStart(void) {
  return true;
}

extern "C" void nanoappEnd(void) {
  // Intentionally do nothing.
}
