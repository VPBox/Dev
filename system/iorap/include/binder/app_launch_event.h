/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef IORAP_BINDER_APP_LAUNCH_EVENT_H_
#define IORAP_BINDER_APP_LAUNCH_EVENT_H_

#include "binder/common.h"
#include "common/introspection.h"
#include "common/expected.h"

#include <binder/Parcel.h>
#include <binder/Parcelable.h>
#include <frameworks/base/core/proto/android/content/intent.pb.h>  // IntentProto
#include <frameworks/base/core/proto/android/server/activitymanagerservice.pb.h>  // ActivityRecord

namespace iorap {
namespace binder {

// These protos are part of the iorapd binder ABI, alias them for easier usage.
using IntentProto = ::android::content::IntentProto;
using ActivityRecordProto = ::com::android::server::am::ActivityRecordProto;

struct AppLaunchEvent : public ::android::Parcelable {
  // Index position matters: Keep up-to-date with AppLaunchEvent.java sTypes field.
  enum class Type : int32_t {
    kUninitialized = -1,
    kIntentStarted = 0,
    kIntentFailed = 1,
    kActivityLaunched = 2,
    kActivityLaunchFinished = 3,
    kActivityLaunchCancelled = 4,
  };

  enum class Temperature : int32_t {
    kUninitialized = -1,
    kCold = 1,
    kWarm = 2,
    kHot = 3,
  };

  Type type{Type::kUninitialized};
  int64_t sequence_id{-1};
  // kIntentStarted only.
  std::unique_ptr<IntentProto> intent_proto;
  // kActivityLaunched only.
  Temperature temperature{Temperature::kUninitialized};
  // kActivityLaunch*. Can be null in kActivityLaunchCancelled.
  std::unique_ptr<ActivityRecordProto> activity_record_proto;

  AppLaunchEvent() = default;
  AppLaunchEvent(Type type,
                 int64_t sequence_id,
                 std::unique_ptr<IntentProto> intent_proto = nullptr,
                 Temperature temperature = Temperature::kUninitialized,
                 std::unique_ptr<ActivityRecordProto> activity_record_proto = nullptr)
    : type(type),
      sequence_id(sequence_id),
      intent_proto(std::move(intent_proto)),
      temperature(temperature),
      activity_record_proto(std::move(activity_record_proto)) {
  }

  ::android::status_t readFromParcel(const android::Parcel* parcel) override {

#   define PARCEL_READ_OR_RETURN(function, ...) \
    if (::android::status_t res = function(__VA_ARGS__); res != ::android::NO_ERROR) { \
      LOG(ERROR) << "AppLaunchEvent::readFromParcel failed"; \
      return res; \
    }

    int32_t type_int;
    PARCEL_READ_OR_RETURN(parcel->readInt32, &type_int);
    type = static_cast<Type>(type_int);

    LOG(VERBOSE) << "AppLaunchEvent::readFromParcel (type=" << type_int << ")";

    PARCEL_READ_OR_RETURN(parcel->readInt64, &sequence_id);

    switch (type) {
      case Type::kIntentStarted:
        PARCEL_READ_OR_RETURN(readIntent, parcel);
        break;
      case Type::kIntentFailed:
        // No extra arguments.
        break;
      case Type::kActivityLaunched: {
        PARCEL_READ_OR_RETURN(readActivityRecordProto, parcel);
        int32_t temperature_int;
        PARCEL_READ_OR_RETURN(parcel->readInt32, &temperature_int);
        temperature = static_cast<Temperature>(temperature_int);
        break;
      }
      case Type::kActivityLaunchFinished:
        PARCEL_READ_OR_RETURN(readActivityRecordProto, parcel);
        break;
      case Type::kActivityLaunchCancelled:
        PARCEL_READ_OR_RETURN(readActivityRecordProtoNullable, parcel);
        break;
      default:
        return android::BAD_VALUE;
    }
#   undef PARCEL_READ_OR_RETURN

    return ::android::NO_ERROR;

    // TODO: std::variant + protobuf implementation in AutoParcelable.
  }

#define PARCEL_WRITE_OR_RETURN(function, ...) \
  if (::android::status_t res = function(__VA_ARGS__); res != ::android::NO_ERROR) { \
    return res; \
  }

  ::android::status_t writeToParcel(android::Parcel* parcel) const override {
    PARCEL_WRITE_OR_RETURN(parcel->writeInt32, static_cast<int32_t>(type));
    PARCEL_WRITE_OR_RETURN(parcel->writeInt64, sequence_id);

    switch (type) {
      case Type::kIntentStarted:
        PARCEL_WRITE_OR_RETURN(writeIntent, parcel);
        break;
      case Type::kIntentFailed:
        // No extra arguments.
        break;
      case Type::kActivityLaunched:
        PARCEL_WRITE_OR_RETURN(writeActivityRecordProto, parcel);
        PARCEL_WRITE_OR_RETURN(parcel->writeInt32, static_cast<int32_t>(temperature));
        break;
      case Type::kActivityLaunchFinished:
        PARCEL_WRITE_OR_RETURN(writeActivityRecordProto, parcel);
        break;
      case Type::kActivityLaunchCancelled:
        PARCEL_WRITE_OR_RETURN(writeActivityRecordProtoNullable, parcel);
        break;
      default:
        DCHECK(false) << "attempted to write an uninitialized AppLaunchEvent to Parcel";
        return android::BAD_VALUE;
    }

#undef PARCEL_WRITE_OR_RETURN

    return android::NO_ERROR;
  }

 private:
  // Using 'unique_ptr' here because protobufs don't have a move constructor. Is there
  // a better way that is cheap to pass them around?
  template <typename T>
  static expected<std::unique_ptr<T>, ::android::status_t>
  ReadProto(const android::Parcel* parcel) {
    DCHECK(parcel != nullptr);

    ::android::status_t res;

    std::vector<uint8_t> byte_vector;
    if ((res = parcel->readByteVector(/*out*/&byte_vector)) != ::android::NO_ERROR) {
      return unexpected(res);
    }
    // TODO: we may want to do this without an extra copy, by parsing
    // the protobuf directly out of the parcel.

    const uint8_t* data = byte_vector.data();
    const size_t size = byte_vector.size();

    std::unique_ptr<T> proto_ptr{new T{}};

    if (!proto_ptr) {
      return unexpected(::android::NO_MEMORY);
    }

    if (!proto_ptr->ParseFromArray(data, size)) {
      return unexpected(::android::BAD_VALUE);
    }

    return proto_ptr;
  }

  template <typename T>
  static expected<std::unique_ptr<T>, ::android::status_t>
  ReadNullableProto(const android::Parcel* parcel) {
    DCHECK(parcel != nullptr);

    bool value;

    ::android::status_t res;
    res = parcel->readBool(/*out*/&value);

    if (res != ::android::NO_ERROR) {
      return unexpected(res);
    }

    if (!value) {
      return std::unique_ptr<T>{nullptr};
    }

    return ReadProto<T>(parcel);
  }

  template <typename T>
  static ::android::status_t
  WriteProto(android::Parcel* parcel, const std::unique_ptr<T>& proto) {
    DCHECK(parcel != nullptr);
    DCHECK(proto != nullptr);

    std::vector<uint8_t> byte_vector;
    {
      const int serialized_size = proto->ByteSize();
      byte_vector.resize(serialized_size);
      if (!proto->SerializeToArray(byte_vector.data(), serialized_size)) {
        return ::android::BAD_VALUE;
      }
    }

    ::android::status_t res;
    if ((res = parcel->writeByteVector(/*in*/byte_vector)) != ::android::NO_ERROR) {
      return res;
    }

    return ::android::NO_ERROR;
  }

  template <typename T>
  static ::android::status_t
  WriteNullableProto(android::Parcel* parcel, const std::unique_ptr<T>& maybe_proto) {
    bool value = (maybe_proto != nullptr);

    ::android::status_t res;
    res = parcel->writeBool(value);

    if (res != ::android::NO_ERROR) {
      return res;
    }

    if (!value) {
      return ::android::NO_ERROR;
    }

    return WriteProto<T>(parcel, maybe_proto);
  }

  android::status_t readIntent(const android::Parcel* parcel) {
    expected<std::unique_ptr<IntentProto>, ::android::status_t> maybe_intent =
        ReadProto<IntentProto>(parcel);

    if (maybe_intent) {
      intent_proto = std::move(maybe_intent.value());
      return ::android::NO_ERROR;
    } else {
      return maybe_intent.error();
    }
  }

  android::status_t readActivityRecordProto(const android::Parcel* parcel) {
    expected<std::unique_ptr<ActivityRecordProto>, ::android::status_t> maybe_record =
        ReadProto<ActivityRecordProto>(parcel);

    if (maybe_record) {
      activity_record_proto = std::move(maybe_record.value());
      return ::android::NO_ERROR;
    } else {
      return maybe_record.error();
    }
  }

  android::status_t readActivityRecordProtoNullable(const android::Parcel* parcel) {
    expected<std::unique_ptr<ActivityRecordProto>, ::android::status_t> maybe_record =
        ReadNullableProto<ActivityRecordProto>(parcel);

    if (maybe_record) {
      activity_record_proto = std::move(maybe_record.value());
      return ::android::NO_ERROR;
    } else {
      return maybe_record.error();
    }
  }

  android::status_t writeIntent(android::Parcel* parcel) const {
    return WriteProto<IntentProto>(parcel, intent_proto);
  }

  android::status_t writeActivityRecordProto(android::Parcel* parcel) const {
    return WriteProto<ActivityRecordProto>(parcel, activity_record_proto);
  }

  android::status_t writeActivityRecordProtoNullable(android::Parcel* parcel) const {
    return WriteNullableProto<ActivityRecordProto>(parcel, activity_record_proto);
  }
};

inline std::ostream& operator<<(std::ostream& os, const AppLaunchEvent::Type& type) {
  switch (type) {
    case AppLaunchEvent::Type::kUninitialized:
      os << "kUninitialized";
      break;
    case AppLaunchEvent::Type::kIntentStarted:
      os << "kIntentStarted";
      break;
    case AppLaunchEvent::Type::kIntentFailed:
      os << "kIntentFailed";
      break;
    case AppLaunchEvent::Type::kActivityLaunched:
      os << "kActivityLaunched";
      break;
    case AppLaunchEvent::Type::kActivityLaunchCancelled:
      os << "kActivityLaunchCancelled";
      break;
    case AppLaunchEvent::Type::kActivityLaunchFinished:
      os << "kActivityLaunchFinished";
      break;
    default:
      os << "(unknown)";
  }
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const AppLaunchEvent::Temperature& type) {
  switch (type) {
    case AppLaunchEvent::Temperature::kUninitialized:
      os << "kUninitialized";
      break;
    case AppLaunchEvent::Temperature::kCold:
      os << "kCold";
      break;
    case AppLaunchEvent::Temperature::kWarm:
      os << "kWarm";
      break;
    case AppLaunchEvent::Temperature::kHot:
      os << "kHot";
      break;
    default:
      os << "(unknown)";
  }
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const AppLaunchEvent& e) {
  os << "AppLaunchEvent{";
  os << "type=" << e.type << ",";
  os << "sequence_id=" << e.sequence_id << ",";

  os << "intent_proto=";
  if (e.intent_proto == nullptr) {
    os << "(nullptr)";
  } else {
    os << "(action=" << e.intent_proto->action() << ",";
    os << "component=";
    if (e.intent_proto->has_component()) {
      // $package/$class_name
      os << e.intent_proto->component().package_name() << "/"
         << e.intent_proto->component().class_name();
    } else {
      os << "(no component)";
    }
    os << ")";
  }
  os << ",";

  os << "temperature=" << e.temperature << ",";
  os << ",";

  os << "activity_record_proto=";
  if (e.activity_record_proto == nullptr) {
    os << "(nullptr)";
  } else {
    // title or component name.
    os << "'" << e.activity_record_proto->identifier().title() << "'";
  }
  os << "}";

  return os;
}

/*
IORAP_INTROSPECT_ADAPT_STRUCT(AppLaunchEvent,
                              type,
                              sequence_id,
                              intent_proto,
                              temperature,
                              activity_record_proto);
*/

}  // namespace binder
}  // namespace iorap

IORAP_JAVA_NAMESPACE_BINDER_TYPEDEF(AppLaunchEvent)

#endif  // IORAP_BINDER_APP_LAUNCH_EVENT_H_
