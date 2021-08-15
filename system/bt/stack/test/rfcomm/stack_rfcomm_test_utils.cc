/******************************************************************************
 *
 *  Copyright 2018 The Android Open Source Project
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#include <bitset>
#include <string>
#include <vector>

#include "rfc_int.h"
#include "stack_test_packet_utils.h"

#include "stack_rfcomm_test_utils.h"

namespace bluetooth {
namespace rfcomm {

uint8_t GetDlci(bool on_originator_side, uint8_t scn) {
  return static_cast<uint8_t>((scn << 1) + (on_originator_side ? 1 : 0));
}

uint8_t GetAddressField(bool ea, bool cr, uint8_t dlci) {
  std::bitset<8> address;
  address.set(0, ea);
  // For UIH frame, cr for initiating device is 1, otherwise 0
  // Otherwise:
  //  Command: Initiator -> Responder: 1
  //  Command: Responder -> Initiator 0
  //  Response: Initiator -> Responder 0
  //  Response: Responder -> Initiator 1
  // Initiator is defined by the one who send SABM=1 command
  address.set(1, cr);
  address |= dlci << 2;
  return static_cast<uint8_t>(address.to_ulong());
}

uint8_t GetControlField(bool pf, uint8_t frame_type) {
  std::bitset<8> control;
  control |= frame_type;
  control.set(4, pf);
  return static_cast<uint8_t>(control.to_ulong());
}

uint8_t GetFrameTypeFromControlField(uint8_t control_field) {
  return static_cast<uint8_t>(control_field & ~(0b10000));
}

std::vector<uint8_t> CreateMccPnFrame(uint8_t dlci, uint8_t i_bits,
                                      uint8_t cl_bits, uint8_t priority,
                                      uint8_t timer_value, uint16_t rfcomm_mtu,
                                      uint8_t max_num_retransmission,
                                      uint8_t err_recovery_window_k) {
  // Data in little endian order
  std::vector<uint8_t> result;
  result.push_back(static_cast<uint8_t>(dlci & 0b00111111));
  result.push_back(static_cast<uint8_t>((cl_bits << 4) | (i_bits & 0x0F)));
  result.push_back(static_cast<uint8_t>(priority & 0b00111111));
  result.push_back(timer_value);
  result.push_back(static_cast<uint8_t>(rfcomm_mtu));
  result.push_back(static_cast<uint8_t>(rfcomm_mtu >> 8));
  result.push_back(max_num_retransmission);
  result.push_back(static_cast<uint8_t>(err_recovery_window_k & 0b111));
  return result;
}

std::vector<uint8_t> CreateMccMscFrame(uint8_t dlci, bool fc, bool rtc,
                                       bool rtr, bool ic, bool dv) {
  // Data in little endian order
  std::vector<uint8_t> result;
  result.push_back(static_cast<uint8_t>((dlci << 2) | 0b11));
  std::bitset<8> v24_signals;
  // EA = 1, single byte
  v24_signals.set(0, true);
  v24_signals.set(1, fc);
  v24_signals.set(2, rtc);
  v24_signals.set(3, rtr);
  v24_signals.set(6, ic);
  v24_signals.set(7, dv);
  result.push_back(static_cast<uint8_t>(v24_signals.to_ulong()));
  return result;
}

std::vector<uint8_t> CreateMultiplexerControlFrame(
    uint8_t command_type, bool cr, const std::vector<uint8_t>& data) {
  // Data in little endian order
  std::vector<uint8_t> result;
  std::bitset<8> header;
  header.set(0, true);  // EA is always 1
  header.set(1, cr);
  header |= command_type << 2;
  result.push_back(static_cast<uint8_t>(header.to_ulong()));
  // 7 bit length + EA(1)
  result.push_back(static_cast<uint8_t>((data.size() << 1) + 1));
  result.insert(result.end(), data.begin(), data.end());
  return result;
}

std::vector<uint8_t> CreateRfcommPacket(uint8_t address, uint8_t control,
                                        int credits,
                                        const std::vector<uint8_t>& data) {
  // Data in little endian order
  std::vector<uint8_t> result;
  result.push_back(address);
  result.push_back(control);
  size_t length = data.size();
  if ((length & 0b1000000) != 0) {
    // 15 bits of length in little endian order + EA(0)
    // Lower 7 bits + EA(0)
    result.push_back(static_cast<uint8_t>(length) << 1);
    // Upper 8 bits
    result.push_back(static_cast<uint8_t>(length >> 8));
  } else {
    // 7 bits of length + EA(1)
    result.push_back(static_cast<uint8_t>((length << 1) + 1));
  }
  if (credits > 0) {
    result.push_back(static_cast<uint8_t>(credits));
  }
  result.insert(result.end(), data.begin(), data.end());
  if (GetFrameTypeFromControlField(control) == RFCOMM_UIH) {
    result.push_back(rfc_calc_fcs(2, result.data()));
  } else {
    result.push_back(
        rfc_calc_fcs(static_cast<uint16_t>(result.size()), result.data()));
  }
  return result;
}

std::vector<uint8_t> CreateQuickUaPacket(uint8_t dlci, uint16_t l2cap_lcid,
                                         uint16_t acl_handle) {
  uint8_t address_field = GetAddressField(true, true, dlci);
  uint8_t control_field = GetControlField(true, RFCOMM_UA);
  std::vector<uint8_t> rfcomm_packet =
      CreateRfcommPacket(address_field, control_field, -1, {});
  std::vector<uint8_t> l2cap_packet =
      CreateL2capDataPacket(l2cap_lcid, rfcomm_packet);
  return CreateAclPacket(acl_handle, 0b10, 0b00, l2cap_packet);
}

std::vector<uint8_t> CreateQuickSabmPacket(uint8_t dlci, uint16_t l2cap_lcid,
                                           uint16_t acl_handle) {
  uint8_t address_field = GetAddressField(true, true, dlci);
  uint8_t control_field = GetControlField(true, RFCOMM_SABME);
  std::vector<uint8_t> rfcomm_packet =
      CreateRfcommPacket(address_field, control_field, -1, {});
  std::vector<uint8_t> l2cap_packet =
      CreateL2capDataPacket(l2cap_lcid, rfcomm_packet);
  return CreateAclPacket(acl_handle, 0b10, 0b00, l2cap_packet);
}

std::vector<uint8_t> CreateQuickPnPacket(bool rfc_cr, uint8_t target_dlci,
                                         bool mx_cr, uint16_t rfc_mtu,
                                         uint8_t cl, uint8_t priority,
                                         uint8_t k, uint16_t l2cap_lcid,
                                         uint16_t acl_handle) {
  uint8_t address_field = GetAddressField(true, rfc_cr, RFCOMM_MX_DLCI);
  uint8_t control_field = GetControlField(false, RFCOMM_UIH);
  std::vector<uint8_t> mcc_pn_data = CreateMccPnFrame(
      target_dlci, 0x0, cl, priority, RFCOMM_T1_DSEC, rfc_mtu, RFCOMM_N2, k);
  std::vector<uint8_t> mcc_payload =
      CreateMultiplexerControlFrame(0x20, mx_cr, mcc_pn_data);
  std::vector<uint8_t> rfcomm_packet =
      CreateRfcommPacket(address_field, control_field, -1, mcc_payload);
  std::vector<uint8_t> l2cap_packet =
      CreateL2capDataPacket(l2cap_lcid, rfcomm_packet);
  return CreateAclPacket(acl_handle, 0b10, 0b00, l2cap_packet);
}

std::vector<uint8_t> CreateQuickMscPacket(bool rfc_cr, uint8_t dlci,
                                          uint16_t l2cap_lcid,
                                          uint16_t acl_handle, bool mx_cr,
                                          bool fc, bool rtc, bool rtr, bool ic,
                                          bool dv) {
  uint8_t address_field = GetAddressField(true, rfc_cr, RFCOMM_MX_DLCI);
  uint8_t control_field = GetControlField(false, RFCOMM_UIH);
  std::vector<uint8_t> mcc_msc_data =
      CreateMccMscFrame(dlci, fc, rtc, rtr, ic, dv);
  std::vector<uint8_t> mcc_payload =
      CreateMultiplexerControlFrame(0x38, mx_cr, mcc_msc_data);
  std::vector<uint8_t> rfcomm_packet =
      CreateRfcommPacket(address_field, control_field, -1, mcc_payload);
  std::vector<uint8_t> l2cap_packet =
      CreateL2capDataPacket(l2cap_lcid, rfcomm_packet);
  return CreateAclPacket(acl_handle, 0b10, 0b00, l2cap_packet);
}

std::vector<uint8_t> CreateQuickDataPacket(uint8_t dlci, bool cr,
                                           uint16_t l2cap_lcid,
                                           uint16_t acl_handle, int credits,
                                           const std::vector<uint8_t>& data) {
  uint8_t address_field = GetAddressField(true, cr, dlci);
  uint8_t control_field =
      GetControlField(credits > 0 ? true : false, RFCOMM_UIH);
  std::vector<uint8_t> rfcomm_packet =
      CreateRfcommPacket(address_field, control_field, credits, data);
  std::vector<uint8_t> l2cap_packet =
      CreateL2capDataPacket(l2cap_lcid, rfcomm_packet);
  return CreateAclPacket(acl_handle, 0b10, 0b00, l2cap_packet);
}

std::vector<uint8_t> CreateQuickDataPacket(uint8_t dlci, bool cr,
                                           uint16_t l2cap_lcid,
                                           uint16_t acl_handle, int credits,
                                           const std::string& str) {
  std::vector<uint8_t> data(str.begin(), str.end());
  return CreateQuickDataPacket(dlci, cr, l2cap_lcid, acl_handle, credits, data);
}

}  // namespace rfcomm
}  // namespace bluetooth