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

package com.android.rs.unittest;

import android.content.Context;
import android.renderscript.Allocation;
import android.renderscript.Element;
import android.renderscript.RenderScript;
import android.renderscript.Type;

public class UT_fp16 extends UnitTest {
    private int dimX = 7;
    private int dimY = 5;
    private int dimZ = 3;
    private Type type;
    private Allocation alloc;

    public UT_fp16(Context ctx) {
        super("Fp16", ctx);
    }

    private void initializeGlobals(RenderScript RS, ScriptC_fp16 s, Element e, int nDims) {
        Type.Builder typeBuilder = new Type.Builder(RS, e);
        switch (nDims) {
            case 1:
                typeBuilder.setX(dimX);
                break;
            case 2:
                typeBuilder.setX(dimX).setY(dimY);
                break;
            case 3:
                typeBuilder.setX(dimX).setY(dimY).setZ(dimZ);
                break;
        }
        type = typeBuilder.create();
        alloc = Allocation.createTyped(RS, type);

        s.set_gDimX(dimX);
        s.set_gDimY(nDims > 1 ? dimY : 0);
        s.set_gDimZ(nDims > 2 ? dimZ : 0);
        s.set_gAlloc(alloc);
    }

    private void TestHalf(RenderScript RS, ScriptC_fp16 s, int nDims) {
        initializeGlobals(RS, s, Element.F16(RS), nDims);

        // set in kernel and test rsGetElementAt in invoke
        s.forEach_set_kernel_half(alloc);
        s.invoke_verify_half();

        RS.finish();
        alloc.destroy();
        type.destroy();

        initializeGlobals(RS, s, Element.F16(RS), nDims);
        // rsSetElementAt in invoke and verify in kernel
        s.invoke_set_half();
        s.forEach_verify_kernel_half(alloc);

        RS.finish();
        alloc.destroy();
        type.destroy();
    }

    private void TestHalf2(RenderScript RS, ScriptC_fp16 s, int nDims) {
        initializeGlobals(RS, s, Element.F16_2(RS), nDims);

        // set in kernel and test rsGetElementAt in invoke
        s.forEach_set_kernel_half2(alloc);
        s.invoke_verify_half2();

        RS.finish();
        alloc.destroy();
        type.destroy();

        initializeGlobals(RS, s, Element.F16_2(RS), nDims);
        // rsSetElementAt in invoke and verify in kernel
        s.invoke_set_half2();
        s.forEach_verify_kernel_half2(alloc);

        RS.finish();
        alloc.destroy();
        type.destroy();
    }

    private void TestHalf3(RenderScript RS, ScriptC_fp16 s, int nDims) {
        initializeGlobals(RS, s, Element.F16_3(RS), nDims);

        // set in kernel and test rsGetElementAt in invoke
        s.forEach_set_kernel_half3(alloc);
        s.invoke_verify_half3();

        RS.finish();
        alloc.destroy();
        type.destroy();

        initializeGlobals(RS, s, Element.F16_3(RS), nDims);
        // rsSetElementAt in invoke and verify in kernel
        s.invoke_set_half3();
        s.forEach_verify_kernel_half3(alloc);

        RS.finish();
        alloc.destroy();
        type.destroy();
    }

    private void TestHalf4(RenderScript RS, ScriptC_fp16 s, int nDims) {
        initializeGlobals(RS, s, Element.F16_4(RS), nDims);

        // set in kernel and test rsGetElementAt in invoke
        s.forEach_set_kernel_half4(alloc);
        s.invoke_verify_half4();

        RS.finish();
        alloc.destroy();
        type.destroy();

        initializeGlobals(RS, s, Element.F16_4(RS), nDims);
        // rsSetElementAt in invoke and verify in kernel
        s.invoke_set_half4();
        s.forEach_verify_kernel_half4(alloc);

        RS.finish();
        alloc.destroy();
        type.destroy();
    }

    public void run() {
        Allocation A;
        RenderScript pRS = createRenderScript(true);
        ScriptC_fp16 s = new ScriptC_fp16(pRS);

        for (int nDims = 1; nDims <= 3; nDims++) {
            TestHalf(pRS, s, nDims);
            TestHalf2(pRS, s, nDims);
            TestHalf3(pRS, s, nDims);
            TestHalf4(pRS, s, nDims);
        }

        s.invoke_fp16_test();
        pRS.finish();
        s.destroy();
        pRS.destroy();
    }
}
