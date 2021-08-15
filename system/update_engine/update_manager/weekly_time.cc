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
#include "update_engine/update_manager/weekly_time.h"

#include <base/strings/string_number_conversions.h>
#include <base/strings/stringprintf.h>
#include <base/time/time.h>

using base::Time;
using base::TimeDelta;
using std::string;

namespace {
const int kDaysInWeek = 7;
}

namespace chromeos_update_manager {

TimeDelta WeeklyTime::GetDurationTo(const WeeklyTime& other) const {
  if (other.TimeFromStartOfWeek() < TimeFromStartOfWeek()) {
    return other.TimeFromStartOfWeek() +
           (TimeDelta::FromDays(kDaysInWeek) - TimeFromStartOfWeek());
  }
  return other.TimeFromStartOfWeek() - TimeFromStartOfWeek();
}

TimeDelta WeeklyTime::TimeFromStartOfWeek() const {
  return TimeDelta::FromDays(day_of_week_) + time_;
}

void WeeklyTime::AddTime(const TimeDelta& offset) {
  time_ += offset;
  int days_over = time_.InDays();
  time_ -= TimeDelta::FromDays(days_over);
  day_of_week_ = (day_of_week_ + days_over - 1) % kDaysInWeek + 1;
}

// static
WeeklyTime WeeklyTime::FromTime(const Time& time) {
  Time::Exploded exploded;
  time.LocalExplode(&exploded);
  return WeeklyTime(exploded.day_of_week,
                    TimeDelta::FromHours(exploded.hour) +
                        TimeDelta::FromMinutes(exploded.minute));
}

bool WeeklyTimeInterval::InRange(const WeeklyTime& time) const {
  return time == start_ ||
         (time.GetDurationTo(start_) >= time.GetDurationTo(end_) &&
          time != end_);
}

string WeeklyTimeInterval::ToString() const {
  return base::StringPrintf(
      "Start: day_of_week=%d time=%d\nEnd: day_of_week=%d time=%d",
      start_.day_of_week(),
      start_.time().InMinutes(),
      end_.day_of_week(),
      end_.time().InMinutes());
}

}  // namespace chromeos_update_manager
