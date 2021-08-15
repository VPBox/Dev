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
double2 double2Test = {2.05, 12.05};
double3 double3Test = {3.05, 13.05, 23.05};
double4 double4Test = {4.05, 14.05, 24.05, 34.05};

static bool basic_test(uint32_t index) {
    bool failed = false;

    // This test focuses primarily on compilation-time, not run-time.
    // For this reason, none of the outputs are actually checked.

    rsDebug("double2Test", double2Test);
    rsDebug("double3Test", double3Test);
    rsDebug("double4Test", double4Test);

    return failed;
}

void test_rsdebug_23(uint32_t index, int test_num) {
    bool failed = false;
    failed |= basic_test(index);

    if (failed) {
        rsSendToClientBlocking(RS_MSG_TEST_FAILED);
        rsDebug("rsdebug_23_test FAILED", -1);
    }
    else {
        rsSendToClientBlocking(RS_MSG_TEST_PASSED);
        rsDebug("rsdebug_23_test PASSED", 0);
    }
}

