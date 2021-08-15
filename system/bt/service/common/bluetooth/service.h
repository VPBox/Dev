//
//  Copyright 2015 Google, Inc.
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

#include <bluetooth/characteristic.h>
#include <bluetooth/uuid.h>

#include <vector>

namespace bluetooth {
class Service {
 public:
  Service() = default;
  Service(const Service& other);
  Service(uint16_t handle, bool primary, const Uuid& uuid,
          const std::vector<Characteristic>& characteristics,
          const std::vector<Service>& included_services)
      : handle_(handle),
        primary_(primary),
        uuid_(uuid),
        characteristics_(characteristics),
        included_services_(included_services){};
  Service& operator=(const Service& other);
  virtual ~Service() = default;

  // Comparison function and operator.
  bool Equals(const Service& other) const;
  bool operator==(const Service& rhs) const;
  bool operator!=(const Service& rhs) const;

  uint16_t handle() const { return handle_; }
  bool primary() const { return primary_; }
  const Uuid& uuid() const { return uuid_; }
  const std::vector<Characteristic>& characteristics() const {
    return characteristics_;
  }
  std::vector<Characteristic>& characteristics() { return characteristics_; }
  const std::vector<Service>& included_services() const {
    return included_services_;
  }

 protected:
  uint16_t handle_;
  bool primary_;
  Uuid uuid_;
  std::vector<Characteristic> characteristics_;
  std::vector<Service> included_services_;
};
}
