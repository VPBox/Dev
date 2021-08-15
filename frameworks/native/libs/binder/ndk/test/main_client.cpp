/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <android-base/logging.h>
#include <android/binder_ibinder_jni.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <gtest/gtest.h>
#include <iface/iface.h>

#include <chrono>
#include <condition_variable>
#include <mutex>

using ::android::sp;

constexpr char kExistingNonNdkService[] = "SurfaceFlinger";

// This is too slow
// TEST(NdkBinder, GetServiceThatDoesntExist) {
//     sp<IFoo> foo = IFoo::getService("asdfghkl;");
//     EXPECT_EQ(nullptr, foo.get());
// }

TEST(NdkBinder, CheckServiceThatDoesntExist) {
    AIBinder* binder = AServiceManager_checkService("asdfghkl;");
    ASSERT_EQ(nullptr, binder);
}

TEST(NdkBinder, CheckServiceThatDoesExist) {
    AIBinder* binder = AServiceManager_checkService(kExistingNonNdkService);
    EXPECT_NE(nullptr, binder);
    EXPECT_EQ(STATUS_OK, AIBinder_ping(binder));

    AIBinder_decStrong(binder);
}

TEST(NdkBinder, DoubleNumber) {
    sp<IFoo> foo = IFoo::getService(IFoo::kSomeInstanceName);
    ASSERT_NE(foo, nullptr);

    int32_t out;
    EXPECT_EQ(STATUS_OK, foo->doubleNumber(1, &out));
    EXPECT_EQ(2, out);
}

void LambdaOnDeath(void* cookie) {
    auto onDeath = static_cast<std::function<void(void)>*>(cookie);
    (*onDeath)();
};
TEST(NdkBinder, DeathRecipient) {
    using namespace std::chrono_literals;

    AIBinder* binder;
    sp<IFoo> foo = IFoo::getService(IFoo::kInstanceNameToDieFor, &binder);
    ASSERT_NE(nullptr, foo.get());
    ASSERT_NE(nullptr, binder);

    std::mutex deathMutex;
    std::condition_variable deathCv;
    bool deathRecieved = false;

    std::function<void(void)> onDeath = [&] {
        std::cerr << "Binder died (as requested)." << std::endl;
        deathRecieved = true;
        deathCv.notify_one();
    };

    AIBinder_DeathRecipient* recipient = AIBinder_DeathRecipient_new(LambdaOnDeath);

    EXPECT_EQ(STATUS_OK, AIBinder_linkToDeath(binder, recipient, static_cast<void*>(&onDeath)));

    // the binder driver should return this if the service dies during the transaction
    EXPECT_EQ(STATUS_DEAD_OBJECT, foo->die());

    foo = nullptr;
    AIBinder_decStrong(binder);
    binder = nullptr;

    std::unique_lock<std::mutex> lock(deathMutex);
    EXPECT_TRUE(deathCv.wait_for(lock, 1s, [&] { return deathRecieved; }));
    EXPECT_TRUE(deathRecieved);

    AIBinder_DeathRecipient_delete(recipient);
}

TEST(NdkBinder, RetrieveNonNdkService) {
    AIBinder* binder = AServiceManager_getService(kExistingNonNdkService);
    ASSERT_NE(nullptr, binder);
    EXPECT_TRUE(AIBinder_isRemote(binder));
    EXPECT_TRUE(AIBinder_isAlive(binder));
    EXPECT_EQ(STATUS_OK, AIBinder_ping(binder));

    AIBinder_decStrong(binder);
}

void OnBinderDeath(void* cookie) {
    LOG(ERROR) << "BINDER DIED. COOKIE: " << cookie;
}

TEST(NdkBinder, LinkToDeath) {
    AIBinder* binder = AServiceManager_getService(kExistingNonNdkService);
    ASSERT_NE(nullptr, binder);

    AIBinder_DeathRecipient* recipient = AIBinder_DeathRecipient_new(OnBinderDeath);
    ASSERT_NE(nullptr, recipient);

    EXPECT_EQ(STATUS_OK, AIBinder_linkToDeath(binder, recipient, nullptr));
    EXPECT_EQ(STATUS_OK, AIBinder_linkToDeath(binder, recipient, nullptr));
    EXPECT_EQ(STATUS_OK, AIBinder_unlinkToDeath(binder, recipient, nullptr));
    EXPECT_EQ(STATUS_OK, AIBinder_unlinkToDeath(binder, recipient, nullptr));
    EXPECT_EQ(STATUS_NAME_NOT_FOUND, AIBinder_unlinkToDeath(binder, recipient, nullptr));

    AIBinder_DeathRecipient_delete(recipient);
    AIBinder_decStrong(binder);
}

class MyTestFoo : public IFoo {
    binder_status_t doubleNumber(int32_t in, int32_t* out) override {
        *out = 2 * in;
        LOG(INFO) << "doubleNumber (" << in << ") => " << *out;
        return STATUS_OK;
    }
    binder_status_t die() override {
        ADD_FAILURE() << "die called on local instance";
        return STATUS_OK;
    }
};

TEST(NdkBinder, GetServiceInProcess) {
    static const char* kInstanceName = "test-get-service-in-process";

    sp<IFoo> foo = new MyTestFoo;
    EXPECT_EQ(STATUS_OK, foo->addService(kInstanceName));

    sp<IFoo> getFoo = IFoo::getService(kInstanceName);
    EXPECT_EQ(foo.get(), getFoo.get());

    int32_t out;
    EXPECT_EQ(STATUS_OK, getFoo->doubleNumber(1, &out));
    EXPECT_EQ(2, out);
}

TEST(NdkBinder, EqualityOfRemoteBinderPointer) {
    AIBinder* binderA = AServiceManager_getService(kExistingNonNdkService);
    ASSERT_NE(nullptr, binderA);

    AIBinder* binderB = AServiceManager_getService(kExistingNonNdkService);
    ASSERT_NE(nullptr, binderB);

    EXPECT_EQ(binderA, binderB);

    AIBinder_decStrong(binderA);
    AIBinder_decStrong(binderB);
}

TEST(NdkBinder, ToFromJavaNullptr) {
    EXPECT_EQ(nullptr, AIBinder_toJavaBinder(nullptr, nullptr));
    EXPECT_EQ(nullptr, AIBinder_fromJavaBinder(nullptr, nullptr));
}

TEST(NdkBinder, ABpBinderRefCount) {
    AIBinder* binder = AServiceManager_getService(kExistingNonNdkService);
    AIBinder_Weak* wBinder = AIBinder_Weak_new(binder);

    ASSERT_NE(nullptr, binder);
    EXPECT_EQ(1, AIBinder_debugGetRefCount(binder));

    AIBinder_decStrong(binder);

    // assert because would need to decStrong if non-null and we shouldn't need to add a no-op here
    ASSERT_NE(nullptr, AIBinder_Weak_promote(wBinder));

    AIBinder_Weak_delete(wBinder);
}

TEST(NdkBinder, AddServiceMultipleTimes) {
    static const char* kInstanceName1 = "test-multi-1";
    static const char* kInstanceName2 = "test-multi-2";
    sp<IFoo> foo = new MyTestFoo;
    EXPECT_EQ(STATUS_OK, foo->addService(kInstanceName1));
    EXPECT_EQ(STATUS_OK, foo->addService(kInstanceName2));
    EXPECT_EQ(IFoo::getService(kInstanceName1), IFoo::getService(kInstanceName2));
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);

    ABinderProcess_setThreadPoolMaxThreadCount(1);  // to recieve death notifications/callbacks
    ABinderProcess_startThreadPool();

    return RUN_ALL_TESTS();
}

#include <android/binder_auto_utils.h>
#include <android/binder_interface_utils.h>
#include <android/binder_parcel_utils.h>
