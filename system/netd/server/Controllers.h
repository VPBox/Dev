/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef _CONTROLLERS_H__
#define _CONTROLLERS_H__

#include "BandwidthController.h"
#include "ClatdController.h"
#include "EventReporter.h"
#include "FirewallController.h"
#include "IdletimerController.h"
#include "InterfaceController.h"
#include "IptablesRestoreController.h"
#include "NetworkController.h"
#include "PppController.h"
#include "StrictController.h"
#include "TcpSocketMonitor.h"
#include "TetherController.h"
#include "TrafficController.h"
#include "WakeupController.h"
#include "XfrmController.h"
#include "netdutils/Log.h"

namespace android {
namespace net {

class Controllers {
  public:
    Controllers();

    NetworkController netCtrl;
    TetherController tetherCtrl;
    PppController pppCtrl;
    BandwidthController bandwidthCtrl;
    IdletimerController idletimerCtrl;
    FirewallController firewallCtrl;
    ClatdController clatdCtrl;
    StrictController strictCtrl;
    EventReporter eventReporter;
    IptablesRestoreController iptablesRestoreCtrl;
    WakeupController wakeupCtrl;
    XfrmController xfrmCtrl;
    TrafficController trafficCtrl;
    TcpSocketMonitor tcpSocketMonitor;

    void init();

  private:
    friend class ControllersTest;
    void initIptablesRules();
    static void initChildChains();
    static std::set<std::string> findExistingChildChains(const IptablesTarget target,
                                                         const char* table,
                                                         const char* parentChain);
    static void createChildChains(IptablesTarget target, const char* table, const char* parentChain,
                                  const std::vector<const char*>& childChains, bool exclusive);
    static int (*execIptablesRestore)(IptablesTarget, const std::string&);
    static int (*execIptablesRestoreWithOutput)(IptablesTarget, const std::string&, std::string *);
};

extern netdutils::Log gLog;
extern netdutils::Log gUnsolicitedLog;
extern Controllers* gCtls;

}  // namespace net
}  // namespace android

#endif  // _CONTROLLERS_H__
