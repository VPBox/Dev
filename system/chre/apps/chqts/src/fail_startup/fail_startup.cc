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
 * Trivial nanoapp which fails on startup.
 *
 * Non-trivial nanoapp tests may indicate early failure by failing the
 * nanoappStart() method.  This test assures that those failures are
 * properly reported by the CHRE, so we know our other test failures will
 * be caught.
 */

#include <cstdint>

#include <chre.h>

#include <shared/abort.h>

extern "C" void nanoappHandleEvent(uint32_t /* senderInstanceId */,
                                   uint16_t /* eventType */,
                                   const void* /* eventData */) {
  // Intentionally do nothing.
}

extern "C" bool nanoappStart(void) {
  // Return failure
  return false;
}

extern "C" void nanoappEnd(void) {
  // It is an error for the CHRE to call this method.
  nanoapp_testing::abort(nanoapp_testing::AbortBlame::kChreInNanoappEnd);
}
