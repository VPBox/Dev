/*
 * Copyright 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * tun_interface.h - creates tun interfaces for testing purposes
 */

#ifndef _SYSTEM_NETD_TESTS_TUN_INTERACE_H
#define _SYSTEM_NETD_TESTS_TUN_INTERACE_H

namespace android {
namespace net {

class TunInterface {
public:
    TunInterface() = default;
    ~TunInterface() { destroy(); }

    // Creates a tun interface. Returns 0 on success or -errno on failure. Must succeed before it is
    // legal to call any of the other methods in this class.
    int init(const std::string& ifName = "");
    void destroy();

    const std::string& name() const { return mIfName; }
    int ifindex() const { return mIfIndex; }
    const in6_addr& srcAddr() const { return mSrcAddr; }
    const in6_addr& dstAddr() const { return mDstAddr; }

    int addAddress(const std::string& addr, int prefixlen);
    int getFdForTesting() const { return mFd; }

  private:
    int mFd = -1;
    std::string mIfName;
    int mIfIndex;
    in6_addr mSrcAddr, mDstAddr;
};

}  // namespace net
}  // namespace android

#endif
