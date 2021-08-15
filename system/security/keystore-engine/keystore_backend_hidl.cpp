/* Copyright 2017 The Android Open Source Project
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

#include "keystore_backend_hidl.h"

#include <android/system/wifi/keystore/1.0/IKeystore.h>
#include <log/log.h>

using android::hardware::hidl_vec;
using android::hardware::Return;
using android::sp;
using android::system::wifi::keystore::V1_0::IKeystore;

int32_t KeystoreBackendHidl::sign(
        const char *key_id, const uint8_t* in, size_t len, uint8_t** reply,
        size_t* reply_len) {
    if (key_id == nullptr || in == nullptr || reply == nullptr || reply_len == nullptr) {
        ALOGE("Null pointer argument passed");
        return -1;
    }

    sp<IKeystore> service = IKeystore::tryGetService();
    if (service == nullptr) {
        ALOGE("could not contact keystore HAL");
        return -1;
    }

    bool success = false;
    auto cb = [&](IKeystore::KeystoreStatusCode status,
                  hidl_vec<uint8_t> signedData) {
      if (status == IKeystore::KeystoreStatusCode::SUCCESS) {
          *reply_len = signedData.size();
          *reply = signedData.releaseData();
          success = true;
      }
    };
    Return<void> ret = service->sign(
        key_id, std::vector<uint8_t>(in, in + len), cb);
    if (!ret.isOk() || !success) {
        return 1;
    }
    return 0;
}

int32_t KeystoreBackendHidl::get_pubkey(
        const char *key_id, uint8_t** pubkey, size_t* pubkey_len) {
    if (key_id == nullptr || pubkey == nullptr || pubkey_len == nullptr) {
        ALOGE("Null pointer argument passed");
        return -1;
    }

    sp<IKeystore> service = IKeystore::tryGetService();
    if (service == nullptr) {
        ALOGE("could not contact keystore HAL");
        return -1;
    }

    bool success = false;
    auto cb = [&](IKeystore::KeystoreStatusCode status,
                  hidl_vec<uint8_t> publicKey) {
      if (status == IKeystore::KeystoreStatusCode::SUCCESS) {
          *pubkey_len = publicKey.size();
          *pubkey = publicKey.releaseData();
          success = true;
      }
    };
    Return<void> ret = service->getPublicKey(key_id, cb);
    if (!ret.isOk() || !success) {
        return 1;
    }
    return 0;
}
