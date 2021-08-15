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

#ifndef SYSTEM_KEYMASTER_ASYMMETRIC_KEY_H
#define SYSTEM_KEYMASTER_ASYMMETRIC_KEY_H

#include <openssl/evp.h>

#include <keymaster/key.h>

namespace keymaster {

class AsymmetricKey : public Key {
  public:
    AsymmetricKey(AuthorizationSet&& hw_enforced, AuthorizationSet&& sw_enforced,
                  const KeyFactory* key_factory)
        : Key(move(hw_enforced), move(sw_enforced), key_factory) {}
    virtual ~AsymmetricKey() {}

    keymaster_error_t formatted_key_material(keymaster_key_format_t format,
                                             UniquePtr<uint8_t[]>* material,
                                             size_t* size) const override;

    virtual bool InternalToEvp(EVP_PKEY* pkey) const = 0;
    virtual bool EvpToInternal(const EVP_PKEY* pkey) = 0;
};

}  // namespace keymaster

#endif  // SYSTEM_KEYMASTER_ASYMMETRIC_KEY_H
