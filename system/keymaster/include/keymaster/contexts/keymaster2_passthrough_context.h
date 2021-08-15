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

#ifndef SOFTWARE_CONTEXT_KEYMASTER2_PASSTHROUGH_CONTEXT_H_
#define SOFTWARE_CONTEXT_KEYMASTER2_PASSTHROUGH_CONTEXT_H_

#include <unordered_map>

#include <hardware/keymaster2.h>
#include <hardware/keymaster_defs.h>

#include <keymaster/keymaster_context.h>
#include <keymaster/legacy_support/keymaster_passthrough_engine.h>
#include <keymaster/legacy_support/keymaster_passthrough_key.h>

namespace keymaster {

class Keymaster2PassthroughContext : public KeymasterContext {
  public:
    explicit Keymaster2PassthroughContext(keymaster2_device_t* dev);

    /**
     * Sets the system version as reported by the system *itself*.  This is used to verify that the
     * system believes itself to be running the same version that is reported by the bootloader, in
     * hardware implementations.  For SoftKeymasterDevice, this sets the version information used.
     *
     * If the specified values don't match the bootloader-provided values, this method must return
     * KM_ERROR_INVALID_ARGUMENT;
     */
    keymaster_error_t SetSystemVersion(uint32_t os_version, uint32_t os_patchlevel) override;

    /**
     * Returns the system version.  For hardware-based implementations this will be the value
     * reported by the bootloader.  For SoftKeymasterDevice it will be the verion information set by
     * SetSystemVersion above.
     */
    void GetSystemVersion(uint32_t* os_version, uint32_t* os_patchlevel) const override;

    KeyFactory* GetKeyFactory(keymaster_algorithm_t algorithm) const override;
    OperationFactory* GetOperationFactory(keymaster_algorithm_t algorithm,
                                          keymaster_purpose_t purpose) const override;
    keymaster_algorithm_t* GetSupportedAlgorithms(size_t* algorithms_count) const override;

    /**
     * UpgradeKeyBlob takes an existing blob, parses out key material and constructs a new blob with
     * the current format and OS version info.
     */
    keymaster_error_t UpgradeKeyBlob(const KeymasterKeyBlob& key_to_upgrade,
                                     const AuthorizationSet& upgrade_params,
                                     KeymasterKeyBlob* upgraded_key) const override;

    /**
     * ParseKeyBlob takes a blob and extracts authorization sets and key material, returning an
     * error if the blob fails integrity checking or decryption.  Note that the returned key
     * material may itself be an opaque blob usable only by secure hardware (in the hybrid case).
     *
     * This method is called by AndroidKeymaster.
     */
    keymaster_error_t ParseKeyBlob(const KeymasterKeyBlob& blob,
                                   const AuthorizationSet& additional_params,
                                   UniquePtr<Key>* key) const override;

    /**
     * Take whatever environment-specific action is appropriate (if any) to delete the specified
     * key.
     */
    keymaster_error_t DeleteKey(const KeymasterKeyBlob& /* blob */) const override;

    /**
     * Take whatever environment-specific action is appropriate to delete all keys.
     */
    keymaster_error_t DeleteAllKeys() const override;

    /**
     * Adds entropy to the Cryptographic Pseudo Random Number Generator used to generate key
     * material, and other cryptographic protocol elements.  Note that if the underlying CPRNG
     * tracks the size of its entropy pool, it should not assume that the provided data contributes
     * any entropy, and it should also ensure that data provided through this interface cannot
     * "poison" the CPRNG outputs, making them predictable.
     */
    keymaster_error_t AddRngEntropy(const uint8_t* buf, size_t length) const override;

    /**
     * Return the enforcement policy for this context, or null if no enforcement should be done.
     */
    KeymasterEnforcement* enforcement_policy() override;

    keymaster_error_t GenerateAttestation(const Key& key, const AuthorizationSet& attest_params,
                                          CertChainPtr* cert_chain) const override;

    keymaster_error_t
    UnwrapKey(const KeymasterKeyBlob& wrapped_key_blob, const KeymasterKeyBlob& wrapping_key_blob,
              const AuthorizationSet& wrapping_key_params, const KeymasterKeyBlob& masking_key,
              AuthorizationSet* wrapped_key_params, keymaster_key_format_t* wrapped_key_format,
              KeymasterKeyBlob* wrapped_key_material) const override;

  private:
    keymaster2_device_t* device_;
    mutable std::unordered_map<keymaster_algorithm_t, UniquePtr<KeymasterPassthroughKeyFactory>>
        factories_;
    UniquePtr<KeymasterPassthroughEngine> engine_;
    uint32_t os_version_;
    uint32_t os_patchlevel_;
};

}  // namespace keymaster

#endif  // SOFTWARE_CONTEXT_KEYMASTER2_PASSTHROUGH_CONTEXT_H_
