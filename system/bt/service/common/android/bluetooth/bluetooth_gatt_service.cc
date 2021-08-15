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

#include "android/bluetooth/bluetooth_gatt_service.h"
#include "android/bluetooth/bluetooth_gatt_characteristic.h"
#include "android/bluetooth/uuid.h"

#include <utils/String16.h>
#include <utils/String8.h>

using android::OK;
using android::String8;
using android::String16;

namespace android {
namespace bluetooth {

status_t BluetoothGattService::writeToParcel(Parcel* parcel) const {
  status_t status = parcel->writeInt32(handle_);
  if (status != OK) return status;

  status = parcel->writeBool(primary_);
  if (status != OK) return status;

  status = parcel->writeParcelable((UUID)uuid_);
  if (status != OK) return status;

  std::vector<BluetoothGattCharacteristic> characteristics;
  for (const auto& chrc : characteristics_) characteristics.push_back(chrc);

  status = parcel->writeParcelableVector(characteristics);

  std::vector<BluetoothGattIncludedService> includedServices;
  for (const auto& service : included_services_)
    includedServices.push_back(service);

  status = parcel->writeParcelableVector(includedServices);

  return status;
}

status_t BluetoothGattService::readFromParcel(const Parcel* parcel) {
  int32_t tmp;
  status_t status = parcel->readInt32(&tmp);
  if (status != OK) return status;
  handle_ = tmp;

  status = parcel->readBool(&primary_);
  if (status != OK) return status;

  UUID uuid;
  status = parcel->readParcelable(&uuid);
  if (status != OK) return status;
  uuid_ = uuid.uuid;

  std::vector<BluetoothGattCharacteristic> characteristics;
  status = parcel->readParcelableVector(&characteristics);
  if (status != OK) return status;

  for (const auto& chrc : characteristics) characteristics_.push_back(chrc);

  std::vector<BluetoothGattIncludedService> includedServices;
  status = parcel->readParcelableVector(&includedServices);
  if (status != OK) return status;

  for (const auto& srvc : includedServices)
    included_services_.push_back(BluetoothGattService(srvc));

  return status;
}

}  // namespace bluetooth
}  // namespace android
