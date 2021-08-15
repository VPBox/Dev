//
//  Copyright 2017 Google, Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at:
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include "bluetooth/remote_device_props.h"

using std::string;

namespace bluetooth {

RemoteDeviceProps::RemoteDeviceProps() = default;
RemoteDeviceProps::RemoteDeviceProps(const RemoteDeviceProps& other) = default;
RemoteDeviceProps::RemoteDeviceProps(const std::string& name,
                                     const std::string& address,
                                     const std::vector<Uuid>& service_uuids,
                                     int32_t device_class, int32_t device_type,
                                     int32_t rssi)
    : name_(name),
      address_(address),
      service_uuids_(service_uuids),
      device_class_(device_class),
      device_type_(device_type),
      rssi_(rssi) {}
RemoteDeviceProps::~RemoteDeviceProps() = default;

}  // namespace bluetooth
