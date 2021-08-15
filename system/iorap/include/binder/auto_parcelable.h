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

#ifndef IORAP_BINDER_AUTO_PARCELABLE_H_
#define IORAP_BINDER_AUTO_PARCELABLE_H_

#include "binder/Parcelable.h"
#include "binder/Parcel.h"
#include "binder/Status.h"

#include "common/introspection.h"

namespace iorap {
namespace binder {

//
// Implements the android::Parcelable interface (readFromParcel, writeToParcel)
// automatically by using compile-time introspection on T.
//
// Requires that 'T' implements introspection by using the IORAP_INTROSPECT_ADAPT_STRUCT macro.
//
template <typename T>
struct AutoParcelable : public ::android::Parcelable {
 private:
  using Status = android::binder::Status;
  using Parcel = android::Parcel;
  using Parcelable = android::Parcelable;
  using status_t = android::status_t;

 public:
  // Write every (introspected) field to the parcel by automatically inferring the correct
  // write method to invoke on the parcel from the member type.
  status_t writeToParcel(Parcel* parcel) const override {
    if (parcel == nullptr) {
      return ::android::UNEXPECTED_NULL;
    }

    status_t result = android::NO_ERROR;
    ::iorap::introspect::for_each_member_field_value(*Self(), [&](auto&& value) {
      if (result == android::NO_ERROR) {
        result = writeAnyToParcel(/*inout*/parcel, value);
      }
    });

    return result;
  }

  // Read every (introspected) field to the parcel by automatically inferring the correct
  // read method to invoke on the parcel from the member type.
  //
  // Resilient to partial read failures: A return code other than NO_ERROR means that
  // the current value is left unmodified.
  status_t readFromParcel(const Parcel* parcel) override {
    if (parcel == nullptr) {
      return ::android::UNEXPECTED_NULL;
    }

    T tmp{*Self()};

    // Unpack all the parcelable data into a temporary copy.
    // Parceling could fail halfway through, in which case
    // the original object is unaffected.

    status_t result = android::NO_ERROR;
    ::iorap::introspect::for_each_member_field_set_value(tmp, [&](auto field_type) {
      // type<?> field_type

      using ValueT = typename decltype(field_type)::type;

      if (result == android::NO_ERROR) {
        auto&& [res, read_value] = readAnyFromParcel<ValueT>(/*inout*/parcel);
        result = res;
        return ::iorap::introspect::aliasing_forward<ValueT>(read_value);
      } else {
        // TODO: nice-to-have fold over members to early-out on failure.
        return ValueT{};
      }
    });

    if (result != android::NO_ERROR) {
      return result;
    }

    // Success! Now we can copy all the data in a single step.
    *Self() = std::move(tmp);

    // TODO: nice-to-have some kind of invariants-checking after reading the parcel data.

    return ::android::NO_ERROR;
  }

 private:
#define AUTO_PARCELABLE_BINDER_MAPPING(FN) \
FN(Byte,int8_t)\
FN(Int32,int32_t)\
FN(Uint32,uint32_t)\
FN(Int64,int64_t)\
FN(Uint64,uint64_t)\
FN(Float,float)\
FN(Double,double)\
FN(Bool,bool)\
FN(CString,const char*)\
FN(String16,const String16&)\
FN(String16,const std::unique_ptr<String16>&)\
FN(StrongBinder,const sp<IBinder>&)\
FN(WeakBinder,const wp<IBinder>&)\

  template <typename F>
  static status_t writeAnyToParcel(Parcel* parcel, const F& value) {
    using namespace android;  // NOLINT

    // 'F' is the original type of the field here, so it's safe to use it undecayed.
    // However, to make matching easier we almost always want to match against the decayed type.
    using D = std::decay_t<F>;  // [const] [volatile] X[&][&] -> X

    if constexpr (std::is_base_of_v<Parcelable, D>) {
      return value.writeToParcel(parcel);
    } else if constexpr (std::is_enum_v<D>) {
      return writeAnyToParcel(parcel, static_cast<std::underlying_type_t<F>>(value));
#define AUTO_PARCELABLE_WRITE_TO_PARCEL(fn_name, type_name) \
    } else if constexpr (std::is_same_v<D, std::decay_t<type_name>>) { \
      return parcel->write ## fn_name (value);
AUTO_PARCELABLE_BINDER_MAPPING(AUTO_PARCELABLE_WRITE_TO_PARCEL)
    } else if constexpr (std::is_same_v<D, std::string>) {
      return parcel->writeUtf8AsUtf16(value);
    } else {
      STATIC_FAIL(D, "Unsupported type: Add more manual type conversions above^^^");
    }

#undef AUTO_PARCELABLE_WRITE_TO_PARCEL
  }

  template <typename F>
  static auto readAnyFromParcel(const Parcel* parcel) {
    // returns pair(status_t, ~F~)
    using namespace android;

    // Since 'F' is almost always an lvalue reference (due to F=decltype(auto&&),
    // we should lose the references, and also any consts.
    using D = std::decay_t<F>;

    D value;
    status_t result;

    if constexpr (std::is_base_of_v<Parcelable, D>) {
      status_t result = value.readFromParcel(/*in*/parcel);
    } else if constexpr (std::is_enum_v<D>) {
      auto&& [res, val] = readAnyFromParcel<std::underlying_type_t<D>>(parcel);
      result = res;
      value = static_cast<D>(val);
#define AUTO_PARCELABLE_READ_FROM_PARCEL(fn_name, type_name) \
    } else if constexpr (std::is_same_v<D, std::decay_t<type_name>>) { \
      result = parcel->read ## fn_name (/*out*/&value);
AUTO_PARCELABLE_BINDER_MAPPING(AUTO_PARCELABLE_READ_FROM_PARCEL)
    } else if constexpr (std::is_same_v<D, std::string>) {
      result = parcel->readUtf8FromUtf16(/*out*/&value);
    } else {
      STATIC_FAIL(D, "Unsupported type: Add more manual type conversions above^^^");
    }
#undef AUTO_PARCELABLE_READ_FROM_PARCEL

    return std::make_pair(result, std::move(value));
  }

  T* Self() {
    return static_cast<T*>(this);
  }
  const T* Self() const {
    return static_cast<const T*>(this);
  }
};

}  // namespace binder
}  // namespace iorap

#endif  // IORAP_BINDER_AUTO_PARCELABLE_H_
