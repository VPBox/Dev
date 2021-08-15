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

#pragma once

#include "avrcp_browse_packet.h"

namespace bluetooth {
namespace avrcp {

class GetFolderItemsResponseBuilder : public BrowsePacketBuilder {
 public:
  virtual ~GetFolderItemsResponseBuilder() = default;
  static std::unique_ptr<GetFolderItemsResponseBuilder> MakePlayerListBuilder(
      Status status, uint16_t uid_counter, size_t mtu);
  static std::unique_ptr<GetFolderItemsResponseBuilder> MakeVFSBuilder(
      Status status, uint16_t uid_counter, size_t mtu);
  static std::unique_ptr<GetFolderItemsResponseBuilder> MakeNowPlayingBuilder(
      Status status, uint16_t uid_counter, size_t mtu);

  virtual size_t size() const override;
  virtual bool Serialize(
      const std::shared_ptr<::bluetooth::Packet>& pkt) override;

  // Returns false if adding an item would exceed the MTU
  bool AddMediaPlayer(MediaPlayerItem item);
  bool AddSong(MediaElementItem item);
  bool AddFolder(FolderItem item);

 protected:
  Scope scope_;
  std::vector<MediaListItem> items_;
  Status status_;
  uint16_t uid_counter_;
  size_t mtu_;

  GetFolderItemsResponseBuilder(Scope scope, Status status,
                                uint16_t uid_counter, size_t mtu)
      : BrowsePacketBuilder(BrowsePdu::GET_FOLDER_ITEMS),
        scope_(scope),
        status_(status),
        uid_counter_(uid_counter),
        mtu_(mtu){};

 private:
  void PushMediaListItem(const std::shared_ptr<::bluetooth::Packet>& pkt,
                         const MediaListItem& item);
  void PushMediaPlayerItem(const std::shared_ptr<::bluetooth::Packet>& pkt,
                           const MediaPlayerItem& item);
  void PushMediaElementItem(const std::shared_ptr<::bluetooth::Packet>& pkt,
                            const MediaElementItem& item);
  void PushFolderItem(const std::shared_ptr<::bluetooth::Packet>& pkt,
                      const FolderItem& item);
};

class GetFolderItemsRequest : public BrowsePacket {
 public:
  virtual ~GetFolderItemsRequest() = default;

  /**
   * Avrcp Change Path Packet Layout
   *   BrowsePacket:
   *     uint8_t pdu_;
   *     uint16_t length_;
   *   GetFolderItemsRequest:
   *     uint8_t scope_;
   *     uint32_t start_item_;
   *     uint32_t end_item_;
   *     uint8_t attr_count_;
   *     uint32_t[] attr_requested_;
   */
  static constexpr size_t kMinSize() { return BrowsePacket::kMinSize() + 10; }

  Scope GetScope() const;
  uint32_t GetStartItem() const;
  uint32_t GetEndItem() const;
  uint8_t GetNumAttributes() const;
  std::vector<Attribute> GetAttributesRequested() const;

  virtual bool IsValid() const override;
  virtual std::string ToString() const override;

 protected:
  using BrowsePacket::BrowsePacket;
};

class GetFolderItemsRequestBuilder : public BrowsePacketBuilder {
 public:
  virtual ~GetFolderItemsRequestBuilder() = default;

  static std::unique_ptr<GetFolderItemsRequestBuilder> MakeBuilder(
      Scope scope, uint32_t start_item, uint32_t end_item,
      const std::set<Attribute>& requested_attrs);

  virtual size_t size() const override;
  virtual bool Serialize(
      const std::shared_ptr<::bluetooth::Packet>& pkt) override;

 protected:
  GetFolderItemsRequestBuilder(Scope scope, uint32_t start_item,
                               uint32_t end_item,
                               const std::set<Attribute>& requested_attrs)
      : BrowsePacketBuilder(BrowsePdu::GET_FOLDER_ITEMS),
        scope_(scope),
        start_item_(start_item),
        end_item_(end_item),
        requested_attrs_(requested_attrs){};

  Scope scope_;
  uint32_t start_item_;
  uint32_t end_item_;
  std::set<Attribute> requested_attrs_;
};

}  // namespace avrcp
}  // namespace bluetooth