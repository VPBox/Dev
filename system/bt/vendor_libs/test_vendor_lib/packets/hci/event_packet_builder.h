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

#include <base/logging.h>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "include/hci.h"
#include "include/le_advertisement.h"
#include "packets/counted_builder.h"
#include "packets/hci/hci_packet_builder.h"
#include "packets/packet_builder.h"
#include "packets/packet_view.h"
#include "packets/raw_builder.h"
#include "types/address.h"

namespace test_vendor_lib {
namespace packets {

// Event Packets are specified in the Bluetooth Core Specification Version 4.2,
// Volume 2, Part E, Section 5.4.4 (page 477). Event Packets begin with a 2
// octet header formatted as follows:
// - Event Code: 1 octet
// - Parameter Total Length: 1 octet
class EventPacketBuilder : public HciPacketBuilder {
 public:
  virtual ~EventPacketBuilder() override = default;

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.1
  static std::unique_ptr<EventPacketBuilder> CreateInquiryCompleteEvent(hci::Status status);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.14
  // This should only be used for testing to send non-standard packets
  // Most code should use the more specific functions that follow
  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteEvent(
      hci::OpCode command_opcode, const std::vector<uint8_t>& event_return_parameters);

  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteOnlyStatusEvent(hci::OpCode command_opcode,
                                                                                  hci::Status status);

  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteStatusAndAddressEvent(hci::OpCode command_opcode,
                                                                                        hci::Status status,
                                                                                        const Address& address);

  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteUnknownOpCodeEvent(uint16_t command_opcode);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.15
  static std::unique_ptr<EventPacketBuilder> CreateCommandStatusEvent(hci::Status status, hci::OpCode command_opcode);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.19
  static std::unique_ptr<EventPacketBuilder> CreateNumberOfCompletedPacketsEvent(uint16_t handle,
                                                                                 uint16_t num_completed_packets);

  void AddCompletedPackets(uint16_t handle, uint16_t num_completed_packets);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.1.10
  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteLinkKeyRequestReply(hci::Status status,
                                                                                      Address address);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.1.11
  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteLinkKeyRequestNegativeReply(hci::Status status,
                                                                                              Address address);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.2.10
  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteWriteLinkPolicySettings(hci::Status status,
                                                                                          uint16_t handle);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.3.10
  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteDeleteStoredLinkKey(hci::Status status,
                                                                                      uint16_t num_keys_deleted);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.3.12
  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteReadLocalName(hci::Status status,
                                                                                const std::vector<uint8_t>& local_name);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.3.23
  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteReadAuthenticationEnable(hci::Status status,
                                                                                           uint8_t enable);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.3.42
  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteWriteLinkSupervisionTimeout(hci::Status status,
                                                                                              uint16_t handle);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.4.1
  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteReadLocalVersionInformation(
      hci::Status status, uint8_t hci_version, uint16_t hci_revision, uint8_t lmp_pal_version,
      uint16_t manufacturer_name, uint16_t lmp_pal_subversion);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.4.2
  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteReadLocalSupportedCommands(
      hci::Status status, const std::vector<uint8_t>& supported_commands);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.4.4
  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteReadLocalExtendedFeatures(
      hci::Status status, uint8_t page_number, uint8_t maximum_page_number, uint64_t extended_lmp_features);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.4.5
  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteReadBufferSize(
      hci::Status status, uint16_t hc_acl_data_packet_length, uint8_t hc_synchronous_data_packet_length,
      uint16_t hc_total_num_acl_data_packets, uint16_t hc_total_synchronous_data_packets);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.4.6
  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteReadBdAddr(hci::Status status,
                                                                             const Address& bt_address);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.4.8
  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteReadLocalSupportedCodecs(
      hci::Status status, const std::vector<uint8_t>& supported_codecs,
      const std::vector<uint32_t>& vendor_specific_codecs);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.6.1
  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteReadLoopbackMode(hci::Status status,
                                                                                   hci::LoopbackMode mode);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.2
  static std::unique_ptr<EventPacketBuilder> CreateInquiryResultEvent();

  // Returns true if the result can be added to the event packet.
  bool AddInquiryResult(const Address& bt_address, uint8_t page_scan_repetition_mode, ClassOfDevice class_of_device,
                        uint16_t clock_offset);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.3
  static std::unique_ptr<EventPacketBuilder> CreateConnectionCompleteEvent(hci::Status status, uint16_t handle,
                                                                           const Address& address,
                                                                           hci::LinkType link_type,
                                                                           bool encryption_enabled);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.4
  static std::unique_ptr<EventPacketBuilder> CreateConnectionRequestEvent(const Address& address,
                                                                          ClassOfDevice class_of_device,
                                                                          hci::LinkType link_type);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.5
  static std::unique_ptr<EventPacketBuilder> CreateDisconnectionCompleteEvent(hci::Status status, uint16_t handle,
                                                                              uint8_t reason);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.6
  static std::unique_ptr<EventPacketBuilder> CreateAuthenticationCompleteEvent(hci::Status status, uint16_t handle);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.7
  static std::unique_ptr<EventPacketBuilder> CreateRemoteNameRequestCompleteEvent(hci::Status status,
                                                                                  const Address& bt_address,
                                                                                  const std::string& name);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.11
  static std::unique_ptr<EventPacketBuilder> CreateRemoteSupportedFeaturesEvent(hci::Status status, uint16_t handle,
                                                                                uint64_t features);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.12
  static std::unique_ptr<EventPacketBuilder> CreateReadRemoteVersionInformationEvent(hci::Status status,
                                                                                     uint16_t connection_handle,
                                                                                     uint8_t lmp_pal_version,
                                                                                     uint16_t manufacturer_name,
                                                                                     uint16_t lmp_pal_subversion);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.23
  static std::unique_ptr<EventPacketBuilder> CreateLinkKeyRequestEvent(const Address& remote);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.24
  static std::unique_ptr<EventPacketBuilder> CreateLinkKeyNotificationEvent(const Address& remote,
                                                                            const std::vector<uint8_t>& key,
                                                                            uint8_t key_type);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.25
  static std::unique_ptr<EventPacketBuilder> CreateLoopbackCommandEvent(hci::OpCode opcode, PacketView<true> payload);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.28
  static std::unique_ptr<EventPacketBuilder> CreateReadClockOffsetEvent(hci::Status status, uint16_t handle,
                                                                        uint16_t packet_type);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.29
  static std::unique_ptr<EventPacketBuilder> CreateConnectionPacketTypeChangedEvent(hci::Status status, uint16_t handle,
                                                                                    uint16_t offset);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.34
  static std::unique_ptr<EventPacketBuilder> CreateReadRemoteExtendedFeaturesEvent(hci::Status status, uint16_t handle,
                                                                                   uint8_t page_number,
                                                                                   uint8_t maximum_page_number,
                                                                                   uint64_t extended_lmp_features);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.37
  static std::unique_ptr<EventPacketBuilder> CreateSniffSubratingEvent(hci::Status status, uint16_t handle);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.38
  static std::unique_ptr<EventPacketBuilder> CreateExtendedInquiryResultEvent(
      const Address& bt_address, uint8_t page_scan_repetition_mode, ClassOfDevice class_of_device,
      uint16_t clock_offset, uint8_t rssi, const std::vector<uint8_t>& extended_inquiry_response);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.40
  static std::unique_ptr<EventPacketBuilder> CreateIoCapabilityRequestEvent(const Address& peer);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.41
  static std::unique_ptr<EventPacketBuilder> CreateIoCapabilityResponseEvent(const Address& peer, uint8_t io_capability,
                                                                             bool oob_data_present,
                                                                             uint8_t authentication_requirements);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.42
  static std::unique_ptr<EventPacketBuilder> CreateUserConfirmationRequestEvent(const Address& peer,
                                                                                uint32_t numeric_value);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.43
  static std::unique_ptr<EventPacketBuilder> CreateUserPasskeyRequestEvent(const Address& peer);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.44
  static std::unique_ptr<EventPacketBuilder> CreateRemoteOobDataRequestEvent(const Address& peer);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.45
  static std::unique_ptr<EventPacketBuilder> CreateSimplePairingCompleteEvent(hci::Status status, const Address& peer);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.48
  static std::unique_ptr<EventPacketBuilder> CreateUserPasskeyNotificationEvent(const Address& peer, uint32_t passkey);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.49
  static std::unique_ptr<EventPacketBuilder> CreateKeypressNotificationEvent(const Address& peer,
                                                                             uint8_t notification_type);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section
  // 7.7.65.1
  static std::unique_ptr<EventPacketBuilder> CreateLeConnectionCompleteEvent(hci::Status status, uint16_t handle,
                                                                             uint8_t role, uint8_t peer_address_type,
                                                                             const Address& peer, uint16_t interval,
                                                                             uint16_t latency,
                                                                             uint16_t supervision_timeout);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section
  // 7.7.65.2
  static std::unique_ptr<EventPacketBuilder> CreateLeAdvertisingReportEvent();

  // Returns true if the report can be added to the event packet.
  bool AddLeAdvertisingReport(LeAdvertisement::AdvertisementType event_type, LeAdvertisement::AddressType addr_type,
                              const Address& addr, const std::vector<uint8_t>& data, uint8_t rssi);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section
  // 7.7.65.3
  static std::unique_ptr<EventPacketBuilder> CreateLeConnectionUpdateCompleteEvent(hci::Status status, uint16_t handle,
                                                                                   uint16_t interval, uint16_t latency,
                                                                                   uint16_t supervision_timeout);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section
  // 7.7.65.4
  static std::unique_ptr<EventPacketBuilder> CreateLeRemoteUsedFeaturesEvent(hci::Status status, uint16_t handle,
                                                                             uint64_t features);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section
  // 7.7.65.10
  static std::unique_ptr<EventPacketBuilder> CreateLeEnhancedConnectionCompleteEvent(
      hci::Status status, uint16_t handle, uint8_t role, uint8_t peer_address_type, const Address& peer,
      const Address& local_private_address, const Address& peer_private_address, uint16_t interval, uint16_t latency,
      uint16_t supervision_timeout);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.8.2
  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteLeReadBufferSize(
      hci::Status status, uint16_t hc_le_data_packet_length, uint8_t hc_total_num_le_data_packets);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.8.3
  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteLeReadLocalSupportedFeatures(hci::Status status,
                                                                                               uint64_t le_features);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.8.14
  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteLeReadWhiteListSize(hci::Status status,
                                                                                      uint8_t white_list_size);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.8.23
  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteLeRand(hci::Status status, uint64_t random_val);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.8.27
  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteLeReadSupportedStates(hci::Status status,
                                                                                        uint64_t le_states);

  /*
  static std::unique_ptr<EventPacketBuilder>
      CreateLeStartEncryption(hci::Status status, uint8_t encryption_enable);
*/
  static std::unique_ptr<EventPacketBuilder> CreateEncryptionChange(hci::Status status, uint16_t handle,
                                                                    uint8_t encryption_enable);

  // Vendor-specific commands

  static std::unique_ptr<EventPacketBuilder> CreateCommandCompleteLeGetVendorCapabilities(
      hci::Status status, const std::vector<uint8_t>& vendor_cap);

  virtual size_t size() const override;

  virtual void Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const override;

  bool CanAddPayloadOctets(size_t octets);

  bool AddPayloadOctets(size_t octets, const std::vector<uint8_t>& bytes);

  bool AddPayloadOctets(const std::vector<uint8_t>& bytes);

  bool AddPayloadOctets1(uint8_t value);
  bool AddPayloadOctets2(uint16_t value);
  bool AddPayloadOctets3(uint32_t value);
  bool AddPayloadOctets4(uint32_t value);
  bool AddPayloadOctets6(uint64_t value);
  bool AddPayloadOctets8(uint64_t value);

  bool AddPayloadAddress(Address address);

  bool AddBuilder(std::unique_ptr<BasePacketBuilder> builder);

 private:
  explicit EventPacketBuilder(hci::EventCode event_code);
  explicit EventPacketBuilder(hci::EventCode event_code, std::unique_ptr<RawBuilder> payload);
  hci::EventCode event_code_;
  std::unique_ptr<RawBuilder> payload_;
};

}  // namespace packets
}  // namespace test_vendor_lib
