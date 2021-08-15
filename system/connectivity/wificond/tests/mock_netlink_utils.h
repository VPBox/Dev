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

#ifndef WIFICOND_TEST_MOCK_NETLINK_UTILS_H_
#define WIFICOND_TEST_MOCK_NETLINK_UTILS_H_

#include <gmock/gmock.h>

#include "wificond/net/netlink_utils.h"

namespace android {
namespace wificond {

class MockNetlinkUtils : public NetlinkUtils {
 public:
  MockNetlinkUtils(NetlinkManager* netlink_manager);
  ~MockNetlinkUtils() override = default;

  MOCK_METHOD1(GetWiphyIndex, bool(uint32_t* out_wiphy_index));
  MOCK_METHOD2(GetWiphyIndex,
               bool(uint32_t* out_wiphy_index, const std::string& iface_name));
  MOCK_METHOD1(UnsubscribeMlmeEvent, void(uint32_t interface_index));
  MOCK_METHOD1(UnsubscribeRegDomainChange, void(uint32_t wiphy_index));
  MOCK_METHOD1(UnsubscribeStationEvent, void(uint32_t interface_index));
  MOCK_METHOD1(UnsubscribeChannelSwitchEvent, void(uint32_t interface_index));
  MOCK_METHOD1(UnsubscribeFrameTxStatusEvent, void(uint32_t interface_index));
  MOCK_METHOD1(GetProtocolFeatures, bool(uint32_t* features));

  MOCK_METHOD2(SetInterfaceMode,
               bool(uint32_t interface_index, InterfaceMode mode));
  MOCK_METHOD2(SubscribeMlmeEvent,
               void(uint32_t interface_index,
                    MlmeEventHandler* handler));
  MOCK_METHOD2(SubscribeRegDomainChange,
               void(uint32_t wiphy_index,
                    OnRegDomainChangedHandler handler));
  MOCK_METHOD2(SubscribeStationEvent,
               void(uint32_t interface_index,
                    OnStationEventHandler handler));
  MOCK_METHOD2(SubscribeChannelSwitchEvent,
               void(uint32_t interface_index,
                    OnChannelSwitchEventHandler handler));
  MOCK_METHOD2(SubscribeFrameTxStatusEvent,
               void(uint32_t interface_index,
                    OnFrameTxStatusEventHandler handler));

  MOCK_METHOD2(GetInterfaces,
               bool(uint32_t wiphy_index,
                    std::vector<InterfaceInfo>* interfaces));
  MOCK_METHOD4(GetWiphyInfo,
               bool(uint32_t wiphy_index,
                    BandInfo* band_info,
                    ScanCapabilities* scan_capabilities,
                    WiphyFeatures* wiphy_features));
  MOCK_METHOD4(SendMgmtFrame,
               bool(uint32_t interface_index,
                    const std::vector<uint8_t>& frame,
                    int32_t mcs,
                    uint64_t* out_cookie));

};  // class MockNetlinkUtils

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_TEST_MOCK_NETLINK_UTILS_H
