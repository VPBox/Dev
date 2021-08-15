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

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits>
#include <cstddef>
#include <vector>

#include "android-base/file.h"
#include <gtest/gtest.h>

#include <binder/Parcel.h>
#include <binder/Value.h>
#include <binder/Debug.h>

using ::android::binder::Value;
using ::android::os::PersistableBundle;
using ::android::String16;
using ::std::vector;

#define VALUE_TYPE_TEST(T, TYPENAME, VAL)         \
    TEST(ValueType, Handles ## TYPENAME) {        \
        T x = VAL;                                \
        T y = T();                                \
        Value value = VAL;                        \
        ASSERT_FALSE(value.empty());              \
        ASSERT_TRUE(value.is ## TYPENAME ());     \
        ASSERT_TRUE(value.get ## TYPENAME (&y));  \
        ASSERT_EQ(x, y);                          \
        ASSERT_EQ(value, Value(y));               \
        value.put ## TYPENAME (x);                \
        ASSERT_EQ(value, Value(y));               \
        value = Value();                          \
        ASSERT_TRUE(value.empty());               \
        ASSERT_NE(value, Value(y));               \
        value = y;                                \
        ASSERT_EQ(value, Value(x));               \
    }

#define VALUE_TYPE_VECTOR_TEST(T, TYPENAME, VAL)      \
    TEST(ValueType, Handles ## TYPENAME ## Vector) {  \
        vector<T> x;                                  \
        vector<T> y;                                  \
        x.push_back(VAL);                             \
        x.push_back(T());                             \
        Value value(x);                               \
        ASSERT_FALSE(value.empty());                  \
        ASSERT_TRUE(value.is ## TYPENAME ## Vector());    \
        ASSERT_TRUE(value.get ## TYPENAME ## Vector(&y)); \
        ASSERT_EQ(x, y);                              \
        ASSERT_EQ(value, Value(y));                   \
        value.put ## TYPENAME ## Vector(x);           \
        ASSERT_EQ(value, Value(y));                   \
        value = Value();                              \
        ASSERT_TRUE(value.empty());                   \
        ASSERT_NE(value, Value(y));                   \
        value = y;                                    \
        ASSERT_EQ(value, Value(x));                   \
    }

VALUE_TYPE_TEST(bool, Boolean, true)
VALUE_TYPE_TEST(int32_t, Int, 31337)
VALUE_TYPE_TEST(int64_t, Long, 13370133701337L)
VALUE_TYPE_TEST(double, Double, 3.14159265358979323846)
VALUE_TYPE_TEST(String16, String, String16("Lovely"))

VALUE_TYPE_VECTOR_TEST(bool, Boolean, true)
VALUE_TYPE_VECTOR_TEST(int32_t, Int, 31337)
VALUE_TYPE_VECTOR_TEST(int64_t, Long, 13370133701337L)
VALUE_TYPE_VECTOR_TEST(double, Double, 3.14159265358979323846)
VALUE_TYPE_VECTOR_TEST(String16, String, String16("Lovely"))

VALUE_TYPE_TEST(PersistableBundle, PersistableBundle, PersistableBundle())

TEST(ValueType, HandlesClear) {
    Value value;
    ASSERT_TRUE(value.empty());
    value.putInt(31337);
    ASSERT_FALSE(value.empty());
    value.clear();
    ASSERT_TRUE(value.empty());
}

TEST(ValueType, HandlesSwap) {
    Value value_a, value_b;
    int32_t int_x;
    value_a.putInt(31337);
    ASSERT_FALSE(value_a.empty());
    ASSERT_TRUE(value_b.empty());
    value_a.swap(value_b);
    ASSERT_FALSE(value_b.empty());
    ASSERT_TRUE(value_a.empty());
    ASSERT_TRUE(value_b.getInt(&int_x));
    ASSERT_EQ(31337, int_x);
}
