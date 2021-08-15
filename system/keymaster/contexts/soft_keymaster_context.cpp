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

#include <keymaster/contexts/soft_keymaster_context.h>

#include <memory>

#include <openssl/rand.h>

#include <keymaster/android_keymaster_utils.h>
#include <keymaster/key_blob_utils/auth_encrypted_key_blob.h>
#include <keymaster/key_blob_utils/integrity_assured_key_blob.h>
#include <keymaster/key_blob_utils/ocb_utils.h>
#include <keymaster/key_blob_utils/software_keyblobs.h>
#include <keymaster/km_openssl/aes_key.h>
#include <keymaster/km_openssl/asymmetric_key.h>
#include <keymaster/km_openssl/attestation_utils.h>
#include <keymaster/km_openssl/hmac_key.h>
#include <keymaster/km_openssl/openssl_err.h>
#include <keymaster/km_openssl/triple_des_key.h>
#include <keymaster/legacy_support/ec_keymaster0_key.h>
#include <keymaster/legacy_support/ec_keymaster1_key.h>
#include <keymaster/legacy_support/keymaster0_engine.h>
#include <keymaster/legacy_support/rsa_keymaster0_key.h>
#include <keymaster/legacy_support/rsa_keymaster1_key.h>
#include <keymaster/logger.h>

#include "soft_attestation_cert.h"

using std::unique_ptr;

namespace keymaster {

namespace {

KeymasterBlob string2Blob(const std::string& str) {
    return KeymasterBlob(reinterpret_cast<const uint8_t*>(str.data()), str.size());
}

}  // anonymous namespace

SoftKeymasterContext::SoftKeymasterContext(const std::string& root_of_trust)
    : rsa_factory_(new RsaKeyFactory(this)), ec_factory_(new EcKeyFactory(this)),
      aes_factory_(new AesKeyFactory(this, this)),
      tdes_factory_(new TripleDesKeyFactory(this, this)),
      hmac_factory_(new HmacKeyFactory(this, this)), km1_dev_(nullptr),
      root_of_trust_(string2Blob(root_of_trust)), os_version_(0), os_patchlevel_(0) {}

SoftKeymasterContext::~SoftKeymasterContext() {}

keymaster_error_t SoftKeymasterContext::SetHardwareDevice(keymaster0_device_t* keymaster0_device) {
    if (!keymaster0_device)
        return KM_ERROR_UNEXPECTED_NULL_POINTER;

    if ((keymaster0_device->flags & KEYMASTER_SOFTWARE_ONLY) != 0) {
        LOG_E("SoftKeymasterContext only wraps hardware keymaster0 devices", 0);
        return KM_ERROR_INVALID_ARGUMENT;
    }

    km0_engine_.reset(new Keymaster0Engine(keymaster0_device));
    rsa_factory_.reset(new RsaKeymaster0KeyFactory(this, km0_engine_.get()));
    ec_factory_.reset(new EcdsaKeymaster0KeyFactory(this, km0_engine_.get()));
    // Keep AES and HMAC factories.

    return KM_ERROR_OK;
}

keymaster_error_t SoftKeymasterContext::SetHardwareDevice(keymaster1_device_t* keymaster1_device) {
    if (!keymaster1_device)
        return KM_ERROR_UNEXPECTED_NULL_POINTER;

    km1_dev_ = keymaster1_device;

    km1_engine_.reset(new Keymaster1Engine(keymaster1_device));
    rsa_factory_.reset(new RsaKeymaster1KeyFactory(this, km1_engine_.get()));
    ec_factory_.reset(new EcdsaKeymaster1KeyFactory(this, km1_engine_.get()));

    // Use default HMAC and AES key factories. Higher layers will pass HMAC/AES keys/ops that are
    // supported by the hardware to it and other ones to the software-only factory.

    return KM_ERROR_OK;
}

keymaster_error_t SoftKeymasterContext::SetSystemVersion(uint32_t os_version,
                                                         uint32_t os_patchlevel) {
    os_version_ = os_version;
    os_patchlevel_ = os_patchlevel;
    return KM_ERROR_OK;
}

void SoftKeymasterContext::GetSystemVersion(uint32_t* os_version, uint32_t* os_patchlevel) const {
    *os_version = os_version_;
    *os_patchlevel = os_patchlevel_;
}

KeyFactory* SoftKeymasterContext::GetKeyFactory(keymaster_algorithm_t algorithm) const {
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
SoftKeymasterContext::GetSupportedAlgorithms(size_t* algorithms_count) const {
    *algorithms_count = array_length(supported_algorithms);
    return supported_algorithms;
}

OperationFactory* SoftKeymasterContext::GetOperationFactory(keymaster_algorithm_t algorithm,
                                                            keymaster_purpose_t purpose) const {
    KeyFactory* key_factory = GetKeyFactory(algorithm);
    if (!key_factory)
        return nullptr;
    return key_factory->GetOperationFactory(purpose);
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

static keymaster_error_t SetAuthorizations(const AuthorizationSet& key_description,
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

keymaster_error_t SoftKeymasterContext::CreateKeyBlob(const AuthorizationSet& key_description,
                                                      const keymaster_key_origin_t origin,
                                                      const KeymasterKeyBlob& key_material,
                                                      KeymasterKeyBlob* blob,
                                                      AuthorizationSet* hw_enforced,
                                                      AuthorizationSet* sw_enforced) const {
    keymaster_error_t error = SetAuthorizations(key_description, origin, os_version_,
                                                os_patchlevel_, hw_enforced, sw_enforced);
    if (error != KM_ERROR_OK)
        return error;

    AuthorizationSet hidden;
    error = BuildHiddenAuthorizations(key_description, &hidden, root_of_trust_);
    if (error != KM_ERROR_OK)
        return error;

    return SerializeIntegrityAssuredBlob(key_material, hidden, *hw_enforced, *sw_enforced, blob);
}

keymaster_error_t SoftKeymasterContext::UpgradeKeyBlob(const KeymasterKeyBlob& key_to_upgrade,
                                                       const AuthorizationSet& upgrade_params,
                                                       KeymasterKeyBlob* upgraded_key) const {
    UniquePtr<Key> key;
    keymaster_error_t error = ParseKeyBlob(key_to_upgrade, upgrade_params, &key);
    if (error != KM_ERROR_OK)
        return error;

    // Three cases here:
    //
    // 1. Software key blob.  Version info, if present, is in sw_enforced.  If not present, we
    //    should add it.
    //
    // 2. Keymaster0 hardware key blob.  Version info, if present, is in sw_enforced.  If not
    //    present we should add it.
    //
    // 3. Keymaster1 hardware key blob.  Version info is not present and we shouldn't have been
    //    asked to upgrade.

    // Handle case 3.
    if (km1_dev_ && key->hw_enforced().Contains(TAG_PURPOSE) &&
            !key->hw_enforced().Contains(TAG_OS_PATCHLEVEL))
        return KM_ERROR_INVALID_ARGUMENT;

    // Handle case 1 and 2
    return UpgradeSoftKeyBlob(key, os_version_, os_patchlevel_, upgrade_params, upgraded_key);
}

keymaster_error_t SoftKeymasterContext::ParseKeyBlob(const KeymasterKeyBlob& blob,
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
    // 4.  New keymaster0 hardware key blobs.  These are integrity-assured but not encrypted (though
    //     they're protected by the keymaster0 hardware implementation).  The keymaster0 key blob
    //     and auth sets should be extracted and returned.
    //
    // 5.  Keymaster1 hardware key blobs.  These are raw hardware key blobs.  They contain auth
    //     sets, which we retrieve from the hardware module.
    //
    // 6.  Old keymaster0 hardware key blobs.  These are raw hardware key blobs.  They don't have
    //     auth sets so reasonable defaults are generated and returned along with the key blob.
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
    AuthorizationSet hidden;
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

    error = BuildHiddenAuthorizations(additional_params, &hidden, root_of_trust_);
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
    if (error != KM_ERROR_INVALID_KEY_BLOB)
        return constructKey();

    if (km1_dev_) {
        error = ParseKeymaster1HwBlob(blob, additional_params, &key_material, &hw_enforced,
                                      &sw_enforced);
    } else if (km0_engine_) {
        error = ParseKeymaster0HwBlob(blob, &key_material, &hw_enforced, &sw_enforced);
    } else {
        return KM_ERROR_INVALID_KEY_BLOB;
    }
    return constructKey();
}

keymaster_error_t SoftKeymasterContext::DeleteKey(const KeymasterKeyBlob& blob) const {
    if (km1_engine_) {
        // HACK. Due to a bug with Qualcomm's Keymaster implementation, which causes the device to
        // reboot if we pass it a key blob it doesn't understand, we need to check for software
        // keys.  If it looks like a software key there's nothing to do so we just return.
        KeymasterKeyBlob key_material;
        AuthorizationSet hw_enforced, sw_enforced;
        keymaster_error_t error = DeserializeIntegrityAssuredBlob_NoHmacCheck(
            blob, &key_material, &hw_enforced, &sw_enforced);
        if (error == KM_ERROR_OK) {
            return KM_ERROR_OK;
        }

        return km1_engine_->DeleteKey(blob);
    }

    if (km0_engine_) {
        // This could be a keymaster0 hardware key, and it could be either raw or encapsulated in an
        // integrity-assured blob.  If it's integrity-assured, we can't validate it strongly,
        // because we don't have the necessary additional_params data.  However, the probability
        // that anything other than an integrity-assured blob would have all of the structure
        // required to decode as a valid blob is low -- unless it's maliciously-constructed, but the
        // deserializer should be proof against bad data, as should the keymaster0 hardware.
        //
        // Thus, we first try to parse it as integrity-assured.  If that works, we pass the result
        // to the underlying hardware.  If not, we pass blob unmodified to the underlying hardware.
        KeymasterKeyBlob key_material;
        AuthorizationSet hw_enforced, sw_enforced;
        keymaster_error_t error = DeserializeIntegrityAssuredBlob_NoHmacCheck(
            blob, &key_material, &hw_enforced, &sw_enforced);
        if (error == KM_ERROR_OK && km0_engine_->DeleteKey(key_material))
            return KM_ERROR_OK;

        km0_engine_->DeleteKey(blob);

        // We succeed unconditionally at this point, even if delete failed.  Failure indicates that
        // either the blob is a software blob (which we can't distinguish with certainty without
        // additional_params) or because it is a hardware blob and the hardware failed.  In the
        // first case, there is no error.  In the second case, the client can't do anything to fix
        // it anyway, so it's not too harmful to simply swallow the error.  This is not ideal, but
        // it's the least-bad alternative.
        return KM_ERROR_OK;
    }

    // Nothing to do for software-only contexts.
    return KM_ERROR_OK;
}

keymaster_error_t SoftKeymasterContext::DeleteAllKeys() const {
    if (km1_engine_)
        return km1_engine_->DeleteAllKeys();

    if (km0_engine_ && !km0_engine_->DeleteAllKeys())
        return KM_ERROR_UNKNOWN_ERROR;

    return KM_ERROR_OK;
}

keymaster_error_t SoftKeymasterContext::AddRngEntropy(const uint8_t* buf, size_t length) const {
    RAND_add(buf, length, 0 /* Don't assume any entropy is added to the pool. */);
    return KM_ERROR_OK;
}

keymaster_error_t SoftKeymasterContext::ParseKeymaster1HwBlob(
    const KeymasterKeyBlob& blob, const AuthorizationSet& additional_params,
    KeymasterKeyBlob* key_material, AuthorizationSet* hw_enforced,
    AuthorizationSet* sw_enforced) const {
    assert(km1_dev_);

    keymaster_blob_t client_id = {nullptr, 0};
    keymaster_blob_t app_data = {nullptr, 0};
    keymaster_blob_t* client_id_ptr = nullptr;
    keymaster_blob_t* app_data_ptr = nullptr;
    if (additional_params.GetTagValue(TAG_APPLICATION_ID, &client_id))
        client_id_ptr = &client_id;
    if (additional_params.GetTagValue(TAG_APPLICATION_DATA, &app_data))
        app_data_ptr = &app_data;

    // Get key characteristics, which incidentally verifies that the HW recognizes the key.
    keymaster_key_characteristics_t* characteristics;
    keymaster_error_t error = km1_dev_->get_key_characteristics(km1_dev_, &blob, client_id_ptr,
                                                                app_data_ptr, &characteristics);
    if (error != KM_ERROR_OK)
        return error;
    unique_ptr<keymaster_key_characteristics_t, Characteristics_Delete> characteristics_deleter(
        characteristics);

    LOG_D("Module \"%s\" accepted key", km1_dev_->common.module->name);

    hw_enforced->Reinitialize(characteristics->hw_enforced);
    sw_enforced->Reinitialize(characteristics->sw_enforced);
    *key_material = blob;
    return KM_ERROR_OK;
}

keymaster_error_t SoftKeymasterContext::ParseKeymaster0HwBlob(const KeymasterKeyBlob& blob,
                                                              KeymasterKeyBlob* key_material,
                                                              AuthorizationSet* hw_enforced,
                                                              AuthorizationSet* sw_enforced) const {
    assert(km0_engine_);

    unique_ptr<EVP_PKEY, EVP_PKEY_Delete> tmp_key(km0_engine_->GetKeymaster0PublicKey(blob));

    if (!tmp_key)
        return KM_ERROR_INVALID_KEY_BLOB;

    LOG_D("Module \"%s\" accepted key", km0_engine_->device()->common.module->name);
    keymaster_error_t error = FakeKeyAuthorizations(tmp_key.get(), hw_enforced, sw_enforced);
    if (error == KM_ERROR_OK)
        *key_material = blob;

    return error;
}

keymaster_error_t SoftKeymasterContext::GenerateAttestation(const Key& key,
        const AuthorizationSet& attest_params, CertChainPtr* cert_chain) const {

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

keymaster_error_t SoftKeymasterContext::UnwrapKey(const KeymasterKeyBlob&, const KeymasterKeyBlob&,
                                                  const AuthorizationSet&, const KeymasterKeyBlob&,
                                                  AuthorizationSet*, keymaster_key_format_t*,
                                                  KeymasterKeyBlob*) const {
    return KM_ERROR_UNIMPLEMENTED;
}

}  // namespace keymaster
