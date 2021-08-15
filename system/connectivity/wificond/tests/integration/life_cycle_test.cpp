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

#include <gtest/gtest.h>

#include "wificond/ipc_constants.h"
#include "wificond/tests/shell_utils.h"
#include "wificond/tests/integration/process_utils.h"

using android::wificond::ipc_constants::kServiceName;
using android::wificond::tests::integration::RunShellCommand;
using android::wificond::tests::integration::WaitForTrue;
using android::wificond::tests::integration::IsBinderServiceRegistered;
using android::wificond::tests::integration::ScopedDevModeWificond;
using android::wificond::tests::integration::WificondIsRunning;
using android::wificond::tests::integration::WificondIsDead;
using android::wificond::tests::integration::WificondSetDevMode;

namespace android {
namespace wificond {

TEST(LifeCycleTest, ProcessStartsUp) {
  // Request that wificond be stopped (regardless of its current state).
  RunShellCommand("stop wificond");
  EXPECT_TRUE(WaitForTrue(WificondIsDead,
                          ScopedDevModeWificond::kWificondDeathTimeoutSeconds));

  // Confirm that the service manager has no binder for wificond.
  EXPECT_FALSE(IsBinderServiceRegistered(kServiceName));

  // Start wificond.
  RunShellCommand("start wificond");
  EXPECT_TRUE(WaitForTrue(WificondIsRunning,
                          ScopedDevModeWificond::kWificondStartTimeoutSeconds));

  // wificond should eventually register with the service manager.
  EXPECT_TRUE(WaitForTrue(std::bind(IsBinderServiceRegistered, kServiceName),
                          ScopedDevModeWificond::kWificondStartTimeoutSeconds));
}

}  // namespace wificond
}  // namespace android
