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

#include <base/bind.h>
#include <base/memory/weak_ptr.h>
#include <map>
#include <memory>

#include "avrcp_internal.h"
#include "packet/avrcp/avrcp_packet.h"
#include "packet/base/packet.h"
#include "profile/avrcp/device.h"
#include "raw_address.h"

namespace bluetooth {
namespace avrcp {

// TODO: Remove the singleton design structure for this class.
// AvrcpTargetService is already a singleton and can manage the lifetime of this
// object. multiple singleton objects can lead to code that is hard to test and
// have hard to debug lifetimes.

// TODO (apanicke): Use a device factory instead of just the constructor in
// order to create device objects. This will allow us to create specific device
// classes that can provide interop fixes for certain badly behaving devices.

/**
 * ConnectionHandler handles SDP, connecting to remote AVRCP devices
 * and multiplexing/delivering messages to devices.
 */
class ConnectionHandler {
 public:
  /**
   * This callback is used to return a new device after a connection attempt.
   * A reference to the new Avrcp device is located in the shared_ptr.
   * If there was an issue during connection the pointer value will be null.
   */
  using ConnectionCallback = base::Callback<void(std::shared_ptr<Device>)>;

  /**
   * Initializes the singleton instance and sets up SDP. Also Opens the
   * AVRCP Acceptor to receive connection requests from a remote device.
   *
   * Params:
   * callback - A callback that gets called any time a new AVRCP Device
   *            is connected. Will return nullpointer if a device fails
   *            to connect via ConnectDevice();
   *
   * TODO: Add message loop to determine which thread events are posted to
   */
  static bool Initialize(const ConnectionCallback& callback,
                         AvrcpInterface* avrcp, SdpInterface* sdp,
                         VolumeInterface* vol);

  /**
   * Clears the singleton and tears down SDP
   */
  static bool CleanUp();

  /**
   * Get the singleton instance of Connection Handler
   */
  static ConnectionHandler* Get();

  /**
   * Attempt to connect AVRCP on a device. The callback will be called with
   * either a smart pointer pointing to the connected AVRCP device or null
   * if the connection failed.
   *
   * The order of operations for this function is as follows.
   *   1. Perform SDP on remote device
   *   2. Connect the AVCTP Channel
   *   2. (Optional) If supported connect the AVCTP Browse channel
   *   4. Call the provided callback with the new
   *
   * Params:
   * bdaddr - Bluetooth address of device to connect to
   * callback - The function that gets called when a connection succeeds or
   *            fails. The pointer being cleared implies that the connection
   *            failed.
   *
   * Returns:
   * true if the connection attempt starts, false if there are no resources to
   * connect AVRCP
   */
  virtual bool ConnectDevice(const RawAddress& bdaddr);

  /**
   * Disconnects AVRCP from a device that was successfully connected too using
   * ConnectionHandler::ConnectDevice
   *
   * Returns:
   * true if the AVRCP was successfully disconnected for the device or false
   * if the device was already disconnected or in an invalid state
   */
  virtual bool DisconnectDevice(const RawAddress& bdaddr);

  virtual std::vector<std::shared_ptr<Device>> GetListOfDevices() const;

  /**
   * Provide a custom ConnectionHandler that will be returned by Get().
   * Initialize and CleanUp should not be called as the owner of the handler
   * determines its lifetime.
   */
  static void InitForTesting(ConnectionHandler* handler);

 private:
  AvrcpInterface* avrc_;
  SdpInterface* sdp_;
  VolumeInterface* vol_;

  ConnectionCallback connection_cb_;

  std::map<uint8_t, std::shared_ptr<Device>> device_map_;
  // TODO (apanicke): Replace the features with a class that has individual
  // fields.
  std::map<RawAddress, uint16_t> feature_map_;

  static ConnectionHandler* instance_;

  using SdpCallback = base::Callback<void(uint16_t status, uint16_t version,
                                          uint16_t features)>;
  virtual bool SdpLookup(const RawAddress& bdaddr, SdpCallback cb);
  void SdpCb(const RawAddress& bdaddr, SdpCallback cb,
             tSDP_DISCOVERY_DB* disc_db, uint16_t status);

  virtual bool AvrcpConnect(bool initiator, const RawAddress& bdaddr);

  // Callbacks when connecting to a device
  void InitiatorControlCb(uint8_t handle, uint8_t event, uint16_t result,
                          const RawAddress* peer_addr);
  void AcceptorControlCb(uint8_t handle, uint8_t event, uint16_t result,
                         const RawAddress* peer_addr);
  void MessageCb(uint8_t handle, uint8_t label, uint8_t opcode,
                 tAVRC_MSG* p_msg);

  ConnectionHandler() : weak_ptr_factory_(this){};
  virtual ~ConnectionHandler() = default;

  // Callback for when sending a response to a device
  void SendMessage(uint8_t handle, uint8_t label, bool browse,
                   std::unique_ptr<::bluetooth::PacketBuilder> message);

  base::WeakPtrFactory<ConnectionHandler> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(ConnectionHandler);
};

}  // namespace avrcp
}  // namespace bluetooth
