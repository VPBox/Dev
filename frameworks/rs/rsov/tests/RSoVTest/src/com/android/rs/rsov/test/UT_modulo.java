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

package com.android.rs.rsov.test;

import android.content.Context;
import android.renderscript.Allocation;
import android.renderscript.Element;
import android.renderscript.RenderScript;
import android.renderscript.Type;
import android.util.Log;

public class UT_modulo extends UnitTest {
    private Allocation A;
    private Allocation B;
    private final int X = 96;
    private final int Y = 64;

    protected UT_modulo(RSoVTestCore rstc, Context ctx) {
        super(rstc, "modulo", ctx);
    }

    private void initializeGlobals(RenderScript RS, ScriptC_modulo s) {
        Type.Builder typeBuilder = new Type.Builder(RS, Element.I32(RS));
        typeBuilder.setX(X);
        typeBuilder.setY(Y);

        A = Allocation.createTyped(RS, typeBuilder.create());
        B = Allocation.createTyped(RS, typeBuilder.create());
        return;
    }

    public void run() {
        RenderScript pRS = RenderScript.create(mCtx);
        ScriptC_modulo s = new ScriptC_modulo(pRS);

        initializeGlobals(pRS, s);

        int a[] = new int[X*Y];
        int b[] = new int[X*Y];

        java.util.Random rand = new java.util.Random();

        for (int i = 0; i < X * Y; i++) {
            a[i] = rand.nextInt(65536);
        }

        A.copyFrom(a);

        s.forEach_modulo(A, B);

        B.copyTo(b);

        pRS.finish();
        pRS.destroy();

        boolean failed = false;
        for (int i = 0; i < X * Y; i++) {
            int expected = a[i] % 256;
            if (b[i] != expected) {
                Log.e(name, "expects " + expected + " got " + b[i]);
                failed = true;
                break;
            }
        }

        if (failed) {
            failTest();
        } else {
            passTest();
        }
    }
}
