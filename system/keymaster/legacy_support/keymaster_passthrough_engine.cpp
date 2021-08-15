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
#include "keymaster_passthrough_operation.h"
#include <keymaster/legacy_support/keymaster_passthrough_engine.h>
#include <keymaster/legacy_support/keymaster_passthrough_key.h>

#include <hardware/keymaster1.h>
#include <hardware/keymaster2.h>

#include <assert.h>

#include <algorithm>
#include <memory>
#include <type_traits>

#define LOG_TAG "Keymaster2Engine"
#include <android/log.h>

using std::shared_ptr;
using std::unique_ptr;

namespace keymaster {

template <typename KeymasterDeviceType>
class TKeymasterPassthroughEngine : public KeymasterPassthroughEngine {
    using opfactory_t = KeymasterPassthroughOperationFactory<KeymasterDeviceType>;

  public:
    /**
     * The engine takes ownership of the device, and will close it during destruction.
     */
    explicit TKeymasterPassthroughEngine(const KeymasterDeviceType* km_device)
        : km_device_(km_device) {
        rsa_encrypt_op_factory_.reset(
            new opfactory_t(KM_ALGORITHM_RSA, KM_PURPOSE_ENCRYPT, km_device_));
        rsa_decrypt_op_factory_.reset(
            new opfactory_t(KM_ALGORITHM_RSA, KM_PURPOSE_DECRYPT, km_device_));
        rsa_sign_op_factory_.reset(new opfactory_t(KM_ALGORITHM_RSA, KM_PURPOSE_SIGN, km_device_));
        rsa_verify_op_factory_.reset(
            new opfactory_t(KM_ALGORITHM_RSA, KM_PURPOSE_VERIFY, km_device_));
        ec_encrypt_op_factory_.reset(
            new opfactory_t(KM_ALGORITHM_EC, KM_PURPOSE_ENCRYPT, km_device_));
        ec_decrypt_op_factory_.reset(
            new opfactory_t(KM_ALGORITHM_EC, KM_PURPOSE_DECRYPT, km_device_));
        ec_sign_op_factory_.reset(new opfactory_t(KM_ALGORITHM_EC, KM_PURPOSE_SIGN, km_device_));
        ec_verify_op_factory_.reset(
            new opfactory_t(KM_ALGORITHM_EC, KM_PURPOSE_VERIFY, km_device_));
        ec_derive_op_factory_.reset(
            new opfactory_t(KM_ALGORITHM_EC, KM_PURPOSE_DERIVE_KEY, km_device_));
        aes_encrypt_op_factory_.reset(
            new opfactory_t(KM_ALGORITHM_AES, KM_PURPOSE_ENCRYPT, km_device_));
        aes_decrypt_op_factory_.reset(
            new opfactory_t(KM_ALGORITHM_AES, KM_PURPOSE_DECRYPT, km_device_));
        triple_des_encrypt_op_factory_.reset(
            new opfactory_t(KM_ALGORITHM_TRIPLE_DES, KM_PURPOSE_ENCRYPT, km_device_));
        triple_des_decrypt_op_factory_.reset(
            new opfactory_t(KM_ALGORITHM_TRIPLE_DES, KM_PURPOSE_DECRYPT, km_device_));
        hmac_sign_op_factory_.reset(
            new opfactory_t(KM_ALGORITHM_HMAC, KM_PURPOSE_SIGN, km_device_));
        hmac_verify_op_factory_.reset(
            new opfactory_t(KM_ALGORITHM_HMAC, KM_PURPOSE_VERIFY, km_device_));
    }
    virtual ~TKeymasterPassthroughEngine() {
        // QUIRK: we only take ownership if this is a KM2 device.
        //        For KM1 the Keymaster1Engine takes ownership
        if (std::is_same<KeymasterDeviceType, keymaster2_device_t>::value)
            km_device_->common.close(
                    reinterpret_cast<hw_device_t*>(const_cast<KeymasterDeviceType*>(km_device_)));
    }

    keymaster_error_t GenerateKey(const AuthorizationSet& key_description,
                                  KeymasterKeyBlob* key_material, AuthorizationSet* hw_enforced,
                                  AuthorizationSet* sw_enforced) const override;

    keymaster_error_t ImportKey(const AuthorizationSet& key_description,
                                keymaster_key_format_t input_key_material_format,
                                const KeymasterKeyBlob& input_key_material,
                                KeymasterKeyBlob* output_key_blob, AuthorizationSet* hw_enforced,
                                AuthorizationSet* sw_enforced) const override;
    keymaster_error_t ExportKey(keymaster_key_format_t format,
                                const KeymasterKeyBlob& blob,
                                const KeymasterBlob& client_id,
                                const KeymasterBlob& app_data,
                                KeymasterBlob* export_data) const override {
        keymaster_blob_t my_export_data = {};
        keymaster_error_t error = km_device_->export_key(km_device_, format, &blob, &client_id,
                                                         &app_data, &my_export_data);
        if (error != KM_ERROR_OK)
            return error;
        *export_data = KeymasterBlob(my_export_data.data, my_export_data.data_length);
        free(const_cast<uint8_t*>(my_export_data.data));
        if (export_data->data == nullptr) {
            return KM_ERROR_MEMORY_ALLOCATION_FAILED;
        }
        return error;
    }
    keymaster_error_t DeleteKey(const KeymasterKeyBlob& blob) const override {
        return km_device_->delete_key(km_device_, &blob);
    }
    keymaster_error_t DeleteAllKeys() const override {
        return km_device_->delete_all_keys(km_device_);
    }
    OperationFactory* GetOperationFactory(keymaster_purpose_t purpose,
                                          keymaster_algorithm_t algorithm) const override {
        switch(algorithm) {
        case KM_ALGORITHM_RSA:
            switch(purpose) {
            case KM_PURPOSE_ENCRYPT:
                return rsa_encrypt_op_factory_.get();
            case KM_PURPOSE_DECRYPT:
                return rsa_decrypt_op_factory_.get();
            case KM_PURPOSE_SIGN:
                return rsa_sign_op_factory_.get();
            case KM_PURPOSE_VERIFY:
                return rsa_verify_op_factory_.get();
            default:
                return nullptr;
            }
        case KM_ALGORITHM_EC:
            switch(purpose) {
            case KM_PURPOSE_ENCRYPT:
                return ec_encrypt_op_factory_.get();
            case KM_PURPOSE_DECRYPT:
                return ec_decrypt_op_factory_.get();
            case KM_PURPOSE_SIGN:
                return ec_sign_op_factory_.get();
            case KM_PURPOSE_VERIFY:
                return ec_verify_op_factory_.get();
            case KM_PURPOSE_DERIVE_KEY:
                return ec_derive_op_factory_.get();
            default:
                return nullptr;
            }
        case KM_ALGORITHM_AES:
            switch(purpose) {
            case KM_PURPOSE_ENCRYPT:
                return aes_encrypt_op_factory_.get();
            case KM_PURPOSE_DECRYPT:
                return aes_decrypt_op_factory_.get();
            default:
                return nullptr;
            }
        case KM_ALGORITHM_TRIPLE_DES:
            switch (purpose) {
            case KM_PURPOSE_ENCRYPT:
                return triple_des_encrypt_op_factory_.get();
            case KM_PURPOSE_DECRYPT:
                return triple_des_decrypt_op_factory_.get();
            default:
                return nullptr;
            }
        case KM_ALGORITHM_HMAC:
            switch (purpose) {
            case KM_PURPOSE_SIGN:
                return hmac_sign_op_factory_.get();
            case KM_PURPOSE_VERIFY:
                return hmac_verify_op_factory_.get();
            default:
                return nullptr;
            }
        }
    }

    const KeymasterDeviceType* device() const { return km_device_; }

  private:
    TKeymasterPassthroughEngine(const KeymasterPassthroughEngine&) = delete;  // Uncopyable
    void operator=(const KeymasterPassthroughEngine&) = delete;    // Unassignable

    const KeymasterDeviceType* const km_device_;
    std::unique_ptr<opfactory_t> rsa_encrypt_op_factory_;
    std::unique_ptr<opfactory_t> rsa_decrypt_op_factory_;
    std::unique_ptr<opfactory_t> rsa_sign_op_factory_;
    std::unique_ptr<opfactory_t> rsa_verify_op_factory_;
    std::unique_ptr<opfactory_t> ec_encrypt_op_factory_;
    std::unique_ptr<opfactory_t> ec_decrypt_op_factory_;
    std::unique_ptr<opfactory_t> ec_sign_op_factory_;
    std::unique_ptr<opfactory_t> ec_verify_op_factory_;
    std::unique_ptr<opfactory_t> ec_derive_op_factory_;
    std::unique_ptr<opfactory_t> aes_encrypt_op_factory_;
    std::unique_ptr<opfactory_t> aes_decrypt_op_factory_;
    std::unique_ptr<opfactory_t> triple_des_encrypt_op_factory_;
    std::unique_ptr<opfactory_t> triple_des_decrypt_op_factory_;
    std::unique_ptr<opfactory_t> hmac_sign_op_factory_;
    std::unique_ptr<opfactory_t> hmac_verify_op_factory_;
};

static void ConvertCharacteristics(const keymaster_key_characteristics_t& characteristics,
                                   AuthorizationSet* hw_enforced, AuthorizationSet* sw_enforced) {
    if (hw_enforced)
        hw_enforced->Reinitialize(characteristics.hw_enforced);
    if (sw_enforced)
        sw_enforced->Reinitialize(characteristics.sw_enforced);
}

template<>
keymaster_error_t
TKeymasterPassthroughEngine<keymaster1_device_t>::GenerateKey(const AuthorizationSet& key_description,
                                                KeymasterKeyBlob* key_blob,
                                                AuthorizationSet* hw_enforced,
                                                AuthorizationSet* sw_enforced) const {
    assert(key_blob);

    keymaster_key_characteristics_t* characteristics = nullptr;
    keymaster_key_blob_t blob = {};
    keymaster_error_t error = km_device_->generate_key(km_device_, &key_description,
                                                               &blob, &characteristics);
    if (error != KM_ERROR_OK)
        return error;
    unique_ptr<uint8_t, Malloc_Delete> blob_deleter(const_cast<uint8_t*>(blob.key_material));
    key_blob->key_material = dup_buffer(blob.key_material, blob.key_material_size);
    key_blob->key_material_size = blob.key_material_size;

    ConvertCharacteristics(*characteristics, hw_enforced, sw_enforced);
    keymaster_free_characteristics(characteristics);
    free (characteristics);
    return error;
}
template<>
keymaster_error_t
TKeymasterPassthroughEngine<keymaster2_device_t>::GenerateKey(const AuthorizationSet& key_description,
                                                KeymasterKeyBlob* key_blob,
                                                AuthorizationSet* hw_enforced,
                                                AuthorizationSet* sw_enforced) const {
    assert(key_blob);

    keymaster_key_characteristics_t characteristics = {};
    keymaster_key_blob_t blob = {};
    keymaster_error_t error = km_device_->generate_key(km_device_, &key_description,
                                                               &blob, &characteristics);
    if (error != KM_ERROR_OK)
        return error;
    unique_ptr<uint8_t, Malloc_Delete> blob_deleter(const_cast<uint8_t*>(blob.key_material));
    key_blob->key_material = dup_buffer(blob.key_material, blob.key_material_size);
    key_blob->key_material_size = blob.key_material_size;

    ConvertCharacteristics(characteristics, hw_enforced, sw_enforced);
    keymaster_free_characteristics(&characteristics);
    return error;
}

template<>
keymaster_error_t
TKeymasterPassthroughEngine<keymaster1_device_t>::ImportKey(const AuthorizationSet& key_description,
                                              keymaster_key_format_t input_key_material_format,
                                              const KeymasterKeyBlob& input_key_material,
                                              KeymasterKeyBlob* output_key_blob,
                                              AuthorizationSet* hw_enforced,
                                              AuthorizationSet* sw_enforced) const {
    assert(output_key_blob);

    keymaster_key_characteristics_t* characteristics = {};
    const keymaster_blob_t input_key = {input_key_material.key_material,
                                        input_key_material.key_material_size};
    keymaster_key_blob_t blob = {};
    keymaster_error_t error = km_device_->import_key(km_device_, &key_description,
                                                     input_key_material_format, &input_key,
                                                     &blob, &characteristics);
    if (error != KM_ERROR_OK)
        return error;
    unique_ptr<uint8_t, Malloc_Delete> blob_deleter(const_cast<uint8_t*>(blob.key_material));

    *output_key_blob = KeymasterKeyBlob(blob);

    ConvertCharacteristics(*characteristics, hw_enforced, sw_enforced);
    keymaster_free_characteristics(characteristics);
    free(characteristics);
    return error;
}

template<>
keymaster_error_t
TKeymasterPassthroughEngine<keymaster2_device_t>::ImportKey(const AuthorizationSet& key_description,
                                              keymaster_key_format_t input_key_material_format,
                                              const KeymasterKeyBlob& input_key_material,
                                              KeymasterKeyBlob* output_key_blob,
                                              AuthorizationSet* hw_enforced,
                                              AuthorizationSet* sw_enforced) const {
    assert(output_key_blob);

    keymaster_key_characteristics_t characteristics = {};
    const keymaster_blob_t input_key = {input_key_material.key_material,
                                        input_key_material.key_material_size};
    keymaster_key_blob_t blob = {};
    keymaster_error_t error = km_device_->import_key(km_device_, &key_description,
                                                     input_key_material_format, &input_key,
                                                     &blob, &characteristics);
    if (error != KM_ERROR_OK)
        return error;
    unique_ptr<uint8_t, Malloc_Delete> blob_deleter(const_cast<uint8_t*>(blob.key_material));
    // TODO why duplicate the blob if we have ownership here anyway?
    output_key_blob->key_material = dup_buffer(blob.key_material, blob.key_material_size);
    output_key_blob->key_material_size = blob.key_material_size;

    ConvertCharacteristics(characteristics, hw_enforced, sw_enforced);
    keymaster_free_characteristics(&characteristics);
    return error;
}

typedef UniquePtr<KeymasterPassthroughEngine> engine_ptr_t;

engine_ptr_t
KeymasterPassthroughEngine::createInstance(const keymaster1_device_t* dev) {
    return engine_ptr_t(new TKeymasterPassthroughEngine<keymaster1_device_t>(dev));
}
engine_ptr_t
KeymasterPassthroughEngine::createInstance(const keymaster2_device_t* dev) {
    return engine_ptr_t(new TKeymasterPassthroughEngine<keymaster2_device_t>(dev));
}

}  // namespace keymaster
