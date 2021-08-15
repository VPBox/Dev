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

#include <memory>

#include <gtest/gtest.h>

#include "wificond/net/kernel-header-latest/nl80211.h"
#include "wificond/net/nl80211_attribute.h"

namespace android {
namespace wificond {

namespace {

const uint32_t kU8Value1 = 200;
const uint32_t kU16Value1 = 5000;
const uint32_t kU32Value1 = 250000;
const uint32_t kU32Value2 = 500000;
const std::string kIFName = "wlan0";
const uint8_t kMacAddress[] = {
    0xc0, 0x3f, 0x0e, 0x77, 0xe8, 0x7f
};

// This header contains invalid buffer length
const uint8_t kBrokenBuffer[] = {
    0xff, 0xff, // nla_len = 0xffff
    0x01, 0x11, // nla_type
    0x15, 0x12, // payload
    0x00, 0x00  // padding
};
const uint8_t kValidU32AttrBuffer[] = {
    0x08, 0x00, // nla_len = 8
    0x01, 0x00, // nla_type
    0xf1, 0x12, 0x12, 0x2a // payload
};
const uint8_t kBufferContainsStringWithTrailingZero[] = {
    0x0a, 0x00, // nla_len = 10
    0x01, 0x00, // nla_type
    'w', 'l', 'a', 'n', '0', '\0',
    0x00, 0x00  // padding
};
const uint8_t kBufferContainsStringWithTrailingZeros[] = {
    0x0c, 0x00, // nla_len = 12
    0x01, 0x00, // nla_type
    'w', 'l', 'a', 'n', '0', '\0', '\0', '\0'
};
const uint8_t kBufferContainsStringWithoutTrailingZero[] = {
    0x09, 0x00, // nla_len = 9
    0x01, 0x00, // nla_type
    'w', 'l', 'a', 'n', '0',
    0x00, 0x00, 0x00  // padding
};

const uint8_t kBufferContainsListOfAttributes[] = {
    0x28, 0x00, // nla_len = 40
    0x01, 0x00, // nla_type
    // List of attributes:
    // They have attribute id from 0 to N.
    0x0a, 0x00, // nla_len = 10
    0x00, 0x00, // nla_type = 0
    'f', 'i', 'r', 's', 't','\0',
    0x00, 0x00,  // padding
    0x0b, 0x00, // nla_len = 11
    0x01, 0x00, // nla_type = 1
    's', 'e', 'c', 'o', 'n', 'd','\0',
    0x00, // padding
    0x0a, 0x00, // nla_len = 10
    0x02, 0x00, // nla_type = 2
    't', 'h', 'i', 'r', 'd','\0',
    0x00, 0x00, // padding
};

const uint8_t kBufferContainsListOfNestedAttributes[] = {
    0x28, 0x00, // nla_len = 40
    0x01, 0x00, // nla_type

    // List of nested attributes:
    // They have attribute id from 0 to N.

    // Nested attribute 1:
    0x0c, 0x00, // nla_len = 12
    0x00, 0x00, // nla_type = 0
        0x06, 0x00, // nla_len = 6
        0x01, 0x00, // nla_type
        0x05, 0x00, // uint16_t attribute with value 5
        0x00, 0x00, // padding

    // Nested attribute 2:
    0x0c, 0x00, // nla_len = 12
    0x01, 0x00, // nla_type = 1
        0x08, 0x00, // nla_len = 8
        0x01, 0x00, // nla_type
        0x0a, 0x00,
        0x00, 0x00, // uint32_t attribute with value 10

    // Nested attribute 3:
    0x0c, 0x00, // nla_len = 12
    0x02, 0x00, // nla_type = 2
        0x05, 0x00, // nla_len = 5
        0x01, 0x00, // nla_type
        0x08, 0x00, // uint8_t attribute with value 8
        0x00, 0x00, // padding
};

}  // namespace

TEST(NL80211AttributeTest,U8AttributesSeriallizeCorrectly) {
  NL80211Attr<uint8_t> u8_attr(1, kU8Value1);
  EXPECT_EQ(u8_attr.GetValue(), kU8Value1);
}

TEST(NL80211AttributeTest,U16AttributesSeriallizeCorrectly) {
  NL80211Attr<uint16_t> u16_attr(1, kU16Value1);
  EXPECT_EQ(u16_attr.GetValue(), kU16Value1);
}

TEST(NL80211AttributeTest,U32AttributesSeriallizeCorrectly) {
  NL80211Attr<uint32_t> u32_attr(1, kU32Value1);
  EXPECT_EQ(u32_attr.GetValue(), kU32Value1);
}

TEST(NL80211AttributeTest,StringAttributesSeriallizeCorrectly) {
  NL80211Attr<std::string> str_attr(1, kIFName);
  EXPECT_EQ(str_attr.GetValue(), kIFName);
}

TEST(NL80211AttributeTest, ByteVectorsSeriallizeCorrectly) {
  std::vector<uint8_t> mac_address(
      kMacAddress,
      kMacAddress + sizeof(kMacAddress));
  NL80211Attr<std::vector<uint8_t>> byte_vector_attr(1, mac_address);
  EXPECT_EQ(byte_vector_attr.GetValue(), mac_address);
}

TEST(NL80211AttributeTest, CanGetNestedAttributes) {
  NL80211NestedAttr nested_attr(1);
  NL80211Attr<uint32_t> u32_attr_1(1, kU32Value1);
  NL80211Attr<uint32_t> u32_attr_2(2, kU32Value2);

  nested_attr.AddAttribute(u32_attr_1);
  nested_attr.AddAttribute(u32_attr_2);

  EXPECT_TRUE(nested_attr.HasAttribute(1));
  EXPECT_TRUE(nested_attr.HasAttribute(2));

  uint32_t attr_value;
  EXPECT_TRUE(nested_attr.GetAttributeValue(1, &attr_value));
  EXPECT_EQ(attr_value, kU32Value1);
  EXPECT_TRUE(nested_attr.GetAttributeValue(2, &attr_value));
  EXPECT_EQ(attr_value, kU32Value2);
}

TEST(NL80211AttributeTest, CannotGetDoubleNestedAttributes) {
  NL80211NestedAttr nested_attr(1);
  NL80211NestedAttr deeper_nested_attr(2);
  NL80211Attr<uint32_t> u32_attr_1(3, kU32Value1);

  deeper_nested_attr.AddAttribute(u32_attr_1);
  nested_attr.AddAttribute(deeper_nested_attr);

  EXPECT_FALSE(nested_attr.HasAttribute(3));
}

TEST(NL80211AttributeTest, CannotGetMissingAttribute) {
  NL80211NestedAttr nested_attr(1);
  NL80211Attr<uint32_t> u32_attr_1(1, kU32Value1);

  nested_attr.AddAttribute(u32_attr_1);

  uint32_t attr_value;
  EXPECT_FALSE(nested_attr.HasAttribute(2));
  EXPECT_FALSE(nested_attr.GetAttributeValue(2, &attr_value));
}

TEST(NL80211AttributeTest, CannotGetAttributeWithWrongType) {
  NL80211NestedAttr nested_attr(1);
  NL80211Attr<uint32_t> u32_attr_1(1, kU32Value1);

  nested_attr.AddAttribute(u32_attr_1);

  uint16_t attr_value;
  EXPECT_TRUE(nested_attr.HasAttribute(1));
  EXPECT_FALSE(nested_attr.GetAttributeValue(1, &attr_value));
}


TEST(NL80211AttributeTest, InvalidU32AttributeWithEmptyBuffer) {
  std::vector<uint8_t> buffer;
  NL80211Attr<uint32_t> invalid_attr(buffer);
  EXPECT_FALSE(invalid_attr.IsValid());
}

TEST(NL80211AttributeTest, InvalidU32AttributeWithBrokenBuffer) {
  std::vector<uint8_t> buffer(
      kBrokenBuffer,
      kBrokenBuffer + sizeof(kBrokenBuffer));
  NL80211Attr<uint32_t> invalid_attr(buffer);
  EXPECT_FALSE(invalid_attr.IsValid());
}

TEST(NL80211AttributeTest, InvalidU16AttributeWithU32Buffer) {
  std::vector<uint8_t> buffer(
      kValidU32AttrBuffer,
      kValidU32AttrBuffer + sizeof(kValidU32AttrBuffer));
  NL80211Attr<uint32_t> valid_attr(buffer);
  NL80211Attr<uint16_t> invalid_attr(buffer);
  EXPECT_TRUE(valid_attr.IsValid());
  EXPECT_FALSE(invalid_attr.IsValid());
}

TEST(NL80211AttributeTest, InitStringAttributeWithTrailingZeroFromBuffer) {
  std::vector<uint8_t> buffer(
      kBufferContainsStringWithTrailingZero,
      kBufferContainsStringWithTrailingZero +
          sizeof(kBufferContainsStringWithTrailingZero));
  NL80211Attr<std::string> str_attr(buffer);
  EXPECT_EQ("wlan0", str_attr.GetValue());
}

TEST(NL80211AttributeTest, InitStringAttributeWithTrailingZerosFromBuffer) {
  std::vector<uint8_t> buffer(
      kBufferContainsStringWithTrailingZeros,
      kBufferContainsStringWithTrailingZeros +
          sizeof(kBufferContainsStringWithTrailingZeros));
  NL80211Attr<std::string> str_attr(buffer);
  EXPECT_EQ("wlan0", str_attr.GetValue());
}

TEST(NL80211AttributeTest, InitStringAttributeWithoutTrailingZeroFromBuffer) {
  std::vector<uint8_t> buffer(
      kBufferContainsStringWithoutTrailingZero,
      kBufferContainsStringWithoutTrailingZero +
          sizeof(kBufferContainsStringWithoutTrailingZero));
  NL80211Attr<std::string> str_attr(buffer);
  EXPECT_EQ("wlan0", str_attr.GetValue());
}

TEST(NL80211AttributeTest, GetListOfStringsFromBuffer) {
  std::vector<uint8_t> buffer(
      kBufferContainsListOfAttributes,
      kBufferContainsListOfAttributes +
          sizeof(kBufferContainsListOfAttributes));
  std::vector<std::string> strs;
  std::vector<std::string> expected_strs = {"first", "second", "third"};
  NL80211NestedAttr nested_attr(buffer);
  nested_attr.GetListOfAttributeValues(&strs);
  EXPECT_EQ(expected_strs, strs);
}

TEST(NL80211AttributeTest, GetListOfAttributesFromBuffer) {
  std::vector<uint8_t> buffer(
      kBufferContainsListOfAttributes,
      kBufferContainsListOfAttributes +
          sizeof(kBufferContainsListOfAttributes));

  std::vector<NL80211Attr<std::string>> attrs;
  NL80211NestedAttr attr(buffer);
  EXPECT_TRUE(attr.GetListOfAttributes(&attrs));
  EXPECT_TRUE(attrs.size() == 3);
  ASSERT_EQ(0, attrs[0].GetAttributeId());
  ASSERT_EQ(1, attrs[1].GetAttributeId());
  ASSERT_EQ(2, attrs[2].GetAttributeId());
  ASSERT_EQ("first", attrs[0].GetValue());
  ASSERT_EQ("second", attrs[1].GetValue());
  ASSERT_EQ("third", attrs[2].GetValue());
}

TEST(NL80211AttributeTest, GetListOfNestedAttributesFromBuffer) {
  std::vector<uint8_t> buffer(
      kBufferContainsListOfNestedAttributes,
      kBufferContainsListOfNestedAttributes +
          sizeof(kBufferContainsListOfNestedAttributes));
  std::vector<NL80211NestedAttr> nested_attrs;
  NL80211NestedAttr attr(buffer);
  EXPECT_TRUE(attr.GetListOfNestedAttributes(&nested_attrs));
  EXPECT_TRUE(nested_attrs.size() == 3);
  uint16_t value1 = 0;
  uint32_t value2 = 0;
  uint8_t value3 = 0;
  ASSERT_TRUE(nested_attrs[0].GetAttributeValue(1, &value1));
  ASSERT_TRUE(nested_attrs[1].GetAttributeValue(1, &value2));
  ASSERT_TRUE(nested_attrs[2].GetAttributeValue(1, &value3));
  EXPECT_TRUE(value1 == 5);
  EXPECT_TRUE(value2 == 10);
  EXPECT_TRUE(value3 == 8);
}

TEST(NL80211AttributeTest, MergeAttributes) {
  NL80211Attr<std::vector<uint8_t>> attr1(1, {'a', 'b', 'c'});
  NL80211Attr<std::vector<uint8_t>> attr2(1, {'d', 'e'});
  ASSERT_TRUE(attr1.Merge(attr2));
  std::vector<uint8_t> expected_value{{'a', 'b', 'c', 'd', 'e'}};
  EXPECT_EQ(expected_value, attr1.GetValue());
}

TEST(NL80211AttributeTest, CannotMergeInvalidAttributeWithBrokenBuffer) {
  NL80211Attr<std::vector<uint8_t>> valid_attr(1, {'a', 'b', 'c'});
  std::vector<uint8_t> broken_buffer(
      kBrokenBuffer,
      kBrokenBuffer + sizeof(kBrokenBuffer));
  NL80211Attr<std::vector<uint8_t>> invalid_attr(broken_buffer);
  EXPECT_FALSE(valid_attr.Merge(invalid_attr));
}

TEST(NL80211AttributeTest, CannotMergeAttributesWithDifferentIds) {
  NL80211Attr<std::vector<uint8_t>> attr1(1, {'a', 'b', 'c'});
  NL80211Attr<std::vector<uint8_t>> attr2(2, {'d', 'e', 'f'});
  EXPECT_FALSE(attr1.Merge(attr2));
}

TEST(NL80211AttributeTest, MergeNestedAttributes) {
  NL80211NestedAttr nested_attr1(0);
  NL80211NestedAttr nested_attr2(0);
  NL80211Attr<uint32_t> uint32_attr1(1, kU32Value1);
  NL80211Attr<uint32_t> uint32_attr2(2, kU32Value2);
  nested_attr1.AddAttribute(uint32_attr1);
  nested_attr2.AddAttribute(uint32_attr2);
  ASSERT_TRUE(nested_attr1.Merge(nested_attr2));

  uint32_t value1, value2;
  EXPECT_TRUE(nested_attr1.GetAttributeValue(1, &value1));
  EXPECT_TRUE(value1 == kU32Value1);
  EXPECT_TRUE(nested_attr1.GetAttributeValue(2, &value2));
  EXPECT_TRUE(value2 == kU32Value2);
}

}  // namespace wificond
}  // namespace android
