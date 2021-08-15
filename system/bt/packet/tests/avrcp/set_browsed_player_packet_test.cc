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
#include "set_browsed_player.h"

namespace bluetooth {
namespace avrcp {

using TestSetBrowsedPlayerPacket = TestPacketType<SetBrowsedPlayerRequest>;

TEST(SetBrowsedPlayerResponseBuilder, builderTest) {
  auto builder = SetBrowsedPlayerResponseBuilder::MakeBuilder(Status::NO_ERROR,
                                                              0x0000, 4, 0, "");
  ASSERT_EQ(builder->size(), set_browsed_player_response.size());

  auto test_packet = TestSetBrowsedPlayerPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), set_browsed_player_response);
}

TEST(SetBrowsedPlayerResponseBuilder, errorStatusTest) {
  std::vector<uint8_t> player_not_browsable_status = {0x70, 0x00, 0x01, 0x12};
  auto builder = SetBrowsedPlayerResponseBuilder::MakeBuilder(
      Status::PLAYER_NOT_BROWSABLE, 0x1234, 5, 6, "Field Not Used");
  ASSERT_EQ(builder->size(), player_not_browsable_status.size());

  auto test_packet = TestSetBrowsedPlayerPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), player_not_browsable_status);
}

TEST(SetBrowsedPlayerRequestTest, getterTest) {
  auto test_packet =
      TestSetBrowsedPlayerPacket::Make(set_browsed_player_request);

  ASSERT_EQ(test_packet->GetPlayerId(), 0x0002u);
}

TEST(SetBrowsedPlayerRequestTest, validTest) {
  auto test_packet =
      TestSetBrowsedPlayerPacket::Make(set_browsed_player_request);
  ASSERT_TRUE(test_packet->IsValid());
}

TEST(SetBrowsedPlayerRequestTest, invalidTest) {
  auto packet_copy = set_browsed_player_request;
  packet_copy.push_back(0x00);
  auto test_packet = TestSetBrowsedPlayerPacket::Make(packet_copy);
  ASSERT_FALSE(test_packet->IsValid());

  std::vector<uint8_t> short_packet = {0x00, 0x01, 0x02, 0x03, 0x04};
  test_packet = TestSetBrowsedPlayerPacket::Make(short_packet);
  ASSERT_FALSE(test_packet->IsValid());
}

}  // namespace avrcp
}  // namespace bluetooth