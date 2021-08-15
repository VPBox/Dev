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

#ifndef WIFICOND_SERVER_H_
#define WIFICOND_SERVER_H_

#include <memory>
#include <string>
#include <vector>

#include <android-base/macros.h>
#include <wifi_system/interface_tool.h>

#include "android/net/wifi/BnWificond.h"
#include "android/net/wifi/IApInterface.h"
#include "android/net/wifi/IClientInterface.h"
#include "android/net/wifi/IInterfaceEventCallback.h"

#include "wificond/ap_interface_impl.h"
#include "wificond/client_interface_impl.h"

namespace android {
namespace wificond {

class NL80211Packet;
class NetlinkUtils;
class ScanUtils;

struct InterfaceInfo;

class Server : public android::net::wifi::BnWificond {
 public:
  Server(std::unique_ptr<wifi_system::InterfaceTool> if_tool,
         NetlinkUtils* netlink_utils,
         ScanUtils* scan_utils);
  ~Server() override = default;

  android::binder::Status RegisterCallback(
      const android::sp<android::net::wifi::IInterfaceEventCallback>&
          callback) override;
  android::binder::Status UnregisterCallback(
      const android::sp<android::net::wifi::IInterfaceEventCallback>&
          callback) override;
  // Returns a vector of available frequencies for 2.4GHz channels.
  android::binder::Status getAvailable2gChannels(
      ::std::unique_ptr<::std::vector<int32_t>>* out_frequencies) override;
  // Returns a vector of available frequencies for 5GHz non-DFS channels.
  android::binder::Status getAvailable5gNonDFSChannels(
      ::std::unique_ptr<::std::vector<int32_t>>* out_frequencies) override;
  // Returns a vector of available frequencies for DFS channels.
  android::binder::Status getAvailableDFSChannels(
      ::std::unique_ptr<::std::vector<int32_t>>* out_frequencies) override;

  android::binder::Status createApInterface(
      const std::string& iface_name,
      android::sp<android::net::wifi::IApInterface>*
          created_interface) override;

  android::binder::Status createClientInterface(
      const std::string& iface_name,
      android::sp<android::net::wifi::IClientInterface>*
          created_interface) override;

  android::binder::Status tearDownApInterface(
      const std::string& iface_name,
      bool* out_success) override;

  android::binder::Status tearDownClientInterface(
      const std::string& iface_name,
      bool* out_success) override;

  android::binder::Status tearDownInterfaces() override;

  android::binder::Status GetClientInterfaces(
      std::vector<android::sp<android::IBinder>>* out_client_ifs) override;
  android::binder::Status GetApInterfaces(
      std::vector<android::sp<android::IBinder>>* out_ap_ifs) override;
  status_t dump(int fd, const Vector<String16>& args) override;

 private:
  // Request interface information from kernel and setup local interface object.
  // This assumes that interface should be in STATION mode. Even if we setup
  // interface on behalf of createApInterace(), it is Hostapd that configure
  // the interface to Ap mode later.
  // Returns true on success, false otherwise.
  bool SetupInterface(const std::string& iface_name, InterfaceInfo* interface);
  bool RefreshWiphyIndex(const std::string& iface_num);
  void LogSupportedBands();
  void OnRegDomainChanged(std::string& country_code);
  void BroadcastClientInterfaceReady(
      android::sp<android::net::wifi::IClientInterface> network_interface);
  void BroadcastApInterfaceReady(
      android::sp<android::net::wifi::IApInterface> network_interface);
  void BroadcastClientInterfaceTornDown(
      android::sp<android::net::wifi::IClientInterface> network_interface);
  void BroadcastApInterfaceTornDown(
      android::sp<android::net::wifi::IApInterface> network_interface);
  void MarkDownAllInterfaces();

  const std::unique_ptr<wifi_system::InterfaceTool> if_tool_;
  NetlinkUtils* const netlink_utils_;
  ScanUtils* const scan_utils_;

  uint32_t wiphy_index_;
  std::map<std::string, std::unique_ptr<ApInterfaceImpl>> ap_interfaces_;
  std::map<std::string, std::unique_ptr<ClientInterfaceImpl>> client_interfaces_;
  std::vector<android::sp<android::net::wifi::IInterfaceEventCallback>>
      interface_event_callbacks_;

  // Cached interface list from kernel.
  std::vector<InterfaceInfo> interfaces_;

  DISALLOW_COPY_AND_ASSIGN(Server);
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_SERVER_H_
