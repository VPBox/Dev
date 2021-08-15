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

#include "wificond/ap_interface_binder.h"

#include <android-base/logging.h>

#include "wificond/ap_interface_impl.h"

using android::net::wifi::IApInterfaceEventCallback;

namespace android {
namespace wificond {

ApInterfaceBinder::ApInterfaceBinder(ApInterfaceImpl* impl)
    : impl_{impl}, ap_interface_event_callback_(nullptr) {}

ApInterfaceBinder::~ApInterfaceBinder() {
}

void ApInterfaceBinder::NotifyNumAssociatedStationsChanged(int num_stations) {
  if (ap_interface_event_callback_ != nullptr) {
    ap_interface_event_callback_->onNumAssociatedStationsChanged(num_stations);
  }
}

void ApInterfaceBinder::NotifySoftApChannelSwitched(
    int frequency, ChannelBandwidth channel_bandwidth) {
  if (ap_interface_event_callback_ == nullptr) {
    return;
  }

  int bandwidth;
  switch (channel_bandwidth) {
    case ChannelBandwidth::BW_INVALID:
      bandwidth = IApInterfaceEventCallback::BANDWIDTH_INVALID;
      break;
    case ChannelBandwidth::BW_20_NOHT:
      bandwidth = IApInterfaceEventCallback::BANDWIDTH_20_NOHT;
      break;
    case ChannelBandwidth::BW_20:
      bandwidth = IApInterfaceEventCallback::BANDWIDTH_20;
      break;
    case ChannelBandwidth::BW_40:
      bandwidth = IApInterfaceEventCallback::BANDWIDTH_40;
      break;
    case ChannelBandwidth::BW_80:
      bandwidth = IApInterfaceEventCallback::BANDWIDTH_80;
      break;
    case ChannelBandwidth::BW_80P80:
      bandwidth = IApInterfaceEventCallback::BANDWIDTH_80P80;
      break;
    case ChannelBandwidth::BW_160:
      bandwidth = IApInterfaceEventCallback::BANDWIDTH_160;
      break;
    default:
      bandwidth = IApInterfaceEventCallback::BANDWIDTH_INVALID;
  }
  ap_interface_event_callback_->onSoftApChannelSwitched(frequency, bandwidth);
}

binder::Status ApInterfaceBinder::registerCallback(
    const sp<IApInterfaceEventCallback>& callback, bool* out_success) {
  *out_success = true;
  ap_interface_event_callback_ = callback;
  return binder::Status::ok();
}

binder::Status ApInterfaceBinder::getInterfaceName(std::string* out_name) {
  *out_name = impl_->GetInterfaceName();
  return binder::Status::ok();
}

binder::Status ApInterfaceBinder::getNumberOfAssociatedStations(
    int* out_num_of_stations) {
  *out_num_of_stations = impl_->GetNumberOfAssociatedStations();
  return binder::Status::ok();
}

}  // namespace wificond
}  // namespace android
