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

#include "general_reject_packet.h"

namespace bluetooth {
namespace avrcp {

std::unique_ptr<GeneralRejectBuilder> GeneralRejectBuilder::MakeBuilder(Status reason) {
  std::unique_ptr<GeneralRejectBuilder> builder =
      std::unique_ptr<GeneralRejectBuilder>(new GeneralRejectBuilder(reason));

  return builder;
}

size_t GeneralRejectBuilder::size() const {
  return BrowsePacket::kMinSize() + 1;
}

bool GeneralRejectBuilder::Serialize(
    const std::shared_ptr<::bluetooth::Packet>& pkt) {
  ReserveSpace(pkt, size());

  BrowsePacketBuilder::PushHeader(pkt, size() - BrowsePacket::kMinSize());

  AddPayloadOctets1(pkt, static_cast<uint8_t>(reason_));

  return true;
}

}  // namespace avrcp
}  // namespace bluetooth
