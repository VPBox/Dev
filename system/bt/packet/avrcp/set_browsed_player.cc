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

#include "set_browsed_player.h"

namespace bluetooth {
namespace avrcp {

std::unique_ptr<SetBrowsedPlayerResponseBuilder>
SetBrowsedPlayerResponseBuilder::MakeBuilder(Status status,
                                             uint16_t uid_counter,
                                             uint32_t num_items_in_folder,
                                             uint8_t folder_depth,
                                             std::string folder_name) {
  std::unique_ptr<SetBrowsedPlayerResponseBuilder> builder(
      new SetBrowsedPlayerResponseBuilder(
          status, uid_counter, num_items_in_folder, folder_depth, folder_name));

  return builder;
}

size_t SetBrowsedPlayerResponseBuilder::size() const {
  size_t len = BrowsePacket::kMinSize();
  len += 1;  // Status

  // If the status isn't success the rest of the fields are ommited
  if (status_ != Status::NO_ERROR) return len;

  len += 2;  // UID Counter
  len += 4;  // Number of items in folder
  len += 2;  // UTF-8 Character Set
  len += 1;  // Folder Depth

  // This is only included if the folder returned isn't the root folder
  if (folder_depth_ != 0) {
    len += 2;                    // Folder Name Size;
    len += folder_name_.size();  // Folder Name
  }

  return len;
}

bool SetBrowsedPlayerResponseBuilder::Serialize(
    const std::shared_ptr<::bluetooth::Packet>& pkt) {
  ReserveSpace(pkt, size());

  BrowsePacketBuilder::PushHeader(pkt, size() - BrowsePacket::kMinSize());

  AddPayloadOctets1(pkt, (uint8_t)status_);

  if (status_ != Status::NO_ERROR) return true;
  AddPayloadOctets2(pkt, base::ByteSwap(uid_counter_));
  AddPayloadOctets4(pkt, base::ByteSwap(num_items_in_folder_));
  AddPayloadOctets2(pkt, base::ByteSwap((uint16_t)0x006a));  // UTF-8
  AddPayloadOctets1(pkt, folder_depth_);

  // Skip adding the folder name if the folder depth is 0
  if (folder_depth_ == 0) return true;
  uint16_t folder_name_len = folder_name_.size();
  AddPayloadOctets2(pkt, base::ByteSwap(folder_name_len));
  for (auto it = folder_name_.begin(); it != folder_name_.end(); it++) {
    AddPayloadOctets1(pkt, *it);
  }

  return true;
}

uint16_t SetBrowsedPlayerRequest::GetPlayerId() const {
  auto it = begin() + BrowsePacket::kMinSize();
  return it.extractBE<uint16_t>();
}

bool SetBrowsedPlayerRequest::IsValid() const {
  if (!BrowsePacket::IsValid()) return false;
  return size() == kMinSize();
}

std::string SetBrowsedPlayerRequest::ToString() const {
  std::stringstream ss;
  ss << "SetBrowsedPlayerRequestPacket: " << std::endl;
  ss << "  └ PDU = " << GetPdu() << std::endl;
  ss << "  └ Length = " << GetLength() << std::endl;
  ss << "  └ Player ID = " << loghex(GetPlayerId()) << std::endl;
  ss << std::endl;

  return ss.str();
}

}  // namespace avrcp
}  // namespace bluetooth
