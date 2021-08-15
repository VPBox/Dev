//
// Copyright (C) 2015 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "update_engine/metrics_utils.h"

#include <gtest/gtest.h>

#include "update_engine/common/fake_clock.h"
#include "update_engine/common/fake_prefs.h"
#include "update_engine/fake_system_state.h"

namespace chromeos_update_engine {
namespace metrics_utils {

class MetricsUtilsTest : public ::testing::Test {};

TEST(MetricsUtilsTest, GetConnectionType) {
  // Check that expected combinations map to the right value.
  EXPECT_EQ(metrics::ConnectionType::kUnknown,
            GetConnectionType(ConnectionType::kUnknown,
                              ConnectionTethering::kUnknown));
  EXPECT_EQ(metrics::ConnectionType::kDisconnected,
            GetConnectionType(ConnectionType::kDisconnected,
                              ConnectionTethering::kUnknown));
  EXPECT_EQ(metrics::ConnectionType::kEthernet,
            GetConnectionType(ConnectionType::kEthernet,
                              ConnectionTethering::kUnknown));
  EXPECT_EQ(
      metrics::ConnectionType::kWifi,
      GetConnectionType(ConnectionType::kWifi, ConnectionTethering::kUnknown));
  EXPECT_EQ(
      metrics::ConnectionType::kWimax,
      GetConnectionType(ConnectionType::kWimax, ConnectionTethering::kUnknown));
  EXPECT_EQ(metrics::ConnectionType::kBluetooth,
            GetConnectionType(ConnectionType::kBluetooth,
                              ConnectionTethering::kUnknown));
  EXPECT_EQ(metrics::ConnectionType::kCellular,
            GetConnectionType(ConnectionType::kCellular,
                              ConnectionTethering::kUnknown));
  EXPECT_EQ(metrics::ConnectionType::kTetheredEthernet,
            GetConnectionType(ConnectionType::kEthernet,
                              ConnectionTethering::kConfirmed));
  EXPECT_EQ(metrics::ConnectionType::kTetheredWifi,
            GetConnectionType(ConnectionType::kWifi,
                              ConnectionTethering::kConfirmed));

  // Ensure that we don't report tethered ethernet unless it's confirmed.
  EXPECT_EQ(metrics::ConnectionType::kEthernet,
            GetConnectionType(ConnectionType::kEthernet,
                              ConnectionTethering::kNotDetected));
  EXPECT_EQ(metrics::ConnectionType::kEthernet,
            GetConnectionType(ConnectionType::kEthernet,
                              ConnectionTethering::kSuspected));
  EXPECT_EQ(metrics::ConnectionType::kEthernet,
            GetConnectionType(ConnectionType::kEthernet,
                              ConnectionTethering::kUnknown));

  // Ditto for tethered wifi.
  EXPECT_EQ(metrics::ConnectionType::kWifi,
            GetConnectionType(ConnectionType::kWifi,
                              ConnectionTethering::kNotDetected));
  EXPECT_EQ(metrics::ConnectionType::kWifi,
            GetConnectionType(ConnectionType::kWifi,
                              ConnectionTethering::kSuspected));
  EXPECT_EQ(
      metrics::ConnectionType::kWifi,
      GetConnectionType(ConnectionType::kWifi, ConnectionTethering::kUnknown));
}

TEST(MetricsUtilsTest, WallclockDurationHelper) {
  FakeSystemState fake_system_state;
  FakeClock fake_clock;
  base::TimeDelta duration;
  const std::string state_variable_key = "test-prefs";
  FakePrefs fake_prefs;

  fake_system_state.set_clock(&fake_clock);
  fake_system_state.set_prefs(&fake_prefs);

  // Initialize wallclock to 1 sec.
  fake_clock.SetWallclockTime(base::Time::FromInternalValue(1000000));

  // First time called so no previous measurement available.
  EXPECT_FALSE(metrics_utils::WallclockDurationHelper(
      &fake_system_state, state_variable_key, &duration));

  // Next time, we should get zero since the clock didn't advance.
  EXPECT_TRUE(metrics_utils::WallclockDurationHelper(
      &fake_system_state, state_variable_key, &duration));
  EXPECT_EQ(duration.InSeconds(), 0);

  // We can also call it as many times as we want with it being
  // considered a failure.
  EXPECT_TRUE(metrics_utils::WallclockDurationHelper(
      &fake_system_state, state_variable_key, &duration));
  EXPECT_EQ(duration.InSeconds(), 0);
  EXPECT_TRUE(metrics_utils::WallclockDurationHelper(
      &fake_system_state, state_variable_key, &duration));
  EXPECT_EQ(duration.InSeconds(), 0);

  // Advance the clock one second, then we should get 1 sec on the
  // next call and 0 sec on the subsequent call.
  fake_clock.SetWallclockTime(base::Time::FromInternalValue(2000000));
  EXPECT_TRUE(metrics_utils::WallclockDurationHelper(
      &fake_system_state, state_variable_key, &duration));
  EXPECT_EQ(duration.InSeconds(), 1);
  EXPECT_TRUE(metrics_utils::WallclockDurationHelper(
      &fake_system_state, state_variable_key, &duration));
  EXPECT_EQ(duration.InSeconds(), 0);

  // Advance clock two seconds and we should get 2 sec and then 0 sec.
  fake_clock.SetWallclockTime(base::Time::FromInternalValue(4000000));
  EXPECT_TRUE(metrics_utils::WallclockDurationHelper(
      &fake_system_state, state_variable_key, &duration));
  EXPECT_EQ(duration.InSeconds(), 2);
  EXPECT_TRUE(metrics_utils::WallclockDurationHelper(
      &fake_system_state, state_variable_key, &duration));
  EXPECT_EQ(duration.InSeconds(), 0);

  // There's a possibility that the wallclock can go backwards (NTP
  // adjustments, for example) so check that we properly handle this
  // case.
  fake_clock.SetWallclockTime(base::Time::FromInternalValue(3000000));
  EXPECT_FALSE(metrics_utils::WallclockDurationHelper(
      &fake_system_state, state_variable_key, &duration));
  fake_clock.SetWallclockTime(base::Time::FromInternalValue(4000000));
  EXPECT_TRUE(metrics_utils::WallclockDurationHelper(
      &fake_system_state, state_variable_key, &duration));
  EXPECT_EQ(duration.InSeconds(), 1);
}

TEST(MetricsUtilsTest, MonotonicDurationHelper) {
  int64_t storage = 0;
  FakeSystemState fake_system_state;
  FakeClock fake_clock;
  base::TimeDelta duration;

  fake_system_state.set_clock(&fake_clock);

  // Initialize monotonic clock to 1 sec.
  fake_clock.SetMonotonicTime(base::Time::FromInternalValue(1000000));

  // First time called so no previous measurement available.
  EXPECT_FALSE(metrics_utils::MonotonicDurationHelper(
      &fake_system_state, &storage, &duration));

  // Next time, we should get zero since the clock didn't advance.
  EXPECT_TRUE(metrics_utils::MonotonicDurationHelper(
      &fake_system_state, &storage, &duration));
  EXPECT_EQ(duration.InSeconds(), 0);

  // We can also call it as many times as we want with it being
  // considered a failure.
  EXPECT_TRUE(metrics_utils::MonotonicDurationHelper(
      &fake_system_state, &storage, &duration));
  EXPECT_EQ(duration.InSeconds(), 0);
  EXPECT_TRUE(metrics_utils::MonotonicDurationHelper(
      &fake_system_state, &storage, &duration));
  EXPECT_EQ(duration.InSeconds(), 0);

  // Advance the clock one second, then we should get 1 sec on the
  // next call and 0 sec on the subsequent call.
  fake_clock.SetMonotonicTime(base::Time::FromInternalValue(2000000));
  EXPECT_TRUE(metrics_utils::MonotonicDurationHelper(
      &fake_system_state, &storage, &duration));
  EXPECT_EQ(duration.InSeconds(), 1);
  EXPECT_TRUE(metrics_utils::MonotonicDurationHelper(
      &fake_system_state, &storage, &duration));
  EXPECT_EQ(duration.InSeconds(), 0);

  // Advance clock two seconds and we should get 2 sec and then 0 sec.
  fake_clock.SetMonotonicTime(base::Time::FromInternalValue(4000000));
  EXPECT_TRUE(metrics_utils::MonotonicDurationHelper(
      &fake_system_state, &storage, &duration));
  EXPECT_EQ(duration.InSeconds(), 2);
  EXPECT_TRUE(metrics_utils::MonotonicDurationHelper(
      &fake_system_state, &storage, &duration));
  EXPECT_EQ(duration.InSeconds(), 0);
}

}  // namespace metrics_utils
}  // namespace chromeos_update_engine
