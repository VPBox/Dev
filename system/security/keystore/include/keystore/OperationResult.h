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

#ifndef KEYSTORE_INCLUDE_KEYSTORE_OPERATIONRESULT_H_
#define KEYSTORE_INCLUDE_KEYSTORE_OPERATIONRESULT_H_

#include <binder/Parcel.h>
#include <binder/Parcelable.h>

#include "keymaster_types.h"
#include "keystore_return_types.h"

namespace android {
namespace security {
namespace keymaster {

struct OperationResult : public ::android::Parcelable {
    OperationResult();
    status_t readFromParcel(const Parcel* in) override;
    status_t writeToParcel(Parcel* out) const override;

    // Native code may need to use KeyStoreNativeReturnCode
    ::keystore::KeyStoreServiceReturnCode resultCode;
    sp<IBinder> token;
    uint64_t handle;
    int inputConsumed;
    ::keystore::hidl_vec<uint8_t> data;
    ::keystore::hidl_vec<::keystore::KeyParameter> outParams;
};

OperationResult operationFailed(const ::keystore::KeyStoreServiceReturnCode& error);

}  // namespace keymaster
}  // namespace security
}  // namespace android

#endif  // KEYSTORE_INCLUDE_KEYSTORE_OPERATIONRESULT_H_
