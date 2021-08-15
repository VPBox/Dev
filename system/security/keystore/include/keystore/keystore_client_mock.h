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

#ifndef KEYSTORE_KEYSTORE_CLIENT_MOCK_H_
#define KEYSTORE_KEYSTORE_CLIENT_MOCK_H_

#include "keystore/keystore_client.h"
#include "gmock/gmock.h"

using testing::_;

namespace keystore {

// A mock implementation of KeystoreClient. By default all methods do nothing
// and return KM_ERROR_OK (or false).
class KeystoreClientMock : public KeystoreClient {
  public:
    KeystoreClientMock() = default;
    ~KeystoreClientMock() = default;

    MOCK_METHOD3(encryptWithAuthentication,
                 bool(const std::string& key_name, const std::string& data,
                      std::string* encrypted_data));
    MOCK_METHOD3(decryptWithAuthentication,
                 bool(const std::string& key_name, const std::string& encrypted_data,
                      std::string* data));
    MOCK_METHOD7(oneShotOperation,
                 bool(keymaster_purpose_t purpose, const std::string& key_name,
                      const keymaster::AuthorizationSet& input_parameters,
                      const std::string& input_data, const std::string& signature_to_verify,
                      keymaster::AuthorizationSet* output_parameters, std::string* output_data));
    MOCK_METHOD1(addRandomNumberGeneratorEntropy, int32_t(const std::string& entropy));
    MOCK_METHOD4(generateKey,
                 int32_t(const std::string& key_name,
                         const keymaster::AuthorizationSet& key_parameters,
                         keymaster::AuthorizationSet* hardware_enforced_characteristics,
                         keymaster::AuthorizationSet* software_enforced_characteristics));
    MOCK_METHOD3(getKeyCharacteristics,
                 int32_t(const std::string& key_name,
                         keymaster::AuthorizationSet* hardware_enforced_characteristics,
                         keymaster::AuthorizationSet* software_enforced_characteristics));
    MOCK_METHOD6(importKey,
                 int32_t(const std::string& key_name,
                         const keymaster::AuthorizationSet& key_parameters,
                         keymaster_key_format_t key_format, const std::string& key_data,
                         keymaster::AuthorizationSet* hardware_enforced_characteristics,
                         keymaster::AuthorizationSet* software_enforced_characteristics));
    MOCK_METHOD3(exportKey, int32_t(keymaster_key_format_t export_format,
                                    const std::string& key_name, std::string* export_data));
    MOCK_METHOD1(deleteKey, int32_t(const std::string& key_name));
    MOCK_METHOD0(deleteAllKeys, int32_t());
    MOCK_METHOD5(beginOperation, int32_t(keymaster_purpose_t purpose, const std::string& key_name,
                                         const keymaster::AuthorizationSet& input_parameters,
                                         keymaster::AuthorizationSet* output_parameters,
                                         keymaster_operation_handle_t* handle));
    MOCK_METHOD6(updateOperation,
                 int32_t(keymaster_operation_handle_t handle,
                         const keymaster::AuthorizationSet& input_parameters,
                         const std::string& input_data, size_t* num_input_bytes_consumed,
                         keymaster::AuthorizationSet* output_parameters, std::string* output_data));
    MOCK_METHOD5(finishOperation,
                 int32_t(keymaster_operation_handle_t handle,
                         const keymaster::AuthorizationSet& input_parameters,
                         const std::string& signature_to_verify,
                         keymaster::AuthorizationSet* output_parameters, std::string* output_data));
    MOCK_METHOD1(abortOperation, int32_t(keymaster_operation_handle_t handle));
    MOCK_METHOD1(doesKeyExist, bool(const std::string& key_name));
    MOCK_METHOD2(listKeys,
                 bool(const std::string& prefix, std::vector<std::string>* key_name_list));

  private:
    DISALLOW_COPY_AND_ASSIGN(KeystoreClientMock);
};

}  // namespace keystore

#endif  // KEYSTORE_KEYSTORE_CLIENT_MOCK_H_
