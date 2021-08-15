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
uint32_t gDimX, gDimY, gDimZ;
static bool failed = false;

void init_vars(int *out) {
    *out = 7;
}

int RS_KERNEL root(int ain, rs_kernel_context context, uint32_t x, uint32_t y, uint32_t z) {
    if (!_RS_ASSERT_EQU(ain, 7))
        rsDebug("root at x, y, z", x, y, z);
    uint32_t dimX = rsGetDimX(context);
    uint32_t dimY = rsGetDimY(context);
    uint32_t dimZ = rsGetDimZ(context);
    _RS_ASSERT_EQU(dimX, gDimX);
    _RS_ASSERT_EQU(dimY, gDimY);
    _RS_ASSERT_EQU(dimZ, gDimZ);
    return ain + x + dimX * y + dimX * dimY * z;
}

static bool test_root_output() {
    bool failed = false;
    int i, j, k;

    for (i = 0; i < gDimX; i++) {
        for (j = 0; j < gDimY; j++) {
            for (k = 0; k < gDimZ; k++) {
                int bElt = rsGetElementAt_int(B, i, j, k);
                int aElt = rsGetElementAt_int(A, i, j, k);
                if (!_RS_ASSERT_EQU(bElt, (aElt + i + gDimX * j + gDimX * gDimY * k)))
                    rsDebug("test_root_output at i, j, k", i, j, k);
            }
        }
    }

    if (failed) {
        rsDebug("kernel3d test_root_output FAILED", 0);
    }
    else {
        rsDebug("kernel3d test_root_output PASSED", 0);
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
