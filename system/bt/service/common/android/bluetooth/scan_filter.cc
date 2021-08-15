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

#include "android/bluetooth/scan_filter.h"
#include "android/bluetooth/uuid.h"

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

status_t ScanFilter::writeToParcel(Parcel* parcel) const {
  status_t status =
      parcel->writeString16(String16(String8(device_name_.c_str())));
  if (status != OK) return status;

  status = parcel->writeString16(String16(String8(device_address_.c_str())));
  if (status != OK) return status;

  // TODO(jpawlowski) make type casting nicer
  // uuid won't really keep ownership, it's just for type casting
  std::unique_ptr<UUID> uuid;
  UUID tmp;

  if (service_uuid_) {
    tmp = *service_uuid_;
    uuid.reset(&tmp);
  } else {
    uuid.reset(nullptr);
  }
  status = parcel->writeNullableParcelable(uuid);
  uuid.release();
  if (status != OK) return status;

  if (service_uuid_mask_) {
    tmp = *service_uuid_mask_;
    uuid.reset(&tmp);
  } else {
    uuid.reset(nullptr);
  }
  status = parcel->writeNullableParcelable(uuid);
  uuid.release();

  return status;
}

status_t ScanFilter::readFromParcel(const Parcel* parcel) {
  String16 name;
  status_t status = parcel->readString16(&name);
  if (status != OK) return status;
  device_name_ = std::string(String8(name).string());

  String16 addr;
  status = parcel->readString16(&addr);
  if (status != OK) return status;
  device_address_ = std::string(String8(addr).string());

  UUID uuid;
  status = parcel->readParcelable(&uuid);
  if (status != OK) return status;
  service_uuid_.reset(new ::bluetooth::Uuid(uuid.uuid));

  status = parcel->readParcelable(&uuid);
  if (status != OK) return status;
  service_uuid_mask_.reset(new ::bluetooth::Uuid(uuid.uuid));

  return status;
}

}  // namespace bluetooth
}  // namespace android
