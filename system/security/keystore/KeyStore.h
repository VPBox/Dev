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

#ifndef KEYSTORE_KEYSTORE_H_
#define KEYSTORE_KEYSTORE_H_

#include <android/hardware/keymaster/3.0/IKeymasterDevice.h>
#include <keymasterV4_0/Keymaster.h>
#include <utils/Vector.h>

#include <keystore/keymaster_types.h>

#include "auth_token_table.h"
#include "blob.h"
#include "confirmation_manager.h"
#include "grant_store.h"
#include "keymaster_worker.h"
#include "keystore_keymaster_enforcement.h"
#include "operation.h"
#include "user_state.h"

#include <array>
#include <optional>
#include <tuple>

namespace keystore {

using ::android::sp;
using keymaster::support::Keymaster;

template <typename T, size_t count> class Devices : public std::array<T, count> {
  public:
    T& operator[](SecurityLevel secLevel) {
        static_assert(uint32_t(SecurityLevel::SOFTWARE) == 0 &&
                          uint32_t(SecurityLevel::TRUSTED_ENVIRONMENT) == 1 &&
                          uint32_t(SecurityLevel::STRONGBOX) == 2,
                      "Numeric values of security levels have changed");
        return std::array<T, count>::at(static_cast<uint32_t>(secLevel));
    }
    T operator[](SecurityLevel secLevel) const {
        if (static_cast<uint32_t>(secLevel) > static_cast<uint32_t>(SecurityLevel::STRONGBOX)) {
            LOG(ERROR) << "Invalid security level requested";
            return {};
        }
        return (*const_cast<Devices*>(this))[secLevel];
    }
};

}  // namespace keystore

namespace std {
template <typename T, size_t count> struct tuple_size<keystore::Devices<T, count>> {
  public:
    static constexpr size_t value = std::tuple_size<std::array<T, count>>::value;
};
}  // namespace std

namespace keystore {

using KeymasterWorkers = Devices<std::shared_ptr<KeymasterWorker>, 3>;
using KeymasterDevices = Devices<sp<Keymaster>, 3>;

class KeyStore : public ::android::IBinder::DeathRecipient {
  public:
    KeyStore(const KeymasterDevices& kmDevices,
             SecurityLevel minimalAllowedSecurityLevelForNewKeys);
    ~KeyStore();

    std::shared_ptr<KeymasterWorker> getDevice(SecurityLevel securityLevel) const {
        return mKmDevices[securityLevel];
    }

    std::shared_ptr<KeymasterWorker> getFallbackDevice() const {
        // we only return the fallback device if the creation of new fallback key blobs is
        // allowed. (also see getDevice below)
        if (mAllowNewFallback) {
            return mKmDevices[SecurityLevel::SOFTWARE];
        } else {
            return nullptr;
        }
    }

    std::shared_ptr<KeymasterWorker> getDevice(const Blob& blob) {
        return mKmDevices[blob.getSecurityLevel()];
    }

    ResponseCode initialize();

    State getState(uid_t userId) { return mUserStateDB.getUserState(userId)->getState(); }

    ResponseCode initializeUser(const android::String8& pw, uid_t userId);

    ResponseCode copyMasterKey(uid_t srcUser, uid_t dstUser);
    ResponseCode writeMasterKey(const android::String8& pw, uid_t userId);
    ResponseCode readMasterKey(const android::String8& pw, uid_t userId);

    LockedKeyBlobEntry getLockedBlobEntryIfNotExists(const std::string& alias, uid_t uid);
    std::optional<KeyBlobEntry> getBlobEntryIfExists(const std::string& alias, uid_t uid);
    LockedKeyBlobEntry getLockedBlobEntryIfExists(const std::string& alias, uid_t uid);
    /*
     * Delete entries owned by userId. If keepUnencryptedEntries is true
     * then only encrypted entries will be removed, otherwise all entries will
     * be removed.
     */
    void resetUser(uid_t userId, bool keepUnenryptedEntries);
    bool isEmpty(uid_t userId) const;

    void lock(uid_t userId);

    std::tuple<ResponseCode, Blob, Blob> get(const LockedKeyBlobEntry& blobfile);
    ResponseCode put(const LockedKeyBlobEntry& blobfile, Blob keyBlob, Blob characteristicsBlob);
    ResponseCode del(const LockedKeyBlobEntry& blobfile);

    std::string addGrant(const LockedKeyBlobEntry& blobfile, uid_t granteeUid);
    bool removeGrant(const LockedKeyBlobEntry& blobfile, const uid_t granteeUid);
    void removeAllGrantsToUid(const uid_t granteeUid);

    ResponseCode importKey(const uint8_t* key, size_t keyLen, const LockedKeyBlobEntry& blobfile,
                           uid_t userId, int32_t flags);

    bool isHardwareBacked(const android::String16& keyType) const;

    std::tuple<ResponseCode, Blob, Blob, LockedKeyBlobEntry>
    getKeyForName(const android::String8& keyName, const uid_t uid, const BlobType type);

    void binderDied(const ::android::wp<IBinder>& who) override;

    UserStateDB& getUserStateDB() { return mUserStateDB; }
    AuthTokenTable& getAuthTokenTable() { return mAuthTokenTable; }
    KeystoreKeymasterEnforcement& getEnforcementPolicy() { return mEnforcementPolicy; }
    ConfirmationManager& getConfirmationManager() { return *mConfirmationManager; }

    void addOperationDevice(sp<IBinder> token, std::shared_ptr<KeymasterWorker> dev) {
        std::lock_guard<std::mutex> lock(operationDeviceMapMutex_);
        operationDeviceMap_.emplace(std::move(token), std::move(dev));
    }
    std::shared_ptr<KeymasterWorker> getOperationDevice(const sp<IBinder>& token) {
        std::lock_guard<std::mutex> lock(operationDeviceMapMutex_);
        auto it = operationDeviceMap_.find(token);
        if (it != operationDeviceMap_.end()) {
            return it->second;
        }
        return {};
    }
    void removeOperationDevice(const sp<IBinder>& token) {
        std::lock_guard<std::mutex> lock(operationDeviceMapMutex_);
        operationDeviceMap_.erase(token);
    }

  private:
    static const char* kOldMasterKey;
    static const char* kMetaDataFile;
    static const android::String16 kRsaKeyType;
    static const android::String16 kEcKeyType;

    KeymasterWorkers mKmDevices;

    bool mAllowNewFallback;

    UserStateDB mUserStateDB;
    AuthTokenTable mAuthTokenTable;
    KeystoreKeymasterEnforcement mEnforcementPolicy;
    sp<ConfirmationManager> mConfirmationManager;

    ::keystore::GrantStore mGrants;

    typedef struct { uint32_t version; } keystore_metadata_t;

    keystore_metadata_t mMetaData;

    /**
     * Upgrade the key from the current version to whatever is newest.
     */
    bool upgradeBlob(Blob* blob, const uint8_t oldVersion);

    void readMetaData();
    void writeMetaData();

    bool upgradeKeystore();

    std::mutex operationDeviceMapMutex_;
    std::map<sp<IBinder>, std::shared_ptr<KeymasterWorker>> operationDeviceMap_;
};

}  // namespace keystore

#endif  // KEYSTORE_KEYSTORE_H_
