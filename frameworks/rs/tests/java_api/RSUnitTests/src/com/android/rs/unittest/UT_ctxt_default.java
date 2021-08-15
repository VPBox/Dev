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

public class UT_ctxt_default extends UnitTest {
    private Type T;
    private Allocation A;
    private Allocation B;

    public UT_ctxt_default(Context ctx) {
        super("Kernel context default", ctx);
    }

    private void initializeGlobals(RenderScript RS, ScriptC_ctxt_default s) {
        Type.Builder typeBuilder = new Type.Builder(RS, Element.I32(RS));
        int X = 2;
        s.set_gDimX(X);
        typeBuilder.setX(X);

        T = typeBuilder.create();
        A = Allocation.createTyped(RS, T);
        s.set_A(A);
        B = Allocation.createTyped(RS, T);
        s.set_B(B);
        return;
    }

    public void run() {
        RenderScript pRS = createRenderScript(true);
        ScriptC_ctxt_default s = new ScriptC_ctxt_default(pRS);
        initializeGlobals(pRS, s);
        s.forEach_init_vars(A);
        s.forEach_root(A, B);
        s.invoke_verify_root();
        s.invoke_kernel_test();
        pRS.finish();
        T.destroy();
        A.destroy();
        B.destroy();
        s.destroy();
        pRS.destroy();
    }
}
