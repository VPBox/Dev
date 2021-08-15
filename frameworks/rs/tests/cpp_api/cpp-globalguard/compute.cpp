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

// This file is based on frameworks/rs/tests/cpp_api/cppallocation/compute.cpp.

#include "RenderScript.h"

#include "ScriptC_multiply.h"

sp<RS> rs;
sp<const Element> e;
sp<const Type> t;
sp<Allocation> ain;
sp<Allocation> aout;
sp<ScriptC_multiply> sc;

int main(int argc, char** argv)
{
    uint32_t numElems = 1024;

    if (argc >= 2) {
        int tempNumElems = atoi(argv[1]);
        if (tempNumElems < 1) {
            printf("numElems must be greater than 0\n");
            return 1;
        }
        numElems = (uint32_t) tempNumElems;
    }

    rs = new RS();

    if (!rs->init("/system/bin")) {
        printf("Could not initialize RenderScript\n");
        return 1;
    }

    e = Element::U32(rs);

    Type::Builder tb(rs, e);
    tb.setX(numElems);
    t = tb.create();

    ain = Allocation::createTyped(rs, t);
    aout = Allocation::createTyped(rs, t);

    sc = new ScriptC_multiply(rs);

    uint32_t* buf = new uint32_t[numElems];
    for (uint32_t ct=0; ct < numElems; ct++) {
        buf[ct] = (uint32_t)ct;
    }

    ain->copy1DRangeFrom(0, numElems, buf);

    sc->forEach_multiply(ain, aout);

    aout->copy1DRangeTo(0, numElems, buf);

    for (uint32_t ct=0; ct < numElems; ct++) {
        if (buf[ct] !=  ct * 2) {
            printf("Mismatch at location %d: %u\n", ct, buf[ct]);
            return 1;
        }
    }

    printf("Test successful with %u elems!\n", numElems);
}
