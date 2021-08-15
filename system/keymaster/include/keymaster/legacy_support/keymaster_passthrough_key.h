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

#ifndef SYSTEM_KEYMASTER_KEYMASTER_PASSTHROUGH_KEY_H_
#define SYSTEM_KEYMASTER_KEYMASTER_PASSTHROUGH_KEY_H_

#include <keymaster/key.h>
#include <keymaster/android_keymaster_utils.h>
#include <keymaster/logger.h>
#include <keymaster/key_factory.h>
#include <keymaster/authorization_set.h>

#include "keymaster_passthrough_engine.h"

namespace keymaster {

//class SoftKeymasterContext;

/**
 * KeymasterPassthroughKeyFactory is a KeyFactory that creates and loads keys which are actually backed
 * by a hardware keymaster2 module.
 */
class KeymasterPassthroughKeyFactory : public KeyFactory {
    using engine_t = KeymasterPassthroughEngine;
  public:
    KeymasterPassthroughKeyFactory(const engine_t* engine, keymaster_algorithm_t algorithm)
          : KeyFactory(), engine_(engine), algorithm_(algorithm) {}

    keymaster_error_t GenerateKey(const AuthorizationSet& key_description,
                                  KeymasterKeyBlob* key_blob, AuthorizationSet* hw_enforced,
                                  AuthorizationSet* sw_enforced) const override {
        return engine_->GenerateKey(key_description, key_blob, hw_enforced, sw_enforced);
    }

    keymaster_error_t ImportKey(const AuthorizationSet& key_description,
                                keymaster_key_format_t input_key_material_format,
                                const KeymasterKeyBlob& input_key_material,
                                KeymasterKeyBlob* output_key_blob, AuthorizationSet* hw_enforced,
                                AuthorizationSet* sw_enforced) const override {
        return engine_->ImportKey(key_description, input_key_material_format, input_key_material,
                                  output_key_blob, hw_enforced, sw_enforced);
    }

    keymaster_error_t LoadKey(KeymasterKeyBlob&& key_material,
                              const AuthorizationSet& additional_params,
                              AuthorizationSet&& hw_enforced,
                              AuthorizationSet&& sw_enforced,
                              UniquePtr<Key>* key) const override;

    OperationFactory* GetOperationFactory(keymaster_purpose_t purpose) const override {
        return engine_->GetOperationFactory(purpose, algorithm_);
    }

    const keymaster_key_format_t* SupportedImportFormats(size_t* format_count) const override;
    const keymaster_key_format_t* SupportedExportFormats(size_t* format_count) const override;

  private:
    const engine_t* engine_;
    keymaster_algorithm_t algorithm_;
};

class KeymasterPassthroughKey : public Key {
  public:
    KeymasterPassthroughKey(KeymasterKeyBlob&& key_material, AuthorizationSet&& hw_enforced,
                       AuthorizationSet&& sw_enforced,
                       const KeyFactory* key_factory, keymaster_error_t* error,
                       const AuthorizationSet& additional_parameters,
                       const KeymasterPassthroughEngine* engine)
        : Key(move(hw_enforced), move(sw_enforced), key_factory),
          additional_parameters_(additional_parameters), engine_(engine) {
        key_material_ = move(key_material);
        if (*error != KM_ERROR_OK) return;
        if (additional_parameters.is_valid() != additional_parameters_.is_valid()
                && additional_parameters_.is_valid() == AuthorizationSet::ALLOCATION_FAILURE) {
            *error = KM_ERROR_MEMORY_ALLOCATION_FAILED;
        }
    }

    keymaster_error_t formatted_key_material(keymaster_key_format_t format,
                                             UniquePtr<uint8_t[]>* material,
                                             size_t* size) const override;

  protected:
    AuthorizationSet additional_parameters_;
    const KeymasterPassthroughEngine* engine_;
};

}  // namespace keymaster

#endif  // SYSTEM_KEYMASTER_KEYMASTER_PASSTHROUGH_KEY_H_
