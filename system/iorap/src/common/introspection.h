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

#ifndef IORAP_COMMON_INTROSPECTION_H
#define IORAP_COMMON_INTROSPECTION_H

/*
 * Provide zero-cost compile-time introspection of struct member fields.
 *
 * Example:
 *
 * // Declaration
 * struct PackageEvent {
 *
 *   int type;
 *   std::string package_uri;
 *   std::string package_name;
 * };
 *
 * IORAP_INTROSPECT_ADAPT_STRUCT(PackageEvent, type, package_uri, package_name);
 *
 * // Usage
 * {
 *   std::stringstream str;
 *   for_each_member_field(PackageEvent{123,"hello","world"}, [&](auto&& val) {
 *     str << val << ",";
 *   }
 *   CHECK_EQ("123,hello,world,"s, str.str());
 * }
 */

#include "common/macros.h"
#include "common/type.h"

#include <tuple>

namespace iorap {
namespace introspect {

template <auto value>
struct member_type;

// Compile-time introspection data for a member-to-pointer.
//
// Example:
//   using package_uri_member_type = member_type<&PackageEvent::&package_uri>
//   int type = package_uri_member_type::value(PackageEvent{123,"hello","world"});
//   CHECK_EQ(type, 123);
template <typename T, typename F, F T::*member>
struct member_type<member> {
  // The type of the struct this field is located in, e.g. 'struct XYZ {...}' -> XYZ.
  static constexpr auto struct_t = type_c<T>;
  // The type of the field, e.g. 'struct XYZ { int x; }' -> int.
  static constexpr auto type = type_c<F>;

  // Allow a 'const U', 'volatile U', 'U&' etc here.
  // Returns the value inside of 'U'.
  template <typename U>
  static constexpr decltype(auto) value(U&& v) {
    static_assert(std::is_same_v<T, std::decay_t<U>>, "U must be cvref of T");

    using U_noref = std::remove_reference_t<U>;

    // This casts from the regular non-const pointer-to-member to a potentially const/volatile
    // pointer-to-member.
    F U_noref::*safer_member = member;

    // Now dereference it,
    return v.*safer_member;
    // TODO: are we properly returning && for rvalue, & for lvalue refs, etc?
  }

  static constexpr void set_value(typename decltype(struct_t)::type& s,
                                  typename decltype(type)::type&& value) {
    s.*member = std::forward<typename decltype(type)::type>(value);
  }
};

// Given a self : T, where T has introspection-enabled support, T has some
// members m1, m2, m3, ... , mN.
//
// Invokes fun(self.*m1); fun(self.*m2); fun(self.*m3); ... ; fun(self.*mN).
template <typename T, typename F>
static constexpr void for_each_member_field_value(T&& self, F&& fun) {
  constexpr auto members = introspect_members(type_c<std::decay_t<T>>);
  // std::tuple<member_type<A>, member_type<B>, ...>

  // Warning: Don't use 'v=std::forward<V>(v)' as that actually captures-by-value.
  for_each(members, [&fun, &self](auto&& type) mutable {
    // Note that 'type' is a member_type
    fun(type.value(std::forward<T>(self)));
  });
}

// Given a self : T, where T has introspection-enabled support, T has some
// members m1, m2, m3, ... , mN. The basic_type of each member is t1, t2, t3, ..., tN.
//
// Invokes
//   self.*m1 = fun(self, t1);
//   self.*m2 = fun(self, t2);
//   self.*m3 = fun(self, t3);
//   ...;
//   self.*mN = fun(self, tN).
template <typename T, typename F>
static constexpr void for_each_member_field_set_value(T&& self, F&& fun) {
  constexpr auto members = introspect_members(type_c<std::decay_t<T>>);
  // std::tuple<member_type<A>, member_type<B>, ...>

  // Warning: Don't use 'v=std::forward<V>(v)' as that actually captures-by-value.
  for_each(members, [&fun, &self](auto&& type) mutable {
    // Note that 'type' is a member_type
    type.set_value(std::forward<T>(self), fun(type.type));
  });
}

}
}

// Add compile-time introspection capabilities to a pre-existing struct or class.
//
// Arguments: Name, [Member1, Member2, ... MemberN]
//
// Example:
//
//   struct Rectangle {
//     int height;
//     int width;
//   };
//
//   IORAP_INTROSPECT_ADAPT_STRUCT(Rectangle, height, width);
//
// See also for_each_member_field_value.
#define IORAP_INTROSPECT_ADAPT_STRUCT(/*name, [member1, member2, member3, ...]*/...) \
  IORAP_INTROSPECT_ADAPT_STRUCT_IMPL(IORAP_PP_NARG(__VA_ARGS__), __VA_ARGS__)

#define IORAP_INTROSPECT_ADAPT_STRUCT_IMPL(N, ...) \
  IORAP_PP_CONCAT(IORAP_INTROSPECT_ADAPT_STRUCT_IMPL_, N)(__VA_ARGS__)

// This simple implementation relies on the 'introspect_members' function being overloaded
// for the type<T> values. ADL is then applied to resolve the exact overload for any T,
// thus allowing this function definition to be in any namespace.

// The auto signature must conform to:
//   introspect_members(type<T>) -> std::tuple<member_type1, member_type_2, ...>

// TODO: it would be nice to capture the name of the member as a string literal.
#define IORAP_INTROSPECT_ADAPT_STRUCT_IMPL_1(TYPE) \
  static constexpr auto introspect_members(::iorap::introspect::type<TYPE>) { \
    return std::make_tuple();\
  }
#define IORAP_INTROSPECT_ADAPT_STRUCT_IMPL_2(TYPE, m1) \
  static constexpr auto introspect_members(::iorap::introspect::type<TYPE>) { \
    return std::make_tuple(::iorap::introspect::member_type<&TYPE::m1>{}\
    );\
  }

#define IORAP_INTROSPECT_ADAPT_STRUCT_IMPL_3(TYPE, m1, m2) \
  static constexpr auto introspect_members(::iorap::introspect::type<TYPE>) { \
    return std::make_tuple(::iorap::introspect::member_type<&TYPE::m1>{},\
                           ::iorap::introspect::member_type<&TYPE::m2>{}\
    ); \
  }

#define IORAP_INTROSPECT_ADAPT_STRUCT_IMPL_4(TYPE, m1, m2, m3) \
  static constexpr auto introspect_members(::iorap::introspect::type<TYPE>) { \
    return std::make_tuple(::iorap::introspect::member_type<&TYPE::m1>{},\
                           ::iorap::introspect::member_type<&TYPE::m2>{},\
                           ::iorap::introspect::member_type<&TYPE::m3>{}\
    ); \
  }

// TODO: Consider using IORAP_PP_MAP


#endif  // IORAP_COMMON_INTROSPECTION_H