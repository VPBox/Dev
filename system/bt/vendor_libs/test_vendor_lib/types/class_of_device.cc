/******************************************************************************
 *
 *  Copyright 2018 The Android Open Source Project
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

#include <base/strings/string_split.h>
#include <base/strings/stringprintf.h>
#include <stdint.h>
#include <algorithm>
#include <vector>

static_assert(sizeof(ClassOfDevice) == ClassOfDevice::kLength, "ClassOfDevice must be 3 bytes long!");

ClassOfDevice::ClassOfDevice(const uint8_t (&class_of_device)[kLength]) {
  std::copy(class_of_device, class_of_device + kLength, cod);
};

std::string ClassOfDevice::ToString() const {
  return base::StringPrintf("%03x-%01x-%02x", (static_cast<uint16_t>(cod[2]) << 4) | cod[1] >> 4, cod[1] & 0x0f,
                            cod[0]);
}

bool ClassOfDevice::FromString(const std::string& from, ClassOfDevice& to) {
  ClassOfDevice new_cod;
  if (from.length() != 8) return false;

  std::vector<std::string> byte_tokens = base::SplitString(from, "-", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);

  if (byte_tokens.size() != 3) return false;
  if (byte_tokens[0].length() != 3) return false;
  if (byte_tokens[1].length() != 1) return false;
  if (byte_tokens[2].length() != 2) return false;

  uint16_t values[3];

  for (size_t i = 0; i < kLength; i++) {
    const auto& token = byte_tokens[i];

    char* temp = nullptr;
    values[i] = strtol(token.c_str(), &temp, 16);
    if (*temp != '\0') return false;
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
