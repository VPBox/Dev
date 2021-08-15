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

#define LOG_TAG "hwservicemanager"

#include "TokenManager.h"

#include <functional>
#include <log/log.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>

namespace android {
namespace hidl {
namespace token {
namespace V1_0 {
namespace implementation {

static void ReadRandomBytes(uint8_t *buf, size_t len) {
    int fd = TEMP_FAILURE_RETRY(open("/dev/urandom", O_RDONLY | O_CLOEXEC | O_NOFOLLOW));
    if (fd == -1) {
        ALOGE("%s: cannot read /dev/urandom", __func__);
        return;
    }

    size_t n;
    while ((n = TEMP_FAILURE_RETRY(read(fd, buf, len))) > 0) {
        len -= n;
        buf += n;
    }
    if (len > 0) {
        ALOGW("%s: there are %d bytes skipped", __func__, (int)len);
    }
    close(fd);
}

TokenManager::TokenManager() {
    ReadRandomBytes(mKey.data(), mKey.size());
}

// Methods from ::android::hidl::token::V1_0::ITokenManager follow.
Return<void> TokenManager::createToken(const sp<IBase>& store, createToken_cb hidl_cb) {
    TokenInterface interface = generateToken(store);

    if (interface.interface == nullptr) {
        hidl_cb({});
        return Void();
    }

    uint64_t id = getTokenId(interface.token);

    if (id != interface.id) {
        ALOGE("Token creation failed.");
        hidl_cb({});
        return Void();
    }

    if (id == TOKEN_ID_NONE) {
        hidl_cb({});
        return Void();
    }

    mMap[id] = interface;

    hidl_cb(interface.token);
    return Void();
}

std::unordered_map<uint64_t,  TokenManager::TokenInterface>::const_iterator
        TokenManager::lookupToken(const hidl_vec<uint8_t> &token) {
    uint64_t tokenId = getTokenId(token);

    if (tokenId == TOKEN_ID_NONE) {
        return mMap.end();
    }

    auto it = mMap.find(tokenId);

    if (it == mMap.end()) {
        return mMap.end();
    }

    const TokenInterface &interface = it->second;

    if (!constantTimeCompare(token, interface.token)) {
        ALOGE("Fetch of token with invalid hash.");
        return mMap.end();
    }

    return it;
}

Return<bool> TokenManager::unregister(const hidl_vec<uint8_t> &token) {
    auto it = lookupToken(token);

    if (it == mMap.end()) {
        return false;
    }

    mMap.erase(it);
    return true;
}

Return<sp<IBase>> TokenManager::get(const hidl_vec<uint8_t> &token) {
    auto it = lookupToken(token);

    if (it == mMap.end()) {
        return nullptr;
    }

    return it->second.interface;
}


TokenManager::TokenInterface TokenManager::generateToken(const sp<IBase> &interface) {
    uint64_t id = ++mTokenIndex;

    std::array<uint8_t, EVP_MAX_MD_SIZE> hmac;
    uint32_t hmacSize;

    uint8_t *hmacOut = HMAC(EVP_sha256(),
                            mKey.data(), mKey.size(),
                            (uint8_t*) &id, sizeof(id),
                            hmac.data(), &hmacSize);

    if (hmacOut == nullptr ||
            hmacOut != hmac.data()) {
        ALOGE("Generating token failed, got %p.", hmacOut);
        return { nullptr, TOKEN_ID_NONE, {} };
    }

    // only care about the first HMAC_SIZE bytes of the HMAC
    const hidl_vec<uint8_t> &token = makeToken(id, hmac.data(), hmacSize);

    return { interface, id, token };
}

__attribute__((optnone))
bool TokenManager::constantTimeCompare(const hidl_vec<uint8_t> &t1, const hidl_vec<uint8_t> &t2) {
    if (t1.size() != t2.size()) {
        return false;
    }

    uint8_t x = 0;
    for (size_t i = 0; i < t1.size(); i++) {
        x |= t1[i] ^ t2[i];
    }

    return x == 0;
}

uint64_t TokenManager::getTokenId(const hidl_vec<uint8_t> &token) {
    uint64_t id = 0;

    if (token.size() < sizeof(id)) {
        return TOKEN_ID_NONE;
    }

    memcpy(&id, token.data(), sizeof(id));

    return id;
}

hidl_vec<uint8_t> TokenManager::makeToken(const uint64_t id, const uint8_t *hmac, uint64_t hmacSize) {
    hidl_vec<uint8_t> token;
    token.resize(sizeof(id) + hmacSize);

    memcpy(token.data(), &id, sizeof(id));
    memcpy(token.data() + sizeof(id), hmac, hmacSize);

    return token;
}


}  // namespace implementation
}  // namespace V1_0
}  // namespace token
}  // namespace hidl
}  // namespace android
