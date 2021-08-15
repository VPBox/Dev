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

#include <bluetooth/service.h>

namespace bluetooth {
Service::Service(const Service& other) {
  handle_ = other.handle_;
  primary_ = other.primary_;
  uuid_ = other.uuid_;
  characteristics_ = other.characteristics_;
  included_services_ = other.included_services_;
}

Service& Service::operator=(const Service& other) {
  if (*this == other) return *this;

  handle_ = other.handle_;
  primary_ = other.primary_;
  uuid_ = other.uuid_;
  characteristics_ = other.characteristics_;
  included_services_ = other.included_services_;
  return *this;
}

bool Service::Equals(const Service& other) const {
  return handle_ == other.handle_ && primary_ == other.primary_ &&
         uuid_ == other.uuid_ && characteristics_ == other.characteristics_ &&
         included_services_ == other.included_services_;
}

bool Service::operator==(const Service& rhs) const { return Equals(rhs); }

bool Service::operator!=(const Service& rhs) const { return !Equals(rhs); }
}
