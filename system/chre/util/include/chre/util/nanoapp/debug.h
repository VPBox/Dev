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

#ifndef CHRE_UTIL_NANOAPP_DEBUG_H_
#define CHRE_UTIL_NANOAPP_DEBUG_H_

#include <cstddef>
#include <cstdint>

namespace chre {

/**
 * Logs a buffer as LOGD. If this method is invoked from a static nanoapp or the
 * CHRE implementation itself, the CHRE platform LOGD macro is used. If it is
 * invoked by an external nanoapp, the util/nanoapp/log.h LOGD macro is used,
 * which is based upon chreLog.
 *
 * The implementation makes some assumptions about the return value of snprintf
 * which may result in some very bad behavior. This code is not intended to be
 * used in production, just for interactive debugging.
 *
 * @param buffer The buffer to log.
 * @param bufferSize The size of the buffer to log.
 */
void logBuffer(const uint8_t *buffer, size_t bufferSize);

}  // namespace chre

#endif  // CHRE_UTIL_NANOAPP_DEBUG_H_
