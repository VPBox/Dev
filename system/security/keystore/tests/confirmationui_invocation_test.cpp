/*
**
** Copyright 2019, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <android/hardware/confirmationui/1.0/types.h>
#include <android/security/BnConfirmationPromptCallback.h>
#include <android/security/keystore/IKeystoreService.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>

#include <gtest/gtest.h>

#include <chrono>
#include <future>
#include <tuple>
#include <vector>

using ConfirmationResponseCode = android::hardware::confirmationui::V1_0::ResponseCode;
using android::IBinder;
using android::IServiceManager;
using android::sp;
using android::String16;
using android::security::keystore::IKeystoreService;

using namespace std::literals::chrono_literals;

class ConfirmationListener
    : public android::security::BnConfirmationPromptCallback,
      public std::promise<std::tuple<ConfirmationResponseCode, std::vector<uint8_t>>> {
  public:
    ConfirmationListener() {}

    virtual ::android::binder::Status
    onConfirmationPromptCompleted(int32_t result,
                                  const ::std::vector<uint8_t>& dataThatWasConfirmed) override {
        this->set_value({static_cast<ConfirmationResponseCode>(result), dataThatWasConfirmed});
        return ::android::binder::Status::ok();
    }
};

TEST(ConfirmationInvocationTest, InvokeAndCancel) {
    android::ProcessState::self()->startThreadPool();

    sp<IServiceManager> sm = android::initdefaultServiceManager();
    sp<IBinder> binder = sm->getService(String16("android.security.keystore"));
    sp<IKeystoreService> service = android::interface_cast<IKeystoreService>(binder);
    ASSERT_TRUE(service);

    String16 promptText16("Just a little test!");
    String16 locale16("en");
    std::vector<uint8_t> extraData{0xaa, 0xff, 0x00, 0x55};

    sp<ConfirmationListener> listener = new ConfirmationListener();

    auto future = listener->get_future();
    int32_t aidl_return;

    android::binder::Status status = service->presentConfirmationPrompt(
        listener, promptText16, extraData, locale16, 0, &aidl_return);
    ASSERT_TRUE(status.isOk()) << "Presenting confirmation prompt failed with binder status '"
                               << status.toString8().c_str() << "'.\n";
    ConfirmationResponseCode responseCode = static_cast<ConfirmationResponseCode>(aidl_return);
    ASSERT_EQ(responseCode, ConfirmationResponseCode::OK)
        << "Presenting confirmation prompt failed with response code " << aidl_return << ".\n";

    auto fstatus = future.wait_for(2s);
    EXPECT_EQ(fstatus, std::future_status::timeout);

    status = service->cancelConfirmationPrompt(listener, &aidl_return);
    ASSERT_TRUE(status.isOk());

    responseCode = static_cast<ConfirmationResponseCode>(aidl_return);
    ASSERT_EQ(responseCode, ConfirmationResponseCode::OK);

    future.wait();
    auto [rc, dataThatWasConfirmed] = future.get();

    ASSERT_EQ(rc, ConfirmationResponseCode::Aborted);
}
