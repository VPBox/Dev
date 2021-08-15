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

#include "crypto.h"

extern "C" {
#include <string.h>

#include <openssl/mem.h>
#include <openssl/sha.h>
}  // extern "C"

namespace nvram {
namespace crypto {

void SHA256(const uint8_t* data,
            size_t data_size,
            uint8_t* digest,
            size_t digest_size) {
  // SHA256 requires an output buffer of at least SHA256_DIGEST_LENGTH.
  // |digest_size| might be less, so store the digest in a local buffer.
  uint8_t buffer[SHA256_DIGEST_LENGTH];
  ::SHA256(data, data_size, buffer);

  // Copy the result to |digest|.
  if (digest_size < sizeof(buffer)) {
    memcpy(digest, buffer, digest_size);
  } else {
    memcpy(digest, buffer, sizeof(buffer));
    memset(digest + sizeof(buffer), 0, digest_size - sizeof(buffer));
  }
}

}  // namespace crypto
}  // namespace nvram
