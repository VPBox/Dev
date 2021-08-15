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

#include "ScriptC_allocs.h"

sp<RS> mRS;
sp<Allocation> mBoolAllocation;  // boolean

sp<Allocation> mCharAllocation;  // char
sp<Allocation> mChar2Allocation;   // char2
sp<Allocation> mChar3Allocation;   // char3
sp<Allocation> mChar4Allocation;   // char4

sp<Allocation> mUCharAllocation;   // uchar
sp<Allocation> mUChar2Allocation;  // uchar2
sp<Allocation> mUChar3Allocation;  // uchar3
sp<Allocation> mUChar4Allocation;  // uchar4

sp<Allocation> mShortAllocation;   // short
sp<Allocation> mShort2Allocation;  // short2
sp<Allocation> mShort3Allocation;  // short3
sp<Allocation> mShort4Allocation;  // short4

sp<Allocation> mUShortAllocation;  // ushort
sp<Allocation> mUShort2Allocation; // ushort2
sp<Allocation> mUShort3Allocation; // ushort3
sp<Allocation> mUShort4Allocation; // ushort4

sp<Allocation> mIntAllocation;   // int
sp<Allocation> mInt2Allocation;  // int2
sp<Allocation> mInt3Allocation;  // int3
sp<Allocation> mInt4Allocation;  // int4

sp<Allocation> mUIntAllocation;  // uint
sp<Allocation> mUInt2Allocation;   // uint2
sp<Allocation> mUInt3Allocation;   // uint3
sp<Allocation> mUInt4Allocation;   // uint4

sp<Allocation> mLongAllocation;  // long
sp<Allocation> mLong2Allocation;   // long2
sp<Allocation> mLong3Allocation;   // long3
sp<Allocation> mLong4Allocation;   // long4

sp<Allocation> mULongAllocation;   // ulong
sp<Allocation> mULong2Allocation;  // ulong2
sp<Allocation> mULong3Allocation;  // ulong3
sp<Allocation> mULong4Allocation;  // ulong4

sp<Allocation> mHalfAllocation;  // half
sp<Allocation> mHalf2Allocation;   // half2
sp<Allocation> mHalf3Allocation;   // half3
sp<Allocation> mHalf4Allocation;   // half4

sp<Allocation> mFloatAllocation;   // float
sp<Allocation> mFloat2Allocation;  // float2
sp<Allocation> mFloat3Allocation;  // float3
sp<Allocation> mFloat4Allocation;  // float4

sp<Allocation> mDoubleAllocation;  // double
sp<Allocation> mDouble2Allocation; // double2
sp<Allocation> mDouble3Allocation; // double3
sp<Allocation> mDouble4Allocation; // double4

const int mAllocSize = 24; // Needs to be < CHAR_MAX and divisible by 4.
const int mBitmapSize = 64;

void createSignedAllocations() {
    Type::Builder typeI8Builder(mRS, Element::I8(mRS));
    typeI8Builder.setX(1); // One element here to test 16 byte memory alignment
    typeI8Builder.setY(3);
    typeI8Builder.setZ(8);

    mCharAllocation = Allocation::createTyped(mRS, typeI8Builder.create());
    mChar2Allocation = Allocation::createSized(mRS, Element::I8_2(mRS), mAllocSize / 2);
    mChar3Allocation = Allocation::createSized(mRS, Element::I8_3(mRS), mAllocSize / 4);
    mChar4Allocation = Allocation::createSized(mRS, Element::I8_4(mRS), mAllocSize / 4);

    Type::Builder typeI16_2Builder(mRS, Element::I16_2(mRS));
    typeI16_2Builder.setX(6);
    typeI16_2Builder.setY(1);
    typeI16_2Builder.setZ(2);

    mShortAllocation = Allocation::createSized(mRS, Element::I16(mRS), mAllocSize);
    mShort2Allocation = Allocation::createTyped(mRS, typeI16_2Builder.create());
    mShort3Allocation = Allocation::createSized(mRS, Element::I16_3(mRS), mAllocSize / 4);
    mShort4Allocation = Allocation::createSized(mRS, Element::I16_4(mRS), mAllocSize / 4);

    Type::Builder typeI32_3Builder(mRS, Element::I32_3(mRS));
    typeI32_3Builder.setX(3);
    typeI32_3Builder.setY(2);

    mIntAllocation = Allocation::createSized(mRS, Element::I32(mRS), mAllocSize);
    mInt2Allocation = Allocation::createSized(mRS, Element::I32_2(mRS), mAllocSize / 2);
    mInt3Allocation = Allocation::createTyped(mRS, typeI32_3Builder.create());
    mInt4Allocation = Allocation::createSized(mRS, Element::I32_4(mRS), mAllocSize / 4);

    Type::Builder typeI64_4Builder(mRS, Element::I64_4(mRS));
    typeI64_4Builder.setX(1);
    typeI64_4Builder.setY(6);

    mLongAllocation = Allocation::createSized(mRS, Element::I64(mRS), mAllocSize);
    mLong2Allocation = Allocation::createSized(mRS, Element::I64_2(mRS), mAllocSize / 2);
    mLong3Allocation = Allocation::createSized(mRS, Element::I64_3(mRS), mAllocSize / 4);
    mLong4Allocation = Allocation::createTyped(mRS, typeI64_4Builder.create());

    mBoolAllocation = Allocation::createSized(mRS, Element::BOOLEAN(mRS), mAllocSize);
}

void initSignedAllocations() {
    char *buffer_char = new char[mAllocSize];
    short *buffer_short = new short[mAllocSize];
    int *buffer_int = new int[mAllocSize];
    int64_t *buffer_long = new int64_t[mAllocSize];
    char *buffer_bool = new char[mAllocSize];

    for(int i = 0; i < mAllocSize; ++i) {
        buffer_char[i] = (char) i;
        buffer_short[i] = (short) i;
        buffer_int[i] = (int) i;
        buffer_long[i] = (int64_t) i;
        buffer_bool[i] =  (char) (0x01 & i);
    }

    mCharAllocation->copy3DRangeFrom(0, 0, 0, 1, 3, 8, buffer_char);
    mChar2Allocation->copy1DRangeFrom(0, mAllocSize/2, buffer_char);
    mChar3Allocation->copy1DRangeFrom(0, mAllocSize/4, buffer_char);
    mChar4Allocation->copy1DRangeFrom(0, mAllocSize/4, buffer_char);

    delete [] buffer_char;

    mShortAllocation->copy1DRangeFrom(0, mAllocSize, buffer_short);
    mShort2Allocation->copy3DRangeFrom(0, 0, 0, 6, 1, 2, buffer_short);
    mShort3Allocation->copy1DRangeFrom(0, mAllocSize/4, buffer_short);
    mShort4Allocation->copy1DRangeFrom(0, mAllocSize/4, buffer_short);

    delete [] buffer_short;

    mIntAllocation->copy1DRangeFrom(0, mAllocSize, buffer_int);
    mInt2Allocation->copy1DRangeFrom(0, mAllocSize/2, buffer_int);
    mInt3Allocation->copy2DRangeFrom(0, 0, 3, 2, buffer_int);
    mInt4Allocation->copy1DRangeFrom(0, mAllocSize/4, buffer_int);

    delete [] buffer_int;

    mLongAllocation->copy1DRangeFrom(0, mAllocSize, buffer_long);
    mLong2Allocation->copy1DRangeFrom(0, mAllocSize/2, buffer_long);
    mLong3Allocation->copy1DRangeFrom(0, mAllocSize/4, buffer_long);
    mLong4Allocation->copy2DRangeFrom(0, 0, 1, 6, buffer_long);

    delete [] buffer_long;

    mBoolAllocation->copy1DRangeFrom(0, mAllocSize, buffer_bool);

    delete [] buffer_bool;
}

void createUnsignedAllocations() {
    Type::Builder typeU8_2Builder(mRS, Element::U8_2(mRS));
    typeU8_2Builder.setX(2);
    typeU8_2Builder.setY(6);

    mUCharAllocation = Allocation::createSized(mRS, Element::U8(mRS), mAllocSize);
    mUChar2Allocation = Allocation::createTyped(mRS, typeU8_2Builder.create());
    mUChar3Allocation = Allocation::createSized(mRS, Element::U8_3(mRS), mAllocSize / 4);
    mUChar4Allocation = Allocation::createSized(mRS, Element::U8_4(mRS), mAllocSize / 4);

    Type::Builder typeU16_3Builder(mRS, Element::U16_3(mRS));
    typeU16_3Builder.setX(1);
    typeU16_3Builder.setY(6);

    mUShortAllocation = Allocation::createSized(mRS, Element::U16(mRS), mAllocSize);
    mUShort2Allocation = Allocation::createSized(mRS, Element::U16_2(mRS), mAllocSize / 2);
    mUShort3Allocation = Allocation::createTyped(mRS, typeU16_3Builder.create());
    mUShort4Allocation = Allocation::createSized(mRS, Element::U16_4(mRS), mAllocSize / 4);

    Type::Builder typeU32_4Builder(mRS, Element::U32_4(mRS));
    typeU32_4Builder.setX(1);
    typeU32_4Builder.setY(1);
    typeU32_4Builder.setZ(6);

    mUIntAllocation = Allocation::createSized(mRS, Element::U32(mRS), mAllocSize);
    mUInt2Allocation = Allocation::createSized(mRS, Element::U32_2(mRS), mAllocSize / 2);
    mUInt3Allocation = Allocation::createSized(mRS, Element::U32_3(mRS), mAllocSize / 4);
    mUInt4Allocation = Allocation::createTyped(mRS, typeU32_4Builder.create());

    Type::Builder typeU64Builder(mRS, Element::U64(mRS));
    typeU64Builder.setX(4);
    typeU64Builder.setY(3);
    typeU64Builder.setZ(2);

    mULongAllocation = Allocation::createTyped(mRS, typeU64Builder.create());
    mULong2Allocation = Allocation::createSized(mRS, Element::U64_2(mRS), mAllocSize / 2);
    mULong3Allocation = Allocation::createSized(mRS, Element::U64_3(mRS), mAllocSize / 4);
    mULong4Allocation = Allocation::createSized(mRS, Element::U64_4(mRS), mAllocSize / 4);
}

void initUnsignedAllocations() {
    char *buffer_char = new char[mAllocSize];
    short *buffer_short = new short[mAllocSize];
    int *buffer_int = new int[mAllocSize];
    uint64_t *buffer_long = new uint64_t[mAllocSize];

    for(int i = 0; i < mAllocSize; ++i) {
        buffer_char[i] = (char) i;
        buffer_short[i] = (short) i;
        buffer_int[i] = (int) i;
        buffer_long[i] = (uint64_t) i;
    }

    mUCharAllocation->copy1DRangeFrom(0, mAllocSize, buffer_char);
    mUChar2Allocation->copy2DRangeFrom(0, 0, 2, 6, buffer_char);
    mUChar3Allocation->copy1DRangeFrom(0, mAllocSize/4, buffer_char);
    mUChar4Allocation->copy1DRangeFrom(0, mAllocSize/4, buffer_char);

    delete [] buffer_char;

    mUShortAllocation->copy1DRangeFrom(0, mAllocSize, buffer_short);
    mUShort2Allocation->copy1DRangeFrom(0, mAllocSize/2, buffer_short);
    mUShort3Allocation->copy2DRangeFrom(0, 0, 1, 6, buffer_short);
    mUShort4Allocation->copy1DRangeFrom(0, mAllocSize/4, buffer_short);

    delete [] buffer_short;

    mUIntAllocation->copy1DRangeFrom(0, mAllocSize, buffer_int);
    mUInt2Allocation->copy1DRangeFrom(0, mAllocSize/2, buffer_int);
    mUInt3Allocation->copy1DRangeFrom(0, mAllocSize/4, buffer_int);
    mUInt4Allocation->copy3DRangeFrom(0, 0, 0, 1, 1, 6, buffer_int);

    delete [] buffer_int;

    mULongAllocation->copy3DRangeFrom(0, 0, 0, 4, 3, 2, buffer_long);
    mULong2Allocation->copy1DRangeFrom(0, mAllocSize/2, buffer_long);
    mULong3Allocation->copy1DRangeFrom(0, mAllocSize/4, buffer_long);
    mULong4Allocation->copy1DRangeFrom(0, mAllocSize/4, buffer_long);

    delete [] buffer_long;
}

void createFloatAllocations() {
    Type::Builder typeF16_3Builder(mRS, Element::F16_3(mRS));
    typeF16_3Builder.setX(1);
    typeF16_3Builder.setY(6);

    mHalfAllocation = Allocation::createSized(mRS, Element::F16(mRS), mAllocSize);
    mHalf2Allocation = Allocation::createSized(mRS, Element::F16_2(mRS), mAllocSize / 2);
    mHalf3Allocation = Allocation::createTyped(mRS, typeF16_3Builder.create());
    mHalf4Allocation = Allocation::createSized(mRS, Element::F16_4(mRS), mAllocSize / 4);

    Type::Builder typeF32_4Builder(mRS, Element::F32_4(mRS));
    typeF32_4Builder.setX(3);
    typeF32_4Builder.setY(2);

    mFloatAllocation = Allocation::createSized(mRS, Element::F32(mRS), mAllocSize);
    mFloat2Allocation = Allocation::createSized(mRS, Element::F32_2(mRS), mAllocSize / 2);
    mFloat3Allocation = Allocation::createSized(mRS, Element::F32_3(mRS), mAllocSize / 4);
    mFloat4Allocation = Allocation::createTyped(mRS, typeF32_4Builder.create());

    Type::Builder typeF64_2Builder(mRS, Element::F64_2(mRS));
    typeF64_2Builder.setX(4);
    typeF64_2Builder.setY(1);
    typeF64_2Builder.setZ(3);

    mDoubleAllocation = Allocation::createSized(mRS, Element::F64(mRS), mAllocSize);
    mDouble2Allocation = Allocation::createTyped(mRS, typeF64_2Builder.create());

    Type::Builder typeF64_3Builder(mRS, Element::F64_3(mRS));
    typeF64_3Builder.setX(1);
    typeF64_3Builder.setY(2);
    typeF64_3Builder.setZ(3);

    Type::Builder typeF64_4Builder(mRS, Element::F64_4(mRS));
    typeF64_4Builder.setX(1);
    typeF64_4Builder.setY(2);
    typeF64_4Builder.setZ(3);

    mDouble3Allocation = Allocation::createTyped(mRS, typeF64_3Builder.create());
    mDouble4Allocation = Allocation::createTyped(mRS, typeF64_4Builder.create());
}

void initFloatAllocations() {
    __fp16 *buffer_half = new __fp16[mAllocSize];
    float *buffer_float = new float[mAllocSize];
    double *buffer_double = new double[mAllocSize];

    for(int i = 0; i < mAllocSize; ++i) {
        buffer_half[i] = (__fp16) 1 / i;
        buffer_float[i] = (float) 1 / i;
        buffer_double[i] = (double) 1 / i;
    }

    mHalfAllocation->copy1DRangeFrom(0, mAllocSize, buffer_half);
    mHalf2Allocation->copy1DRangeFrom(0, mAllocSize/2, buffer_half);
    mHalf3Allocation->copy2DRangeFrom(0, 0, 1, 6, buffer_half);
    mHalf4Allocation->copy1DRangeFrom(0, mAllocSize/4, buffer_half);

    delete [] buffer_half;

    mFloatAllocation->copy1DRangeFrom(0, mAllocSize, buffer_float);
    mFloat2Allocation->copy1DRangeFrom(0, mAllocSize/2, buffer_float);
    mFloat3Allocation->copy1DRangeFrom(0, mAllocSize/4, buffer_float);
    mFloat4Allocation->copy2DRangeFrom(0, 0, 3, 2, buffer_float);

    delete [] buffer_float;

    mDoubleAllocation->copy1DRangeFrom(0, mAllocSize, buffer_double);
    mDouble2Allocation->copy3DRangeFrom(0, 0, 0, 4, 1, 3, buffer_double);
    mDouble3Allocation->copy3DRangeFrom(0, 0, 0, 1, 2, 3, buffer_double);
    mDouble4Allocation->copy3DRangeFrom(0, 0, 0, 1, 2, 3, buffer_double);

    delete [] buffer_double;
}

int main()
{
    mRS = new RS();

    mRS->init("/data/rscache", RS_INIT_LOW_LATENCY | RS_INIT_WAIT_FOR_ATTACH);

    sp<ScriptC_allocs> mScript = new ScriptC_allocs(mRS);

    Type::Builder typeRGBA_888Builder(mRS, Element::RGBA_8888(mRS));
    typeRGBA_888Builder.setX(mBitmapSize);
    typeRGBA_888Builder.setY(mBitmapSize);

    sp<Allocation> mInAllocation = Allocation::createTyped(mRS, typeRGBA_888Builder.create());

    const int image_area = mBitmapSize*mBitmapSize;
    const int image_size = image_area*sizeof(int);

    char *zero_buffer = new char[image_size];
    memset(zero_buffer, 0, image_size);
    mInAllocation->copy1DRangeFrom(0, image_area, zero_buffer);
    delete [] zero_buffer;

    sp<Allocation> mOutAllocation = Allocation::createTyped(mRS, typeRGBA_888Builder.create());
    createSignedAllocations();
    initSignedAllocations();

    mRS->finish();
    mScript->forEach_swizzle_kernel(mInAllocation, mOutAllocation);
    mRS->finish();

    mCharAllocation.clear();
    mChar2Allocation.clear();
    mChar3Allocation.clear();
    mChar4Allocation.clear();

    mShort2Allocation.clear();
    mShort3Allocation.clear();
    mShort4Allocation.clear();

    mIntAllocation.clear();
    mInt2Allocation.clear();
    mInt3Allocation.clear();
    mInt4Allocation.clear();

    mLongAllocation.clear();
    mLong2Allocation.clear();
    mLong3Allocation.clear();
    mLong4Allocation.clear();

    mBoolAllocation.clear();

    createUnsignedAllocations();
    initUnsignedAllocations();

    mInAllocation = mUShortAllocation; // Host side assignment

    mRS->finish();
    mScript->forEach_square_kernel(mInAllocation, mUIntAllocation);
    mRS->finish();

    mUCharAllocation.clear();
    mUChar2Allocation.clear();
    mUChar3Allocation.clear();
    mUChar4Allocation.clear();

    mUShortAllocation.clear();
    mUShort2Allocation.clear();
    mUShort3Allocation.clear();
    mUShort4Allocation.clear();

    mUInt2Allocation.clear();
    mUInt3Allocation.clear();
    mUInt4Allocation.clear();

    mULongAllocation.clear();
    mULong2Allocation.clear();
    mULong3Allocation.clear();
    mULong4Allocation.clear();

    createFloatAllocations();
    initFloatAllocations();

    mRS->finish();
    mScript->forEach_add_half_kernel(mDouble4Allocation, mDouble3Allocation);
    mRS->finish();

    return 0;
}

