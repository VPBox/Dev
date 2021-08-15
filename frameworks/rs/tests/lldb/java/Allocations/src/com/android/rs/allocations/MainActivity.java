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

package com.android.rs.allocations;

import android.app.Activity;
import android.os.Bundle;
import android.graphics.Bitmap;
import android.renderscript.*;

public class MainActivity extends Activity {
    private RenderScript mRS;

    private Allocation mInAllocation;      // script input
    private Allocation mOutAllocation;     // script output

    private Allocation mStructInAlloc;     // complexStruct input
    private Allocation mStructOutAlloc;    // complexStruct output

    private Allocation mBoolAllocation;    // boolean

    private Allocation mCharAllocation;    // char
    private Allocation mChar2Allocation;   // char2
    private Allocation mChar3Allocation;   // char3
    private Allocation mChar4Allocation;   // char4

    private Allocation mUCharAllocation;   // uchar
    private Allocation mUChar2Allocation;  // uchar2
    private Allocation mUChar3Allocation;  // uchar3
    private Allocation mUChar4Allocation;  // uchar4

    private Allocation mShortAllocation;   // short
    private Allocation mShort2Allocation;  // short2
    private Allocation mShort3Allocation;  // short3
    private Allocation mShort4Allocation;  // short4

    private Allocation mUShortAllocation;  // ushort
    private Allocation mUShort2Allocation; // ushort2
    private Allocation mUShort3Allocation; // ushort3
    private Allocation mUShort4Allocation; // ushort4

    private Allocation mIntAllocation;     // int
    private Allocation mInt2Allocation;    // int2
    private Allocation mInt3Allocation;    // int3
    private Allocation mInt4Allocation;    // int4

    private Allocation mUIntAllocation;    // uint
    private Allocation mUInt2Allocation;   // uint2
    private Allocation mUInt3Allocation;   // uint3
    private Allocation mUInt4Allocation;   // uint4

    private Allocation mLongAllocation;    // long
    private Allocation mLong2Allocation;   // long2
    private Allocation mLong3Allocation;   // long3
    private Allocation mLong4Allocation;   // long4

    private Allocation mULongAllocation;   // ulong
    private Allocation mULong2Allocation;  // ulong2
    private Allocation mULong3Allocation;  // ulong3
    private Allocation mULong4Allocation;  // ulong4

    private Allocation mHalfAllocation;    // half
    private Allocation mHalf2Allocation;   // half2
    private Allocation mHalf3Allocation;   // half3
    private Allocation mHalf4Allocation;   // half4

    private Allocation mFloatAllocation;   // float
    private Allocation mFloat2Allocation;  // float2
    private Allocation mFloat3Allocation;  // float3
    private Allocation mFloat4Allocation;  // float4

    private Allocation mDoubleAllocation;  // double
    private Allocation mDouble2Allocation; // double2
    private Allocation mDouble3Allocation; // double3
    private Allocation mDouble4Allocation; // double4

    private ScriptC_allocs mScript;

    private int mAllocSize = 24; // Chosen as allocation size since it's easily divisible

    private Bitmap mBitmapIn;
    private Bitmap mBitmapOut;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.main_layout);

        mBitmapIn = Bitmap.createBitmap(64, 64, Bitmap.Config.ARGB_8888);
        mBitmapOut = Bitmap.createBitmap(mBitmapIn.getWidth(), mBitmapIn.getHeight(), mBitmapIn.getConfig());

        createScript();
        runScript();
    }

    private void createScript() {
        mRS = RenderScript.create(this,
            RenderScript.ContextType.NORMAL,
            RenderScript.CREATE_FLAG_LOW_LATENCY |
            RenderScript.CREATE_FLAG_WAIT_FOR_ATTACH);

        mScript = new ScriptC_allocs(mRS);
    }

    private void createSignedAllocations() {
        Type.Builder typeI8Builder = new Type.Builder(mRS, Element.I8(mRS));
        typeI8Builder.setX(1); // One element here to test 16 byte memory alignment
        typeI8Builder.setY(3);
        typeI8Builder.setZ(8);

        mCharAllocation = Allocation.createTyped(mRS, typeI8Builder.create());
        mRS.finish();
        mChar2Allocation = Allocation.createSized(mRS, Element.I8_2(mRS), mAllocSize / 2);
        mRS.finish();
        mChar3Allocation = Allocation.createSized(mRS, Element.I8_3(mRS), mAllocSize / 4);
        mRS.finish();
        mChar4Allocation = Allocation.createSized(mRS, Element.I8_4(mRS), mAllocSize / 4);
        mRS.finish();

        Type.Builder typeI16_2Builder = new Type.Builder(mRS, Element.I16_2(mRS));
        typeI16_2Builder.setX(6);
        typeI16_2Builder.setY(1);
        typeI16_2Builder.setZ(2);

        mShortAllocation = Allocation.createSized(mRS, Element.I16(mRS), mAllocSize);
        mRS.finish();
        mShort2Allocation = Allocation.createTyped(mRS, typeI16_2Builder.create());
        mRS.finish();
        mShort3Allocation = Allocation.createSized(mRS, Element.I16_3(mRS), mAllocSize / 4);
        mRS.finish();
        mShort4Allocation = Allocation.createSized(mRS, Element.I16_4(mRS), mAllocSize / 4);
        mRS.finish();

        Type.Builder typeI32_3Builder = new Type.Builder(mRS, Element.I32_3(mRS));
        typeI32_3Builder.setX(3);
        typeI32_3Builder.setY(2);

        mIntAllocation = Allocation.createSized(mRS, Element.I32(mRS), mAllocSize);
        mRS.finish();
        mInt2Allocation = Allocation.createSized(mRS, Element.I32_2(mRS), mAllocSize / 2);
        mRS.finish();
        mInt3Allocation = Allocation.createTyped(mRS, typeI32_3Builder.create());
        mRS.finish();
        mInt4Allocation = Allocation.createSized(mRS, Element.I32_4(mRS), mAllocSize / 4);
        mRS.finish();

        Type.Builder typeI64_4Builder = new Type.Builder(mRS, Element.I64_4(mRS));
        typeI64_4Builder.setX(1);
        typeI64_4Builder.setY(6);

        mLongAllocation = Allocation.createSized(mRS, Element.I64(mRS), mAllocSize);
        mRS.finish();
        mLong2Allocation = Allocation.createSized(mRS, Element.I64_2(mRS), mAllocSize / 2);
        mRS.finish();
        mLong3Allocation = Allocation.createSized(mRS, Element.I64_3(mRS), mAllocSize / 4);
        mRS.finish();
        mLong4Allocation = Allocation.createTyped(mRS, typeI64_4Builder.create());
        mRS.finish();

        mBoolAllocation = Allocation.createSized(mRS, Element.BOOLEAN(mRS), mAllocSize);
        mRS.finish();
    }

    private void initSignedAllocations() {
        byte[] buffer_char = new byte[mAllocSize];
        short[] buffer_short = new short[mAllocSize];
        int[] buffer_int = new int[mAllocSize];
        long[] buffer_long = new long[mAllocSize];
        byte[] buffer_bool = new byte[mAllocSize];

        for(int i = 0; i < mAllocSize; ++i) {
            buffer_char[i] = (byte) i;
            buffer_short[i] = (short) i;
            buffer_int[i] = (int) i;
            buffer_long[i] = (long) i;
            buffer_bool[i] =  (byte) (0x01 & i);
        }

        mCharAllocation.copyFrom(buffer_char);
        mChar2Allocation.copyFrom(buffer_char);
        mChar3Allocation.copyFrom(buffer_char);
        mChar4Allocation.copyFrom(buffer_char);

        mShortAllocation.copyFrom(buffer_short);
        mShort2Allocation.copyFrom(buffer_short);
        mShort3Allocation.copyFrom(buffer_short);
        mShort4Allocation.copyFrom(buffer_short);

        mIntAllocation.copyFrom(buffer_int);
        mInt2Allocation.copyFrom(buffer_int);
        mInt3Allocation.copyFrom(buffer_int);
        mInt4Allocation.copyFrom(buffer_int);

        mLongAllocation.copyFrom(buffer_long);
        mLong2Allocation.copyFrom(buffer_long);
        mLong3Allocation.copyFrom(buffer_long);
        mLong4Allocation.copyFrom(buffer_long);

        mBoolAllocation.copyFromUnchecked(buffer_bool);
    }

    private void createUnsignedAllocations() {
        Type.Builder typeU8_2Builder = new Type.Builder(mRS, Element.U8_2(mRS));
        typeU8_2Builder.setX(2);
        typeU8_2Builder.setY(6);

        mUCharAllocation = Allocation.createSized(mRS, Element.U8(mRS), mAllocSize);
        mRS.finish();
        mUChar2Allocation = Allocation.createTyped(mRS, typeU8_2Builder.create());
        mRS.finish();
        mUChar3Allocation = Allocation.createSized(mRS, Element.U8_3(mRS), mAllocSize / 4);
        mRS.finish();
        mUChar4Allocation = Allocation.createSized(mRS, Element.U8_4(mRS), mAllocSize / 4);
        mRS.finish();

        Type.Builder typeU16_3Builder = new Type.Builder(mRS, Element.U16_3(mRS));
        typeU16_3Builder.setX(1);
        typeU16_3Builder.setY(6);

        mUShortAllocation = Allocation.createSized(mRS, Element.U16(mRS), mAllocSize);
        mRS.finish();
        mUShort2Allocation = Allocation.createSized(mRS, Element.U16_2(mRS), mAllocSize / 2);
        mRS.finish();
        mUShort3Allocation = Allocation.createTyped(mRS, typeU16_3Builder.create());
        mRS.finish();
        mUShort4Allocation = Allocation.createSized(mRS, Element.U16_4(mRS), mAllocSize / 4);
        mRS.finish();

        Type.Builder typeU32_4Builder = new Type.Builder(mRS, Element.U32_4(mRS));
        typeU32_4Builder.setX(1);
        typeU32_4Builder.setY(1);
        typeU32_4Builder.setZ(6);

        mUIntAllocation = Allocation.createSized(mRS, Element.U32(mRS), mAllocSize);
        mRS.finish();
        mUInt2Allocation = Allocation.createSized(mRS, Element.U32_2(mRS), mAllocSize / 2);
        mRS.finish();
        mUInt3Allocation = Allocation.createSized(mRS, Element.U32_3(mRS), mAllocSize / 4);
        mRS.finish();
        mUInt4Allocation = Allocation.createTyped(mRS, typeU32_4Builder.create());
        mRS.finish();

        Type.Builder typeU64Builder = new Type.Builder(mRS, Element.U64(mRS));
        typeU64Builder.setX(4);
        typeU64Builder.setY(3);
        typeU64Builder.setZ(2);

        mULongAllocation = Allocation.createTyped(mRS, typeU64Builder.create());
        mRS.finish();
        mULong2Allocation = Allocation.createSized(mRS, Element.U64_2(mRS), mAllocSize / 2);
        mRS.finish();
        mULong3Allocation = Allocation.createSized(mRS, Element.U64_3(mRS), mAllocSize / 4);
        mRS.finish();
        mULong4Allocation = Allocation.createSized(mRS, Element.U64_4(mRS), mAllocSize / 4);
        mRS.finish();
    }

    private void initUnsignedAllocations() {
        byte[] buffer_char = new byte[mAllocSize];
        short[] buffer_short = new short[mAllocSize];
        int[] buffer_int = new int[mAllocSize];
        long[] buffer_long = new long[mAllocSize];

        for(int i = 0; i < mAllocSize; ++i) {
            buffer_char[i] = (byte) i;
            buffer_short[i] = (short) i;
            buffer_int[i] = (int) i;
            buffer_long[i] = (long) i;
        }

        mUCharAllocation.copyFrom(buffer_char);
        mUChar2Allocation.copyFrom(buffer_char);
        mUChar3Allocation.copyFrom(buffer_char);
        mUChar4Allocation.copyFrom(buffer_char);

        mUShortAllocation.copyFrom(buffer_short);
        mUShort2Allocation.copyFrom(buffer_short);
        mUShort3Allocation.copyFrom(buffer_short);
        mUShort4Allocation.copyFrom(buffer_short);

        mUIntAllocation.copyFrom(buffer_int);
        mUInt2Allocation.copyFrom(buffer_int);
        mUInt3Allocation.copyFrom(buffer_int);
        mUInt4Allocation.copyFrom(buffer_int);

        mULongAllocation.copyFrom(buffer_long);
        mULong2Allocation.copyFrom(buffer_long);
        mULong3Allocation.copyFrom(buffer_long);
        mULong4Allocation.copyFrom(buffer_long);
    }

    private void createFloatAllocations() {
        Type.Builder typeF16_3Builder = new Type.Builder(mRS, Element.F16_3(mRS));
        typeF16_3Builder.setX(1);
        typeF16_3Builder.setY(6);

        mHalfAllocation = Allocation.createSized(mRS, Element.F16(mRS), mAllocSize);
        mRS.finish();
        mHalf2Allocation = Allocation.createSized(mRS, Element.F16_2(mRS), mAllocSize / 2);
        mRS.finish();
        mHalf3Allocation = Allocation.createTyped(mRS, typeF16_3Builder.create());
        mRS.finish();
        mHalf4Allocation = Allocation.createSized(mRS, Element.F16_4(mRS), mAllocSize / 4);
        mRS.finish();

        Type.Builder typeF32_4Builder = new Type.Builder(mRS, Element.F32_4(mRS));
        typeF32_4Builder.setX(3);
        typeF32_4Builder.setY(2);

        mFloatAllocation = Allocation.createSized(mRS, Element.F32(mRS), mAllocSize);
        mRS.finish();
        mFloat2Allocation = Allocation.createSized(mRS, Element.F32_2(mRS), mAllocSize / 2);
        mRS.finish();
        mFloat3Allocation = Allocation.createSized(mRS, Element.F32_3(mRS), mAllocSize / 4);
        mRS.finish();
        mFloat4Allocation = Allocation.createTyped(mRS, typeF32_4Builder.create());
        mRS.finish();

        Type.Builder typeF64_2Builder = new Type.Builder(mRS, Element.F64_2(mRS));
        typeF64_2Builder.setX(4);
        typeF64_2Builder.setY(1);
        typeF64_2Builder.setZ(3);

        mDoubleAllocation = Allocation.createSized(mRS, Element.F64(mRS), mAllocSize);
        mRS.finish();
        mDouble2Allocation = Allocation.createTyped(mRS, typeF64_2Builder.create());
        mRS.finish();

        Type.Builder typeF64_3Builder = new Type.Builder(mRS, Element.F64_3(mRS));
        typeF64_3Builder.setX(1);
        typeF64_3Builder.setY(2);
        typeF64_3Builder.setZ(3);

        Type.Builder typeF64_4Builder = new Type.Builder(mRS, Element.F64_4(mRS));
        typeF64_4Builder.setX(1);
        typeF64_4Builder.setY(2);
        typeF64_4Builder.setZ(3);

        mDouble3Allocation = Allocation.createTyped(mRS, typeF64_3Builder.create());
        mRS.finish();
        mDouble4Allocation = Allocation.createTyped(mRS, typeF64_4Builder.create());
        mRS.finish();
    }

    private void initFloatAllocations() {
        // No java type for half precision float, so bitcast 16-bit int
        short[] buffer_half = new short[mAllocSize];
        float[] buffer_float = new float[mAllocSize];
        double[] buffer_double = new double[mAllocSize];

        for(int i = 0; i < mAllocSize; ++i) {
            // Construct IEEE 754 half with increasing fraction.
            byte mantissa = (byte)(i);
            byte exponent = 0b00111100; // keep exponent constant at one
            buffer_half[i] = (short)((exponent << 8) | mantissa);

            buffer_float[i] = (float) 1 / i;
            buffer_double[i] = (double) 1 / i;
        }

        mHalfAllocation.copyFromUnchecked(buffer_half);
        mHalf2Allocation.copyFromUnchecked(buffer_half);
        mHalf3Allocation.copyFromUnchecked(buffer_half);
        mHalf4Allocation.copyFromUnchecked(buffer_half);

        mFloatAllocation.copyFrom(buffer_float);
        mFloat2Allocation.copyFrom(buffer_float);
        mFloat3Allocation.copyFrom(buffer_float);
        mFloat4Allocation.copyFrom(buffer_float);

        mDoubleAllocation.copyFrom(buffer_double);
        mDouble2Allocation.copyFrom(buffer_double);
        mDouble3Allocation.copyFrom(buffer_double);
        mDouble4Allocation.copyFrom(buffer_double);
    }

    private void createStructAllocations() {
        ScriptField_complexStruct complex_struct;

        complex_struct = new ScriptField_complexStruct(mRS, mAllocSize);
        mRS.finish();
        mScript.bind_g_complexStruct_in(complex_struct);
        mRS.finish();
        mStructInAlloc = complex_struct.getAllocation();
        mRS.finish();

        complex_struct = new ScriptField_complexStruct(mRS, mAllocSize);
        mRS.finish();
        mScript.bind_g_complexStruct_out(complex_struct);
        mRS.finish();
        mStructOutAlloc = complex_struct.getAllocation();
        mRS.finish();
    }

    private void overwriteFloatAllocations() {
        float[] buffer_float = new float[mAllocSize];

        // Set float allocations to -1/n
        for(int i = 0; i < mAllocSize; ++i) {
            buffer_float[i] = -1f / i;
        }

        mFloatAllocation.copyFrom(buffer_float);
        mFloat2Allocation.copyFrom(buffer_float);
        mFloat3Allocation.copyFrom(buffer_float);
        mFloat4Allocation.copyFrom(buffer_float);
    }

    private void runScript() {
        mInAllocation = Allocation.createFromBitmap(mRS, mBitmapIn);
        mRS.finish();
        mOutAllocation = Allocation.createFromBitmap(mRS, mBitmapOut);
        mRS.finish();


        createSignedAllocations();
        initSignedAllocations();

        mRS.finish();
        mScript.forEach_swizzle_kernel(mInAllocation, mOutAllocation);
        mRS.finish();

        mOutAllocation.copyTo(mBitmapOut);

        mCharAllocation.destroy();
        mRS.finish();
        mChar2Allocation.destroy();
        mRS.finish();
        mChar3Allocation.destroy();
        mRS.finish();
        mChar4Allocation.destroy();
        mRS.finish();

        mShort2Allocation.destroy();
        mRS.finish();
        mShort3Allocation.destroy();
        mRS.finish();
        mShort4Allocation.destroy();
        mRS.finish();

        mIntAllocation.destroy();
        mRS.finish();
        mInt2Allocation.destroy();
        mRS.finish();
        mInt3Allocation.destroy();
        mRS.finish();
        mInt4Allocation.destroy();
        mRS.finish();

        mLongAllocation.destroy();
        mRS.finish();
        mLong2Allocation.destroy();
        mRS.finish();
        mLong3Allocation.destroy();
        mRS.finish();
        mLong4Allocation.destroy();
        mRS.finish();

        mBoolAllocation.destroy();
        mRS.finish();


        createUnsignedAllocations();
        initUnsignedAllocations();

        mInAllocation = mUShortAllocation; // Host side assignment

        mRS.finish();
        mScript.forEach_square_kernel(mInAllocation, mUIntAllocation);
        mRS.finish();

        mUCharAllocation.destroy();
        mRS.finish();
        mUChar2Allocation.destroy();
        mRS.finish();
        mUChar3Allocation.destroy();
        mRS.finish();
        mUChar4Allocation.destroy();
        mRS.finish();

        mUShortAllocation.destroy();
        mRS.finish();
        mUShort2Allocation.destroy();
        mRS.finish();
        mUShort3Allocation.destroy();
        mRS.finish();
        mUShort4Allocation.destroy();
        mRS.finish();

        mUInt2Allocation.destroy();
        mRS.finish();
        mUInt3Allocation.destroy();
        mRS.finish();
        mUInt4Allocation.destroy();
        mRS.finish();

        mULongAllocation.destroy();
        mRS.finish();
        mULong2Allocation.destroy();
        mRS.finish();
        mULong3Allocation.destroy();
        mRS.finish();
        mULong4Allocation.destroy();
        mRS.finish();


        createFloatAllocations();
        initFloatAllocations();

        mRS.finish();
        mScript.forEach_add_half_kernel(mDouble4Allocation, mDouble3Allocation);
        mRS.finish();

        mHalfAllocation.destroy();
        mRS.finish();
        mHalf2Allocation.destroy();
        mRS.finish();
        mHalf3Allocation.destroy();
        mRS.finish();
        mHalf4Allocation.destroy();
        mRS.finish();

        mDoubleAllocation.destroy();
        mRS.finish();
        mDouble2Allocation.destroy();
        mRS.finish();
        mDouble4Allocation.destroy();
        mRS.finish();

        overwriteFloatAllocations();

        createStructAllocations();

        mRS.finish();
        mScript.forEach_struct_kernel(mStructInAlloc, mStructOutAlloc);
        mRS.finish();
    }
}
