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

#ifndef KEYSTORE_USER_STATE_H_
#define KEYSTORE_USER_STATE_H_

#include <sys/types.h>

#include <openssl/aes.h>

#include <utils/String8.h>

#include <keystore/keystore.h>

#include "blob.h"
#include "keystore_utils.h"

#include <android-base/logging.h>
#include <condition_variable>
#include <keystore/keystore_concurrency.h>
#include <mutex>
#include <set>
#include <vector>

namespace keystore {

class UserState;

template <typename UserState> using LockedUserState = ProxyLock<UnlockProxyLockHelper<UserState>>;

class UserState {
  public:
    explicit UserState(uid_t userId);

    bool initialize();

    uid_t getUserId() const { return mUserId; }
    const std::string& getUserDirName() const { return mMasterKeyEntry.user_dir(); }

    std::string getMasterKeyFileName() const { return mMasterKeyEntry.getKeyBlobPath(); }

    void setState(State state);
    State getState() const { return mState; }

    int8_t getRetry() const { return mRetry; }

    void zeroizeMasterKeysInMemory();
    bool deleteMasterKey();

    ResponseCode initialize(const android::String8& pw);

    ResponseCode copyMasterKey(LockedUserState<UserState>* src);
    ResponseCode copyMasterKeyFile(LockedUserState<UserState>* src);
    ResponseCode writeMasterKey(const android::String8& pw);
    ResponseCode readMasterKey(const android::String8& pw);

    const std::vector<uint8_t>& getEncryptionKey() const { return mMasterKey; }

    bool reset();

    bool operator<(const UserState& rhs) const;
    bool operator<(uid_t userId) const;

  private:
    static constexpr int SHA1_DIGEST_SIZE_BYTES = 16;
    static constexpr int SHA256_DIGEST_SIZE_BYTES = 32;

    static constexpr int MASTER_KEY_SIZE_BYTES = kAes256KeySizeBytes;
    static constexpr int MASTER_KEY_SIZE_BITS = MASTER_KEY_SIZE_BYTES * 8;

    static constexpr int MAX_RETRY = 4;
    static constexpr size_t SALT_SIZE = 16;

    void generateKeyFromPassword(std::vector<uint8_t>& key, const android::String8& pw,
                                 uint8_t* salt);
    bool generateSalt();
    bool generateMasterKey();
    void setupMasterKeys();

    KeyBlobEntry mMasterKeyEntry;

    uid_t mUserId;
    State mState;
    int8_t mRetry;

    std::vector<uint8_t> mMasterKey;
    uint8_t mSalt[SALT_SIZE];
};

bool operator<(uid_t userId, const UserState& rhs);

class UserStateDB {
  public:
    LockedUserState<UserState> getUserState(uid_t userId);
    LockedUserState<UserState> getUserStateByUid(uid_t uid);
    LockedUserState<const UserState> getUserState(uid_t userId) const;
    LockedUserState<const UserState> getUserStateByUid(uid_t uid) const;

  private:
    mutable std::set<const UserState*> locked_state_;
    mutable std::mutex locked_state_mutex_;
    mutable std::condition_variable locked_state_mutex_cond_var_;

    template <typename UserState>
    LockedUserState<UserState> get(std::unique_lock<std::mutex> lock, UserState* entry) const {
        locked_state_mutex_cond_var_.wait(
            lock, [&] { return locked_state_.find(entry) == locked_state_.end(); });
        locked_state_.insert(entry);
        return {entry, [&](UserState* entry) {
                    std::unique_lock<std::mutex> lock(locked_state_mutex_);
                    locked_state_.erase(entry);
                    lock.unlock();
                    locked_state_mutex_cond_var_.notify_all();
                }};
    }

    std::map<uid_t, UserState> mMasterKeys;
};

}  //  namespace keystore

#endif  // KEYSTORE_USER_STATE_H_
