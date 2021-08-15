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

#include "wificond/net/mlme_event.h"

#include <vector>

#include <android-base/logging.h>

#include "wificond/net/kernel-header-latest/nl80211.h"
#include "wificond/net/nl80211_packet.h"

using std::array;
using std::unique_ptr;
using std::vector;

namespace android {
namespace wificond {

namespace {

bool GetCommonFields(const NL80211Packet* packet,
                     uint32_t* if_index,
                     array<uint8_t, ETH_ALEN>* bssid) {
  if (!packet->GetAttributeValue(NL80211_ATTR_IFINDEX, if_index)) {
     LOG(ERROR) << "Failed to get NL80211_ATTR_IFINDEX";
     return false;
  }
  // Some MLME events do not contain MAC address.
  if (!packet->GetAttributeValue(NL80211_ATTR_MAC, bssid)) {
    LOG(DEBUG) << "Failed to get NL80211_ATTR_MAC";
  }
  return true;
}

}  // namespace

unique_ptr<MlmeAssociateEvent> MlmeAssociateEvent::InitFromPacket(
    const NL80211Packet* packet) {
  if (packet->GetCommand() != NL80211_CMD_ASSOCIATE) {
    return nullptr;
  }
  unique_ptr<MlmeAssociateEvent> associate_event(new MlmeAssociateEvent());

  if (!GetCommonFields(packet,
                       &(associate_event->interface_index_),
                       &(associate_event->bssid_))){
    return nullptr;
  }
  // According to wpa_supplicant, status code of an ASSOCIATE event should be
  // parsed from NL80211_ATTR_FRAME attribute.
  // TODO(nywang): Parse NL80211_ATTR_FRAME 80211 management frame and get
  // status code.
  associate_event->status_code_ = 0;
  associate_event->is_timeout_ = packet->HasAttribute(NL80211_ATTR_TIMED_OUT);

  return associate_event;
}

unique_ptr<MlmeConnectEvent> MlmeConnectEvent::InitFromPacket(
    const NL80211Packet* packet) {
  if (packet->GetCommand() != NL80211_CMD_CONNECT) {
    return nullptr;
  }
  unique_ptr<MlmeConnectEvent> connect_event(new MlmeConnectEvent());
  if (!GetCommonFields(packet,
                       &(connect_event->interface_index_),
                       &(connect_event->bssid_))){
    return nullptr;
  }

  if (!packet->GetAttributeValue(NL80211_ATTR_STATUS_CODE,
                                 &(connect_event->status_code_))) {
    LOG(WARNING) << "Failed to get NL80211_ATTR_STATUS_CODE";
    connect_event->status_code_ = 0;
  }
  connect_event->is_timeout_ = packet->HasAttribute(NL80211_ATTR_TIMED_OUT);

  return connect_event;
}

unique_ptr<MlmeRoamEvent> MlmeRoamEvent::InitFromPacket(
    const NL80211Packet* packet) {
  if (packet->GetCommand() != NL80211_CMD_ROAM) {
    return nullptr;
  }
  unique_ptr<MlmeRoamEvent> roam_event(new MlmeRoamEvent());
  if (!GetCommonFields(packet,
                       &(roam_event->interface_index_),
                       &(roam_event->bssid_))){
    return nullptr;
  }

  return roam_event;
}

unique_ptr<MlmeDisconnectEvent> MlmeDisconnectEvent::InitFromPacket(
    const NL80211Packet* packet) {
  if (packet->GetCommand() != NL80211_CMD_DISCONNECT) {
    return nullptr;
  }
  unique_ptr<MlmeDisconnectEvent> disconnect_event(new MlmeDisconnectEvent());
  if (!GetCommonFields(packet,
                       &(disconnect_event->interface_index_),
                       &(disconnect_event->bssid_))){
    return nullptr;
  }
  return disconnect_event;
}

unique_ptr<MlmeDisassociateEvent> MlmeDisassociateEvent::InitFromPacket(
    const NL80211Packet* packet) {
  if (packet->GetCommand() != NL80211_CMD_DISASSOCIATE) {
    return nullptr;
  }
  unique_ptr<MlmeDisassociateEvent> disassociate_event(new MlmeDisassociateEvent());
  if (!GetCommonFields(packet,
                       &(disassociate_event->interface_index_),
                       &(disassociate_event->bssid_))){
    return nullptr;
  }
  return disassociate_event;
}

}  // namespace wificond
}  // namespace android
