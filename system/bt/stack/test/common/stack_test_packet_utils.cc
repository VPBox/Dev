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

#include "hci_layer.h"
#include "l2c_api.h"
#include "osi/include/allocator.h"

#include "stack_test_packet_utils.h"

namespace bluetooth {

std::vector<uint8_t> CreateL2capDataPacket(uint16_t lcid,
                                           const std::vector<uint8_t>& data) {
  // Data in little endian order
  std::vector<uint8_t> result;
  auto data_size = static_cast<uint16_t>(data.size());
  result.push_back(static_cast<uint8_t>(data_size));
  result.push_back(static_cast<uint8_t>(data_size >> 8));
  result.push_back(static_cast<uint8_t>(lcid));
  result.push_back(static_cast<uint8_t>(lcid >> 8));
  result.insert(result.end(), data.begin(), data.end());
  return result;
}

std::vector<uint8_t> CreateAclPacket(uint16_t handle, uint8_t pb, uint8_t bc,
                                     const std::vector<uint8_t>& data) {
  // Data in little endian order
  std::vector<uint8_t> result;
  result.push_back(static_cast<uint8_t>(handle & 0x0F));
  uint8_t second_byte = 0;
  second_byte |= (bc << 6) & 0b11000000;
  second_byte |= (pb << 4) & 0b00110000;
  second_byte |= (handle >> 8) & 0b00001111;
  result.push_back(second_byte);
  auto data_size = static_cast<uint16_t>(data.size());
  result.push_back(static_cast<uint8_t>(data_size));
  result.push_back(static_cast<uint8_t>(data_size >> 8));
  result.insert(result.end(), data.begin(), data.end());
  return result;
}

BT_HDR* AllocateWrappedIncomingL2capAclPacket(const uint8_t* acl_packet_bytes,
                                              size_t buffer_length) {
  size_t packet_size = buffer_length + BT_HDR_SIZE;
  auto packet = reinterpret_cast<BT_HDR*>(osi_malloc(packet_size));
  // Add ACL packet overhead + L2CAP packet overhead
  packet->offset = 4 + L2CAP_PKT_OVERHEAD;
  packet->len = static_cast<uint16_t>(buffer_length - 4 - L2CAP_PKT_OVERHEAD);
  packet->layer_specific = 0;
  packet->event = MSG_HC_TO_STACK_HCI_ACL;
  memcpy(packet->data, acl_packet_bytes, buffer_length);
  return packet;
}

BT_HDR* AllocateWrappedIncomingL2capAclPacket(
    const std::vector<uint8_t>& buffer) {
  return AllocateWrappedIncomingL2capAclPacket(buffer.data(), buffer.size());
}

BT_HDR* AllocateWrappedOutgoingL2capAclPacket(const uint8_t* acl_packet_bytes,
                                              size_t buffer_length) {
  size_t acl_l2cap_header_size = 4 + L2CAP_PKT_OVERHEAD;
  CHECK_GE(L2CAP_MIN_OFFSET, static_cast<int>(acl_l2cap_header_size));
  size_t packet_size =
      BT_HDR_SIZE + L2CAP_MIN_OFFSET + buffer_length - acl_l2cap_header_size;
  auto packet = reinterpret_cast<BT_HDR*>(osi_malloc(packet_size));
  packet->offset = L2CAP_MIN_OFFSET;
  packet->len = static_cast<uint16_t>(buffer_length - acl_l2cap_header_size);
  packet->layer_specific = 0;
  packet->event = 0;
  memcpy(packet->data + packet->offset - acl_l2cap_header_size,
         acl_packet_bytes, buffer_length);
  return packet;
}

BT_HDR* AllocateWrappedOutgoingL2capAclPacket(
    const std::vector<uint8_t>& buffer) {
  return AllocateWrappedOutgoingL2capAclPacket(buffer.data(), buffer.size());
}

}  // namespace bluetooth