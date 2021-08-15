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

#include <cstdint>
#include <string>
#include <vector>

#include "link.h"
#include "packets/link_layer/command_builder.h"
#include "packets/link_layer/disconnect_builder.h"
#include "packets/link_layer/encrypt_connection_builder.h"
#include "packets/link_layer/inquiry_builder.h"
#include "packets/link_layer/inquiry_response_builder.h"
#include "packets/link_layer/io_capability_builder.h"
#include "packets/link_layer/io_capability_negative_response_builder.h"
#include "packets/link_layer/le_advertisement_builder.h"
#include "packets/link_layer/page_builder.h"
#include "packets/link_layer/page_response_builder.h"
#include "packets/link_layer/response_builder.h"
#include "packets/link_layer/view_forwarder_builder.h"
#include "packets/packet_builder.h"
#include "types/address.h"

namespace test_vendor_lib {
namespace packets {

// Link-layer packets are an abstraction of LMP PDUs.
class LinkLayerPacketBuilder : PacketBuilder<true> {
 public:
  virtual ~LinkLayerPacketBuilder() = default;

  static std::shared_ptr<LinkLayerPacketBuilder> WrapAcl(std::unique_ptr<ViewForwarderBuilder> acl,
                                                         const Address& source, const Address& dest);
  static std::shared_ptr<LinkLayerPacketBuilder> WrapCommand(std::unique_ptr<CommandBuilder> command,
                                                             const Address& source, const Address& dest);
  static std::shared_ptr<LinkLayerPacketBuilder> WrapDisconnect(std::unique_ptr<DisconnectBuilder> disconnect,
                                                                const Address& source, const Address& dest);
  static std::shared_ptr<LinkLayerPacketBuilder> WrapEncryptConnection(
      std::unique_ptr<EncryptConnectionBuilder> encrypt_connection, const Address& source, const Address& dest);
  static std::shared_ptr<LinkLayerPacketBuilder> WrapEncryptConnectionResponse(
      std::unique_ptr<EncryptConnectionBuilder> encrypt_connection, const Address& source, const Address& dest);
  static std::shared_ptr<LinkLayerPacketBuilder> WrapInquiry(std::unique_ptr<InquiryBuilder> inquiry,
                                                             const Address& source);
  static std::shared_ptr<LinkLayerPacketBuilder> WrapInquiryResponse(
      std::unique_ptr<InquiryResponseBuilder> inquiry_response, const Address& source, const Address& dest);
  static std::shared_ptr<LinkLayerPacketBuilder> WrapIoCapabilityRequest(
      std::unique_ptr<IoCapabilityBuilder> io_capability, const Address& source, const Address& dest);
  static std::shared_ptr<LinkLayerPacketBuilder> WrapIoCapabilityResponse(
      std::unique_ptr<IoCapabilityBuilder> io_capability, const Address& source, const Address& dest);
  static std::shared_ptr<LinkLayerPacketBuilder> WrapIoCapabilityNegativeResponse(
      std::unique_ptr<IoCapabilityNegativeResponseBuilder> io_capability_negative_response, const Address& source,
      const Address& dest);
  static std::shared_ptr<LinkLayerPacketBuilder> WrapLeAdvertisement(
      std::unique_ptr<LeAdvertisementBuilder> advertisement, const Address& source);
  static std::shared_ptr<LinkLayerPacketBuilder> WrapLeScan(const Address& source, const Address& dest);
  static std::shared_ptr<LinkLayerPacketBuilder> WrapLeScanResponse(
      std::unique_ptr<LeAdvertisementBuilder> scan_response, const Address& source, const Address& dest);
  static std::shared_ptr<LinkLayerPacketBuilder> WrapPage(std::unique_ptr<PageBuilder> page, const Address& source,
                                                          const Address& dest);
  static std::shared_ptr<LinkLayerPacketBuilder> WrapPageResponse(std::unique_ptr<PageResponseBuilder> page_response,
                                                                  const Address& source, const Address& dest);
  static std::shared_ptr<LinkLayerPacketBuilder> WrapResponse(const std::unique_ptr<ResponseBuilder> response,
                                                              const Address& source, const Address& dest);
  static std::shared_ptr<LinkLayerPacketBuilder> WrapSco(std::unique_ptr<ViewForwarderBuilder> sco,
                                                         const Address& source, const Address& dest);
  static std::shared_ptr<LinkLayerPacketBuilder> ReWrap(const std::shared_ptr<std::vector<uint8_t>> raw_packet);

  virtual void Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const override;

  virtual size_t size() const override;

 private:
  LinkLayerPacketBuilder(const LinkLayerPacketBuilder&) = delete;
  LinkLayerPacketBuilder() = delete;
  LinkLayerPacketBuilder(Link::PacketType type, const Address& source, const Address& dest);
  LinkLayerPacketBuilder(Link::PacketType type, std::unique_ptr<PacketBuilder> builder, const Address& source,
                         const Address& dest);
  LinkLayerPacketBuilder(Link::PacketType type, std::unique_ptr<PacketBuilder> builder, const Address& source);
  Link::PacketType type_;
  Address source_addr_;
  Address dest_addr_;
  std::unique_ptr<PacketBuilder> builder_;
};

}  // namespace packets
}  // namespace test_vendor_lib
