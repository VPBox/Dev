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

#ifndef WAKEUP_CONTROLLER_H
#define WAKEUP_CONTROLLER_H

#include <functional>

#include <netdutils/Status.h>

#include "IptablesRestoreController.h"
#include "NFLogListener.h"

namespace android {
namespace net {

class WakeupController {
  public:

    // Simple data struct for passing back packet wakeup event information to the ReportFn callback.
    struct ReportArgs {
        std::string prefix;
        uint64_t timestampNs;
        int uid;
        int gid;
        int ethertype;
        int ipNextHeader;
        std::vector<uint8_t> dstHw;
        std::string srcIp;
        std::string dstIp;
        int srcPort;
        int dstPort;
    };

    // Callback that is triggered for every wakeup event.
    using ReportFn = std::function<void(const struct ReportArgs&)>;

    // iptables chain where wakeup packets are matched
    static const char LOCAL_MANGLE_INPUT[];

    static const uint32_t kDefaultPacketCopyRange;

    WakeupController(ReportFn report, IptablesRestoreInterface* iptables)
        : mReport(report), mIptables(iptables) {}

    ~WakeupController();

    // Subscribe this controller to a NFLOG events arriving at |listener|.
    netdutils::Status init(NFLogListenerInterface* listener);

    // Install iptables rules to match packets arriving on |ifName|
    // which match |mark|/|mask|. Metadata from matching packets will
    // be delivered along with the arbitrary string |prefix| to
    // INetdEventListener::onWakeupEvent.
    netdutils::Status addInterface(const std::string& ifName, const std::string& prefix,
                                   uint32_t mark, uint32_t mask);

    // Remove iptables rules previously installed by addInterface().
    // |ifName|, |prefix|, |mark| and |mask| must match precisely.
    netdutils::Status delInterface(const std::string& ifName, const std::string& prefix,
                                   uint32_t mark, uint32_t mask);

  private:
    netdutils::Status execIptables(const std::string& action, const std::string& ifName,
                                   const std::string& prefix, uint32_t mark, uint32_t mask);

    ReportFn const mReport;
    IptablesRestoreInterface* const mIptables;
    NFLogListenerInterface* mListener;
};

}  // namespace net
}  // namespace android

#endif /* WAKEUP_CONTROLLER_H */
