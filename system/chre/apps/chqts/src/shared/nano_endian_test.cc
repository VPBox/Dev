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

#include <shared/nano_endian.h>

#include <cstdint>
#include <cstring>

#include <gtest/gtest.h>
#include <shared/array_length.h>


template<size_t kByteCount>
static void swapByteTest() {
  uint8_t bytes[] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
  static constexpr uint8_t postSwap[] = {
    0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09,
    0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01 };

#ifdef __clang__
  // This static_assert crashes g++, but it's legit and works in clang.
  static_assert(arrayLength(bytes) == arrayLength(postSwap),
                "Mismatched arrays");
#endif
  static_assert((kByteCount > 0) && (kByteCount <= arrayLength(bytes)),
                "Invalid test");

  constexpr const uint8_t *kExpected =
      postSwap + (arrayLength(postSwap) - kByteCount);
  nanoapp_testing::swapBytes(bytes, kByteCount);
  EXPECT_EQ(0, ::memcmp(bytes, kExpected, kByteCount));

  if (arrayLength(bytes) < kByteCount) {
    // Confirm that we didn't modify out of bounds.
    EXPECT_EQ(kByteCount + 1, bytes[kByteCount]);
  }
}


TEST(EndianTest, SwapBytes1) {
  swapByteTest<1>();
}

TEST(EndianTest, SwapBytes2) {
  swapByteTest<2>();
}

TEST(EndianTest, SwapBytes4) {
  swapByteTest<4>();
}

TEST(EndianTest, SwapBytes8) {
  swapByteTest<8>();
}

TEST(EndianTest, SwapBytes16) {
  swapByteTest<16>();
}


// These tests should work regardless of which endian platform this
// test happens to be built and running on.

static constexpr uint32_t kValue = UINT32_C(0x04030201);
static constexpr uint8_t kLittleEndianRepresentation[4] = {
  0x01, 0x02, 0x03, 0x04 };

TEST(EndianTest, LittleEndianToHost) {
  uint32_t value;
  ::memcpy(&value, kLittleEndianRepresentation, sizeof(value));

  value = nanoapp_testing::littleEndianToHost(value);
  EXPECT_EQ(kValue, value);
}

TEST(EndianTest, HostToLittleEndian) {
  uint32_t value = kValue;
  value = nanoapp_testing::hostToLittleEndian(value);

  const uint8_t *bytes = reinterpret_cast<uint8_t*>(&value);
  EXPECT_EQ(0, ::memcmp(kLittleEndianRepresentation, bytes,
                        sizeof(kLittleEndianRepresentation)));
}
