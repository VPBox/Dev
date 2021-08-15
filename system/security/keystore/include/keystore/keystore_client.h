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

#ifndef KEYSTORE_KEYSTORE_CLIENT_H_
#define KEYSTORE_KEYSTORE_CLIENT_H_

#include <set>
#include <string>
#include <vector>

#include <android-base/macros.h>

#include "keymaster_types.h"
#include "keystore.h"
#include "keystore_return_types.h"

namespace keystore {

// An abstract class providing a convenient interface to keystore services. This
// interface is designed to:
//   - hide details of the IPC mechanism (e.g. binder)
//   - use std data types
//   - encourage the use of keystore::AuthorizationSet[Builder]
//   - be convenient for native services integrating with keystore
//   - be safely mocked for unit testing (e.g. pure virtual methods)
//
// Example usage:
//   KeystoreClient* keystore = new KeyStoreClientImpl();
//   keystore->AddRandomNumberGeneratorEntropy("unpredictable");
//
// Notes on error codes:
//   Keystore binder methods return a variety of values including ResponseCode
//   values defined in keystore.h, keymaster_error_t values defined in
//   keymaster_defs.h, or just 0 or -1 (both of which conflict with
//   keymaster_error_t). The methods in this class converge on a single success
//   indicator for convenience. KM_ERROR_OK was chosen over ::NO_ERROR for two
//   reasons:
//   1) KM_ERROR_OK is 0, which is a common convention for success, is the gmock
//      default, and allows error checks like 'if (error) {...'.
//   2) Although both pollute the global namespace, KM_ERROR_OK has a prefix per
//      C convention and hopefully clients can use this interface without
//      needing to include 'keystore.h' directly.
class KeystoreClient {
  public:
    KeystoreClient() = default;
    virtual ~KeystoreClient() = default;

    // Encrypts and authenticates |data| with minimal configuration for local
    // decryption. If a key identified by |key_name| does not already exist it
    // will be generated. On success returns true and populates |encrypted_data|.
    // Note: implementations may generate more than one key but they will always
    // have |key_name| as a prefix.
    virtual bool encryptWithAuthentication(const std::string& key_name, const std::string& data,
                                           int32_t flags, std::string* encrypted_data) = 0;

    // Decrypts and authenticates |encrypted_data| as output by
    // EncryptWithAuthentication using the key(s) identified by |key_name|. On
    // success returns true and populates |data|.
    virtual bool decryptWithAuthentication(const std::string& key_name,
                                           const std::string& encrypted_data,
                                           std::string* data) = 0;

    // Performs a Begin/Update/Finish sequence for an operation. The |purpose|,
    // |key_name|, |input_parameters|, and |output_parameters| are as in
    // BeginOperation. The |input_data| is as in UpdateOperation. The
    // |signature_to_verify| and |output_data| are as in FinishOperation. On
    // success returns true.
    virtual bool oneShotOperation(KeyPurpose purpose, const std::string& key_name,
                                  const keystore::AuthorizationSet& input_parameters,
                                  const std::string& input_data,
                                  const std::string& signature_to_verify,
                                  keystore::AuthorizationSet* output_parameters,
                                  std::string* output_data) = 0;

    // Adds |entropy| to the random number generator. Returns KM_ERROR_OK on
    // success and a Keystore ResponseCode or keymaster_error_t on failure.
    virtual KeyStoreNativeReturnCode addRandomNumberGeneratorEntropy(const std::string& entropy,
                                                                     int32_t flags) = 0;

    // Generates a key according to the given |key_parameters| and stores it with
    // the given |key_name|. The [hardware|software]_enforced_characteristics of
    // the key are provided on success. Returns KM_ERROR_OK on success. Returns
    // KM_ERROR_OK on success and a Keystore ResponseCode or keymaster_error_t on
    // failure.
    virtual KeyStoreNativeReturnCode
    generateKey(const std::string& key_name, const keystore::AuthorizationSet& key_parameters,
                int32_t flags, keystore::AuthorizationSet* hardware_enforced_characteristics,
                keystore::AuthorizationSet* software_enforced_characteristics) = 0;

    // Provides the [hardware|software]_enforced_characteristics of a key
    // identified by |key_name|. Returns KM_ERROR_OK on success and a Keystore
    // ResponseCode or keymaster_error_t on failure.
    virtual KeyStoreNativeReturnCode
    getKeyCharacteristics(const std::string& key_name,
                          keystore::AuthorizationSet* hardware_enforced_characteristics,
                          keystore::AuthorizationSet* software_enforced_characteristics) = 0;

    // Imports |key_data| in the given |key_format|, applies the given
    // |key_parameters|, and stores it with the given |key_name|. The
    // [hardware|software]_enforced_characteristics of the key are provided on
    // success. Returns KM_ERROR_OK on success and a Keystore ResponseCode or
    // keymaster_error_t on failure.
    virtual KeyStoreNativeReturnCode
    importKey(const std::string& key_name, const keystore::AuthorizationSet& key_parameters,
              KeyFormat key_format, const std::string& key_data,
              keystore::AuthorizationSet* hardware_enforced_characteristics,
              keystore::AuthorizationSet* software_enforced_characteristics) = 0;

    // Exports the public key identified by |key_name| to |export_data| using
    // |export_format|. Returns KM_ERROR_OK on success and a Keystore ResponseCode
    // or keymaster_error_t on failure.
    virtual KeyStoreNativeReturnCode exportKey(KeyFormat export_format, const std::string& key_name,
                                               std::string* export_data) = 0;

    // Deletes the key identified by |key_name|. Returns KM_ERROR_OK on success
    // and a Keystore ResponseCode or keymaster_error_t on failure.
    virtual KeyStoreNativeReturnCode deleteKey(const std::string& key_name) = 0;

    // Deletes all keys owned by the caller. Returns KM_ERROR_OK on success and a
    // Keystore ResponseCode or keymaster_error_t on failure.
    virtual KeyStoreNativeReturnCode deleteAllKeys() = 0;

    // Begins a cryptographic operation (e.g. encrypt, sign) identified by
    // |purpose| using the key identified by |key_name| and the given
    // |input_parameters|. On success, any |output_parameters| and an operation
    // |handle| are populated. Returns KM_ERROR_OK on success and a Keystore
    // ResponseCode or keymaster_error_t on failure.
    virtual KeyStoreNativeReturnCode
    beginOperation(KeyPurpose purpose, const std::string& key_name,
                   const keystore::AuthorizationSet& input_parameters,
                   keystore::AuthorizationSet* output_parameters, uint64_t* handle) = 0;

    // Continues the operation associated with |handle| using the given
    // |input_parameters| and |input_data|. On success, the
    // |num_input_bytes_consumed| and any |output_parameters| are populated. Any
    // |output_data| will be appended. Returns KM_ERROR_OK on success and a
    // Keystore ResponseCode or keymaster_error_t on failure.
    virtual KeyStoreNativeReturnCode
    updateOperation(uint64_t handle, const keystore::AuthorizationSet& input_parameters,
                    const std::string& input_data, size_t* num_input_bytes_consumed,
                    keystore::AuthorizationSet* output_parameters, std::string* output_data) = 0;

    // Finishes the operation associated with |handle| using the given
    // |input_parameters| and, if necessary, a |signature_to_verify|. On success,
    // any |output_parameters| are populated and |output_data| is appended.
    // Returns KM_ERROR_OK on success and a Keystore ResponseCode or
    // keymaster_error_t on failure.
    virtual KeyStoreNativeReturnCode
    finishOperation(uint64_t handle, const keystore::AuthorizationSet& input_parameters,
                    const std::string& signature_to_verify,
                    keystore::AuthorizationSet* output_parameters, std::string* output_data) = 0;

    // Aborts the operation associated with |handle|. Returns KM_ERROR_OK on
    // success and a Keystore ResponseCode or keymaster_error_t on failure.
    virtual KeyStoreNativeReturnCode abortOperation(uint64_t handle) = 0;

    // Returns true if a key identified by |key_name| exists in the caller's
    // key store. Returns false if an error occurs.
    virtual bool doesKeyExist(const std::string& key_name) = 0;

    // Provides a |key_name_list| containing all existing key names in the
    // caller's key store starting with |prefix|. Returns true on success.
    virtual bool listKeys(const std::string& prefix, std::vector<std::string>* key_name_list) = 0;

  private:
    DISALLOW_COPY_AND_ASSIGN(KeystoreClient);
};

}  // namespace keystore

#endif  // KEYSTORE_KEYSTORE_CLIENT_H_
