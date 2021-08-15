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

#pragma once

#include <base/macros.h>
#include <hardware/bluetooth.h>
#include <hardware/bt_rc.h>

namespace bluetooth {
namespace hal {

class BluetoothAvrcpInterface {
 public:
  // The HAL interface doesn't allow registering "user data" that carries
  // context beyond the callback parameters, forcing implementations to deal
  // with global variables. The *Observer interface is to redirect these events
  // to interested parties in an object-oriented manner.
  class TargetObserver {
   public:
    virtual ~TargetObserver() = default;

    virtual void RemoteFeaturesCallback(const RawAddress& bd_addr,
                                        btrc_remote_features_t features);
    virtual void GetPlayStatusCallback(const RawAddress& bd_addr);
    virtual void ListPlayerAppAttrCallback(const RawAddress& bd_addr);
    virtual void ListPlayerAppValuesCallback(btrc_player_attr_t attr_id,
                                             const RawAddress& bd_addr);
    virtual void GetPlayerAppValueCallback(uint8_t num_attr,
                                           btrc_player_attr_t* p_attrs,
                                           const RawAddress& bd_addr);
    virtual void GetPlayerAppAttrsTextCallback(uint8_t num_attr,
                                               btrc_player_attr_t* p_attrs,
                                               const RawAddress& bd_addr);
    virtual void GetPlayerAppValuesTextCallback(uint8_t attr_id,
                                                uint8_t num_val,
                                                uint8_t* p_vals,
                                                const RawAddress& bd_addr);
    virtual void SetPlayerAppValueCallback(btrc_player_settings_t* p_vals,
                                           const RawAddress& bd_addr);
    virtual void GetElementAttrCallback(uint8_t num_attr,
                                        btrc_media_attr_t* p_attrs,
                                        const RawAddress& bd_addr);
    virtual void RegisterNotificationCallback(btrc_event_id_t event_id,
                                              uint32_t param,
                                              const RawAddress& bd_addr);
    virtual void VolumeChangeCallback(uint8_t volume, uint8_t ctype,
                                      const RawAddress& bd_addr);
    virtual void PassthroughCmdCallback(int id, int key_state,
                                        const RawAddress& bd_addr);
    virtual void SetAddressedPlayerCallback(uint16_t player_id,
                                            const RawAddress& bd_addr);
    virtual void SetBrowsedPlayerCallback(uint16_t player_id,
                                          const RawAddress& bd_addr);
    virtual void GetFolderItemsCallback(uint8_t scope, uint32_t start_item,
                                        uint32_t end_item, uint8_t num_attr,
                                        uint32_t* p_attr_ids,
                                        const RawAddress& bd_addr);
    virtual void ChangePathCallback(uint8_t direction, uint8_t* folder_uid,
                                    const RawAddress& bd_addr);
    virtual void GetItemAttrCallback(uint8_t scope, uint8_t* uid,
                                     uint16_t uid_counter, uint8_t num_attr,
                                     btrc_media_attr_t* p_attrs,
                                     const RawAddress& bd_addr);
    virtual void PlayItemCallback(uint8_t scope, uint16_t uid_counter,
                                  uint8_t* uid, const RawAddress& bd_addr);
    virtual void GetTotalNumOfItemsCallback(uint8_t scope,
                                            const RawAddress& bd_addr);
    virtual void SearchCallback(uint16_t str_len, uint8_t* p_str,
                                const RawAddress& bd_addr);
    virtual void AddToNowPlayingCallback(uint8_t scope, uint8_t* uid,
                                         uint16_t uid_counter,
                                         const RawAddress& bd_addr);
  };

  class ControlObserver {
   public:
    virtual ~ControlObserver() = default;

    virtual void PassthroughRspCallback(const RawAddress& bd_addr, int id,
                                        int key_state);
    virtual void GroupnavigationRspCallback(int id, int key_state);
    virtual void ConnectionStateCallback(bool rc_connect, bool bt_connect,
                                         const RawAddress& bd_addr);
    virtual void CtrlGetrcfeaturesCallback(const RawAddress& bd_addr,
                                           int features);
    virtual void CtrlSetplayerapplicationsettingRspCallback(
        const RawAddress& bd_addr, uint8_t accepted);
    virtual void CtrlPlayerapplicationsettingCallback(
        const RawAddress& bd_addr, uint8_t num_attr,
        btrc_player_app_attr_t* app_attrs, uint8_t num_ext_attr,
        btrc_player_app_ext_attr_t* ext_attrs);
    virtual void CtrlPlayerapplicationsettingChangedCallback(
        const RawAddress& bd_addr, const btrc_player_settings_t& p_vals);
    virtual void CtrlSetabsvolCmdCallback(const RawAddress& bd_addr,
                                          uint8_t abs_vol, uint8_t label);
    virtual void CtrlRegisternotificationAbsVolCallback(
        const RawAddress& bd_addr, uint8_t label);
    virtual void CtrlTrackChangedCallback(const RawAddress& bd_addr,
                                          uint8_t num_attr,
                                          btrc_element_attr_val_t* p_attrs);
    virtual void CtrlPlayPositionChangedCallback(const RawAddress& bd_addr,
                                                 uint32_t song_len,
                                                 uint32_t song_pos);
    virtual void CtrlPlayStatusChangedCallback(const RawAddress& bd_addr,
                                               btrc_play_status_t play_status);
    virtual void CtrlGetFolderItemsCallback(
        const RawAddress& bd_addr, btrc_status_t status,
        const btrc_folder_items_t* folder_items, uint8_t count);
    virtual void CtrlChangePathCallback(const RawAddress& bd_addr,
                                        uint32_t count);
    virtual void CtrlSetBrowsedPlayerCallback(const RawAddress& bd_addr,
                                              uint8_t num_items, uint8_t depth);
    virtual void CtrlSetAddressedPlayerCallback(const RawAddress& bd_addr,
                                                uint8_t status);
  };

  // Initialize and clean up the BluetoothInterface singleton. Returns false if
  // the underlying HAL interface failed to initialize, and true on success.
  static bool Initialize();

  // Shuts down and cleans up the interface. CleanUp must be called on the same
  // thread that called Initialize.
  static void CleanUp();

  // Returns true if the interface was initialized and a global singleton has
  // been created.
  static bool IsInitialized();

  // Initialize for testing. Use this to inject a test version of
  // BluetoothAvrcpInterface. To be used from unit tests only.
  static void InitializeForTesting(BluetoothAvrcpInterface* test_instance);

  // Returns the BluetoothAvrcpInterface singleton. If the interface has
  // not been initialized, returns nullptr. This method is thread-safe, in that
  // it will block if the internal lock is being held by another thread. Don't
  // call this re-entrantly from an observer event as this may cause a deadlock.
  static BluetoothAvrcpInterface* Get();

  virtual bool AvrcpControlEnable() = 0;
  virtual void AvrcpControlDisable() = 0;
  virtual bool AvrcpTargetEnable() = 0;
  virtual void AvrcpTargetDisable() = 0;

  // Thread-safety is guaranteed by ObserverList.
  virtual void AddTargetObserver(TargetObserver* observer) = 0;
  virtual void RemoveTargetObserver(TargetObserver* observer) = 0;

  // Thread-safety is guaranteed by ObserverList.
  virtual void AddControlObserver(ControlObserver* observer) = 0;
  virtual void RemoveControlObserver(ControlObserver* observer) = 0;

  // The HAL module pointers provided by the shared Bluetooth library
  virtual const btrc_interface_t* GetTargetHALInterface() const = 0;
  virtual const btrc_ctrl_interface_t* GetControlHALInterface() const = 0;

 protected:
  BluetoothAvrcpInterface() = default;
  virtual ~BluetoothAvrcpInterface() = default;

 private:
  DISALLOW_COPY_AND_ASSIGN(BluetoothAvrcpInterface);
};

}  // namespace hal
}  // namespace bluetooth
