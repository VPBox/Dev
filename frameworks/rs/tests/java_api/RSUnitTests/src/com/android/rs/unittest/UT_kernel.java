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

public class UT_kernel extends UnitTest {
    private Type TA;
    private Type TB;
    private Allocation A;
    private Allocation B;

    public UT_kernel(Context ctx) {
        super("Kernels (pass-by-value)", ctx);
    }

    private void initializeGlobals(RenderScript RS, ScriptC_kernel s) {
        Type.Builder typeBuilder = new Type.Builder(RS, Element.I32(RS));
        int X = 5;
        s.set_dimX(X);
        typeBuilder.setX(X);
        TA = typeBuilder.create();
        A = Allocation.createTyped(RS, TA);
        s.bind_ain(A);
        TB = typeBuilder.create();
        B = Allocation.createTyped(RS, TB);
        s.bind_aout(B);

        return;
    }

    public void run() {
        RenderScript pRS = createRenderScript(true);
        ScriptC_kernel s = new ScriptC_kernel(pRS);
        initializeGlobals(pRS, s);
        s.forEach_init_vars(A);
        s.forEach_root(A, B);
        s.invoke_verify_root();
        s.invoke_kernel_test();
        pRS.finish();
        A.destroy();
        B.destroy();
        TA.destroy();
        TB.destroy();
        s.destroy();
        pRS.destroy();
    }
}
