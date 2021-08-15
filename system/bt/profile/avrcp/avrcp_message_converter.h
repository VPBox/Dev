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

#include <iostream>
#include <vector>

#include "packet/avrcp/avrcp_packet.h"

// These classes are temporary placeholders to easily switch between BT_HDR and
// packets.
class VectorPacket : public ::bluetooth::Packet {
 public:
  using Packet::Packet;  // Inherit constructors

  static std::shared_ptr<VectorPacket> Make() {
    return std::shared_ptr<VectorPacket>(new VectorPacket());
  };

  static std::shared_ptr<VectorPacket> Make(std::vector<uint8_t> payload) {
    auto pkt = VectorPacket::Make();
    pkt->packet_start_index_ = 0;
    pkt->packet_end_index_ = payload.size();
    pkt->data_ = std::make_shared<std::vector<uint8_t>>(std::move(payload));
    return pkt;
  };

  const std::vector<uint8_t>& GetData() { return *data_; };

  virtual std::string ToString() const override {
    std::stringstream ss;
    ss << "VectorPacket:" << std::endl;
    ss << "  └ Payload =";
    for (auto it = begin(); it != end(); it++) {
      ss << " " << loghex(*it);
    }
    ss << std::endl;

    return ss.str();
  };

  virtual std::pair<size_t, size_t> GetPayloadIndecies() const override {
    return std::pair<size_t, size_t>(packet_start_index_, packet_end_index_);
  }

  virtual bool IsValid() const override { return true; }
};

// TODO (apanicke): When deleting the old AVRCP Stack, remove this class and
// instead create a BT_HDR Parsing packet.
class AvrcpMessageConverter {
 public:
  static std::shared_ptr<::bluetooth::Packet> Parse(tAVRC_MSG* m) {
    std::vector<uint8_t> data;

    switch (m->hdr.opcode) {
      case AVRC_OP_VENDOR: {
        tAVRC_MSG_VENDOR* msg = (tAVRC_MSG_VENDOR*)m;
        data.push_back(m->hdr.ctype);
        data.push_back((m->hdr.subunit_type << 3) | m->hdr.subunit_id);
        data.push_back(m->hdr.opcode);
        for (int i = 2; i >= 0; i--) {
          data.push_back((uint8_t)((msg->company_id >> i * 8) & 0xff));
        }
        for (uint8_t i = 0; i < msg->vendor_len; i++) {
          data.push_back(msg->p_vendor_data[i]);
        }
      } break;
      case AVRC_OP_PASS_THRU: {
        tAVRC_MSG_PASS* msg = (tAVRC_MSG_PASS*)m;
        data.push_back(m->hdr.ctype);
        data.push_back((m->hdr.subunit_type << 3) | m->hdr.subunit_id);
        data.push_back(m->hdr.opcode);
        data.push_back((msg->state << 7) | msg->op_id);
        data.push_back(0x00);
      } break;
      case AVRC_OP_BROWSE: {
        tAVRC_MSG_BROWSE* msg = (tAVRC_MSG_BROWSE*)m;
        // The first 3 bytes are header bytes that aren't actually in AVRCP
        // packets
        for (int i = 0; i < msg->browse_len; i++) {
          data.push_back(msg->p_browse_data[i]);
        }
      } break;
      default:
        LOG(ERROR) << "Unknown opcode for AVRCP message";
        break;
    }

    return VectorPacket::Make(data);
  }
};