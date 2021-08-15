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

#include "address.h"

#include <stdint.h>
#include <algorithm>
#include <sstream>
#include <vector>

namespace bluetooth {
namespace common {

static_assert(sizeof(Address) == 6, "Address must be 6 bytes long!");

const Address Address::kAny{{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};
const Address Address::kEmpty{{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

Address::Address(const uint8_t (&addr)[6]) {
  std::copy(addr, addr + kLength, address);
};

std::string Address::ToString() const {
  char buffer[] = "00:00:00:00:00:00";
  std::snprintf(&buffer[0], sizeof(buffer),
      "%02x:%02x:%02x:%02x:%02x:%02x", address[5], address[4], address[3], address[2], address[1], address[0]);
  std::string str(buffer);
  return str;
}

bool Address::FromString(const std::string& from, Address& to) {
  Address new_addr;
  if (from.length() != 17) {
    return false;
  }

  std::istringstream stream(from);
  std::string token;
  int index = 0;
  while (getline(stream, token, ':')) {
    if (index >= 6) {
      return false;
    }

    if (token.length() != 2) {
      return false;
    }

    char* temp = nullptr;
    new_addr.address[5 - index] = strtol(token.c_str(), &temp, 16);
    if (*temp != '\0') {
      return false;
    }

    index++;
  }

  if (index != 6) {
    return false;
  }

  to = new_addr;
  return true;
}

size_t Address::FromOctets(const uint8_t* from) {
  std::copy(from, from + kLength, address);
  return kLength;
};

bool Address::IsValidAddress(const std::string& address) {
  Address tmp;
  return Address::FromString(address, tmp);
}

}  // namespace common
}  // namespace bluetooth
