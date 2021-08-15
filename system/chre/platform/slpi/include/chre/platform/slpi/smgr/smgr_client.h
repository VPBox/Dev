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

#ifndef CHRE_PLATFORM_SLPI_SMGR_SMGR_CLIENT_H_
#define CHRE_PLATFORM_SLPI_SMGR_SMGR_CLIENT_H_

#include "chre/platform/slpi/smgr/smr_helper.h"
#include "chre/util/singleton.h"

/**
 * @file
 * Exposes the SMR helper and SMGR (non-internal) client handle used by the
 * platform sensor implementation, for use in other modules.
 */

namespace chre {

//! A singleton instance of SmrHelper that can be used for making synchronous
//! sensor requests while remaining in micro-image. This must only be used from
//! the CHRE thread.
typedef Singleton<SmrHelper> SmrHelperSingleton;

/**
 * Convenience method for fetching the SMR helper singleton instance. Must only
 * be used from the CHRE thread.
 */
inline SmrHelper *getSmrHelper() {
  return SmrHelperSingleton::get();
}

/**
 * @return The SMR client handle to the SMGR (non-internal) API, created by the
 *         SLPI platform-specific sensors implementation
 */
smr_client_hndl getSensorServiceSmrClientHandle();

}  // namespace chre

#endif  // CHRE_PLATFORM_SLPI_SMGR_SMGR_CLIENT_H_
