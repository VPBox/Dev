/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License for the specific language governing permissions and limitations under
 * the License.
 */

package com.android.rs.unittest;

import android.content.Context;
import android.renderscript.Allocation;
import android.renderscript.Element;
import android.renderscript.RenderScript;
import android.renderscript.ScriptGroup;
import android.renderscript.Type;
import android.util.Log;

public class UT_script_group2_float extends UnitTest {
    private static final int ARRAY_SIZE = 8;

    private static final String TAG = "ScriptGroup2 (Floating Point Test)";

    public UT_script_group2_float(Context ctx) {
        super(TAG, ctx);
    }

    public void run() {
        RenderScript pRS = createRenderScript(false);

        ScriptC_float_test s_float = new ScriptC_float_test(pRS);

        float[] array = new float[ARRAY_SIZE];

        for (int i = 0; i < ARRAY_SIZE; i++) {
            array[i] = 0.0f;
        }

        Allocation input = Allocation.createSized(pRS, Element.F32(pRS), ARRAY_SIZE);
        input.copyFrom(array);

        ScriptGroup.Builder2 builder = new ScriptGroup.Builder2(pRS);

        ScriptGroup.Input unbound = builder.addInput();

        final float floatVal = 3.14f;
        final double doubleVal = 1.23456789;
        final long longVal = 0x100000000L;
        final Type T = Type.createX(pRS, Element.F64(pRS), ARRAY_SIZE);
        ScriptGroup.Closure c0 =
                builder.addKernel(s_float.getKernelID_foo(),
                                  T,
                                  unbound,
                                  new ScriptGroup.Binding(s_float.getFieldID_floatVal(),
                                                          floatVal),
                                  new ScriptGroup.Binding(s_float.getFieldID_val(),
                                                          doubleVal));

        ScriptGroup.Closure c1 =
                builder.addKernel(s_float.getKernelID_goo(),
                                  T,
                                  c0.getReturn(),
                                  new ScriptGroup.Binding(s_float.getFieldID_valLong(),
                                                          longVal));

        ScriptGroup group = builder.create("TestFloatAnd64bit", c1.getReturn());

        double[] a = new double[ARRAY_SIZE];
        ((Allocation) group.execute(input)[0]).copyTo(a);

        pRS.finish();
        T.destroy();
        group.destroy();
        input.destroy();
        s_float.destroy();
        pRS.destroy();

        boolean failed = false;
        for (int i = 0; i < ARRAY_SIZE; i++) {
            if (a[i] != doubleVal + longVal + floatVal) {
                Log.e(TAG, "a[" + i + "]=" + a[i] + ", should be " + (doubleVal + longVal));
                failed = true;
            }
        }
        if (failed) {
            failTest();
            return;
        }
        passTest();
    }
}
