/*
 * Copyright (C) 2012 The Android Open Source Project
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

namespace bluetooth {
namespace headset {

/* AT response code - OK/Error */
typedef enum {
  BTHF_AT_RESPONSE_ERROR = 0,
  BTHF_AT_RESPONSE_OK
} bthf_at_response_t;

typedef enum {
  BTHF_CONNECTION_STATE_DISCONNECTED = 0,
  BTHF_CONNECTION_STATE_CONNECTING,
  BTHF_CONNECTION_STATE_CONNECTED,
  BTHF_CONNECTION_STATE_SLC_CONNECTED,
  BTHF_CONNECTION_STATE_DISCONNECTING
} bthf_connection_state_t;

typedef enum {
  BTHF_AUDIO_STATE_DISCONNECTED = 0,
  BTHF_AUDIO_STATE_CONNECTING,
  BTHF_AUDIO_STATE_CONNECTED,
  BTHF_AUDIO_STATE_DISCONNECTING
} bthf_audio_state_t;

typedef enum {
  BTHF_VR_STATE_STOPPED = 0,
  BTHF_VR_STATE_STARTED
} bthf_vr_state_t;

typedef enum {
  BTHF_VOLUME_TYPE_SPK = 0,
  BTHF_VOLUME_TYPE_MIC
} bthf_volume_type_t;

/* Noise Reduction and Echo Cancellation */
typedef enum { BTHF_NREC_STOP, BTHF_NREC_START } bthf_nrec_t;

/* WBS codec setting */
typedef enum { BTHF_WBS_NONE, BTHF_WBS_NO, BTHF_WBS_YES } bthf_wbs_config_t;

/* CHLD - Call held handling */
typedef enum {
  BTHF_CHLD_TYPE_RELEASEHELD,  // Terminate all held or set UDUB("busy") to a
                               // waiting call
  BTHF_CHLD_TYPE_RELEASEACTIVE_ACCEPTHELD,  // Terminate all active calls and
                                            // accepts a waiting/held call
  BTHF_CHLD_TYPE_HOLDACTIVE_ACCEPTHELD,  // Hold all active calls and accepts a
                                         // waiting/held call
  BTHF_CHLD_TYPE_ADDHELDTOCONF,          // Add all held calls to a conference
} bthf_chld_type_t;

/* HF Indicators HFP 1.7 */
typedef enum {
  BTHF_HF_IND_ENHANCED_DRIVER_SAFETY = 1,
  BTHF_HF_IND_BATTERY_LEVEL_STATUS = 2,
} bthf_hf_ind_type_t;

typedef enum {
  BTHF_HF_IND_DISABLED = 0,
  BTHF_HF_IND_ENABLED,
} bthf_hf_ind_status_t;

/** Network Status */
typedef enum {
  BTHF_NETWORK_STATE_NOT_AVAILABLE = 0,
  BTHF_NETWORK_STATE_AVAILABLE
} bthf_network_state_t;

/** Service type */
typedef enum {
  BTHF_SERVICE_TYPE_HOME = 0,
  BTHF_SERVICE_TYPE_ROAMING
} bthf_service_type_t;

typedef enum {
  BTHF_CALL_STATE_ACTIVE = 0,
  BTHF_CALL_STATE_HELD,
  BTHF_CALL_STATE_DIALING,
  BTHF_CALL_STATE_ALERTING,
  BTHF_CALL_STATE_INCOMING,
  BTHF_CALL_STATE_WAITING,
  BTHF_CALL_STATE_IDLE,
  BTHF_CALL_STATE_DISCONNECTED
} bthf_call_state_t;

typedef enum {
  BTHF_CALL_DIRECTION_OUTGOING = 0,
  BTHF_CALL_DIRECTION_INCOMING
} bthf_call_direction_t;

typedef enum {
  BTHF_CALL_TYPE_VOICE = 0,
  BTHF_CALL_TYPE_DATA,
  BTHF_CALL_TYPE_FAX
} bthf_call_mode_t;

typedef enum {
  BTHF_CALL_MPTY_TYPE_SINGLE = 0,
  BTHF_CALL_MPTY_TYPE_MULTI
} bthf_call_mpty_type_t;

typedef enum {
  BTHF_CALL_ADDRTYPE_UNKNOWN = 0x81,
  BTHF_CALL_ADDRTYPE_INTERNATIONAL = 0x91
} bthf_call_addrtype_t;

}  // namespace headset
}  // namespace bluetooth