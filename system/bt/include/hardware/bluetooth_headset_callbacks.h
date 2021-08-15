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

#include "bt_hf.h"

namespace bluetooth {
namespace headset {

/**
 * Headset related callbacks invoked from from the Bluetooth native stack
 * All callbacks are invoked on the JNI thread
 */
class Callbacks {
 public:
  virtual ~Callbacks() = default;
  /**
   * Callback for connection state change.
   *
   * @param state one of the values from bthf_connection_state_t
   * @param bd_addr remote device address
   */
  virtual void ConnectionStateCallback(bthf_connection_state_t state,
                                       RawAddress* bd_addr) = 0;

  /**
   * Callback for audio connection state change.
   *
   * @param state one of the values from bthf_audio_state_t
   * @param bd_addr remote device address
   */
  virtual void AudioStateCallback(bthf_audio_state_t state,
                                  RawAddress* bd_addr) = 0;

  /**
   * Callback for VR connection state change.
   *
   * @param state one of the values from bthf_vr_state_t
   * @param bd_addr
   */
  virtual void VoiceRecognitionCallback(bthf_vr_state_t state,
                                        RawAddress* bd_addr) = 0;

  /**
   * Callback for answer incoming call (ATA)
   *
   * @param bd_addr remote device address
   */
  virtual void AnswerCallCallback(RawAddress* bd_addr) = 0;

  /**
   * Callback for disconnect call (AT+CHUP)
   *
   * @param bd_addr remote device address
   */
  virtual void HangupCallCallback(RawAddress* bd_addr) = 0;

  /**
   * Callback for disconnect call (AT+CHUP)
   *
   * @param type denote Speaker/Mic gain bthf_volume_type_t
   * @param volume volume value 0 to 15, p69, HFP 1.7.1 spec
   * @param bd_addr remote device address
   */
  virtual void VolumeControlCallback(bthf_volume_type_t type, int volume,
                                     RawAddress* bd_addr) = 0;

  /**
   * Callback for dialing an outgoing call
   *
   * @param number intended phone number, if number is NULL, redial
   * @param bd_addr remote device address
   */
  virtual void DialCallCallback(char* number, RawAddress* bd_addr) = 0;

  /**
   * Callback for sending DTMF tones
   *
   * @param tone contains the dtmf character to be sent
   * @param bd_addr remote device address
   */
  virtual void DtmfCmdCallback(char tone, RawAddress* bd_addr) = 0;

  /**
   * Callback for enabling/disabling noise reduction/echo cancellation
   *
   * @param nrec 1 to enable, 0 to disable
   * @param bd_addr remote device address
   */
  virtual void NoiseReductionCallback(bthf_nrec_t nrec,
                                      RawAddress* bd_addr) = 0;

  /**
   * Callback for AT+BCS and event from BAC
   *
   * @param wbs WBS enable, WBS disable
   * @param bd_addr remote device address
   */
  virtual void WbsCallback(bthf_wbs_config_t wbs, RawAddress* bd_addr) = 0;

  /**
   * Callback for call hold handling (AT+CHLD)
   *
   * @param chld the call hold command (0, 1, 2, 3)
   * @param bd_addr remote device address
   */
  virtual void AtChldCallback(bthf_chld_type_t chld, RawAddress* bd_addr) = 0;

  /**
   * Callback for CNUM (subscriber number)
   *
   * @param bd_addr remote device address
   */
  virtual void AtCnumCallback(RawAddress* bd_addr) = 0;

  /**
   * Callback for indicators (CIND)
   *
   * @param bd_addr remote device address
   */
  virtual void AtCindCallback(RawAddress* bd_addr) = 0;

  /**
   * Callback for operator selection (COPS)
   *
   * @param bd_addr remote device address
   */
  virtual void AtCopsCallback(RawAddress* bd_addr) = 0;

  /**
   * Callback for call list (AT+CLCC)
   *
   * @param bd_addr remote device address
   */
  virtual void AtClccCallback(RawAddress* bd_addr) = 0;

  /**
   * Callback for unknown AT command recd from HF
   *
   * @param at_string he unparsed AT string
   * @param bd_addr remote device address
   */
  virtual void UnknownAtCallback(char* at_string, RawAddress* bd_addr) = 0;

  /**
   * Callback for keypressed (HSP) event.
   *
   * @param bd_addr remote device address
   */
  virtual void KeyPressedCallback(RawAddress* bd_addr) = 0;

  /**
   * Callback for BIND. Pass the remote HF Indicators supported.
   *
   * @param at_string unparsed AT command string
   * @param bd_addr remote device address
   */
  virtual void AtBindCallback(char* at_string, RawAddress* bd_addr) = 0;

  /**
   * Callback for BIEV. Pass the change in the Remote HF indicator values
   *
   * @param ind_id HF indicator id
   * @param ind_value HF indicator value
   * @param bd_addr remote device address
   */
  virtual void AtBievCallback(bthf_hf_ind_type_t ind_id, int ind_value,
                              RawAddress* bd_addr) = 0;

  /**
   * Callback for BIA. Pass the change in AG indicator activation.
   * NOTE: Call, Call Setup and Call Held indicators are mandatory and cannot
   *       be disabled. Thus, they are not included here.
   *
   * @param service whether HF should receive network service state update
   * @param roam whether HF should receive roaming state update
   * @param signal whether HF should receive signal strength update
   * @param battery whether HF should receive AG battery level update
   * @param bd_addr remote HF device address
   */
  virtual void AtBiaCallback(bool service, bool roam, bool signal, bool battery,
                             RawAddress* bd_addr) = 0;
};

}  // namespace headset
}  // namespace bluetooth