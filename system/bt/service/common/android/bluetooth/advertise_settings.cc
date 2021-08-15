//
//  Copyright 2016 Google, Inc.
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

#include "android/bluetooth/advertise_settings.h"

using android::OK;

namespace android {
namespace bluetooth {

status_t AdvertiseSettings::writeToParcel(Parcel* parcel) const {
  status_t status = parcel->writeInt32(mode_);
  if (status != OK) return status;

  status = parcel->writeInt32(tx_power_level_);
  if (status != OK) return status;

  status = parcel->writeInt32(connectable_);
  if (status != OK) return status;

  status = parcel->writeInt32(timeout_.InMilliseconds());
  return status;
}

status_t AdvertiseSettings::readFromParcel(const Parcel* parcel) {
  int32_t value;
  status_t status = parcel->readInt32(&value);
  if (status != OK) return status;

  mode_ = static_cast<AdvertiseSettings::Mode>(value);

  status = parcel->readInt32(&value);
  if (status != OK) return status;

  tx_power_level_ = static_cast<AdvertiseSettings::TxPowerLevel>(value);

  status = parcel->readInt32(&value);
  if (status != OK) return status;

  connectable_ = static_cast<bool>(value);

  status = parcel->readInt32(&value);
  if (status != OK) return status;

  timeout_ = ::base::TimeDelta::FromMilliseconds(value);
  return status;
}

}  // namespace bluetooth
}  // namespace android
