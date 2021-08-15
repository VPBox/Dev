/******************************************************************************
 *
 *  Copyright (C) 2017 Google, Inc.
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

#include <bluetooth/uuid.h>
#include <gtest/gtest.h>

using bluetooth::Uuid;

static const Uuid ONES = Uuid::From128BitBE(
    Uuid::UUID128Bit{{0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11}});

static const Uuid SEQUENTIAL = Uuid::From128BitBE(
    Uuid::UUID128Bit{{0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89}});

constexpr Uuid kBase = Uuid::From128BitBE(
    Uuid::UUID128Bit{{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb}});

TEST(UuidTest, IsEmpty) {
  EXPECT_TRUE(Uuid::kEmpty.IsEmpty());
  EXPECT_FALSE(kBase.IsEmpty());
}

TEST(UuidTest, GetShortestRepresentationSize) {
  EXPECT_TRUE(Uuid::kNumBytes16 == kBase.GetShortestRepresentationSize());
  EXPECT_TRUE(Uuid::kNumBytes32 == Uuid::From32Bit(0x01234567).GetShortestRepresentationSize());
  EXPECT_TRUE(Uuid::kNumBytes128 == Uuid::kEmpty.GetShortestRepresentationSize());
}

TEST(UuidTest, As16Bit) {
  // Even though this is is not 16bit UUID, we should be able to get proper bits
  EXPECT_EQ((uint16_t)0x1111, ONES.As16Bit());
  EXPECT_EQ((uint16_t)0x4567, SEQUENTIAL.As16Bit());
  EXPECT_EQ((uint16_t)0x0000, kBase.As16Bit());
}

TEST(UuidTest, As32Bit) {
  // Even though this is is not 32bit UUID, we should be able to get proper bits
  EXPECT_EQ((uint32_t)0x11111111, ONES.As32Bit());
  EXPECT_EQ((uint32_t)0x01234567, SEQUENTIAL.As32Bit());
  EXPECT_EQ((uint32_t)0x00000000, kBase.As32Bit());
  EXPECT_EQ((uint32_t)0x12345678, Uuid::From32Bit(0x12345678).As32Bit());
}

TEST(UuidTest, Is16Bit) {
  EXPECT_FALSE(ONES.Is16Bit());
  EXPECT_FALSE(SEQUENTIAL.Is16Bit());
  EXPECT_TRUE(kBase.Is16Bit());
  EXPECT_TRUE(Uuid::FromString("1ae8").Is16Bit());
}

TEST(UuidTest, From16Bit) {
  EXPECT_EQ(Uuid::From16Bit(0x0000), kBase);

  const uint8_t u2[] = {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb};
  Uuid uuid = Uuid::From16Bit(0x0001);
  EXPECT_TRUE(memcmp(&uuid, u2, sizeof(u2)) == 0);

  const uint8_t u3[] = {0x00, 0x00, 0x55, 0x3e, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb};
  uuid = Uuid::From16Bit(0x553e);
  EXPECT_TRUE(memcmp(&uuid, u3, sizeof(u3)) == 0);

  const uint8_t u4[] = {0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb};
  uuid = Uuid::From16Bit(0xffff);
  EXPECT_TRUE(memcmp(&uuid, u4, sizeof(u4)) == 0);
}

TEST(UuidTest, From32Bit) {
  EXPECT_EQ(Uuid::From32Bit(0x00000000), kBase);

  const uint8_t u2[] = {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb};
  Uuid uuid = Uuid::From32Bit(0x00000001);
  EXPECT_TRUE(memcmp(&uuid, u2, sizeof(u2)) == 0);

  const uint8_t u3[] = {0x33, 0x44, 0x55, 0x3e, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb};
  uuid = Uuid::From32Bit(0x3344553e);
  EXPECT_TRUE(memcmp(&uuid, u3, sizeof(u3)) == 0);

  const uint8_t u4[] = {0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb};
  uuid = Uuid::From32Bit(0xffffffff);
  EXPECT_TRUE(memcmp(&uuid, u4, sizeof(u4)) == 0);
}

TEST(UuidTest, ToString) {
  const std::string UUID_BASE_STR = "00000000-0000-1000-8000-00805f9b34fb";
  const std::string UUID_EMP_STR = "00000000-0000-0000-0000-000000000000";
  const std::string UUID_ONES_STR = "11111111-1111-1111-1111-111111111111";
  const std::string UUID_SEQ_STR = "01234567-89ab-cdef-abcd-ef0123456789";

  EXPECT_EQ(UUID_BASE_STR, kBase.ToString());
  EXPECT_EQ(UUID_EMP_STR, Uuid::kEmpty.ToString());
  EXPECT_EQ(UUID_ONES_STR, ONES.ToString());
  EXPECT_EQ(UUID_SEQ_STR, SEQUENTIAL.ToString());

  Uuid uuid = Uuid::From32Bit(0x12345678);
  EXPECT_EQ("12345678-0000-1000-8000-00805f9b34fb", uuid.ToString());
}

TEST(BtifStorageTest, test_string_to_uuid) {
  const uint8_t u1[] = {0xe3, 0x9c, 0x62, 0x85, 0x86, 0x7f, 0x4b, 0x1d, 0x9d, 0xb0, 0x35, 0xfb, 0xd9, 0xae, 0xbf, 0x22};
  bool is_valid = false;
  Uuid uuid = Uuid::FromString("e39c6285-867f-4b1d-9db0-35fbd9aebf22", &is_valid);
  EXPECT_TRUE(is_valid);
  EXPECT_TRUE(memcmp(&uuid, u1, sizeof(u1)) == 0);

  const uint8_t u2[] = {0x00, 0x00, 0x1a, 0xe8, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb};
  is_valid = false;
  uuid = Uuid::FromString("1Ae8", &is_valid);
  EXPECT_TRUE(is_valid);
  EXPECT_TRUE(memcmp(&uuid, u2, sizeof(u2)) == 0);

  const uint8_t u3[] = {0x12, 0x34, 0x11, 0x28, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb};
  is_valid = false;
  uuid = Uuid::FromString("12341128", &is_valid);
  EXPECT_TRUE(is_valid);
  EXPECT_TRUE(memcmp(&uuid, u3, sizeof(u3)) == 0);
}

TEST(BtifStorageTest, test_string_to_uuid_invalid) {
  bool is_valid = false;
  Uuid uuid = Uuid::FromString("This is not a UUID", &is_valid);
  EXPECT_FALSE(is_valid);

  uuid = Uuid::FromString("11212", &is_valid);
  EXPECT_FALSE(is_valid);

  uuid = Uuid::FromString("1121 ", &is_valid);
  EXPECT_FALSE(is_valid);

  uuid = Uuid::FromString("AGFE", &is_valid);
  EXPECT_FALSE(is_valid);

  uuid = Uuid::FromString("ABFG", &is_valid);
  EXPECT_FALSE(is_valid);

  uuid = Uuid::FromString("e39c6285867f14b1d9db035fbd9aebf22", &is_valid);
  EXPECT_FALSE(is_valid);

  uuid = Uuid::FromString("12234567-89ab-cdef-abcd-ef01234567ZZ", &is_valid);
  EXPECT_FALSE(is_valid);
}
