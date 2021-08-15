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

package com.android.rs.singlesource;

import android.app.Activity;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.widget.ImageView;
import android.renderscript.*;

public class MainActivity extends Activity {

    private RenderScript mRS;
    private Allocation mAllocIn1;
    private Allocation mAllocIn2;
    private Allocation mAllocOut;
    private ScriptC_rs_single_source mScript;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        setContentView(R.layout.main_layout);

        // create renderscript context
        mRS = RenderScript.create(
              this,
              RenderScript.ContextType.NORMAL,
              RenderScript.CREATE_FLAG_WAIT_FOR_ATTACH |
              RenderScript.CREATE_FLAG_LOW_LATENCY);

        // create a new instance of the script
        mScript = new ScriptC_rs_single_source(mRS);

        // create the first input allocation
        mAllocIn1 = Allocation.createSized(mRS, Element.F32(mRS), 4);
        float [] in1 = new float[]{ 1.f, 2.f, 3.f, 4.f };
        mAllocIn1.copyFrom(in1);

        // create second input allocation
        mAllocIn2 = Allocation.createSized(mRS, Element.F32(mRS), 4);
        float [] in2 = new float[]{ 5.f, 6.f, 7.f, 8.f };
        mAllocIn2.copyFrom(in2);

        // create output allocation
        mAllocOut = Allocation.createSized(mRS, Element.F32(mRS), 4);

        // setup the global output allocation
        mScript.set_global_alloc(Allocation.createSized(mRS, Element.F32(mRS), 4));

        // invoke static function 1
        mScript.invoke_script_invoke_1(mAllocOut, mAllocIn1, mAllocIn2);

        // invoke static function 2
        mScript.invoke_script_invoke_2();

        // invoke void kernel
        Script.LaunchOptions options = new Script.LaunchOptions();
        options.setX(0, 4);
        mScript.forEach_void_kernel_1(options);
    }
}
