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

#include "packet_builder.h"

#include <base/logging.h>

#include "packet.h"

namespace bluetooth {

void PacketBuilder::ReserveSpace(const std::shared_ptr<Packet>& pkt,
                                 size_t size) {
  pkt->data_->reserve(size);
}

bool PacketBuilder::AddPayloadOctets(const std::shared_ptr<Packet>& pkt,
                                     size_t octets, uint64_t value) {
  CHECK_LE(octets, sizeof(uint64_t));

  for (size_t i = 0; i < octets; i++) {
    pkt->data_->push_back(value & 0xff);
    pkt->packet_end_index_++;
    value = value >> 8;
  }

  return true;
}

}  // namespace bluetooth