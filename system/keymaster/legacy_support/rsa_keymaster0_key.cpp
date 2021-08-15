/*
 * Copyright 2015 The Android Open Source Project
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

#include <keymaster/legacy_support/rsa_keymaster0_key.h>

#include <memory>

#include <keymaster/android_keymaster_utils.h>
#include <keymaster/contexts/soft_keymaster_context.h>
#include <keymaster/km_openssl/openssl_utils.h>
#include <keymaster/legacy_support/keymaster0_engine.h>
#include <keymaster/logger.h>


using std::unique_ptr;

namespace keymaster {

RsaKeymaster0KeyFactory::RsaKeymaster0KeyFactory(const SoftwareKeyBlobMaker* blob_maker,
                                                 const Keymaster0Engine* engine)
    : RsaKeyFactory(blob_maker), engine_(engine) {}

keymaster_error_t RsaKeymaster0KeyFactory::GenerateKey(const AuthorizationSet& key_description,
                                                       KeymasterKeyBlob* key_blob,
                                                       AuthorizationSet* hw_enforced,
                                                       AuthorizationSet* sw_enforced) const {
    if (!key_blob || !hw_enforced || !sw_enforced)
        return KM_ERROR_OUTPUT_PARAMETER_NULL;

    uint64_t public_exponent;
    if (!key_description.GetTagValue(TAG_RSA_PUBLIC_EXPONENT, &public_exponent)) {
        LOG_E("%s", "No public exponent specified for RSA key generation");
        return KM_ERROR_INVALID_ARGUMENT;
    }

    uint32_t key_size;
    if (!key_description.GetTagValue(TAG_KEY_SIZE, &key_size)) {
        LOG_E("%s", "No key size specified for RSA key generation");
        return KM_ERROR_UNSUPPORTED_KEY_SIZE;
    }

    KeymasterKeyBlob key_material;
    if (!engine_->GenerateRsaKey(public_exponent, key_size, &key_material))
        return KM_ERROR_UNKNOWN_ERROR;

    // These tags are hardware-enforced.  Putting them in the hw_enforced set here will ensure that
    // blob_maker_->CreateKeyBlob doesn't put them in sw_enforced.
    hw_enforced->push_back(TAG_ALGORITHM, KM_ALGORITHM_RSA);
    hw_enforced->push_back(TAG_RSA_PUBLIC_EXPONENT, public_exponent);
    hw_enforced->push_back(TAG_KEY_SIZE, key_size);
    hw_enforced->push_back(TAG_ORIGIN, KM_ORIGIN_UNKNOWN);

    return blob_maker_.CreateKeyBlob(key_description, KM_ORIGIN_UNKNOWN, key_material, key_blob,
                                     hw_enforced, sw_enforced);
}

keymaster_error_t RsaKeymaster0KeyFactory::ImportKey(
    const AuthorizationSet& key_description, keymaster_key_format_t input_key_material_format,
    const KeymasterKeyBlob& input_key_material, KeymasterKeyBlob* output_key_blob,
    AuthorizationSet* hw_enforced, AuthorizationSet* sw_enforced) const {
    if (!output_key_blob || !hw_enforced || !sw_enforced)
        return KM_ERROR_OUTPUT_PARAMETER_NULL;

    AuthorizationSet authorizations;
    uint64_t public_exponent;
    uint32_t key_size;
    keymaster_error_t error =
        UpdateImportKeyDescription(key_description, input_key_material_format, input_key_material,
                                   &authorizations, &public_exponent, &key_size);
    if (error != KM_ERROR_OK)
        return error;

    KeymasterKeyBlob imported_hw_key;
    if (!engine_->ImportKey(input_key_material_format, input_key_material, &imported_hw_key))
        return KM_ERROR_UNKNOWN_ERROR;

    // These tags are hardware-enforced.  Putting them in the hw_enforced set here will ensure that
    // blob_maker_->CreateKeyBlob doesn't put them in sw_enforced.
    hw_enforced->push_back(TAG_ALGORITHM, KM_ALGORITHM_RSA);
    hw_enforced->push_back(TAG_RSA_PUBLIC_EXPONENT, public_exponent);
    hw_enforced->push_back(TAG_KEY_SIZE, key_size);
    hw_enforced->push_back(TAG_ORIGIN, KM_ORIGIN_UNKNOWN);

    return blob_maker_.CreateKeyBlob(authorizations, KM_ORIGIN_UNKNOWN, imported_hw_key,
                                     output_key_blob, hw_enforced, sw_enforced);
}

keymaster_error_t RsaKeymaster0KeyFactory::LoadKey(KeymasterKeyBlob&& key_material,
                                                   const AuthorizationSet& additional_params,
                                                   AuthorizationSet&& hw_enforced,
                                                   AuthorizationSet&& sw_enforced,
                                                   UniquePtr<Key>* key) const {
    if (!key)
        return KM_ERROR_OUTPUT_PARAMETER_NULL;

    if (sw_enforced.GetTagCount(TAG_ALGORITHM) == 1)
        return super::LoadKey(move(key_material), additional_params, move(hw_enforced),
                              move(sw_enforced), key);

    unique_ptr<RSA, RSA_Delete> rsa(engine_->BlobToRsaKey(key_material));
    if (!rsa)
        return KM_ERROR_UNKNOWN_ERROR;

    key->reset(new (std::nothrow)
                   RsaKeymaster0Key(rsa.release(), move(hw_enforced), move(sw_enforced), this));
    if (!(*key))
        return KM_ERROR_MEMORY_ALLOCATION_FAILED;

    (*key)->key_material() = move(key_material);
    return KM_ERROR_OK;
}

}  // namespace keymaster
