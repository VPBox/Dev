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

#ifndef _NETLINKHANDLER_H
#define _NETLINKHANDLER_H

#include <string>
#include <vector>

#include <sysutils/NetlinkEvent.h>
// TODO: stop depending on sysutils/NetlinkListener.h
#include <sysutils/NetlinkListener.h>
#include "NetlinkManager.h"

namespace android {
namespace net {

class NetlinkHandler : public ::NetlinkListener {
    NetlinkManager *mNm;

public:
    NetlinkHandler(NetlinkManager *nm, int listenerSocket, int format);
    virtual ~NetlinkHandler();

    int start();
    int stop();

  protected:
    virtual void onEvent(NetlinkEvent *evt);

    void notifyInterfaceAdded(const std::string& ifName);
    void notifyInterfaceRemoved(const std::string& ifName);
    void notifyInterfaceChanged(const std::string& ifName, bool isUp);
    void notifyInterfaceLinkChanged(const std::string& ifName, bool isUp);
    void notifyQuotaLimitReached(const std::string& labelName, const std::string& ifName);
    void notifyInterfaceClassActivityChanged(int label, bool isActive, int64_t timestamp, int uid);
    void notifyAddressUpdated(const std::string& addr, const std::string& ifName, int flags,
                              int scope);
    void notifyAddressRemoved(const std::string& addr, const std::string& ifName, int flags,
                              int scope);
    void notifyInterfaceDnsServers(const std::string& ifName, int64_t lifetime,
                                   const std::vector<std::string>& servers);
    void notifyRouteChange(bool updated, const std::string& route, const std::string& gateway,
                           const std::string& ifName);
    void notifyStrictCleartext(uid_t uid, const std::string& hex);
};

}  // namespace net
}  // namespace android

#endif
