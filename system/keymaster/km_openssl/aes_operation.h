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

#ifndef SYSTEM_KEYMASTER_AES_OPERATION_H_
#define SYSTEM_KEYMASTER_AES_OPERATION_H_

#include <openssl/aes.h>

#include "block_cipher_operation.h"

namespace keymaster {

class AesEvpCipherDescription : public EvpCipherDescription {
  public:
    keymaster_algorithm_t algorithm() const override { return KM_ALGORITHM_AES; }

    const keymaster_block_mode_t* SupportedBlockModes(size_t* block_mode_count) const override;

    const EVP_CIPHER* GetCipherInstance(size_t key_size, keymaster_block_mode_t block_mode,
                                        keymaster_error_t* error) const override;

    size_t block_size_bytes() const override { return AES_BLOCK_SIZE; }
};

class AesOperationFactory : public BlockCipherOperationFactory {
  public:
    explicit AesOperationFactory(keymaster_purpose_t purpose)
        : BlockCipherOperationFactory(purpose) {}
    const EvpCipherDescription& GetCipherDescription() const override;
};

}  // namespace keymaster

#endif  // SYSTEM_KEYMASTER_AES_OPERATION_H_
