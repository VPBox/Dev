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

class GetTotalNumberOfItemsResponseBuilder : public BrowsePacketBuilder {
 public:
  virtual ~GetTotalNumberOfItemsResponseBuilder() = default;

  static std::unique_ptr<GetTotalNumberOfItemsResponseBuilder> MakeBuilder(
      Status status, uint16_t uid_counter, uint32_t num_items_in_folder);

  virtual size_t size() const override;
  virtual bool Serialize(
      const std::shared_ptr<::bluetooth::Packet>& pkt) override;

 protected:
  Status status_;
  uint16_t uid_counter_;
  uint32_t num_items_in_folder_;

  GetTotalNumberOfItemsResponseBuilder(Status status, uint16_t uid_counter,
                                       uint32_t num_items_in_folder)
      : BrowsePacketBuilder(BrowsePdu::GET_TOTAL_NUMBER_OF_ITEMS),
        status_(status),
        uid_counter_(uid_counter),
        num_items_in_folder_(num_items_in_folder){};
};

class GetTotalNumberOfItemsRequest : public BrowsePacket {
 public:
  virtual ~GetTotalNumberOfItemsRequest() = default;

  /**
   * AVRCP Get Total Number Of Items Packet Layout
   *   BrowsePacket:
   *     uint8_t pdu_;
   *     uint16_t length_;
   *   GetTotalNumberOfItemsRequest:
   *     uint8_t scope_;
   */
  static constexpr size_t kMinSize() { return BrowsePacket::kMinSize() + 1; }

  Scope GetScope() const;

  virtual bool IsValid() const override;
  virtual std::string ToString() const override;

 protected:
  using BrowsePacket::BrowsePacket;
};

}  // namespace avrcp
}  // namespace bluetooth