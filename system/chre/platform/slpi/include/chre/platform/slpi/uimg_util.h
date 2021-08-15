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

#ifndef CHRE_PLATFORM_SLPI_UIMG_UTIL_H_
#define CHRE_PLATFORM_SLPI_UIMG_UTIL_H_

namespace chre {

/**
 * @return true if CHRE on micro-image is supported
 */
constexpr bool isSlpiUimgSupported() {
#ifdef CHRE_SLPI_UIMG_ENABLED
  return true;
#else
  return false;
#endif // CHRE_SLPI_UIMG_ENABLED
}

}  // namespace chre

#endif  // CHRE_PLATFORM_SLPI_UIMG_UTIL_H_
