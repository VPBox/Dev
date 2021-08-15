//
//  Copyright (C) 2017 Google, Inc.
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

#pragma once

#include <string>
#include <vector>

#include "bluetooth/uuid.h"

namespace bluetooth {

class RemoteDeviceProps {
 public:
  RemoteDeviceProps();
  RemoteDeviceProps(const RemoteDeviceProps& other);
  RemoteDeviceProps(const std::string& name, const std::string& address,
                    const std::vector<Uuid>& service_uuids,
                    int32_t device_class, int32_t device_type, int32_t rssi);
  ~RemoteDeviceProps();

  void set_address(const std::string& address) { address_ = address; }

  const std::string& name() const { return name_; }
  const std::string& address() const { return address_; }
  const std::vector<Uuid>& service_uuids() const { return service_uuids_; }
  int32_t device_class() const { return device_class_; }
  int32_t device_type() const { return device_type_; }
  int32_t rssi() const { return rssi_; }

 protected:
  std::string name_;
  std::string address_;
  std::vector<Uuid> service_uuids_;
  int32_t device_class_ = -1;
  int32_t device_type_ = -1;
  int32_t rssi_ = -1;
};

}  // namespace bluetooth
