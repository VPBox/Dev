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

half gHalf;
half2 gHalf2;
half3 gHalf3;
half4 gHalf4;

static bool failed = false;

void validateHalf(half h) {
    _RS_ASSERT_EQU((float) h, 10.f);
}

void validateHalf2(half2 h2) {
    _RS_ASSERT_EQU((float) h2.x, 10.f);
    _RS_ASSERT_EQU((float) h2.y, 11.f);
}

void validateHalf3(half3 h3) {
    _RS_ASSERT_EQU((float) h3.x, 10.f);
    _RS_ASSERT_EQU((float) h3.y, 11.f);
    _RS_ASSERT_EQU((float) h3.z, -12.f);
}

void validateHalf4(half4 h4) {
    _RS_ASSERT_EQU((float) h4.x, 10.f);
    _RS_ASSERT_EQU((float) h4.y, 11.f);
    _RS_ASSERT_EQU((float) h4.z, -12.f);
    _RS_ASSERT_EQU((float) h4.w, -13.f);
}

void test(half h, half2 h2, half3 h3, half4 h4) {
    validateHalf(gHalf);
    validateHalf2(gHalf2);
    validateHalf3(gHalf3);
    validateHalf4(gHalf4);

    validateHalf(h);
    validateHalf2(h2);
    validateHalf3(h3);
    validateHalf4(h4);

    if (failed) {
        rsSendToClientBlocking(RS_MSG_TEST_FAILED);
    }
    else {
        rsSendToClientBlocking(RS_MSG_TEST_PASSED);
    }
}
