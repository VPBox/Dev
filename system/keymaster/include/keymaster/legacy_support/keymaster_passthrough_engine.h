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

#ifndef SYSTEM_KEYMASTER_KEYMASTER2_ENGINE_H_
#define SYSTEM_KEYMASTER_KEYMASTER2_ENGINE_H_

#include <memory>

#include <hardware/keymaster_defs.h>

#include <keymaster/UniquePtr.h>

struct keymaster1_device;
typedef struct keymaster1_device keymaster1_device_t;
struct keymaster2_device;
typedef struct keymaster2_device keymaster2_device_t;

namespace keymaster {

template <typename BlobType> struct TKeymasterBlob;
typedef TKeymasterBlob<keymaster_key_blob_t> KeymasterKeyBlob;
typedef TKeymasterBlob<keymaster_blob_t> KeymasterBlob;
class AuthorizationSet;
class OperationFactory;

class KeymasterPassthroughEngine {
  public:
    virtual ~KeymasterPassthroughEngine() {}
    virtual keymaster_error_t GenerateKey(const AuthorizationSet& key_description,
                                  KeymasterKeyBlob* key_material, AuthorizationSet* hw_enforced,
                                  AuthorizationSet* sw_enforced) const = 0;

    virtual keymaster_error_t ImportKey(const AuthorizationSet& key_description,
                                keymaster_key_format_t input_key_material_format,
                                const KeymasterKeyBlob& input_key_material,
                                KeymasterKeyBlob* output_key_blob, AuthorizationSet* hw_enforced,
                                AuthorizationSet* sw_enforced) const = 0;
    virtual keymaster_error_t ExportKey(keymaster_key_format_t format,
            const KeymasterKeyBlob& blob,
            const KeymasterBlob& client_id,
            const KeymasterBlob& app_data,
            KeymasterBlob* export_data) const = 0;
    virtual keymaster_error_t DeleteKey(const KeymasterKeyBlob& blob) const = 0;
    virtual keymaster_error_t DeleteAllKeys() const = 0;
    virtual OperationFactory* GetOperationFactory(keymaster_purpose_t purpose,
                                                  keymaster_algorithm_t algorithm) const = 0;

    static UniquePtr<KeymasterPassthroughEngine>
    createInstance(const keymaster1_device_t* dev);
    static UniquePtr<KeymasterPassthroughEngine>
    createInstance(const keymaster2_device_t* dev);
  protected:
    KeymasterPassthroughEngine() {}
};

} // namespace keymaster

#endif  // SYSTEM_KEYMASTER_KEYMASTER2_ENGINE_H_
