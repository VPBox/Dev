/******************************************************************************
 *
 *  Copyright 2019 The Android Open Source Project
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#pragma once

#include <string>

namespace bluetooth {
namespace common {

class ClassOfDevice final {
 public:
  static constexpr unsigned int kLength = 3;

  uint8_t cod[kLength];

  ClassOfDevice() = default;
  ClassOfDevice(const uint8_t (&class_of_device)[kLength]);

  bool operator==(const ClassOfDevice& rhs) const {
    return (std::memcmp(cod, rhs.cod, sizeof(cod)) == 0);
  }

  std::string ToString() const;

  // Converts |string| to ClassOfDevice and places it in |to|. If |from| does
  // not represent a Class of Device, |to| is not modified and this function
  // returns false. Otherwise, it returns true.
  static bool FromString(const std::string& from, ClassOfDevice& to);

  // Copies |from| raw Class of Device octets to the local object.
  // Returns the number of copied octets (always ClassOfDevice::kLength)
  size_t FromOctets(const uint8_t* from);

  static bool IsValid(const std::string& class_of_device);
};

inline std::ostream& operator<<(std::ostream& os, const ClassOfDevice& c) {
  os << c.ToString();
  return os;
}

}  // namespace common
}  // namespace bluetooth
