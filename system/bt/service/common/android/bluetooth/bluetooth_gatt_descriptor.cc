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

#include "android/bluetooth/bluetooth_gatt_descriptor.h"
#include "android/bluetooth/uuid.h"

#include <utils/String16.h>
#include <utils/String8.h>

using android::OK;
using android::String8;
using android::String16;

namespace android {
namespace bluetooth {

status_t BluetoothGattDescriptor::writeToParcel(Parcel* parcel) const {
  status_t status = parcel->writeParcelable((UUID)uuid_);
  if (status != OK) return status;

  status = parcel->writeInt32(handle_);
  if (status != OK) return status;

  status = parcel->writeInt32(permissions_);
  return status;
}

status_t BluetoothGattDescriptor::readFromParcel(const Parcel* parcel) {
  UUID uuid;
  status_t status = parcel->readParcelable(&uuid);
  if (status != OK) return status;
  uuid_ = uuid.uuid;

  int32_t tmp;
  status = parcel->readInt32(&tmp);
  if (status != OK) return status;
  handle_ = tmp;

  status = parcel->readInt32(&tmp);
  if (status != OK) return status;
  permissions_ = tmp;

  return status;
}

}  // namespace bluetooth
}  // namespace android
