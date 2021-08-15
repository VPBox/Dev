//
//  Copyright (C) 2017 Google, Inc.
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

#include "service/avrcp_target.h"

#include <algorithm>
#include <cerrno>
#include <climits>
#include <string>

#include "base/logging.h"
#include "base/macros.h"
#include "base/memory/ptr_util.h"
#include "service/logging_helpers.h"

#include "stack/include/avrc_defs.h"

#define PARSE_ADDR(str)                                        \
  ({                                                           \
    RawAddress tmp;                                            \
    if (!RawAddress::FromString((str), tmp)) {                 \
      LOG(ERROR) << "Invalid device address given: " << (str); \
      return false;                                            \
    }                                                          \
    tmp;                                                       \
  })

#define TRY_RET(expr, err_msg) \
  do {                         \
    if (!(expr)) {             \
      LOG(ERROR) << err_msg;   \
      return false;            \
    }                          \
    return true;               \
  } while (0)

#define TRY_RET_FUNC(expr) TRY_RET(expr, __func__ << " failed")

using LockGuard = std::lock_guard<std::mutex>;

namespace bluetooth {

namespace {

std::vector<btrc_player_setting_text_t> StringValueToPlayerSettingsText(
    const std::vector<AvrcpStringValue>& attrs) {
  std::vector<btrc_player_setting_text_t> btrc_attrs(attrs.size());
  for (size_t i = 0; i < attrs.size(); ++i) {
    btrc_attrs[i].id = attrs[i].id();
    std::string str(attrs[i].value());
    size_t to_copy = std::min(sizeof(btrc_attrs[i].text) - 1, str.size());
    if (to_copy < str.size()) {
      LOG(WARNING) << "Value truncated";
    }

    memcpy(btrc_attrs[i].text, str.data(), to_copy);
    btrc_attrs[i].text[to_copy] = '\0';
  }

  return btrc_attrs;
}

std::vector<btrc_element_attr_val_t> StringValueToElementAttrVal(
    const std::vector<AvrcpStringValue>& attrs) {
  std::vector<btrc_element_attr_val_t> btrc_attrs(attrs.size());
  for (size_t i = 0; i < attrs.size(); ++i) {
    btrc_attrs[i].attr_id = attrs[i].id();
    std::string str(attrs[i].value());
    size_t to_copy = std::min(sizeof(btrc_attrs[i].text) - 1, str.size());
    if (to_copy < str.size()) {
      LOG(WARNING) << "Value truncated";
    }

    memcpy(btrc_attrs[i].text, str.data(), to_copy);
    btrc_attrs[i].text[to_copy] = '\0';
  }

  return btrc_attrs;
}

}  // namespace

// static
const int AvrcpTarget::kSingletonInstanceId = 0;

AvrcpTarget::AvrcpTarget(const Uuid& uuid) : app_identifier_(uuid) {
  hal::BluetoothAvrcpInterface::Get()->AddTargetObserver(this);
}

AvrcpTarget::~AvrcpTarget() {
  hal::BluetoothAvrcpInterface::Get()->RemoveTargetObserver(this);
}

const Uuid& AvrcpTarget::GetAppIdentifier() const { return app_identifier_; }

int AvrcpTarget::GetInstanceId() const { return kSingletonInstanceId; }

void AvrcpTarget::SetDelegate(Delegate* delegate) {
  LockGuard lock(delegate_mutex_);
  delegate_ = delegate;
}

bool AvrcpTarget::Enable() {
  LockGuard lock(mutex_);
  return hal::BluetoothAvrcpInterface::Get()->AvrcpTargetEnable();
}

void AvrcpTarget::Disable() {
  LockGuard lock(mutex_);
  hal::BluetoothAvrcpInterface::Get()->AvrcpTargetDisable();
}

bool AvrcpTarget::GetPlayStatusResponse(const std::string& str_addr,
                                        int32_t play_status, uint32_t song_len,
                                        uint32_t song_pos) {
  RawAddress addr = PARSE_ADDR(str_addr);
  LockGuard lock(mutex_);
  TRY_RET_FUNC(hal::BluetoothAvrcpInterface::Get()
                   ->GetTargetHALInterface()
                   ->get_play_status_rsp(
                       addr, static_cast<btrc_play_status_t>(play_status),
                       song_len, song_pos) == BT_STATUS_SUCCESS);
}

bool AvrcpTarget::ListPlayerAppAttrResponse(const std::string& str_addr,
                                            const std::vector<int32_t>& attrs) {
  RawAddress addr = PARSE_ADDR(str_addr);

  std::vector<btrc_player_attr_t> btrc_attrs;
  btrc_attrs.reserve(attrs.size());
  for (auto attr : attrs) {
    btrc_attrs.push_back(static_cast<btrc_player_attr_t>(attr));
  }

  LockGuard lock(mutex_);
  TRY_RET_FUNC(hal::BluetoothAvrcpInterface::Get()
                   ->GetTargetHALInterface()
                   ->list_player_app_attr_rsp(addr, btrc_attrs.size(),
                                              btrc_attrs.data()) ==
               BT_STATUS_SUCCESS);
}

bool AvrcpTarget::GetPlayerAppValueResponse(
    const std::string& str_addr, const std::vector<AvrcpIntValue>& values) {
  RawAddress addr = PARSE_ADDR(str_addr);
  btrc_player_settings_t btrc_values;
  if (values.size() >= arraysize(btrc_values.attr_ids)) {
    LOG(ERROR) << "Too many attribute values";
    return false;
  }

  btrc_values.num_attr = values.size();
  for (size_t i = 0; i < values.size(); ++i) {
    btrc_values.attr_ids[i] = values[i].id();
    btrc_values.attr_values[i] = values[i].value();
  }

  LockGuard lock(mutex_);
  TRY_RET_FUNC(hal::BluetoothAvrcpInterface::Get()
                   ->GetTargetHALInterface()
                   ->get_player_app_value_rsp(addr, &btrc_values) ==
               BT_STATUS_SUCCESS);
}

bool AvrcpTarget::GetPlayerAppAttrTextResponse(
    const std::string& str_addr, const std::vector<AvrcpStringValue>& attrs) {
  RawAddress addr = PARSE_ADDR(str_addr);
  auto btrc_attrs = StringValueToPlayerSettingsText(attrs);
  LockGuard lock(mutex_);
  TRY_RET_FUNC(hal::BluetoothAvrcpInterface::Get()
                   ->GetTargetHALInterface()
                   ->get_player_app_attr_text_rsp(addr, btrc_attrs.size(),
                                                  btrc_attrs.data()) ==
               BT_STATUS_SUCCESS);
}

bool AvrcpTarget::GetPlayerAppValueTextResponse(
    const std::string& str_addr, const std::vector<AvrcpStringValue>& values) {
  RawAddress addr = PARSE_ADDR(str_addr);
  auto btrc_values = StringValueToPlayerSettingsText(values);
  LockGuard lock(mutex_);
  TRY_RET_FUNC(hal::BluetoothAvrcpInterface::Get()
                   ->GetTargetHALInterface()
                   ->get_player_app_value_text_rsp(addr, btrc_values.size(),
                                                   btrc_values.data()) ==
               BT_STATUS_SUCCESS);
}

bool AvrcpTarget::GetElementAttrResponse(
    const std::string& str_addr, const std::vector<AvrcpStringValue>& attrs) {
  RawAddress addr = PARSE_ADDR(str_addr);
  auto btrc_attrs = StringValueToElementAttrVal(attrs);
  LockGuard lock(mutex_);
  TRY_RET_FUNC(
      hal::BluetoothAvrcpInterface::Get()
          ->GetTargetHALInterface()
          ->get_element_attr_rsp(addr, btrc_attrs.size(), btrc_attrs.data()) ==
      BT_STATUS_SUCCESS);
}

bool AvrcpTarget::SetPlayerAppValueResponse(const std::string& str_addr,
                                            int32_t rsp_status) {
  RawAddress addr = PARSE_ADDR(str_addr);
  LockGuard lock(mutex_);
  TRY_RET_FUNC(hal::BluetoothAvrcpInterface::Get()
                   ->GetTargetHALInterface()
                   ->set_player_app_value_rsp(
                       addr, static_cast<btrc_status_t>(rsp_status)) ==
               BT_STATUS_SUCCESS);
}

bool AvrcpTarget::RegisterNotificationResponse(
    int32_t event_id, int32_t type,
    const AvrcpRegisterNotificationResponse& param) {
  auto param_copy = param.data();
  LockGuard lock(mutex_);
  TRY_RET_FUNC(hal::BluetoothAvrcpInterface::Get()
                   ->GetTargetHALInterface()
                   ->register_notification_rsp(
                       static_cast<btrc_event_id_t>(event_id),
                       static_cast<btrc_notification_type_t>(type),
                       &param_copy) == BT_STATUS_SUCCESS);
}

bool AvrcpTarget::SetVolume(int volume) {
  LockGuard lock(mutex_);
  TRY_RET_FUNC(
      hal::BluetoothAvrcpInterface::Get()->GetTargetHALInterface()->set_volume(
          volume) == BT_STATUS_SUCCESS);
}

void AvrcpTarget::RemoteFeaturesCallback(const RawAddress& bd_addr,
                                         btrc_remote_features_t features) {
  auto str_addr = BtAddrString(&bd_addr);
  LockGuard lock(delegate_mutex_);
  if (delegate_)
    delegate_->OnGetRemoteFeatures(str_addr, static_cast<int32_t>(features));
}

void AvrcpTarget::GetPlayStatusCallback(const RawAddress& bd_addr) {
  auto str_addr = BtAddrString(&bd_addr);
  LockGuard lock(delegate_mutex_);
  if (delegate_) delegate_->OnGetPlayStatus(str_addr);
}

void AvrcpTarget::ListPlayerAppAttrCallback(const RawAddress& bd_addr) {
  auto str_addr = BtAddrString(&bd_addr);
  LockGuard lock(delegate_mutex_);
  if (delegate_) delegate_->OnListPlayerAppAttr(str_addr);
}

void AvrcpTarget::ListPlayerAppValuesCallback(btrc_player_attr_t attr_id,
                                              const RawAddress& bd_addr) {
  auto str_addr = BtAddrString(&bd_addr);
  LockGuard lock(delegate_mutex_);
  if (delegate_)
    delegate_->OnListPlayerAppValues(str_addr, static_cast<int32_t>(attr_id));
}

void AvrcpTarget::GetPlayerAppValueCallback(uint8_t num_attr,
                                            btrc_player_attr_t* p_attrs,
                                            const RawAddress& bd_addr) {
  auto str_addr = BtAddrString(&bd_addr);
  std::vector<int32_t> attr_vec;
  attr_vec.reserve(num_attr);
  for (auto* it = p_attrs; it != p_attrs + num_attr; ++it) {
    attr_vec.push_back(*it);
  }

  LockGuard lock(delegate_mutex_);
  if (delegate_) delegate_->OnGetPlayerAppValue(str_addr, attr_vec);
}

void AvrcpTarget::GetPlayerAppAttrsTextCallback(uint8_t num_attr,
                                                btrc_player_attr_t* p_attrs,
                                                const RawAddress& bd_addr) {
  auto str_addr = BtAddrString(&bd_addr);
  std::vector<int32_t> attr_vec;
  attr_vec.reserve(num_attr);
  for (auto* it = p_attrs; it != p_attrs + num_attr; ++it) {
    attr_vec.push_back(*it);
  }

  LockGuard lock(delegate_mutex_);
  if (delegate_) delegate_->OnGetPlayerAppAttrsText(str_addr, attr_vec);
}

void AvrcpTarget::GetPlayerAppValuesTextCallback(uint8_t attr_id,
                                                 uint8_t num_val,
                                                 uint8_t* p_vals,
                                                 const RawAddress& bd_addr) {
  auto str_addr = BtAddrString(&bd_addr);
  std::vector<int32_t> val_vec;
  val_vec.reserve(num_val);
  for (auto* it = p_vals; it != p_vals + num_val; ++it) {
    val_vec.push_back(*it);
  }
  LockGuard lock(delegate_mutex_);
  if (delegate_)
    delegate_->OnGetPlayerAppValuesText(str_addr, attr_id, val_vec);
}

void AvrcpTarget::SetPlayerAppValueCallback(btrc_player_settings_t* p_vals,
                                            const RawAddress& bd_addr) {
  auto str_addr = BtAddrString(&bd_addr);
  std::vector<AvrcpIntValue> values;
  values.reserve(p_vals->num_attr);
  for (size_t i = 0; i < p_vals->num_attr; ++i) {
    values.emplace_back(p_vals->attr_ids[i], p_vals->attr_values[i]);
  }

  LockGuard lock(delegate_mutex_);
  if (delegate_) delegate_->OnSetPlayerAppValue(str_addr, values);
}

void AvrcpTarget::GetElementAttrCallback(uint8_t num_attr,
                                         btrc_media_attr_t* p_attrs,
                                         const RawAddress& bd_addr) {
  auto str_addr = BtAddrString(&bd_addr);
  std::vector<int32_t> attr_vec;
  attr_vec.reserve(num_attr);
  for (auto* it = p_attrs; it != p_attrs + num_attr; ++it) {
    attr_vec.push_back(*it);
  }
  LockGuard lock(delegate_mutex_);
  if (delegate_) delegate_->OnGetElementAttrs(str_addr, attr_vec);
}

void AvrcpTarget::RegisterNotificationCallback(btrc_event_id_t event_id,
                                               uint32_t param,
                                               const RawAddress& bd_addr) {
  auto str_addr = BtAddrString(&bd_addr);
  LockGuard lock(delegate_mutex_);
  if (delegate_)
    delegate_->OnRegisterNotification(str_addr, static_cast<int32_t>(event_id),
                                      param);
}

void AvrcpTarget::VolumeChangeCallback(uint8_t volume, uint8_t ctype,
                                       const RawAddress& bd_addr) {
  auto str_addr = BtAddrString(&bd_addr);
  LockGuard lock(delegate_mutex_);
  if (delegate_) delegate_->OnVolumeChange(str_addr, volume, ctype);
}

void AvrcpTarget::PassthroughCmdCallback(int id, int key_state,
                                         const RawAddress& bd_addr) {
  auto str_addr = BtAddrString(&bd_addr);
  LockGuard lock(delegate_mutex_);
  if (delegate_) delegate_->OnPassThroughCommand(str_addr, id, key_state);
}

// AvrcpTargetFactory implementation
// ========================================================

AvrcpTargetFactory::AvrcpTargetFactory() = default;
AvrcpTargetFactory::~AvrcpTargetFactory() = default;

bool AvrcpTargetFactory::RegisterInstance(const Uuid& uuid,
                                          const RegisterCallback& callback) {
  VLOG(1) << __func__ << " - UUID: " << uuid.ToString();

  auto avrcp_target = base::WrapUnique(new AvrcpTarget(uuid));
  callback(BLE_STATUS_SUCCESS, uuid, std::move(avrcp_target));
  return true;
}

}  // namespace bluetooth
