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

#include "android/bluetooth/scan_settings.h"

#include <binder/Parcel.h>

using android::Parcelable;
using android::Parcel;
using android::String16;
using android::status_t;
using android::OK;

namespace android {
namespace bluetooth {

status_t ScanSettings::writeToParcel(Parcel* parcel) const {
  status_t status = parcel->writeInt32(mode_);
  if (status != OK) return status;

  status = parcel->writeInt32(callback_type_);
  if (status != OK) return status;

  status = parcel->writeInt32(result_type_);
  if (status != OK) return status;

  status = parcel->writeInt64(report_delay_ms_.InMilliseconds());
  if (status != OK) return status;

  status = parcel->writeInt32(match_mode_);
  if (status != OK) return status;

  status = parcel->writeInt32(match_count_per_filter_);
  return status;
}

status_t ScanSettings::readFromParcel(const Parcel* parcel) {
  int value;
  status_t status = parcel->readInt32(&value);
  if (status != OK) return status;
  mode_ = static_cast<ScanSettings::Mode>(value);

  status = parcel->readInt32(&value);
  if (status != OK) return status;
  callback_type_ = static_cast<ScanSettings::CallbackType>(value);

  status = parcel->readInt32(&value);
  if (status != OK) return status;
  result_type_ = static_cast<ScanSettings::ResultType>(value);

  int64_t value64;
  status = parcel->readInt64(&value64);
  report_delay_ms_ = ::base::TimeDelta::FromMilliseconds(value64);

  status = parcel->readInt32(&value);
  if (status != OK) return status;
  match_mode_ = static_cast<ScanSettings::MatchMode>(value);

  status = parcel->readInt32(&value);
  if (status != OK) return status;
  match_count_per_filter_ = static_cast<ScanSettings::MatchCount>(value);

  return status;
}

}  // namespace bluetooth
}  // namespace android
