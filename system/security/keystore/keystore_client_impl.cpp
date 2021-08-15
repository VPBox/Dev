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

#define LOG_TAG "keystore_client"

#include "keystore/keystore_client_impl.h"

#include <future>
#include <string>
#include <vector>

#include <android/security/keystore/IKeystoreService.h>
#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/IServiceManager.h>
#include <keystore/keystore.h>
#include <log/log.h>
#include <utils/String16.h>
#include <utils/String8.h>

#include <keystore/keymaster_types.h>
#include <keystore/keystore_hidl_support.h>
#include <keystore/keystore_promises.h>

#include "keystore_client.pb.h"

namespace {

// Use the UID of the current process.
const int kDefaultUID = -1;
const char kEncryptSuffix[] = "_ENC";
const char kAuthenticateSuffix[] = "_AUTH";
constexpr uint32_t kAESKeySize = 256;      // bits
constexpr uint32_t kHMACKeySize = 256;     // bits
constexpr uint32_t kHMACOutputSize = 256;  // bits

using android::String16;
using android::security::keymaster::ExportResult;
using android::security::keymaster::OperationResult;
using android::security::keystore::KeystoreResponse;
using keystore::AuthorizationSet;
using keystore::AuthorizationSetBuilder;
using keystore::KeyCharacteristics;
using keystore::KeyStoreServiceReturnCode;
}  // namespace

namespace keystore {

KeystoreClientImpl::KeystoreClientImpl() {
    service_manager_ = android::initdefaultServiceManager();
    keystore_binder_ = service_manager_->getService(String16("android.security.keystore"));
    keystore_ =
        android::interface_cast<android::security::keystore::IKeystoreService>(keystore_binder_);
}

bool KeystoreClientImpl::encryptWithAuthentication(const std::string& key_name,
                                                   const std::string& data, int32_t flags,
                                                   std::string* encrypted_data) {
    // The encryption algorithm is AES-256-CBC with PKCS #7 padding and a random
    // IV. The authentication algorithm is HMAC-SHA256 and is computed over the
    // cipher-text (i.e. Encrypt-then-MAC approach). This was chosen over AES-GCM
    // because hardware support for GCM is not mandatory for all Brillo devices.
    std::string encryption_key_name = key_name + kEncryptSuffix;
    if (!createOrVerifyEncryptionKey(encryption_key_name, flags)) {
        return false;
    }
    std::string authentication_key_name = key_name + kAuthenticateSuffix;
    if (!createOrVerifyAuthenticationKey(authentication_key_name, flags)) {
        return false;
    }
    AuthorizationSetBuilder encrypt_params;
    encrypt_params.Padding(PaddingMode::PKCS7);
    encrypt_params.Authorization(TAG_BLOCK_MODE, BlockMode::CBC);
    AuthorizationSet output_params;
    std::string raw_encrypted_data;
    if (!oneShotOperation(KeyPurpose::ENCRYPT, encryption_key_name, encrypt_params, data,
                          std::string(), /* signature_to_verify */
                          &output_params, &raw_encrypted_data)) {
        ALOGE("Encrypt: AES operation failed.");
        return false;
    }
    auto init_vector_blob = output_params.GetTagValue(TAG_NONCE);
    if (!init_vector_blob.isOk()) {
        ALOGE("Encrypt: Missing initialization vector.");
        return false;
    }
    std::string init_vector = hidlVec2String(init_vector_blob.value());

    AuthorizationSetBuilder authenticate_params;
    authenticate_params.Digest(Digest::SHA_2_256);
    authenticate_params.Authorization(TAG_MAC_LENGTH, kHMACOutputSize);
    std::string raw_authentication_data;
    if (!oneShotOperation(KeyPurpose::SIGN, authentication_key_name, authenticate_params,
                          init_vector + raw_encrypted_data, std::string(), /* signature_to_verify */
                          &output_params, &raw_authentication_data)) {
        ALOGE("Encrypt: HMAC operation failed.");
        return false;
    }
    EncryptedData protobuf;
    protobuf.set_init_vector(init_vector);
    protobuf.set_authentication_data(raw_authentication_data);
    protobuf.set_encrypted_data(raw_encrypted_data);
    if (!protobuf.SerializeToString(encrypted_data)) {
        ALOGE("Encrypt: Failed to serialize EncryptedData protobuf.");
        return false;
    }
    return true;
}

bool KeystoreClientImpl::decryptWithAuthentication(const std::string& key_name,
                                                   const std::string& encrypted_data,
                                                   std::string* data) {
    EncryptedData protobuf;
    if (!protobuf.ParseFromString(encrypted_data)) {
        ALOGE("Decrypt: Failed to parse EncryptedData protobuf.");
    }
    // Verify authentication before attempting decryption.
    std::string authentication_key_name = key_name + kAuthenticateSuffix;
    AuthorizationSetBuilder authenticate_params;
    authenticate_params.Digest(Digest::SHA_2_256);
    AuthorizationSet output_params;
    std::string output_data;
    if (!oneShotOperation(KeyPurpose::VERIFY, authentication_key_name, authenticate_params,
                          protobuf.init_vector() + protobuf.encrypted_data(),
                          protobuf.authentication_data(), &output_params, &output_data)) {
        ALOGE("Decrypt: HMAC operation failed.");
        return false;
    }
    std::string encryption_key_name = key_name + kEncryptSuffix;
    AuthorizationSetBuilder encrypt_params;
    encrypt_params.Padding(PaddingMode::PKCS7);
    encrypt_params.Authorization(TAG_BLOCK_MODE, BlockMode::CBC);
    encrypt_params.Authorization(TAG_NONCE, protobuf.init_vector().data(),
                                 protobuf.init_vector().size());
    if (!oneShotOperation(KeyPurpose::DECRYPT, encryption_key_name, encrypt_params,
                          protobuf.encrypted_data(), std::string(), /* signature_to_verify */
                          &output_params, data)) {
        ALOGE("Decrypt: AES operation failed.");
        return false;
    }
    return true;
}

bool KeystoreClientImpl::oneShotOperation(KeyPurpose purpose, const std::string& key_name,
                                          const AuthorizationSet& input_parameters,
                                          const std::string& input_data,
                                          const std::string& signature_to_verify,
                                          AuthorizationSet* output_parameters,
                                          std::string* output_data) {
    uint64_t handle;
    auto result = beginOperation(purpose, key_name, input_parameters, output_parameters, &handle);
    if (!result.isOk()) {
        ALOGE("BeginOperation failed: %d", result.getErrorCode());
        return false;
    }
    AuthorizationSet empty_params;
    size_t num_input_bytes_consumed;
    AuthorizationSet ignored_params;
    result = updateOperation(handle, empty_params, input_data, &num_input_bytes_consumed,
                             &ignored_params, output_data);
    if (!result.isOk()) {
        ALOGE("UpdateOperation failed: %d", result.getErrorCode());
        return false;
    }
    result =
        finishOperation(handle, empty_params, signature_to_verify, &ignored_params, output_data);
    if (!result.isOk()) {
        ALOGE("FinishOperation failed: %d", result.getErrorCode());
        return false;
    }
    return true;
}

KeyStoreNativeReturnCode
KeystoreClientImpl::addRandomNumberGeneratorEntropy(const std::string& entropy, int32_t flags) {
    int32_t error_code;

    android::sp<KeystoreResponsePromise> promise(new KeystoreResponsePromise());
    auto future = promise->get_future();

    auto binder_result =
        keystore_->addRngEntropy(promise, blob2hidlVec(entropy), flags, &error_code);
    if (!binder_result.isOk()) return ResponseCode::SYSTEM_ERROR;

    KeyStoreNativeReturnCode rc(error_code);
    if (!rc.isOk()) return rc;

    auto result = future.get();

    return KeyStoreNativeReturnCode(result.response_code());
}

KeyStoreNativeReturnCode
KeystoreClientImpl::generateKey(const std::string& key_name, const AuthorizationSet& key_parameters,
                                int32_t flags, AuthorizationSet* hardware_enforced_characteristics,
                                AuthorizationSet* software_enforced_characteristics) {
    String16 key_name16(key_name.data(), key_name.size());
    int32_t error_code;
    android::sp<KeyCharacteristicsPromise> promise(new KeyCharacteristicsPromise);
    auto future = promise->get_future();
    auto binder_result = keystore_->generateKey(
        promise, key_name16,
        ::android::security::keymaster::KeymasterArguments(key_parameters.hidl_data()),
        hidl_vec<uint8_t>() /* entropy */, kDefaultUID, flags, &error_code);
    if (!binder_result.isOk()) return ResponseCode::SYSTEM_ERROR;

    KeyStoreNativeReturnCode rc(error_code);
    if (!rc.isOk()) return rc;

    auto [km_response, characteristics] = future.get();

    /* assignment (hidl_vec<KeyParameter> -> AuthorizationSet) makes a deep copy.
     * There are no references to Parcel memory after that, and ownership of the newly acquired
     * memory is with the AuthorizationSet objects. */
    *hardware_enforced_characteristics = characteristics.hardwareEnforced.getParameters();
    *software_enforced_characteristics = characteristics.softwareEnforced.getParameters();
    return KeyStoreNativeReturnCode(km_response.response_code());
}

KeyStoreNativeReturnCode
KeystoreClientImpl::getKeyCharacteristics(const std::string& key_name,
                                          AuthorizationSet* hardware_enforced_characteristics,
                                          AuthorizationSet* software_enforced_characteristics) {
    String16 key_name16(key_name.data(), key_name.size());
    int32_t error_code;
    android::sp<KeyCharacteristicsPromise> promise(new KeyCharacteristicsPromise);
    auto future = promise->get_future();
    auto binder_result = keystore_->getKeyCharacteristics(
        promise, key_name16, android::security::keymaster::KeymasterBlob(),
        android::security::keymaster::KeymasterBlob(), kDefaultUID, &error_code);
    if (!binder_result.isOk()) return ResponseCode::SYSTEM_ERROR;

    KeyStoreNativeReturnCode rc(error_code);
    if (!rc.isOk()) return rc;

    auto [km_response, characteristics] = future.get();

    /* assignment (hidl_vec<KeyParameter> -> AuthorizationSet) makes a deep copy.
     * There are no references to Parcel memory after that, and ownership of the newly acquired
     * memory is with the AuthorizationSet objects. */
    *hardware_enforced_characteristics = characteristics.hardwareEnforced.getParameters();
    *software_enforced_characteristics = characteristics.softwareEnforced.getParameters();
    return KeyStoreNativeReturnCode(km_response.response_code());
}

KeyStoreNativeReturnCode
KeystoreClientImpl::importKey(const std::string& key_name, const AuthorizationSet& key_parameters,
                              KeyFormat key_format, const std::string& key_data,
                              AuthorizationSet* hardware_enforced_characteristics,
                              AuthorizationSet* software_enforced_characteristics) {
    String16 key_name16(key_name.data(), key_name.size());
    auto hidlKeyData = blob2hidlVec(key_data);
    int32_t error_code;
    android::sp<KeyCharacteristicsPromise> promise(new KeyCharacteristicsPromise);
    auto future = promise->get_future();
    auto binder_result = keystore_->importKey(
        promise, key_name16,
        ::android::security::keymaster::KeymasterArguments(key_parameters.hidl_data()),
        (int)key_format, hidlKeyData, kDefaultUID, KEYSTORE_FLAG_NONE, &error_code);
    if (!binder_result.isOk()) return ResponseCode::SYSTEM_ERROR;

    KeyStoreNativeReturnCode rc(error_code);
    if (!rc.isOk()) return rc;

    auto [km_response, characteristics] = future.get();

    /* assignment (hidl_vec<KeyParameter> -> AuthorizationSet) makes a deep copy.
     * There are no references to Parcel memory after that, and ownership of the newly acquired
     * memory is with the AuthorizationSet objects. */
    *hardware_enforced_characteristics = characteristics.hardwareEnforced.getParameters();
    *software_enforced_characteristics = characteristics.softwareEnforced.getParameters();
    return KeyStoreNativeReturnCode(km_response.response_code());
}

KeyStoreNativeReturnCode KeystoreClientImpl::exportKey(KeyFormat export_format,
                                                       const std::string& key_name,
                                                       std::string* export_data) {
    String16 key_name16(key_name.data(), key_name.size());
    int32_t error_code;
    android::sp<KeystoreExportPromise> promise(new KeystoreExportPromise);
    auto future = promise->get_future();
    auto binder_result = keystore_->exportKey(
        promise, key_name16, (int)export_format, android::security::keymaster::KeymasterBlob(),
        android::security::keymaster::KeymasterBlob(), kDefaultUID, &error_code);
    if (!binder_result.isOk()) return ResponseCode::SYSTEM_ERROR;

    KeyStoreNativeReturnCode rc(error_code);
    if (!rc.isOk()) return rc;

    auto export_result = future.get();
    if (!export_result.resultCode.isOk()) return export_result.resultCode;

    *export_data = hidlVec2String(export_result.exportData);

    return export_result.resultCode;
}

KeyStoreNativeReturnCode KeystoreClientImpl::deleteKey(const std::string& key_name) {
    String16 key_name16(key_name.data(), key_name.size());
    int32_t result;
    auto binder_result = keystore_->del(key_name16, kDefaultUID, &result);
    if (!binder_result.isOk()) return ResponseCode::SYSTEM_ERROR;
    return KeyStoreNativeReturnCode(result);
}

KeyStoreNativeReturnCode KeystoreClientImpl::deleteAllKeys() {
    int32_t result;
    auto binder_result = keystore_->clear_uid(kDefaultUID, &result);
    if (!binder_result.isOk()) return ResponseCode::SYSTEM_ERROR;
    return KeyStoreNativeReturnCode(result);
}

KeyStoreNativeReturnCode
KeystoreClientImpl::beginOperation(KeyPurpose purpose, const std::string& key_name,
                                   const AuthorizationSet& input_parameters,
                                   AuthorizationSet* output_parameters, uint64_t* handle) {
    android::sp<android::IBinder> token(new android::BBinder);
    String16 key_name16(key_name.data(), key_name.size());
    int32_t error_code;
    android::sp<OperationResultPromise> promise(new OperationResultPromise{});
    auto future = promise->get_future();
    auto binder_result = keystore_->begin(
        promise, token, key_name16, (int)purpose, true /*pruneable*/,
        android::security::keymaster::KeymasterArguments(input_parameters.hidl_data()),
        hidl_vec<uint8_t>() /* entropy */, kDefaultUID, &error_code);
    if (!binder_result.isOk()) return ResponseCode::SYSTEM_ERROR;
    KeyStoreNativeReturnCode rc(error_code);
    if (!rc.isOk()) return rc;

    OperationResult result = future.get();
    if (result.resultCode.isOk()) {
        *handle = getNextVirtualHandle();
        active_operations_[*handle] = result.token;
        if (result.outParams.size()) {
            *output_parameters = result.outParams;
        }
    }
    return result.resultCode;
}

KeyStoreNativeReturnCode
KeystoreClientImpl::updateOperation(uint64_t handle, const AuthorizationSet& input_parameters,
                                    const std::string& input_data, size_t* num_input_bytes_consumed,
                                    AuthorizationSet* output_parameters, std::string* output_data) {
    if (active_operations_.count(handle) == 0) {
        return ErrorCode::INVALID_OPERATION_HANDLE;
    }
    auto hidlInputData = blob2hidlVec(input_data);
    int32_t error_code;
    android::sp<OperationResultPromise> promise(new OperationResultPromise{});
    auto future = promise->get_future();
    auto binder_result = keystore_->update(
        promise, active_operations_[handle],
        android::security::keymaster::KeymasterArguments(input_parameters.hidl_data()),
        hidlInputData, &error_code);
    if (!binder_result.isOk()) return ResponseCode::SYSTEM_ERROR;
    KeyStoreNativeReturnCode rc(error_code);
    if (!rc.isOk()) return rc;

    OperationResult result = future.get();

    if (result.resultCode.isOk()) {
        *num_input_bytes_consumed = result.inputConsumed;
        if (result.outParams.size()) {
            *output_parameters = result.outParams;
        }
        // TODO verify that append should not be assign
        output_data->append(hidlVec2String(result.data));
    }
    return result.resultCode;
}

KeyStoreNativeReturnCode
KeystoreClientImpl::finishOperation(uint64_t handle, const AuthorizationSet& input_parameters,
                                    const std::string& signature_to_verify,
                                    AuthorizationSet* output_parameters, std::string* output_data) {
    if (active_operations_.count(handle) == 0) {
        return ErrorCode::INVALID_OPERATION_HANDLE;
    }
    int32_t error_code;
    auto hidlSignature = blob2hidlVec(signature_to_verify);
    android::sp<OperationResultPromise> promise(new OperationResultPromise{});
    auto future = promise->get_future();
    auto binder_result = keystore_->finish(
        promise, active_operations_[handle],
        android::security::keymaster::KeymasterArguments(input_parameters.hidl_data()),
        (std::vector<uint8_t>)hidlSignature, hidl_vec<uint8_t>(), &error_code);
    if (!binder_result.isOk()) return ResponseCode::SYSTEM_ERROR;
    KeyStoreNativeReturnCode rc(error_code);
    if (!rc.isOk()) return rc;

    OperationResult result = future.get();
    if (result.resultCode.isOk()) {
        if (result.outParams.size()) {
            *output_parameters = result.outParams;
        }
        // TODO verify that append should not be assign
        output_data->append(hidlVec2String(result.data));
        active_operations_.erase(handle);
    }
    return result.resultCode;
}

KeyStoreNativeReturnCode KeystoreClientImpl::abortOperation(uint64_t handle) {
    if (active_operations_.count(handle) == 0) {
        return ErrorCode::INVALID_OPERATION_HANDLE;
    }
    int32_t result;
    android::sp<KeystoreResponsePromise> promise(new KeystoreResponsePromise{});
    auto future = promise->get_future();
    // Current implementation does not return exceptions in android::binder::Status
    auto binder_result = keystore_->abort(promise, active_operations_[handle], &result);
    if (!binder_result.isOk()) return ResponseCode::SYSTEM_ERROR;
    KeyStoreNativeReturnCode rc(result);
    if (!rc.isOk()) return rc;
    rc = KeyStoreNativeReturnCode(future.get().response_code());
    if (rc.isOk()) {
        active_operations_.erase(handle);
    }
    return rc;
}

bool KeystoreClientImpl::doesKeyExist(const std::string& key_name) {
    String16 key_name16(key_name.data(), key_name.size());
    int32_t result;
    auto binder_result = keystore_->exist(key_name16, kDefaultUID, &result);
    if (!binder_result.isOk()) return false;  // binder error
    return result == static_cast<int32_t>(ResponseCode::NO_ERROR);
}

bool KeystoreClientImpl::listKeys(const std::string& prefix,
                                  std::vector<std::string>* key_name_list) {
    String16 prefix16(prefix.data(), prefix.size());
    std::vector<::android::String16> matches;
    auto binder_result = keystore_->list(prefix16, kDefaultUID, &matches);
    if (!binder_result.isOk()) return false;

    for (const auto& match : matches) {
        android::String8 key_name(match);
        key_name_list->push_back(prefix + std::string(key_name.string(), key_name.size()));
    }
    return true;
}

uint64_t KeystoreClientImpl::getNextVirtualHandle() {
    return next_virtual_handle_++;
}

bool KeystoreClientImpl::createOrVerifyEncryptionKey(const std::string& key_name, int32_t flags) {
    bool key_exists = doesKeyExist(key_name);
    if (key_exists) {
        bool verified = false;
        if (!verifyEncryptionKeyAttributes(key_name, &verified)) {
            return false;
        }
        if (!verified) {
            auto result = deleteKey(key_name);
            if (!result.isOk()) {
                ALOGE("Failed to delete invalid encryption key: %d", result.getErrorCode());
                return false;
            }
            key_exists = false;
        }
    }
    if (!key_exists) {
        AuthorizationSetBuilder key_parameters;
        key_parameters.AesEncryptionKey(kAESKeySize)
            .Padding(PaddingMode::PKCS7)
            .Authorization(TAG_BLOCK_MODE, BlockMode::CBC)
            .Authorization(TAG_NO_AUTH_REQUIRED);
        AuthorizationSet hardware_enforced_characteristics;
        AuthorizationSet software_enforced_characteristics;
        auto result =
            generateKey(key_name, key_parameters, flags, &hardware_enforced_characteristics,
                        &software_enforced_characteristics);
        if (!result.isOk()) {
            ALOGE("Failed to generate encryption key: %d", result.getErrorCode());
            return false;
        }
        if (hardware_enforced_characteristics.size() == 0) {
            ALOGW("WARNING: Encryption key is not hardware-backed.");
        }
    }
    return true;
}

bool KeystoreClientImpl::createOrVerifyAuthenticationKey(const std::string& key_name,
                                                         int32_t flags) {
    bool key_exists = doesKeyExist(key_name);
    if (key_exists) {
        bool verified = false;
        if (!verifyAuthenticationKeyAttributes(key_name, &verified)) {
            return false;
        }
        if (!verified) {
            auto result = deleteKey(key_name);
            if (!result.isOk()) {
                ALOGE("Failed to delete invalid authentication key: %d", result.getErrorCode());
                return false;
            }
            key_exists = false;
        }
    }
    if (!key_exists) {
        AuthorizationSetBuilder key_parameters;
        key_parameters.HmacKey(kHMACKeySize)
            .Digest(Digest::SHA_2_256)
            .Authorization(TAG_MIN_MAC_LENGTH, kHMACOutputSize)
            .Authorization(TAG_NO_AUTH_REQUIRED);
        AuthorizationSet hardware_enforced_characteristics;
        AuthorizationSet software_enforced_characteristics;
        auto result =
            generateKey(key_name, key_parameters, flags, &hardware_enforced_characteristics,
                        &software_enforced_characteristics);
        if (!result.isOk()) {
            ALOGE("Failed to generate authentication key: %d", result.getErrorCode());
            return false;
        }
        if (hardware_enforced_characteristics.size() == 0) {
            ALOGW("WARNING: Authentication key is not hardware-backed.");
        }
    }
    return true;
}

bool KeystoreClientImpl::verifyEncryptionKeyAttributes(const std::string& key_name,
                                                       bool* verified) {
    AuthorizationSet hardware_enforced_characteristics;
    AuthorizationSet software_enforced_characteristics;
    auto result = getKeyCharacteristics(key_name, &hardware_enforced_characteristics,
                                        &software_enforced_characteristics);
    if (!result.isOk()) {
        ALOGE("Failed to query encryption key: %d", result.getErrorCode());
        return false;
    }
    *verified = true;
    auto algorithm = NullOrOr(hardware_enforced_characteristics.GetTagValue(TAG_ALGORITHM),
                              software_enforced_characteristics.GetTagValue(TAG_ALGORITHM));
    if (!algorithm.isOk() || algorithm.value() != Algorithm::AES) {
        ALOGW("Found encryption key with invalid algorithm.");
        *verified = false;
    }
    auto key_size = NullOrOr(hardware_enforced_characteristics.GetTagValue(TAG_KEY_SIZE),
                             software_enforced_characteristics.GetTagValue(TAG_KEY_SIZE));
    if (!key_size.isOk() || key_size.value() != kAESKeySize) {
        ALOGW("Found encryption key with invalid size.");
        *verified = false;
    }
    auto block_mode = NullOrOr(hardware_enforced_characteristics.GetTagValue(TAG_BLOCK_MODE),
                               software_enforced_characteristics.GetTagValue(TAG_BLOCK_MODE));
    if (!block_mode.isOk() || block_mode.value() != BlockMode::CBC) {
        ALOGW("Found encryption key with invalid block mode.");
        *verified = false;
    }
    auto padding_mode = NullOrOr(hardware_enforced_characteristics.GetTagValue(TAG_PADDING),
                                 software_enforced_characteristics.GetTagValue(TAG_PADDING));
    if (!padding_mode.isOk() || padding_mode.value() != PaddingMode::PKCS7) {
        ALOGW("Found encryption key with invalid padding mode.");
        *verified = false;
    }
    if (hardware_enforced_characteristics.size() == 0) {
        ALOGW("WARNING: Encryption key is not hardware-backed.");
    }
    return true;
}

bool KeystoreClientImpl::verifyAuthenticationKeyAttributes(const std::string& key_name,
                                                           bool* verified) {
    AuthorizationSet hardware_enforced_characteristics;
    AuthorizationSet software_enforced_characteristics;
    auto result = getKeyCharacteristics(key_name, &hardware_enforced_characteristics,
                                        &software_enforced_characteristics);
    if (!result.isOk()) {
        ALOGE("Failed to query authentication key: %d", result.getErrorCode());
        return false;
    }
    *verified = true;
    auto algorithm = NullOrOr(hardware_enforced_characteristics.GetTagValue(TAG_ALGORITHM),
                              software_enforced_characteristics.GetTagValue(TAG_ALGORITHM));
    if (!algorithm.isOk() || algorithm.value() != Algorithm::HMAC) {
        ALOGW("Found authentication key with invalid algorithm.");
        *verified = false;
    }
    auto key_size = NullOrOr(hardware_enforced_characteristics.GetTagValue(TAG_KEY_SIZE),
                             software_enforced_characteristics.GetTagValue(TAG_KEY_SIZE));
    if (!key_size.isOk() || key_size.value() != kHMACKeySize) {
        ALOGW("Found authentication key with invalid size.");
        *verified = false;
    }
    auto mac_size = NullOrOr(hardware_enforced_characteristics.GetTagValue(TAG_MIN_MAC_LENGTH),
                             software_enforced_characteristics.GetTagValue(TAG_MIN_MAC_LENGTH));
    if (!mac_size.isOk() || mac_size.value() != kHMACOutputSize) {
        ALOGW("Found authentication key with invalid minimum mac size.");
        *verified = false;
    }
    auto digest = NullOrOr(hardware_enforced_characteristics.GetTagValue(TAG_DIGEST),
                           software_enforced_characteristics.GetTagValue(TAG_DIGEST));
    if (!digest.isOk() || digest.value() != Digest::SHA_2_256) {
        ALOGW("Found authentication key with invalid digest list.");
        *verified = false;
    }
    if (hardware_enforced_characteristics.size() == 0) {
        ALOGW("WARNING: Authentication key is not hardware-backed.");
    }
    return true;
}

}  // namespace keystore
