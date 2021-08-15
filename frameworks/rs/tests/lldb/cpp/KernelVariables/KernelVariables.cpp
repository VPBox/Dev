/*
* Copyright (C) 2016 The Android Open Source Project
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

#include <RenderScript.h>

#include "ScriptC_simple.h"

int main()
{
    static const int size = 64;
    sp<RS> rs = new RS();

    rs->init("/data/rscache", RS_INIT_LOW_LATENCY | RS_INIT_WAIT_FOR_ATTACH);

    auto e = Element::RGBA_8888(rs);
    Type::Builder tb(rs, e);
    tb.setX(size);
    tb.setY(size);
    auto t = tb.create();

    auto a = Allocation::createTyped(rs, t);
    auto b = Allocation::createTyped(rs, t);

    sp<ScriptC_simple> s = new ScriptC_simple(rs);

    static const int buffer_int[] = {1, 2, 3, 4};
    sp<Allocation> int_allocation = Allocation::createSized(rs, Element::I32(rs), 4);
    int_allocation->copy1DRangeFrom(0, 4, buffer_int);
    s->set_allocation_1D_global(int_allocation);

    static const int buffer_int2[] = {5, 6, 7, 8};

    Type::Builder typeI32Builder2D(rs, Element::I32(rs));
    typeI32Builder2D.setX(2);
    typeI32Builder2D.setY(2);

    sp<Allocation> int_allocation2 = Allocation::createTyped(rs, typeI32Builder2D.create());
    int_allocation2->copy2DRangeFrom(0, 0, 2, 2, buffer_int2);
    s->set_allocation_1D_global2(int_allocation2);

    s->set_allocation_2D_global(a);
    s->set_allocation_2D_global2(b);

    static const int buffer_int3[] = {9, 10, 11, 12, 13, 14, 15, 16};

    Type::Builder typeI32Builder3D(rs, Element::I32(rs));
    typeI32Builder3D.setX(2);
    typeI32Builder3D.setY(2);
    typeI32Builder3D.setZ(2);

    sp<Allocation> int_allocation3 = Allocation::createTyped(rs, typeI32Builder3D.create());
    int_allocation3->copy3DRangeFrom(0, 0, 0, 2, 2, 2, buffer_int3);
    s->set_allocation_3D_global(int_allocation3);

    Type::Builder yuvTypeBuilder(rs, Element::YUV(rs));
    yuvTypeBuilder.setX(4);
    yuvTypeBuilder.setY(4);
    yuvTypeBuilder.setYuvFormat(RS_YUV_YV12);

    sp<Allocation> yuv_allocation = Allocation::createTyped(rs, yuvTypeBuilder.create());
    s->set_allocation_YUV_2D_global(yuv_allocation);

    s->set_sampler_global(Sampler::CLAMP_LINEAR(rs));

    // Script is executed once, then the data is copied back when finished
    s->forEach_kernel(a, b);
    rs->finish();
    uint32_t * output = new uint32_t[size*size];
    b->copy2DRangeTo(0, 0, size, size, output);
    delete [] output;

    return 0;
}

