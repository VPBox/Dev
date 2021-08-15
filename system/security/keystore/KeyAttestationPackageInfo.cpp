/*
**
** Copyright 2016, The Android Open Source Project
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

#include "include/keystore/KeyAttestationPackageInfo.h"

#include <binder/Parcel.h>

namespace android {
namespace security {
namespace keymaster {

KeyAttestationPackageInfo::KeyAttestationPackageInfo() = default;

KeyAttestationPackageInfo::KeyAttestationPackageInfo(const String16& packageName,
                                                     int64_t versionCode,
                                                     SharedSignaturesVector signatures)
    : packageName_(new String16(packageName)), versionCode_(versionCode), signatures_(signatures) {}

status_t KeyAttestationPackageInfo::writeToParcel(Parcel* parcel) const {
    auto rc = parcel->writeString16(packageName_);
    if (rc != NO_ERROR) return rc;
    rc = parcel->writeInt64(versionCode_);
    if (rc != NO_ERROR) return rc;
    return parcel->writeParcelableVector(signatures_);
}

status_t KeyAttestationPackageInfo::readFromParcel(const Parcel* parcel) {
    auto rc = parcel->readString16(&packageName_);
    if (rc != NO_ERROR) return rc;
    rc = parcel->readInt64(&versionCode_);
    if (rc != NO_ERROR) return rc;

    std::unique_ptr<SignaturesVector> temp_vector;
    rc = parcel->readParcelableVector(&temp_vector);
    if (rc != NO_ERROR) return rc;
    signatures_.reset(temp_vector.release());
    return NO_ERROR;
}

}  // namespace keymaster
}  // namespace security
}  // namespace android
