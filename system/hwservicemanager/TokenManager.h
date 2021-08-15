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

#ifndef ANDROID_HIDL_TOKEN_V1_0_TOKENMANAGER_H
#define ANDROID_HIDL_TOKEN_V1_0_TOKENMANAGER_H

#include <android/hidl/token/1.0/ITokenManager.h>
#include <chrono>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <unordered_map>
#include <array>

namespace android {
namespace hidl {
namespace token {
namespace V1_0 {
namespace implementation {

using ::android::hidl::base::V1_0::IBase;
using ::android::hidl::token::V1_0::ITokenManager;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct TokenManager : public ITokenManager {
    TokenManager();

    // Methods from ::android::hidl::token::V1_0::ITokenManager follow.
    Return<void> createToken(const sp<IBase>& store, createToken_cb hidl_cb) override;
    Return<bool> unregister(const hidl_vec<uint8_t> &token) override;
    Return<sp<IBase>> get(const hidl_vec<uint8_t> &token) override;

private:
    static constexpr uint64_t KEY_SIZE = 16;

    static constexpr uint64_t TOKEN_ID_NONE = 0;

    static bool constantTimeCompare(const hidl_vec<uint8_t> &t1, const hidl_vec<uint8_t> &t2);

    static hidl_vec<uint8_t> makeToken(const uint64_t id, const uint8_t *hmac, uint64_t hmacSize);
    static uint64_t getTokenId(const hidl_vec<uint8_t> &token);

    std::array<uint8_t, KEY_SIZE> mKey;

    struct TokenInterface {
        sp<IBase> interface;
        uint64_t id;
        hidl_vec<uint8_t> token; // First eight bytes are tokenId. Remaining bytes are hmac.
    };

    TokenInterface generateToken(const sp<IBase> &interface);

    // verifies token, returns iterator into mMap
    std::unordered_map<uint64_t, TokenInterface>::const_iterator
            lookupToken(const hidl_vec<uint8_t> &token);

    std::unordered_map<uint64_t, TokenInterface> mMap; // map getTokenId(i.token) -> i
    uint64_t mTokenIndex = TOKEN_ID_NONE; // last token index
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace token
}  // namespace hidl
}  // namespace android

#endif  // ANDROID_HIDL_TOKEN_V1_0_TOKENMANAGER_H
