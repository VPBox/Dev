/*
 * Copyright (C) 2016 The Android Open Source Project
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

// Local utilities (macros and free-standing functions).

#ifndef LOCAL_UTILS_H_
#define LOCAL_UTILS_H_

#include <limits>
#include <type_traits>

#include "android-base/logging.h"

// Converts the value SRC to a value of DST_TYPE, in the range of [MIN, MAX].
// Values less than MIN are clamped to MIN, and values greater than MAX are
// clamped to MAX. Conversions are safe in the sense that the range is checked
// to be valid for both SRC and DST_TYPE, at compile-time.
//
// As compared to static_cast<>, SAFELY_CLAMP is a) more explicit, b) more
// flexible, and c) less prone to surprising conversions (e.g. -1 becoming
// UINT_MAX).
#define SAFELY_CLAMP(SRC, DST_TYPE, MIN, MAX)                                \
  local_utils::internal::SafelyClamp<decltype(SRC), DST_TYPE, MIN, MAX, MIN, \
                                     MAX>(SRC)

// While attributes are standard in C++11, these attributes are not part of
// the standard. We use macros to abstract these attributes, to allow
// the code to compile with compilers that don't recognize these attributes.
#if defined(__clang__)
#define NONNULL [[gnu::nonnull]]                 /* NOLINT(whitespace/braces) */
#define RETURNS_NONNULL [[gnu::returns_nonnull]] /* NOLINT ... */
#else
#define NONNULL
#define RETURNS_NONNULL
#endif

namespace android {
namespace wifilogd {
namespace local_utils {

// Returns the value in |enum_value|, as the integral type underlying the
// enum. (E.g. uint8_t, int32_t, etc.)
template <typename T>
constexpr auto CastEnumToInteger(T enum_value) {
  static_assert(std::is_enum<T>::value, "argument must be of an enum type");
  return static_cast<typename std::underlying_type<T>::type>(enum_value);
}

// Copies a |T| out of |buf|, aborting if |buf| is too short to hold a |T|.
//
// As compared to accessing the underlying data using reinterpret_cast<>,
// CopyFromBufferOrDie() provides three benefits:
// 1. Guarantees that the returned header is properly aligned. While
//    many processors support some unaligned reads, there are some
//    exceptions. E.g, a 64-bit unaligned read on 32-bit ARM may cause
//    a program to abort.
// 2. Removes the potential for bugs due to compiler optimizations based
//    on type-based alias analysis. (These are the kinds of bugs that
//    "strict-aliasing" warnings try to call out.)
// 3. Verifies that the source buffer is large enough to contain the
//    data we're trying to read out.
template <typename T>
T CopyFromBufferOrDie(NONNULL const void* buf, size_t buf_len) {
  static_assert(std::is_trivially_copyable<T>::value,
                "CopyFromBufferOrDie can only copy trivially copyable types");
  T out;
  CHECK(buf_len >= sizeof(out));
  std::memcpy(&out, buf, sizeof(out));
  return out;
}

// Returns the maximal value representable by T. Generates a compile-time
// error if T is not an integral type.
template <typename T>
constexpr T GetMaxVal() {
  // Give a useful error for non-numeric types, and avoid returning zero for
  // pointers and C-style enums (http://stackoverflow.com/a/9201960).
  static_assert(std::is_integral<T>::value,
                "GetMaxVal requires an integral type");
  return std::numeric_limits<T>::max();
}

// Returns the maximal value representable by |t_instance|. Generates a
// compile-time error if |t_instance| is not an instance of an integral type.
template <typename T>
constexpr T GetMaxVal(const T& /* t_instance */) {
  return GetMaxVal<T>();
}

// Returns true if |c| is a printable character, for ASCII data.
inline bool IsAsciiPrintable(uint8_t c) {
  return (c == '\t' || c == '\n' || (c >= ' ' && c <= '~'));
}

namespace internal {

// Implements the functionality documented for the SAFELY_CLAMP macro.
// This function should be used via the SAFELY_CLAMP macro.
template <typename SrcType, typename DstType, SrcType MinAsSrcType,
          SrcType MaxAsSrcType, DstType MinAsDstType, DstType MaxAsDstType>
DstType SafelyClamp(SrcType input) {
  static_assert(std::is_integral<SrcType>::value,
                "source type must be integral");
  static_assert(std::is_integral<DstType>::value,
                "destination type must be integral");
  static_assert(MinAsSrcType < MaxAsSrcType, "invalid source range");
  static_assert(MinAsDstType < MaxAsDstType, "invalid destination range");
  // Clients should use the SAFELY_CLAMP macro, in which case this should never
  // happen. (When the SAFELY_CLAMP macro is used, the values can only be
  // unequal if there was a narrowing conversion. But, in that case, the value
  // should have failed to match the template, since narrowing-conversions are
  // not allowed for non-type template arguments.
  // http://stackoverflow.com/a/24346350)
  //
  // Anyway, these checks provide a fail-safe, in case clients use the template
  // function directly, and pass in inconsistent values for the range
  // definition.
  static_assert(MinAsSrcType == MinAsDstType, "inconsistent range min");
  static_assert(MaxAsSrcType == MaxAsDstType, "inconsistent range max");

  if (input < MinAsSrcType) {
    return MinAsDstType;
  } else if (input > MaxAsSrcType) {
    return MaxAsDstType;
  } else {
    // - Given that the template has matched, we know that MinAsSrcType,
    //   MaxAsSrcType, MinAsDstType, and MaxAsDstType are valid for their
    //   respective types. (See narrowing-conversion comment above.)
    // - Given the static_assert()s above, we know that a) the ranges are
    //   well-formed, and that the b) source range is identical to the
    //   destination range.
    // - Given the range checks above, we know that |input| is within the range.
    //
    // Hence, the value to be returned must be valid for DstType, and the
    // expression below has the same value as |input|.
    return static_cast<DstType>(input);
  }
}

}  // namespace internal

}  // namespace local_utils
}  // namespace wifilogd
}  // namespace android

#endif  // LOCAL_UTILS_H_
