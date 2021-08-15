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

#include "include/keystore/OperationResult.h"

#include <utility>

#include <binder/Parcel.h>

#include <keystore/keymaster_types.h>

#include "keystore_aidl_hidl_marshalling_utils.h"

namespace android {
namespace security {
namespace keymaster {

using keystore::keymaster::ErrorCode;
using ::android::status_t;

OperationResult::OperationResult() : resultCode(), token(), handle(0), inputConsumed(0), data() {}

status_t OperationResult::readFromParcel(const Parcel* inn) {
    const Parcel& in = *inn;
    resultCode = ErrorCode(in.readInt32());
    token = in.readStrongBinder();
    handle = static_cast<uint64_t>(in.readInt64());
    inputConsumed = in.readInt32();
    data = keystore::readKeymasterBlob(in);
    outParams = keystore::readParamSetFromParcel(in);
    return OK;
}

status_t OperationResult::writeToParcel(Parcel* out) const {
    out->writeInt32(resultCode.getErrorCode());
    out->writeStrongBinder(token);
    out->writeInt64(handle);
    out->writeInt32(inputConsumed);
    keystore::writeKeymasterBlob(data, out);
    keystore::writeParamSetToParcel(outParams, out);
    return OK;
}

OperationResult operationFailed(const ::keystore::KeyStoreServiceReturnCode& error) {
    OperationResult opResult = {};
    opResult.resultCode = error;
    return opResult;
}

}  // namespace keymaster
}  // namespace security
}  // namespace android
