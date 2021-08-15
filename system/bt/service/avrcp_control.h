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

#include <atomic>
#include <mutex>

#include "base/macros.h"

#include "bluetooth/uuid.h"
#include "service/bluetooth_instance.h"
#include "service/common/bluetooth/avrcp_media_attr.h"
#include "service/common/bluetooth/service.h"
#include "service/hal/bluetooth_avrcp_interface.h"

namespace bluetooth {

class AvrcpControl : public BluetoothInstance,
                     private hal::BluetoothAvrcpInterface::ControlObserver {
 public:
  class Delegate {
   public:
    virtual void OnConnectionState(bool rc_connect, bool bt_connect,
                                   const std::string& device_address) = 0;

    virtual void OnTrackChanged(const std::string& device_address,
                                const AvrcpMediaAttr& attr) = 0;

    virtual void OnSetAbsVolumeRequest(const std::string& device_address,
                                       int32_t abs_vol, int32_t label) = 0;

    virtual void OnRegisterForAbsVolumeCallbackRequest(
        const std::string& device_address, int32_t label) = 0;

   protected:
    virtual ~Delegate() = default;
  };

  // The destructor automatically unregisters this instance from the stack.
  ~AvrcpControl() override;

  // Assigns a delegate to this instance. |delegate| must out-live this
  // AvrcpControl instance.
  void SetDelegate(Delegate* delegate);

  // BluetoothClientInstace overrides:
  const Uuid& GetAppIdentifier() const override;
  int GetInstanceId() const override;

  bool Enable();
  void Disable();

  // Send a remote control button command. Commands which can be sent
  // are defined here:
  // http://1394ta.org/wp-content/uploads/2015/07/2007001.pdf
  bool SendPassThroughCommand(const std::string& device_address,
                              uint8_t key_code, bool key_pressed);

  // Send a response to a request to change absolute volume.
  bool SetAbsVolumeResponse(const std::string& device_address, int32_t abs_vol,
                            int32_t label);

  // Send a response to a register for absolute volume change callback.
  bool RegisterForAbsVolumeCallbackResponse(const std::string& device_address,
                                            int32_t response_type,
                                            int32_t abs_vol, int32_t label);

 private:
  friend class AvrcpControlFactory;

  // Constructor shouldn't be called directly as instances are meant to be
  // obtained from the factory.
  AvrcpControl(const Uuid& uuid, int control_id);

  // hal::BluetoothAvrcpInterface::ControlObserver implementation:
  void ConnectionStateCallback(bool rc_connect, bool bt_connect,
                               const RawAddress& bd_addr) override;
  void CtrlSetabsvolCmdCallback(const RawAddress& bd_addr, uint8_t abs_vol,
                                uint8_t label) override;
  void CtrlRegisternotificationAbsVolCallback(const RawAddress& bd_addr,
                                              uint8_t label) override;
  void CtrlTrackChangedCallback(const RawAddress& bd_addr, uint8_t num_attr,
                                btrc_element_attr_val_t* p_attrs) override;

  // See getters for documentation.
  const Uuid app_identifier_;
  const int control_id_;

  // Mutex that synchronizes access to the entries below.
  std::mutex mutex_;

  // Raw handle to the Delegate, which must outlive this AvrcpControl instance.
  std::mutex delegate_mutex_;
  Delegate* delegate_ = nullptr;

  DISALLOW_COPY_AND_ASSIGN(AvrcpControl);
};

// AvrcpControlFactory is used to register and obtain a per-application
// AvrcpControl
// instance. Users should call RegisterClient to obtain their own unique
// AvrcpControl instance that has been registered with the Bluetooth stack.
class AvrcpControlFactory
    : public BluetoothInstanceFactory,
      private hal::BluetoothAvrcpInterface::ControlObserver {
 public:
  // Don't construct/destruct directly except in tests. Instead, obtain a handle
  // from an Adapter instance.
  AvrcpControlFactory();
  ~AvrcpControlFactory() override;

  // BluetoothInstanceFactory override:
  bool RegisterInstance(const Uuid& uuid,
                        const RegisterCallback& callback) override;

 private:
  std::atomic<int> next_control_id_{0};
  DISALLOW_COPY_AND_ASSIGN(AvrcpControlFactory);
};

}  // namespace bluetooth
