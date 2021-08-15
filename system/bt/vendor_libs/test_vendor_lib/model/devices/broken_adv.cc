/*
 * Copyright 2016 The Android Open Source Project
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

#define LOG_TAG "broken_adv"

#include "broken_adv.h"

#include "model/setup/device_boutique.h"
#include "osi/include/log.h"

using std::vector;

namespace test_vendor_lib {

bool BrokenAdv::registered_ = DeviceBoutique::Register(LOG_TAG, &BrokenAdv::Create);

BrokenAdv::BrokenAdv() {
  advertising_interval_ms_ = std::chrono::milliseconds(1280);
  properties_.SetLeAdvertisementType(BTM_BLE_NON_CONNECT_EVT);
  constant_adv_data_ = {
      0x02,  // Length
      BTM_BLE_AD_TYPE_FLAG,
      BTM_BLE_BREDR_NOT_SPT | BTM_BLE_GEN_DISC_FLAG,
      0x13,  // Length
      BTM_BLE_AD_TYPE_NAME_CMPL,
      'g',
      'D',
      'e',
      'v',
      'i',
      'c',
      'e',
      '-',
      'b',
      'r',
      'o',
      'k',
      'e',
      'n',
      '_',
      'a',
      'd',
      'v',
  };
  properties_.SetLeAdvertisement(constant_adv_data_);

  properties_.SetLeScanResponse({0x0b,  // Length
                                 BTM_BLE_AD_TYPE_NAME_SHORT, 'b', 'r', 'o', 'k', 'e', 'n', 'n', 'e', 's', 's'});

  properties_.SetExtendedInquiryData({0x07,  // Length
                                      BT_EIR_COMPLETE_LOCAL_NAME_TYPE, 'B', 'R', '0', 'K', '3', 'N'});
  properties_.SetPageScanRepetitionMode(0);
  page_scan_delay_ms_ = std::chrono::milliseconds(600);
}

void BrokenAdv::Initialize(const vector<std::string>& args) {
  if (args.size() < 2) return;

  Address addr;
  if (Address::FromString(args[1], addr)) properties_.SetLeAddress(addr);

  if (args.size() < 3) return;

  SetAdvertisementInterval(std::chrono::milliseconds(std::stoi(args[2])));
}

// Mostly return the correct length
static uint8_t random_length(size_t bytes_remaining) {
  uint32_t randomness = rand();

  switch ((randomness & 0xf000000) >> 24) {
    case (0):
      return bytes_remaining + (randomness & 0xff);
    case (1):
      return bytes_remaining - (randomness & 0xff);
    case (2):
      return bytes_remaining + (randomness & 0xf);
    case (3):
      return bytes_remaining - (randomness & 0xf);
    case (5):
    case (6):
      return bytes_remaining + (randomness & 0x3);
    case (7):
    case (8):
      return bytes_remaining - (randomness & 0x3);
    default:
      return bytes_remaining;
  }
}

static size_t random_adv_type() {
  uint32_t randomness = rand();

  switch ((randomness & 0xf000000) >> 24) {
    case (0):
      return BTM_EIR_MANUFACTURER_SPECIFIC_TYPE;
    case (1):
      return (randomness & 0xff);
    default:
      return (randomness & 0x1f);
  }
}

static size_t random_data_length(size_t length, size_t bytes_remaining) {
  uint32_t randomness = rand();

  switch ((randomness & 0xf000000) >> 24) {
    case (0):
      return bytes_remaining;
    case (1):
      return (length + (randomness & 0xff)) % bytes_remaining;
    default:
      return (length <= bytes_remaining ? length : bytes_remaining);
  }
}

static void RandomizeAdvertisement(vector<uint8_t>& ad, size_t max) {
  uint8_t length = random_length(max);
  uint8_t data_length = random_data_length(length, max);

  ad.push_back(random_adv_type());
  ad.push_back(length);
  for (size_t i = 0; i < data_length; i++) ad.push_back(rand() & 0xff);
}

void BrokenAdv::UpdateAdvertisement() {
  std::vector<uint8_t> adv_data;
  for (size_t i = 0; i < constant_adv_data_.size(); i++) adv_data.push_back(constant_adv_data_[i]);

  RandomizeAdvertisement(adv_data, 31 - adv_data.size());
  properties_.SetLeAdvertisement(adv_data);

  adv_data.clear();
  RandomizeAdvertisement(adv_data, 31);
  properties_.SetLeScanResponse(adv_data);

  Address le_addr = properties_.GetLeAddress();
  uint8_t* low_order_byte = (uint8_t*)(&le_addr);
  *low_order_byte += 1;
  properties_.SetLeAddress(le_addr);
}

std::string BrokenAdv::ToString() const {
  std::string str =
      Device::ToString() + std::string(": Interval = ") + std::to_string(advertising_interval_ms_.count());
  return str;
}

void BrokenAdv::UpdatePageScan() {
  RandomizeAdvertisement(constant_scan_data_, 31);

  Address page_addr = properties_.GetAddress();
  uint8_t* low_order_byte = (uint8_t*)(&page_addr);
  *low_order_byte += 1;
  properties_.SetAddress(page_addr);
}

void BrokenAdv::TimerTick() {
  UpdatePageScan();
  UpdateAdvertisement();
}

}  // namespace test_vendor_lib
