/******************************************************************************
 *
 *  Copyright 2018 Google, Inc.
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

#include "address_obfuscator.h"

#include <algorithm>

#include <base/logging.h>
#include <openssl/hmac.h>

#include "bt_trace.h"

namespace bluetooth {
namespace common {

bool AddressObfuscator::IsSaltValid(const Octet32& salt_256bit) {
  return !std::all_of(salt_256bit.begin(), salt_256bit.end(),
                      [](uint8_t i) { return i == 0; });
}

void AddressObfuscator::Initialize(const Octet32& salt_256bit) {
  std::lock_guard<std::recursive_mutex> lock(instance_mutex_);
  salt_256bit_ = salt_256bit;
}

bool AddressObfuscator::IsInitialized() {
  std::lock_guard<std::recursive_mutex> lock(instance_mutex_);
  return IsSaltValid(salt_256bit_);
}

std::string AddressObfuscator::Obfuscate(const RawAddress& address) {
  std::lock_guard<std::recursive_mutex> lock(instance_mutex_);
  CHECK(IsInitialized());
  std::array<uint8_t, EVP_MAX_MD_SIZE> result = {};
  unsigned int out_len = 0;
  CHECK(::HMAC(EVP_sha256(), salt_256bit_.data(), salt_256bit_.size(),
               address.address, address.kLength, result.data(),
               &out_len) != nullptr);
  CHECK_EQ(out_len, static_cast<unsigned int>(kOctet32Length));
  return std::string(reinterpret_cast<const char*>(result.data()), out_len);
}

}  // namespace common
}  // namespace bluetooth