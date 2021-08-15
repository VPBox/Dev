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

#ifndef UPDATE_ENGINE_NETWORK_SELECTOR_INTERFACE_H_
#define UPDATE_ENGINE_NETWORK_SELECTOR_INTERFACE_H_

#include <cstdint>

namespace chromeos_update_engine {

typedef uint64_t NetworkId;

// A constant value used to indicate the default network id. Defined in the
// network_selector_*.cc file.
extern const NetworkId kDefaultNetworkId;

// A class that handles the network used for the connections performed from this
// process in a platform-specific way.

class NetworkSelectorInterface {
 public:
  virtual ~NetworkSelectorInterface() = default;

  // Set the current process network. All sockets created in the future will be
  // bound to this particular network. Call this with the special value
  // kNetworkId to use the default network.
  virtual bool SetProcessNetwork(NetworkId network_id) = 0;

 protected:
  NetworkSelectorInterface() = default;
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_NETWORK_SELECTOR_INTERFACE_H_
