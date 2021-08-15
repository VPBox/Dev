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

#ifndef KEYSTORE_KEYSTORE_SERVICE_H_
#define KEYSTORE_KEYSTORE_SERVICE_H_

#include <android/security/keystore/BnKeystoreService.h>

#include "auth_token_table.h"
#include "confirmation_manager.h"

#include "KeyStore.h"
#include "keystore_keymaster_enforcement.h"
#include "operation.h"
#include "permissions.h"

#include <keystore/ExportResult.h>
#include <keystore/KeyCharacteristics.h>
#include <keystore/KeymasterArguments.h>
#include <keystore/KeymasterBlob.h>
#include <keystore/KeymasterCertificateChain.h>
#include <keystore/OperationResult.h>
#include <keystore/keystore_return_types.h>

#include <mutex>

namespace keystore {

// Class provides implementation for generated BnKeystoreService.h based on
// gen/aidl/android/security/BnKeystoreService.h generated from
// java/android/security/IKeystoreService.aidl Note that all generated methods return binder::Status
// and use last arguments to send actual result to the caller. Private methods don't need to handle
// binder::Status. Input parameters cannot be null unless annotated with @nullable in .aidl file.
class KeyStoreService : public android::security::keystore::BnKeystoreService {
  public:
    explicit KeyStoreService(sp<KeyStore> keyStore) : mKeyStore(keyStore) {}
    virtual ~KeyStoreService() = default;

    void binderDied(const android::wp<android::IBinder>& who);

    ::android::binder::Status getState(int32_t userId, int32_t* _aidl_return) override;
    ::android::binder::Status get(const ::android::String16& name, int32_t uid,
                                  ::std::vector<uint8_t>* _aidl_return) override;
    ::android::binder::Status insert(const ::android::String16& name,
                                     const ::std::vector<uint8_t>& item, int32_t uid, int32_t flags,
                                     int32_t* _aidl_return) override;
    ::android::binder::Status del(const ::android::String16& name, int32_t uid,
                                  int32_t* _aidl_return) override;
    ::android::binder::Status exist(const ::android::String16& name, int32_t uid,
                                    int32_t* _aidl_return) override;
    ::android::binder::Status list(const ::android::String16& namePrefix, int32_t uid,
                                   ::std::vector<::android::String16>* _aidl_return) override;
    ::android::binder::Status listUidsOfAuthBoundKeys(std::vector<::std::string>* uids,
                                                      int32_t* _aidl_return) override;

    ::android::binder::Status reset(int32_t* _aidl_return) override;
    ::android::binder::Status onUserPasswordChanged(int32_t userId,
                                                    const ::android::String16& newPassword,
                                                    int32_t* _aidl_return) override;
    ::android::binder::Status lock(int32_t userId, int32_t* _aidl_return) override;
    ::android::binder::Status unlock(int32_t userId, const ::android::String16& userPassword,
                                     int32_t* _aidl_return) override;
    ::android::binder::Status isEmpty(int32_t userId, int32_t* _aidl_return) override;
    ::android::binder::Status grant(const ::android::String16& name, int32_t granteeUid,
                                    ::android::String16* _aidl_return) override;
    ::android::binder::Status ungrant(const ::android::String16& name, int32_t granteeUid,
                                      int32_t* _aidl_return) override;
    ::android::binder::Status getmtime(const ::android::String16& name, int32_t uid,
                                       int64_t* _aidl_return) override;
    ::android::binder::Status is_hardware_backed(const ::android::String16& string,
                                                 int32_t* _aidl_return) override;
    ::android::binder::Status clear_uid(int64_t uid, int32_t* _aidl_return) override;
    ::android::binder::Status
    addRngEntropy(const ::android::sp<::android::security::keystore::IKeystoreResponseCallback>& cb,
                  const ::std::vector<uint8_t>& data, int32_t flags,
                  int32_t* _aidl_return) override;
    ::android::binder::Status generateKey(
        const ::android::sp<::android::security::keystore::IKeystoreKeyCharacteristicsCallback>& cb,
        const ::android::String16& alias,
        const ::android::security::keymaster::KeymasterArguments& arguments,
        const ::std::vector<uint8_t>& entropy, int32_t uid, int32_t flags,
        int32_t* _aidl_return) override;
    ::android::binder::Status getKeyCharacteristics(
        const ::android::sp<::android::security::keystore::IKeystoreKeyCharacteristicsCallback>& cb,
        const ::android::String16& alias,
        const ::android::security::keymaster::KeymasterBlob& clientId,
        const ::android::security::keymaster::KeymasterBlob& appId, int32_t uid,
        int32_t* _aidl_return) override;
    ::android::binder::Status importKey(
        const ::android::sp<::android::security::keystore::IKeystoreKeyCharacteristicsCallback>& cb,
        const ::android::String16& alias,
        const ::android::security::keymaster::KeymasterArguments& arguments, int32_t format,
        const ::std::vector<uint8_t>& keyData, int32_t uid, int32_t flags,
        int32_t* _aidl_return) override;
    ::android::binder::Status
    exportKey(const ::android::sp<::android::security::keystore::IKeystoreExportKeyCallback>& cb,
              const ::android::String16& alias, int32_t format,
              const ::android::security::keymaster::KeymasterBlob& clientId,
              const ::android::security::keymaster::KeymasterBlob& appId, int32_t uid,
              int32_t* _aidl_return) override;
    ::android::binder::Status
    begin(const ::android::sp<::android::security::keystore::IKeystoreOperationResultCallback>& cb,
          const ::android::sp<::android::IBinder>& appToken, const ::android::String16& alias,
          int32_t purpose, bool pruneable,
          const ::android::security::keymaster::KeymasterArguments& params,
          const ::std::vector<uint8_t>& entropy, int32_t uid, int32_t* _aidl_return) override;
    ::android::binder::Status
    update(const ::android::sp<::android::security::keystore::IKeystoreOperationResultCallback>& cb,
           const ::android::sp<::android::IBinder>& token,
           const ::android::security::keymaster::KeymasterArguments& params,
           const ::std::vector<uint8_t>& input, int32_t* _aidl_return) override;
    ::android::binder::Status
    finish(const ::android::sp<::android::security::keystore::IKeystoreOperationResultCallback>& cb,
           const ::android::sp<::android::IBinder>& token,
           const ::android::security::keymaster::KeymasterArguments& params,
           const ::std::vector<uint8_t>& signature, const ::std::vector<uint8_t>& entropy,
           int32_t* _aidl_return) override;
    ::android::binder::Status
    abort(const ::android::sp<::android::security::keystore::IKeystoreResponseCallback>& cb,
          const ::android::sp<::android::IBinder>& token, int32_t* _aidl_return) override;
    ::android::binder::Status addAuthToken(const ::std::vector<uint8_t>& authToken,
                                           int32_t* _aidl_return) override;
    ::android::binder::Status onUserAdded(int32_t userId, int32_t parentId,
                                          int32_t* _aidl_return) override;
    ::android::binder::Status onUserRemoved(int32_t userId, int32_t* _aidl_return) override;
    ::android::binder::Status attestKey(
        const ::android::sp<::android::security::keystore::IKeystoreCertificateChainCallback>& cb,
        const ::android::String16& alias,
        const ::android::security::keymaster::KeymasterArguments& params,
        int32_t* _aidl_return) override;
    ::android::binder::Status attestDeviceIds(
        const ::android::sp<::android::security::keystore::IKeystoreCertificateChainCallback>& cb,
        const ::android::security::keymaster::KeymasterArguments& params,
        int32_t* _aidl_return) override;
    ::android::binder::Status onDeviceOffBody(int32_t* _aidl_return) override;

    ::android::binder::Status importWrappedKey(
        const ::android::sp<::android::security::keystore::IKeystoreKeyCharacteristicsCallback>& cb,
        const ::android::String16& wrappedKeyAlias, const ::std::vector<uint8_t>& wrappedKey,
        const ::android::String16& wrappingKeyAlias, const ::std::vector<uint8_t>& maskingKey,
        const ::android::security::keymaster::KeymasterArguments& params, int64_t rootSid,
        int64_t fingerprintSid, int32_t* _aidl_return) override;

    ::android::binder::Status presentConfirmationPrompt(
        const ::android::sp<::android::IBinder>& listener, const ::android::String16& promptText,
        const ::std::vector<uint8_t>& extraData, const ::android::String16& locale,
        int32_t uiOptionsAsFlags, int32_t* _aidl_return) override;
    ::android::binder::Status
    cancelConfirmationPrompt(const ::android::sp<::android::IBinder>& listener,
                             int32_t* _aidl_return) override;
    ::android::binder::Status isConfirmationPromptSupported(bool* _aidl_return) override;

    ::android::binder::Status onKeyguardVisibilityChanged(bool isShowing, int32_t userId,
                                                          int32_t* _aidl_return) override;

  private:
    static const int32_t UID_SELF = -1;

    /**
     * Get the effective target uid for a binder operation that takes an
     * optional uid as the target.
     */
    uid_t getEffectiveUid(int32_t targetUid);

    /**
     * Check if the caller of the current binder method has the required
     * permission and if acting on other uids the grants to do so.
     */
    bool checkBinderPermission(perm_t permission, int32_t targetUid = UID_SELF);

    /**
     * Check if the caller of the current binder method has the required
     * permission and the target uid is the caller or the caller is system.
     */
    bool checkBinderPermissionSelfOrSystem(perm_t permission, int32_t targetUid);

    /**
     * Check if the caller of the current binder method has the required
     * permission or the target of the operation is the caller's uid. This is
     * for operation where the permission is only for cross-uid activity and all
     * uids are allowed to act on their own (ie: clearing all entries for a
     * given uid).
     */
    bool checkBinderPermissionOrSelfTarget(perm_t permission, int32_t targetUid);

    /**
     * Helper method to check that the caller has the required permission as
     * well as the keystore is in the unlocked state if checkUnlocked is true.
     *
     * Returns NO_ERROR on success, PERMISSION_DENIED on a permission error and
     * otherwise the state of keystore when not unlocked and checkUnlocked is
     * true.
     */
    KeyStoreServiceReturnCode checkBinderPermissionAndKeystoreState(perm_t permission,
                                                                    int32_t targetUid = -1,
                                                                    bool checkUnlocked = true);

    bool isKeystoreUnlocked(State state);

    /**
     * Check that all keymaster_key_param_t's provided by the application are
     * allowed. Any parameter that keystore adds itself should be disallowed here.
     */
    bool checkAllowedOperationParams(const hidl_vec<KeyParameter>& params);

    void addLegacyBeginParams(const android::String16& name, AuthorizationSet* params);

    KeyStoreServiceReturnCode doLegacySignVerify(const android::String16& name,
                                                 const hidl_vec<uint8_t>& data,
                                                 hidl_vec<uint8_t>* out,
                                                 const hidl_vec<uint8_t>& signature,
                                                 KeyPurpose purpose);

    /**
     * Adds a Confirmation Token to the key parameters if needed.
     */
    void appendConfirmationTokenIfNeeded(const KeyCharacteristics& keyCharacteristics,
                                         std::vector<KeyParameter>* params);

    sp<KeyStore> mKeyStore;

    /**
     * This mutex locks keystore operations from concurrent execution.
     * The keystore service has always been conceptually single threaded. Even with the introduction
     * of keymaster workers, it was assumed that the dispatcher thread executes exclusively on
     * certain code paths. With the introduction of wifi Keystore service in the keystore process
     * this assumption no longer holds as the hwbinder thread servicing this interface makes
     * functions (rather than IPC) calls into keystore. This mutex protects the keystore logic
     * from concurrent execution.
     */
    std::mutex keystoreServiceMutex_;

};

};  // namespace keystore

#endif  // KEYSTORE_KEYSTORE_SERVICE_H_
