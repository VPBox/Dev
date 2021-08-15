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

// For this test, we pretend we're a little endian platform, even if we don't
// happen to be.
#define CHRE_NO_ENDIAN_H 1
#define __LITTLE_ENDIAN 0
#define __BIG_ENDIAN 1
#define __BYTE_ORDER __LITTLE_ENDIAN

#include <shared/nano_endian.h>

#include <cstdint>
#include <cstring>

#include <gtest/gtest.h>
#include <shared/array_length.h>


static constexpr uint8_t kLittleEndianRepresentation[4] = {
  0x01, 0x02, 0x03, 0x04 };

TEST(EndianTest, LittleEndianToLittleEndianHost) {
  uint32_t value;
  ::memcpy(&value, kLittleEndianRepresentation, sizeof(value));

  value = nanoapp_testing::littleEndianToHost(value);
  EXPECT_EQ(0, ::memcmp(&value, kLittleEndianRepresentation, sizeof(value)));
}

TEST(EndianTest, LittleEndianHostToLittleEndian) {
  uint32_t value;
  ::memcpy(&value, kLittleEndianRepresentation, sizeof(value));

  value = nanoapp_testing::hostToLittleEndian(value);
  EXPECT_EQ(0, ::memcmp(&value, kLittleEndianRepresentation, sizeof(value)));
}
