/*
 * Copyright (C) 2008 The Android Open Source Project
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

#ifndef _CLATD_CONTROLLER_H
#define _CLATD_CONTROLLER_H

#include <map>
#include <mutex>
#include <string>

#include <linux/if.h>
#include <netinet/in.h>

#include <android-base/thread_annotations.h>

#include "Fwmark.h"
#include "NetdConstants.h"
#include "bpf/BpfMap.h"
#include "netdbpf/bpf_shared.h"
#include "netdutils/DumpWriter.h"

namespace android {
namespace net {

class NetworkController;

class ClatdController {
  public:
    explicit ClatdController(NetworkController* controller) EXCLUDES(mutex)
        : mNetCtrl(controller){};
    virtual ~ClatdController() EXCLUDES(mutex){};

    /* First thing init/startClatd/stopClatd/dump do is grab the mutex. */
    void init(void) EXCLUDES(mutex);

    int startClatd(const std::string& interface, const std::string& nat64Prefix,
                   std::string* v6Addr) EXCLUDES(mutex);
    int stopClatd(const std::string& interface) EXCLUDES(mutex);

    void dump(netdutils::DumpWriter& dw) EXCLUDES(mutex);

    static constexpr const char LOCAL_RAW_PREROUTING[] = "clat_raw_PREROUTING";

  private:
    struct ClatdTracker {
        pid_t pid = -1;
        unsigned ifIndex;
        char iface[IFNAMSIZ];
        unsigned v4ifIndex;
        char v4iface[IFNAMSIZ];
        Fwmark fwmark;
        char fwmarkString[UINT32_STRLEN];
        unsigned netId;
        char netIdString[UINT32_STRLEN];
        in_addr v4;
        char v4Str[INET_ADDRSTRLEN];
        in6_addr v6;
        char v6Str[INET6_ADDRSTRLEN];
        in6_addr pfx96;
        char pfx96String[INET6_ADDRSTRLEN];

        int init(unsigned networkId, const std::string& interface, const std::string& v4interface,
                 const std::string& nat64Prefix);
    };

    std::mutex mutex;

    const NetworkController* mNetCtrl GUARDED_BY(mutex);
    std::map<std::string, ClatdTracker> mClatdTrackers GUARDED_BY(mutex);
    ClatdTracker* getClatdTracker(const std::string& interface) REQUIRES(mutex);

    static in_addr_t selectIpv4Address(const in_addr ip, int16_t prefixlen);
    static int generateIpv6Address(const char* iface, const in_addr v4, const in6_addr& nat64Prefix,
                                   in6_addr* v6);
    static void makeChecksumNeutral(in6_addr* v6, const in_addr v4, const in6_addr& nat64Prefix);

    enum eClatEbpfMode {
        ClatEbpfDisabled,  //  <4.9 kernel ||  <P api shipping level -- will not work
        ClatEbpfMaybe,     // >=4.9 kernel &&   P api shipping level -- might work
        ClatEbpfEnabled,   // >=4.9 kernel && >=Q api shipping level -- must work
    };
    eClatEbpfMode mClatEbpfMode GUARDED_BY(mutex);
    eClatEbpfMode getEbpfMode() EXCLUDES(mutex) {
        std::lock_guard guard(mutex);
        return mClatEbpfMode;
    }

    base::unique_fd mNetlinkFd GUARDED_BY(mutex);
    bpf::BpfMap<ClatIngressKey, ClatIngressValue> mClatIngressMap GUARDED_BY(mutex);

    void maybeStartBpf(const ClatdTracker& tracker) REQUIRES(mutex);
    void maybeStopBpf(const ClatdTracker& tracker) REQUIRES(mutex);
    void maybeSetIptablesDropRule(bool add, const char* pfx96Str, const char* v6Str)
            REQUIRES(mutex);

    // For testing.
    friend class ClatdControllerTest;

    static bool (*isIpv4AddressFreeFunc)(in_addr_t);
    static bool isIpv4AddressFree(in_addr_t addr);
    static int (*iptablesRestoreFunction)(IptablesTarget target, const std::string& commands);
};

}  // namespace net
}  // namespace android

#endif
