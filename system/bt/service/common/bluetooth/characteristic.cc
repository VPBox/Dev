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

#include <bluetooth/characteristic.h>

namespace bluetooth {
Characteristic::Characteristic(const Characteristic& other) {
  handle_ = other.handle_;
  uuid_ = other.uuid_;
  properties_ = other.properties_;
  permissions_ = other.permissions_;
  descriptors_ = other.descriptors_;
}

Characteristic& Characteristic::operator=(const Characteristic& other) {
  if (*this == other) return *this;

  handle_ = other.handle_;
  uuid_ = other.uuid_;
  properties_ = other.properties_;
  permissions_ = other.permissions_;
  descriptors_ = other.descriptors_;

  return *this;
}

bool Characteristic::Equals(const Characteristic& other) const {
  return handle_ == other.handle_ && uuid_ == other.uuid_ &&
         properties_ == other.properties_ &&
         permissions_ == other.permissions_ &&
         descriptors_ == other.descriptors_;
}

bool Characteristic::operator==(const Characteristic& rhs) const {
  return Equals(rhs);
}

bool Characteristic::operator!=(const Characteristic& rhs) const {
  return !Equals(rhs);
}
}
