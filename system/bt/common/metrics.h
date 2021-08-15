/******************************************************************************
 *
 *  Copyright 2016 Google, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#pragma once

#include <android/bluetooth/hci/enums.pb.h>
#include <bta/include/bta_api.h>
#include <frameworks/base/core/proto/android/bluetooth/enums.pb.h>
#include <stdint.h>
#include <memory>
#include <string>

namespace bluetooth {

namespace common {

// Typedefs to hide protobuf definition to the rest of stack

typedef enum {
  DEVICE_TYPE_UNKNOWN,
  DEVICE_TYPE_BREDR,
  DEVICE_TYPE_LE,
  DEVICE_TYPE_DUMO,
} device_type_t;

typedef enum {
  WAKE_EVENT_UNKNOWN,
  WAKE_EVENT_ACQUIRED,
  WAKE_EVENT_RELEASED,
} wake_event_type_t;

typedef enum {
  SCAN_TYPE_UNKNOWN,
  SCAN_TECH_TYPE_LE,
  SCAN_TECH_TYPE_BREDR,
  SCAN_TECH_TYPE_BOTH,
} scan_tech_t;

typedef enum {
  CONNECTION_TECHNOLOGY_TYPE_UNKNOWN,
  CONNECTION_TECHNOLOGY_TYPE_LE,
  CONNECTION_TECHNOLOGY_TYPE_BREDR,
} connection_tech_t;

typedef enum {
  DISCONNECT_REASON_UNKNOWN,
  DISCONNECT_REASON_METRICS_DUMP,
  DISCONNECT_REASON_NEXT_START_WITHOUT_END_PREVIOUS,
} disconnect_reason_t;

/* Values of A2DP metrics that we care about
 *
 *    audio_duration_ms : sum of audio duration (in milliseconds).
 *    device_class: device class of the paired device.
 *    media_timer_min_ms : minimum scheduled time (in milliseconds)
 *                         of the media timer.
 *    media_timer_max_ms: maximum scheduled time (in milliseconds)
 *                        of the media timer.
 *    media_timer_avg_ms: average scheduled time (in milliseconds)
 *                        of the media timer.
 *    buffer_overruns_max_count: TODO - not clear what this is.
 *    buffer_overruns_total : number of times the media buffer with
 *                            audio data has overrun
 *    buffer_underruns_average: TODO - not clear what this is.
 *    buffer_underruns_count: number of times there was no enough
 *                            audio data to add to the media buffer.
 * NOTE: Negative values are invalid
 */
class A2dpSessionMetrics {
 public:
  A2dpSessionMetrics() {}

  /*
   * Update the metrics value in the current metrics object using the metrics
   * objects supplied
   */
  void Update(const A2dpSessionMetrics& metrics);

  /*
   * Compare whether two metrics objects are equal
   */
  bool operator==(const A2dpSessionMetrics& rhs) const;

  /*
   * Initialize all values to -1 which is invalid in order to make a distinction
   * between 0 and invalid values
   */
  int64_t audio_duration_ms = -1;
  int32_t media_timer_min_ms = -1;
  int32_t media_timer_max_ms = -1;
  int32_t media_timer_avg_ms = -1;
  int64_t total_scheduling_count = -1;
  int32_t buffer_overruns_max_count = -1;
  int32_t buffer_overruns_total = -1;
  float buffer_underruns_average = -1;
  int32_t buffer_underruns_count = -1;
  int64_t codec_index = -1;
  bool is_a2dp_offload = false;
};

class BluetoothMetricsLogger {
 public:
  static BluetoothMetricsLogger* GetInstance() {
    static BluetoothMetricsLogger* instance = new BluetoothMetricsLogger();
    return instance;
  }

  /*
   * Record a pairing event
   *
   * Parameters:
   *    timestamp_ms: Unix epoch time in milliseconds
   *    device_class: class of remote device
   *    device_type: type of remote device
   *    disconnect_reason: HCI reason for pairing disconnection.
   *                       See: stack/include/hcidefs.h
   */
  void LogPairEvent(uint32_t disconnect_reason, uint64_t timestamp_ms,
                    uint32_t device_class, device_type_t device_type);

  /*
   * Record a wake event
   *
   * Parameters:
   *    timestamp_ms: Unix epoch time in milliseconds
   *    type: whether it was acquired or released
   *    requestor: if provided is the service requesting the wake lock
   *    name: the name of the wake lock held
   */
  void LogWakeEvent(wake_event_type_t type, const std::string& requestor,
                    const std::string& name, uint64_t timestamp_ms);

  /*
   * Record a scan event
   *
   * Parameters
   *    timestamp_ms : Unix epoch time in milliseconds
   *    start : true if this is the beginning of the scan
   *    initiator: a unique ID identifying the app starting the scan
   *    type: whether the scan reports BR/EDR, LE, or both.
   *    results: number of results to be reported.
   */
  void LogScanEvent(bool start, const std::string& initator, scan_tech_t type,
                    uint32_t results, uint64_t timestamp_ms);

  /*
   * Start logging a Bluetooth session
   *
   * A Bluetooth session is defined a a connection between this device and
   * another remote device which may include multiple profiles and protocols
   *
   * Only one Bluetooth session can exist at one time. Calling this method twice
   * without LogBluetoothSessionEnd will result in logging a premature end of
   * current Bluetooth session
   *
   * Parameters:
   *    connection_tech_type : type of connection technology
   *    timestamp_ms : the timestamp for session start, 0 means now
   *
   */
  void LogBluetoothSessionStart(connection_tech_t connection_tech_type,
                                uint64_t timestamp_ms);

  /*
   * Stop logging a Bluetooth session and pushes it to the log queue
   *
   * If no Bluetooth session exist, this method exits immediately
   *
   * Parameters:
   *    disconnect_reason : A string representation of disconnect reason
   *    timestamp_ms : the timestamp of session end, 0 means now
   *
   */
  void LogBluetoothSessionEnd(disconnect_reason_t disconnect_reason,
                              uint64_t timestamp_ms);

  /*
   * Log information about remote device in a current Bluetooth session
   *
   * If a Bluetooth session does not exist, create one with default parameter
   * and timestamp now
   *
   * Parameters:
   *    device_class : device_class defined in btm_api_types.h
   *    device_type : type of remote device
   */
  void LogBluetoothSessionDeviceInfo(uint32_t device_class,
                                     device_type_t device_type);

  /*
   * Log A2DP Audio Session Information
   *
   * - Repeated calls to this method will override previous metrics if in the
   *   same Bluetooth connection
   * - If a Bluetooth session does not exist, create one with default parameter
   *   and timestamp now
   *
   * Parameters:
   *    a2dp_session_metrics - pointer to struct holding a2dp stats
   *
   */
  void LogA2dpSession(const A2dpSessionMetrics& a2dp_session_metrics);

  /**
   * Log Headset profile RFCOMM connection event
   *
   * @param service_id the BTA service ID for this headset connection
   */
  void LogHeadsetProfileRfcConnection(tBTA_SERVICE_ID service_id);

  /*
   * Writes the metrics, in base64 protobuf format, into the descriptor FD,
   * metrics events are always cleared after dump
   */
  void WriteBase64(int fd);
  void WriteBase64String(std::string* serialized);
  void WriteString(std::string* serialized);

  /*
   * Reset the metrics logger by cleaning up its staging queues and existing
   * protobuf objects.
   */
  void Reset();

  /*
   * Maximum number of log entries for each session or event
   */
  static const size_t kMaxNumBluetoothSession = 50;
  static const size_t kMaxNumPairEvent = 50;
  static const size_t kMaxNumWakeEvent = 1000;
  static const size_t kMaxNumScanEvent = 50;

 private:
  BluetoothMetricsLogger();

  /*
   * When a Bluetooth session is on and the user initiates a metrics dump, we
   * need to be able to upload whatever we have first. This method breaks the
   * ongoing Bluetooth session into two sessions with the previous one labeled
   * as "METRICS_DUMP" for the disconnect reason.
   */
  void CutoffSession();

  /*
   * Build the internal metrics object using information gathered
   */
  void Build();

  /*
   * Reset objects related to current Bluetooth session
   */
  void ResetSession();

  /*
   * Reset the underlining BluetoothLog object
   */
  void ResetLog();

  /*
   * PIMPL style implementation to hide internal dependencies
   */
  struct impl;
  std::unique_ptr<impl> const pimpl_;
};

/**
 * Unknown connection handle for metrics purpose
 */
static const uint32_t kUnknownConnectionHandle = 0xFFFF;

/**
 * Log link layer connection event
 *
 * @param address Stack wide consistent Bluetooth address of this event,
 *                nullptr if unknown
 * @param connection_handle connection handle of this event,
 *                          {@link kUnknownConnectionHandle} if unknown
 * @param direction direction of this connection
 * @param link_type type of the link
 * @param hci_cmd HCI command opecode associated with this event, if any
 * @param hci_event HCI event code associated with this event, if any
 * @param hci_ble_event HCI BLE event code associated with this event, if any
 * @param cmd_status Command status associated with this event, if any
 * @param reason_code Reason code associated with this event, if any
 */
void LogLinkLayerConnectionEvent(const RawAddress* address,
                                 uint32_t connection_handle,
                                 android::bluetooth::DirectionEnum direction,
                                 uint16_t link_type, uint32_t hci_cmd,
                                 uint16_t hci_event, uint16_t hci_ble_event,
                                 uint16_t cmd_status, uint16_t reason_code);

/**
 * Logs when Bluetooth controller failed to reply with command status within
 * a timeout period after receiving an HCI command from the host
 *
 * @param hci_cmd opcode of HCI command that caused this timeout
 */
void LogHciTimeoutEvent(uint32_t hci_cmd);

/**
 * Logs when we receive Bluetooth Read Remote Version Information Complete
 * Event from the remote device, as documented by the Bluetooth Core HCI
 * specification
 *
 * Reference: 5.0 Core Specification, Vol 2, Part E, Page 1118
 *
 * @param handle handle of associated ACL connection
 * @param status HCI command status of this event
 * @param version version code from read remote version complete event
 * @param manufacturer_name manufacturer code from read remote version complete
 *                          event
 * @param subversion subversion code from read remote version complete event
 */
void LogRemoteVersionInfo(uint16_t handle, uint8_t status, uint8_t version,
                          uint16_t manufacturer_name, uint16_t subversion);

/**
 * Log A2DP audio buffer underrun event
 *
 * @param address A2DP device associated with this event
 * @param encoding_interval_millis encoding interval in milliseconds
 * @param num_missing_pcm_bytes number of PCM bytes that cannot be read from
 *                              the source
 */
void LogA2dpAudioUnderrunEvent(const RawAddress& address,
                               uint64_t encoding_interval_millis,
                               int num_missing_pcm_bytes);

/**
 * Log A2DP audio buffer overrun event
 *
 * @param address A2DP device associated with this event
 * @param encoding_interval_millis encoding interval in milliseconds
 * @param num_dropped_buffers number of encoded buffers dropped from Tx queue
 * @param num_dropped_encoded_frames number of encoded frames dropped from Tx
 *                                   queue
 * @param num_dropped_encoded_bytes number of encoded bytes dropped from Tx
 *                                  queue
 */
void LogA2dpAudioOverrunEvent(const RawAddress& address,
                              uint64_t encoding_interval_millis,
                              int num_dropped_buffers,
                              int num_dropped_encoded_frames,
                              int num_dropped_encoded_bytes);

/**
 * Log read RSSI result
 *
 * @param address device associated with this event
 * @param handle connection handle of this event,
 *               {@link kUnknownConnectionHandle} if unknown
 * @param cmd_status command status from read RSSI command
 * @param rssi rssi value in dBm
 */
void LogReadRssiResult(const RawAddress& address, uint16_t handle,
                       uint32_t cmd_status, int8_t rssi);

/**
 * Log failed contact counter report
 *
 * @param address device associated with this event
 * @param handle connection handle of this event,
 *               {@link kUnknownConnectionHandle} if unknown
 * @param cmd_status command status from read failed contact counter command
 * @param failed_contact_counter Number of consecutive failed contacts for a
 *                               connection corresponding to the Handle
 */
void LogReadFailedContactCounterResult(const RawAddress& address,
                                       uint16_t handle, uint32_t cmd_status,
                                       int32_t failed_contact_counter);

/**
 * Log transmit power level for a particular device after read
 *
 * @param address device associated with this event
 * @param handle connection handle of this event,
 *               {@link kUnknownConnectionHandle} if unknown
 * @param cmd_status command status from read failed contact counter command
 * @param transmit_power_level transmit power level for connection to this
 *                             device
 */
void LogReadTxPowerLevelResult(const RawAddress& address, uint16_t handle,
                               uint32_t cmd_status,
                               int32_t transmit_power_level);

/**
 * Logs when there is an event related to Bluetooth Security Manager Protocol
 *
 * @param address address of associated device
 * @param smp_cmd SMP command code associated with this event
 * @param direction direction of this SMP command
 * @param smp_fail_reason SMP pairing failure reason code from SMP spec
 */
void LogSmpPairingEvent(const RawAddress& address, uint8_t smp_cmd,
                        android::bluetooth::DirectionEnum direction,
                        uint8_t smp_fail_reason);

/**
 * Logs there is an event related Bluetooth classic pairing
 *
 * @param address address of associated device
 * @param handle connection handle of this event,
 *               {@link kUnknownConnectionHandle} if unknown
 * @param hci_cmd HCI command associated with this event
 * @param hci_event HCI event associated with this event
 * @param cmd_status Command status associated with this event
 * @param reason_code Reason code associated with this event
 * @param event_value A status value related to this specific event
 */
void LogClassicPairingEvent(const RawAddress& address, uint16_t handle, uint32_t hci_cmd, uint16_t hci_event,
                            uint16_t cmd_status, uint16_t reason_code, int64_t event_value);

/**
 * Logs when certain Bluetooth SDP attributes are discovered
 *
 * @param address address of associated device
 * @param protocol_uuid 16 bit protocol UUID from Bluetooth Assigned Numbers
 * @param attribute_id 16 bit attribute ID from Bluetooth Assigned Numbers
 * @param attribute_size size of this attribute
 * @param attribute_value pointer to the attribute data, must be larger than
 *                        attribute_size
 */
void LogSdpAttribute(const RawAddress& address, uint16_t protocol_uuid,
                     uint16_t attribute_id, size_t attribute_size,
                     const char* attribute_value);

/**
 * Logs when there is a change in Bluetooth socket connection state
 *
 * @param address address of associated device, empty if this is a server port
 * @param port port of this socket connection
 * @param type type of socket
 * @param connection_state socket connection state
 * @param tx_bytes number of bytes transmitted
 * @param rx_bytes number of bytes received
 * @param server_port server port of this socket, if any. When both
 *        |server_port| and |port| fields are populated, |port| must be spawned
 *        by |server_port|
 * @param socket_role role of this socket, server or connection
 * @param uid socket owner's uid
 */
void LogSocketConnectionState(
    const RawAddress& address, int port, int type,
    android::bluetooth::SocketConnectionstateEnum connection_state,
    int64_t tx_bytes, int64_t rx_bytes, int uid, int server_port,
    android::bluetooth::SocketRoleEnum socket_role);

/**
 * Logs when a Bluetooth device's manufacturer information is learnt
 *
 * @param address address of associated device
 * @param source_type where is this device info obtained from
 * @param source_name name of the data source, internal or external
 * @param manufacturer name of the manufacturer of this device
 * @param model model of this device
 * @param hardware_version hardware version of this device
 * @param software_version software version of this device
 */
void LogManufacturerInfo(const RawAddress& address,
                         android::bluetooth::DeviceInfoSrcEnum source_type,
                         const std::string& source_name,
                         const std::string& manufacturer,
                         const std::string& model,
                         const std::string& hardware_version,
                         const std::string& software_version);
}  // namespace common

}  // namespace bluetooth
