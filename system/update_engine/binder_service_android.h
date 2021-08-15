//
// Copyright (C) 2015 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef UPDATE_ENGINE_BINDER_SERVICE_ANDROID_H_
#define UPDATE_ENGINE_BINDER_SERVICE_ANDROID_H_

#include <stdint.h>

#include <string>
#include <vector>

#include <utils/Errors.h>
#include <utils/String16.h>
#include <utils/StrongPointer.h>

#include "android/os/BnUpdateEngine.h"
#include "android/os/IUpdateEngineCallback.h"
#include "update_engine/service_delegate_android_interface.h"
#include "update_engine/service_observer_interface.h"

namespace chromeos_update_engine {

class BinderUpdateEngineAndroidService : public android::os::BnUpdateEngine,
                                         public ServiceObserverInterface {
 public:
  explicit BinderUpdateEngineAndroidService(
      ServiceDelegateAndroidInterface* service_delegate);
  ~BinderUpdateEngineAndroidService() override = default;

  const char* ServiceName() const { return "android.os.UpdateEngineService"; }

  // ServiceObserverInterface overrides.
  void SendStatusUpdate(
      const update_engine::UpdateEngineStatus& update_engine_status) override;
  void SendPayloadApplicationComplete(ErrorCode error_code) override;

  // android::os::BnUpdateEngine overrides.
  android::binder::Status applyPayload(
      const android::String16& url,
      int64_t payload_offset,
      int64_t payload_size,
      const std::vector<android::String16>& header_kv_pairs) override;
  android::binder::Status bind(
      const android::sp<android::os::IUpdateEngineCallback>& callback,
      bool* return_value) override;
  android::binder::Status unbind(
      const android::sp<android::os::IUpdateEngineCallback>& callback,
      bool* return_value) override;
  android::binder::Status suspend() override;
  android::binder::Status resume() override;
  android::binder::Status cancel() override;
  android::binder::Status resetStatus() override;
  android::binder::Status verifyPayloadApplicable(
      const android::String16& metadata_filename, bool* return_value) override;

 private:
  // Remove the passed |callback| from the list of registered callbacks. Called
  // on unbind() or whenever the callback object is destroyed.
  // Returns true on success.
  bool UnbindCallback(const IBinder* callback);

  // List of currently bound callbacks.
  std::vector<android::sp<android::os::IUpdateEngineCallback>> callbacks_;

  // Cached copy of the last status update sent. Used to send an initial
  // notification when bind() is called from the client.
  int last_status_{-1};
  double last_progress_{0.0};

  ServiceDelegateAndroidInterface* service_delegate_;
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_BINDER_SERVICE_ANDROID_H_
