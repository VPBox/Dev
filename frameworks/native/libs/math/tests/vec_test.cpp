/*
 * Copyright 2013 The Android Open Source Project
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

#define LOG_TAG "VecTest"

#include <math.h>
#include <stdlib.h>

#include <math/vec4.h>

#include <gtest/gtest.h>

namespace android {

class VecTest : public testing::Test {
};

TEST_F(VecTest, Basics) {
    vec4 v4;
    vec3& v3(v4.xyz);

    EXPECT_EQ(sizeof(vec4), sizeof(float)*4);
    EXPECT_EQ(sizeof(vec3), sizeof(float)*3);
    EXPECT_EQ(sizeof(vec2), sizeof(float)*2);
    EXPECT_EQ(reinterpret_cast<void*>(&v3), reinterpret_cast<void*>(&v4));
}

TEST_F(VecTest, Constructors) {
    vec4 v0;
    EXPECT_EQ(v0.x, 0);
    EXPECT_EQ(v0.y, 0);
    EXPECT_EQ(v0.z, 0);
    EXPECT_EQ(v0.w, 0);

    vec4 v1(1);
    EXPECT_EQ(v1.x, 1);
    EXPECT_EQ(v1.y, 1);
    EXPECT_EQ(v1.z, 1);
    EXPECT_EQ(v1.w, 1);

    vec4 v2(1, 2, 3, 4);
    EXPECT_EQ(v2.x, 1);
    EXPECT_EQ(v2.y, 2);
    EXPECT_EQ(v2.z, 3);
    EXPECT_EQ(v2.w, 4);

    vec4 v3(v2);
    EXPECT_EQ(v3.x, 1);
    EXPECT_EQ(v3.y, 2);
    EXPECT_EQ(v3.z, 3);
    EXPECT_EQ(v3.w, 4);

    vec4 v4(v3.xyz, 42);
    EXPECT_EQ(v4.x, 1);
    EXPECT_EQ(v4.y, 2);
    EXPECT_EQ(v4.z, 3);
    EXPECT_EQ(v4.w, 42);

    vec4 v5(vec3(v2.xy, 42), 24);
    EXPECT_EQ(v5.x, 1);
    EXPECT_EQ(v5.y, 2);
    EXPECT_EQ(v5.z, 42);
    EXPECT_EQ(v5.w, 24);

    float4 vf(2);
    EXPECT_EQ(vf.x, 2);
    EXPECT_EQ(vf.y, 2);
    EXPECT_EQ(vf.z, 2);
    EXPECT_EQ(vf.w, 2);
}

TEST_F(VecTest, Access) {
    vec4 v0(1, 2, 3, 4);

    v0.x = 10;
    v0.y = 20;
    v0.z = 30;
    v0.w = 40;
    EXPECT_EQ(v0.x, 10);
    EXPECT_EQ(v0.y, 20);
    EXPECT_EQ(v0.z, 30);
    EXPECT_EQ(v0.w, 40);

    v0[0] = 100;
    v0[1] = 200;
    v0[2] = 300;
    v0[3] = 400;
    EXPECT_EQ(v0.x, 100);
    EXPECT_EQ(v0.y, 200);
    EXPECT_EQ(v0.z, 300);
    EXPECT_EQ(v0.w, 400);

    v0.xyz = vec3(1, 2, 3);
    EXPECT_EQ(v0.x, 1);
    EXPECT_EQ(v0.y, 2);
    EXPECT_EQ(v0.z, 3);
    EXPECT_EQ(v0.w, 400);
}

TEST_F(VecTest, UnaryOps) {
    vec4 v0(1, 2, 3, 4);

    v0 += 1;
    EXPECT_EQ(v0.x, 2);
    EXPECT_EQ(v0.y, 3);
    EXPECT_EQ(v0.z, 4);
    EXPECT_EQ(v0.w, 5);

    v0 -= 1;
    EXPECT_EQ(v0.x, 1);
    EXPECT_EQ(v0.y, 2);
    EXPECT_EQ(v0.z, 3);
    EXPECT_EQ(v0.w, 4);

    v0 *= 2;
    EXPECT_EQ(v0.x, 2);
    EXPECT_EQ(v0.y, 4);
    EXPECT_EQ(v0.z, 6);
    EXPECT_EQ(v0.w, 8);

    v0 /= 2;
    EXPECT_EQ(v0.x, 1);
    EXPECT_EQ(v0.y, 2);
    EXPECT_EQ(v0.z, 3);
    EXPECT_EQ(v0.w, 4);

    vec4 v1(10, 20, 30, 40);

    v0 += v1;
    EXPECT_EQ(v0.x, 11);
    EXPECT_EQ(v0.y, 22);
    EXPECT_EQ(v0.z, 33);
    EXPECT_EQ(v0.w, 44);

    v0 -= v1;
    EXPECT_EQ(v0.x, 1);
    EXPECT_EQ(v0.y, 2);
    EXPECT_EQ(v0.z, 3);
    EXPECT_EQ(v0.w, 4);

    v0 *= v1;
    EXPECT_EQ(v0.x, 10);
    EXPECT_EQ(v0.y, 40);
    EXPECT_EQ(v0.z, 90);
    EXPECT_EQ(v0.w, 160);

    v0 /= v1;
    EXPECT_EQ(v0.x, 1);
    EXPECT_EQ(v0.y, 2);
    EXPECT_EQ(v0.z, 3);
    EXPECT_EQ(v0.w, 4);

    v1 = -v1;
    EXPECT_EQ(v1.x, -10);
    EXPECT_EQ(v1.y, -20);
    EXPECT_EQ(v1.z, -30);
    EXPECT_EQ(v1.w, -40);

    float4 fv(1, 2, 3, 4);
    v1 += fv;
    EXPECT_EQ(v1.x,  -9);
    EXPECT_EQ(v1.y, -18);
    EXPECT_EQ(v1.z, -27);
    EXPECT_EQ(v1.w, -36);
}

TEST_F(VecTest, ComparisonOps) {
    vec4 v0(1, 2, 3, 4);
    vec4 v1(10, 20, 30, 40);

    EXPECT_TRUE(v0 == v0);
    EXPECT_TRUE(v0 != v1);
    EXPECT_FALSE(v0 != v0);
    EXPECT_FALSE(v0 == v1);
}

TEST_F(VecTest, ComparisonFunctions) {
    vec4 v0(1, 2, 3, 4);
    vec4 v1(10, 20, 30, 40);

    EXPECT_TRUE(all(equal(v0, v0)));
    EXPECT_TRUE(all(notEqual(v0, v1)));
    EXPECT_FALSE(any(notEqual(v0, v0)));
    EXPECT_FALSE(any(equal(v0, v1)));

    EXPECT_FALSE(all(lessThan(v0, v0)));
    EXPECT_TRUE(all(lessThanEqual(v0, v0)));
    EXPECT_FALSE(all(greaterThan(v0, v0)));
    EXPECT_TRUE(all(greaterThanEqual(v0, v0)));
    EXPECT_TRUE(all(lessThan(v0, v1)));
    EXPECT_TRUE(all(greaterThan(v1, v0)));
}

TEST_F(VecTest, ArithmeticOps) {
    vec4 v0(1, 2, 3, 4);
    vec4 v1(10, 20, 30, 40);

    vec4 v2(v0 + v1);
    EXPECT_EQ(v2.x, 11);
    EXPECT_EQ(v2.y, 22);
    EXPECT_EQ(v2.z, 33);
    EXPECT_EQ(v2.w, 44);

    v0 = v1 * 2;
    EXPECT_EQ(v0.x, 20);
    EXPECT_EQ(v0.y, 40);
    EXPECT_EQ(v0.z, 60);
    EXPECT_EQ(v0.w, 80);

    v0 = 2 * v1;
    EXPECT_EQ(v0.x, 20);
    EXPECT_EQ(v0.y, 40);
    EXPECT_EQ(v0.z, 60);
    EXPECT_EQ(v0.w, 80);

    float4 vf(2);
    v0 = v1 * vf;
    EXPECT_EQ(v0.x, 20);
    EXPECT_EQ(v0.y, 40);
    EXPECT_EQ(v0.z, 60);
    EXPECT_EQ(v0.w, 80);
}

TEST_F(VecTest, ArithmeticFunc) {
    vec3 east(1, 0, 0);
    vec3 north(0, 1, 0);
    vec3 up(cross(east, north));
    EXPECT_EQ(up, vec3(0, 0, 1));
    EXPECT_EQ(dot(east, north), 0);
    EXPECT_EQ(length(east), 1);
    EXPECT_EQ(distance(east, north), sqrtf(2));

    vec3 v0(1, 2, 3);
    vec3 vn(normalize(v0));
    EXPECT_FLOAT_EQ(1, length(vn));
    EXPECT_FLOAT_EQ(length(v0), dot(v0, vn));

    float3 vf(east);
    EXPECT_EQ(length(vf), 1);

    EXPECT_TRUE(any(vec3(0, 0, 1)));
    EXPECT_FALSE(any(vec3(0, 0, 0)));

    EXPECT_TRUE(all(vec3(1, 1, 1)));
    EXPECT_FALSE(all(vec3(0, 0, 1)));

    EXPECT_TRUE(any(bool3(false, false, true)));
    EXPECT_FALSE(any(bool3(false)));

    EXPECT_TRUE(all(bool3(true)));
    EXPECT_FALSE(all(bool3(false, false, true)));

    std::function<bool(float)> p = [](auto v) -> bool { return v > 0.0f; };
    EXPECT_TRUE(all(map(vec3(1, 2, 3), p)));
}

}; // namespace android
