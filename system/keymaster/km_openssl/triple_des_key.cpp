/*
 * Copyright 2018 The Android Open Source Project
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

#include <keymaster/km_openssl/triple_des_key.h>

#include <assert.h>

#include <keymaster/new>

#include <openssl/err.h>
#include <openssl/rand.h>

#include "triple_des_operation.h"

namespace keymaster {

static TripleDesOperationFactory encrypt_factory(KM_PURPOSE_ENCRYPT);
static TripleDesOperationFactory decrypt_factory(KM_PURPOSE_DECRYPT);

OperationFactory* TripleDesKeyFactory::GetOperationFactory(keymaster_purpose_t purpose) const {
    switch (purpose) {
    case KM_PURPOSE_ENCRYPT:
        return &encrypt_factory;
    case KM_PURPOSE_DECRYPT:
        return &decrypt_factory;
    default:
        return nullptr;
    }
}

keymaster_error_t TripleDesKeyFactory::LoadKey(KeymasterKeyBlob&& key_material,
                                               const AuthorizationSet& /* additional_params */,
                                               AuthorizationSet&& hw_enforced,
                                               AuthorizationSet&& sw_enforced,
                                               UniquePtr<Key>* key) const {
    if (!key) return KM_ERROR_OUTPUT_PARAMETER_NULL;

    keymaster_error_t error = KM_ERROR_OK;
    key->reset(new (std::nothrow)
                   TripleDesKey(move(key_material), move(hw_enforced), move(sw_enforced), this));
    if (!key->get()) error = KM_ERROR_MEMORY_ALLOCATION_FAILED;
    return error;
}

keymaster_error_t TripleDesKeyFactory::validate_algorithm_specific_new_key_params(
    const AuthorizationSet& key_description) const {
    if (key_description.Contains(TAG_MIN_MAC_LENGTH)) return KM_ERROR_INVALID_TAG;
    return KM_ERROR_OK;
}

}  // namespace keymaster
