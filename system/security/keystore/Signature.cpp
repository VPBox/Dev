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

#include "include/keystore/Signature.h"

#include <binder/Parcel.h>

namespace android {
namespace content {
namespace pm {

status_t Signature::writeToParcel(Parcel* parcel) const {
    return parcel->writeByteVector(sig_data_);
}

status_t Signature::readFromParcel(const Parcel* parcel) {
    return parcel->readByteVector(&sig_data_);
}

Signature::Signature(std::vector<uint8_t> signature_data) : sig_data_(std::move(signature_data)) {}

}  // namespace pm
}  // namespace content
}  // namespace android
