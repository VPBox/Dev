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

float4 f4 = { 2.0f, 4.0f, 6.0f, 8.0f };

char4 i8_4 = { -1, -2, -3, 4 };

static bool test_convert() {
    bool failed = false;

    f4 = convert_float4(i8_4);
    _RS_ASSERT(f4.x == -1.0f);
    _RS_ASSERT(f4.y == -2.0f);
    _RS_ASSERT(f4.z == -3.0f);
    _RS_ASSERT(f4.w == 4.0f);

    if (failed) {
        rsDebug("test_convert FAILED", 0);
    }
    else {
        rsDebug("test_convert PASSED", 0);
    }

    return failed;
}

void convert_test() {
    bool failed = false;
    failed |= test_convert();

    if (failed) {
        rsSendToClientBlocking(RS_MSG_TEST_FAILED);
    }
    else {
        rsSendToClientBlocking(RS_MSG_TEST_PASSED);
    }
}

