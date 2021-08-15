/*
 * Copyright (C) 2017 The Android Open Source Project
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

#pragma once

#include <vintf/Transport.h>

#include <set>
#include <string>

namespace android {
namespace hardware {

// Get transport method from vendor interface manifest.
// interfaceName has the format "android.hardware.foo@1.0::IFoo"
// instanceName is "default", "ashmem", etc.
// If it starts with "android.hidl.", a static map is looked up instead.
vintf::Transport getTransport(const std::string &interfaceName,
                              const std::string &instanceName);

// All HALs on the device in manifests.
std::set<std::string> getInstances(const std::string& interfaceName);

}  // hardware
}  // android
