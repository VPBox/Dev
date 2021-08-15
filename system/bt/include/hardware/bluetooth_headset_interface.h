/*
 * Copyright 2017 The Android Open Source Project
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

#include "bluetooth_headset_callbacks.h"
#include "bt_hf.h"

namespace bluetooth {
namespace headset {

/**
 * Programming interface for Headset profiles in the Fluoride stack
 * Thread-safe
 */
class Interface {
 public:
  virtual ~Interface() = default;
  /**
   * Register the BtHf callbacks
   *
   * @param callbacks callbacks for the user of the native stack
   * @param max_hf_clients maximum number of headset clients
   * @param inband_ringing_enabled whether inband ringtone is enabled
   * @return BT_STATUS_SUCCESS on success
   */
  virtual bt_status_t Init(Callbacks* callbacks, int max_hf_clients,
                           bool inband_ringing_enabled) = 0;

  /**
   * Connect to headset
   *
   * @param bd_addr remote device address
   * @return BT_STATUS_SUCCESS on success
   */
  virtual bt_status_t Connect(RawAddress* bd_addr) = 0;

  /**
   * Disconnect from headset
   *
   * @param bd_addr remote device address
   * @return BT_STATUS_SUCCESS on success
   */
  virtual bt_status_t Disconnect(RawAddress* bd_addr) = 0;

  /**
   * Create an audio connection
   *
   * @param bd_addr remote device address
   * @return BT_STATUS_SUCCESS on success
   */
  virtual bt_status_t ConnectAudio(RawAddress* bd_addr) = 0;

  /**
   * Close the audio connection
   *
   * @param bd_addr remote device address
   * @return BT_STATUS_SUCCESS on success
   */
  virtual bt_status_t DisconnectAudio(RawAddress* bd_addr) = 0;

  /** start voice recognition */
  /**
   * Start voice recognition
   * @param bd_addr remote device address
   * @return BT_STATUS_SUCCESS on success
   */
  virtual bt_status_t StartVoiceRecognition(RawAddress* bd_addr) = 0;

  /**
   * Stop voice recognition
   *
   * @param bd_addr remote device address
   * @return BT_STATUS_SUCCESS on success
   */
  virtual bt_status_t StopVoiceRecognition(RawAddress* bd_addr) = 0;

  /**
   * Change HFP related volume on remote headset
   *
   * @param type Speaker (+VGS) or Mic (+VGM)
   * @param volume volume level on scale from 0 to 15, p69, HFP 1.7.1 spec
   * @param bd_addr remote device address
   * @return BT_STATUS_SUCCESS on success
   */
  virtual bt_status_t VolumeControl(bthf_volume_type_t type, int volume,
                                    RawAddress* bd_addr) = 0;

  /**
   * Combined device status change notification
   *
   * @param ntk_state Network state, available or not available
   * @param svc_type Service type, roaming or home
   * @param signal Signal strength, 0 to 5, p86, HFP 1.7.1 spec
   * @param batt_chg Battery level of the phone, 0 to 5, p87, HFP 1.7.1 spec
   * @param bd_addr remote device address
   * @return BT_STATUS_SUCCESS on success
   */
  virtual bt_status_t DeviceStatusNotification(bthf_network_state_t ntk_state,
                                               bthf_service_type_t svc_type,
                                               int signal, int batt_chg,
                                               RawAddress* bd_addr) = 0;

  /**
   * Response for COPS (Query Operator Selection) command
   *
   * @param cops Operator Name, max length 16 char, p32 HFP 1.7.1 spec
   * @param bd_addr remote device address
   * @return BT_STATUS_SUCCESS on success
   */
  virtual bt_status_t CopsResponse(const char* cops, RawAddress* bd_addr) = 0;

  /**
   * Response for CIND (Stanford Indicator Update) command
   *
   * @param svc service availability, available or not available
   * @param num_active number of active calls
   * @param num_held number of held calls
   * @param call_setup_state call setup state
   * @param signal signal strength, 0 to 5, p86 HFP 1.7.1 spec
   * @param roam roaming state, 1 for roaming, 0 for home, p86 HFP 1.7.1 spec
   * @param batt_chg AG battery charge, 0 to 5, p87 HFP 1.7.1 spec
   * @param bd_addr remote device address
   * @return BT_STATUS_SUCCESS on success
   */
  virtual bt_status_t CindResponse(int svc, int num_active, int num_held,
                                   bthf_call_state_t call_setup_state,
                                   int signal, int roam, int batt_chg,
                                   RawAddress* bd_addr) = 0;

  /**
   * Pre-formatted AT response, typically in response to unknown AT cmd
   *
   * @param rsp formatted AT response
   * @param bd_addr remote device address
   * @return BT_STATUS_SUCCESS on success
   */
  virtual bt_status_t FormattedAtResponse(const char* rsp,
                                          RawAddress* bd_addr) = 0;

  /**
   * ok/error response to AT commands
   *
   * @param response_code OK or ERROR
   * @param error_code actual error code depend on use case
   * @param bd_addr remote device address
   * @return BT_STATUS_SUCCESS on success
   */
  virtual bt_status_t AtResponse(bthf_at_response_t response_code,
                                 int error_code, RawAddress* bd_addr) = 0;

  /**
   * Response for CLCC (Current List of Calls) command.
   * Can be iteratively called for each call index
   * Call index of 0 will be treated as NULL termination (Completes response)
   *
   * @param index index of the call
   * @param dir direction of the call
   * @param state state of the call
   * @param mode mode of the call
   * @param mpty whether the call is multi party
   * @param number phone number of the call
   * @param type type of the call
   * @param bd_addr remote device address
   * @return BT_STATUS_SUCCESS on success
   */
  virtual bt_status_t ClccResponse(
      int index, bthf_call_direction_t dir, bthf_call_state_t state,
      bthf_call_mode_t mode, bthf_call_mpty_type_t mpty, const char* number,
      bthf_call_addrtype_t type, RawAddress* bd_addr) = 0;

  /**
   * Notify of a call state change
   *  Each update notifies
   *    1. Number of active/held/ringing calls
   *    2. call_state: This denotes the state change that triggered this msg
   *                   This will take one of the values from BtHfCallState
   *    3. number & type: valid only for incoming & waiting call
   *
   * @param num_active number of active calls
   * @param num_held number of held calls
   * @param call_setup_state current call setup state
   * @param number phone number of the call
   * @param type type of the call
   * @param name caller display name
   * @param bd_addr remote device address
   * @return BT_STATUS_SUCCESS on success
   */
  virtual bt_status_t PhoneStateChange(int num_active, int num_held,
                                       bthf_call_state_t call_setup_state,
                                       const char* number,
                                       bthf_call_addrtype_t type,
                                       const char* name,
                                       RawAddress* bd_addr) = 0;

  /**
   * Closes the interface.
   */
  virtual void Cleanup() = 0;

  /**
   * Whether we are allowed to initiate SCO
   *
   * @param value true to allow, false to disallow
   * @return BT_STATUS_SUCCESS on success
   */
  virtual bt_status_t SetScoAllowed(bool value) = 0;

  /**
   * Send +BSIR response code to enable/disable in-band ringtone in an active
   * HFP service level connection
   *
   * @param value true for enabled, false for disable
   * @param bd_addr remote device address
   */
  virtual bt_status_t SendBsir(bool value, RawAddress* bd_addr) = 0;

  /**
   * Set the current active headset device for SCO audio
   *
   * @param active_device_addr remote device address
   */
  virtual bt_status_t SetActiveDevice(RawAddress* active_device_addr) = 0;
};

}  // namespace headset
}  // namespace bluetooth
