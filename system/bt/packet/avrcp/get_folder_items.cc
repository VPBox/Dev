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

#include "get_folder_items.h"

namespace bluetooth {
namespace avrcp {

std::unique_ptr<GetFolderItemsResponseBuilder>
GetFolderItemsResponseBuilder::MakePlayerListBuilder(Status status,
                                                     uint16_t uid_counter,
                                                     size_t mtu) {
  std::unique_ptr<GetFolderItemsResponseBuilder> builder(
      new GetFolderItemsResponseBuilder(Scope::MEDIA_PLAYER_LIST, status,
                                        uid_counter, mtu));

  return builder;
}

std::unique_ptr<GetFolderItemsResponseBuilder>
GetFolderItemsResponseBuilder::MakeVFSBuilder(Status status,
                                              uint16_t uid_counter,
                                              size_t mtu) {
  std::unique_ptr<GetFolderItemsResponseBuilder> builder(
      new GetFolderItemsResponseBuilder(Scope::VFS, status, uid_counter, mtu));

  return builder;
}

std::unique_ptr<GetFolderItemsResponseBuilder>
GetFolderItemsResponseBuilder::MakeNowPlayingBuilder(Status status,
                                                     uint16_t uid_counter,
                                                     size_t mtu) {
  std::unique_ptr<GetFolderItemsResponseBuilder> builder(
      new GetFolderItemsResponseBuilder(Scope::NOW_PLAYING, status, uid_counter,
                                        mtu));

  return builder;
}

size_t GetFolderItemsResponseBuilder::size() const {
  size_t len = BrowsePacket::kMinSize();
  len += 1;  // Status

  // There is nothing other than the status in the packet if the status isn't
  // NO_ERROR
  if (status_ != Status::NO_ERROR || items_.size() == 0) return len;

  len += 2;  // UID Counter
  len += 2;  // Number of Items;
  for (const auto& item : items_) {
    len += item.size();
  }

  return len;
}

bool GetFolderItemsResponseBuilder::Serialize(
    const std::shared_ptr<::bluetooth::Packet>& pkt) {
  ReserveSpace(pkt, size());

  BrowsePacketBuilder::PushHeader(pkt, size() - BrowsePacket::kMinSize());

  if (status_ == Status::NO_ERROR && items_.size() == 0) {
    // Return range out of bounds if there are zero items in the folder
    status_ = Status::RANGE_OUT_OF_BOUNDS;
  }

  AddPayloadOctets1(pkt, (uint8_t)status_);  // Status
  if (status_ != Status::NO_ERROR) return true;

  AddPayloadOctets2(pkt, base::ByteSwap(uid_counter_));
  uint16_t num_items = items_.size();
  AddPayloadOctets2(pkt, base::ByteSwap(num_items));

  for (const auto& item : items_) {
    PushMediaListItem(pkt, item);
  }

  return true;
}

bool GetFolderItemsResponseBuilder::AddMediaPlayer(MediaPlayerItem item) {
  CHECK(scope_ == Scope::MEDIA_PLAYER_LIST);

  if (size() + item.size() > mtu_) return false;

  items_.push_back(MediaListItem(item));
  return true;
}

bool GetFolderItemsResponseBuilder::AddSong(MediaElementItem item) {
  CHECK(scope_ == Scope::VFS || scope_ == Scope::NOW_PLAYING);

  if (size() + item.size() > mtu_) return false;

  items_.push_back(MediaListItem(item));
  return true;
}

bool GetFolderItemsResponseBuilder::AddFolder(FolderItem item) {
  CHECK(scope_ == Scope::VFS);

  if (size() + item.size() > mtu_) return false;

  items_.push_back(MediaListItem(item));
  return true;
}

void GetFolderItemsResponseBuilder::PushMediaListItem(
    const std::shared_ptr<::bluetooth::Packet>& pkt,
    const MediaListItem& item) {
  switch (item.type_) {
    case MediaListItem::PLAYER:
      PushMediaPlayerItem(pkt, item.player_);
      break;
    case MediaListItem::FOLDER:
      PushFolderItem(pkt, item.folder_);
      break;
    case MediaListItem::SONG:
      PushMediaElementItem(pkt, item.song_);
      break;
  }
}

void GetFolderItemsResponseBuilder::PushMediaPlayerItem(
    const std::shared_ptr<::bluetooth::Packet>& pkt,
    const MediaPlayerItem& item) {
  AddPayloadOctets1(pkt, 0x01);  // Media Player Item
  uint16_t item_len = item.size() - 3;
  AddPayloadOctets2(pkt, base::ByteSwap(item_len));  // Item length
  AddPayloadOctets2(pkt, base::ByteSwap(item.id_));  // Player ID
  AddPayloadOctets1(pkt, 0x01);                      // Player Type
  AddPayloadOctets4(pkt, 0x00000000);                // Player Subtype
  AddPayloadOctets1(
      pkt, 0x02);  // Player Play Status // TODO: Add this as a passed field

  // Features
  AddPayloadOctets1(pkt, 0x00);
  AddPayloadOctets1(pkt, 0x00);
  AddPayloadOctets1(pkt, 0x00);
  AddPayloadOctets1(pkt, 0x00);
  AddPayloadOctets1(pkt, 0x00);
  AddPayloadOctets1(pkt, 0xb7);
  AddPayloadOctets1(pkt, 0x01);
  if (item.browsable_) {
    AddPayloadOctets1(pkt, 0x0C);
    AddPayloadOctets1(pkt, 0x0a);
  } else {
    AddPayloadOctets1(pkt, 0x04);
    AddPayloadOctets1(pkt, 0x00);
  }
  AddPayloadOctets1(pkt, 0x00);
  AddPayloadOctets1(pkt, 0x00);
  AddPayloadOctets1(pkt, 0x00);
  AddPayloadOctets1(pkt, 0x00);
  AddPayloadOctets1(pkt, 0x00);
  AddPayloadOctets1(pkt, 0x00);
  AddPayloadOctets1(pkt, 0x00);

  AddPayloadOctets2(pkt, base::ByteSwap((uint16_t)0x006a));
  uint16_t name_len = item.name_.size();
  AddPayloadOctets2(pkt, base::ByteSwap(name_len));

  for (const uint8_t& byte : item.name_) {
    AddPayloadOctets1(pkt, byte);
  }
}

void GetFolderItemsResponseBuilder::PushFolderItem(
    const std::shared_ptr<::bluetooth::Packet>& pkt, const FolderItem& item) {
  AddPayloadOctets1(pkt, 0x02);  // Folder Item
  uint16_t item_len = item.size() - 3;
  AddPayloadOctets2(pkt, base::ByteSwap(item_len));
  AddPayloadOctets8(pkt, base::ByteSwap(item.uid_));
  AddPayloadOctets1(pkt, item.folder_type_);
  AddPayloadOctets1(pkt, item.is_playable_ ? 0x01 : 0x00);
  AddPayloadOctets2(pkt,
                    base::ByteSwap((uint16_t)0x006a));  // UTF-8 Character Set
  uint16_t name_len = item.name_.size();
  AddPayloadOctets2(pkt, base::ByteSwap(name_len));
  for (const uint8_t& byte : item.name_) {
    AddPayloadOctets1(pkt, byte);
  }
}

void GetFolderItemsResponseBuilder::PushMediaElementItem(
    const std::shared_ptr<::bluetooth::Packet>& pkt,
    const MediaElementItem& item) {
  AddPayloadOctets1(pkt, 0x03);  // Media Element Item
  uint16_t item_len = item.size() - 3;
  AddPayloadOctets2(pkt, base::ByteSwap(item_len));
  AddPayloadOctets8(pkt, base::ByteSwap(item.uid_));
  AddPayloadOctets1(pkt, 0x00);  // Media Type Audio
  AddPayloadOctets2(pkt,
                    base::ByteSwap((uint16_t)0x006a));  // UTF-8 Character Set
  uint16_t name_len = item.name_.size();
  AddPayloadOctets2(pkt, base::ByteSwap(name_len));
  for (const uint8_t& byte : item.name_) {
    AddPayloadOctets1(pkt, byte);
  }

  AddPayloadOctets1(pkt, (uint8_t)item.attributes_.size());
  for (const auto& entry : item.attributes_) {
    AddPayloadOctets4(pkt, base::ByteSwap((uint32_t)entry.attribute()));
    AddPayloadOctets2(pkt,
                      base::ByteSwap((uint16_t)0x006a));  // UTF-8 Character Set

    std::string attr_val = entry.value();
    uint16_t attr_len = attr_val.size();

    AddPayloadOctets2(pkt, base::ByteSwap(attr_len));
    for (const uint8_t& byte : attr_val) {
      AddPayloadOctets1(pkt, byte);
    }
  }
}

Scope GetFolderItemsRequest::GetScope() const {
  auto it = begin() + BrowsePacket::kMinSize();
  return static_cast<Scope>(*it);
}

uint32_t GetFolderItemsRequest::GetStartItem() const {
  auto it = begin() + BrowsePacket::kMinSize() + static_cast<size_t>(1);
  return it.extractBE<uint32_t>();
}

uint32_t GetFolderItemsRequest::GetEndItem() const {
  auto it = begin() + BrowsePacket::kMinSize() + static_cast<size_t>(5);
  return it.extractBE<uint32_t>();
}

uint8_t GetFolderItemsRequest::GetNumAttributes() const {
  auto it = begin() + BrowsePacket::kMinSize() + static_cast<size_t>(9);
  return *it;
}

std::vector<Attribute> GetFolderItemsRequest::GetAttributesRequested() const {
  auto it = begin() + BrowsePacket::kMinSize() + static_cast<size_t>(9);

  size_t number_of_attributes = it.extract<uint8_t>();
  std::vector<Attribute> attribute_list;

  // No attributes requested
  if (number_of_attributes == 0xFF) return attribute_list;

  // TODO: If the number of attributes equals 0, then all attributes are
  // requested right now thats handled in the service itself, but it'd be nice
  // to have this function return a vector with all the attributes

  for (size_t i = 0; i < number_of_attributes; i++) {
    attribute_list.push_back((Attribute)it.extractBE<uint32_t>());
  }

  return attribute_list;
}

bool GetFolderItemsRequest::IsValid() const {
  if (!BrowsePacket::IsValid()) return false;
  // The minimum size required to be valid
  if (size() < kMinSize()) return false;

  auto attr_count = GetNumAttributes();

  // No items requested
  if (attr_count == 0xFF) return true;

  auto attr_start = begin() + kMinSize();

  // Casting the int returned from end - attr_start should be fine. If an
  // overflow occurs we can definitly say the packet is invalid
  return (attr_count * sizeof(Attribute)) == (size_t)(end() - attr_start);
}

std::string GetFolderItemsRequest::ToString() const {
  std::stringstream ss;
  ss << "GetFolderItemsRequestPacket: " << std::endl;
  ss << "  └ PDU = " << GetPdu() << std::endl;
  ss << "  └ Length = " << GetLength() << std::endl;
  ss << "  └ Scope = " << GetScope() << std::endl;
  ss << "  └ Start Item = " << loghex(GetStartItem()) << std::endl;
  ss << "  └ End Item = " << loghex(GetEndItem()) << std::endl;
  ss << "  └ Attribute Count = " << loghex(GetNumAttributes()) << std::endl;

  ss << std::endl;

  return ss.str();
}

std::unique_ptr<GetFolderItemsRequestBuilder>
GetFolderItemsRequestBuilder::MakeBuilder(
    Scope scope, uint32_t start_item, uint32_t end_item,
    const std::set<Attribute>& requested_attrs) {
  std::unique_ptr<GetFolderItemsRequestBuilder> builder(
      new GetFolderItemsRequestBuilder(scope, start_item, end_item,
                                       requested_attrs));

  return builder;
}

size_t GetFolderItemsRequestBuilder::size() const {
  size_t len = GetFolderItemsRequest::kMinSize();
  len += requested_attrs_.size() * sizeof(Attribute);
  return len;
}

bool GetFolderItemsRequestBuilder::Serialize(
    const std::shared_ptr<::bluetooth::Packet>& pkt) {
  ReserveSpace(pkt, size());

  BrowsePacketBuilder::PushHeader(pkt, size() - BrowsePacket::kMinSize());

  AddPayloadOctets1(pkt, static_cast<uint8_t>(scope_));
  AddPayloadOctets4(pkt, base::ByteSwap(start_item_));
  AddPayloadOctets4(pkt, base::ByteSwap(end_item_));

  if (requested_attrs_.size() == 0) {
    // 0xFF is the value to signify that there are no attributes requested.
    AddPayloadOctets1(pkt, 0xFF);
    return true;
  }

  AddPayloadOctets1(pkt, requested_attrs_.size());
  for (const auto& attr : requested_attrs_) {
    AddPayloadOctets4(pkt, base::ByteSwap(static_cast<uint32_t>(attr)));
  }
  return true;
}

}  // namespace avrcp
}  // namespace bluetooth
