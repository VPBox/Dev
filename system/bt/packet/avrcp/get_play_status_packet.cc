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

#include "get_play_status_packet.h"

namespace bluetooth {
namespace avrcp {

std::string GetPlayStatusRequest::ToString() const {
  return "GetPlayStatusRequest";
}

std::unique_ptr<GetPlayStatusResponseBuilder>
GetPlayStatusResponseBuilder::MakeBuilder(uint32_t song_length,
                                          uint32_t song_position,
                                          uint8_t play_status) {
  std::unique_ptr<GetPlayStatusResponseBuilder> builder(
      new GetPlayStatusResponseBuilder(song_length, song_position,
                                       play_status));

  return builder;
}

size_t GetPlayStatusResponseBuilder::size() const {
  return VendorPacket::kMinSize() + 4 + 4 + 1;
}

bool GetPlayStatusResponseBuilder::Serialize(
    const std::shared_ptr<::bluetooth::Packet>& pkt) {
  ReserveSpace(pkt, size());

  // Push the standard avrcp headers
  PacketBuilder::PushHeader(pkt);

  // Push the avrcp vendor command headers
  uint16_t parameter_count = size() - VendorPacket::kMinSize();
  VendorPacketBuilder::PushHeader(pkt, parameter_count);

  AddPayloadOctets4(pkt, base::ByteSwap(song_length_));
  AddPayloadOctets4(pkt, base::ByteSwap(song_position_));
  AddPayloadOctets1(pkt, play_status_);

  return true;
}

}  // namespace avrcp
}  // namespace bluetooth