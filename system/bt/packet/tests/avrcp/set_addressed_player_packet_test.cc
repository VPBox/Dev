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

#include "avrcp_test_packets.h"
#include "packet_test_helper.h"
#include "set_addressed_player.h"

namespace bluetooth {
namespace avrcp {

using TestSetAddrPlayerPacket = TestPacketType<SetAddressedPlayerRequest>;

TEST(SetAddressedPlayerResponseBuilderTest, builderTest) {
  auto builder =
      SetAddressedPlayerResponseBuilder::MakeBuilder(Status::NO_ERROR);
  ASSERT_EQ(builder->size(), set_addressed_player_response.size());

  auto test_packet = TestSetAddrPlayerPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), set_addressed_player_response);
}

TEST(SetAddressedPlayerRequestTest, getterTest) {
  auto test_packet =
      TestSetAddrPlayerPacket::Make(set_addressed_player_request);

  ASSERT_EQ(test_packet->GetPlayerId(), 0x0000u);
}

TEST(SetAddressedPlayerRequestTest, validTest) {
  auto test_packet =
      TestSetAddrPlayerPacket::Make(set_addressed_player_request);
  ASSERT_TRUE(test_packet->IsValid());
}

TEST(SetAddressedPlayerRequestTest, invalidTest) {
  auto packet_copy = set_addressed_player_request;
  packet_copy.push_back(0x00);
  auto test_packet = TestSetAddrPlayerPacket::Make(packet_copy);
  ASSERT_FALSE(test_packet->IsValid());

  std::vector<uint8_t> short_packet = {0x00, 0x01, 0x02, 0x03, 0x04};
  test_packet = TestSetAddrPlayerPacket::Make(short_packet);
  ASSERT_FALSE(test_packet->IsValid());
}

}  // namespace avrcp
}  // namespace bluetooth
