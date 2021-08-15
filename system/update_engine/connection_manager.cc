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

#include "update_engine/connection_manager.h"

#include <memory>
#include <set>
#include <string>

#include <base/stl_util.h>
#include <base/strings/string_util.h>
#include <policy/device_policy.h>
#include <shill/dbus-constants.h>
#include <shill/dbus-proxies.h>

#include "update_engine/common/prefs.h"
#include "update_engine/common/utils.h"
#include "update_engine/connection_utils.h"
#include "update_engine/shill_proxy.h"
#include "update_engine/system_state.h"
#include "update_engine/update_attempter.h"

using org::chromium::flimflam::ManagerProxyInterface;
using org::chromium::flimflam::ServiceProxyInterface;
using std::set;
using std::string;

namespace chromeos_update_engine {

namespace connection_manager {
std::unique_ptr<ConnectionManagerInterface> CreateConnectionManager(
    SystemState* system_state) {
  return std::unique_ptr<ConnectionManagerInterface>(
      new ConnectionManager(new ShillProxy(), system_state));
}
}  // namespace connection_manager

ConnectionManager::ConnectionManager(ShillProxyInterface* shill_proxy,
                                     SystemState* system_state)
    : shill_proxy_(shill_proxy), system_state_(system_state) {}

bool ConnectionManager::IsUpdateAllowedOver(
    ConnectionType type, ConnectionTethering tethering) const {
  switch (type) {
    case ConnectionType::kBluetooth:
      return false;

    case ConnectionType::kCellular: {
      set<string> allowed_types;

      const policy::DevicePolicy* device_policy =
          system_state_->device_policy();

      // The device_policy is loaded in a lazy way before an update check. Load
      // it now from the libbrillo cache if it wasn't already loaded.
      if (!device_policy) {
        UpdateAttempter* update_attempter = system_state_->update_attempter();
        if (update_attempter) {
          update_attempter->RefreshDevicePolicy();
          device_policy = system_state_->device_policy();
        }
      }

      if (!device_policy) {
        // Device policy fails to be loaded (possibly due to guest account). We
        // do not check the local user setting here, which should be checked by
        // |OmahaRequestAction| during checking for update.
        LOG(INFO) << "Allowing updates over cellular as device policy "
                     "fails to be loaded.";
        return true;
      }

      if (device_policy->GetAllowedConnectionTypesForUpdate(&allowed_types)) {
        // The update setting is enforced by the device policy.

        if (!base::ContainsKey(allowed_types, shill::kTypeCellular)) {
          LOG(INFO) << "Disabling updates over cellular connection as it's not "
                       "allowed in the device policy.";
          return false;
        }

        LOG(INFO) << "Allowing updates over cellular per device policy.";
        return true;
      }

      // If there's no update setting in the device policy, we do not check
      // the local user setting here, which should be checked by
      // |OmahaRequestAction| during checking for update.
      LOG(INFO) << "Allowing updates over cellular as device policy does "
                   "not include update setting.";
      return true;
    }

    default:
      if (tethering == ConnectionTethering::kConfirmed) {
        // Treat this connection as if it is a cellular connection.
        LOG(INFO) << "Current connection is confirmed tethered, using Cellular "
                     "setting.";
        return IsUpdateAllowedOver(ConnectionType::kCellular,
                                   ConnectionTethering::kUnknown);
      }
      return true;
  }
}

bool ConnectionManager::IsAllowedConnectionTypesForUpdateSet() const {
  const policy::DevicePolicy* device_policy = system_state_->device_policy();
  if (!device_policy) {
    LOG(INFO) << "There's no device policy loaded yet.";
    return false;
  }

  set<string> allowed_types;
  if (!device_policy->GetAllowedConnectionTypesForUpdate(&allowed_types)) {
    return false;
  }

  return true;
}

bool ConnectionManager::GetConnectionProperties(
    ConnectionType* out_type, ConnectionTethering* out_tethering) {
  dbus::ObjectPath default_service_path;
  TEST_AND_RETURN_FALSE(GetDefaultServicePath(&default_service_path));
  if (!default_service_path.IsValid())
    return false;
  // Shill uses the "/" service path to indicate that it is not connected.
  if (default_service_path.value() == "/") {
    *out_type = ConnectionType::kDisconnected;
    *out_tethering = ConnectionTethering::kUnknown;
    return true;
  }
  TEST_AND_RETURN_FALSE(
      GetServicePathProperties(default_service_path, out_type, out_tethering));
  return true;
}

bool ConnectionManager::GetDefaultServicePath(dbus::ObjectPath* out_path) {
  brillo::VariantDictionary properties;
  brillo::ErrorPtr error;
  ManagerProxyInterface* manager_proxy = shill_proxy_->GetManagerProxy();
  if (!manager_proxy)
    return false;
  TEST_AND_RETURN_FALSE(manager_proxy->GetProperties(&properties, &error));

  const auto& prop_default_service =
      properties.find(shill::kDefaultServiceProperty);
  if (prop_default_service == properties.end())
    return false;

  *out_path = prop_default_service->second.TryGet<dbus::ObjectPath>();
  return out_path->IsValid();
}

bool ConnectionManager::GetServicePathProperties(
    const dbus::ObjectPath& path,
    ConnectionType* out_type,
    ConnectionTethering* out_tethering) {
  // We create and dispose the ServiceProxyInterface on every request.
  std::unique_ptr<ServiceProxyInterface> service =
      shill_proxy_->GetServiceForPath(path);

  brillo::VariantDictionary properties;
  brillo::ErrorPtr error;
  TEST_AND_RETURN_FALSE(service->GetProperties(&properties, &error));

  // Populate the out_tethering.
  const auto& prop_tethering = properties.find(shill::kTetheringProperty);
  if (prop_tethering == properties.end()) {
    // Set to Unknown if not present.
    *out_tethering = ConnectionTethering::kUnknown;
  } else {
    // If the property doesn't contain a string value, the empty string will
    // become kUnknown.
    *out_tethering = connection_utils::ParseConnectionTethering(
        prop_tethering->second.TryGet<string>());
  }

  // Populate the out_type property.
  const auto& prop_type = properties.find(shill::kTypeProperty);
  if (prop_type == properties.end()) {
    // Set to Unknown if not present.
    *out_type = ConnectionType::kUnknown;
    return false;
  }

  string type_str = prop_type->second.TryGet<string>();
  if (type_str == shill::kTypeVPN) {
    const auto& prop_physical =
        properties.find(shill::kPhysicalTechnologyProperty);
    if (prop_physical == properties.end()) {
      LOG(ERROR) << "No PhysicalTechnology property found for a VPN"
                    " connection (service: "
                 << path.value() << "). Returning default kUnknown value.";
      *out_type = ConnectionType::kUnknown;
    } else {
      *out_type = connection_utils::ParseConnectionType(
          prop_physical->second.TryGet<string>());
    }
  } else {
    *out_type = connection_utils::ParseConnectionType(type_str);
  }
  return true;
}

}  // namespace chromeos_update_engine
