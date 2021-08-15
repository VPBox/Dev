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

#include <mutex>
#include <string>
#include <vector>

#include "base/macros.h"

#include "service/common/bluetooth/avrcp_int_value.h"
#include "service/common/bluetooth/avrcp_register_notification_response.h"
#include "service/common/bluetooth/avrcp_string_value.h"

#include "service/bluetooth_instance.h"
#include "service/hal/bluetooth_avrcp_interface.h"

namespace bluetooth {

// Note: presently this only supports
// (BTRC_FEAT_METADATA | BTRC_FEAT_ABSOLUTE_VOLUME)
class AvrcpTarget : public BluetoothInstance,
                    private hal::BluetoothAvrcpInterface::TargetObserver {
 public:
  // We only allow one instance of this object at a time.
  static const int kSingletonInstanceId;

  class Delegate {
   public:
    virtual void OnGetRemoteFeatures(const std::string& addr,
                                     int32_t features) = 0;
    virtual void OnGetPlayStatus(const std::string& addr) = 0;
    virtual void OnListPlayerAppAttr(const std::string& addr) = 0;
    virtual void OnListPlayerAppValues(const std::string& addr,
                                       int32_t attr_id) = 0;
    virtual void OnGetPlayerAppValue(const std::string& addr,
                                     const std::vector<int32_t>& attrs) = 0;
    virtual void OnGetPlayerAppAttrsText(const std::string& addr,
                                         const std::vector<int32_t>& attrs) = 0;
    virtual void OnGetPlayerAppValuesText(
        const std::string& addr, int32_t attr_id,
        const std::vector<int32_t>& values) = 0;
    virtual void OnSetPlayerAppValue(
        const std::string& addr, const std::vector<AvrcpIntValue>& values) = 0;
    virtual void OnGetElementAttrs(const std::string& addr,
                                   const std::vector<int32_t>& attrs) = 0;
    virtual void OnRegisterNotification(const std::string& addr,
                                        int32_t event_id, uint32_t param) = 0;
    virtual void OnVolumeChange(const std::string& addr, int32_t volume,
                                int32_t ctype) = 0;
    virtual void OnPassThroughCommand(const std::string& addr, int32_t id,
                                      int32_t key_state) = 0;

   protected:
    virtual ~Delegate() = default;
  };

  // The destructor automatically unregisters this instance from the stack.
  ~AvrcpTarget() override;

  // Assigns a delegate to this instance. |delegate| must out-live this
  // AvrcpTarget instance.
  void SetDelegate(Delegate* delegate);

  // BluetoothClientInstace overrides:
  const Uuid& GetAppIdentifier() const override;
  int GetInstanceId() const override;

  bool Enable();
  void Disable();

  bool GetPlayStatusResponse(const std::string& addr, int32_t play_status,
                             uint32_t song_len, uint32_t song_pos);

  bool ListPlayerAppAttrResponse(const std::string& addr,
                                 const std::vector<int32_t>& attrs);

  bool GetPlayerAppValueResponse(const std::string& addr,
                                 const std::vector<AvrcpIntValue>& values);

  bool GetPlayerAppAttrTextResponse(const std::string& addr,
                                    const std::vector<AvrcpStringValue>& attrs);

  bool GetPlayerAppValueTextResponse(
      const std::string& addr, const std::vector<AvrcpStringValue>& attrs);

  bool GetElementAttrResponse(const std::string& addr,
                              const std::vector<AvrcpStringValue>& attrs);

  bool SetPlayerAppValueResponse(const std::string& addr, int32_t rsp_status);

  bool RegisterNotificationResponse(
      int32_t event_id, int32_t type,
      const AvrcpRegisterNotificationResponse& param);

  bool SetVolume(int volume);

 private:
  friend class AvrcpTargetFactory;

  // Constructor shouldn't be called directly as instances are meant to be
  // obtained from the factory.
  AvrcpTarget(const Uuid& uuid);

  // hal::BluetoothAvrcpInterface::TargetObserver implementation:
  void RemoteFeaturesCallback(const RawAddress& bd_addr,
                              btrc_remote_features_t features) override;
  void GetPlayStatusCallback(const RawAddress& bd_addr) override;
  void ListPlayerAppAttrCallback(const RawAddress& bd_addr) override;
  void ListPlayerAppValuesCallback(btrc_player_attr_t attr_id,
                                   const RawAddress& bd_addr) override;
  void GetPlayerAppValueCallback(uint8_t num_attr, btrc_player_attr_t* p_attrs,
                                 const RawAddress& bd_addr) override;
  void GetPlayerAppAttrsTextCallback(uint8_t num_attr,
                                     btrc_player_attr_t* p_attrs,
                                     const RawAddress& bd_addr) override;
  void GetPlayerAppValuesTextCallback(uint8_t attr_id, uint8_t num_val,
                                      uint8_t* p_vals,
                                      const RawAddress& bd_addr) override;
  void SetPlayerAppValueCallback(btrc_player_settings_t* p_vals,
                                 const RawAddress& bd_addr) override;
  void GetElementAttrCallback(uint8_t num_attr, btrc_media_attr_t* p_attrs,
                              const RawAddress& bd_addr) override;
  void RegisterNotificationCallback(btrc_event_id_t event_id, uint32_t param,
                                    const RawAddress& bd_addr) override;
  void VolumeChangeCallback(uint8_t volume, uint8_t ctype,
                            const RawAddress& bd_addr) override;
  void PassthroughCmdCallback(int id, int key_state,
                              const RawAddress& bd_addr) override;

  // See getters for documentation.
  const Uuid app_identifier_;

  // Mutex that synchronizes access to the entries below.
  std::mutex mutex_;

  // Raw handle to the Delegate, which must outlive this AvrcpTarget instance.
  std::mutex delegate_mutex_;
  Delegate* delegate_ = nullptr;

  DISALLOW_COPY_AND_ASSIGN(AvrcpTarget);
};

// AvrcpTargetFactory is used to register and obtain a per-application
// AvrcpTarget
// instance. Users should call RegisterClient to obtain their own unique
// AvrcpTarget instance that has been registered with the Bluetooth stack.
class AvrcpTargetFactory
    : public BluetoothInstanceFactory,
      private hal::BluetoothAvrcpInterface::TargetObserver {
 public:
  // Don't construct/destruct directly except in tests. Instead, obtain a handle
  // from an Adapter instance.
  AvrcpTargetFactory();
  ~AvrcpTargetFactory() override;

  // BluetoothInstanceFactory override:
  bool RegisterInstance(const Uuid& uuid,
                        const RegisterCallback& callback) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(AvrcpTargetFactory);
};

}  // namespace bluetooth
