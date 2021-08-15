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

#ifndef SYSTEM_KEYMASTER_RSA_KEYMASTER0_KEY_H_
#define SYSTEM_KEYMASTER_RSA_KEYMASTER0_KEY_H_

#include <openssl/rsa.h>

#include <keymaster/km_openssl/rsa_key.h>
#include <keymaster/km_openssl/rsa_key_factory.h>

namespace keymaster {

class Keymaster0Engine;

/**
 * An RsaKeyFactory which can delegate key generation, importing and loading operations to a
 * keymaster0-backed OpenSSL engine.
 */
class RsaKeymaster0KeyFactory : public RsaKeyFactory {
    typedef RsaKeyFactory super;

  public:
    RsaKeymaster0KeyFactory(const SoftwareKeyBlobMaker* blob_maker,
                            const Keymaster0Engine* engine);

    keymaster_error_t GenerateKey(const AuthorizationSet& key_description,
                                  KeymasterKeyBlob* key_blob, AuthorizationSet* hw_enforced,
                                  AuthorizationSet* sw_enforced) const override;

    keymaster_error_t ImportKey(const AuthorizationSet& key_description,
                                keymaster_key_format_t input_key_material_format,
                                const KeymasterKeyBlob& input_key_material,
                                KeymasterKeyBlob* output_key_blob, AuthorizationSet* hw_enforced,
                                AuthorizationSet* sw_enforced) const override;

    keymaster_error_t LoadKey(KeymasterKeyBlob&& key_material,
                              const AuthorizationSet& additional_params,
                              AuthorizationSet&& hw_enforced,
                              AuthorizationSet&& sw_enforced,
                              UniquePtr<Key>* key) const override;

  private:
    const Keymaster0Engine* engine_;
};

class RsaKeymaster0Key : public RsaKey {
  public:
    RsaKeymaster0Key(RSA* rsa_key, AuthorizationSet&& hw_enforced,
                     AuthorizationSet&& sw_enforced,
                     const KeyFactory* key_factory)
        : RsaKey(rsa_key, move(hw_enforced), move(sw_enforced), key_factory) {}
};

}  // namespace keymaster

#endif  // SYSTEM_KEYMASTER_RSA_KEYMASTER0_KEY_H_
