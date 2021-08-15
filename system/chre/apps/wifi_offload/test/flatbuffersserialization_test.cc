/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <cstring>
#include "gtest/gtest.h"

#include "chre/apps/wifi_offload/flatbuffers_serialization.h"

#include "include/utility.h"

using wifi_offload::fbs::Serialize;
using wifi_offload::fbs::Deserialize;

template <typename TestType>
class FlatbuffersSerializationTest : public testing::Test {
 public:
  // RandomGenerator used to initialize data-types with random values
  wifi_offload_test::RandomGenerator random_gen_;

  static const size_t kBufferLen = CHRE_MESSAGE_TO_HOST_MAX_SIZE;
  uint8_t buffer[kBufferLen];
};

typedef testing::Types<wifi_offload::ScanStats, wifi_offload::ScanConfig,
                       wifi_offload::Vector<wifi_offload::ScanResult>>
    Implementations;

TYPED_TEST_CASE(FlatbuffersSerializationTest, Implementations);

TYPED_TEST(FlatbuffersSerializationTest,
           SerializationWithNullBufferReturnsRequiredBufferSize) {
  TypeParam test_obj;
  init(test_obj, this->random_gen_);

  size_t required_buff_size = Serialize(test_obj, nullptr, 0);
  EXPECT_NE(0, required_buff_size);

  size_t serialized_size = Serialize(test_obj, this->buffer, this->kBufferLen);
  ASSERT_NE(0, serialized_size);

  EXPECT_EQ(serialized_size, required_buff_size);
}

TYPED_TEST(FlatbuffersSerializationTest,
           SerializationThenDeserializationCreatesEqualValue) {
  TypeParam test_obj;
  init(test_obj, this->random_gen_);

  size_t serialized_size = Serialize(test_obj, this->buffer, this->kBufferLen);
  ASSERT_NE(0, serialized_size);

  TypeParam deserialized_obj;
  ASSERT_TRUE(Deserialize(this->buffer, serialized_size, &deserialized_obj));
  EXPECT_EQ(test_obj, deserialized_obj);
}

TYPED_TEST(FlatbuffersSerializationTest, NegativeTestsForSerialization) {
  TypeParam test_obj;
  init(test_obj, this->random_gen_);

  EXPECT_EQ(0, Serialize(test_obj, this->buffer, 0));   // zero buffer size
  EXPECT_EQ(0, Serialize(test_obj, this->buffer, 10));  // buffer too small
}

TYPED_TEST(FlatbuffersSerializationTest, NegativeTestsForDeserialization) {
  TypeParam test_obj;
  init(test_obj, this->random_gen_);

  // The first 4 bytes in the buffer represent the position of the root
  // table, so corrupting it should force the deserialization to fail.
  constexpr size_t kRootTableOffsetSize = 4;
  size_t serialized_size = Serialize(test_obj, this->buffer, this->kBufferLen);
  ASSERT_GE(serialized_size, kRootTableOffsetSize);

  TypeParam new_obj;
  EXPECT_FALSE(Deserialize(nullptr, serialized_size, &new_obj));
  EXPECT_FALSE(Deserialize(this->buffer, 0, &new_obj));

  // Corrupt the root table's offset
  std::memset(this->buffer, 0xff, kRootTableOffsetSize);
  EXPECT_FALSE(Deserialize(this->buffer, serialized_size, &new_obj));
}
