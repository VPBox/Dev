//
//  Copyright 2017 Google, Inc.
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

#include "android/bluetooth/bluetooth_remote_device_props.h"

#include <utils/String16.h>
#include <utils/String8.h>

#include "android/bluetooth/uuid.h"

using android::OK;
using android::String16;
using android::String8;

namespace android {
namespace bluetooth {

status_t BluetoothRemoteDeviceProps::writeToParcel(Parcel* parcel) const {
  status_t status =
      parcel->writeString16(String16(name_.c_str(), name_.size()));
  if (status != OK) return status;

  status = parcel->writeString16(String16(address_.c_str(), address_.size()));
  if (status != OK) return status;

  std::vector<UUID> uuids;
  for (const auto& uuid : service_uuids_) {
    uuids.push_back(uuid);
  }

  status = parcel->writeParcelableVector(uuids);
  if (status != OK) return status;

  status = parcel->writeInt32(device_class_);
  if (status != OK) return status;

  status = parcel->writeInt32(device_type_);
  if (status != OK) return status;

  status = parcel->writeInt32(rssi_);
  if (status != OK) return status;

  return status;
}

status_t BluetoothRemoteDeviceProps::readFromParcel(const Parcel* parcel) {
  String16 name;
  status_t status = parcel->readString16(&name);
  if (status != OK) return status;
  name_ = String8(name).string();

  String16 address;
  status = parcel->readString16(&address);
  if (status != OK) return status;
  address_ = String8(address).string();

  std::vector<UUID> uuids;
  status = parcel->readParcelableVector(&uuids);
  if (status != OK) return status;

  for (const auto& uuid : uuids) {
    service_uuids_.push_back(uuid.uuid);
  }

  status = parcel->readInt32(&device_class_);
  if (status != OK) return status;

  status = parcel->readInt32(&device_type_);
  if (status != OK) return status;

  status = parcel->readInt32(&rssi_);
  if (status != OK) return status;

  return status;
}

}  // namespace bluetooth
}  // namespace android
