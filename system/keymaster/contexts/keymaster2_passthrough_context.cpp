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

#include <keymaster/contexts/keymaster2_passthrough_context.h>

#include <keymaster/legacy_support/keymaster_passthrough_key.h>
#include <keymaster/legacy_support/keymaster_passthrough_engine.h>

namespace keymaster {

Keymaster2PassthroughContext::Keymaster2PassthroughContext(keymaster2_device_t* dev)
        : device_(dev), engine_(KeymasterPassthroughEngine::createInstance(dev)) {

}

keymaster_error_t Keymaster2PassthroughContext::SetSystemVersion(uint32_t os_version,
        uint32_t os_patchlevel) {
    os_version_ = os_version;
    os_patchlevel_ = os_patchlevel;
    return KM_ERROR_OK;
}

void Keymaster2PassthroughContext::GetSystemVersion(uint32_t* os_version,
        uint32_t* os_patchlevel) const {
    if (os_version) *os_version = os_version_;
    if (os_patchlevel) *os_patchlevel = os_patchlevel_;
}

KeyFactory* Keymaster2PassthroughContext::GetKeyFactory(keymaster_algorithm_t algorithm) const {
    auto& result = factories_[algorithm];
    if (!result) {
        result.reset(new KeymasterPassthroughKeyFactory(engine_.get(), algorithm));
    }
    return result.get();
}
OperationFactory* Keymaster2PassthroughContext::GetOperationFactory(keymaster_algorithm_t algorithm,
        keymaster_purpose_t purpose) const {
    auto keyfactory = GetKeyFactory(algorithm);
    return keyfactory->GetOperationFactory(purpose);
}
keymaster_algorithm_t* Keymaster2PassthroughContext::GetSupportedAlgorithms(
        size_t* algorithms_count) const {
    if (algorithms_count) *algorithms_count = 0;
    return nullptr;
}

keymaster_error_t Keymaster2PassthroughContext::UpgradeKeyBlob(
        const KeymasterKeyBlob& key_to_upgrade, const AuthorizationSet& upgrade_params,
        KeymasterKeyBlob* upgraded_key) const {
    if (!upgraded_key) return KM_ERROR_UNEXPECTED_NULL_POINTER;
    *upgraded_key = {};
    return device_->upgrade_key(device_, &key_to_upgrade, &upgrade_params, upgraded_key);
}

keymaster_error_t Keymaster2PassthroughContext::ParseKeyBlob(const KeymasterKeyBlob& blob,
        const AuthorizationSet& additional_params, UniquePtr<Key>* key) const {
    keymaster_key_characteristics_t characteristics = {};
    keymaster_blob_t clientId;
    keymaster_blob_t applicationData;
    keymaster_blob_t* clientIdPtr = &clientId;
    keymaster_blob_t* applicationDataPtr = &applicationData;
    if (!additional_params.GetTagValue(TAG_APPLICATION_ID, clientIdPtr)) {
        clientIdPtr = nullptr;
    }
    if (!additional_params.GetTagValue(TAG_APPLICATION_DATA, applicationDataPtr)) {
        applicationDataPtr = nullptr;
    }

    auto rc = device_->get_key_characteristics(device_, &blob, clientIdPtr, applicationDataPtr,
                                               &characteristics);

    if (rc != KM_ERROR_OK) return rc;

    AuthorizationSet hw_enforced;
    AuthorizationSet sw_enforced;

    hw_enforced.Reinitialize(characteristics.hw_enforced);
    sw_enforced.Reinitialize(characteristics.sw_enforced);

    keymaster_free_characteristics(&characteristics);

    // GetKeyFactory
    keymaster_algorithm_t algorithm;
    if (!hw_enforced.GetTagValue(TAG_ALGORITHM, &algorithm) &&
        !sw_enforced.GetTagValue(TAG_ALGORITHM, &algorithm)) {
        return KM_ERROR_INVALID_ARGUMENT;
    }

    KeymasterKeyBlob key_material = blob;
    auto factory = GetKeyFactory(algorithm);
    return factory->LoadKey(move(key_material), additional_params, move(hw_enforced),
                            move(sw_enforced), key);
}

keymaster_error_t Keymaster2PassthroughContext::DeleteKey(const KeymasterKeyBlob& blob) const {
    return device_->delete_key(device_, &blob);
}

keymaster_error_t Keymaster2PassthroughContext::DeleteAllKeys() const {
    return device_->delete_all_keys(device_);
}

keymaster_error_t Keymaster2PassthroughContext::AddRngEntropy(const uint8_t* buf,
        size_t length) const {
    return device_->add_rng_entropy(device_, buf, length);
}


KeymasterEnforcement* Keymaster2PassthroughContext::enforcement_policy() {
    return nullptr;
}

keymaster_error_t Keymaster2PassthroughContext::GenerateAttestation(const Key& key,
        const AuthorizationSet& attest_params, CertChainPtr* cert_chain) const {
    if (!cert_chain) return KM_ERROR_UNEXPECTED_NULL_POINTER;

    keymaster_cert_chain_t cchain{};

    auto rc = device_->attest_key(device_, &key.key_material(), &attest_params, &cchain);
    if (rc == KM_ERROR_OK) {
        cert_chain->reset(new keymaster_cert_chain_t);
        **cert_chain = { new keymaster_blob_t[cchain.entry_count], cchain.entry_count };
        for (size_t i = 0; i < cchain.entry_count; ++i) {
            (*cert_chain)->entries[i] = { dup_array(cchain.entries[i].data,
                                              cchain.entries[i].data_length),
                                          cchain.entries[i].data_length };
            free(const_cast<uint8_t*>(cchain.entries[i].data));
        }
        free(cchain.entries);
    }
    return rc;
}

keymaster_error_t Keymaster2PassthroughContext::UnwrapKey(
    const KeymasterKeyBlob&, const KeymasterKeyBlob&, const AuthorizationSet&,
    const KeymasterKeyBlob&, AuthorizationSet*, keymaster_key_format_t*, KeymasterKeyBlob*) const {
    return KM_ERROR_UNIMPLEMENTED;
}

} // namespace keymaster
