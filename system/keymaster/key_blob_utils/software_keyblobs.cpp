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

#include <keymaster/key_blob_utils/software_keyblobs.h>

#include <stdint.h>

#include <hardware/keymaster_defs.h>

#include <keymaster/android_keymaster_utils.h>
#include <keymaster/authorization_set.h>
#include <keymaster/key.h>
#include <keymaster/key_blob_utils/auth_encrypted_key_blob.h>
#include <keymaster/key_blob_utils/integrity_assured_key_blob.h>
#include <keymaster/key_blob_utils/ocb_utils.h>
#include <keymaster/km_openssl/openssl_utils.h>
#include <keymaster/km_openssl/openssl_err.h>
#include <keymaster/logger.h>
#include <keymaster/UniquePtr.h>

#include <openssl/aes.h>

namespace keymaster {

static uint8_t SWROT[2] = {'S', 'W'};
KeymasterBlob softwareRootOfTrust(SWROT);

namespace {

bool UpgradeIntegerTag(keymaster_tag_t tag, uint32_t value, AuthorizationSet* set,
                              bool* set_changed) {
    int index = set->find(tag);
    if (index == -1) {
        keymaster_key_param_t param;
        param.tag = tag;
        param.integer = value;
        set->push_back(param);
        *set_changed = true;
        return true;
    }

    if (set->params[index].integer > value)
        return false;

    if (set->params[index].integer != value) {
        set->params[index].integer = value;
        *set_changed = true;
    }
    return true;
}

keymaster_error_t TranslateAuthorizationSetError(AuthorizationSet::Error err) {
    switch (err) {
    case AuthorizationSet::OK:
        return KM_ERROR_OK;
    case AuthorizationSet::ALLOCATION_FAILURE:
        return KM_ERROR_MEMORY_ALLOCATION_FAILED;
    case AuthorizationSet::MALFORMED_DATA:
        return KM_ERROR_UNKNOWN_ERROR;
    }
    return KM_ERROR_OK;
}

} // anonymous namespace

keymaster_error_t BuildHiddenAuthorizations(const AuthorizationSet& input_set,
                                            AuthorizationSet* hidden,
                                            const KeymasterBlob& root_of_trust) {
    keymaster_blob_t entry;
    if (input_set.GetTagValue(TAG_APPLICATION_ID, &entry))
        hidden->push_back(TAG_APPLICATION_ID, entry.data, entry.data_length);
    if (input_set.GetTagValue(TAG_APPLICATION_DATA, &entry))
        hidden->push_back(TAG_APPLICATION_DATA, entry.data, entry.data_length);

    hidden->push_back(TAG_ROOT_OF_TRUST, root_of_trust);

    return TranslateAuthorizationSetError(hidden->is_valid());
}

keymaster_error_t FakeKeyAuthorizations(EVP_PKEY* pubkey,
                                        AuthorizationSet* hw_enforced,
                                        AuthorizationSet* sw_enforced) {
    hw_enforced->Clear();
    sw_enforced->Clear();

    switch (EVP_PKEY_type(pubkey->type)) {
    case EVP_PKEY_RSA: {
        hw_enforced->push_back(TAG_ALGORITHM, KM_ALGORITHM_RSA);
        hw_enforced->push_back(TAG_DIGEST, KM_DIGEST_NONE);
        hw_enforced->push_back(TAG_DIGEST, KM_DIGEST_MD5);
        hw_enforced->push_back(TAG_DIGEST, KM_DIGEST_SHA1);
        hw_enforced->push_back(TAG_DIGEST, KM_DIGEST_SHA_2_224);
        hw_enforced->push_back(TAG_DIGEST, KM_DIGEST_SHA_2_256);
        hw_enforced->push_back(TAG_DIGEST, KM_DIGEST_SHA_2_384);
        hw_enforced->push_back(TAG_DIGEST, KM_DIGEST_SHA_2_512);
        hw_enforced->push_back(TAG_PADDING, KM_PAD_NONE);
        hw_enforced->push_back(TAG_PADDING, KM_PAD_RSA_PKCS1_1_5_SIGN);
        hw_enforced->push_back(TAG_PADDING, KM_PAD_RSA_PKCS1_1_5_ENCRYPT);
        hw_enforced->push_back(TAG_PADDING, KM_PAD_RSA_PSS);
        hw_enforced->push_back(TAG_PADDING, KM_PAD_RSA_OAEP);

        sw_enforced->push_back(TAG_PURPOSE, KM_PURPOSE_SIGN);
        sw_enforced->push_back(TAG_PURPOSE, KM_PURPOSE_VERIFY);
        sw_enforced->push_back(TAG_PURPOSE, KM_PURPOSE_ENCRYPT);
        sw_enforced->push_back(TAG_PURPOSE, KM_PURPOSE_DECRYPT);

        RSA_Ptr rsa(EVP_PKEY_get1_RSA(pubkey));
        if (!rsa)
            return TranslateLastOpenSslError();
        hw_enforced->push_back(TAG_KEY_SIZE, RSA_size(rsa.get()) * 8);
        uint64_t public_exponent = BN_get_word(rsa->e);
        if (public_exponent == 0xffffffffL)
            return KM_ERROR_INVALID_KEY_BLOB;
        hw_enforced->push_back(TAG_RSA_PUBLIC_EXPONENT, public_exponent);
        break;
    }

    case EVP_PKEY_EC: {
        hw_enforced->push_back(TAG_ALGORITHM, KM_ALGORITHM_RSA);
        hw_enforced->push_back(TAG_DIGEST, KM_DIGEST_NONE);
        hw_enforced->push_back(TAG_DIGEST, KM_DIGEST_MD5);
        hw_enforced->push_back(TAG_DIGEST, KM_DIGEST_SHA1);
        hw_enforced->push_back(TAG_DIGEST, KM_DIGEST_SHA_2_224);
        hw_enforced->push_back(TAG_DIGEST, KM_DIGEST_SHA_2_256);
        hw_enforced->push_back(TAG_DIGEST, KM_DIGEST_SHA_2_384);
        hw_enforced->push_back(TAG_DIGEST, KM_DIGEST_SHA_2_512);

        sw_enforced->push_back(TAG_PURPOSE, KM_PURPOSE_SIGN);
        sw_enforced->push_back(TAG_PURPOSE, KM_PURPOSE_VERIFY);

        UniquePtr<EC_KEY, EC_KEY_Delete> ec_key(EVP_PKEY_get1_EC_KEY(pubkey));
        if (!ec_key.get())
            return TranslateLastOpenSslError();
        size_t key_size_bits;
        keymaster_error_t error =
            ec_get_group_size(EC_KEY_get0_group(ec_key.get()), &key_size_bits);
        if (error != KM_ERROR_OK)
            return error;
        hw_enforced->push_back(TAG_KEY_SIZE, key_size_bits);
        break;
    }

    default:
        return KM_ERROR_UNSUPPORTED_ALGORITHM;
    }

    sw_enforced->push_back(TAG_ALL_USERS);
    sw_enforced->push_back(TAG_NO_AUTH_REQUIRED);

    return KM_ERROR_OK;
}


// Note: This parsing code in below is from system/security/softkeymaster/keymaster_openssl.cpp's
// unwrap_key function, modified for the preferred function signature and formatting.  It does some
// odd things, but they have been left unchanged to avoid breaking compatibility.
static const uint8_t SOFT_KEY_MAGIC[] = {'P', 'K', '#', '8'};
keymaster_error_t ParseOldSoftkeymasterBlob(
    const KeymasterKeyBlob& blob, KeymasterKeyBlob* key_material, AuthorizationSet* hw_enforced,
    AuthorizationSet* sw_enforced) {
    long publicLen = 0;
    long privateLen = 0;
    const uint8_t* p = blob.key_material;
    const uint8_t* end = blob.key_material + blob.key_material_size;

    int type = 0;
    ptrdiff_t min_size =
        sizeof(SOFT_KEY_MAGIC) + sizeof(type) + sizeof(publicLen) + 1 + sizeof(privateLen) + 1;
    if (end - p < min_size) {
        LOG_W("key blob appears to be truncated (if an old SW key)", 0);
        return KM_ERROR_INVALID_KEY_BLOB;
    }

    if (memcmp(p, SOFT_KEY_MAGIC, sizeof(SOFT_KEY_MAGIC)) != 0)
        return KM_ERROR_INVALID_KEY_BLOB;
    p += sizeof(SOFT_KEY_MAGIC);

    for (size_t i = 0; i < sizeof(type); i++)
        type = (type << 8) | *p++;

    for (size_t i = 0; i < sizeof(type); i++)
        publicLen = (publicLen << 8) | *p++;

    if (p + publicLen > end) {
        LOG_W("public key length encoding error: size=%ld, end=%td", publicLen, end - p);
        return KM_ERROR_INVALID_KEY_BLOB;
    }
    p += publicLen;

    if (end - p < 2) {
        LOG_W("key blob appears to be truncated (if an old SW key)", 0);
        return KM_ERROR_INVALID_KEY_BLOB;
    }

    for (size_t i = 0; i < sizeof(type); i++)
        privateLen = (privateLen << 8) | *p++;

    if (p + privateLen > end) {
        LOG_W("private key length encoding error: size=%ld, end=%td", privateLen, end - p);
        return KM_ERROR_INVALID_KEY_BLOB;
    }

    // Just to be sure, make sure that the ASN.1 structure parses correctly.  We don't actually use
    // the EVP_PKEY here.
    const uint8_t* key_start = p;
    EVP_PKEY_Ptr pkey(d2i_PrivateKey(type, nullptr, &p, privateLen));
    if (pkey.get() == nullptr) {
        LOG_W("Failed to parse PKCS#8 key material (if old SW key)", 0);
        return KM_ERROR_INVALID_KEY_BLOB;
    }

    // All auths go into sw_enforced, including those that would be HW-enforced if we were faking
    // auths for a HW-backed key.
    hw_enforced->Clear();
    keymaster_error_t error = FakeKeyAuthorizations(pkey.get(), sw_enforced, sw_enforced);
    if (error != KM_ERROR_OK)
        return error;

    if (!key_material->Reset(privateLen))
        return KM_ERROR_MEMORY_ALLOCATION_FAILED;
    memcpy(key_material->writable_data(), key_start, privateLen);

    return KM_ERROR_OK;
}

static uint8_t master_key_bytes[AES_BLOCK_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const KeymasterKeyBlob MASTER_KEY(master_key_bytes, array_length(master_key_bytes));

keymaster_error_t ParseOcbAuthEncryptedBlob(const KeymasterKeyBlob& blob,
                                            const AuthorizationSet& hidden,
                                            KeymasterKeyBlob* key_material,
                                            AuthorizationSet* hw_enforced,
                                            AuthorizationSet* sw_enforced) {
    Buffer nonce, tag;
    KeymasterKeyBlob encrypted_key_material;
    keymaster_error_t error = DeserializeAuthEncryptedBlob(blob, &encrypted_key_material,
                                                           hw_enforced, sw_enforced, &nonce, &tag);
    if (error != KM_ERROR_OK)
        return error;

    if (nonce.available_read() != OCB_NONCE_LENGTH || tag.available_read() != OCB_TAG_LENGTH)
        return KM_ERROR_INVALID_KEY_BLOB;

    return OcbDecryptKey(*hw_enforced, *sw_enforced, hidden, MASTER_KEY, encrypted_key_material,
                         nonce, tag, key_material);
}

keymaster_error_t SetKeyBlobAuthorizations(const AuthorizationSet& key_description,
                                           keymaster_key_origin_t origin, uint32_t os_version,
                                           uint32_t os_patchlevel, AuthorizationSet* hw_enforced,
                                           AuthorizationSet* sw_enforced) {
    sw_enforced->Clear();

    for (auto& entry : key_description) {
        switch (entry.tag) {
        // These cannot be specified by the client.
        case KM_TAG_ROOT_OF_TRUST:
        case KM_TAG_ORIGIN:
            LOG_E("Root of trust and origin tags may not be specified", 0);
            return KM_ERROR_INVALID_TAG;

        // These don't work.
        case KM_TAG_ROLLBACK_RESISTANT:
            LOG_E("KM_TAG_ROLLBACK_RESISTANT not supported", 0);
            return KM_ERROR_UNSUPPORTED_TAG;

        // These are hidden.
        case KM_TAG_APPLICATION_ID:
        case KM_TAG_APPLICATION_DATA:
            break;

        // Everything else we just copy into sw_enforced, unless the KeyFactory has placed it in
        // hw_enforced, in which case we defer to its decision.
        default:
            if (hw_enforced->GetTagCount(entry.tag) == 0)
                sw_enforced->push_back(entry);
            break;
        }
    }

    sw_enforced->push_back(TAG_CREATION_DATETIME, java_time(time(nullptr)));
    sw_enforced->push_back(TAG_ORIGIN, origin);
    sw_enforced->push_back(TAG_OS_VERSION, os_version);
    sw_enforced->push_back(TAG_OS_PATCHLEVEL, os_patchlevel);

    return TranslateAuthorizationSetError(sw_enforced->is_valid());
}


keymaster_error_t UpgradeSoftKeyBlob(const UniquePtr<Key>& key,
                                 const uint32_t os_version, const uint32_t os_patchlevel,
                                 const AuthorizationSet& upgrade_params,
                                 KeymasterKeyBlob* upgraded_key) {
    bool set_changed = false;

    if (os_version == 0) {
        // We need to allow "upgrading" OS version to zero, to support upgrading from proper
        // numbered releases to unnumbered development and preview releases.

        int key_os_version_pos = key->sw_enforced().find(TAG_OS_VERSION);
        if (key_os_version_pos != -1) {
            uint32_t key_os_version = key->sw_enforced()[key_os_version_pos].integer;
            if (key_os_version != 0) {
                key->sw_enforced()[key_os_version_pos].integer = os_version;
                set_changed = true;
            }
        }
    }

    if (!UpgradeIntegerTag(TAG_OS_VERSION, os_version, &key->sw_enforced(), &set_changed) ||
        !UpgradeIntegerTag(TAG_OS_PATCHLEVEL, os_patchlevel, &key->sw_enforced(), &set_changed))
        // One of the version fields would have been a downgrade. Not allowed.
        return KM_ERROR_INVALID_ARGUMENT;

    if (!set_changed)
        // Dont' need an upgrade.
        return KM_ERROR_OK;

    AuthorizationSet hidden;
    auto error = BuildHiddenAuthorizations(upgrade_params, &hidden, softwareRootOfTrust);
    if (error != KM_ERROR_OK)
        return error;
    return SerializeIntegrityAssuredBlob(key->key_material(), hidden, key->hw_enforced(),
                                         key->sw_enforced(), upgraded_key);
}

} // namespace keymaster
