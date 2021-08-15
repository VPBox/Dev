/******************************************************************************
 *
 *  Copyright 2018 Google, Inc.
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

#include "metrics.h"

#include <base/logging.h>

#include "leaky_bonded_queue.h"

namespace bluetooth {

namespace common {

void A2dpSessionMetrics::Update(const A2dpSessionMetrics& metrics) {}

bool A2dpSessionMetrics::operator==(const A2dpSessionMetrics& rhs) const {
  LOG(FATAL) << "UNIMPLEMENTED";
  return 0;
}

struct BluetoothMetricsLogger::impl {
  impl(size_t max_bluetooth_session, size_t max_pair_event,
       size_t max_wake_event, size_t max_scan_event) {}
};

BluetoothMetricsLogger::BluetoothMetricsLogger()
    : pimpl_(new impl(kMaxNumBluetoothSession, kMaxNumPairEvent,
                      kMaxNumWakeEvent, kMaxNumScanEvent)) {}

void BluetoothMetricsLogger::LogPairEvent(uint32_t disconnect_reason,
                                          uint64_t timestamp_ms,
                                          uint32_t device_class,
                                          device_type_t device_type) {}

void BluetoothMetricsLogger::LogWakeEvent(wake_event_type_t type,
                                          const std::string& requestor,
                                          const std::string& name,
                                          uint64_t timestamp_ms) {}

void BluetoothMetricsLogger::LogScanEvent(bool start,
                                          const std::string& initator,
                                          scan_tech_t type, uint32_t results,
                                          uint64_t timestamp_ms) {}

void BluetoothMetricsLogger::LogBluetoothSessionStart(
    connection_tech_t connection_tech_type, uint64_t timestamp_ms) {}

void BluetoothMetricsLogger::LogBluetoothSessionEnd(
    disconnect_reason_t disconnect_reason, uint64_t timestamp_ms) {}

void BluetoothMetricsLogger::LogBluetoothSessionDeviceInfo(
    uint32_t device_class, device_type_t device_type) {}

void BluetoothMetricsLogger::LogA2dpSession(
    const A2dpSessionMetrics& a2dp_session_metrics) {}

void BluetoothMetricsLogger::LogHeadsetProfileRfcConnection(
    tBTA_SERVICE_ID service_id) {}

void BluetoothMetricsLogger::WriteString(std::string* serialized) {}

void BluetoothMetricsLogger::WriteBase64String(std::string* serialized) {}

void BluetoothMetricsLogger::WriteBase64(int fd) {}

void BluetoothMetricsLogger::CutoffSession() {}

void BluetoothMetricsLogger::Build() {}

void BluetoothMetricsLogger::ResetSession() {}

void BluetoothMetricsLogger::ResetLog() {}

void BluetoothMetricsLogger::Reset() {}

}  // namespace common

}  // namespace bluetooth
