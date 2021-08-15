//
// Copyright (C) 2017 The Android Open Source Project
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

#include "update_engine/update_manager/policy_test_utils.h"

#include <memory>
#include <tuple>
#include <vector>

#include "update_engine/update_manager/next_update_check_policy_impl.h"

using base::Time;
using base::TimeDelta;
using chromeos_update_engine::ErrorCode;
using std::string;
using std::tuple;
using std::vector;

namespace chromeos_update_manager {

void UmPolicyTestBase::SetUp() {
  loop_.SetAsCurrent();
  SetUpDefaultClock();
  eval_ctx_ = new EvaluationContext(&fake_clock_, TimeDelta::FromSeconds(5));
  SetUpDefaultState();
}

void UmPolicyTestBase::TearDown() {
  EXPECT_FALSE(loop_.PendingTasks());
}

// Sets the clock to fixed values.
void UmPolicyTestBase::SetUpDefaultClock() {
  fake_clock_.SetMonotonicTime(Time::FromInternalValue(12345678L));
  fake_clock_.SetWallclockTime(Time::FromInternalValue(12345678901234L));
}

void UmPolicyTestBase::SetUpDefaultTimeProvider() {
  Time current_time = fake_clock_.GetWallclockTime();
  base::Time::Exploded exploded;
  current_time.LocalExplode(&exploded);
  fake_state_.time_provider()->var_curr_hour()->reset(new int(exploded.hour));
  fake_state_.time_provider()->var_curr_minute()->reset(
      new int(exploded.minute));
  fake_state_.time_provider()->var_curr_date()->reset(
      new Time(current_time.LocalMidnight()));
}

void UmPolicyTestBase::SetUpDefaultState() {
  fake_state_.updater_provider()->var_updater_started_time()->reset(
      new Time(fake_clock_.GetWallclockTime()));
  fake_state_.updater_provider()->var_last_checked_time()->reset(
      new Time(fake_clock_.GetWallclockTime()));
  fake_state_.updater_provider()->var_consecutive_failed_update_checks()->reset(
      new unsigned int(0));  // NOLINT(readability/casting)
  fake_state_.updater_provider()->var_server_dictated_poll_interval()->reset(
      new unsigned int(0));  // NOLINT(readability/casting)
  fake_state_.updater_provider()->var_forced_update_requested()->reset(
      new UpdateRequestStatus{UpdateRequestStatus::kNone});

  // Chosen by fair dice roll.  Guaranteed to be random.
  fake_state_.random_provider()->var_seed()->reset(new uint64_t(4));
}

// Returns a default UpdateState structure:
UpdateState UmPolicyTestBase::GetDefaultUpdateState(
    TimeDelta first_seen_period) {
  Time first_seen_time = fake_clock_.GetWallclockTime() - first_seen_period;
  UpdateState update_state = UpdateState();

  // This is a non-interactive check returning a delta payload, seen for the
  // first time (|first_seen_period| ago). Clearly, there were no failed
  // attempts so far.
  update_state.interactive = false;
  update_state.is_delta_payload = false;
  update_state.first_seen = first_seen_time;
  update_state.num_checks = 1;
  update_state.num_failures = 0;
  update_state.failures_last_updated = Time();  // Needs to be zero.
  // There's a single HTTP download URL with a maximum of 10 retries.
  update_state.download_urls = vector<string>{"http://fake/url/"};
  update_state.download_errors_max = 10;
  // Download was never attempted.
  update_state.last_download_url_idx = -1;
  update_state.last_download_url_num_errors = 0;
  // There were no download errors.
  update_state.download_errors = vector<tuple<int, ErrorCode, Time>>();
  // P2P is not disabled by Omaha.
  update_state.p2p_downloading_disabled = false;
  update_state.p2p_sharing_disabled = false;
  // P2P was not attempted.
  update_state.p2p_num_attempts = 0;
  update_state.p2p_first_attempted = Time();
  // No active backoff period, backoff is not disabled by Omaha.
  update_state.backoff_expiry = Time();
  update_state.is_backoff_disabled = false;
  // There is no active scattering wait period (max 7 days allowed) nor check
  // threshold (none allowed).
  update_state.scatter_wait_period = TimeDelta();
  update_state.scatter_check_threshold = 0;
  update_state.scatter_wait_period_max = TimeDelta::FromDays(7);
  update_state.scatter_check_threshold_min = 0;
  update_state.scatter_check_threshold_max = 0;

  return update_state;
}

}  // namespace chromeos_update_manager
