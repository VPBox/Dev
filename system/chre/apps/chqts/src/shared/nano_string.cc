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

#include <shared/nano_string.h>

#ifndef INTERNAL_TESTING

#include <shared/send_message.h>
#define REPORT_INTERNAL_ERROR(msg) \
    sendInternalFailureToHost(msg)

#else

#include <gtest/gtest.h>
#define REPORT_INTERNAL_ERROR(msg) FAIL() << msg

#endif


namespace nanoapp_testing {

void memset(void *mem, int val, size_t count) {
  uint8_t *bytes = static_cast<uint8_t*>(mem);
  for (size_t i = 0; i < count; i++) {
    bytes[i] = static_cast<uint8_t>(val);
  }
}

void memcpy(void *d, const void *s, size_t bytes) {
  uint8_t *dst = static_cast<uint8_t*>(d);
  const uint8_t *src = static_cast<const uint8_t*>(s);
  for (size_t i = 0; i < bytes; i++) {
    dst[i] = src[i];
  }
}

size_t strlen(char const *str) {
  size_t ret = 0;
  for (; str[ret] != '\0'; ret++) {}
  return ret;
}

char *strncpy(char *dest, const char *src, size_t len) {
  size_t i;
  for (i = 0; (i < len) && (src[i] != '\0'); i++) {
    dest[i] = src[i];
  }
  for (; i < len; i++) {
    dest[i] = '\0';
  }
  return dest;
}

void uint32ToHexAscii(char *buffer, size_t buffer_len, uint32_t value) {
  constexpr char lookup[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
  if (buffer_len < kUint32ToHexAsciiBufferMinLen) {
    // We chose not to send our buffer_len here, as that would invoke
    // another call to this method and risk infinite recursion if something
    // was really screwed up.
    REPORT_INTERNAL_ERROR("uint32ToHexAscii got undersized buffer_len");
    return;
  }
  buffer[0] = '0';
  buffer[1] = 'x';
  for (size_t i = 0, shift = 28; i < 8; i++, shift -= 4) {
    buffer[2 + i] = lookup[(value >> shift) & 0xF];
  }
}

}  // namespace nanoapp_testing
