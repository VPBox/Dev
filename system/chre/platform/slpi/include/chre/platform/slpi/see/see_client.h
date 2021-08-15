/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef CHRE_PLATFORM_SLPI_SEE_SEE_CLIENT_H_
#define CHRE_PLATFORM_SLPI_SEE_SEE_CLIENT_H_

#include "chre/platform/slpi/see/see_helper.h"
#include "chre/util/singleton.h"

/**
 * @file
 * Exposes the SEE helper used by the platform sensor implementation, for use
 * in other modules.
 */

namespace chre {

//! A singleton instance of SeeHelper that can be used for making sensor
//! requests. This must only be used from the CHRE thread.
typedef Singleton<SeeHelper> SeeHelperSingleton;

/**
 * Convenience method for fetching the SEE helper singleton instance. Must only
 * be used from the CHRE thread.
 */
inline SeeHelper *getSeeHelper() {
  return SeeHelperSingleton::get();
}

#ifdef CHRE_SLPI_UIMG_ENABLED
typedef Singleton<BigImageSeeHelper> BigImageSeeHelperSingleton;

inline SeeHelper *getBigImageSeeHelper() {
  return BigImageSeeHelperSingleton::get();
}
#endif  // CHRE_SLPI_UIMG_ENABLED

}  // namespace chre

#endif  // CHRE_PLATFORM_SLPI_SEE_SEE_CLIENT_H_
