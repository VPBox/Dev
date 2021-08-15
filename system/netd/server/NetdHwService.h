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

#ifndef ANDROID_NET_HW_SERVICE_H
#define ANDROID_NET_HW_SERVICE_H

#include <android/system/net/netd/1.1/INetd.h>

namespace android {
namespace net {

using android::hardware::Return;
using android::hardware::hidl_string;
using INetdHw = android::system::net::netd::V1_1::INetd;
using StatusCode = android::system::net::netd::V1_1::INetd::StatusCode;

class NetdHwService : INetdHw {
  public:
    // 1.0
    status_t start();
    Return<void> createOemNetwork(createOemNetwork_cb _hidl_cb) override;
    Return<StatusCode> destroyOemNetwork(uint64_t netHandle) override;

    // 1.1
    Return <StatusCode> addRouteToOemNetwork(
            uint64_t networkHandle, const hidl_string& ifname, const hidl_string& destination,
            const hidl_string& nexthop) override;
    Return <StatusCode> removeRouteFromOemNetwork(
            uint64_t networkHandle, const hidl_string& ifname, const hidl_string& destination,
            const hidl_string& nexthop) override;
    Return <StatusCode> addInterfaceToOemNetwork(uint64_t networkHandle,
                                                 const hidl_string& ifname) override;
    Return <StatusCode> removeInterfaceFromOemNetwork(uint64_t networkHandle,
                                                      const hidl_string& ifname) override;
    Return <StatusCode> setIpForwardEnable(bool enable) override;
    Return <StatusCode> setForwardingBetweenInterfaces(const hidl_string& inputIfName,
                                                       const hidl_string& outputIfName,
                                                       bool enable) override;
};

}  // namespace net
}  // namespace android

#endif  // ANDROID_NET_HW_SERVICE_H

