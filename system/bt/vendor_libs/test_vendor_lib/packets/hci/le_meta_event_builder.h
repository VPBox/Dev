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
#include <string>
#include <vector>

#include "include/hci.h"
#include "include/le_advertisement.h"
#include "packets/counted_builder.h"
#include "packets/hci/hci_packet_builder.h"
#include "packets/packet_builder.h"
#include "packets/packet_view.h"
#include "packets/raw_builder.h"
#include "types/address.h"

namespace test_vendor_lib {
namespace packets {

// LE Meta Event Packets are specified in the Bluetooth Core Specification
// Version 4.2, Volume 2, Part E, Section 7.7.65. The first byte is the
// Subevent_Code.
class LeMetaEventBuilder : public RawBuilder {
 public:
  virtual ~LeMetaEventBuilder() override = default;

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section
  // 7.7.65.1
  static std::unique_ptr<LeMetaEventBuilder> CreateLeConnectionCompleteEvent(hci::Status status, uint16_t handle,
                                                                             uint8_t role, uint8_t peer_address_type,
                                                                             const Address& peer, uint16_t interval,
                                                                             uint16_t latency,
                                                                             uint16_t supervision_timeout);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section
  // 7.7.65.2
  static std::unique_ptr<LeMetaEventBuilder> CreateLeAdvertisingReportEvent();

  // Returns true if the report can be added to the event packet.
  bool AddLeAdvertisingReport(LeAdvertisement::AdvertisementType event_type, LeAdvertisement::AddressType addr_type,
                              const Address& addr, const std::vector<uint8_t>& data, uint8_t rssi);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section
  // 7.7.65.3
  static std::unique_ptr<LeMetaEventBuilder> CreateLeConnectionUpdateCompleteEvent(hci::Status status, uint16_t handle,
                                                                                   uint16_t interval, uint16_t latency,
                                                                                   uint16_t supervision_timeout);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section
  // 7.7.65.4
  static std::unique_ptr<LeMetaEventBuilder> CreateLeRemoteUsedFeaturesEvent(hci::Status status, uint16_t handle,
                                                                             uint64_t features);

  // Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section
  // 7.7.65.10
  static std::unique_ptr<LeMetaEventBuilder> CreateLeEnhancedConnectionCompleteEvent(
      hci::Status status, uint16_t handle, uint8_t role, uint8_t peer_address_type, const Address& peer,
      const Address& local_private_address, const Address& peer_private_address, uint16_t interval, uint16_t latency,
      uint16_t supervision_timeout);

  virtual size_t size() const override;

  virtual void Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const override;

  bool AddBuilder(std::unique_ptr<BasePacketBuilder> builder);

 private:
  explicit LeMetaEventBuilder(hci::LeSubEventCode sub_event_code);
  explicit LeMetaEventBuilder(hci::LeSubEventCode sub_event_code, std::unique_ptr<RawBuilder> payload);
  hci::LeSubEventCode sub_event_code_;
  std::unique_ptr<RawBuilder> payload_;
};

}  // namespace packets
}  // namespace test_vendor_lib
