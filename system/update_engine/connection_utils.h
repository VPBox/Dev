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

#ifndef UPDATE_ENGINE_CONNECTION_UTILS_H_
#define UPDATE_ENGINE_CONNECTION_UTILS_H_

#include <string>

namespace chromeos_update_engine {

enum class ConnectionType {
  kDisconnected,
  kEthernet,
  kWifi,
  kWimax,
  kBluetooth,
  kCellular,
  kUnknown
};

enum class ConnectionTethering {
  kNotDetected,
  kSuspected,
  kConfirmed,
  kUnknown,
};

namespace connection_utils {
// Helper methods for converting shill strings into symbolic values.
ConnectionType ParseConnectionType(const std::string& type_str);
ConnectionTethering ParseConnectionTethering(const std::string& tethering_str);

// Returns the string representation corresponding to the given connection type.
const char* StringForConnectionType(ConnectionType type);
}  // namespace connection_utils

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_CONNECTION_UTILS_H_
