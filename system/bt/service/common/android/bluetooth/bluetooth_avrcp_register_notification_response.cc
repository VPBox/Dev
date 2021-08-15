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

#include "android/bluetooth/bluetooth_avrcp_register_notification_response.h"

#include <utils/String16.h>
#include <utils/String8.h>

using android::OK;
using android::String16;
using android::String8;

namespace android {
namespace bluetooth {

status_t BluetoothAvrcpRegisterNotificationResponse::writeToParcel(
    Parcel* parcel) const {
  status_t status = parcel->write(&data_, sizeof(data_));
  if (status != OK) return status;

  return status;
}

status_t BluetoothAvrcpRegisterNotificationResponse::readFromParcel(
    const Parcel* parcel) {
  status_t status = parcel->read(&data_, sizeof(data_));
  if (status != OK) return status;

  return status;
}

}  // namespace bluetooth
}  // namespace android
