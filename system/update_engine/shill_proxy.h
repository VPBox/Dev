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

#ifndef UPDATE_ENGINE_SHILL_PROXY_H_
#define UPDATE_ENGINE_SHILL_PROXY_H_

#include <memory>
#include <string>

#include <base/macros.h>
#include <dbus/bus.h>
#include <dbus/object_path.h>
#include <shill/dbus-proxies.h>

#include "update_engine/shill_proxy_interface.h"

namespace chromeos_update_engine {

// This class implements the connection to shill using real DBus calls.
class ShillProxy : public ShillProxyInterface {
 public:
  ShillProxy();
  ~ShillProxy() override = default;

  // ShillProxyInterface overrides.
  org::chromium::flimflam::ManagerProxyInterface* GetManagerProxy() override;
  std::unique_ptr<org::chromium::flimflam::ServiceProxyInterface>
  GetServiceForPath(const dbus::ObjectPath& path) override;

 private:
  // A reference to the main bus for creating new ServiceProxy instances.
  scoped_refptr<dbus::Bus> bus_;
  std::unique_ptr<org::chromium::flimflam::ManagerProxyInterface>
      manager_proxy_;

  DISALLOW_COPY_AND_ASSIGN(ShillProxy);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_SHILL_PROXY_H_
