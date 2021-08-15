/**
 * Copyright (c) 2018, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "TestUnsolService"

#include <cinttypes>
#include <vector>

#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <log/log.h>
#include <utils/Errors.h>
#include <utils/String16.h>

#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include "android/net/BnNetdUnsolicitedEventListener.h"

#include "TestUnsolService.h"

using android::base::StringPrintf;

namespace android {
namespace net {

TestUnsolService* TestUnsolService::start() {
    IPCThreadState::self()->disableBackgroundScheduling(true);
    sp<IServiceManager> sm(defaultServiceManager());

    auto service = new TestUnsolService();
    const status_t ret = sm->addService(String16(getServiceName()), service, false,
                                        IServiceManager::DUMP_FLAG_PRIORITY_DEFAULT);
    if (ret != android::OK) {
        return nullptr;
    }
    sp<ProcessState> ps(ProcessState::self());
    ps->startThreadPool();
    ps->giveThreadPoolName();

    return service;
}

namespace {

bool containsSubstring(const std::vector<std::string>& vec, const std::string& subStr) {
    return std::any_of(vec.begin(), vec.end(), [&subStr](const std::string& str) {
        return (str.find(subStr) != std::string::npos);
    });
}

}  // namespace

void TestUnsolService::checkTarget(const std::string& ifName, uint32_t flag) {
    if (containsSubstring(tarVec, ifName)) {
        received_ |= flag;
        maybeNotify();
    };
}

void TestUnsolService::maybeNotify() {
    // We only have test case for below event currently.
    if (received_ == (InterfaceAddressUpdated | InterfaceAdded | InterfaceRemoved |
                      InterfaceLinkStatusChanged | RouteChanged)) {
        std::lock_guard lock(cv_mutex_);
        cv_.notify_one();
    }
}

binder::Status TestUnsolService::onInterfaceClassActivityChanged(bool isActive, int label,
                                                                 int64_t timestamp, int uid) {
    events_.push_back(StringPrintf("onInterfaceClassActivityChanged %d %d %" PRId64 "%d", isActive,
                                   label, timestamp, uid));
    return binder::Status::ok();
}

binder::Status TestUnsolService::onQuotaLimitReached(const std::string& alertName,
                                                     const std::string& ifName) {
    events_.push_back(StringPrintf("onQuotaLimitReached %s %s", alertName.c_str(), ifName.c_str()));
    return binder::Status::ok();
}

binder::Status TestUnsolService::onInterfaceDnsServerInfo(const std::string& ifName,
                                                          int64_t lifetime,
                                                          const std::vector<std::string>& servers) {
    events_.push_back(StringPrintf("onInterfaceDnsServerInfo %s %" PRId64 "%s", ifName.c_str(),
                                   lifetime, base::Join(servers, ", ").c_str()));
    return binder::Status::ok();
}

binder::Status TestUnsolService::onInterfaceAddressUpdated(const std::string&,
                                                           const std::string& ifName, int, int) {
    checkTarget(ifName, InterfaceAddressUpdated);
    return binder::Status::ok();
}

binder::Status TestUnsolService::onInterfaceAddressRemoved(const std::string& addr,
                                                           const std::string& ifName, int flags,
                                                           int scope) {
    events_.push_back(StringPrintf("onInterfaceAddressRemoved %s %s %d %d", addr.c_str(),
                                   ifName.c_str(), flags, scope));
    return binder::Status::ok();
}

binder::Status TestUnsolService::onInterfaceAdded(const std::string& ifName) {
    checkTarget(ifName, InterfaceAdded);
    return binder::Status::ok();
}

binder::Status TestUnsolService::onInterfaceRemoved(const std::string& ifName) {
    checkTarget(ifName, InterfaceRemoved);
    return binder::Status::ok();
}

binder::Status TestUnsolService::onInterfaceChanged(const std::string& ifName, bool status) {
    events_.push_back(StringPrintf("onInterfaceChanged %s %d", ifName.c_str(), status));
    return binder::Status::ok();
}

binder::Status TestUnsolService::onInterfaceLinkStateChanged(const std::string& ifName, bool) {
    checkTarget(ifName, InterfaceLinkStatusChanged);
    return binder::Status::ok();
}

binder::Status TestUnsolService::onRouteChanged(bool, const std::string&, const std::string&,
                                                const std::string& ifName) {
    checkTarget(ifName, RouteChanged);
    return binder::Status::ok();
}

binder::Status TestUnsolService::onStrictCleartextDetected(int uid, const std::string& hex) {
    events_.push_back(StringPrintf("onStrictCleartextDetected %d %s", uid, hex.c_str()));
    return binder::Status::ok();
}

}  // namespace net
}  // namespace android