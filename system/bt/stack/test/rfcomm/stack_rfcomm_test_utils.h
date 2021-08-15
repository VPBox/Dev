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
#pragma once

#include <vector>

#include <gmock/gmock.h>

#include "bt_types.h"

namespace bluetooth {
namespace rfcomm {

class RfcommCallback {
 public:
  virtual void PortManagementCallback(uint32_t code, uint16_t port_handle,
                                      uint16_t callback_index) = 0;
  virtual void PortEventCallback(uint32_t code, uint16_t port_handle,
                                 uint16_t callback_index) = 0;
  virtual ~RfcommCallback() = default;
};

class MockRfcommCallback : public RfcommCallback {
 public:
  MOCK_METHOD3(PortManagementCallback, void(uint32_t code, uint16_t port_handle,
                                            uint16_t callback_index));
  MOCK_METHOD3(PortEventCallback, void(uint32_t code, uint16_t port_handle,
                                       uint16_t callback_index));
};

/**
 * Create DLCI using direction of service channel number
 *
 * @param on_originator_side is this a channel on initiator side
 * @param scn service channel number
 * @return DLCI
 */
uint8_t GetDlci(bool on_originator_side, uint8_t scn);

/**
 * Create address field in RFCOMM packet
 *
 * @param ea end of field byte, true if field only has 1 byte, false otherwise
 * @param cr command and response bit, true if packet is from initiator,
 *           false otherwise, not actual "command" and "response"
 * @param dlci DLCI of this pcaket, RFCOMM_MX_DLCI for multiplexer control
 * @return address field
 */
uint8_t GetAddressField(bool ea, bool cr, uint8_t dlci);

/**
 * Create control field in RFCOMM packet
 *
 * @param pf Poll/Finish bit, normally 1 for multiplexer control channel
 * @param frame_type frame type
 * @return control field
 */
uint8_t GetControlField(bool pf, uint8_t frame_type);

/**
 * Create Multiplexer control channel parameter negotiation frame
 *
 * @param dlci DLCI to be configured
 * @param i_bits i bits
 * @param cl_bits cl bits
 * @param priority priority
 * @param timer_value timer value
 * @param rfcomm_mtu rfcomm mtu
 * @param max_num_retransmission maximum number of retransmission
 * @param err_recovery_window_k error recovery window k
 * @return vector of bytes of this frame
 */
std::vector<uint8_t> CreateMccPnFrame(uint8_t dlci, uint8_t i_bits,
                                      uint8_t cl_bits, uint8_t priority,
                                      uint8_t timer_value, uint16_t rfcomm_mtu,
                                      uint8_t max_num_retransmission,
                                      uint8_t err_recovery_window_k);
/**
 * Create Multiplexer Control Modem Status Configuration Frame
 *
 * @param dlci DLCI to be configured
 * @param fc flow control
 * @param rtc ready to communicate
 * @param rtr ready to receive
 * @param ic incoming call indicator
 * @param dv is data valid
 * @return vector of bytes
 */
std::vector<uint8_t> CreateMccMscFrame(uint8_t dlci, bool fc, bool rtc,
                                       bool rtr, bool ic, bool dv);

/**
 * Create Multiplexer Control Frame
 *
 * @param command_type type of command
 * @param cr command or response flag, true when this is a command, false when
 *           this is a response, regardless of connection direction
 * @param data frame data
 * @return vector of bytes
 */
std::vector<uint8_t> CreateMultiplexerControlFrame(
    uint8_t command_type, bool cr, const std::vector<uint8_t>& data);

/**
 * Create a general RFCOMM packet
 *
 * @param address address byte
 * @param control control byte
 * @param credits number of credits, <= 0 will cause this to be ignored
 * @param data frame data
 * @return vector of bytes
 */
std::vector<uint8_t> CreateRfcommPacket(uint8_t address, uint8_t control,
                                        int credits,
                                        const std::vector<uint8_t>& data);
/*
 * Various shortcut for getting frequently used packets
 */

/**
 * Create SABM packet that is used to connect to a service channel number in a
 * multiplexer
 *
 * @param dlci DLCI to be connected
 * @param l2cap_lcid L2CAP channel ID
 * @param acl_handle ACL handle
 * @return vector of bytes of unwrapped ACL packet
 */
std::vector<uint8_t> CreateQuickSabmPacket(uint8_t dlci, uint16_t l2cap_lcid,
                                           uint16_t acl_handle);

/**
 * Create UA packet that is used to acknowledge service channel connection
 *
 * @param dlci DLCI to be connected
 * @param l2cap_lcid L2CAP channel ID
 * @param acl_handle ACL handle
 * @return vector of bytes of unwrapped ACL packet
 */
std::vector<uint8_t> CreateQuickUaPacket(uint8_t dlci, uint16_t l2cap_lcid,
                                         uint16_t acl_handle);

/**
 * Create parameter negotiation packet used to setup parameters for a DLCI
 *
 * @param rfc_cr RFCOMM command/response bit, true of initiator
 * @param target_dlci DLCI to be configured
 * @param mx_cr multiplexer command or reponse, regardless of initiator
 * @param rfc_mtu RFCOMM mtu to be used for DLCI
 * @param cl CL bit
 * @param priority prirority
 * @param k error recovery window k
 * @param l2cap_lcid L2CAP channel ID
 * @param acl_handle ACL handle
 * @return vector of bytes of unwrapped ACL packet
 */
std::vector<uint8_t> CreateQuickPnPacket(bool rfc_cr, uint8_t target_dlci,
                                         bool mx_cr, uint16_t rfc_mtu,
                                         uint8_t cl, uint8_t priority,
                                         uint8_t k, uint16_t l2cap_lcid,
                                         uint16_t acl_handle);

/**
 * Create modem signal control packet
 *
 * @param rfc_cr RFCOMM command/response bit, true of initiator
 * @param dlci DLCI to be configured
 * @param l2cap_lcid L2CAP channel ID
 * @param acl_handle ACL handle
 * @param mx_cr multiplexer command or reponse, regardless of initiator
 * @param fc flow control
 * @param rtc ready to communicate
 * @param rtr ready to receive
 * @param ic incoming call indicator
 * @param dv data valid
 * @return vector of bytes of unwrapped ACL packet
 */
std::vector<uint8_t> CreateQuickMscPacket(bool rfc_cr, uint8_t dlci,
                                          uint16_t l2cap_lcid,
                                          uint16_t acl_handle, bool mx_cr,
                                          bool fc, bool rtc, bool rtr, bool ic,
                                          bool dv);

/**
 * Create a quick RFCOMM data packet
 *
 * @param dlci DLCI of this packet
 * @param cr command or control, true for initiator
 * @param l2cap_lcid L2CAP channel ID
 * @param acl_handle ACL handle
 * @param credits number of credits
 * @param data data bytes
 * @return vector of bytes of unwrapped ACL packet
 */
std::vector<uint8_t> CreateQuickDataPacket(uint8_t dlci, bool cr,
                                           uint16_t l2cap_lcid,
                                           uint16_t acl_handle, int credits,
                                           const std::vector<uint8_t>& data);

/**
 * Create a quick RFCOMM data packet
 *
 * @param dlci DLCI of this packet
 * @param cr command or control, true for initiator
 * @param l2cap_lcid L2CAP channel ID
 * @param acl_handle ACL handle
 * @param credits number of credits
 * @param str message in string format
 * @return vector of bytes of unwrapped ACL packet
 */
std::vector<uint8_t> CreateQuickDataPacket(uint8_t dlci, bool cr,
                                           uint16_t l2cap_lcid,
                                           uint16_t acl_handle, int credits,
                                           const std::string& str);

}  // namespace rfcomm
}  // namespace bluetooth