/*
 * Copyright 2015 The Android Open Source Project
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

#include <unistd.h>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "base/time/time.h"
#include "link_layer_controller.h"
#include "model/devices/device.h"
#include "model/setup/async_manager.h"
#include "security_manager.h"
#include "types/address.h"

namespace test_vendor_lib {

// Emulates a dual mode BR/EDR + LE controller by maintaining the link layer
// state machine detailed in the Bluetooth Core Specification Version 4.2,
// Volume 6, Part B, Section 1.1 (page 30). Provides methods corresponding to
// commands sent by the HCI. These methods will be registered as callbacks from
// a controller instance with the HciHandler. To implement a new Bluetooth
// command, simply add the method declaration below, with return type void and a
// single const std::vector<uint8_t>& argument. After implementing the
// method, simply register it with the HciHandler using the SET_HANDLER macro in
// the controller's default constructor. Be sure to name your method after the
// corresponding Bluetooth command in the Core Specification with the prefix
// "Hci" to distinguish it as a controller command.
class DualModeController : public Device {
  // The location of the config file loaded to populate controller attributes.
  static constexpr char kControllerPropertiesFile[] = "/etc/bluetooth/controller_properties.json";
  static constexpr uint16_t kSecurityManagerNumKeys = 15;

 public:
  // Sets all of the methods to be used as callbacks in the HciHandler.
  DualModeController(const std::string& properties_filename = std::string(kControllerPropertiesFile),
                     uint16_t num_keys = kSecurityManagerNumKeys);

  ~DualModeController() = default;

  // Device methods.
  virtual void Initialize(const std::vector<std::string>& args) override;

  virtual std::string GetTypeString() const override;

  virtual void IncomingPacket(packets::LinkLayerPacketView incoming) override;

  virtual void TimerTick() override;

  // Send packets to remote devices
  void SendLinkLayerPacket(std::shared_ptr<packets::LinkLayerPacketBuilder> to_send, Phy::Type phy_type);

  // Route commands and data from the stack.
  void HandleAcl(std::shared_ptr<std::vector<uint8_t>> acl_packet);
  void HandleCommand(std::shared_ptr<std::vector<uint8_t>> command_packet);
  void HandleSco(std::shared_ptr<std::vector<uint8_t>> sco_packet);

  // Set the callbacks for scheduling tasks.
  void RegisterTaskScheduler(std::function<AsyncTaskId(std::chrono::milliseconds, const TaskCallback&)> evtScheduler);

  void RegisterPeriodicTaskScheduler(
      std::function<AsyncTaskId(std::chrono::milliseconds, std::chrono::milliseconds, const TaskCallback&)>
          periodicEvtScheduler);

  void RegisterTaskCancel(std::function<void(AsyncTaskId)> cancel);

  // Set the callbacks for sending packets to the HCI.
  void RegisterEventChannel(const std::function<void(std::shared_ptr<std::vector<uint8_t>>)>& send_event);

  void RegisterAclChannel(const std::function<void(std::shared_ptr<std::vector<uint8_t>>)>& send_acl);

  void RegisterScoChannel(const std::function<void(std::shared_ptr<std::vector<uint8_t>>)>& send_sco);

  // Controller commands. For error codes, see the Bluetooth Core Specification,
  // Version 4.2, Volume 2, Part D (page 370).

  // Link Control Commands
  // Bluetooth Core Specification Version 4.2 Volume 2 Part E 7.1

  // 7.1.1
  void HciInquiry(packets::PacketView<true> args);

  // 7.1.2
  void HciInquiryCancel(packets::PacketView<true> args);

  // 7.1.5
  void HciCreateConnection(packets::PacketView<true> args);

  // 7.1.6
  void HciDisconnect(packets::PacketView<true> args);

  // 7.1.8
  void HciAcceptConnectionRequest(packets::PacketView<true> args);

  // 7.1.9
  void HciRejectConnectionRequest(packets::PacketView<true> args);

  // 7.1.10
  void HciLinkKeyRequestReply(packets::PacketView<true> args);

  // 7.1.11
  void HciLinkKeyRequestNegativeReply(packets::PacketView<true> args);

  // 7.1.14
  void HciChangeConnectionPacketType(packets::PacketView<true> args);

  // 7.1.15
  void HciAuthenticationRequested(packets::PacketView<true> args);

  // 7.1.16
  void HciSetConnectionEncryption(packets::PacketView<true> args);

  // 7.1.19
  void HciRemoteNameRequest(packets::PacketView<true> args);

  // 7.1.21
  void HciReadRemoteSupportedFeatures(packets::PacketView<true> args);

  // 7.1.22
  void HciReadRemoteExtendedFeatures(packets::PacketView<true> args);

  // 7.1.23
  void HciReadRemoteVersionInformation(packets::PacketView<true> args);

  // 7.1.24
  void HciReadClockOffset(packets::PacketView<true> args);

  // 7.1.29
  void HciIoCapabilityRequestReply(packets::PacketView<true> args);

  // 7.1.30
  void HciUserConfirmationRequestReply(packets::PacketView<true> args);

  // 7.1.31
  void HciUserConfirmationRequestNegativeReply(packets::PacketView<true> args);

  // 7.1.32
  void HciUserPasskeyRequestReply(packets::PacketView<true> args);

  // 7.1.33
  void HciUserPasskeyRequestNegativeReply(packets::PacketView<true> args);

  // 7.1.34
  void HciRemoteOobDataRequestReply(packets::PacketView<true> args);

  // 7.1.35
  void HciRemoteOobDataRequestNegativeReply(packets::PacketView<true> args);

  // 7.1.36
  void HciIoCapabilityRequestNegativeReply(packets::PacketView<true> args);

  // Link Policy Commands
  // Bluetooth Core Specification Version 4.2 Volume 2 Part E 7.2

  // 7.2.10
  void HciWriteLinkPolicySettings(packets::PacketView<true> args);

  // 7.2.12
  void HciWriteDefaultLinkPolicySettings(packets::PacketView<true> args);

  // 7.2.14
  void HciSniffSubrating(packets::PacketView<true> args);

  // Link Controller Commands
  // Bluetooth Core Specification Version 4.2 Volume 2 Part E 7.3

  // 7.3.1
  void HciSetEventMask(packets::PacketView<true> args);

  // 7.3.2
  void HciReset(packets::PacketView<true> args);

  // 7.3.3
  void HciSetEventFilter(packets::PacketView<true> args);

  // 7.3.10
  void HciDeleteStoredLinkKey(packets::PacketView<true> args);

  // 7.3.11
  void HciWriteLocalName(packets::PacketView<true> args);

  // 7.3.12
  void HciReadLocalName(packets::PacketView<true> args);

  // 7.3.16
  void HciWritePageTimeout(packets::PacketView<true> args);

  // 7.3.18
  void HciWriteScanEnable(packets::PacketView<true> args);

  // 7.3.22
  void HciWriteInquiryScanActivity(packets::PacketView<true> args);

  // 7.3.23
  void HciReadAuthenticationEnable(packets::PacketView<true> args);

  // 7.3.24
  void HciWriteAuthenticationEnable(packets::PacketView<true> args);

  // 7.3.26
  void HciWriteClassOfDevice(packets::PacketView<true> args);

  // 7.3.28
  void HciWriteVoiceSetting(packets::PacketView<true> args);

  // 7.3.39
  void HciHostBufferSize(packets::PacketView<true> args);

  // 7.3.42
  void HciWriteLinkSupervisionTimeout(packets::PacketView<true> args);

  // 7.3.45
  void HciWriteCurrentIacLap(packets::PacketView<true> args);

  // 7.3.48
  void HciWriteInquiryScanType(packets::PacketView<true> args);

  // 7.3.50
  void HciWriteInquiryMode(packets::PacketView<true> args);

  // 7.3.52
  void HciWritePageScanType(packets::PacketView<true> args);

  // 7.3.56
  void HciWriteExtendedInquiryResponse(packets::PacketView<true> args);

  // 7.3.59
  void HciWriteSimplePairingMode(packets::PacketView<true> args);

  // 7.3.79
  void HciWriteLeHostSupport(packets::PacketView<true> args);

  // Informational Parameters Commands
  // Bluetooth Core Specification Version 4.2 Volume 2 Part E 7.4

  // 7.4.5
  void HciReadBufferSize(packets::PacketView<true> args);

  // 7.4.1
  void HciReadLocalVersionInformation(packets::PacketView<true> args);

  // 7.4.6
  void HciReadBdAddr(packets::PacketView<true> args);

  // 7.4.2
  void HciReadLocalSupportedCommands(packets::PacketView<true> args);

  // 7.4.4
  void HciReadLocalExtendedFeatures(packets::PacketView<true> args);

  // 7.4.8
  void HciReadLocalSupportedCodecs(packets::PacketView<true> args);

  // Status Parameters Commands
  // Bluetooth Core Specification Version 4.2 Volume 2 Part E 7.5

  // Test Commands
  // Bluetooth Core Specification Version 4.2 Volume 2 Part E 7.7

  // 7.7.1
  void HciReadLoopbackMode(packets::PacketView<true> args);

  // 7.7.2
  void HciWriteLoopbackMode(packets::PacketView<true> args);

  // LE Controller Commands
  // Bluetooth Core Specification Version 4.2 Volume 2 Part E 7.8

  // 7.8.1
  void HciLeSetEventMask(packets::PacketView<true> args);

  // 7.8.2
  void HciLeReadBufferSize(packets::PacketView<true> args);

  // 7.8.3
  void HciLeReadLocalSupportedFeatures(packets::PacketView<true> args);

  // 7.8.4
  void HciLeSetRandomAddress(packets::PacketView<true> args);

  // 7.8.5
  void HciLeSetAdvertisingParameters(packets::PacketView<true> args);

  // 7.8.7
  void HciLeSetAdvertisingData(packets::PacketView<true> args);

  // 7.8.10
  void HciLeSetScanParameters(packets::PacketView<true> args);

  // 7.8.11
  void HciLeSetScanEnable(packets::PacketView<true> args);

  // 7.8.12
  void HciLeCreateConnection(packets::PacketView<true> args);

  // 7.8.18
  void HciLeConnectionUpdate(packets::PacketView<true> args);

  // 7.8.13
  void HciLeConnectionCancel(packets::PacketView<true> args);

  // 7.8.14
  void HciLeReadWhiteListSize(packets::PacketView<true> args);

  // 7.8.15
  void HciLeClearWhiteList(packets::PacketView<true> args);

  // 7.8.16
  void HciLeAddDeviceToWhiteList(packets::PacketView<true> args);

  // 7.8.17
  void HciLeRemoveDeviceFromWhiteList(packets::PacketView<true> args);

  // 7.8.21
  void HciLeReadRemoteFeatures(packets::PacketView<true> args);

  // 7.8.23
  void HciLeRand(packets::PacketView<true> args);

  // 7.8.24
  void HciLeStartEncryption(packets::PacketView<true> args);

  // 7.8.27
  void HciLeReadSupportedStates(packets::PacketView<true> args);

  // Vendor-specific Commands

  void HciLeVendorSleepMode(packets::PacketView<true> args);
  void HciLeVendorCap(packets::PacketView<true> args);
  void HciLeVendorMultiAdv(packets::PacketView<true> args);
  void HciLeVendor155(packets::PacketView<true> args);
  void HciLeVendor157(packets::PacketView<true> args);
  void HciLeEnergyInfo(packets::PacketView<true> args);
  void HciLeAdvertisingFilter(packets::PacketView<true> args);
  void HciLeExtendedScanParams(packets::PacketView<true> args);

  void SetTimerPeriod(std::chrono::milliseconds new_period);
  void StartTimer();
  void StopTimer();

 protected:
  LinkLayerController link_layer_controller_{properties_};

 private:
  // Set a timer for a future action
  void AddControllerEvent(std::chrono::milliseconds, const TaskCallback& callback);

  void AddConnectionAction(const TaskCallback& callback, uint16_t handle);

  // Creates a command complete event and sends it back to the HCI.
  void SendCommandComplete(hci::OpCode command_opcode, const std::vector<uint8_t>& return_parameters) const;

  // Sends a command complete event with no return parameters.
  void SendCommandCompleteSuccess(hci::OpCode command_opcode) const;

  void SendCommandCompleteUnknownOpCodeEvent(uint16_t command_opcode) const;

  // Sends a command complete event with no return parameters.
  void SendCommandCompleteOnlyStatus(hci::OpCode command_opcode, hci::Status status) const;

  void SendCommandCompleteStatusAndAddress(hci::OpCode command_opcode, hci::Status status,
                                           const Address& address) const;

  // Creates a command status event and sends it back to the HCI.
  void SendCommandStatus(hci::Status status, hci::OpCode command_opcode) const;

  // Sends a command status event with default event parameters.
  void SendCommandStatusSuccess(hci::OpCode command_opcode) const;

  // Callbacks to send packets back to the HCI.
  std::function<void(std::shared_ptr<std::vector<uint8_t>>)> send_acl_;
  std::function<void(std::shared_ptr<std::vector<uint8_t>>)> send_event_;
  std::function<void(std::shared_ptr<std::vector<uint8_t>>)> send_sco_;

  // Maintains the commands to be registered and used in the HciHandler object.
  // Keys are command opcodes and values are the callbacks to handle each
  // command.
  std::unordered_map<uint16_t, std::function<void(packets::PacketView<true>)>> active_hci_commands_;

  hci::LoopbackMode loopback_mode_;

  SecurityManager security_manager_;

  DualModeController(const DualModeController& cmdPckt) = delete;
  DualModeController& operator=(const DualModeController& cmdPckt) = delete;
};

}  // namespace test_vendor_lib
