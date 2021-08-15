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

#include "avrcp_packet.h"
#include "avrcp_test_packets.h"
#include "packet_test_helper.h"

namespace bluetooth {

// A helper class that has public accessors to protected methods
class TestPacketBuilder : public PacketBuilder {
 public:
  static std::unique_ptr<TestPacketBuilder> MakeBuilder(
      std::vector<uint8_t> data) {
    std::unique_ptr<TestPacketBuilder> builder(new TestPacketBuilder(data));
    return builder;
  };

  // Make all the utility functions public
  using PacketBuilder::ReserveSpace;
  using PacketBuilder::AddPayloadOctets1;
  using PacketBuilder::AddPayloadOctets2;
  using PacketBuilder::AddPayloadOctets3;
  using PacketBuilder::AddPayloadOctets4;
  using PacketBuilder::AddPayloadOctets6;
  using PacketBuilder::AddPayloadOctets8;

  size_t size() const override { return data_.size(); };

  bool Serialize(const std::shared_ptr<Packet>& pkt) override {
    ReserveSpace(pkt, size());

    for (uint8_t byte : data_) {
      AddPayloadOctets1(pkt, byte);
    }

    return true;
  }

  TestPacketBuilder(std::vector<uint8_t> data) : data_(data){};

  std::vector<uint8_t> data_;
};

namespace avrcp {

using TestAvrcpPacket = TestPacketType<Packet>;

TEST(AvrcpPacketBuilderTest, buildPacketTest) {
  std::vector<uint8_t> get_capabilities_request_payload = {
      0x00, 0x19, 0x58, 0x10, 0x00, 0x00, 0x01, 0x03};
  auto cap_req_builder =
      TestPacketBuilder::MakeBuilder(get_capabilities_request_payload);

  auto builder = PacketBuilder::MakeBuilder(
      CType::STATUS, 0x09, 0x00, Opcode::VENDOR, std::move(cap_req_builder));

  ASSERT_EQ(builder->size(), get_capabilities_request.size());

  auto test_packet = TestAvrcpPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), get_capabilities_request);
}

TEST(AvrcpPacketTest, getterTests) {
  auto test_avrcp_packet = TestAvrcpPacket::Make(get_capabilities_request);

  ASSERT_EQ(test_avrcp_packet->GetCType(), CType::STATUS);
  ASSERT_EQ(test_avrcp_packet->GetSubunitType(), 0x09);
  ASSERT_EQ(test_avrcp_packet->GetSubunitId(), 0x00);
  ASSERT_EQ(test_avrcp_packet->GetOpcode(), Opcode::VENDOR);
}

TEST(AvrcpPacketTest, getterMaskTests) {
  auto bad_get_cap_data = get_capabilities_request;
  bad_get_cap_data[0] = 0xFF;  // CType
  bad_get_cap_data[1] = 0xFF;  // Subunit Type & ID

  auto test_avrcp_packet = TestAvrcpPacket::Make(bad_get_cap_data);

  ASSERT_EQ(test_avrcp_packet->GetCType(), CType::INTERIM);
  ASSERT_EQ(test_avrcp_packet->GetSubunitType(), 0b00011111);
  ASSERT_EQ(test_avrcp_packet->GetSubunitId(), 0b00000111);
}

TEST(AvrcpPacketTest, payloadBoundsTest) {
  auto test_avrcp_packet = TestAvrcpPacket::Make(get_capabilities_request);

  std::vector<uint8_t> get_cap_payload_data = {0x00, 0x19, 0x58, 0x10,
                                               0x00, 0x00, 0x01, 0x03};

  auto get_cap_payload_packet = TestAvrcpPacket::Make(test_avrcp_packet);

  // We are unable to do a direct vector compare here as one of the packets is
  // a larger vector that only has a segment of data currently visible.
  for (size_t i = 0; i < get_cap_payload_data.size(); i++) {
    ASSERT_EQ(get_cap_payload_data[i], (*get_cap_payload_packet)[i]);
  }
}

}  // namespace avrcp
}  // namespace bluetooth