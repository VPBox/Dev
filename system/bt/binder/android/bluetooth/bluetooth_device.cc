//
//  Copyright 2017, The Android Open Source Project
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at:
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include "android/bluetooth/bluetooth_device.h"

#include <utils/String16.h>

using android::OK;
using android::Parcel;
using android::status_t;
using android::String16;
using android::String8;

namespace android {
namespace bluetooth {

status_t BluetoothDevice::writeToParcel(Parcel* parcel) const {
  status_t status = parcel->writeString16(String16(address.ToString().c_str()));
  return status;
}

status_t BluetoothDevice::readFromParcel(const Parcel* parcel) {
  String16 tmp;

  status_t status = parcel->readString16(&tmp);
  if (status != OK) return status;

  RawAddress::FromString(String8(tmp).string(), address);
  return OK;
}

}  // namespace bluetooth
}  // namespace android