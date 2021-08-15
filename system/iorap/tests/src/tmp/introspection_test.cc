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

#include <common/introspection.h>

#include <ostream>
#include <sstream>
#include <string>

#include <gtest/gtest.h>

struct TestStructXyz {
  int x;
  double y;
  char z;
};

IORAP_INTROSPECT_ADAPT_STRUCT(TestStructXyz, x, y, z);

namespace iorap {
namespace introspect {

TEST(Introspection, ReadValues) {
  TestStructXyz xyz = {1,2.1,'x'};

  std::stringstream ss;

  for_each_member_field_value(xyz, [&](auto&& value) {
    ss << value << ",";
  });

  EXPECT_EQ(std::string("1,2.1,x,"), ss.str());
}

template <typename TestType, typename TargetType>
constexpr bool is_same_after_decay() {
  return std::is_same_v<TestType, std::decay_t<TargetType>>;
}
#define IS_SAME_AFTER_DECAY(test, target_variable) \
  is_same_after_decay<test, decltype(target_variable)>()

template <typename TestType, typename TargetType>
constexpr bool is_type_same_after_decay(basic_type<TargetType>) {
  return std::is_same_v<TestType, std::decay_t<TargetType>>;
}

#define IS_TYPE_SAME_AFTER_DECAY(test, target_type_variable) \
  is_type_same_after_decay<test>(CONSTEXPRIFY_TYPE(target_type_variable))

#define CONSTEXPRIFY_TYPE(type_var) \
  decltype(type_var){}

TEST(Introspection, ForEachmemberFieldSetValues) {
  TestStructXyz xyz{};
  TestStructXyz xyz_expected = {1,2.1,'x'};

  std::stringstream ss;

  for_each_member_field_set_value(xyz, [&ss](auto&& value) {
    // This is really confusing, value is type<?>.
    // It should probably be merely the old value.
    //
    // The way the functions works now is more like an inplace_map.

    if constexpr (IS_TYPE_SAME_AFTER_DECAY(int, value)) {
      // value = 1;
      ss << "int,";
      return 1;
    } else if constexpr (IS_TYPE_SAME_AFTER_DECAY(double, value)) {
      // value = 2.1;
      ss << "double,";
      return 2.1;
    } else if constexpr (IS_TYPE_SAME_AFTER_DECAY(char, value)) {
      // value = 'x';
      ss << "char,";
      return 'x';
    } else {
      STATIC_FAIL_DT(value, "Unhandled type");
    }
  });

  EXPECT_EQ(std::string("int,double,char,"), ss.str());

  EXPECT_EQ(xyz_expected.x, xyz.x);
  EXPECT_EQ(xyz_expected.y, xyz.y);
  EXPECT_EQ(xyz_expected.z, xyz.z);
}

TEST(Introspection, MemberFieldSetValue) {
  TestStructXyz xyz{};
  TestStructXyz xyz_expected = {1,2.1,'x'};

  std::stringstream ss;

  auto&& [member_x, member_y, member_z]  = introspect_members(type_c<TestStructXyz>);
  member_x.set_value(xyz, 1);
  member_y.set_value(xyz, 2.1);
  member_z.set_value(xyz, 'x');

  EXPECT_EQ(xyz_expected.x, xyz.x);
  EXPECT_EQ(xyz_expected.y, xyz.y);
  EXPECT_EQ(xyz_expected.z, xyz.z);
}

template <typename M, typename T, typename V>
constexpr void call_set_value(M member_type, T&& self, V&& value) {
  member_type.set_value(std::forward<T>(self), std::forward<V>(value));
}

TEST(Introspection, MemberFieldSetValueIndirect) {
  TestStructXyz xyz{};
  TestStructXyz xyz_expected = {1,2.1,'x'};

  std::stringstream ss;

  auto&& [member_x, member_y, member_z]  = introspect_members(type_c<TestStructXyz>);
  call_set_value(member_x, xyz, 1);
  call_set_value(member_y, xyz, 2.1);
  call_set_value(member_z, xyz, 'x');

  EXPECT_EQ(xyz_expected.x, xyz.x);
  EXPECT_EQ(xyz_expected.y, xyz.y);
  EXPECT_EQ(xyz_expected.z, xyz.z);
}

template <typename M, typename T, typename V>
constexpr void call_set_value_lambda(M member_type, T&& self, V&& value) {
  ([member_type, &self](auto&& value) mutable {
    member_type.set_value(std::forward<T>(self), std::forward<V>(value));
  })(std::forward<V>(value));
}

TEST(Introspection, MemberFieldSetValueIndirectLambda) {
  TestStructXyz xyz{};
  TestStructXyz xyz_expected = {1,2.1,'x'};

  std::stringstream ss;

  auto&& [member_x, member_y, member_z]  = introspect_members(type_c<TestStructXyz>);
  call_set_value_lambda(member_x, xyz, 1);
  call_set_value_lambda(member_y, xyz, 2.1);
  call_set_value_lambda(member_z, xyz, 'x');

  EXPECT_EQ(xyz_expected.x, xyz.x);
  EXPECT_EQ(xyz_expected.y, xyz.y);
  EXPECT_EQ(xyz_expected.z, xyz.z);
}

struct Simple {
  int x;
};

template <typename T, typename V>
constexpr void call_set_simple_value_with_lambda(T&& self, V&& value) {
  // DON'T DO THIS:
  //  This captures by value, so we always get a copy of self.
  //([self = std::forward<T>(self)](auto&& value) mutable {
  ([&self](auto&& value) mutable {
    // &self is not ideal since prvalues are captured-by-reference instead of by-value.
    // this appears to be good enough for our use-case.
    self.x = value;
  })(std::forward<V>(value));
}

TEST(Introspection, SetSimpleValue) {
  Simple x{};
  Simple x_expected{123};

  call_set_simple_value_with_lambda(x, 123);
  EXPECT_EQ(x_expected.x, x.x);
}


}  // namespace introspect
}  // namespace iorap
