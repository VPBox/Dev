/*
 * Copyright (C) 2012 The Android Open Source Project
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

#ifndef _FIREWALL_CONTROLLER_H
#define _FIREWALL_CONTROLLER_H

#include <sys/types.h>
#include <mutex>
#include <set>
#include <string>
#include <vector>

#include "android/net/INetd.h"

#include "NetdConstants.h"
#include "bpf/BpfUtils.h"

namespace android {
namespace net {

enum FirewallRule { ALLOW = INetd::FIREWALL_RULE_ALLOW, DENY = INetd::FIREWALL_RULE_DENY };

// WHITELIST means the firewall denies all by default, uids must be explicitly ALLOWed
// BLACKLIST means the firewall allows all by default, uids must be explicitly DENYed

enum FirewallType { WHITELIST = INetd::FIREWALL_WHITELIST, BLACKLIST = INetd::FIREWALL_BLACKLIST };

enum ChildChain {
    NONE = INetd::FIREWALL_CHAIN_NONE,
    DOZABLE = INetd::FIREWALL_CHAIN_DOZABLE,
    STANDBY = INetd::FIREWALL_CHAIN_STANDBY,
    POWERSAVE = INetd::FIREWALL_CHAIN_POWERSAVE,
    INVALID_CHAIN
};

/*
 * Simple firewall that drops all packets except those matching explicitly
 * defined ALLOW rules.
 *
 * Methods in this class must be called when holding a write lock on |lock|, and may not call
 * any other controller without explicitly managing that controller's lock. There are currently
 * no such methods.
 */
class FirewallController {
public:
    FirewallController();

    int setupIptablesHooks(void);

    int setFirewallType(FirewallType);
    int resetFirewall(void);
    int isFirewallEnabled(void);

    /* Match traffic going in/out over the given iface. */
    int setInterfaceRule(const char*, FirewallRule);
    /* Match traffic owned by given UID. This is specific to a particular chain. */
    int setUidRule(ChildChain, int, FirewallRule);

    int enableChildChains(ChildChain, bool);

    int replaceUidChain(const std::string&, bool, const std::vector<int32_t>&);

    static std::string makeCriticalCommands(IptablesTarget target, const char* chainName);
    static uid_t discoverMaximumValidUid(const std::string& fileName);

    static const char* TABLE;

    static const char* LOCAL_INPUT;
    static const char* LOCAL_OUTPUT;
    static const char* LOCAL_FORWARD;

    static const char* LOCAL_DOZABLE;
    static const char* LOCAL_STANDBY;
    static const char* LOCAL_POWERSAVE;

    static const char* ICMPV6_TYPES[];

    std::mutex lock;

protected:
    friend class FirewallControllerTest;
    std::string makeUidRules(IptablesTarget target, const char *name, bool isWhitelist,
                             const std::vector<int32_t>& uids);
    static int (*execIptablesRestore)(IptablesTarget target, const std::string& commands);

private:
  // Netd supports two cases, in both of which mMaxUid that derives from the uid mapping is const:
  //  - netd runs in a root namespace which contains all UIDs.
  //  - netd runs in a user namespace where the uid mapping is written once before netd starts.
  //    In that case, an attempt to write more than once to a uid_map file in a user namespace
  //    fails with EPERM. Netd can therefore assumes the max valid uid to be const.
  const uid_t mMaxUid;
  FirewallType mFirewallType;
  android::bpf::BpfLevel mUseBpfOwnerMatch;
  std::set<std::string> mIfaceRules;
  int attachChain(const char*, const char*);
  int detachChain(const char*, const char*);
  int createChain(const char*, FirewallType);
  FirewallType getFirewallType(ChildChain);
};

}  // namespace net
}  // namespace android

#endif
