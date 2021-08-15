//
// Copyright (C) 2016 The Android Open Source Project
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

#include "update_engine/network_selector_android.h"

#include <memory>

#include <android/multinetwork.h>
#include <base/logging.h>

namespace chromeos_update_engine {

namespace network {

// Factory defined in network_selector.h.
std::unique_ptr<NetworkSelectorInterface> CreateNetworkSelector() {
  return std::make_unique<NetworkSelectorAndroid>();
}

}  // namespace network

// Defined in network_selector_interface.h.
const NetworkId kDefaultNetworkId = NETWORK_UNSPECIFIED;

bool NetworkSelectorAndroid::SetProcessNetwork(NetworkId network_id) {
  if (android_setprocnetwork(network_id) < 0) {
    PLOG(ERROR) << "Binding the network to " << network_id;
    return false;
  }
  return true;
}

}  // namespace chromeos_update_engine
