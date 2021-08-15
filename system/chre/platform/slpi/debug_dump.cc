/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include "chre/platform/slpi/debug_dump.h"

// Some devices don't have ash/debug.h implemented for them so allow swapping
// out that implementation with an empty one until an implementation can be
// provided for all devices.
#ifdef CHRE_ENABLE_DEBUG_DUMP
#include "ash/debug.h"
#endif  // CHRE_ENABLE_DEBUG_DUMP

namespace chre {

// debugDumpTimeoutMs is unused outside of ash/debug.h, but is useful in helping
// understand where the value is defined until ash/debug.h is modified for use
// on all devices.
#ifdef CHRE_ENABLE_DEBUG_DUMP
size_t debugDumpStrMaxSize = ASH_DEBUG_DUMP_STR_MAX_SIZE;
uint16_t debugDumpTimeoutMs = ASH_DEBUG_DUMP_TIMEOUT_MS;
#else  // CHRE_ENABLE_DEBUG_DUMP
size_t debugDumpStrMaxSize = 0;
uint16_t debugDumpTimeoutMs = 0;
#endif  // CHRE_ENABLE_DEBUG_DUMP

bool registerDebugDumpCallback(const char *name, debugDumpCbFunc *callback,
                               void *cookie) {
#ifdef CHRE_ENABLE_DEBUG_DUMP
  return ashRegisterDebugDumpCallback(name, callback, cookie);
#else  // CHRE_ENABLE_DEBUG_DUMP
  return false;
#endif  // CHRE_ENABLE_DEBUG_DUMP
}

void unregisterDebugDumpCallback(debugDumpCbFunc *callback) {
#ifdef CHRE_ENABLE_DEBUG_DUMP
  ashUnregisterDebugDumpCallback(callback);
#endif  // CHRE_ENABLE_DEBUG_DUMP
}

bool commitDebugDump(uint32_t handle, const char *debugStr, bool done) {
#ifdef CHRE_ENABLE_DEBUG_DUMP
  return ashCommitDebugDump(handle, debugStr, done);
#else  // CHRE_ENABLE_DEBUG_DUMP
  return false;
#endif  // CHRE_ENABLE_DEBUG_DUMP
}

bool triggerDebugDump(debugDumpReadyCbFunc *readyCb, void *cookie) {
#ifdef CHRE_ENABLE_DEBUG_DUMP
  return ashTriggerDebugDump(readyCb, cookie);
#else  // CHRE_ENABLE_DEBUG_DUMP
  return false;
#endif  // CHRE_ENABLE_DEBUG_DUMP
}

}  // namespace chre
