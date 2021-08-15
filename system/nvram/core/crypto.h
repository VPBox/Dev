/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef NVRAM_CORE_CRYPTO_H_
#define NVRAM_CORE_CRYPTO_H_

extern "C" {
#include <stddef.h>
#include <stdint.h>
}  // extern "C"

namespace nvram {
namespace crypto {

// Size of a SHA-256 digest in bytes.
constexpr size_t kSHA256DigestSize = 32;

// Computes the SHA-256 digest of the |data_size| input bytes stored at |data|.
// The digest is written to |digest|, which is a buffer of size |digest_size|.
// Note that |digest_size| doesn't have to match SHA-256's output size of 32
// bytes. If it doesn't the digest is truncated or zero-padded as necessary.
//
// Returns true if the digest was computed successfully, false otherwise.
void SHA256(const uint8_t* data,
            size_t data_size,
            uint8_t* digest,
            size_t digest_size);

}  // namespace crypto
}  // namespace nvram

#endif  // NVRAM_CORE_CRYPTO_H_
