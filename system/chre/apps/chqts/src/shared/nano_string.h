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

#ifndef _GTS_NANOAPPS_SHARED_NANO_STRING_H_
#define _GTS_NANOAPPS_SHARED_NANO_STRING_H_

#include <cstddef>
#include <cstdint>

namespace nanoapp_testing {

void memset(void *mem, int val, size_t count);

void memcpy(void *dst, const void *src, size_t bytes);

size_t strlen(char const *str);

char *strncpy(char *dest, const char *src, size_t len);

// Does not NUL terminate.  buffer_len must be >= kUint32ToHexAsciiBufferMinLen.
// Example: Given 'value' of 1234, 'buffer' will be filled with "0x000004D2".
void uint32ToHexAscii(char *buffer, size_t buffer_len, uint32_t value);

constexpr size_t kUint32ToHexAsciiBufferMinLen = 10;

}  // namespace nanoapp_testing

#endif  // _GTS_NANOAPPS_SHARED_NANO_STRING_H_
