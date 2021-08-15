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

#include <keymaster/contexts/pure_soft_keymaster_context.h>

#include <memory>

#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/x509v3.h>

#include <keymaster/android_keymaster_utils.h>
#include <keymaster/key_blob_utils/auth_encrypted_key_blob.h>
#include <keymaster/key_blob_utils/integrity_assured_key_blob.h>
#include <keymaster/key_blob_utils/ocb_utils.h>
#include <keymaster/key_blob_utils/software_keyblobs.h>
#include <keymaster/km_openssl/aes_key.h>
#include <keymaster/km_openssl/asymmetric_key.h>
#include <keymaster/km_openssl/attestation_utils.h>
#include <keymaster/km_openssl/ec_key_factory.h>
#include <keymaster/km_openssl/hmac_key.h>
#include <keymaster/km_openssl/openssl_err.h>
#include <keymaster/km_openssl/openssl_utils.h>
#include <keymaster/km_openssl/rsa_key_factory.h>
#include <keymaster/km_openssl/soft_keymaster_enforcement.h>
#include <keymaster/km_openssl/triple_des_key.h>
#include <keymaster/logger.h>
#include <keymaster/operation.h>
#include <keymaster/wrapped_key.h>

#include "soft_attestation_cert.h"

using std::unique_ptr;

namespace keymaster {

PureSoftKeymasterContext::PureSoftKeymasterContext()
    : rsa_factory_(new RsaKeyFactory(this)), ec_factory_(new EcKeyFactory(this)),
      aes_factory_(new AesKeyFactory(this, this)),
      tdes_factory_(new TripleDesKeyFactory(this, this)),
      hmac_factory_(new HmacKeyFactory(this, this)), os_version_(0), os_patchlevel_(0),
      soft_keymaster_enforcement_(64, 64) {}

PureSoftKeymasterContext::~PureSoftKeymasterContext() {}

keymaster_error_t PureSoftKeymasterContext::SetSystemVersion(uint32_t os_version,
                                                         uint32_t os_patchlevel) {
    os_version_ = os_version;
    os_patchlevel_ = os_patchlevel;
    return KM_ERROR_OK;
}

void PureSoftKeymasterContext::GetSystemVersion(uint32_t* os_version, uint32_t* os_patchlevel) const {
    *os_version = os_version_;
    *os_patchlevel = os_patchlevel_;
}

KeyFactory* PureSoftKeymasterContext::GetKeyFactory(keymaster_algorithm_t algorithm) const {
    switch (algorithm) {
    case KM_ALGORITHM_RSA:
        return rsa_factory_.get();
    case KM_ALGORITHM_EC:
        return ec_factory_.get();
    case KM_ALGORITHM_AES:
        return aes_factory_.get();
    case KM_ALGORITHM_TRIPLE_DES:
        return tdes_factory_.get();
    case KM_ALGORITHM_HMAC:
        return hmac_factory_.get();
    default:
        return nullptr;
    }
}

static keymaster_algorithm_t supported_algorithms[] = {KM_ALGORITHM_RSA, KM_ALGORITHM_EC,
                                                       KM_ALGORITHM_AES, KM_ALGORITHM_HMAC};

keymaster_algorithm_t*
PureSoftKeymasterContext::GetSupportedAlgorithms(size_t* algorithms_count) const {
    *algorithms_count = array_length(supported_algorithms);
    return supported_algorithms;
}

OperationFactory* PureSoftKeymasterContext::GetOperationFactory(keymaster_algorithm_t algorithm,
                                                            keymaster_purpose_t purpose) const {
    KeyFactory* key_factory = GetKeyFactory(algorithm);
    if (!key_factory)
        return nullptr;
    return key_factory->GetOperationFactory(purpose);
}

keymaster_error_t PureSoftKeymasterContext::CreateKeyBlob(const AuthorizationSet& key_description,
                                                      const keymaster_key_origin_t origin,
                                                      const KeymasterKeyBlob& key_material,
                                                      KeymasterKeyBlob* blob,
                                                      AuthorizationSet* hw_enforced,
                                                      AuthorizationSet* sw_enforced) const {
    keymaster_error_t error = SetKeyBlobAuthorizations(key_description, origin, os_version_,
                                                       os_patchlevel_, hw_enforced, sw_enforced);
    if (error != KM_ERROR_OK)
        return error;

    AuthorizationSet hidden;
    error = BuildHiddenAuthorizations(key_description, &hidden, softwareRootOfTrust);
    if (error != KM_ERROR_OK)
        return error;

    return SerializeIntegrityAssuredBlob(key_material, hidden, *hw_enforced, *sw_enforced, blob);
}

keymaster_error_t PureSoftKeymasterContext::UpgradeKeyBlob(const KeymasterKeyBlob& key_to_upgrade,
                                                       const AuthorizationSet& upgrade_params,
                                                       KeymasterKeyBlob* upgraded_key) const {
    UniquePtr<Key> key;
    keymaster_error_t error = ParseKeyBlob(key_to_upgrade, upgrade_params, &key);
    if (error != KM_ERROR_OK)
        return error;

    return UpgradeSoftKeyBlob(key, os_version_, os_patchlevel_, upgrade_params, upgraded_key);
}

keymaster_error_t PureSoftKeymasterContext::ParseKeyBlob(const KeymasterKeyBlob& blob,
                                                         const AuthorizationSet& additional_params,
                                                         UniquePtr<Key>* key) const {
    // This is a little bit complicated.
    //
    // The SoftKeymasterContext has to handle a lot of different kinds of key blobs.
    //
    // 1.  New keymaster1 software key blobs.  These are integrity-assured but not encrypted.  The
    //     raw key material and auth sets should be extracted and returned.  This is the kind
    //     produced by this context when the KeyFactory doesn't use keymaster0 to back the keys.
    //
    // 2.  Old keymaster1 software key blobs.  These are OCB-encrypted with an all-zero master key.
    //     They should be decrypted and the key material and auth sets extracted and returned.
    //
    // 3.  Old keymaster0 software key blobs.  These are raw key material with a small header tacked
    //     on the front.  They don't have auth sets, so reasonable defaults are generated and
    //     returned along with the raw key material.
    //
    // Determining what kind of blob has arrived is somewhat tricky.  What helps is that
    // integrity-assured and OCB-encrypted blobs are self-consistent and effectively impossible to
    // parse as anything else.  Old keymaster0 software key blobs have a header.  It's reasonably
    // unlikely that hardware keys would have the same header.  So anything that is neither
    // integrity-assured nor OCB-encrypted and lacks the old software key header is assumed to be
    // keymaster0 hardware.

    AuthorizationSet hw_enforced;
    AuthorizationSet sw_enforced;
    KeymasterKeyBlob key_material;
    keymaster_error_t error;

    auto constructKey = [&, this] () mutable -> keymaster_error_t {
        // GetKeyFactory
        if (error != KM_ERROR_OK) return error;
        keymaster_algorithm_t algorithm;
        if (!hw_enforced.GetTagValue(TAG_ALGORITHM, &algorithm) &&
            !sw_enforced.GetTagValue(TAG_ALGORITHM, &algorithm)) {
            return KM_ERROR_INVALID_ARGUMENT;
        }
        auto factory = GetKeyFactory(algorithm);
        return factory->LoadKey(move(key_material), additional_params, move(hw_enforced),
                                move(sw_enforced), key);
    };

    AuthorizationSet hidden;
    error = BuildHiddenAuthorizations(additional_params, &hidden, softwareRootOfTrust);
    if (error != KM_ERROR_OK)
        return error;

    // Assume it's an integrity-assured blob (new software-only blob, or new keymaster0-backed
    // blob).
    error = DeserializeIntegrityAssuredBlob(blob, hidden, &key_material, &hw_enforced, &sw_enforced);
    if (error != KM_ERROR_INVALID_KEY_BLOB)
        return constructKey();

    // Wasn't an integrity-assured blob.  Maybe it's an OCB-encrypted blob.
    error = ParseOcbAuthEncryptedBlob(blob, hidden, &key_material, &hw_enforced, &sw_enforced);
    if (error == KM_ERROR_OK)
        LOG_D("Parsed an old keymaster1 software key", 0);
    if (error != KM_ERROR_INVALID_KEY_BLOB)
        return constructKey();

    // Wasn't an OCB-encrypted blob.  Maybe it's an old softkeymaster blob.
    error = ParseOldSoftkeymasterBlob(blob, &key_material, &hw_enforced, &sw_enforced);
    if (error == KM_ERROR_OK)
        LOG_D("Parsed an old sofkeymaster key", 0);

    return constructKey();
}

keymaster_error_t PureSoftKeymasterContext::DeleteKey(const KeymasterKeyBlob& /* blob */) const {
    // Nothing to do for software-only contexts.
    return KM_ERROR_OK;
}

keymaster_error_t PureSoftKeymasterContext::DeleteAllKeys() const {
    return KM_ERROR_OK;
}

keymaster_error_t PureSoftKeymasterContext::AddRngEntropy(const uint8_t* buf, size_t length) const {
    // XXX TODO according to boringssl openssl/rand.h RAND_add is deprecated and does
    // nothing
    RAND_add(buf, length, 0 /* Don't assume any entropy is added to the pool. */);
    return KM_ERROR_OK;
}

keymaster_error_t PureSoftKeymasterContext::GenerateAttestation(const Key& key,
                                      const AuthorizationSet& attest_params,
                                      CertChainPtr* cert_chain) const {

    keymaster_error_t error = KM_ERROR_OK;
    keymaster_algorithm_t key_algorithm;
    if (!key.authorizations().GetTagValue(TAG_ALGORITHM, &key_algorithm)) {
        return KM_ERROR_UNKNOWN_ERROR;
    }

    if ((key_algorithm != KM_ALGORITHM_RSA && key_algorithm != KM_ALGORITHM_EC))
        return KM_ERROR_INCOMPATIBLE_ALGORITHM;

    // We have established that the given key has the correct algorithm, and because this is the
    // SoftKeymasterContext we can assume that the Key is an AsymmetricKey. So we can downcast.
    const AsymmetricKey& asymmetric_key = static_cast<const AsymmetricKey&>(key);

    auto attestation_chain = getAttestationChain(key_algorithm, &error);
    if (error != KM_ERROR_OK) return error;

    auto attestation_key = getAttestationKey(key_algorithm, &error);
    if (error != KM_ERROR_OK) return error;

    return generate_attestation(asymmetric_key, attest_params,
            *attestation_chain, *attestation_key, *this, cert_chain);
}

static keymaster_error_t TranslateAuthorizationSetError(AuthorizationSet::Error err) {
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

keymaster_error_t PureSoftKeymasterContext::UnwrapKey(
    const KeymasterKeyBlob& wrapped_key_blob, const KeymasterKeyBlob& wrapping_key_blob,
    const AuthorizationSet& /* wrapping_key_params */, const KeymasterKeyBlob& masking_key,
    AuthorizationSet* wrapped_key_params, keymaster_key_format_t* wrapped_key_format,
    KeymasterKeyBlob* wrapped_key_material) const {
    keymaster_error_t error = KM_ERROR_OK;

    if (!wrapped_key_material) return KM_ERROR_UNEXPECTED_NULL_POINTER;

    // Parse wrapped key data
    KeymasterBlob iv;
    KeymasterKeyBlob transit_key;
    KeymasterKeyBlob secure_key;
    KeymasterBlob tag;
    KeymasterBlob wrapped_key_description;
    error = parse_wrapped_key(wrapped_key_blob, &iv, &transit_key, &secure_key, &tag,
                              wrapped_key_params, wrapped_key_format, &wrapped_key_description);
    if (error != KM_ERROR_OK) return error;

    UniquePtr<Key> key;
    auto wrapping_key_params = AuthorizationSetBuilder()
                                   .RsaEncryptionKey(2048, 65537)
                                   .Digest(KM_DIGEST_SHA1)
                                   .Padding(KM_PAD_RSA_OAEP)
                                   .Authorization(TAG_PURPOSE, KM_PURPOSE_WRAP)
                                   .build();
    error = ParseKeyBlob(wrapping_key_blob, wrapping_key_params, &key);
    if (error != KM_ERROR_OK) return error;

    // Ensure the wrapping key has the right purpose
    if (!key->hw_enforced().Contains(TAG_PURPOSE, KM_PURPOSE_WRAP) &&
        !key->sw_enforced().Contains(TAG_PURPOSE, KM_PURPOSE_WRAP)) {
        return KM_ERROR_INCOMPATIBLE_PURPOSE;
    }

    auto operation_factory = GetOperationFactory(KM_ALGORITHM_RSA, KM_PURPOSE_DECRYPT);
    if (!operation_factory) return KM_ERROR_UNKNOWN_ERROR;

    AuthorizationSet out_params;
    OperationPtr operation(
        operation_factory->CreateOperation(move(*key), wrapping_key_params, &error));
    if (!operation.get()) return error;

    error = operation->Begin(wrapping_key_params, &out_params);
    if (error != KM_ERROR_OK) return error;

    Buffer input;
    Buffer output;
    if (!input.Reinitialize(transit_key.key_material, transit_key.key_material_size)) {
        return KM_ERROR_MEMORY_ALLOCATION_FAILED;
    }

    error = operation->Finish(wrapping_key_params, input, Buffer() /* signature */, &out_params,
                              &output);
    if (error != KM_ERROR_OK) return error;

    // decrypt the encrypted key material with the transit key
    KeymasterKeyBlob key_material = {output.peek_read(), output.available_read()};

    // XOR the transit key with the masking key
    if (key_material.key_material_size != masking_key.key_material_size) {
        return KM_ERROR_INVALID_ARGUMENT;
    }
    for (size_t i = 0; i < key_material.key_material_size; i++) {
        key_material.writable_data()[i] ^= masking_key.key_material[i];
    }

    auto transit_key_authorizations = AuthorizationSetBuilder()
                                          .AesEncryptionKey(256)
                                          .Padding(KM_PAD_NONE)
                                          .Authorization(TAG_BLOCK_MODE, KM_MODE_GCM)
                                          .Authorization(TAG_NONCE, iv)
                                          .Authorization(TAG_MIN_MAC_LENGTH, 128)
                                          .build();
    if (transit_key_authorizations.is_valid() != AuthorizationSet::Error::OK) {
        return TranslateAuthorizationSetError(transit_key_authorizations.is_valid());
    }
    auto gcm_params = AuthorizationSetBuilder()
                          .Padding(KM_PAD_NONE)
                          .Authorization(TAG_BLOCK_MODE, KM_MODE_GCM)
                          .Authorization(TAG_NONCE, iv)
                          .Authorization(TAG_MAC_LENGTH, 128)
                          .build();
    if (gcm_params.is_valid() != AuthorizationSet::Error::OK) {
        return TranslateAuthorizationSetError(transit_key_authorizations.is_valid());
    }

    auto aes_factory = GetKeyFactory(KM_ALGORITHM_AES);
    if (!aes_factory) return KM_ERROR_UNKNOWN_ERROR;

    UniquePtr<Key> aes_key;
    error = aes_factory->LoadKey(move(key_material), gcm_params, move(transit_key_authorizations),
                                 AuthorizationSet(), &aes_key);
    if (error != KM_ERROR_OK) return error;

    auto aes_operation_factory = GetOperationFactory(KM_ALGORITHM_AES, KM_PURPOSE_DECRYPT);
    if (!aes_operation_factory) return KM_ERROR_UNKNOWN_ERROR;

    OperationPtr aes_operation(
        aes_operation_factory->CreateOperation(move(*aes_key), gcm_params, &error));
    if (!aes_operation.get()) return error;

    error = aes_operation->Begin(gcm_params, &out_params);
    if (error != KM_ERROR_OK) return error;

    size_t consumed = 0;
    Buffer encrypted_key, plaintext;
    if (!plaintext.Reinitialize(secure_key.key_material_size + tag.data_length)) {
        return KM_ERROR_MEMORY_ALLOCATION_FAILED;
    }
    if (!encrypted_key.Reinitialize(secure_key.key_material_size + tag.data_length)) {
        return KM_ERROR_MEMORY_ALLOCATION_FAILED;
    }
    if (!encrypted_key.write(secure_key.key_material, secure_key.key_material_size)) {
        return KM_ERROR_UNKNOWN_ERROR;
    }
    if (!encrypted_key.write(tag.data, tag.data_length)) {
        return KM_ERROR_UNKNOWN_ERROR;
    }

    AuthorizationSet update_outparams;
    auto update_params = AuthorizationSetBuilder()
                             .Authorization(TAG_ASSOCIATED_DATA, wrapped_key_description.data,
                                            wrapped_key_description.data_length)
                             .build();
    if (update_params.is_valid() != AuthorizationSet::Error::OK) {
        return TranslateAuthorizationSetError(transit_key_authorizations.is_valid());
    }

    error = aes_operation->Update(update_params, encrypted_key, &update_outparams, &plaintext,
                                  &consumed);
    if (error != KM_ERROR_OK) return error;

    AuthorizationSet finish_params, finish_out_params;
    Buffer finish_input;
    error = aes_operation->Finish(finish_params, finish_input, Buffer() /* signature */,
                                  &finish_out_params, &plaintext);
    if (error != KM_ERROR_OK) return error;

    *wrapped_key_material = {plaintext.peek_read(), plaintext.available_read()};
    if (!wrapped_key_material->key_material && plaintext.peek_read()) {
        return KM_ERROR_MEMORY_ALLOCATION_FAILED;
    }

    return error;
}

}  // namespace keymaster
