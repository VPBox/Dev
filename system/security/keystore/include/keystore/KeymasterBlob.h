// Copyright 2017 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef KEYSTORE_INCLUDE_KEYSTORE_KEYMASTERBLOB_H_
#define KEYSTORE_INCLUDE_KEYSTORE_KEYMASTERBLOB_H_

#include <binder/Parcelable.h>

namespace android {
namespace security {
namespace keymaster {

// Parcelable which wraps hardware::hidl_vec<uint8_t>
struct KeymasterBlob : public ::android::Parcelable {
    KeymasterBlob(){};
    explicit KeymasterBlob(hardware::hidl_vec<uint8_t> data) : data_(data) {}
    status_t readFromParcel(const Parcel* in) override;
    status_t writeToParcel(Parcel* out) const override;
    const hardware::hidl_vec<uint8_t>& getData() const { return data_; }

  private:
    hardware::hidl_vec<uint8_t> data_;
};

}  // namespace keymaster
}  // namespace security
}  // namespace android

#endif  // KEYSTORE_INCLUDE_KEYSTORE_KEYMASTERBLOB_H_
