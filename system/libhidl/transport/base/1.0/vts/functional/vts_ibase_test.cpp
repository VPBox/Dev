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

#include <functional>
#include <map>
#include <string>

#include <android/hidl/base/1.0/IBase.h>
#include <android/hidl/manager/1.0/IServiceManager.h>
#include <gtest/gtest.h>
#include <hidl/HidlBinderSupport.h>
#include <hidl/ServiceManagement.h>

using android::hardware::hidl_array;
using android::hardware::hidl_death_recipient;
using android::hardware::hidl_handle;
using android::hardware::hidl_string;
using android::hardware::hidl_vec;
using android::hardware::IBinder;
using android::hardware::toBinder;
using android::hidl::base::V1_0::IBase;
using android::hidl::manager::V1_0::IServiceManager;
using android::sp;
using android::wp;

template <typename T>
static inline ::testing::AssertionResult isOk(const ::android::hardware::Return<T>& ret) {
    return ret.isOk() ? (::testing::AssertionSuccess() << ret.description())
                      : (::testing::AssertionFailure() << ret.description());
}
#define ASSERT_OK(__ret__) ASSERT_TRUE(isOk(__ret__))
#define EXPECT_OK(__ret__) EXPECT_TRUE(isOk(__ret__))

struct Hal {
    sp<IBase> service;
    std::string name;  // space separated list of android.hidl.foo@1.0::IFoo/instance-name
};

class VtsHalBaseV1_0TargetTest : public ::testing::Test {
   public:
    virtual void SetUp() override {
        default_manager_ = ::android::hardware::defaultServiceManager();

        ASSERT_NE(default_manager_, nullptr)
            << "Failed to get default service manager." << std::endl;

        ASSERT_OK(default_manager_->list([&](const auto& list) {
            for (const auto& name : list) {
                const std::string strName = name;
                auto loc = strName.find_first_of('/');
                if (loc == std::string::npos) {
                    ADD_FAILURE() << "Invalid FQName: " << strName;
                    continue;
                }
                const std::string fqName = strName.substr(0, loc);
                const std::string instance = strName.substr(loc + 1);

                sp<IBase> service = default_manager_->get(fqName, instance);
                if (service == nullptr) {
                    ADD_FAILURE() << "Null service for " << name << " " << fqName << " "
                                  << instance;
                    continue;
                }

                sp<IBinder> binder = toBinder(service);
                if (binder == nullptr) {
                    ADD_FAILURE() << "Null binder for " << name;
                    continue;
                }

                auto iter = all_hals_.find(binder);
                if (iter != all_hals_.end()) {
                    // include all the names this is registered as for error messages
                    iter->second.name += " " + strName;
                } else {
                    all_hals_.insert(iter, {binder, Hal{service, strName}});
                }
            }
        }));

        ASSERT_FALSE(all_hals_.empty());  // sanity
    }

    void EachHal(const std::function<void(const Hal&)>& check) {
        for (auto iter = all_hals_.begin(); iter != all_hals_.end(); ++iter) {
            check(iter->second);
        }
    }

    // default service manager
    sp<IServiceManager> default_manager_;

    // map from underlying instance to actual instance
    //
    // this prevents calling the same service twice since the same service
    // will get registered multiple times for its entire inheritance
    // hierarchy (or perhaps as different instance names)
    std::map<sp<IBinder>, Hal> all_hals_;
};

TEST_F(VtsHalBaseV1_0TargetTest, CanPing) {
    EachHal(
        [&](const Hal& base) { EXPECT_OK(base.service->ping()) << "Cannot ping " << base.name; });
}

TEST_F(VtsHalBaseV1_0TargetTest, InterfaceChain) {
    EachHal([&](const Hal& base) {
        EXPECT_OK(base.service->interfaceChain([&](const auto& interfaceChain) {
            // must include IBase + subclasses
            EXPECT_GT(interfaceChain.size(), 1u) << "Invalid instance name " << base.name;
        })) << base.name;
    });
}

TEST_F(VtsHalBaseV1_0TargetTest, Descriptor) {
    EachHal([&](const Hal& base) {
        EXPECT_OK(base.service->interfaceDescriptor([&](const auto& descriptor) {
            // must include IBase + subclasses
            EXPECT_GT(descriptor.size(), 0u) << base.name;
            EXPECT_NE(IBase::descriptor, descriptor) << base.name;
        })) << base.name;
    });
}

TEST_F(VtsHalBaseV1_0TargetTest, Death) {
    struct HidlDeathRecipient : hidl_death_recipient {
        virtual void serviceDied(uint64_t /* cookie */, const wp<IBase>& /* who */){};
    };
    sp<hidl_death_recipient> recipient = new HidlDeathRecipient;

    EachHal([&](const Hal& base) {
        EXPECT_OK(base.service->linkToDeath(recipient, 0 /* cookie */))
            << "Register death recipient " << base.name;
        EXPECT_OK(base.service->unlinkToDeath(recipient)) << "Unlink death recipient " << base.name;
    });
}

TEST_F(VtsHalBaseV1_0TargetTest, Debug) {
    EachHal([&](const Hal& base) {
        // normally one is passed, but this is tested by dumpstate
        EXPECT_OK(base.service->debug(hidl_handle(), {}))
            << "Handle empty debug handle " << base.name;
    });
}

TEST_F(VtsHalBaseV1_0TargetTest, HashChain) {
    EachHal([&](const Hal& base) {
        EXPECT_OK(base.service->getHashChain([&](const auto& hashChain) {
            // must include IBase + subclasses
            EXPECT_NE(0u, hashChain.size()) << "Invalid hash chain " << base.name;
        })) << base.name;
    });
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
