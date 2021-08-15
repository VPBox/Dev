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

#include "gtest/gtest.h"

#include "include/utility.h"

template <typename TestType>
class OffloadTypesTest : public testing::Test {
 public:
  // RandomGenerator used to initialize data-types with random values
  wifi_offload_test::RandomGenerator random_gen_;

  flatbuffers::FlatBufferBuilder builder;

  void EqualOperatorReturnsTrueForEqualValues() {
    TestType lhs;
    init(lhs, random_gen_);

    random_gen_.Reset();
    TestType rhs;
    init(rhs, random_gen_);

    EXPECT_EQ(lhs, rhs);
  }

  void EqualOperatorReturnsFalseForDifferentValues() {
    TestType lhs, rhs;
    init(lhs, random_gen_);
    init(rhs, random_gen_);

    ASSERT_FALSE(lhs == rhs);
  }

  void SerializationThenDeserializationCreatesEqualValue() {
    TestType test_obj;
    init(test_obj, random_gen_);
    builder.Finish(test_obj.Serialize(&builder));

    const uint8_t *serialized_buff = builder.GetBufferPointer();
    const size_t serialized_size = builder.GetSize();
    ASSERT_NE(nullptr, serialized_buff);
    ASSERT_NE(0, serialized_size);

    flatbuffers::Verifier verifier(serialized_buff, serialized_size);
    ASSERT_TRUE(verifier.VerifyBuffer<typename TestType::FbsType>(nullptr));

    const auto fbs_obj =
        flatbuffers::GetRoot<typename TestType::FbsType>(serialized_buff);
    ASSERT_NE(nullptr, fbs_obj);

    TestType deserialized_obj;
    ASSERT_TRUE(deserialized_obj.Deserialize(*fbs_obj));
    EXPECT_EQ(test_obj, deserialized_obj);
  }
};

typedef testing::Types<wifi_offload::PreferredNetwork, wifi_offload::ScanResult,
                       wifi_offload::ScanParams, wifi_offload::ScanFilter,
                       wifi_offload::ScanConfig, wifi_offload::ScanRecord,
                       wifi_offload::RpcLogRecord, wifi_offload::ScanStats>
    Implementations;

TYPED_TEST_CASE(OffloadTypesTest, Implementations);

TYPED_TEST(OffloadTypesTest, EqualOperatorReturnsTrueForEqualValues) {
  this->EqualOperatorReturnsTrueForEqualValues();
}

TYPED_TEST(OffloadTypesTest, EqualOperatorReturnsFalseForDifferentValues) {
  this->EqualOperatorReturnsFalseForDifferentValues();
}

TYPED_TEST(OffloadTypesTest,
           SerializationThenDeserializationCreatesEqualValue) {
  this->SerializationThenDeserializationCreatesEqualValue();
}
