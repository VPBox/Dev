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

#include "packets/hci/event_packet_builder.h"

#include <gtest/gtest.h>
#include <forward_list>
#include <memory>

#include "types/address.h"

using std::vector;

namespace {
vector<uint8_t> count = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
};

}  // namespace

namespace test_vendor_lib {
namespace packets {

class EventBuilderTest : public ::testing::Test {
 public:
  EventBuilderTest() = default;
  ~EventBuilderTest() = default;
};

TEST(EventBuilderTest, buildLeAdvertisementSmallTest) {
  LeAdvertisement::AdvertisementType adv_type = LeAdvertisement::AdvertisementType::ADV_SCAN_IND;
  LeAdvertisement::AddressType addr_type = LeAdvertisement::AddressType::RANDOM;
  std::unique_ptr<EventPacketBuilder> le_adv = EventPacketBuilder::CreateLeAdvertisingReportEvent();
  Address addr({1, 2, 3, 4, 5, 6});
  uint8_t rssi = -93;

  std::vector<uint8_t> payload({0x23});
  le_adv->AddLeAdvertisingReport(adv_type, addr_type, addr, {payload}, rssi);

  uint8_t payload_size = payload.size();
  uint8_t event_size = payload_size + sizeof(Address) + sizeof(rssi) + 5;
  std::vector<uint8_t> expected({
      0x3e,  // HCI LE Event
      event_size,
      0x02,          // LE Advertising subevent code
      0x01,          // Number of responses
      0x02,          // Event type is scannable undirected
      0x01,          // Address type is random
      0x01,          // Address
      0x02,          // Address
      0x03,          // Address
      0x04,          // Address
      0x05,          // Address
      0x06,          // Address
      payload_size,  // Length of the data
  });

  expected.push_back(payload[0]);
  expected.push_back(rssi);

  ASSERT_EQ(expected.size(), le_adv->size());
  ASSERT_EQ(expected, *le_adv->ToVector());
}

TEST(EventBuilderTest, buildLeAdvertisementTest) {
  LeAdvertisement::AdvertisementType adv_type = LeAdvertisement::AdvertisementType::ADV_SCAN_IND;
  LeAdvertisement::AddressType addr_type = LeAdvertisement::AddressType::RANDOM;
  std::unique_ptr<EventPacketBuilder> le_adv = EventPacketBuilder::CreateLeAdvertisingReportEvent();
  Address addr({1, 2, 3, 4, 5, 6});
  uint8_t rssi = -93;

  le_adv->AddLeAdvertisingReport(adv_type, addr_type, addr, count, rssi);

  uint8_t count_size = static_cast<uint8_t>(count.size());
  uint8_t event_size = count_size + sizeof(Address) + sizeof(rssi) + 5;
  std::vector<uint8_t> expected({
      0x3e,  // HCI LE Event
      event_size,
      0x02,        // LE Advertising subevent code
      0x01,        // Number of responses
      0x02,        // Event type is scannable undirected
      0x01,        // Address type is random
      0x01,        // Address
      0x02,        // Address
      0x03,        // Address
      0x04,        // Address
      0x05,        // Address
      0x06,        // Address
      count_size,  // Length of the data
  });

  for (size_t i = 0; i < count.size(); i++) {
    expected.push_back(count[i]);
  }
  expected.push_back(rssi);

  std::shared_ptr<std::vector<uint8_t>> raw_adv = le_adv->ToVector();
  ASSERT_EQ(expected, *raw_adv);
}

TEST(EventBuilderTest, buildNumberOfCompletedPackets) {
  uint16_t handle = 0x0102;
  uint16_t num_packets = 0x0304;

  std::unique_ptr<EventPacketBuilder> event =
      EventPacketBuilder::CreateNumberOfCompletedPacketsEvent(handle, num_packets);

  uint8_t number_of_handles = 1;
  uint8_t event_size = sizeof(uint8_t) + number_of_handles * 2 * sizeof(uint16_t);
  std::vector<uint8_t> expected({
      0x13,                           // HCI Number Of Completed Packets Event code
      event_size, number_of_handles,  //
      0x02, 0x01,                     // handle
      0x04, 0x03,                     // count
  });

  std::shared_ptr<std::vector<uint8_t>> raw_event = event->ToVector();
  ASSERT_EQ(expected, *raw_event);
}

TEST(EventBuilderTest, buildNumberOfCompletedPacketsMultiple) {
  uint16_t handle = 0x0102;
  uint16_t num_packets = 0x0304;
  uint16_t handle2 = 0x0506;
  uint16_t num_packets2 = 0x0708;

  std::unique_ptr<EventPacketBuilder> event =
      EventPacketBuilder::CreateNumberOfCompletedPacketsEvent(handle, num_packets);
  event->AddCompletedPackets(handle2, num_packets2);

  uint8_t number_of_handles = 2;
  uint8_t event_size = sizeof(uint8_t) + number_of_handles * 2 * sizeof(uint16_t);
  std::vector<uint8_t> expected({
      0x13,                           // HCI Number Of Completed Packets Event code
      event_size, number_of_handles,  //
      0x02, 0x01,                     // handle
      0x04, 0x03,                     // count
      0x06, 0x05,                     // handle
      0x08, 0x07,                     // count
  });

  std::shared_ptr<std::vector<uint8_t>> raw_event = event->ToVector();
  ASSERT_EQ(expected, *raw_event);
}

}  // namespace packets
}  // namespace test_vendor_lib
