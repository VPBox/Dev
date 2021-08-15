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

#include "vendor_packet.h"

namespace bluetooth {
namespace avrcp {

class RejectBuilder : public VendorPacketBuilder {
 public:
  virtual ~RejectBuilder() = default;

  static std::unique_ptr<RejectBuilder> MakeBuilder(CommandPdu pdu,
                                                    Status reason);

  virtual size_t size() const override;
  virtual bool Serialize(
      const std::shared_ptr<::bluetooth::Packet>& pkt) override;

 protected:
  Status reason_;

  RejectBuilder(CommandPdu pdu, Status reason)
      : VendorPacketBuilder(CType::REJECTED, pdu, PacketType::SINGLE),
        reason_(reason){};
};

}  // namespace avrcp
}  // namespace bluetooth