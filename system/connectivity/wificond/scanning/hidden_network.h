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

#ifndef WIFICOND_SCANNING_HIDDEN_NETWORK_H_
#define WIFICOND_SCANNING_HIDDEN_NETWORK_H_

#include <vector>

#include <binder/Parcel.h>
#include <binder/Parcelable.h>

namespace com {
namespace android {
namespace server {
namespace wifi {
namespace wificond {

class HiddenNetwork : public ::android::Parcelable {
 public:
  HiddenNetwork() = default;
  bool operator==(const HiddenNetwork& rhs) const {
    return ssid_ == rhs.ssid_;
  }
  ::android::status_t writeToParcel(::android::Parcel* parcel) const override;
  ::android::status_t readFromParcel(const ::android::Parcel* parcel) override;

  std::vector<uint8_t> ssid_;
};

}  // namespace wificond
}  // namespace wifi
}  // namespace server
}  // namespace android
}  // namespace com

#endif  // WIFICOND_SCANNING_HIDDEN_NETWORK_H_
