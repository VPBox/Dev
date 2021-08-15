/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef KEYSTORE_GRANT_STORE_H_
#define KEYSTORE_GRANT_STORE_H_

#include <mutex>
#include <set>
#include <shared_mutex>
#include <string>
#include <unordered_map>

#include <keystore/keystore_concurrency.h>

#include "blob.h"

namespace keystore {

class Grant;

using ReadLockedGrant =
    ProxyLock<MutexProxyLockHelper<const Grant, std::shared_mutex, std::shared_lock>>;

/**
 * Grant represents a mapping from an alias to a key file.
 * Normally, key file names are derived from the alias chosen by the client
 * and the clients UID, to generate a per client name space.
 * Grants allow assotiating a key file with a new name, thereby making
 * it visible in another client's - the grantee's - namespace.
 */
class Grant {
public:
  Grant(const KeyBlobEntry& entry, const uint64_t grant_no);
  KeyBlobEntry entry_;

  uint64_t grant_no_;  ///< numeric grant identifier - randomly assigned

  // NOLINTNEXTLINE(google-explicit-constructor)
  operator const uint64_t&() const { return grant_no_; }
};

/**
 * The GrantStore holds a set of sets of Grants. One set of Grants for each grantee.
 * The uid parameter to each of the GrantStore function determines the grantee's
 * name space. The methods put, get, and removeByAlias/ByFileName create, lookup, and
 * remove a Grant, respectively.
 * put also returns a new alias for the newly granted key which has to be returned
 * to the granter. The grantee, and only the grantee, can use the granted key
 * by this new alias.
 */
class GrantStore {
public:
    GrantStore() : grants_() {}
    std::string put(const uid_t uid, const LockedKeyBlobEntry& blobfile);
    ReadLockedGrant get(const uid_t uid, const std::string& alias) const;
    bool removeByFileAlias(const uid_t granteeUid, const LockedKeyBlobEntry& lockedEntry);
    void removeAllGrantsToKey(const uid_t granterUid, const std::string& alias);
    void removeAllGrantsToUid(const uid_t granteeUid);

    // GrantStore is neither copyable nor movable.
    GrantStore(const GrantStore&) = delete;
    GrantStore& operator=(const GrantStore&) = delete;
private:
    std::unordered_map<uid_t, std::set<Grant, std::less<>>> grants_;
    mutable std::shared_mutex mutex_;
};

}  // namespace keystore

#endif  // KEYSTORE_GRANT_STORE_H_
