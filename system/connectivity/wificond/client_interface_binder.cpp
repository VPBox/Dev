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

#include "wificond/client_interface_binder.h"

#include <algorithm>
#include <vector>

#include <linux/if_ether.h>

#include <android-base/logging.h>

#include <binder/Status.h>

#include "wificond/client_interface_impl.h"

using android::binder::Status;
using android::net::wifi::ISendMgmtFrameEvent;
using android::net::wifi::IWifiScannerImpl;
using std::vector;

namespace android {
namespace wificond {

ClientInterfaceBinder::ClientInterfaceBinder(ClientInterfaceImpl* impl)
    : impl_(impl) {
}

ClientInterfaceBinder::~ClientInterfaceBinder() {
}

Status ClientInterfaceBinder::getPacketCounters(
    vector<int32_t>* out_packet_counters) {
  if (impl_ == nullptr) {
    return Status::ok();
  }
  impl_->GetPacketCounters(out_packet_counters);
  return Status::ok();
}

Status ClientInterfaceBinder::signalPoll(
    vector<int32_t>* out_signal_poll_results) {
  if (impl_ == nullptr) {
    return Status::ok();
  }
  impl_->SignalPoll(out_signal_poll_results);
  return Status::ok();
}

Status ClientInterfaceBinder::getMacAddress(vector<uint8_t>* out_mac_address) {
  if (impl_ == nullptr) {
    return Status::ok();
  }
  const std::array<uint8_t, ETH_ALEN>& mac = impl_->GetMacAddress();
  *out_mac_address = vector<uint8_t>(mac.begin(), mac.end());
  return Status::ok();
}

Status ClientInterfaceBinder::getInterfaceName(std::string* out_name) {
  if (impl_ == nullptr) {
    return Status::ok();
  }
  *out_name = impl_->GetInterfaceName();
  return Status::ok();
}

Status ClientInterfaceBinder::getWifiScannerImpl(
    sp<IWifiScannerImpl>* out_wifi_scanner_impl) {
  if (impl_ == nullptr) {
    *out_wifi_scanner_impl = nullptr;
    return Status::ok();
  }
  *out_wifi_scanner_impl = impl_->GetScanner();
  return Status::ok();
}


Status ClientInterfaceBinder::setMacAddress(const vector<uint8_t>& mac, bool* success) {
  if (impl_ == nullptr) {
    *success = false;
    return Status::ok();
  }
  if (mac.size() != ETH_ALEN) {
    LOG(ERROR) << "Invalid MAC length " << mac.size();
    *success = false;
    return Status::ok();
  }
  std::array<uint8_t, ETH_ALEN> mac_array;
  std::copy_n(mac.begin(), ETH_ALEN, mac_array.begin());
  *success = impl_->SetMacAddress(mac_array);
  return Status::ok();
}

Status ClientInterfaceBinder::SendMgmtFrame(const vector<uint8_t>& frame,
    const sp<ISendMgmtFrameEvent>& callback, int32_t mcs) {
  if (impl_ == nullptr) {
    callback->OnFailure(ISendMgmtFrameEvent::SEND_MGMT_FRAME_ERROR_UNKNOWN);
    return Status::ok();
  }
  // TODO (b/112029045) validate mcs
  impl_->SendMgmtFrame(frame, callback, mcs);
  return Status::ok();
}

}  // namespace wificond
}  // namespace android
