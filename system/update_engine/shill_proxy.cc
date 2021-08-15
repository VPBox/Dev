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

#include "update_engine/shill_proxy.h"

#include "update_engine/dbus_connection.h"

using org::chromium::flimflam::ManagerProxy;
using org::chromium::flimflam::ManagerProxyInterface;
using org::chromium::flimflam::ServiceProxy;
using org::chromium::flimflam::ServiceProxyInterface;

namespace chromeos_update_engine {

ShillProxy::ShillProxy()
    : bus_(DBusConnection::Get()->GetDBus()),
      manager_proxy_(new ManagerProxy(bus_)) {}

ManagerProxyInterface* ShillProxy::GetManagerProxy() {
  return manager_proxy_.get();
}

std::unique_ptr<ServiceProxyInterface> ShillProxy::GetServiceForPath(
    const dbus::ObjectPath& path) {
  DCHECK(bus_.get());
  return std::unique_ptr<ServiceProxyInterface>(new ServiceProxy(bus_, path));
}

}  // namespace chromeos_update_engine
