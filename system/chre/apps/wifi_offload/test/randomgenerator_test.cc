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

#include <limits>
#include "gtest/gtest.h"

#include "include/random_generator.h"

template <typename TestType>
class RandomGeneratorTest : public testing::Test {
 public:
  wifi_offload_test::RandomGenerator random_gen_;

  void GeneratedNumbersAreSmallerOrEqualTypeMaxValue() {
    uint64_t rand_val = random_gen_.get<TestType>();
    uint64_t max_val = std::numeric_limits<TestType>::max();

    EXPECT_TRUE(rand_val <= max_val);
  }

  void AllRandomGeneratorsGenerateTheSameSequence() {
    constexpr size_t num_values = 10;
    TestType rand_values[num_values];

    for (size_t i = 0; i < num_values; i++) {
      rand_values[i] = random_gen_.get<TestType>();
    }

    wifi_offload_test::RandomGenerator another_random_gen_;
    for (size_t i = 0; i < num_values; i++) {
      ASSERT_EQ(rand_values[i], another_random_gen_.get<TestType>());
    }
  }

  void AfterResetGeneratesTheSameSequence() {
    constexpr size_t num_values = 10;
    TestType rand_values[num_values];

    for (size_t i = 0; i < num_values; i++) {
      rand_values[i] = random_gen_.get<TestType>();
    }

    random_gen_.Reset();
    for (size_t i = 0; i < num_values; i++) {
      ASSERT_EQ(rand_values[i], random_gen_.get<TestType>());
    }
  }

  void GeneratesDifferentNumbersIn8Bytes() {
    constexpr size_t num_values = 10;
    uint64_t rand_values[num_values];

    constexpr size_t repeats_to_fill_8_bytes =
        sizeof(uint64_t) / sizeof(TestType);
    constexpr size_t shift_size =
        (sizeof(TestType) * 8) % (sizeof(uint64_t) * 8);

    for (size_t i = 0; i < num_values; i++) {
      rand_values[i] = 0;
      for (size_t j = 0; j < repeats_to_fill_8_bytes; j++) {
        rand_values[i] <<= shift_size;
        rand_values[i] |= random_gen_.get<TestType>();
      }
    }

    // The probability of choosing equal random numbers out of 2 ^ 64 values
    // is "extremely" small. Smaller than the change of memory failure.
    for (size_t i = 0; i < num_values - 1; i++) {
      for (size_t j = i + 1; j < num_values; j++) {
        ASSERT_NE(rand_values[i], rand_values[j]);
      }
    }
  }
};

typedef testing::Types<uint8_t, uint16_t, uint32_t, uint64_t> Implementations;

TYPED_TEST_CASE(RandomGeneratorTest, Implementations);

TYPED_TEST(RandomGeneratorTest, GeneratedNumbersAreSmallerOrEqualTypeMaxValue) {
  this->GeneratedNumbersAreSmallerOrEqualTypeMaxValue();
}

TYPED_TEST(RandomGeneratorTest, AllRandomGeneratorsGenerateTheSameSequence) {
  this->AllRandomGeneratorsGenerateTheSameSequence();
}

TYPED_TEST(RandomGeneratorTest, AfterResetGeneratesTheSameSequence) {
  this->AfterResetGeneratesTheSameSequence();
}

TYPED_TEST(RandomGeneratorTest, GeneratesDifferentNumbersIn8Bytes) {
  this->GeneratesDifferentNumbersIn8Bytes();
}
