/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "shared.rsh"

rs_allocation gAlloc;
static bool failed = false;

static half gStart = 12.0f;
static half gIncr = 0.125;
int gDimX;
int gDimY;
int gDimZ;

static half ElementAt_half(int x, int y, int z) {
    return gStart + x + y * gDimX + z * gDimX * gDimY;
}

static half2 ElementAt_half2(int x, int y, int z) {
    half r = gStart + x + y * gDimX + z * gDimX * gDimY;
    half2 ret = {r, r + gIncr};
    return ret;
}

static half3 ElementAt_half3(int x, int y, int z) {
    half r = gStart + x + y * gDimX + z * gDimX * gDimY;
    half3 ret = {r, r + gIncr, r + gIncr * 2};
    return ret;
}

static half4 ElementAt_half4(int x, int y, int z) {
    half r = gStart + x + y * gDimX + z * gDimX * gDimY;
    half4 ret = {r, r + gIncr, r + gIncr * 2, r + gIncr * 3};
    return ret;
}

static void compare_half(half x, half y) {
    _RS_ASSERT_EQU(x, y);
    if (x != y) {
        rsDebug("Different half values: ", x, y);
    }
}

static void compare_half2(half2 x, half2 y) {
    _RS_ASSERT_EQU(x.r, y.r);
    _RS_ASSERT_EQU(x.g, y.g);
    if (x.r != y.r || x.g != y.g) {
        rsDebug("Different half vectors v1: ", x.r, x.g);
        rsDebug("                       v2: ", y.r, y.g);
    }
}

static void compare_half3(half3 x, half3 y) {
    _RS_ASSERT_EQU(x.r, y.r);
    _RS_ASSERT_EQU(x.g, y.g);
    _RS_ASSERT_EQU(x.b, y.b);
    if (x.r != y.r || x.g != y.g || x.b != y.b) {
        rsDebug("Different half vectors v1: ", x.r, x.g, x.b);
        rsDebug("                       v2: ", y.r, y.g, y.b);
    }
}

static void compare_half4(half4 x, half4 y) {
    _RS_ASSERT_EQU(x.r, y.r);
    _RS_ASSERT_EQU(x.g, y.g);
    _RS_ASSERT_EQU(x.b, y.b);
    _RS_ASSERT_EQU(x.a, y.a);
    if (x.r != y.r || x.g != y.g || x.b != y.b || x.a != y.a) {
        rsDebug("Different half vectors v1: ", x.r, x.g, x.b, x.a);
        rsDebug("                       v2: ", y.r, y.g, x.b, x.a);
    }
}

#define SET_KERNEL(T)                                 \
    T RS_KERNEL set_kernel_##T(int x, int y, int z) { \
        return ElementAt_##T(x, y, z);                \
    }

SET_KERNEL(half)
SET_KERNEL(half2)
SET_KERNEL(half3)
SET_KERNEL(half4)

#define INVOKE_SET(T)                                                  \
    void set_##T() {                                                   \
        int yEnd = (gDimY != 0) ? gDimY: 1;                            \
        int zEnd = (gDimZ != 0) ? gDimZ: 1;                            \
        for (int x=0; x < gDimX; x ++)                                 \
            for (int y=0; y < yEnd; y++)                               \
                for (int z=0; z < zEnd; z++)                           \
                    rsSetElementAt_##T(gAlloc, ElementAt_##T(x, y, z), \
                                       x, y, z);                       \
    }

INVOKE_SET(half)
INVOKE_SET(half2)
INVOKE_SET(half3)
INVOKE_SET(half4)

#define VERIFY_KERNEL(T)                                           \
    void RS_KERNEL verify_kernel_##T(T val, int x, int y, int z) { \
        compare_##T(val, ElementAt_##T(x, y, z));                  \
    }

VERIFY_KERNEL(half);
VERIFY_KERNEL(half2);
VERIFY_KERNEL(half3);
VERIFY_KERNEL(half4);

#define INVOKE_VERIFY(T)                                             \
    void verify_##T() {                                              \
            int yEnd = (gDimY != 0) ? gDimY: 1;                      \
            int zEnd = (gDimZ != 0) ? gDimZ: 1;                      \
            for (int x=0; x < gDimX; x ++) {                         \
                for (int y=0; y < yEnd; y++) {                       \
                    for (int z=0; z < zEnd; z++) {                   \
                        T val = rsGetElementAt_##T(gAlloc, x, y, z); \
                        compare_##T(val, ElementAt_##T(x, y, z));    \
                    }                                                \
                }                                                    \
            }                                                        \
    }

INVOKE_VERIFY(half)
INVOKE_VERIFY(half2)
INVOKE_VERIFY(half3)
INVOKE_VERIFY(half4)

void fp16_test() {
    if (failed) {
        rsSendToClientBlocking(RS_MSG_TEST_FAILED);
    }
    else {
        rsSendToClientBlocking(RS_MSG_TEST_PASSED);
    }
}

