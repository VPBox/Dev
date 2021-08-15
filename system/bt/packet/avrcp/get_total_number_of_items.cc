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

#include "get_total_number_of_items.h"

namespace bluetooth {
namespace avrcp {

std::unique_ptr<GetTotalNumberOfItemsResponseBuilder>
GetTotalNumberOfItemsResponseBuilder::MakeBuilder(
    Status status, uint16_t uid_counter, uint32_t num_items_in_folder) {
  std::unique_ptr<GetTotalNumberOfItemsResponseBuilder> builder(
      new GetTotalNumberOfItemsResponseBuilder(status, uid_counter,
                                               num_items_in_folder));

  return builder;
}

size_t GetTotalNumberOfItemsResponseBuilder::size() const {
  size_t len = BrowsePacket::kMinSize();
  len += 1;  // Status

  if (status_ != Status::NO_ERROR) return len;

  len += 2;  // UID Counter
  len += 4;  // Number of items in folder
  return len;
}

bool GetTotalNumberOfItemsResponseBuilder::Serialize(
    const std::shared_ptr<::bluetooth::Packet>& pkt) {
  ReserveSpace(pkt, size());

  BrowsePacketBuilder::PushHeader(pkt, size() - BrowsePacket::kMinSize());

  AddPayloadOctets1(pkt, (uint8_t)status_);

  if (status_ != Status::NO_ERROR) return true;
  AddPayloadOctets2(pkt, base::ByteSwap(uid_counter_));
  AddPayloadOctets4(pkt, base::ByteSwap(num_items_in_folder_));
  return true;
}

Scope GetTotalNumberOfItemsRequest::GetScope() const {
  auto it = begin() + BrowsePacket::kMinSize();
  return static_cast<Scope>(*it);
}

bool GetTotalNumberOfItemsRequest::IsValid() const {
  if (!BrowsePacket::IsValid()) return false;
  return size() == kMinSize();
}

std::string GetTotalNumberOfItemsRequest::ToString() const {
  std::stringstream ss;
  ss << "GetTotalNumberOfItemsRequest: " << std::endl;
  ss << "  └ PDU = " << GetPdu() << std::endl;
  ss << "  └ Length = " << GetLength() << std::endl;
  ss << "  └ Scope = " << GetScope() << std::endl;
  ss << std::endl;

  return ss.str();
}

}  // namespace avrcp
}  // namespace bluetooth
