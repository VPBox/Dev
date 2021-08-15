/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include "auth_token_table.h"

#include <assert.h>
#include <time.h>

#include <algorithm>

#include <log/log.h>

namespace keystore {

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

//
// Some trivial template wrappers around std algorithms, so they take containers not ranges.
//
template <typename Container, typename Predicate>
typename Container::iterator find_if(Container& container, Predicate pred) {
    return std::find_if(container.begin(), container.end(), pred);
}

template <typename Container, typename Predicate>
typename Container::iterator remove_if(Container& container, Predicate pred) {
    return std::remove_if(container.begin(), container.end(), pred);
}

template <typename Container> typename Container::iterator min_element(Container& container) {
    return std::min_element(container.begin(), container.end());
}

time_t clock_gettime_raw() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC_RAW, &time);
    return time.tv_sec;
}

void AuthTokenTable::AddAuthenticationToken(HardwareAuthToken&& auth_token) {
    Entry new_entry(std::move(auth_token), clock_function_());
    // STOPSHIP: debug only, to be removed
    ALOGD("AddAuthenticationToken: timestamp = %llu, time_received = %lld",
          static_cast<unsigned long long>(new_entry.token().timestamp),
          static_cast<long long>(new_entry.time_received()));

    std::lock_guard<std::mutex> lock(entries_mutex_);
    RemoveEntriesSupersededBy(new_entry);
    if (entries_.size() >= max_entries_) {
        ALOGW("Auth token table filled up; replacing oldest entry");
        *min_element(entries_) = std::move(new_entry);
    } else {
        entries_.push_back(std::move(new_entry));
    }
}

inline bool is_secret_key_operation(Algorithm algorithm, KeyPurpose purpose) {
    if ((algorithm != Algorithm::RSA && algorithm != Algorithm::EC)) return true;
    if (purpose == KeyPurpose::SIGN || purpose == KeyPurpose::DECRYPT) return true;
    return false;
}

inline bool KeyRequiresAuthentication(const AuthorizationSet& key_info, KeyPurpose purpose) {
    auto algorithm = defaultOr(key_info.GetTagValue(TAG_ALGORITHM), Algorithm::AES);
    return is_secret_key_operation(algorithm, purpose) &&
           key_info.find(Tag::NO_AUTH_REQUIRED) == -1;
}

inline bool KeyRequiresAuthPerOperation(const AuthorizationSet& key_info, KeyPurpose purpose) {
    auto algorithm = defaultOr(key_info.GetTagValue(TAG_ALGORITHM), Algorithm::AES);
    return is_secret_key_operation(algorithm, purpose) && key_info.find(Tag::AUTH_TIMEOUT) == -1;
}

std::tuple<AuthTokenTable::Error, HardwareAuthToken>
AuthTokenTable::FindAuthorization(const AuthorizationSet& key_info, KeyPurpose purpose,
                                  uint64_t op_handle) {

    std::lock_guard<std::mutex> lock(entries_mutex_);

    if (!KeyRequiresAuthentication(key_info, purpose)) return {AUTH_NOT_REQUIRED, {}};

    auto auth_type =
        defaultOr(key_info.GetTagValue(TAG_USER_AUTH_TYPE), HardwareAuthenticatorType::NONE);

    std::vector<uint64_t> key_sids;
    ExtractSids(key_info, &key_sids);

    if (KeyRequiresAuthPerOperation(key_info, purpose))
        return FindAuthPerOpAuthorization(key_sids, auth_type, op_handle);
    else
        return FindTimedAuthorization(key_sids, auth_type, key_info);
}

std::tuple<AuthTokenTable::Error, HardwareAuthToken> AuthTokenTable::FindAuthPerOpAuthorization(
    const std::vector<uint64_t>& sids, HardwareAuthenticatorType auth_type, uint64_t op_handle) {
    if (op_handle == 0) return {OP_HANDLE_REQUIRED, {}};

    auto matching_op = find_if(
        entries_, [&](Entry& e) { return e.token().challenge == op_handle && !e.completed(); });

    if (matching_op == entries_.end()) return {AUTH_TOKEN_NOT_FOUND, {}};

    if (!matching_op->SatisfiesAuth(sids, auth_type)) return {AUTH_TOKEN_WRONG_SID, {}};

    return {OK, matching_op->token()};
}

std::tuple<AuthTokenTable::Error, HardwareAuthToken>
AuthTokenTable::FindTimedAuthorization(const std::vector<uint64_t>& sids,
                                       HardwareAuthenticatorType auth_type,
                                       const AuthorizationSet& key_info) {
    Entry* newest_match = nullptr;
    for (auto& entry : entries_)
        if (entry.SatisfiesAuth(sids, auth_type) && entry.is_newer_than(newest_match))
            newest_match = &entry;

    if (!newest_match) return {AUTH_TOKEN_NOT_FOUND, {}};

    auto timeout = defaultOr(key_info.GetTagValue(TAG_AUTH_TIMEOUT), 0);

    time_t now = clock_function_();
    if (static_cast<int64_t>(newest_match->time_received()) + timeout < static_cast<int64_t>(now))
        return {AUTH_TOKEN_EXPIRED, {}};

    if (key_info.GetTagValue(TAG_ALLOW_WHILE_ON_BODY).isOk()) {
        if (static_cast<int64_t>(newest_match->time_received()) <
            static_cast<int64_t>(last_off_body_)) {
            return {AUTH_TOKEN_EXPIRED, {}};
        }
    }

    newest_match->UpdateLastUse(now);
    return {OK, newest_match->token()};
}

void AuthTokenTable::ExtractSids(const AuthorizationSet& key_info, std::vector<uint64_t>* sids) {
    assert(sids);
    for (auto& param : key_info)
        if (param.tag == Tag::USER_SECURE_ID)
            sids->push_back(authorizationValue(TAG_USER_SECURE_ID, param).value());
}

void AuthTokenTable::RemoveEntriesSupersededBy(const Entry& entry) {
    entries_.erase(remove_if(entries_, [&](Entry& e) { return entry.Supersedes(e); }),
                   entries_.end());
}

void AuthTokenTable::onDeviceOffBody() {
    last_off_body_ = clock_function_();
}

void AuthTokenTable::Clear() {
    std::lock_guard<std::mutex> lock(entries_mutex_);

    entries_.clear();
}

size_t AuthTokenTable::size() const {
    std::lock_guard<std::mutex> lock(entries_mutex_);
    return entries_.size();
}

bool AuthTokenTable::IsSupersededBySomeEntry(const Entry& entry) {
    return std::any_of(entries_.begin(), entries_.end(),
                       [&](Entry& e) { return e.Supersedes(entry); });
}

void AuthTokenTable::MarkCompleted(const uint64_t op_handle) {
    std::lock_guard<std::mutex> lock(entries_mutex_);

    auto found = find_if(entries_, [&](Entry& e) { return e.token().challenge == op_handle; });
    if (found == entries_.end()) return;

    assert(!IsSupersededBySomeEntry(*found));
    found->mark_completed();

    if (IsSupersededBySomeEntry(*found)) entries_.erase(found);
}

AuthTokenTable::Entry::Entry(HardwareAuthToken&& token, time_t current_time)
    : token_(std::move(token)), time_received_(current_time), last_use_(current_time),
      operation_completed_(token_.challenge == 0) {}

bool AuthTokenTable::Entry::SatisfiesAuth(const std::vector<uint64_t>& sids,
                                          HardwareAuthenticatorType auth_type) {
    for (auto sid : sids) {
        if (SatisfiesAuth(sid, auth_type)) return true;
    }
    return false;
}

void AuthTokenTable::Entry::UpdateLastUse(time_t time) {
    this->last_use_ = time;
}

bool AuthTokenTable::Entry::Supersedes(const Entry& entry) const {
    if (!entry.completed()) return false;

    return (token_.userId == entry.token_.userId &&
            token_.authenticatorType == entry.token_.authenticatorType &&
            token_.authenticatorId == entry.token_.authenticatorId && is_newer_than(&entry));
}

}  // namespace keystore
