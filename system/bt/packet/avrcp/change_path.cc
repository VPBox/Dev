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

#include "change_path.h"

namespace bluetooth {
namespace avrcp {

std::unique_ptr<ChangePathResponseBuilder>
ChangePathResponseBuilder::MakeBuilder(Status status,
                                       uint32_t num_items_in_folder) {
  std::unique_ptr<ChangePathResponseBuilder> builder(
      new ChangePathResponseBuilder(status, num_items_in_folder));

  return builder;
}

size_t ChangePathResponseBuilder::size() const {
  size_t len = BrowsePacket::kMinSize();
  len += 1;  // Status
  if (status_ != Status::NO_ERROR) return len;

  len += 4;  // Number of items in folder
  return len;
}

bool ChangePathResponseBuilder::Serialize(
    const std::shared_ptr<::bluetooth::Packet>& pkt) {
  ReserveSpace(pkt, size());

  BrowsePacketBuilder::PushHeader(pkt, size() - BrowsePacket::kMinSize());

  AddPayloadOctets1(pkt, (uint8_t)status_);
  if (status_ != Status::NO_ERROR) return true;

  AddPayloadOctets4(pkt, base::ByteSwap(num_items_in_folder_));
  return true;
}

uint16_t ChangePathRequest::GetUidCounter() const {
  auto it = begin() + BrowsePacket::kMinSize();
  return it.extractBE<uint16_t>();
}

Direction ChangePathRequest::GetDirection() const {
  auto it = begin() + BrowsePacket::kMinSize() + static_cast<size_t>(2);
  return static_cast<Direction>(*it);
}

uint64_t ChangePathRequest::GetUid() const {
  auto it = begin() + BrowsePacket::kMinSize() + static_cast<size_t>(3);
  return it.extractBE<uint64_t>();
}

bool ChangePathRequest::IsValid() const {
  if (!BrowsePacket::IsValid()) return false;
  // Change path request packets are always the same size
  return size() == kMinSize();
}

std::string ChangePathRequest::ToString() const {
  std::stringstream ss;
  ss << "ChangePathRequestPacket: " << std::endl;
  ss << "  └ PDU = " << GetPdu() << std::endl;
  ss << "  └ Length = " << GetLength() << std::endl;
  ss << "  └ UID Counter = " << loghex(GetUidCounter()) << std::endl;
  ss << "  └ Direction = " << GetDirection() << std::endl;
  ss << "  └ UID Requested = " << loghex(GetUid()) << std::endl;
  ss << std::endl;

  return ss.str();
}

std::unique_ptr<ChangePathRequestBuilder> ChangePathRequestBuilder::MakeBuilder(
    uint16_t uid_counter, Direction direction, uint64_t folder_uid) {
  std::unique_ptr<ChangePathRequestBuilder> builder(
      new ChangePathRequestBuilder(uid_counter, direction, folder_uid));

  return builder;
}

size_t ChangePathRequestBuilder::size() const {
  return ChangePathRequest::kMinSize();
}

bool ChangePathRequestBuilder::Serialize(
    const std::shared_ptr<::bluetooth::Packet>& pkt) {
  ReserveSpace(pkt, size());

  BrowsePacketBuilder::PushHeader(pkt, size() - BrowsePacket::kMinSize());

  AddPayloadOctets2(pkt, base::ByteSwap(uid_counter_));
  AddPayloadOctets1(pkt, static_cast<uint8_t>(direction_));
  AddPayloadOctets8(pkt, base::ByteSwap(folder_uid_));
  return true;
}

}  // namespace avrcp
}  // namespace bluetooth
