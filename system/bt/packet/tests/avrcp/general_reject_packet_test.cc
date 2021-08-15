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
#include "general_reject_packet.h"
#include "packet_test_helper.h"

namespace bluetooth {
namespace avrcp {

using TestGeneralRejectPacket = TestPacketType<BrowsePacket>;

TEST(GeneralRejectPacketBuilderTest, buildPacketTest) {
  auto builder = GeneralRejectBuilder::MakeBuilder(Status::INVALID_COMMAND);

  ASSERT_EQ(builder->size(), general_reject_invalid_command_packet.size());

  auto test_packet = TestGeneralRejectPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), general_reject_invalid_command_packet);
}

}  // namespace avrcp
}  // namespace bluetooth
