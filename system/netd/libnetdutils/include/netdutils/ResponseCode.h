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

#ifndef NETDUTILS_RESPONSECODE_H
#define NETDUTILS_RESPONSECODE_H

namespace android {
namespace netdutils {

class ResponseCode {
    // Keep in sync with
    // frameworks/base/services/java/com/android/server/NetworkManagementService.java
  public:
    // 100 series - Requestion action was initiated; expect another reply
    // before proceeding with a new command.
    // clang-format off
    static constexpr int ActionInitiated                = 100;
    static constexpr int InterfaceListResult            = 110;
    static constexpr int TetherInterfaceListResult      = 111;
    static constexpr int TetherDnsFwdTgtListResult      = 112;
    static constexpr int TtyListResult                  = 113;
    static constexpr int TetheringStatsListResult       = 114;
    static constexpr int TetherDnsFwdNetIdResult        = 115;

    // 200 series - Requested action has been successfully completed
    static constexpr int CommandOkay                    = 200;
    static constexpr int TetherStatusResult             = 210;
    static constexpr int IpFwdStatusResult              = 211;
    static constexpr int InterfaceGetCfgResult          = 213;
    // Formerly: int SoftapStatusResult                 = 214;
    static constexpr int UsbRNDISStatusResult           = 215;
    static constexpr int InterfaceRxCounterResult       = 216;
    static constexpr int InterfaceTxCounterResult       = 217;
    static constexpr int InterfaceRxThrottleResult      = 218;
    static constexpr int InterfaceTxThrottleResult      = 219;
    static constexpr int QuotaCounterResult             = 220;
    static constexpr int TetheringStatsResult           = 221;
    // NOTE: keep synced with bionic/libc/dns/net/gethnamaddr.c
    static constexpr int DnsProxyQueryResult            = 222;
    static constexpr int ClatdStatusResult              = 223;

    // 400 series - The command was accepted but the requested action
    // did not take place.
    static constexpr int OperationFailed                = 400;
    static constexpr int DnsProxyOperationFailed        = 401;
    static constexpr int ServiceStartFailed             = 402;
    static constexpr int ServiceStopFailed              = 403;

    // 500 series - The command was not accepted and the requested
    // action did not take place.
    static constexpr int CommandSyntaxError             = 500;
    static constexpr int CommandParameterError          = 501;

    // 600 series - Unsolicited broadcasts
    static constexpr int InterfaceChange                = 600;
    static constexpr int BandwidthControl               = 601;
    static constexpr int ServiceDiscoveryFailed         = 602;
    static constexpr int ServiceDiscoveryServiceAdded   = 603;
    static constexpr int ServiceDiscoveryServiceRemoved = 604;
    static constexpr int ServiceRegistrationFailed      = 605;
    static constexpr int ServiceRegistrationSucceeded   = 606;
    static constexpr int ServiceResolveFailed           = 607;
    static constexpr int ServiceResolveSuccess          = 608;
    static constexpr int ServiceSetHostnameFailed       = 609;
    static constexpr int ServiceSetHostnameSuccess      = 610;
    static constexpr int ServiceGetAddrInfoFailed       = 611;
    static constexpr int ServiceGetAddrInfoSuccess      = 612;
    static constexpr int InterfaceClassActivity         = 613;
    static constexpr int InterfaceAddressChange         = 614;
    static constexpr int InterfaceDnsInfo               = 615;
    static constexpr int RouteChange                    = 616;
    static constexpr int StrictCleartext                = 617;
    // clang-format on
};

}  // namespace netdutils
}  // namespace android

#endif  // NETDUTILS_RESPONSECODE_H
