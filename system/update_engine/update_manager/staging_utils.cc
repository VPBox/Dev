//
// Copyright (C) 2018 The Android Open Source Project
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

#include "update_engine/update_manager/staging_utils.h"

#include <utility>
#include <vector>

#include <base/logging.h>
#include <base/rand_util.h>
#include <base/time/time.h>
#include <policy/device_policy.h>

#include "update_engine/common/constants.h"
#include "update_engine/common/hardware_interface.h"
#include "update_engine/common/prefs_interface.h"
#include "update_engine/system_state.h"

using base::TimeDelta;
using chromeos_update_engine::kPrefsWallClockStagingWaitPeriod;
using chromeos_update_engine::PrefsInterface;
using chromeos_update_engine::SystemState;
using policy::DevicePolicy;

namespace chromeos_update_manager {

int GetStagingSchedule(const DevicePolicy* device_policy,
                       StagingSchedule* staging_schedule_out) {
  StagingSchedule staging_schedule;
  if (!device_policy->GetDeviceUpdateStagingSchedule(&staging_schedule) ||
      staging_schedule.empty()) {
    return 0;
  }

  // Last percentage of the schedule should be 100.
  if (staging_schedule.back().percentage != 100) {
    LOG(ERROR) << "Last percentage of the schedule is not 100, it's: "
               << staging_schedule.back().percentage;
    return 0;
  }

  int previous_days = 0;
  int previous_percentage = -1;
  // Ensure that the schedule has a monotonically increasing set of percentages
  // and that days are also monotonically increasing.
  for (const auto& staging_pair : staging_schedule) {
    int days = staging_pair.days;
    if (previous_days >= days) {
      LOG(ERROR) << "Days in staging schedule are not monotonically "
                 << "increasing. Previous value: " << previous_days
                 << " Current value: " << days;
      return 0;
    }
    previous_days = days;
    int percentage = staging_pair.percentage;
    if (previous_percentage >= percentage) {
      LOG(ERROR) << "Percentages in staging schedule are not monotonically "
                 << "increasing. Previous value: " << previous_percentage
                 << " Current value: " << percentage;
      return 0;
    }
    previous_percentage = percentage;
  }
  // Modify staging schedule only if the schedule in the device policy is valid.
  if (staging_schedule_out)
    *staging_schedule_out = std::move(staging_schedule);

  return previous_days;
}

int CalculateWaitTimeInDaysFromSchedule(
    const StagingSchedule& staging_schedule) {
  int prev_days = 0;
  int percentage_position = base::RandInt(1, 100);
  for (const auto& staging_pair : staging_schedule) {
    int days = staging_pair.days;
    if (percentage_position <= staging_pair.percentage) {
      // Scatter between the start of the range and the end.
      return prev_days + base::RandInt(1, days - prev_days);
    }
    prev_days = days;
  }
  // Something went wrong.
  NOTREACHED();
  return 0;
}

StagingCase CalculateStagingCase(const DevicePolicy* device_policy,
                                 PrefsInterface* prefs,
                                 TimeDelta* staging_wait_time,
                                 StagingSchedule* staging_schedule) {
  // Check that the schedule in the device policy is correct.
  StagingSchedule new_staging_schedule;
  int max_days = GetStagingSchedule(device_policy, &new_staging_schedule);
  if (max_days == 0)
    return StagingCase::kOff;

  // Calculate the new wait time.
  TimeDelta new_staging_wait_time = TimeDelta::FromDays(
      CalculateWaitTimeInDaysFromSchedule(new_staging_schedule));
  DCHECK_GT(new_staging_wait_time.InSeconds(), 0);
  if (staging_wait_time->InSeconds() > 0) {
    // If there hasn't been any changes to the schedule and there is a value
    // set, don't change the waiting time.
    if (new_staging_schedule == *staging_schedule) {
      return StagingCase::kNoAction;
    }
    // Otherwise, update the schedule and wait time.
    *staging_wait_time = new_staging_wait_time;
    *staging_schedule = std::move(new_staging_schedule);
    return StagingCase::kNoSavedValue;
  }
  // Getting this means the schedule changed, update the old schedule.
  *staging_schedule = std::move(new_staging_schedule);

  int64_t wait_period_in_days;
  // There exists a persisted value that is valid. That is, it's smaller than
  // the maximum amount of days of staging set by the user.
  if (prefs->GetInt64(kPrefsWallClockStagingWaitPeriod, &wait_period_in_days) &&
      wait_period_in_days > 0 && wait_period_in_days <= max_days) {
    *staging_wait_time = TimeDelta::FromDays(wait_period_in_days);
    return StagingCase::kSetStagingFromPref;
  }

  *staging_wait_time = new_staging_wait_time;
  return StagingCase::kNoSavedValue;
}

}  // namespace chromeos_update_manager
