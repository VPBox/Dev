/*
 * Copyright 2018 The Android Open Source Project
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

#include "packets/counted_builder.h"

#include <gtest/gtest.h>
#include <forward_list>
#include <memory>

#include "types/address.h"

using std::vector;

namespace {
vector<uint8_t> count = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
};

}  // namespace

namespace test_vendor_lib {
namespace packets {

class CountedBuilderTest : public ::testing::Test {
 public:
  CountedBuilderTest() = default;
  ~CountedBuilderTest() = default;
};

TEST(CountedBuilderTest, buildCountTest) {
  std::unique_ptr<CountedBuilder> count_builder = std::make_unique<CountedBuilder>();
  ASSERT_EQ(1u, count_builder->size());
  std::unique_ptr<RawBuilder> raw1 = std::make_unique<RawBuilder>();
  std::unique_ptr<RawBuilder> raw2 = std::make_unique<RawBuilder>();
  std::unique_ptr<RawBuilder> raw3 = std::make_unique<RawBuilder>();
  std::unique_ptr<RawBuilder> raw4 = std::make_unique<RawBuilder>();
  std::unique_ptr<RawBuilder> raw5 = std::make_unique<RawBuilder>();
  std::unique_ptr<RawBuilder> raw6 = std::make_unique<RawBuilder>();
  std::unique_ptr<RawBuilder> raw7 = std::make_unique<RawBuilder>();
  raw1->AddOctets8(0x0706050403020100);
  raw2->AddOctets4(0x0b0a0908);
  raw3->AddOctets2(0x0d0c);
  raw4->AddOctets1(0x0e);
  raw5->AddOctets1(0x0f);
  raw6->AddAddress(Address({0x10, 0x11, 0x12, 0x13, 0x14, 0x15}));
  std::vector<uint8_t> count_subset(count.begin() + 0x16, count.end());
  raw7->AddOctets(count_subset);

  count_builder->Add(std::move(raw1));
  count_builder->Add(std::move(raw2));
  count_builder->Add(std::move(raw3));
  count_builder->Add(std::move(raw4));
  count_builder->Add(std::move(raw5));
  count_builder->Add(std::move(raw6));
  count_builder->Add(std::move(raw7));

  ASSERT_EQ(count.size(), count_builder->size() - 1);

  std::vector<uint8_t> packet;
  std::back_insert_iterator<std::vector<uint8_t>> it(packet);

  count_builder->Serialize(it);
  ASSERT_EQ(7u, packet[0]);
  std::vector<uint8_t> payload_packet(packet.begin() + 1, packet.end());

  ASSERT_EQ(count, payload_packet);
}

}  // namespace packets
}  // namespace test_vendor_lib
