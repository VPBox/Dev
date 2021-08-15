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

#define LOG_TAG "acl_connection_handler"

#include "acl_connection_handler.h"

#include "base/logging.h"

#include "osi/include/log.h"
#include "types/address.h"

using std::shared_ptr;

namespace test_vendor_lib {

bool AclConnectionHandler::HasHandle(uint16_t handle) const {
  if (acl_connections_.count(handle) == 0) {
    return false;
  }
  return true;
}

uint16_t AclConnectionHandler::GetUnusedHandle() {
  static uint16_t sNextHandle = acl::kReservedHandle - 2;
  while (acl_connections_.count(sNextHandle) == 1) {
    sNextHandle = (sNextHandle + 1) % acl::kReservedHandle;
  }
  uint16_t unused_handle = sNextHandle;
  sNextHandle = (sNextHandle + 1) % acl::kReservedHandle;
  return unused_handle;
}

bool AclConnectionHandler::CreatePendingConnection(const Address& addr) {
  if ((pending_connections_.size() + 1 > max_pending_connections_) || HasPendingConnection(addr)) {
    return false;
  }
  pending_connections_.insert(addr);
  return true;
}

bool AclConnectionHandler::HasPendingConnection(const Address& addr) {
  return pending_connections_.count(addr) == 1;
}

bool AclConnectionHandler::CancelPendingConnection(const Address& addr) {
  if (!HasPendingConnection(addr)) {
    return false;
  }
  pending_connections_.erase(addr);
  return true;
}

uint16_t AclConnectionHandler::CreateConnection(const Address& addr) {
  if (CancelPendingConnection(addr)) {
    uint16_t handle = GetUnusedHandle();
    acl_connections_.emplace(handle, addr);
    SetConnected(handle, true);
    return handle;
  }
  return acl::kReservedHandle;
}

bool AclConnectionHandler::Disconnect(uint16_t handle) {
  return acl_connections_.erase(handle) > 0;
}

uint16_t AclConnectionHandler::GetHandle(const Address& addr) const {
  for (auto pair : acl_connections_) {
    if (std::get<AclConnection>(pair).GetAddress() == addr) {
      return std::get<0>(pair);
    }
  }
  return acl::kReservedHandle;
}

const Address& AclConnectionHandler::GetAddress(uint16_t handle) const {
  CHECK(HasHandle(handle)) << "Handle unknown " << handle;
  return acl_connections_.at(handle).GetAddress();
}

void AclConnectionHandler::SetConnected(uint16_t handle, bool connected) {
  if (!HasHandle(handle)) {
    return;
  }
  acl_connections_.at(handle).SetConnected(connected);
}

bool AclConnectionHandler::IsConnected(uint16_t handle) const {
  if (!HasHandle(handle)) {
    return false;
  }
  return acl_connections_.at(handle).IsConnected();
}

void AclConnectionHandler::Encrypt(uint16_t handle) {
  if (!HasHandle(handle)) {
    return;
  }
  acl_connections_.at(handle).Encrypt();
}

bool AclConnectionHandler::IsEncrypted(uint16_t handle) const {
  if (!HasHandle(handle)) {
    return false;
  }
  return acl_connections_.at(handle).IsEncrypted();
}

void AclConnectionHandler::SetAddress(uint16_t handle, const Address& address) {
  if (!HasHandle(handle)) {
    return;
  }
  acl_connections_.at(handle).SetAddress(address);
}

}  // namespace test_vendor_lib
