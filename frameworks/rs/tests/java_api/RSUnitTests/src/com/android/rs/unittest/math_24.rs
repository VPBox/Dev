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

// Testing math_24 library

volatile float f1;
volatile float2 f2;
volatile float3 f3;
volatile float4 f4;

#define TEST_FN_FUNC_FN(fnc)        \
    rsDebug("Testing " #fnc, 0);    \
    f1 = fnc(f1);                   \
    f2 = fnc(f2);                   \
    f3 = fnc(f3);                   \
    f4 = fnc(f4);

static bool test_fp_math_24(uint32_t index) {
    bool failed = false;
    start();

    TEST_FN_FUNC_FN(fract);

    float time = end(index);

    if (failed) {
        rsDebug("test_fp_math_24 FAILED", time);
    }
    else {
        rsDebug("test_fp_math_24 PASSED", time);
    }

    return failed;
}

void math_24_test(uint32_t index, int test_num) {
    bool failed = false;
    failed |= test_fp_math_24(index);

    if (failed) {
        rsSendToClientBlocking(RS_MSG_TEST_FAILED);
    }
    else {
        rsSendToClientBlocking(RS_MSG_TEST_PASSED);
    }
}
