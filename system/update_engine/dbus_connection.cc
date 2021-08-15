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

#include "update_engine/dbus_connection.h"

#include <base/time/time.h>

namespace chromeos_update_engine {

namespace {
const int kDBusSystemMaxWaitSeconds = 2 * 60;

DBusConnection* dbus_connection_singleton = nullptr;
}  // namespace

DBusConnection::DBusConnection() {
  // We wait for the D-Bus connection for up two minutes to avoid re-spawning
  // the daemon too fast causing thrashing if dbus-daemon is not running.
  bus_ = dbus_connection_.ConnectWithTimeout(
      base::TimeDelta::FromSeconds(kDBusSystemMaxWaitSeconds));

  if (!bus_) {
    // TODO(deymo): Make it possible to run update_engine even if dbus-daemon
    // is not running or constantly crashing.
    LOG(FATAL) << "Failed to initialize DBus, aborting.";
  }

  CHECK(bus_->SetUpAsyncOperations());
}

const scoped_refptr<dbus::Bus>& DBusConnection::GetDBus() {
  CHECK(bus_);
  return bus_;
}

DBusConnection* DBusConnection::Get() {
  if (!dbus_connection_singleton)
    dbus_connection_singleton = new DBusConnection();
  return dbus_connection_singleton;
}

}  // namespace chromeos_update_engine
