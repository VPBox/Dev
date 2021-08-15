/******************************************************************************
 *
 *  Copyright 2018 The Android Open Source Project
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#include <gtest/gtest.h>

#include "class_of_device.h"

static const char* test_class = "efc-d-ab";
static const uint8_t test_bytes[]{0xab, 0xcd, 0xef};

TEST(ClassOfDeviceUnittest, test_constructor_array) {
  ClassOfDevice cod(test_bytes);

  ASSERT_EQ(test_bytes[0], cod.cod[0]);
  ASSERT_EQ(test_bytes[1], cod.cod[1]);
  ASSERT_EQ(test_bytes[2], cod.cod[2]);

  std::string ret = cod.ToString();

  ASSERT_STREQ(test_class, ret.c_str());
}

TEST(ClassOfDeviceUnittest, test_to_from_str) {
  ClassOfDevice cod;
  ClassOfDevice::FromString(test_class, cod);

  ASSERT_EQ(test_bytes[0], cod.cod[0]);
  ASSERT_EQ(test_bytes[1], cod.cod[1]);
  ASSERT_EQ(test_bytes[2], cod.cod[2]);

  std::string ret = cod.ToString();

  ASSERT_STREQ(test_class, ret.c_str());
}

TEST(ClassOfDeviceUnittest, test_from_octets) {
  ClassOfDevice cod;
  size_t expected_result = ClassOfDevice::kLength;
  ASSERT_EQ(expected_result, cod.FromOctets(test_bytes));

  ASSERT_EQ(test_bytes[0], cod.cod[0]);
  ASSERT_EQ(test_bytes[1], cod.cod[1]);
  ASSERT_EQ(test_bytes[2], cod.cod[2]);

  std::string ret = cod.ToString();

  ASSERT_STREQ(test_class, ret.c_str());
}

TEST(ClassOfDeviceTest, test_copy) {
  ClassOfDevice cod1;
  ClassOfDevice cod2;
  ClassOfDevice::FromString(test_class, cod1);
  cod2 = cod1;

  ASSERT_EQ(cod1.cod[0], cod2.cod[0]);
  ASSERT_EQ(cod1.cod[1], cod2.cod[1]);
  ASSERT_EQ(cod1.cod[2], cod2.cod[2]);
}

TEST(ClassOfDeviceTest, IsValid) {
  EXPECT_FALSE(ClassOfDevice::IsValid(""));
  EXPECT_FALSE(ClassOfDevice::IsValid("000000"));
  EXPECT_FALSE(ClassOfDevice::IsValid("00-00-00"));
  EXPECT_FALSE(ClassOfDevice::IsValid("000-0-0"));
  EXPECT_TRUE(ClassOfDevice::IsValid("000-0-00"));
  EXPECT_TRUE(ClassOfDevice::IsValid("ABc-d-00"));
  EXPECT_TRUE(ClassOfDevice::IsValid("aBc-D-eF"));
}

TEST(ClassOfDeviceTest, classOfDeviceFromString) {
  ClassOfDevice cod;

  EXPECT_TRUE(ClassOfDevice::FromString("000-0-00", cod));
  const ClassOfDevice result0 = {{0x00, 0x00, 0x00}};
  EXPECT_EQ(0, memcmp(&cod, &result0, sizeof(cod)));

  EXPECT_TRUE(ClassOfDevice::FromString("ab2-1-4C", cod));
  const ClassOfDevice result1 = {{0x4c, 0x21, 0xab}};
  EXPECT_EQ(0, memcmp(&cod, &result1, sizeof(cod)));
}
