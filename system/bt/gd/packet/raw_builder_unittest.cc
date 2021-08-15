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

#include "packet/raw_builder.h"

#include <gtest/gtest.h>
#include <forward_list>
#include <memory>

#include "common/address.h"

using bluetooth::common::Address;
using bluetooth::packet::BitInserter;
using std::vector;

namespace {
vector<uint8_t> count = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
};

}  // namespace

namespace bluetooth {
namespace packet {

class RawBuilderTest : public ::testing::Test {
 public:
  RawBuilderTest() = default;
  ~RawBuilderTest() = default;
};

TEST(RawBuilderTest, buildCountTest) {
  std::unique_ptr<RawBuilder> count_builder = std::make_unique<RawBuilder>();
  ASSERT_EQ(0u, count_builder->size());
  count_builder->AddOctets8(0x0706050403020100);
  count_builder->AddOctets4(0x0b0a0908);
  count_builder->AddOctets2(0x0d0c);
  count_builder->AddOctets1(0x0e);
  count_builder->AddOctets1(0x0f);
  count_builder->AddAddress(Address({0x10, 0x11, 0x12, 0x13, 0x14, 0x15}));
  std::vector<uint8_t> count_subset(count.begin() + 0x16, count.end());
  count_builder->AddOctets(count_subset);

  ASSERT_EQ(count.size(), count_builder->size());

  std::vector<uint8_t> packet;
  BitInserter it(packet);

  count_builder->Serialize(it);

  ASSERT_EQ(count, packet);
}

}  // namespace packet
}  // namespace bluetooth
