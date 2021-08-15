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

int dimX;
int dimY;

rs_allocation gAllocOut;

void oldFoo(const int* in, int* out, uint32_t x, uint32_t y) {
  out[0] = in[0] * 3;
}

int __attribute__((kernel)) foo(int a) {
    return a * 2;
}

void oldGoo(const int* a, int* out) {
  *out = *a + 47;
}

int __attribute__((kernel)) goo(int a, int b) {
    return a + b;
}

void __attribute__((kernel)) bar(int x, int y) {
  int a = rsGetElementAt_int(gAllocOut, x, y);
  a++;
  rsSetElementAt_int(gAllocOut, a, x, y);
}

void validate(rs_allocation out) {
    bool failed = false;

    int i, j;

    for (j = 0; j < dimY; j++) {
        for (i = 0; i < dimX; i++) {
            const int actual = rsGetElementAt_int(out, i, j);
            int expected = (i + j * dimX) * 4;
            if (j < dimY / 2) {
                expected *= 2;
            }
            expected += (i + j * dimX) + 1;
            expected *= 3;
            expected += 47;
            if (actual != expected) {
                failed = true;
                rsDebug("row     ", j);
                rsDebug("column  ", i);
                rsDebug("expects ", expected);
                rsDebug("got     ", actual);
            }
        }
    }

    if (failed) {
        rsDebug("FAILED", 0);
    } else {
        rsDebug("PASSED", 0);
    }

    if (failed) {
        rsSendToClientBlocking(RS_MSG_TEST_FAILED);
    } else {
        rsSendToClientBlocking(RS_MSG_TEST_PASSED);
    }
}

void entrypoint(rs_allocation in, rs_allocation out) {
    int i, j;
    for (i = 0; i < dimX; i++) {
        for (j = 0; j < dimY; j++) {
            rsSetElementAt_int(in, j * dimX + i, i, j);
        }
    }

    rsForEach(foo, in, out);
    rsForEach(foo, out, out);

    rs_script_call_t opts = {0};
    opts.xStart = 0;
    opts.xEnd = dimX;
    opts.yStart = 0;
    opts.yEnd = dimY / 2;
    rsForEachWithOptions(foo, &opts, out, out);

    rsForEach(goo, in, out, out);

    gAllocOut = out;
    rs_script_call_t opts2 = {0};
    opts2.xStart = 0;
    opts2.xEnd = dimX;
    opts2.yStart = 0;
    opts2.yEnd = dimY;
    rsForEachWithOptions(bar, &opts2);
}
