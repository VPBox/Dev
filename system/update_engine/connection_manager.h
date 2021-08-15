//
// Copyright (C) 2012 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_CONNECTION_MANAGER_H_
#define UPDATE_ENGINE_CONNECTION_MANAGER_H_

#include <memory>
#include <string>

#include <base/macros.h>
#include <dbus/object_path.h>

#include "update_engine/connection_manager_interface.h"
#include "update_engine/shill_proxy_interface.h"

namespace chromeos_update_engine {

// This class implements the concrete class that talks with the connection
// manager (shill) over DBus.
// TODO(deymo): Remove this class and use ShillProvider from the UpdateManager.
class ConnectionManager : public ConnectionManagerInterface {
 public:
  // Constructs a new ConnectionManager object initialized with the
  // given system state.
  ConnectionManager(ShillProxyInterface* shill_proxy,
                    SystemState* system_state);
  ~ConnectionManager() override = default;

  // ConnectionManagerInterface overrides.
  bool GetConnectionProperties(ConnectionType* out_type,
                               ConnectionTethering* out_tethering) override;
  bool IsUpdateAllowedOver(ConnectionType type,
                           ConnectionTethering tethering) const override;
  bool IsAllowedConnectionTypesForUpdateSet() const override;

 private:
  // Returns (via out_path) the default network path, or "/" if there's no
  // network up. Returns true on success.
  bool GetDefaultServicePath(dbus::ObjectPath* out_path);

  bool GetServicePathProperties(const dbus::ObjectPath& path,
                                ConnectionType* out_type,
                                ConnectionTethering* out_tethering);

  // The mockable interface to access the shill DBus proxies.
  std::unique_ptr<ShillProxyInterface> shill_proxy_;

  // The global context for update_engine.
  SystemState* system_state_;

  DISALLOW_COPY_AND_ASSIGN(ConnectionManager);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_CONNECTION_MANAGER_H_
