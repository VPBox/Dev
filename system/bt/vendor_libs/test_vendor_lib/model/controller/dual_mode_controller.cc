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

#define LOG_TAG "dual_mode_controller"

#include "dual_mode_controller.h"

#include <memory>

#include <base/files/file_util.h>
#include <base/json/json_reader.h>
#include <base/logging.h>
#include <base/values.h>

#include "osi/include/log.h"
#include "osi/include/osi.h"

#include "hci.h"
#include "packets/hci/acl_packet_view.h"
#include "packets/hci/command_packet_view.h"
#include "packets/hci/event_packet_builder.h"
#include "packets/hci/sco_packet_view.h"

using std::vector;
using test_vendor_lib::hci::EventCode;
using test_vendor_lib::hci::OpCode;

namespace {

size_t LastNonZero(test_vendor_lib::packets::PacketView<true> view) {
  for (size_t i = view.size() - 1; i > 0; i--) {
    if (view[i] != 0) {
      return i;
    }
  }
  return 0;
}

}  // namespace

namespace test_vendor_lib {
constexpr char DualModeController::kControllerPropertiesFile[];
constexpr uint16_t DualModeController::kSecurityManagerNumKeys;

// Device methods.
void DualModeController::Initialize(const std::vector<std::string>& args) {
  if (args.size() < 2) return;

  Address addr;
  if (Address::FromString(args[1], addr)) properties_.SetAddress(addr);
};

std::string DualModeController::GetTypeString() const {
  return "Simulated Bluetooth Controller";
}

void DualModeController::IncomingPacket(packets::LinkLayerPacketView incoming) {
  link_layer_controller_.IncomingPacket(incoming);
}

void DualModeController::TimerTick() {
  link_layer_controller_.TimerTick();
}

void DualModeController::SendLinkLayerPacket(std::shared_ptr<packets::LinkLayerPacketBuilder> to_send,
                                             Phy::Type phy_type) {
  for (auto phy_pair : phy_layers_) {
    auto phy_list = std::get<1>(phy_pair);
    if (phy_type != std::get<0>(phy_pair)) {
      continue;
    }
    for (auto phy : phy_list) {
      phy->Send(to_send);
    }
  }
}

/*
void DualModeController::AddConnectionAction(const TaskCallback& task,
                                             uint16_t handle) {
  for (size_t i = 0; i < connections_.size(); i++)
    if (connections_[i]->GetHandle() == handle)
connections_[i]->AddAction(task);
}
*/

void DualModeController::SendCommandCompleteSuccess(OpCode command_opcode) const {
  send_event_(packets::EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(command_opcode, hci::Status::SUCCESS)
                  ->ToVector());
}

void DualModeController::SendCommandCompleteUnknownOpCodeEvent(uint16_t command_opcode) const {
  send_event_(packets::EventPacketBuilder::CreateCommandCompleteUnknownOpCodeEvent(command_opcode)->ToVector());
}

void DualModeController::SendCommandCompleteOnlyStatus(OpCode command_opcode, hci::Status status) const {
  send_event_(packets::EventPacketBuilder::CreateCommandCompleteOnlyStatusEvent(command_opcode, status)->ToVector());
}

void DualModeController::SendCommandCompleteStatusAndAddress(OpCode command_opcode, hci::Status status,
                                                             const Address& address) const {
  send_event_(packets::EventPacketBuilder::CreateCommandCompleteStatusAndAddressEvent(command_opcode, status, address)
                  ->ToVector());
}

void DualModeController::SendCommandStatus(hci::Status status, OpCode command_opcode) const {
  send_event_(packets::EventPacketBuilder::CreateCommandStatusEvent(status, command_opcode)->ToVector());
}

void DualModeController::SendCommandStatusSuccess(OpCode command_opcode) const {
  SendCommandStatus(hci::Status::SUCCESS, command_opcode);
}

DualModeController::DualModeController(const std::string& properties_filename, uint16_t num_keys)
    : Device(properties_filename), security_manager_(num_keys) {
  loopback_mode_ = hci::LoopbackMode::NO;

  Address public_address;
  CHECK(Address::FromString("3C:5A:B4:04:05:06", public_address));
  properties_.SetAddress(public_address);

  link_layer_controller_.RegisterRemoteChannel(
      [this](std::shared_ptr<packets::LinkLayerPacketBuilder> packet, Phy::Type phy_type) {
        DualModeController::SendLinkLayerPacket(packet, phy_type);
      });

#define SET_HANDLER(opcode, method) \
  active_hci_commands_[static_cast<uint16_t>(opcode)] = [this](packets::PacketView<true> param) { method(param); };
  SET_HANDLER(OpCode::RESET, HciReset);
  SET_HANDLER(OpCode::READ_BUFFER_SIZE, HciReadBufferSize);
  SET_HANDLER(OpCode::HOST_BUFFER_SIZE, HciHostBufferSize);
  SET_HANDLER(OpCode::SNIFF_SUBRATING, HciSniffSubrating);
  SET_HANDLER(OpCode::READ_LOCAL_VERSION_INFORMATION, HciReadLocalVersionInformation);
  SET_HANDLER(OpCode::READ_BD_ADDR, HciReadBdAddr);
  SET_HANDLER(OpCode::READ_LOCAL_SUPPORTED_COMMANDS, HciReadLocalSupportedCommands);
  SET_HANDLER(OpCode::READ_LOCAL_SUPPORTED_CODECS, HciReadLocalSupportedCodecs);
  SET_HANDLER(OpCode::READ_LOCAL_EXTENDED_FEATURES, HciReadLocalExtendedFeatures);
  SET_HANDLER(OpCode::READ_REMOTE_EXTENDED_FEATURES, HciReadRemoteExtendedFeatures);
  SET_HANDLER(OpCode::READ_REMOTE_SUPPORTED_FEATURES, HciReadRemoteSupportedFeatures);
  SET_HANDLER(OpCode::READ_CLOCK_OFFSET, HciReadClockOffset);
  SET_HANDLER(OpCode::IO_CAPABILITY_REQUEST_REPLY, HciIoCapabilityRequestReply);
  SET_HANDLER(OpCode::USER_CONFIRMATION_REQUEST_REPLY, HciUserConfirmationRequestReply);
  SET_HANDLER(OpCode::USER_CONFIRMATION_REQUEST_NEGATIVE_REPLY, HciUserConfirmationRequestNegativeReply);
  SET_HANDLER(OpCode::IO_CAPABILITY_REQUEST_NEGATIVE_REPLY, HciIoCapabilityRequestNegativeReply);
  SET_HANDLER(OpCode::WRITE_SIMPLE_PAIRING_MODE, HciWriteSimplePairingMode);
  SET_HANDLER(OpCode::WRITE_LE_HOST_SUPPORT, HciWriteLeHostSupport);
  SET_HANDLER(OpCode::SET_EVENT_MASK, HciSetEventMask);
  SET_HANDLER(OpCode::WRITE_INQUIRY_MODE, HciWriteInquiryMode);
  SET_HANDLER(OpCode::WRITE_PAGE_SCAN_TYPE, HciWritePageScanType);
  SET_HANDLER(OpCode::WRITE_INQUIRY_SCAN_TYPE, HciWriteInquiryScanType);
  SET_HANDLER(OpCode::AUTHENTICATION_REQUESTED, HciAuthenticationRequested);
  SET_HANDLER(OpCode::SET_CONNECTION_ENCRYPTION, HciSetConnectionEncryption);
  SET_HANDLER(OpCode::WRITE_AUTHENTICATION_ENABLE, HciWriteAuthenticationEnable);
  SET_HANDLER(OpCode::READ_AUTHENTICATION_ENABLE, HciReadAuthenticationEnable);
  SET_HANDLER(OpCode::WRITE_CLASS_OF_DEVICE, HciWriteClassOfDevice);
  SET_HANDLER(OpCode::WRITE_PAGE_TIMEOUT, HciWritePageTimeout);
  SET_HANDLER(OpCode::WRITE_LINK_SUPERVISION_TIMEOUT, HciWriteLinkSupervisionTimeout);
  SET_HANDLER(OpCode::WRITE_DEFAULT_LINK_POLICY_SETTINGS, HciWriteDefaultLinkPolicySettings);
  SET_HANDLER(OpCode::WRITE_LINK_POLICY_SETTINGS, HciWriteLinkPolicySettings);
  SET_HANDLER(OpCode::CHANGE_CONNECTION_PACKET_TYPE, HciChangeConnectionPacketType);
  SET_HANDLER(OpCode::WRITE_LOCAL_NAME, HciWriteLocalName);
  SET_HANDLER(OpCode::READ_LOCAL_NAME, HciReadLocalName);
  SET_HANDLER(OpCode::WRITE_EXTENDED_INQUIRY_RESPONSE, HciWriteExtendedInquiryResponse);
  SET_HANDLER(OpCode::WRITE_VOICE_SETTING, HciWriteVoiceSetting);
  SET_HANDLER(OpCode::WRITE_CURRENT_IAC_LAP, HciWriteCurrentIacLap);
  SET_HANDLER(OpCode::WRITE_INQUIRY_SCAN_ACTIVITY, HciWriteInquiryScanActivity);
  SET_HANDLER(OpCode::WRITE_SCAN_ENABLE, HciWriteScanEnable);
  SET_HANDLER(OpCode::SET_EVENT_FILTER, HciSetEventFilter);
  SET_HANDLER(OpCode::INQUIRY, HciInquiry);
  SET_HANDLER(OpCode::INQUIRY_CANCEL, HciInquiryCancel);
  SET_HANDLER(OpCode::ACCEPT_CONNECTION_REQUEST, HciAcceptConnectionRequest);
  SET_HANDLER(OpCode::REJECT_CONNECTION_REQUEST, HciRejectConnectionRequest);
  SET_HANDLER(OpCode::LINK_KEY_REQUEST_REPLY, HciLinkKeyRequestReply);
  SET_HANDLER(OpCode::LINK_KEY_REQUEST_NEGATIVE_REPLY, HciLinkKeyRequestNegativeReply);
  SET_HANDLER(OpCode::DELETE_STORED_LINK_KEY, HciDeleteStoredLinkKey);
  SET_HANDLER(OpCode::REMOTE_NAME_REQUEST, HciRemoteNameRequest);
  SET_HANDLER(OpCode::LE_SET_EVENT_MASK, HciLeSetEventMask);
  SET_HANDLER(OpCode::LE_READ_BUFFER_SIZE, HciLeReadBufferSize);
  SET_HANDLER(OpCode::LE_READ_LOCAL_SUPPORTED_FEATURES, HciLeReadLocalSupportedFeatures);
  SET_HANDLER(OpCode::LE_SET_RANDOM_ADDRESS, HciLeSetRandomAddress);
  SET_HANDLER(OpCode::LE_SET_ADVERTISING_DATA, HciLeSetAdvertisingData);
  SET_HANDLER(OpCode::LE_SET_ADVERTISING_PARAMETERS, HciLeSetAdvertisingParameters);
  SET_HANDLER(OpCode::LE_SET_SCAN_PARAMETERS, HciLeSetScanParameters);
  SET_HANDLER(OpCode::LE_SET_SCAN_ENABLE, HciLeSetScanEnable);
  SET_HANDLER(OpCode::LE_CREATE_CONNECTION, HciLeCreateConnection);
  SET_HANDLER(OpCode::CREATE_CONNECTION, HciCreateConnection);
  SET_HANDLER(OpCode::DISCONNECT, HciDisconnect);
  SET_HANDLER(OpCode::LE_CREATE_CONNECTION_CANCEL, HciLeConnectionCancel);
  SET_HANDLER(OpCode::LE_READ_WHITE_LIST_SIZE, HciLeReadWhiteListSize);
  SET_HANDLER(OpCode::LE_CLEAR_WHITE_LIST, HciLeClearWhiteList);
  SET_HANDLER(OpCode::LE_ADD_DEVICE_TO_WHITE_LIST, HciLeAddDeviceToWhiteList);
  SET_HANDLER(OpCode::LE_REMOVE_DEVICE_FROM_WHITE_LIST, HciLeRemoveDeviceFromWhiteList);
  SET_HANDLER(OpCode::LE_RAND, HciLeRand);
  SET_HANDLER(OpCode::LE_READ_SUPPORTED_STATES, HciLeReadSupportedStates);
  SET_HANDLER(OpCode::LE_GET_VENDOR_CAPABILITIES, HciLeVendorCap);
  SET_HANDLER(OpCode::LE_MULTI_ADVT, HciLeVendorMultiAdv);
  SET_HANDLER(OpCode::LE_ADV_FILTER, HciLeAdvertisingFilter);
  SET_HANDLER(OpCode::LE_ENERGY_INFO, HciLeEnergyInfo);
  SET_HANDLER(OpCode::LE_EXTENDED_SCAN_PARAMS, HciLeExtendedScanParams);
  SET_HANDLER(OpCode::LE_READ_REMOTE_FEATURES, HciLeReadRemoteFeatures);
  SET_HANDLER(OpCode::READ_REMOTE_VERSION_INFORMATION, HciReadRemoteVersionInformation);
  SET_HANDLER(OpCode::LE_CONNECTION_UPDATE, HciLeConnectionUpdate);
  SET_HANDLER(OpCode::LE_START_ENCRYPTION, HciLeStartEncryption);
  // Testing Commands
  SET_HANDLER(OpCode::READ_LOOPBACK_MODE, HciReadLoopbackMode);
  SET_HANDLER(OpCode::WRITE_LOOPBACK_MODE, HciWriteLoopbackMode);
#undef SET_HANDLER
}

void DualModeController::HciSniffSubrating(packets::PacketView<true> args) {
  CHECK(args.size() == 8) << __func__ << " size=" << args.size();

  uint16_t handle = args.begin().extract<uint16_t>();

  send_event_(packets::EventPacketBuilder::CreateSniffSubratingEvent(hci::Status::SUCCESS, handle)->ToVector());
}

void DualModeController::RegisterTaskScheduler(
    std::function<AsyncTaskId(std::chrono::milliseconds, const TaskCallback&)> oneshot_scheduler) {
  link_layer_controller_.RegisterTaskScheduler(oneshot_scheduler);
}

void DualModeController::RegisterPeriodicTaskScheduler(
    std::function<AsyncTaskId(std::chrono::milliseconds, std::chrono::milliseconds, const TaskCallback&)>
        periodic_scheduler) {
  link_layer_controller_.RegisterPeriodicTaskScheduler(periodic_scheduler);
}

void DualModeController::RegisterTaskCancel(std::function<void(AsyncTaskId)> task_cancel) {
  link_layer_controller_.RegisterTaskCancel(task_cancel);
}

void DualModeController::HandleAcl(std::shared_ptr<std::vector<uint8_t>> packet) {
  auto acl_packet = packets::AclPacketView::Create(packet);
  if (loopback_mode_ == hci::LoopbackMode::LOCAL) {
    uint16_t handle = acl_packet.GetHandle();
    send_acl_(packet);
    send_event_(packets::EventPacketBuilder::CreateNumberOfCompletedPacketsEvent(handle, 1)->ToVector());
    return;
  }

  link_layer_controller_.SendAclToRemote(acl_packet);
}

void DualModeController::HandleSco(std::shared_ptr<std::vector<uint8_t>> packet) {
  auto sco_packet = packets::ScoPacketView::Create(packet);
  if (loopback_mode_ == hci::LoopbackMode::LOCAL) {
    uint16_t handle = sco_packet.GetHandle();
    send_sco_(packet);
    send_event_(packets::EventPacketBuilder::CreateNumberOfCompletedPacketsEvent(handle, 1)->ToVector());
    return;
  }
}

void DualModeController::HandleCommand(std::shared_ptr<std::vector<uint8_t>> packet) {
  auto command_packet = packets::CommandPacketView::Create(packet);
  uint16_t opcode = command_packet.GetOpcode();
  hci::OpCode op = static_cast<hci::OpCode>(opcode);

  if (loopback_mode_ == hci::LoopbackMode::LOCAL &&
      // Loopback exceptions.
      op != OpCode::RESET && op != OpCode::SET_CONTROLLER_TO_HOST_FLOW_CONTROL && op != OpCode::HOST_BUFFER_SIZE &&
      op != OpCode::HOST_NUM_COMPLETED_PACKETS && op != OpCode::READ_BUFFER_SIZE && op != OpCode::READ_LOOPBACK_MODE &&
      op != OpCode::WRITE_LOOPBACK_MODE) {
    send_event_(packets::EventPacketBuilder::CreateLoopbackCommandEvent(op, command_packet.GetPayload())->ToVector());
  } else if (active_hci_commands_.count(opcode) > 0) {
    active_hci_commands_[opcode](command_packet.GetPayload());
  } else {
    SendCommandCompleteUnknownOpCodeEvent(opcode);
    LOG_INFO(LOG_TAG, "Command opcode: 0x%04X, OGF: 0x%04X, OCF: 0x%04X", opcode, opcode & 0xFC00, opcode & 0x03FF);
  }
}

void DualModeController::RegisterEventChannel(
    const std::function<void(std::shared_ptr<std::vector<uint8_t>>)>& callback) {
  link_layer_controller_.RegisterEventChannel(callback);
  send_event_ = callback;
}

void DualModeController::RegisterAclChannel(
    const std::function<void(std::shared_ptr<std::vector<uint8_t>>)>& callback) {
  link_layer_controller_.RegisterAclChannel(callback);
  send_acl_ = callback;
}

void DualModeController::RegisterScoChannel(
    const std::function<void(std::shared_ptr<std::vector<uint8_t>>)>& callback) {
  link_layer_controller_.RegisterScoChannel(callback);
  send_sco_ = callback;
}

void DualModeController::HciReset(packets::PacketView<true> args) {
  CHECK(args.size() == 0) << __func__ << " size=" << args.size();
  link_layer_controller_.Reset();

  SendCommandCompleteSuccess(OpCode::RESET);
}

void DualModeController::HciReadBufferSize(packets::PacketView<true> args) {
  CHECK(args.size() == 0) << __func__ << " size=" << args.size();
  std::shared_ptr<packets::EventPacketBuilder> command_complete =
      packets::EventPacketBuilder::CreateCommandCompleteReadBufferSize(
          hci::Status::SUCCESS, properties_.GetAclDataPacketSize(), properties_.GetSynchronousDataPacketSize(),
          properties_.GetTotalNumAclDataPackets(), properties_.GetTotalNumSynchronousDataPackets());

  send_event_(command_complete->ToVector());
}

void DualModeController::HciHostBufferSize(packets::PacketView<true> args) {
  CHECK(args.size() == 7) << __func__ << " size=" << args.size();
  SendCommandCompleteSuccess(OpCode::HOST_BUFFER_SIZE);
}

void DualModeController::HciReadLocalVersionInformation(packets::PacketView<true> args) {
  CHECK(args.size() == 0) << __func__ << " size=" << args.size();
  std::shared_ptr<packets::EventPacketBuilder> command_complete =
      packets::EventPacketBuilder::CreateCommandCompleteReadLocalVersionInformation(
          hci::Status::SUCCESS, properties_.GetVersion(), properties_.GetRevision(), properties_.GetLmpPalVersion(),
          properties_.GetManufacturerName(), properties_.GetLmpPalSubversion());
  send_event_(command_complete->ToVector());
}

void DualModeController::HciReadRemoteVersionInformation(packets::PacketView<true> args) {
  CHECK(args.size() == 2) << __func__ << " size=" << args.size();

  uint16_t handle = args.begin().extract<uint16_t>();

  hci::Status status =
      link_layer_controller_.SendCommandToRemoteByHandle(OpCode::READ_REMOTE_VERSION_INFORMATION, args, handle);

  SendCommandStatus(status, OpCode::READ_REMOTE_VERSION_INFORMATION);
}

void DualModeController::HciReadBdAddr(packets::PacketView<true> args) {
  CHECK(args.size() == 0) << __func__ << " size=" << args.size();
  std::shared_ptr<packets::EventPacketBuilder> command_complete =
      packets::EventPacketBuilder::CreateCommandCompleteReadBdAddr(hci::Status::SUCCESS, properties_.GetAddress());
  send_event_(command_complete->ToVector());
}

void DualModeController::HciReadLocalSupportedCommands(packets::PacketView<true> args) {
  CHECK(args.size() == 0) << __func__ << " size=" << args.size();
  std::shared_ptr<packets::EventPacketBuilder> command_complete =
      packets::EventPacketBuilder::CreateCommandCompleteReadLocalSupportedCommands(hci::Status::SUCCESS,
                                                                                   properties_.GetSupportedCommands());
  send_event_(command_complete->ToVector());
}

void DualModeController::HciReadLocalSupportedCodecs(packets::PacketView<true> args) {
  CHECK(args.size() == 0) << __func__ << " size=" << args.size();
  std::shared_ptr<packets::EventPacketBuilder> command_complete =
      packets::EventPacketBuilder::CreateCommandCompleteReadLocalSupportedCodecs(
          hci::Status::SUCCESS, properties_.GetSupportedCodecs(), properties_.GetVendorSpecificCodecs());
  send_event_(command_complete->ToVector());
}

void DualModeController::HciReadLocalExtendedFeatures(packets::PacketView<true> args) {
  CHECK(args.size() == 1) << __func__ << " size=" << args.size();
  uint8_t page_number = args.begin().extract<uint8_t>();
  send_event_(packets::EventPacketBuilder::CreateCommandCompleteReadLocalExtendedFeatures(
                  hci::Status::SUCCESS, page_number, properties_.GetExtendedFeaturesMaximumPageNumber(),
                  properties_.GetExtendedFeatures(page_number))
                  ->ToVector());
}

void DualModeController::HciReadRemoteExtendedFeatures(packets::PacketView<true> args) {
  CHECK(args.size() == 3) << __func__ << " size=" << args.size();

  uint16_t handle = args.begin().extract<uint16_t>();

  hci::Status status =
      link_layer_controller_.SendCommandToRemoteByHandle(OpCode::READ_REMOTE_EXTENDED_FEATURES, args, handle);

  SendCommandStatus(status, OpCode::READ_REMOTE_EXTENDED_FEATURES);
}

void DualModeController::HciReadRemoteSupportedFeatures(packets::PacketView<true> args) {
  CHECK(args.size() == 2) << __func__ << " size=" << args.size();

  uint16_t handle = args.begin().extract<uint16_t>();

  hci::Status status =
      link_layer_controller_.SendCommandToRemoteByHandle(OpCode::READ_REMOTE_SUPPORTED_FEATURES, args, handle);

  SendCommandStatus(status, OpCode::READ_REMOTE_SUPPORTED_FEATURES);
}

void DualModeController::HciReadClockOffset(packets::PacketView<true> args) {
  CHECK(args.size() == 2) << __func__ << " size=" << args.size();

  uint16_t handle = args.begin().extract<uint16_t>();

  hci::Status status = link_layer_controller_.SendCommandToRemoteByHandle(OpCode::READ_CLOCK_OFFSET, args, handle);

  SendCommandStatus(status, OpCode::READ_CLOCK_OFFSET);
}

void DualModeController::HciIoCapabilityRequestReply(packets::PacketView<true> args) {
  CHECK(args.size() == 9) << __func__ << " size=" << args.size();

  auto args_itr = args.begin();
  Address peer = args_itr.extract<Address>();
  uint8_t io_capability = args_itr.extract<uint8_t>();
  uint8_t oob_data_present_flag = args_itr.extract<uint8_t>();
  uint8_t authentication_requirements = args_itr.extract<uint8_t>();

  hci::Status status = link_layer_controller_.IoCapabilityRequestReply(peer, io_capability, oob_data_present_flag,
                                                                       authentication_requirements);

  SendCommandCompleteStatusAndAddress(OpCode::IO_CAPABILITY_REQUEST_REPLY, status, peer);
}

void DualModeController::HciUserConfirmationRequestReply(packets::PacketView<true> args) {
  CHECK(args.size() == 6) << __func__ << " size=" << args.size();

  Address peer = args.begin().extract<Address>();

  hci::Status status = link_layer_controller_.UserConfirmationRequestReply(peer);

  SendCommandCompleteStatusAndAddress(OpCode::USER_CONFIRMATION_REQUEST_REPLY, status, peer);
}

void DualModeController::HciUserConfirmationRequestNegativeReply(packets::PacketView<true> args) {
  CHECK(args.size() == 6) << __func__ << " size=" << args.size();

  Address peer = args.begin().extract<Address>();

  hci::Status status = link_layer_controller_.UserConfirmationRequestNegativeReply(peer);

  SendCommandCompleteStatusAndAddress(OpCode::USER_CONFIRMATION_REQUEST_NEGATIVE_REPLY, status, peer);
}

void DualModeController::HciUserPasskeyRequestReply(packets::PacketView<true> args) {
  CHECK(args.size() == 10) << __func__ << " size=" << args.size();

  auto args_itr = args.begin();
  Address peer = args_itr.extract<Address>();
  uint32_t numeric_value = args_itr.extract<uint32_t>();

  hci::Status status = link_layer_controller_.UserPasskeyRequestReply(peer, numeric_value);

  SendCommandCompleteStatusAndAddress(OpCode::USER_PASSKEY_REQUEST_REPLY, status, peer);
}

void DualModeController::HciUserPasskeyRequestNegativeReply(packets::PacketView<true> args) {
  CHECK(args.size() == 6) << __func__ << " size=" << args.size();

  Address peer = args.begin().extract<Address>();

  hci::Status status = link_layer_controller_.UserPasskeyRequestNegativeReply(peer);

  SendCommandCompleteStatusAndAddress(OpCode::USER_PASSKEY_REQUEST_NEGATIVE_REPLY, status, peer);
}

void DualModeController::HciRemoteOobDataRequestReply(packets::PacketView<true> args) {
  CHECK(args.size() == 38) << __func__ << " size=" << args.size();

  auto args_itr = args.begin();
  Address peer = args_itr.extract<Address>();
  std::vector<uint8_t> c;
  std::vector<uint8_t> r;
  for (size_t i = 0; i < 16; i++) {
    c.push_back(args_itr.extract<uint8_t>());
  }
  for (size_t i = 0; i < 16; i++) {
    r.push_back(args_itr.extract<uint8_t>());
  }
  hci::Status status = link_layer_controller_.RemoteOobDataRequestReply(peer, c, r);

  SendCommandCompleteStatusAndAddress(OpCode::REMOTE_OOB_DATA_REQUEST_REPLY, status, peer);
}

void DualModeController::HciRemoteOobDataRequestNegativeReply(packets::PacketView<true> args) {
  CHECK(args.size() == 6) << __func__ << " size=" << args.size();

  Address peer = args.begin().extract<Address>();

  hci::Status status = link_layer_controller_.RemoteOobDataRequestNegativeReply(peer);

  SendCommandCompleteStatusAndAddress(OpCode::REMOTE_OOB_DATA_REQUEST_NEGATIVE_REPLY, status, peer);
}

void DualModeController::HciIoCapabilityRequestNegativeReply(packets::PacketView<true> args) {
  CHECK(args.size() == 7) << __func__ << " size=" << args.size();

  auto args_itr = args.begin();
  Address peer = args_itr.extract<Address>();
  hci::Status reason = args_itr.extract<hci::Status>();

  hci::Status status = link_layer_controller_.IoCapabilityRequestNegativeReply(peer, reason);

  SendCommandCompleteStatusAndAddress(OpCode::IO_CAPABILITY_REQUEST_NEGATIVE_REPLY, status, peer);
}

void DualModeController::HciWriteSimplePairingMode(packets::PacketView<true> args) {
  CHECK(args.size() == 1) << __func__ << " size=" << args.size();
  CHECK(args[0] == 1 || args[0] == 0);
  link_layer_controller_.WriteSimplePairingMode(args[0] == 1);
  SendCommandCompleteSuccess(OpCode::WRITE_SIMPLE_PAIRING_MODE);
}

void DualModeController::HciChangeConnectionPacketType(packets::PacketView<true> args) {
  CHECK(args.size() == 4) << __func__ << " size=" << args.size();
  auto args_itr = args.begin();
  uint16_t handle = args_itr.extract<uint16_t>();
  uint16_t packet_type = args_itr.extract<uint16_t>();

  hci::Status status = link_layer_controller_.ChangeConnectionPacketType(handle, packet_type);

  SendCommandStatus(status, OpCode::CHANGE_CONNECTION_PACKET_TYPE);
}

void DualModeController::HciWriteLeHostSupport(packets::PacketView<true> args) {
  CHECK(args.size() == 2) << __func__ << " size=" << args.size();
  SendCommandCompleteSuccess(OpCode::WRITE_LE_HOST_SUPPORT);
}

void DualModeController::HciSetEventMask(packets::PacketView<true> args) {
  CHECK(args.size() == 8) << __func__ << " size=" << args.size();
  SendCommandCompleteSuccess(OpCode::SET_EVENT_MASK);
}

void DualModeController::HciWriteInquiryMode(packets::PacketView<true> args) {
  CHECK(args.size() == 1) << __func__ << " size=" << args.size();
  link_layer_controller_.SetInquiryMode(args[0]);
  SendCommandCompleteSuccess(OpCode::WRITE_INQUIRY_MODE);
}

void DualModeController::HciWritePageScanType(packets::PacketView<true> args) {
  CHECK(args.size() == 1) << __func__ << " size=" << args.size();
  SendCommandCompleteSuccess(OpCode::WRITE_PAGE_SCAN_TYPE);
}

void DualModeController::HciWriteInquiryScanType(packets::PacketView<true> args) {
  CHECK(args.size() == 1) << __func__ << " size=" << args.size();
  SendCommandCompleteSuccess(OpCode::WRITE_INQUIRY_SCAN_TYPE);
}

void DualModeController::HciAuthenticationRequested(packets::PacketView<true> args) {
  CHECK(args.size() == 2) << __func__ << " size=" << args.size();
  uint16_t handle = args.begin().extract<uint16_t>();
  hci::Status status = link_layer_controller_.AuthenticationRequested(handle);

  SendCommandStatus(status, OpCode::AUTHENTICATION_REQUESTED);
}

void DualModeController::HciSetConnectionEncryption(packets::PacketView<true> args) {
  CHECK(args.size() == 3) << __func__ << " size=" << args.size();
  auto args_itr = args.begin();
  uint16_t handle = args_itr.extract<uint16_t>();
  uint8_t encryption_enable = args_itr.extract<uint8_t>();
  hci::Status status = link_layer_controller_.SetConnectionEncryption(handle, encryption_enable);

  SendCommandStatus(status, OpCode::SET_CONNECTION_ENCRYPTION);
}

void DualModeController::HciWriteAuthenticationEnable(packets::PacketView<true> args) {
  CHECK(args.size() == 1) << __func__ << " size=" << args.size();
  properties_.SetAuthenticationEnable(args[0]);
  SendCommandCompleteSuccess(OpCode::WRITE_AUTHENTICATION_ENABLE);
}

void DualModeController::HciReadAuthenticationEnable(packets::PacketView<true> args) {
  CHECK(args.size() == 0) << __func__ << " size=" << args.size();
  std::shared_ptr<packets::EventPacketBuilder> command_complete =
      packets::EventPacketBuilder::CreateCommandCompleteReadAuthenticationEnable(hci::Status::SUCCESS,
                                                                                 properties_.GetAuthenticationEnable());
  send_event_(command_complete->ToVector());
}

void DualModeController::HciWriteClassOfDevice(packets::PacketView<true> args) {
  CHECK(args.size() == 3) << __func__ << " size=" << args.size();
  properties_.SetClassOfDevice(args[0], args[1], args[2]);
  SendCommandCompleteSuccess(OpCode::WRITE_CLASS_OF_DEVICE);
}

void DualModeController::HciWritePageTimeout(packets::PacketView<true> args) {
  CHECK(args.size() == 2) << __func__ << " size=" << args.size();
  SendCommandCompleteSuccess(OpCode::WRITE_PAGE_TIMEOUT);
}

void DualModeController::HciWriteDefaultLinkPolicySettings(packets::PacketView<true> args) {
  CHECK(args.size() == 2) << __func__ << " size=" << args.size();
  SendCommandCompleteSuccess(OpCode::WRITE_DEFAULT_LINK_POLICY_SETTINGS);
}

void DualModeController::HciWriteLinkPolicySettings(packets::PacketView<true> args) {
  CHECK(args.size() == 4) << __func__ << " size=" << args.size();

  auto args_itr = args.begin();
  uint16_t handle = args_itr.extract<uint16_t>();
  uint16_t settings = args_itr.extract<uint16_t>();

  hci::Status status = link_layer_controller_.WriteLinkPolicySettings(handle, settings);

  send_event_(packets::EventPacketBuilder::CreateCommandCompleteWriteLinkPolicySettings(status, handle)->ToVector());
}

void DualModeController::HciWriteLinkSupervisionTimeout(packets::PacketView<true> args) {
  CHECK(args.size() == 4) << __func__ << " size=" << args.size();

  auto args_itr = args.begin();
  uint16_t handle = args_itr.extract<uint16_t>();
  uint16_t timeout = args_itr.extract<uint16_t>();

  hci::Status status = link_layer_controller_.WriteLinkSupervisionTimeout(handle, timeout);

  send_event_(
      packets::EventPacketBuilder::CreateCommandCompleteWriteLinkSupervisionTimeout(status, handle)->ToVector());
}

void DualModeController::HciReadLocalName(packets::PacketView<true> args) {
  CHECK(args.size() == 0) << __func__ << " size=" << args.size();
  std::shared_ptr<packets::EventPacketBuilder> command_complete =
      packets::EventPacketBuilder::CreateCommandCompleteReadLocalName(hci::Status::SUCCESS, properties_.GetName());
  send_event_(command_complete->ToVector());
}

void DualModeController::HciWriteLocalName(packets::PacketView<true> args) {
  CHECK(args.size() == 248) << __func__ << " size=" << args.size();
  std::vector<uint8_t> clipped(args.begin(), args.begin() + LastNonZero(args) + 1);
  properties_.SetName(clipped);
  SendCommandCompleteSuccess(OpCode::WRITE_LOCAL_NAME);
}

void DualModeController::HciWriteExtendedInquiryResponse(packets::PacketView<true> args) {
  CHECK(args.size() == 241) << __func__ << " size=" << args.size();
  // Strip FEC byte and trailing zeros
  std::vector<uint8_t> clipped(args.begin() + 1, args.begin() + LastNonZero(args) + 1);
  properties_.SetExtendedInquiryData(clipped);
  LOG_WARN(LOG_TAG, "Write EIR Inquiry - Size = %d (%d)", static_cast<int>(properties_.GetExtendedInquiryData().size()),
           static_cast<int>(clipped.size()));
  SendCommandCompleteSuccess(OpCode::WRITE_EXTENDED_INQUIRY_RESPONSE);
}

void DualModeController::HciWriteVoiceSetting(packets::PacketView<true> args) {
  CHECK(args.size() == 2) << __func__ << " size=" << args.size();
  SendCommandCompleteSuccess(OpCode::WRITE_VOICE_SETTING);
}

void DualModeController::HciWriteCurrentIacLap(packets::PacketView<true> args) {
  CHECK(args.size() > 0);
  CHECK(args.size() == 1 + (3 * args[0]));  // count + 3-byte IACs

  SendCommandCompleteSuccess(OpCode::WRITE_CURRENT_IAC_LAP);
}

void DualModeController::HciWriteInquiryScanActivity(packets::PacketView<true> args) {
  CHECK(args.size() == 4) << __func__ << " size=" << args.size();
  SendCommandCompleteSuccess(OpCode::WRITE_INQUIRY_SCAN_ACTIVITY);
}

void DualModeController::HciWriteScanEnable(packets::PacketView<true> args) {
  CHECK(args.size() == 1) << __func__ << " size=" << args.size();
  link_layer_controller_.SetInquiryScanEnable(args[0] & 0x1);
  link_layer_controller_.SetPageScanEnable(args[0] & 0x2);
  SendCommandCompleteSuccess(OpCode::WRITE_SCAN_ENABLE);
}

void DualModeController::HciSetEventFilter(packets::PacketView<true> args) {
  CHECK(args.size() > 0);
  SendCommandCompleteSuccess(OpCode::SET_EVENT_FILTER);
}

void DualModeController::HciInquiry(packets::PacketView<true> args) {
  CHECK(args.size() == 5) << __func__ << " size=" << args.size();
  link_layer_controller_.SetInquiryLAP(args[0] | (args[1] << 8) | (args[2] << 16));
  link_layer_controller_.SetInquiryMaxResponses(args[4]);
  link_layer_controller_.StartInquiry(std::chrono::milliseconds(args[3] * 1280));

  SendCommandStatusSuccess(OpCode::INQUIRY);
}

void DualModeController::HciInquiryCancel(packets::PacketView<true> args) {
  CHECK(args.size() == 0) << __func__ << " size=" << args.size();
  link_layer_controller_.InquiryCancel();
  SendCommandCompleteSuccess(OpCode::INQUIRY_CANCEL);
}

void DualModeController::HciAcceptConnectionRequest(packets::PacketView<true> args) {
  CHECK(args.size() == 7) << __func__ << " size=" << args.size();
  Address addr = args.begin().extract<Address>();
  bool try_role_switch = args[6] == 0;
  hci::Status status = link_layer_controller_.AcceptConnectionRequest(addr, try_role_switch);
  SendCommandStatus(status, OpCode::ACCEPT_CONNECTION_REQUEST);
}

void DualModeController::HciRejectConnectionRequest(packets::PacketView<true> args) {
  CHECK(args.size() == 7) << __func__ << " size=" << args.size();
  auto args_itr = args.begin();
  Address addr = args_itr.extract<Address>();
  uint8_t reason = args_itr.extract<uint8_t>();
  hci::Status status = link_layer_controller_.RejectConnectionRequest(addr, reason);
  SendCommandStatus(status, OpCode::REJECT_CONNECTION_REQUEST);
}

void DualModeController::HciLinkKeyRequestReply(packets::PacketView<true> args) {
  CHECK(args.size() == 22) << __func__ << " size=" << args.size();
  Address addr = args.begin().extract<Address>();
  packets::PacketView<true> key = args.SubViewLittleEndian(6, 22);
  hci::Status status = link_layer_controller_.LinkKeyRequestReply(addr, key);
  send_event_(packets::EventPacketBuilder::CreateCommandCompleteLinkKeyRequestReply(status, addr)->ToVector());
}

void DualModeController::HciLinkKeyRequestNegativeReply(packets::PacketView<true> args) {
  CHECK(args.size() == 6) << __func__ << " size=" << args.size();
  Address addr = args.begin().extract<Address>();
  hci::Status status = link_layer_controller_.LinkKeyRequestNegativeReply(addr);
  send_event_(packets::EventPacketBuilder::CreateCommandCompleteLinkKeyRequestNegativeReply(status, addr)->ToVector());
}

void DualModeController::HciDeleteStoredLinkKey(packets::PacketView<true> args) {
  CHECK(args.size() == 7) << __func__ << " size=" << args.size();

  uint16_t deleted_keys = 0;

  if (args[6] == 0) {
    Address addr = args.begin().extract<Address>();
    deleted_keys = security_manager_.DeleteKey(addr);
  }

  if (args[6] == 1) {
    security_manager_.DeleteAllKeys();
  }

  send_event_(packets::EventPacketBuilder::CreateCommandCompleteDeleteStoredLinkKey(hci::Status::SUCCESS, deleted_keys)
                  ->ToVector());
}

void DualModeController::HciRemoteNameRequest(packets::PacketView<true> args) {
  CHECK(args.size() == 10) << __func__ << " size=" << args.size();

  Address remote_addr = args.begin().extract<Address>();

  hci::Status status =
      link_layer_controller_.SendCommandToRemoteByAddress(OpCode::REMOTE_NAME_REQUEST, args, remote_addr, false);

  SendCommandStatus(status, OpCode::REMOTE_NAME_REQUEST);
}

void DualModeController::HciLeSetEventMask(packets::PacketView<true> args) {
  CHECK(args.size() == 8) << __func__ << " size=" << args.size();
  /*
    uint64_t mask = args.begin().extract<uint64_t>();
    link_layer_controller_.SetLeEventMask(mask);
  */
  SendCommandCompleteSuccess(OpCode::LE_SET_EVENT_MASK);
}

void DualModeController::HciLeReadBufferSize(packets::PacketView<true> args) {
  CHECK(args.size() == 0) << __func__ << " size=" << args.size();
  std::shared_ptr<packets::EventPacketBuilder> command_complete =
      packets::EventPacketBuilder::CreateCommandCompleteLeReadBufferSize(
          hci::Status::SUCCESS, properties_.GetLeDataPacketLength(), properties_.GetTotalNumLeDataPackets());
  send_event_(command_complete->ToVector());
}

void DualModeController::HciLeReadLocalSupportedFeatures(packets::PacketView<true> args) {
  CHECK(args.size() == 0) << __func__ << " size=" << args.size();
  std::shared_ptr<packets::EventPacketBuilder> command_complete =
      packets::EventPacketBuilder::CreateCommandCompleteLeReadLocalSupportedFeatures(
          hci::Status::SUCCESS, properties_.GetLeSupportedFeatures());
  send_event_(command_complete->ToVector());
}

void DualModeController::HciLeSetRandomAddress(packets::PacketView<true> args) {
  CHECK(args.size() == 6) << __func__ << " size=" << args.size();
  properties_.SetLeAddress(args.begin().extract<Address>());
  SendCommandCompleteSuccess(OpCode::LE_SET_RANDOM_ADDRESS);
}

void DualModeController::HciLeSetAdvertisingParameters(packets::PacketView<true> args) {
  CHECK(args.size() == 15) << __func__ << " size=" << args.size();

  SendCommandCompleteSuccess(OpCode::LE_SET_ADVERTISING_PARAMETERS);
}

void DualModeController::HciLeSetAdvertisingData(packets::PacketView<true> args) {
  CHECK(args.size() > 0);
  SendCommandCompleteSuccess(OpCode::LE_SET_ADVERTISING_DATA);
}

void DualModeController::HciLeSetScanParameters(packets::PacketView<true> args) {
  CHECK(args.size() == 7) << __func__ << " size=" << args.size();
  link_layer_controller_.SetLeScanType(args[0]);
  link_layer_controller_.SetLeScanInterval(args[1] | (args[2] << 8));
  link_layer_controller_.SetLeScanWindow(args[3] | (args[4] << 8));
  link_layer_controller_.SetLeAddressType(args[5]);
  link_layer_controller_.SetLeScanFilterPolicy(args[6]);
  SendCommandCompleteSuccess(OpCode::LE_SET_SCAN_PARAMETERS);
}

void DualModeController::HciLeSetScanEnable(packets::PacketView<true> args) {
  CHECK(args.size() == 2) << __func__ << " size=" << args.size();
  LOG_INFO(LOG_TAG, "SetScanEnable: %d %d", args[0], args[1]);
  link_layer_controller_.SetLeScanEnable(args[0]);
  link_layer_controller_.SetLeFilterDuplicates(args[1]);
  SendCommandCompleteSuccess(OpCode::LE_SET_SCAN_ENABLE);
}

void DualModeController::HciLeCreateConnection(packets::PacketView<true> args) {
  CHECK(args.size() == 25) << __func__ << " size=" << args.size();
  auto args_itr = args.begin();
  link_layer_controller_.SetLeScanInterval(args_itr.extract<uint16_t>());
  link_layer_controller_.SetLeScanWindow(args_itr.extract<uint16_t>());
  uint8_t initiator_filter_policy = args_itr.extract<uint8_t>();
  link_layer_controller_.SetLeInitiatorFilterPolicy(initiator_filter_policy);

  if (initiator_filter_policy == 0) {  // White list not used
    uint8_t peer_address_type = args_itr.extract<uint8_t>();
    Address peer_address = args_itr.extract<Address>();
    link_layer_controller_.SetLePeerAddressType(peer_address_type);
    link_layer_controller_.SetLePeerAddress(peer_address);
  }
  link_layer_controller_.SetLeAddressType(args_itr.extract<uint8_t>());
  link_layer_controller_.SetLeConnectionIntervalMin(args_itr.extract<uint16_t>());
  link_layer_controller_.SetLeConnectionIntervalMax(args_itr.extract<uint16_t>());
  link_layer_controller_.SetLeConnectionLatency(args_itr.extract<uint16_t>());
  link_layer_controller_.SetLeSupervisionTimeout(args_itr.extract<uint16_t>());
  link_layer_controller_.SetLeMinimumCeLength(args_itr.extract<uint16_t>());
  link_layer_controller_.SetLeMaximumCeLength(args_itr.extract<uint16_t>());

  hci::Status status = link_layer_controller_.SetLeConnect(true);

  SendCommandStatus(status, OpCode::LE_CREATE_CONNECTION);
}

void DualModeController::HciLeConnectionUpdate(packets::PacketView<true> args) {
  CHECK(args.size() == 14) << __func__ << " size=" << args.size();

  SendCommandStatus(hci::Status::CONNECTION_REJECTED_UNACCEPTABLE_BD_ADDR, OpCode::LE_CONNECTION_UPDATE);

  send_event_(packets::EventPacketBuilder::CreateLeConnectionUpdateCompleteEvent(hci::Status::SUCCESS, 0x0002, 0x0006,
                                                                                 0x0000, 0x01f4)
                  ->ToVector());
}

void DualModeController::HciCreateConnection(packets::PacketView<true> args) {
  CHECK(args.size() == 13) << __func__ << " size=" << args.size();

  auto args_itr = args.begin();
  Address address = args_itr.extract<Address>();
  uint16_t packet_type = args_itr.extract<uint16_t>();
  uint8_t page_scan_mode = args_itr.extract<uint8_t>();
  uint16_t clock_offset = args_itr.extract<uint16_t>();
  uint8_t allow_role_switch = args_itr.extract<uint8_t>();

  hci::Status status =
      link_layer_controller_.CreateConnection(address, packet_type, page_scan_mode, clock_offset, allow_role_switch);

  SendCommandStatus(status, OpCode::CREATE_CONNECTION);
}

void DualModeController::HciDisconnect(packets::PacketView<true> args) {
  CHECK(args.size() == 3) << __func__ << " size=" << args.size();

  auto args_itr = args.begin();
  uint16_t handle = args_itr.extract<uint16_t>();
  uint8_t reason = args_itr.extract<uint8_t>();

  hci::Status status = link_layer_controller_.Disconnect(handle, reason);

  SendCommandStatus(status, OpCode::DISCONNECT);
}

void DualModeController::HciLeConnectionCancel(packets::PacketView<true> args) {
  CHECK(args.size() == 0) << __func__ << " size=" << args.size();
  link_layer_controller_.SetLeConnect(false);
  SendCommandStatusSuccess(OpCode::LE_CREATE_CONNECTION_CANCEL);
  /* For testing Jakub's patch:  Figure out a neat way to call this without
     recompiling.  I'm thinking about a bad device. */
  /*
  SendCommandCompleteOnlyStatus(OpCode::LE_CREATE_CONNECTION_CANCEL,
                                Status::ERR_COMMAND_DISALLOWED);
  */
}

void DualModeController::HciLeReadWhiteListSize(packets::PacketView<true> args) {
  CHECK(args.size() == 0) << __func__ << " size=" << args.size();
  std::shared_ptr<packets::EventPacketBuilder> command_complete =
      packets::EventPacketBuilder::CreateCommandCompleteLeReadWhiteListSize(hci::Status::SUCCESS,
                                                                            properties_.GetLeWhiteListSize());
  send_event_(command_complete->ToVector());
}

void DualModeController::HciLeClearWhiteList(packets::PacketView<true> args) {
  CHECK(args.size() == 0) << __func__ << " size=" << args.size();
  link_layer_controller_.LeWhiteListClear();
  SendCommandCompleteSuccess(OpCode::LE_CLEAR_WHITE_LIST);
}

void DualModeController::HciLeAddDeviceToWhiteList(packets::PacketView<true> args) {
  CHECK(args.size() == 7) << __func__ << " size=" << args.size();

  if (link_layer_controller_.LeWhiteListFull()) {
    SendCommandCompleteOnlyStatus(OpCode::LE_ADD_DEVICE_TO_WHITE_LIST, hci::Status::MEMORY_CAPACITY_EXCEEDED);
    return;
  }
  auto args_itr = args.begin();
  uint8_t addr_type = args_itr.extract<uint8_t>();
  Address address = args_itr.extract<Address>();
  link_layer_controller_.LeWhiteListAddDevice(address, addr_type);
  SendCommandCompleteSuccess(OpCode::LE_ADD_DEVICE_TO_WHITE_LIST);
}

void DualModeController::HciLeRemoveDeviceFromWhiteList(packets::PacketView<true> args) {
  CHECK(args.size() == 7) << __func__ << " size=" << args.size();

  auto args_itr = args.begin();
  uint8_t addr_type = args_itr.extract<uint8_t>();
  Address address = args_itr.extract<Address>();
  link_layer_controller_.LeWhiteListRemoveDevice(address, addr_type);
  SendCommandCompleteSuccess(OpCode::LE_REMOVE_DEVICE_FROM_WHITE_LIST);
}

/*
void DualModeController::HciLeReadRemoteUsedFeaturesRsp(uint16_t handle,
                                                        uint64_t features) {
  std::shared_ptr<packets::EventPacketBuilder> event =
      packets::EventPacketBuilder::CreateLeRemoteUsedFeaturesEvent(
          hci::Status::SUCCESS, handle, features);
  send_event_(event->ToVector());
}
*/

void DualModeController::HciLeReadRemoteFeatures(packets::PacketView<true> args) {
  CHECK(args.size() == 2) << __func__ << " size=" << args.size();

  uint16_t handle = args.begin().extract<uint16_t>();

  hci::Status status =
      link_layer_controller_.SendCommandToRemoteByHandle(OpCode::LE_READ_REMOTE_FEATURES, args, handle);

  SendCommandStatus(status, OpCode::LE_READ_REMOTE_FEATURES);
}

void DualModeController::HciLeRand(packets::PacketView<true> args) {
  CHECK(args.size() == 0) << __func__ << " size=" << args.size();
  uint64_t random_val = 0;
  for (size_t rand_bytes = 0; rand_bytes < sizeof(uint64_t); rand_bytes += sizeof(RAND_MAX)) {
    random_val = (random_val << (8 * sizeof(RAND_MAX))) | random();
  }
  std::shared_ptr<packets::EventPacketBuilder> command_complete =
      packets::EventPacketBuilder::CreateCommandCompleteLeRand(hci::Status::SUCCESS, random_val);
  send_event_(command_complete->ToVector());
}

void DualModeController::HciLeReadSupportedStates(packets::PacketView<true> args) {
  CHECK(args.size() == 0) << __func__ << " size=" << args.size();
  std::shared_ptr<packets::EventPacketBuilder> command_complete =
      packets::EventPacketBuilder::CreateCommandCompleteLeReadSupportedStates(hci::Status::SUCCESS,
                                                                              properties_.GetLeSupportedStates());
  send_event_(command_complete->ToVector());
}

void DualModeController::HciLeVendorCap(packets::PacketView<true> args) {
  CHECK(args.size() == 0) << __func__ << " size=" << args.size();
  vector<uint8_t> caps = properties_.GetLeVendorCap();
  if (caps.size() == 0) {
    SendCommandCompleteOnlyStatus(OpCode::LE_GET_VENDOR_CAPABILITIES, hci::Status::UNKNOWN_COMMAND);
    return;
  }

  std::shared_ptr<packets::EventPacketBuilder> command_complete =
      packets::EventPacketBuilder::CreateCommandCompleteLeGetVendorCapabilities(hci::Status::SUCCESS,
                                                                                properties_.GetLeVendorCap());
  send_event_(command_complete->ToVector());
}

void DualModeController::HciLeVendorMultiAdv(packets::PacketView<true> args) {
  CHECK(args.size() > 0);
  SendCommandCompleteOnlyStatus(OpCode::LE_MULTI_ADVT, hci::Status::UNKNOWN_COMMAND);
}

void DualModeController::HciLeAdvertisingFilter(packets::PacketView<true> args) {
  CHECK(args.size() > 0);
  SendCommandCompleteOnlyStatus(OpCode::LE_ADV_FILTER, hci::Status::UNKNOWN_COMMAND);
}

void DualModeController::HciLeEnergyInfo(packets::PacketView<true> args) {
  CHECK(args.size() > 0);
  SendCommandCompleteOnlyStatus(OpCode::LE_ENERGY_INFO, hci::Status::UNKNOWN_COMMAND);
}

void DualModeController::HciLeExtendedScanParams(packets::PacketView<true> args) {
  CHECK(args.size() > 0);
  SendCommandCompleteOnlyStatus(OpCode::LE_EXTENDED_SCAN_PARAMS, hci::Status::UNKNOWN_COMMAND);
}

void DualModeController::HciLeStartEncryption(packets::PacketView<true> args) {
  CHECK(args.size() == 28) << __func__ << " size=" << args.size();

  auto args_itr = args.begin();
  uint16_t handle = args_itr.extract<uint16_t>();
  // uint64_t random_number = args_itr.extract<uint64_t>();
  // uint16_t encrypted_diversifier = args_itr.extract<uint16_t>();
  // std::vector<uint8_t> long_term_key;
  // for (size_t i = 0; i < 16; i++) {
  //   long_term_key.push_back(args_itr.extract<uint18_t>();
  // }
  SendCommandStatus(hci::Status::SUCCESS, OpCode::LE_START_ENCRYPTION);

  send_event_(packets::EventPacketBuilder::CreateEncryptionChange(hci::Status::SUCCESS, handle, 0x01)->ToVector());
#if 0

  std::shared_ptr<packets::AclPacketBuilder> encryption_information =
      std::make_shared<packets::AclPacketBuilder>(
          0x0002, Acl::FIRST_AUTOMATICALLY_FLUSHABLE, Acl::POINT_TO_POINT,
          std::vector<uint8_t>({}));

  encryption_information->AddPayloadOctets2(0x0011);
  encryption_information->AddPayloadOctets2(0x0006);
  encryption_information->AddPayloadOctets1(0x06);
  encryption_information->AddPayloadOctets8(0x0706050403020100);
  encryption_information->AddPayloadOctets8(0x0F0E0D0C0B0A0908);

  send_acl_(encryption_information);

  encryption_information = std::make_shared<packets::AclPacketBuilder>(
      0x0002, Acl::FIRST_AUTOMATICALLY_FLUSHABLE, Acl::POINT_TO_POINT,
      std::vector<uint8_t>({}));

  encryption_information->AddPayloadOctets2(0x000B);
  encryption_information->AddPayloadOctets2(0x0006);
  encryption_information->AddPayloadOctets1(0x07);
  encryption_information->AddPayloadOctets2(0xBEEF);
  encryption_information->AddPayloadOctets8(0x0706050403020100);

  send_acl_(encryption_information);

  encryption_information = std::make_shared<packets::AclPacketBuilder>(
      0x0002, Acl::FIRST_AUTOMATICALLY_FLUSHABLE, Acl::POINT_TO_POINT,
      std::vector<uint8_t>({}));

  encryption_information->AddPayloadOctets2(0x0011);
  encryption_information->AddPayloadOctets2(0x0006);
  encryption_information->AddPayloadOctets1(0x08);
  encryption_information->AddPayloadOctets8(0x0F0E0D0C0B0A0908);
  encryption_information->AddPayloadOctets8(0x0706050403020100);

  send_acl_(encryption_information);

  encryption_information = std::make_shared<packets::AclPacketBuilder>(
      0x0002, Acl::FIRST_AUTOMATICALLY_FLUSHABLE, Acl::POINT_TO_POINT,
      std::vector<uint8_t>({}));

  encryption_information->AddPayloadOctets2(0x0008);
  encryption_information->AddPayloadOctets2(0x0006);
  encryption_information->AddPayloadOctets1(0x09);
  encryption_information->AddPayloadOctets1(0x01);
  encryption_information->AddPayloadOctets6(0xDEADBEEFF00D);
  send_acl_(encryption_information);
  // send_event_(packets::EventPacketBuilder::CreateLeStartEncryption()->ToVector());

#endif
}

void DualModeController::HciReadLoopbackMode(packets::PacketView<true> args) {
  CHECK(args.size() == 0) << __func__ << " size=" << args.size();
  send_event_(packets::EventPacketBuilder::CreateCommandCompleteReadLoopbackMode(hci::Status::SUCCESS, loopback_mode_)
                  ->ToVector());
}

void DualModeController::HciWriteLoopbackMode(packets::PacketView<true> args) {
  CHECK(args.size() == 1) << __func__ << " size=" << args.size();
  loopback_mode_ = static_cast<hci::LoopbackMode>(args[0]);
  // ACL channel
  uint16_t acl_handle = 0x123;
  send_event_(packets::EventPacketBuilder::CreateConnectionCompleteEvent(
                  hci::Status::SUCCESS, acl_handle, properties_.GetAddress(), hci::LinkType::ACL, false)
                  ->ToVector());
  // SCO channel
  uint16_t sco_handle = 0x345;
  send_event_(packets::EventPacketBuilder::CreateConnectionCompleteEvent(
                  hci::Status::SUCCESS, sco_handle, properties_.GetAddress(), hci::LinkType::SCO, false)
                  ->ToVector());
  SendCommandCompleteSuccess(OpCode::WRITE_LOOPBACK_MODE);
}

}  // namespace test_vendor_lib
