/*
 * Copyright (C) 2016, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstdint>
#include <limits>

#include "android-base/stringprintf.h"
#include "gtest/gtest.h"

#include "wifilogd/local_utils.h"

namespace android {
namespace wifilogd {

using local_utils::CastEnumToInteger;
using local_utils::CopyFromBufferOrDie;
using local_utils::GetMaxVal;
using local_utils::IsAsciiPrintable;

TEST(LocalUtilsTest, CastEnumToIntegerWorksForValidEnumValues) {
  enum class basic_enum { VALUE0, VALUE1 };
  EXPECT_EQ(0, CastEnumToInteger(basic_enum::VALUE0));
  EXPECT_EQ(1, CastEnumToInteger(basic_enum::VALUE1));
}

TEST(LocalUtilsTest, CastEnumToIntegerWorksForInvalidEnumValues) {
  enum class basic_enum : int { VALUE0, VALUE1 };
  constexpr int invalid_enum_value = 2;
  EXPECT_EQ(0, CastEnumToInteger(basic_enum::VALUE0));
  EXPECT_EQ(1, CastEnumToInteger(basic_enum::VALUE1));
  EXPECT_EQ(2, CastEnumToInteger(CopyFromBufferOrDie<basic_enum>(
                   &invalid_enum_value, sizeof(invalid_enum_value))));
}

TEST(LocalUtilsTest, CopyFromBufferOrDieCopiesData) {
  struct Message {
    int a;
    char b;
  };
  const Message original{5, 'c'};
  const auto& duplicate =
      CopyFromBufferOrDie<Message>(&original, sizeof(original));
  EXPECT_EQ(original.a, duplicate.a);
  EXPECT_EQ(original.b, duplicate.b);
}

TEST(LocalUtilsTest, GetMaxValFromTypeIsCorrectForUnsignedTypes) {
  EXPECT_EQ(std::numeric_limits<uint8_t>::max(), GetMaxVal<uint8_t>());
  EXPECT_EQ(std::numeric_limits<uint16_t>::max(), GetMaxVal<uint16_t>());
  EXPECT_EQ(std::numeric_limits<uint32_t>::max(), GetMaxVal<uint32_t>());
  EXPECT_EQ(std::numeric_limits<uint64_t>::max(), GetMaxVal<uint64_t>());
}

TEST(LocalUtilsTest, GetMaxValFromTypeIsCorrectForSignedTypes) {
  EXPECT_EQ(std::numeric_limits<int8_t>::max(), GetMaxVal<int8_t>());
  EXPECT_EQ(std::numeric_limits<int16_t>::max(), GetMaxVal<int16_t>());
  EXPECT_EQ(std::numeric_limits<int32_t>::max(), GetMaxVal<int32_t>());
  EXPECT_EQ(std::numeric_limits<int64_t>::max(), GetMaxVal<int64_t>());
}

TEST(LocalUtilsTest, GetMaxValFromInstanceIsCorrectForUnsignedTypes) {
  EXPECT_EQ(std::numeric_limits<uint8_t>::max(), GetMaxVal(uint8_t{}));
  EXPECT_EQ(std::numeric_limits<uint16_t>::max(), GetMaxVal(uint16_t{}));
  EXPECT_EQ(std::numeric_limits<uint32_t>::max(), GetMaxVal(uint32_t{}));
  EXPECT_EQ(std::numeric_limits<uint64_t>::max(), GetMaxVal(uint64_t{}));
}

TEST(LocalUtilsTest, GetMaxValFromInstanceIsCorrectForSignedTypes) {
  EXPECT_EQ(std::numeric_limits<int8_t>::max(), GetMaxVal(int8_t{}));
  EXPECT_EQ(std::numeric_limits<int16_t>::max(), GetMaxVal(int16_t{}));
  EXPECT_EQ(std::numeric_limits<int32_t>::max(), GetMaxVal(int32_t{}));
  EXPECT_EQ(std::numeric_limits<int64_t>::max(), GetMaxVal(int64_t{}));
}

TEST(LocalUtilsTest, IsAsciiPrintableReturnsTrueForAlphabeticCharacters) {
  for (const char c : {'a', 'z', 'A', 'Z'}) {
    EXPECT_TRUE(IsAsciiPrintable(c)) << "Failed with: c == '" << c << "'";
  }
}

TEST(LocalUtilsTest, IsAsciiPrintableReturnsTrueForNumericCharacters) {
  for (const char c : {'0', '9'}) {
    EXPECT_TRUE(IsAsciiPrintable(c)) << "Failed with: c == '" << c << "'";
  }
}

TEST(LocalUtilsTest, IsAsciiPrintableReturnsTrueForPrintableSpaces) {
  for (const char c : {'\n', '\t', ' '}) {
    EXPECT_TRUE(IsAsciiPrintable(c))
        << base::StringPrintf("Failed with: c == 0x%02d", c);
  }
}

TEST(LocalUtilsTest, IsAsciiPrintableReturnsTrueForMaximalPrintable) {
  EXPECT_TRUE(IsAsciiPrintable('~'));
}

TEST(LocalUtilsTest, IsAsciiPrintableReturnsFalseForUnprintableSpaces) {
  for (const char c : {'\f', '\r', '\v'}) {
    EXPECT_FALSE(IsAsciiPrintable(c))
        << base::StringPrintf("Failed with: c == 0x%02d", c);
  }
}

TEST(LocalUtilsTest, IsAsciiPrintableReturnsFalseForNeighborsOfPrintables) {
  for (const char c : {char{0x1f}, char{0x7f}}) {
    EXPECT_FALSE(IsAsciiPrintable(c))
        << base::StringPrintf("Failed with: c == 0x%02d", c);
  }
}

TEST(LocalUtilsTest, IsAsciiPrintableReturnsFalseForMinimalAndMaximalChars) {
  using uchar = unsigned char;
  for (const uchar c : {uchar{0x00}, uchar{0xff}}) {
    EXPECT_FALSE(IsAsciiPrintable(c))
        << base::StringPrintf("Failed with: c == 0x%02d", c);
  }
}

TEST(LocalUtilsTest, SafelyClampWorksForSameTypeClamping) {
  EXPECT_EQ(int8_t{0}, (SAFELY_CLAMP(int8_t{-1}, int8_t, 0, 2)));
  EXPECT_EQ(int8_t{0}, (SAFELY_CLAMP(int8_t{0}, int8_t, 0, 2)));
  EXPECT_EQ(int8_t{1}, (SAFELY_CLAMP(int8_t{1}, int8_t, 0, 2)));
  EXPECT_EQ(int8_t{2}, (SAFELY_CLAMP(int8_t{2}, int8_t, 0, 2)));
  EXPECT_EQ(int8_t{2}, (SAFELY_CLAMP(int8_t{3}, int8_t, 0, 2)));
}

TEST(LocalUtilsTest, SafelyClampWorksForSignedToUnsigned) {
  static_assert(std::numeric_limits<int8_t>::max() == 127,
                "upper bound is set incorrectly");
  EXPECT_EQ(uint8_t{0}, (SAFELY_CLAMP(int8_t{-1}, uint8_t, 0, 127)));
  EXPECT_EQ(uint8_t{0}, (SAFELY_CLAMP(int8_t{0}, uint8_t, 0, 127)));
  EXPECT_EQ(uint8_t{1}, (SAFELY_CLAMP(int8_t{1}, uint8_t, 0, 127)));
  EXPECT_EQ(uint8_t{127}, (SAFELY_CLAMP(int8_t{127}, uint8_t, 0, 127)));
}

TEST(LocalUtilsTest, SafelyClampWorksForUnsignedToSigned) {
  static_assert(std::numeric_limits<int8_t>::max() == 127,
                "upper bound is set incorrectly");
  EXPECT_EQ(int8_t{0}, (SAFELY_CLAMP(uint8_t{0}, int8_t, 0, 127)));
  EXPECT_EQ(int8_t{1}, (SAFELY_CLAMP(uint8_t{1}, int8_t, 0, 127)));
  EXPECT_EQ(int8_t{127}, (SAFELY_CLAMP(uint8_t{127}, int8_t, 0, 127)));
  EXPECT_EQ(int8_t{127}, (SAFELY_CLAMP(uint8_t{128}, int8_t, 0, 127)));
}

// Per
// github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#death-tests,
// death tests should be specially named.

TEST(LocalUtilsDeathTest, CopyFromBufferOrDieWithShortBufferCausesDeath) {
  struct Message {
    int a;
    char b;
  };
  const Message original{5, 'c'};
  EXPECT_DEATH((CopyFromBufferOrDie<Message>(&original, sizeof(original) - 1)),
               "Check failed");
}

}  // namespace wifilogd
}  // namespace android
