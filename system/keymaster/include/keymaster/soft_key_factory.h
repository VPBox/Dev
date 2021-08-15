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

#ifndef SYSTEM_KEYMASTER_SOFTWARE_KEY_FACTORY_H_
#define SYSTEM_KEYMASTER_SOFTWARE_KEY_FACTORY_H_

#include "key_factory.h"
#include <keymaster/attestation_record.h>

namespace keymaster {

class SoftwareKeyBlobMaker {
  protected:
    // make destructor protected so only implementers can destroy instances.
    virtual ~SoftwareKeyBlobMaker() {}

  public:
    /**
     * CreateKeyBlob takes authorization sets and key material and produces a key blob and hardware
     * and software authorization lists ready to be returned to the AndroidKeymaster client
     * (Keystore, generally).  The blob must be integrity-checked and may be encrypted, depending
     * on the needs of the context.
     */
    virtual keymaster_error_t CreateKeyBlob(const AuthorizationSet& key_description,
                                            keymaster_key_origin_t origin,
                                            const KeymasterKeyBlob& key_material,
                                            KeymasterKeyBlob* blob, AuthorizationSet* hw_enforced,
                                            AuthorizationSet* sw_enforced) const = 0;
};

class SoftKeyFactoryMixin {
  public:
    explicit SoftKeyFactoryMixin(const SoftwareKeyBlobMaker* blob_maker)
        : blob_maker_(*blob_maker) {}
    virtual ~SoftKeyFactoryMixin() {}

  protected:
    const SoftwareKeyBlobMaker& blob_maker_;
};

}  // namespace keymaster

#endif  // SYSTEM_KEYMASTER_SOFTWARE_KEY_FACTORY_H_
