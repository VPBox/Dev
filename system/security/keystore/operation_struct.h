/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef KEYSTORE_OPERATION_STRUCT_H_
#define KEYSTORE_OPERATION_STRUCT_H_

#include <binder/Binder.h>
#include <binder/IBinder.h>
#include <keymasterV4_0/Keymaster.h>
#include <utils/StrongPointer.h>

#include <keystore/keymaster_types.h>
#include <keystore/keystore_hidl_support.h>
#include <keystore/keystore_return_types.h>

#include <future>

namespace keystore {

using ::android::IBinder;
using ::android::sp;
using keymaster::support::Keymaster;

struct Operation {
    Operation() = default;
    Operation(uint64_t handle_, uint64_t keyid_, KeyPurpose purpose_, const sp<Keymaster>& device_,
              KeyCharacteristics&& characteristics_, sp<IBinder> appToken_,
              const hidl_vec<KeyParameter> params_)
        : handle(handle_), keyid(keyid_), purpose(purpose_), device(device_),
          characteristics(characteristics_), appToken(appToken_), authToken(), verificationToken(),
          params(params_) {}
    Operation(Operation&&) = default;
    Operation(const Operation&) = delete;

    bool hasAuthToken() const { return authToken.mac.size() != 0; }

    uint64_t handle;
    uint64_t keyid;
    KeyPurpose purpose;
    sp<Keymaster> device;
    KeyCharacteristics characteristics;
    sp<IBinder> appToken;
    std::promise<KeyStoreServiceReturnCode> authTokenPromise;
    std::future<KeyStoreServiceReturnCode> authTokenFuture;
    HardwareAuthToken authToken;
    VerificationToken verificationToken;
    const hidl_vec<KeyParameter> params;
};

}  // namespace keystore

#endif
