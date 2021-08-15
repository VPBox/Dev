/*
 * Copyright 2018 The Android Open Source Project
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
#define LOG_TAG "audio_utils_variadic_tests"
#include <audio_utils/variadic_utils.h>

#include <stdio.h>
#include <gtest/gtest.h>

// Our near expectation is 16x the bit that doesn't fit the mantissa.
// this works so long as we add values close in exponent with each other
// realizing that errors accumulate as the sqrt of N (random walk, lln, etc).
#define TEST_EXPECT_NEAR(e, v) \
    EXPECT_NEAR((e), (v), abs((e) * std::numeric_limits<decltype(e)>::epsilon() * 8))

#define PRINT_AND_EXPECT_EQ(expected, expr) { \
    auto value = (expr); \
    printf("(%s): %s\n", #expr, std::to_string(value).c_str()); \
    if ((expected) == (expected)) { EXPECT_EQ((expected), (value)); } \
    EXPECT_EQ((expected) != (expected), (value) != (value)); /* nan check */\
}

#define PRINT_AND_EXPECT_NEAR(expected, expr) { \
    auto ref = (expected); \
    auto value = (expr); \
    printf("(%s): %s\n", #expr, std::to_string(value).c_str()); \
    TEST_EXPECT_NEAR(ref, value); \
}

TEST(variadic_tests, printing)
{
    // for operator overloading...
    using namespace android::audio_utils;

    // print simple, deep value
    std::cout << "std::make_tuple(1, 2, 3)= " << std::make_tuple(1, 2, 3) << "\n";
    std::cout << "std::make_pair(1, std::make_pair(0, 1))= "
              << std::make_pair(1, std::make_pair(0, 1)) << "\n";
}

TEST(variadic_tests, equivalence)
{
    using android::audio_utils::equivalent;
    auto deep = std::make_pair(1., std::make_pair(2, 3));

    EXPECT_TRUE(equivalent(deep, deep));
    EXPECT_TRUE(equivalent(std::make_pair(1, 2), std::make_tuple(1, 2)));
    EXPECT_FALSE(equivalent(std::make_pair(1, 2), std::make_pair(0, 2)));
    EXPECT_FALSE(equivalent(std::make_pair(1, 2), 1));
    EXPECT_FALSE(equivalent(0, 2));
    EXPECT_TRUE(equivalent(1, 1.));
}

TEST(variadic_tests, template_checks)
{
    EXPECT_FALSE(android::audio_utils::is_variadic<double>::value);

    using tuple_t = std::tuple<double, double>;

    EXPECT_TRUE(android::audio_utils::is_variadic<tuple_t>::value);
    EXPECT_TRUE(android::audio_utils::is_tuple<tuple_t>::value);
    EXPECT_FALSE(android::audio_utils::is_pair<tuple_t>::value);
    EXPECT_FALSE(android::audio_utils::is_array<tuple_t>::value);
    EXPECT_FALSE(std::is_array<tuple_t>::value);

    using pair_t = std::pair<double, double>;

    EXPECT_TRUE(android::audio_utils::is_variadic<pair_t>::value);
    EXPECT_FALSE(android::audio_utils::is_tuple<pair_t>::value);
    EXPECT_TRUE(android::audio_utils::is_pair<pair_t>::value);
    EXPECT_FALSE(android::audio_utils::is_array<pair_t>::value);
    EXPECT_FALSE(std::is_array<pair_t>::value);

    using array_t = std::array<double, 2>;

    EXPECT_TRUE(android::audio_utils::is_variadic<array_t>::value);
    EXPECT_FALSE(android::audio_utils::is_tuple<array_t>::value);
    EXPECT_FALSE(android::audio_utils::is_pair<array_t>::value);
    EXPECT_TRUE(android::audio_utils::is_array<array_t>::value);
    EXPECT_FALSE(std::is_array<array_t>::value);

    EXPECT_FALSE(android::audio_utils::is_iterator<char>::value);
    EXPECT_TRUE(android::audio_utils::is_iterator<char *>::value);
    EXPECT_TRUE(android::audio_utils::is_iterator<decltype(std::vector<int>{}.begin())>::value);
}

TEST(variadic_tests, basic_math)
{
    // for operator overloading...
    using namespace android::audio_utils;

    using tuple_t = std::tuple<double, double>;
    tuple_t x{1, 2};
    tuple_t y{0, 3};
    double z = 3;

    std::cout << "x=" << x << " y=" << y << " x+y=" << (x + y) << "\n";
    std::cout << "x=" << x << " y=" << y << " x*y=" << (x * y) << "\n";
    std::cout << "x=" << x << " z=" << z << " x+z=" << (x + z) << "\n";
    std::cout << "x=" << x << " z=" << z << " x*z=" << (x * z) << "\n";
    std::cout << "x=" << x << " y=" << y << " innerProduct(x, y)=" << innerProduct(x, y) << "\n";
    std::cout << "x=" << x << " y=" << y << " outerProduct(x, y)=" << outerProduct(x, y) << "\n";
    std::cout << "x=" << x << " sqrt(x)=" << android::audio_utils::sqrt(x) << "\n";
    std::cout << "x=" << x << " y=" << y
            << " min(x, y)" <<  android::audio_utils::min(x, y) << "\n";

    // check opequals mode
    std::cout << "x=" << x;
    std::cout << " x+=2" << (x += 2) << "\n";
    std::cout << "x=" << x << " y=" << y;
    std::cout << " x*=y" << (x *= y) << "\n";

    using pair_t = std::pair<double, double>;
    pair_t px{1, 2};
    pair_t py{0, 3};

    std::cout << "px=" << px << " py=" << py << " px+py=" << (px + py) << "\n";
    std::cout << "px=" << px << " py=" << py << " px*py=" << (px * py) << "\n";
    std::cout << "px=" << px << " z="  << z  << " px+z="  << (px + z) << "\n";
    std::cout << "px=" << px << " z="  << z  << " px*z="  << (px * z) << "\n";
    std::cout << "px=" << px << " py=" << py << " innerProduct(px, py)="
            << innerProduct(px, py) << "\n";
    std::cout << "px=" << px << " py=" << py << " outerProduct(px, py)="
            << outerProduct(px, py) << "\n";

    using array_t = std::array<double, 2>;
    array_t ax{1, 2};
    array_t ay{0, 3};

    std::cout << "ax=" << ax << " ay=" << ay << " ax+ay=" << (ax + ay) << "\n";
    std::cout << "ax=" << ax << " ay=" << ay << " ax*ay=" << (ax * ay) << "\n";
    std::cout << "ax=" << ax << " z="  << z  << " ax+z="  << (ax + z) << "\n";
    std::cout << "ax=" << ax << " z="  << z  << " ax*z="  << (ax * z) << "\n";
    std::cout << "ax=" << px << " ay=" << ay << " innerProduct(ax, ay)="
            << innerProduct(ax, ay) << "\n";
    std::cout << "ax=" << px << " ay=" << ay << " outerProduct(ax, ay)="
            << outerProduct(ax, ay) << "\n";

    // deep math
    auto deep = std::make_pair(1., std::make_pair(2, 3));
    std::cout << "deep= " << deep << "\n";
    std::cout << "deep + deep= " << deep + deep << "\n";
    std::cout << "deep + 1= " << deep + 1 << "\n";
}
