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

#ifndef UPDATE_ENGINE_FAKE_SHILL_PROXY_H_
#define UPDATE_ENGINE_FAKE_SHILL_PROXY_H_

#include <map>
#include <memory>
#include <string>

#include <base/macros.h>
#include <shill/dbus-proxies.h>
#include <shill/dbus-proxy-mocks.h>

#include "update_engine/shill_proxy_interface.h"

namespace chromeos_update_engine {

// This class implements the connection to shill using real DBus calls.
class FakeShillProxy : public ShillProxyInterface {
 public:
  FakeShillProxy();
  ~FakeShillProxy() override = default;

  // ShillProxyInterface overrides.

  // GetManagerProxy returns the subclass ManagerProxyMock so tests can easily
  // use it. Mocks for the return value of GetServiceForPath() can be provided
  // with SetServiceForPath().
  org::chromium::flimflam::ManagerProxyMock* GetManagerProxy() override;
  std::unique_ptr<org::chromium::flimflam::ServiceProxyInterface>
  GetServiceForPath(const dbus::ObjectPath& path) override;

  // Sets the service_proxy that will be returned by GetServiceForPath().
  void SetServiceForPath(
      const dbus::ObjectPath& path,
      std::unique_ptr<org::chromium::flimflam::ServiceProxyInterface>
          service_proxy);

 private:
  std::unique_ptr<org::chromium::flimflam::ManagerProxyMock>
      manager_proxy_mock_;

  std::map<std::string,
           std::unique_ptr<org::chromium::flimflam::ServiceProxyInterface>>
      service_proxy_mocks_;

  DISALLOW_COPY_AND_ASSIGN(FakeShillProxy);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_FAKE_SHILL_PROXY_H_
