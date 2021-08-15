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

package com.android.rs.scriptgroup;

import android.app.Activity;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.widget.ImageView;
import android.renderscript.*;

public class MainActivity extends Activity {
    private static final int ARRAY_SIZE = 8;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_layout);

        // create renderscript context
        RenderScript pRS = RenderScript.create(this, RenderScript.ContextType.NORMAL,
            RenderScript.CREATE_FLAG_WAIT_FOR_ATTACH | RenderScript.CREATE_FLAG_LOW_LATENCY);

        ScriptC_scriptgroup script = new ScriptC_scriptgroup(pRS);

        // create and initalize a simple input allocation
        int[] array = new int[ARRAY_SIZE];
        for (int i = 0; i < ARRAY_SIZE; i++) {
            array[i] = i;
        }
        Allocation input = Allocation.createSized(pRS, Element.I32(pRS), ARRAY_SIZE);
        input.copyFrom(array);

        ScriptGroup.Builder2 builder = new ScriptGroup.Builder2(pRS);

        ScriptGroup.Input unbound = builder.addInput();

        ScriptGroup.Closure c0 = builder.addKernel(
            script.getKernelID_foo(), Type.createX(pRS, Element.I32(pRS), ARRAY_SIZE), unbound);

        ScriptGroup.Closure c1 = builder.addKernel(script.getKernelID_goo(),
            Type.createX(pRS, Element.I32(pRS), ARRAY_SIZE), c0.getReturn());

        ScriptGroup group = builder.create("scriptgroup_test", c1.getReturn());

        int[] a = new int[ARRAY_SIZE];
        ((Allocation) group.execute(input)[0]).copyTo(a);

        pRS.finish();
        pRS.destroy();
    }
}
