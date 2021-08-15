/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef WIFICOND_NET_MLME_EVENT_H_
#define WIFICOND_NET_MLME_EVENT_H_

#include <array>
#include <memory>

#include <linux/if_ether.h>

#include <android-base/macros.h>

namespace android {
namespace wificond {

class NL80211Packet;

class MlmeConnectEvent {
 public:
  static std::unique_ptr<MlmeConnectEvent> InitFromPacket(
      const NL80211Packet* packet);
  // Returns the BSSID of the associated AP.
  const std::array<uint8_t, ETH_ALEN>& GetBSSID() const { return bssid_; }
  // Get the status code of this connect event.
  // 0 = success, non-zero = failure.
  // Status codes definition: IEEE 802.11-2012, 8.4.1.9, Table 8-37
  uint16_t GetStatusCode() const { return status_code_; }
  uint32_t GetInterfaceIndex() const { return interface_index_; }
  bool IsTimeout() const { return is_timeout_; }

 private:
  MlmeConnectEvent() = default;

  uint32_t interface_index_;
  std::array<uint8_t, ETH_ALEN> bssid_;
  uint16_t status_code_;
  bool is_timeout_;

  DISALLOW_COPY_AND_ASSIGN(MlmeConnectEvent);
};

class MlmeAssociateEvent {
 public:
  static std::unique_ptr<MlmeAssociateEvent> InitFromPacket(
      const NL80211Packet* packet);
  // Returns the BSSID of the associated AP.
  const std::array<uint8_t, ETH_ALEN>& GetBSSID() const { return bssid_; }
  // Get the status code of this associate event.
  // 0 = success, non-zero = failure.
  // Status codes definition: IEEE 802.11-2012, 8.4.1.9, Table 8-37
  uint16_t GetStatusCode() const { return status_code_; }
  uint32_t GetInterfaceIndex() const { return interface_index_; }
  bool IsTimeout() const { return is_timeout_; }

 private:
  MlmeAssociateEvent() = default;

  uint32_t interface_index_;
  std::array<uint8_t, ETH_ALEN> bssid_;
  uint16_t status_code_;
  bool is_timeout_;

  DISALLOW_COPY_AND_ASSIGN(MlmeAssociateEvent);
};

class MlmeRoamEvent {
 public:
  static std::unique_ptr<MlmeRoamEvent> InitFromPacket(
      const NL80211Packet* packet);
  // Returns the BSSID of the associated AP.
  const std::array<uint8_t, ETH_ALEN>& GetBSSID() const { return bssid_; }
  uint32_t GetInterfaceIndex() const { return interface_index_; }

 private:
  MlmeRoamEvent() = default;

  uint32_t interface_index_;
  std::array<uint8_t, ETH_ALEN> bssid_;

  DISALLOW_COPY_AND_ASSIGN(MlmeRoamEvent);
};


class MlmeDisconnectEvent {
 public:
  static std::unique_ptr<MlmeDisconnectEvent> InitFromPacket(
      const NL80211Packet* packet);
  uint32_t GetInterfaceIndex() const { return interface_index_; }
 private:
  MlmeDisconnectEvent() = default;

  uint32_t interface_index_;
  std::array<uint8_t, ETH_ALEN> bssid_;

  DISALLOW_COPY_AND_ASSIGN(MlmeDisconnectEvent);
};

class MlmeDisassociateEvent {
 public:
  static std::unique_ptr<MlmeDisassociateEvent> InitFromPacket(
      const NL80211Packet* packet);
  uint32_t GetInterfaceIndex() const { return interface_index_; }
 private:
  MlmeDisassociateEvent() = default;

  uint32_t interface_index_;
  std::array<uint8_t, ETH_ALEN> bssid_;

  DISALLOW_COPY_AND_ASSIGN(MlmeDisassociateEvent);
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_NET_MLME_EVENT_H_
