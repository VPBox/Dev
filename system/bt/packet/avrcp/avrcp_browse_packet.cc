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

#include "avrcp_browse_packet.h"

#include <base/logging.h>

namespace bluetooth {
namespace avrcp {

std::unique_ptr<BrowsePacketBuilder> BrowsePacketBuilder::MakeBuilder(
    BrowsePdu pdu, std::unique_ptr<::bluetooth::PacketBuilder> payload) {
  std::unique_ptr<BrowsePacketBuilder> builder =
      std::unique_ptr<BrowsePacketBuilder>(new BrowsePacketBuilder(pdu));

  builder->payload_ = std::move(payload);

  return builder;
}

size_t BrowsePacketBuilder::size() const {
  return BrowsePacket::kMinSize() + payload_->size();
}

bool BrowsePacketBuilder::Serialize(
    const std::shared_ptr<::bluetooth::Packet>& pkt) {
  ReserveSpace(pkt, size());

  PushHeader(pkt, payload_->size());

  return payload_->Serialize(pkt);
}

void BrowsePacketBuilder::PushHeader(
    const std::shared_ptr<::bluetooth::Packet>& pkt, uint16_t length) {
  AddPayloadOctets1(pkt, (uint8_t)pdu_);
  AddPayloadOctets2(pkt, base::ByteSwap(length));
}

std::shared_ptr<BrowsePacket> BrowsePacket::Parse(
    std::shared_ptr<::bluetooth::Packet> pkt) {
  return std::shared_ptr<BrowsePacket>(new BrowsePacket(pkt));
}

BrowsePdu BrowsePacket::GetPdu() const {
  return static_cast<BrowsePdu>(*begin());
}

uint16_t BrowsePacket::GetLength() const {
  auto it = begin() + static_cast<size_t>(1);
  return it.extractBE<uint16_t>();
}

bool BrowsePacket::IsValid() const {
  if (size() < kMinSize()) return false;
  return size() == GetLength() + kMinSize();
}

std::string BrowsePacket::ToString() const {
  std::stringstream ss;
  ss << "AvrcpBrowsePacket: " << std::endl;
  ss << "  └ PDU = " << GetPdu() << std::endl;
  ss << "  └ Length = " << GetLength() << std::endl;
  ss << "  └ Payload =";
  for (auto it = begin() + static_cast<size_t>(3); it != end(); it++) {
    ss << " " << loghex(*it);
  }
  ss << std::endl;

  return ss.str();
}

std::pair<size_t, size_t> BrowsePacket::GetPayloadIndecies() const {
  return std::pair<size_t, size_t>(packet_start_index_ + 3, packet_end_index_);
}

}  // namespace avrcp
}  // namespace bluetooth
