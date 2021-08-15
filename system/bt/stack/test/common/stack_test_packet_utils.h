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

#include "bt_types.h"

namespace bluetooth {

/**
 * Create L2CAP data packet
 *
 * @param lcid
 * @param data
 * @return vector of bytes
 */
std::vector<uint8_t> CreateL2capDataPacket(uint16_t lcid,
                                           const std::vector<uint8_t>& data);

/**
 * Create ACL data packet
 *
 * @param handle ACL connection hanle
 * @param pb pb byte
 * @param bc bc byte
 * @param data frame data
 * @return vector of bytes
 */
std::vector<uint8_t> CreateAclPacket(uint16_t handle, uint8_t pb, uint8_t bc,
                                     const std::vector<uint8_t>& data);

/**
 * Given an array of ACL packet bytes from BTSNOOP log, allocate an OSI
 * allocated BT_HDR pointer to a packet that can be processed by L2CAP
 * application layer
 *
 * Note: BT_HDR offset is configured for incoming packets
 *
 * @param acl_packet_bytes pointer to array of ACL packet bytes
 * @param buffer_length length of the packet buffer
 * @return BT_HDR pointer to an OSI heap allocated packet
 */
BT_HDR* AllocateWrappedIncomingL2capAclPacket(const uint8_t* acl_packet_bytes,
                                              size_t buffer_length);
BT_HDR* AllocateWrappedIncomingL2capAclPacket(
    const std::vector<uint8_t>& buffer);

/**
 * Given an array of ACL packet bytes from BTSNOOP log, allocate an OSI
 * allocated BT_HDR pointer to a packet that can be processed by L2CAP
 * application layer
 *
 * Note: BT_HDR offset is configured for outgoing packets
 *
 * @param acl_packet_bytes pointer to array of ACL packet bytes
 * @param buffer_length length of the packet buffer
 * @return BT_HDR pointer to an OSI heap allocated packet
 */
BT_HDR* AllocateWrappedOutgoingL2capAclPacket(const uint8_t* acl_packet_bytes,
                                              size_t buffer_length);
BT_HDR* AllocateWrappedOutgoingL2capAclPacket(
    const std::vector<uint8_t>& buffer);

}  // namespace bluetooth
