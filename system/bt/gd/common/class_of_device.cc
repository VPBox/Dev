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

#include "class_of_device.h"

#include <stdint.h>
#include <algorithm>
#include <sstream>
#include <vector>

#include "os/log.h"

namespace bluetooth {
namespace common {

static_assert(sizeof(ClassOfDevice) == ClassOfDevice::kLength, "ClassOfDevice must be 3 bytes long!");

ClassOfDevice::ClassOfDevice(const uint8_t (&class_of_device)[kLength]) {
  std::copy(class_of_device, class_of_device + kLength, cod);
};

std::string ClassOfDevice::ToString() const {
  char buffer[] = "000-0-00";
  std::snprintf(&buffer[0], sizeof(buffer),
      "%03x-%01x-%02x", (static_cast<uint16_t>(cod[2]) << 4) | cod[1] >> 4, cod[1] & 0x0f, cod[0]);
  std::string str(buffer);
  return str;

}

bool ClassOfDevice::FromString(const std::string& from, ClassOfDevice& to) {
  ClassOfDevice new_cod;
  if (from.length() != 8) return false;

  std::istringstream stream(from);
  std::string token;
  int index = 0;
  uint16_t values[3];

  while (getline(stream, token, '-')) {
    if (index >= 3) {
      return false;
    }

    if (index == 0 && token.length() != 3) {
      return false;
    } else if (index == 1 && token.length() != 1) {
      return false;
    } else if (index == 2 && token.length() != 2) {
      return false;
    }
    char* temp = nullptr;
    values[index] = strtol(token.c_str(), &temp, 16);
    if (*temp != '\0') {
      return false;
    }

    index++;
  }

  if (index != 3) {
    return false;
  }

  new_cod.cod[0] = values[2];
  new_cod.cod[1] = values[1] | ((values[0] & 0xf) << 4);
  new_cod.cod[2] = values[0] >> 4;

  to = new_cod;
  return true;
}

size_t ClassOfDevice::FromOctets(const uint8_t* from) {
  std::copy(from, from + kLength, cod);
  return kLength;
};

bool ClassOfDevice::IsValid(const std::string& cod) {
  ClassOfDevice tmp;
  return ClassOfDevice::FromString(cod, tmp);
}
}  // namespace common
}  // namespace bluetooth
