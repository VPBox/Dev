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

#include "avrcp_browse_packet.h"
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

using TestBrowsePacket = TestPacketType<BrowsePacket>;

TEST(AvrcpBrowsePacketBuilderTest, buildPacketTest) {
  std::vector<uint8_t> get_folder_items_request_payload = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00};
  auto payload_builder =
      TestPacketBuilder::MakeBuilder(get_folder_items_request_payload);

  auto builder = BrowsePacketBuilder::MakeBuilder(BrowsePdu::GET_FOLDER_ITEMS,
                                                  std::move(payload_builder));

  ASSERT_EQ(builder->size(), get_folder_items_request.size());

  auto test_packet = TestBrowsePacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), get_folder_items_request);
}

TEST(AvrcpBrowsePacketTest, gettersTest) {
  auto test_browse_packet = TestBrowsePacket::Make(get_folder_items_request);

  ASSERT_EQ(test_browse_packet->GetPdu(), BrowsePdu::GET_FOLDER_ITEMS);
  ASSERT_EQ(test_browse_packet->GetLength(), 10u);
}

TEST(AvrcpBrowsePacketTest, payloadBoundsTest) {
  auto test_browse_packet = TestBrowsePacket::Make(get_folder_items_request);
  std::vector<uint8_t> get_folder_items_request_payload = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00};

  // Make a folder items payload packet out of the payload of the browse packet
  auto get_folder_items_payload = TestBrowsePacket::Make(test_browse_packet);

  for (size_t i = 0; i < get_folder_items_request_payload.size(); i++) {
    ASSERT_EQ((*get_folder_items_payload)[i],
              get_folder_items_request_payload[i]);
  }
}

TEST(AvrcpBrowsePacketTest, validTest) {
  auto test_browse_packet = TestBrowsePacket::Make(get_folder_items_request);

  ASSERT_TRUE(test_browse_packet->IsValid());
}

TEST(AvrcpBrowsePacketTest, invalidTest) {
  auto packet_copy = get_folder_items_request;
  packet_copy.push_back(0x00);
  auto test_browse_packet = TestBrowsePacket::Make(packet_copy);
  ASSERT_FALSE(test_browse_packet->IsValid());

  std::vector<uint8_t> short_packet = {0x00, 0x01};
  test_browse_packet = TestBrowsePacket::Make(short_packet);
  ASSERT_FALSE(test_browse_packet->IsValid());
}

}  // namespace avrcp
}  // namespace bluetooth