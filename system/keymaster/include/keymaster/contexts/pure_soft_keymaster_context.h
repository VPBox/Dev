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

#ifndef SYSTEM_KEYMASTER_PURE_SOFT_KEYMASTER_CONTEXT_H_
#define SYSTEM_KEYMASTER_PURE_SOFT_KEYMASTER_CONTEXT_H_


#include <memory>
#include <string>

#include <keymaster/keymaster_context.h>
#include <keymaster/attestation_record.h>
#include <keymaster/km_openssl/software_random_source.h>
#include <keymaster/km_openssl/soft_keymaster_enforcement.h>
#include <keymaster/soft_key_factory.h>
#include <keymaster/random_source.h>

namespace keymaster {

class SoftKeymasterKeyRegistrations;
class Keymaster0Engine;
class Keymaster1Engine;
class Key;

/**
 * SoftKeymasterContext provides the context for a non-secure implementation of AndroidKeymaster.
 */
class PureSoftKeymasterContext: public KeymasterContext,
        protected SoftwareKeyBlobMaker,
        AttestationRecordContext,
        SoftwareRandomSource {
  public:
    explicit PureSoftKeymasterContext();
    ~PureSoftKeymasterContext() override;

    /*********************************************************************************************
     * Implement KeymasterContext
     */
    keymaster_error_t SetSystemVersion(uint32_t os_version, uint32_t os_patchlevel) override;
    void GetSystemVersion(uint32_t* os_version, uint32_t* os_patchlevel) const override;

    KeyFactory* GetKeyFactory(keymaster_algorithm_t algorithm) const override;
    OperationFactory* GetOperationFactory(keymaster_algorithm_t algorithm,
                                          keymaster_purpose_t purpose) const override;
    keymaster_algorithm_t* GetSupportedAlgorithms(size_t* algorithms_count) const override;
    keymaster_error_t UpgradeKeyBlob(const KeymasterKeyBlob& key_to_upgrade,
                                     const AuthorizationSet& upgrade_params,
                                     KeymasterKeyBlob* upgraded_key) const override;
    keymaster_error_t ParseKeyBlob(const KeymasterKeyBlob& blob,
                                   const AuthorizationSet& additional_params,
                                   UniquePtr<Key>* key) const override;
    keymaster_error_t DeleteKey(const KeymasterKeyBlob& blob) const override;
    keymaster_error_t DeleteAllKeys() const override;
    keymaster_error_t AddRngEntropy(const uint8_t* buf, size_t length) const override;

    keymaster_error_t GenerateAttestation(const Key& key,
                                          const AuthorizationSet& attest_params,
                                          CertChainPtr* cert_chain) const override;


    KeymasterEnforcement* enforcement_policy() override {
        // SoftKeymaster does no enforcement; it's all done by Keystore.
        return &soft_keymaster_enforcement_;
    }

    /*********************************************************************************************
     * Implement SoftwareKeyBlobMaker
     */
    keymaster_error_t CreateKeyBlob(const AuthorizationSet& auths, keymaster_key_origin_t origin,
                                    const KeymasterKeyBlob& key_material, KeymasterKeyBlob* blob,
                                    AuthorizationSet* hw_enforced,
                                    AuthorizationSet* sw_enforced) const override;

    keymaster_error_t
    UnwrapKey(const KeymasterKeyBlob& wrapped_key_blob, const KeymasterKeyBlob& wrapping_key_blob,
              const AuthorizationSet& wrapping_key_params, const KeymasterKeyBlob& masking_key,
              AuthorizationSet* wrapped_key_params, keymaster_key_format_t* wrapped_key_format,
              KeymasterKeyBlob* wrapped_key_material) const override;

  protected:
    std::unique_ptr<KeyFactory> rsa_factory_;
    std::unique_ptr<KeyFactory> ec_factory_;
    std::unique_ptr<KeyFactory> aes_factory_;
    std::unique_ptr<KeyFactory> tdes_factory_;
    std::unique_ptr<KeyFactory> hmac_factory_;
    uint32_t os_version_;
    uint32_t os_patchlevel_;
    SoftKeymasterEnforcement soft_keymaster_enforcement_;
};

}  // namespace keymaster

#endif  // SYSTEM_KEYMASTER_PURE_SOFT_KEYMASTER_CONTEXT_H_
