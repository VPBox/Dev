//
//  Copyright 2015 Google, Inc.
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

#include "service/hal/bluetooth_avrcp_interface.h"

#include <mutex>
#include <shared_mutex>

#include <base/logging.h>
#include <base/observer_list.h>

#include "service/hal/bluetooth_interface.h"
#include "service/logging_helpers.h"

using std::lock_guard;
using std::mutex;
using std::shared_lock;
using std::unique_lock;
#if defined(OS_GENERIC) && defined(_LIBCPP_VERSION) && (_LIBCPP_VERSION < 3500)
using shared_mutex_impl = std::shared_mutex;
#else
using shared_mutex_impl = std::shared_timed_mutex;
#endif

namespace bluetooth {
namespace hal {

namespace {

// The global BluetoothAvrcpInterface instance.
BluetoothAvrcpInterface* g_interface = nullptr;

// Mutex used by callbacks to access |g_interface|. If we initialize or clean it
// use unique_lock. If only accessing |g_interface| use shared lock.
// TODO(jpawlowski): this should be just shared_mutex, as we currently don't use
// timed methods. Change to shared_mutex when we upgrade to C++14
shared_mutex_impl g_instance_lock;

// Helper for obtaining the observer lists. This is forward declared here
// and defined below since it depends on BluetoothInterfaceImpl.
base::ObserverList<BluetoothAvrcpInterface::TargetObserver>*
GetTargetObservers();

base::ObserverList<BluetoothAvrcpInterface::ControlObserver>*
GetControlObservers();

#define VERIFY_INTERFACE_OR_RETURN()                                   \
  do {                                                                 \
    if (!g_interface) {                                                \
      LOG(WARNING) << "Callback received while |g_interface| is NULL"; \
      return;                                                          \
    }                                                                  \
  } while (0)

void RemoteFeaturesCallback(const RawAddress& bd_addr,
                            btrc_remote_features_t features) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.RemoteFeaturesCallback(bd_addr, features);
  }
}

void GetPlayStatusCallback(const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.GetPlayStatusCallback(bd_addr);
  }
}

void ListPlayerAppAttrCallback(const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.ListPlayerAppAttrCallback(bd_addr);
  }
}

void ListPlayerAppValuesCallback(btrc_player_attr_t attr_id,
                                 const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.ListPlayerAppValuesCallback(attr_id, bd_addr);
  }
}

void GetPlayerAppValueCallback(uint8_t num_attr, btrc_player_attr_t* p_attrs,
                               const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.GetPlayerAppValueCallback(num_attr, p_attrs, bd_addr);
  }
}

void GetPlayerAppAttrsTextCallback(uint8_t num_attr,
                                   btrc_player_attr_t* p_attrs,
                                   const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.GetPlayerAppAttrsTextCallback(num_attr, p_attrs, bd_addr);
  }
}

void GetPlayerAppValuesTextCallback(uint8_t attr_id, uint8_t num_val,
                                    uint8_t* p_vals,
                                    const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.GetPlayerAppValuesTextCallback(attr_id, num_val, p_vals, bd_addr);
  }
}

void SetPlayerAppValueCallback(btrc_player_settings_t* p_vals,
                               const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.SetPlayerAppValueCallback(p_vals, bd_addr);
  }
}

void GetElementAttrCallback(uint8_t num_attr, btrc_media_attr_t* p_attrs,
                            const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.GetElementAttrCallback(num_attr, p_attrs, bd_addr);
  }
}

void RegisterNotificationCallback(btrc_event_id_t event_id, uint32_t param,
                                  const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.RegisterNotificationCallback(event_id, param, bd_addr);
  }
}

void VolumeChangeCallback(uint8_t volume, uint8_t ctype,
                          const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.VolumeChangeCallback(volume, ctype, bd_addr);
  }
}

void PassthroughCmdCallback(int id, int key_state, const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.PassthroughCmdCallback(id, key_state, bd_addr);
  }
}

void SetAddressedPlayerCallback(uint16_t player_id, const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.SetAddressedPlayerCallback(player_id, bd_addr);
  }
}

void SetBrowsedPlayerCallback(uint16_t player_id, const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.SetBrowsedPlayerCallback(player_id, bd_addr);
  }
}

void GetFolderItemsCallback(uint8_t scope, uint32_t start_item,
                            uint32_t end_item, uint8_t num_attr,
                            uint32_t* p_attr_ids, const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.GetFolderItemsCallback(scope, start_item, end_item, num_attr,
                                    p_attr_ids, bd_addr);
  }
}

void ChangePathCallback(uint8_t direction, uint8_t* folder_uid,
                        const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.ChangePathCallback(direction, folder_uid, bd_addr);
  }
}

void GetItemAttrCallback(uint8_t scope, uint8_t* uid, uint16_t uid_counter,
                         uint8_t num_attr, btrc_media_attr_t* p_attrs,
                         const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.GetItemAttrCallback(scope, uid, uid_counter, num_attr, p_attrs,
                                 bd_addr);
  }
}

void PlayItemCallback(uint8_t scope, uint16_t uid_counter, uint8_t* uid,
                      const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.PlayItemCallback(scope, uid_counter, uid, bd_addr);
  }
}

void GetTotalNumOfItemsCallback(uint8_t scope, const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.GetTotalNumOfItemsCallback(scope, bd_addr);
  }
}

void SearchCallback(uint16_t charset_id, uint16_t str_len, uint8_t* p_str,
                    const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.SearchCallback(str_len, p_str, bd_addr);
  }
}

void AddToNowPlayingCallback(uint8_t scope, uint8_t* uid, uint16_t uid_counter,
                             const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetTargetObservers()) {
    observer.AddToNowPlayingCallback(scope, uid, uid_counter, bd_addr);
  }
}

void PassthroughRspCallback(const RawAddress& bd_addr, int id, int key_state) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetControlObservers()) {
    observer.PassthroughRspCallback(bd_addr, id, key_state);
  }
}

void GroupnavigationRspCallback(int id, int key_state) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetControlObservers()) {
    observer.GroupnavigationRspCallback(id, key_state);
  }
}

void ConnectionStateCallback(bool rc_connect, bool bt_connect,
                             const RawAddress& bd_addr) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetControlObservers()) {
    observer.ConnectionStateCallback(rc_connect, bt_connect, bd_addr);
  }
}

void CtrlGetrcfeaturesCallback(const RawAddress& bd_addr, int features) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetControlObservers()) {
    observer.CtrlGetrcfeaturesCallback(bd_addr, features);
  }
}

void CtrlSetplayerapplicationsettingRspCallback(const RawAddress& bd_addr,
                                                uint8_t accepted) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();

  for (auto& observer : *GetControlObservers()) {
    observer.CtrlSetplayerapplicationsettingRspCallback(bd_addr, accepted);
  }
}

void CtrlPlayerapplicationsettingCallback(
    const RawAddress& bd_addr, uint8_t num_attr,
    btrc_player_app_attr_t* app_attrs, uint8_t num_ext_attr,
    btrc_player_app_ext_attr_t* ext_attrs) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetControlObservers()) {
    observer.CtrlPlayerapplicationsettingCallback(bd_addr, num_attr, app_attrs,
                                                  num_ext_attr, ext_attrs);
  }
}

void CtrlPlayerapplicationsettingChangedCallback(
    const RawAddress& bd_addr, const btrc_player_settings_t& vals) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetControlObservers()) {
    observer.CtrlPlayerapplicationsettingChangedCallback(bd_addr, vals);
  }
}

void CtrlSetabsvolCmdCallback(const RawAddress& bd_addr, uint8_t abs_vol,
                              uint8_t label) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetControlObservers()) {
    observer.CtrlSetabsvolCmdCallback(bd_addr, abs_vol, label);
  }
}

void CtrlRegisternotificationAbsVolCallback(const RawAddress& bd_addr,
                                            uint8_t label) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetControlObservers()) {
    observer.CtrlRegisternotificationAbsVolCallback(bd_addr, label);
  }
}

void CtrlTrackChangedCallback(const RawAddress& bd_addr, uint8_t num_attr,
                              btrc_element_attr_val_t* p_attrs) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetControlObservers()) {
    observer.CtrlTrackChangedCallback(bd_addr, num_attr, p_attrs);
  }
}

void CtrlPlayPositionChangedCallback(const RawAddress& bd_addr,
                                     uint32_t song_len, uint32_t song_pos) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetControlObservers()) {
    observer.CtrlPlayPositionChangedCallback(bd_addr, song_len, song_pos);
  }
}

void CtrlPlayStatusChangedCallback(const RawAddress& bd_addr,
                                   btrc_play_status_t play_status) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetControlObservers()) {
    observer.CtrlPlayStatusChangedCallback(bd_addr, play_status);
  }
}

void CtrlGetFolderItemsCallback(const RawAddress& bd_addr, btrc_status_t status,
                                const btrc_folder_items_t* folder_items,
                                uint8_t count) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetControlObservers()) {
    observer.CtrlGetFolderItemsCallback(bd_addr, status, folder_items, count);
  }
}

void CtrlChangePathCallback(const RawAddress& bd_addr, uint32_t count) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetControlObservers()) {
    observer.CtrlChangePathCallback(bd_addr, count);
  }
}

void CtrlSetBrowsedPlayerCallback(const RawAddress& bd_addr, uint8_t num_items,
                                  uint8_t depth) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetControlObservers()) {
    observer.CtrlSetBrowsedPlayerCallback(bd_addr, num_items, depth);
  }
}

void CtrlSetAddressedPlayerCallback(const RawAddress& bd_addr, uint8_t status) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__;
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetControlObservers()) {
    observer.CtrlSetAddressedPlayerCallback(bd_addr, status);
  }
}

btrc_callbacks_t target_callbacks = {
    .size = sizeof(btrc_callbacks_t),
    .remote_features_cb = RemoteFeaturesCallback,
    .get_play_status_cb = GetPlayStatusCallback,
    .list_player_app_attr_cb = ListPlayerAppAttrCallback,
    .list_player_app_values_cb = ListPlayerAppValuesCallback,
    .get_player_app_value_cb = GetPlayerAppValueCallback,
    .get_player_app_attrs_text_cb = GetPlayerAppAttrsTextCallback,
    .get_player_app_values_text_cb = GetPlayerAppValuesTextCallback,
    .set_player_app_value_cb = SetPlayerAppValueCallback,
    .get_element_attr_cb = GetElementAttrCallback,
    .register_notification_cb = RegisterNotificationCallback,
    .volume_change_cb = VolumeChangeCallback,
    .passthrough_cmd_cb = PassthroughCmdCallback,
    .set_addressed_player_cb = SetAddressedPlayerCallback,
    .set_browsed_player_cb = SetBrowsedPlayerCallback,
    .get_folder_items_cb = GetFolderItemsCallback,
    .change_path_cb = ChangePathCallback,
    .get_item_attr_cb = GetItemAttrCallback,
    .play_item_cb = PlayItemCallback,
    .get_total_num_of_items_cb = GetTotalNumOfItemsCallback,
    .search_cb = SearchCallback,
    .add_to_now_playing_cb = AddToNowPlayingCallback,
};

btrc_ctrl_callbacks_t control_callbacks = {
    .size = sizeof(btrc_ctrl_callbacks_t),
    .passthrough_rsp_cb = PassthroughRspCallback,
    .groupnavigation_rsp_cb = GroupnavigationRspCallback,
    .connection_state_cb = ConnectionStateCallback,
    .getrcfeatures_cb = CtrlGetrcfeaturesCallback,
    .setplayerappsetting_rsp_cb = CtrlSetplayerapplicationsettingRspCallback,
    .playerapplicationsetting_cb = CtrlPlayerapplicationsettingCallback,
    .playerapplicationsetting_changed_cb =
        CtrlPlayerapplicationsettingChangedCallback,
    .setabsvol_cmd_cb = CtrlSetabsvolCmdCallback,
    .registernotification_absvol_cb = CtrlRegisternotificationAbsVolCallback,
    .track_changed_cb = CtrlTrackChangedCallback,
    .play_position_changed_cb = CtrlPlayPositionChangedCallback,
    .play_status_changed_cb = CtrlPlayStatusChangedCallback,
    .get_folder_items_cb = CtrlGetFolderItemsCallback,
    .change_folder_path_cb = CtrlChangePathCallback,
    .set_browsed_player_cb = CtrlSetBrowsedPlayerCallback,
    .set_addressed_player_cb = CtrlSetAddressedPlayerCallback,
};

}  // namespace

// BluetoothAvrcpInterface implementation for production.
class BluetoothAvrcpInterfaceImpl : public BluetoothAvrcpInterface {
 public:
  BluetoothAvrcpInterfaceImpl() : control_iface_(nullptr) {}

  ~BluetoothAvrcpInterfaceImpl() override {
    if (control_iface_) control_iface_->cleanup();
  }

  bool AvrcpControlEnable() override {
    if (control_enabled_) {
      return true;
    }

    if (control_iface_->init(&control_callbacks) != BT_STATUS_SUCCESS) {
      LOG(ERROR) << "Failed to initialize HAL AVRCP control interface";
      return false;
    }

    control_enabled_ = true;
    return true;
  }

  void AvrcpControlDisable() override {
    if (!control_enabled_) {
      return;
    }

    control_iface_->cleanup();
    control_enabled_ = false;
  }

  bool AvrcpTargetEnable() override {
    if (target_enabled_) {
      return true;
    }

    if (target_iface_->init(&target_callbacks) != BT_STATUS_SUCCESS) {
      LOG(ERROR) << "Failed to initialize HAL AVRCP target interface";
      return false;
    }

    target_enabled_ = true;
    return true;
  }

  void AvrcpTargetDisable() override {
    if (!target_enabled_) {
      return;
    }

    target_iface_->cleanup();
    target_enabled_ = false;
  }

  void AddTargetObserver(TargetObserver* observer) override {
    target_observers_.AddObserver(observer);
  }

  void RemoveTargetObserver(TargetObserver* observer) override {
    target_observers_.RemoveObserver(observer);
  }

  void AddControlObserver(ControlObserver* observer) override {
    control_observers_.AddObserver(observer);
  }

  void RemoveControlObserver(ControlObserver* observer) override {
    control_observers_.RemoveObserver(observer);
  }

  const btrc_interface_t* GetTargetHALInterface() const override {
    return target_iface_;
  }

  const btrc_ctrl_interface_t* GetControlHALInterface() const override {
    return control_iface_;
  }

  // Initialize the interface.
  bool Initialize() {
    const bt_interface_t* bt_iface =
        BluetoothInterface::Get()->GetHALInterface();
    CHECK(bt_iface);

    auto* target_iface = reinterpret_cast<const btrc_interface_t*>(
        bt_iface->get_profile_interface(BT_PROFILE_AV_RC_ID));
    if (!target_iface) {
      LOG(ERROR) << "Failed to obtain HAL AVRCP target interface handle";
      return false;
    }

    auto* control_iface = reinterpret_cast<const btrc_ctrl_interface_t*>(
        bt_iface->get_profile_interface(BT_PROFILE_AV_RC_CTRL_ID));
    if (!control_iface) {
      LOG(ERROR) << "Failed to obtain HAL AVRCP control interface handle";
      return false;
    }

    control_iface_ = control_iface;
    target_iface_ = target_iface;

    // Only initialize the control interface.
    return AvrcpControlEnable();
  }

  base::ObserverList<TargetObserver>* target_observers() {
    return &target_observers_;
  }

  base::ObserverList<ControlObserver>* control_observers() {
    return &control_observers_;
  }

 private:
  // List of observers that are interested in notifications from us.
  // We're not using a base::ObserverListThreadSafe, which it posts observer
  // events automatically on the origin threads, as we want to avoid that
  // overhead and simply forward the events to the upper layer.
  base::ObserverList<TargetObserver> target_observers_;
  base::ObserverList<ControlObserver> control_observers_;

  // The HAL handle obtained from the shared library. We hold a weak reference
  // to this since the actual data resides in the shared Bluetooth library.
  const btrc_interface_t* target_iface_ = nullptr;
  const btrc_ctrl_interface_t* control_iface_ = nullptr;

  bool control_enabled_ = false;
  bool target_enabled_ = false;

  DISALLOW_COPY_AND_ASSIGN(BluetoothAvrcpInterfaceImpl);
};

namespace {

base::ObserverList<BluetoothAvrcpInterface::TargetObserver>*
GetTargetObservers() {
  CHECK(g_interface);
  return static_cast<BluetoothAvrcpInterfaceImpl*>(g_interface)
      ->target_observers();
}

base::ObserverList<BluetoothAvrcpInterface::ControlObserver>*
GetControlObservers() {
  CHECK(g_interface);
  return static_cast<BluetoothAvrcpInterfaceImpl*>(g_interface)
      ->control_observers();
}

}  // namespace

void BluetoothAvrcpInterface::TargetObserver::RemoteFeaturesCallback(
    const RawAddress& bd_addr, btrc_remote_features_t features) {
  // Do nothing.
}

void BluetoothAvrcpInterface::TargetObserver::GetPlayStatusCallback(
    const RawAddress& bd_addr) {
  // Do nothing.
}

void BluetoothAvrcpInterface::TargetObserver::ListPlayerAppAttrCallback(
    const RawAddress& bd_addr) {
  // Do nothing.
}

void BluetoothAvrcpInterface::TargetObserver::ListPlayerAppValuesCallback(
    btrc_player_attr_t attr_id, const RawAddress& bd_addr) {
  // Do nothing.
}

void BluetoothAvrcpInterface::TargetObserver::GetPlayerAppValueCallback(
    uint8_t num_attr, btrc_player_attr_t* p_attrs, const RawAddress& bd_addr) {
  // Do nothing.
}

void BluetoothAvrcpInterface::TargetObserver::GetPlayerAppAttrsTextCallback(
    uint8_t num_attr, btrc_player_attr_t* p_attrs, const RawAddress& bd_addr) {
  // Do nothing.
}

void BluetoothAvrcpInterface::TargetObserver::GetPlayerAppValuesTextCallback(
    uint8_t attr_id, uint8_t num_val, uint8_t* p_vals,
    const RawAddress& bd_addr) {
  // Do nothing.
}

void BluetoothAvrcpInterface::TargetObserver::SetPlayerAppValueCallback(
    btrc_player_settings_t* p_vals, const RawAddress& bd_addr) {
  // Do nothing.
}

void BluetoothAvrcpInterface::TargetObserver::GetElementAttrCallback(
    uint8_t num_attr, btrc_media_attr_t* p_attrs, const RawAddress& bd_addr) {
  // Do nothing.
}

void BluetoothAvrcpInterface::TargetObserver::RegisterNotificationCallback(
    btrc_event_id_t event_id, uint32_t param, const RawAddress& bd_addr) {
  // Do nothing.
}

void BluetoothAvrcpInterface::TargetObserver::VolumeChangeCallback(
    uint8_t volume, uint8_t ctype, const RawAddress& bd_addr) {
  // Do nothing.
}

void BluetoothAvrcpInterface::TargetObserver::PassthroughCmdCallback(
    int id, int key_state, const RawAddress& bd_addr) {
  // Do nothing.
}

void BluetoothAvrcpInterface::TargetObserver::SetAddressedPlayerCallback(
    uint16_t player_id, const RawAddress& bd_addr) {
  // Do nothing.
}

void BluetoothAvrcpInterface::TargetObserver::SetBrowsedPlayerCallback(
    uint16_t player_id, const RawAddress& bd_addr) {
  // Do nothing.
}

void BluetoothAvrcpInterface::TargetObserver::GetFolderItemsCallback(
    uint8_t scope, uint32_t start_item, uint32_t end_item, uint8_t num_attr,
    uint32_t* p_attr_ids, const RawAddress& bd_addr) {
  // Do nothing.
}

void BluetoothAvrcpInterface::TargetObserver::ChangePathCallback(
    uint8_t direction, uint8_t* folder_uid, const RawAddress& bd_addr) {
  // Do nothing.
}

void BluetoothAvrcpInterface::TargetObserver::GetItemAttrCallback(
    uint8_t scope, uint8_t* uid, uint16_t uid_counter, uint8_t num_attr,
    btrc_media_attr_t* p_attrs, const RawAddress& bd_addr) {
  // Do nothing.
}

void BluetoothAvrcpInterface::TargetObserver::PlayItemCallback(
    uint8_t scope, uint16_t uid_counter, uint8_t* uid,
    const RawAddress& bd_addr) {
  // Do nothing.
}

void BluetoothAvrcpInterface::TargetObserver::GetTotalNumOfItemsCallback(
    uint8_t scope, const RawAddress& bd_addr) {
  // Do nothing.
}

void BluetoothAvrcpInterface::TargetObserver::SearchCallback(
    uint16_t str_len, uint8_t* p_str, const RawAddress& bd_addr) {
  // Do nothing.
}

void BluetoothAvrcpInterface::TargetObserver::AddToNowPlayingCallback(
    uint8_t scope, uint8_t* uid, uint16_t uid_counter,
    const RawAddress& bd_addr) {
  // Do nothing.
}

void BluetoothAvrcpInterface::ControlObserver::PassthroughRspCallback(
    const RawAddress& /* bd_addr */, int /* id */, int /* key_state */) {
  // Do nothing.
}

void BluetoothAvrcpInterface::ControlObserver::GroupnavigationRspCallback(
    int /* id */, int /* key_state */) {
  // Do nothing.
}

void BluetoothAvrcpInterface::ControlObserver::ConnectionStateCallback(
    bool /* rc_connect */, bool /* bt_connect */,
    const RawAddress& /* bd_addr */) {
  // Do nothing.
}

void BluetoothAvrcpInterface::ControlObserver::CtrlGetrcfeaturesCallback(
    const RawAddress& /* bd_addr */, int /* features */) {
  // Do nothing.
}

void BluetoothAvrcpInterface::ControlObserver::
    CtrlSetplayerapplicationsettingRspCallback(const RawAddress& /* bd_addr */,
                                               uint8_t /* accepted */) {
  // Do nothing.
}

void BluetoothAvrcpInterface::ControlObserver::
    CtrlPlayerapplicationsettingCallback(
        const RawAddress& /* bd_addr */, uint8_t /* num_attr */,
        btrc_player_app_attr_t* /* app_attrs */, uint8_t /* num_ext_attr */,
        btrc_player_app_ext_attr_t* /* ext_attrs */) {
  // Do nothing.
}

void BluetoothAvrcpInterface::ControlObserver::
    CtrlPlayerapplicationsettingChangedCallback(
        const RawAddress& /* bd_addr*/,
        const btrc_player_settings_t& /* vals */) {
  // Do nothing.
}

void BluetoothAvrcpInterface::ControlObserver::CtrlSetabsvolCmdCallback(
    const RawAddress& /* bd_addr */, uint8_t /* abs_vol */,
    uint8_t /* label */) {
  // Do nothing.
}

void BluetoothAvrcpInterface::ControlObserver::
    CtrlRegisternotificationAbsVolCallback(const RawAddress& /* bd_addr */,
                                           uint8_t /* label */) {
  // Do nothing.
}

void BluetoothAvrcpInterface::ControlObserver::CtrlTrackChangedCallback(
    const RawAddress& /*bd_addr */, uint8_t /* num_attr */,
    btrc_element_attr_val_t* /* p_attrs */) {
  // Do nothing.
}

void BluetoothAvrcpInterface::ControlObserver::CtrlPlayPositionChangedCallback(
    const RawAddress& /* bd_addr */, uint32_t /* song_len */,
    uint32_t /* song_pos */) {
  // Do nothing.
}

void BluetoothAvrcpInterface::ControlObserver::CtrlPlayStatusChangedCallback(
    const RawAddress& /* bd_addr */, btrc_play_status_t /* play_status */) {
  // Do nothing.
}

void BluetoothAvrcpInterface::ControlObserver::CtrlGetFolderItemsCallback(
    const RawAddress& /* bd_addr */, btrc_status_t /* status */,
    const btrc_folder_items_t* /*folder_items */, uint8_t /* count */) {
  // Do nothing.
}

void BluetoothAvrcpInterface::ControlObserver::CtrlChangePathCallback(
    const RawAddress& /* bd_addr */, uint32_t /* count */) {
  // Do nothing.
}

void BluetoothAvrcpInterface::ControlObserver::CtrlSetBrowsedPlayerCallback(
    const RawAddress& /* bd_addr */, uint8_t /* num_items */,
    uint8_t /* depth */) {
  // Do nothing.
}

void BluetoothAvrcpInterface::ControlObserver::CtrlSetAddressedPlayerCallback(
    const RawAddress& /* bd_addr */, uint8_t /* status */) {
  // Do nothing.
}

// static
bool BluetoothAvrcpInterface::Initialize() {
  unique_lock<shared_mutex_impl> lock(g_instance_lock);
  CHECK(!g_interface);

  std::unique_ptr<BluetoothAvrcpInterfaceImpl> impl(
      new BluetoothAvrcpInterfaceImpl());
  if (!impl->Initialize()) {
    LOG(ERROR) << "Failed to initialize BluetoothAvrcpInterface";
    return false;
  }

  g_interface = impl.release();

  return true;
}

// static
void BluetoothAvrcpInterface::CleanUp() {
  unique_lock<shared_mutex_impl> lock(g_instance_lock);
  CHECK(g_interface);

  delete g_interface;
  g_interface = nullptr;
}

// static
bool BluetoothAvrcpInterface::IsInitialized() {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);

  return g_interface != nullptr;
}

// static
BluetoothAvrcpInterface* BluetoothAvrcpInterface::Get() {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  CHECK(g_interface);
  return g_interface;
}

// static
void BluetoothAvrcpInterface::InitializeForTesting(
    BluetoothAvrcpInterface* test_instance) {
  unique_lock<shared_mutex_impl> lock(g_instance_lock);
  CHECK(test_instance);
  CHECK(!g_interface);

  g_interface = test_instance;
}

}  // namespace hal
}  // namespace bluetooth
