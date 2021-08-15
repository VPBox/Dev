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
#include <vector>

#include "include/hci.h"
#include "packets/hci/hci_packet_builder.h"
#include "packets/packet_builder.h"

namespace test_vendor_lib {
namespace packets {

// ACL data packets are specified in the Bluetooth Core Specification Version
// 4.2, Volume 2, Part E, Section 5.4.2
class CommandPacketBuilder : public HciPacketBuilder {
 public:
  virtual ~CommandPacketBuilder() override = default;

  static std::unique_ptr<CommandPacketBuilder> Create(hci::OpCode opcode, std::unique_ptr<BasePacketBuilder> payload);

  virtual size_t size() const override;

  virtual void Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const;

 private:
  CommandPacketBuilder(hci::OpCode opcode, std::unique_ptr<BasePacketBuilder> payload);
  CommandPacketBuilder() = delete;
  hci::OpCode opcode_;
  std::unique_ptr<BasePacketBuilder> payload_;
};

}  // namespace packets
}  // namespace test_vendor_lib
