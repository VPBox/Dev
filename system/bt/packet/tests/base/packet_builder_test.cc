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

#include <gtest/gtest.h>
#include <memory>

#include "packet.h"
#include "packet_test_common.h"
#include "test_packets.h"

using std::vector;

namespace bluetooth {

TEST(PacketBuilderTest, serializeTest) {
  auto builder = TestPacketBuilder::MakeBuilder(test_l2cap_data);
  auto packet = TestPacket::Make();

  builder->Serialize(packet);

  for (size_t i = 0; i < test_l2cap_data.size(); i++) {
    ASSERT_EQ(test_l2cap_data[i], (*packet)[i]);
  }
}

TEST(PacketBuilderTest, sizeTest) {
  auto builder = TestPacketBuilder::MakeBuilder(test_l2cap_data);

  ASSERT_EQ(builder->size(), test_l2cap_data.size());
}

TEST(PacketBuilderTest, reserveSpaceTest) {
  auto packet = TestPacket::Make();
  ASSERT_EQ(packet->GetData().capacity(), 0u);

  auto builder = TestPacketBuilder::MakeBuilder(test_l2cap_data);
  builder->ReserveSpace(packet, test_l2cap_data.size());

  ASSERT_GE(packet->GetData().capacity(), test_l2cap_data.size());
}

TEST(PacketBuilderTest, addPayloadOctetsTest) {
  auto builder = TestPacketBuilder::MakeBuilder(test_l2cap_data);
  auto packet = TestPacket::Make();

  builder->AddPayloadOctets1(packet, 0x01u);
  builder->AddPayloadOctets2(packet, 0x0302u);
  builder->AddPayloadOctets3(packet, 0x060504u);
  builder->AddPayloadOctets4(packet, 0x0A090807u);
  builder->AddPayloadOctets6(packet, 0x100F0E0D0C0Bu);
  builder->AddPayloadOctets8(packet, 0x1817161514131211u);

  for (size_t i = 0; i < 0x18; i++) {
    ASSERT_EQ((*packet)[i], i + 1);
  }
}

}  // namespace bluetooth