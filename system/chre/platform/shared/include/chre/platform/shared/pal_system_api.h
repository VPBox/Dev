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

#include "chre/pal/system.h"

namespace chre {

/**
 * Memory allocation coming from the PAL. The semantics are the same as malloc.
 * This function needs to be implemented by the platform, and is not provided by
 * the shared code.
 */
void *palSystemApiMemoryAlloc(size_t size);

/**
 * Memory free coming from the PAL. The semantics are the same as free.
 * This function needs to be implemented by the platform, and is not provided by
 * the shared code.
 */
void palSystemApiMemoryFree(void *pointer);

//! Provides a global instance of the PAL system API for all PAL subsystems to
//! leverage.
extern const chrePalSystemApi gChrePalSystemApi;

}  // namespace chre
