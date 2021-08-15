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

#ifndef WIFICOND_CLIENT_INTERFACE_IMPL_H_
#define WIFICOND_CLIENT_INTERFACE_IMPL_H_

#include <array>
#include <string>

#include <linux/if_ether.h>

#include <android-base/macros.h>
#include <utils/StrongPointer.h>
#include <wifi_system/interface_tool.h>

#include "android/net/wifi/IClientInterface.h"
#include "android/net/wifi/ISendMgmtFrameEvent.h"
#include "wificond/net/mlme_event_handler.h"
#include "wificond/net/netlink_utils.h"
#include "wificond/scanning/offload/offload_service_utils.h"
#include "wificond/scanning/scanner_impl.h"

namespace android {
namespace wificond {

class ClientInterfaceBinder;
class ClientInterfaceImpl;
class ScanUtils;

class MlmeEventHandlerImpl : public MlmeEventHandler {
 public:
  MlmeEventHandlerImpl(ClientInterfaceImpl* client_interface);
  ~MlmeEventHandlerImpl() override;
  void OnConnect(std::unique_ptr<MlmeConnectEvent> event) override;
  void OnRoam(std::unique_ptr<MlmeRoamEvent> event) override;
  void OnAssociate(std::unique_ptr<MlmeAssociateEvent> event) override;
  void OnDisconnect(std::unique_ptr<MlmeDisconnectEvent> event) override;
  void OnDisassociate(std::unique_ptr<MlmeDisassociateEvent> event) override;

 private:
  ClientInterfaceImpl* client_interface_;
};

// Holds the guts of how we control network interfaces capable of connecting to
// access points via wpa_supplicant.
//
// Because remote processes may hold on to the corresponding
// binder object past the lifetime of the local object, we are forced to
// keep this object separate from the binder representation of itself.
class ClientInterfaceImpl {
 public:
  ClientInterfaceImpl(
      uint32_t wiphy_index,
      const std::string& interface_name,
      uint32_t interface_index,
      const std::array<uint8_t, ETH_ALEN>& interface_mac_addr,
      android::wifi_system::InterfaceTool* if_tool,
      NetlinkUtils* netlink_utils,
      ScanUtils* scan_utils);
  virtual ~ClientInterfaceImpl();

  // Get a pointer to the binder representing this ClientInterfaceImpl.
  android::sp<android::net::wifi::IClientInterface> GetBinder() const;

  bool GetPacketCounters(std::vector<int32_t>* out_packet_counters);
  bool SignalPoll(std::vector<int32_t>* out_signal_poll_results);
  const std::array<uint8_t, ETH_ALEN>& GetMacAddress();
  const std::string& GetInterfaceName() const { return interface_name_; }
  const android::sp<ScannerImpl> GetScanner() { return scanner_; };
  bool SetMacAddress(const std::array<uint8_t, ETH_ALEN>& mac);
  virtual bool IsAssociated() const;
  void Dump(std::stringstream* ss) const;
  void SendMgmtFrame(
      const std::vector<uint8_t>& frame,
      const sp<::android::net::wifi::ISendMgmtFrameEvent>& callback,
      int32_t mcs);

 private:
  bool RefreshAssociateFreq();

  const uint32_t wiphy_index_;
  const std::string interface_name_;
  const uint32_t interface_index_;
  const std::array<uint8_t, ETH_ALEN> interface_mac_addr_;
  android::wifi_system::InterfaceTool* const if_tool_;
  NetlinkUtils* const netlink_utils_;
  ScanUtils* const scan_utils_;
  const std::shared_ptr<OffloadServiceUtils> offload_service_utils_;
  const std::unique_ptr<MlmeEventHandlerImpl> mlme_event_handler_;
  const android::sp<ClientInterfaceBinder> binder_;
  android::sp<ScannerImpl> scanner_;

  // Cached information for this connection.
  bool is_associated_;
  std::array<uint8_t, ETH_ALEN> bssid_;
  uint32_t associate_freq_;

  // Capability information for this wiphy/interface.
  BandInfo band_info_;
  ScanCapabilities scan_capabilities_;
  WiphyFeatures wiphy_features_;

  // handler for frame tx status messages
  bool frame_tx_in_progress_;
  uint64_t frame_tx_status_cookie_;
  std::function<void(bool was_acked)> on_frame_tx_status_event_handler_;

  DISALLOW_COPY_AND_ASSIGN(ClientInterfaceImpl);
  friend class MlmeEventHandlerImpl;
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_CLIENT_INTERFACE_IMPL_H_
