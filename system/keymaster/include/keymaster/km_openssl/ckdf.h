/*
 * Copyright 2017 The Android Open Source Project
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

#ifndef SYSTEM_KEYMASTER_CKDF_H_
#define SYSTEM_KEYMASTER_CKDF_H_

#include <keymaster/android_keymaster_utils.h>

namespace keymaster {

/**
 * Implementation of CKDF, aka AES-CMAC KDF, from NIST SP 800-108.  Uses 32-bit i and L, and
 * prefixes with i.  This version takes the context in an array of keymaster_blob_ts.
 */
keymaster_error_t ckdf(const KeymasterKeyBlob& key, const KeymasterBlob& label,
                       const keymaster_blob_t* context_chunks, size_t num_chunks,
                       KeymasterKeyBlob* output);

/**
 * Implementation of CKDF, aka AES-CMAC KDF, from NIST SP 800-108.  Uses 32-bit i and L, and
 * prefixes with i.  This version takes the context as a single keymaster_blob_t&.
 */
inline keymaster_error_t ckdf(const KeymasterKeyBlob& key, const KeymasterBlob& label,
                              const keymaster_blob_t& context_chunks, KeymasterKeyBlob* output) {
    return ckdf(key, label, &context_chunks, 1 /* num_chunks */, output);
}

}  // namespace keymaster

#endif  // SYSTEM_KEYMASTER_KDF_H_
