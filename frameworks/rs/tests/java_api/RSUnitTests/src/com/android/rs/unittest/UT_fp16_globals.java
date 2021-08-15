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
import android.renderscript.RenderScript;
import android.renderscript.Short2;
import android.renderscript.Short3;
import android.renderscript.Short4;

public class UT_fp16_globals extends UnitTest {
    private static final short mHalfConst0 = (short) 0x4900; // 10.f
    private static final short mHalfConst1 = (short) 0x4980; // 11.f
    private static final short mHalfConst2 = (short) 0xca00; // -12.f
    private static final short mHalfConst3 = (short) 0xca80; // -13.f

    public UT_fp16_globals(Context ctx) {
        super("Fp16 Globals", ctx);
    }

    public void run() {
        RenderScript pRS = createRenderScript(true);
        ScriptC_fp16_globals s = new ScriptC_fp16_globals(pRS);

        Short2 half2Value = new Short2(mHalfConst0, mHalfConst1);
        Short3 half3Value = new Short3(mHalfConst0, mHalfConst1, mHalfConst2);
        Short4 half4Value = new Short4(mHalfConst0, mHalfConst1, mHalfConst2, mHalfConst3);

        s.set_gHalf(mHalfConst0);
        s.set_gHalf2(half2Value);
        s.set_gHalf3(half3Value);
        s.set_gHalf4(half4Value);

        s.invoke_test(mHalfConst0, half2Value, half3Value, half4Value);
        s.invoke_validateHalf(mHalfConst0);
        s.invoke_validateHalf2(half2Value);
        s.invoke_validateHalf3(half3Value);
        s.invoke_validateHalf4(half4Value);

        pRS.finish();
        s.destroy();
        pRS.destroy();
    }
}
