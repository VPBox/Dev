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

#define LOG_TAG "ConfirmationManager"

#include "confirmation_manager.h"

#include <android/hardware/confirmationui/1.0/IConfirmationResultCallback.h>
#include <android/hardware/confirmationui/1.0/IConfirmationUI.h>
#include <android/hardware/confirmationui/1.0/types.h>
#include <android/security/BpConfirmationPromptCallback.h>
#include <binder/BpBinder.h>
#include <binder/IPCThreadState.h>
#include <binder/Parcel.h>

#include "keystore_aidl_hidl_marshalling_utils.h"

namespace keystore {

using android::IBinder;
using android::sp;
using android::String16;
using android::String8;
using android::wp;
using android::binder::Status;
using android::hardware::hidl_vec;
using android::hardware::Return;
using android::hardware::confirmationui::V1_0::IConfirmationResultCallback;
using android::hardware::confirmationui::V1_0::IConfirmationUI;
using android::hardware::confirmationui::V1_0::UIOption;

using android::security::BpConfirmationPromptCallback;
using std::lock_guard;
using std::mutex;
using std::vector;

ConfirmationManager::ConfirmationManager(IBinder::DeathRecipient* deathRecipient)
    : IConfirmationResultCallback(), mDeathRecipient(deathRecipient) {}

// Called by keystore main thread.
Status ConfirmationManager::presentConfirmationPrompt(const sp<IBinder>& listener,
                                                      const String16& promptText,
                                                      const hidl_vec<uint8_t>& extraData,
                                                      const String16& locale, int uiOptionsAsFlags,
                                                      int32_t* aidl_return) {
    lock_guard<mutex> lock(mMutex);

    if (mCurrentListener != nullptr) {
        *aidl_return = static_cast<int32_t>(ConfirmationResponseCode::OperationPending);
        return Status::ok();
    }

    sp<IConfirmationUI> confirmationUI = IConfirmationUI::tryGetService();
    if (confirmationUI == nullptr) {
        ALOGW("Error getting confirmationUI service\n");
        *aidl_return = static_cast<int32_t>(ConfirmationResponseCode::Unimplemented);
        return Status::ok();
    }

    uid_t callingUid = android::IPCThreadState::self()->getCallingUid();
    if (!mRateLimiting.tryPrompt(callingUid)) {
        *aidl_return = static_cast<int32_t>(ConfirmationResponseCode::SystemError);
        return Status::ok();
    }

    String8 promptText8(promptText);
    String8 locale8(locale);
    vector<UIOption> uiOptionsVector;
    for (int n = 0; n < 32; n++) {
        if (uiOptionsAsFlags & (1 << n)) {
            uiOptionsVector.push_back(UIOption(n));
        }
    }
    ConfirmationResponseCode responseCode;
    responseCode = confirmationUI->promptUserConfirmation(sp<IConfirmationResultCallback>(this),
                                                          promptText8.string(), extraData,
                                                          locale8.string(), uiOptionsVector);
    if (responseCode != ConfirmationResponseCode::OK) {
        ALOGW("Unexpecxted responseCode %d from promptUserConfirmation\n", responseCode);
        *aidl_return = static_cast<int32_t>(responseCode);
        return Status::ok();
    }

    listener->linkToDeath(mDeathRecipient);
    confirmationUI->linkToDeath(this, 0);
    mCurrentListener = listener;
    mCurrentConfirmationUI = confirmationUI;

    *aidl_return = static_cast<int32_t>(ConfirmationResponseCode::OK);
    return Status::ok();
}

// Called by keystore main thread.
Status ConfirmationManager::cancelConfirmationPrompt(const sp<IBinder>& listener,
                                                     int32_t* aidl_return) {
    mMutex.lock();
    if (mCurrentListener != listener) {
        // If the prompt was displayed by another application, return
        // OperationPending.
        mMutex.unlock();
        *aidl_return = static_cast<int32_t>(ConfirmationResponseCode::OperationPending);
        return Status::ok();
    }
    mMutex.unlock();

    finalizeTransaction(ConfirmationResponseCode::Aborted, {}, true);

    *aidl_return = static_cast<int32_t>(ConfirmationResponseCode::OK);
    return Status::ok();
}

// Called by keystore main thread.
Status ConfirmationManager::isConfirmationPromptSupported(bool* aidl_return) {
    sp<IConfirmationUI> confirmationUI = IConfirmationUI::tryGetService();
    if (confirmationUI == nullptr) {
        ALOGW("Error getting confirmationUI service\n");
        *aidl_return = false;
        return Status::ok();
    }

    *aidl_return = true;
    return Status::ok();
}

void ConfirmationManager::finalizeTransaction(ConfirmationResponseCode responseCode,
                                              hidl_vec<uint8_t> dataThatWasConfirmed,
                                              bool callAbortOnHal) {
    // Note that confirmationUI->abort() may make the remote HAL process do an IPC call back
    // into our process resulting in confirmationResultCallback() to be called... this in turn
    // calls finalizeTransaction(). So we have to be careful a) not holding any locks;
    // and b) ensure state has been cleared; before doing this...

    mMutex.lock();
    mRateLimiting.processResult(responseCode);
    sp<IBinder> listener = mCurrentListener;
    if (mCurrentListener != nullptr) {
        mCurrentListener->unlinkToDeath(mDeathRecipient);
        mCurrentListener = nullptr;
    }
    sp<IConfirmationUI> confirmationUI = mCurrentConfirmationUI;
    if (mCurrentConfirmationUI != nullptr) {
        mCurrentConfirmationUI->unlinkToDeath(this);
        mCurrentConfirmationUI = nullptr;
    }
    mMutex.unlock();

    // Tell the HAL to shut down the confirmation dialog, if requested.
    if (confirmationUI != nullptr && callAbortOnHal) {
        confirmationUI->abort();
    }

    // Deliver result to the application that started the operation.
    if (listener != nullptr) {
        sp<BpConfirmationPromptCallback> obj = new BpConfirmationPromptCallback(listener);
        Status status = obj->onConfirmationPromptCompleted(static_cast<int32_t>(responseCode),
                                                           dataThatWasConfirmed);
        if (!status.isOk()) {
            ALOGW("Error sending onConfirmationPromptCompleted - status: %s\n",
                  status.toString8().c_str());
        }
    }
}

// Called by hwbinder thread (not keystore main thread).
Return<void> ConfirmationManager::result(ConfirmationResponseCode responseCode,
                                         const hidl_vec<uint8_t>& dataThatWasConfirmed,
                                         const hidl_vec<uint8_t>& confirmationToken) {
    finalizeTransaction(responseCode, dataThatWasConfirmed, false);
    lock_guard<mutex> lock(mMutex);
    mLatestConfirmationToken = confirmationToken;
    return Return<void>();
}

// Called by keystore main thread or keymaster worker
hidl_vec<uint8_t> ConfirmationManager::getLatestConfirmationToken() {
    lock_guard<mutex> lock(mMutex);
    return mLatestConfirmationToken;
}

void ConfirmationManager::binderDied(const wp<IBinder>& who) {
    // This is also called for other binders so need to check it's for
    // us before acting on it.
    mMutex.lock();
    if (who == mCurrentListener) {
        // Clear this so we don't call back into the already dead
        // binder in finalizeTransaction().
        mCurrentListener->unlinkToDeath(mDeathRecipient);
        mCurrentListener = nullptr;
        mMutex.unlock();
        ALOGW("The process which requested the confirmation dialog died.\n");
        finalizeTransaction(ConfirmationResponseCode::SystemError, {}, true);
    } else {
        mMutex.unlock();
    }
}

void ConfirmationManager::serviceDied(uint64_t /* cookie */,
                                      const wp<android::hidl::base::V1_0::IBase>& /* who */) {
    ALOGW("The ConfirmationUI HAL died.\n");
    finalizeTransaction(ConfirmationResponseCode::SystemError, {}, false);
}

}  // namespace keystore
