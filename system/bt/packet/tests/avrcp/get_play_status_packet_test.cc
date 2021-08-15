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
#include "get_play_status_packet.h"
#include "packet_test_helper.h"

namespace bluetooth {
namespace avrcp {

using TestGetPlayStatusRspPacket = TestPacketType<Packet>;

TEST(GetPlayStatusResponseBuilderTest, builderTest) {
  auto builder = GetPlayStatusResponseBuilder::MakeBuilder(0, 0xFFFFFFFF, 0);

  ASSERT_EQ(builder->size(), get_play_status_response.size());

  auto test_packet = TestGetPlayStatusRspPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), get_play_status_response);
}

}  // namespace avrcp
}  // namespace bluetooth