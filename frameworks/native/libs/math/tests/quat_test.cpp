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

#define LOG_TAG "QuatTest"

#include <math.h>
#include <stdlib.h>

#include <random>
#include <functional>

#include <math/quat.h>
#include <math/mat4.h>
#include <math/vec3.h>
#include <math/vec4.h>

#include <gtest/gtest.h>

namespace android {

class QuatTest : public testing::Test {
protected:
};

TEST_F(QuatTest, Basics) {
    quatd q;
    double4& v(q.xyzw);

    EXPECT_EQ(sizeof(quatd), sizeof(double)*4);
    EXPECT_EQ(reinterpret_cast<void*>(&q), reinterpret_cast<void*>(&v));
}

TEST_F(QuatTest, Constructors) {
    quatd q0;
    EXPECT_EQ(q0.x, 0);
    EXPECT_EQ(q0.y, 0);
    EXPECT_EQ(q0.z, 0);
    EXPECT_EQ(q0.w, 0);

    quatd q1(1);
    EXPECT_EQ(q1.x, 0);
    EXPECT_EQ(q1.y, 0);
    EXPECT_EQ(q1.z, 0);
    EXPECT_EQ(q1.w, 1);

    quatd q2(1, 2, 3, 4);
    EXPECT_EQ(q2.x, 2);
    EXPECT_EQ(q2.y, 3);
    EXPECT_EQ(q2.z, 4);
    EXPECT_EQ(q2.w, 1);

    quatd q3(q2);
    EXPECT_EQ(q3.x, 2);
    EXPECT_EQ(q3.y, 3);
    EXPECT_EQ(q3.z, 4);
    EXPECT_EQ(q3.w, 1);

    quatd q4(q3.xyz, 42);
    EXPECT_EQ(q4.x, 2);
    EXPECT_EQ(q4.y, 3);
    EXPECT_EQ(q4.z, 4);
    EXPECT_EQ(q4.w, 42);

    quatd q5(double3(q2.xy, 42), 24);
    EXPECT_EQ(q5.x, 2);
    EXPECT_EQ(q5.y, 3);
    EXPECT_EQ(q5.z, 42);
    EXPECT_EQ(q5.w, 24);

    quatd q6;
    q6 = 12;
    EXPECT_EQ(q6.x, 0);
    EXPECT_EQ(q6.y, 0);
    EXPECT_EQ(q6.z, 0);
    EXPECT_EQ(q6.w, 12);

    quatd q7 = 1 + 2_id + 3_jd + 4_kd;
    EXPECT_EQ(q7.x, 2);
    EXPECT_EQ(q7.y, 3);
    EXPECT_EQ(q7.z, 4);
    EXPECT_EQ(q7.w, 1);

    quatf qf(2);
    EXPECT_EQ(qf.x, 0);
    EXPECT_EQ(qf.y, 0);
    EXPECT_EQ(qf.z, 0);
    EXPECT_EQ(qf.w, 2);
}

TEST_F(QuatTest, Access) {
    quatd q0(1, 2, 3, 4);
    q0.x = 10;
    q0.y = 20;
    q0.z = 30;
    q0.w = 40;
    EXPECT_EQ(q0.x, 10);
    EXPECT_EQ(q0.y, 20);
    EXPECT_EQ(q0.z, 30);
    EXPECT_EQ(q0.w, 40);

    q0[0] = 100;
    q0[1] = 200;
    q0[2] = 300;
    q0[3] = 400;
    EXPECT_EQ(q0.x, 100);
    EXPECT_EQ(q0.y, 200);
    EXPECT_EQ(q0.z, 300);
    EXPECT_EQ(q0.w, 400);

    q0.xyz = double3(1, 2, 3);
    EXPECT_EQ(q0.x, 1);
    EXPECT_EQ(q0.y, 2);
    EXPECT_EQ(q0.z, 3);
    EXPECT_EQ(q0.w, 400);
}

TEST_F(QuatTest, UnaryOps) {
    quatd q0(1, 2, 3, 4);

    q0 += 1;
    EXPECT_EQ(q0.x, 2);
    EXPECT_EQ(q0.y, 3);
    EXPECT_EQ(q0.z, 4);
    EXPECT_EQ(q0.w, 2);

    q0 -= 1;
    EXPECT_EQ(q0.x, 2);
    EXPECT_EQ(q0.y, 3);
    EXPECT_EQ(q0.z, 4);
    EXPECT_EQ(q0.w, 1);

    q0 *= 2;
    EXPECT_EQ(q0.x, 4);
    EXPECT_EQ(q0.y, 6);
    EXPECT_EQ(q0.z, 8);
    EXPECT_EQ(q0.w, 2);

    q0 /= 2;
    EXPECT_EQ(q0.x, 2);
    EXPECT_EQ(q0.y, 3);
    EXPECT_EQ(q0.z, 4);
    EXPECT_EQ(q0.w, 1);

    quatd q1(10, 20, 30, 40);

    q0 += q1;
    EXPECT_EQ(q0.x, 22);
    EXPECT_EQ(q0.y, 33);
    EXPECT_EQ(q0.z, 44);
    EXPECT_EQ(q0.w, 11);

    q0 -= q1;
    EXPECT_EQ(q0.x, 2);
    EXPECT_EQ(q0.y, 3);
    EXPECT_EQ(q0.z, 4);
    EXPECT_EQ(q0.w, 1);

    q1 = -q1;
    EXPECT_EQ(q1.x, -20);
    EXPECT_EQ(q1.y, -30);
    EXPECT_EQ(q1.z, -40);
    EXPECT_EQ(q1.w, -10);

    // TODO(mathias): multiplies
}

TEST_F(QuatTest, ComparisonOps) {
    quatd q0(1, 2, 3, 4);
    quatd q1(10, 20, 30, 40);

    EXPECT_TRUE(q0 == q0);
    EXPECT_TRUE(q0 != q1);
    EXPECT_FALSE(q0 != q0);
    EXPECT_FALSE(q0 == q1);
}

TEST_F(QuatTest, ArithmeticOps) {
    quatd q0(1, 2, 3, 4);
    quatd q1(10, 20, 30, 40);

    quatd q2(q0 + q1);
    EXPECT_EQ(q2.x, 22);
    EXPECT_EQ(q2.y, 33);
    EXPECT_EQ(q2.z, 44);
    EXPECT_EQ(q2.w, 11);

    q0 = q1 * 2;
    EXPECT_EQ(q0.x, 40);
    EXPECT_EQ(q0.y, 60);
    EXPECT_EQ(q0.z, 80);
    EXPECT_EQ(q0.w, 20);

    q0 = 2 * q1;
    EXPECT_EQ(q0.x, 40);
    EXPECT_EQ(q0.y, 60);
    EXPECT_EQ(q0.z, 80);
    EXPECT_EQ(q0.w, 20);

    quatf qf(2);
    q0 = q1 * qf;
    EXPECT_EQ(q0.x, 40);
    EXPECT_EQ(q0.y, 60);
    EXPECT_EQ(q0.z, 80);
    EXPECT_EQ(q0.w, 20);

    EXPECT_EQ(1_id * 1_id, quat(-1));
    EXPECT_EQ(1_jd * 1_jd, quat(-1));
    EXPECT_EQ(1_kd * 1_kd, quat(-1));
    EXPECT_EQ(1_id * 1_jd * 1_kd, quat(-1));
}

TEST_F(QuatTest, ArithmeticFunc) {
    quatd q(1, 2, 3, 4);
    quatd qc(conj(q));
    __attribute__((unused)) quatd qi(inverse(q));
    quatd qn(normalize(q));

    EXPECT_EQ(qc.x, -2);
    EXPECT_EQ(qc.y, -3);
    EXPECT_EQ(qc.z, -4);
    EXPECT_EQ(qc.w,  1);

    EXPECT_EQ(~q, qc);
    EXPECT_EQ(length(q), length(qc));
    EXPECT_EQ(sqrt(30), length(q));
    EXPECT_FLOAT_EQ(1, length(qn));
    EXPECT_FLOAT_EQ(1, dot(qn, qn));

    quatd qr = quatd::fromAxisAngle(double3(0, 0, 1), M_PI / 2);
    EXPECT_EQ(mat4d(qr).toQuaternion(), qr);
    EXPECT_EQ(1_id, mat4d(1_id).toQuaternion());
    EXPECT_EQ(1_jd, mat4d(1_jd).toQuaternion());
    EXPECT_EQ(1_kd, mat4d(1_kd).toQuaternion());


    EXPECT_EQ(qr, log(exp(qr)));

    quatd qq = qr * qr;
    quatd q2 = pow(qr, 2);
    EXPECT_NEAR(qq.x, q2.x, 1e-15);
    EXPECT_NEAR(qq.y, q2.y, 1e-15);
    EXPECT_NEAR(qq.z, q2.z, 1e-15);
    EXPECT_NEAR(qq.w, q2.w, 1e-15);

    quatd qa = quatd::fromAxisAngle(double3(0, 0, 1), 0);
    quatd qb = quatd::fromAxisAngle(double3(0, 0, 1), M_PI / 2);
    quatd qs = slerp(qa, qb, 0.5);
    qr = quatd::fromAxisAngle(double3(0, 0, 1), M_PI / 4);
    EXPECT_FLOAT_EQ(qr.x, qs.x);
    EXPECT_FLOAT_EQ(qr.y, qs.y);
    EXPECT_FLOAT_EQ(qr.z, qs.z);
    EXPECT_FLOAT_EQ(qr.w, qs.w);

    qs = nlerp(qa, qb, 0.5);
    EXPECT_FLOAT_EQ(qr.x, qs.x);
    EXPECT_FLOAT_EQ(qr.y, qs.y);
    EXPECT_FLOAT_EQ(qr.z, qs.z);
    EXPECT_FLOAT_EQ(qr.w, qs.w);
}

TEST_F(QuatTest, MultiplicationExhaustive) {
    static constexpr double value_eps = double(1000) * std::numeric_limits<double>::epsilon();

    std::default_random_engine generator(171717);
    std::uniform_real_distribution<double> distribution(-10.0, 10.0);
    auto rand_gen = std::bind(distribution, generator);

    for (size_t i = 0; i < (1024 * 1024); ++i) {
        double3 axis_a = normalize(double3(rand_gen(), rand_gen(), rand_gen()));
        double angle_a = rand_gen();
        quatd a = quatd::fromAxisAngle(axis_a, angle_a);

        double3 axis_b = normalize(double3(rand_gen(), rand_gen(), rand_gen()));
        double angle_b = rand_gen();
        quatd b = quatd::fromAxisAngle(axis_b, angle_b);

        quatd ab = a * b;
        quatd ab_other(a.w * b.xyz + b.w * a.xyz + cross(a.xyz, b.xyz),
            (a.w * b.w) - dot(a.xyz, b.xyz));

        ASSERT_NEAR(ab.x, ab_other.x, value_eps);
        ASSERT_NEAR(ab.y, ab_other.y, value_eps);
        ASSERT_NEAR(ab.z, ab_other.z, value_eps);
        ASSERT_NEAR(ab.w, ab_other.w, value_eps);
    }
}

}; // namespace android
