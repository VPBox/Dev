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

#include "grant_store.h"

#include "blob.h"
#include <algorithm>
#include <sstream>

namespace keystore {

static constexpr uint64_t kInvalidGrantNo = std::numeric_limits<uint64_t>::max();
static const char* kKeystoreGrantInfix = "_KEYSTOREGRANT_";
static constexpr size_t kKeystoreGrantInfixLength = 15;

Grant::Grant(const KeyBlobEntry& entry, const uint64_t grant_no)
    : entry_(entry), grant_no_(grant_no) {}

static std::pair<uint64_t, std::string> parseGrantAlias(const std::string& grantAlias) {
    auto pos = grantAlias.rfind(kKeystoreGrantInfix);
    if (pos == std::string::npos) return {kInvalidGrantNo, ""};
    std::stringstream s(grantAlias.substr(pos + kKeystoreGrantInfixLength));
    std::string wrapped_alias = grantAlias.substr(0, pos);
    uint64_t grant_no = kInvalidGrantNo;
    s >> grant_no;
    if (s.fail() || grant_no == kInvalidGrantNo) return {kInvalidGrantNo, ""};
    return {grant_no, wrapped_alias};
}

std::string GrantStore::put(const uid_t uid, const LockedKeyBlobEntry& lockedEntry) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    std::stringstream s;
    KeyBlobEntry blobEntry = *lockedEntry;
    s << blobEntry.alias() << kKeystoreGrantInfix;

    std::set<Grant, std::less<>>& uid_grant_list = grants_[uid];

    bool success = false;
    auto iterator =
        std::find_if(uid_grant_list.begin(), uid_grant_list.end(),
                     [&](const Grant& entry) { return success = entry.entry_ == blobEntry; });
    while (!success) {
        std::tie(iterator, success) = uid_grant_list.emplace(blobEntry, std::rand());
    }
    s << iterator->grant_no_;
    return s.str();
}

ReadLockedGrant GrantStore::get(const uid_t uid, const std::string& alias) const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    uint64_t grant_no;
    std::string wrappedAlias;
    std::tie(grant_no, wrappedAlias) = parseGrantAlias(alias);
    if (grant_no == kInvalidGrantNo) return {};
    auto uid_set_iter = grants_.find(uid);
    if (uid_set_iter == grants_.end()) return {};
    auto& uid_grant_list = uid_set_iter->second;
    auto grant = uid_grant_list.find(grant_no);
    if (grant == uid_grant_list.end()) return {};
    if (grant->entry_.alias() != wrappedAlias) return {};
    return {&(*grant), std::move(lock)};
}

bool GrantStore::removeByFileAlias(const uid_t granteeUid, const LockedKeyBlobEntry& lockedEntry) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    auto& uid_grant_list = grants_[granteeUid];
    for (auto i = uid_grant_list.begin(); i != uid_grant_list.end(); ++i) {
        if (i->entry_ == *lockedEntry) {
            uid_grant_list.erase(i);
            return true;
        }
    }
    return false;
}

void GrantStore::removeAllGrantsToKey(const uid_t granterUid, const std::string& alias) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    for (auto& uid_grant_list : grants_) {
        for (auto i = uid_grant_list.second.begin(); i != uid_grant_list.second.end(); ++i) {
            if (i->entry_.alias() == alias && i->entry_.uid() == granterUid) {
                uid_grant_list.second.erase(i);
                break;
            }
        }
    }
}

void GrantStore::removeAllGrantsToUid(const uid_t granteeUid) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    grants_.erase(granteeUid);
}

}  // namespace keystore
