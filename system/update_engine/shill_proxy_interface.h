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

#ifndef UPDATE_ENGINE_SHILL_PROXY_INTERFACE_H_
#define UPDATE_ENGINE_SHILL_PROXY_INTERFACE_H_

#include <memory>
#include <string>

#include <base/macros.h>
#include <dbus/object_path.h>
#include <shill/dbus-proxies.h>

namespace chromeos_update_engine {

// This class handles the DBus connection with shill daemon. The DBus interface
// with shill requires to monitor or request the current service by interacting
// with the org::chromium::flimflam::ManagerProxy and then request or monitor
// properties on the selected org::chromium::flimflam::ServiceProxy. This class
// provides a mockable way to access that.
class ShillProxyInterface {
 public:
  virtual ~ShillProxyInterface() = default;

  // Return the ManagerProxy instance of the shill daemon. The instance is owned
  // by this ShillProxyInterface instance.
  virtual org::chromium::flimflam::ManagerProxyInterface* GetManagerProxy() = 0;

  // Return a ServiceProxy for the given path. The ownership of the returned
  // instance is transferred to the caller.
  virtual std::unique_ptr<org::chromium::flimflam::ServiceProxyInterface>
  GetServiceForPath(const dbus::ObjectPath& path) = 0;

 protected:
  ShillProxyInterface() = default;

 private:
  DISALLOW_COPY_AND_ASSIGN(ShillProxyInterface);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_SHILL_PROXY_INTERFACE_H_
