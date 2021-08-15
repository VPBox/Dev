//
//  Copyright (C) 2017 Google, Inc.
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

#include "android/bluetooth/bluetooth_avrcp_string_value.h"

#include <utils/String16.h>
#include <utils/String8.h>

using android::OK;
using android::String16;
using android::String8;

namespace android {
namespace bluetooth {

status_t BluetoothAvrcpStringValue::writeToParcel(Parcel* parcel) const {
  status_t status = parcel->writeInt32(id_);
  if (status != OK) return status;

  status = parcel->writeString16(String16(value_.c_str(), value_.size()));
  if (status != OK) return status;

  return status;
}

status_t BluetoothAvrcpStringValue::readFromParcel(const Parcel* parcel) {
  int32_t tmp;
  status_t status = parcel->readInt32(&tmp);
  if (status != OK) return status;
  id_ = tmp;

  String16 value;
  status = parcel->readString16(&value);
  if (status != OK) return status;
  value_ = String8(value).string();

  return status;
}

}  // namespace bluetooth
}  // namespace android
