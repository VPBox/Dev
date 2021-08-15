/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <binder/app_launch_event.h>

#include <gtest/gtest.h>

// TODO: move to app_launch_event.h
#include <google/protobuf/util/message_differencer.h>

using namespace iorap::binder;  // NOLINT

using android::Parcel;

using Type = AppLaunchEvent::Type;
using Temperature = AppLaunchEvent::Temperature;

namespace iorap::binder {

inline bool ProtosEqual(const ::google::protobuf::Message& lhs,
                        const ::google::protobuf::Message& rhs) {
  return ::google::protobuf::util::MessageDifferencer::Equals(lhs, rhs);
}

inline bool ProtosEqual(const ::google::protobuf::MessageLite& lhs,
                        const ::google::protobuf::MessageLite& rhs) {
  // MessageLite does not support 'MessageDifferencer' which requires protobuf-full
  // because it uses reflection.
  //
  // Serialize as a string and compare. This may lead to false inequality when protobufs
  // are actually the same but their encodings are slightly different.
  return lhs.GetTypeName() == rhs.GetTypeName()
      && lhs.SerializeAsString() == rhs.SerializeAsString();
}

template <typename T>
inline bool ProtoPointersEqual(const T& lhs_ptr, const T& rhs_ptr) {
  if (lhs_ptr == nullptr && rhs_ptr == nullptr) {
    return true;
  }
  else if (lhs_ptr != nullptr && rhs_ptr != nullptr) {
    return ProtosEqual(*lhs_ptr, *rhs_ptr);
  }
  return false;
}

// Field-by-field equality.
// Protos are compared according by checking that their serialized encodings are the same.
inline bool operator==(const AppLaunchEvent& lhs, const AppLaunchEvent& rhs) {
# define EQ_OR_RETURN(l, r, val) if (!(l.val == r.val)) { return false; }
# define PROTO_EQ_OR_RETURN(l, r, val) if (!ProtoPointersEqual(l.val, r.val)) { return false; }

  EQ_OR_RETURN(lhs, rhs, type);
  EQ_OR_RETURN(lhs, rhs, sequence_id);
  PROTO_EQ_OR_RETURN(lhs, rhs, intent_proto);
  EQ_OR_RETURN(lhs, rhs, temperature);
  PROTO_EQ_OR_RETURN(lhs, rhs, activity_record_proto);

# undef EQ_OR_RETURN
# undef PROTO_EQ_OR_RETURN

  return true;
}

inline bool operator!=(const AppLaunchEvent& lhs, const AppLaunchEvent& rhs) {
  return !(lhs == rhs);
}

static AppLaunchEvent MakeIntentStarted(int64_t sequence_id,
                                            // non-null
                                            std::unique_ptr<IntentProto> intent_proto) {
  DCHECK(intent_proto != nullptr);

  AppLaunchEvent e{Type::kIntentStarted, sequence_id, std::move(intent_proto)};
  return e;
}

static AppLaunchEvent MakeIntentFailed(int64_t sequence_id) {
  AppLaunchEvent e{Type::kIntentFailed, sequence_id};
  return e;
}

static AppLaunchEvent
MakeActivityLaunched(int64_t sequence_id,
                     Temperature temperature,
                     // non-null
                     std::unique_ptr<ActivityRecordProto> activity_record_proto) {
  DCHECK(activity_record_proto != nullptr);

  AppLaunchEvent e{Type::kActivityLaunched,
                   sequence_id,
                   /*intent_proto*/nullptr,
                   temperature,
                   std::move(activity_record_proto)};
  return e;
}

static AppLaunchEvent
MakeActivityLaunchCancelled(int64_t sequence_id,
                            // nullable
                            std::unique_ptr<ActivityRecordProto> activity_record_proto = nullptr) {
  AppLaunchEvent e{Type::kActivityLaunchCancelled,
                   sequence_id,
                   /*intent_proto*/nullptr,
                   Temperature::kUninitialized,
                   std::move(activity_record_proto)};
  return e;
}

static AppLaunchEvent
MakeActivityLaunchFinished(int64_t sequence_id,
                           // non-null
                           std::unique_ptr<ActivityRecordProto> activity_record_proto) {
  DCHECK(activity_record_proto != nullptr);
  AppLaunchEvent e{Type::kActivityLaunchFinished,
                   sequence_id,
                   /*intent_proto*/nullptr,
                   Temperature::kUninitialized,
                   std::move(activity_record_proto)};
  return e;
}

}  // namespace iorap::binder

auto MakeDummyIntent(std::string action = "package_name/.ClassName") {
  std::unique_ptr<IntentProto> dummy_intent{new IntentProto{}};
  dummy_intent->set_action(action);
  return dummy_intent;
}

auto MakeDummyActivityRecord(std::string title = "package_name/.ClassName") {
  std::unique_ptr<ActivityRecordProto> dummy{new ActivityRecordProto{}};

  dummy->mutable_identifier()->set_title(title);

  return dummy;
}

TEST(AppLaunchEventTest, Equals) {
  EXPECT_EQ(MakeIntentStarted(456, MakeDummyIntent()), MakeIntentStarted(456, MakeDummyIntent()));
  EXPECT_NE(MakeIntentStarted(45, MakeDummyIntent()), MakeIntentStarted(45, MakeDummyIntent("a")));

  EXPECT_EQ(MakeIntentFailed(123), MakeIntentFailed(123));
  EXPECT_NE(MakeIntentFailed(0), MakeIntentFailed(123));

  EXPECT_EQ((MakeActivityLaunched(3, Temperature::kHot, MakeDummyActivityRecord())),
            (MakeActivityLaunched(3, Temperature::kHot, MakeDummyActivityRecord())));
  EXPECT_NE((MakeActivityLaunched(3, Temperature::kHot, MakeDummyActivityRecord())),
            (MakeActivityLaunched(3, Temperature::kCold, MakeDummyActivityRecord())));
  EXPECT_NE((MakeActivityLaunched(3, Temperature::kHot, MakeDummyActivityRecord())),
            (MakeActivityLaunched(3, Temperature::kHot, MakeDummyActivityRecord("other title"))));

  EXPECT_EQ((MakeActivityLaunchCancelled(4)),
            (MakeActivityLaunchCancelled(4)));
  EXPECT_EQ((MakeActivityLaunchCancelled(4, MakeDummyActivityRecord())),
            (MakeActivityLaunchCancelled(4, MakeDummyActivityRecord())));
  EXPECT_NE((MakeActivityLaunchCancelled(4, MakeDummyActivityRecord())),
            (MakeActivityLaunchCancelled(4, MakeDummyActivityRecord("other"))));
  EXPECT_NE((MakeActivityLaunchCancelled(4, MakeDummyActivityRecord())),
            (MakeActivityLaunchCancelled(4)));

  EXPECT_EQ((MakeActivityLaunchFinished(5, MakeDummyActivityRecord())),
            (MakeActivityLaunchFinished(5, MakeDummyActivityRecord())));
  EXPECT_NE((MakeActivityLaunchFinished(5, MakeDummyActivityRecord())),
            (MakeActivityLaunchFinished(5, MakeDummyActivityRecord("other title"))));
}

template <typename T>
T ValueParcelRoundTrip(const T& value) {
  ::android::Parcel p;
  CHECK_EQ(value.writeToParcel(&p), ::android::NO_ERROR);

  T new_value;
  p.setDataPosition(0);
  CHECK_EQ(new_value.readFromParcel(&p), ::android::NO_ERROR);

  return new_value;
}

#define EXPECT_PARCELING_ROUND_TRIP(a) EXPECT_EQ((a), ValueParcelRoundTrip((a)))

TEST(AppLaunchEventTest, ParcelingRoundTrip) {
  EXPECT_PARCELING_ROUND_TRIP(MakeIntentStarted(456, MakeDummyIntent()));
  EXPECT_PARCELING_ROUND_TRIP(MakeIntentFailed(123));
  EXPECT_PARCELING_ROUND_TRIP(MakeActivityLaunched(3, Temperature::kHot, MakeDummyActivityRecord()));
  EXPECT_PARCELING_ROUND_TRIP(MakeActivityLaunchCancelled(4));
  EXPECT_PARCELING_ROUND_TRIP(MakeActivityLaunchCancelled(4, MakeDummyActivityRecord()));
  EXPECT_PARCELING_ROUND_TRIP(MakeActivityLaunchFinished(5, MakeDummyActivityRecord()));
}
