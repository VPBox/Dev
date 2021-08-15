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

#include "chre_api/chre/version.h"

#include "chre/util/macros.h"

static_assert(CHRE_API_VERSION <= UINT32_MAX,
              "API version must fit in 32 bits");

static_assert(CHRE_PATCH_VERSION <= UINT16_MAX,
              "Patch version must fit in 16 bits");

static_assert(CHRE_PLATFORM_ID <= UINT64_MAX,
              "Platform ID must fit in 64 bits");

DLL_EXPORT uint32_t chreGetApiVersion(void) {
  return CHRE_API_VERSION;
}

DLL_EXPORT uint32_t chreGetVersion(void) {
  return chreGetApiVersion() | CHRE_PATCH_VERSION;
}

DLL_EXPORT uint64_t chreGetPlatformId(void) {
  return CHRE_PLATFORM_ID;
}
