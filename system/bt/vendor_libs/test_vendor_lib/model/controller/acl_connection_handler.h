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
#include <set>
#include <unordered_map>

#include "acl_connection.h"
#include "include/acl.h"
#include "types/address.h"

namespace test_vendor_lib {

class AclConnectionHandler {
 public:
  AclConnectionHandler(size_t max_pending_connections = 1) : max_pending_connections_(max_pending_connections) {}

  virtual ~AclConnectionHandler() = default;

  bool CreatePendingConnection(const Address& addr);
  bool HasPendingConnection(const Address& addr);
  bool CancelPendingConnection(const Address& addr);

  uint16_t CreateConnection(const Address& addr);
  bool Disconnect(uint16_t handle);
  bool HasHandle(uint16_t handle) const;

  uint16_t GetHandle(const Address& addr) const;
  const Address& GetAddress(uint16_t handle) const;

  void SetConnected(uint16_t handle, bool connected);
  bool IsConnected(uint16_t handle) const;

  void Encrypt(uint16_t handle);
  bool IsEncrypted(uint16_t handle) const;

  void SetAddress(uint16_t handle, const Address& address);

 private:
  std::unordered_map<uint16_t, AclConnection> acl_connections_;
  size_t max_pending_connections_;
  std::set<Address> pending_connections_;
  uint16_t GetUnusedHandle();
};

}  // namespace test_vendor_lib
