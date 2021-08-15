/******************************************************************************
 *
 *  Copyright 2017 The Android Open Source Project
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

#include "raw_address.h"

static const char* test_addr = "12:34:56:78:9a:bc";
static const char* test_addr2 = "cb:a9:87:65:43:21";

TEST(RawAddressUnittest, test_constructor_array) {
  RawAddress bdaddr({0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc});

  ASSERT_EQ(0x12, bdaddr.address[0]);
  ASSERT_EQ(0x34, bdaddr.address[1]);
  ASSERT_EQ(0x56, bdaddr.address[2]);
  ASSERT_EQ(0x78, bdaddr.address[3]);
  ASSERT_EQ(0x9A, bdaddr.address[4]);
  ASSERT_EQ(0xBC, bdaddr.address[5]);

  std::string ret = bdaddr.ToString();

  ASSERT_STREQ(test_addr, ret.c_str());
}

TEST(RawAddressUnittest, test_is_empty) {
  RawAddress empty;
  RawAddress::FromString("00:00:00:00:00:00", empty);
  ASSERT_TRUE(empty.IsEmpty());

  RawAddress not_empty;
  RawAddress::FromString("00:00:00:00:00:01", not_empty);
  ASSERT_FALSE(not_empty.IsEmpty());
}

TEST(RawAddressUnittest, test_to_from_str) {
  RawAddress bdaddr;
  RawAddress::FromString(test_addr, bdaddr);

  ASSERT_EQ(0x12, bdaddr.address[0]);
  ASSERT_EQ(0x34, bdaddr.address[1]);
  ASSERT_EQ(0x56, bdaddr.address[2]);
  ASSERT_EQ(0x78, bdaddr.address[3]);
  ASSERT_EQ(0x9A, bdaddr.address[4]);
  ASSERT_EQ(0xBC, bdaddr.address[5]);

  std::string ret = bdaddr.ToString();

  ASSERT_STREQ(test_addr, ret.c_str());
}

TEST(RawAddressUnittest, test_from_octets) {
  static const uint8_t test_addr_array[] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc};

  RawAddress bdaddr;
  size_t expected_result = RawAddress::kLength;
  ASSERT_EQ(expected_result, bdaddr.FromOctets(test_addr_array));

  ASSERT_EQ(0x12, bdaddr.address[0]);
  ASSERT_EQ(0x34, bdaddr.address[1]);
  ASSERT_EQ(0x56, bdaddr.address[2]);
  ASSERT_EQ(0x78, bdaddr.address[3]);
  ASSERT_EQ(0x9A, bdaddr.address[4]);
  ASSERT_EQ(0xBC, bdaddr.address[5]);

  std::string ret = bdaddr.ToString();

  ASSERT_STREQ(test_addr, ret.c_str());
}

TEST(RawAddressTest, test_equals) {
  RawAddress bdaddr1;
  RawAddress bdaddr2;
  RawAddress bdaddr3;
  RawAddress::FromString(test_addr, bdaddr1);
  RawAddress::FromString(test_addr, bdaddr2);
  EXPECT_TRUE(bdaddr1 == bdaddr2);
  EXPECT_FALSE(bdaddr1 != bdaddr2);
  EXPECT_TRUE(bdaddr1 == bdaddr1);
  EXPECT_FALSE(bdaddr1 != bdaddr1);

  RawAddress::FromString(test_addr2, bdaddr3);
  EXPECT_FALSE(bdaddr2 == bdaddr3);
  EXPECT_TRUE(bdaddr2 != bdaddr3);
}

TEST(RawAddressTest, test_less_than) {
  RawAddress bdaddr1;
  RawAddress bdaddr2;
  RawAddress bdaddr3;
  RawAddress::FromString(test_addr, bdaddr1);
  RawAddress::FromString(test_addr, bdaddr2);
  EXPECT_FALSE(bdaddr1 < bdaddr2);
  EXPECT_FALSE(bdaddr1 < bdaddr1);

  RawAddress::FromString(test_addr2, bdaddr3);
  EXPECT_TRUE(bdaddr2 < bdaddr3);
  EXPECT_FALSE(bdaddr3 < bdaddr2);
}

TEST(RawAddressTest, test_more_than) {
  RawAddress bdaddr1;
  RawAddress bdaddr2;
  RawAddress bdaddr3;
  RawAddress::FromString(test_addr, bdaddr1);
  RawAddress::FromString(test_addr, bdaddr2);
  EXPECT_FALSE(bdaddr1 > bdaddr2);
  EXPECT_FALSE(bdaddr1 > bdaddr1);

  RawAddress::FromString(test_addr2, bdaddr3);
  EXPECT_FALSE(bdaddr2 > bdaddr3);
  EXPECT_TRUE(bdaddr3 > bdaddr2);
}

TEST(RawAddressTest, test_less_than_or_equal) {
  RawAddress bdaddr1;
  RawAddress bdaddr2;
  RawAddress bdaddr3;
  RawAddress::FromString(test_addr, bdaddr1);
  RawAddress::FromString(test_addr, bdaddr2);
  EXPECT_TRUE(bdaddr1 <= bdaddr2);
  EXPECT_TRUE(bdaddr1 <= bdaddr1);

  RawAddress::FromString(test_addr2, bdaddr3);
  EXPECT_TRUE(bdaddr2 <= bdaddr3);
  EXPECT_FALSE(bdaddr3 <= bdaddr2);
}

TEST(RawAddressTest, test_more_than_or_equal) {
  RawAddress bdaddr1;
  RawAddress bdaddr2;
  RawAddress bdaddr3;
  RawAddress::FromString(test_addr, bdaddr1);
  RawAddress::FromString(test_addr, bdaddr2);
  EXPECT_TRUE(bdaddr1 >= bdaddr2);
  EXPECT_TRUE(bdaddr1 >= bdaddr1);

  RawAddress::FromString(test_addr2, bdaddr3);
  EXPECT_FALSE(bdaddr2 >= bdaddr3);
  EXPECT_TRUE(bdaddr3 >= bdaddr2);
}

TEST(RawAddressTest, test_copy) {
  RawAddress bdaddr1;
  RawAddress bdaddr2;
  RawAddress::FromString(test_addr, bdaddr1);
  bdaddr2 = bdaddr1;

  EXPECT_TRUE(bdaddr1 == bdaddr2);
}

TEST(RawAddressTest, IsValidAddress) {
  EXPECT_FALSE(RawAddress::IsValidAddress(""));
  EXPECT_FALSE(RawAddress::IsValidAddress("000000000000"));
  EXPECT_FALSE(RawAddress::IsValidAddress("00:00:00:00:0000"));
  EXPECT_FALSE(RawAddress::IsValidAddress("00:00:00:00:00:0"));
  EXPECT_FALSE(RawAddress::IsValidAddress("00:00:00:00:00:0;"));
  EXPECT_TRUE(RawAddress::IsValidAddress("00:00:00:00:00:00"));
  EXPECT_TRUE(RawAddress::IsValidAddress("AB:cd:00:00:00:00"));
  EXPECT_FALSE(RawAddress::IsValidAddress("aB:cD:eF:Gh:iJ:Kl"));
}

TEST(RawAddressTest, BdAddrFromString) {
  RawAddress addr;
  memset(&addr, 0, sizeof(addr));

  EXPECT_TRUE(RawAddress::FromString("00:00:00:00:00:00", addr));
  const RawAddress result0 = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
  EXPECT_EQ(0, memcmp(&addr, &result0, sizeof(addr)));

  EXPECT_TRUE(RawAddress::FromString("ab:01:4C:d5:21:9f", addr));
  const RawAddress result1 = {{0xab, 0x01, 0x4c, 0xd5, 0x21, 0x9f}};
  EXPECT_EQ(0, memcmp(&addr, &result1, sizeof(addr)));
}
