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

#include "key_store_service.h"

#include <fcntl.h>
#include <sys/stat.h>

#include <algorithm>
#include <atomic>
#include <sstream>

#include <android-base/scopeguard.h>
#include <binder/IInterface.h>
#include <binder/IPCThreadState.h>
#include <binder/IPermissionController.h>
#include <binder/IServiceManager.h>
#include <cutils/multiuser.h>
#include <log/log_event_list.h>

#include <private/android_filesystem_config.h>
#include <private/android_logger.h>

#include <android/hardware/confirmationui/1.0/IConfirmationUI.h>
#include <android/hardware/keymaster/3.0/IHwKeymasterDevice.h>

#include "defaults.h"
#include "key_proto_handler.h"
#include "keystore_attestation_id.h"
#include "keystore_keymaster_enforcement.h"
#include "keystore_utils.h"
#include <keystore/keystore_hidl_support.h>
#include <keystore/keystore_return_types.h>

#include <hardware/hw_auth_token.h>

namespace keystore {

using namespace android;

namespace {

using ::android::binder::Status;
using android::security::keymaster::ExportResult;
using android::security::keymaster::KeymasterArguments;
using android::security::keymaster::KeymasterBlob;
using android::security::keymaster::KeymasterCertificateChain;
using android::security::keymaster::operationFailed;
using android::security::keymaster::OperationResult;
using ConfirmationResponseCode = android::hardware::confirmationui::V1_0::ResponseCode;
using ::android::security::keystore::IKeystoreOperationResultCallback;
using ::android::security::keystore::IKeystoreResponseCallback;
using ::android::security::keystore::KeystoreResponse;

constexpr double kIdRotationPeriod = 30 * 24 * 60 * 60; /* Thirty days, in seconds */
const char* kTimestampFilePath = "timestamp";

struct BIGNUM_Delete {
    void operator()(BIGNUM* p) const { BN_free(p); }
};
typedef std::unique_ptr<BIGNUM, BIGNUM_Delete> Unique_BIGNUM;

bool containsTag(const hidl_vec<KeyParameter>& params, Tag tag) {
    return params.end() !=
           std::find_if(params.begin(), params.end(),
                        [&](const KeyParameter& param) { return param.tag == tag; });
}

#define AIDL_RETURN(rc) (*_aidl_return = KeyStoreServiceReturnCode(rc).getErrorCode(), Status::ok())
#define KEYSTORE_SERVICE_LOCK std::lock_guard<std::mutex> keystore_lock(keystoreServiceMutex_)

std::pair<KeyStoreServiceReturnCode, bool> hadFactoryResetSinceIdRotation() {
    struct stat sbuf;
    if (stat(kTimestampFilePath, &sbuf) == 0) {
        double diff_secs = difftime(time(nullptr), sbuf.st_ctime);
        return {ResponseCode::NO_ERROR, diff_secs < kIdRotationPeriod};
    }

    if (errno != ENOENT) {
        ALOGE("Failed to stat \"timestamp\" file, with error %d", errno);
        return {ResponseCode::SYSTEM_ERROR, false /* don't care */};
    }

    int fd = creat(kTimestampFilePath, 0600);
    if (fd < 0) {
        ALOGE("Couldn't create \"timestamp\" file, with error %d", errno);
        return {ResponseCode::SYSTEM_ERROR, false /* don't care */};
    }

    if (close(fd)) {
        ALOGE("Couldn't close \"timestamp\" file, with error %d", errno);
        return {ResponseCode::SYSTEM_ERROR, false /* don't care */};
    }

    return {ResponseCode::NO_ERROR, true};
}

using ::android::security::KEY_ATTESTATION_APPLICATION_ID_MAX_SIZE;

KeyStoreServiceReturnCode updateParamsForAttestation(uid_t callingUid, AuthorizationSet* params) {
    KeyStoreServiceReturnCode responseCode;
    bool factoryResetSinceIdRotation;
    std::tie(responseCode, factoryResetSinceIdRotation) = hadFactoryResetSinceIdRotation();

    if (!responseCode.isOk()) return responseCode;
    if (factoryResetSinceIdRotation) params->push_back(TAG_RESET_SINCE_ID_ROTATION);

    auto asn1_attestation_id_result = security::gather_attestation_application_id(callingUid);
    if (!asn1_attestation_id_result.isOk()) {
        ALOGE("failed to gather attestation_id");
        return ErrorCode::ATTESTATION_APPLICATION_ID_MISSING;
    }
    std::vector<uint8_t>& asn1_attestation_id = asn1_attestation_id_result;

    /*
     * The attestation application ID must not be longer than
     * KEY_ATTESTATION_APPLICATION_ID_MAX_SIZE, error out if gather_attestation_application_id
     * returned such an invalid vector.
     */
    if (asn1_attestation_id.size() > KEY_ATTESTATION_APPLICATION_ID_MAX_SIZE) {
        ALOGE("BUG: Gathered Attestation Application ID is too big (%d)",
              static_cast<int32_t>(asn1_attestation_id.size()));
        return ErrorCode::CANNOT_ATTEST_IDS;
    }

    params->push_back(TAG_ATTESTATION_APPLICATION_ID, asn1_attestation_id);

    return ResponseCode::NO_ERROR;
}

}  // anonymous namespace

Status KeyStoreService::getState(int32_t userId, int32_t* aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    if (!checkBinderPermission(P_GET_STATE)) {
        *aidl_return = static_cast<int32_t>(ResponseCode::PERMISSION_DENIED);
        return Status::ok();
    }
    *aidl_return = mKeyStore->getState(userId);
    return Status::ok();
}

Status KeyStoreService::get(const String16& name, int32_t uid, ::std::vector<uint8_t>* item) {
    KEYSTORE_SERVICE_LOCK;
    uid_t targetUid = getEffectiveUid(uid);
    if (!checkBinderPermission(P_GET, targetUid)) {
        // see keystore/keystore.h
        return Status::fromServiceSpecificError(
            static_cast<int32_t>(ResponseCode::PERMISSION_DENIED));
    }

    String8 name8(name);
    ResponseCode rc;
    Blob keyBlob;
    Blob charBlob;
    LockedKeyBlobEntry lockedEntry;

    std::tie(rc, keyBlob, charBlob, lockedEntry) =
        mKeyStore->getKeyForName(name8, targetUid, TYPE_GENERIC);
    if (rc != ResponseCode::NO_ERROR) {
        *item = ::std::vector<uint8_t>();
        // Return empty array if key is not found
        // TODO: consider having returned value nullable or parse exception on the client.
        return Status::fromServiceSpecificError(static_cast<int32_t>(rc));
    }
    auto resultBlob = blob2hidlVec(keyBlob);
    // The static_cast here is needed to prevent a move, forcing a deep copy.
    if (item) *item = static_cast<const hidl_vec<uint8_t>&>(blob2hidlVec(keyBlob));
    return Status::ok();
}

Status KeyStoreService::insert(const String16& name, const ::std::vector<uint8_t>& item,
                               int targetUid, int32_t flags, int32_t* aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    targetUid = getEffectiveUid(targetUid);
    KeyStoreServiceReturnCode result =
        checkBinderPermissionAndKeystoreState(P_INSERT, targetUid, flags & KEYSTORE_FLAG_ENCRYPTED);
    if (!result.isOk()) {
        *aidl_return = result.getErrorCode();
        return Status::ok();
    }

    String8 name8(name);
    auto lockedEntry = mKeyStore->getLockedBlobEntryIfNotExists(name8.string(), targetUid);

    if (!lockedEntry) {
        ALOGE("failed to grab lock on blob entry %u_%s", targetUid, name8.string());
        *aidl_return = static_cast<int32_t>(ResponseCode::KEY_ALREADY_EXISTS);
        return Status::ok();
    }

    Blob keyBlob(&item[0], item.size(), nullptr, 0, ::TYPE_GENERIC);
    keyBlob.setEncrypted(flags & KEYSTORE_FLAG_ENCRYPTED);

    *aidl_return = static_cast<int32_t>(mKeyStore->put(lockedEntry, keyBlob, {}));
    return Status::ok();
}

Status KeyStoreService::del(const String16& name, int targetUid, int32_t* aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    targetUid = getEffectiveUid(targetUid);
    if (!checkBinderPermission(P_DELETE, targetUid)) {
        *aidl_return = static_cast<int32_t>(ResponseCode::PERMISSION_DENIED);
        return Status::ok();
    }
    String8 name8(name);
    ALOGI("del %s %d", name8.string(), targetUid);
    auto lockedEntry = mKeyStore->getLockedBlobEntryIfExists(name8.string(), targetUid);
    if (!lockedEntry) {
        *aidl_return = static_cast<int32_t>(ResponseCode::KEY_NOT_FOUND);
        return Status::ok();
    }

    ResponseCode result = mKeyStore->del(lockedEntry);

    *aidl_return = static_cast<int32_t>(result);
    return Status::ok();
}

Status KeyStoreService::exist(const String16& name, int targetUid, int32_t* aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    targetUid = getEffectiveUid(targetUid);
    if (!checkBinderPermission(P_EXIST, targetUid)) {
        *aidl_return = static_cast<int32_t>(ResponseCode::PERMISSION_DENIED);
        return Status::ok();
    }

    LockedKeyBlobEntry lockedEntry =
        mKeyStore->getLockedBlobEntryIfExists(String8(name).string(), targetUid);
    *aidl_return =
        static_cast<int32_t>(lockedEntry ? ResponseCode::NO_ERROR : ResponseCode::KEY_NOT_FOUND);
    return Status::ok();
}

Status KeyStoreService::list(const String16& prefix, int32_t targetUid,
                             ::std::vector<::android::String16>* matches) {
    KEYSTORE_SERVICE_LOCK;
    targetUid = getEffectiveUid(targetUid);
    if (!checkBinderPermission(P_LIST, targetUid)) {
        return Status::fromServiceSpecificError(
            static_cast<int32_t>(ResponseCode::PERMISSION_DENIED));
    }
    const String8 prefix8(prefix);
    const std::string stdPrefix(prefix8.string());

    ResponseCode rc;
    std::list<LockedKeyBlobEntry> internal_matches;
    auto userDirName = mKeyStore->getUserStateDB().getUserStateByUid(targetUid)->getUserDirName();

    std::tie(rc, internal_matches) =
        LockedKeyBlobEntry::list(userDirName, [&](uid_t uid, const std::string& alias) {
            std::mismatch(stdPrefix.begin(), stdPrefix.end(), alias.begin(), alias.end());
            return uid == static_cast<uid_t>(targetUid) &&
                   std::mismatch(stdPrefix.begin(), stdPrefix.end(), alias.begin(), alias.end())
                           .first == stdPrefix.end();
        });

    if (rc != ResponseCode::NO_ERROR) {
        return Status::fromServiceSpecificError(static_cast<int32_t>(rc));
    }

    for (LockedKeyBlobEntry& entry : internal_matches) {
        matches->push_back(String16(entry->alias().substr(prefix8.size()).c_str()));
    }
    return Status::ok();
}

/*
 * This method will return the uids of all auth bound keys for the calling user.
 * This is intended to be used for alerting the user about which apps will be affected
 * if the password/pin is removed. Only allowed to be called by system.
 * The output is bound by the initial size of uidsOut to be compatible with Java.
 */
Status KeyStoreService::listUidsOfAuthBoundKeys(std::vector<std::string>* uidsOut,
                                                int32_t* aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    const int32_t callingUid = IPCThreadState::self()->getCallingUid();
    const int32_t userId = get_user_id(callingUid);
    const int32_t appId = get_app_id(callingUid);
    if (appId != AID_SYSTEM) {
        ALOGE("Permission listUidsOfAuthBoundKeys denied for aid %d", appId);
        *aidl_return = static_cast<int32_t>(ResponseCode::PERMISSION_DENIED);
        return Status::ok();
    }

    const String8 prefix8("");
    auto userState = mKeyStore->getUserStateDB().getUserState(userId);
    const std::string userDirName = userState->getUserDirName();
    auto encryptionKey = userState->getEncryptionKey();
    auto state = userState->getState();
    // unlock the user state
    userState = {};

    ResponseCode rc;
    std::list<LockedKeyBlobEntry> internal_matches;
    std::tie(rc, internal_matches) =
        LockedKeyBlobEntry::list(userDirName, [&](uid_t, const std::string&) {
            // Need to filter on auth bound state, so just return true.
            return true;
        });
    if (rc != ResponseCode::NO_ERROR) {
        ALOGE("Error listing blob entries for user %d", userId);
        return Status::fromServiceSpecificError(static_cast<int32_t>(rc));
    }

    for (LockedKeyBlobEntry& entry : internal_matches) {
        // Need to store uids as a list of strings because integer list output
        // parameters is not supported in aidl-cpp.
        std::string entryUid = std::to_string(entry->uid());
        if (std::find(uidsOut->begin(), uidsOut->end(), entryUid) != uidsOut->end()) {
            // uid already in list, skip
            continue;
        }

        auto [rc, blob, charBlob] = entry.readBlobs(encryptionKey, state);
        if (rc != ResponseCode::NO_ERROR && rc != ResponseCode::LOCKED) {
            ALOGE("Error reading blob for key %s", entry->alias().c_str());
            continue;
        }

        if (blob && blob.isEncrypted()) {
            uidsOut->push_back(entryUid);
        } else if (charBlob) {
            auto [success, hwEnforced, swEnforced] = charBlob.getKeyCharacteristics();
            if (!success) {
                ALOGE("Error reading blob characteristics for key %s", entry->alias().c_str());
                continue;
            }
            if (hwEnforced.Contains(TAG_USER_SECURE_ID) ||
                swEnforced.Contains(TAG_USER_SECURE_ID)) {
                uidsOut->push_back(entryUid);
            }
        }
    }
    *aidl_return = static_cast<int32_t>(ResponseCode::NO_ERROR);
    return Status::ok();
}

Status KeyStoreService::reset(int32_t* aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    if (!checkBinderPermission(P_RESET)) {
        *aidl_return = static_cast<int32_t>(ResponseCode::PERMISSION_DENIED);
        return Status::ok();
    }

    uid_t callingUid = IPCThreadState::self()->getCallingUid();
    mKeyStore->resetUser(get_user_id(callingUid), false);
    *aidl_return = static_cast<int32_t>(ResponseCode::NO_ERROR);
    return Status::ok();
}

Status KeyStoreService::onUserPasswordChanged(int32_t userId, const String16& password,
                                              int32_t* aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    if (!checkBinderPermission(P_PASSWORD)) {
        *aidl_return = static_cast<int32_t>(ResponseCode::PERMISSION_DENIED);
        return Status::ok();
    }

    if (password.size() == 0) {
        ALOGI("Secure lockscreen for user %d removed, deleting encrypted entries", userId);
        mKeyStore->resetUser(userId, true);
        *aidl_return = static_cast<int32_t>(ResponseCode::NO_ERROR);
        return Status::ok();
    } else {
        const String8 password8(password);
        switch (mKeyStore->getState(userId)) {
        case ::STATE_UNINITIALIZED: {
            // generate master key, encrypt with password, write to file,
            // initialize mMasterKey*.
            *aidl_return = static_cast<int32_t>(mKeyStore->initializeUser(password8, userId));
            return Status::ok();
        }
        case ::STATE_NO_ERROR: {
            // rewrite master key with new password.
            *aidl_return = static_cast<int32_t>(mKeyStore->writeMasterKey(password8, userId));
            return Status::ok();
        }
        case ::STATE_LOCKED: {
            ALOGE("Changing user %d's password while locked, clearing old encryption", userId);
            mKeyStore->resetUser(userId, true);
            *aidl_return = static_cast<int32_t>(mKeyStore->initializeUser(password8, userId));
            return Status::ok();
        }
        }
        *aidl_return = static_cast<int32_t>(ResponseCode::SYSTEM_ERROR);
        return Status::ok();
    }
}

Status KeyStoreService::onUserAdded(int32_t userId, int32_t parentId, int32_t* aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    if (!checkBinderPermission(P_USER_CHANGED)) {
        *aidl_return = static_cast<int32_t>(ResponseCode::PERMISSION_DENIED);
        return Status::ok();
    }

    // Sanity check that the new user has an empty keystore.
    if (!mKeyStore->isEmpty(userId)) {
        ALOGW("New user %d's keystore not empty. Clearing old entries.", userId);
    }
    // Unconditionally clear the keystore, just to be safe.
    mKeyStore->resetUser(userId, false);
    if (parentId != -1) {
        // This profile must share the same master key password as the parent profile. Because the
        // password of the parent profile is not known here, the best we can do is copy the parent's
        // master key and master key file. This makes this profile use the same master key as the
        // parent profile, forever.
        *aidl_return = static_cast<int32_t>(mKeyStore->copyMasterKey(parentId, userId));
        return Status::ok();
    } else {
        *aidl_return = static_cast<int32_t>(ResponseCode::NO_ERROR);
        return Status::ok();
    }
}

Status KeyStoreService::onUserRemoved(int32_t userId, int32_t* aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    if (!checkBinderPermission(P_USER_CHANGED)) {
        *aidl_return = static_cast<int32_t>(ResponseCode::PERMISSION_DENIED);
        return Status::ok();
    }

    mKeyStore->resetUser(userId, false);
    *aidl_return = static_cast<int32_t>(ResponseCode::NO_ERROR);
    return Status::ok();
}

Status KeyStoreService::lock(int32_t userId, int32_t* aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    if (!checkBinderPermission(P_LOCK)) {
        *aidl_return = static_cast<int32_t>(ResponseCode::PERMISSION_DENIED);
        return Status::ok();
    }

    State state = mKeyStore->getState(userId);
    if (state != ::STATE_NO_ERROR) {
        ALOGD("calling lock in state: %d", state);
        *aidl_return = static_cast<int32_t>(ResponseCode(state));
        return Status::ok();
    }

    mKeyStore->getEnforcementPolicy().set_device_locked(true, userId);
    mKeyStore->lock(userId);
    *aidl_return = static_cast<int32_t>(ResponseCode::NO_ERROR);
    return Status::ok();
}

Status KeyStoreService::unlock(int32_t userId, const String16& pw, int32_t* aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    if (!checkBinderPermission(P_UNLOCK)) {
        *aidl_return = static_cast<int32_t>(ResponseCode::PERMISSION_DENIED);
        return Status::ok();
    }

    State state = mKeyStore->getState(userId);
    if (state != ::STATE_LOCKED) {
        switch (state) {
        case ::STATE_NO_ERROR:
            ALOGI("calling unlock when already unlocked, ignoring.");
            break;
        case ::STATE_UNINITIALIZED:
            ALOGE("unlock called on uninitialized keystore.");
            break;
        default:
            ALOGE("unlock called on keystore in unknown state: %d", state);
            break;
        }
        *aidl_return = static_cast<int32_t>(ResponseCode(state));
        return Status::ok();
    }

    mKeyStore->getEnforcementPolicy().set_device_locked(false, userId);
    const String8 password8(pw);
    // read master key, decrypt with password, initialize mMasterKey*.
    *aidl_return = static_cast<int32_t>(mKeyStore->readMasterKey(password8, userId));
    return Status::ok();
}

Status KeyStoreService::isEmpty(int32_t userId, int32_t* aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    if (!checkBinderPermission(P_IS_EMPTY)) {
        *aidl_return = static_cast<int32_t>(false);
        return Status::ok();
    }

    *aidl_return = static_cast<int32_t>(mKeyStore->isEmpty(userId));
    return Status::ok();
}

Status KeyStoreService::grant(const String16& name, int32_t granteeUid,
                              ::android::String16* aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    uid_t callingUid = IPCThreadState::self()->getCallingUid();
    auto result =
        checkBinderPermissionAndKeystoreState(P_GRANT, /*targetUid=*/-1, /*checkUnlocked=*/false);
    if (!result.isOk()) {
        *aidl_return = String16();
        return Status::ok();
    }

    String8 name8(name);
    auto lockedEntry = mKeyStore->getLockedBlobEntryIfExists(name8.string(), callingUid);
    if (!lockedEntry) {
        *aidl_return = String16();
        return Status::ok();
    }

    *aidl_return = String16(mKeyStore->addGrant(lockedEntry, granteeUid).c_str());
    return Status::ok();
}

Status KeyStoreService::ungrant(const String16& name, int32_t granteeUid, int32_t* aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    uid_t callingUid = IPCThreadState::self()->getCallingUid();
    KeyStoreServiceReturnCode result =
        checkBinderPermissionAndKeystoreState(P_GRANT, /*targetUid=*/-1, /*checkUnlocked=*/false);
    if (!result.isOk()) {
        *aidl_return = result.getErrorCode();
        return Status::ok();
    }

    String8 name8(name);

    auto lockedEntry = mKeyStore->getLockedBlobEntryIfExists(name8.string(), callingUid);
    if (!lockedEntry) {
        *aidl_return = static_cast<int32_t>(ResponseCode::KEY_NOT_FOUND);
    }

    *aidl_return = mKeyStore->removeGrant(lockedEntry, granteeUid);
    return Status::ok();
}

Status KeyStoreService::getmtime(const String16& name, int32_t uid, int64_t* time) {
    KEYSTORE_SERVICE_LOCK;
    uid_t targetUid = getEffectiveUid(uid);
    if (!checkBinderPermission(P_GET, targetUid)) {
        ALOGW("permission denied for %d: getmtime", targetUid);
        *time = -1L;
        return Status::ok();
    }
    String8 name8(name);

    auto lockedEntry = mKeyStore->getLockedBlobEntryIfExists(name8.string(), targetUid);
    if (!lockedEntry) {
        ALOGW("could not access key with alias %s for getmtime", name8.string());
        *time = -1L;
        return Status::ok();
    }

    std::string filename = lockedEntry->getKeyBlobPath();

    int fd = TEMP_FAILURE_RETRY(open(filename.c_str(), O_NOFOLLOW, O_RDONLY));
    if (fd < 0) {
        ALOGW("could not open %s for getmtime", filename.c_str());
        *time = -1L;
        return Status::ok();
    }

    struct stat s;
    int ret = fstat(fd, &s);
    close(fd);
    if (ret == -1) {
        ALOGW("could not stat %s for getmtime", filename.c_str());
        *time = -1L;
        return Status::ok();
    }

    *time = static_cast<int64_t>(s.st_mtime);
    return Status::ok();
}

Status KeyStoreService::is_hardware_backed(const String16& keyType, int32_t* aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    *aidl_return = static_cast<int32_t>(mKeyStore->isHardwareBacked(keyType) ? 1 : 0);
    return Status::ok();
}

Status KeyStoreService::clear_uid(int64_t targetUid64, int32_t* _aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    uid_t targetUid = getEffectiveUid(targetUid64);
    if (!checkBinderPermissionSelfOrSystem(P_CLEAR_UID, targetUid)) {
        return AIDL_RETURN(ResponseCode::PERMISSION_DENIED);
    }
    ALOGI("clear_uid %" PRId64, targetUid64);

    mKeyStore->removeAllGrantsToUid(targetUid);

    ResponseCode rc;
    std::list<LockedKeyBlobEntry> entries;
    auto userDirName = mKeyStore->getUserStateDB().getUserStateByUid(targetUid)->getUserDirName();

    // list has a fence making sure no workers are modifying blob files before iterating the
    // data base. All returned entries are locked.
    std::tie(rc, entries) = LockedKeyBlobEntry::list(
        userDirName, [&](uid_t uid, const std::string&) -> bool { return uid == targetUid; });

    if (rc != ResponseCode::NO_ERROR) {
        return AIDL_RETURN(rc);
    }

    for (LockedKeyBlobEntry& lockedEntry : entries) {
        if (get_app_id(targetUid) == AID_SYSTEM) {
            Blob keyBlob;
            Blob charBlob;
            std::tie(rc, keyBlob, charBlob) = mKeyStore->get(lockedEntry);
            if (rc == ResponseCode::NO_ERROR && keyBlob.isCriticalToDeviceEncryption()) {
                // Do not clear keys critical to device encryption under system uid.
                continue;
            }
        }
        mKeyStore->del(lockedEntry);
    }
    return AIDL_RETURN(ResponseCode::NO_ERROR);
}

Status KeyStoreService::addRngEntropy(
    const ::android::sp<::android::security::keystore::IKeystoreResponseCallback>& cb,
    const ::std::vector<uint8_t>& entropy, int32_t flags, int32_t* _aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    auto device = mKeyStore->getDevice(flagsToSecurityLevel(flags));
    if (!device) {
        return AIDL_RETURN(ErrorCode::HARDWARE_TYPE_UNAVAILABLE);
    }

    device->addRngEntropy(entropy, [device, cb](Return<ErrorCode> rc) {
        cb->onFinished(KeyStoreServiceReturnCode(KS_HANDLE_HIDL_ERROR(device, rc)));
    });

    return AIDL_RETURN(ResponseCode::NO_ERROR);
}

Status KeyStoreService::generateKey(
    const ::android::sp<::android::security::keystore::IKeystoreKeyCharacteristicsCallback>& cb,
    const String16& name, const KeymasterArguments& params, const ::std::vector<uint8_t>& entropy,
    int uid, int flags, int32_t* _aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    // TODO(jbires): remove this getCallingUid call upon implementation of b/25646100
    uid_t originalUid = IPCThreadState::self()->getCallingUid();
    uid = getEffectiveUid(uid);
    auto logOnScopeExit = android::base::make_scope_guard([&] {
        if (__android_log_security()) {
            android_log_event_list(SEC_TAG_AUTH_KEY_GENERATED)
                << int32_t(*_aidl_return == static_cast<int32_t>(ResponseCode::NO_ERROR))
                << String8(name) << int32_t(uid) << LOG_ID_SECURITY;
        }
    });
    KeyStoreServiceReturnCode rc =
        checkBinderPermissionAndKeystoreState(P_INSERT, uid, flags & KEYSTORE_FLAG_ENCRYPTED);
    if (!rc.isOk()) {
        return AIDL_RETURN(rc);
    }
    if ((flags & KEYSTORE_FLAG_CRITICAL_TO_DEVICE_ENCRYPTION) && get_app_id(uid) != AID_SYSTEM) {
        ALOGE("Non-system uid %d cannot set FLAG_CRITICAL_TO_DEVICE_ENCRYPTION", uid);
        return AIDL_RETURN(ResponseCode::PERMISSION_DENIED);
    }

    if (containsTag(params.getParameters(), Tag::INCLUDE_UNIQUE_ID)) {
        // TODO(jbires): remove uid checking upon implementation of b/25646100
        if (!checkBinderPermission(P_GEN_UNIQUE_ID) ||
            originalUid != IPCThreadState::self()->getCallingUid()) {
            return AIDL_RETURN(ResponseCode::PERMISSION_DENIED);
        }
    }

    SecurityLevel securityLevel = flagsToSecurityLevel(flags);
    auto dev = mKeyStore->getDevice(securityLevel);
    if (!dev) {
        return AIDL_RETURN(ErrorCode::HARDWARE_TYPE_UNAVAILABLE);
    }

    String8 name8(name);
    auto lockedEntry = mKeyStore->getLockedBlobEntryIfNotExists(name8.string(), uid);
    if (!lockedEntry) {
        return AIDL_RETURN(ResponseCode::KEY_ALREADY_EXISTS);
    }

    logOnScopeExit.Disable();

    dev->generateKey(
        std::move(lockedEntry), params.getParameters(), entropy, flags,
        [cb, uid, name](KeyStoreServiceReturnCode rc, KeyCharacteristics keyCharacteristics) {
            if (__android_log_security()) {
                android_log_event_list(SEC_TAG_AUTH_KEY_GENERATED)
                    << rc.isOk() << String8(name) << int32_t(uid) << LOG_ID_SECURITY;
            }
            cb->onFinished(rc,
                           android::security::keymaster::KeyCharacteristics(keyCharacteristics));
        });

    return AIDL_RETURN(ResponseCode::NO_ERROR);
}

Status KeyStoreService::getKeyCharacteristics(
    const ::android::sp<::android::security::keystore::IKeystoreKeyCharacteristicsCallback>& cb,
    const String16& name, const ::android::security::keymaster::KeymasterBlob& clientId,
    const ::android::security::keymaster::KeymasterBlob& appData, int32_t uid,
    int32_t* _aidl_return) {
    KEYSTORE_SERVICE_LOCK;

    uid_t targetUid = getEffectiveUid(uid);
    uid_t callingUid = IPCThreadState::self()->getCallingUid();
    if (!is_granted_to(callingUid, targetUid)) {
        ALOGW("uid %d not permitted to act for uid %d in getKeyCharacteristics", callingUid,
              targetUid);
        return AIDL_RETURN(ResponseCode::PERMISSION_DENIED);
    }

    String8 name8(name);

    ResponseCode rc;
    Blob keyBlob;
    Blob charBlob;
    LockedKeyBlobEntry lockedEntry;

    std::tie(rc, keyBlob, charBlob, lockedEntry) =
        mKeyStore->getKeyForName(name8, targetUid, TYPE_KEYMASTER_10);

    if (rc != ResponseCode::NO_ERROR) {
        return AIDL_RETURN(rc);
    }

    auto dev = mKeyStore->getDevice(keyBlob);
    if (!dev) {
        return AIDL_RETURN(ResponseCode::SYSTEM_ERROR);
    }

    // If the charBlob is up to date, it simply moves the argument blobs to the returned blobs
    // and extracts the characteristics on the way. Otherwise it updates the cache file with data
    // from keymaster. It may also upgrade the key blob.
    dev->getKeyCharacteristics(
        std::move(lockedEntry), clientId.getData(), appData.getData(), std::move(keyBlob),
        std::move(charBlob),
        [cb](KeyStoreServiceReturnCode rc, KeyCharacteristics keyCharacteristics) {
            cb->onFinished(rc,
                           android::security::keymaster::KeyCharacteristics(keyCharacteristics));
        });

    return AIDL_RETURN(ResponseCode::NO_ERROR);
}

Status KeyStoreService::importKey(
    const ::android::sp<::android::security::keystore::IKeystoreKeyCharacteristicsCallback>& cb,
    const String16& name, const KeymasterArguments& params, int32_t format,
    const ::std::vector<uint8_t>& keyData, int uid, int flags, int32_t* _aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    uid = getEffectiveUid(uid);
    auto logOnScopeExit = android::base::make_scope_guard([&] {
        if (__android_log_security()) {
            android_log_event_list(SEC_TAG_KEY_IMPORTED)
                << int32_t(*_aidl_return == static_cast<int32_t>(ResponseCode::NO_ERROR))
                << String8(name) << int32_t(uid) << LOG_ID_SECURITY;
        }
    });
    KeyStoreServiceReturnCode rc =
        checkBinderPermissionAndKeystoreState(P_INSERT, uid, flags & KEYSTORE_FLAG_ENCRYPTED);
    if (!rc.isOk()) {
        LOG(ERROR) << "permissission denied";
        return AIDL_RETURN(rc);
    }
    if ((flags & KEYSTORE_FLAG_CRITICAL_TO_DEVICE_ENCRYPTION) && get_app_id(uid) != AID_SYSTEM) {
        ALOGE("Non-system uid %d cannot set FLAG_CRITICAL_TO_DEVICE_ENCRYPTION", uid);
        return AIDL_RETURN(ResponseCode::PERMISSION_DENIED);
    }

    SecurityLevel securityLevel = flagsToSecurityLevel(flags);
    auto dev = mKeyStore->getDevice(securityLevel);
    if (!dev) {
        LOG(ERROR) << "importKey - cound not get keymaster device";
        return AIDL_RETURN(ErrorCode::HARDWARE_TYPE_UNAVAILABLE);
    }

    String8 name8(name);
    auto lockedEntry = mKeyStore->getLockedBlobEntryIfNotExists(name8.string(), uid);
    if (!lockedEntry) {
        LOG(ERROR) << "importKey - key: " << name8.string() << " " << int(uid)
                   << " already exists.";
        return AIDL_RETURN(ResponseCode::KEY_ALREADY_EXISTS);
    }

    logOnScopeExit.Disable();

    dev->importKey(
        std::move(lockedEntry), params.getParameters(), KeyFormat(format), keyData, flags,
        [cb, uid, name](KeyStoreServiceReturnCode rc, KeyCharacteristics keyCharacteristics) {
            if (__android_log_security()) {
                android_log_event_list(SEC_TAG_KEY_IMPORTED)
                    << rc.isOk() << String8(name) << int32_t(uid) << LOG_ID_SECURITY;
            }
            cb->onFinished(rc,
                           android::security::keymaster::KeyCharacteristics(keyCharacteristics));
        });

    return AIDL_RETURN(ResponseCode::NO_ERROR);
}

Status KeyStoreService::exportKey(
    const ::android::sp<::android::security::keystore::IKeystoreExportKeyCallback>& cb,
    const String16& name, int32_t format,
    const ::android::security::keymaster::KeymasterBlob& clientId,
    const ::android::security::keymaster::KeymasterBlob& appData, int32_t uid,
    int32_t* _aidl_return) {
    KEYSTORE_SERVICE_LOCK;

    uid_t targetUid = getEffectiveUid(uid);
    uid_t callingUid = IPCThreadState::self()->getCallingUid();
    if (!is_granted_to(callingUid, targetUid)) {
        ALOGW("uid %d not permitted to act for uid %d in exportKey", callingUid, targetUid);
        return AIDL_RETURN(ResponseCode::PERMISSION_DENIED);
    }

    String8 name8(name);

    KeyStoreServiceReturnCode rc;
    Blob keyBlob;
    Blob charBlob;
    LockedKeyBlobEntry lockedEntry;

    std::tie(rc, keyBlob, charBlob, lockedEntry) =
        mKeyStore->getKeyForName(name8, targetUid, TYPE_KEYMASTER_10);
    if (!rc.isOk()) {
        return AIDL_RETURN(rc);
    }

    auto dev = mKeyStore->getDevice(keyBlob);

    dev->exportKey(std::move(lockedEntry), KeyFormat(format), clientId.getData(), appData.getData(),
                   std::move(keyBlob), std::move(charBlob),
                   [cb](ExportResult exportResult) { cb->onFinished(exportResult); });

    return AIDL_RETURN(ResponseCode::NO_ERROR);
}

Status KeyStoreService::begin(const sp<IKeystoreOperationResultCallback>& cb,
                              const sp<IBinder>& appToken, const String16& name, int32_t purpose,
                              bool pruneable, const KeymasterArguments& params,
                              const ::std::vector<uint8_t>& entropy, int32_t uid,
                              int32_t* _aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    uid_t callingUid = IPCThreadState::self()->getCallingUid();
    uid_t targetUid = getEffectiveUid(uid);
    if (!is_granted_to(callingUid, targetUid)) {
        ALOGW("uid %d not permitted to act for uid %d in begin", callingUid, targetUid);
        return AIDL_RETURN(ResponseCode::PERMISSION_DENIED);
    }
    if (!pruneable && get_app_id(callingUid) != AID_SYSTEM) {
        ALOGE("Non-system uid %d trying to start non-pruneable operation", callingUid);
        return AIDL_RETURN(ResponseCode::PERMISSION_DENIED);
    }
    if (!checkAllowedOperationParams(params.getParameters())) {
        return AIDL_RETURN(ErrorCode::INVALID_ARGUMENT);
    }

    String8 name8(name);
    Blob keyBlob;
    Blob charBlob;
    LockedKeyBlobEntry lockedEntry;
    ResponseCode rc;

    std::tie(rc, keyBlob, charBlob, lockedEntry) =
        mKeyStore->getKeyForName(name8, targetUid, TYPE_KEYMASTER_10);

    if (rc == ResponseCode::LOCKED && keyBlob.isSuperEncrypted()) {
        return AIDL_RETURN(ErrorCode::KEY_USER_NOT_AUTHENTICATED);
    }
    if (rc != ResponseCode::NO_ERROR) return AIDL_RETURN(rc);

    auto dev = mKeyStore->getDevice(keyBlob);
    AuthorizationSet opParams = params.getParameters();

    dev->begin(std::move(lockedEntry), appToken, std::move(keyBlob), std::move(charBlob), pruneable,
               static_cast<KeyPurpose>(purpose), std::move(opParams), entropy,
               [this, cb, dev](OperationResult result_) {
                   if (result_.resultCode.isOk() ||
                       result_.resultCode == ResponseCode::OP_AUTH_NEEDED) {
                       mKeyStore->addOperationDevice(result_.token, dev);
                   }
                   cb->onFinished(result_);
               });

    return AIDL_RETURN(ResponseCode::NO_ERROR);
}

Status KeyStoreService::update(const ::android::sp<IKeystoreOperationResultCallback>& cb,
                               const ::android::sp<::android::IBinder>& token,
                               const ::android::security::keymaster::KeymasterArguments& params,
                               const ::std::vector<uint8_t>& input, int32_t* _aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    if (!checkAllowedOperationParams(params.getParameters())) {
        return AIDL_RETURN(ErrorCode::INVALID_ARGUMENT);
    }

    auto dev = mKeyStore->getOperationDevice(token);
    if (!dev) {
        return AIDL_RETURN(ErrorCode::INVALID_OPERATION_HANDLE);
    }

    dev->update(token, params.getParameters(), input, [this, cb, token](OperationResult result_) {
        if (!result_.resultCode.isOk()) {
            mKeyStore->removeOperationDevice(token);
        }
        cb->onFinished(result_);
    });

    return AIDL_RETURN(ResponseCode::NO_ERROR);
}

Status KeyStoreService::finish(const ::android::sp<IKeystoreOperationResultCallback>& cb,
                               const ::android::sp<::android::IBinder>& token,
                               const ::android::security::keymaster::KeymasterArguments& params,
                               const ::std::vector<uint8_t>& signature,
                               const ::std::vector<uint8_t>& entropy, int32_t* _aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    if (!checkAllowedOperationParams(params.getParameters())) {
        return AIDL_RETURN(ErrorCode::INVALID_ARGUMENT);
    }

    auto dev = mKeyStore->getOperationDevice(token);
    if (!dev) {
        return AIDL_RETURN(ErrorCode::INVALID_OPERATION_HANDLE);
    }

    dev->finish(token, params.getParameters(), {}, signature, entropy,
                [this, cb, token](OperationResult result_) {
                    mKeyStore->removeOperationDevice(token);
                    cb->onFinished(result_);
                });

    return AIDL_RETURN(ResponseCode::NO_ERROR);
}

Status KeyStoreService::abort(const ::android::sp<IKeystoreResponseCallback>& cb,
                              const ::android::sp<::android::IBinder>& token,
                              int32_t* _aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    auto dev = mKeyStore->getOperationDevice(token);
    if (!dev) {
        return AIDL_RETURN(ErrorCode::INVALID_OPERATION_HANDLE);
    }

    dev->abort(token, [this, cb, token](KeyStoreServiceReturnCode rc) {
        mKeyStore->removeOperationDevice(token);
        cb->onFinished(rc);
    });

    return AIDL_RETURN(ResponseCode::NO_ERROR);
}

Status KeyStoreService::addAuthToken(const ::std::vector<uint8_t>& authTokenAsVector,
                                     int32_t* aidl_return) {
    KEYSTORE_SERVICE_LOCK;

    // TODO(swillden): When gatekeeper and fingerprint are ready, this should be updated to
    // receive a HardwareAuthToken, rather than an opaque byte array.

    if (!checkBinderPermission(P_ADD_AUTH)) {
        ALOGW("addAuthToken: permission denied for %d", IPCThreadState::self()->getCallingUid());
        *aidl_return = static_cast<int32_t>(ResponseCode::PERMISSION_DENIED);
        return Status::ok();
    }
    if (authTokenAsVector.size() != sizeof(hw_auth_token_t)) {
        *aidl_return = KeyStoreServiceReturnCode(ErrorCode::INVALID_ARGUMENT).getErrorCode();
        return Status::ok();
    }

    hw_auth_token_t authToken;
    memcpy(reinterpret_cast<void*>(&authToken), authTokenAsVector.data(), sizeof(hw_auth_token_t));
    if (authToken.version != 0) {
        *aidl_return = KeyStoreServiceReturnCode(ErrorCode::INVALID_ARGUMENT).getErrorCode();
        return Status::ok();
    }

    mKeyStore->getAuthTokenTable().AddAuthenticationToken(
        hidlVec2AuthToken(hidl_vec<uint8_t>(authTokenAsVector)));
    *aidl_return = static_cast<int32_t>(ResponseCode::NO_ERROR);
    return Status::ok();
}

bool isDeviceIdAttestationRequested(const KeymasterArguments& params) {
    const hardware::hidl_vec<KeyParameter>& paramsVec = params.getParameters();
    for (size_t i = 0; i < paramsVec.size(); ++i) {
        switch (paramsVec[i].tag) {
        case Tag::ATTESTATION_ID_BRAND:
        case Tag::ATTESTATION_ID_DEVICE:
        case Tag::ATTESTATION_ID_MANUFACTURER:
        case Tag::ATTESTATION_ID_MODEL:
        case Tag::ATTESTATION_ID_PRODUCT:
        case Tag::ATTESTATION_ID_IMEI:
        case Tag::ATTESTATION_ID_MEID:
        case Tag::ATTESTATION_ID_SERIAL:
            return true;
        default:
            continue;
        }
    }
    return false;
}

Status KeyStoreService::attestKey(
    const ::android::sp<::android::security::keystore::IKeystoreCertificateChainCallback>& cb,
    const String16& name, const KeymasterArguments& params, int32_t* _aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    // check null output if method signature is updated and return ErrorCode::OUTPUT_PARAMETER_NULL
    if (!checkAllowedOperationParams(params.getParameters())) {
        return AIDL_RETURN(ErrorCode::INVALID_ARGUMENT);
    }

    uid_t callingUid = IPCThreadState::self()->getCallingUid();

    if (isDeviceIdAttestationRequested(params) && (get_app_id(callingUid) != AID_SYSTEM)) {
        return AIDL_RETURN(KeyStoreServiceReturnCode(ErrorCode::INVALID_ARGUMENT));
    }

    AuthorizationSet mutableParams = params.getParameters();
    KeyStoreServiceReturnCode rc = updateParamsForAttestation(callingUid, &mutableParams);
    if (!rc.isOk()) {
        return AIDL_RETURN(rc);
    }

    String8 name8(name);
    Blob keyBlob;
    Blob charBlob;
    LockedKeyBlobEntry lockedEntry;

    std::tie(rc, keyBlob, charBlob, lockedEntry) =
        mKeyStore->getKeyForName(name8, callingUid, TYPE_KEYMASTER_10);

    if (!rc.isOk()) {
        return AIDL_RETURN(rc);
    }

    auto dev = mKeyStore->getDevice(keyBlob);
    auto hidlKey = blob2hidlVec(keyBlob);
    dev->attestKey(
        std::move(hidlKey), mutableParams.hidl_data(),
        [dev, cb](Return<void> rc,
                  std::tuple<ErrorCode, hidl_vec<hidl_vec<uint8_t>>>&& hidlResult) {
            auto& [ret, certChain] = hidlResult;
            if (!rc.isOk()) {
                cb->onFinished(KeyStoreServiceReturnCode(ResponseCode::SYSTEM_ERROR), {});
            } else if (ret != ErrorCode::OK) {
                dev->logIfKeymasterVendorError(ret);
                cb->onFinished(KeyStoreServiceReturnCode(ret), {});
            } else {
                cb->onFinished(KeyStoreServiceReturnCode(ret),
                               KeymasterCertificateChain(std::move(certChain)));
            }
        });

    return AIDL_RETURN(ResponseCode::NO_ERROR);
}

// My IDE defines "CAPTURE_MOVE(x) x" because it does not understand generalized lambda captures.
// It should never be redefined by a build system though.
#ifndef CAPTURE_MOVE
#define CAPTURE_MOVE(x) x = std::move(x)
#endif

Status KeyStoreService::attestDeviceIds(
    const ::android::sp<::android::security::keystore::IKeystoreCertificateChainCallback>& cb,
    const KeymasterArguments& params, int32_t* _aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    // check null output if method signature is updated and return ErrorCode::OUTPUT_PARAMETER_NULL

    if (!checkAllowedOperationParams(params.getParameters())) {
        return AIDL_RETURN(ErrorCode::INVALID_ARGUMENT);
    }

    if (!isDeviceIdAttestationRequested(params)) {
        // There is an attestKey() method for attesting keys without device ID attestation.
        return AIDL_RETURN(ErrorCode::INVALID_ARGUMENT);
    }

    uid_t callingUid = IPCThreadState::self()->getCallingUid();
    sp<IBinder> binder = defaultServiceManager()->getService(String16("permission"));
    if (binder == nullptr) {
        return AIDL_RETURN(ErrorCode::CANNOT_ATTEST_IDS);
    }
    if (!interface_cast<IPermissionController>(binder)->checkPermission(
            String16("android.permission.READ_PRIVILEGED_PHONE_STATE"),
            IPCThreadState::self()->getCallingPid(), callingUid)) {
        return AIDL_RETURN(ErrorCode::CANNOT_ATTEST_IDS);
    }

    AuthorizationSet mutableParams = params.getParameters();
    KeyStoreServiceReturnCode rc = updateParamsForAttestation(callingUid, &mutableParams);
    if (!rc.isOk()) {
        return AIDL_RETURN(rc);
    }

    // Generate temporary key.
    auto dev = mKeyStore->getDevice(SecurityLevel::TRUSTED_ENVIRONMENT);

    if (!dev) {
        return AIDL_RETURN(ResponseCode::SYSTEM_ERROR);
    }


    AuthorizationSet keyCharacteristics;
    keyCharacteristics.push_back(TAG_PURPOSE, KeyPurpose::VERIFY);
    keyCharacteristics.push_back(TAG_ALGORITHM, Algorithm::EC);
    keyCharacteristics.push_back(TAG_DIGEST, Digest::SHA_2_256);
    keyCharacteristics.push_back(TAG_NO_AUTH_REQUIRED);
    keyCharacteristics.push_back(TAG_EC_CURVE, EcCurve::P_256);

    std::promise<KeyStoreServiceReturnCode> resultPromise;
    auto resultFuture = resultPromise.get_future();

    dev->generateKey(
        keyCharacteristics.hidl_data(),
        [cb, dev, CAPTURE_MOVE(mutableParams)](
            Return<void> rc,
            std::tuple<ErrorCode, ::std::vector<uint8_t>, KeyCharacteristics>&& hidlResult) {
            auto& [ret, hidlKeyBlob_, dummyCharacteristics] = hidlResult;
            auto hidlKeyBlob = std::move(hidlKeyBlob_);
            if (!rc.isOk()) {
                cb->onFinished(KeyStoreServiceReturnCode(ResponseCode::SYSTEM_ERROR), {});
                return;
            }
            if (ret != ErrorCode::OK) {
                dev->logIfKeymasterVendorError(ret);
                cb->onFinished(KeyStoreServiceReturnCode(ret), {});
                return;
            }
            dev->attestKey(
                hidlKeyBlob, mutableParams.hidl_data(),
                [cb, dev,
                 hidlKeyBlob](Return<void> rc,
                              std::tuple<ErrorCode, hidl_vec<hidl_vec<uint8_t>>>&& hidlResult) {
                    auto& [ret, certChain] = hidlResult;
                    // schedule temp key for deletion
                    dev->deleteKey(std::move(hidlKeyBlob), [dev](Return<ErrorCode> rc) {
                        // log error but don't return an error
                        KS_HANDLE_HIDL_ERROR(dev, rc);
                    });
                    if (!rc.isOk()) {
                        cb->onFinished(KeyStoreServiceReturnCode(ResponseCode::SYSTEM_ERROR), {});
                        return;
                    }
                    if (ret == ErrorCode::OK) {
                        cb->onFinished(
                            KeyStoreServiceReturnCode(ret),
                            ::android::security::keymaster::KeymasterCertificateChain(certChain));
                    } else {
                        dev->logIfKeymasterVendorError(ret);
                        cb->onFinished(KeyStoreServiceReturnCode(ret), {});
                    }
                });
        });

    return AIDL_RETURN(ResponseCode::NO_ERROR);
}

Status KeyStoreService::onDeviceOffBody(int32_t* aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    // TODO(tuckeris): add permission check.  This should be callable from ClockworkHome only.
    mKeyStore->getAuthTokenTable().onDeviceOffBody();
    *aidl_return = static_cast<int32_t>(ResponseCode::NO_ERROR);
    return Status::ok();
}

Status KeyStoreService::importWrappedKey(
    const ::android::sp<::android::security::keystore::IKeystoreKeyCharacteristicsCallback>& cb,
    const ::android::String16& wrappedKeyAlias, const ::std::vector<uint8_t>& wrappedKey,
    const ::android::String16& wrappingKeyAlias, const ::std::vector<uint8_t>& maskingKey,
    const KeymasterArguments& params, int64_t rootSid, int64_t fingerprintSid,
    int32_t* _aidl_return) {
    KEYSTORE_SERVICE_LOCK;

    uid_t callingUid = IPCThreadState::self()->getCallingUid();

    if (!checkBinderPermission(P_INSERT, callingUid)) {
        return AIDL_RETURN(ResponseCode::PERMISSION_DENIED);
    }

    String8 wrappingKeyName8(wrappingKeyAlias);

    KeyStoreServiceReturnCode rc;
    Blob wrappingKeyBlob;
    Blob wrappingCharBlob;
    LockedKeyBlobEntry wrappingLockedEntry;

    std::tie(rc, wrappingKeyBlob, wrappingCharBlob, wrappingLockedEntry) =
        mKeyStore->getKeyForName(wrappingKeyName8, callingUid, TYPE_KEYMASTER_10);
    if (!rc.isOk()) {
        return AIDL_RETURN(rc);
    }

    String8 wrappedKeyName8(wrappedKeyAlias);
    auto wrappedLockedEntry =
        mKeyStore->getLockedBlobEntryIfNotExists(wrappedKeyName8.string(), callingUid);
    if (!wrappedLockedEntry) {
        return AIDL_RETURN(ResponseCode::KEY_ALREADY_EXISTS);
    }

    SecurityLevel securityLevel = wrappingKeyBlob.getSecurityLevel();
    auto dev = mKeyStore->getDevice(securityLevel);
    if (!dev) {
        return AIDL_RETURN(ErrorCode::HARDWARE_TYPE_UNAVAILABLE);
    }

    dev->importWrappedKey(
        std::move(wrappingLockedEntry), std::move(wrappedLockedEntry), wrappedKey, maskingKey,
        params.getParameters(), std::move(wrappingKeyBlob), std::move(wrappingCharBlob), rootSid,
        fingerprintSid, [cb](KeyStoreServiceReturnCode rc, KeyCharacteristics keyCharacteristics) {
            cb->onFinished(rc,
                           ::android::security::keymaster::KeyCharacteristics(keyCharacteristics));
        });

    return AIDL_RETURN(ResponseCode::NO_ERROR);
}

Status KeyStoreService::presentConfirmationPrompt(const sp<IBinder>& listener,
                                                  const String16& promptText,
                                                  const ::std::vector<uint8_t>& extraData,
                                                  const String16& locale, int32_t uiOptionsAsFlags,
                                                  int32_t* aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    return mKeyStore->getConfirmationManager().presentConfirmationPrompt(
        listener, promptText, extraData, locale, uiOptionsAsFlags, aidl_return);
}

Status KeyStoreService::cancelConfirmationPrompt(const sp<IBinder>& listener,
                                                 int32_t* aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    return mKeyStore->getConfirmationManager().cancelConfirmationPrompt(listener, aidl_return);
}

Status KeyStoreService::isConfirmationPromptSupported(bool* aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    return mKeyStore->getConfirmationManager().isConfirmationPromptSupported(aidl_return);
}

/**
 * Get the effective target uid for a binder operation that takes an
 * optional uid as the target.
 */
uid_t KeyStoreService::getEffectiveUid(int32_t targetUid) {
    if (targetUid == UID_SELF) {
        return IPCThreadState::self()->getCallingUid();
    }
    return static_cast<uid_t>(targetUid);
}

/**
 * Check if the caller of the current binder method has the required
 * permission and if acting on other uids the grants to do so.
 */
bool KeyStoreService::checkBinderPermission(perm_t permission, int32_t targetUid) {
    uid_t callingUid = IPCThreadState::self()->getCallingUid();
    pid_t spid = IPCThreadState::self()->getCallingPid();
    const char* ssid = IPCThreadState::self()->getCallingSid();
    if (!has_permission(callingUid, permission, spid, ssid)) {
        ALOGW("permission %s denied for %d", get_perm_label(permission), callingUid);
        return false;
    }
    if (!is_granted_to(callingUid, getEffectiveUid(targetUid))) {
        ALOGW("uid %d not granted to act for %d", callingUid, targetUid);
        return false;
    }
    return true;
}

/**
 * Check if the caller of the current binder method has the required
 * permission and the target uid is the caller or the caller is system.
 */
bool KeyStoreService::checkBinderPermissionSelfOrSystem(perm_t permission, int32_t targetUid) {
    uid_t callingUid = IPCThreadState::self()->getCallingUid();
    pid_t spid = IPCThreadState::self()->getCallingPid();
    const char* ssid = IPCThreadState::self()->getCallingSid();
    if (!has_permission(callingUid, permission, spid, ssid)) {
        ALOGW("permission %s denied for %d", get_perm_label(permission), callingUid);
        return false;
    }
    return getEffectiveUid(targetUid) == callingUid || callingUid == AID_SYSTEM;
}

/**
 * Check if the caller of the current binder method has the required
 * permission or the target of the operation is the caller's uid. This is
 * for operation where the permission is only for cross-uid activity and all
 * uids are allowed to act on their own (ie: clearing all entries for a
 * given uid).
 */
bool KeyStoreService::checkBinderPermissionOrSelfTarget(perm_t permission, int32_t targetUid) {
    uid_t callingUid = IPCThreadState::self()->getCallingUid();
    if (getEffectiveUid(targetUid) == callingUid) {
        return true;
    } else {
        return checkBinderPermission(permission, targetUid);
    }
}

/**
 * Helper method to check that the caller has the required permission as
 * well as the keystore is in the unlocked state if checkUnlocked is true.
 *
 * Returns NO_ERROR on success, PERMISSION_DENIED on a permission error and
 * otherwise the state of keystore when not unlocked and checkUnlocked is
 * true.
 */
KeyStoreServiceReturnCode
KeyStoreService::checkBinderPermissionAndKeystoreState(perm_t permission, int32_t targetUid,
                                                       bool checkUnlocked) {
    if (!checkBinderPermission(permission, targetUid)) {
        return ResponseCode::PERMISSION_DENIED;
    }
    State state = mKeyStore->getState(get_user_id(getEffectiveUid(targetUid)));
    if (checkUnlocked && !isKeystoreUnlocked(state)) {
        // All State values coincide with ResponseCodes
        return static_cast<ResponseCode>(state);
    }

    return ResponseCode::NO_ERROR;
}

bool KeyStoreService::isKeystoreUnlocked(State state) {
    switch (state) {
    case ::STATE_NO_ERROR:
        return true;
    case ::STATE_UNINITIALIZED:
    case ::STATE_LOCKED:
        return false;
    }
    return false;
}

/**
 * Check that all KeyParameters provided by the application are allowed. Any parameter that keystore
 * adds itself should be disallowed here.
 */
bool KeyStoreService::checkAllowedOperationParams(const hidl_vec<KeyParameter>& params) {
    for (size_t i = 0; i < params.size(); ++i) {
        switch (params[i].tag) {
        case Tag::ATTESTATION_APPLICATION_ID:
        case Tag::RESET_SINCE_ID_ROTATION:
            return false;
        default:
            break;
        }
    }
    return true;
}

Status KeyStoreService::onKeyguardVisibilityChanged(bool isShowing, int32_t userId,
                                                    int32_t* aidl_return) {
    KEYSTORE_SERVICE_LOCK;
    mKeyStore->getEnforcementPolicy().set_device_locked(isShowing, userId);
    *aidl_return = static_cast<int32_t>(ResponseCode::NO_ERROR);

    return Status::ok();
}

}  // namespace keystore
