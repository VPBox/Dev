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

#ifndef KEYSTORE_INCLUDE_KEYSTORE_KEYSTORE_PROMISES_H_
#define KEYSTORE_INCLUDE_KEYSTORE_KEYSTORE_PROMISES_H_

#include <android/security/keystore/BnKeystoreCertificateChainCallback.h>
#include <android/security/keystore/BnKeystoreExportKeyCallback.h>
#include <android/security/keystore/BnKeystoreKeyCharacteristicsCallback.h>
#include <android/security/keystore/BnKeystoreOperationResultCallback.h>
#include <android/security/keystore/BnKeystoreResponseCallback.h>
#include <future>

namespace keystore {

template <typename BnInterface, typename Result>
class CallbackPromise : public BnInterface, public std::promise<Result> {
  public:
    ::android::binder::Status onFinished(const Result& result) override {
        this->set_value(result);
        return ::android::binder::Status::ok();
    }
};

template <typename BnInterface, typename... Results>
class CallbackPromise<BnInterface, std::tuple<Results...>>
    : public BnInterface, public std::promise<std::tuple<Results...>> {
  public:
    ::android::binder::Status onFinished(const Results&... results) override {
        this->set_value({results...});
        return ::android::binder::Status::ok();
    }
};

using OperationResultPromise =
    CallbackPromise<::android::security::keystore::BnKeystoreOperationResultCallback,
                    ::android::security::keymaster::OperationResult>;

using KeystoreResponsePromise =
    CallbackPromise<::android::security::keystore::BnKeystoreResponseCallback,
                    ::android::security::keystore::KeystoreResponse>;

using KeyCharacteristicsPromise =
    CallbackPromise<::android::security::keystore::BnKeystoreKeyCharacteristicsCallback,
                    std::tuple<::android::security::keystore::KeystoreResponse,
                               ::android::security::keymaster::KeyCharacteristics>>;
using KeystoreExportPromise =
    CallbackPromise<::android::security::keystore::BnKeystoreExportKeyCallback,
                    ::android::security::keymaster::ExportResult>;

using KeyCertChainPromise =
    CallbackPromise<::android::security::keystore::BnKeystoreCertificateChainCallback,
                    std::tuple<::android::security::keystore::KeystoreResponse,
                               ::android::security::keymaster::KeymasterCertificateChain>>;

}  // namespace keystore

#endif  // KEYSTORE_INCLUDE_KEYSTORE_KEYSTORE_PROMISES_H_
