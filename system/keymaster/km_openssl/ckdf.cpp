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

#include <keymaster/km_openssl/ckdf.h>

#include <assert.h>

#include <openssl/aes.h>
#include <openssl/cmac.h>

#include <keymaster/km_openssl/openssl_err.h>
#include <keymaster/km_openssl/openssl_utils.h>
#include <keymaster/serializable.h>

namespace keymaster {

inline uint32_t div_round_up(uint32_t dividend, uint32_t divisor) {
    return (dividend + divisor - 1) / divisor;
}

size_t min(size_t a, size_t b) {
    return a < b ? a : b;
}

DEFINE_OPENSSL_OBJECT_POINTER(CMAC_CTX)

keymaster_error_t ckdf(const KeymasterKeyBlob& key, const KeymasterBlob& label,
                       const keymaster_blob_t* context_chunks, size_t num_chunks,
                       KeymasterKeyBlob* output) {
    // Note: the variables i and L correspond to i and L in the standard.  See page 12 of
    // http://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-108.pdf.

    const uint32_t blocks = div_round_up(output->key_material_size, AES_BLOCK_SIZE);
    const uint32_t L = output->key_material_size * 8;  // bits
    const uint32_t net_order_L = hton(L);

    CMAC_CTX_Ptr ctx(CMAC_CTX_new());
    if (!ctx.get()) return KM_ERROR_MEMORY_ALLOCATION_FAILED;

    auto algo = EVP_aes_128_cbc();
    switch (key.key_material_size) {
    case AES_BLOCK_SIZE:
        /* Already set */
        break;
    case AES_BLOCK_SIZE * 2:
        algo = EVP_aes_256_cbc();
        break;
    default:
        return KM_ERROR_UNSUPPORTED_KEY_SIZE;
    }

    if (!CMAC_Init(ctx.get(), key.key_material, key.key_material_size, algo,
                   nullptr /* engine */)) {
        return TranslateLastOpenSslError();
    }

    auto output_pos = const_cast<uint8_t*>(output->begin());
    memset(output_pos, 0, output->key_material_size);
    for (uint32_t i = 1; i <= blocks; ++i) {
        // Data to mac is i || label || 0x00 || context || L, with i and L represented in 32 bits,
        // in network order.

        // i
        uint32_t net_order_i = hton(i);
        if (!CMAC_Update(ctx.get(), reinterpret_cast<uint8_t*>(&net_order_i),
                         sizeof(net_order_i))) {
            return TranslateLastOpenSslError();
        }

        // label
        if (!CMAC_Update(ctx.get(), label.data, label.data_length)) {
            return TranslateLastOpenSslError();
        }

        // 0x00
        uint8_t zero = 0;
        if (!CMAC_Update(ctx.get(), &zero, sizeof(zero))) return TranslateLastOpenSslError();

        // context
        for (size_t chunk = 0; chunk < num_chunks; ++chunk) {
            if (!CMAC_Update(ctx.get(), context_chunks[chunk].data,
                             context_chunks[chunk].data_length)) {
                return TranslateLastOpenSslError();
            }
        }

        // L
        uint8_t buf[4];
        memcpy(buf, &net_order_L, 4);
        if (!CMAC_Update(ctx.get(), buf, sizeof(buf))) TranslateLastOpenSslError();

        size_t out_len;
        if (output_pos <= output->end() - AES_BLOCK_SIZE) {
            if (!CMAC_Final(ctx.get(), output_pos, &out_len)) return TranslateLastOpenSslError();
            output_pos += out_len;
        } else {
            uint8_t cmac[AES_BLOCK_SIZE];
            if (!CMAC_Final(ctx.get(), cmac, &out_len)) return TranslateLastOpenSslError();
            size_t to_copy = output->end() - output_pos;
            memcpy(output_pos, cmac, to_copy);
            output_pos += to_copy;
        }

        CMAC_Reset(ctx.get());
    }
    assert(output_pos == output->end());

    return KM_ERROR_OK;
}

}  // namespace keymaster
