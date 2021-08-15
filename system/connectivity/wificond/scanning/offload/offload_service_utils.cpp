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
#include "wificond/scanning/offload/offload_service_utils.h"

#include <android-base/logging.h>
#include <cutils/properties.h>

#include "wificond/scanning/offload/offload_scan_manager.h"
#include "wificond/scanning/scanner_impl.h"

using ::android::hardware::wifi::offload::V1_0::IOffload;

namespace android {
namespace wificond {

android::sp<IOffload> OffloadServiceUtils::GetOffloadService() {
  return IOffload::tryGetService();
}

android::sp<OffloadCallback> OffloadServiceUtils::GetOffloadCallback(
    OffloadCallbackHandlers* handlers) {
  return new OffloadCallback(handlers);
}

OffloadDeathRecipient* OffloadServiceUtils::GetOffloadDeathRecipient(
    OffloadDeathRecipientHandler handler) {
  return new OffloadDeathRecipient(handler);
}

bool OffloadServiceUtils::IsOffloadScanSupported() const {
  if (property_get_bool("persist.wifi.offload.enable", false)) {
    LOG(INFO) << "Offload HAL supported";
    return true;
  }
  LOG(INFO) << "Offload HAL not supported ";
  return false;
}

std::shared_ptr<OffloadScanCallbackInterfaceImpl>
OffloadServiceUtils::GetOffloadScanCallbackInterface(ScannerImpl* parent) {
  return std::make_shared<OffloadScanCallbackInterfaceImpl>(parent);
}

std::shared_ptr<OffloadScanManager> OffloadServiceUtils::GetOffloadScanManager(
    std::weak_ptr<OffloadServiceUtils> service_utils,
    std::shared_ptr<OffloadScanCallbackInterfaceImpl> callback_interface) {
  return std::make_shared<OffloadScanManager>(service_utils, callback_interface);
}

OffloadDeathRecipient::OffloadDeathRecipient(
    OffloadDeathRecipientHandler handler)
    : handler_(handler) {}

}  // namespace wificond
}  // namespace android
