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

#ifndef CHRE_PLATFORM_LINUX_FATAL_ERROR_H_
#define CHRE_PLATFORM_LINUX_FATAL_ERROR_H_

#include <cstdlib>

#define FATAL_ERROR_QUIT() do { \
    chre::preFatalError();      \
    abort();                    \
  } while (0)

namespace chre {

/**
 * Do preparation for an impending fatal error including flushing logs.
 *
 * It must not be possible for FATAL_ERROR() to be called by this function or
 * any of its callees.
 */
void preFatalError();

}  // namespace chre

#endif  // CHRE_PLATFORM_LINUX_FATAL_ERROR_H_
