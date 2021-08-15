/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef WIFICOND_SCANNING_RADIO_CHAIN_INFO_H_
#define WIFICOND_SCANNING_RADIO_CHAIN_INFO_H_

#include <binder/Parcel.h>
#include <binder/Parcelable.h>

namespace com {
namespace android {
namespace server {
namespace wifi {
namespace wificond {

class RadioChainInfo : public ::android::Parcelable {
 public:
  RadioChainInfo(int32_t chain_id, int32_t level)
      : chain_id(chain_id), level(level) {}
  RadioChainInfo()
      : chain_id(0), level(0) {}
  bool operator==(const RadioChainInfo& rhs) const {
    return chain_id == rhs.chain_id && level == rhs.level;
  }
  ::android::status_t writeToParcel(::android::Parcel* parcel) const override;
  ::android::status_t readFromParcel(const ::android::Parcel* parcel) override;

  int32_t chain_id;
  int32_t level;
};

}  // namespace wificond
}  // namespace wifi
}  // namespace server
}  // namespace android
}  // namespace com

#endif  // WIFICOND_SCANNING_RADIO_CHAIN_INFO_H_
