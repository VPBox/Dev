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

rs_allocation aRaw;
int dimX;
int dimY;
static bool failed = false;

void root(int *out, uint32_t x, uint32_t y) {
    *out = x + y * dimX;
}

void foo(const int *in, int *out, uint32_t x, uint32_t y) {
    _RS_ASSERT(*in == (x + y * dimX));
    *out = 99 + x + y * dimX;
    _RS_ASSERT(*out == (99 + x + y * dimX));
}

static bool test_root_output() {
    bool failed = false;
    int i, j;

    for (j = 0; j < dimY; j++) {
        for (i = 0; i < dimX; i++) {
            int v = rsGetElementAt_int(aRaw, i, j);
            _RS_ASSERT(v == (i + j * dimX));
        }
    }

    if (failed) {
        rsDebug("test_root_output FAILED", 0);
    }
    else {
        rsDebug("test_root_output PASSED", 0);
    }

    return failed;
}

static bool test_foo_output() {
    bool failed = false;
    int i, j;

    for (j = 0; j < dimY; j++) {
        for (i = 0; i < dimX; i++) {
            int v = rsGetElementAt_int(aRaw, i, j);
            _RS_ASSERT(v == (99 + i + j * dimX));
        }
    }

    if (failed) {
        rsDebug("test_foo_output FAILED", 0);
    }
    else {
        rsDebug("test_foo_output PASSED", 0);
    }

    return failed;
}

void verify_root() {
    failed |= test_root_output();
}

void verify_foo() {
    failed |= test_foo_output();
}

void foreach_test() {
    if (failed) {
        rsSendToClientBlocking(RS_MSG_TEST_FAILED);
    }
    else {
        rsSendToClientBlocking(RS_MSG_TEST_PASSED);
    }
}

