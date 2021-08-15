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
#include "set_absolute_volume.h"

namespace bluetooth {
namespace avrcp {

using TestSetVolumeRspPacket = TestPacketType<SetAbsoluteVolumeResponse>;

TEST(SetAbsoluteVolumeRequestBuilderTest, builderTest) {
  auto builder = SetAbsoluteVolumeRequestBuilder::MakeBuilder(0x48);
  ASSERT_EQ(builder->size(), set_absolute_volume_request.size());

  auto test_packet = TestSetVolumeRspPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), set_absolute_volume_request);
}

// Test whether the volume field has the highest bit masked
TEST(SetAbsoluteVolumeRequestBuilderTest, volumeMaskTest) {
  auto builder = SetAbsoluteVolumeRequestBuilder::MakeBuilder(0xc8);
  auto test_packet = TestSetVolumeRspPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), set_absolute_volume_request);
}

TEST(SetAbsoluteVolumeResponseTest, getterTest) {
  auto test_packet = TestSetVolumeRspPacket::Make(set_absolute_volume_response);

  ASSERT_EQ(test_packet->GetVolume(), 0x43);
}

TEST(SetAbsoluteVolumeResponseTest, validTest) {
  auto test_packet = TestSetVolumeRspPacket::Make(set_absolute_volume_response);

  ASSERT_TRUE(test_packet->IsValid());
}

TEST(SetAbsoluteVolumeResponseTest, invalidTest) {
  auto packet_copy = set_absolute_volume_request;
  packet_copy.push_back(0x00);
  auto test_packet = TestSetVolumeRspPacket::Make(packet_copy);
  ASSERT_FALSE(test_packet->IsValid());

  std::vector<uint8_t> short_packet = {0x00, 0x01, 0x02, 0x03, 0x04};
  test_packet = TestSetVolumeRspPacket::Make(short_packet);
  ASSERT_FALSE(test_packet->IsValid());

  auto wrong_ctype = set_absolute_volume_request;
  wrong_ctype[0] = 0x00;
  test_packet = TestSetVolumeRspPacket::Make(wrong_ctype);
  ASSERT_FALSE(test_packet->IsValid());
}

}  // namespace avrcp
}  // namespace bluetooth