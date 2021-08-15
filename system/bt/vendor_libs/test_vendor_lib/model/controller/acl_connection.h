/*
 * Copyright 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <cstdint>

#include "types/address.h"

namespace test_vendor_lib {

// Model the connection of a device to the controller.
class AclConnection {
 public:
  AclConnection(const Address& addr) : address_(addr), connected_(false), encrypted_(false) {}

  virtual ~AclConnection() = default;

  void SetConnected(bool connected) {
    connected_ = connected;
  };
  bool IsConnected() const {
    return connected_;
  };

  void Encrypt() {
    encrypted_ = true;
  };
  bool IsEncrypted() const {
    return encrypted_;
  };

  const Address& GetAddress() const {
    return address_;
  }
  void SetAddress(const Address& address) {
    address_ = address;
  }

 private:
  Address address_;

  // State variables
  bool connected_;
  bool encrypted_;
};

}  // namespace test_vendor_lib
