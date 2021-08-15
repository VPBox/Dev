/*
 * Copyright (C) 2014 The Android Open Source Project
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

#define LOG_TAG "keystore"

#include "keymaster_enforcement.h"

#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <string.h>

#include <openssl/evp.h>

#include <hardware/hw_auth_token.h>
#include <log/log.h>

#include <list>

#include <keystore/keystore_hidl_support.h>

namespace keystore {

bool is_public_key_algorithm(const AuthorizationSet& auth_set) {
    auto algorithm = auth_set.GetTagValue(TAG_ALGORITHM);
    return algorithm.isOk() &&
           (algorithm.value() == Algorithm::RSA || algorithm.value() == Algorithm::EC);
}

static ErrorCode authorized_purpose(const KeyPurpose purpose, const AuthorizationSet& auth_set) {
    switch (purpose) {
    case KeyPurpose::VERIFY:
    case KeyPurpose::ENCRYPT:
    case KeyPurpose::SIGN:
    case KeyPurpose::DECRYPT:
        if (auth_set.Contains(TAG_PURPOSE, purpose)) return ErrorCode::OK;
        return ErrorCode::INCOMPATIBLE_PURPOSE;

    default:
        return ErrorCode::UNSUPPORTED_PURPOSE;
    }
}

inline bool is_origination_purpose(KeyPurpose purpose) {
    return purpose == KeyPurpose::ENCRYPT || purpose == KeyPurpose::SIGN;
}

inline bool is_usage_purpose(KeyPurpose purpose) {
    return purpose == KeyPurpose::DECRYPT || purpose == KeyPurpose::VERIFY;
}

KeymasterEnforcement::KeymasterEnforcement(uint32_t max_access_time_map_size,
                                           uint32_t max_access_count_map_size)
    : access_time_map_(max_access_time_map_size), access_count_map_(max_access_count_map_size) {}

KeymasterEnforcement::~KeymasterEnforcement() {
}

ErrorCode KeymasterEnforcement::AuthorizeOperation(const KeyPurpose purpose, const km_id_t keyid,
                                                   const AuthorizationSet& auth_set,
                                                   const AuthorizationSet& operation_params,
                                                   const HardwareAuthToken& auth_token,
                                                   uint64_t op_handle, bool is_begin_operation) {
    if (is_public_key_algorithm(auth_set)) {
        switch (purpose) {
        case KeyPurpose::ENCRYPT:
        case KeyPurpose::VERIFY:
            /* Public key operations are always authorized. */
            return ErrorCode::OK;

        case KeyPurpose::DECRYPT:
        case KeyPurpose::SIGN:
            break;

        case KeyPurpose::WRAP_KEY:
            return ErrorCode::INCOMPATIBLE_PURPOSE;
        };
    };

    if (is_begin_operation)
        return AuthorizeBegin(purpose, keyid, auth_set, operation_params, auth_token);
    else
        return AuthorizeUpdateOrFinish(auth_set, auth_token, op_handle);
}

// For update and finish the only thing to check is user authentication, and then only if it's not
// timeout-based.
ErrorCode KeymasterEnforcement::AuthorizeUpdateOrFinish(const AuthorizationSet& auth_set,
                                                        const HardwareAuthToken& auth_token,
                                                        uint64_t op_handle) {
    int auth_type_index = -1;
    for (size_t pos = 0; pos < auth_set.size(); ++pos) {
        switch (auth_set[pos].tag) {
        case Tag::NO_AUTH_REQUIRED:
        case Tag::AUTH_TIMEOUT:
            // If no auth is required or if auth is timeout-based, we have nothing to check.
            return ErrorCode::OK;

        case Tag::USER_AUTH_TYPE:
            auth_type_index = pos;
            break;

        default:
            break;
        }
    }

    // Note that at this point we should be able to assume that authentication is required, because
    // authentication is required if KM_TAG_NO_AUTH_REQUIRED is absent.  However, there are legacy
    // keys which have no authentication-related tags, so we assume that absence is equivalent to
    // presence of KM_TAG_NO_AUTH_REQUIRED.
    //
    // So, if we found KM_TAG_USER_AUTH_TYPE or if we find KM_TAG_USER_SECURE_ID then authentication
    // is required.  If we find neither, then we assume authentication is not required and return
    // success.
    bool authentication_required = (auth_type_index != -1);
    for (auto& param : auth_set) {
        auto user_secure_id = authorizationValue(TAG_USER_SECURE_ID, param);
        if (user_secure_id.isOk()) {
            authentication_required = true;
            int auth_timeout_index = -1;
            if (auth_token.mac.size() &&
                AuthTokenMatches(auth_set, auth_token, user_secure_id.value(), auth_type_index,
                                 auth_timeout_index, op_handle, false /* is_begin_operation */))
                return ErrorCode::OK;
        }
    }

    if (authentication_required) return ErrorCode::KEY_USER_NOT_AUTHENTICATED;

    return ErrorCode::OK;
}

ErrorCode KeymasterEnforcement::AuthorizeBegin(const KeyPurpose purpose, const km_id_t keyid,
                                               const AuthorizationSet& auth_set,
                                               const AuthorizationSet& operation_params,
                                               NullOr<const HardwareAuthToken&> auth_token) {
    // Find some entries that may be needed to handle KM_TAG_USER_SECURE_ID
    int auth_timeout_index = -1;
    int auth_type_index = -1;
    int no_auth_required_index = -1;
    for (size_t pos = 0; pos < auth_set.size(); ++pos) {
        switch (auth_set[pos].tag) {
        case Tag::AUTH_TIMEOUT:
            auth_timeout_index = pos;
            break;
        case Tag::USER_AUTH_TYPE:
            auth_type_index = pos;
            break;
        case Tag::NO_AUTH_REQUIRED:
            no_auth_required_index = pos;
            break;
        default:
            break;
        }
    }

    ErrorCode error = authorized_purpose(purpose, auth_set);
    if (error != ErrorCode::OK) return error;

    // If successful, and if key has a min time between ops, this will be set to the time limit
    uint32_t min_ops_timeout = UINT32_MAX;

    bool update_access_count = false;
    bool caller_nonce_authorized_by_key = false;
    bool authentication_required = false;
    bool auth_token_matched = false;
    bool unlocked_device_required = false;
    int32_t user_id = -1;

    for (auto& param : auth_set) {

        // KM_TAG_PADDING_OLD and KM_TAG_DIGEST_OLD aren't actually members of the enum, so we can't
        // switch on them.  There's nothing to validate for them, though, so just ignore them.
        if (int32_t(param.tag) == KM_TAG_PADDING_OLD || int32_t(param.tag) == KM_TAG_DIGEST_OLD)
            continue;

        switch (param.tag) {

        case Tag::ACTIVE_DATETIME: {
            auto date = authorizationValue(TAG_ACTIVE_DATETIME, param);
            if (date.isOk() && !activation_date_valid(date.value()))
                return ErrorCode::KEY_NOT_YET_VALID;
            break;
        }
        case Tag::ORIGINATION_EXPIRE_DATETIME: {
            auto date = authorizationValue(TAG_ORIGINATION_EXPIRE_DATETIME, param);
            if (is_origination_purpose(purpose) && date.isOk() &&
                expiration_date_passed(date.value()))
                return ErrorCode::KEY_EXPIRED;
            break;
        }
        case Tag::USAGE_EXPIRE_DATETIME: {
            auto date = authorizationValue(TAG_USAGE_EXPIRE_DATETIME, param);
            if (is_usage_purpose(purpose) && date.isOk() && expiration_date_passed(date.value()))
                return ErrorCode::KEY_EXPIRED;
            break;
        }
        case Tag::MIN_SECONDS_BETWEEN_OPS: {
            auto min_ops_timeout = authorizationValue(TAG_MIN_SECONDS_BETWEEN_OPS, param);
            if (min_ops_timeout.isOk() && !MinTimeBetweenOpsPassed(min_ops_timeout.value(), keyid))
                return ErrorCode::KEY_RATE_LIMIT_EXCEEDED;
            break;
        }
        case Tag::MAX_USES_PER_BOOT: {
            auto max_users = authorizationValue(TAG_MAX_USES_PER_BOOT, param);
            update_access_count = true;
            if (max_users.isOk() && !MaxUsesPerBootNotExceeded(keyid, max_users.value()))
                return ErrorCode::KEY_MAX_OPS_EXCEEDED;
            break;
        }
        case Tag::USER_SECURE_ID:
            if (no_auth_required_index != -1) {
                // Key has both KM_TAG_USER_SECURE_ID and KM_TAG_NO_AUTH_REQUIRED
                return ErrorCode::INVALID_KEY_BLOB;
            }

            if (auth_timeout_index != -1) {
                auto secure_id = authorizationValue(TAG_USER_SECURE_ID, param);
                authentication_required = true;
                if (secure_id.isOk() && auth_token.isOk() &&
                    AuthTokenMatches(auth_set, auth_token.value(), secure_id.value(),
                                     auth_type_index, auth_timeout_index, 0 /* op_handle */,
                                     true /* is_begin_operation */))
                    auth_token_matched = true;
            }
            break;

        case Tag::USER_ID:
            user_id = authorizationValue(TAG_USER_ID, param).value();
            break;

        case Tag::CALLER_NONCE:
            caller_nonce_authorized_by_key = true;
            break;

        case Tag::UNLOCKED_DEVICE_REQUIRED:
            unlocked_device_required = true;
            break;

        /* Tags should never be in key auths. */
        case Tag::INVALID:
        case Tag::ROOT_OF_TRUST:
        case Tag::APPLICATION_DATA:
        case Tag::ATTESTATION_CHALLENGE:
        case Tag::ATTESTATION_APPLICATION_ID:
        case Tag::ATTESTATION_ID_BRAND:
        case Tag::ATTESTATION_ID_DEVICE:
        case Tag::ATTESTATION_ID_PRODUCT:
        case Tag::ATTESTATION_ID_SERIAL:
        case Tag::ATTESTATION_ID_IMEI:
        case Tag::ATTESTATION_ID_MEID:
        case Tag::ATTESTATION_ID_MANUFACTURER:
        case Tag::ATTESTATION_ID_MODEL:
            return ErrorCode::INVALID_KEY_BLOB;

        /* Tags used for cryptographic parameters in keygen.  Nothing to enforce. */
        case Tag::PURPOSE:
        case Tag::ALGORITHM:
        case Tag::KEY_SIZE:
        case Tag::BLOCK_MODE:
        case Tag::DIGEST:
        case Tag::MAC_LENGTH:
        case Tag::PADDING:
        case Tag::NONCE:
        case Tag::MIN_MAC_LENGTH:
        case Tag::EC_CURVE:

        /* Tags not used for operations. */
        case Tag::BLOB_USAGE_REQUIREMENTS:

        /* Algorithm specific parameters not used for access control. */
        case Tag::RSA_PUBLIC_EXPONENT:

        /* Informational tags. */
        case Tag::CREATION_DATETIME:
        case Tag::ORIGIN:
        case Tag::ROLLBACK_RESISTANCE:

        /* Tags handled when KM_TAG_USER_SECURE_ID is handled */
        case Tag::NO_AUTH_REQUIRED:
        case Tag::USER_AUTH_TYPE:
        case Tag::AUTH_TIMEOUT:

        /* Tag to provide data to operations. */
        case Tag::ASSOCIATED_DATA:

        /* Tags that are implicitly verified by secure side */
        case Tag::APPLICATION_ID:
        case Tag::BOOT_PATCHLEVEL:
        case Tag::OS_PATCHLEVEL:
        case Tag::OS_VERSION:
        case Tag::TRUSTED_USER_PRESENCE_REQUIRED:
        case Tag::VENDOR_PATCHLEVEL:

        /* TODO(swillden): Handle these */
        case Tag::INCLUDE_UNIQUE_ID:
        case Tag::UNIQUE_ID:
        case Tag::RESET_SINCE_ID_ROTATION:
        case Tag::ALLOW_WHILE_ON_BODY:
        case Tag::HARDWARE_TYPE:
        case Tag::TRUSTED_CONFIRMATION_REQUIRED:
        case Tag::CONFIRMATION_TOKEN:
            break;

        case Tag::BOOTLOADER_ONLY:
            return ErrorCode::INVALID_KEY_BLOB;
        }
    }

    if (unlocked_device_required && is_device_locked(user_id)) {
        switch (purpose) {
        case KeyPurpose::ENCRYPT:
        case KeyPurpose::VERIFY:
            /* These are okay */
            break;
        case KeyPurpose::DECRYPT:
        case KeyPurpose::SIGN:
        case KeyPurpose::WRAP_KEY:
            return ErrorCode::DEVICE_LOCKED;
        };
    }

    if (authentication_required && !auth_token_matched) {
        ALOGE("Auth required but no matching auth token found");
        return ErrorCode::KEY_USER_NOT_AUTHENTICATED;
    }

    if (!caller_nonce_authorized_by_key && is_origination_purpose(purpose) &&
        operation_params.Contains(Tag::NONCE))
        return ErrorCode::CALLER_NONCE_PROHIBITED;

    if (min_ops_timeout != UINT32_MAX) {
        if (!access_time_map_.UpdateKeyAccessTime(keyid, get_current_time(), min_ops_timeout)) {
            ALOGE("Rate-limited keys table full.  Entries will time out.");
            return ErrorCode::TOO_MANY_OPERATIONS;
        }
    }

    if (update_access_count) {
        if (!access_count_map_.IncrementKeyAccessCount(keyid)) {
            ALOGE("Usage count-limited keys table full, until reboot.");
            return ErrorCode::TOO_MANY_OPERATIONS;
        }
    }

    return ErrorCode::OK;
}

class EvpMdCtx {
  public:
    EvpMdCtx() { EVP_MD_CTX_init(&ctx_); }
    ~EvpMdCtx() { EVP_MD_CTX_cleanup(&ctx_); }

    EVP_MD_CTX* get() { return &ctx_; }

  private:
    EVP_MD_CTX ctx_;
};

/* static */
std::optional<km_id_t> KeymasterEnforcement::CreateKeyId(const hidl_vec<uint8_t>& key_blob) {
    EvpMdCtx ctx;
    km_id_t keyid;

    uint8_t hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    if (EVP_DigestInit_ex(ctx.get(), EVP_sha256(), nullptr /* ENGINE */) &&
        EVP_DigestUpdate(ctx.get(), &key_blob[0], key_blob.size()) &&
        EVP_DigestFinal_ex(ctx.get(), hash, &hash_len)) {
        assert(hash_len >= sizeof(keyid));
        memcpy(&keyid, hash, sizeof(keyid));
        return keyid;
    }

    return {};
}

bool KeymasterEnforcement::MinTimeBetweenOpsPassed(uint32_t min_time_between, const km_id_t keyid) {
    uint32_t last_access_time;
    if (!access_time_map_.LastKeyAccessTime(keyid, &last_access_time)) return true;
    return min_time_between <= static_cast<int64_t>(get_current_time()) - last_access_time;
}

bool KeymasterEnforcement::MaxUsesPerBootNotExceeded(const km_id_t keyid, uint32_t max_uses) {
    uint32_t key_access_count;
    if (!access_count_map_.KeyAccessCount(keyid, &key_access_count)) return true;
    return key_access_count < max_uses;
}

template <typename IntType, uint32_t byteOrder> struct choose_hton;

template <typename IntType> struct choose_hton<IntType, __ORDER_LITTLE_ENDIAN__> {
    inline static IntType hton(const IntType& value) {
        IntType result = 0;
        const unsigned char* inbytes = reinterpret_cast<const unsigned char*>(&value);
        unsigned char* outbytes = reinterpret_cast<unsigned char*>(&result);
        for (int i = sizeof(IntType) - 1; i >= 0; --i) {
            *(outbytes++) = inbytes[i];
        }
        return result;
    }
};

template <typename IntType> struct choose_hton<IntType, __ORDER_BIG_ENDIAN__> {
    inline static IntType hton(const IntType& value) { return value; }
};

template <typename IntType> inline IntType hton(const IntType& value) {
    return choose_hton<IntType, __BYTE_ORDER__>::hton(value);
}

template <typename IntType> inline IntType ntoh(const IntType& value) {
    // same operation and hton
    return choose_hton<IntType, __BYTE_ORDER__>::hton(value);
}

bool KeymasterEnforcement::AuthTokenMatches(const AuthorizationSet& auth_set,
                                            const HardwareAuthToken& auth_token,
                                            const uint64_t user_secure_id,
                                            const int auth_type_index, const int auth_timeout_index,
                                            const uint64_t op_handle,
                                            bool is_begin_operation) const {
    assert(auth_type_index < static_cast<int>(auth_set.size()));
    assert(auth_timeout_index < static_cast<int>(auth_set.size()));

    if (!ValidateTokenSignature(auth_token)) {
        ALOGE("Auth token signature invalid");
        return false;
    }

    if (auth_timeout_index == -1 && op_handle && op_handle != auth_token.challenge) {
        ALOGE("Auth token has the challenge %" PRIu64 ", need %" PRIu64, auth_token.challenge,
              op_handle);
        return false;
    }

    if (user_secure_id != auth_token.userId && user_secure_id != auth_token.authenticatorId) {
        ALOGI("Auth token SIDs %" PRIu64 " and %" PRIu64 " do not match key SID %" PRIu64,
              auth_token.userId, auth_token.authenticatorId, user_secure_id);
        return false;
    }

    if (auth_type_index < 0 || auth_type_index > static_cast<int>(auth_set.size())) {
        ALOGE("Auth required but no auth type found");
        return false;
    }

    assert(auth_set[auth_type_index].tag == TAG_USER_AUTH_TYPE);
    auto key_auth_type_mask = authorizationValue(TAG_USER_AUTH_TYPE, auth_set[auth_type_index]);
    if (!key_auth_type_mask.isOk()) return false;

    if ((uint32_t(key_auth_type_mask.value()) & auth_token.authenticatorType) == 0) {
        ALOGE("Key requires match of auth type mask 0%uo, but token contained 0%uo",
              key_auth_type_mask.value(), auth_token.authenticatorType);
        return false;
    }

    if (auth_timeout_index != -1 && is_begin_operation) {
        assert(auth_set[auth_timeout_index].tag == TAG_AUTH_TIMEOUT);
        auto auth_token_timeout =
            authorizationValue(TAG_AUTH_TIMEOUT, auth_set[auth_timeout_index]);
        if (!auth_token_timeout.isOk()) return false;

        if (auth_token_timed_out(auth_token, auth_token_timeout.value())) {
            ALOGE("Auth token has timed out");
            return false;
        }
    }

    // Survived the whole gauntlet.  We have authentage!
    return true;
}

bool AccessTimeMap::LastKeyAccessTime(km_id_t keyid, uint32_t* last_access_time) const {
    std::lock_guard<std::mutex> lock(list_lock_);
    for (auto& entry : last_access_list_)
        if (entry.keyid == keyid) {
            *last_access_time = entry.access_time;
            return true;
        }
    return false;
}

bool AccessTimeMap::UpdateKeyAccessTime(km_id_t keyid, uint32_t current_time, uint32_t timeout) {
    std::lock_guard<std::mutex> lock(list_lock_);
    for (auto iter = last_access_list_.begin(); iter != last_access_list_.end();) {
        if (iter->keyid == keyid) {
            iter->access_time = current_time;
            return true;
        }

        // Expire entry if possible.
        assert(current_time >= iter->access_time);
        if (current_time - iter->access_time >= iter->timeout)
            iter = last_access_list_.erase(iter);
        else
            ++iter;
    }

    if (last_access_list_.size() >= max_size_) return false;

    AccessTime new_entry;
    new_entry.keyid = keyid;
    new_entry.access_time = current_time;
    new_entry.timeout = timeout;
    last_access_list_.push_front(new_entry);
    return true;
}

bool AccessCountMap::KeyAccessCount(km_id_t keyid, uint32_t* count) const {
    std::lock_guard<std::mutex> lock(list_lock_);
    for (auto& entry : access_count_list_)
        if (entry.keyid == keyid) {
            *count = entry.access_count;
            return true;
        }
    return false;
}

bool AccessCountMap::IncrementKeyAccessCount(km_id_t keyid) {
    std::lock_guard<std::mutex> lock(list_lock_);
    for (auto& entry : access_count_list_)
        if (entry.keyid == keyid) {
            // Note that the 'if' below will always be true because KM_TAG_MAX_USES_PER_BOOT is a
            // uint32_t, and as soon as entry.access_count reaches the specified maximum value
            // operation requests will be rejected and access_count won't be incremented any more.
            // And, besides, UINT64_MAX is huge.  But we ensure that it doesn't wrap anyway, out of
            // an abundance of caution.
            if (entry.access_count < UINT64_MAX) ++entry.access_count;
            return true;
        }

    if (access_count_list_.size() >= max_size_) return false;

    AccessCount new_entry;
    new_entry.keyid = keyid;
    new_entry.access_count = 1;
    access_count_list_.push_front(new_entry);
    return true;
}
}; /* namespace keystore */
