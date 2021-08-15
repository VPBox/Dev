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
#ifndef WIFICOND_OFFLOAD_SERVICE_UTILS_H_
#define WIFICOND_OFFLOAD_SERVICE_UTILS_H_

#include <android/hardware/wifi/offload/1.0/IOffload.h>
#include "wificond/scanning/offload/offload_callback.h"
#include "wificond/scanning/offload/offload_callback_handlers.h"
#include "wificond/scanning/offload_scan_callback_interface_impl.h"

namespace android {
namespace wificond {

typedef std::function<void(uint64_t)> OffloadDeathRecipientHandler;
class ScannerImpl;
class OffloadServiceUtils;
class OffloadScanManager;

class OffloadDeathRecipient : public android::hardware::hidl_death_recipient {
 public:
  OffloadDeathRecipient(OffloadDeathRecipientHandler handler);

  void serviceDied(
      uint64_t cookie,
      const android::wp<android::hidl::base::V1_0::IBase>& who) override {
    this->handler_(cookie);
  }

 private:
  OffloadDeathRecipientHandler handler_;
};

// Provides methods to get Offload HAL service and create callback
class OffloadServiceUtils {
 public:
  OffloadServiceUtils() = default;
  virtual ~OffloadServiceUtils() = default;
  virtual android::sp<android::hardware::wifi::offload::V1_0::IOffload>
      GetOffloadService();
  // Check if Offload scan is supported on this device.
  virtual bool IsOffloadScanSupported() const;
  virtual android::sp<OffloadCallback> GetOffloadCallback(
      OffloadCallbackHandlers* handlers);
  virtual OffloadDeathRecipient* GetOffloadDeathRecipient(
      OffloadDeathRecipientHandler handler);
  virtual std::shared_ptr<OffloadScanCallbackInterfaceImpl>
  GetOffloadScanCallbackInterface(ScannerImpl* parent);
  virtual std::shared_ptr<OffloadScanManager> GetOffloadScanManager(
      std::weak_ptr<OffloadServiceUtils> service_utils,
      std::shared_ptr<OffloadScanCallbackInterfaceImpl> callback_interface);
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_OFFLOAD_SERVICE_UTILS_H
