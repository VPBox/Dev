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
#include <chrono>
#include <cstdint>
#include <string>
#include <thread>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <base/logging.h>
#include <include/hardware/bt_av.h>

#include "bluetooth/metrics/bluetooth.pb.h"
#include "common/metrics.h"
#include "common/time_util.h"

#define BTM_COD_MAJOR_AUDIO_TEST 0x04

namespace testing {

using bluetooth::common::A2dpSessionMetrics;
using bluetooth::common::BluetoothMetricsLogger;
using bluetooth::metrics::BluetoothMetricsProto::A2DPSession;
using bluetooth::metrics::BluetoothMetricsProto::A2dpSourceCodec;
using bluetooth::metrics::BluetoothMetricsProto::BluetoothLog;
using bluetooth::metrics::BluetoothMetricsProto::BluetoothSession;
using bluetooth::metrics::BluetoothMetricsProto::
    BluetoothSession_ConnectionTechnologyType;
using bluetooth::metrics::BluetoothMetricsProto::
    BluetoothSession_DisconnectReasonType;
using bluetooth::metrics::BluetoothMetricsProto::DeviceInfo;
using bluetooth::metrics::BluetoothMetricsProto::DeviceInfo_DeviceType;
using bluetooth::metrics::BluetoothMetricsProto::HeadsetProfileConnectionStats;
using bluetooth::metrics::BluetoothMetricsProto::HeadsetProfileType;
using bluetooth::metrics::BluetoothMetricsProto::PairEvent;
using bluetooth::metrics::BluetoothMetricsProto::RFCommSession;
using bluetooth::metrics::BluetoothMetricsProto::ScanEvent;
using bluetooth::metrics::BluetoothMetricsProto::ScanEvent_ScanEventType;
using bluetooth::metrics::BluetoothMetricsProto::ScanEvent_ScanTechnologyType;
using bluetooth::metrics::BluetoothMetricsProto::WakeEvent;
using bluetooth::metrics::BluetoothMetricsProto::WakeEvent_WakeEventType;

namespace {
const size_t kMaxEventGenerationLimit = 5000;
}

static void sleep_ms(int64_t t) {
  std::this_thread::sleep_for(std::chrono::milliseconds(t));
}

DeviceInfo* MakeDeviceInfo(int32_t device_class,
                           DeviceInfo_DeviceType device_type) {
  DeviceInfo* info = new DeviceInfo();
  info->set_device_class(device_class);
  info->set_device_type(device_type);
  return info;
}

PairEvent* MakePairEvent(int32_t disconnect_reason, int64_t timestamp_ms,
                         DeviceInfo* device_info) {
  PairEvent* event = new PairEvent();
  event->set_disconnect_reason(disconnect_reason);
  event->set_event_time_millis(timestamp_ms);
  if (device_info) event->set_allocated_device_paired_with(device_info);
  return event;
}

WakeEvent* MakeWakeEvent(WakeEvent_WakeEventType event_type,
                         const std::string& requestor, const std::string& name,
                         int64_t timestamp_ms) {
  WakeEvent* event = new WakeEvent();
  event->set_wake_event_type(event_type);
  event->set_requestor(requestor);
  event->set_name(name);
  event->set_event_time_millis(timestamp_ms);
  return event;
}

ScanEvent* MakeScanEvent(ScanEvent_ScanEventType event_type,
                         const std::string& initiator,
                         ScanEvent_ScanTechnologyType tech_type,
                         int32_t num_results, int64_t timestamp_ms) {
  ScanEvent* event = new ScanEvent();
  event->set_scan_event_type(event_type);
  event->set_initiator(initiator);
  event->set_scan_technology_type(tech_type);
  event->set_number_results(num_results);
  event->set_event_time_millis(timestamp_ms);
  return event;
}

A2DPSession* MakeA2DPSession(const A2dpSessionMetrics& metrics,
                             A2dpSourceCodec source_codec) {
  A2DPSession* session = new A2DPSession();
  session->set_media_timer_min_millis(metrics.media_timer_min_ms);
  session->set_media_timer_max_millis(metrics.media_timer_max_ms);
  session->set_media_timer_avg_millis(metrics.media_timer_avg_ms);
  session->set_buffer_overruns_max_count(metrics.buffer_overruns_max_count);
  session->set_buffer_overruns_total(metrics.buffer_overruns_total);
  session->set_buffer_underruns_average(metrics.buffer_underruns_average);
  session->set_buffer_underruns_count(metrics.buffer_underruns_count);
  session->set_audio_duration_millis(metrics.audio_duration_ms);
  session->set_source_codec(source_codec);
  session->set_is_a2dp_offload(metrics.is_a2dp_offload);
  return session;
}

BluetoothSession* MakeBluetoothSession(
    int64_t session_duration_sec,
    BluetoothSession_ConnectionTechnologyType conn_type,
    BluetoothSession_DisconnectReasonType disconnect_reason,
    DeviceInfo* device_info, RFCommSession* rfcomm_session,
    A2DPSession* a2dp_session) {
  BluetoothSession* session = new BluetoothSession();
  if (a2dp_session) session->set_allocated_a2dp_session(a2dp_session);
  if (rfcomm_session) session->set_allocated_rfcomm_session(rfcomm_session);
  if (device_info) session->set_allocated_device_connected_to(device_info);
  session->set_session_duration_sec(session_duration_sec);
  session->set_connection_technology_type(conn_type);
  session->set_disconnect_reason_type(disconnect_reason);
  return session;
}

BluetoothLog* MakeBluetoothLog(std::vector<BluetoothSession*> bt_sessions,
                               std::vector<PairEvent*> pair_events,
                               std::vector<WakeEvent*> wake_events,
                               std::vector<ScanEvent*> scan_events) {
  BluetoothLog* bt_log = new BluetoothLog();
  for (BluetoothSession* session : bt_sessions) {
    bt_log->mutable_session()->AddAllocated(session);
  }
  bt_sessions.clear();
  for (PairEvent* event : pair_events) {
    bt_log->mutable_pair_event()->AddAllocated(event);
  }
  pair_events.clear();
  for (WakeEvent* event : wake_events) {
    bt_log->mutable_wake_event()->AddAllocated(event);
  }
  wake_events.clear();
  for (ScanEvent* event : scan_events) {
    bt_log->mutable_scan_event()->AddAllocated(event);
  }
  scan_events.clear();
  return bt_log;
}

void GenerateWakeEvents(size_t start, size_t end,
                        std::vector<WakeEvent*>* wake_events) {
  for (size_t i = start; i < end; ++i) {
    wake_events->push_back(MakeWakeEvent(
        i % 2 == 0 ? WakeEvent_WakeEventType::WakeEvent_WakeEventType_ACQUIRED
                   : WakeEvent_WakeEventType::WakeEvent_WakeEventType_RELEASED,
        "TEST_REQ", "TEST_NAME", i));
  }
}

#define COMPARE_A2DP_METRICS(a, b)                                           \
  do {                                                                       \
    EXPECT_EQ((a).audio_duration_ms, (b).audio_duration_ms);                 \
    EXPECT_EQ((a).media_timer_min_ms, (b).media_timer_min_ms);               \
    EXPECT_EQ((a).media_timer_max_ms, (b).media_timer_max_ms);               \
    EXPECT_EQ((a).media_timer_avg_ms, (b).media_timer_avg_ms);               \
    EXPECT_EQ((a).total_scheduling_count, (b).total_scheduling_count);       \
    EXPECT_EQ((a).buffer_overruns_max_count, (b).buffer_overruns_max_count); \
    EXPECT_EQ((a).buffer_overruns_total, (b).buffer_overruns_total);         \
    EXPECT_THAT((a).buffer_underruns_average,                                \
                FloatNear((b).buffer_underruns_average, 0.01));              \
    (a).buffer_underruns_average = (b).buffer_underruns_average;             \
    EXPECT_EQ((a).buffer_underruns_count, (b).buffer_underruns_count);       \
    EXPECT_EQ((a).codec_index, (b).codec_index);                             \
    EXPECT_EQ((a).is_a2dp_offload, (b).is_a2dp_offload);                     \
  } while (0)

/*
 * metrics_sum = metrics1 + metrics2
 */
TEST(BluetoothA2DPSessionMetricsTest, TestUpdateNormal) {
  A2dpSessionMetrics metrics1;
  A2dpSessionMetrics metrics2;
  A2dpSessionMetrics metrics_sum;
  metrics1.audio_duration_ms = 10;
  metrics2.audio_duration_ms = 25;
  metrics_sum.audio_duration_ms = 35;
  metrics1.media_timer_min_ms = 10;
  metrics2.media_timer_min_ms = 25;
  metrics_sum.media_timer_min_ms = 10;
  metrics1.media_timer_max_ms = 100;
  metrics2.media_timer_max_ms = 200;
  metrics_sum.media_timer_max_ms = 200;
  metrics1.media_timer_avg_ms = 50;
  metrics1.total_scheduling_count = 50;
  metrics2.media_timer_avg_ms = 100;
  metrics2.total_scheduling_count = 50;
  metrics_sum.media_timer_avg_ms = 75;
  metrics_sum.total_scheduling_count = 100;
  metrics1.buffer_overruns_max_count = 70;
  metrics2.buffer_overruns_max_count = 80;
  metrics_sum.buffer_overruns_max_count = 80;
  metrics1.buffer_underruns_average = 80;
  metrics1.buffer_underruns_count = 1200;
  metrics2.buffer_underruns_average = 130;
  metrics2.buffer_underruns_count = 2400;
  metrics_sum.buffer_underruns_average = 113.33333333;
  metrics_sum.buffer_underruns_count = 3600;
  metrics1.codec_index = -1;
  metrics2.codec_index = BTAV_A2DP_CODEC_INDEX_SOURCE_AAC;
  metrics_sum.codec_index = BTAV_A2DP_CODEC_INDEX_SOURCE_AAC;
  metrics1.is_a2dp_offload = false;
  metrics2.is_a2dp_offload = true;
  metrics_sum.is_a2dp_offload = true;
  metrics1.Update(metrics2);
  COMPARE_A2DP_METRICS(metrics1, metrics_sum);
  EXPECT_TRUE(metrics1 == metrics_sum);
  EXPECT_EQ(metrics1, metrics_sum);
}

TEST(BluetoothA2DPSessionMetricsTest, TestUpdateNew) {
  A2dpSessionMetrics metrics1;
  A2dpSessionMetrics metrics2;
  A2dpSessionMetrics metrics_sum;
  metrics2.audio_duration_ms = 25;
  metrics_sum.audio_duration_ms = 25;
  metrics2.media_timer_min_ms = 25;
  metrics_sum.media_timer_min_ms = 25;
  metrics2.media_timer_max_ms = 200;
  metrics_sum.media_timer_max_ms = 200;
  metrics2.media_timer_avg_ms = 100;
  metrics2.total_scheduling_count = 50;
  metrics_sum.media_timer_avg_ms = 100;
  metrics_sum.total_scheduling_count = 50;
  metrics2.buffer_overruns_max_count = 80;
  metrics_sum.buffer_overruns_max_count = 80;
  metrics2.buffer_underruns_average = 130;
  metrics2.buffer_underruns_count = 2400;
  metrics_sum.buffer_underruns_average = 130;
  metrics_sum.buffer_underruns_count = 2400;
  metrics2.codec_index = BTAV_A2DP_CODEC_INDEX_SOURCE_APTX;
  metrics_sum.codec_index = BTAV_A2DP_CODEC_INDEX_SOURCE_APTX;
  metrics2.is_a2dp_offload = true;
  metrics_sum.is_a2dp_offload = true;
  metrics1.Update(metrics2);
  COMPARE_A2DP_METRICS(metrics1, metrics_sum);
  EXPECT_TRUE(metrics1 == metrics_sum);
  EXPECT_EQ(metrics1, metrics_sum);
}

TEST(BluetoothA2DPSessionMetricsTest, TestNullUpdate) {
  A2dpSessionMetrics metrics1;
  A2dpSessionMetrics metrics2;
  A2dpSessionMetrics metrics_sum;
  metrics2.audio_duration_ms = 25;
  metrics_sum.audio_duration_ms = 25;
  metrics2.media_timer_min_ms = 25;
  metrics_sum.media_timer_min_ms = 25;
  metrics2.media_timer_max_ms = 200;
  metrics_sum.media_timer_max_ms = 200;
  metrics2.media_timer_avg_ms = 100;
  metrics2.total_scheduling_count = 50;
  metrics_sum.media_timer_avg_ms = 100;
  metrics_sum.total_scheduling_count = 50;
  metrics2.buffer_overruns_max_count = 80;
  metrics_sum.buffer_overruns_max_count = 80;
  metrics2.buffer_underruns_average = 130;
  metrics2.buffer_underruns_count = 2400;
  metrics_sum.buffer_underruns_average = 130;
  metrics_sum.buffer_underruns_count = 2400;
  metrics2.codec_index = BTAV_A2DP_CODEC_INDEX_SOURCE_APTX_HD;
  metrics_sum.codec_index = BTAV_A2DP_CODEC_INDEX_SOURCE_APTX_HD;
  metrics2.is_a2dp_offload = true;
  metrics_sum.is_a2dp_offload = true;
  metrics2.Update(metrics1);
  COMPARE_A2DP_METRICS(metrics2, metrics_sum);
  EXPECT_TRUE(metrics2 == metrics_sum);
  EXPECT_EQ(metrics2, metrics_sum);
}

TEST(BluetoothA2DPSessionMetricsTest, TestPartialUpdate) {
  A2dpSessionMetrics metrics1;
  A2dpSessionMetrics metrics2;
  A2dpSessionMetrics metrics_sum;
  metrics1.audio_duration_ms = 10;
  metrics2.audio_duration_ms = 25;
  metrics_sum.audio_duration_ms = 35;
  metrics1.media_timer_min_ms = 10;
  metrics_sum.media_timer_min_ms = 10;
  metrics1.media_timer_max_ms = 100;
  metrics_sum.media_timer_max_ms = 100;
  metrics1.media_timer_avg_ms = 50;
  metrics1.total_scheduling_count = 50;
  metrics2.media_timer_avg_ms = 100;
  metrics_sum.media_timer_avg_ms = 50;
  metrics_sum.total_scheduling_count = 50;
  metrics1.buffer_overruns_max_count = 70;
  metrics_sum.buffer_overruns_max_count = 70;
  metrics1.buffer_underruns_average = 80;
  metrics1.buffer_underruns_count = 1200;
  metrics2.buffer_underruns_count = 2400;
  metrics_sum.buffer_underruns_average = 80;
  metrics_sum.buffer_underruns_count = 1200;
  metrics1.Update(metrics2);
  COMPARE_A2DP_METRICS(metrics1, metrics_sum);
  EXPECT_TRUE(metrics1 == metrics_sum);
  EXPECT_EQ(metrics1, metrics_sum);
}

class BluetoothMetricsLoggerTest : public Test {
 protected:
  // Use to hold test protos
  std::vector<PairEvent*> pair_events_;
  std::vector<WakeEvent*> wake_events_;
  std::vector<ScanEvent*> scan_events_;
  std::vector<BluetoothSession*> bt_sessions_;
  int64_t num_pair_event_ = 0;
  int64_t num_wake_event_ = 0;
  int64_t num_scan_event_ = 0;
  int64_t num_bt_session_ = 0;
  BluetoothLog* bt_log_;
  std::string bt_log_str_;
  std::string bt_log_ascii_str_;

  void UpdateLog() {
    for (BluetoothSession* session : bt_sessions_) {
      bt_log_->mutable_session()->AddAllocated(session);
    }
    if (num_bt_session_ > 0) {
      bt_log_->set_num_bluetooth_session(num_bt_session_);
    } else if (bt_sessions_.size() > 0) {
      bt_log_->set_num_bluetooth_session(bt_sessions_.size());
    }
    bt_sessions_.clear();
    for (PairEvent* event : pair_events_) {
      bt_log_->mutable_pair_event()->AddAllocated(event);
    }
    if (num_pair_event_ > 0) {
      bt_log_->set_num_pair_event(num_pair_event_);
    } else if (pair_events_.size() > 0) {
      bt_log_->set_num_pair_event(pair_events_.size());
    }
    pair_events_.clear();
    for (WakeEvent* event : wake_events_) {
      bt_log_->mutable_wake_event()->AddAllocated(event);
    }
    if (num_wake_event_ > 0) {
      bt_log_->set_num_wake_event(num_wake_event_);
    } else if (wake_events_.size() > 0) {
      bt_log_->set_num_wake_event(wake_events_.size());
    }
    wake_events_.clear();
    for (ScanEvent* event : scan_events_) {
      bt_log_->mutable_scan_event()->AddAllocated(event);
    }
    if (num_scan_event_ > 0) {
      bt_log_->set_num_scan_event(num_scan_event_);
    } else if (scan_events_.size() > 0) {
      bt_log_->set_num_scan_event(scan_events_.size());
    }
    scan_events_.clear();
    bt_log_->SerializeToString(&bt_log_str_);
  }

  void ClearLog() {
    for (BluetoothSession* session : bt_sessions_) {
      session->Clear();
      delete session;
    }
    bt_sessions_.clear();
    for (PairEvent* event : pair_events_) {
      event->Clear();
      delete event;
    }
    pair_events_.clear();
    for (WakeEvent* event : wake_events_) {
      event->Clear();
      delete event;
    }
    wake_events_.clear();
    for (ScanEvent* event : scan_events_) {
      event->Clear();
      delete event;
    }
    scan_events_.clear();
    bt_log_->Clear();
  }

  void SetUp() {
    bt_log_ = new BluetoothLog();
    // Clear existing metrics entries, if any
    BluetoothMetricsLogger::GetInstance()->Reset();
  }
  void TearDown() {
    // Clear remaining metrics entries, if any
    BluetoothMetricsLogger::GetInstance()->Reset();
    ClearLog();
    delete bt_log_;
  }

 public:
};

TEST_F(BluetoothMetricsLoggerTest, PairEventTest) {
  pair_events_.push_back(MakePairEvent(
      35, 12345,
      MakeDeviceInfo(
          42, DeviceInfo_DeviceType::DeviceInfo_DeviceType_DEVICE_TYPE_BREDR)));
  UpdateLog();
  BluetoothMetricsLogger::GetInstance()->LogPairEvent(
      35, 12345, 42, bluetooth::common::DEVICE_TYPE_BREDR);
  std::string msg_str;
  BluetoothMetricsLogger::GetInstance()->WriteString(&msg_str);
  EXPECT_THAT(msg_str, StrEq(bt_log_str_));
}

TEST_F(BluetoothMetricsLoggerTest, WakeEventTest) {
  wake_events_.push_back(
      MakeWakeEvent(WakeEvent_WakeEventType::WakeEvent_WakeEventType_ACQUIRED,
                    "TEST_REQ", "TEST_NAME", 12345));
  UpdateLog();
  BluetoothMetricsLogger::GetInstance()->LogWakeEvent(
      bluetooth::common::WAKE_EVENT_ACQUIRED, "TEST_REQ", "TEST_NAME", 12345);
  std::string msg_str;
  BluetoothMetricsLogger::GetInstance()->WriteString(&msg_str);
  EXPECT_THAT(msg_str, StrEq(bt_log_str_));
}

TEST_F(BluetoothMetricsLoggerTest, WakeEventOverrunTest) {
  GenerateWakeEvents(
      kMaxEventGenerationLimit - BluetoothMetricsLogger::kMaxNumWakeEvent,
      kMaxEventGenerationLimit, &wake_events_);
  num_wake_event_ = kMaxEventGenerationLimit;
  UpdateLog();
  for (size_t i = 0; i < kMaxEventGenerationLimit; ++i) {
    BluetoothMetricsLogger::GetInstance()->LogWakeEvent(
        i % 2 == 0 ? bluetooth::common::WAKE_EVENT_ACQUIRED
                   : bluetooth::common::WAKE_EVENT_RELEASED,
        "TEST_REQ", "TEST_NAME", i);
  }
  std::string msg_str;
  BluetoothMetricsLogger::GetInstance()->WriteString(&msg_str);
  EXPECT_THAT(msg_str, StrEq(bt_log_str_));
}

TEST_F(BluetoothMetricsLoggerTest, ScanEventTest) {
  scan_events_.push_back(MakeScanEvent(
      ScanEvent_ScanEventType::ScanEvent_ScanEventType_SCAN_EVENT_STOP,
      "TEST_INITIATOR",
      ScanEvent_ScanTechnologyType::
          ScanEvent_ScanTechnologyType_SCAN_TECH_TYPE_BREDR,
      42, 123456));
  UpdateLog();
  BluetoothMetricsLogger::GetInstance()->LogScanEvent(
      false, "TEST_INITIATOR", bluetooth::common::SCAN_TECH_TYPE_BREDR, 42,
      123456);
  std::string msg_str;
  BluetoothMetricsLogger::GetInstance()->WriteString(&msg_str);
  EXPECT_THAT(msg_str, StrEq(bt_log_str_));
}

TEST_F(BluetoothMetricsLoggerTest, BluetoothSessionTest) {
  bt_sessions_.push_back(MakeBluetoothSession(
      10,
      BluetoothSession_ConnectionTechnologyType::
          BluetoothSession_ConnectionTechnologyType_CONNECTION_TECHNOLOGY_TYPE_LE,
      BluetoothSession_DisconnectReasonType::
          BluetoothSession_DisconnectReasonType_UNKNOWN,
      nullptr, nullptr, nullptr));
  UpdateLog();
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionStart(
      bluetooth::common::CONNECTION_TECHNOLOGY_TYPE_LE, 123456);
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionEnd(
      bluetooth::common::DISCONNECT_REASON_UNKNOWN, 133456);
  std::string msg_str;
  BluetoothMetricsLogger::GetInstance()->WriteString(&msg_str);
  EXPECT_THAT(msg_str, StrEq(bt_log_str_));
}

TEST_F(BluetoothMetricsLoggerTest, BluetoothSessionDumpBeforeEndTest) {
  bt_sessions_.push_back(MakeBluetoothSession(
      1,
      BluetoothSession_ConnectionTechnologyType::
          BluetoothSession_ConnectionTechnologyType_CONNECTION_TECHNOLOGY_TYPE_LE,
      BluetoothSession_DisconnectReasonType::
          BluetoothSession_DisconnectReasonType_METRICS_DUMP,
      nullptr, nullptr, nullptr));
  UpdateLog();
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionStart(
      bluetooth::common::CONNECTION_TECHNOLOGY_TYPE_LE,
      bluetooth::common::time_get_os_boottime_ms());
  sleep_ms(1000);
  std::string msg_str;
  BluetoothMetricsLogger::GetInstance()->WriteString(&msg_str);
  EXPECT_THAT(msg_str, StrEq(bt_log_str_));
}

TEST_F(BluetoothMetricsLoggerTest, BluetoothSessionStartBeforeEndTest) {
  bt_sessions_.push_back(MakeBluetoothSession(
      1,
      BluetoothSession_ConnectionTechnologyType::
          BluetoothSession_ConnectionTechnologyType_CONNECTION_TECHNOLOGY_TYPE_UNKNOWN,
      BluetoothSession_DisconnectReasonType::
          BluetoothSession_DisconnectReasonType_NEXT_START_WITHOUT_END_PREVIOUS,
      nullptr, nullptr, nullptr));
  bt_sessions_.push_back(MakeBluetoothSession(
      2,
      BluetoothSession_ConnectionTechnologyType::
          BluetoothSession_ConnectionTechnologyType_CONNECTION_TECHNOLOGY_TYPE_LE,
      BluetoothSession_DisconnectReasonType::
          BluetoothSession_DisconnectReasonType_METRICS_DUMP,
      nullptr, nullptr, nullptr));
  UpdateLog();
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionStart(
      bluetooth::common::CONNECTION_TECHNOLOGY_TYPE_UNKNOWN, 0);
  sleep_ms(1000);
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionStart(
      bluetooth::common::CONNECTION_TECHNOLOGY_TYPE_LE, 0);
  sleep_ms(2000);
  std::string msg_str;
  BluetoothMetricsLogger::GetInstance()->WriteString(&msg_str);
  EXPECT_THAT(msg_str, StrEq(bt_log_str_));
}

/*
 * Test Case: A2DPSessionTwoUpdatesTest
 *
 * 1. Create Instance
 * 2. LogBluetoothSessionStart
 * 3. LogBluetoothSessionDeviceInfo
 * 4. LogA2dpSession
 * 5. LogA2dpSession
 * 6. LogBluetoothSessionEnd
 * 7. WriteString
 *
 */
TEST_F(BluetoothMetricsLoggerTest, A2DPSessionTwoUpdatesTest) {
  /* Same metrics from BluetoothA2DPSessionMetricsTest.TestUpdateNormal */
  A2dpSessionMetrics metrics1;
  A2dpSessionMetrics metrics2;
  A2dpSessionMetrics metrics_sum;
  metrics1.audio_duration_ms = 10;
  metrics2.audio_duration_ms = 25;
  metrics_sum.audio_duration_ms = 35;
  metrics1.media_timer_min_ms = 10;
  metrics2.media_timer_min_ms = 25;
  metrics_sum.media_timer_min_ms = 10;
  metrics1.media_timer_max_ms = 100;
  metrics2.media_timer_max_ms = 200;
  metrics_sum.media_timer_max_ms = 200;
  metrics1.media_timer_avg_ms = 50;
  metrics1.total_scheduling_count = 50;
  metrics2.media_timer_avg_ms = 100;
  metrics2.total_scheduling_count = 50;
  metrics_sum.media_timer_avg_ms = 75;
  metrics_sum.total_scheduling_count = 100;
  metrics1.buffer_overruns_max_count = 70;
  metrics2.buffer_overruns_max_count = 80;
  metrics_sum.buffer_overruns_max_count = 80;
  metrics1.buffer_underruns_average = 80;
  metrics1.buffer_underruns_count = 1200;
  metrics2.buffer_underruns_average = 130;
  metrics2.buffer_underruns_count = 2400;
  metrics_sum.buffer_underruns_average = 113.33333333;
  metrics_sum.buffer_underruns_count = 3600;
  metrics1.codec_index = -1;
  metrics2.codec_index = BTAV_A2DP_CODEC_INDEX_SOURCE_AAC;
  metrics_sum.codec_index = BTAV_A2DP_CODEC_INDEX_SOURCE_AAC;
  metrics1.is_a2dp_offload = false;
  metrics2.is_a2dp_offload = true;
  metrics_sum.is_a2dp_offload = true;
  DeviceInfo* info = MakeDeviceInfo(
      BTM_COD_MAJOR_AUDIO_TEST,
      DeviceInfo_DeviceType::DeviceInfo_DeviceType_DEVICE_TYPE_BREDR);
  A2DPSession* session =
      MakeA2DPSession(metrics_sum, A2dpSourceCodec::A2DP_SOURCE_CODEC_AAC);
  bt_sessions_.push_back(MakeBluetoothSession(
      10,
      BluetoothSession_ConnectionTechnologyType::
          BluetoothSession_ConnectionTechnologyType_CONNECTION_TECHNOLOGY_TYPE_BREDR,
      BluetoothSession_DisconnectReasonType::
          BluetoothSession_DisconnectReasonType_UNKNOWN,
      info, nullptr, session));
  UpdateLog();
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionStart(
      bluetooth::common::CONNECTION_TECHNOLOGY_TYPE_BREDR, 123456);
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionDeviceInfo(
      BTM_COD_MAJOR_AUDIO_TEST, bluetooth::common::DEVICE_TYPE_BREDR);
  BluetoothMetricsLogger::GetInstance()->LogA2dpSession(metrics1);
  BluetoothMetricsLogger::GetInstance()->LogA2dpSession(metrics2);
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionEnd(
      bluetooth::common::DISCONNECT_REASON_UNKNOWN, 133456);
  std::string msg_str;
  BluetoothMetricsLogger::GetInstance()->WriteString(&msg_str);
  EXPECT_THAT(msg_str, StrEq(bt_log_str_));
}

/*
 * Test Case: A2DPSessionTwoUpdatesSeparatedbyDumpTest
 *
 * 1. Create Instance
 * 2. LogBluetoothSessionStart
 * 3. LogBluetoothSessionDeviceInfo
 * 4. LogA2dpSession
 * 5. WriteString
 * 6. LogA2dpSession
 * 7. LogBluetoothSessionEnd
 * 8. WriteString
 *
 */
TEST_F(BluetoothMetricsLoggerTest, A2DPSessionTwoUpdatesSeparatedbyDumpTest) {
  /* Same metrics from BluetoothA2DPSessionMetricsTest.TestUpdateNormal */
  A2dpSessionMetrics metrics1;
  A2dpSessionMetrics metrics2;
  metrics1.audio_duration_ms = 10;
  metrics2.audio_duration_ms = 25;
  metrics1.media_timer_min_ms = 10;
  metrics2.media_timer_min_ms = 25;
  metrics1.media_timer_max_ms = 100;
  metrics2.media_timer_max_ms = 200;
  metrics1.media_timer_avg_ms = 50;
  metrics1.total_scheduling_count = 50;
  metrics2.media_timer_avg_ms = 100;
  metrics2.total_scheduling_count = 50;
  metrics1.buffer_overruns_max_count = 70;
  metrics2.buffer_overruns_max_count = 80;
  metrics1.buffer_underruns_average = 80;
  metrics1.buffer_underruns_count = 1200;
  metrics2.buffer_underruns_average = 130;
  metrics2.buffer_underruns_count = 2400;
  metrics1.codec_index = BTAV_A2DP_CODEC_INDEX_SOURCE_SBC;
  metrics2.codec_index = BTAV_A2DP_CODEC_INDEX_SOURCE_AAC;
  DeviceInfo* info = MakeDeviceInfo(
      BTM_COD_MAJOR_AUDIO_TEST,
      DeviceInfo_DeviceType::DeviceInfo_DeviceType_DEVICE_TYPE_BREDR);
  A2DPSession* session =
      MakeA2DPSession(metrics1, A2dpSourceCodec::A2DP_SOURCE_CODEC_SBC);
  bt_sessions_.push_back(MakeBluetoothSession(
      1,
      BluetoothSession_ConnectionTechnologyType::
          BluetoothSession_ConnectionTechnologyType_CONNECTION_TECHNOLOGY_TYPE_BREDR,
      BluetoothSession_DisconnectReasonType::
          BluetoothSession_DisconnectReasonType_METRICS_DUMP,
      info, nullptr, session));
  UpdateLog();
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionStart(
      bluetooth::common::CONNECTION_TECHNOLOGY_TYPE_BREDR, 0);
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionDeviceInfo(
      BTM_COD_MAJOR_AUDIO_TEST, bluetooth::common::DEVICE_TYPE_BREDR);
  BluetoothMetricsLogger::GetInstance()->LogA2dpSession(metrics1);
  sleep_ms(1000);
  std::string msg_str;
  BluetoothMetricsLogger::GetInstance()->WriteString(&msg_str);
  EXPECT_THAT(msg_str, StrEq(bt_log_str_));
  ClearLog();
  info = MakeDeviceInfo(
      BTM_COD_MAJOR_AUDIO_TEST,
      DeviceInfo_DeviceType::DeviceInfo_DeviceType_DEVICE_TYPE_BREDR);
  session = MakeA2DPSession(metrics2, A2dpSourceCodec::A2DP_SOURCE_CODEC_AAC);
  bt_sessions_.push_back(MakeBluetoothSession(
      1,
      BluetoothSession_ConnectionTechnologyType::
          BluetoothSession_ConnectionTechnologyType_CONNECTION_TECHNOLOGY_TYPE_BREDR,
      BluetoothSession_DisconnectReasonType::
          BluetoothSession_DisconnectReasonType_UNKNOWN,
      info, nullptr, session));
  UpdateLog();
  sleep_ms(1000);
  BluetoothMetricsLogger::GetInstance()->LogA2dpSession(metrics2);
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionEnd(
      bluetooth::common::DISCONNECT_REASON_UNKNOWN, 0);
  msg_str.clear();
  BluetoothMetricsLogger::GetInstance()->WriteString(&msg_str);
  EXPECT_THAT(msg_str, StrEq(bt_log_str_));
}

/*
 * Test Case: A2DPSessionTwoUpdatesSeparatedbyEndTest
 *
 * 1. Create Instance
 * 2. LogBluetoothSessionStart
 * 3. LogA2dpSession
 * 4. LogBluetoothSessionEnd
 * 5. LogBluetoothSessionStart
 * 6. LogA2dpSession
 * 7. LogBluetoothSessionEnd
 * 8. WriteString
 *
 */
TEST_F(BluetoothMetricsLoggerTest, A2DPSessionTwoUpdatesSeparatedbyEndTest) {
  /* Same metrics from BluetoothA2DPSessionMetricsTest.TestUpdateNormal */
  A2dpSessionMetrics metrics1;
  metrics1.audio_duration_ms = 10;
  metrics1.media_timer_min_ms = 10;
  metrics1.media_timer_max_ms = 100;
  metrics1.media_timer_avg_ms = 50;
  metrics1.total_scheduling_count = 50;
  metrics1.buffer_overruns_max_count = 70;
  metrics1.buffer_underruns_average = 80;
  metrics1.buffer_underruns_count = 1200;
  metrics1.codec_index = BTAV_A2DP_CODEC_INDEX_SOURCE_SBC;
  DeviceInfo* info = MakeDeviceInfo(
      BTM_COD_MAJOR_AUDIO_TEST,
      DeviceInfo_DeviceType::DeviceInfo_DeviceType_DEVICE_TYPE_BREDR);
  A2DPSession* session =
      MakeA2DPSession(metrics1, A2dpSourceCodec::A2DP_SOURCE_CODEC_SBC);
  bt_sessions_.push_back(MakeBluetoothSession(
      1,
      BluetoothSession_ConnectionTechnologyType::
          BluetoothSession_ConnectionTechnologyType_CONNECTION_TECHNOLOGY_TYPE_BREDR,
      BluetoothSession_DisconnectReasonType::
          BluetoothSession_DisconnectReasonType_UNKNOWN,
      info, nullptr, session));
  UpdateLog();
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionStart(
      bluetooth::common::CONNECTION_TECHNOLOGY_TYPE_BREDR, 0);
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionDeviceInfo(
      BTM_COD_MAJOR_AUDIO_TEST, bluetooth::common::DEVICE_TYPE_BREDR);
  BluetoothMetricsLogger::GetInstance()->LogA2dpSession(metrics1);
  sleep_ms(1000);
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionEnd(
      bluetooth::common::DISCONNECT_REASON_UNKNOWN, 0);
  std::string msg_str;
  BluetoothMetricsLogger::GetInstance()->WriteString(&msg_str);
  EXPECT_THAT(msg_str, StrEq(bt_log_str_));
  ClearLog();
  A2dpSessionMetrics metrics2;
  metrics2.audio_duration_ms = 25;
  metrics2.media_timer_min_ms = 25;
  metrics2.media_timer_max_ms = 200;
  metrics2.media_timer_avg_ms = 100;
  metrics2.total_scheduling_count = 50;
  metrics2.buffer_overruns_max_count = 80;
  metrics2.buffer_underruns_average = 130;
  metrics2.buffer_underruns_count = 2400;
  metrics2.codec_index = BTAV_A2DP_CODEC_INDEX_SOURCE_AAC;
  session = MakeA2DPSession(metrics2, A2dpSourceCodec::A2DP_SOURCE_CODEC_AAC);
  bt_sessions_.push_back(MakeBluetoothSession(
      1,
      BluetoothSession_ConnectionTechnologyType::
          BluetoothSession_ConnectionTechnologyType_CONNECTION_TECHNOLOGY_TYPE_BREDR,
      BluetoothSession_DisconnectReasonType::
          BluetoothSession_DisconnectReasonType_UNKNOWN,
      nullptr, nullptr, session));
  UpdateLog();
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionStart(
      bluetooth::common::CONNECTION_TECHNOLOGY_TYPE_BREDR, 0);
  sleep_ms(1000);
  BluetoothMetricsLogger::GetInstance()->LogA2dpSession(metrics2);
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionEnd(
      bluetooth::common::DISCONNECT_REASON_UNKNOWN, 0);
  msg_str.clear();
  BluetoothMetricsLogger::GetInstance()->WriteString(&msg_str);
  EXPECT_THAT(msg_str, StrEq(bt_log_str_));
}

/*
 * Test Case 1: A2DPSessionOnlyTest
 *
 * 1. Create Instance
 * 4. LogA2dpSession
 * 5. WriteString
 * 6. LogA2dpSession
 * 8. WriteString
 *
 */
TEST_F(BluetoothMetricsLoggerTest, A2DPSessionOnlyTest) {
  /* Same metrics from BluetoothA2DPSessionMetricsTest.TestUpdateNormal */
  A2dpSessionMetrics metrics1;
  A2dpSessionMetrics metrics2;
  A2dpSessionMetrics metrics_sum;
  metrics1.audio_duration_ms = 10;
  metrics2.audio_duration_ms = 25;
  metrics_sum.audio_duration_ms = 35;
  metrics1.media_timer_min_ms = 10;
  metrics2.media_timer_min_ms = 25;
  metrics_sum.media_timer_min_ms = 10;
  metrics1.media_timer_max_ms = 100;
  metrics2.media_timer_max_ms = 200;
  metrics_sum.media_timer_max_ms = 200;
  metrics1.media_timer_avg_ms = 50;
  metrics1.total_scheduling_count = 50;
  metrics2.media_timer_avg_ms = 100;
  metrics2.total_scheduling_count = 50;
  metrics_sum.media_timer_avg_ms = 75;
  metrics_sum.total_scheduling_count = 100;
  metrics1.buffer_overruns_max_count = 70;
  metrics2.buffer_overruns_max_count = 80;
  metrics_sum.buffer_overruns_max_count = 80;
  metrics1.buffer_underruns_average = 80;
  metrics1.buffer_underruns_count = 1200;
  metrics2.buffer_underruns_average = 130;
  metrics2.buffer_underruns_count = 2400;
  metrics_sum.buffer_underruns_average = 113.33333333;
  metrics_sum.buffer_underruns_count = 3600;
  metrics1.codec_index = BTAV_A2DP_CODEC_INDEX_SOURCE_SBC;
  metrics2.codec_index = BTAV_A2DP_CODEC_INDEX_SOURCE_AAC;
  metrics_sum.codec_index = BTAV_A2DP_CODEC_INDEX_SOURCE_SBC;
  DeviceInfo* info = MakeDeviceInfo(
      BTM_COD_MAJOR_AUDIO_TEST,
      DeviceInfo_DeviceType::DeviceInfo_DeviceType_DEVICE_TYPE_BREDR);
  A2DPSession* session =
      MakeA2DPSession(metrics_sum, A2dpSourceCodec::A2DP_SOURCE_CODEC_SBC);
  bt_sessions_.push_back(MakeBluetoothSession(
      1,
      BluetoothSession_ConnectionTechnologyType::
          BluetoothSession_ConnectionTechnologyType_CONNECTION_TECHNOLOGY_TYPE_BREDR,
      BluetoothSession_DisconnectReasonType::
          BluetoothSession_DisconnectReasonType_METRICS_DUMP,
      info, nullptr, session));
  UpdateLog();
  BluetoothMetricsLogger::GetInstance()->LogA2dpSession(metrics1);
  BluetoothMetricsLogger::GetInstance()->LogA2dpSession(metrics2);
  sleep_ms(1000);
  std::string msg_str;
  BluetoothMetricsLogger::GetInstance()->WriteString(&msg_str);
  EXPECT_THAT(msg_str, StrEq(bt_log_str_));
}

/*
 * Test Case: A2DPSessionDumpBeforeTwoUpdatesTest
 *
 * 1. Create Instance
 * 2. LogBluetoothSessionStart
 * 3. LogBluetoothSessionDeviceInfo
 * 5. WriteString
 * 6. LogA2dpSession
 * 7. LogA2dpSession
 * 8. LogBluetoothSessionEnd
 * 9. WriteString
 *
 */
TEST_F(BluetoothMetricsLoggerTest, A2DPSessionDumpBeforeTwoUpdatesTest) {
  /* Same metrics from BluetoothA2DPSessionMetricsTest.TestUpdateNormal */
  A2dpSessionMetrics metrics1;
  A2dpSessionMetrics metrics2;
  A2dpSessionMetrics metrics_sum;
  metrics1.audio_duration_ms = 10;
  metrics2.audio_duration_ms = 25;
  metrics_sum.audio_duration_ms = 35;
  metrics1.media_timer_min_ms = 10;
  metrics2.media_timer_min_ms = 25;
  metrics_sum.media_timer_min_ms = 10;
  metrics1.media_timer_max_ms = 100;
  metrics2.media_timer_max_ms = 200;
  metrics_sum.media_timer_max_ms = 200;
  metrics1.media_timer_avg_ms = 50;
  metrics1.total_scheduling_count = 50;
  metrics2.media_timer_avg_ms = 100;
  metrics2.total_scheduling_count = 50;
  metrics_sum.media_timer_avg_ms = 75;
  metrics_sum.total_scheduling_count = 100;
  metrics1.buffer_overruns_max_count = 70;
  metrics2.buffer_overruns_max_count = 80;
  metrics_sum.buffer_overruns_max_count = 80;
  metrics1.buffer_underruns_average = 80;
  metrics1.buffer_underruns_count = 1200;
  metrics2.buffer_underruns_average = 130;
  metrics2.buffer_underruns_count = 2400;
  metrics_sum.buffer_underruns_average = 113.33333333;
  metrics_sum.buffer_underruns_count = 3600;
  metrics1.codec_index = BTAV_A2DP_CODEC_INDEX_SOURCE_SBC;
  metrics2.codec_index = BTAV_A2DP_CODEC_INDEX_SOURCE_AAC;
  metrics_sum.codec_index = BTAV_A2DP_CODEC_INDEX_SOURCE_SBC;
  DeviceInfo* info = MakeDeviceInfo(
      BTM_COD_MAJOR_AUDIO_TEST,
      DeviceInfo_DeviceType::DeviceInfo_DeviceType_DEVICE_TYPE_BREDR);
  bt_sessions_.push_back(MakeBluetoothSession(
      1,
      BluetoothSession_ConnectionTechnologyType::
          BluetoothSession_ConnectionTechnologyType_CONNECTION_TECHNOLOGY_TYPE_BREDR,
      BluetoothSession_DisconnectReasonType::
          BluetoothSession_DisconnectReasonType_METRICS_DUMP,
      info, nullptr, nullptr));
  UpdateLog();
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionStart(
      bluetooth::common::CONNECTION_TECHNOLOGY_TYPE_BREDR, 0);
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionDeviceInfo(
      BTM_COD_MAJOR_AUDIO_TEST, bluetooth::common::DEVICE_TYPE_BREDR);
  sleep_ms(1000);
  std::string msg_str;
  BluetoothMetricsLogger::GetInstance()->WriteString(&msg_str);
  EXPECT_THAT(msg_str, StrEq(bt_log_str_));
  ClearLog();
  info = MakeDeviceInfo(
      BTM_COD_MAJOR_AUDIO_TEST,
      DeviceInfo_DeviceType::DeviceInfo_DeviceType_DEVICE_TYPE_BREDR);
  A2DPSession* session =
      MakeA2DPSession(metrics_sum, A2dpSourceCodec::A2DP_SOURCE_CODEC_SBC);
  bt_sessions_.push_back(MakeBluetoothSession(
      1,
      BluetoothSession_ConnectionTechnologyType::
          BluetoothSession_ConnectionTechnologyType_CONNECTION_TECHNOLOGY_TYPE_BREDR,
      BluetoothSession_DisconnectReasonType::
          BluetoothSession_DisconnectReasonType_UNKNOWN,
      info, nullptr, session));
  UpdateLog();
  BluetoothMetricsLogger::GetInstance()->LogA2dpSession(metrics1);
  BluetoothMetricsLogger::GetInstance()->LogA2dpSession(metrics2);
  sleep_ms(1000);
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionEnd(
      bluetooth::common::DISCONNECT_REASON_UNKNOWN, 0);
  msg_str.clear();
  BluetoothMetricsLogger::GetInstance()->WriteString(&msg_str);
  EXPECT_THAT(msg_str, StrEq(bt_log_str_));
}

TEST_F(BluetoothMetricsLoggerTest, LogHeadsetProfileRfcConnectionTest) {
  BluetoothMetricsLogger::GetInstance()->LogHeadsetProfileRfcConnection(
      BTA_HSP_SERVICE_ID);
  BluetoothMetricsLogger::GetInstance()->LogHeadsetProfileRfcConnection(
      BTA_HFP_SERVICE_ID);
  BluetoothMetricsLogger::GetInstance()->LogHeadsetProfileRfcConnection(
      BTA_HFP_SERVICE_ID);
  std::string msg_str;
  BluetoothMetricsLogger::GetInstance()->WriteString(&msg_str);
  BluetoothLog* metrics = BluetoothLog::default_instance().New();
  metrics->ParseFromString(msg_str);
  EXPECT_EQ(metrics->headset_profile_connection_stats_size(), 2);
  bool hfp_correct = false;
  bool hsp_correct = false;
  for (const HeadsetProfileConnectionStats& headset_profile_connection_stats :
       metrics->headset_profile_connection_stats()) {
    switch (headset_profile_connection_stats.headset_profile_type()) {
      case HeadsetProfileType::HFP:
        EXPECT_EQ(headset_profile_connection_stats.num_times_connected(), 2);
        hfp_correct = true;
        break;
      case HeadsetProfileType::HSP:
        EXPECT_EQ(headset_profile_connection_stats.num_times_connected(), 1);
        hsp_correct = true;
        break;
      default:
        FAIL();
    }
  }
  EXPECT_TRUE(hfp_correct);
  EXPECT_TRUE(hsp_correct);
  metrics->clear_headset_profile_connection_stats();
  EXPECT_EQ(metrics->headset_profile_connection_stats_size(), 0);
  msg_str.clear();
  // Verify that dump after clean up result in an empty list
  BluetoothMetricsLogger::GetInstance()->WriteString(&msg_str);
  metrics->ParseFromString(msg_str);
  EXPECT_EQ(metrics->headset_profile_connection_stats_size(), 0);
  delete metrics;
}

TEST_F(BluetoothMetricsLoggerTest, LogHeadsetProfileRfcConnectionErrorTest) {
  BluetoothMetricsLogger::GetInstance()->LogHeadsetProfileRfcConnection(
      BTA_HSP_SERVICE_ID);
  BluetoothMetricsLogger::GetInstance()->LogHeadsetProfileRfcConnection(
      BTA_HFP_SERVICE_ID);
  BluetoothMetricsLogger::GetInstance()->LogHeadsetProfileRfcConnection(
      BTA_BIP_SERVICE_ID);
  BluetoothMetricsLogger::GetInstance()->LogHeadsetProfileRfcConnection(
      BTA_HSP_SERVICE_ID);
  std::string msg_str;
  BluetoothMetricsLogger::GetInstance()->WriteString(&msg_str);
  BluetoothLog* metrics = BluetoothLog::default_instance().New();
  metrics->ParseFromString(msg_str);
  EXPECT_EQ(metrics->headset_profile_connection_stats_size(), 3);
  bool hfp_correct = false;
  bool hsp_correct = false;
  bool unknown_correct = false;
  for (const HeadsetProfileConnectionStats& headset_profile_connection_stats :
       metrics->headset_profile_connection_stats()) {
    switch (headset_profile_connection_stats.headset_profile_type()) {
      case HeadsetProfileType::HFP:
        EXPECT_EQ(headset_profile_connection_stats.num_times_connected(), 1);
        hfp_correct = true;
        break;
      case HeadsetProfileType::HSP:
        EXPECT_EQ(headset_profile_connection_stats.num_times_connected(), 2);
        hsp_correct = true;
        break;
      default:
        EXPECT_EQ(headset_profile_connection_stats.num_times_connected(), 1);
        unknown_correct = true;
        break;
    }
  }
  EXPECT_TRUE(hfp_correct);
  EXPECT_TRUE(hsp_correct);
  EXPECT_TRUE(unknown_correct);
  metrics->clear_headset_profile_connection_stats();
  EXPECT_EQ(metrics->headset_profile_connection_stats_size(), 0);
  // Verify that dump after clean up result in an empty list
  BluetoothMetricsLogger::GetInstance()->WriteString(&msg_str);
  metrics->ParseFromString(msg_str);
  EXPECT_EQ(metrics->headset_profile_connection_stats_size(), 0);
  delete metrics;
}
}  // namespace testing
