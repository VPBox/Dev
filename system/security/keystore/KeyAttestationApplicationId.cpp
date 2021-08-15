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

#include "include/keystore/KeyAttestationApplicationId.h"

#include <binder/Parcel.h>

namespace android {
namespace security {
namespace keymaster {

KeyAttestationApplicationId::KeyAttestationApplicationId() = default;

KeyAttestationApplicationId::KeyAttestationApplicationId(
        std::unique_ptr<KeyAttestationPackageInfo> package) :
    packageInfos_(new std::vector<std::unique_ptr<KeyAttestationPackageInfo>>()) {
    packageInfos_->push_back(std::move(package));
}

KeyAttestationApplicationId::KeyAttestationApplicationId(PackageInfoVector packages)
    : packageInfos_(std::make_shared<PackageInfoVector>(std::move(packages))) {}

status_t KeyAttestationApplicationId::writeToParcel(Parcel* parcel) const {
    return parcel->writeParcelableVector(packageInfos_);
}

status_t KeyAttestationApplicationId::readFromParcel(const Parcel* parcel) {
    std::unique_ptr<std::vector<std::unique_ptr<KeyAttestationPackageInfo>>> temp_vector;
    auto rc = parcel->readParcelableVector(&temp_vector);
    if (rc != NO_ERROR) return rc;
    packageInfos_.reset(temp_vector.release());
    return NO_ERROR;
}

}  // namespace keymaster
}  // namespace security
}  // namespace android
