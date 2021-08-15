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

rs_allocation A;
rs_allocation B;
uint32_t gDimX;
static bool failed = false;

void init_vars(int *out) {
    *out = 7;
}

int RS_KERNEL root(int ain, rs_kernel_context context, uint32_t x) {
    _RS_ASSERT_EQU(rsGetArray0(context), 0);
    _RS_ASSERT_EQU(rsGetArray1(context), 0);
    _RS_ASSERT_EQU(rsGetArray2(context), 0);
    _RS_ASSERT_EQU(rsGetArray3(context), 0);
    _RS_ASSERT_EQU(rsGetFace(context), RS_ALLOCATION_CUBEMAP_FACE_POSITIVE_X);
    _RS_ASSERT_EQU(rsGetLod(context), 0);

    _RS_ASSERT_EQU(rsGetDimY(context), 0);
    _RS_ASSERT_EQU(rsGetDimZ(context), 0);
    _RS_ASSERT_EQU(rsGetDimArray0(context), 0);
    _RS_ASSERT_EQU(rsGetDimArray1(context), 0);
    _RS_ASSERT_EQU(rsGetDimArray2(context), 0);
    _RS_ASSERT_EQU(rsGetDimArray3(context), 0);
    _RS_ASSERT_EQU(rsGetDimHasFaces(context), false);
    _RS_ASSERT_EQU(rsGetDimLod(context), 0);

    if (!_RS_ASSERT_EQU(ain, 7))
        rsDebug("root at x", x);
    uint32_t dimX = rsGetDimX(context);
    _RS_ASSERT_EQU(dimX, gDimX);
    return ain + x;
}

static bool test_root_output() {
    bool failed = false;
    int i;

    for (i = 0; i < gDimX; i++) {
        int bElt = rsGetElementAt_int(B, i);
        int aElt = rsGetElementAt_int(A, i);
        if (!_RS_ASSERT_EQU(bElt, (aElt + i)))
            rsDebug("test_root_output at i", i);
    }

    if (failed) {
        rsDebug("ctxt_default test_root_output FAILED", 0);
    }
    else {
        rsDebug("ctxt_default test_root_output PASSED", 0);
    }

    return failed;
}

void verify_root() {
    failed |= test_root_output();
}

void kernel_test() {
    if (failed) {
        rsSendToClientBlocking(RS_MSG_TEST_FAILED);
    }
    else {
        rsSendToClientBlocking(RS_MSG_TEST_PASSED);
    }
}
