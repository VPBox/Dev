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

#pragma once

#include <array>
#include <mutex>
#include <string>

#include "raw_address.h"

namespace bluetooth {
namespace common {

class AddressObfuscator {
 public:
  static constexpr unsigned int kOctet32Length = 32;
  using Octet32 = std::array<uint8_t, kOctet32Length>;
  static AddressObfuscator* GetInstance() {
    static auto instance = new AddressObfuscator();
    return instance;
  }

  /**
   * Return true if the input salt is valid
   * Criteria:
   * - Salt must be non-zero
   *
   * @param salt_256bit
   * @return true if the salt is valid
   */
  static bool IsSaltValid(const Octet32& salt_256bit);

  /**
   * Initialize this obfuscator with necessary parameters
   *
   * @param salt_256bit a 256 bit salt used to hash the fixed length address
   */
  void Initialize(const Octet32& salt_256bit);

  /**
   * Return true if Initialize() was called earlier
   */
  bool IsInitialized();

  /**
   * Obfuscate Bluetooth MAC address into an anonymous ID string
   *
   * @param address Bluetooth MAC address to be obfuscated
   * @return the obfuscated MAC address in 256 bit
   */
  std::string Obfuscate(const RawAddress& address);

 private:
  AddressObfuscator() : salt_256bit_({0}) {}
  Octet32 salt_256bit_;
  std::recursive_mutex instance_mutex_;
};

}  // namespace common
}  // namespace bluetooth