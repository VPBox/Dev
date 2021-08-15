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

#pragma once

#include "android/bluetooth/bluetooth_gatt_included_service.h"
#include "bluetooth/service.h"

#include <binder/Parcel.h>
#include <binder/Parcelable.h>

using android::Parcel;
using android::Parcelable;
using android::status_t;

namespace android {
namespace bluetooth {

class BluetoothGattService : public Parcelable, public ::bluetooth::Service {
 public:
  BluetoothGattService() = default;
  BluetoothGattService(const ::bluetooth::Service& service)
      : ::bluetooth::Service(service){};  // NOLINT(implicit)
  BluetoothGattService(
      const BluetoothGattIncludedService& includedService)  // NOLINT(implicit)
      : ::bluetooth::Service(includedService.handle(),
                             includedService.primary(), includedService.uuid(),
                             {}, {}){};
  ~BluetoothGattService() = default;

  // Write |this| parcelable to the given |parcel|.  Keep in mind that
  // implementations of writeToParcel must be manually kept in sync
  // with readFromParcel and the Java equivalent versions of these methods.
  //
  // Returns android::OK on success and an appropriate error otherwise.
  status_t writeToParcel(Parcel* parcel) const override;

  // Read data from the given |parcel| into |this|.  After readFromParcel
  // completes, |this| should have equivalent state to the object that
  // wrote itself to the parcel.
  //
  // Returns android::OK on success and an appropriate error otherwise.
  status_t readFromParcel(const Parcel* parcel) override;
};
}  // namespace bluetooth
}  // namespace android
