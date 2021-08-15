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

#include <bluetooth/uuid.h>

namespace bluetooth {
class Descriptor {
 public:
  Descriptor() = default;
  Descriptor(const Descriptor& other);
  Descriptor& operator=(const Descriptor& other);
  Descriptor(uint16_t handle, const Uuid& uuid, uint16_t permissions)
      : handle_(handle), uuid_(uuid), permissions_(permissions){};
  virtual ~Descriptor() = default;

  // Comparison function and operator.
  bool Equals(const Descriptor& other) const;
  bool operator==(const Descriptor& rhs) const;
  bool operator!=(const Descriptor& rhs) const;

  uint16_t handle() const { return handle_; }
  uint16_t permissions() const { return permissions_; }
  const Uuid& uuid() const { return uuid_; }

 protected:
  uint16_t handle_;
  Uuid uuid_;
  uint16_t permissions_;
};
}
