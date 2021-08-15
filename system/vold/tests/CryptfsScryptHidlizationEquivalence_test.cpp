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

#define LOG_TAG "scrypt_test"
#include <log/log.h>

#include <gtest/gtest.h>
#include <hardware/keymaster0.h>
#include <hardware/keymaster1.h>
#include <cstring>

#include "../Keymaster.h"
#include "../cryptfs.h"

#ifdef CONFIG_HW_DISK_ENCRYPTION
#include "cryptfs_hw.h"
#endif

#define min(a, b) ((a) < (b) ? (a) : (b))

/* Maximum allowed keymaster blob size. */
#define KEYMASTER_BLOB_SIZE 2048

/* Key Derivation Function algorithms */
#define KDF_PBKDF2 1
#define KDF_SCRYPT 2
/* Algorithms 3 & 4 deprecated before shipping outside of google, so removed */
#define KDF_SCRYPT_KEYMASTER 5

#define KEY_LEN_BYTES 16

#define DEFAULT_PASSWORD "default_password"

#define RSA_KEY_SIZE 2048
#define RSA_KEY_SIZE_BYTES (RSA_KEY_SIZE / 8)
#define RSA_EXPONENT 0x10001
#define KEYMASTER_CRYPTFS_RATE_LIMIT 1  // Maximum one try per second

static int keymaster_init(keymaster0_device_t** keymaster0_dev,
                          keymaster1_device_t** keymaster1_dev) {
    int rc;

    const hw_module_t* mod;
    rc = hw_get_module_by_class(KEYSTORE_HARDWARE_MODULE_ID, NULL, &mod);
    if (rc) {
        ALOGE("could not find any keystore module");
        goto err;
    }

    SLOGI("keymaster module name is %s", mod->name);
    SLOGI("keymaster version is %d", mod->module_api_version);

    *keymaster0_dev = NULL;
    *keymaster1_dev = NULL;
    if (mod->module_api_version == KEYMASTER_MODULE_API_VERSION_1_0) {
        SLOGI("Found keymaster1 module, using keymaster1 API.");
        rc = keymaster1_open(mod, keymaster1_dev);
    } else {
        SLOGI("Found keymaster0 module, using keymaster0 API.");
        rc = keymaster0_open(mod, keymaster0_dev);
    }

    if (rc) {
        ALOGE("could not open keymaster device in %s (%s)", KEYSTORE_HARDWARE_MODULE_ID,
              strerror(-rc));
        goto err;
    }

    return 0;

err:
    *keymaster0_dev = NULL;
    *keymaster1_dev = NULL;
    return rc;
}

/* Should we use keymaster? */
static int keymaster_check_compatibility_old() {
    keymaster0_device_t* keymaster0_dev = 0;
    keymaster1_device_t* keymaster1_dev = 0;
    int rc = 0;

    if (keymaster_init(&keymaster0_dev, &keymaster1_dev)) {
        SLOGE("Failed to init keymaster");
        rc = -1;
        goto out;
    }

    if (keymaster1_dev) {
        rc = 1;
        goto out;
    }

    if (!keymaster0_dev || !keymaster0_dev->common.module) {
        rc = -1;
        goto out;
    }

    // TODO(swillden): Check to see if there's any reason to require v0.3.  I think v0.1 and v0.2
    // should work.
    if (keymaster0_dev->common.module->module_api_version < KEYMASTER_MODULE_API_VERSION_0_3) {
        rc = 0;
        goto out;
    }

    if (!(keymaster0_dev->flags & KEYMASTER_SOFTWARE_ONLY) &&
        (keymaster0_dev->flags & KEYMASTER_BLOBS_ARE_STANDALONE)) {
        rc = 1;
    }

out:
    if (keymaster1_dev) {
        keymaster1_close(keymaster1_dev);
    }
    if (keymaster0_dev) {
        keymaster0_close(keymaster0_dev);
    }
    return rc;
}

/* Create a new keymaster key and store it in this footer */
static int keymaster_create_key_old(struct crypt_mnt_ftr* ftr) {
    uint8_t* key = 0;
    keymaster0_device_t* keymaster0_dev = 0;
    keymaster1_device_t* keymaster1_dev = 0;

    if (ftr->keymaster_blob_size) {
        SLOGI("Already have key");
        return 0;
    }

    if (keymaster_init(&keymaster0_dev, &keymaster1_dev)) {
        SLOGE("Failed to init keymaster");
        return -1;
    }

    int rc = 0;
    size_t key_size = 0;
    if (keymaster1_dev) {
        keymaster_key_param_t params[] = {
            /* Algorithm & size specifications.  Stick with RSA for now.  Switch to AES later. */
            keymaster_param_enum(KM_TAG_ALGORITHM, KM_ALGORITHM_RSA),
            keymaster_param_int(KM_TAG_KEY_SIZE, RSA_KEY_SIZE),
            keymaster_param_long(KM_TAG_RSA_PUBLIC_EXPONENT, RSA_EXPONENT),

            /* The only allowed purpose for this key is signing. */
            keymaster_param_enum(KM_TAG_PURPOSE, KM_PURPOSE_SIGN),

            /* Padding & digest specifications. */
            keymaster_param_enum(KM_TAG_PADDING, KM_PAD_NONE),
            keymaster_param_enum(KM_TAG_DIGEST, KM_DIGEST_NONE),

            /* Require that the key be usable in standalone mode.  File system isn't available. */
            keymaster_param_enum(KM_TAG_BLOB_USAGE_REQUIREMENTS, KM_BLOB_STANDALONE),

            /* No auth requirements, because cryptfs is not yet integrated with gatekeeper. */
            keymaster_param_bool(KM_TAG_NO_AUTH_REQUIRED),

            /* Rate-limit key usage attempts, to rate-limit brute force */
            keymaster_param_int(KM_TAG_MIN_SECONDS_BETWEEN_OPS, KEYMASTER_CRYPTFS_RATE_LIMIT),
        };
        keymaster_key_param_set_t param_set = {params, sizeof(params) / sizeof(*params)};
        keymaster_key_blob_t key_blob;
        keymaster_error_t error = keymaster1_dev->generate_key(
            keymaster1_dev, &param_set, &key_blob, NULL /* characteristics */);
        if (error != KM_ERROR_OK) {
            SLOGE("Failed to generate keymaster1 key, error %d", error);
            rc = -1;
            goto out;
        }

        key = (uint8_t*)key_blob.key_material;
        key_size = key_blob.key_material_size;
    } else if (keymaster0_dev) {
        keymaster_rsa_keygen_params_t params;
        memset(&params, '\0', sizeof(params));
        params.public_exponent = RSA_EXPONENT;
        params.modulus_size = RSA_KEY_SIZE;

        if (keymaster0_dev->generate_keypair(keymaster0_dev, TYPE_RSA, &params, &key, &key_size)) {
            SLOGE("Failed to generate keypair");
            rc = -1;
            goto out;
        }
    } else {
        SLOGE("Cryptfs bug: keymaster_init succeeded but didn't initialize a device");
        rc = -1;
        goto out;
    }

    if (key_size > KEYMASTER_BLOB_SIZE) {
        SLOGE("Keymaster key too large for crypto footer");
        rc = -1;
        goto out;
    }

    memcpy(ftr->keymaster_blob, key, key_size);
    ftr->keymaster_blob_size = key_size;

out:
    if (keymaster0_dev) keymaster0_close(keymaster0_dev);
    if (keymaster1_dev) keymaster1_close(keymaster1_dev);
    free(key);
    return rc;
}

/* This signs the given object using the keymaster key. */
static int keymaster_sign_object_old(struct crypt_mnt_ftr* ftr, const unsigned char* object,
                                     const size_t object_size, unsigned char** signature,
                                     size_t* signature_size) {
    int rc = 0;
    keymaster0_device_t* keymaster0_dev = 0;
    keymaster1_device_t* keymaster1_dev = 0;

    unsigned char to_sign[RSA_KEY_SIZE_BYTES];
    size_t to_sign_size = sizeof(to_sign);
    memset(to_sign, 0, RSA_KEY_SIZE_BYTES);

    if (keymaster_init(&keymaster0_dev, &keymaster1_dev)) {
        SLOGE("Failed to init keymaster");
        rc = -1;
        goto out;
    }

    // To sign a message with RSA, the message must satisfy two
    // constraints:
    //
    // 1. The message, when interpreted as a big-endian numeric value, must
    //    be strictly less than the public modulus of the RSA key.  Note
    //    that because the most significant bit of the public modulus is
    //    guaranteed to be 1 (else it's an (n-1)-bit key, not an n-bit
    //    key), an n-bit message with most significant bit 0 always
    //    satisfies this requirement.
    //
    // 2. The message must have the same length in bits as the public
    //    modulus of the RSA key.  This requirement isn't mathematically
    //    necessary, but is necessary to ensure consistency in
    //    implementations.
    switch (ftr->kdf_type) {
        case KDF_SCRYPT_KEYMASTER:
            // This ensures the most significant byte of the signed message
            // is zero.  We could have zero-padded to the left instead, but
            // this approach is slightly more robust against changes in
            // object size.  However, it's still broken (but not unusably
            // so) because we really should be using a proper deterministic
            // RSA padding function, such as PKCS1.
            memcpy(to_sign + 1, object, min(RSA_KEY_SIZE_BYTES - 1, object_size));
            SLOGI("Signing safely-padded object");
            break;
        default:
            SLOGE("Unknown KDF type %d", ftr->kdf_type);
            rc = -1;
            goto out;
    }

    if (keymaster0_dev) {
        keymaster_rsa_sign_params_t params;
        params.digest_type = DIGEST_NONE;
        params.padding_type = PADDING_NONE;

        rc = keymaster0_dev->sign_data(keymaster0_dev, &params, ftr->keymaster_blob,
                                       ftr->keymaster_blob_size, to_sign, to_sign_size, signature,
                                       signature_size);
        goto out;
    } else if (keymaster1_dev) {
        keymaster_key_blob_t key = {ftr->keymaster_blob, ftr->keymaster_blob_size};
        keymaster_key_param_t params[] = {
            keymaster_param_enum(KM_TAG_PADDING, KM_PAD_NONE),
            keymaster_param_enum(KM_TAG_DIGEST, KM_DIGEST_NONE),
        };
        keymaster_key_param_set_t param_set = {params, sizeof(params) / sizeof(*params)};
        keymaster_operation_handle_t op_handle;
        keymaster_error_t error = keymaster1_dev->begin(
            keymaster1_dev, KM_PURPOSE_SIGN, &key, &param_set, NULL /* out_params */, &op_handle);
        if (error == KM_ERROR_KEY_RATE_LIMIT_EXCEEDED) {
            // Key usage has been rate-limited.  Wait a bit and try again.
            sleep(KEYMASTER_CRYPTFS_RATE_LIMIT);
            error = keymaster1_dev->begin(keymaster1_dev, KM_PURPOSE_SIGN, &key, &param_set,
                                          NULL /* out_params */, &op_handle);
        }
        if (error != KM_ERROR_OK) {
            SLOGE("Error starting keymaster signature transaction: %d", error);
            rc = -1;
            goto out;
        }

        keymaster_blob_t input = {to_sign, to_sign_size};
        size_t input_consumed;
        error = keymaster1_dev->update(keymaster1_dev, op_handle, NULL /* in_params */, &input,
                                       &input_consumed, NULL /* out_params */, NULL /* output */);
        if (error != KM_ERROR_OK) {
            SLOGE("Error sending data to keymaster signature transaction: %d", error);
            rc = -1;
            goto out;
        }
        if (input_consumed != to_sign_size) {
            // This should never happen.  If it does, it's a bug in the keymaster implementation.
            SLOGE("Keymaster update() did not consume all data.");
            keymaster1_dev->abort(keymaster1_dev, op_handle);
            rc = -1;
            goto out;
        }

        keymaster_blob_t tmp_sig;
        error = keymaster1_dev->finish(keymaster1_dev, op_handle, NULL /* in_params */,
                                       NULL /* verify signature */, NULL /* out_params */, &tmp_sig);
        if (error != KM_ERROR_OK) {
            SLOGE("Error finishing keymaster signature transaction: %d", error);
            rc = -1;
            goto out;
        }

        *signature = (uint8_t*)tmp_sig.data;
        *signature_size = tmp_sig.data_length;
    } else {
        SLOGE("Cryptfs bug: keymaster_init succeded but didn't initialize a device.");
        rc = -1;
        goto out;
    }

out:
    if (keymaster1_dev) keymaster1_close(keymaster1_dev);
    if (keymaster0_dev) keymaster0_close(keymaster0_dev);

    return rc;
}

/* Should we use keymaster? */
static int keymaster_check_compatibility_new() {
    return keymaster_compatibility_cryptfs_scrypt();
}

#if 0
/* Create a new keymaster key and store it in this footer */
static int keymaster_create_key_new(struct crypt_mnt_ftr *ftr)
{
    if (ftr->keymaster_blob_size) {
        SLOGI("Already have key");
        return 0;
    }

    int rc = keymaster_create_key_for_cryptfs_scrypt(RSA_KEY_SIZE, RSA_EXPONENT,
            KEYMASTER_CRYPTFS_RATE_LIMIT, ftr->keymaster_blob, KEYMASTER_BLOB_SIZE,
            &ftr->keymaster_blob_size);
    if (rc) {
        if (ftr->keymaster_blob_size > KEYMASTER_BLOB_SIZE) {
            SLOGE("Keymaster key blob to large)");
            ftr->keymaster_blob_size = 0;
        }
        SLOGE("Failed to generate keypair");
        return -1;
    }
    return 0;
}
#endif

/* This signs the given object using the keymaster key. */
static int keymaster_sign_object_new(struct crypt_mnt_ftr* ftr, const unsigned char* object,
                                     const size_t object_size, unsigned char** signature,
                                     size_t* signature_size) {
    unsigned char to_sign[RSA_KEY_SIZE_BYTES];
    size_t to_sign_size = sizeof(to_sign);
    memset(to_sign, 0, RSA_KEY_SIZE_BYTES);

    // To sign a message with RSA, the message must satisfy two
    // constraints:
    //
    // 1. The message, when interpreted as a big-endian numeric value, must
    //    be strictly less than the public modulus of the RSA key.  Note
    //    that because the most significant bit of the public modulus is
    //    guaranteed to be 1 (else it's an (n-1)-bit key, not an n-bit
    //    key), an n-bit message with most significant bit 0 always
    //    satisfies this requirement.
    //
    // 2. The message must have the same length in bits as the public
    //    modulus of the RSA key.  This requirement isn't mathematically
    //    necessary, but is necessary to ensure consistency in
    //    implementations.
    switch (ftr->kdf_type) {
        case KDF_SCRYPT_KEYMASTER:
            // This ensures the most significant byte of the signed message
            // is zero.  We could have zero-padded to the left instead, but
            // this approach is slightly more robust against changes in
            // object size.  However, it's still broken (but not unusably
            // so) because we really should be using a proper deterministic
            // RSA padding function, such as PKCS1.
            memcpy(to_sign + 1, object, min(RSA_KEY_SIZE_BYTES - 1, object_size));
            SLOGI("Signing safely-padded object");
            break;
        default:
            SLOGE("Unknown KDF type %d", ftr->kdf_type);
            return -1;
    }
    if (keymaster_sign_object_for_cryptfs_scrypt(
            ftr->keymaster_blob, ftr->keymaster_blob_size, KEYMASTER_CRYPTFS_RATE_LIMIT, to_sign,
            to_sign_size, signature, signature_size) != KeymasterSignResult::ok)
        return -1;
    return 0;
}

namespace android {

class CryptFsTest : public testing::Test {
  protected:
    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST_F(CryptFsTest, ScryptHidlizationEquivalenceTest) {
    crypt_mnt_ftr ftr;
    ftr.kdf_type = KDF_SCRYPT_KEYMASTER;
    ftr.keymaster_blob_size = 0;

    ASSERT_EQ(0, keymaster_create_key_old(&ftr));

    uint8_t* sig1 = nullptr;
    uint8_t* sig2 = nullptr;
    size_t sig_size1 = 123456789;
    size_t sig_size2 = 123456789;
    uint8_t object[] = "the object";

    ASSERT_EQ(1, keymaster_check_compatibility_old());
    ASSERT_EQ(1, keymaster_check_compatibility_new());
    ASSERT_EQ(0, keymaster_sign_object_old(&ftr, object, 10, &sig1, &sig_size1));
    ASSERT_EQ(0, keymaster_sign_object_new(&ftr, object, 10, &sig2, &sig_size2));

    ASSERT_EQ(sig_size1, sig_size2);
    ASSERT_NE(nullptr, sig1);
    ASSERT_NE(nullptr, sig2);
    EXPECT_EQ(0, memcmp(sig1, sig2, sig_size1));
    free(sig1);
    free(sig2);
}

}  // namespace android
