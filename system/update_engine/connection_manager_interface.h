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

#ifndef UPDATE_ENGINE_CONNECTION_MANAGER_INTERFACE_H_
#define UPDATE_ENGINE_CONNECTION_MANAGER_INTERFACE_H_

#include <memory>

#include <base/macros.h>

#include "update_engine/connection_utils.h"

namespace chromeos_update_engine {

class SystemState;

// This class exposes a generic interface to the connection manager
// (e.g FlimFlam, Shill, etc.) to consolidate all connection-related
// logic in update_engine.
class ConnectionManagerInterface {
 public:
  virtual ~ConnectionManagerInterface() = default;

  // Populates |out_type| with the type of the network connection
  // that we are currently connected and |out_tethering| with the estimate of
  // whether that network is being tethered.
  virtual bool GetConnectionProperties(ConnectionType* out_type,
                                       ConnectionTethering* out_tethering) = 0;

  // Returns true if we're allowed to update the system when we're
  // connected to the internet through the given network connection type and the
  // given tethering state.
  virtual bool IsUpdateAllowedOver(ConnectionType type,
                                   ConnectionTethering tethering) const = 0;

  // Returns true if the allowed connection types for update is set in the
  // device policy. Otherwise, returns false.
  virtual bool IsAllowedConnectionTypesForUpdateSet() const = 0;

 protected:
  ConnectionManagerInterface() = default;

 private:
  DISALLOW_COPY_AND_ASSIGN(ConnectionManagerInterface);
};

namespace connection_manager {
// Factory function which creates a ConnectionManager.
std::unique_ptr<ConnectionManagerInterface> CreateConnectionManager(
    SystemState* system_state);
}  // namespace connection_manager

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_CONNECTION_MANAGER_INTERFACE_H_
