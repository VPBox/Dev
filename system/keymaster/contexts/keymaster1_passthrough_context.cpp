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

#include <keymaster/contexts/keymaster1_passthrough_context.h>

#include <keymaster/legacy_support/keymaster_passthrough_key.h>
#include <keymaster/legacy_support/keymaster_passthrough_engine.h>
#include <keymaster/legacy_support/keymaster1_legacy_support.h>
#include <keymaster/legacy_support/keymaster1_engine.h>
#include <keymaster/legacy_support/rsa_keymaster1_key.h>
#include <keymaster/legacy_support/ec_keymaster1_key.h>
#include <keymaster/key_blob_utils/software_keyblobs.h>
#include <keymaster/key_blob_utils/integrity_assured_key_blob.h>
#include <keymaster/key_blob_utils/ocb_utils.h>
#include <keymaster/km_openssl/aes_key.h>
#include <keymaster/km_openssl/hmac_key.h>
#include <keymaster/km_openssl/attestation_utils.h>
#include "soft_attestation_cert.h"

namespace keymaster {

Keymaster1PassthroughContext::Keymaster1PassthroughContext(keymaster1_device_t* dev)
        : device_(dev), pt_engine_(KeymasterPassthroughEngine::createInstance(dev)),
          km1_engine_(new Keymaster1Engine(dev)) {

}

keymaster_error_t Keymaster1PassthroughContext::SetSystemVersion(uint32_t os_version,
        uint32_t os_patchlevel) {
    os_version_ = os_version;
    os_patchlevel_ = os_patchlevel;
    return KM_ERROR_OK;
}

void Keymaster1PassthroughContext::GetSystemVersion(uint32_t* os_version,
        uint32_t* os_patchlevel) const {
    if (os_version) *os_version = os_version_;
    if (os_patchlevel) *os_patchlevel = os_patchlevel_;
}

KeyFactory* Keymaster1PassthroughContext::GetKeyFactory(keymaster_algorithm_t algorithm) const {
    auto& result = factories_[algorithm];
    if (!result) {
        switch(algorithm) {
        case KM_ALGORITHM_RSA:
            result.reset(new Keymaster1ArbitrationFactory<RsaKeymaster1KeyFactory>(pt_engine_.get(),
                    KM_ALGORITHM_RSA, device_, this, km1_engine_.get()));
            break;
        case KM_ALGORITHM_EC:
            result.reset(new Keymaster1ArbitrationFactory<EcdsaKeymaster1KeyFactory>(pt_engine_.get(),
                    KM_ALGORITHM_EC, device_, this, km1_engine_.get()));
            break;
        case KM_ALGORITHM_AES:
            result.reset(new Keymaster1ArbitrationFactory<AesKeyFactory>(pt_engine_.get(),
                    KM_ALGORITHM_AES, device_, this, this));
            break;
        case KM_ALGORITHM_HMAC:
            result.reset(new Keymaster1ArbitrationFactory<HmacKeyFactory>(pt_engine_.get(),
                    KM_ALGORITHM_HMAC, device_, this, this));
            break;
        case KM_ALGORITHM_TRIPLE_DES:
            // Not supported by KM1.
            return nullptr;
        }
    }
    return result.get();
}
OperationFactory* Keymaster1PassthroughContext::GetOperationFactory(keymaster_algorithm_t algorithm,
        keymaster_purpose_t purpose) const {
    auto keyfactory = GetKeyFactory(algorithm);
    return keyfactory->GetOperationFactory(purpose);
}
keymaster_algorithm_t* Keymaster1PassthroughContext::GetSupportedAlgorithms(
        size_t* algorithms_count) const {
    if (algorithms_count) *algorithms_count = 0;
    return nullptr;
}

keymaster_error_t Keymaster1PassthroughContext::UpgradeKeyBlob(
        const KeymasterKeyBlob& key_to_upgrade, const AuthorizationSet& upgrade_params,
        KeymasterKeyBlob* upgraded_key) const {

    UniquePtr<Key> key;
    keymaster_error_t error = ParseKeyBlob(key_to_upgrade, upgrade_params, &key);
    if (error != KM_ERROR_OK)
        return error;

    if (key->hw_enforced().Contains(TAG_PURPOSE) &&
            !key->hw_enforced().Contains(TAG_OS_PATCHLEVEL)) {
        return KM_ERROR_INVALID_ARGUMENT;
    }

    return UpgradeSoftKeyBlob(key, os_version_, os_patchlevel_, upgrade_params, upgraded_key);
}

static keymaster_error_t parseKeymaster1HwBlob(const keymaster1_device_t* device,
                                               const KeymasterKeyBlob& blob,
                                               const AuthorizationSet& additional_params,
                                               KeymasterKeyBlob* key_material,
                                               AuthorizationSet* hw_enforced,
                                               AuthorizationSet* sw_enforced) {
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
    keymaster_error_t error = device->get_key_characteristics(device, &blob, client_id_ptr,
                                                                app_data_ptr, &characteristics);
    if (error != KM_ERROR_OK)
        return error;

    UniquePtr<keymaster_key_characteristics_t, Characteristics_Delete> characteristics_deleter(
        characteristics);

    hw_enforced->Reinitialize(characteristics->hw_enforced);
    sw_enforced->Reinitialize(characteristics->sw_enforced);
    *key_material = blob;
    return KM_ERROR_OK;
}

keymaster_error_t Keymaster1PassthroughContext::ParseKeyBlob(const KeymasterKeyBlob& blob,
        const AuthorizationSet& additional_params, UniquePtr<Key>* key) const {
    AuthorizationSet hw_enforced;
    AuthorizationSet sw_enforced;
    KeymasterKeyBlob key_material;

    AuthorizationSet hidden;
    keymaster_error_t error = BuildHiddenAuthorizations(additional_params, &hidden,
                                                        softwareRootOfTrust);
    if (error != KM_ERROR_OK)
        return error;

    // Assume it's an integrity-assured blob (new software-only blob
    error = DeserializeIntegrityAssuredBlob(blob, hidden, &key_material, &hw_enforced, &sw_enforced);
    if (error != KM_ERROR_INVALID_KEY_BLOB && error != KM_ERROR_OK)
        return error;

    if (error == KM_ERROR_INVALID_KEY_BLOB) {
        error = parseKeymaster1HwBlob(km1_engine_->device(), blob, additional_params,
                                      &key_material, &hw_enforced, &sw_enforced);
        if (error != KM_ERROR_OK) return error;
    }

    // GetKeyFactory
    keymaster_algorithm_t algorithm;
    if (!hw_enforced.GetTagValue(TAG_ALGORITHM, &algorithm) &&
        !sw_enforced.GetTagValue(TAG_ALGORITHM, &algorithm)) {
        return KM_ERROR_INVALID_ARGUMENT;
    }
    auto factory = GetKeyFactory(algorithm);

    return factory->LoadKey(move(key_material), additional_params, move(hw_enforced),
                            move(sw_enforced), key);
}

keymaster_error_t Keymaster1PassthroughContext::DeleteKey(const KeymasterKeyBlob& blob) const {
     // HACK. Due to a bug with Qualcomm's Keymaster implementation, which causes the device to
     // reboot if we pass it a key blob it doesn't understand, we need to check for software
     // keys.  If it looks like a software key there's nothing to do so we just return.
     // Can be removed once b/33385206 is fixed
     KeymasterKeyBlob key_material;
     AuthorizationSet hw_enforced, sw_enforced;
     keymaster_error_t error = DeserializeIntegrityAssuredBlob_NoHmacCheck(
         blob, &key_material, &hw_enforced, &sw_enforced);
     if (error == KM_ERROR_OK) {
         return KM_ERROR_OK;
     }

     error = km1_engine_->DeleteKey(blob);
     if (error == KM_ERROR_INVALID_KEY_BLOB) {
         // Some implementations diagnose invalid keys.
         // However, all care we about is that the key blob, as supplied, is not usable after the
         // call.
         return KM_ERROR_OK;
     }
     return error;
}

keymaster_error_t Keymaster1PassthroughContext::DeleteAllKeys() const {
    return km1_engine_->DeleteAllKeys();
}

keymaster_error_t Keymaster1PassthroughContext::AddRngEntropy(const uint8_t* buf,
        size_t length) const {
    return device_->add_rng_entropy(device_, buf, length);
}


KeymasterEnforcement* Keymaster1PassthroughContext::enforcement_policy() {
    return nullptr;
}

keymaster_error_t Keymaster1PassthroughContext::CreateKeyBlob(const AuthorizationSet& key_description,
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

keymaster_error_t Keymaster1PassthroughContext::GenerateAttestation(const Key& key,
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

keymaster_error_t Keymaster1PassthroughContext::UnwrapKey(
    const KeymasterKeyBlob&, const KeymasterKeyBlob&, const AuthorizationSet&,
    const KeymasterKeyBlob&, AuthorizationSet*, keymaster_key_format_t*, KeymasterKeyBlob*) const {
    return KM_ERROR_UNIMPLEMENTED;
}

} // namespace keymaster
