/*
 * Copyright 2019 The Android Open Source Project
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

#include "packet/bit_inserter.h"

#include <gtest/gtest.h>
#include <memory>

#include "os/log.h"

using bluetooth::packet::BitInserter;
using std::vector;

namespace bluetooth {
namespace packet {

class BitInserterTest : public ::testing::Test {
 public:
  BitInserterTest() {}
  ~BitInserterTest() = default;
};

TEST(BitInserterTest, addMoreBits) {
  std::vector<uint8_t> bytes;
  BitInserter it(bytes);

  for (size_t i = 0; i < 9; i++) {
    it.insert_bits(static_cast<uint8_t>(i), i);
  }
  it.insert_bits(static_cast<uint8_t>(0b1010), 4);
  std::vector<uint8_t> result = {
      0b00011101 /* 3 2 1 */,
      0b00010101 /* 5 4 */,
      0b11100011 /* 7 6 */,
      0b10000000 /* 8 */,
      0b10100000 /* filled with 1010 */};

  ASSERT_EQ(result.size(), bytes.size());
  for (size_t i = 0; i < bytes.size(); i++) {
    ASSERT_EQ(result[i], bytes[i]);
  }
}

}  // namespace packet
}  // namespace bluetooth
