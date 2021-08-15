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

#pragma once

#include <map>
#include <memory>

#include "hardware/avrcp/avrcp.h"
#include "osi/include/properties.h"
#include "profile/avrcp/connection_handler.h"
#include "raw_address.h"

namespace bluetooth {
namespace avrcp {

/**
 * AvrcpService is the management interface for AVRCP Target. It handles any
 * required thread switching, interface registration, and provides an API
 * for connecting and disconnecting devices.
 * TODO (apanicke): Instead of providing a service interface implementation,
 * have the AvrcpService itself be its interface so we don't have to access
 * it indirectly.
 */
class AvrcpService : public MediaCallbacks {
 public:
  /**
   * Gets a handle to the AvrcpService
   *
   * Currently used by A2DP to tell AVRCP to initiate a connection to the
   * remote device.
   */
  static AvrcpService* Get();

  /**
   * Returns an interface to control this service. The Avrcp::ServiceInterface
   * handles all thread switching between the caller thread and the thread the
   * service runs on, that way whoever uses the interface doesn't need to be
   * aware which thread the service runs on.
   */
  static ServiceInterface* GetServiceInterface();

  void Init(MediaInterface* media_interface, VolumeInterface* volume_interface);
  void Cleanup();

  void ConnectDevice(const RawAddress& bdaddr);
  void DisconnectDevice(const RawAddress& bdaddr);

  // Functions inherited from MediaCallbacks in order to receive updates
  void SendMediaUpdate(bool track_changed, bool play_state,
                       bool queue) override;
  void SendFolderUpdate(bool available_players, bool addressed_player,
                        bool queue) override;
  void SendActiveDeviceChanged(const RawAddress& address) override;

  class ServiceInterfaceImpl : public ServiceInterface {
   public:
    void Init(MediaInterface* media_interface,
              VolumeInterface* volume_interface) override;
    bool ConnectDevice(const RawAddress& bdaddr) override;
    bool DisconnectDevice(const RawAddress& bdaddr) override;
    bool Cleanup() override;

   private:
    std::mutex service_interface_lock_;
  };

  static void DebugDump(int fd);

 protected:
  void DeviceCallback(std::shared_ptr<Device> device);

 private:
  static AvrcpService* instance_;
  static ServiceInterfaceImpl* service_interface_;

  MediaInterface* media_interface_ = nullptr;
  VolumeInterface* volume_interface_ = nullptr;

  ConnectionHandler* connection_handler_;
};

}  // namespace avrcp
}  // namespace bluetooth

inline bool is_new_avrcp_enabled() {
  return osi_property_get_bool("persist.bluetooth.enablenewavrcp", true);
}