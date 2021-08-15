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

#ifndef KEYSTORE_INCLUDE_KEYSTORE_KEYCHARACTERISTICS_H_
#define KEYSTORE_INCLUDE_KEYSTORE_KEYCHARACTERISTICS_H_

#include <binder/Parcelable.h>

#include "KeymasterArguments.h"
#include "keymaster_types.h"

namespace android {
namespace security {
namespace keymaster {

// Parcelable version of keystore::KeyCharacteristics
struct KeyCharacteristics : public ::android::Parcelable {
    KeyCharacteristics(){};
    explicit KeyCharacteristics(::keystore::KeyCharacteristics&& other) {
        softwareEnforced = std::move(other.softwareEnforced);
        hardwareEnforced = std::move(other.hardwareEnforced);
    }
    explicit KeyCharacteristics(const ::keystore::KeyCharacteristics& other) {
        softwareEnforced = KeymasterArguments(other.softwareEnforced);
        hardwareEnforced = KeymasterArguments(other.hardwareEnforced);
    }
    status_t readFromParcel(const Parcel* in) override;
    status_t writeToParcel(Parcel* out) const override;

    KeymasterArguments softwareEnforced;
    KeymasterArguments hardwareEnforced;
};

}  // namespace keymaster
}  // namespace security
}  // namespace android

#endif  // KEYSTORE_INCLUDE_KEYSTORE_KEYCHARACTERISTICS_H_
