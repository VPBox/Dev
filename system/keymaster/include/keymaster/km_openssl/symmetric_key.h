/*
 * Copyright 2014 The Android Open Source Project
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

#ifndef SYSTEM_KEYMASTER_SYMMETRIC_KEY_H_
#define SYSTEM_KEYMASTER_SYMMETRIC_KEY_H_

#include <keymaster/key_factory.h>
#include <keymaster/random_source.h>
#include <keymaster/soft_key_factory.h>

#include <keymaster/key.h>

namespace keymaster {

class SymmetricKey;

class SymmetricKeyFactory : public KeyFactory, public SoftKeyFactoryMixin {
  public:
    explicit SymmetricKeyFactory(const SoftwareKeyBlobMaker* blob_maker,
                                 const RandomSource* random_source) :
            SoftKeyFactoryMixin(blob_maker),
            random_source_(*random_source) {}

    keymaster_error_t GenerateKey(const AuthorizationSet& key_description,
                                  KeymasterKeyBlob* key_blob, AuthorizationSet* hw_enforced,
                                  AuthorizationSet* sw_enforced) const override;
    keymaster_error_t ImportKey(const AuthorizationSet& key_description,
                                keymaster_key_format_t input_key_material_format,
                                const KeymasterKeyBlob& input_key_material,
                                KeymasterKeyBlob* output_key_blob, AuthorizationSet* hw_enforced,
                                AuthorizationSet* sw_enforced) const override;

    virtual const keymaster_key_format_t* SupportedImportFormats(size_t* count) const override;
    virtual const keymaster_key_format_t* SupportedExportFormats(size_t* count) const override {
        return NoFormats(count);
    };

  private:
    virtual bool key_size_supported(size_t key_size_bits) const = 0;

    // These methods translate between key size in bits and bytes.  Normally it's just 8 bits to the
    // byte, but DES is different.
    virtual size_t key_size_bytes(size_t key_size_bits) const { return key_size_bits / 8; }
    virtual size_t key_size_bits(size_t key_size_bytes) const { return key_size_bytes * 8; }

    virtual keymaster_error_t
    validate_algorithm_specific_new_key_params(const AuthorizationSet& key_description) const = 0;

    const keymaster_key_format_t* NoFormats(size_t* format_count) const {
        *format_count = 0;
        return nullptr;
    }
    const RandomSource& random_source_;
};

class SymmetricKey : public Key {
  public:
    ~SymmetricKey();

    virtual keymaster_error_t formatted_key_material(keymaster_key_format_t, UniquePtr<uint8_t[]>*,
                                                     size_t*) const {
        return KM_ERROR_UNSUPPORTED_KEY_FORMAT;
    }

  protected:
    SymmetricKey(KeymasterKeyBlob&& key_material, AuthorizationSet&& hw_enforced,
                 AuthorizationSet&& sw_enforced,
                 const KeyFactory* key_factory);
};

}  // namespace keymaster

#endif  // SYSTEM_KEYMASTER_AES_KEY_H_
