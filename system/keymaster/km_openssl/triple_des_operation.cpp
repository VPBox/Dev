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

#include "triple_des_operation.h"

namespace keymaster {

static const keymaster_block_mode_t supported_block_modes[] = {KM_MODE_ECB, KM_MODE_CBC};

const keymaster_block_mode_t*
TripleDesEvpCipherDescription::SupportedBlockModes(size_t* block_mode_count) const {
    *block_mode_count = array_length(supported_block_modes);
    return supported_block_modes;
}

const EVP_CIPHER*
TripleDesEvpCipherDescription::GetCipherInstance(size_t key_size, keymaster_block_mode_t block_mode,
                                                 keymaster_error_t* error) const {
    *error = KM_ERROR_OK;

    switch (block_mode) {
    case KM_MODE_ECB:
        switch (key_size) {
        case 16:
            return EVP_des_ede();  // Note: OpenSSL 1.1.0 renamed this to EVP_des_ede_ecb
        case 24:
            return EVP_des_ede3();  // Note: OpenSSL 1.1.0 renamed this to EVP_des_ede3_ecb
        default:
            *error = KM_ERROR_UNSUPPORTED_KEY_SIZE;
            break;
        }
        break;

    case KM_MODE_CBC:
        switch (key_size) {
        case 16:
            return EVP_des_ede_cbc();
        case 24:
            return EVP_des_ede3_cbc();
        default:
            *error = KM_ERROR_UNSUPPORTED_KEY_SIZE;
            break;
        }
        break;

    default:
        *error = KM_ERROR_UNSUPPORTED_BLOCK_MODE;
        break;
    }

    assert(*error != KM_ERROR_OK);
    return nullptr;
}

static TripleDesEvpCipherDescription description;
const EvpCipherDescription& TripleDesOperationFactory::GetCipherDescription() const {
    return description;
}

}  // namespace keymaster
