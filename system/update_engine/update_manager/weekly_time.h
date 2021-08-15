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
#ifndef UPDATE_ENGINE_UPDATE_MANAGER_WEEKLY_TIME_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_WEEKLY_TIME_H_

#include <string>
#include <vector>

#include <base/time/time.h>

namespace chromeos_update_manager {

// Represents a day of the week and the time since it started.
class WeeklyTime {
 public:
  // Day of week (Sunday = 0 and so on) and time since start of the day (12 AM).
  WeeklyTime(const int& day_of_week, const base::TimeDelta& time)
      : day_of_week_(day_of_week), time_(time) {}

  // Create a weekly time from a time object.
  static WeeklyTime FromTime(const base::Time& time);

  bool operator==(const WeeklyTime& other) const {
    return time_ == other.time() && day_of_week_ == other.day_of_week();
  }

  bool operator!=(const WeeklyTime& other) const { return !(*this == other); }

  // Return the duration between WeeklyTime and |other|. |other| is always
  // considered to be after WeeklyTime. i.e. calling this function on [Friday
  // 12:00, Monday 12:00] would return 3 days.
  base::TimeDelta GetDurationTo(const WeeklyTime& other) const;

  // Gets the weekly time represented as a time delta.
  base::TimeDelta TimeFromStartOfWeek() const;

  // Adds the given |offset| to the time with proper wraparound (e.g. Sunday + 1
  // day = Monday).
  void AddTime(const base::TimeDelta& offset);

  int day_of_week() const { return day_of_week_; }

  base::TimeDelta time() const { return time_; }

 private:
  int day_of_week_;
  base::TimeDelta time_;
};

// Represents an interval of time during a week represented with WeeklyTime
// objects. This interval can span at most 7 days. |end| is always considered to
// be after |start|, this is possible since the times of the week are cyclic.
// For example, the interval [Thursday 12:00, Monday 12:00) will span the time
// between Thursday and Monday.
class WeeklyTimeInterval {
 public:
  WeeklyTimeInterval(const WeeklyTime& start, const WeeklyTime& end)
      : start_(start), end_(end) {}

  // Determines if |time| is in this interval.
  bool InRange(const WeeklyTime& time) const;

  WeeklyTime start() const { return start_; }

  WeeklyTime end() const { return end_; }

  bool operator==(const WeeklyTimeInterval& other) const {
    return start_ == other.start() && end_ == other.end();
  }

  // Converts the interval to a string. Used for the BoxedValue ToString
  // function.
  std::string ToString() const;

 private:
  WeeklyTime start_;
  WeeklyTime end_;
};

using WeeklyTimeIntervalVector = std::vector<WeeklyTimeInterval>;

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_WEEKLY_TIME_H_
