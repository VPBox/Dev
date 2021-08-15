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

#include <tuple>

#include <gtest/gtest.h>

#include "avrcp_test_packets.h"
#include "packet_test_helper.h"
#include "vendor_packet.h"

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

using TestVendorPacket = TestPacketType<VendorPacket>;

TEST(VendorPacketBuilderTest, builderTest) {
  std::vector<uint8_t> get_cap_payload_data = {0x03};

  auto get_cap_payload = TestPacketBuilder::MakeBuilder(get_cap_payload_data);

  auto builder = VendorPacketBuilder::MakeBuilder(
      CType::STATUS, CommandPdu::GET_CAPABILITIES, PacketType::SINGLE,
      std::move(get_cap_payload));

  ASSERT_EQ(builder->size(), get_capabilities_request.size());

  auto test_packet = TestVendorPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), get_capabilities_request);
}

using TestParam = std::tuple<std::vector<uint8_t>, CommandPdu>;
class VendorPacketTest : public ::testing::TestWithParam<TestParam> {
 public:
  std::vector<uint8_t> GetPacketData() { return std::get<0>(GetParam()); }
  CommandPdu GetCommandPdu() { return std::get<1>(GetParam()); }
};

INSTANTIATE_TEST_CASE_P(
    VendorPacketParameterTest, VendorPacketTest,
    ::testing::Values(
        std::make_tuple(get_capabilities_request, CommandPdu::GET_CAPABILITIES),
        std::make_tuple(get_capabilities_response_company_id,
                        CommandPdu::GET_CAPABILITIES),
        std::make_tuple(get_capabilities_response_events_supported,
                        CommandPdu::GET_CAPABILITIES),
        std::make_tuple(get_element_attributes_request_partial,
                        CommandPdu::GET_ELEMENT_ATTRIBUTES),
        std::make_tuple(get_element_attributes_request_full,
                        CommandPdu::GET_ELEMENT_ATTRIBUTES),
        std::make_tuple(get_elements_attributes_response_full,
                        CommandPdu::GET_ELEMENT_ATTRIBUTES),
        std::make_tuple(get_play_status_request, CommandPdu::GET_PLAY_STATUS),
        std::make_tuple(get_play_status_response, CommandPdu::GET_PLAY_STATUS),
        std::make_tuple(register_play_status_notification,
                        CommandPdu::REGISTER_NOTIFICATION),
        std::make_tuple(interim_play_status_notification,
                        CommandPdu::REGISTER_NOTIFICATION),
        std::make_tuple(interim_track_changed_notification,
                        CommandPdu::REGISTER_NOTIFICATION),
        std::make_tuple(changed_play_pos_notification,
                        CommandPdu::REGISTER_NOTIFICATION)));

TEST_P(VendorPacketTest, getterTest) {
  auto test_packet = TestVendorPacket::Make(GetPacketData());

  ASSERT_EQ(test_packet->GetCompanyId(), BLUETOOTH_COMPANY_ID);
  ASSERT_EQ(test_packet->GetCommandPdu(), GetCommandPdu());
  ASSERT_EQ(test_packet->GetPacketType(), PacketType::SINGLE);
  // ASSERT_EQ(test_packet->GetParameterLength(), 0x01);
}

TEST_P(VendorPacketTest, validTest) {
  auto test_packet = TestVendorPacket::Make(GetPacketData());
  ASSERT_TRUE(test_packet->IsValid());
}

TEST_F(VendorPacketTest, invalidTest) {
  auto get_capabilities_request_copy = get_capabilities_request;
  get_capabilities_request_copy.push_back(0x00);
  auto test_packet = TestVendorPacket::Make(get_capabilities_request_copy);
  ASSERT_FALSE(test_packet->IsValid());

  std::vector<uint8_t> short_packet = {0x01, 0x02, 0x03, 0x04, 0x05};
  test_packet = TestVendorPacket::Make(short_packet);
  ASSERT_FALSE(test_packet->IsValid());
}

}  // namespace avrcp
}  // namespace bluetooth