/*
 * Copyright (C) 2015 The Android Open Source Project
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

package com.example.android.rs.blasbenchmark;

import android.renderscript.*;
import android.util.Log;
import java.util.ArrayList;
import java.util.Random;
import java.lang.Math;

public class GoogLeNet extends TestBase {
    ScriptIntrinsicBLAS mBLAS;
    private ArrayList<Allocation> matA;
    private ArrayList<Allocation> matB;
    private ArrayList<Allocation> matC;

    private Random mRand=new Random();
    private int usage;
    // Output data array
    public byte[] byteOut;
    public float[] floatOut;

    private static final int USE_SGEMM        = 1;
    private static final int USE_BNNM         = 2;
    private static final int USE_SGEMM_PADDED = 3;
    private static final int USE_BNNM_PADDED  = 4;

    // Work load to emulate deep neural net. Each line has 3 intergers: m, n, k,
    // representing the dimensions of matrices.
    // The actual computation is: C(m*n) = A(m*k) * B(k*n);
    private int googlenet_gemm_sizes[] = {
        12544, 64, 147,
        3136, 64, 64,
        3136, 192, 576,
        784, 64, 192,
        784, 96, 192,
        784, 128, 864,
        784, 16, 192,
        784, 32, 400,
        784, 32, 192,
        784, 128, 256,
        784, 128, 256,
        784, 192, 1152,
        784, 32, 256,
        784, 96, 800,
        784, 64, 256,
        196, 192, 480,
        196, 96, 480,
        196, 204, 864,
        196, 16, 480,
        196, 48, 400,
        196, 64, 480,
        196, 160, 508,
        196, 112, 508,
        196, 224, 1008,
        196, 24, 508,
        196, 64, 600,
        196, 64, 508,
        196, 128, 512,
        196, 128, 512,
        196, 256, 1152,
        196, 24, 512,
        196, 64, 600,
        196, 64, 512,
        196, 112, 512,
        196, 144, 512,
        196, 288, 1296,
        196, 32, 512,
        196, 64, 800,
        196, 64, 512,
        196, 256, 528,
        196, 160, 528,
        196, 320, 1440,
        196, 32, 528,
        196, 128, 800,
        196, 128, 528,
        49, 256, 832,
        49, 160, 832,
        49, 320, 1440,
        49, 48, 832,
        49, 128, 1200,
        49, 128, 832,
        49, 384, 832,
        49, 192, 832,
        49, 384, 1728,
        49, 48, 832,
        49, 128, 1200,
        49, 128, 832,
        16, 128, 508,
        1, 1024, 2048,
        1, 1008, 1024,
        16, 128, 528,
        1, 1024, 2048,
        1, 1008, 1024,
        1, 1008, 1024,
    };

    GoogLeNet(int usage) {
        this.usage = usage;
    }

    public void createTest() {
        mBLAS = ScriptIntrinsicBLAS.create(mRS);
        byteOut = new byte[2];
        floatOut = new float[2];
        setTest();
    }

    // The padded tests are used for GPU BLAS, which has certain requirements
    // for the stride.
    private int roundUp(int input, int roundN) {
        int result = input;
        if (input % roundN > 0) {
            result += roundN - input % roundN;
        }
        return result;
    }

    // Generate random data for input Allocations.
    private byte[] getByteArr(int len) {
        byte[] result = new byte[len];
        mRand.nextBytes(result);
        return result;
    }
    private float[] getFloatArr(int len) {
        float[] result = new float[len];
        for (int i=0; i<len; i++) {
            result[i] = mRand.nextFloat();
        }
        return result;
    }

    // Initialize all the Allocations before executing the benchmark.
    private void setTest() {
        matA = new ArrayList<Allocation>();
        matB = new ArrayList<Allocation>();
        matC = new ArrayList<Allocation>();

        Element e;
        if (usage == USE_SGEMM || usage == USE_SGEMM_PADDED) {
            // SGEMM uses F32 Element.
            e = Element.F32(mRS);
        } else {
            // BNNM uses U8 Element.
            e = Element.U8(mRS);
        }

        for (int i=0; i<googlenet_gemm_sizes.length; i+=3) {
            int m, n, k;
            if (usage == USE_SGEMM || usage == USE_BNNM) {
                // Original work load (no padding needed).
                m = googlenet_gemm_sizes[i];
                n = googlenet_gemm_sizes[i+1];
                k = googlenet_gemm_sizes[i+2];
            } else {
                // Special padding for GPU BLAS.
                m = roundUp(googlenet_gemm_sizes[i], 8);
                n = roundUp(googlenet_gemm_sizes[i+1], 4);
                k = roundUp(googlenet_gemm_sizes[i+2], 4);
            }

            // Create the Types and Allocations.
            Type.Builder builder = new Type.Builder(mRS, e);
            Type a_type = builder.setX(k).setY(m).create();
            Type b_type = builder.setX(n).setY(k).create();
            Type c_type = builder.setX(n).setY(m).create();
            // Special case for BNNM tests.
            if (usage == USE_BNNM || usage == USE_BNNM_PADDED) {
                b_type = builder.setX(k).setY(n).create();
            }

            Allocation A = Allocation.createTyped(mRS, a_type);
            Allocation B = Allocation.createTyped(mRS, b_type);
            Allocation C = Allocation.createTyped(mRS, c_type);

            // Populate the data to the Allocations.
            if (usage == USE_SGEMM || usage == USE_SGEMM_PADDED) {
                A.copyFrom(getFloatArr(k*m));
                B.copyFrom(getFloatArr(k*n));
                C.copyFrom(getFloatArr(n*m));
            } else {
                A.copyFrom(getByteArr(k*m));
                B.copyFrom(getByteArr(k*n));
                C.copyFrom(getByteArr(n*m));
            }

            // Add the Allocations to the corresponding ArrayLists.
            matA.add(A);
            matB.add(B);
            matC.add(C);
        }
    }

    public void runTest() {
        if (usage == USE_SGEMM || usage == USE_SGEMM_PADDED) {
            int i;
            for (i=0; i<googlenet_gemm_sizes.length/3; i++) {
                mBLAS.SGEMM(ScriptIntrinsicBLAS.NO_TRANSPOSE, ScriptIntrinsicBLAS.NO_TRANSPOSE,
                            1.0f, matA.get(i), matB.get(i), 0.f, matC.get(i));
            }
            // Copy the first cell out, to force the driver finish all the execution.
            matC.get(i-1).copy1DRangeTo(0, 1, floatOut);
        } else {
            int a_offset = 1;
            int b_offset = 1;
            int c_mult_int = 1;
            int c_offset = 1;

            int i;
            for (i=0; i<googlenet_gemm_sizes.length/3; i++) {
                mBLAS.BNNM(matA.get(i), a_offset, matB.get(i), b_offset, matC.get(i), c_offset, c_mult_int);
            }
            // Copy the first cell out, to force the driver finish all the execution.
            matC.get(i-1).copy1DRangeTo(0, 1, byteOut);
        }
    }

    public String getTestInfo() {
        return "GoogLeNetTest: " + googlenet_gemm_sizes.length / 3;
    }
}
