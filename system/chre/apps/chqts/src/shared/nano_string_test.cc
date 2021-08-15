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

#include <string.h>  // brings strlen, strncpy, and memset into scope.
#include <stdint.h>

#include <gtest/gtest.h>
#include <shared/array_length.h>

// This "using directive" intentionally makes the symbols 'strlen', 'strncpy',
// and 'memset' "ambigious" at this point.  This means that every use of these
// needs to be fully qualified in our tests below.  That's as desired for
// clarity and to avoid accidentally invoking the wrong version.
// Note that a leading bare "::" is the fully qualified version of the
// C library methods.
using namespace nanoapp_testing;

static constexpr size_t kMemsetBufferLen = 16;
static constexpr int kUnsetValue = 0x5F;
static constexpr int kNewValue   = 0xB8;

template<size_t kLenToSet>
static void testMemset() {
  uint8_t expected[kMemsetBufferLen];
  uint8_t actual[arrayLength(expected)];

  static_assert(kLenToSet <= arrayLength(expected), "Bad test invocation");

  ::memset(expected, kUnsetValue, sizeof(expected));
  ::memset(actual, kUnsetValue, sizeof(actual));

  ::memset(expected, kNewValue, kLenToSet);
  nanoapp_testing::memset(actual, kNewValue, kLenToSet);

  EXPECT_EQ(0, memcmp(expected, actual, sizeof(expected)));
}

TEST(NanoStringsTest, MemsetZeroBytes) {
  testMemset<0>();
}

TEST(NanoStringsTest, MemsetPartialArray) {
  testMemset<(kMemsetBufferLen / 2) - 1>();
}

TEST(NanoStringsTest, MemsetFullArray) {
  testMemset<kMemsetBufferLen>();
}


static constexpr size_t kMemcpyBufferLen = 8;
static constexpr uint8_t kMemcpySrc[kMemcpyBufferLen] = {
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };

template<size_t kLenToCopy>
static void testMemcpy() {
  uint8_t expected[arrayLength(kMemcpySrc)];
  uint8_t actual[arrayLength(expected)];

  static_assert(kLenToCopy <= arrayLength(kMemcpySrc), "Bad test invocation");

  ::memset(expected, kUnsetValue, sizeof(expected));
  ::memset(actual, kUnsetValue, sizeof(actual));

  ::memcpy(expected, kMemcpySrc, kLenToCopy);
  nanoapp_testing::memcpy(actual, kMemcpySrc, kLenToCopy);

  EXPECT_EQ(0, memcmp(expected, actual, sizeof(expected)));
}

TEST(NanoStringsTest, MemcpyZeroBytes) {
  testMemcpy<0>();
}

TEST(NanoStringsTest, MemcpyPartialArray) {
  testMemcpy<(kMemcpyBufferLen / 2) - 1>();
}

TEST(NanoStringsTest, MemcpyFullArray) {
  testMemcpy<kMemcpyBufferLen>();
}


TEST(NanoStringsTest, StrlenEmptyString) {
  const char *str = "";
  EXPECT_EQ(::strlen(str), nanoapp_testing::strlen(str));
}

TEST(NanoStringsTest, StrlenNormal) {
  const char *str = "random string\n";
  EXPECT_EQ(::strlen(str), nanoapp_testing::strlen(str));
}

static constexpr size_t kStrncpyMax = 10;
static constexpr char kShortString[] = "short";
static constexpr char kLongString[] = "Kind of long string";
static constexpr char kExactString[] = "0123456789";

static void testStrncpy(const char *str, size_t len) {
  char expected[kStrncpyMax];
  char actual[arrayLength(expected)];

  ::memset(expected, kUnsetValue, sizeof(expected));
  ::memset(actual, kUnsetValue, sizeof(actual));

  ::strncpy(expected, str, len);
  nanoapp_testing::strncpy(actual, str, len);

  EXPECT_EQ(0, memcmp(expected, actual, sizeof(expected)));
}

TEST(NanoStringsTest, Strncpy) {
  testStrncpy(kShortString, ::strlen(kShortString));
}

TEST(NanoStringsTest, StrncpySetsTrailingBytes) {
  ASSERT_LT(::strlen(kShortString), kStrncpyMax);
  testStrncpy(kShortString, kStrncpyMax);
}

TEST(NanoStringsTest, StrncpyMax) {
  ASSERT_GT(::strlen(kLongString), kStrncpyMax);
  testStrncpy(kLongString, kStrncpyMax);
}

TEST(NanoStringsTest, StrncpyNothing) {
  testStrncpy(kLongString, 0);
}

TEST(NanoStringsTest, StrncpyExactFit) {
  ASSERT_EQ(::strlen(kExactString), kStrncpyMax);
  testStrncpy(kExactString, kStrncpyMax);
}


static void testHexAscii(uint32_t value, const char *str) {
  static constexpr size_t kAsciiLen =
      nanoapp_testing::kUint32ToHexAsciiBufferMinLen;

  char array[kAsciiLen + 1];
  array[kAsciiLen] = kUnsetValue;
  uint32ToHexAscii(array, sizeof(array), value);
  EXPECT_EQ(kUnsetValue, array[kAsciiLen]);
  array[kAsciiLen] = '\0';
  EXPECT_STREQ(str, array);
}

TEST(NanoStringsTest, Uint32ToHexAscii) {
  testHexAscii(0x1234ABCD, "0x1234ABCD");
}

TEST(NanoStringsTest, Uint32ToHexAsciiMin) {
  testHexAscii(0, "0x00000000");
}

TEST(NanoStringsTest, Uint32ToHexAsciiMax) {
  testHexAscii(0xFFFFFFFF, "0xFFFFFFFF");
}

