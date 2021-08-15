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

#include <bluetooth/descriptor.h>
#include <bluetooth/uuid.h>

#include <vector>

namespace bluetooth {
class Characteristic {
 public:
  Characteristic() = default;
  Characteristic(const Characteristic& other);
  Characteristic(uint16_t handle, const Uuid& uuid, uint8_t properties,
                 uint16_t permissions,
                 const std::vector<Descriptor>& descriptors)
      : handle_(handle),
        uuid_(uuid),
        properties_(properties),
        permissions_(permissions),
        descriptors_(descriptors){};
  Characteristic& operator=(const Characteristic& other);
  virtual ~Characteristic() = default;

  // Comparison function and operator.
  bool Equals(const Characteristic& other) const;
  bool operator==(const Characteristic& rhs) const;
  bool operator!=(const Characteristic& rhs) const;

  uint16_t handle() const { return handle_; }
  const Uuid& uuid() const { return uuid_; }
  uint8_t properties() const { return properties_; }
  uint16_t permissions() const { return permissions_; }
  const std::vector<Descriptor>& descriptors() const { return descriptors_; }
  std::vector<Descriptor>& descriptors() { return descriptors_; }

 protected:
  uint16_t handle_;
  Uuid uuid_;
  uint8_t properties_;
  uint16_t permissions_;
  std::vector<Descriptor> descriptors_;
};
}
