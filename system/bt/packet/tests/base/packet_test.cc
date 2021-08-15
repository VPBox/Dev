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

#include "packet.h"
#include "packet_test_common.h"
#include "test_packets.h"

namespace bluetooth {

// Test making a packet from another packet. The new packet should have the
// same payload bounds as the old packet.
TEST(PacketTest, newPacketFromPacketTest) {
  // Create a packet with payload bounds
  auto packet = TestPacket::Make(
      test_avctp_data, test_avctp_data_payload_offset, test_avctp_data.size());

  // Create packet from bounded packet
  auto new_packet = TestPacket::Make(packet);

  // Check to see if the new packet is bounded by the payload of the old packet
  auto it = new_packet->begin();
  for (size_t i = 0; i < test_avrcp_data.size(); i++) {
    ASSERT_EQ(test_avrcp_data[i], *it++);
  }
}

// Test that the correct payload length is returned
TEST(PacketTest, sizeTest) {
  auto packet = TestPacket::Make(test_avctp_data);
  ASSERT_EQ(packet->size(), test_avctp_data.size());

  packet = TestPacket::Make(test_avctp_data, test_avctp_data_payload_offset,
                            test_avctp_data.size());
  ASSERT_EQ(packet->size(), test_avrcp_data.size());
}

// Test the array access operator
TEST(PacketTest, arrayAccessTest) {
  auto packet = TestPacket::Make(test_l2cap_data);
  for (size_t i = 0; i < test_l2cap_data.size(); i++) {
    ASSERT_EQ(test_l2cap_data[i], (*packet)[i]);
  }

  packet = TestPacket::Make(test_avctp_data, test_avctp_data_payload_offset,
                            test_avctp_data.size());
  for (size_t i = 0; i < test_avrcp_data.size(); i++) {
    ASSERT_EQ(test_avrcp_data[i], (*packet)[i]);
  }
}

// Test that accessing past the end of the defined payload dies
TEST(PacketTest, arrayAccessDeathTest) {
  auto packet =
      TestPacket::Make(test_l2cap_data, 3, test_l2cap_data.size() - 2);
  ASSERT_DEATH((*packet)[test_l2cap_data.size()], "");
}

// Test that the iterator and array access operator return the same data
TEST(PacketTest, iteratorTest) {
  auto packet = TestPacket::Make(
      test_avctp_data, test_avctp_data_payload_offset, test_avctp_data.size());

  // Check to see if the data matches
  auto it = packet->begin();
  for (size_t i = 0; i < packet->size(); i++) {
    ASSERT_EQ((*packet)[i], *it++);
  }

  // Check to see if the iterator points to the end of the data
  ASSERT_EQ(it, packet->end());
}

class ChildTestPacket : public TestPacket {
 public:
  using TestPacket::TestPacket;

  std::string ToString() const override { return "ChildTestPacket"; };
};

// Test specializing a packet to another packet type
TEST(PacketTest, specializeTest) {
  auto packet = TestPacket::Make(test_l2cap_data);

  std::shared_ptr<ChildTestPacket> specialized_packet =
      Packet::Specialize<ChildTestPacket>(packet);

  // Test that the new packet is an instance of ChildTestPacket
  ASSERT_EQ(specialized_packet->ToString(), "ChildTestPacket");

  // Test that the underlying data is the same and no copy took place
  ASSERT_EQ(&specialized_packet->GetData(), &packet->GetData());
}

// Test that when the packet goes out of scope, that the underlying memory is
// freed
TEST(PacketTest, memoryFreeTest) {
  auto packet = TestPacket::Make(test_l2cap_data);
  std::weak_ptr<std::vector<uint8_t>> data_ptr(packet->GetDataPointer());

  ASSERT_FALSE(data_ptr.expired());

  packet.reset();

  ASSERT_TRUE(data_ptr.expired());
}

}  // namespace bluetooth