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

#include "CounterMetric.h"

namespace android {

/**
 * Unit tests for the CounterMetric class.
 */
class CounterMetricTest : public ::testing::Test {
};

TEST_F(CounterMetricTest, IntDataTypeEmpty) {
  CounterMetric<int> metric("MyMetricName", "MetricAttributeName");

  std::map<int, int64_t> values;

  metric.ExportValues(
      [&] (int attribute_value, int64_t value) {
          values[attribute_value] = value;
      });

  EXPECT_TRUE(values.empty());
}

TEST_F(CounterMetricTest, IntDataType) {
  CounterMetric<int> metric("MyMetricName", "MetricAttributeName");

  std::map<int, int64_t> values;

  metric.Increment(7);
  metric.Increment(8);
  metric.Increment(8);

  metric.ExportValues(
      [&] (int attribute_value, int64_t value) {
          values[attribute_value] = value;
      });

  ASSERT_EQ(2u, values.size());
  EXPECT_EQ(1, values[7]);
  EXPECT_EQ(2, values[8]);
}

TEST_F(CounterMetricTest, StringDataType) {
  CounterMetric<std::string> metric("MyMetricName", "MetricAttributeName");

  std::map<std::string, int64_t> values;

  metric.Increment("a");
  metric.Increment("b");
  metric.Increment("b");

  metric.ExportValues(
      [&] (std::string attribute_value, int64_t value) {
          values[attribute_value] = value;
      });

  ASSERT_EQ(2u, values.size());
  EXPECT_EQ(1, values["a"]);
  EXPECT_EQ(2, values["b"]);
}

}  // namespace android
