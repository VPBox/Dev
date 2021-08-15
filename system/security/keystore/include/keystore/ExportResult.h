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

#ifndef KEYSTORE_INCLUDE_KEYSTORE_EXPORTRESULT_H_
#define KEYSTORE_INCLUDE_KEYSTORE_EXPORTRESULT_H_

#include <binder/Parcelable.h>

#include "keystore_return_types.h"

namespace android {
namespace security {
namespace keymaster {

struct ExportResult : public ::android::Parcelable {
    ExportResult();
    ~ExportResult();
    status_t readFromParcel(const Parcel* in) override;
    status_t writeToParcel(Parcel* out) const override;

    ::keystore::KeyStoreServiceReturnCode resultCode;
    hardware::hidl_vec<uint8_t> exportData;
};

}  // namespace keymaster
}  // namespace security
}  // namespace android

#endif  // KEYSTORE_INCLUDE_KEYSTORE_EXPORTRESULT_H_
