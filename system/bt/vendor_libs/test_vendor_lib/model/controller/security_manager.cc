/*
 * Copyright 2017 The Android Open Source Project
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

#define LOG_TAG "security_manager"

#include "security_manager.h"

#include "base/logging.h"

using std::vector;

namespace test_vendor_lib {

uint16_t SecurityManager::DeleteAllKeys() {
  uint16_t size = key_store_.size();
  key_store_.clear();
  return size;
}

uint16_t SecurityManager::DeleteKey(const Address& addr) {
  uint16_t count = key_store_.count(addr.ToString());
  if (count) {
    key_store_.erase(addr.ToString());
  }
  return count;
}

uint16_t SecurityManager::ReadAllKeys() const {
  return key_store_.size();
}

uint16_t SecurityManager::ReadKey(const Address& addr) const {
  return key_store_.count(addr.ToString());
}

uint16_t SecurityManager::WriteKey(const Address& addr, const std::vector<uint8_t>& key) {
  if (key_store_.size() >= max_keys_) {
    return 0;
  }
  key_store_[addr.ToString()] = key;
  return 1;
}

const std::vector<uint8_t>& SecurityManager::GetKey(const Address& addr) const {
  CHECK(ReadKey(addr)) << "No such key";
  return key_store_.at(addr.ToString());
}

void SecurityManager::AuthenticationRequest(const Address& addr, uint16_t handle) {
  authenticating_ = true;
  current_handle_ = handle;
  peer_address_ = addr;
}

void SecurityManager::AuthenticationRequestFinished() {
  authenticating_ = false;
}

bool SecurityManager::AuthenticationInProgress() {
  return authenticating_;
}

uint16_t SecurityManager::GetAuthenticationHandle() {
  return current_handle_;
}

Address SecurityManager::GetAuthenticationAddress() {
  return peer_address_;
}

void SecurityManager::SetPeerIoCapability(const Address& addr, uint8_t io_capability, uint8_t oob_present_flag,
                                          uint8_t authentication_requirements) {
  CHECK_EQ(addr, peer_address_);
  peer_capabilities_valid_ = true;
  if (io_capability <= static_cast<uint8_t>(IoCapabilityType::NO_INPUT_NO_OUTPUT)) {
    peer_io_capability_ = static_cast<IoCapabilityType>(io_capability);
  } else {
    peer_io_capability_ = IoCapabilityType::INVALID;
    peer_capabilities_valid_ = false;
  }
  peer_oob_present_flag_ = (oob_present_flag == 1);
  if (authentication_requirements <= static_cast<uint8_t>(AuthenticationType::GENERAL_BONDING_MITM)) {
    peer_authentication_requirements_ = static_cast<AuthenticationType>(authentication_requirements);
  } else {
    peer_authentication_requirements_ = AuthenticationType::INVALID;
    peer_capabilities_valid_ = false;
  }
}

void SecurityManager::SetLocalIoCapability(const Address& peer, uint8_t io_capability, uint8_t oob_present_flag,
                                           uint8_t authentication_requirements) {
  CHECK_EQ(peer, peer_address_);
  CHECK(io_capability <= static_cast<uint8_t>(IoCapabilityType::NO_INPUT_NO_OUTPUT))
      << "io_capability = " << io_capability;
  CHECK(oob_present_flag <= 1) << "oob_present_flag = " << oob_present_flag;
  CHECK(authentication_requirements <= static_cast<uint8_t>(AuthenticationType::GENERAL_BONDING_MITM))
      << "authentication_requirements = " << authentication_requirements;
  host_io_capability_ = static_cast<IoCapabilityType>(io_capability);
  host_oob_present_flag_ = (oob_present_flag == 1);
  host_authentication_requirements_ = static_cast<AuthenticationType>(authentication_requirements);
  host_capabilities_valid_ = true;
}

void SecurityManager::InvalidateIoCapabilities() {
  host_capabilities_valid_ = false;
  peer_capabilities_valid_ = false;
}

PairingType SecurityManager::GetSimplePairingType() {
  if (!host_capabilities_valid_ || !peer_capabilities_valid_) {
    return PairingType::INVALID;
  }
  bool host_requires_mitm = (host_authentication_requirements_ == AuthenticationType::NO_BONDING_MITM) ||
                            (host_authentication_requirements_ == AuthenticationType::DEDICATED_BONDING_MITM) ||
                            (host_authentication_requirements_ == AuthenticationType::GENERAL_BONDING_MITM);
  bool peer_requires_mitm = (peer_authentication_requirements_ == AuthenticationType::NO_BONDING_MITM) ||
                            (peer_authentication_requirements_ == AuthenticationType::DEDICATED_BONDING_MITM) ||
                            (peer_authentication_requirements_ == AuthenticationType::GENERAL_BONDING_MITM);
  if (!(peer_requires_mitm || host_requires_mitm)) {
    return PairingType::AUTO_CONFIRMATION;
  }
  return PairingType::INVALID;
}

}  // namespace test_vendor_lib
