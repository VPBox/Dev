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

#include <gtest/gtest.h>

#include "EventMetric.h"

namespace android {

/**
 * Unit tests for the EventMetric class.
 */

TEST(EventMetricTest, IntDataTypeEmpty) {
  EventMetric<int> metric("MyMetricName", "MetricAttributeName");

  std::map<int, EventStatistics> values;

  metric.ExportValues(
      [&] (int attribute_value, const EventStatistics& value) {
          values[attribute_value] = value;
      });

  EXPECT_TRUE(values.empty());
}

TEST(EventMetricTest, IntDataType) {
  EventMetric<int> metric("MyMetricName", "MetricAttributeName");

  std::map<int, EventStatistics> values;

  metric.Record(4, 7);
  metric.Record(5, 8);
  metric.Record(5, 8);
  metric.Record(5, 8);
  metric.Record(6, 8);
  metric.Record(6, 8);
  metric.Record(6, 8);

  metric.ExportValues(
      [&] (int attribute_value, const EventStatistics& value) {
          values[attribute_value] = value;
      });

  ASSERT_EQ(2u, values.size());
  EXPECT_EQ(4, values[7].min);
  EXPECT_EQ(4, values[7].max);
  EXPECT_EQ(4, values[7].mean);
  EXPECT_EQ(1, values[7].count);

  EXPECT_EQ(5, values[8].min);
  EXPECT_EQ(6, values[8].max);
  // This is an approximate value because of the technique we're using.
  EXPECT_NEAR(5.5, values[8].mean, 0.2);
  EXPECT_EQ(6, values[8].count);
}

TEST(EventMetricTest, StringDataType) {
  EventMetric<std::string> metric("MyMetricName", "MetricAttributeName");

  std::map<std::string, EventStatistics> values;

  metric.Record(1, "a");
  metric.Record(2, "b");
  metric.Record(2, "b");
  metric.Record(3, "b");
  metric.Record(3, "b");

  metric.ExportValues(
      [&] (std::string attribute_value, const EventStatistics& value) {
          values[attribute_value] = value;
      });

  ASSERT_EQ(2u, values.size());
  EXPECT_EQ(1, values["a"].min);
  EXPECT_EQ(1, values["a"].max);
  EXPECT_EQ(1, values["a"].mean);
  EXPECT_EQ(1, values["a"].count);

  EXPECT_EQ(2, values["b"].min);
  EXPECT_EQ(3, values["b"].max);
  EXPECT_NEAR(2.5, values["b"].mean, 0.2);
  EXPECT_EQ(4, values["b"].count);
}

// Helper class that allows us to mock the clock.
template<typename AttributeType>
class MockEventTimer : public EventTimer<AttributeType> {
 public:
  explicit MockEventTimer(nsecs_t time_delta_ns,
                          EventMetric<AttributeType>* metric)
      : EventTimer<AttributeType>(metric) {
    // Pretend the event started earlier.
    this->start_time_ = systemTime() - time_delta_ns;
  }
};

TEST(EventTimerTest, IntDataType) {
  EventMetric<int> metric("MyMetricName", "MetricAttributeName");

  for (int i = 0; i < 5; i++) {
    {
      // Add a mock time delta.
      MockEventTimer<int> metric_timer(i * 1000000, &metric);
      metric_timer.SetAttribute(i % 2);
    }
  }

  std::map<int, EventStatistics> values;
  metric.ExportValues(
      [&] (int attribute_value, const EventStatistics& value) {
          values[attribute_value] = value;
      });

  ASSERT_EQ(2u, values.size());
  EXPECT_LT(values[0].min, values[0].max);
  EXPECT_GE(4000, values[0].max);
  EXPECT_GT(values[0].mean, values[0].min);
  EXPECT_LE(values[0].mean, values[0].max);
  EXPECT_EQ(3, values[0].count);

  EXPECT_LT(values[1].min, values[1].max);
  EXPECT_GE(3000, values[1].max);
  EXPECT_GT(values[1].mean, values[1].min);
  EXPECT_LE(values[1].mean, values[1].max);
  EXPECT_EQ(2, values[1].count);
}

}  // namespace android
