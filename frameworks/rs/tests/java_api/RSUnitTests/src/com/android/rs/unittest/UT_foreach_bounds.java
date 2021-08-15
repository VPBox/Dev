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
import android.renderscript.Script;
import android.renderscript.Type;

public class UT_foreach_bounds extends UnitTest {
    private Allocation A;

    public UT_foreach_bounds(Context ctx) {
        super("ForEach (bounds)", ctx);
    }

    private void initializeGlobals(RenderScript RS, ScriptC_foreach_bounds s) {
        Type.Builder typeBuilder = new Type.Builder(RS, Element.I32(RS));
        int X = 5;
        int Y = 7;
        final int xStart = 2;
        final int xEnd = 5;
        final int yStart = 3;
        final int yEnd = 6;
        s.set_dimX(X);
        s.set_dimY(Y);
        typeBuilder.setX(X).setY(Y);
        A = Allocation.createTyped(RS, typeBuilder.create());
        s.set_aRaw(A);
        s.set_s(s);
        s.set_ain(A);
        s.set_aout(A);
        s.set_xStart(xStart);
        s.set_xEnd(xEnd);
        s.set_yStart(yStart);
        s.set_yEnd(yEnd);
        s.forEach_zero(A);

        Script.LaunchOptions sc = new Script.LaunchOptions();
        sc.setX(xStart, xEnd).setY(yStart, yEnd);
        s.forEach_root(A, sc);

        return;
    }

    public void run() {
        RenderScript pRS = createRenderScript(true);
        ScriptC_foreach_bounds s = new ScriptC_foreach_bounds(pRS);
        initializeGlobals(pRS, s);
        s.invoke_verify_root();
        s.invoke_foreach_bounds_test();
        pRS.finish();
        A.getType().destroy();
        A.destroy();
        s.destroy();
        pRS.destroy();
    }
}
