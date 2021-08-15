/******************************************************************************
 *
 *  Copyright 2018 Google, Inc.
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

#include "address_obfuscator.h"

#include <gtest/gtest.h>

using bluetooth::common::AddressObfuscator;

constexpr AddressObfuscator::Octet32 kEmptyKey = {0};

constexpr AddressObfuscator::Octet32 kTestKey1 = {
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
};
static RawAddress kTestData1 = {{0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}};
constexpr AddressObfuscator::Octet32 kTestResultRaw1 = {
    0x9b, 0x52, 0xb9, 0xb9, 0xb9, 0x34, 0x80, 0x1d, 0x98, 0x0b, 0x10,
    0xbe, 0x45, 0xa2, 0x6d, 0xaa, 0x99, 0xc3, 0x04, 0x10, 0x08, 0x03,
    0xb7, 0xb4, 0xa9, 0xde, 0xcf, 0x89, 0xe1, 0x5d, 0xd4, 0xaa};
static std::string kTestResult1(
    reinterpret_cast<const char*>(kTestResultRaw1.data()),
    kTestResultRaw1.size());

constexpr AddressObfuscator::Octet32 kTestKey2 = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
    0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
    0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20};

static RawAddress kTestData2_1 = {{0x9e, 0xf5, 0x3d, 0x6c, 0x2e, 0x33}};
constexpr AddressObfuscator::Octet32 kTestResultRaw2_1 = {
    0xb4, 0xe2, 0xfc, 0xb9, 0x59, 0x0d, 0x1f, 0xcf, 0x68, 0x80, 0xb2,
    0x3d, 0x08, 0x55, 0x4e, 0x64, 0xf5, 0x3b, 0x33, 0x0d, 0xb6, 0x31,
    0x9a, 0xbc, 0x4e, 0xce, 0x61, 0xbd, 0x46, 0x66, 0x45, 0x94};
static std::string kTestResult2_1(
    reinterpret_cast<const char*>(kTestResultRaw2_1.data()),
    kTestResultRaw2_1.size());

static RawAddress kTestData2_2 = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
constexpr AddressObfuscator::Octet32 kTestResultRaw2_2 = {
    0xd4, 0xd8, 0x23, 0xc0, 0x24, 0xba, 0xde, 0xe3, 0x1c, 0xad, 0x84,
    0x8b, 0x3d, 0xc6, 0xda, 0x93, 0x88, 0xb2, 0x5c, 0x60, 0x13, 0xe5,
    0xe2, 0x3e, 0x75, 0x5f, 0xd7, 0x15, 0x56, 0xf7, 0xaf, 0x27};
static std::string kTestResult2_2(
    reinterpret_cast<const char*>(kTestResultRaw2_2.data()),
    kTestResultRaw2_2.size());

static RawAddress kTestData2_3 = {{0xff, 0xff, 0xff, 0xff, 0xff, 0xff}};
constexpr AddressObfuscator::Octet32 kTestResultRaw2_3 = {
    0x6f, 0x7c, 0x3d, 0x23, 0xcc, 0x7a, 0xf2, 0x68, 0xee, 0xe8, 0x6c,
    0x0f, 0xb5, 0xe0, 0x0c, 0x88, 0xf6, 0x38, 0x71, 0x44, 0x88, 0x09,
    0x45, 0x0a, 0xa2, 0xd7, 0xf6, 0x70, 0xba, 0x8c, 0xe9, 0x79};
static std::string kTestResult2_3(
    reinterpret_cast<const char*>(kTestResultRaw2_3.data()),
    kTestResultRaw2_3.size());

TEST(AddressObfuscatorTest, test_invalid_key) {
  EXPECT_FALSE(AddressObfuscator::IsSaltValid(kEmptyKey));
}

TEST(AddressObfuscatorTest, test_valid_key) {
  EXPECT_TRUE(AddressObfuscator::IsSaltValid(kTestKey1));
}

TEST(AddressObfuscatorTest, test_initialize_negative) {
  AddressObfuscator::GetInstance()->Initialize(kEmptyKey);
  EXPECT_FALSE(AddressObfuscator::GetInstance()->IsInitialized());
}

TEST(AddressObfuscatorTest, test_initialize_positive) {
  AddressObfuscator::GetInstance()->Initialize(kTestKey1);
  EXPECT_TRUE(AddressObfuscator::GetInstance()->IsInitialized());
}

TEST(AddressObfuscatorTest, test_obfuscate_address_key1) {
  AddressObfuscator::GetInstance()->Initialize(kTestKey1);
  std::string result = AddressObfuscator::GetInstance()->Obfuscate(kTestData1);
  EXPECT_EQ(result.size(), AddressObfuscator::kOctet32Length);
  EXPECT_EQ(result, kTestResult1);
}

TEST(AddressObfuscatorTest, test_obfuscate_address_key2) {
  AddressObfuscator::GetInstance()->Initialize(kTestKey2);
  std::string result =
      AddressObfuscator::GetInstance()->Obfuscate(kTestData2_1);
  EXPECT_EQ(result.size(), AddressObfuscator::kOctet32Length);
  EXPECT_EQ(result, kTestResult2_1);
}

TEST(AddressObfuscatorTest, test_obfuscate_address_key2_empty_adddress) {
  AddressObfuscator::GetInstance()->Initialize(kTestKey2);
  std::string result =
      AddressObfuscator::GetInstance()->Obfuscate(kTestData2_2);
  EXPECT_EQ(result.size(), AddressObfuscator::kOctet32Length);
  EXPECT_EQ(result, kTestResult2_2);
}

TEST(AddressObfuscatorTest, test_obfuscate_address_key2_max_address) {
  AddressObfuscator::GetInstance()->Initialize(kTestKey2);
  std::string result =
      AddressObfuscator::GetInstance()->Obfuscate(kTestData2_3);
  EXPECT_EQ(result.size(), AddressObfuscator::kOctet32Length);
  EXPECT_EQ(result, kTestResult2_3);
}