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

#ifndef KEYSTORE_CONFIRMATION_MANAGER_H_
#define KEYSTORE_CONFIRMATION_MANAGER_H_

#include <android/hardware/confirmationui/1.0/IConfirmationUI.h>
#include <android/hardware/confirmationui/1.0/types.h>
#include <binder/Binder.h>
#include <binder/IBinder.h>
#include <binder/Status.h>
#include <keystore/keymaster_types.h>
#include <map>
#include <mutex>
#include <utils/LruCache.h>
#include <utils/StrongPointer.h>
#include <vector>

#include "confirmationui_rate_limiting.h"

namespace keystore {

using android::binder::Status;
using android::hardware::confirmationui::V1_0::IConfirmationResultCallback;
using ConfirmationResponseCode = android::hardware::confirmationui::V1_0::ResponseCode;

class ConfirmationManager;

class ConfirmationManager : public android::hardware::hidl_death_recipient,
                            public IConfirmationResultCallback {
  public:
    explicit ConfirmationManager(android::IBinder::DeathRecipient* deathRecipient);

    // Calls into the confirmationui HAL to start a new prompt.
    //
    // Returns OperationPending if another application is already
    // showing a confirmation. Otherwise returns the return code from
    // the HAL.
    Status presentConfirmationPrompt(const android::sp<android::IBinder>& listener,
                                     const android::String16& promptText,
                                     const hidl_vec<uint8_t>& extraData,
                                     const android::String16& locale, int uiOptionsAsFlags,
                                     int32_t* aidl_return);

    // Calls into the confirmationui HAL to cancel displaying a
    // prompt.
    //
    // Returns OperatingPending if another application is showing a
    // confirmation. Otherwise returns the return code from the HAL.
    Status cancelConfirmationPrompt(const android::sp<android::IBinder>& listener,
                                    int32_t* aidl_return);

    // Checks if the confirmationUI HAL is available.
    Status isConfirmationPromptSupported(bool* aidl_return);

    // Gets the latest confirmation token received from the ConfirmationUI HAL.
    hidl_vec<uint8_t> getLatestConfirmationToken();

    // Called by KeyStoreService when a client binder has died.
    void binderDied(const android::wp<android::IBinder>& who);

    // hidl_death_recipient overrides:
    virtual void serviceDied(uint64_t cookie,
                             const android::wp<android::hidl::base::V1_0::IBase>& who) override;

    // IConfirmationResultCallback overrides:
    android::hardware::Return<void> result(ConfirmationResponseCode responseCode,
                                           const hidl_vec<uint8_t>& dataThatWasConfirmed,
                                           const hidl_vec<uint8_t>& confirmationToken) override;

  private:
    friend class ConfirmationResultCallback;

    void finalizeTransaction(ConfirmationResponseCode responseCode,
                             hidl_vec<uint8_t> dataThatWasConfirmed, bool callAbortOnHal);

    // This mutex protects all data below it.
    std::mutex mMutex;

    // The mCurrentListener and mCurrentConfirmationUI fields are set
    // if and only if a prompt is currently showing.
    android::sp<android::IBinder> mCurrentListener;
    android::sp<android::hardware::confirmationui::V1_0::IConfirmationUI> mCurrentConfirmationUI;
    android::IBinder::DeathRecipient* mDeathRecipient;
    hidl_vec<uint8_t> mLatestConfirmationToken;
    RateLimiting<> mRateLimiting;
};

}  // namespace keystore

#endif  // KEYSTORE_CONFIRMATION_MANAGER_H_
