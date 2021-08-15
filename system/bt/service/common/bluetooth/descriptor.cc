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

#include <bluetooth/descriptor.h>

namespace bluetooth {
Descriptor::Descriptor(const Descriptor& other) {
  handle_ = other.handle_;
  uuid_ = other.uuid_;
  permissions_ = other.permissions_;
}

Descriptor& Descriptor::operator=(const Descriptor& other) {
  if (*this == other) return *this;

  handle_ = other.handle_;
  uuid_ = other.uuid_;
  permissions_ = other.permissions_;

  return *this;
}

bool Descriptor::Equals(const Descriptor& other) const {
  return handle_ == other.handle_ && uuid_ == other.uuid_ &&
         permissions_ == other.permissions_;
}

bool Descriptor::operator==(const Descriptor& rhs) const { return Equals(rhs); }

bool Descriptor::operator!=(const Descriptor& rhs) const {
  return !Equals(rhs);
}
}
