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

#include "link_layer_packet_builder.h"
#include "link_layer_packet_view.h"

#include "base/logging.h"

using std::vector;

namespace test_vendor_lib {

namespace packets {

LinkLayerPacketBuilder::LinkLayerPacketBuilder(Link::PacketType type, const Address& source, const Address& dest)
    : type_(type), source_addr_(source), dest_addr_(dest) {}

LinkLayerPacketBuilder::LinkLayerPacketBuilder(Link::PacketType type, std::unique_ptr<PacketBuilder> packet,
                                               const Address& source)
    : type_(type), source_addr_(source), dest_addr_(Address::kEmpty), builder_(std::move(packet)) {}

LinkLayerPacketBuilder::LinkLayerPacketBuilder(Link::PacketType type, std::unique_ptr<PacketBuilder> packet,
                                               const Address& source, const Address& dest)
    : type_(type), source_addr_(source), dest_addr_(dest), builder_(std::move(packet)) {}

std::shared_ptr<LinkLayerPacketBuilder> LinkLayerPacketBuilder::WrapAcl(std::unique_ptr<ViewForwarderBuilder> acl,
                                                                        const Address& source, const Address& dest) {
  return std::shared_ptr<LinkLayerPacketBuilder>(
      new LinkLayerPacketBuilder(Link::PacketType::ACL, std::move(acl), source, dest));
}

std::shared_ptr<LinkLayerPacketBuilder> LinkLayerPacketBuilder::WrapCommand(std::unique_ptr<CommandBuilder> command,
                                                                            const Address& source,
                                                                            const Address& dest) {
  return std::shared_ptr<LinkLayerPacketBuilder>(
      new LinkLayerPacketBuilder(Link::PacketType::COMMAND, std::move(command), source, dest));
}

std::shared_ptr<LinkLayerPacketBuilder> LinkLayerPacketBuilder::WrapDisconnect(
    std::unique_ptr<DisconnectBuilder> disconnect, const Address& source, const Address& dest) {
  return std::shared_ptr<LinkLayerPacketBuilder>(
      new LinkLayerPacketBuilder(Link::PacketType::DISCONNECT, std::move(disconnect), source, dest));
}

std::shared_ptr<LinkLayerPacketBuilder> LinkLayerPacketBuilder::WrapEncryptConnection(
    std::unique_ptr<EncryptConnectionBuilder> encrypt_connection, const Address& source, const Address& dest) {
  return std::shared_ptr<LinkLayerPacketBuilder>(
      new LinkLayerPacketBuilder(Link::PacketType::ENCRYPT_CONNECTION, std::move(encrypt_connection), source, dest));
}

std::shared_ptr<LinkLayerPacketBuilder> LinkLayerPacketBuilder::WrapEncryptConnectionResponse(
    std::unique_ptr<EncryptConnectionBuilder> encrypt_connection, const Address& source, const Address& dest) {
  return std::shared_ptr<LinkLayerPacketBuilder>(new LinkLayerPacketBuilder(
      Link::PacketType::ENCRYPT_CONNECTION_RESPONSE, std::move(encrypt_connection), source, dest));
}

std::shared_ptr<LinkLayerPacketBuilder> LinkLayerPacketBuilder::WrapInquiry(std::unique_ptr<InquiryBuilder> inquiry,
                                                                            const Address& source) {
  return std::shared_ptr<LinkLayerPacketBuilder>(
      new LinkLayerPacketBuilder(Link::PacketType::INQUIRY, std::move(inquiry), source));
}

std::shared_ptr<LinkLayerPacketBuilder> LinkLayerPacketBuilder::WrapInquiryResponse(
    std::unique_ptr<InquiryResponseBuilder> inquiry_response, const Address& source, const Address& dest) {
  return std::shared_ptr<LinkLayerPacketBuilder>(
      new LinkLayerPacketBuilder(Link::PacketType::INQUIRY_RESPONSE, std::move(inquiry_response), source, dest));
}

std::shared_ptr<LinkLayerPacketBuilder> LinkLayerPacketBuilder::WrapIoCapabilityRequest(
    std::unique_ptr<IoCapabilityBuilder> io_capability, const Address& source, const Address& dest) {
  return std::shared_ptr<LinkLayerPacketBuilder>(
      new LinkLayerPacketBuilder(Link::PacketType::IO_CAPABILITY_REQUEST, std::move(io_capability), source, dest));
}

std::shared_ptr<LinkLayerPacketBuilder> LinkLayerPacketBuilder::WrapIoCapabilityResponse(
    std::unique_ptr<IoCapabilityBuilder> io_capability, const Address& source, const Address& dest) {
  return std::shared_ptr<LinkLayerPacketBuilder>(
      new LinkLayerPacketBuilder(Link::PacketType::IO_CAPABILITY_RESPONSE, std::move(io_capability), source, dest));
}

std::shared_ptr<LinkLayerPacketBuilder> LinkLayerPacketBuilder::WrapIoCapabilityNegativeResponse(
    std::unique_ptr<IoCapabilityNegativeResponseBuilder> io_capability_negative_response, const Address& source,
    const Address& dest) {
  return std::shared_ptr<LinkLayerPacketBuilder>(new LinkLayerPacketBuilder(
      Link::PacketType::IO_CAPABILITY_NEGATIVE_RESPONSE, std::move(io_capability_negative_response), source, dest));
}

std::shared_ptr<LinkLayerPacketBuilder> LinkLayerPacketBuilder::WrapLeAdvertisement(
    std::unique_ptr<LeAdvertisementBuilder> advertisement, const Address& source) {
  return std::shared_ptr<LinkLayerPacketBuilder>(
      new LinkLayerPacketBuilder(Link::PacketType::LE_ADVERTISEMENT, std::move(advertisement), source));
}

std::shared_ptr<LinkLayerPacketBuilder> LinkLayerPacketBuilder::WrapLeScan(const Address& source, const Address& dest) {
  return std::shared_ptr<LinkLayerPacketBuilder>(new LinkLayerPacketBuilder(Link::PacketType::LE_SCAN, source, dest));
}

std::shared_ptr<LinkLayerPacketBuilder> LinkLayerPacketBuilder::WrapLeScanResponse(
    std::unique_ptr<LeAdvertisementBuilder> scan_response, const Address& source, const Address& dest) {
  return std::shared_ptr<LinkLayerPacketBuilder>(
      new LinkLayerPacketBuilder(Link::PacketType::LE_SCAN_RESPONSE, std ::move(scan_response), source, dest));
}

std::shared_ptr<LinkLayerPacketBuilder> LinkLayerPacketBuilder::WrapPage(std::unique_ptr<PageBuilder> page,
                                                                         const Address& source, const Address& dest) {
  return std::shared_ptr<LinkLayerPacketBuilder>(
      new LinkLayerPacketBuilder(Link::PacketType::PAGE, std::move(page), source, dest));
}

std::shared_ptr<LinkLayerPacketBuilder> LinkLayerPacketBuilder::WrapPageResponse(
    std::unique_ptr<PageResponseBuilder> page_response, const Address& source, const Address& dest) {
  return std::shared_ptr<LinkLayerPacketBuilder>(
      new LinkLayerPacketBuilder(Link::PacketType::PAGE_RESPONSE, std::move(page_response), source, dest));
}

std::shared_ptr<LinkLayerPacketBuilder> LinkLayerPacketBuilder::WrapResponse(std::unique_ptr<ResponseBuilder> response,
                                                                             const Address& source,
                                                                             const Address& dest) {
  return std::shared_ptr<LinkLayerPacketBuilder>(
      new LinkLayerPacketBuilder(Link::PacketType::RESPONSE, std::move(response), source, dest));
}

std::shared_ptr<LinkLayerPacketBuilder> LinkLayerPacketBuilder::WrapSco(std::unique_ptr<ViewForwarderBuilder> sco,
                                                                        const Address& source, const Address& dest) {
  return std::shared_ptr<LinkLayerPacketBuilder>(
      new LinkLayerPacketBuilder(Link::PacketType::SCO, std::move(sco), source, dest));
}

std::shared_ptr<LinkLayerPacketBuilder> LinkLayerPacketBuilder::ReWrap(
    const std::shared_ptr<std::vector<uint8_t>> raw_packet) {
  LinkLayerPacketView received = LinkLayerPacketView::Create(raw_packet);
  Link::PacketType packet_type = received.GetType();
  Address source = received.GetSourceAddress();
  Address dest = received.GetDestinationAddress();
  PacketView<true> payload = received.GetPayload();
  std::unique_ptr<PacketBuilder> builder = ViewForwarderBuilder::Create(payload);
  return std::shared_ptr<LinkLayerPacketBuilder>(
      new LinkLayerPacketBuilder(packet_type, std::move(builder), source, dest));
}

size_t LinkLayerPacketBuilder::size() const {
  size_t builder_size = (builder_ ? builder_->size() : 0);
  return Link::kTypeBytes + Link::kSizeBytes + 2 * Address::kLength + builder_size;
}

void LinkLayerPacketBuilder::Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const {
  insert(static_cast<uint32_t>(size() - Link::kSizeBytes), it);
  insert(static_cast<uint8_t>(type_), it);
  insert_address(source_addr_.address, it);
  insert_address(dest_addr_.address, it);
  if (builder_) {
    builder_->Serialize(it);
  }
}

}  // namespace packets
}  // namespace test_vendor_lib
