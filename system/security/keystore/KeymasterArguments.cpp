/*
**
** Copyright 2017, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include "include/keystore/KeymasterArguments.h"
#include "keystore_aidl_hidl_marshalling_utils.h"

#include <binder/Parcel.h>

namespace android {
namespace security {
namespace keymaster {

using ::android::status_t;
status_t KeymasterArguments::readFromParcel(const android::Parcel* in) {
    data_ = keystore::readParamSetFromParcel(*in);
    return OK;
};

status_t KeymasterArguments::writeToParcel(android::Parcel* out) const {
    return keystore::writeParamSetToParcel(data_, out);
};

KeymasterArguments::KeymasterArguments(hardware::hidl_vec<keystore::KeyParameter>&& other)
    : data_(std::move(other)) {}

KeymasterArguments::KeymasterArguments(const hardware::hidl_vec<keystore::KeyParameter>& other)
    : data_(other) {}

}  // namespace keymaster
}  // namespace security
}  // namespace android
