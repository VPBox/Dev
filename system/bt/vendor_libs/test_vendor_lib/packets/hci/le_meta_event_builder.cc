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

#include "packets/hci/le_meta_event_builder.h"

#include <base/logging.h>

using std::vector;
using test_vendor_lib::hci::LeSubEventCode;
using test_vendor_lib::hci::Status;

namespace test_vendor_lib {
namespace packets {

LeMetaEventBuilder::LeMetaEventBuilder(LeSubEventCode sub_event_code)
    : sub_event_code_(sub_event_code), payload_(std::make_unique<RawBuilder>()) {}

LeMetaEventBuilder::LeMetaEventBuilder(LeSubEventCode sub_event_code, std::unique_ptr<RawBuilder> payload)
    : sub_event_code_(sub_event_code), payload_(std::move(payload)) {}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.65.1
std::unique_ptr<LeMetaEventBuilder> LeMetaEventBuilder::CreateLeConnectionCompleteEvent(
    Status status, uint16_t handle, uint8_t role, uint8_t peer_address_type, const Address& peer, uint16_t interval,
    uint16_t latency, uint16_t supervision_timeout) {
  std::unique_ptr<LeMetaEventBuilder> evt_ptr =
      std::unique_ptr<LeMetaEventBuilder>(new LeMetaEventBuilder(LeSubEventCode::CONNECTION_COMPLETE));

  CHECK(evt_ptr->AddOctets1(static_cast<uint8_t>(status)));
  CHECK(evt_ptr->AddOctets2(handle));
  CHECK(evt_ptr->AddOctets1(role));
  CHECK(evt_ptr->AddOctets1(peer_address_type));
  CHECK(evt_ptr->AddAddress(peer));
  CHECK(evt_ptr->AddOctets2(interval));
  CHECK(evt_ptr->AddOctets2(latency));
  CHECK(evt_ptr->AddOctets2(supervision_timeout));
  CHECK(evt_ptr->AddOctets1(0x00));  // Master Clock Accuracy (unused for master)

  return evt_ptr;
}

std::unique_ptr<LeMetaEventBuilder> LeMetaEventBuilder::CreateLeEnhancedConnectionCompleteEvent(
    Status status, uint16_t handle, uint8_t role, uint8_t peer_address_type, const Address& peer,
    const Address& local_private_address, const Address& peer_private_address, uint16_t interval, uint16_t latency,
    uint16_t supervision_timeout) {
  std::unique_ptr<LeMetaEventBuilder> evt_ptr =
      std::unique_ptr<LeMetaEventBuilder>(new LeMetaEventBuilder(LeSubEventCode::ENHANCED_CONNECTION_COMPLETE));

  CHECK(evt_ptr->AddOctets1(static_cast<uint8_t>(status)));
  CHECK(evt_ptr->AddOctets2(handle));
  CHECK(evt_ptr->AddOctets1(role));
  CHECK(evt_ptr->AddOctets1(peer_address_type));
  CHECK(evt_ptr->AddAddress(peer));
  CHECK(evt_ptr->AddAddress(local_private_address));
  CHECK(evt_ptr->AddAddress(peer_private_address));
  CHECK(evt_ptr->AddOctets2(interval));
  CHECK(evt_ptr->AddOctets2(latency));
  CHECK(evt_ptr->AddOctets2(supervision_timeout));
  CHECK(evt_ptr->AddOctets1(0x00));  // Master Clock Accuracy (unused for master)

  return evt_ptr;
}

std::unique_ptr<LeMetaEventBuilder> LeMetaEventBuilder::CreateLeConnectionUpdateCompleteEvent(
    Status status, uint16_t handle, uint16_t interval, uint16_t latency, uint16_t supervision_timeout) {
  std::unique_ptr<LeMetaEventBuilder> evt_ptr =
      std::unique_ptr<LeMetaEventBuilder>(new LeMetaEventBuilder(LeSubEventCode::CONNECTION_UPDATE_COMPLETE));

  CHECK(evt_ptr->AddOctets1(static_cast<uint8_t>(status)));
  CHECK(evt_ptr->AddOctets2(handle));
  CHECK(evt_ptr->AddOctets2(interval));
  CHECK(evt_ptr->AddOctets2(latency));
  CHECK(evt_ptr->AddOctets2(supervision_timeout));

  return evt_ptr;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.65.2
std::unique_ptr<LeMetaEventBuilder> LeMetaEventBuilder::CreateLeAdvertisingReportEvent() {
  std::unique_ptr<LeMetaEventBuilder> evt_ptr = std::unique_ptr<LeMetaEventBuilder>(
      new LeMetaEventBuilder(LeSubEventCode::ADVERTISING_REPORT, std::unique_ptr<RawBuilder>(new CountedBuilder())));

  return evt_ptr;
}

bool LeMetaEventBuilder::AddLeAdvertisingReport(LeAdvertisement::AdvertisementType event_type,
                                                LeAdvertisement::AddressType addr_type, const Address& addr,
                                                const vector<uint8_t>& data, uint8_t rssi) {
  if (!CanAddOctets(10 + data.size())) return false;

  CHECK(sub_event_code_ == LeSubEventCode::ADVERTISING_REPORT);

  std::unique_ptr<RawBuilder> ad = std::make_unique<RawBuilder>();

  CHECK(ad->AddOctets1(static_cast<uint8_t>(event_type)));
  CHECK(ad->AddOctets1(static_cast<uint8_t>(addr_type)));
  CHECK(ad->AddAddress(addr));
  CHECK(ad->AddOctets1(data.size()));
  CHECK(ad->AddOctets(data));
  CHECK(ad->AddOctets1(rssi));
  AddBuilder(std::move(ad));
  return true;
}

// Bluetooth Core Specification Version 4.2, Volume 2, Part E, Section 7.7.65.4
std::unique_ptr<LeMetaEventBuilder> LeMetaEventBuilder::CreateLeRemoteUsedFeaturesEvent(Status status, uint16_t handle,
                                                                                        uint64_t features) {
  std::unique_ptr<LeMetaEventBuilder> evt_ptr =
      std::unique_ptr<LeMetaEventBuilder>(new LeMetaEventBuilder(LeSubEventCode::READ_REMOTE_FEATURES_COMPLETE));

  CHECK(evt_ptr->AddOctets1(static_cast<uint8_t>(status)));
  CHECK(evt_ptr->AddOctets2(handle));
  CHECK(evt_ptr->AddOctets8(features));

  return evt_ptr;
}

size_t LeMetaEventBuilder::size() const {
  return 1 + payload_->size();  // Add the sub_event_code
}

void LeMetaEventBuilder::Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const {
  insert(static_cast<uint8_t>(sub_event_code_), it);
  uint8_t payload_size = size() - sizeof(uint8_t);
  CHECK(size() - sizeof(uint8_t) == static_cast<size_t>(payload_size)) << "Payload too large for an event: " << size();
  payload_->Serialize(it);
}

bool LeMetaEventBuilder::AddBuilder(std::unique_ptr<BasePacketBuilder> builder) {
  // Upcast the payload to add the next builder.
  CountedBuilder* temp_ptr = static_cast<CountedBuilder*>(payload_.get());
  temp_ptr->Add(std::move(builder));
  return true;
}
}  // namespace packets
}  // namespace test_vendor_lib
