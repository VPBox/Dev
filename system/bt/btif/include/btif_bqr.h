/*
 * Copyright 2019 The Android Open Source Project
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

#ifndef BTIF_BQR_H_
#define BTIF_BQR_H_

#include "btm_api_types.h"
#include "common/leaky_bonded_queue.h"

namespace bluetooth {
namespace bqr {

// Bluetooth Quality Report (BQR)
//
// It is a feature to start the mechanism in the Bluetooth controller to report
// Bluetooth Quality event to the host and there are four options could be
// enabled:
//   [Quality Monitoring Mode]
//     The controller shall periodically send Bluetooth Quality Report sub-event
//     to the host.
//
//   [Approaching LSTO]
//     Once no packets are received from the connected Bluetooth device for a
//     duration longer than the half of LSTO (Link Supervision TimeOut) value,
//     the controller shall report Approaching LSTO event to the host.
//
//   [A2DP Audio Choppy]
//     When the controller detects the factors which will cause audio choppy,
//     the controller shall report A2DP Audio Choppy event to the host.
//
//   [(e)SCO Voice Choppy]
//     When the controller detects the factors which will cause voice choppy,
//     the controller shall report (e)SCO Voice Choppy event to the host.

// Bit masks for the selected quality event reporting.
static constexpr uint32_t kQualityEventMaskAllOff = 0;
static constexpr uint32_t kQualityEventMaskMonitorMode = 0x00000001;
static constexpr uint32_t kQualityEventMaskApproachLsto = 0x00000002;
static constexpr uint32_t kQualityEventMaskA2dpAudioChoppy = 0x00000004;
static constexpr uint32_t kQualityEventMaskScoVoiceChoppy = 0x00000008;
static constexpr uint32_t kQualityEventMaskAll =
    kQualityEventMaskMonitorMode | kQualityEventMaskApproachLsto |
    kQualityEventMaskA2dpAudioChoppy | kQualityEventMaskScoVoiceChoppy;
// Define the minimum time interval (in ms) of quality event reporting for the
// selected quality event(s). Controller Firmware should not report the next
// event within the defined time interval.
static constexpr uint16_t kMinReportIntervalNoLimit = 0;
static constexpr uint16_t kMinReportIntervalMaxMs = 0xFFFF;
// Total length of all BQR parameters except Vendor Specific Parameters.
static constexpr uint8_t kBqrParamTotalLen = 48;
// Warning criteria of the RSSI value.
static constexpr int8_t kCriWarnRssi = -80;
// Warning criteria of the unused AFH channel count.
static constexpr uint8_t kCriWarnUnusedCh = 55;
// The queue size of recording the BQR events.
static constexpr uint8_t kBqrEventQueueSize = 25;
// The minimum size of the ROOT_INFLAMMATION event
// HCI_VENDOR_SPECIFIC_EVT(1) + BQR sub event(1) + BQR report ID(1) +
// error code(1) + vendor error code(1) = 5
static constexpr uint8_t kRootInflammationPacketMinSize = 5;

// The Property of BQR event mask configuration.
static constexpr const char* kpPropertyEventMask =
    "persist.bluetooth.bqr.event_mask";
// The Property of BQR minimum report interval configuration.
static constexpr const char* kpPropertyMinReportIntervalMs =
    "persist.bluetooth.bqr.min_interval_ms";

// Action definition
//
// Action to Add, Delete or Clear the reporting of quality event(s).
// Delete will clear specific quality event(s) reporting. Clear will clear all
// quality events reporting.
enum BqrReportAction : uint8_t {
  REPORT_ACTION_ADD = 0x00,
  REPORT_ACTION_DELETE = 0x01,
  REPORT_ACTION_CLEAR = 0x02
};

// Report ID definition
enum BqrQualityReportId : uint8_t {
  QUALITY_REPORT_ID_MONITOR_MODE = 0x01,
  QUALITY_REPORT_ID_APPROACH_LSTO = 0x02,
  QUALITY_REPORT_ID_A2DP_AUDIO_CHOPPY = 0x03,
  QUALITY_REPORT_ID_SCO_VOICE_CHOPPY = 0x04,
  QUALITY_REPORT_ID_ROOT_INFLAMMATION = 0x05
};

// Packet Type definition
enum BqrPacketType : uint8_t {
  PACKET_TYPE_ID = 0x01,
  PACKET_TYPE_NULL,
  PACKET_TYPE_POLL,
  PACKET_TYPE_FHS,
  PACKET_TYPE_HV1,
  PACKET_TYPE_HV2,
  PACKET_TYPE_HV3,
  PACKET_TYPE_DV,
  PACKET_TYPE_EV3,
  PACKET_TYPE_EV4,
  PACKET_TYPE_EV5,
  PACKET_TYPE_2EV3,
  PACKET_TYPE_2EV5,
  PACKET_TYPE_3EV3,
  PACKET_TYPE_3EV5,
  PACKET_TYPE_DM1,
  PACKET_TYPE_DH1,
  PACKET_TYPE_DM3,
  PACKET_TYPE_DH3,
  PACKET_TYPE_DM5,
  PACKET_TYPE_DH5,
  PACKET_TYPE_AUX1,
  PACKET_TYPE_2DH1,
  PACKET_TYPE_2DH3,
  PACKET_TYPE_2DH5,
  PACKET_TYPE_3DH1,
  PACKET_TYPE_3DH3,
  PACKET_TYPE_3DH5
};

// Configuration Parameters
typedef struct {
  BqrReportAction report_action;
  uint32_t quality_event_mask;
  uint16_t minimum_report_interval_ms;
} BqrConfiguration;

// BQR sub-event of Vendor Specific Event
class BqrVseSubEvt {
 public:
  // Parse the Bluetooth Quality Report VSE sub-event.
  //
  // @param length Total length of all parameters contained in the sub-event.
  // @param p_param_buf A pointer to the parameters contained in the sub-event.
  // @return false If the parameter total length is abnormal.
  //         true If all parameters are parsed successfully.
  bool ParseBqrEvt(uint8_t length, uint8_t* p_param_buf);

  // Get a string representation of the Bluetooth Quality event.
  //
  // @return a string representation of the Bluetooth Quality event.
  std::string ToString() const;

  friend std::ostream& operator<<(std::ostream& os, const BqrVseSubEvt& a) {
    return os << a.ToString();
  }

  virtual ~BqrVseSubEvt() = default;

  // Quality report ID.
  uint8_t quality_report_id_ = 0;
  // Packet type of the connection.
  uint8_t packet_types_ = 0;
  // Connection handle of the connection.
  uint16_t connection_handle_ = 0;
  // Performing Role for the connection.
  uint8_t connection_role_ = 0;
  // Current Transmit Power Level for the connection. This value is the same as
  // the controller's response to the HCI_Read_Transmit_Power_Level HCI command.
  uint8_t tx_power_level_ = 0;
  // Received Signal Strength Indication (RSSI) value for the connection. This
  // value is an absolute receiver signal strength value.
  int8_t rssi_ = 0;
  // Signal-to-Noise Ratio (SNR) value for the connection. It is the average
  // SNR of all the channels used by the link currently.
  uint8_t snr_ = 0;
  // Indicates the number of unused channels in AFH_channel_map.
  uint8_t unused_afh_channel_count_ = 0;
  // Indicates the number of the channels which are interfered and quality is
  // bad but are still selected for AFH.
  uint8_t afh_select_unideal_channel_count_ = 0;
  // Current Link Supervision Timeout Setting.
  // Unit: N * 0.3125 ms (1 Bluetooth Clock)
  uint16_t lsto_ = 0;
  // Piconet Clock for the specified Connection_Handle. This value is the same
  // as the controller's response to HCI_Read_Clock HCI command with the
  // parameter "Which_Clock" of 0x01 (Piconet Clock).
  // Unit: N * 0.3125 ms (1 Bluetooth Clock)
  uint32_t connection_piconet_clock_ = 0;
  // The count of retransmission.
  uint32_t retransmission_count_ = 0;
  // The count of no RX.
  uint32_t no_rx_count_ = 0;
  // The count of NAK (Negative Acknowledge).
  uint32_t nak_count_ = 0;
  // Timestamp of last TX ACK.
  // Unit: N * 0.3125 ms (1 Bluetooth Clock)
  uint32_t last_tx_ack_timestamp_ = 0;
  // The count of Flow-off (STOP).
  uint32_t flow_off_count_ = 0;
  // Timestamp of last Flow-on (GO).
  // Unit: N * 0.3125 ms (1 Bluetooth Clock)
  uint32_t last_flow_on_timestamp_ = 0;
  // Buffer overflow count (how many bytes of TX data are dropped) since the
  // last event.
  uint32_t buffer_overflow_bytes_ = 0;
  // Buffer underflow count (in byte).
  uint32_t buffer_underflow_bytes_ = 0;
  // Local wall clock timestamp of receiving BQR VSE sub-event
  std::tm tm_timestamp_ = {};
};

// Get a string representation of the Quality Report ID.
//
// @param quality_report_id The quality report ID to convert.
// @return a string representation of the Quality Report ID.
std::string QualityReportIdToString(uint8_t quality_report_id);

// Get a string representation of the Packet Type.
//
// @param packet_type The packet type to convert.
// @return a string representation of the Packet Type.
std::string PacketTypeToString(uint8_t packet_type);

// Enable/Disable Bluetooth Quality Report mechanism.
//
// Which Quality event will be enabled is according to the setting of the
// property "persist.bluetooth.bqr.event_mask".
// And the minimum time interval of quality event reporting depends on the
// setting of property "persist.bluetooth.bqr.min_interval_ms".
//
// @param is_enable True/False to enable/disable Bluetooth Quality Report
//   mechanism in the Bluetooth controller.
void EnableBtQualityReport(bool is_enable);

// Dump Bluetooth Quality Report information.
//
// @param fd The file descriptor to use for dumping information.
void DebugDump(int fd);

// Configure Bluetooth Quality Report setting to the Bluetooth controller.
//
// @param bqr_config The struct of configuration parameters.
void ConfigureBqr(const BqrConfiguration& bqr_config);

// Invoked on completion of Bluetooth Quality Report configuration. Then it will
// Register/Unregister for receiving VSE - Bluetooth Quality Report sub event.
//
// @param current_evt_mask Indicates current quality event bit mask setting in
//   the Bluetooth controller.
void ConfigureBqrCmpl(uint32_t current_evt_mask);

// Callback invoked on completion of vendor specific Bluetooth Quality Report
// command.
//
// @param p_vsc_cmpl_params A pointer to the parameters contained in the vendor
//   specific command complete event.
void BqrVscCompleteCallback(tBTM_VSC_CMPL* p_vsc_cmpl_params);

// Record a new incoming Bluetooth Quality Report in quality event queue.
//
// @param len Lengths of the quality report sent from the Bluetooth
//   controller.
// @param p_quality_report A pointer to the quality report which is sent from
//   the Bluetooth controller via Vendor Specific Event.
void AddBqrEventToQueue(uint8_t length, uint8_t* p_stream);

}  // namespace bqr
}  // namespace bluetooth

#endif  // BTIF_BQR_H_
