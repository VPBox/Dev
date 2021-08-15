//
//  Copyright 2017 Google, Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at:
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include "service/avrcp_control.h"

#include <cerrno>
#include <climits>
#include <string>

#include <base/logging.h>
#include <base/memory/ptr_util.h>
#include <base/strings/string_number_conversions.h>

#include "service/logging_helpers.h"

#include "stack/include/avrc_defs.h"


namespace bluetooth {

AvrcpControl::AvrcpControl(const Uuid& uuid, int control_id)
    : app_identifier_(uuid), control_id_(control_id) {
  hal::BluetoothAvrcpInterface::Get()->AddControlObserver(this);
}

AvrcpControl::~AvrcpControl() {
  hal::BluetoothAvrcpInterface::Get()->RemoveControlObserver(this);
}

const Uuid& AvrcpControl::GetAppIdentifier() const { return app_identifier_; }

int AvrcpControl::GetInstanceId() const { return control_id_; }

void AvrcpControl::SetDelegate(Delegate* delegate) {
  std::lock_guard<std::mutex> lock(delegate_mutex_);
  delegate_ = delegate;
}

bool AvrcpControl::Enable() {
  std::lock_guard<std::mutex> lock(mutex_);
  return hal::BluetoothAvrcpInterface::Get()->AvrcpControlEnable();
}

void AvrcpControl::Disable() {
  std::lock_guard<std::mutex> lock(mutex_);
  hal::BluetoothAvrcpInterface::Get()->AvrcpControlDisable();
}

bool AvrcpControl::SendPassThroughCommand(const std::string& device_address,
                                          uint8_t key_code, bool key_pressed) {
  RawAddress addr;
  if (!RawAddress::FromString(device_address, addr)) {
    LOG(ERROR) << "Invalid device address given: " << device_address;
    return false;
  }

  uint8_t key_state = key_pressed ? AVRC_STATE_PRESS : AVRC_STATE_RELEASE;
  bt_status_t status;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    status = hal::BluetoothAvrcpInterface::Get()
                 ->GetControlHALInterface()
                 ->send_pass_through_cmd(addr, key_code, key_state);
  }
  if (status != BT_STATUS_SUCCESS) {
    LOG(ERROR) << "Failed to send passthrough command";
    return false;
  }

  return true;
}

bool AvrcpControl::SetAbsVolumeResponse(const std::string& device_address,
                                        int32_t abs_vol, int32_t label) {
  RawAddress addr;
  if (!RawAddress::FromString(device_address, addr)) {
    LOG(ERROR) << "Invalid device address given: " << device_address;
    return false;
  }

  bt_status_t status;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    status = hal::BluetoothAvrcpInterface::Get()
                 ->GetControlHALInterface()
                 ->set_volume_rsp(addr, abs_vol, label);
  }
  if (status != BT_STATUS_SUCCESS) {
    LOG(ERROR) << "Failed to send set absolute volume response";
    return false;
  }

  return true;
}

bool AvrcpControl::RegisterForAbsVolumeCallbackResponse(
    const std::string& device_address, int32_t response_type, int32_t abs_vol,
    int32_t label) {
  RawAddress addr;
  if (!RawAddress::FromString(device_address, addr)) {
    LOG(ERROR) << "Invalid device address given: " << device_address;
    return false;
  }

  bt_status_t status;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    status = hal::BluetoothAvrcpInterface::Get()
                 ->GetControlHALInterface()
                 ->register_abs_vol_rsp(
                     addr, static_cast<btrc_notification_type_t>(response_type),
                     abs_vol, label);
  }
  if (status != BT_STATUS_SUCCESS) {
    LOG(ERROR)
        << "Failed to send send register for absolute volume change callback";
    return false;
  }

  return true;
}

void AvrcpControl::ConnectionStateCallback(bool rc_connect, bool bt_connect,
                                           const RawAddress& bd_addr) {
  std::string device_address = BtAddrString(&bd_addr);
  std::lock_guard<std::mutex> lock(delegate_mutex_);
  if (delegate_)
    delegate_->OnConnectionState(rc_connect, bt_connect, device_address);
}

void AvrcpControl::CtrlSetabsvolCmdCallback(const RawAddress& bd_addr,
                                            uint8_t abs_vol, uint8_t label) {
  std::string device_address = BtAddrString(&bd_addr);
  std::lock_guard<std::mutex> lock(delegate_mutex_);
  if (delegate_)
    delegate_->OnSetAbsVolumeRequest(device_address, abs_vol, label);
}

void AvrcpControl::CtrlRegisternotificationAbsVolCallback(
    const RawAddress& bd_addr, uint8_t label) {
  std::string device_address = BtAddrString(&bd_addr);
  std::lock_guard<std::mutex> lock(delegate_mutex_);
  if (delegate_)
    delegate_->OnRegisterForAbsVolumeCallbackRequest(device_address, label);
}

void AvrcpControl::CtrlTrackChangedCallback(const RawAddress& bd_addr,
                                            uint8_t num_attr,
                                            btrc_element_attr_val_t* p_attrs) {
  std::string device_address = BtAddrString(&bd_addr);

  std::string title;
  std::string artist;
  std::string album;
  std::string genre;
  int track_num = -1;
  int num_tracks = -1;
  int play_time = -1;

  for (size_t i = 0; i < num_attr; ++i) {
    auto attr_text = reinterpret_cast<char*>(p_attrs[i].text);
    switch (p_attrs[i].attr_id) {
      case BTRC_MEDIA_ATTR_ID_TITLE:
        title = attr_text;
        break;
      case BTRC_MEDIA_ATTR_ID_ARTIST:
        artist = attr_text;
        break;
      case BTRC_MEDIA_ATTR_ID_ALBUM:
        album = attr_text;
        break;
      case BTRC_MEDIA_ATTR_ID_TRACK_NUM:
        if (!base::StringToInt(attr_text, &track_num)) {
          LOG(ERROR) << "Failed to parse track number";
        }
        break;
      case BTRC_MEDIA_ATTR_ID_NUM_TRACKS:
        if (!base::StringToInt(attr_text, &num_tracks)) {
          LOG(ERROR) << "Failed to parse number of tracks";
        }
        break;
      case BTRC_MEDIA_ATTR_ID_GENRE:
        genre = attr_text;
        break;
      case BTRC_MEDIA_ATTR_ID_PLAYING_TIME:
        if (!base::StringToInt(attr_text, &play_time)) {
          LOG(ERROR) << "Failed to parse playing time";
        }
        break;
      default:
        NOTREACHED();
    }
  }

  const AvrcpMediaAttr attr(title, artist, album, genre, track_num, num_tracks,
                            play_time);

  std::lock_guard<std::mutex> lock(delegate_mutex_);
  if (delegate_) delegate_->OnTrackChanged(device_address, attr);
}

// AvrcpControlFactory implementation
// ========================================================

AvrcpControlFactory::AvrcpControlFactory() = default;
AvrcpControlFactory::~AvrcpControlFactory() = default;

bool AvrcpControlFactory::RegisterInstance(const Uuid& uuid,
                                           const RegisterCallback& callback) {
  VLOG(1) << __func__ << " - Uuid: " << uuid.ToString();

  int control_id = next_control_id_++;
  std::unique_ptr<AvrcpControl> hf_client(new AvrcpControl(uuid, control_id));
  callback(BLE_STATUS_SUCCESS, uuid, std::move(hf_client));
  return true;
}

}  // namespace bluetooth
