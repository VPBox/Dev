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

#ifndef _NETD_TEST_UNSOLSERVICE_H_
#define _NETD_TEST_UNSOLSERVICE_H_

#include <string>
#include <vector>

#include <binder/BinderService.h>

#include "android/net/BnNetdUnsolicitedEventListener.h"

enum UnsolEvent : uint32_t {
    InterfaceClassActivity = 1 << 0,
    QuotaLimitReached = 1 << 1,
    InterfaceDnsServersAdded = 1 << 2,
    InterfaceAddressUpdated = 1 << 3,
    InterfaceAddressRemoved = 1 << 4,
    InterfaceAdded = 1 << 5,
    InterfaceRemoved = 1 << 6,
    InterfaceChanged = 1 << 7,
    InterfaceLinkStatusChanged = 1 << 8,
    RouteChanged = 1 << 9,
    StrictCleartextDetected = 1 << 10,
};

namespace android {
namespace net {

class TestUnsolService : public BinderService<TestUnsolService>,
                         public BnNetdUnsolicitedEventListener {
  public:
    TestUnsolService() = default;
    ~TestUnsolService() = default;
    static TestUnsolService* start();
    static char const* getServiceName() { return "testUnsol"; }
    const std::vector<std::string>& getEvents() const { return events_; }
    void clearEvents() { events_.clear(); }
    uint32_t getReceived() { return received_; }
    std::condition_variable& getCv() { return cv_; }
    std::mutex& getCvMutex() { return cv_mutex_; }
    binder::Status onInterfaceClassActivityChanged(bool isActive, int label, int64_t timestamp,
                                                   int uid) override;
    binder::Status onQuotaLimitReached(const std::string& alertName,
                                       const std::string& ifName) override;
    binder::Status onInterfaceDnsServerInfo(const std::string& ifName, int64_t lifetime,
                                            const std::vector<std::string>& servers) override;
    binder::Status onInterfaceAddressUpdated(const std::string& addr, const std::string& ifName,
                                             int flags, int scope) override;
    binder::Status onInterfaceAddressRemoved(const std::string& addr, const std::string& ifName,
                                             int flags, int scope) override;
    binder::Status onInterfaceAdded(const std::string& ifName) override;
    binder::Status onInterfaceRemoved(const std::string& ifName) override;
    binder::Status onInterfaceChanged(const std::string& ifName, bool status) override;
    binder::Status onInterfaceLinkStateChanged(const std::string& ifName, bool status) override;
    binder::Status onRouteChanged(bool updated, const std::string& route,
                                  const std::string& gateway, const std::string& ifName) override;
    binder::Status onStrictCleartextDetected(int uid, const std::string& hex) override;

    std::vector<std::string> tarVec;

  private:
    void maybeNotify();
    void checkTarget(const std::string& ifName, uint32_t flag);

    std::vector<std::string> events_;
    std::mutex cv_mutex_;
    std::condition_variable cv_;
    uint32_t received_{};
};

}  // namespace net
}  // namespace android

#endif  // _NETD_TEST_UNSOLSERVICE_H_
