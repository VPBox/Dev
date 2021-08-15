/*
 * Copyright 2014 The Android Open Source Project
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

//#define LOG_NDEBUG 0
#define LOG_TAG "C2_test"

#include <gtest/gtest.h>

#include <C2.h>

/* ======================================= STATIC TESTS ======================================= */

template<int N>
struct c2_const_checker
{
    inline constexpr static int num() { return N; }
};

constexpr auto min_i32_i32 = c2_min(int32_t(1), int32_t(2));
static_assert(std::is_same<decltype(min_i32_i32), const int32_t>::value, "should be int32_t");
constexpr auto min_i32_i64 = c2_min(int32_t(3), int64_t(2));
static_assert(std::is_same<decltype(min_i32_i64), const int64_t>::value, "should be int64_t");
constexpr auto min_i8_i32 = c2_min(int8_t(0xff), int32_t(0xffffffff));
static_assert(std::is_same<decltype(min_i8_i32), const int32_t>::value, "should be int32_t");

static_assert(c2_const_checker<min_i32_i32>::num() == 1, "should be 1");
static_assert(c2_const_checker<min_i32_i64>::num() == 2, "should be 2");
static_assert(c2_const_checker<min_i8_i32>::num() == 0xffffffff, "should be 0xffffffff");

constexpr auto min_u32_u32 = c2_min(uint32_t(1), uint32_t(2));
static_assert(std::is_same<decltype(min_u32_u32), const uint32_t>::value, "should be uint32_t");
constexpr auto min_u32_u64 = c2_min(uint32_t(3), uint64_t(2));
static_assert(std::is_same<decltype(min_u32_u64), const uint32_t>::value, "should be uint32_t");
constexpr auto min_u32_u8 = c2_min(uint32_t(0xffffffff), uint8_t(0xff));
static_assert(std::is_same<decltype(min_u32_u8), const uint8_t>::value, "should be uint8_t");

static_assert(c2_const_checker<min_u32_u32>::num() == 1, "should be 1");
static_assert(c2_const_checker<min_u32_u64>::num() == 2, "should be 2");
static_assert(c2_const_checker<min_u32_u8>::num() == 0xff, "should be 0xff");

constexpr auto max_i32_i32 = c2_max(int32_t(1), int32_t(2));
static_assert(std::is_same<decltype(max_i32_i32), const int32_t>::value, "should be int32_t");
constexpr auto max_i32_i64 = c2_max(int32_t(3), int64_t(2));
static_assert(std::is_same<decltype(max_i32_i64), const int64_t>::value, "should be int64_t");
constexpr auto max_i8_i32 = c2_max(int8_t(0xff), int32_t(0xffffffff));
static_assert(std::is_same<decltype(max_i8_i32), const int32_t>::value, "should be int32_t");

static_assert(c2_const_checker<max_i32_i32>::num() == 2, "should be 2");
static_assert(c2_const_checker<max_i32_i64>::num() == 3, "should be 3");
static_assert(c2_const_checker<max_i8_i32>::num() == 0xffffffff, "should be 0xffffffff");

constexpr auto max_u32_u32 = c2_max(uint32_t(1), uint32_t(2));
static_assert(std::is_same<decltype(max_u32_u32), const uint32_t>::value, "should be uint32_t");
constexpr auto max_u32_u64 = c2_max(uint32_t(3), uint64_t(2));
static_assert(std::is_same<decltype(max_u32_u64), const uint64_t>::value, "should be uint64_t");
constexpr auto max_u32_u8 = c2_max(uint32_t(0x7fffffff), uint8_t(0xff));
static_assert(std::is_same<decltype(max_u32_u8), const uint32_t>::value, "should be uint32_t");

static_assert(c2_const_checker<max_u32_u32>::num() == 2, "should be 2");
static_assert(c2_const_checker<max_u32_u64>::num() == 3, "should be 3");
static_assert(c2_const_checker<max_u32_u8>::num() == 0x7fffffff, "should be 0x7fffffff");

/* ======================================= COUNTER TESTS ======================================= */

void c2_cntr_static_test() {
    // sanity checks for construction/assignment
    constexpr c2_cntr32_t c32_a(123);
    constexpr c2_cntr64_t c64_a(-456);
    c2_cntr32_t c32_b __unused = c64_a;
    // c32_b = 64.; // DISALLOWED
    // c2_cntr64_t c64_b = c32_a; // DISALLOWED

    // sanity checks for some constexpr operators
    static_assert(std::is_same<decltype(c32_a + c64_a), decltype(c64_a + c32_a)>::value, "+ should result same type");
    static_assert(c32_a + c64_a == c2_cntr32_t(-333), "123 + -456 = -333");
    static_assert(c32_a + c32_a == c2_cntr32_t(246), "123 + 123 = 246");
    static_assert(c64_a + c32_a == c2_cntr32_t(-333), "-456 + 123 = 579");
    static_assert(std::is_same<decltype(c32_a + 1), decltype(1 + c32_a)>::value, "+ should result same type");
    static_assert(c32_a + 456 == c2_cntr32_t(579), "123 + 456 = 579");
    static_assert(456 + c64_a == c2_cntr64_t(0), "456 + -456 = 0");
    static_assert(std::is_same<decltype(c32_a - c64_a), decltype(c64_a - c32_a)>::value, "- should result same type");
    static_assert(c32_a - c64_a == c2_cntr32_t(579), "123 - -456 = 579");
    static_assert(c32_a - c32_a == c2_cntr32_t(0), "123 - 123 = 0");
    static_assert(c64_a - c32_a == c2_cntr32_t(-579), "-456 - 123 = -579");
    static_assert(std::is_same<decltype(c32_a - 1), decltype(1 - c32_a)>::value, "- should result same type");
    static_assert(c32_a - 456 == c2_cntr32_t(-333), "123 - 456 = -333");
    static_assert(456 - c64_a == c2_cntr64_t(912), "456 - -456 = 912");
    static_assert(std::is_same<decltype(c32_a * c64_a), decltype(c64_a * c32_a)>::value, "* should result same type");
    static_assert(c32_a * c64_a == c2_cntr32_t(-56088), "123 * -456 = -56088");
    static_assert(c32_a * c32_a == c2_cntr32_t(15129), "123 * 123 = 15129");
    static_assert(c64_a * c32_a == c2_cntr32_t(-56088), "-456 * 123 = -56088");
    static_assert(std::is_same<decltype(c32_a * 1), decltype(1 * c32_a)>::value, "* should result same type");
    static_assert(c32_a * 456 == c2_cntr32_t(56088), "123 * 456 = 56088");
    static_assert(456 * c64_a == c2_cntr64_t(-207936), "456 * -456 = -207936");

    static_assert((c32_a << 26u) == c2_cntr32_t(0xEC000000), "123 << 26 = 0xEC000000");

    // sanity checks for unary operators
    static_assert(c2_cntr32_t(1) == +c2_cntr32_t(1), "1 == +1");
    static_assert(c2_cntr32_t(1) == -c2_cntr32_t(-1), "1 == --1");

    // sanity checks for comparison
    using c8_t = c2_cntr_t<uint8_t>;
    static_assert(c8_t(-0x80) > c8_t(0x7f), "80 > 7F");
    static_assert(c8_t(-0x80) >= c8_t(0x7f), "80 >= 7F");
    static_assert(c8_t(0x7f) > c8_t(0x7e), "7F > 7E");
    static_assert(c8_t(0x7f) >= c8_t(0x7e), "7F >= 7E");
    static_assert(!(c8_t(-0x80) > c8_t(0)), "80 !> 00");
    static_assert(!(c8_t(-0x80) >= c8_t(0)), "80 !>= 00");
    static_assert(!(c8_t(-0x80) > c8_t(-0x80)), "80 !> 80");
    static_assert(c8_t(-0x80) >= c8_t(-0x80), "80 >= 80");

    static_assert(c8_t(-0x80) == c8_t(0x80), "80 == 80");
    static_assert(!(c8_t(-0x80) == c8_t(0)), "80 != 0");
    static_assert(c8_t(-0x80) != c8_t(0x7f), "80 != 7F");
    static_assert(!(c8_t(0x7f) != c8_t(0x7f)), "80 != 7F");

    static_assert(c8_t(0x7f) < c8_t(-0x80), "7F < 80");
    static_assert(c8_t(0x7f) <= c8_t(-0x80), "7F < 80");
    static_assert(c8_t(0x7e) < c8_t(0x7f), "7E < 7F");
    static_assert(c8_t(0x7e) <= c8_t(0x7f), "7E < 7F");
    static_assert(!(c8_t(-0x40) < c8_t(0x40)), "-40 !< 40");
    static_assert(!(c8_t(-0x40) <= c8_t(0x40)), "-40 !<= 40");
    static_assert(!(c8_t(-0x40) < c8_t(-0x40)), "-40 !< -40");
    static_assert(c8_t(-0x40) <= c8_t(-0x40), "-40 <= -40");

    static_assert(c2_cntr32_t(-0x7fffffff - 1) > c2_cntr32_t(0x7fffffff), "80 > 7F");
    static_assert(!(c2_cntr32_t(-0x7fffffff - 1) > c2_cntr32_t(0)), "80 !> 00");
    static_assert(c2_cntr32_t(1) == c2_cntr32_t(c2_cntr64_t(0x100000001ul)), "1 == 1");
}

class C2Test : public ::testing::Test {
};

TEST_F(C2Test, CounterTest) {
    c2_cntr32_t c32_a(123);
    c2_cntr64_t c64_a(-456);
    EXPECT_EQ(c32_a += 3, c2_cntr32_t(126));
    EXPECT_EQ(c32_a += c64_a, c2_cntr32_t(-330));
    EXPECT_EQ(c32_a <<= 2u, c2_cntr32_t(-1320));
    EXPECT_EQ(c64_a *= 3, c2_cntr64_t(-1368));
    EXPECT_EQ(c32_a -= c64_a, c2_cntr32_t(48));
    EXPECT_EQ(c32_a -= 40, c2_cntr32_t(8));
    EXPECT_EQ(c32_a *= c32_a, c2_cntr32_t(64));
}

