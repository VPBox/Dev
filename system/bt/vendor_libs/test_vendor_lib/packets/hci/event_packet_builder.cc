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

#include "packets/hci/event_packet_builder.h"

#include <base/logging.h>
#include "hci.h"
#include "packets/hci/le_meta_event_builder.h"

using std::vector;
using test_vendor_lib::hci::EventCode;
using test_vendor_lib::hci::OpCode;
using test_vendor_lib::hci::Status;

namespace test_vendor_lib {
namespace packets {

EventPacketBuilder::EventPacketBuilder(EventCode event_code)
    : event_code_(event_code), payload_(std::make_unique<RawBuilder>()) {}

EventPacketBuilder::EventPacketBuilder(EventCode event_code, std::unique_ptr<RawBuilder> payload)
    : event_code_(event_code), payload_(std::move(payload)) {}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.1
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateInquiryCompleteEvent(hci::Status status) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::INQUIRY_COMPLETE));
  CHECK(evt_ptr->AddPayloadOctets1(static_cast<uint8_t>(status)));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.14
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteEvent(
    hci::OpCode command_opcode, const vector<uint8_t>& event_return_parameters) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::COMMAND_COMPLETE));

  CHECK(evt_ptr->AddPayloadOctets1(1));  // num_hci_command_packets
  CHECK(evt_ptr->AddPayloadOctets2(static_cast<uint16_t>(command_opcode)));
  CHECK(evt_ptr->AddPayloadOctets(event_return_parameters));

  return evt_ptr;
}

std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(hci::OpCode command_opcode,
                                                                                             hci::Status status) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::COMMAND_COMPLETE));

  CHECK(evt_ptr->AddPayloadOctets1(1));  // num_hci_command_packets
  CHECK(evt_ptr->AddPayloadOctets2(static_cast<uint16_t>(command_opcode)));
  CHECK(evt_ptr->AddPayloadOctets1(static_cast<uint8_t>(status)));

  return evt_ptr;
}

std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteStatusAndAddressEvent(
    hci::OpCode command_opcode, hci::Status status, const Address& address) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::COMMAND_COMPLETE));

  CHECK(evt_ptr->AddPayloadOctets1(1));  // num_hci_command_packets
  CHECK(evt_ptr->AddPayloadOctets2(static_cast<uint16_t>(command_opcode)));
  CHECK(evt_ptr->AddPayloadOctets1(static_cast<uint8_t>(status)));
  CHECK(evt_ptr->AddPayloadAddress(address));

  return evt_ptr;
}

std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteUnknownOpCodeEvent(
    uint16_t command_opcode) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::COMMAND_COMPLETE));

  CHECK(evt_ptr->AddPayloadOctets1(1));  // num_hci_command_packets
  CHECK(evt_ptr->AddPayloadOctets2(static_cast<uint16_t>(command_opcode)));
  CHECK(evt_ptr->AddPayloadOctets1(static_cast<uint8_t>(Status::UNKNOWN_COMMAND)));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.15
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandStatusEvent(hci::Status status,
                                                                                 hci::OpCode command_opcode) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::COMMAND_STATUS));

  CHECK(evt_ptr->AddPayloadOctets1(static_cast<uint8_t>(status)));
  CHECK(evt_ptr->AddPayloadOctets1(1));  // num_hci_command_packets
  CHECK(evt_ptr->AddPayloadOctets2(static_cast<uint16_t>(command_opcode)));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.19
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateNumberOfCompletedPacketsEvent(
    uint16_t handle, uint16_t num_completed_packets) {
  std::unique_ptr<RawBuilder> payload = std::make_unique<CountedBuilder>();
  std::unique_ptr<EventPacketBuilder> evt_ptr = std::unique_ptr<EventPacketBuilder>(
      new EventPacketBuilder(EventCode::NUMBER_OF_COMPLETED_PACKETS, std::move(payload)));

  evt_ptr->AddCompletedPackets(handle, num_completed_packets);

  return evt_ptr;
}

void EventPacketBuilder::AddCompletedPackets(uint16_t handle, uint16_t num_completed_packets) {
  CHECK(event_code_ == EventCode::NUMBER_OF_COMPLETED_PACKETS);

  std::unique_ptr<RawBuilder> handle_pair = std::make_unique<RawBuilder>();
  CHECK(handle_pair->AddOctets2(handle));
  CHECK(handle_pair->AddOctets2(num_completed_packets));
  AddBuilder(std::move(handle_pair));
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.3.10
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteDeleteStoredLinkKey(
    hci::Status status, uint16_t num_keys_deleted) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::DELETE_STORED_LINK_KEY, status);

  CHECK(evt_ptr->AddPayloadOctets2(num_keys_deleted));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.3.12
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteReadLocalName(
    hci::Status status, const std::vector<uint8_t>& local_name) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::READ_LOCAL_NAME, status);

  size_t len = local_name.size();
  if (len > 247) {
    len = 247;
  }
  CHECK(evt_ptr->AddPayloadOctets(len, local_name));
  CHECK(evt_ptr->AddPayloadOctets1(0));  // Null terminated
  for (size_t i = 0; i < 248 - len - 1; i++) CHECK(evt_ptr->AddPayloadOctets1(0xFF));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.3.23
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteReadAuthenticationEnable(
    hci::Status status, uint8_t enable) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::READ_LOCAL_NAME, status);
  CHECK(evt_ptr->AddPayloadOctets1(enable));
  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.4.1
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteReadLocalVersionInformation(
    hci::Status status, uint8_t hci_version, uint16_t hci_revision, uint8_t lmp_pal_version, uint16_t manufacturer_name,
    uint16_t lmp_pal_subversion) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::READ_LOCAL_VERSION_INFORMATION, status);

  CHECK(evt_ptr->AddPayloadOctets1(hci_version));
  CHECK(evt_ptr->AddPayloadOctets2(hci_revision));
  CHECK(evt_ptr->AddPayloadOctets1(lmp_pal_version));
  CHECK(evt_ptr->AddPayloadOctets2(manufacturer_name));
  CHECK(evt_ptr->AddPayloadOctets2(lmp_pal_subversion));

  return evt_ptr;
}

std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateReadRemoteVersionInformationEvent(
    hci::Status status, uint16_t connection_handle, uint8_t lmp_pal_version, uint16_t manufacturer_name,
    uint16_t lmp_pal_subversion) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::READ_REMOTE_VERSION_INFORMATION_COMPLETE));

  CHECK(evt_ptr->AddPayloadOctets1(static_cast<uint8_t>(status)));
  CHECK(evt_ptr->AddPayloadOctets2(connection_handle));
  CHECK(evt_ptr->AddPayloadOctets1(lmp_pal_version));
  CHECK(evt_ptr->AddPayloadOctets2(manufacturer_name));
  CHECK(evt_ptr->AddPayloadOctets2(lmp_pal_subversion));

  return evt_ptr;
}
// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.4.2
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteReadLocalSupportedCommands(
    hci::Status status, const vector<uint8_t>& supported_commands) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::READ_LOCAL_SUPPORTED_COMMANDS, status);

  CHECK(evt_ptr->AddPayloadOctets(64, supported_commands));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.4.4
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteReadLocalExtendedFeatures(
    hci::Status status, uint8_t page_number, uint8_t maximum_page_number, uint64_t extended_lmp_features) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::READ_LOCAL_EXTENDED_FEATURES, status);

  CHECK(evt_ptr->AddPayloadOctets1(page_number));
  CHECK(evt_ptr->AddPayloadOctets1(maximum_page_number));
  CHECK(evt_ptr->AddPayloadOctets8(extended_lmp_features));

  return evt_ptr;
}

std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateReadRemoteExtendedFeaturesEvent(
    hci::Status status, uint16_t handle, uint8_t page_number, uint8_t maximum_page_number,
    uint64_t extended_lmp_features) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::READ_REMOTE_EXTENDED_FEATURES_COMPLETE));

  CHECK(evt_ptr->AddPayloadOctets1(static_cast<uint8_t>(status)));
  CHECK(evt_ptr->AddPayloadOctets2(handle));
  CHECK(evt_ptr->AddPayloadOctets1(page_number));
  CHECK(evt_ptr->AddPayloadOctets1(maximum_page_number));
  CHECK(evt_ptr->AddPayloadOctets8(extended_lmp_features));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.4.5
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteReadBufferSize(
    hci::Status status, uint16_t hc_acl_data_packet_length, uint8_t hc_synchronous_data_packet_length,
    uint16_t hc_total_num_acl_data_packets, uint16_t hc_total_synchronous_data_packets) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::READ_BUFFER_SIZE, status);

  CHECK(evt_ptr->AddPayloadOctets2(hc_acl_data_packet_length));
  CHECK(evt_ptr->AddPayloadOctets1(hc_synchronous_data_packet_length));
  CHECK(evt_ptr->AddPayloadOctets2(hc_total_num_acl_data_packets));
  CHECK(evt_ptr->AddPayloadOctets2(hc_total_synchronous_data_packets));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.4.6
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteReadBdAddr(hci::Status status,
                                                                                        const Address& address) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::READ_BD_ADDR, status);

  CHECK(evt_ptr->AddPayloadAddress(address));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.4.8
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteReadLocalSupportedCodecs(
    hci::Status status, const vector<uint8_t>& supported_codecs, const vector<uint32_t>& vendor_specific_codecs) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::READ_LOCAL_SUPPORTED_CODECS, status);

  CHECK(evt_ptr->AddPayloadOctets1(supported_codecs.size()));
  CHECK(evt_ptr->AddPayloadOctets(supported_codecs));
  CHECK(evt_ptr->AddPayloadOctets1(vendor_specific_codecs.size()));
  for (size_t i = 0; i < vendor_specific_codecs.size(); i++)
    CHECK(evt_ptr->AddPayloadOctets4(vendor_specific_codecs[i]));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.6.1
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteReadLoopbackMode(hci::Status status,
                                                                                              hci::LoopbackMode mode) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::READ_LOOPBACK_MODE, status);
  CHECK(evt_ptr->AddPayloadOctets1(static_cast<uint8_t>(mode)));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.2
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateInquiryResultEvent() {
  std::unique_ptr<RawBuilder> payload = std::unique_ptr<RawBuilder>(new CountedBuilder());
  std::unique_ptr<EventPacketBuilder> evt_ptr(new EventPacketBuilder(EventCode::INQUIRY_RESULT, std::move(payload)));

  return evt_ptr;
}

bool EventPacketBuilder::AddInquiryResult(const Address& address, uint8_t page_scan_repetition_mode,
                                          ClassOfDevice class_of_device, uint16_t clock_offset) {
  CHECK(event_code_ == EventCode::INQUIRY_RESULT);

  if (!CanAddPayloadOctets(14)) return false;

  std::unique_ptr<RawBuilder> result = std::make_unique<RawBuilder>();

  CHECK(result->AddAddress(address));
  CHECK(result->AddOctets1(page_scan_repetition_mode));
  CHECK(result->AddOctets2(0));  // Reserved
  CHECK(result->AddOctets1(class_of_device.cod[0]));
  CHECK(result->AddOctets1(class_of_device.cod[1]));
  CHECK(result->AddOctets1(class_of_device.cod[2]));
  CHECK(!(clock_offset & 0x8000));
  CHECK(result->AddOctets2(clock_offset));
  AddBuilder(std::move(result));
  return true;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.3
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateConnectionCompleteEvent(
    hci::Status status, uint16_t handle, const Address& address, hci::LinkType link_type, bool encryption_enabled) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::CONNECTION_COMPLETE));

  CHECK(evt_ptr->AddPayloadOctets1(static_cast<uint8_t>(status)));
  CHECK((handle & 0xf000) == 0);  // Handles are 12-bit values.
  CHECK(evt_ptr->AddPayloadOctets2(handle));
  CHECK(evt_ptr->AddPayloadAddress(address));
  CHECK(evt_ptr->AddPayloadOctets1(static_cast<uint8_t>(link_type)));
  CHECK(evt_ptr->AddPayloadOctets1(encryption_enabled ? 1 : 0));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.4
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateConnectionRequestEvent(const Address& address,
                                                                                     ClassOfDevice class_of_device,
                                                                                     hci::LinkType link_type) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::CONNECTION_REQUEST));

  CHECK(evt_ptr->AddPayloadAddress(address));
  CHECK(evt_ptr->AddPayloadOctets1(class_of_device.cod[0]));
  CHECK(evt_ptr->AddPayloadOctets1(class_of_device.cod[1]));
  CHECK(evt_ptr->AddPayloadOctets1(class_of_device.cod[2]));
  CHECK(evt_ptr->AddPayloadOctets1(static_cast<uint8_t>(link_type)));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.5
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateDisconnectionCompleteEvent(hci::Status status,
                                                                                         uint16_t handle,
                                                                                         uint8_t reason) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::DISCONNECTION_COMPLETE));

  CHECK(evt_ptr->AddPayloadOctets1(static_cast<uint8_t>(status)));
  CHECK((handle & 0xf000) == 0);  // Handles are 12-bit values.
  CHECK(evt_ptr->AddPayloadOctets2(handle));
  CHECK(evt_ptr->AddPayloadOctets1(reason));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.6
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateAuthenticationCompleteEvent(hci::Status status,
                                                                                          uint16_t handle) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::AUTHENTICATION_COMPLETE));
  CHECK(evt_ptr->AddPayloadOctets1(static_cast<uint8_t>(status)));
  CHECK((handle & 0xf000) == 0);  // Handles are 12-bit values.
  CHECK(evt_ptr->AddPayloadOctets2(handle));
  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.7
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateRemoteNameRequestCompleteEvent(
    hci::Status status, const Address& address, const std::string& remote_name) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::REMOTE_NAME_REQUEST_COMPLETE));

  CHECK(evt_ptr->AddPayloadOctets1(static_cast<uint8_t>(status)));
  CHECK(evt_ptr->AddPayloadAddress(address));
  for (size_t i = 0; i < remote_name.length(); i++) CHECK(evt_ptr->AddPayloadOctets1(remote_name[i]));
  CHECK(evt_ptr->AddPayloadOctets1(0));  // Null terminated
  for (size_t i = 0; i < 248 - remote_name.length() - 1; i++) CHECK(evt_ptr->AddPayloadOctets1(0xFF));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.23
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateLinkKeyRequestEvent(const Address& remote) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::LINK_KEY_REQUEST));
  CHECK(evt_ptr->AddPayloadAddress(remote));
  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.24
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateLinkKeyNotificationEvent(const Address& remote,
                                                                                       const std::vector<uint8_t>& key,
                                                                                       uint8_t key_type) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::LINK_KEY_NOTIFICATION));
  CHECK(evt_ptr->AddPayloadAddress(remote));
  CHECK(key.size() == 16);
  CHECK(evt_ptr->AddPayloadOctets(key));
  CHECK(evt_ptr->AddPayloadOctets1(key_type));
  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.25
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateLoopbackCommandEvent(hci::OpCode opcode,
                                                                                   PacketView<true> payload) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::LOOPBACK_COMMAND));
  CHECK(evt_ptr->AddPayloadOctets2(static_cast<uint16_t>(opcode)));
  for (const auto& payload_byte : payload)  // Fill the packet.
    evt_ptr->AddPayloadOctets1(payload_byte);
  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.28
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateReadClockOffsetEvent(hci::Status status, uint16_t handle,
                                                                                   uint16_t offset) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::READ_CLOCK_OFFSET_COMPLETE));
  CHECK(evt_ptr->AddPayloadOctets1(static_cast<uint8_t>(status)));
  CHECK(evt_ptr->AddPayloadOctets2(handle));
  CHECK(evt_ptr->AddPayloadOctets2(offset));
  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.29
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateConnectionPacketTypeChangedEvent(hci::Status status,
                                                                                               uint16_t handle,
                                                                                               uint16_t packet_type) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::CONNECTION_PACKET_TYPE_CHANGE));
  CHECK(evt_ptr->AddPayloadOctets1(static_cast<uint8_t>(status)));
  CHECK(evt_ptr->AddPayloadOctets2(handle));
  CHECK(evt_ptr->AddPayloadOctets2(packet_type));
  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.37
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateSniffSubratingEvent(const hci::Status status,
                                                                                  uint16_t handle) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::SNIFF_SUBRATING, status);

  CHECK(evt_ptr->AddPayloadOctets2(handle));
  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.38
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateExtendedInquiryResultEvent(
    const Address& address, uint8_t page_scan_repetition_mode, ClassOfDevice class_of_device, uint16_t clock_offset,
    uint8_t rssi, const vector<uint8_t>& extended_inquiry_response) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::EXTENDED_INQUIRY_RESULT));

  CHECK(evt_ptr->AddPayloadOctets1(1));  // Always contains a single response

  CHECK(evt_ptr->AddPayloadAddress(address));
  CHECK(evt_ptr->AddPayloadOctets1(page_scan_repetition_mode));
  CHECK(evt_ptr->AddPayloadOctets1(0));  // Reserved
  CHECK(evt_ptr->AddPayloadOctets1(class_of_device.cod[0]));
  CHECK(evt_ptr->AddPayloadOctets1(class_of_device.cod[1]));
  CHECK(evt_ptr->AddPayloadOctets1(class_of_device.cod[2]));
  CHECK(!(clock_offset & 0x8000));
  CHECK(evt_ptr->AddPayloadOctets2(clock_offset));
  CHECK(evt_ptr->AddPayloadOctets1(rssi));
  CHECK(evt_ptr->AddPayloadOctets(extended_inquiry_response));
  evt_ptr->AddPayloadOctets1(0x00);  // End marker
  while (evt_ptr->AddPayloadOctets1(0x00))
    ;  // Fill packet
  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.40
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateIoCapabilityRequestEvent(const Address& peer) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::IO_CAPABILITY_REQUEST));

  CHECK(evt_ptr->AddPayloadAddress(peer));
  return evt_ptr;
}  // namespace packets

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.41
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateIoCapabilityResponseEvent(
    const Address& peer, uint8_t io_capability, bool oob_data_present, uint8_t authentication_requirements) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::IO_CAPABILITY_RESPONSE));

  CHECK(evt_ptr->AddPayloadAddress(peer));
  CHECK(evt_ptr->AddPayloadOctets1(io_capability));
  CHECK(evt_ptr->AddPayloadOctets1(oob_data_present));
  CHECK(evt_ptr->AddPayloadOctets1(authentication_requirements));
  return evt_ptr;
}  // namespace test_vendor_lib

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.42
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateUserConfirmationRequestEvent(const Address& peer,
                                                                                           uint32_t numeric_value) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::USER_CONFIRMATION_REQUEST));

  CHECK(evt_ptr->AddPayloadAddress(peer));
  CHECK(evt_ptr->AddPayloadOctets4(numeric_value));
  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.43
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateUserPasskeyRequestEvent(const Address& peer) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::USER_PASSKEY_REQUEST));

  CHECK(evt_ptr->AddPayloadAddress(peer));
  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.44
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateRemoteOobDataRequestEvent(const Address& peer) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::REMOTE_OOB_DATA_REQUEST));

  CHECK(evt_ptr->AddPayloadAddress(peer));
  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.45
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateSimplePairingCompleteEvent(hci::Status status,
                                                                                         const Address& peer) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::SIMPLE_PAIRING_COMPLETE));

  CHECK(evt_ptr->AddPayloadOctets1(static_cast<uint8_t>(status)));
  CHECK(evt_ptr->AddPayloadAddress(peer));
  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.48
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateUserPasskeyNotificationEvent(const Address& peer,
                                                                                           uint32_t passkey) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::USER_PASSKEY_NOTIFICATION));

  CHECK(evt_ptr->AddPayloadAddress(peer));
  CHECK(evt_ptr->AddPayloadOctets4(passkey));
  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.49
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateKeypressNotificationEvent(const Address& peer,
                                                                                        uint8_t notification_type) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::KEYPRESS_NOTIFICATION));

  CHECK(evt_ptr->AddPayloadAddress(peer));
  CHECK(evt_ptr->AddPayloadOctets1(notification_type));
  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.65.1
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateLeConnectionCompleteEvent(
    hci::Status status, uint16_t handle, uint8_t role, uint8_t peer_address_type, const Address& peer,
    uint16_t interval, uint16_t latency, uint16_t supervision_timeout) {
  std::unique_ptr<RawBuilder> meta_evt = LeMetaEventBuilder::CreateLeConnectionCompleteEvent(
      status, handle, role, peer_address_type, peer, interval, latency, supervision_timeout);

  return std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::LE_META_EVENT, std::move(meta_evt)));
}

std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateLeEnhancedConnectionCompleteEvent(
    hci::Status status, uint16_t handle, uint8_t role, uint8_t peer_address_type, const Address& peer,
    const Address& local_private_address, const Address& peer_private_address, uint16_t interval, uint16_t latency,
    uint16_t supervision_timeout) {
  std::unique_ptr<RawBuilder> meta_evt = LeMetaEventBuilder::CreateLeEnhancedConnectionCompleteEvent(
      status, handle, role, peer_address_type, peer, local_private_address, peer_private_address, interval, latency,
      supervision_timeout);

  return std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::LE_META_EVENT, std::move(meta_evt)));
}

std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateLeConnectionUpdateCompleteEvent(
    hci::Status status, uint16_t handle, uint16_t interval, uint16_t latency, uint16_t supervision_timeout) {
  std::unique_ptr<RawBuilder> meta_evt =
      LeMetaEventBuilder::CreateLeConnectionUpdateCompleteEvent(status, handle, interval, latency, supervision_timeout);

  return std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::LE_META_EVENT, std::move(meta_evt)));
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.65.2
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateLeAdvertisingReportEvent() {
  std::unique_ptr<RawBuilder> meta_evt = LeMetaEventBuilder::CreateLeAdvertisingReportEvent();

  return std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::LE_META_EVENT, std::move(meta_evt)));
}

bool EventPacketBuilder::AddLeAdvertisingReport(LeAdvertisement::AdvertisementType event_type,
                                                LeAdvertisement::AddressType addr_type, const Address& addr,
                                                const vector<uint8_t>& data, uint8_t rssi) {
  CHECK(event_code_ == EventCode::LE_META_EVENT);

  // Upcast the payload to add the next report.
  LeMetaEventBuilder* meta_ptr = static_cast<LeMetaEventBuilder*>(payload_.get());
  return meta_ptr->AddLeAdvertisingReport(event_type, addr_type, addr, data, rssi);
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.65.4
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateLeRemoteUsedFeaturesEvent(hci::Status status,
                                                                                        uint16_t handle,
                                                                                        uint64_t features) {
  std::unique_ptr<RawBuilder> meta_evt = LeMetaEventBuilder::CreateLeRemoteUsedFeaturesEvent(status, handle, features);
  return std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::LE_META_EVENT, std::move(meta_evt)));
}

std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateRemoteSupportedFeaturesEvent(hci::Status status,
                                                                                           uint16_t handle,
                                                                                           uint64_t features) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::READ_REMOTE_SUPPORTED_FEATURES_COMPLETE));

  CHECK(evt_ptr->AddPayloadOctets1(static_cast<uint8_t>(status)));
  CHECK(evt_ptr->AddPayloadOctets2(handle));
  CHECK(evt_ptr->AddPayloadOctets8(features));

  return evt_ptr;
}

std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteLinkKeyRequestReply(hci::Status status,
                                                                                                 Address address) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::LINK_KEY_REQUEST_REPLY, status);

  CHECK(evt_ptr->AddPayloadAddress(address));

  return evt_ptr;
}

std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteLinkKeyRequestNegativeReply(
    hci::Status status, Address address) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::LINK_KEY_REQUEST_NEGATIVE_REPLY, status);

  CHECK(evt_ptr->AddPayloadAddress(address));

  return evt_ptr;
}

std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteWriteLinkPolicySettings(hci::Status status,
                                                                                                     uint16_t handle) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::WRITE_LINK_POLICY_SETTINGS, status);

  CHECK(evt_ptr->AddPayloadOctets2(handle));

  return evt_ptr;
}

std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteWriteLinkSupervisionTimeout(
    hci::Status status, uint16_t handle) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::WRITE_LINK_SUPERVISION_TIMEOUT, status);

  CHECK(evt_ptr->AddPayloadOctets2(handle));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.8.2
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteLeReadBufferSize(
    hci::Status status, uint16_t hc_le_data_packet_length, uint8_t hc_total_num_le_data_packets) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::LE_READ_BUFFER_SIZE, status);

  CHECK(evt_ptr->AddPayloadOctets2(hc_le_data_packet_length));
  CHECK(evt_ptr->AddPayloadOctets1(hc_total_num_le_data_packets));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.8.3
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteLeReadLocalSupportedFeatures(
    hci::Status status, uint64_t le_features) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::LE_READ_LOCAL_SUPPORTED_FEATURES, status);

  CHECK(evt_ptr->AddPayloadOctets8(le_features));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.8.14
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteLeReadWhiteListSize(
    hci::Status status, uint8_t white_list_size) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::LE_READ_WHITE_LIST_SIZE, status);

  CHECK(evt_ptr->AddPayloadOctets8(white_list_size));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.8.23
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteLeRand(hci::Status status,
                                                                                    uint64_t random_val) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::LE_RAND, status);

  CHECK(evt_ptr->AddPayloadOctets8(random_val));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.8.27
std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteLeReadSupportedStates(hci::Status status,
                                                                                                   uint64_t le_states) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::LE_READ_SUPPORTED_STATES, status);

  CHECK(evt_ptr->AddPayloadOctets8(le_states));

  return evt_ptr;
}

// Vendor-specific commands

std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateCommandCompleteLeGetVendorCapabilities(
    hci::Status status, const vector<uint8_t>& vendor_cap) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(OpCode::LE_GET_VENDOR_CAPABILITIES, status);

  CHECK(evt_ptr->AddPayloadOctets(vendor_cap));

  return evt_ptr;
}

std::unique_ptr<EventPacketBuilder> EventPacketBuilder::CreateEncryptionChange(hci::Status status, uint16_t handle,
                                                                               uint8_t encryption_enable) {
  std::unique_ptr<EventPacketBuilder> evt_ptr =
      std::unique_ptr<EventPacketBuilder>(new EventPacketBuilder(EventCode::ENCRYPTION_CHANGE));

  CHECK(evt_ptr->AddPayloadOctets1(static_cast<uint8_t>(status)));
  CHECK(evt_ptr->AddPayloadOctets2(handle));
  CHECK(evt_ptr->AddPayloadOctets1(encryption_enable));

  return evt_ptr;
}

size_t EventPacketBuilder::size() const {
  size_t header_size = 2;  // Event code and payload size
  return header_size + payload_->size();
}

void EventPacketBuilder::Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const {
  insert(static_cast<uint8_t>(event_code_), it);
  uint8_t payload_size = size() - 2;  // Event code and payload size
  CHECK(size() - 2 == static_cast<size_t>(payload_size)) << "Payload too large for an event: " << size();
  insert(payload_size, it);
  payload_->Serialize(it);
}

bool EventPacketBuilder::CanAddPayloadOctets(size_t octets) {
  return payload_->CanAddOctets(octets);
}

bool EventPacketBuilder::AddPayloadOctets(size_t octets, const std::vector<uint8_t>& bytes) {
  return payload_->AddOctets(octets, bytes);
}

bool EventPacketBuilder::AddPayloadOctets(const std::vector<uint8_t>& bytes) {
  return payload_->AddOctets(bytes);
}

bool EventPacketBuilder::AddPayloadOctets1(uint8_t value) {
  return payload_->AddOctets1(value);
}

bool EventPacketBuilder::AddPayloadOctets2(uint16_t value) {
  return payload_->AddOctets2(value);
}

bool EventPacketBuilder::AddPayloadOctets3(uint32_t value) {
  return payload_->AddOctets3(value);
}

bool EventPacketBuilder::AddPayloadOctets4(uint32_t value) {
  return payload_->AddOctets4(value);
}

bool EventPacketBuilder::AddPayloadOctets6(uint64_t value) {
  return payload_->AddOctets6(value);
}

bool EventPacketBuilder::AddPayloadOctets8(uint64_t value) {
  return payload_->AddOctets8(value);
}

bool EventPacketBuilder::AddPayloadAddress(Address address) {
  return payload_->AddAddress(address);
}

bool EventPacketBuilder::AddBuilder(std::unique_ptr<BasePacketBuilder> builder) {
  // Upcast the payload to add the next builder.
  CountedBuilder* temp_ptr = static_cast<CountedBuilder*>(payload_.get());
  temp_ptr->Add(std::move(builder));
  return true;
}

}  // namespace packets
}  // namespace test_vendor_lib
