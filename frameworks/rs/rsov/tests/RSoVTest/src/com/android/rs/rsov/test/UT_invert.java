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

public class UT_invert extends UnitTest {
    protected UT_invert(RSoVTestCore rstc, Context ctx) {
        super(rstc, "invert", ctx);
    }

    private boolean Test(int width, int height, int depth) {
        RenderScript pRS = RenderScript.create(mCtx);
        ScriptC_invert s = new ScriptC_invert(pRS);

        Type.Builder typeBuilder = new Type.Builder(pRS, Element.F32_4(pRS));
        typeBuilder.setX(width);
        if (height > 0) {
            typeBuilder.setY(height);
            if (depth > 0) {
                typeBuilder.setZ(depth);
            }
        }

        if (depth < 1) {
            depth = 1;
        }

        if (height < 1) {
            height = 1;
        }

        Allocation A = Allocation.createTyped(pRS, typeBuilder.create());
        Allocation B = Allocation.createTyped(pRS, typeBuilder.create());

        float a[] = new float[width * height * depth * 4];
        float b[] = new float[width * height * depth * 4];

        java.util.Random rand = new java.util.Random();

        for (int i = 0; i < width * height * depth * 4; i++) {
            a[i] = rand.nextFloat();
        }

        A.copyFrom(a);

        s.forEach_invert(A, B);

        B.copyTo(b);

        B.destroy();
        A.destroy();

        pRS.finish();
        pRS.destroy();

        boolean failed = false;
        for (int i = 0; i < width * height * depth * 4; i++) {
            if (b[i] != 1.0f - a[i]) {
                Log.e(name, "expects " + (1.0f - a[i]) + " for element " + i + 
                      ". got " + b[i]);
                failed = true;
                break;
            }
        }

        return !failed;
    }

    public void run() {
        final int X = 96;
        final int Y = 64;
        final int Z = 32;

        if (Test(X, 0, 0) && Test(X, Y, 0)) {
            passTest();
            return;
        }

        failTest();
    }
}
