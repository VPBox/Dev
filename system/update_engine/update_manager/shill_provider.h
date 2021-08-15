//
// Copyright (C) 2014 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_SHILL_PROVIDER_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_SHILL_PROVIDER_H_

#include <base/time/time.h>

#include "update_engine/connection_utils.h"
#include "update_engine/update_manager/provider.h"
#include "update_engine/update_manager/variable.h"

namespace chromeos_update_manager {

// Provider for networking related information.
class ShillProvider : public Provider {
 public:
  ~ShillProvider() override {}

  // A variable returning whether we currently have network connectivity.
  virtual Variable<bool>* var_is_connected() = 0;

  // A variable returning the current network connection type. Unknown if not
  // connected.
  virtual Variable<chromeos_update_engine::ConnectionType>* var_conn_type() = 0;

  // A variable returning the tethering mode of a network connection. Unknown if
  // not connected.
  virtual Variable<chromeos_update_engine::ConnectionTethering>*
  var_conn_tethering() = 0;

  // A variable returning the time when network connection last changed.
  // Initialized to current time.
  virtual Variable<base::Time>* var_conn_last_changed() = 0;

 protected:
  ShillProvider() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(ShillProvider);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_SHILL_PROVIDER_H_
