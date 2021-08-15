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

#ifndef SYSTEM_KEYMASTER_TRIPLE_DES_OPERATION_H_
#define SYSTEM_KEYMASTER_TRIPLE_DES_OPERATION_H_

#include <openssl/des.h>

#include "block_cipher_operation.h"

namespace keymaster {

class TripleDesEvpCipherDescription : public EvpCipherDescription {
  public:
    keymaster_algorithm_t algorithm() const override { return KM_ALGORITHM_TRIPLE_DES; }

    const keymaster_block_mode_t* SupportedBlockModes(size_t* block_mode_count) const override;

    const EVP_CIPHER* GetCipherInstance(size_t key_size, keymaster_block_mode_t block_mode,
                                        keymaster_error_t* error) const override;

    size_t block_size_bytes() const override { return 8 /* DES_BLOCK_SIZE */; }
};

class TripleDesOperationFactory : public BlockCipherOperationFactory {
  public:
    explicit TripleDesOperationFactory(keymaster_purpose_t purpose)
        : BlockCipherOperationFactory(purpose) {}
    const EvpCipherDescription& GetCipherDescription() const override;
};

}  // namespace keymaster

#endif  // SYSTEM_KEYMASTER_TRIPLE_DES_OPERATION_H_
