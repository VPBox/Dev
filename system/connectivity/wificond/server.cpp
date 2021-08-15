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

#include "wificond/server.h"

#include <sstream>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>
#include <binder/IPCThreadState.h>
#include <binder/PermissionCache.h>

#include "wificond/logging_utils.h"
#include "wificond/net/netlink_utils.h"
#include "wificond/scanning/scan_utils.h"

using android::base::WriteStringToFd;
using android::binder::Status;
using android::sp;
using android::IBinder;
using android::net::wifi::IApInterface;
using android::net::wifi::IClientInterface;
using android::net::wifi::IInterfaceEventCallback;
using android::wifi_system::InterfaceTool;

using std::endl;
using std::placeholders::_1;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::vector;

namespace android {
namespace wificond {

namespace {

constexpr const char* kPermissionDump = "android.permission.DUMP";

}  // namespace

Server::Server(unique_ptr<InterfaceTool> if_tool,
               NetlinkUtils* netlink_utils,
               ScanUtils* scan_utils)
    : if_tool_(std::move(if_tool)),
      netlink_utils_(netlink_utils),
      scan_utils_(scan_utils) {
}

Status Server::RegisterCallback(const sp<IInterfaceEventCallback>& callback) {
  for (auto& it : interface_event_callbacks_) {
    if (IInterface::asBinder(callback) == IInterface::asBinder(it)) {
      LOG(WARNING) << "Ignore duplicate interface event callback registration";
      return Status::ok();
    }
  }
  LOG(INFO) << "New interface event callback registered";
  interface_event_callbacks_.push_back(callback);
  return Status::ok();
}

Status Server::UnregisterCallback(const sp<IInterfaceEventCallback>& callback) {
  for (auto it = interface_event_callbacks_.begin();
       it != interface_event_callbacks_.end();
       it++) {
    if (IInterface::asBinder(callback) == IInterface::asBinder(*it)) {
      interface_event_callbacks_.erase(it);
      LOG(INFO) << "Unregister interface event callback";
      return Status::ok();
    }
  }
  LOG(WARNING) << "Failed to find registered interface event callback"
               << " to unregister";
  return Status::ok();
}

Status Server::createApInterface(const std::string& iface_name,
                                 sp<IApInterface>* created_interface) {
  InterfaceInfo interface;
  if (!SetupInterface(iface_name, &interface)) {
    return Status::ok();  // Logging was done internally
  }

  unique_ptr<ApInterfaceImpl> ap_interface(new ApInterfaceImpl(
      interface.name,
      interface.index,
      netlink_utils_,
      if_tool_.get()));
  *created_interface = ap_interface->GetBinder();
  BroadcastApInterfaceReady(ap_interface->GetBinder());
  ap_interfaces_[iface_name] = std::move(ap_interface);

  return Status::ok();
}

Status Server::tearDownApInterface(const std::string& iface_name,
                                   bool* out_success) {
  *out_success = false;
  const auto iter = ap_interfaces_.find(iface_name);
  if (iter != ap_interfaces_.end()) {
    BroadcastApInterfaceTornDown(iter->second->GetBinder());
    ap_interfaces_.erase(iter);
    *out_success = true;
  }
  return Status::ok();
}

Status Server::createClientInterface(const std::string& iface_name,
                                     sp<IClientInterface>* created_interface) {
  InterfaceInfo interface;
  if (!SetupInterface(iface_name, &interface)) {
    return Status::ok();  // Logging was done internally
  }

  unique_ptr<ClientInterfaceImpl> client_interface(new ClientInterfaceImpl(
      wiphy_index_,
      interface.name,
      interface.index,
      interface.mac_address,
      if_tool_.get(),
      netlink_utils_,
      scan_utils_));
  *created_interface = client_interface->GetBinder();
  BroadcastClientInterfaceReady(client_interface->GetBinder());
  client_interfaces_[iface_name] = std::move(client_interface);

  return Status::ok();
}

Status Server::tearDownClientInterface(const std::string& iface_name,
                                       bool* out_success) {
  *out_success = false;
  const auto iter = client_interfaces_.find(iface_name);
  if (iter != client_interfaces_.end()) {
    BroadcastClientInterfaceTornDown(iter->second->GetBinder());
    client_interfaces_.erase(iter);
    *out_success = true;
  }
  return Status::ok();
}

Status Server::tearDownInterfaces() {
  for (auto& it : client_interfaces_) {
    BroadcastClientInterfaceTornDown(it.second->GetBinder());
  }
  client_interfaces_.clear();

  for (auto& it : ap_interfaces_) {
    BroadcastApInterfaceTornDown(it.second->GetBinder());
  }
  ap_interfaces_.clear();

  MarkDownAllInterfaces();

  netlink_utils_->UnsubscribeRegDomainChange(wiphy_index_);

  return Status::ok();
}

Status Server::GetClientInterfaces(vector<sp<IBinder>>* out_client_interfaces) {
  vector<sp<android::IBinder>> client_interfaces_binder;
  for (auto& it : client_interfaces_) {
    out_client_interfaces->push_back(asBinder(it.second->GetBinder()));
  }
  return binder::Status::ok();
}

Status Server::GetApInterfaces(vector<sp<IBinder>>* out_ap_interfaces) {
  vector<sp<IBinder>> ap_interfaces_binder;
  for (auto& it : ap_interfaces_) {
    out_ap_interfaces->push_back(asBinder(it.second->GetBinder()));
  }
  return binder::Status::ok();
}

status_t Server::dump(int fd, const Vector<String16>& /*args*/) {
  if (!PermissionCache::checkCallingPermission(String16(kPermissionDump))) {
    IPCThreadState* ipc = android::IPCThreadState::self();
    LOG(ERROR) << "Caller (uid: " << ipc->getCallingUid()
               << ") is not permitted to dump wificond state";
    return PERMISSION_DENIED;
  }

  stringstream ss;
  ss << "Current wiphy index: " << wiphy_index_ << endl;
  ss << "Cached interfaces list from kernel message: " << endl;
  for (const auto& iface : interfaces_) {
    ss << "Interface index: " << iface.index
       << ", name: " << iface.name
       << ", mac address: "
       << LoggingUtils::GetMacString(iface.mac_address) << endl;
  }

  string country_code;
  if (netlink_utils_->GetCountryCode(&country_code)) {
    ss << "Current country code from kernel: " << country_code << endl;
  } else {
    ss << "Failed to get country code from kernel." << endl;
  }

  for (const auto& iface : client_interfaces_) {
    iface.second->Dump(&ss);
  }

  for (const auto& iface : ap_interfaces_) {
    iface.second->Dump(&ss);
  }

  if (!WriteStringToFd(ss.str(), fd)) {
    PLOG(ERROR) << "Failed to dump state to fd " << fd;
    return FAILED_TRANSACTION;
  }

  return OK;
}

void Server::MarkDownAllInterfaces() {
  uint32_t wiphy_index;
  vector<InterfaceInfo> interfaces;
  if (netlink_utils_->GetWiphyIndex(&wiphy_index) &&
      netlink_utils_->GetInterfaces(wiphy_index, &interfaces)) {
    for (InterfaceInfo& interface : interfaces) {
      if_tool_->SetUpState(interface.name.c_str(), false);
    }
  }
}

Status Server::getAvailable2gChannels(
    std::unique_ptr<vector<int32_t>>* out_frequencies) {
  BandInfo band_info;
  ScanCapabilities scan_capabilities_ignored;
  WiphyFeatures wiphy_features_ignored;

  if (!netlink_utils_->GetWiphyInfo(wiphy_index_, &band_info,
                                    &scan_capabilities_ignored,
                                    &wiphy_features_ignored)) {
    LOG(ERROR) << "Failed to get wiphy info from kernel";
    out_frequencies->reset(nullptr);
    return Status::ok();
  }

  out_frequencies->reset(
      new vector<int32_t>(band_info.band_2g.begin(), band_info.band_2g.end()));
  return Status::ok();
}

Status Server::getAvailable5gNonDFSChannels(
    std::unique_ptr<vector<int32_t>>* out_frequencies) {
  BandInfo band_info;
  ScanCapabilities scan_capabilities_ignored;
  WiphyFeatures wiphy_features_ignored;

  if (!netlink_utils_->GetWiphyInfo(wiphy_index_, &band_info,
                                    &scan_capabilities_ignored,
                                    &wiphy_features_ignored)) {
    LOG(ERROR) << "Failed to get wiphy info from kernel";
    out_frequencies->reset(nullptr);
    return Status::ok();
  }

  out_frequencies->reset(
      new vector<int32_t>(band_info.band_5g.begin(), band_info.band_5g.end()));
  return Status::ok();
}

Status Server::getAvailableDFSChannels(
    std::unique_ptr<vector<int32_t>>* out_frequencies) {
  BandInfo band_info;
  ScanCapabilities scan_capabilities_ignored;
  WiphyFeatures wiphy_features_ignored;

  if (!netlink_utils_->GetWiphyInfo(wiphy_index_, &band_info,
                                    &scan_capabilities_ignored,
                                    &wiphy_features_ignored)) {
    LOG(ERROR) << "Failed to get wiphy info from kernel";
    out_frequencies->reset(nullptr);
    return Status::ok();
  }

  out_frequencies->reset(new vector<int32_t>(band_info.band_dfs.begin(),
                                             band_info.band_dfs.end()));
  return Status::ok();
}

bool Server::SetupInterface(const std::string& iface_name,
                            InterfaceInfo* interface) {
  if (!RefreshWiphyIndex(iface_name)) {
    return false;
  }

  netlink_utils_->SubscribeRegDomainChange(
          wiphy_index_,
          std::bind(&Server::OnRegDomainChanged,
          this,
          _1));

  interfaces_.clear();
  if (!netlink_utils_->GetInterfaces(wiphy_index_, &interfaces_)) {
    LOG(ERROR) << "Failed to get interfaces info from kernel";
    return false;
  }

  for (const auto& iface : interfaces_) {
    if (iface.name == iface_name) {
      *interface = iface;
      return true;
    }
  }

  LOG(ERROR) << "No usable interface found";
  return false;
}

bool Server::RefreshWiphyIndex(const std::string& iface_name) {
  if (!netlink_utils_->GetWiphyIndex(&wiphy_index_, iface_name)) {
    LOG(ERROR) << "Failed to get wiphy index";
    return false;
  }
  return true;
}

void Server::OnRegDomainChanged(std::string& country_code) {
  if (country_code.empty()) {
    LOG(INFO) << "Regulatory domain changed";
  } else {
    LOG(INFO) << "Regulatory domain changed to country: " << country_code;
  }
  LogSupportedBands();
}

void Server::LogSupportedBands() {
  BandInfo band_info;
  ScanCapabilities scan_capabilities;
  WiphyFeatures wiphy_features;
  netlink_utils_->GetWiphyInfo(wiphy_index_,
                               &band_info,
                               &scan_capabilities,
                               &wiphy_features);

  stringstream ss;
  for (unsigned int i = 0; i < band_info.band_2g.size(); i++) {
    ss << " " << band_info.band_2g[i];
  }
  LOG(INFO) << "2.4Ghz frequencies:"<< ss.str();
  ss.str("");

  for (unsigned int i = 0; i < band_info.band_5g.size(); i++) {
    ss << " " << band_info.band_5g[i];
  }
  LOG(INFO) << "5Ghz non-DFS frequencies:"<< ss.str();
  ss.str("");

  for (unsigned int i = 0; i < band_info.band_dfs.size(); i++) {
    ss << " " << band_info.band_dfs[i];
  }
  LOG(INFO) << "5Ghz DFS frequencies:"<< ss.str();
}

void Server::BroadcastClientInterfaceReady(
    sp<IClientInterface> network_interface) {
  for (auto& it : interface_event_callbacks_) {
    it->OnClientInterfaceReady(network_interface);
  }
}

void Server::BroadcastApInterfaceReady(
    sp<IApInterface> network_interface) {
  for (auto& it : interface_event_callbacks_) {
    it->OnApInterfaceReady(network_interface);
  }
}

void Server::BroadcastClientInterfaceTornDown(
    sp<IClientInterface> network_interface) {
  for (auto& it : interface_event_callbacks_) {
    it->OnClientTorndownEvent(network_interface);
  }
}

void Server::BroadcastApInterfaceTornDown(
    sp<IApInterface> network_interface) {
  for (auto& it : interface_event_callbacks_) {
    it->OnApTorndownEvent(network_interface);
  }
}

}  // namespace wificond
}  // namespace android
