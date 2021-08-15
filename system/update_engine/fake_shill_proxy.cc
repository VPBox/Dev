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

#include "update_engine/fake_shill_proxy.h"

#include <utility>

using org::chromium::flimflam::ManagerProxyMock;
using org::chromium::flimflam::ServiceProxyInterface;

namespace chromeos_update_engine {

FakeShillProxy::FakeShillProxy()
    : manager_proxy_mock_(new ManagerProxyMock()) {}

ManagerProxyMock* FakeShillProxy::GetManagerProxy() {
  return manager_proxy_mock_.get();
}

std::unique_ptr<ServiceProxyInterface> FakeShillProxy::GetServiceForPath(
    const dbus::ObjectPath& path) {
  auto it = service_proxy_mocks_.find(path.value());
  CHECK(it != service_proxy_mocks_.end())
      << "No ServiceProxyMock set for " << path.value();
  std::unique_ptr<ServiceProxyInterface> result = std::move(it->second);
  service_proxy_mocks_.erase(it);
  return result;
}

void FakeShillProxy::SetServiceForPath(
    const dbus::ObjectPath& path,
    std::unique_ptr<ServiceProxyInterface> service_proxy) {
  service_proxy_mocks_[path.value()] = std::move(service_proxy);
}

}  // namespace chromeos_update_engine
