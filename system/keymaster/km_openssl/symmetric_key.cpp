/*
 * Copyright 2014 The Android Open Source Project
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

#include <keymaster/km_openssl/symmetric_key.h>

#include <assert.h>

#include <openssl/err.h>
#include <openssl/rand.h>

#include <keymaster/android_keymaster_utils.h>
#include <keymaster/keymaster_context.h>
#include <keymaster/km_openssl/aes_key.h>
#include <keymaster/km_openssl/hmac_key.h>
#include <keymaster/km_openssl/openssl_err.h>
#include <keymaster/logger.h>

namespace keymaster {

keymaster_error_t SymmetricKeyFactory::GenerateKey(const AuthorizationSet& key_description,
                                                   KeymasterKeyBlob* key_blob,
                                                   AuthorizationSet* hw_enforced,
                                                   AuthorizationSet* sw_enforced) const {
    if (!key_blob || !hw_enforced || !sw_enforced)
        return KM_ERROR_OUTPUT_PARAMETER_NULL;

    uint32_t key_size_bits;
    if (!key_description.GetTagValue(TAG_KEY_SIZE, &key_size_bits) ||
        !key_size_supported(key_size_bits))
        return KM_ERROR_UNSUPPORTED_KEY_SIZE;

    keymaster_error_t error = validate_algorithm_specific_new_key_params(key_description);
    if (error != KM_ERROR_OK)
        return error;

    size_t key_data_size = key_size_bytes(key_size_bits);
    KeymasterKeyBlob key_material(key_data_size);
    if (!key_material.key_material)
        return KM_ERROR_MEMORY_ALLOCATION_FAILED;

    error = random_source_.GenerateRandom(key_material.writable_data(), key_data_size);
    if (error != KM_ERROR_OK) {
        LOG_E("Error generating %d bit symmetric key", key_size_bits);
        return error;
    }

    return blob_maker_.CreateKeyBlob(key_description, KM_ORIGIN_GENERATED, key_material, key_blob,
                                     hw_enforced, sw_enforced);
}

keymaster_error_t SymmetricKeyFactory::ImportKey(const AuthorizationSet& key_description,
                                                 keymaster_key_format_t input_key_material_format,
                                                 const KeymasterKeyBlob& input_key_material,
                                                 KeymasterKeyBlob* output_key_blob,
                                                 AuthorizationSet* hw_enforced,
                                                 AuthorizationSet* sw_enforced) const {
    if (!output_key_blob || !hw_enforced || !sw_enforced)
        return KM_ERROR_OUTPUT_PARAMETER_NULL;

    AuthorizationSet authorizations(key_description);

    uint32_t key_bits;
    if (!authorizations.GetTagValue(TAG_KEY_SIZE, &key_bits)) {
        // Determine key size if not provided.
        key_bits = key_size_bits(input_key_material.key_material_size);
        authorizations.push_back(TAG_KEY_SIZE, key_bits);
    }

    keymaster_error_t error = validate_algorithm_specific_new_key_params(key_description);
    if (error != KM_ERROR_OK) return error;
    if (!key_size_supported(key_bits)) return KM_ERROR_UNSUPPORTED_KEY_SIZE;
    if (input_key_material_format != KM_KEY_FORMAT_RAW) return KM_ERROR_UNSUPPORTED_KEY_FORMAT;

    if (key_bits != key_size_bits(input_key_material.key_material_size)) {
        LOG_E("Expected %d-bit key data but got %d-bit key", key_bits,
              key_size_bits(input_key_material.key_material_size));
        return KM_ERROR_INVALID_KEY_BLOB;
    }

    return blob_maker_.CreateKeyBlob(authorizations, KM_ORIGIN_IMPORTED, input_key_material,
                                   output_key_blob, hw_enforced, sw_enforced);
}

static const keymaster_key_format_t supported_import_formats[] = {KM_KEY_FORMAT_RAW};
const keymaster_key_format_t*
SymmetricKeyFactory::SupportedImportFormats(size_t* format_count) const {
    *format_count = array_length(supported_import_formats);
    return supported_import_formats;
}

SymmetricKey::SymmetricKey(KeymasterKeyBlob&& key_material,
                           AuthorizationSet&& hw_enforced, AuthorizationSet&& sw_enforced,
                           const KeyFactory* key_factory)
    : Key(move(hw_enforced), move(sw_enforced), key_factory) {
    key_material_ = move(key_material);
}

SymmetricKey::~SymmetricKey() {}

}  // namespace keymaster
