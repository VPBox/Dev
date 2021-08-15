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
#include "pass_through_packet.h"

namespace bluetooth {
namespace avrcp {

using TestPassThroughPacket = TestPacketType<PassThroughPacket>;

TEST(PassThroughPacketBuilderTest, builderTest) {
  auto builder = PassThroughPacketBuilder::MakeBuilder(false, true, 0x44);
  ASSERT_EQ(builder->size(), pass_through_command_play_pushed.size());
  auto test_packet = TestPassThroughPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), pass_through_command_play_pushed);

  builder = PassThroughPacketBuilder::MakeBuilder(true, false, 0x44);
  ASSERT_EQ(builder->size(), pass_through_command_play_released.size());
  test_packet = TestPassThroughPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), pass_through_command_play_released);
}

TEST(PassThroughPacketTest, getterTest) {
  auto test_packet =
      TestPassThroughPacket::Make(pass_through_command_play_pushed);
  ASSERT_EQ(test_packet->GetKeyState(), KeyState::PUSHED);
  ASSERT_EQ(test_packet->GetOperationId(), 0x44);

  test_packet = TestPassThroughPacket::Make(pass_through_command_play_released);
  ASSERT_EQ(test_packet->GetKeyState(), KeyState::RELEASED);
  ASSERT_EQ(test_packet->GetOperationId(), 0x44);
}

TEST(PassThroughPacketTest, validTest) {
  auto test_packet =
      TestPassThroughPacket::Make(pass_through_command_play_pushed);
  ASSERT_TRUE(test_packet->IsValid());

  test_packet = TestPassThroughPacket::Make(pass_through_command_play_released);
  ASSERT_TRUE(test_packet->IsValid());
}

TEST(PassThroughPacketTest, invalidTest) {
  std::vector<uint8_t> packet_copy = pass_through_command_play_pushed;
  packet_copy.push_back(0x00);
  auto test_packet = TestPassThroughPacket::Make(packet_copy);
  ASSERT_FALSE(test_packet->IsValid());

  std::vector<uint8_t> short_packet = {0, 1, 2, 3, 4, 5};
  test_packet = TestPassThroughPacket::Make(short_packet);
  ASSERT_FALSE(test_packet->IsValid());
}

}  // namespace avrcp
}  // namespace bluetooth