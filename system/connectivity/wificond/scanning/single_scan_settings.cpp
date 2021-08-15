/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "android/net/wifi/IWifiScannerImpl.h"
#include "wificond/scanning/single_scan_settings.h"

#include <android-base/logging.h>

#include "wificond/parcelable_utils.h"

using android::net::wifi::IWifiScannerImpl;
using android::status_t;

namespace com {
namespace android {
namespace server {
namespace wifi {
namespace wificond {
bool SingleScanSettings::isValidScanType() const {
  return (scan_type_ == IWifiScannerImpl::SCAN_TYPE_LOW_SPAN ||
          scan_type_ == IWifiScannerImpl::SCAN_TYPE_LOW_POWER ||
          scan_type_ == IWifiScannerImpl::SCAN_TYPE_HIGH_ACCURACY);
}

status_t SingleScanSettings::writeToParcel(::android::Parcel* parcel) const {
  if (!isValidScanType()) {
    LOG(ERROR) << "Unexpected scan type: " << scan_type_;
    return ::android::BAD_VALUE;
  }
  RETURN_IF_FAILED(parcel->writeInt32(scan_type_));
  RETURN_IF_FAILED(parcel->writeInt32(channel_settings_.size()));
  for (const auto& channel : channel_settings_) {
    // For Java readTypedList():
    // A leading number 1 means this object is not null.
    RETURN_IF_FAILED(parcel->writeInt32(1));
    RETURN_IF_FAILED(channel.writeToParcel(parcel));
  }
  RETURN_IF_FAILED(parcel->writeInt32(hidden_networks_.size()));
  for (const auto& network : hidden_networks_) {
    // For Java readTypedList():
    // A leading number 1 means this object is not null.
    RETURN_IF_FAILED(parcel->writeInt32(1));
    RETURN_IF_FAILED(network.writeToParcel(parcel));
  }
  return ::android::OK;
}

status_t SingleScanSettings::readFromParcel(const ::android::Parcel* parcel) {
  RETURN_IF_FAILED(parcel->readInt32(&scan_type_));
  if (!isValidScanType()) {
    LOG(ERROR) << "Unexpected scan type: " << scan_type_;
    return ::android::BAD_VALUE;
  }
  int32_t num_channels = 0;
  RETURN_IF_FAILED(parcel->readInt32(&num_channels));
  // Convention used by Java side writeTypedList():
  // -1 means a null list.
  // 0 means an empty list.
  // Both are mapped to an empty vector in C++ code.
  for (int i = 0; i < num_channels; i++) {
    ChannelSettings channel;
    // From Java writeTypedList():
    // A leading number 1 means this object is not null.
    // We never expect a 0 or other values here.
    int32_t leading_number = 0;
    RETURN_IF_FAILED(parcel->readInt32(&leading_number));
    if (leading_number != 1) {
      LOG(ERROR) << "Unexpected leading number before an object: "
                 << leading_number;
      return ::android::BAD_VALUE;
    }
    RETURN_IF_FAILED(channel.readFromParcel(parcel));
    channel_settings_.push_back(channel);
  }
  int32_t num_hidden_networks = 0;
  RETURN_IF_FAILED(parcel->readInt32(&num_hidden_networks));
  // Convention used by Java side writeTypedList():
  // -1 means a null list.
  // 0 means an empty list.
  // Both are mapped to an empty vector in C++ code.
  for (int i = 0; i < num_hidden_networks; i++) {
    HiddenNetwork network;
    // From Java writeTypedList():
    // A leading number 1 means this object is not null.
    // We never expect a 0 or other values here.
    int32_t leading_number = 0;
    RETURN_IF_FAILED(parcel->readInt32(&leading_number));
    if (leading_number != 1) {
      LOG(ERROR) << "Unexpected leading number before an object: "
                 << leading_number;
      return ::android::BAD_VALUE;
    }
    RETURN_IF_FAILED(network.readFromParcel(parcel));
    hidden_networks_.push_back(network);
  }
  return ::android::OK;
}

}  // namespace wificond
}  // namespace wifi
}  // namespace server
}  // namespace android
}  // namespace com
