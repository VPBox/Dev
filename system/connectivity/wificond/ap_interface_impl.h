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

#ifndef WIFICOND_AP_INTERFACE_IMPL_H_
#define WIFICOND_AP_INTERFACE_IMPL_H_

#include <array>
#include <string>

#include <linux/if_ether.h>

#include <android-base/macros.h>
#include <wifi_system/interface_tool.h>

#include "wificond/net/netlink_manager.h"

#include "android/net/wifi/IApInterface.h"

namespace android {
namespace wificond {

class ApInterfaceBinder;
class NetlinkUtils;

// Holds the guts of how we control network interfaces capable of exposing an AP
// via hostapd.  Because remote processes may hold on to the corresponding
// binder object past the lifetime of the local object, we are forced to
// keep this object separate from the binder representation of itself.
class ApInterfaceImpl {
 public:
  ApInterfaceImpl(const std::string& interface_name,
                  uint32_t interface_index,
                  NetlinkUtils* netlink_utils,
                  wifi_system::InterfaceTool* if_tool);
  ~ApInterfaceImpl();

  // Get a pointer to the binder representing this ApInterfaceImpl.
  android::sp<android::net::wifi::IApInterface> GetBinder() const;

  std::string GetInterfaceName() { return interface_name_; }
  int GetNumberOfAssociatedStations() const;
  void Dump(std::stringstream* ss) const;

 private:
  const std::string interface_name_;
  const uint32_t interface_index_;
  NetlinkUtils* const netlink_utils_;
  wifi_system::InterfaceTool* const if_tool_;
  const android::sp<ApInterfaceBinder> binder_;

  // Number of associated stations.
  int number_of_associated_stations_;

  void OnStationEvent(StationEvent event,
                      const std::array<uint8_t, ETH_ALEN>& mac_address);

  void OnChannelSwitchEvent(uint32_t frequency, ChannelBandwidth bandwidth);

  DISALLOW_COPY_AND_ASSIGN(ApInterfaceImpl);
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_AP_INTERFACE_IMPL_H_
