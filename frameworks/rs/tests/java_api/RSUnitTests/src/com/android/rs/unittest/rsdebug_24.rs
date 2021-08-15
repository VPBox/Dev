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

static bool basic_test(uint32_t index) {
    bool failed = false;

    // This test focuses primarily on compilation-time, not run-time.
    // For this reason, none of the outputs are actually checked.

    // http://b/27526302 - globals of half type cannot be exported and fail compilation
    half halfTest = (half) 1.5f;
    half2 half2Test = {(half) 1.5f, (half) 2.5f};
    half3 half3Test = {(half) 1.5f, (half) 2.5f, (half) 3.5f};
    half4 half4Test = {(half) 0.f, (half) -0.f, (half) 1.f/0.f, (half) 0.f/0.f};

    rsDebug("halfTest", halfTest);
    rsDebug("half2Test", half2Test);
    rsDebug("half3Test", half3Test);
    rsDebug("half4Test", half4Test);

    return failed;
}

void test_rsdebug_24(uint32_t index, int test_num) {
    bool failed = false;
    failed |= basic_test(index);

    if (failed) {
        rsSendToClientBlocking(RS_MSG_TEST_FAILED);
        rsDebug("rsdebug_24_test FAILED", -1);
    }
    else {
        rsSendToClientBlocking(RS_MSG_TEST_PASSED);
        rsDebug("rsdebug_24_test PASSED", 0);
    }
}

