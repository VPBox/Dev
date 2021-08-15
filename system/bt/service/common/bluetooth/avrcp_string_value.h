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

namespace bluetooth {

class AvrcpStringValue {
 public:
  AvrcpStringValue() = default;
  AvrcpStringValue(const AvrcpStringValue& other) = default;
  AvrcpStringValue(int id, const std::string& value) : id_(id), value_(value){};
  ~AvrcpStringValue() = default;

  int id() const { return id_; }
  const std::string& value() const { return value_; }

 protected:
  int id_ = 0;
  std::string value_;
};

}  // namespace bluetooth
