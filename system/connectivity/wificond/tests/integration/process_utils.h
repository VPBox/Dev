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

#ifndef WIFICOND_TESTS_INTEGRATION_PROCESS_UTILS_H_
#define WIFICOND_TESTS_INTEGRATION_PROCESS_UTILS_H_

#include <time.h>

#include <functional>

#include <android-base/macros.h>
#include <utils/StrongPointer.h>

#include "android/net/wifi/IWificond.h"

namespace android {
namespace wificond {
namespace tests {
namespace integration {

// Instantiate one of these and call EnterDevMode() to put wificond into
// developer mode.  wificond will be automatically restarted in normal
// mode when the ScopedDevModeWificond intance goes out of scope:
//
// TEST(MyTest, TestCase) {
//   ScopedDevModeWificond dev_mode;
//   dev_mode.EnterDevMode();
//
//   // Code can assume that wificond is running in dev mode.
//   // wificond will be cleaned up when |dev_mode| is destroyed.
//
// }
class ScopedDevModeWificond final {
 public:
  static const uint32_t kSystemServerDeathTimeoutSeconds;
  static const uint32_t kSystemServerStartTimeoutSeconds;
  static const uint32_t kWificondDeathTimeoutSeconds;
  static const uint32_t kWificondStartTimeoutSeconds;

  ScopedDevModeWificond() = default;
  ~ScopedDevModeWificond();

  // You have to explicitly request that we enter dev mode.
  // This makes avoids the nastiness of restarting a system process
  // because you instantiated an object.
  // ASSERTs that wificond starts in dev mode.
  android::sp<android::net::wifi::IWificond> EnterDevModeOrDie();
  // Returns true iff wificond entered dev mode.
  android::sp<android::net::wifi::IWificond> MaybeEnterDevMode();

  // This will be called for you in the destructor, but you can do it yourself
  // if you prefer.
  void ExitDevMode();

 private:
  bool in_dev_mode_ = false;

  DISALLOW_COPY_AND_ASSIGN(ScopedDevModeWificond);
};  // ScopedDevModeWificond

// Blocks until either |condition| returns true, or |timeout_seconds| passes.
// Returns true iff |condition| returned true.
bool WaitForTrue(std::function<bool()> condition, time_t timeout_seconds);

// Returns true iff a service is registered as |service_name| with the Binder
// service manager.
bool IsBinderServiceRegistered(const char* service_name);

// Returns true iff the system_server process is running on the system.
bool SystemServerIsRunning();

// Convenient alias for !SystemServerIsRunning
bool SystemServerIsDead();

// Returns true iff the wificond process is running on the system.
bool WificondIsRunning();

// Convenient alias for !WificondIsRunning
bool WificondIsDead();

// Set the system property that controls whether wificond starts up in
// developer mode.  See wificond's main() for what this entails.
bool WificondSetDevMode(bool is_on);

// Returns true iff the hostapd process is running on the system.
bool HostapdIsRunning();

// Convenient alias for !HostapdIsRunning
bool HostapdIsDead();

// Returns true iff the wpa_supplicant process is running on the system.
bool SupplicantIsRunning();

// Convenient alias for !SupplicantIsRunning
bool SupplicantIsDead();

}  // namespace integration
}  // namespace tests
}  // namespace android
}  // namespace wificond

#endif  // WIFICOND_TESTS_INTEGRATION_PROCESS_UTILS_H_
