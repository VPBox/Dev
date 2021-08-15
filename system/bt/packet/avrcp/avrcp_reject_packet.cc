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

#include "avrcp_reject_packet.h"

namespace bluetooth {
namespace avrcp {

std::unique_ptr<RejectBuilder> RejectBuilder::MakeBuilder(CommandPdu pdu,
                                                          Status reason) {
  std::unique_ptr<RejectBuilder> builder =
      std::unique_ptr<RejectBuilder>(new RejectBuilder(pdu, reason));

  return builder;
}

size_t RejectBuilder::size() const { return VendorPacket::kMinSize() + 1; }

bool RejectBuilder::Serialize(const std::shared_ptr<::bluetooth::Packet>& pkt) {
  ReserveSpace(pkt, size());

  // Push the standard avrcp headers
  PacketBuilder::PushHeader(pkt);

  // Push the avrcp vendor command headers
  VendorPacketBuilder::PushHeader(pkt, 1);

  AddPayloadOctets1(pkt, static_cast<uint8_t>(reason_));

  return true;
}

}  // namespace avrcp
}  // namespace bluetooth