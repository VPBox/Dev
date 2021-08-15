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
import android.renderscript.*;
import android.util.Log;

public class UT_global extends UnitTest {
    private static final String TAG = "global";

    protected UT_global(RSoVTestCore rstc, Context ctx) {
        super(rstc, "global", ctx);
    }

    private RenderScript RS;
    private Allocation A;

    private void test(Element elem, java.lang.Runnable action) {
        A = Allocation.createSized(RS, elem, 128);
        action.run();
        waitForMessage();
        A.destroy();
    }

    private void testIntTypes() {
        ScriptC_global_int s = new ScriptC_global_int(RS);

        test(Element.I32(RS), () -> {
                int expected = 65536;
                s.invoke_int_test_init(A);
                s.set_g_int(expected);
                s.invoke_int_test(A, expected);
            });

        test(Element.I32_2(RS), () -> {
                Int2 expected = new Int2(65536, 65537);;
                s.invoke_int2_test_init(A);
                s.set_g_int2(expected);
                s.invoke_int2_test(A, expected);
            });

        test(Element.I32_4(RS), () -> {
                Int4 expected = new Int4(65536, 65537, 65538, 65539);
                s.invoke_int4_test_init(A);
                s.set_g_int4(expected);
                s.invoke_int4_test(A, expected);
            });

        s.destroy();
    }

    private void testFloatTypes() {
        ScriptC_global_float s = new ScriptC_global_float(RS);

        test(Element.F32(RS), () -> {
                float expected = 3.141593f;
                s.invoke_float_test_init(A);
                s.set_g_float(expected);
                s.invoke_float_test(A, expected);
            });

        test(Element.F32_2(RS), () -> {
                Float2 expected = new Float2(3.141593f, 3.141593f / 2);
                s.invoke_float2_test_init(A);
                s.set_g_float2(expected);
                s.invoke_float2_test(A, expected);
            });

        test(Element.F32_4(RS), () -> {
                Float4 expected = new Float4(3.141593f, 3.141593f / 2,
                                             3.141593f / 3, 3.141593f / 4);
                s.invoke_float4_test_init(A);
                s.set_g_float4(expected);
                s.invoke_float4_test(A, expected);
            });

        s.destroy();
    }

    public void run() {
        RS = RenderScript.create(mCtx);
        RS.setMessageHandler(mRsMessage);
        RS.finish();

        testIntTypes();
        testFloatTypes();

        RS.destroy();
    }
}
