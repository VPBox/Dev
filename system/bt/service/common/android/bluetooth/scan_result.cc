//
//  Copyright 2015 Google, Inc.
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

#include "android/bluetooth/scan_result.h"

#include <base/logging.h>
#include <binder/Parcel.h>
#include <utils/String16.h>
#include <utils/String8.h>

using android::Parcelable;
using android::Parcel;
using android::String8;
using android::String16;
using android::status_t;
using android::OK;

namespace android {
namespace bluetooth {

status_t ScanResult::writeToParcel(Parcel* parcel) const {
  status_t status =
      parcel->writeString16(String16(String8(device_address_.c_str())));
  if (status != OK) return status;

  status = parcel->writeByteVector(scan_record_);
  if (status != OK) return status;

  status = parcel->writeInt32(rssi_);
  return status;
}

status_t ScanResult::readFromParcel(const Parcel* parcel) {
  String16 addr;
  status_t status = parcel->readString16(&addr);
  if (status != OK) return status;
  device_address_ = std::string(String8(addr).string());

  status = parcel->readByteVector(&scan_record_);
  if (status != OK) return status;

  status = parcel->readInt32(&rssi_);
  return status;
}

}  // namespace bluetooth
}  // namespace android
