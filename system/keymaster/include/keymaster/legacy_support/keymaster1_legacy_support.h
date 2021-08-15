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

#ifndef LEGACY_SUPPORT_KEYMASTER1_LEGACY_SUPPORT_H_
#define LEGACY_SUPPORT_KEYMASTER1_LEGACY_SUPPORT_H_

#include <map>
#include <vector>

#include <hardware/keymaster_defs.h>
#include <hardware/keymaster1.h>

#include <keymaster/android_keymaster_utils.h>
#include <keymaster/authorization_set.h>
#include <keymaster/key_factory.h>
#include <keymaster/UniquePtr.h>

#include "ec_keymaster1_key.h"
#include "keymaster_passthrough_engine.h"
#include "keymaster_passthrough_key.h"
#include "keymaster1_engine.h"
#include "rsa_keymaster1_key.h"

namespace keymaster {

class Keymaster1LegacySupport {
public:
    typedef std::pair<keymaster_algorithm_t, keymaster_purpose_t> AlgPurposePair;
    typedef std::map<AlgPurposePair, std::vector<keymaster_digest_t>> DigestMap;

    // NOLINTNEXTLINE(google-explicit-constructor)
    Keymaster1LegacySupport(const keymaster1_device_t* dev);

    bool RequiresSoftwareDigesting(const AuthorizationSet& key_description) const;
    bool RequiresSoftwareDigesting(const keymaster_digest_t digest,
                                   const AuthProxy& key_description) const;

private:
    DigestMap device_digests_;
    bool supports_all_;

};

class SoftwareKeyBlobMaker;

template<typename KM1_SOFTDIGEST_FACTORY>
class Keymaster1ArbitrationFactory : public KeyFactory {
public:
    template<typename... SOFT_FACTORY_CONSRUCTOR_ARGS>
    Keymaster1ArbitrationFactory(const KeymasterPassthroughEngine* ptengine,
                                 keymaster_algorithm_t algorithm,
                                 const keymaster1_device_t* dev,
                                 SOFT_FACTORY_CONSRUCTOR_ARGS&&... args)
            : software_digest_factory_(forward<SOFT_FACTORY_CONSRUCTOR_ARGS>(args)...),
              passthrough_factory_(ptengine, algorithm),
              legacy_support_(dev){}
    keymaster_error_t GenerateKey(const AuthorizationSet& key_description,
                                  KeymasterKeyBlob* key_blob, AuthorizationSet* hw_enforced,
                                  AuthorizationSet* sw_enforced) const {
        if (legacy_support_.RequiresSoftwareDigesting(key_description)) {
            return software_digest_factory_.GenerateKey(key_description, key_blob, hw_enforced,
                                                 sw_enforced);
        } else {
            return passthrough_factory_.GenerateKey(key_description, key_blob, hw_enforced,
                                                    sw_enforced);
        }
    }

    keymaster_error_t ImportKey(const AuthorizationSet& key_description,
                                keymaster_key_format_t input_key_material_format,
                                const KeymasterKeyBlob& input_key_material,
                                KeymasterKeyBlob* output_key_blob, AuthorizationSet* hw_enforced,
                                AuthorizationSet* sw_enforced) const {
        if (legacy_support_.RequiresSoftwareDigesting(key_description)) {
            return software_digest_factory_.ImportKey(key_description, input_key_material_format,
                                                      input_key_material, output_key_blob,
                                                      hw_enforced, sw_enforced);
        } else {
            return passthrough_factory_.ImportKey(key_description, input_key_material_format,
                                                  input_key_material, output_key_blob,
                                                  hw_enforced, sw_enforced);
        }
    }

    keymaster_error_t LoadKey(KeymasterKeyBlob&& key_material,
                              const AuthorizationSet& additional_params,
                              AuthorizationSet&& hw_enforced,
                              AuthorizationSet&& sw_enforced,
                              UniquePtr<Key>* key) const override {
        keymaster_digest_t digest;
        if (!additional_params.GetTagValue(TAG_DIGEST, &digest)) {
            digest = KM_DIGEST_NONE;
        }

        if (legacy_support_.RequiresSoftwareDigesting(digest,
                                                      AuthProxy(hw_enforced, sw_enforced))) {
            return software_digest_factory_.LoadKey(move(key_material), additional_params,
                                                    move(hw_enforced), move(sw_enforced), key);
        } else {
            return passthrough_factory_.LoadKey(move(key_material), additional_params,
                                                move(hw_enforced), move(sw_enforced), key);
        }
    }

    OperationFactory* GetOperationFactory(keymaster_purpose_t /*purpose*/) const override {
        // The passthrough operation factory must not be queried. To get the operation factory for
        // a Key call key.key_factory()->GetOperationFactory()
        assert(false);
        return nullptr;
    }

    // Informational methods.
    const keymaster_key_format_t* SupportedImportFormats(size_t* format_count) const  override {
        *format_count = 0;
        return nullptr;
    }
    const keymaster_key_format_t* SupportedExportFormats(size_t* format_count) const override {
        *format_count = 0;
        return nullptr;
    }

private:
    KM1_SOFTDIGEST_FACTORY software_digest_factory_;
    KeymasterPassthroughKeyFactory passthrough_factory_;
    Keymaster1LegacySupport legacy_support_;
};

template<>
keymaster_error_t
Keymaster1ArbitrationFactory<EcdsaKeymaster1KeyFactory>::GenerateKey(
        const AuthorizationSet& key_description,
        KeymasterKeyBlob* key_blob, AuthorizationSet* hw_enforced,
        AuthorizationSet* sw_enforced) const;


template<>
keymaster_error_t
Keymaster1ArbitrationFactory<EcdsaKeymaster1KeyFactory>::LoadKey(KeymasterKeyBlob&& key_material,
        const AuthorizationSet& additional_params,
        AuthorizationSet&& hw_enforced,
        AuthorizationSet&& sw_enforced,
        UniquePtr<Key>* key) const;

template<>
keymaster_error_t
Keymaster1ArbitrationFactory<RsaKeymaster1KeyFactory>::LoadKey(KeymasterKeyBlob&& key_material,
        const AuthorizationSet& additional_params,
        AuthorizationSet&& hw_enforced,
        AuthorizationSet&& sw_enforced,
        UniquePtr<Key>* key) const;

} // namespace keymaster

#endif  // LEGACY_SUPPORT_KEYMASTER1_LEGACY_SUPPORT_H_
