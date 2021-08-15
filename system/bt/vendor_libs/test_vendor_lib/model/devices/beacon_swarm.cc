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

#define LOG_TAG "beacon_swarm"

#include "beacon_swarm.h"

#include "model/setup/device_boutique.h"

using std::vector;

namespace test_vendor_lib {
bool BeaconSwarm::registered_ = DeviceBoutique::Register(LOG_TAG, &BeaconSwarm::Create);

BeaconSwarm::BeaconSwarm() {
  advertising_interval_ms_ = std::chrono::milliseconds(1280);
  properties_.SetLeAdvertisementType(BTM_BLE_NON_CONNECT_EVT);
  properties_.SetLeAdvertisement({
      0x15,  // Length
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
      'e',
      'a',
      'c',
      'o',
      'n',
      '_',
      's',
      'w',
      'a',
      'r',
      'm',
      0x02,  // Length
      BTM_BLE_AD_TYPE_FLAG,
      BTM_BLE_BREDR_NOT_SPT | BTM_BLE_GEN_DISC_FLAG,
  });

  properties_.SetLeScanResponse({0x06,  // Length
                                 BTM_BLE_AD_TYPE_NAME_SHORT, 'c', 'b', 'e', 'a', 'c'});
}

void BeaconSwarm::Initialize(const vector<std::string>& args) {
  if (args.size() < 2) return;

  Address addr;
  if (Address::FromString(args[1], addr)) properties_.SetLeAddress(addr);

  if (args.size() < 3) return;

  SetAdvertisementInterval(std::chrono::milliseconds(std::stoi(args[2])));
}

void BeaconSwarm::TimerTick() {
  Address beacon_addr = properties_.GetLeAddress();
  uint8_t* low_order_byte = (uint8_t*)(&beacon_addr);
  *low_order_byte += 1;
  properties_.SetLeAddress(beacon_addr);
  Beacon::TimerTick();
}

}  // namespace test_vendor_lib
