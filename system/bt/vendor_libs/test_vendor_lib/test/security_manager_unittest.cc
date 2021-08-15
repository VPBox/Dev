/******************************************************************************
 *
 *  Copyright 2017 Google, Inc.
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
#include <string>
#include <vector>
using std::vector;

#include "model/controller/security_manager.h"

namespace {
const std::string kTestAddr1 = "12:34:56:78:9a:bc";
const std::string kTestAddr2 = "cb:a9:87:65:43:21";
const std::string kTestAddr3 = "cb:a9:56:78:9a:bc";
const std::string kTestAddr4 = "12:34:56:78:9a:bc";
const vector<uint8_t> kZeros_octets = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const vector<uint8_t> kTestAddr1_octets = {0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12};
const vector<uint8_t> kTestKey = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                  0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};
}  // namespace

namespace test_vendor_lib {

class SecurityManagerTest : public ::testing::Test {
 public:
  SecurityManagerTest() {}
  ~SecurityManagerTest() {}
};

TEST_F(SecurityManagerTest, WriteKey) {
  Address btaddr1;
  Address btaddr2;
  Address btaddr3;
  Address btaddr4;
  SecurityManager sm(3);

  Address::FromString(kTestAddr1, btaddr1);
  Address::FromString(kTestAddr2, btaddr2);
  Address::FromString(kTestAddr3, btaddr3);
  Address::FromString(kTestAddr4, btaddr4);

  EXPECT_EQ(1, sm.WriteKey(btaddr1, kTestKey));
  EXPECT_EQ(1, sm.WriteKey(btaddr2, kTestKey));
  EXPECT_EQ(1, sm.WriteKey(btaddr3, kTestKey));
  // Key storage is full.
  EXPECT_EQ(0, sm.WriteKey(btaddr4, kTestKey));
}

TEST_F(SecurityManagerTest, ReadKey) {
  Address btaddr1;
  Address btaddr2;
  Address btaddr3;
  SecurityManager sm(3);

  Address::FromString(kTestAddr1, btaddr1);
  Address::FromString(kTestAddr2, btaddr2);
  Address::FromString(kTestAddr3, btaddr3);

  sm.WriteKey(btaddr1, kTestKey);
  sm.WriteKey(btaddr2, kTestKey);

  EXPECT_EQ(1, sm.ReadKey(btaddr1));
  EXPECT_EQ(1, sm.ReadKey(btaddr2));
  EXPECT_EQ(0, sm.ReadKey(btaddr3));
}

TEST_F(SecurityManagerTest, ReadAllKeys) {
  Address btaddr1;
  Address btaddr2;
  SecurityManager sm(3);

  EXPECT_EQ(0, sm.ReadAllKeys());

  Address::FromString(kTestAddr1, btaddr1);
  Address::FromString(kTestAddr2, btaddr1);
  Address::FromString(kTestAddr3, btaddr1);

  sm.WriteKey(btaddr1, kTestKey);

  EXPECT_EQ(1, sm.ReadAllKeys());

  sm.WriteKey(btaddr2, kTestKey);

  EXPECT_EQ(2, sm.ReadAllKeys());
}

TEST_F(SecurityManagerTest, DeleteKey) {
  Address btaddr1;
  Address btaddr2;
  Address btaddr3;
  SecurityManager sm(3);

  Address::FromString(kTestAddr1, btaddr1);
  Address::FromString(kTestAddr2, btaddr2);
  Address::FromString(kTestAddr3, btaddr3);

  EXPECT_EQ(0, sm.DeleteKey(btaddr2));

  sm.WriteKey(btaddr1, kTestKey);

  EXPECT_EQ(1, sm.ReadAllKeys());

  EXPECT_EQ(1, sm.DeleteKey(btaddr1));

  EXPECT_EQ(0, sm.ReadAllKeys());

  sm.WriteKey(btaddr1, kTestKey);
  sm.WriteKey(btaddr2, kTestKey);

  EXPECT_EQ(2, sm.ReadAllKeys());

  EXPECT_EQ(1, sm.DeleteKey(btaddr2));

  EXPECT_EQ(1, sm.ReadAllKeys());

  EXPECT_EQ(0, sm.DeleteKey(btaddr2));

  EXPECT_EQ(1, sm.ReadAllKeys());
}

TEST_F(SecurityManagerTest, DeleteAllKeys) {
  Address btaddr1;
  Address btaddr2;
  Address btaddr3;
  SecurityManager sm(3);

  Address::FromString(kTestAddr1, btaddr1);
  Address::FromString(kTestAddr2, btaddr2);
  Address::FromString(kTestAddr3, btaddr3);

  EXPECT_EQ(0, sm.DeleteAllKeys());

  sm.WriteKey(btaddr1, kTestKey);

  EXPECT_EQ(1, sm.ReadAllKeys());

  EXPECT_EQ(1, sm.DeleteAllKeys());

  EXPECT_EQ(0, sm.ReadAllKeys());

  sm.WriteKey(btaddr1, kTestKey);
  sm.WriteKey(btaddr2, kTestKey);

  EXPECT_EQ(2, sm.ReadAllKeys());

  EXPECT_EQ(2, sm.DeleteAllKeys());

  EXPECT_EQ(0, sm.ReadAllKeys());
}

}  // namespace test_vendor_lib
