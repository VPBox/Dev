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

#include "update_engine/connection_manager_android.h"

#include <memory>

namespace chromeos_update_engine {

namespace connection_manager {
std::unique_ptr<ConnectionManagerInterface> CreateConnectionManager(
    SystemState* system_state) {
  return std::unique_ptr<ConnectionManagerInterface>(
      new ConnectionManagerAndroid());
}
}  // namespace connection_manager

bool ConnectionManagerAndroid::GetConnectionProperties(
    ConnectionType* out_type, ConnectionTethering* out_tethering) {
  return false;
}
bool ConnectionManagerAndroid::IsUpdateAllowedOver(
    ConnectionType type, ConnectionTethering tethering) const {
  return true;
}
bool ConnectionManagerAndroid::IsAllowedConnectionTypesForUpdateSet() const {
  return false;
}

}  // namespace chromeos_update_engine
