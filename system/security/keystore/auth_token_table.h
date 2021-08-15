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

#include <memory>
#include <mutex>
#include <vector>

#include <keystore/keymaster_types.h>

#ifndef KEYSTORE_AUTH_TOKEN_TABLE_H_
#define KEYSTORE_AUTH_TOKEN_TABLE_H_

namespace keystore {

using keymaster::HardwareAuthToken;

namespace test {
class AuthTokenTableTest;
}  // namespace test

time_t clock_gettime_raw();

/**
 * AuthTokenTable manages a set of received authorization tokens and can provide the appropriate
 * token for authorizing a key operation.
 *
 * To keep the table from growing without bound, superseded entries are removed when possible, and
 * least recently used entries are automatically pruned when when the table exceeds a size limit,
 * which is expected to be relatively small, since the implementation uses a linear search.
 */
class AuthTokenTable {
  public:
    explicit AuthTokenTable(size_t max_entries = 32, time_t (*clock_function)() = clock_gettime_raw)
        : max_entries_(max_entries), last_off_body_(clock_function()),
          clock_function_(clock_function) {}

    enum Error {
        OK,
        AUTH_NOT_REQUIRED = -1,
        AUTH_TOKEN_EXPIRED = -2,    // Found a matching token, but it's too old.
        AUTH_TOKEN_WRONG_SID = -3,  // Found a token with the right challenge, but wrong SID.  This
                                    // most likely indicates that the authenticator was updated
                                    // (e.g. new fingerprint enrolled).
        OP_HANDLE_REQUIRED = -4,    // The key requires auth per use but op_handle was zero.
        AUTH_TOKEN_NOT_FOUND = -5,
    };

    /**
     * Add an authorization token to the table.
     */
    void AddAuthenticationToken(HardwareAuthToken&& auth_token);

    /**
     * Find an authorization token that authorizes the operation specified by \p operation_handle on
     * a key with the characteristics specified in \p key_info.
     *
     * This method is O(n * m), where n is the number of KM_TAG_USER_SECURE_ID entries in key_info
     * and m is the number of entries in the table.  It could be made better, but n and m should
     * always be small.
     *
     * The table retains ownership of the returned object.
     */
    std::tuple<Error, HardwareAuthToken> FindAuthorization(const AuthorizationSet& key_info,
                                                           KeyPurpose purpose, uint64_t op_handle);

    /**
     * Mark operation completed.  This allows tokens associated with the specified operation to be
     * superseded by new tokens.
     */
    void MarkCompleted(const uint64_t op_handle);

    /**
     * Update the last_off_body_ timestamp so that tokens which remain authorized only so long as
     * the device stays on body can be revoked.
     */
    void onDeviceOffBody();

    void Clear();

    /**
     * This function shall only be used for testing.
     *
     * BEWARE: Since the auth token table can be accessed
     * concurrently, the size may be out dated as soon as it returns.
     */
    size_t size() const;

  private:
    friend class AuthTokenTableTest;

    class Entry {
      public:
        Entry(HardwareAuthToken&& token, time_t current_time);
        Entry(Entry&& entry) noexcept { *this = std::move(entry); }

        void operator=(Entry&& rhs) noexcept {
            token_ = std::move(rhs.token_);
            time_received_ = rhs.time_received_;
            last_use_ = rhs.last_use_;
            operation_completed_ = rhs.operation_completed_;
        }

        bool operator<(const Entry& rhs) const { return last_use_ < rhs.last_use_; }

        void UpdateLastUse(time_t time);

        bool Supersedes(const Entry& entry) const;
        bool SatisfiesAuth(const std::vector<uint64_t>& sids, HardwareAuthenticatorType auth_type);

        bool is_newer_than(const Entry* entry) const {
            if (!entry) return true;
            uint64_t ts = token_.timestamp;
            uint64_t other_ts = entry->token_.timestamp;
            // Normally comparing timestamp_host_order alone is sufficient, but here is an
            // additional hack to compare time_received value for some devices where their auth
            // tokens contain fixed timestamp (due to the a stuck secure RTC on them)
            return (ts > other_ts) ||
                   ((ts == other_ts) && (time_received_ > entry->time_received_));
        }

        void mark_completed() { operation_completed_ = true; }

        const HardwareAuthToken& token() const & { return token_; }
        time_t time_received() const { return time_received_; }
        bool completed() const { return operation_completed_; }

      private:
        bool SatisfiesAuth(uint64_t sid, HardwareAuthenticatorType auth_type) const {
            return (sid == token_.userId || sid == token_.authenticatorId) &&
                   (auth_type & token_.authenticatorType) != 0;
        }

        HardwareAuthToken token_;
        time_t time_received_;
        time_t last_use_;
        bool operation_completed_;
    };

    std::tuple<Error, HardwareAuthToken>
    FindAuthPerOpAuthorization(const std::vector<uint64_t>& sids,
                               HardwareAuthenticatorType auth_type, uint64_t op_handle);
    std::tuple<Error, HardwareAuthToken> FindTimedAuthorization(const std::vector<uint64_t>& sids,
                                                                HardwareAuthenticatorType auth_type,
                                                                const AuthorizationSet& key_info);
    void ExtractSids(const AuthorizationSet& key_info, std::vector<uint64_t>* sids);
    void RemoveEntriesSupersededBy(const Entry& entry);
    bool IsSupersededBySomeEntry(const Entry& entry);

    /**
     * Guards the entries_ vector against concurrent modification. All public facing methods
     * reading of modifying the vector must grab this mutex.
     */
    mutable std::mutex entries_mutex_;
    std::vector<Entry> entries_;
    size_t max_entries_;
    time_t last_off_body_;
    time_t (*clock_function_)();
};

}  // namespace keystore

#endif  // KEYSTORE_AUTH_TOKEN_TABLE_H_
