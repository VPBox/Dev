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

#pragma once

#include "acl_connection_handler.h"
#include "include/hci.h"
#include "include/inquiry.h"
#include "include/link.h"
#include "include/phy.h"
#include "model/devices/device_properties.h"
#include "model/setup/async_manager.h"
#include "packets/hci/acl_packet_view.h"
#include "packets/hci/sco_packet_view.h"
#include "packets/link_layer/link_layer_packet_builder.h"
#include "packets/link_layer/link_layer_packet_view.h"
#include "security_manager.h"
#include "types/address.h"

namespace test_vendor_lib {

class LinkLayerController {
 public:
  LinkLayerController(const DeviceProperties& properties) : properties_(properties) {}
  hci::Status SendCommandToRemoteByAddress(hci::OpCode opcode, packets::PacketView<true> args, const Address& remote,
                                           bool use_public_address);
  hci::Status SendCommandToRemoteByHandle(hci::OpCode opcode, packets::PacketView<true> args, uint16_t handle);
  hci::Status SendScoToRemote(packets::ScoPacketView sco_packet);
  hci::Status SendAclToRemote(packets::AclPacketView acl_packet);

  void WriteSimplePairingMode(bool enabled);
  void StartSimplePairing(const Address& address);
  void AuthenticateRemoteStage1(const Address& address, PairingType pairing_type);
  void AuthenticateRemoteStage2(const Address& address);
  hci::Status LinkKeyRequestReply(const Address& address, packets::PacketView<true> key);
  hci::Status LinkKeyRequestNegativeReply(const Address& address);
  hci::Status IoCapabilityRequestReply(const Address& peer, uint8_t io_capability, uint8_t oob_data_present_flag,
                                       uint8_t authentication_requirements);
  hci::Status IoCapabilityRequestNegativeReply(const Address& peer, hci::Status reason);
  hci::Status UserConfirmationRequestReply(const Address& peer);
  hci::Status UserConfirmationRequestNegativeReply(const Address& peer);
  hci::Status UserPasskeyRequestReply(const Address& peer, uint32_t numeric_value);
  hci::Status UserPasskeyRequestNegativeReply(const Address& peer);
  hci::Status RemoteOobDataRequestReply(const Address& peer, const std::vector<uint8_t>& c,
                                        const std::vector<uint8_t>& r);
  hci::Status RemoteOobDataRequestNegativeReply(const Address& peer);
  void HandleSetConnectionEncryption(const Address& address, uint16_t handle, uint8_t encryption_enable);
  hci::Status SetConnectionEncryption(uint16_t handle, uint8_t encryption_enable);
  void HandleAuthenticationRequest(const Address& address, uint16_t handle);
  hci::Status AuthenticationRequested(uint16_t handle);

  hci::Status AcceptConnectionRequest(const Address& addr, bool try_role_switch);
  void MakeSlaveConnection(const Address& addr, bool try_role_switch);
  hci::Status RejectConnectionRequest(const Address& addr, uint8_t reason);
  void RejectSlaveConnection(const Address& addr, uint8_t reason);
  hci::Status CreateConnection(const Address& addr, uint16_t packet_type, uint8_t page_scan_mode, uint16_t clock_offset,
                               uint8_t allow_role_switch);
  hci::Status CreateConnectionCancel(const Address& addr);
  hci::Status Disconnect(uint16_t handle, uint8_t reason);

 private:
  void DisconnectCleanup(uint16_t handle, uint8_t reason);

 public:
  void IncomingPacket(packets::LinkLayerPacketView incoming);

  void TimerTick();

  // Set the callbacks for sending packets to the HCI.
  void RegisterEventChannel(const std::function<void(std::shared_ptr<std::vector<uint8_t>>)>& send_event);

  void RegisterAclChannel(const std::function<void(std::shared_ptr<std::vector<uint8_t>>)>& send_acl);

  void RegisterScoChannel(const std::function<void(std::shared_ptr<std::vector<uint8_t>>)>& send_sco);

  void RegisterRemoteChannel(
      const std::function<void(std::shared_ptr<packets::LinkLayerPacketBuilder>, Phy::Type)>& send_to_remote);

  // Set the callbacks for scheduling tasks.
  void RegisterTaskScheduler(
      std::function<AsyncTaskId(std::chrono::milliseconds, const TaskCallback&)> event_scheduler);

  void RegisterPeriodicTaskScheduler(
      std::function<AsyncTaskId(std::chrono::milliseconds, std::chrono::milliseconds, const TaskCallback&)>
          periodic_event_scheduler);

  void RegisterTaskCancel(std::function<void(AsyncTaskId)> cancel);
  void Reset();
  void AddControllerEvent(std::chrono::milliseconds delay, const TaskCallback& task);

  void PageScan();
  void Connections();

  void LeWhiteListClear();
  void LeWhiteListAddDevice(Address addr, uint8_t addr_type);
  void LeWhiteListRemoveDevice(Address addr, uint8_t addr_type);
  bool LeWhiteListContainsDevice(Address addr, uint8_t addr_type);
  bool LeWhiteListFull();

  void SetLeScanEnable(uint8_t le_scan_enable) {
    le_scan_enable_ = le_scan_enable;
  }
  void SetLeScanType(uint8_t le_scan_type) {
    le_scan_type_ = le_scan_type;
  }
  void SetLeScanInterval(uint16_t le_scan_interval) {
    le_scan_interval_ = le_scan_interval;
  }
  void SetLeScanWindow(uint16_t le_scan_window) {
    le_scan_window_ = le_scan_window;
  }
  void SetLeScanFilterPolicy(uint8_t le_scan_filter_policy) {
    le_scan_filter_policy_ = le_scan_filter_policy;
  }
  void SetLeFilterDuplicates(uint8_t le_scan_filter_duplicates) {
    le_scan_filter_duplicates_ = le_scan_filter_duplicates;
  }
  void SetLeAddressType(uint8_t le_address_type) {
    le_address_type_ = le_address_type;
  }
  hci::Status SetLeConnect(bool le_connect) {
    le_connect_ = le_connect;
    return hci::Status::SUCCESS;
  }
  void SetLeConnectionIntervalMin(uint16_t min) {
    le_connection_interval_min_ = min;
  }
  void SetLeConnectionIntervalMax(uint16_t max) {
    le_connection_interval_max_ = max;
  }
  void SetLeConnectionLatency(uint16_t latency) {
    le_connection_latency_ = latency;
  }
  void SetLeSupervisionTimeout(uint16_t timeout) {
    le_connection_supervision_timeout_ = timeout;
  }
  void SetLeMinimumCeLength(uint16_t min) {
    le_connection_minimum_ce_length_ = min;
  }
  void SetLeMaximumCeLength(uint16_t max) {
    le_connection_maximum_ce_length_ = max;
  }
  void SetLeInitiatorFilterPolicy(uint8_t le_initiator_filter_policy) {
    le_initiator_filter_policy_ = le_initiator_filter_policy;
  }
  void SetLePeerAddressType(uint8_t peer_address_type) {
    le_peer_address_type_ = peer_address_type;
  }
  void SetLePeerAddress(const Address& peer_address) {
    le_peer_address_ = peer_address;
  }

  // Classic
  void StartInquiry(std::chrono::milliseconds timeout);
  void InquiryCancel();
  void InquiryTimeout();
  void SetInquiryMode(uint8_t mode);
  void SetInquiryLAP(uint64_t lap);
  void SetInquiryMaxResponses(uint8_t max);
  void Inquiry();

  void SetInquiryScanEnable(bool enable);
  void SetPageScanEnable(bool enable);

  hci::Status ChangeConnectionPacketType(uint16_t handle, uint16_t types);
  hci::Status WriteLinkPolicySettings(uint16_t handle, uint16_t settings);
  hci::Status WriteLinkSupervisionTimeout(uint16_t handle, uint16_t timeout);

 protected:
  void SendLELinkLayerPacket(std::shared_ptr<packets::LinkLayerPacketBuilder> packet);
  void SendLinkLayerPacket(std::shared_ptr<packets::LinkLayerPacketBuilder> packet);
  void IncomingAclPacket(packets::LinkLayerPacketView packet);
  void IncomingAclAckPacket(packets::LinkLayerPacketView packet);
  void IncomingCommandPacket(packets::LinkLayerPacketView packet);
  void IncomingCreateConnectionPacket(packets::LinkLayerPacketView packet);
  void IncomingDisconnectPacket(packets::LinkLayerPacketView packet);
  void IncomingEncryptConnection(packets::LinkLayerPacketView packet);
  void IncomingEncryptConnectionResponse(packets::LinkLayerPacketView packet);
  void IncomingInquiryPacket(packets::LinkLayerPacketView packet);
  void IncomingInquiryResponsePacket(packets::LinkLayerPacketView packet);
  void IncomingIoCapabilityRequestPacket(packets::LinkLayerPacketView packet);
  void IncomingIoCapabilityResponsePacket(packets::LinkLayerPacketView packet);
  void IncomingIoCapabilityNegativeResponsePacket(packets::LinkLayerPacketView packet);
  void IncomingLeAdvertisementPacket(packets::LinkLayerPacketView packet);
  void IncomingLeScanPacket(packets::LinkLayerPacketView packet);
  void IncomingLeScanResponsePacket(packets::LinkLayerPacketView packet);
  void IncomingPagePacket(packets::LinkLayerPacketView packet);
  void IncomingPageResponsePacket(packets::LinkLayerPacketView packet);
  void IncomingResponsePacket(packets::LinkLayerPacketView packet);

 private:
  const DeviceProperties& properties_;
  AclConnectionHandler classic_connections_;
  // Add timestamps?
  std::vector<std::shared_ptr<packets::LinkLayerPacketBuilder>> commands_awaiting_responses_;

  // Timing related state
  std::vector<AsyncTaskId> controller_events_;
  AsyncTaskId timer_tick_task_;
  std::chrono::milliseconds timer_period_ = std::chrono::milliseconds(100);

  // Callbacks to schedule tasks.
  std::function<AsyncTaskId(std::chrono::milliseconds, const TaskCallback&)> schedule_task_;
  std::function<AsyncTaskId(std::chrono::milliseconds, std::chrono::milliseconds, const TaskCallback&)>
      schedule_periodic_task_;
  std::function<void(AsyncTaskId)> cancel_task_;

  // Callbacks to send packets back to the HCI.
  std::function<void(std::shared_ptr<std::vector<uint8_t>>)> send_acl_;
  std::function<void(std::shared_ptr<std::vector<uint8_t>>)> send_event_;
  std::function<void(std::shared_ptr<std::vector<uint8_t>>)> send_sco_;

  // Callback to send packets to remote devices.
  std::function<void(std::shared_ptr<packets::LinkLayerPacketBuilder>, Phy::Type phy_type)> send_to_remote_;

  // LE state
  std::vector<uint8_t> le_event_mask_;

  std::vector<std::tuple<Address, uint8_t>> le_white_list_;

  uint8_t le_scan_enable_;
  uint8_t le_scan_type_;
  uint16_t le_scan_interval_;
  uint16_t le_scan_window_;
  uint8_t le_scan_filter_policy_;
  uint8_t le_scan_filter_duplicates_;
  uint8_t le_address_type_;

  bool le_connect_;
  uint16_t le_connection_interval_min_;
  uint16_t le_connection_interval_max_;
  uint16_t le_connection_latency_;
  uint16_t le_connection_supervision_timeout_;
  uint16_t le_connection_minimum_ce_length_;
  uint16_t le_connection_maximum_ce_length_;
  uint8_t le_initiator_filter_policy_;

  Address le_peer_address_;
  uint8_t le_peer_address_type_;

  // Classic state

  SecurityManager security_manager_{10};
  std::chrono::steady_clock::time_point last_inquiry_;
  Inquiry::InquiryType inquiry_mode_;
  Inquiry::InquiryState inquiry_state_;
  uint64_t inquiry_lap_;
  uint8_t inquiry_max_responses_;

  bool page_scans_enabled_{false};
  bool inquiry_scans_enabled_{false};

  bool simple_pairing_mode_enabled_{false};
};

}  // namespace test_vendor_lib
