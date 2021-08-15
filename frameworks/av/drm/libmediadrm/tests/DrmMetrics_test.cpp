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

#define LOG_TAG "DrmMetricsTest"
#include "mediadrm/DrmMetrics.h"

#include <android/hardware/drm/1.0/types.h>
#include <android/hardware/drm/1.1/types.h>
#include <binder/PersistableBundle.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/message_differencer.h>
#include <gtest/gtest.h>
#include <utils/Log.h>

#include "protos/metrics.pb.h"

using ::android::drm_metrics::DrmFrameworkMetrics;
using ::android::hardware::hidl_vec;
using ::android::hardware::drm::V1_0::EventType;
using ::android::hardware::drm::V1_2::KeyStatusType;
using ::android::hardware::drm::V1_0::Status;
using ::android::hardware::drm::V1_1::DrmMetricGroup;
using ::android::os::PersistableBundle;
using ::google::protobuf::util::MessageDifferencer;
using ::google::protobuf::TextFormat;

namespace android {

/**
 * Unit tests for the MediaDrmMetrics class.
 */
class MediaDrmMetricsTest : public ::testing::Test {};

/**
 * This derived class mocks the clock for testing purposes.
 */
class FakeMediaDrmMetrics : public MediaDrmMetrics {
 public:
  FakeMediaDrmMetrics() : MediaDrmMetrics(), time_(0) {};

  int64_t GetCurrentTimeMs() { return time_++; }
  int64_t time_;
};

TEST_F(MediaDrmMetricsTest, EmptySuccess) {
  MediaDrmMetrics metrics;
  PersistableBundle bundle;

  metrics.Export(&bundle);
  EXPECT_TRUE(bundle.empty());
}

TEST_F(MediaDrmMetricsTest, AllValuesSuccessCounts) {
  MediaDrmMetrics metrics;

  metrics.mOpenSessionCounter.Increment(OK);
  metrics.mCloseSessionCounter.Increment(OK);

  {
    EventTimer<status_t> get_key_request_timer(&metrics.mGetKeyRequestTimeUs);
    EventTimer<status_t> provide_key_response_timer(
        &metrics.mProvideKeyResponseTimeUs);
    get_key_request_timer.SetAttribute(OK);
    provide_key_response_timer.SetAttribute(OK);
  }

  metrics.mGetProvisionRequestCounter.Increment(OK);
  metrics.mProvideProvisionResponseCounter.Increment(OK);
  metrics.mGetDeviceUniqueIdCounter.Increment(OK);

  metrics.mKeyStatusChangeCounter.Increment(KeyStatusType::USABLE);
  metrics.mEventCounter.Increment(EventType::PROVISION_REQUIRED);

  PersistableBundle bundle;

  metrics.Export(&bundle);
  EXPECT_EQ(11U, bundle.size());

  // Verify the list of pairs of int64 metrics.
  std::vector<std::pair<std::string, int64_t>> expected_values = {
      { "drm.mediadrm.open_session.ok.count", 1 },
      { "drm.mediadrm.close_session.ok.count", 1 },
      { "drm.mediadrm.get_key_request.ok.count", 1 },
      { "drm.mediadrm.provide_key_response.ok.count", 1 },
      { "drm.mediadrm.get_provision_request.ok.count", 1 },
      { "drm.mediadrm.provide_provision_response.ok.count", 1 },
      { "drm.mediadrm.key_status_change.USABLE.count", 1 },
      { "drm.mediadrm.event.PROVISION_REQUIRED.count", 1 },
      { "drm.mediadrm.get_device_unique_id.ok.count", 1 }};
  for (const auto& expected_pair : expected_values) {
    String16 key(expected_pair.first.c_str());
    int64_t value = -1;
    EXPECT_TRUE(bundle.getLong(key, &value))
        << "Unexpected error retrieviing key: " << key;
    EXPECT_EQ(expected_pair.second, value)
        << "Unexpected value for " << expected_pair.first << ". " << value;
  }

  // Validate timing values exist.
  String16 get_key_request_key(
      "drm.mediadrm.get_key_request.ok.average_time_micros");
  String16 provide_key_response_key(
      "drm.mediadrm.provide_key_response.ok.average_time_micros");
  int64_t value = -1;
  EXPECT_TRUE(bundle.getLong(get_key_request_key, &value));
  EXPECT_GE(value, 0);
  value = -1;
  EXPECT_TRUE(bundle.getLong(provide_key_response_key, &value));
  EXPECT_GE(value, 0);
}

TEST_F(MediaDrmMetricsTest, AllValuesFull) {
  MediaDrmMetrics metrics;

  metrics.mOpenSessionCounter.Increment(OK);
  metrics.mOpenSessionCounter.Increment(UNEXPECTED_NULL);

  metrics.mCloseSessionCounter.Increment(OK);
  metrics.mCloseSessionCounter.Increment(UNEXPECTED_NULL);

  for (status_t s : {OK, UNEXPECTED_NULL}) {
    {
      EventTimer<status_t> get_key_request_timer(&metrics.mGetKeyRequestTimeUs);
      EventTimer<status_t> provide_key_response_timer(
          &metrics.mProvideKeyResponseTimeUs);
      get_key_request_timer.SetAttribute(s);
      provide_key_response_timer.SetAttribute(s);
    }
  }

  metrics.mGetProvisionRequestCounter.Increment(OK);
  metrics.mGetProvisionRequestCounter.Increment(UNEXPECTED_NULL);
  metrics.mProvideProvisionResponseCounter.Increment(OK);
  metrics.mProvideProvisionResponseCounter.Increment(UNEXPECTED_NULL);
  metrics.mGetDeviceUniqueIdCounter.Increment(OK);
  metrics.mGetDeviceUniqueIdCounter.Increment(UNEXPECTED_NULL);

  metrics.mKeyStatusChangeCounter.Increment(KeyStatusType::USABLE);
  metrics.mKeyStatusChangeCounter.Increment(KeyStatusType::EXPIRED);
  metrics.mKeyStatusChangeCounter.Increment(KeyStatusType::OUTPUTNOTALLOWED);
  metrics.mKeyStatusChangeCounter.Increment(KeyStatusType::STATUSPENDING);
  metrics.mKeyStatusChangeCounter.Increment(KeyStatusType::INTERNALERROR);
  metrics.mEventCounter.Increment(EventType::PROVISION_REQUIRED);
  metrics.mEventCounter.Increment(EventType::KEY_NEEDED);
  metrics.mEventCounter.Increment(EventType::KEY_EXPIRED);
  metrics.mEventCounter.Increment(EventType::VENDOR_DEFINED);
  metrics.mEventCounter.Increment(EventType::SESSION_RECLAIMED);

  android::Vector<uint8_t> sessionId1;
  sessionId1.push_back(1);
  sessionId1.push_back(2);
  android::Vector<uint8_t> sessionId2;
  sessionId2.push_back(3);
  sessionId2.push_back(4);
  String16 hexSessionId1("0102");
  String16 hexSessionId2("0304");

  metrics.SetSessionStart(sessionId1);
  metrics.SetSessionStart(sessionId2);
  metrics.SetSessionEnd(sessionId2);
  metrics.SetSessionEnd(sessionId1);

  PersistableBundle bundle;
  metrics.Export(&bundle);
  EXPECT_EQ(35U, bundle.size());

  // Verify the list of pairs of int64 metrics.
  std::vector<std::pair<std::string, int64_t>> expected_values = {
      { "drm.mediadrm.open_session.ok.count", 1 },
      { "drm.mediadrm.close_session.ok.count", 1 },
      { "drm.mediadrm.get_key_request.ok.count", 1 },
      { "drm.mediadrm.provide_key_response.ok.count", 1 },
      { "drm.mediadrm.get_provision_request.ok.count", 1 },
      { "drm.mediadrm.provide_provision_response.ok.count", 1 },
      { "drm.mediadrm.get_device_unique_id.ok.count", 1 },
      { "drm.mediadrm.open_session.error.count", 1 },
      { "drm.mediadrm.close_session.error.count", 1 },
      { "drm.mediadrm.get_key_request.error.count", 1 },
      { "drm.mediadrm.provide_key_response.error.count", 1 },
      { "drm.mediadrm.get_provision_request.error.count", 1 },
      { "drm.mediadrm.provide_provision_response.error.count", 1 },
      { "drm.mediadrm.get_device_unique_id.error.count", 1 },
      { "drm.mediadrm.key_status_change.USABLE.count", 1 },
      { "drm.mediadrm.key_status_change.EXPIRED.count", 1 },
      { "drm.mediadrm.key_status_change.OUTPUT_NOT_ALLOWED.count", 1 },
      { "drm.mediadrm.key_status_change.STATUS_PENDING.count", 1 },
      { "drm.mediadrm.key_status_change.INTERNAL_ERROR.count", 1 },
      { "drm.mediadrm.event.PROVISION_REQUIRED.count", 1 },
      { "drm.mediadrm.event.KEY_NEEDED.count", 1 },
      { "drm.mediadrm.event.KEY_EXPIRED.count", 1 },
      { "drm.mediadrm.event.VENDOR_DEFINED.count", 1 },
      { "drm.mediadrm.event.SESSION_RECLAIMED.count", 1 }};
  for (const auto& expected_pair : expected_values) {
    String16 key(expected_pair.first.c_str());
    int64_t value = -1;
    EXPECT_TRUE(bundle.getLong(key, &value))
        << "Unexpected error retrieviing key: " << key;
    EXPECT_EQ(expected_pair.second, value)
        << "Unexpected value for " << expected_pair.first << ". " << value;
  }

  // Verify the error lists
  std::vector<std::pair<std::string, std::vector<int64_t>>> expected_vector_values = {
      { "drm.mediadrm.close_session.error.list", { UNEXPECTED_NULL } },
      { "drm.mediadrm.get_device_unique_id.error.list", { UNEXPECTED_NULL } },
      { "drm.mediadrm.get_key_request.error.list", { UNEXPECTED_NULL } },
      { "drm.mediadrm.get_provision_request.error.list", { UNEXPECTED_NULL } },
      { "drm.mediadrm.open_session.error.list", { UNEXPECTED_NULL } },
      { "drm.mediadrm.provide_key_response.error.list", { UNEXPECTED_NULL } },
      { "drm.mediadrm.provide_provision_response.error.list", { UNEXPECTED_NULL } }};
  for (const auto& expected_pair : expected_vector_values) {
    String16 key(expected_pair.first.c_str());
    std::vector<int64_t> values;
    EXPECT_TRUE(bundle.getLongVector(key, &values))
        << "Unexpected error retrieviing key: " << key;
    for (auto expected : expected_pair.second) {
      EXPECT_TRUE(std::find(values.begin(), values.end(), expected) != values.end())
          << "Could not find " << expected << " for key " << expected_pair.first;
    }
  }

  // Verify the lifespans
  PersistableBundle start_times;
  PersistableBundle end_times;
  String16 start_time_key("drm.mediadrm.session_start_times_ms");
  String16 end_time_key("drm.mediadrm.session_end_times_ms");
  ASSERT_TRUE(bundle.getPersistableBundle(start_time_key, &start_times));
  ASSERT_TRUE(bundle.getPersistableBundle(end_time_key, &end_times));
  EXPECT_EQ(2U, start_times.size());
  EXPECT_EQ(2U, end_times.size());
  int64_t start_time, end_time;
  for (const auto& sid : { hexSessionId1, hexSessionId2 }) {
    start_time = -1;
    end_time = -1;
    EXPECT_TRUE(start_times.getLong(sid, &start_time));
    EXPECT_TRUE(end_times.getLong(sid, &end_time));
    EXPECT_GT(start_time, 0);
    EXPECT_GE(end_time, start_time);
  }

  // Validate timing values exist.
  String16 get_key_request_key(
      "drm.mediadrm.get_key_request.ok.average_time_micros");
  String16 provide_key_response_key(
      "drm.mediadrm.provide_key_response.ok.average_time_micros");
  int64_t value = -1;
  EXPECT_TRUE(bundle.getLong(get_key_request_key, &value));
  EXPECT_GE(value, 0);
  value = -1;
  EXPECT_TRUE(bundle.getLong(provide_key_response_key, &value));
  EXPECT_GE(value, 0);
}


TEST_F(MediaDrmMetricsTest, CounterValuesProtoSerialization) {
  MediaDrmMetrics metrics;

  metrics.mOpenSessionCounter.Increment(OK);
  metrics.mOpenSessionCounter.Increment(UNEXPECTED_NULL);
  metrics.mCloseSessionCounter.Increment(OK);
  metrics.mCloseSessionCounter.Increment(UNEXPECTED_NULL);

  metrics.mGetProvisionRequestCounter.Increment(OK);
  metrics.mGetProvisionRequestCounter.Increment(UNEXPECTED_NULL);
  metrics.mProvideProvisionResponseCounter.Increment(OK);
  metrics.mProvideProvisionResponseCounter.Increment(UNEXPECTED_NULL);
  metrics.mGetDeviceUniqueIdCounter.Increment(OK);
  metrics.mGetDeviceUniqueIdCounter.Increment(UNEXPECTED_NULL);

  metrics.mKeyStatusChangeCounter.Increment(KeyStatusType::USABLE);
  metrics.mKeyStatusChangeCounter.Increment(KeyStatusType::EXPIRED);
  metrics.mKeyStatusChangeCounter.Increment(KeyStatusType::OUTPUTNOTALLOWED);
  metrics.mKeyStatusChangeCounter.Increment(KeyStatusType::STATUSPENDING);
  metrics.mKeyStatusChangeCounter.Increment(KeyStatusType::INTERNALERROR);
  metrics.mEventCounter.Increment(EventType::PROVISION_REQUIRED);
  metrics.mEventCounter.Increment(EventType::KEY_NEEDED);
  metrics.mEventCounter.Increment(EventType::KEY_EXPIRED);
  metrics.mEventCounter.Increment(EventType::VENDOR_DEFINED);
  metrics.mEventCounter.Increment(EventType::SESSION_RECLAIMED);

  std::string serializedMetrics;
  ASSERT_EQ(OK, metrics.GetSerializedMetrics(&serializedMetrics));

  DrmFrameworkMetrics metricsProto;
  ASSERT_TRUE(metricsProto.ParseFromString(serializedMetrics));

  std::string expectedMetrics =
      "open_session_counter { count: 1 attributes { error_code: -0x7FFFFFF8 } } "
      "open_session_counter { count: 1 attributes { error_code: 0 } } "
      "close_session_counter { count: 1 attributes { error_code: -0x7FFFFFF8 } } "
      "close_session_counter { count: 1 attributes { error_code: 0 } } "
      "get_provisioning_request_counter { count: 1 attributes { error_code: -0x7FFFFFF8 } } "
      "get_provisioning_request_counter { count: 1 attributes { error_code: 0 } } "
      "provide_provisioning_response_counter { count: 1 attributes { error_code: -0x7ffffff8 } } "
      "provide_provisioning_response_counter { count: 1 attributes { error_code: 0 } } "
      "get_device_unique_id_counter { count: 1 attributes { error_code: -0x7ffffff8 } } "
      "get_device_unique_id_counter { count: 1 attributes { error_code: 0 } } "
      "key_status_change_counter { count: 1 attributes { key_status_type: 0 } } "
      "key_status_change_counter { count: 1 attributes { key_status_type: 1 } } "
      "key_status_change_counter { count: 1 attributes { key_status_type: 2 } } "
      "key_status_change_counter { count: 1 attributes { key_status_type: 3 } } "
      "key_status_change_counter { count: 1 attributes { key_status_type: 4 } } "
      "event_callback_counter { count: 1 attributes { event_type: 0 } } "
      "event_callback_counter { count: 1 attributes { event_type: 1 } } "
      "event_callback_counter { count: 1 attributes { event_type: 2 } } "
      "event_callback_counter { count: 1 attributes { event_type: 3 } } "
      "event_callback_counter { count: 1 attributes { event_type: 4 } } ";

  DrmFrameworkMetrics expectedMetricsProto;
  ASSERT_TRUE(TextFormat::MergeFromString(expectedMetrics, &expectedMetricsProto));

  std::string diffString;
  MessageDifferencer differ;
  differ.ReportDifferencesToString(&diffString);
  ASSERT_TRUE(differ.Compare(expectedMetricsProto, metricsProto))
      << diffString;
}

TEST_F(MediaDrmMetricsTest, TimeMetricsProtoSerialization) {
  MediaDrmMetrics metrics;

  for (status_t s : {OK, UNEXPECTED_NULL}) {
    double time = 0;
    for (int i = 0; i < 5; i++) {
      time += 1.0;
      metrics.mGetKeyRequestTimeUs.Record(time, s);
      metrics.mProvideKeyResponseTimeUs.Record(time, s);
    }
  }

  std::string serializedMetrics;
  ASSERT_EQ(OK, metrics.GetSerializedMetrics(&serializedMetrics));

  DrmFrameworkMetrics metricsProto;
  ASSERT_TRUE(metricsProto.ParseFromString(serializedMetrics));

  std::string expectedMetrics =
      "get_key_request_time_us { "
      "  min: 1 max: 5 mean: 3.5 variance: 1 operation_count: 5 "
      "  attributes { error_code: -0x7FFFFFF8 } "
      "} "
      "get_key_request_time_us { "
      "  min: 1 max: 5 mean: 3.5 variance: 1 operation_count: 5 "
      "  attributes { error_code: 0 } "
      "} "
      "provide_key_response_time_us { "
      "  min: 1 max: 5 mean: 3.5 variance: 1 operation_count: 5 "
      "  attributes { error_code: -0x7FFFFFF8 } "
      "} "
      "provide_key_response_time_us { "
      "  min: 1 max: 5 mean: 3.5 variance: 1 operation_count: 5 "
      "  attributes { error_code: 0 } "
      "} ";

  DrmFrameworkMetrics expectedMetricsProto;
  ASSERT_TRUE(TextFormat::MergeFromString(expectedMetrics, &expectedMetricsProto));

  std::string diffString;
  MessageDifferencer differ;
  differ.ReportDifferencesToString(&diffString);
  ASSERT_TRUE(differ.Compare(expectedMetricsProto, metricsProto))
      << diffString;
}

TEST_F(MediaDrmMetricsTest, SessionLifetimeProtoSerialization) {
  // Use the fake so the clock is predictable;
  FakeMediaDrmMetrics metrics;

  android::Vector<uint8_t> sessionId1;
  sessionId1.push_back(1);
  sessionId1.push_back(2);
  android::Vector<uint8_t> sessionId2;
  sessionId2.push_back(3);
  sessionId2.push_back(4);

  metrics.SetSessionStart(sessionId1);
  metrics.SetSessionStart(sessionId2);
  metrics.SetSessionEnd(sessionId2);
  metrics.SetSessionEnd(sessionId1);

  std::string serializedMetrics;
  ASSERT_EQ(OK, metrics.GetSerializedMetrics(&serializedMetrics));

  DrmFrameworkMetrics metricsProto;
  ASSERT_TRUE(metricsProto.ParseFromString(serializedMetrics));

  std::string expectedMetrics =
      "session_lifetimes: { "
      "  key: '0102' "
      "  value { start_time_ms: 0 end_time_ms: 3 } "
      "} "
      "session_lifetimes: { "
      "  key: '0304' "
      "  value { start_time_ms: 1 end_time_ms: 2 } "
      "} ";

  DrmFrameworkMetrics expectedMetricsProto;
  ASSERT_TRUE(TextFormat::MergeFromString(expectedMetrics, &expectedMetricsProto));

  std::string diffString;
  MessageDifferencer differ;
  differ.ReportDifferencesToString(&diffString);
  ASSERT_TRUE(differ.Compare(expectedMetricsProto, metricsProto))
      << diffString;
}

TEST_F(MediaDrmMetricsTest, HidlToBundleMetricsEmpty) {
  hidl_vec<DrmMetricGroup> hidlMetricGroups;
  PersistableBundle bundleMetricGroups;

  ASSERT_EQ(OK, MediaDrmMetrics::HidlMetricsToBundle(hidlMetricGroups, &bundleMetricGroups));
  ASSERT_EQ(0U, bundleMetricGroups.size());
}

TEST_F(MediaDrmMetricsTest, HidlToBundleMetricsMultiple) {
  DrmMetricGroup hidlMetricGroup =
      { { {
              "open_session_ok",
              { { "status", DrmMetricGroup::ValueType::INT64_TYPE,
                  (int64_t) Status::OK, 0.0, "" } },
              { { "count", DrmMetricGroup::ValueType::INT64_TYPE, 3, 0.0, "" } }
          },
          {
              "close_session_not_opened",
              { { "status", DrmMetricGroup::ValueType::INT64_TYPE,
                  (int64_t) Status::ERROR_DRM_SESSION_NOT_OPENED, 0.0, "" } },
              { { "count", DrmMetricGroup::ValueType::INT64_TYPE, 7, 0.0, "" } }
          } } };

  PersistableBundle bundleMetricGroups;
  ASSERT_EQ(OK, MediaDrmMetrics::HidlMetricsToBundle(hidl_vec<DrmMetricGroup>({hidlMetricGroup}),
                                                     &bundleMetricGroups));
  ASSERT_EQ(1U, bundleMetricGroups.size());
  PersistableBundle bundleMetricGroup;
  ASSERT_TRUE(bundleMetricGroups.getPersistableBundle(String16("[0]"), &bundleMetricGroup));
  ASSERT_EQ(2U, bundleMetricGroup.size());

  // Verify each metric.
  PersistableBundle metric;
  ASSERT_TRUE(bundleMetricGroup.getPersistableBundle(String16("open_session_ok"), &metric));
  PersistableBundle metricInstance;
  ASSERT_TRUE(metric.getPersistableBundle(String16("[0]"), &metricInstance));
  int64_t value = 0;
  ASSERT_TRUE(metricInstance.getLong(String16("count"), &value));
  ASSERT_EQ(3, value);
  PersistableBundle attributeBundle;
  ASSERT_TRUE(metricInstance.getPersistableBundle(String16("attributes"), &attributeBundle));
  ASSERT_TRUE(attributeBundle.getLong(String16("status"), &value));
  ASSERT_EQ((int64_t) Status::OK, value);

  ASSERT_TRUE(bundleMetricGroup.getPersistableBundle(String16("close_session_not_opened"),
                                                     &metric));
  ASSERT_TRUE(metric.getPersistableBundle(String16("[0]"), &metricInstance));
  ASSERT_TRUE(metricInstance.getLong(String16("count"), &value));
  ASSERT_EQ(7, value);
  ASSERT_TRUE(metricInstance.getPersistableBundle(String16("attributes"), &attributeBundle));
  value = 0;
  ASSERT_TRUE(attributeBundle.getLong(String16("status"), &value));
  ASSERT_EQ((int64_t) Status::ERROR_DRM_SESSION_NOT_OPENED, value);
}

}  // namespace android
