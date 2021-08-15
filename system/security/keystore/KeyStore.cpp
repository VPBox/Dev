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

#define LOG_TAG "keystore"

#include "KeyStore.h"

#include <dirent.h>
#include <fcntl.h>

#include <openssl/bio.h>

#include <utils/String16.h>
#include <utils/String8.h>

#include <android-base/scopeguard.h>
#include <android/hardware/keymaster/3.0/IKeymasterDevice.h>
#include <android/security/keystore/IKeystoreService.h>
#include <log/log_event_list.h>

#include <private/android_logger.h>

#include "keystore_utils.h"
#include "permissions.h"
#include <keystore/keystore_hidl_support.h>

#include "keymaster_worker.h"

namespace keystore {

const char* KeyStore::kOldMasterKey = ".masterkey";
const char* KeyStore::kMetaDataFile = ".metadata";

const android::String16 KeyStore::kRsaKeyType("RSA");
const android::String16 KeyStore::kEcKeyType("EC");

using android::String8;

KeyStore::KeyStore(const KeymasterDevices& kmDevices,
                   SecurityLevel minimalAllowedSecurityLevelForNewKeys)
    : mAllowNewFallback(minimalAllowedSecurityLevelForNewKeys == SecurityLevel::SOFTWARE),
      mConfirmationManager(new ConfirmationManager(this)) {
    memset(&mMetaData, '\0', sizeof(mMetaData));

    static_assert(std::tuple_size<std::decay_t<decltype(kmDevices)>>::value ==
                      std::tuple_size<decltype(mKmDevices)>::value,
                  "KmasterDevices and KeymasterWorkers must have the same size");
    for (size_t i = 0; i < kmDevices.size(); ++i) {
        if (kmDevices[SecurityLevel(i)]) {
            mKmDevices[SecurityLevel(i)] =
                std::make_shared<KeymasterWorker>(kmDevices[SecurityLevel(i)], this);
        }
    }
}

KeyStore::~KeyStore() {
}

ResponseCode KeyStore::initialize() {
    readMetaData();
    if (upgradeKeystore()) {
        writeMetaData();
    }

    return ResponseCode::NO_ERROR;
}

ResponseCode KeyStore::initializeUser(const android::String8& pw, uid_t userId) {
    auto userState = mUserStateDB.getUserState(userId);
    return userState->initialize(pw);
}

ResponseCode KeyStore::copyMasterKey(uid_t srcUser, uid_t dstUser) {
    auto userState = mUserStateDB.getUserState(dstUser);
    auto initState = mUserStateDB.getUserState(srcUser);
    return userState->copyMasterKey(&initState);
}

ResponseCode KeyStore::writeMasterKey(const android::String8& pw, uid_t userId) {
    auto userState = mUserStateDB.getUserState(userId);
    return userState->writeMasterKey(pw);
}

ResponseCode KeyStore::readMasterKey(const android::String8& pw, uid_t userId) {
    auto userState = mUserStateDB.getUserState(userId);
    return userState->readMasterKey(pw);
}

LockedKeyBlobEntry KeyStore::getLockedBlobEntryIfNotExists(const std::string& alias, uid_t uid) {
    KeyBlobEntry kbe(alias, mUserStateDB.getUserStateByUid(uid)->getUserDirName(), uid);
    auto result = LockedKeyBlobEntry::get(std::move(kbe));
    if (result->hasKeyBlob()) return {};
    return result;
}

std::optional<KeyBlobEntry> KeyStore::getBlobEntryIfExists(const std::string& alias, uid_t uid) {
    KeyBlobEntry kbe(alias, mUserStateDB.getUserStateByUid(uid)->getUserDirName(), uid);
    if (kbe.hasKeyBlob()) return kbe;

    // If this is one of the legacy UID->UID mappings, use it.
    uid_t euid = get_keystore_euid(uid);
    if (euid != uid) {
        kbe = KeyBlobEntry(alias, mUserStateDB.getUserStateByUid(euid)->getUserDirName(), euid);
        if (kbe.hasKeyBlob()) return kbe;
    }

    // They might be using a granted key.
    auto grant = mGrants.get(uid, alias);
    if (grant) {
        kbe = grant->entry_;
        if (kbe.hasKeyBlob()) return kbe;
    }
    return {};
}
LockedKeyBlobEntry KeyStore::getLockedBlobEntryIfExists(const std::string& alias, uid_t uid) {
    auto blobentry = getBlobEntryIfExists(alias, uid);
    if (!blobentry) return {};
    LockedKeyBlobEntry lockedentry = LockedKeyBlobEntry::get(std::move(*blobentry));
    if (!lockedentry || !lockedentry->hasKeyBlob()) return {};
    return lockedentry;
}

void KeyStore::resetUser(uid_t userId, bool keepUnenryptedEntries) {
    android::String8 prefix("");
    android::Vector<android::String16> aliases;

    auto userState = mUserStateDB.getUserState(userId);
    std::string userDirName = userState->getUserDirName();
    auto encryptionKey = userState->getEncryptionKey();
    auto state = userState->getState();
    // userState is a proxy that holds a lock which may be required by a worker.
    // LockedKeyBlobEntry::list has a fence that waits until all workers have finished which may
    // not happen if a user state lock is held. The following line relinquishes the lock.
    userState = {};

    ResponseCode rc;
    std::list<LockedKeyBlobEntry> matches;

    // must not be called by a keymaster worker. List waits for workers to relinquish all access
    // to blob entries
    std::tie(rc, matches) = LockedKeyBlobEntry::list(userDirName);
    if (rc != ResponseCode::NO_ERROR) {
        return;
    }

    for (LockedKeyBlobEntry& lockedEntry : matches) {
        bool shouldDelete = true;

        if (keepUnenryptedEntries) {
            Blob blob;
            Blob charBlob;
            ResponseCode rc;

            std::tie(rc, blob, charBlob) = lockedEntry.readBlobs(encryptionKey, state);

            switch (rc) {
            case ResponseCode::SYSTEM_ERROR:
            case ResponseCode::VALUE_CORRUPTED:
                // If we can't read blobs, delete them.
                shouldDelete = true;
                break;

            case ResponseCode::NO_ERROR:
            case ResponseCode::LOCKED:
                // Delete encrypted blobs but keep unencrypted blobs and super-encrypted blobs.  We
                // need to keep super-encrypted blobs so we can report that the user is
                // unauthenticated if a caller tries to use them, rather than reporting that they
                // don't exist.
                shouldDelete = blob.isEncrypted();
                break;

            default:
                ALOGE("Got unexpected return code %d from readBlobs", rc);
                // This shouldn't happen.  To be on the safe side, delete it.
                shouldDelete = true;
                break;
            }
        }
        if (shouldDelete) {
            del(lockedEntry);
        }
    }

    userState = mUserStateDB.getUserState(userId);
    if (!userState->deleteMasterKey()) {
        ALOGE("Failed to delete user %d's master key", userId);
    }
    if (!keepUnenryptedEntries) {
        if (!userState->reset()) {
            ALOGE("Failed to remove user %d's directory", userId);
        }
    }
}

bool KeyStore::isEmpty(uid_t userId) const {
    std::string userDirName;
    {
        // userState holds a lock which must be relinquished before list is called. This scope
        // prevents deadlocks.
        auto userState = mUserStateDB.getUserState(userId);
        if (!userState) {
            return true;
        }
        userDirName = userState->getUserDirName();
    }

    ResponseCode rc;
    std::list<LockedKeyBlobEntry> matches;

    // must not be called by a keymaster worker. List waits for workers to relinquish all access
    // to blob entries
    std::tie(rc, matches) = LockedKeyBlobEntry::list(userDirName);

    return rc == ResponseCode::SYSTEM_ERROR || matches.size() == 0;
}

void KeyStore::lock(uid_t userId) {
    auto userState = mUserStateDB.getUserState(userId);
    userState->zeroizeMasterKeysInMemory();
    userState->setState(STATE_LOCKED);
}

static void maybeLogKeyIntegrityViolation(const LockedKeyBlobEntry& lockedEntry,
                                          const BlobType type) {
    if (!__android_log_security() || (type != TYPE_KEY_PAIR && type != TYPE_KEYMASTER_10)) return;
    log_key_integrity_violation(lockedEntry->alias().c_str(), lockedEntry->uid());
}

std::tuple<ResponseCode, Blob, Blob> KeyStore::get(const LockedKeyBlobEntry& blobfile) {
    std::tuple<ResponseCode, Blob, Blob> result;

    uid_t userId = get_user_id(blobfile->uid());
    Blob& keyBlob = std::get<1>(result);
    ResponseCode& rc = std::get<0>(result);

    auto userState = mUserStateDB.getUserState(userId);
    BlobType type = BlobType::TYPE_ANY;
    auto logOnScopeExit = android::base::make_scope_guard([&] {
        if (rc == ResponseCode::VALUE_CORRUPTED) {
            maybeLogKeyIntegrityViolation(blobfile, type);
        }
    });

    result = blobfile.readBlobs(userState->getEncryptionKey(), userState->getState());
    if (rc != ResponseCode::NO_ERROR) {
        return result;
    }

    // update the type for logging (see scope_guard above)
    type = keyBlob.getType();

    const uint8_t version = keyBlob.getVersion();
    if (version < CURRENT_BLOB_VERSION) {
        /* If we upgrade the key, we need to write it to disk again. Then
         * it must be read it again since the blob is encrypted each time
         * it's written.
         */
        if (upgradeBlob(&keyBlob, version)) {
            if ((rc = this->put(blobfile, keyBlob, {})) != ResponseCode::NO_ERROR ||
                (result = blobfile.readBlobs(userState->getEncryptionKey(), userState->getState()),
                 rc) != ResponseCode::NO_ERROR) {
                return result;
            }
        }
    }

    return result;
}

ResponseCode KeyStore::put(const LockedKeyBlobEntry& blobfile, Blob keyBlob,
                           Blob characteristicsBlob) {
    auto userState = mUserStateDB.getUserStateByUid(blobfile->uid());
    return blobfile.writeBlobs(std::move(keyBlob), std::move(characteristicsBlob),
                               userState->getEncryptionKey(), userState->getState());
}

ResponseCode KeyStore::del(const LockedKeyBlobEntry& blobfile) {
    Blob keyBlob;
    Blob charactaristicsBlob;
    ResponseCode rc;
    uid_t uid = blobfile->uid();
    std::string alias = blobfile->alias();

    std::tie(rc, keyBlob, charactaristicsBlob) = get(blobfile);

    // after getting the blob from the file system we scrub the filesystem.
    mGrants.removeAllGrantsToKey(uid, alias);
    auto result = blobfile.deleteBlobs();

    if (rc != ResponseCode::NO_ERROR) {
        LOG(ERROR) << "get keyblob failed " << int(rc);
        return rc;
    }

    // if we got the blob successfully, we try and delete it from the keymaster device
    auto dev = getDevice(keyBlob);

    if (keyBlob.getType() == ::TYPE_KEYMASTER_10) {
        dev->deleteKey(blob2hidlVec(keyBlob), [dev, alias, uid](Return<ErrorCode> rc) {
            auto ret = KS_HANDLE_HIDL_ERROR(dev, rc);
            // A device doesn't have to implement delete_key.
            bool success = ret == ErrorCode::OK || ret == ErrorCode::UNIMPLEMENTED;
            if (__android_log_security()) {
                android_log_event_list(SEC_TAG_KEY_DESTROYED)
                    << int32_t(success) << alias << int32_t(uid) << LOG_ID_SECURITY;
            }
            if (!success) {
                LOG(ERROR) << "Keymaster delete for key " << alias << " of uid " << uid
                           << " failed";
            }
        });
    }

    return result;
}

std::string KeyStore::addGrant(const LockedKeyBlobEntry& blobfile, uid_t granteeUid) {
    return mGrants.put(granteeUid, blobfile);
}

bool KeyStore::removeGrant(const LockedKeyBlobEntry& blobfile, const uid_t granteeUid) {
    return mGrants.removeByFileAlias(granteeUid, blobfile);
}
void KeyStore::removeAllGrantsToUid(const uid_t granteeUid) {
    mGrants.removeAllGrantsToUid(granteeUid);
}

bool KeyStore::isHardwareBacked(const android::String16& keyType) const {
    // if strongbox device is present TEE must also be present and of sufficiently high version
    // to support all keys in hardware
    if (getDevice(SecurityLevel::STRONGBOX)) return true;
    if (!getDevice(SecurityLevel::TRUSTED_ENVIRONMENT)) {
        ALOGW("can't get keymaster device");
        return false;
    }

    auto version = getDevice(SecurityLevel::TRUSTED_ENVIRONMENT)->halVersion();
    if (keyType == kRsaKeyType) return true;  // All versions support RSA
    return keyType == kEcKeyType && version.supportsEc;
}

std::tuple<ResponseCode, Blob, Blob, LockedKeyBlobEntry>
KeyStore::getKeyForName(const android::String8& keyName, const uid_t uid, const BlobType type) {
    std::tuple<ResponseCode, Blob, Blob, LockedKeyBlobEntry> result;
    auto& [rc, keyBlob, charBlob, lockedEntry] = result;

    lockedEntry = getLockedBlobEntryIfExists(keyName.string(), uid);

    if (!lockedEntry) return rc = ResponseCode::KEY_NOT_FOUND, std::move(result);

    std::tie(rc, keyBlob, charBlob) = get(lockedEntry);

    if (rc == ResponseCode::NO_ERROR) {
        if (keyBlob.getType() != type) return rc = ResponseCode::KEY_NOT_FOUND, std::move(result);
    }
    return result;
}

bool KeyStore::upgradeBlob(Blob* blob, const uint8_t oldVersion) {
    bool updated = false;
    uint8_t version = oldVersion;

    if (!blob || !(*blob)) return false;

    /* From V0 -> V1: All old types were unknown */
    if (version == 0) {
        ALOGE("Failed to upgrade key blob. Ancient blob version 0 is no longer supported");

        return false;
    }

    /* From V1 -> V2: All old keys were encrypted */
    if (version == 1) {
        ALOGV("upgrading to version 2");

        blob->setEncrypted(true);
        version = 2;
        updated = true;
    }

    /*
     * If we've updated, set the key blob to the right version
     * and write it.
     */
    if (updated) {
        blob->setVersion(version);
    }

    return updated;
}

struct BIO_Delete {
    void operator()(BIO* p) const { BIO_free(p); }
};
typedef std::unique_ptr<BIO, BIO_Delete> Unique_BIO;

void KeyStore::readMetaData() {
    int in = TEMP_FAILURE_RETRY(open(kMetaDataFile, O_RDONLY));
    if (in < 0) {
        return;
    }
    size_t fileLength = readFully(in, (uint8_t*)&mMetaData, sizeof(mMetaData));
    if (fileLength != sizeof(mMetaData)) {
        ALOGI("Metadata file is %zd bytes (%zd experted); upgrade?", fileLength, sizeof(mMetaData));
    }
    close(in);
}

void KeyStore::writeMetaData() {
    const char* tmpFileName = ".metadata.tmp";
    int out =
        TEMP_FAILURE_RETRY(open(tmpFileName, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR));
    if (out < 0) {
        ALOGE("couldn't write metadata file: %s", strerror(errno));
        return;
    }
    size_t fileLength = writeFully(out, (uint8_t*)&mMetaData, sizeof(mMetaData));
    if (fileLength != sizeof(mMetaData)) {
        ALOGI("Could only write %zd bytes to metadata file (%zd expected)", fileLength,
              sizeof(mMetaData));
    }
    close(out);
    rename(tmpFileName, kMetaDataFile);
}

bool KeyStore::upgradeKeystore() {
    bool upgraded = false;

    if (mMetaData.version == 0) {
        auto userState = getUserStateDB().getUserStateByUid(0);

        // Initialize first so the directory is made.
        userState->initialize();

        // Migrate the old .masterkey file to user 0.
        if (access(kOldMasterKey, R_OK) == 0) {
            if (rename(kOldMasterKey, userState->getMasterKeyFileName().c_str()) < 0) {
                ALOGE("couldn't migrate old masterkey: %s", strerror(errno));
                return false;
            }
        }

        // Initialize again in case we had a key.
        userState->initialize();

        // Try to migrate existing keys.
        DIR* dir = opendir(".");
        if (!dir) {
            // Give up now; maybe we can upgrade later.
            ALOGE("couldn't open keystore's directory; something is wrong");
            return false;
        }

        struct dirent* file;
        while ((file = readdir(dir)) != nullptr) {
            // We only care about files.
            if (file->d_type != DT_REG) {
                continue;
            }

            // Skip anything that starts with a "."
            if (file->d_name[0] == '.') {
                continue;
            }

            // Find the current file's user.
            char* end;
            unsigned long thisUid = strtoul(file->d_name, &end, 10);
            if (end[0] != '_' || end[1] == 0) {
                continue;
            }
            auto otherUser = getUserStateDB().getUserStateByUid(thisUid);
            if (otherUser->getUserId() != 0) {
                unlinkat(dirfd(dir), file->d_name, 0);
            }

            // Rename the file into user directory.
            DIR* otherdir = opendir(otherUser->getUserDirName().c_str());
            if (otherdir == nullptr) {
                ALOGW("couldn't open user directory for rename");
                continue;
            }
            if (renameat(dirfd(dir), file->d_name, dirfd(otherdir), file->d_name) < 0) {
                ALOGW("couldn't rename blob: %s: %s", file->d_name, strerror(errno));
            }
            closedir(otherdir);
        }
        closedir(dir);

        mMetaData.version = 1;
        upgraded = true;
    }

    return upgraded;
}

void KeyStore::binderDied(const ::android::wp<IBinder>& who) {
    for (unsigned i = 0; i < mKmDevices.size(); ++i) {
        if (mKmDevices[SecurityLevel(i)]) mKmDevices[SecurityLevel(i)]->binderDied(who);
    }
    getConfirmationManager().binderDied(who);
}

}  // namespace keystore
