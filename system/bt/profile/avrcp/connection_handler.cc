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

#include "connection_handler.h"

#include <base/bind.h>
#include <base/logging.h>
#include <map>

#include "avrc_defs.h"
#include "avrcp_message_converter.h"
#include "bt_types.h"
#include "btu.h"
#include "packet/avrcp/avrcp_packet.h"
// TODO (apanicke): Remove dependency on this header once we cleanup feature
// handling.
#include "bta/include/bta_av_api.h"
#include "device/include/interop.h"
#include "osi/include/allocator.h"
#include "osi/include/properties.h"

namespace bluetooth {
namespace avrcp {

ConnectionHandler* ConnectionHandler::instance_ = nullptr;

ConnectionHandler* ConnectionHandler::Get() {
  CHECK(instance_);

  return instance_;
}

bool IsAbsoluteVolumeEnabled(const RawAddress* bdaddr) {
  char volume_disabled[PROPERTY_VALUE_MAX] = {0};
  osi_property_get("persist.bluetooth.disableabsvol", volume_disabled, "false");
  if (strncmp(volume_disabled, "true", 4) == 0) {
    LOG(INFO) << "Absolute volume disabled by property";
    return false;
  }
  if (interop_match_addr(INTEROP_DISABLE_ABSOLUTE_VOLUME, bdaddr)) {
    LOG(INFO) << "Absolute volume disabled by IOP table";
    return false;
  }
  return true;
}

bool ConnectionHandler::Initialize(const ConnectionCallback& callback,
                                   AvrcpInterface* avrcp, SdpInterface* sdp,
                                   VolumeInterface* vol) {
  CHECK(instance_ == nullptr);
  CHECK(avrcp != nullptr);
  CHECK(sdp != nullptr);

  // TODO (apanicke): When transitioning to using this service, implement
  // SDP Initialization for AVRCP Here.
  instance_ = new ConnectionHandler();
  instance_->connection_cb_ = callback;
  instance_->avrc_ = avrcp;
  instance_->sdp_ = sdp;
  instance_->vol_ = vol;

  // Set up the AVRCP acceptor connection
  if (!instance_->AvrcpConnect(false, RawAddress::kAny)) {
    instance_->CleanUp();
    return false;
  }

  return true;
}

bool ConnectionHandler::CleanUp() {
  CHECK(instance_ != nullptr);

  // TODO (apanicke): Cleanup the SDP Entries here
  for (const auto& entry : instance_->device_map_) {
    entry.second->DeviceDisconnected();
    instance_->avrc_->Close(entry.first);
  }
  instance_->device_map_.clear();
  instance_->feature_map_.clear();

  instance_->weak_ptr_factory_.InvalidateWeakPtrs();

  delete instance_;
  instance_ = nullptr;

  return true;
}

void ConnectionHandler::InitForTesting(ConnectionHandler* handler) {
  CHECK(instance_ == nullptr);
  instance_ = handler;
}

bool ConnectionHandler::ConnectDevice(const RawAddress& bdaddr) {
  LOG(INFO) << "Attempting to connect to device " << bdaddr;

  for (const auto& pair : device_map_) {
    if (bdaddr == pair.second->GetAddress()) {
      LOG(WARNING) << "Already connected to device with address " << bdaddr;
      return false;
    }
  }

  auto connection_lambda = [](ConnectionHandler* instance_,
                              const RawAddress& bdaddr, uint16_t status,
                              uint16_t version, uint16_t features) {
    LOG(INFO) << __PRETTY_FUNCTION__
              << " SDP Completed features=" << loghex(features);
    if (status != AVRC_SUCCESS || !(features & BTA_AV_FEAT_RCCT)) {
      LOG(ERROR) << "Failed to do SDP: status=" << loghex(status)
                 << " features=" << loghex(features)
                 << " supports controller: " << (features & BTA_AV_FEAT_RCCT);
      instance_->connection_cb_.Run(std::shared_ptr<Device>());
    }

    instance_->feature_map_.emplace(bdaddr, features);
    instance_->AvrcpConnect(true, bdaddr);
    return;
  };

  return SdpLookup(bdaddr, base::Bind(connection_lambda, this, bdaddr));
}

bool ConnectionHandler::DisconnectDevice(const RawAddress& bdaddr) {
  for (auto it = device_map_.begin(); it != device_map_.end(); it++) {
    if (bdaddr == it->second->GetAddress()) {
      uint8_t handle = it->first;
      return avrc_->Close(handle) == AVRC_SUCCESS;
    }
  }

  return false;
}

std::vector<std::shared_ptr<Device>> ConnectionHandler::GetListOfDevices()
    const {
  std::vector<std::shared_ptr<Device>> list;
  for (const auto& device : device_map_) {
    list.push_back(device.second);
  }
  return list;
}

bool ConnectionHandler::SdpLookup(const RawAddress& bdaddr, SdpCallback cb) {
  LOG(INFO) << __PRETTY_FUNCTION__;

  tAVRC_SDP_DB_PARAMS db_params;
  // TODO (apanicke): This needs to be replaced with smarter memory management.
  tSDP_DISCOVERY_DB* disc_db =
      (tSDP_DISCOVERY_DB*)osi_malloc(BT_DEFAULT_BUFFER_SIZE);
  uint16_t attr_list[] = {ATTR_ID_SERVICE_CLASS_ID_LIST,
                          ATTR_ID_BT_PROFILE_DESC_LIST,
                          ATTR_ID_SUPPORTED_FEATURES};

  db_params.db_len =
      BT_DEFAULT_BUFFER_SIZE;  // Some magic number found in the AVRCP code
  db_params.num_attr = sizeof(attr_list) / sizeof(attr_list[0]);
  db_params.p_db = disc_db;
  db_params.p_attrs = attr_list;

  return avrc_->FindService(
             UUID_SERVCLASS_AV_REMOTE_CONTROL, bdaddr, &db_params,
             base::Bind(&ConnectionHandler::SdpCb,
                        weak_ptr_factory_.GetWeakPtr(), bdaddr, cb, disc_db)) ==
         AVRC_SUCCESS;
}

bool ConnectionHandler::AvrcpConnect(bool initiator, const RawAddress& bdaddr) {
  LOG(INFO) << "Connect to device " << bdaddr.ToString();

  tAVRC_CONN_CB open_cb;
  if (initiator) {
    open_cb.ctrl_cback = base::Bind(&ConnectionHandler::InitiatorControlCb,
                                    weak_ptr_factory_.GetWeakPtr());
  } else {
    open_cb.ctrl_cback = base::Bind(&ConnectionHandler::AcceptorControlCb,
                                    weak_ptr_factory_.GetWeakPtr());
  }
  open_cb.msg_cback =
      base::Bind(&ConnectionHandler::MessageCb, weak_ptr_factory_.GetWeakPtr());
  open_cb.company_id = AVRC_CO_GOOGLE;
  open_cb.conn = initiator ? AVRC_CONN_INT
                           : AVRC_CONN_ACP;  // 0 if initiator, 1 if acceptor
  // TODO (apanicke): We shouldn't need RCCT to do absolute volume. The current
  // AVRC_API requires it though.
  open_cb.control = BTA_AV_FEAT_RCTG | BTA_AV_FEAT_RCCT | BTA_AV_FEAT_METADATA;

  uint8_t handle = 0;
  uint16_t status = avrc_->Open(&handle, &open_cb, bdaddr);
  LOG(INFO) << __PRETTY_FUNCTION__ << ": handle=" << loghex(handle)
            << " status= " << loghex(status);
  return status == AVRC_SUCCESS;
}

void ConnectionHandler::InitiatorControlCb(uint8_t handle, uint8_t event,
                                           uint16_t result,
                                           const RawAddress* peer_addr) {
  DCHECK(!connection_cb_.is_null());

  LOG(INFO) << __PRETTY_FUNCTION__ << ": handle=" << loghex(handle)
            << " result=" << loghex(result)
            << " addr=" << (peer_addr ? peer_addr->ToString() : "none");

  switch (event) {
    case AVRC_OPEN_IND_EVT: {
      LOG(INFO) << __PRETTY_FUNCTION__ << ": Connection Opened Event";

      const auto& feature_iter = feature_map_.find(*peer_addr);
      if (feature_iter == feature_map_.end()) {
        LOG(ERROR) << "Features do not exist even though SDP should have been "
                      "done first";
        return;
      }

      bool supports_browsing = feature_iter->second & BTA_AV_FEAT_BROWSE;

      if (supports_browsing) {
        avrc_->OpenBrowse(handle, AVCT_INT);
      }

      // TODO (apanicke): Implement a system to cache SDP entries. For most
      // devices SDP is completed after the device connects AVRCP so that
      // information isn't very useful when trying to control our
      // capabilities. For now always use AVRCP 1.6.
      auto&& callback = base::Bind(&ConnectionHandler::SendMessage,
                                   base::Unretained(this), handle);
      auto&& ctrl_mtu = avrc_->GetPeerMtu(handle) - AVCT_HDR_LEN;
      auto&& browse_mtu = avrc_->GetBrowseMtu(handle) - AVCT_HDR_LEN;
      std::shared_ptr<Device> newDevice = std::make_shared<Device>(
          *peer_addr, !supports_browsing, callback, ctrl_mtu, browse_mtu);

      device_map_[handle] = newDevice;
      // TODO (apanicke): Create the device with all of the interfaces it
      // needs. Return the new device where the service will register the
      // interfaces it needs.
      connection_cb_.Run(newDevice);

      if (feature_iter->second & BTA_AV_FEAT_ADV_CTRL) {
        newDevice->RegisterVolumeChanged();
      } else if (instance_->vol_ != nullptr) {
        instance_->vol_->DeviceConnected(newDevice->GetAddress());
      }

    } break;

    case AVRC_CLOSE_IND_EVT: {
      LOG(INFO) << __PRETTY_FUNCTION__ << ": Connection Closed Event";

      if (device_map_.find(handle) == device_map_.end()) {
        LOG(WARNING)
            << "Connection Close received from device that doesn't exist";
        return;
      }
      avrc_->Close(handle);
      feature_map_.erase(device_map_[handle]->GetAddress());
      device_map_[handle]->DeviceDisconnected();
      device_map_.erase(handle);
    } break;

    case AVRC_BROWSE_OPEN_IND_EVT: {
      LOG(INFO) << __PRETTY_FUNCTION__ << ": Browse Open Event";
      // NOTE (apanicke): We don't need to explicitly handle this message
      // since the AVCTP Layer will still send us browsing messages
      // regardless. It would be useful to note this though for future
      // compatibility issues.
      if (device_map_.find(handle) == device_map_.end()) {
        LOG(WARNING) << "Browse Opened received from device that doesn't exist";
        return;
      }

      auto browse_mtu = avrc_->GetBrowseMtu(handle) - AVCT_HDR_LEN;
      device_map_[handle]->SetBrowseMtu(browse_mtu);
    } break;
    case AVRC_BROWSE_CLOSE_IND_EVT:
      LOG(INFO) << __PRETTY_FUNCTION__ << ": Browse Close Event";
      break;
    default:
      LOG(ERROR) << "Unknown AVRCP Control event";
      break;
  }
}

void ConnectionHandler::AcceptorControlCb(uint8_t handle, uint8_t event,
                                          uint16_t result,
                                          const RawAddress* peer_addr) {
  DCHECK(!connection_cb_.is_null());

  LOG(INFO) << __PRETTY_FUNCTION__ << ": handle=" << loghex(handle)
            << " result=" << loghex(result)
            << " addr=" << (peer_addr ? peer_addr->ToString() : "none");

  switch (event) {
    case AVRC_OPEN_IND_EVT: {
      LOG(INFO) << __PRETTY_FUNCTION__ << ": Connection Opened Event";

      auto&& callback = base::Bind(&ConnectionHandler::SendMessage,
                                   weak_ptr_factory_.GetWeakPtr(), handle);
      auto&& ctrl_mtu = avrc_->GetPeerMtu(handle) - AVCT_HDR_LEN;
      auto&& browse_mtu = avrc_->GetBrowseMtu(handle) - AVCT_HDR_LEN;
      std::shared_ptr<Device> newDevice = std::make_shared<Device>(
          *peer_addr, false, callback, ctrl_mtu, browse_mtu);

      device_map_[handle] = newDevice;
      connection_cb_.Run(newDevice);

      LOG(INFO) << __PRETTY_FUNCTION__
                << ": Performing SDP on connected device. address="
                << peer_addr->ToString();
      auto sdp_lambda = [](ConnectionHandler* instance_, uint8_t handle,
                           uint16_t status, uint16_t version,
                           uint16_t features) {
        if (instance_->device_map_.find(handle) ==
            instance_->device_map_.end()) {
          LOG(WARNING) << __PRETTY_FUNCTION__
                       << ": No device found for handle: " << loghex(handle);
          return;
        }

        auto device = instance_->device_map_[handle];
        instance_->feature_map_.emplace(device->GetAddress(), features);

        // TODO (apanicke): Report to the VolumeInterface that a new Device is
        // connected that doesn't support absolute volume.
        if (features & BTA_AV_FEAT_ADV_CTRL) {
          device->RegisterVolumeChanged();
        } else if (instance_->vol_ != nullptr) {
          instance_->vol_->DeviceConnected(device->GetAddress());
        }
      };

      SdpLookup(*peer_addr, base::Bind(sdp_lambda, this, handle));

      avrc_->OpenBrowse(handle, AVCT_ACP);
      AvrcpConnect(false, RawAddress::kAny);
    } break;

    case AVRC_CLOSE_IND_EVT: {
      LOG(INFO) << __PRETTY_FUNCTION__ << ": Connection Closed Event";

      if (device_map_.find(handle) == device_map_.end()) {
        LOG(WARNING)
            << "Connection Close received from device that doesn't exist";
        return;
      }
      avrc_->Close(handle);
      feature_map_.erase(device_map_[handle]->GetAddress());
      device_map_[handle]->DeviceDisconnected();
      device_map_.erase(handle);
    } break;

    case AVRC_BROWSE_OPEN_IND_EVT: {
      LOG(INFO) << __PRETTY_FUNCTION__ << ": Browse Open Event";
      // NOTE (apanicke): We don't need to explicitly handle this message
      // since the AVCTP Layer will still send us browsing messages
      // regardless. It would be useful to note this though for future
      // compatibility issues.
      if (device_map_.find(handle) == device_map_.end()) {
        LOG(WARNING) << "Browse Opened received from device that doesn't exist";
        return;
      }

      auto browse_mtu = avrc_->GetBrowseMtu(handle) - AVCT_HDR_LEN;
      device_map_[handle]->SetBrowseMtu(browse_mtu);
    } break;
    case AVRC_BROWSE_CLOSE_IND_EVT:
      LOG(INFO) << __PRETTY_FUNCTION__ << ": Browse Close Event";
      break;
    default:
      LOG(ERROR) << "Unknown AVRCP Control event";
      break;
  }
}

void ConnectionHandler::MessageCb(uint8_t handle, uint8_t label, uint8_t opcode,
                                  tAVRC_MSG* p_msg) {
  if (device_map_.find(handle) == device_map_.end()) {
    LOG(ERROR) << "Message received for unconnected device: handle="
               << loghex(handle);
    return;
  }

  auto pkt = AvrcpMessageConverter::Parse(p_msg);

  if (opcode == AVRC_OP_BROWSE) {
    VLOG(4) << "Browse Message received on handle " << (unsigned int)handle;
    device_map_[handle]->BrowseMessageReceived(label, BrowsePacket::Parse(pkt));
    return;
  }

  VLOG(4) << "Message received on handle " << (unsigned int)handle;
  device_map_[handle]->MessageReceived(label, Packet::Parse(pkt));
}

void ConnectionHandler::SdpCb(const RawAddress& bdaddr, SdpCallback cb,
                              tSDP_DISCOVERY_DB* disc_db, uint16_t status) {
  LOG(INFO) << __PRETTY_FUNCTION__ << ": SDP lookup callback received";

  if (status != AVRC_SUCCESS) {
    LOG(ERROR) << __PRETTY_FUNCTION__
               << ": SDP Failure: status = " << (unsigned int)status;
    cb.Run(status, 0, 0);
    osi_free(disc_db);
    return;
  }

  // Check the peer features
  tSDP_DISC_REC* sdp_record = nullptr;
  uint16_t peer_features = 0;
  uint16_t peer_avrcp_version = 0;

  // TODO (apanicke): Replace this in favor of our own supported features.
  sdp_record =
      sdp_->FindServiceInDb(disc_db, UUID_SERVCLASS_AV_REMOTE_CONTROL, nullptr);
  if (sdp_record != nullptr) {
    LOG(INFO) << __PRETTY_FUNCTION__ << ": Device " << bdaddr.ToString()
              << " supports remote control";
    peer_features |= BTA_AV_FEAT_RCCT;

    if ((sdp_->FindAttributeInRec(sdp_record, ATTR_ID_BT_PROFILE_DESC_LIST)) !=
        NULL) {
      /* get profile version (if failure, version parameter is not updated) */
      sdp_->FindProfileVersionInRec(
          sdp_record, UUID_SERVCLASS_AV_REMOTE_CONTROL, &peer_avrcp_version);
      LOG(INFO) << __PRETTY_FUNCTION__ << ": Device " << bdaddr.ToString()
                << " peer avrcp version=" << loghex(peer_avrcp_version);

      if (peer_avrcp_version >= AVRC_REV_1_3) {
        // These are the standard features, another way to check this is to
        // search for CAT1 on the remote device
        LOG(INFO) << __PRETTY_FUNCTION__ << ": Device " << bdaddr.ToString()
                  << " supports metadata";
        peer_features |= (BTA_AV_FEAT_VENDOR | BTA_AV_FEAT_METADATA);
      }
      if (peer_avrcp_version >= AVRC_REV_1_4) {
        /* get supported categories */
        LOG(INFO) << __PRETTY_FUNCTION__ << " Get Supported categories";
        tSDP_DISC_ATTR* sdp_attribute =
            sdp_->FindAttributeInRec(sdp_record, ATTR_ID_SUPPORTED_FEATURES);
        if (sdp_attribute != NULL) {
          LOG(INFO) << __PRETTY_FUNCTION__
                    << "Get Supported categories SDP ATTRIBUTES != null";
          uint16_t categories = sdp_attribute->attr_value.v.u16;
          if (categories & AVRC_SUPF_CT_CAT2) {
            LOG(INFO) << __PRETTY_FUNCTION__ << ": Device " << bdaddr.ToString()
                      << " supports advanced control";
            if (IsAbsoluteVolumeEnabled(&bdaddr)) {
              peer_features |= (BTA_AV_FEAT_ADV_CTRL);
            }
          }
          if (categories & AVRC_SUPF_CT_BROWSE) {
            LOG(INFO) << __PRETTY_FUNCTION__ << ": Device " << bdaddr.ToString()
                      << " supports browsing";
            peer_features |= (BTA_AV_FEAT_BROWSE);
          }
        }
      }
    }
  }

  sdp_record = sdp_->FindServiceInDb(disc_db, UUID_SERVCLASS_AV_REM_CTRL_TARGET,
                                     nullptr);
  if (sdp_record != nullptr) {
    LOG(INFO) << __PRETTY_FUNCTION__ << ": Device " << bdaddr.ToString()
              << " supports remote control target";

    uint16_t peer_avrcp_target_version = 0;
    sdp_->FindProfileVersionInRec(sdp_record, UUID_SERVCLASS_AV_REMOTE_CONTROL,
                                  &peer_avrcp_target_version);
    LOG(INFO) << __PRETTY_FUNCTION__ << ": Device " << bdaddr.ToString()
              << " peer avrcp target version="
              << loghex(peer_avrcp_target_version);

    if ((sdp_->FindAttributeInRec(sdp_record, ATTR_ID_BT_PROFILE_DESC_LIST)) !=
        NULL) {
      if (peer_avrcp_target_version >= AVRC_REV_1_4) {
        /* get supported categories */
        LOG(INFO) << __PRETTY_FUNCTION__ << " Get Supported categories";
        tSDP_DISC_ATTR* sdp_attribute =
            sdp_->FindAttributeInRec(sdp_record, ATTR_ID_SUPPORTED_FEATURES);
        if (sdp_attribute != NULL) {
          LOG(INFO) << __PRETTY_FUNCTION__
                    << "Get Supported categories SDP ATTRIBUTES != null";
          uint16_t categories = sdp_attribute->attr_value.v.u16;
          if (categories & AVRC_SUPF_CT_CAT2) {
            LOG(INFO) << __PRETTY_FUNCTION__ << ": Device " << bdaddr.ToString()
                      << " supports advanced control";
            if (IsAbsoluteVolumeEnabled(&bdaddr)) {
              peer_features |= (BTA_AV_FEAT_ADV_CTRL);
            }
          }
        }
      }
    }
  }

  osi_free(disc_db);

  cb.Run(status, peer_avrcp_version, peer_features);
}

void ConnectionHandler::SendMessage(
    uint8_t handle, uint8_t label, bool browse,
    std::unique_ptr<::bluetooth::PacketBuilder> message) {
  std::shared_ptr<::bluetooth::Packet> packet = VectorPacket::Make();
  message->Serialize(packet);

  uint8_t ctype = AVRC_RSP_ACCEPT;
  if (!browse) {
    ctype =
        (uint8_t)(::bluetooth::Packet::Specialize<Packet>(packet)->GetCType());
  }

  DLOG(INFO) << "SendMessage to handle=" << loghex(handle);

  BT_HDR* pkt = (BT_HDR*)osi_malloc(BT_DEFAULT_BUFFER_SIZE);

  pkt->offset = AVCT_MSG_OFFSET;
  // TODO (apanicke): Update this constant. Currently this is a unique event
  // used to tell the AVRCP API layer that the data is properly formatted and
  // doesn't need to be processed. In the future, this is the only place sending
  // the packet so none of these layer specific fields will be used.
  pkt->event = 0xFFFF;

  // TODO (apanicke): This layer specific stuff can go away once we move over
  // to the new service.
  pkt->layer_specific = AVCT_DATA_CTRL;
  if (browse) {
    pkt->layer_specific = AVCT_DATA_BROWSE;
  }

  pkt->len = packet->size();
  uint8_t* p_data = (uint8_t*)(pkt + 1) + pkt->offset;
  for (auto it = packet->begin(); it != packet->end(); it++) {
    *p_data++ = *it;
  }

  avrc_->MsgReq(handle, label, ctype, pkt);
}

}  // namespace avrcp
}  // namespace bluetooth
