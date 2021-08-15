/* Copyright 2014 The Android Open Source Project
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#define LOG_TAG "keystore-engine"

#include <pthread.h>
#include <sys/socket.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#include <log/log.h>

#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/ec_key.h>
#include <openssl/ecdsa.h>
#include <openssl/engine.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>

#include <memory>

#ifndef BACKEND_WIFI_HIDL
#include "keystore_backend_binder.h"
#else
#include "keystore_backend_hidl.h"
#endif

namespace {
KeystoreBackend *g_keystore_backend;
void ensure_keystore_engine();

/* key_id_dup is called when one of the RSA or EC_KEY objects is duplicated. */
int key_id_dup(CRYPTO_EX_DATA* /* to */,
               const CRYPTO_EX_DATA* /* from */,
               void** from_d,
               int /* index */,
               long /* argl */,
               void* /* argp */) {
    char *key_id = reinterpret_cast<char *>(*from_d);
    if (key_id != nullptr) {
        *from_d = strdup(key_id);
    }
    return 1;
}

/* key_id_free is called when one of the RSA, DSA or EC_KEY object is freed. */
void key_id_free(void* /* parent */,
                 void* ptr,
                 CRYPTO_EX_DATA* /* ad */,
                 int /* index */,
                 long /* argl */,
                 void* /* argp */) {
    char *key_id = reinterpret_cast<char *>(ptr);
    free(key_id);
}

/* Many OpenSSL APIs take ownership of an argument on success but don't free
 * the argument on failure. This means we need to tell our scoped pointers when
 * we've transferred ownership, without triggering a warning by not using the
 * result of release(). */
#define OWNERSHIP_TRANSFERRED(obj) auto _dummy __attribute__((unused)) = (obj).release()

const char* rsa_get_key_id(const RSA* rsa);

/* rsa_private_transform takes a big-endian integer from |in|, calculates the
 * d'th power of it, modulo the RSA modulus, and writes the result as a
 * big-endian integer to |out|. Both |in| and |out| are |len| bytes long. It
 * returns one on success and zero otherwise. */
int rsa_private_transform(RSA *rsa, uint8_t *out, const uint8_t *in, size_t len) {
    ALOGV("rsa_private_transform(%p, %p, %p, %u)", rsa, out, in, (unsigned) len);

    ensure_keystore_engine();

    const char *key_id = rsa_get_key_id(rsa);
    if (key_id == nullptr) {
        ALOGE("key had no key_id!");
        return 0;
    }

    uint8_t* reply = nullptr;
    size_t reply_len;
    int32_t ret = g_keystore_backend->sign(key_id, in, len, &reply, &reply_len);
    if (ret < 0) {
        ALOGW("There was an error during rsa_decrypt: could not connect");
        return 0;
    } else if (ret != 0) {
        ALOGW("Error during sign from keystore: %d", ret);
        return 0;
    } else if (reply_len == 0 || reply == nullptr) {
        ALOGW("No valid signature returned");
        return 0;
    }

    if (reply_len > len) {
        /* The result of the RSA operation can never be larger than the size of
         * the modulus so we assume that the result has extra zeros on the
         * left. This provides attackers with an oracle, but there's nothing
         * that we can do about it here. */
        ALOGW("Reply len %zu greater than expected %zu", reply_len, len);
        memcpy(out, &reply[reply_len - len], len);
    } else if (reply_len < len) {
        /* If the Keystore implementation returns a short value we assume that
         * it's because it removed leading zeros from the left side. This is
         * bad because it provides attackers with an oracle but we cannot do
         * anything about a broken Keystore implementation here. */
        ALOGW("Reply len %zu lesser than expected %zu", reply_len, len);
        memset(out, 0, len);
        memcpy(out + len - reply_len, &reply[0], reply_len);
    } else {
        memcpy(out, &reply[0], len);
    }

    ALOGV("rsa=%p keystore_rsa_priv_dec successful", rsa);
    return 1;
}

const char* ecdsa_get_key_id(const EC_KEY* ec_key);

/* ecdsa_sign signs |digest_len| bytes from |digest| with |ec_key| and writes
 * the resulting signature (an ASN.1 encoded blob) to |sig|. It returns one on
 * success and zero otherwise. */
static int ecdsa_sign(const uint8_t* digest, size_t digest_len, uint8_t* sig,
                      unsigned int* sig_len, EC_KEY* ec_key) {
    ALOGV("ecdsa_sign(%p, %u, %p)", digest, (unsigned) digest_len, ec_key);

    ensure_keystore_engine();

    const char *key_id = ecdsa_get_key_id(ec_key);
    if (key_id == nullptr) {
        ALOGE("key had no key_id!");
        return 0;
    }

    size_t ecdsa_size = ECDSA_size(ec_key);

    uint8_t* reply = nullptr;
    size_t reply_len;
    int32_t ret = g_keystore_backend->sign(
            key_id, digest, digest_len, &reply, &reply_len);
    if (ret < 0) {
        ALOGW("There was an error during ecdsa_sign: could not connect");
        return 0;
    } else if (reply_len == 0 || reply == nullptr) {
        ALOGW("No valid signature returned");
        return 0;
    } else if (reply_len > ecdsa_size) {
        ALOGW("Signature is too large");
        return 0;
    }

    // Reviewer: should't sig_len be checked here? Or is it just assumed that it is at least ecdsa_size?
    memcpy(sig, &reply[0], reply_len);
    *sig_len = reply_len;

    ALOGV("ecdsa_sign(%p, %u, %p) => success", digest, (unsigned)digest_len,
          ec_key);
    return 1;
}

/* KeystoreEngine is a BoringSSL ENGINE that implements RSA and ECDSA by
 * forwarding the requested operations to Keystore. */
class KeystoreEngine {
 public:
  KeystoreEngine()
      : rsa_index_(RSA_get_ex_new_index(0 /* argl */,
                                        nullptr /* argp */,
                                        nullptr /* new_func */,
                                        key_id_dup,
                                        key_id_free)),
        ec_key_index_(EC_KEY_get_ex_new_index(0 /* argl */,
                                              nullptr /* argp */,
                                              nullptr /* new_func */,
                                              key_id_dup,
                                              key_id_free)),
        engine_(ENGINE_new()) {
    memset(&rsa_method_, 0, sizeof(rsa_method_));
    rsa_method_.common.is_static = 1;
    rsa_method_.private_transform = rsa_private_transform;
    rsa_method_.flags = RSA_FLAG_OPAQUE;
    ENGINE_set_RSA_method(engine_, &rsa_method_, sizeof(rsa_method_));

    memset(&ecdsa_method_, 0, sizeof(ecdsa_method_));
    ecdsa_method_.common.is_static = 1;
    ecdsa_method_.sign = ecdsa_sign;
    ecdsa_method_.flags = ECDSA_FLAG_OPAQUE;
    ENGINE_set_ECDSA_method(engine_, &ecdsa_method_, sizeof(ecdsa_method_));
  }

  int rsa_ex_index() const { return rsa_index_; }
  int ec_key_ex_index() const { return ec_key_index_; }

  const ENGINE* engine() const { return engine_; }

 private:
  const int rsa_index_;
  const int ec_key_index_;
  RSA_METHOD rsa_method_;
  ECDSA_METHOD ecdsa_method_;
  ENGINE* const engine_;
};

pthread_once_t g_keystore_engine_once = PTHREAD_ONCE_INIT;
KeystoreEngine *g_keystore_engine;

/* init_keystore_engine is called to initialize |g_keystore_engine|. This
 * should only be called by |pthread_once|. */
void init_keystore_engine() {
  g_keystore_engine = new KeystoreEngine;
#ifndef BACKEND_WIFI_HIDL
  g_keystore_backend = new KeystoreBackendBinder;
#else
  g_keystore_backend = new KeystoreBackendHidl;
#endif
}

/* ensure_keystore_engine ensures that |g_keystore_engine| is pointing to a
 * valid |KeystoreEngine| object and creates one if not. */
void ensure_keystore_engine() {
  pthread_once(&g_keystore_engine_once, init_keystore_engine);
}

const char* rsa_get_key_id(const RSA* rsa) {
  return reinterpret_cast<char*>(
      RSA_get_ex_data(rsa, g_keystore_engine->rsa_ex_index()));
}

const char* ecdsa_get_key_id(const EC_KEY* ec_key) {
  return reinterpret_cast<char*>(
      EC_KEY_get_ex_data(ec_key, g_keystore_engine->ec_key_ex_index()));
}

struct EVP_PKEY_Delete {
    void operator()(EVP_PKEY* p) const {
        EVP_PKEY_free(p);
    }
};
typedef std::unique_ptr<EVP_PKEY, EVP_PKEY_Delete> Unique_EVP_PKEY;

struct RSA_Delete {
    void operator()(RSA* p) const {
        RSA_free(p);
    }
};
typedef std::unique_ptr<RSA, RSA_Delete> Unique_RSA;

struct EC_KEY_Delete {
    void operator()(EC_KEY* ec) const {
        EC_KEY_free(ec);
    }
};
typedef std::unique_ptr<EC_KEY, EC_KEY_Delete> Unique_EC_KEY;

/* wrap_rsa returns an |EVP_PKEY| that contains an RSA key where the public
 * part is taken from |public_rsa| and the private operations are forwarded to
 * KeyStore and operate on the key named |key_id|. */
static EVP_PKEY *wrap_rsa(const char *key_id, const RSA *public_rsa) {
    Unique_RSA rsa(RSA_new_method(g_keystore_engine->engine()));
    if (rsa.get() == nullptr) {
        return nullptr;
    }

    char *key_id_copy = strdup(key_id);
    if (key_id_copy == nullptr) {
        return nullptr;
    }

    if (!RSA_set_ex_data(rsa.get(), g_keystore_engine->rsa_ex_index(),
                         key_id_copy)) {
        free(key_id_copy);
        return nullptr;
    }

    rsa->n = BN_dup(public_rsa->n);
    rsa->e = BN_dup(public_rsa->e);
    if (rsa->n == nullptr || rsa->e == nullptr) {
        return nullptr;
    }

    Unique_EVP_PKEY result(EVP_PKEY_new());
    if (result.get() == nullptr ||
        !EVP_PKEY_assign_RSA(result.get(), rsa.get())) {
        return nullptr;
    }
    OWNERSHIP_TRANSFERRED(rsa);

    return result.release();
}

/* wrap_ecdsa returns an |EVP_PKEY| that contains an ECDSA key where the public
 * part is taken from |public_rsa| and the private operations are forwarded to
 * KeyStore and operate on the key named |key_id|. */
static EVP_PKEY *wrap_ecdsa(const char *key_id, const EC_KEY *public_ecdsa) {
    Unique_EC_KEY ec(EC_KEY_new_method(g_keystore_engine->engine()));
    if (ec.get() == nullptr) {
        return nullptr;
    }

    if (!EC_KEY_set_group(ec.get(), EC_KEY_get0_group(public_ecdsa)) ||
        !EC_KEY_set_public_key(ec.get(), EC_KEY_get0_public_key(public_ecdsa))) {
        return nullptr;
    }

    char *key_id_copy = strdup(key_id);
    if (key_id_copy == nullptr) {
        return nullptr;
    }

    if (!EC_KEY_set_ex_data(ec.get(), g_keystore_engine->ec_key_ex_index(),
                            key_id_copy)) {
        free(key_id_copy);
        return nullptr;
    }

    Unique_EVP_PKEY result(EVP_PKEY_new());
    if (result.get() == nullptr ||
        !EVP_PKEY_assign_EC_KEY(result.get(), ec.get())) {
        return nullptr;
    }
    OWNERSHIP_TRANSFERRED(ec);

    return result.release();
}

}  /* anonymous namespace */

extern "C" {

EVP_PKEY* EVP_PKEY_from_keystore(const char* key_id) __attribute__((visibility("default")));

/* EVP_PKEY_from_keystore returns an |EVP_PKEY| that contains either an RSA or
 * ECDSA key where the public part of the key reflects the value of the key
 * named |key_id| in Keystore and the private operations are forwarded onto
 * KeyStore. */
EVP_PKEY* EVP_PKEY_from_keystore(const char* key_id) {
    ALOGV("EVP_PKEY_from_keystore(\"%s\")", key_id);

    ensure_keystore_engine();

    uint8_t *pubkey = nullptr;
    size_t pubkey_len;
    int32_t ret = g_keystore_backend->get_pubkey(key_id, &pubkey, &pubkey_len);
    if (ret < 0) {
        ALOGW("could not contact keystore");
        return nullptr;
    } else if (ret != 0 || pubkey == nullptr) {
        ALOGW("keystore reports error: %d", ret);
        return nullptr;
    }

    const uint8_t *inp = pubkey;
    Unique_EVP_PKEY pkey(d2i_PUBKEY(nullptr, &inp, pubkey_len));
    if (pkey.get() == nullptr) {
        ALOGW("Cannot convert pubkey");
        return nullptr;
    }

    EVP_PKEY *result;
    switch (EVP_PKEY_type(pkey->type)) {
    case EVP_PKEY_RSA: {
        Unique_RSA public_rsa(EVP_PKEY_get1_RSA(pkey.get()));
        result = wrap_rsa(key_id, public_rsa.get());
        break;
    }
    case EVP_PKEY_EC: {
        Unique_EC_KEY public_ecdsa(EVP_PKEY_get1_EC_KEY(pkey.get()));
        result = wrap_ecdsa(key_id, public_ecdsa.get());
        break;
    }
    default:
        ALOGE("Unsupported key type %d", EVP_PKEY_type(pkey->type));
        result = nullptr;
    }

    return result;
}

}  // extern "C"
