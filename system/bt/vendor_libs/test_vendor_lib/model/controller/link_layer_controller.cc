/*
 * Copyright 2017 The Android Open Source Project
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

#define LOG_TAG "link_layer_controller"

#include "link_layer_controller.h"

#include <base/logging.h>

#include "hci.h"
#include "osi/include/log.h"
#include "packets/hci/acl_packet_builder.h"
#include "packets/hci/command_packet_view.h"
#include "packets/hci/event_packet_builder.h"
#include "packets/hci/sco_packet_builder.h"
#include "packets/link_layer/command_builder.h"
#include "packets/link_layer/command_view.h"
#include "packets/link_layer/disconnect_view.h"
#include "packets/link_layer/encrypt_connection_view.h"
#include "packets/link_layer/inquiry_response_view.h"
#include "packets/link_layer/inquiry_view.h"
#include "packets/link_layer/io_capability_view.h"
#include "packets/link_layer/le_advertisement_view.h"
#include "packets/link_layer/page_response_view.h"
#include "packets/link_layer/page_view.h"
#include "packets/link_layer/response_view.h"

using std::vector;
using namespace std::chrono;
using namespace test_vendor_lib::packets;

namespace test_vendor_lib {

// TODO: Model Rssi?
static uint8_t GetRssi() {
  static uint8_t rssi = 0;
  rssi += 5;
  if (rssi > 128) {
    rssi = rssi % 7;
  }
  return -(rssi);
}

void LinkLayerController::SendLELinkLayerPacket(std::shared_ptr<LinkLayerPacketBuilder> packet) {
  if (schedule_task_) {
    schedule_task_(milliseconds(50), [this, packet]() { send_to_remote_(packet, Phy::Type::LOW_ENERGY); });
  } else {
    send_to_remote_(packet, Phy::Type::LOW_ENERGY);
  }
}

void LinkLayerController::SendLinkLayerPacket(std::shared_ptr<LinkLayerPacketBuilder> packet) {
  if (schedule_task_) {
    schedule_task_(milliseconds(50), [this, packet]() { send_to_remote_(packet, Phy::Type::BR_EDR); });
  } else {
    send_to_remote_(packet, Phy::Type::BR_EDR);
  }
}

hci::Status LinkLayerController::SendCommandToRemoteByAddress(hci::OpCode opcode, PacketView<true> args,
                                                              const Address& remote, bool use_public_address) {
  std::shared_ptr<LinkLayerPacketBuilder> command;
  Address local_address;
  if (use_public_address) {
    local_address = properties_.GetAddress();
  } else {
    local_address = properties_.GetLeAddress();
  }
  command = LinkLayerPacketBuilder::WrapCommand(CommandBuilder::Create(static_cast<uint16_t>(opcode), args),
                                                local_address, remote);
  SendLinkLayerPacket(std::move(command));
  return hci::Status::SUCCESS;
}

hci::Status LinkLayerController::SendCommandToRemoteByHandle(hci::OpCode opcode, PacketView<true> args,
                                                             uint16_t handle) {
  // TODO: Handle LE connections
  bool use_public_address = true;
  if (!classic_connections_.HasHandle(handle)) {
    return hci::Status::UNKNOWN_CONNECTION;
  }
  return SendCommandToRemoteByAddress(opcode, args, classic_connections_.GetAddress(handle), use_public_address);
}

hci::Status LinkLayerController::SendAclToRemote(AclPacketView acl_packet) {
  // TODO: Handle LE connections
  uint16_t handle = acl_packet.GetHandle();
  if (!classic_connections_.HasHandle(handle)) {
    return hci::Status::UNKNOWN_CONNECTION;
  }

  std::unique_ptr<ViewForwarderBuilder> acl_builder = ViewForwarderBuilder::Create(acl_packet);

  std::shared_ptr<LinkLayerPacketBuilder> acl = LinkLayerPacketBuilder::WrapAcl(
      std::move(acl_builder), properties_.GetAddress(), classic_connections_.GetAddress(handle));

  LOG_INFO(LOG_TAG, "%s(%s): handle 0x%x size %d", __func__, properties_.GetAddress().ToString().c_str(), handle,
           static_cast<int>(acl_packet.size()));

  schedule_task_(milliseconds(5), [this, handle]() {
    send_event_(EventPacketBuilder::CreateNumberOfCompletedPacketsEvent(handle, 1)->ToVector());
  });
  SendLinkLayerPacket(acl);
  return hci::Status::SUCCESS;
}

void LinkLayerController::IncomingPacket(LinkLayerPacketView incoming) {
  // TODO: Resolvable private addresses?
  if (incoming.GetDestinationAddress() != properties_.GetAddress() &&
      incoming.GetDestinationAddress() != properties_.GetLeAddress() &&
      incoming.GetDestinationAddress() != Address::kEmpty) {
    // Drop packets not addressed to me
    return;
  }

  switch (incoming.GetType()) {
    case Link::PacketType::ACL:
      IncomingAclPacket(incoming);
      break;
    case Link::PacketType::COMMAND:
      IncomingCommandPacket(incoming);
      break;
    case Link::PacketType::DISCONNECT:
      IncomingDisconnectPacket(incoming);
      break;
    case Link::PacketType::ENCRYPT_CONNECTION:
      IncomingEncryptConnection(incoming);
      break;
    case Link::PacketType::ENCRYPT_CONNECTION_RESPONSE:
      IncomingEncryptConnectionResponse(incoming);
      break;
    case Link::PacketType::INQUIRY:
      if (inquiry_scans_enabled_) {
        IncomingInquiryPacket(incoming);
      }
      break;
    case Link::PacketType::INQUIRY_RESPONSE:
      IncomingInquiryResponsePacket(incoming);
      break;
    case Link::PacketType::IO_CAPABILITY_REQUEST:
      IncomingIoCapabilityRequestPacket(incoming);
      break;
    case Link::PacketType::IO_CAPABILITY_RESPONSE:
      IncomingIoCapabilityResponsePacket(incoming);
      break;
    case Link::PacketType::IO_CAPABILITY_NEGATIVE_RESPONSE:
      IncomingIoCapabilityNegativeResponsePacket(incoming);
      break;
    case Link::PacketType::LE_ADVERTISEMENT:
      if (le_scan_enable_ || le_connect_) {
        IncomingLeAdvertisementPacket(incoming);
      }
      break;
    case Link::PacketType::LE_SCAN:
      // TODO: Check Advertising flags and see if we are scannable.
      IncomingLeScanPacket(incoming);
      break;
    case Link::PacketType::LE_SCAN_RESPONSE:
      if (le_scan_enable_ && le_scan_type_ == 1) {
        IncomingLeScanResponsePacket(incoming);
      }
      break;
    case Link::PacketType::PAGE:
      if (page_scans_enabled_) {
        IncomingPagePacket(incoming);
      }
      break;
    case Link::PacketType::PAGE_RESPONSE:
      IncomingPageResponsePacket(incoming);
      break;
    case Link::PacketType::RESPONSE:
      IncomingResponsePacket(incoming);
      break;
    default:
      LOG_WARN(LOG_TAG, "Dropping unhandled packet of type %d", static_cast<int32_t>(incoming.GetType()));
  }
}

void LinkLayerController::IncomingAclPacket(LinkLayerPacketView incoming) {
  LOG_INFO(LOG_TAG, "Acl Packet %s -> %s", incoming.GetSourceAddress().ToString().c_str(),
           incoming.GetDestinationAddress().ToString().c_str());
  AclPacketView acl_view = AclPacketView::Create(incoming.GetPayload());
  LOG_INFO(LOG_TAG, "%s: remote handle 0x%x size %d", __func__, acl_view.GetHandle(),
           static_cast<int>(acl_view.size()));
  uint16_t local_handle = classic_connections_.GetHandle(incoming.GetSourceAddress());
  LOG_INFO(LOG_TAG, "%s: local handle 0x%x", __func__, local_handle);

  acl::PacketBoundaryFlagsType boundary_flags = acl_view.GetPacketBoundaryFlags();
  acl::BroadcastFlagsType broadcast_flags = acl_view.GetBroadcastFlags();
  std::unique_ptr<ViewForwarderBuilder> builder = ViewForwarderBuilder::Create(acl_view.GetPayload());
  std::unique_ptr<AclPacketBuilder> local_acl =
      AclPacketBuilder::Create(local_handle, boundary_flags, broadcast_flags, std::move(builder));
  send_acl_(local_acl->ToVector());
}

void LinkLayerController::IncomingCommandPacket(LinkLayerPacketView incoming) {
  // TODO: Check the destination address to see if this packet is for me.
  CommandView command = CommandView::GetCommand(incoming);
  hci::OpCode opcode = static_cast<hci::OpCode>(command.GetOpcode());
  auto args = command.GetData();
  std::vector<uint64_t> response_data;

  switch (opcode) {
    case (hci::OpCode::REMOTE_NAME_REQUEST): {
      std::vector<uint8_t> name = properties_.GetName();
      LOG_INFO(LOG_TAG, "Remote Name (Local Name) %d", static_cast<int>(name.size()));
      response_data.push_back(static_cast<uint8_t>(hci::Status::SUCCESS));
      response_data.push_back(name.size());
      uint64_t word = 0;
      for (size_t i = 0; i < name.size(); i++) {
        if (i > 0 && (i % 8 == 0)) {
          response_data.push_back(word);
          word = 0;
        }
        word |= static_cast<uint64_t>(name[i]) << (8 * (i % 8));
      }
      response_data.push_back(word);
    } break;
    case (hci::OpCode::READ_REMOTE_SUPPORTED_FEATURES):
      LOG_INFO(LOG_TAG, "(%s) Remote Supported Features Requested by: %s %x",
               incoming.GetDestinationAddress().ToString().c_str(), incoming.GetSourceAddress().ToString().c_str(),
               static_cast<int>(properties_.GetSupportedFeatures()));
      response_data.push_back(static_cast<uint8_t>(hci::Status::SUCCESS));
      response_data.push_back(properties_.GetSupportedFeatures());
      break;
    case (hci::OpCode::READ_REMOTE_EXTENDED_FEATURES): {
      uint8_t page_number = (args + 2).extract<uint8_t>();  // skip the handle
      LOG_INFO(LOG_TAG, "(%s) Remote Extended Features %d Requested by: %s",
               incoming.GetDestinationAddress().ToString().c_str(), page_number,
               incoming.GetSourceAddress().ToString().c_str());
      uint8_t max_page_number = properties_.GetExtendedFeaturesMaximumPageNumber();
      if (page_number > max_page_number) {
        response_data.push_back(static_cast<uint8_t>(hci::Status::INVALID_HCI_COMMAND_PARAMETERS));
        response_data.push_back(page_number);
        response_data.push_back(max_page_number);
        response_data.push_back(0);
      } else {
        response_data.push_back(static_cast<uint8_t>(hci::Status::SUCCESS));
        response_data.push_back(page_number);
        response_data.push_back(max_page_number);
        response_data.push_back(properties_.GetExtendedFeatures(page_number));
      }
    } break;
    case (hci::OpCode::READ_REMOTE_VERSION_INFORMATION):
      response_data.push_back(static_cast<uint8_t>(hci::Status::SUCCESS));
      response_data.push_back(properties_.GetLmpPalVersion());
      response_data.push_back(properties_.GetManufacturerName());
      response_data.push_back(properties_.GetLmpPalSubversion());
      break;
    case (hci::OpCode::READ_CLOCK_OFFSET):
      response_data.push_back(static_cast<uint8_t>(hci::Status::SUCCESS));
      response_data.push_back(properties_.GetClockOffset());
      break;
    default:
      LOG_INFO(LOG_TAG, "Dropping unhandled command 0x%04x", static_cast<uint16_t>(opcode));
      return;
  }
  SendLinkLayerPacket(
      LinkLayerPacketBuilder::WrapResponse(ResponseBuilder::Create(static_cast<uint16_t>(opcode), response_data),
                                           properties_.GetAddress(), incoming.GetSourceAddress()));
}

void LinkLayerController::IncomingDisconnectPacket(LinkLayerPacketView incoming) {
  LOG_INFO(LOG_TAG, "Disconnect Packet");
  DisconnectView disconnect = DisconnectView::GetDisconnect(incoming);
  Address peer = incoming.GetSourceAddress();
  uint16_t handle = classic_connections_.GetHandle(peer);
  if (handle == acl::kReservedHandle) {
    LOG_INFO(LOG_TAG, "%s: Unknown connection @%s", __func__, peer.ToString().c_str());
    return;
  }
  CHECK(classic_connections_.Disconnect(handle)) << "GetHandle() returned invalid handle " << handle;

  uint8_t reason = disconnect.GetReason();
  schedule_task_(milliseconds(20), [this, handle, reason]() { DisconnectCleanup(handle, reason); });
}

void LinkLayerController::IncomingEncryptConnection(LinkLayerPacketView incoming) {
  LOG_INFO(LOG_TAG, "%s", __func__);
  // TODO: Check keys
  Address peer = incoming.GetSourceAddress();
  uint16_t handle = classic_connections_.GetHandle(peer);
  if (handle == acl::kReservedHandle) {
    LOG_INFO(LOG_TAG, "%s: Unknown connection @%s", __func__, peer.ToString().c_str());
    return;
  }
  send_event_(EventPacketBuilder::CreateEncryptionChange(hci::Status::SUCCESS, handle, 1)->ToVector());
  SendLinkLayerPacket(LinkLayerPacketBuilder::WrapEncryptConnectionResponse(
      EncryptConnectionBuilder::Create(security_manager_.GetKey(peer)), properties_.GetAddress(), peer));
}

void LinkLayerController::IncomingEncryptConnectionResponse(LinkLayerPacketView incoming) {
  LOG_INFO(LOG_TAG, "%s", __func__);
  // TODO: Check keys
  uint16_t handle = classic_connections_.GetHandle(incoming.GetSourceAddress());
  if (handle == acl::kReservedHandle) {
    LOG_INFO(LOG_TAG, "%s: Unknown connection @%s", __func__, incoming.GetSourceAddress().ToString().c_str());
    return;
  }
  send_event_(EventPacketBuilder::CreateEncryptionChange(hci::Status::SUCCESS, handle, 1)->ToVector());
}

void LinkLayerController::IncomingInquiryPacket(LinkLayerPacketView incoming) {
  InquiryView inquiry = InquiryView::GetInquiry(incoming);
  std::unique_ptr<InquiryResponseBuilder> inquiry_response;
  switch (inquiry.GetType()) {
    case (Inquiry::InquiryType::STANDARD):
      inquiry_response = InquiryResponseBuilder::CreateStandard(
          properties_.GetPageScanRepetitionMode(), properties_.GetClassOfDevice(), properties_.GetClockOffset());
      break;

    case (Inquiry::InquiryType::RSSI):
      inquiry_response =
          InquiryResponseBuilder::CreateRssi(properties_.GetPageScanRepetitionMode(), properties_.GetClassOfDevice(),
                                             properties_.GetClockOffset(), GetRssi());
      break;

    case (Inquiry::InquiryType::EXTENDED):
      inquiry_response = InquiryResponseBuilder::CreateExtended(
          properties_.GetPageScanRepetitionMode(), properties_.GetClassOfDevice(), properties_.GetClockOffset(),
          GetRssi(), properties_.GetExtendedInquiryData());
      break;
    default:
      LOG_WARN(LOG_TAG, "Unhandled Incoming Inquiry of type %d", static_cast<int>(inquiry.GetType()));
      return;
  }
  SendLinkLayerPacket(LinkLayerPacketBuilder::WrapInquiryResponse(std::move(inquiry_response), properties_.GetAddress(),
                                                                  incoming.GetSourceAddress()));
  // TODO: Send an Inquriy Response Notification Event 7.7.74
}

void LinkLayerController::IncomingInquiryResponsePacket(LinkLayerPacketView incoming) {
  InquiryResponseView inquiry_response = InquiryResponseView::GetInquiryResponse(incoming);
  std::vector<uint8_t> eir;

  switch (inquiry_response.GetType()) {
    case (Inquiry::InquiryType::STANDARD): {
      LOG_WARN(LOG_TAG, "Incoming Standard Inquiry Response");
      // TODO: Support multiple inquiries in the same packet.
      std::unique_ptr<EventPacketBuilder> inquiry_result = EventPacketBuilder::CreateInquiryResultEvent();
      bool result_added =
          inquiry_result->AddInquiryResult(incoming.GetSourceAddress(), inquiry_response.GetPageScanRepetitionMode(),
                                           inquiry_response.GetClassOfDevice(), inquiry_response.GetClockOffset());
      CHECK(result_added);
      send_event_(inquiry_result->ToVector());
    } break;

    case (Inquiry::InquiryType::RSSI):
      LOG_WARN(LOG_TAG, "Incoming RSSI Inquiry Response");
      send_event_(EventPacketBuilder::CreateExtendedInquiryResultEvent(
                      incoming.GetSourceAddress(), inquiry_response.GetPageScanRepetitionMode(),
                      inquiry_response.GetClassOfDevice(), inquiry_response.GetClockOffset(), GetRssi(), eir)
                      ->ToVector());
      break;

    case (Inquiry::InquiryType::EXTENDED): {
      LOG_WARN(LOG_TAG, "Incoming Extended Inquiry Response");
      auto eir_itr = inquiry_response.GetExtendedData();
      size_t eir_bytes = eir_itr.NumBytesRemaining();
      LOG_WARN(LOG_TAG, "Payload size = %d", static_cast<int>(eir_bytes));
      for (size_t i = 0; i < eir_bytes; i++) {
        eir.push_back(eir_itr.extract<uint8_t>());
      }
      send_event_(EventPacketBuilder::CreateExtendedInquiryResultEvent(
                      incoming.GetSourceAddress(), inquiry_response.GetPageScanRepetitionMode(),
                      inquiry_response.GetClassOfDevice(), inquiry_response.GetClockOffset(), GetRssi(), eir)
                      ->ToVector());
    } break;
    default:
      LOG_WARN(LOG_TAG, "Unhandled Incoming Inquiry Response of type %d", static_cast<int>(inquiry_response.GetType()));
  }
}

void LinkLayerController::IncomingIoCapabilityRequestPacket(LinkLayerPacketView incoming) {
  LOG_DEBUG(LOG_TAG, "%s", __func__);
  if (!simple_pairing_mode_enabled_) {
    LOG_WARN(LOG_TAG, "%s: Only simple pairing mode is implemented", __func__);
    return;
  }
  auto request = IoCapabilityView::GetIoCapability(incoming);
  Address peer = incoming.GetSourceAddress();

  uint8_t io_capability = request.GetIoCapability();
  uint8_t oob_data_present = request.GetOobDataPresent();
  uint8_t authentication_requirements = request.GetAuthenticationRequirements();

  uint16_t handle = classic_connections_.GetHandle(peer);
  if (handle == acl::kReservedHandle) {
    LOG_INFO(LOG_TAG, "%s: Device not connected %s", __func__, peer.ToString().c_str());
    return;
  }

  security_manager_.AuthenticationRequest(peer, handle);

  security_manager_.SetPeerIoCapability(peer, io_capability, oob_data_present, authentication_requirements);

  send_event_(EventPacketBuilder::CreateIoCapabilityResponseEvent(peer, io_capability, oob_data_present,
                                                                  authentication_requirements)
                  ->ToVector());

  StartSimplePairing(peer);
}

void LinkLayerController::IncomingIoCapabilityResponsePacket(LinkLayerPacketView incoming) {
  LOG_DEBUG(LOG_TAG, "%s", __func__);
  auto response = IoCapabilityView::GetIoCapability(incoming);
  Address peer = incoming.GetSourceAddress();
  uint8_t io_capability = response.GetIoCapability();
  uint8_t oob_data_present = response.GetOobDataPresent();
  uint8_t authentication_requirements = response.GetAuthenticationRequirements();

  security_manager_.SetPeerIoCapability(peer, io_capability, oob_data_present, authentication_requirements);

  send_event_(EventPacketBuilder::CreateIoCapabilityResponseEvent(peer, io_capability, oob_data_present,
                                                                  authentication_requirements)
                  ->ToVector());

  PairingType pairing_type = security_manager_.GetSimplePairingType();
  if (pairing_type != PairingType::INVALID) {
    schedule_task_(milliseconds(5), [this, peer, pairing_type]() { AuthenticateRemoteStage1(peer, pairing_type); });
  } else {
    LOG_INFO(LOG_TAG, "%s: Security Manager returned INVALID", __func__);
  }
}

void LinkLayerController::IncomingIoCapabilityNegativeResponsePacket(LinkLayerPacketView incoming) {
  LOG_DEBUG(LOG_TAG, "%s", __func__);
  Address peer = incoming.GetSourceAddress();

  CHECK(security_manager_.GetAuthenticationAddress() == peer);

  security_manager_.InvalidateIoCapabilities();
}

void LinkLayerController::IncomingLeAdvertisementPacket(LinkLayerPacketView incoming) {
  // TODO: Handle multiple advertisements per packet.

  LeAdvertisementView advertisement = LeAdvertisementView::GetLeAdvertisementView(incoming);
  LeAdvertisement::AdvertisementType adv_type = advertisement.GetAdvertisementType();
  LeAdvertisement::AddressType addr_type = advertisement.GetAddressType();

  if (le_scan_enable_) {
    vector<uint8_t> ad;
    auto itr = advertisement.GetData();
    size_t ad_size = itr.NumBytesRemaining();
    for (size_t i = 0; i < ad_size; i++) {
      ad.push_back(itr.extract<uint8_t>());
    }
    std::unique_ptr<EventPacketBuilder> le_adverts = EventPacketBuilder::CreateLeAdvertisingReportEvent();

    if (!le_adverts->AddLeAdvertisingReport(adv_type, addr_type, incoming.GetSourceAddress(), ad, GetRssi())) {
      LOG_INFO(LOG_TAG, "Couldn't add the advertising report.");
    } else {
      send_event_(le_adverts->ToVector());
    }
  }

#if 0
      // Connect
      if (le_connect_ && (adv_type == BTM_BLE_CONNECT_EVT ||
                          adv_type == BTM_BLE_CONNECT_DIR_EVT)) {
        LOG_INFO(LOG_TAG, "Connecting to device %d", static_cast<int>(dev));
        if (le_peer_address_ == addr && le_peer_address_type_ == addr_type &&
            remote_devices_[dev]->LeConnect()) {
          uint16_t handle = LeGetHandle();
          send_event_(EventPacketBuilder::CreateLeConnectionCompleteEvent(
              hci::Status::SUCCESS, handle, HCI_ROLE_MASTER, addr_type, addr, 1,
              2, 3)->ToVector());

          // TODO: LeGetConnInterval(), LeGetConnLatency(),
          // LeGetSupervisionTimeout()));
          le_connect_ = false;

          std::shared_ptr<Connection> new_connection =
              std::make_shared<Connection>(remote_devices_[dev], handle);
          /*
          connections_.push_back(new_connection);

          remote_devices_[dev]->SetConnection(new_connection);
          */
        }

        if (LeWhiteListContainsDevice(addr, addr_type) &&
            remote_devices_[dev]->LeConnect()) {
          LOG_INFO(LOG_TAG, "White List Connecting to device %d",
                   static_cast<int>(dev));
          uint16_t handle = LeGetHandle();
          send_event_(EventPacketBuilder::CreateLeConnectionCompleteEvent(
              hci::Status::SUCCESS, handle, HCI_ROLE_MASTER, addr_type, addr, 1,
              2, 3)->ToVector());
          // TODO: LeGetConnInterval(), LeGetConnLatency(),
          // LeGetSupervisionTimeout()));
          le_connect_ = false;

          std::shared_ptr<Connection> new_connection =
              std::make_shared<Connection>(remote_devices_[dev], handle);
          /*
          connections_.push_back(new_connection);
          remote_devices_[dev]->SetConnection(new_connection);
          */
        }
      }
#endif

  // Active scanning
  if (le_scan_enable_ && le_scan_type_ == 1) {
    std::shared_ptr<LinkLayerPacketBuilder> to_send =
        LinkLayerPacketBuilder::WrapLeScan(properties_.GetLeAddress(), incoming.GetSourceAddress());
    SendLELinkLayerPacket(to_send);
  }
}

void LinkLayerController::IncomingLeScanPacket(LinkLayerPacketView incoming) {
  LOG_INFO(LOG_TAG, "LE Scan Packet");
  std::unique_ptr<LeAdvertisementBuilder> response = LeAdvertisementBuilder::Create(
      static_cast<LeAdvertisement::AddressType>(properties_.GetLeAddressType()),
      static_cast<LeAdvertisement::AdvertisementType>(properties_.GetLeAdvertisementType()),
      properties_.GetLeScanResponse());
  std::shared_ptr<LinkLayerPacketBuilder> to_send = LinkLayerPacketBuilder::WrapLeScanResponse(
      std::move(response), properties_.GetLeAddress(), incoming.GetSourceAddress());
  SendLELinkLayerPacket(to_send);
}

void LinkLayerController::IncomingLeScanResponsePacket(LinkLayerPacketView incoming) {
  LeAdvertisementView scan_response = LeAdvertisementView::GetLeAdvertisementView(incoming);
  vector<uint8_t> ad;
  auto itr = scan_response.GetData();
  size_t scan_size = itr.NumBytesRemaining();
  for (size_t i = 0; i < scan_size; i++) {
    ad.push_back(itr.extract<uint8_t>());
  }

  std::unique_ptr<EventPacketBuilder> le_adverts = EventPacketBuilder::CreateLeAdvertisingReportEvent();

  if (!le_adverts->AddLeAdvertisingReport(scan_response.GetAdvertisementType(), scan_response.GetAddressType(),
                                          incoming.GetSourceAddress(), ad, GetRssi())) {
    LOG_INFO(LOG_TAG, "Couldn't add the scan response.");
  } else {
    LOG_INFO(LOG_TAG, "Sending scan response");
    send_event_(le_adverts->ToVector());
  }
}

void LinkLayerController::IncomingPagePacket(LinkLayerPacketView incoming) {
  PageView page = PageView::GetPage(incoming);
  LOG_INFO(LOG_TAG, "%s from %s", __func__, incoming.GetSourceAddress().ToString().c_str());

  if (!classic_connections_.CreatePendingConnection(incoming.GetSourceAddress())) {
    // Send a response to indicate that we're busy, or drop the packet?
    LOG_WARN(LOG_TAG, "%s: Failed to create a pending connection", __func__);
  }

  send_event_(EventPacketBuilder::CreateConnectionRequestEvent(incoming.GetSourceAddress(), page.GetClassOfDevice(),
                                                               hci::LinkType::ACL)
                  ->ToVector());
}

void LinkLayerController::IncomingPageResponsePacket(LinkLayerPacketView incoming) {
  LOG_INFO(LOG_TAG, "%s: %s", __func__, incoming.GetSourceAddress().ToString().c_str());
  uint16_t handle = classic_connections_.CreateConnection(incoming.GetSourceAddress());
  if (handle == acl::kReservedHandle) {
    LOG_WARN(LOG_TAG, "%s: No free handles", __func__);
    return;
  }
  LOG_INFO(LOG_TAG, "%s: Sending CreateConnectionComplete", __func__);
  send_event_(EventPacketBuilder::CreateConnectionCompleteEvent(hci::Status::SUCCESS, handle,
                                                                incoming.GetSourceAddress(), hci::LinkType::ACL, false)
                  ->ToVector());
}

void LinkLayerController::IncomingResponsePacket(LinkLayerPacketView incoming) {
  ResponseView response = ResponseView::GetResponse(incoming);

  // TODO: Check to see if I'm expecting this response.

  hci::OpCode opcode = static_cast<hci::OpCode>(response.GetOpcode());
  auto args = response.GetResponseData();
  hci::Status status = static_cast<hci::Status>(args.extract<uint64_t>());

  uint16_t handle = classic_connections_.GetHandle(incoming.GetSourceAddress());

  switch (opcode) {
    case (hci::OpCode::REMOTE_NAME_REQUEST): {
      std::string remote_name = "";
      size_t length = args.extract<uint64_t>();
      uint64_t word = 0;
      for (size_t b = 0; b < length; b++) {
        size_t byte = b % 8;
        if (byte == 0) {
          word = args.extract<uint64_t>();
        }
        remote_name += static_cast<uint8_t>(word >> (byte * 8));
      }
      send_event_(
          EventPacketBuilder::CreateRemoteNameRequestCompleteEvent(status, incoming.GetSourceAddress(), remote_name)
              ->ToVector());
    } break;
    case (hci::OpCode::READ_REMOTE_SUPPORTED_FEATURES): {
      send_event_(
          EventPacketBuilder::CreateRemoteSupportedFeaturesEvent(status, handle, args.extract<uint64_t>())->ToVector());
    } break;
    case (hci::OpCode::READ_REMOTE_EXTENDED_FEATURES): {
      if (status == hci::Status::SUCCESS) {
        send_event_(EventPacketBuilder::CreateReadRemoteExtendedFeaturesEvent(
                        status, handle, args.extract<uint64_t>(), args.extract<uint64_t>(), args.extract<uint64_t>())
                        ->ToVector());
      } else {
        send_event_(EventPacketBuilder::CreateReadRemoteExtendedFeaturesEvent(status, handle, 0, 0, 0)->ToVector());
      }
    } break;
    case (hci::OpCode::READ_REMOTE_VERSION_INFORMATION): {
      send_event_(EventPacketBuilder::CreateReadRemoteVersionInformationEvent(
                      status, handle, args.extract<uint64_t>(), args.extract<uint64_t>(), args.extract<uint64_t>())
                      ->ToVector());
      LOG_INFO(LOG_TAG, "Read remote version handle 0x%04x", handle);
    } break;
    case (hci::OpCode::READ_CLOCK_OFFSET): {
      send_event_(EventPacketBuilder::CreateReadClockOffsetEvent(status, handle, args.extract<uint64_t>())->ToVector());
    } break;
    default:
      LOG_INFO(LOG_TAG, "Unhandled response to command 0x%04x", static_cast<uint16_t>(opcode));
  }
}

void LinkLayerController::TimerTick() {
  if (inquiry_state_ == Inquiry::InquiryState::INQUIRY) Inquiry();
  if (inquiry_state_ == Inquiry::InquiryState::INQUIRY) PageScan();
  Connections();
}

void LinkLayerController::Connections() {
  // TODO: Keep connections alive?
}

void LinkLayerController::RegisterEventChannel(
    const std::function<void(std::shared_ptr<std::vector<uint8_t>>)>& callback) {
  send_event_ = callback;
}

void LinkLayerController::RegisterAclChannel(
    const std::function<void(std::shared_ptr<std::vector<uint8_t>>)>& callback) {
  send_acl_ = callback;
}

void LinkLayerController::RegisterScoChannel(
    const std::function<void(std::shared_ptr<std::vector<uint8_t>>)>& callback) {
  send_sco_ = callback;
}

void LinkLayerController::RegisterRemoteChannel(
    const std::function<void(std::shared_ptr<LinkLayerPacketBuilder>, Phy::Type)>& callback) {
  send_to_remote_ = callback;
}

void LinkLayerController::RegisterTaskScheduler(
    std::function<AsyncTaskId(milliseconds, const TaskCallback&)> event_scheduler) {
  schedule_task_ = event_scheduler;
}

void LinkLayerController::RegisterPeriodicTaskScheduler(
    std::function<AsyncTaskId(milliseconds, milliseconds, const TaskCallback&)> periodic_event_scheduler) {
  schedule_periodic_task_ = periodic_event_scheduler;
}

void LinkLayerController::RegisterTaskCancel(std::function<void(AsyncTaskId)> task_cancel) {
  cancel_task_ = task_cancel;
}

void LinkLayerController::AddControllerEvent(milliseconds delay, const TaskCallback& task) {
  controller_events_.push_back(schedule_task_(delay, task));
}

void LinkLayerController::WriteSimplePairingMode(bool enabled) {
  CHECK(enabled) << "The spec says don't disable this!";
  simple_pairing_mode_enabled_ = enabled;
}

void LinkLayerController::StartSimplePairing(const Address& address) {
  // IO Capability Exchange (See the Diagram in the Spec)
  send_event_(EventPacketBuilder::CreateIoCapabilityRequestEvent(address)->ToVector());

  // Get a Key, then authenticate
  // PublicKeyExchange(address);
  // AuthenticateRemoteStage1(address);
  // AuthenticateRemoteStage2(address);
}

void LinkLayerController::AuthenticateRemoteStage1(const Address& peer, PairingType pairing_type) {
  CHECK(security_manager_.GetAuthenticationAddress() == peer);
  // TODO: Public key exchange first?
  switch (pairing_type) {
    case PairingType::AUTO_CONFIRMATION:
      send_event_(EventPacketBuilder::CreateUserConfirmationRequestEvent(peer, 123456)->ToVector());
      break;
    case PairingType::CONFIRM_Y_N:
      CHECK(false) << __func__ << "Unimplemented PairingType" << static_cast<int>(pairing_type);
      break;
    case PairingType::DISPLAY_PIN:
      CHECK(false) << __func__ << "Unimplemented PairingType" << static_cast<int>(pairing_type);
      break;
    case PairingType::DISPLAY_AND_CONFIRM:
      CHECK(false) << __func__ << "Unimplemented PairingType" << static_cast<int>(pairing_type);
      break;
    case PairingType::INPUT_PIN:
      CHECK(false) << __func__ << "Unimplemented PairingType" << static_cast<int>(pairing_type);
      break;
    case PairingType::INVALID:
      CHECK(false) << __func__ << "Unimplemented PairingType" << static_cast<int>(pairing_type);
      break;
    default:
      CHECK(false) << __func__ << ": Invalid PairingType " << static_cast<int>(pairing_type);
  }
}

void LinkLayerController::AuthenticateRemoteStage2(const Address& peer) {
  uint16_t handle = security_manager_.GetAuthenticationHandle();
  CHECK(security_manager_.GetAuthenticationAddress() == peer);
  // Check key in security_manager_ ?
  send_event_(EventPacketBuilder::CreateAuthenticationCompleteEvent(hci::Status::SUCCESS, handle)->ToVector());
}

hci::Status LinkLayerController::LinkKeyRequestReply(const Address& peer, PacketView<true> key) {
  std::vector<uint8_t> key_vec(key.begin(), key.end());
  security_manager_.WriteKey(peer, key_vec);
  security_manager_.AuthenticationRequestFinished();

  schedule_task_(milliseconds(5), [this, peer]() { AuthenticateRemoteStage2(peer); });

  return hci::Status::SUCCESS;
}

hci::Status LinkLayerController::LinkKeyRequestNegativeReply(const Address& address) {
  security_manager_.DeleteKey(address);
  // Simple pairing to get a key
  uint16_t handle = classic_connections_.GetHandle(address);
  if (handle == acl::kReservedHandle) {
    LOG_INFO(LOG_TAG, "%s: Device not connected %s", __func__, address.ToString().c_str());
    return hci::Status::UNKNOWN_CONNECTION;
  }

  security_manager_.AuthenticationRequest(address, handle);

  schedule_task_(milliseconds(5), [this, address]() { StartSimplePairing(address); });
  return hci::Status::SUCCESS;
}

hci::Status LinkLayerController::IoCapabilityRequestReply(const Address& peer, uint8_t io_capability,
                                                          uint8_t oob_data_present_flag,
                                                          uint8_t authentication_requirements) {
  security_manager_.SetLocalIoCapability(peer, io_capability, oob_data_present_flag, authentication_requirements);

  PairingType pairing_type = security_manager_.GetSimplePairingType();
  if (pairing_type != PairingType::INVALID) {
    schedule_task_(milliseconds(5), [this, peer, pairing_type]() { AuthenticateRemoteStage1(peer, pairing_type); });
    SendLinkLayerPacket(LinkLayerPacketBuilder::WrapIoCapabilityResponse(
        IoCapabilityBuilder::Create(io_capability, oob_data_present_flag, authentication_requirements),
        properties_.GetAddress(), peer));
  } else {
    LOG_INFO(LOG_TAG, "%s: Requesting remote capability", __func__);
    SendLinkLayerPacket(LinkLayerPacketBuilder::WrapIoCapabilityRequest(
        IoCapabilityBuilder::Create(io_capability, oob_data_present_flag, authentication_requirements),
        properties_.GetAddress(), peer));
  }

  return hci::Status::SUCCESS;
}

hci::Status LinkLayerController::IoCapabilityRequestNegativeReply(const Address& peer, hci::Status reason) {
  if (security_manager_.GetAuthenticationAddress() != peer) {
    return hci::Status::AUTHENTICATION_FAILURE;
  }

  security_manager_.InvalidateIoCapabilities();

  SendLinkLayerPacket(LinkLayerPacketBuilder::WrapIoCapabilityNegativeResponse(
      IoCapabilityNegativeResponseBuilder::Create(static_cast<uint8_t>(reason)), properties_.GetAddress(), peer));

  return hci::Status::SUCCESS;
}

hci::Status LinkLayerController::UserConfirmationRequestReply(const Address& peer) {
  if (security_manager_.GetAuthenticationAddress() != peer) {
    return hci::Status::AUTHENTICATION_FAILURE;
  }
  // TODO: Key could be calculated here.
  std::vector<uint8_t> key_vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  security_manager_.WriteKey(peer, key_vec);

  security_manager_.AuthenticationRequestFinished();

  schedule_task_(milliseconds(5), [this, peer]() { AuthenticateRemoteStage2(peer); });
  return hci::Status::SUCCESS;
}

hci::Status LinkLayerController::UserConfirmationRequestNegativeReply(const Address& peer) {
  if (security_manager_.GetAuthenticationAddress() != peer) {
    return hci::Status::AUTHENTICATION_FAILURE;
  }
  return hci::Status::SUCCESS;
}

hci::Status LinkLayerController::UserPasskeyRequestReply(const Address& peer, uint32_t numeric_value) {
  if (security_manager_.GetAuthenticationAddress() != peer) {
    return hci::Status::AUTHENTICATION_FAILURE;
  }
  LOG_INFO(LOG_TAG, "TODO:Do something with the passkey %06d", numeric_value);
  return hci::Status::SUCCESS;
}

hci::Status LinkLayerController::UserPasskeyRequestNegativeReply(const Address& peer) {
  if (security_manager_.GetAuthenticationAddress() != peer) {
    return hci::Status::AUTHENTICATION_FAILURE;
  }
  return hci::Status::SUCCESS;
}

hci::Status LinkLayerController::RemoteOobDataRequestReply(const Address& peer, const std::vector<uint8_t>& c,
                                                           const std::vector<uint8_t>& r) {
  if (security_manager_.GetAuthenticationAddress() != peer) {
    return hci::Status::AUTHENTICATION_FAILURE;
  }
  LOG_INFO(LOG_TAG, "TODO:Do something with the OOB data c=%d r=%d", c[0], r[0]);
  return hci::Status::SUCCESS;
}

hci::Status LinkLayerController::RemoteOobDataRequestNegativeReply(const Address& peer) {
  if (security_manager_.GetAuthenticationAddress() != peer) {
    return hci::Status::AUTHENTICATION_FAILURE;
  }
  return hci::Status::SUCCESS;
}

void LinkLayerController::HandleAuthenticationRequest(const Address& address, uint16_t handle) {
  if (simple_pairing_mode_enabled_ == true) {
    security_manager_.AuthenticationRequest(address, handle);
    send_event_(EventPacketBuilder::CreateLinkKeyRequestEvent(address)->ToVector());
  } else {  // Should never happen for our phones
    // Check for a key, try to authenticate, ask for a PIN.
    send_event_(
        EventPacketBuilder::CreateAuthenticationCompleteEvent(hci::Status::AUTHENTICATION_FAILURE, handle)->ToVector());
  }
}

hci::Status LinkLayerController::AuthenticationRequested(uint16_t handle) {
  if (!classic_connections_.HasHandle(handle)) {
    LOG_INFO(LOG_TAG, "Authentication Requested for unknown handle %04x", handle);
    return hci::Status::UNKNOWN_CONNECTION;
  }

  Address remote = classic_connections_.GetAddress(handle);

  schedule_task_(milliseconds(5), [this, remote, handle]() { HandleAuthenticationRequest(remote, handle); });

  return hci::Status::SUCCESS;
}

void LinkLayerController::HandleSetConnectionEncryption(const Address& peer, uint16_t handle,
                                                        uint8_t encryption_enable) {
  // TODO: Block ACL traffic or at least guard against it

  if (classic_connections_.IsEncrypted(handle) && encryption_enable) {
    send_event_(
        EventPacketBuilder::CreateEncryptionChange(hci::Status::SUCCESS, handle, encryption_enable)->ToVector());
    return;
  }

  SendLinkLayerPacket(LinkLayerPacketBuilder::WrapEncryptConnection(
      EncryptConnectionBuilder::Create(security_manager_.GetKey(peer)), properties_.GetAddress(), peer));
}

hci::Status LinkLayerController::SetConnectionEncryption(uint16_t handle, uint8_t encryption_enable) {
  if (!classic_connections_.HasHandle(handle)) {
    LOG_INFO(LOG_TAG, "Authentication Requested for unknown handle %04x", handle);
    return hci::Status::UNKNOWN_CONNECTION;
  }

  if (classic_connections_.IsEncrypted(handle) && !encryption_enable) {
    return hci::Status::ENCRYPTION_MODE_NOT_ACCEPTABLE;
  }
  Address remote = classic_connections_.GetAddress(handle);

  schedule_task_(milliseconds(5), [this, remote, handle, encryption_enable]() {
    HandleSetConnectionEncryption(remote, handle, encryption_enable);
  });

  return hci::Status::SUCCESS;
}

hci::Status LinkLayerController::AcceptConnectionRequest(const Address& addr, bool try_role_switch) {
  if (!classic_connections_.HasPendingConnection(addr)) {
    LOG_INFO(LOG_TAG, "%s: No pending connection", __func__);
    return hci::Status::UNKNOWN_CONNECTION;
  }

  LOG_INFO(LOG_TAG, "%s: Accept in 200ms", __func__);
  schedule_task_(milliseconds(200), [this, addr, try_role_switch]() {
    LOG_INFO(LOG_TAG, "%s: Accepted", __func__);
    MakeSlaveConnection(addr, try_role_switch);
  });

  return hci::Status::SUCCESS;
}

void LinkLayerController::MakeSlaveConnection(const Address& addr, bool try_role_switch) {
  std::shared_ptr<LinkLayerPacketBuilder> to_send = LinkLayerPacketBuilder::WrapPageResponse(
      PageResponseBuilder::Create(try_role_switch), properties_.GetAddress(), addr);
  LOG_INFO(LOG_TAG, "%s sending page response to %s", __func__, addr.ToString().c_str());
  SendLinkLayerPacket(to_send);

  uint16_t handle = classic_connections_.CreateConnection(addr);
  if (handle == acl::kReservedHandle) {
    LOG_INFO(LOG_TAG, "%s CreateConnection failed", __func__);
    return;
  }
  LOG_INFO(LOG_TAG, "%s CreateConnection returned handle 0x%x", __func__, handle);
  send_event_(
      EventPacketBuilder::CreateConnectionCompleteEvent(hci::Status::SUCCESS, handle, addr, hci::LinkType::ACL, false)
          ->ToVector());
}

hci::Status LinkLayerController::RejectConnectionRequest(const Address& addr, uint8_t reason) {
  if (!classic_connections_.HasPendingConnection(addr)) {
    LOG_INFO(LOG_TAG, "%s: No pending connection", __func__);
    return hci::Status::UNKNOWN_CONNECTION;
  }

  LOG_INFO(LOG_TAG, "%s: Reject in 200ms", __func__);
  schedule_task_(milliseconds(200), [this, addr, reason]() {
    LOG_INFO(LOG_TAG, "%s: Reject", __func__);
    RejectSlaveConnection(addr, reason);
  });

  return hci::Status::SUCCESS;
}

void LinkLayerController::RejectSlaveConnection(const Address& addr, uint8_t reason) {
  CHECK(reason > 0x0f || reason < 0x0d);
  send_event_(EventPacketBuilder::CreateConnectionCompleteEvent(static_cast<hci::Status>(reason), 0xeff, addr,
                                                                hci::LinkType::ACL, false)
                  ->ToVector());
}

hci::Status LinkLayerController::CreateConnection(const Address& addr, uint16_t, uint8_t, uint16_t,
                                                  uint8_t allow_role_switch) {
  if (!classic_connections_.CreatePendingConnection(addr)) {
    return hci::Status::CONTROLLER_BUSY;
  }

  std::unique_ptr<PageBuilder> page = PageBuilder::Create(properties_.GetClassOfDevice(), allow_role_switch);
  SendLinkLayerPacket(LinkLayerPacketBuilder::WrapPage(std::move(page), properties_.GetAddress(), addr));

  return hci::Status::SUCCESS;
}

hci::Status LinkLayerController::CreateConnectionCancel(const Address& addr) {
  if (!classic_connections_.CancelPendingConnection(addr)) {
    return hci::Status::UNKNOWN_CONNECTION;
  }
  return hci::Status::SUCCESS;
}

hci::Status LinkLayerController::Disconnect(uint16_t handle, uint8_t reason) {
  // TODO: Handle LE
  if (!classic_connections_.HasHandle(handle)) {
    return hci::Status::UNKNOWN_CONNECTION;
  }

  const Address& remote = classic_connections_.GetAddress(handle);
  std::shared_ptr<LinkLayerPacketBuilder> to_send =
      LinkLayerPacketBuilder::WrapDisconnect(DisconnectBuilder::Create(reason), properties_.GetAddress(), remote);
  SendLinkLayerPacket(to_send);
  CHECK(classic_connections_.Disconnect(handle)) << "Disconnecting " << handle;

  schedule_task_(milliseconds(20), [this, handle]() {
    DisconnectCleanup(handle, static_cast<uint8_t>(hci::Status::CONNECTION_TERMINATED_BY_LOCAL_HOST));
  });

  return hci::Status::SUCCESS;
}

void LinkLayerController::DisconnectCleanup(uint16_t handle, uint8_t reason) {
  // TODO: Clean up other connection state.
  send_event_(EventPacketBuilder::CreateDisconnectionCompleteEvent(hci::Status::SUCCESS, handle, reason)->ToVector());
}

hci::Status LinkLayerController::ChangeConnectionPacketType(uint16_t handle, uint16_t types) {
  if (!classic_connections_.HasHandle(handle)) {
    return hci::Status::UNKNOWN_CONNECTION;
  }
  std::unique_ptr<EventPacketBuilder> packet =
      EventPacketBuilder::CreateConnectionPacketTypeChangedEvent(hci::Status::SUCCESS, handle, types);
  std::shared_ptr<std::vector<uint8_t>> raw_packet = packet->ToVector();
  if (schedule_task_) {
    schedule_task_(milliseconds(20), [this, raw_packet]() { send_event_(raw_packet); });
  } else {
    send_event_(raw_packet);
  }

  return hci::Status::SUCCESS;
}

hci::Status LinkLayerController::WriteLinkPolicySettings(uint16_t handle, uint16_t) {
  if (!classic_connections_.HasHandle(handle)) {
    return hci::Status::UNKNOWN_CONNECTION;
  }
  return hci::Status::SUCCESS;
}

hci::Status LinkLayerController::WriteLinkSupervisionTimeout(uint16_t handle, uint16_t) {
  if (!classic_connections_.HasHandle(handle)) {
    return hci::Status::UNKNOWN_CONNECTION;
  }
  return hci::Status::SUCCESS;
}

void LinkLayerController::LeWhiteListClear() {
  le_white_list_.clear();
}

void LinkLayerController::LeWhiteListAddDevice(Address addr, uint8_t addr_type) {
  std::tuple<Address, uint8_t> new_tuple = std::make_tuple(addr, addr_type);
  for (auto dev : le_white_list_) {
    if (dev == new_tuple) {
      return;
    }
  }
  le_white_list_.emplace_back(new_tuple);
}

void LinkLayerController::LeWhiteListRemoveDevice(Address addr, uint8_t addr_type) {
  // TODO: Add checks to see if advertising, scanning, or a connection request
  // with the white list is ongoing.
  std::tuple<Address, uint8_t> erase_tuple = std::make_tuple(addr, addr_type);
  for (size_t i = 0; i < le_white_list_.size(); i++) {
    if (le_white_list_[i] == erase_tuple) {
      le_white_list_.erase(le_white_list_.begin() + i);
    }
  }
}

bool LinkLayerController::LeWhiteListContainsDevice(Address addr, uint8_t addr_type) {
  std::tuple<Address, uint8_t> sought_tuple = std::make_tuple(addr, addr_type);
  for (size_t i = 0; i < le_white_list_.size(); i++) {
    if (le_white_list_[i] == sought_tuple) {
      return true;
    }
  }
  return false;
}

bool LinkLayerController::LeWhiteListFull() {
  return le_white_list_.size() >= properties_.GetLeWhiteListSize();
}

void LinkLayerController::Reset() {
  inquiry_state_ = Inquiry::InquiryState::STANDBY;
  last_inquiry_ = steady_clock::now();
  le_scan_enable_ = 0;
  le_connect_ = 0;
}

void LinkLayerController::PageScan() {}

void LinkLayerController::StartInquiry(milliseconds timeout) {
  schedule_task_(milliseconds(timeout), [this]() { LinkLayerController::InquiryTimeout(); });
  inquiry_state_ = Inquiry::InquiryState::INQUIRY;
  LOG_INFO(LOG_TAG, "InquiryState = %d ", static_cast<int>(inquiry_state_));
}

void LinkLayerController::InquiryCancel() {
  CHECK(inquiry_state_ == Inquiry::InquiryState::INQUIRY);
  inquiry_state_ = Inquiry::InquiryState::STANDBY;
}

void LinkLayerController::InquiryTimeout() {
  if (inquiry_state_ == Inquiry::InquiryState::INQUIRY) {
    inquiry_state_ = Inquiry::InquiryState::STANDBY;
    send_event_(EventPacketBuilder::CreateInquiryCompleteEvent(hci::Status::SUCCESS)->ToVector());
  }
}

void LinkLayerController::SetInquiryMode(uint8_t mode) {
  inquiry_mode_ = static_cast<Inquiry::InquiryType>(mode);
}

void LinkLayerController::SetInquiryLAP(uint64_t lap) {
  inquiry_lap_ = lap;
}

void LinkLayerController::SetInquiryMaxResponses(uint8_t max) {
  inquiry_max_responses_ = max;
}

void LinkLayerController::Inquiry() {
  steady_clock::time_point now = steady_clock::now();
  if (duration_cast<milliseconds>(now - last_inquiry_) < milliseconds(2000)) {
    return;
  }
  LOG_INFO(LOG_TAG, "Inquiry ");
  std::unique_ptr<InquiryBuilder> inquiry = InquiryBuilder::Create(inquiry_mode_);
  std::shared_ptr<LinkLayerPacketBuilder> to_send =
      LinkLayerPacketBuilder::WrapInquiry(std::move(inquiry), properties_.GetAddress());
  SendLinkLayerPacket(to_send);
  last_inquiry_ = now;
}

void LinkLayerController::SetInquiryScanEnable(bool enable) {
  inquiry_scans_enabled_ = enable;
}

void LinkLayerController::SetPageScanEnable(bool enable) {
  page_scans_enabled_ = enable;
}

/* TODO: Connection handling
  // TODO: Handle in the link manager.
  uint16_t handle = LeGetHandle();

  std::shared_ptr<Connection> new_connection =
      std::make_shared<Connection>(peer, handle);
  connections_.push_back(new_connection);
  peer->SetConnection(new_connection);

  send_event_(EventPacketBuilder::CreateLeEnhancedConnectionCompleteEvent(
      hci::Status::SUCCESS, handle, 0x00,  // role
      le_peer_address_type_, le_peer_address_, Address::kEmpty,
  Address::kEmpty, 0x0024, 0x0000, 0x01f4)->ToVector());
*/
}  // namespace test_vendor_lib
