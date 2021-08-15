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

#include "user_state.h"

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <openssl/digest.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include <log/log.h>

#include "blob.h"
#include "keystore_utils.h"

namespace keystore {

UserState::UserState(uid_t userId)
    : mMasterKeyEntry(".masterkey", "user_" + std::to_string(userId), userId, /* masterkey */ true),
      mUserId(userId), mState(STATE_UNINITIALIZED), mRetry(MAX_RETRY) {}

bool UserState::operator<(const UserState& rhs) const {
    return getUserId() < rhs.getUserId();
}

bool UserState::operator<(uid_t userId) const {
    return getUserId() < userId;
}

bool operator<(uid_t userId, const UserState& rhs) {
    return userId < rhs.getUserId();
}

bool UserState::initialize() {
    if ((mkdir(mMasterKeyEntry.user_dir().c_str(), S_IRUSR | S_IWUSR | S_IXUSR) < 0) &&
        (errno != EEXIST)) {
        ALOGE("Could not create directory '%s'", mMasterKeyEntry.user_dir().c_str());
        return false;
    }

    if (mMasterKeyEntry.hasKeyBlob()) {
        setState(STATE_LOCKED);
    } else {
        setState(STATE_UNINITIALIZED);
    }

    return true;
}

void UserState::setState(State state) {
    mState = state;
    if (mState == STATE_NO_ERROR || mState == STATE_UNINITIALIZED) {
        mRetry = MAX_RETRY;
    }
}

void UserState::zeroizeMasterKeysInMemory() {
    memset(mMasterKey.data(), 0, mMasterKey.size());
    memset(mSalt, 0, sizeof(mSalt));
}

bool UserState::deleteMasterKey() {
    setState(STATE_UNINITIALIZED);
    zeroizeMasterKeysInMemory();
    return unlink(mMasterKeyEntry.getKeyBlobPath().c_str()) == 0 || errno == ENOENT;
}

ResponseCode UserState::initialize(const android::String8& pw) {
    if (!generateMasterKey()) {
        return ResponseCode::SYSTEM_ERROR;
    }
    ResponseCode response = writeMasterKey(pw);
    if (response != ResponseCode::NO_ERROR) {
        return response;
    }
    setupMasterKeys();
    return ResponseCode::NO_ERROR;
}

ResponseCode UserState::copyMasterKey(LockedUserState<UserState>* src) {
    if (mState != STATE_UNINITIALIZED) {
        return ResponseCode::SYSTEM_ERROR;
    }
    if ((*src)->getState() != STATE_NO_ERROR) {
        return ResponseCode::SYSTEM_ERROR;
    }
    mMasterKey = (*src)->mMasterKey;
    setupMasterKeys();
    return copyMasterKeyFile(src);
}

ResponseCode UserState::copyMasterKeyFile(LockedUserState<UserState>* src) {
    /* Copy the master key file to the new user.  Unfortunately we don't have the src user's
     * password so we cannot generate a new file with a new salt.
     */
    int in = TEMP_FAILURE_RETRY(open((*src)->getMasterKeyFileName().c_str(), O_RDONLY));
    if (in < 0) {
        return ResponseCode::SYSTEM_ERROR;
    }
    blobv3 rawBlob;
    size_t length = readFully(in, (uint8_t*)&rawBlob, sizeof(rawBlob));
    if (close(in) != 0) {
        return ResponseCode::SYSTEM_ERROR;
    }
    int out = TEMP_FAILURE_RETRY(open(mMasterKeyEntry.getKeyBlobPath().c_str(),
                                      O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR));
    if (out < 0) {
        return ResponseCode::SYSTEM_ERROR;
    }
    size_t outLength = writeFully(out, (uint8_t*)&rawBlob, length);
    if (close(out) != 0) {
        return ResponseCode::SYSTEM_ERROR;
    }
    if (outLength != length) {
        ALOGW("blob not fully written %zu != %zu", outLength, length);
        unlink(mMasterKeyEntry.getKeyBlobPath().c_str());
        return ResponseCode::SYSTEM_ERROR;
    }
    return ResponseCode::NO_ERROR;
}

ResponseCode UserState::writeMasterKey(const android::String8& pw) {
    std::vector<uint8_t> passwordKey(mMasterKey.size());
    generateKeyFromPassword(passwordKey, pw, mSalt);
    auto blobType = TYPE_MASTER_KEY_AES256;
    if (mMasterKey.size() == kAes128KeySizeBytes) {
        blobType = TYPE_MASTER_KEY;
    }
    Blob masterKeyBlob(mMasterKey.data(), mMasterKey.size(), mSalt, sizeof(mSalt), blobType);
    auto lockedEntry = LockedKeyBlobEntry::get(mMasterKeyEntry);
    return lockedEntry.writeBlobs(masterKeyBlob, {}, passwordKey, STATE_NO_ERROR);
}

ResponseCode UserState::readMasterKey(const android::String8& pw) {

    auto lockedEntry = LockedKeyBlobEntry::get(mMasterKeyEntry);

    int in = TEMP_FAILURE_RETRY(open(mMasterKeyEntry.getKeyBlobPath().c_str(), O_RDONLY));
    if (in < 0) {
        return ResponseCode::SYSTEM_ERROR;
    }

    // We read the raw blob to just to get the salt to generate the AES key, then we create the Blob
    // to use with decryptBlob
    blobv3 rawBlob;
    size_t length = readFully(in, (uint8_t*)&rawBlob, sizeof(rawBlob));
    if (close(in) != 0) {
        return ResponseCode::SYSTEM_ERROR;
    }
    // find salt at EOF if present, otherwise we have an old file
    uint8_t* salt;
    if (length > SALT_SIZE && rawBlob.info == SALT_SIZE) {
        salt = (uint8_t*)&rawBlob + length - SALT_SIZE;
    } else {
        salt = nullptr;
    }

    size_t masterKeySize = MASTER_KEY_SIZE_BYTES;
    if (rawBlob.type == TYPE_MASTER_KEY) {
        masterKeySize = kAes128KeySizeBytes;
    }

    std::vector<uint8_t> passwordKey(masterKeySize);
    generateKeyFromPassword(passwordKey, pw, salt);
    Blob masterKeyBlob, dummyBlob;
    ResponseCode response;
    std::tie(response, masterKeyBlob, dummyBlob) =
        lockedEntry.readBlobs(passwordKey, STATE_NO_ERROR);
    if (response == ResponseCode::SYSTEM_ERROR) {
        return response;
    }

    size_t masterKeyBlobLength = static_cast<size_t>(masterKeyBlob.getLength());

    if (response == ResponseCode::NO_ERROR && masterKeyBlobLength == masterKeySize) {
        // If salt was missing, generate one and write a new master key file with the salt.
        if (salt == nullptr) {
            if (!generateSalt()) {
                return ResponseCode::SYSTEM_ERROR;
            }
            response = writeMasterKey(pw);
        }
        if (response == ResponseCode::NO_ERROR) {
            mMasterKey = std::vector<uint8_t>(masterKeyBlob.getValue(),
                                              masterKeyBlob.getValue() + masterKeyBlob.getLength());

            setupMasterKeys();
        }
        return response;
    }
    if (mRetry <= 0) {
        reset();
        return ResponseCode::UNINITIALIZED;
    }
    --mRetry;
    switch (mRetry) {
    case 0:
        return ResponseCode::WRONG_PASSWORD_0;
    case 1:
        return ResponseCode::WRONG_PASSWORD_1;
    case 2:
        return ResponseCode::WRONG_PASSWORD_2;
    case 3:
        return ResponseCode::WRONG_PASSWORD_3;
    default:
        return ResponseCode::WRONG_PASSWORD_3;
    }
}

bool UserState::reset() {
    DIR* dir = opendir(mMasterKeyEntry.user_dir().c_str());
    if (!dir) {
        // If the directory doesn't exist then nothing to do.
        if (errno == ENOENT) {
            return true;
        }
        ALOGW("couldn't open user directory: %s", strerror(errno));
        return false;
    }

    struct dirent* file;
    while ((file = readdir(dir)) != nullptr) {
        // skip . and ..
        if (!strcmp(".", file->d_name) || !strcmp("..", file->d_name)) {
            continue;
        }

        unlinkat(dirfd(dir), file->d_name, 0);
    }
    closedir(dir);
    return true;
}

void UserState::generateKeyFromPassword(std::vector<uint8_t>& key, const android::String8& pw,
                                        uint8_t* salt) {
    size_t saltSize;
    if (salt != nullptr) {
        saltSize = SALT_SIZE;
    } else {
        // Pre-gingerbread used this hardwired salt, readMasterKey will rewrite these when found
        salt = (uint8_t*)"keystore";
        // sizeof = 9, not strlen = 8
        saltSize = sizeof("keystore");
    }

    const EVP_MD* digest = EVP_sha256();

    // SHA1 was used prior to increasing the key size
    if (key.size() == kAes128KeySizeBytes) {
        digest = EVP_sha1();
    }

    PKCS5_PBKDF2_HMAC(reinterpret_cast<const char*>(pw.string()), pw.length(), salt, saltSize, 8192,
                      digest, key.size(), key.data());
}

bool UserState::generateSalt() {
    return RAND_bytes(mSalt, sizeof(mSalt));
}

bool UserState::generateMasterKey() {
    mMasterKey.resize(MASTER_KEY_SIZE_BYTES);
    if (!RAND_bytes(mMasterKey.data(), mMasterKey.size())) {
        return false;
    }
    if (!generateSalt()) {
        return false;
    }
    return true;
}

void UserState::setupMasterKeys() {
    setState(STATE_NO_ERROR);
}

LockedUserState<UserState> UserStateDB::getUserState(uid_t userId) {
    std::unique_lock<std::mutex> lock(locked_state_mutex_);
    decltype(mMasterKeys.begin()) it;
    bool inserted;
    std::tie(it, inserted) = mMasterKeys.emplace(userId, userId);
    if (inserted) {
        if (!it->second.initialize()) {
            /* There's not much we can do if initialization fails. Trying to
             * unlock the keystore for that user will fail as well, so any
             * subsequent request for this user will just return SYSTEM_ERROR.
             */
            ALOGE("User initialization failed for %u; subsequent operations will fail", userId);
        }
    }
    return get(std::move(lock), &it->second);
}

LockedUserState<UserState> UserStateDB::getUserStateByUid(uid_t uid) {
    return getUserState(get_user_id(uid));
}

LockedUserState<const UserState> UserStateDB::getUserState(uid_t userId) const {
    std::unique_lock<std::mutex> lock(locked_state_mutex_);
    auto it = mMasterKeys.find(userId);
    if (it == mMasterKeys.end()) return {};
    return get(std::move(lock), &it->second);
}

LockedUserState<const UserState> UserStateDB::getUserStateByUid(uid_t uid) const {
    return getUserState(get_user_id(uid));
}

}  // namespace keystore
