/*
 * Copyright 2017 The Android Open Source Project
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

#include <keymaster/attestation_record.h>
#include <keymaster/logger.h>
#include <keymaster/wrapped_key.h>

#include <openssl/asn1t.h>

#include <keymaster/android_keymaster_utils.h>
#include <keymaster/km_openssl/openssl_err.h>
#include <keymaster/km_openssl/openssl_utils.h>

namespace keymaster {

IMPLEMENT_ASN1_FUNCTIONS(KM_WRAPPED_KEY_DESCRIPTION);
IMPLEMENT_ASN1_FUNCTIONS(KM_WRAPPED_KEY);

struct KM_WRAPPED_KEY_Delete {
    void operator()(KM_WRAPPED_KEY* p) { KM_WRAPPED_KEY_free(p); }
};

struct KM_WRAPPED_KEY_DESCRIPTION_Delete {
    void operator()(KM_WRAPPED_KEY_DESCRIPTION* p) { KM_WRAPPED_KEY_DESCRIPTION_free(p); }
};

// DER encode a wrapped key for secure import
keymaster_error_t build_wrapped_key(const KeymasterKeyBlob& transit_key, const KeymasterBlob& iv,
                                    keymaster_key_format_t key_format,
                                    const KeymasterKeyBlob& secure_key, const KeymasterBlob& tag,
                                    const AuthorizationSet& auth_set,
                                    KeymasterKeyBlob* der_wrapped_key) {
    UniquePtr<KM_WRAPPED_KEY, KM_WRAPPED_KEY_Delete> wrapped_key(KM_WRAPPED_KEY_new());
    if (!wrapped_key.get()) return KM_ERROR_MEMORY_ALLOCATION_FAILED;

    if (!ASN1_OCTET_STRING_set(wrapped_key->transit_key, transit_key.key_material,
                               transit_key.key_material_size) ||
        !ASN1_OCTET_STRING_set(wrapped_key->iv, iv.data, iv.data_length) ||
        !ASN1_OCTET_STRING_set(wrapped_key->secure_key, secure_key.key_material,
                               secure_key.key_material_size) ||
        !ASN1_OCTET_STRING_set(wrapped_key->tag, tag.data, tag.data_length) ||
        !ASN1_INTEGER_set(wrapped_key->wrapped_key_description->key_format, key_format)) {
        return TranslateLastOpenSslError();
    }

    auto err = build_auth_list(auth_set, wrapped_key->wrapped_key_description->auth_list);
    if (err != KM_ERROR_OK) {
        return err;
    }

    int len = i2d_KM_WRAPPED_KEY(wrapped_key.get(), nullptr);
    if (len < 0) {
        return TranslateLastOpenSslError();
    }
    if (!der_wrapped_key->Reset(len)) {
        return KM_ERROR_MEMORY_ALLOCATION_FAILED;
    }

    uint8_t* p = der_wrapped_key->writable_data();
    len = i2d_KM_WRAPPED_KEY(wrapped_key.get(), &p);
    if (len < 0) {
        return TranslateLastOpenSslError();
    }

    return KM_ERROR_OK;
}

// Parse the DER-encoded wrapped key format
keymaster_error_t parse_wrapped_key(const KeymasterKeyBlob& wrapped_key, KeymasterBlob* iv,
                                    KeymasterKeyBlob* transit_key, KeymasterKeyBlob* secure_key,
                                    KeymasterBlob* tag, AuthorizationSet* auth_list,
                                    keymaster_key_format_t* key_format,
                                    KeymasterBlob* wrapped_key_description) {
    if (!iv || !transit_key || !secure_key || !tag || !auth_list || !key_format ||
        !wrapped_key_description) {
        return KM_ERROR_UNEXPECTED_NULL_POINTER;
    }

    const uint8_t* tmp = wrapped_key.key_material;
    UniquePtr<KM_WRAPPED_KEY, KM_WRAPPED_KEY_Delete> record(
        d2i_KM_WRAPPED_KEY(nullptr, &tmp, wrapped_key.key_material_size));
    if (!record.get()) return TranslateLastOpenSslError();

    *iv = KeymasterBlob(record->iv->data, record->iv->length);
    if (record->iv->data && !iv->data) {
        return KM_ERROR_MEMORY_ALLOCATION_FAILED;
    }

    *transit_key = KeymasterKeyBlob(record->transit_key->data, record->transit_key->length);
    if (record->transit_key->data && !transit_key->key_material) {
        return KM_ERROR_MEMORY_ALLOCATION_FAILED;
    }

    *secure_key = KeymasterKeyBlob(record->secure_key->data, record->secure_key->length);
    if (record->secure_key->data && !secure_key->key_material) {
        return KM_ERROR_MEMORY_ALLOCATION_FAILED;
    }

    *tag = KeymasterBlob(record->tag->data, record->tag->length);
    if (record->tag->data && !tag->data) {
        return KM_ERROR_MEMORY_ALLOCATION_FAILED;
    }

    // Re-serialize the wrapped key description
    int len = i2d_KM_WRAPPED_KEY_DESCRIPTION(record->wrapped_key_description, nullptr);
    if (len < 0) {
        return TranslateLastOpenSslError();
    }
    if (!wrapped_key_description->Reset(len)) {
        return KM_ERROR_MEMORY_ALLOCATION_FAILED;
    }
    uint8_t* p = wrapped_key_description->writable_data();
    if (i2d_KM_WRAPPED_KEY_DESCRIPTION(record->wrapped_key_description, &p) < 0) {
        return TranslateLastOpenSslError();
    }

    *key_format = static_cast<keymaster_key_format_t>(
        ASN1_INTEGER_get(record->wrapped_key_description->key_format));
    return extract_auth_list(record->wrapped_key_description->auth_list, auth_list);
}

}  // namespace keymaster
