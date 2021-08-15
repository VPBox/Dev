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

public class UT_global_query extends UnitTest {
    protected UT_global_query(RSoVTestCore rstc, Context ctx) {
        super(rstc, "global_query", ctx);
    }

    private boolean Test(int width, int height, int depth) {
        RenderScript pRS = RenderScript.create(mCtx);
        ScriptC_global_query s = new ScriptC_global_query(pRS);

        Type.Builder typeBuilder = new Type.Builder(pRS, Element.I32(pRS));
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

        Allocation G = Allocation.createTyped(pRS, typeBuilder.create());
        Allocation dummy = Allocation.createTyped(pRS, typeBuilder.create());
        Allocation R = Allocation.createTyped(pRS, typeBuilder.create());

        int g[] = new int[width * height * depth];
        int d[] = new int[width * height * depth];
        int r[] = new int[width * height * depth];

        java.util.Random rand = new java.util.Random();

        for (int i = 0; i < width * height * depth; i++) {
            g[i] = rand.nextInt(123456);
            d[i] = rand.nextInt(123456);
        }

        G.copyFrom(g);
        dummy.copyFrom(d);

        s.set_g(G);

        s.forEach_getDim(dummy, R);

        R.copyTo(r);

        R.destroy();
        dummy.destroy();
        G.destroy();

        pRS.finish();
        pRS.destroy();

        boolean failed = false;
        for (int i = 0; i < width * height * depth; i++) {
            if (r[i] != width) {
                Log.e(name, "expects " + width + " for element " + i +
                      ". got " + r[i]);
                failed = true;
                break;
            }
        }

        return !failed;
    }

    public void run() {
        final int X = 96;

        if (Test(X, 0, 0)) {
            passTest();
            return;
        }

        failTest();
    }
}
