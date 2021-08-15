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

#include "address.h"

static const char* test_addr = "bc:9a:78:56:34:12";
static const char* test_addr2 = "21:43:65:87:a9:cb";

TEST(AddressUnittest, test_constructor_array) {
  Address bdaddr({0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc});

  ASSERT_EQ(0x12, bdaddr.address[0]);
  ASSERT_EQ(0x34, bdaddr.address[1]);
  ASSERT_EQ(0x56, bdaddr.address[2]);
  ASSERT_EQ(0x78, bdaddr.address[3]);
  ASSERT_EQ(0x9A, bdaddr.address[4]);
  ASSERT_EQ(0xBC, bdaddr.address[5]);

  std::string ret = bdaddr.ToString();

  ASSERT_STREQ(test_addr, ret.c_str());
}

TEST(AddressUnittest, test_is_empty) {
  Address empty;
  Address::FromString("00:00:00:00:00:00", empty);
  ASSERT_TRUE(empty.IsEmpty());

  Address not_empty;
  Address::FromString("00:00:00:00:00:01", not_empty);
  ASSERT_FALSE(not_empty.IsEmpty());
}

TEST(AddressUnittest, test_to_from_str) {
  Address bdaddr;
  Address::FromString(test_addr, bdaddr);

  ASSERT_EQ(0x12, bdaddr.address[0]);
  ASSERT_EQ(0x34, bdaddr.address[1]);
  ASSERT_EQ(0x56, bdaddr.address[2]);
  ASSERT_EQ(0x78, bdaddr.address[3]);
  ASSERT_EQ(0x9A, bdaddr.address[4]);
  ASSERT_EQ(0xBC, bdaddr.address[5]);

  std::string ret = bdaddr.ToString();

  ASSERT_STREQ(test_addr, ret.c_str());
}

TEST(AddressUnittest, test_from_octets) {
  static const uint8_t test_addr_array[] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc};

  Address bdaddr;
  size_t expected_result = Address::kLength;
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

TEST(AddressTest, test_equals) {
  Address bdaddr1;
  Address bdaddr2;
  Address bdaddr3;
  Address::FromString(test_addr, bdaddr1);
  Address::FromString(test_addr, bdaddr2);
  EXPECT_TRUE(bdaddr1 == bdaddr2);
  EXPECT_FALSE(bdaddr1 != bdaddr2);
  EXPECT_TRUE(bdaddr1 == bdaddr1);
  EXPECT_FALSE(bdaddr1 != bdaddr1);

  Address::FromString(test_addr2, bdaddr3);
  EXPECT_FALSE(bdaddr2 == bdaddr3);
  EXPECT_TRUE(bdaddr2 != bdaddr3);
}

TEST(AddressTest, test_less_than) {
  Address bdaddr1;
  Address bdaddr2;
  Address bdaddr3;
  Address::FromString(test_addr, bdaddr1);
  Address::FromString(test_addr, bdaddr2);
  EXPECT_FALSE(bdaddr1 < bdaddr2);
  EXPECT_FALSE(bdaddr1 < bdaddr1);

  Address::FromString(test_addr2, bdaddr3);
  EXPECT_TRUE(bdaddr2 < bdaddr3);
  EXPECT_FALSE(bdaddr3 < bdaddr2);
}

TEST(AddressTest, test_more_than) {
  Address bdaddr1;
  Address bdaddr2;
  Address bdaddr3;
  Address::FromString(test_addr, bdaddr1);
  Address::FromString(test_addr, bdaddr2);
  EXPECT_FALSE(bdaddr1 > bdaddr2);
  EXPECT_FALSE(bdaddr1 > bdaddr1);

  Address::FromString(test_addr2, bdaddr3);
  EXPECT_FALSE(bdaddr2 > bdaddr3);
  EXPECT_TRUE(bdaddr3 > bdaddr2);
}

TEST(AddressTest, test_less_than_or_equal) {
  Address bdaddr1;
  Address bdaddr2;
  Address bdaddr3;
  Address::FromString(test_addr, bdaddr1);
  Address::FromString(test_addr, bdaddr2);
  EXPECT_TRUE(bdaddr1 <= bdaddr2);
  EXPECT_TRUE(bdaddr1 <= bdaddr1);

  Address::FromString(test_addr2, bdaddr3);
  EXPECT_TRUE(bdaddr2 <= bdaddr3);
  EXPECT_FALSE(bdaddr3 <= bdaddr2);
}

TEST(AddressTest, test_more_than_or_equal) {
  Address bdaddr1;
  Address bdaddr2;
  Address bdaddr3;
  Address::FromString(test_addr, bdaddr1);
  Address::FromString(test_addr, bdaddr2);
  EXPECT_TRUE(bdaddr1 >= bdaddr2);
  EXPECT_TRUE(bdaddr1 >= bdaddr1);

  Address::FromString(test_addr2, bdaddr3);
  EXPECT_FALSE(bdaddr2 >= bdaddr3);
  EXPECT_TRUE(bdaddr3 >= bdaddr2);
}

TEST(AddressTest, test_copy) {
  Address bdaddr1;
  Address bdaddr2;
  Address::FromString(test_addr, bdaddr1);
  bdaddr2 = bdaddr1;

  EXPECT_TRUE(bdaddr1 == bdaddr2);
}

TEST(AddressTest, IsValidAddress) {
  EXPECT_FALSE(Address::IsValidAddress(""));
  EXPECT_FALSE(Address::IsValidAddress("000000000000"));
  EXPECT_FALSE(Address::IsValidAddress("00:00:00:00:0000"));
  EXPECT_FALSE(Address::IsValidAddress("00:00:00:00:00:0"));
  EXPECT_FALSE(Address::IsValidAddress("00:00:00:00:00:0;"));
  EXPECT_TRUE(Address::IsValidAddress("00:00:00:00:00:00"));
  EXPECT_TRUE(Address::IsValidAddress("AB:cd:00:00:00:00"));
  EXPECT_FALSE(Address::IsValidAddress("aB:cD:eF:Gh:iJ:Kl"));
}

TEST(AddressTest, BdAddrFromString) {
  Address addr;
  memset(&addr, 0, sizeof(addr));

  EXPECT_TRUE(Address::FromString("00:00:00:00:00:00", addr));
  const Address result0 = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
  EXPECT_EQ(0, memcmp(&addr, &result0, sizeof(addr)));

  EXPECT_TRUE(Address::FromString("ab:01:4C:d5:21:9f", addr));
  const Address result1 = {{0x9f, 0x21, 0xd5, 0x4c, 0x01, 0xab}};
  EXPECT_EQ(0, memcmp(&addr, &result1, sizeof(addr)));
}

TEST(AddressTest, BdAddrFromStringToStringEquivalent) {
  std::string address = "c1:c2:c3:d1:d2:d3";
  Address addr;

  EXPECT_TRUE(Address::FromString(address, addr));
  EXPECT_EQ(addr.ToString(), address);
}
