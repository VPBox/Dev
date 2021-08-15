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

#include "android/bluetooth/bluetooth_a2dp_codec_config.h"

#include <utils/String16.h>
#include <utils/String8.h>

using android::OK;
using android::String16;
using android::String8;

namespace android {
namespace bluetooth {

status_t BluetoothA2dpCodecConfig::writeToParcel(Parcel* parcel) const {
  status_t status = parcel->writeInt32(codec_type_);
  if (status != OK) return status;

  status = parcel->writeInt32(codec_priority_);
  if (status != OK) return status;

  status = parcel->writeInt32(sample_rate_);
  if (status != OK) return status;

  status = parcel->writeInt32(bits_per_sample_);
  if (status != OK) return status;

  status = parcel->writeInt32(channel_mode_);
  if (status != OK) return status;

  status = parcel->writeInt64(codec_specific_1_);
  if (status != OK) return status;

  status = parcel->writeInt64(codec_specific_2_);
  if (status != OK) return status;

  status = parcel->writeInt64(codec_specific_3_);
  if (status != OK) return status;

  status = parcel->writeInt64(codec_specific_4_);
  if (status != OK) return status;

  return status;
}

status_t BluetoothA2dpCodecConfig::readFromParcel(const Parcel* parcel) {
  int32_t tmp;
  status_t status = parcel->readInt32(&tmp);
  if (status != OK) return status;
  codec_type_ = tmp;

  status = parcel->readInt32(&tmp);
  if (status != OK) return status;
  codec_priority_ = tmp;

  status = parcel->readInt32(&tmp);
  if (status != OK) return status;
  sample_rate_ = tmp;

  status = parcel->readInt32(&tmp);
  if (status != OK) return status;
  bits_per_sample_ = tmp;

  status = parcel->readInt32(&tmp);
  if (status != OK) return status;
  channel_mode_ = tmp;

  int64_t tmp64;
  status = parcel->readInt64(&tmp64);
  if (status != OK) return status;
  codec_specific_1_ = tmp64;

  status = parcel->readInt64(&tmp64);
  if (status != OK) return status;
  codec_specific_2_ = tmp64;

  status = parcel->readInt64(&tmp64);
  if (status != OK) return status;
  codec_specific_3_ = tmp64;

  status = parcel->readInt64(&tmp64);
  if (status != OK) return status;
  codec_specific_4_ = tmp64;

  return status;
}

}  // namespace bluetooth
}  // namespace android
