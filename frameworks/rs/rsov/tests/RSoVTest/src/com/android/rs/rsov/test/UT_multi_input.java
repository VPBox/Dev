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

package com.android.rs.rsov.test;

import android.content.Context;
import android.renderscript.Allocation;
import android.renderscript.Element;
import android.renderscript.RenderScript;
import android.renderscript.Type;
import android.util.Log;

public class UT_multi_input extends UnitTest {
    private Allocation Ain0;
    private Allocation Ain1;

    private Allocation Out0;

    private final int Xdim = 100;
    private final float tolerance = 1e-6f;

    protected UT_multi_input(RSoVTestCore rstc, Context ctx) {
        super(rstc, "Foreach Multi-input", ctx);
    }

    private void initializeGlobals(RenderScript RS, ScriptC_multi_input s) {
        Type.Builder floatBuilder = new Type.Builder(RS, Element.F32(RS));

        floatBuilder.setX(Xdim);

        Ain0 = Allocation.createTyped(RS, floatBuilder.create());
        Ain1 = Allocation.createTyped(RS, floatBuilder.create());
        Out0 = Allocation.createTyped(RS, floatBuilder.create());
        return;
    }

    public void run() {
        RenderScript pRS = RenderScript.create(mCtx);
        ScriptC_multi_input s = new ScriptC_multi_input(pRS);

        initializeGlobals(pRS, s);

        float a[] = new float[Xdim];
        float b[] = new float[Xdim];


        java.util.Random rand = new java.util.Random();

        for (int i = 0; i < Xdim; i++) {
            a[i] = rand.nextFloat();
            b[i] = rand.nextFloat();
        }

        Ain0.copyFrom(a);
        Ain1.copyFrom(b);

        s.forEach_sum2(Ain0, Ain1, Out0);

        float out0[] = new float[Xdim];
        float ain0[] = new float[Xdim];
        float ain1[] = new float[Xdim];
        Ain0.copyTo(ain0);
        Ain1.copyTo(ain1);
        Out0.copyTo(out0);

        pRS.finish();
        pRS.destroy();

        boolean failed = false;
        for (int i = 0; i < Xdim; i++) {
            if (ain0[i] != a[i]) {
                Log.e(name, "Ain0 was " + a[i] + " but changed to " + ain0[i]);
                failed = true;
                break;
            }
            if (ain1[i] != b[i]) {
                Log.e(name, "Ain1 was " + b[i] + " but changed to " + ain1[i]);
                failed = true;
                break;
            }
            if ((a[i] + b[i] - out0[i]) > tolerance) {
                float expected = a[i]+b[i];
                Log.e(name, "expects " + expected + " got " + out0[i]);
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
