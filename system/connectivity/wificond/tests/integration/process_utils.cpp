/*
 * Copyright (C) 2016, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "wificond/tests/integration/process_utils.h"

#include <unistd.h>

#include <android-base/logging.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <binder/IBinder.h>
#include <binder/IServiceManager.h>
#include <utils/String16.h>
#include <utils/StrongPointer.h>

#include "wificond/ipc_constants.h"
#include "wificond/tests/shell_utils.h"

using android::String16;
using android::base::StringPrintf;
using android::base::Trim;
using android::net::wifi::IWificond;
using android::sp;
using android::wificond::ipc_constants::kServiceName;

namespace android {
namespace wificond {
namespace tests {
namespace integration {
namespace {

bool IsProcessRunning(const char* process_name) {
  std::string output;
  RunShellCommand(StringPrintf("pgrep -c ^%s$", process_name), &output);
  output = Trim(output);
  if (output == "0") {
    return false;
  }
  return true;
}

}

const uint32_t ScopedDevModeWificond::kSystemServerDeathTimeoutSeconds = 10;
const uint32_t ScopedDevModeWificond::kSystemServerStartTimeoutSeconds = 10;
const uint32_t ScopedDevModeWificond::kWificondDeathTimeoutSeconds = 10;
const uint32_t ScopedDevModeWificond::kWificondStartTimeoutSeconds = 10;

ScopedDevModeWificond::~ScopedDevModeWificond() {
  if (in_dev_mode_) {
    ExitDevMode();
  }
}

sp<IWificond> ScopedDevModeWificond::EnterDevModeOrDie() {
  sp<IWificond> service;
  RunShellCommand("stop wificond");
  CHECK(WaitForTrue(WificondIsDead, kWificondDeathTimeoutSeconds));
  RunShellCommand("stop");
  CHECK(WaitForTrue(SystemServerIsDead, kSystemServerDeathTimeoutSeconds));
  RunShellCommand("start wificond");
  auto wificond_restarted = std::bind(IsBinderServiceRegistered, kServiceName);
  CHECK(WaitForTrue(wificond_restarted, kWificondStartTimeoutSeconds));
  status_t status = getService(String16(kServiceName), &service);
  if (status != NO_ERROR || service.get() == nullptr) {
    LOG(FATAL) << "Failed to restart wificond in dev mode";
  }
  in_dev_mode_ = true;
  return service;
}

void ScopedDevModeWificond::ExitDevMode() {
  RunShellCommand("stop wificond");
  CHECK(WaitForTrue(WificondIsDead, kWificondDeathTimeoutSeconds));
  RunShellCommand("start");
  CHECK(WaitForTrue(SystemServerIsRunning, kSystemServerStartTimeoutSeconds));
  RunShellCommand("start wificond");
  CHECK(WaitForTrue(WificondIsRunning, kWificondStartTimeoutSeconds));
  in_dev_mode_ = false;
}

bool WaitForTrue(std::function<bool()> condition, time_t timeout_seconds) {
  time_t start_time_seconds = time(nullptr);
  do {
    if (condition()) {
      return true;
    }
    usleep(1000);
  } while ((time(nullptr) - start_time_seconds) < timeout_seconds);
  return false;
}

bool IsBinderServiceRegistered(const char* service_name) {
  sp<IBinder> service =
      defaultServiceManager()->checkService(String16(service_name));
  return service.get() != nullptr;
}

bool SystemServerIsRunning() {
  return IsProcessRunning("system_server");
}

bool SystemServerIsDead() {
  return !IsProcessRunning("system_server");
}

bool WificondIsRunning() {
  return IsProcessRunning("wificond");
}

bool WificondIsDead() { return !WificondIsRunning(); }

bool HostapdIsRunning() {
  return IsProcessRunning("hostapd");
}

bool HostapdIsDead() { return !HostapdIsRunning(); }

bool SupplicantIsRunning() {
  return IsProcessRunning("wpa_supplicant");
}

bool SupplicantIsDead() { return !SupplicantIsRunning(); }

}  // namespace integration
}  // namespace tests
}  // namespace android
}  // namespace wificond
