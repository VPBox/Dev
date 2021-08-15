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

#include "packets/hci/acl_packet_builder.h"
#include "packets/hci/acl_packet_view.h"
#include "packets/raw_builder.h"

#include <gtest/gtest.h>
#include <forward_list>
#include <memory>

#include "types/address.h"

using std::vector;
using test_vendor_lib::acl::BroadcastFlagsType;
using test_vendor_lib::acl::PacketBoundaryFlagsType;

namespace {
vector<uint8_t> count = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
};

vector<uint8_t> information_request = {
    0xfe, 0x2e, 0x0a, 0x00, 0x06, 0x00, 0x01, 0x00, 0x0a, 0x02, 0x02, 0x00, 0x02, 0x00,
};

}  // namespace

namespace test_vendor_lib {
namespace packets {

class AclBuilderTest : public ::testing::Test {
 public:
  AclBuilderTest() = default;
  ~AclBuilderTest() = default;
};

TEST(AclBuilderTest, buildAclCountTest) {
  uint16_t handle = 0x0102;
  PacketBoundaryFlagsType packet_boundary_flags = PacketBoundaryFlagsType::FIRST_AUTOMATICALLY_FLUSHABLE;
  BroadcastFlagsType broadcast_flags = BroadcastFlagsType::ACTIVE_SLAVE_BROADCAST;

  std::unique_ptr<RawBuilder> count_payload = std::make_unique<RawBuilder>();
  count_payload->AddOctets(count);
  ASSERT_EQ(count.size(), count_payload->size());

  std::unique_ptr<AclPacketBuilder> count_packet =
      AclPacketBuilder::Create(handle, packet_boundary_flags, broadcast_flags, std::move(count_payload));

  ASSERT_EQ(count.size() + 4, count_packet->size());

  std::shared_ptr<std::vector<uint8_t>> count_packet_bytes = count_packet->ToVector();
  AclPacketView count_packet_view = AclPacketView::Create(count_packet_bytes);

  ASSERT_EQ(handle, count_packet_view.GetHandle());
  ASSERT_EQ(packet_boundary_flags, count_packet_view.GetPacketBoundaryFlags());
  ASSERT_EQ(broadcast_flags, count_packet_view.GetBroadcastFlags());
  PacketView<true> count_view = count_packet_view.GetPayload();

  ASSERT_EQ(count_view.size(), count.size());
  for (size_t i = 0; i < count_view.size(); i++) {
    ASSERT_EQ(count_view[i], count[i]);
  }
}

TEST(AclBuilderTest, buildInformationRequest) {
  uint16_t handle = 0x0efe;
  PacketBoundaryFlagsType packet_boundary_flags = PacketBoundaryFlagsType::FIRST_AUTOMATICALLY_FLUSHABLE;
  BroadcastFlagsType broadcast_flags = BroadcastFlagsType::POINT_TO_POINT;

  std::vector<uint8_t> payload_bytes(information_request.begin() + 4, information_request.end());
  std::unique_ptr<RawBuilder> payload = std::make_unique<RawBuilder>();
  payload->AddOctets(payload_bytes);
  ASSERT_EQ(payload_bytes.size(), payload->size());

  std::unique_ptr<AclPacketBuilder> packet =
      AclPacketBuilder::Create(handle, packet_boundary_flags, broadcast_flags, std::move(payload));

  ASSERT_EQ(information_request.size(), packet->size());

  std::shared_ptr<std::vector<uint8_t>> packet_bytes = packet->ToVector();
  AclPacketView packet_view = AclPacketView::Create(packet_bytes);

  ASSERT_EQ(packet_bytes->size(), information_request.size());
  for (size_t i = 0; i < packet_bytes->size(); i++) {
    ASSERT_EQ((*packet_bytes)[i], information_request[i]);
  }

  ASSERT_EQ(handle, packet_view.GetHandle());
  ASSERT_EQ(packet_boundary_flags, packet_view.GetPacketBoundaryFlags());
  ASSERT_EQ(broadcast_flags, packet_view.GetBroadcastFlags());
  PacketView<true> payload_view = packet_view.GetPayload();

  ASSERT_EQ(payload_view.size(), payload_bytes.size());
  for (size_t i = 0; i < payload_view.size(); i++) {
    ASSERT_EQ(payload_view[i], payload_bytes[i]);
  }

  ASSERT_EQ(packet_view.size(), information_request.size());
  for (size_t i = 0; i < packet_view.size(); i++) {
    ASSERT_EQ(packet_view[i], information_request[i]);
  }
}

}  // namespace packets
}  // namespace test_vendor_lib
