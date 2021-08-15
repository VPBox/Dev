/*
**
** Copyright 2018, The Android Open Source Project
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

#include <binder/Parcel.h>
#include <keystore/keymaster_types.h>
#include <utility>
#include <utils/String16.h>

#include "include/keystore/KeystoreResponse.h"

namespace android {
namespace security {
namespace keystore {

status_t KeystoreResponse::readFromParcel(const Parcel* in) {
    auto rc = in->readInt32(&response_code_);
    if (rc != NO_ERROR) return rc;
    return in->readString16(&error_msg_);
}

status_t KeystoreResponse::writeToParcel(Parcel* out) const {
    auto rc = out->writeInt32(response_code_);
    if (rc != NO_ERROR) return rc;
    return out->writeString16(error_msg_);
}

}  // namespace keystore
}  // namespace security
}  // namespace android
