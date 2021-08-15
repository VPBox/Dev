//
// Copyright (C) 2014 The Android Open Source Project
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

#include "update_engine/update_manager/boxed_value.h"

#include <gtest/gtest.h>
#include <list>
#include <map>
#include <set>
#include <string>
#include <utility>

#include <base/strings/stringprintf.h>
#include <base/time/time.h>

#include "update_engine/update_manager/rollback_prefs.h"
#include "update_engine/update_manager/shill_provider.h"
#include "update_engine/update_manager/umtest_utils.h"
#include "update_engine/update_manager/updater_provider.h"
#include "update_engine/update_manager/weekly_time.h"

using base::Time;
using base::TimeDelta;
using chromeos_update_engine::ConnectionTethering;
using chromeos_update_engine::ConnectionType;
using std::list;
using std::map;
using std::set;
using std::string;

namespace chromeos_update_manager {

// The DeleterMarker flags a bool variable when the class is destroyed.
class DeleterMarker {
 public:
  explicit DeleterMarker(bool* marker) : marker_(marker) { *marker_ = false; }

  ~DeleterMarker() { *marker_ = true; }

 private:
  friend string BoxedValue::ValuePrinter<DeleterMarker>(const void*);

  // Pointer to the bool marker.
  bool* marker_;
};

template <>
string BoxedValue::ValuePrinter<DeleterMarker>(const void* value) {
  const DeleterMarker* val = reinterpret_cast<const DeleterMarker*>(value);
  return base::StringPrintf("DeleterMarker:%s",
                            *val->marker_ ? "true" : "false");
}

TEST(UmBoxedValueTest, Deleted) {
  bool marker = true;
  const DeleterMarker* deleter_marker = new DeleterMarker(&marker);

  EXPECT_FALSE(marker);
  BoxedValue* box = new BoxedValue(deleter_marker);
  EXPECT_FALSE(marker);
  delete box;
  EXPECT_TRUE(marker);
}

TEST(UmBoxedValueTest, MoveConstructor) {
  bool marker = true;
  const DeleterMarker* deleter_marker = new DeleterMarker(&marker);

  BoxedValue* box = new BoxedValue(deleter_marker);
  BoxedValue* new_box = new BoxedValue(std::move(*box));
  // box is now undefined but valid.
  delete box;
  EXPECT_FALSE(marker);
  // The deleter_marker gets deleted at this point.
  delete new_box;
  EXPECT_TRUE(marker);
}

TEST(UmBoxedValueTest, MixedList) {
  list<BoxedValue> lst;
  // This is mostly a compile test.
  lst.emplace_back(new const int{42});
  lst.emplace_back(new const string("Hello world!"));
  bool marker;
  lst.emplace_back(new const DeleterMarker(&marker));
  EXPECT_FALSE(marker);
  lst.clear();
  EXPECT_TRUE(marker);
}

TEST(UmBoxedValueTest, MixedMap) {
  map<int, BoxedValue> m;
  m.emplace(42, BoxedValue(new const string("Hola mundo!")));

  auto it = m.find(42);
  ASSERT_NE(it, m.end());
  EXPECT_NE(nullptr, it->second.value());
  EXPECT_EQ(nullptr, m[33].value());
}

TEST(UmBoxedValueTest, StringToString) {
  EXPECT_EQ("Hej Verden!", BoxedValue(new string("Hej Verden!")).ToString());
}

TEST(UmBoxedValueTest, IntToString) {
  EXPECT_EQ("42", BoxedValue(new int(42)).ToString());
}

TEST(UmBoxedValueTest, Int64ToString) {
  // -123456789012345 doesn't fit in 32-bit integers.
  EXPECT_EQ("-123456789012345",
            BoxedValue(new int64_t(-123456789012345LL)).ToString());
}

TEST(UmBoxedValueTest, UnsignedIntToString) {
  // 4294967295 is the biggest possible 32-bit unsigned integer.
  EXPECT_EQ("4294967295",
            BoxedValue(new unsigned int(4294967295U)).ToString());  // NOLINT
}

TEST(UmBoxedValueTest, UnsignedInt64ToString) {
  // 18446744073709551615 is the biggest possible 64-bit unsigned integer.
  EXPECT_EQ("18446744073709551615",
            BoxedValue(new uint64_t(18446744073709551615ULL)).ToString());
}

TEST(UmBoxedValueTest, BoolToString) {
  EXPECT_EQ("false", BoxedValue(new bool(false)).ToString());
  EXPECT_EQ("true", BoxedValue(new bool(true)).ToString());
}

TEST(UmBoxedValueTest, DoubleToString) {
  EXPECT_EQ("1.501", BoxedValue(new double(1.501)).ToString());
}

TEST(UmBoxedValueTest, TimeToString) {
  // Tue Apr 29 22:30:55 UTC 2014 is 1398810655 seconds since the Unix Epoch.
  EXPECT_EQ("4/29/2014 22:30:55 GMT",
            BoxedValue(new Time(Time::FromTimeT(1398810655))).ToString());
}

TEST(UmBoxedValueTest, TimeDeltaToString) {
  // 12345 seconds is 3 hours, 25 minutes and 45 seconds.
  EXPECT_EQ(
      "3h25m45s",
      BoxedValue(new TimeDelta(TimeDelta::FromSeconds(12345))).ToString());
}

TEST(UmBoxedValueTest, ConnectionTypeToString) {
  EXPECT_EQ(
      "Disconnected",
      BoxedValue(new ConnectionType(ConnectionType::kDisconnected)).ToString());
  EXPECT_EQ(
      "ethernet",
      BoxedValue(new ConnectionType(ConnectionType::kEthernet)).ToString());
  EXPECT_EQ("wifi",
            BoxedValue(new ConnectionType(ConnectionType::kWifi)).ToString());
  EXPECT_EQ("wimax",
            BoxedValue(new ConnectionType(ConnectionType::kWimax)).ToString());
  EXPECT_EQ(
      "bluetooth",
      BoxedValue(new ConnectionType(ConnectionType::kBluetooth)).ToString());
  EXPECT_EQ(
      "cellular",
      BoxedValue(new ConnectionType(ConnectionType::kCellular)).ToString());
  EXPECT_EQ(
      "Unknown",
      BoxedValue(new ConnectionType(ConnectionType::kUnknown)).ToString());
}

TEST(UmBoxedValueTest, ConnectionTetheringToString) {
  EXPECT_EQ(
      "Not Detected",
      BoxedValue(new ConnectionTethering(ConnectionTethering::kNotDetected))
          .ToString());
  EXPECT_EQ("Suspected",
            BoxedValue(new ConnectionTethering(ConnectionTethering::kSuspected))
                .ToString());
  EXPECT_EQ("Confirmed",
            BoxedValue(new ConnectionTethering(ConnectionTethering::kConfirmed))
                .ToString());
  EXPECT_EQ("Unknown",
            BoxedValue(new ConnectionTethering(ConnectionTethering::kUnknown))
                .ToString());
}

TEST(UmBoxedValueTest, RollbackToTargetVersionToString) {
  EXPECT_EQ("Unspecified",
            BoxedValue(new RollbackToTargetVersion(
                           RollbackToTargetVersion::kUnspecified))
                .ToString());
  EXPECT_EQ("Disabled",
            BoxedValue(
                new RollbackToTargetVersion(RollbackToTargetVersion::kDisabled))
                .ToString());
  EXPECT_EQ("Rollback and powerwash",
            BoxedValue(new RollbackToTargetVersion(
                           RollbackToTargetVersion::kRollbackAndPowerwash))
                .ToString());
  EXPECT_EQ(
      "Rollback and restore if possible",
      BoxedValue(new RollbackToTargetVersion(
                     RollbackToTargetVersion::kRollbackAndRestoreIfPossible))
          .ToString());
  EXPECT_EQ(
      "Rollback only if restore is possible",
      BoxedValue(new RollbackToTargetVersion(
                     RollbackToTargetVersion::kRollbackOnlyIfRestorePossible))
          .ToString());
}

TEST(UmBoxedValueTest, SetConnectionTypeToString) {
  set<ConnectionType>* set1 = new set<ConnectionType>;
  set1->insert(ConnectionType::kWimax);
  set1->insert(ConnectionType::kEthernet);
  EXPECT_EQ("ethernet,wimax", BoxedValue(set1).ToString());

  set<ConnectionType>* set2 = new set<ConnectionType>;
  set2->insert(ConnectionType::kWifi);
  EXPECT_EQ("wifi", BoxedValue(set2).ToString());
}

TEST(UmBoxedValueTest, StageToString) {
  EXPECT_EQ("Idle", BoxedValue(new Stage(Stage::kIdle)).ToString());
  EXPECT_EQ("Checking For Update",
            BoxedValue(new Stage(Stage::kCheckingForUpdate)).ToString());
  EXPECT_EQ("Update Available",
            BoxedValue(new Stage(Stage::kUpdateAvailable)).ToString());
  EXPECT_EQ("Downloading",
            BoxedValue(new Stage(Stage::kDownloading)).ToString());
  EXPECT_EQ("Verifying", BoxedValue(new Stage(Stage::kVerifying)).ToString());
  EXPECT_EQ("Finalizing", BoxedValue(new Stage(Stage::kFinalizing)).ToString());
  EXPECT_EQ("Updated, Need Reboot",
            BoxedValue(new Stage(Stage::kUpdatedNeedReboot)).ToString());
  EXPECT_EQ("Reporting Error Event",
            BoxedValue(new Stage(Stage::kReportingErrorEvent)).ToString());
  EXPECT_EQ("Attempting Rollback",
            BoxedValue(new Stage(Stage::kAttemptingRollback)).ToString());
}

TEST(UmBoxedValueTest, DeleterMarkerToString) {
  bool marker = false;
  BoxedValue value = BoxedValue(new DeleterMarker(&marker));
  EXPECT_EQ("DeleterMarker:false", value.ToString());
  marker = true;
  EXPECT_EQ("DeleterMarker:true", value.ToString());
}

TEST(UmBoxedValueTest, UpdateRestrictionsToString) {
  EXPECT_EQ(
      "None",
      BoxedValue(new UpdateRestrictions(UpdateRestrictions::kNone)).ToString());
  EXPECT_EQ("Flags: RestrictDownloading",
            BoxedValue(new UpdateRestrictions(
                           UpdateRestrictions::kRestrictDownloading))
                .ToString());
}

TEST(UmBoxedValueTest, WeeklyTimeIntervalToString) {
  EXPECT_EQ("Start: day_of_week=2 time=100\nEnd: day_of_week=4 time=200",
            BoxedValue(new WeeklyTimeInterval(
                           WeeklyTime(2, TimeDelta::FromMinutes(100)),
                           WeeklyTime(4, TimeDelta::FromMinutes(200))))
                .ToString());
  EXPECT_EQ("Start: day_of_week=1 time=10\nEnd: day_of_week=1 time=20",
            BoxedValue(new WeeklyTimeInterval(
                           WeeklyTime(1, TimeDelta::FromMinutes(10)),
                           WeeklyTime(1, TimeDelta::FromMinutes(20))))
                .ToString());
}

TEST(UmBoxedValueTest, WeeklyTimeIntervalVectorToString) {
  WeeklyTimeIntervalVector intervals;
  intervals.emplace_back(WeeklyTime(5, TimeDelta::FromMinutes(10)),
                         WeeklyTime(1, TimeDelta::FromMinutes(30)));
  EXPECT_EQ(
      "Disallowed intervals:\nStart: day_of_week=5 time=10\nEnd: "
      "day_of_week=1 time=30\n",
      BoxedValue(new WeeklyTimeIntervalVector(intervals)).ToString());
  intervals.emplace_back(WeeklyTime(1, TimeDelta::FromMinutes(5)),
                         WeeklyTime(6, TimeDelta::FromMinutes(1000)));
  EXPECT_EQ(
      "Disallowed intervals:\nStart: day_of_week=5 time=10\nEnd: "
      "day_of_week=1 time=30\nStart: day_of_week=1 time=5\nEnd: day_of_week=6 "
      "time=1000\n",
      BoxedValue(new WeeklyTimeIntervalVector(intervals)).ToString());
}

}  // namespace chromeos_update_manager
