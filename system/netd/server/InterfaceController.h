/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _INTERFACE_CONTROLLER_H
#define _INTERFACE_CONTROLLER_H

#include <functional>
#include <map>
#include <string>

#include <android/net/InterfaceConfigurationParcel.h>
#include <netdutils/Status.h>
#include <netdutils/StatusOr.h>

namespace android {
namespace net {

class StablePrivacyTest;

class InterfaceController {
public:
    static void initializeAll();

    static int setEnableIPv6(const char* ifName, const int on);
    static android::netdutils::Status setIPv6AddrGenMode(const std::string& ifName, int mode);
    static int setAcceptIPv6Ra(const char* ifName, const int on);
    static int setAcceptIPv6Dad(const char* ifName, const int on);
    static int setIPv6DadTransmits(const char* ifName, const char* value);
    static int setIPv6PrivacyExtensions(const char* ifName, const int on);
    static int setMtu(const char* ifName, const char* mtu);
    static int addAddress(const char* ifName, const char* addrString, int prefixLength);
    static int delAddress(const char* ifName, const char* addrString, int prefixLength);
    static int disableIcmpRedirects();
    static android::netdutils::Status setCfg(const InterfaceConfigurationParcel& cfg);
    static android::netdutils::StatusOr<InterfaceConfigurationParcel> getCfg(
            const std::string& ifName);
    static int clearAddrs(const std::string& ifName);

    // Read and write values in files of the form:
    //     /proc/sys/net/<family>/<which>/<ifName>/<parameter>
    //
    // NOTE: getParameter() trims whitespace so the caller does not need extra
    // code to crop trailing newlines, for example.
    static int getParameter(const char* family, const char* which, const char* ifName,
                            const char* parameter, std::string* value);
    static int setParameter(const char* family, const char* which, const char* ifName,
                            const char* parameter, const char* value);

    static android::netdutils::StatusOr<std::vector<std::string>> getIfaceNames();
    static android::netdutils::StatusOr<std::map<std::string, uint32_t>> getIfaceList();

    static std::mutex mutex;

  private:
    friend class android::net::StablePrivacyTest;

    using GetPropertyFn =
            std::function<std::string(const std::string& key, const std::string& dflt)>;
    using SetPropertyFn = std::function<android::netdutils::Status(const std::string& key,
                                                                   const std::string& val)>;

    // Helper function exported from this compilation unit for testing.
    static android::netdutils::Status enableStablePrivacyAddresses(
            const std::string& ifName, const GetPropertyFn& getProperty,
            const SetPropertyFn& setProperty);

    static void setAcceptRA(const char* value);
    static void setAcceptRARouteTable(int tableOrOffset);
    static void setBaseReachableTimeMs(unsigned int millis);
    static void setIPv6OptimisticMode(const char* value);

    InterfaceController() = delete;
    ~InterfaceController() = delete;
};

}  // namespace net
}  // namespace android

#endif
