/******************************************************************************
 *
 *  Copyright 2017 The Android Open Source Project
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

#include "raw_address.h"

#include <base/strings/string_split.h>
#include <base/strings/stringprintf.h>
#include <stdint.h>
#include <algorithm>
#include <vector>

static_assert(sizeof(RawAddress) == 6, "RawAddress must be 6 bytes long!");

const RawAddress RawAddress::kAny{{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};
const RawAddress RawAddress::kEmpty{{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

RawAddress::RawAddress(const uint8_t (&addr)[6]) {
  std::copy(addr, addr + kLength, address);
};

std::string RawAddress::ToString() const {
  return base::StringPrintf("%02x:%02x:%02x:%02x:%02x:%02x", address[0],
                            address[1], address[2], address[3], address[4],
                            address[5]);
}

bool RawAddress::FromString(const std::string& from, RawAddress& to) {
  RawAddress new_addr;
  if (from.length() != 17) return false;

  std::vector<std::string> byte_tokens =
      base::SplitString(from, ":", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);

  if (byte_tokens.size() != 6) return false;

  for (int i = 0; i < 6; i++) {
    const auto& token = byte_tokens[i];

    if (token.length() != 2) return false;

    char* temp = nullptr;
    new_addr.address[i] = strtol(token.c_str(), &temp, 16);
    if (*temp != '\0') return false;
  }

  to = new_addr;
  return true;
}

size_t RawAddress::FromOctets(const uint8_t* from) {
  std::copy(from, from + kLength, address);
  return kLength;
};

bool RawAddress::IsValidAddress(const std::string& address) {
  RawAddress tmp;
  return RawAddress::FromString(address, tmp);
}
