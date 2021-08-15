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

// Testing primitive types
float floatTest = 1.99f;
float2 float2Test = {2.99f, 12.99f};
float3 float3Test = {3.99f, 13.99f, 23.99f};
float4 float4Test = {4.99f, 14.99f, 24.99f, 34.99f};
double doubleTest = 2.05;
char charTest = -8;
short shortTest = -16;
int intTest = -32;
long longTest = 17179869184l; // 1 << 34
long long longlongTest = 68719476736l; // 1 << 36

uchar ucharTest = 8;
ushort ushortTest = 16;
uint uintTest = 32;
ulong ulongTest = 4611686018427387904L;
int64_t int64_tTest = -17179869184l; // - 1 << 34
uint64_t uint64_tTest = 117179869184l;

static bool basic_test(uint32_t index) {
    bool failed = false;

    // This test focuses primarily on compilation-time, not run-time.
    // For this reason, none of the outputs are actually checked.

    rsDebug("floatTest", floatTest);
    rsDebug("float2Test", float2Test);
    rsDebug("float3Test", float3Test);
    rsDebug("float4Test", float4Test);
    rsDebug("doubleTest", doubleTest);
    rsDebug("charTest", charTest);
    rsDebug("shortTest", shortTest);
    rsDebug("intTest", intTest);
    rsDebug("longTest", longTest);
    rsDebug("longlongTest", longlongTest);

    rsDebug("ucharTest", ucharTest);
    rsDebug("ushortTest", ushortTest);
    rsDebug("uintTest", uintTest);
    rsDebug("ulongTest", ulongTest);
    rsDebug("int64_tTest", int64_tTest);
    rsDebug("uint64_tTest", uint64_tTest);

    return failed;
}

void test_rsdebug(uint32_t index, int test_num) {
    bool failed = false;
    failed |= basic_test(index);

    if (failed) {
        rsSendToClientBlocking(RS_MSG_TEST_FAILED);
        rsDebug("rsdebug_test FAILED", -1);
    }
    else {
        rsSendToClientBlocking(RS_MSG_TEST_PASSED);
        rsDebug("rsdebug_test PASSED", 0);
    }
}

