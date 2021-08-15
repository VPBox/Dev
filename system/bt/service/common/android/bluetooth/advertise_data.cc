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

#include "android/bluetooth/advertise_data.h"

using android::OK;

namespace android {
namespace bluetooth {

status_t AdvertiseData::writeToParcel(Parcel* parcel) const {
  status_t status = parcel->writeByteVector(data_);
  return status;
}

status_t AdvertiseData::readFromParcel(const Parcel* parcel) {
  status_t status = parcel->readByteVector(&data_);
  return status;
}

}  // namespace bluetooth
}  // namespace android
