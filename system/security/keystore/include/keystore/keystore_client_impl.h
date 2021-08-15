// Copyright 2015 The Android Open Source Project
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

#ifndef KEYSTORE_KEYSTORE_CLIENT_IMPL_H_
#define KEYSTORE_KEYSTORE_CLIENT_IMPL_H_

#include "keystore_client.h"

#include <future>
#include <map>
#include <string>
#include <vector>

#include <android/security/keystore/IKeystoreService.h>
#include <binder/IBinder.h>
#include <binder/IServiceManager.h>
#include <utils/StrongPointer.h>

namespace keystore {

class KeystoreClientImpl : public KeystoreClient {
  public:
    KeystoreClientImpl();
    ~KeystoreClientImpl() override = default;

    // KeystoreClient methods.
    bool encryptWithAuthentication(const std::string& key_name, const std::string& data,
                                   int32_t flags, std::string* encrypted_data) override;
    bool decryptWithAuthentication(const std::string& key_name, const std::string& encrypted_data,
                                   std::string* data) override;
    bool oneShotOperation(KeyPurpose purpose, const std::string& key_name,
                          const keystore::AuthorizationSet& input_parameters,
                          const std::string& input_data, const std::string& signature_to_verify,
                          keystore::AuthorizationSet* output_parameters,
                          std::string* output_data) override;
    KeyStoreNativeReturnCode addRandomNumberGeneratorEntropy(const std::string& entropy,
                                                             int32_t flags) override;
    KeyStoreNativeReturnCode
    generateKey(const std::string& key_name, const keystore::AuthorizationSet& key_parameters,
                int32_t flags, keystore::AuthorizationSet* hardware_enforced_characteristics,
                keystore::AuthorizationSet* software_enforced_characteristics) override;
    KeyStoreNativeReturnCode
    getKeyCharacteristics(const std::string& key_name,
                          keystore::AuthorizationSet* hardware_enforced_characteristics,
                          keystore::AuthorizationSet* software_enforced_characteristics) override;
    KeyStoreNativeReturnCode
    importKey(const std::string& key_name, const keystore::AuthorizationSet& key_parameters,
              KeyFormat key_format, const std::string& key_data,
              keystore::AuthorizationSet* hardware_enforced_characteristics,
              keystore::AuthorizationSet* software_enforced_characteristics) override;
    KeyStoreNativeReturnCode exportKey(KeyFormat export_format, const std::string& key_name,
                                       std::string* export_data) override;
    KeyStoreNativeReturnCode deleteKey(const std::string& key_name) override;
    KeyStoreNativeReturnCode deleteAllKeys() override;
    KeyStoreNativeReturnCode beginOperation(KeyPurpose purpose, const std::string& key_name,
                                            const keystore::AuthorizationSet& input_parameters,
                                            keystore::AuthorizationSet* output_parameters,
                                            uint64_t* handle) override;
    KeyStoreNativeReturnCode updateOperation(uint64_t handle,
                                             const keystore::AuthorizationSet& input_parameters,
                                             const std::string& input_data,
                                             size_t* num_input_bytes_consumed,
                                             keystore::AuthorizationSet* output_parameters,
                                             std::string* output_data) override;
    KeyStoreNativeReturnCode finishOperation(uint64_t handle,
                                             const keystore::AuthorizationSet& input_parameters,
                                             const std::string& signature_to_verify,
                                             keystore::AuthorizationSet* output_parameters,
                                             std::string* output_data) override;
    KeyStoreNativeReturnCode abortOperation(uint64_t handle) override;
    bool doesKeyExist(const std::string& key_name) override;
    bool listKeys(const std::string& prefix, std::vector<std::string>* key_name_list) override;

  private:
    // Returns an available virtual operation handle.
    uint64_t getNextVirtualHandle();

    // Maps a keystore error code to a code where all success cases use
    // KM_ERROR_OK (not keystore's NO_ERROR).
    //    int32_t mapKeystoreError(int32_t keystore_error);

    // Creates an encryption key suitable for EncryptWithAuthentication or
    // verifies attributes if the key already exists. Returns true on success.
    bool createOrVerifyEncryptionKey(const std::string& key_name, int32_t flags);

    // Creates an authentication key suitable for EncryptWithAuthentication or
    // verifies attributes if the key already exists. Returns true on success.
    bool createOrVerifyAuthenticationKey(const std::string& key_name, int32_t flags);

    // Verifies attributes of an encryption key suitable for
    // EncryptWithAuthentication. Returns true on success and populates |verified|
    // with the result of the verification.
    bool verifyEncryptionKeyAttributes(const std::string& key_name, bool* verified);

    // Verifies attributes of an authentication key suitable for
    // EncryptWithAuthentication. Returns true on success and populates |verified|
    // with the result of the verification.
    bool verifyAuthenticationKeyAttributes(const std::string& key_name, bool* verified);

    android::sp<android::IServiceManager> service_manager_;
    android::sp<android::IBinder> keystore_binder_;
    android::sp<android::security::keystore::IKeystoreService> keystore_;
    uint64_t next_virtual_handle_ = 1;
    std::map<uint64_t, android::sp<android::IBinder>> active_operations_;

    DISALLOW_COPY_AND_ASSIGN(KeystoreClientImpl);
};

}  // namespace keystore

#endif  // KEYSTORE_KEYSTORE_CLIENT_IMPL_H_
