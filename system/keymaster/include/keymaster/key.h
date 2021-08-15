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

#ifndef SYSTEM_KEYMASTER_KEY_H_
#define SYSTEM_KEYMASTER_KEY_H_

#include <assert.h>

#include <hardware/keymaster_defs.h>
#include <keymaster/UniquePtr.h>
#include <keymaster/android_keymaster_utils.h>
#include <keymaster/authorization_set.h>

namespace keymaster {

class KeyFactory;

class Key {
  public:
    virtual ~Key() {}
    Key(const Key&) = delete;
    void operator=(const Key&) = delete;

    /**
     * Return a copy of raw key material, in the specified format.
     */
    virtual keymaster_error_t formatted_key_material(keymaster_key_format_t format,
                                                     UniquePtr<uint8_t[]>* material,
                                                     size_t* size) const = 0;

    AuthProxy authorizations() const { return AuthProxy(hw_enforced_, sw_enforced_); }
    const AuthorizationSet& hw_enforced() const { return hw_enforced_; }
    const AuthorizationSet& sw_enforced() const { return sw_enforced_; }
    AuthorizationSet& hw_enforced() { return hw_enforced_; }
    AuthorizationSet& sw_enforced() { return sw_enforced_; }

    const KeymasterKeyBlob& key_material() const { return key_material_; }
    KeymasterKeyBlob& key_material() { return key_material_; }

    // Methods to move data out of the key.  These could be overloads of the methods above, with ref
    // qualifiers, but naming them differently makes it harder to accidentally make a temporary copy
    // when we mean to move.
    AuthorizationSet&& hw_enforced_move() { return move(hw_enforced_); }
    AuthorizationSet&& sw_enforced_move() { return move(sw_enforced_); }
    KeymasterKeyBlob&& key_material_move() { return move(key_material_); }

    const KeyFactory* key_factory() const { return key_factory_; }
    const KeyFactory*& key_factory() { return key_factory_; }

  protected:
    Key(AuthorizationSet&& hw_enforced, AuthorizationSet&& sw_enforced,
        const KeyFactory* key_factory)
        : hw_enforced_(move(hw_enforced)), sw_enforced_(move(sw_enforced)),
          key_factory_(key_factory) {}

  protected:
    AuthorizationSet hw_enforced_;
    AuthorizationSet sw_enforced_;
    KeymasterKeyBlob key_material_;
    const KeyFactory* key_factory_;
};

}  // namespace keymaster

#endif  // SYSTEM_KEYMASTER_KEY_H_
