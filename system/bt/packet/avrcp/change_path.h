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

class ChangePathResponseBuilder : public BrowsePacketBuilder {
 public:
  virtual ~ChangePathResponseBuilder() = default;

  static std::unique_ptr<ChangePathResponseBuilder> MakeBuilder(
      Status status, uint32_t num_items_in_folder);

  virtual size_t size() const override;
  virtual bool Serialize(
      const std::shared_ptr<::bluetooth::Packet>& pkt) override;

 private:
  Status status_;
  uint32_t num_items_in_folder_;

  ChangePathResponseBuilder(Status status, uint32_t num_items_in_folder)
      : BrowsePacketBuilder(BrowsePdu::CHANGE_PATH),
        status_(status),
        num_items_in_folder_(num_items_in_folder) {}
};

class ChangePathRequest : public BrowsePacket {
 public:
  virtual ~ChangePathRequest() = default;

  /**
   * Avrcp Change Path Packet Layout
   *   BrowsePacket:
   *     uint8_t pdu_;
   *     uint16_t length_;
   *   ChangePathRequest:
   *     uint16_t uid_counter_;
   *     uint8_t direction_;
   *     uint64_t folder_uid_;
   */
  static constexpr size_t kMinSize() { return BrowsePacket::kMinSize() + 11; }

  uint16_t GetUidCounter() const;
  Direction GetDirection() const;
  uint64_t GetUid() const;

  virtual bool IsValid() const override;
  virtual std::string ToString() const override;

 protected:
  using BrowsePacket::BrowsePacket;
};

class ChangePathRequestBuilder : public BrowsePacketBuilder {
 public:
  virtual ~ChangePathRequestBuilder() = default;

  static std::unique_ptr<ChangePathRequestBuilder> MakeBuilder(
      uint16_t uid_counter, Direction direction, uint64_t folder_uid);

  virtual size_t size() const override;
  virtual bool Serialize(
      const std::shared_ptr<::bluetooth::Packet>& pkt) override;

 private:
  ChangePathRequestBuilder(uint16_t uid_counter, Direction direction,
                           uint64_t folder_uid)
      : BrowsePacketBuilder(BrowsePdu::CHANGE_PATH),
        uid_counter_(uid_counter),
        direction_(direction),
        folder_uid_(folder_uid){};

  uint16_t uid_counter_;
  Direction direction_;
  uint64_t folder_uid_;
};

}  // namespace avrcp
}  // namespace bluetooth