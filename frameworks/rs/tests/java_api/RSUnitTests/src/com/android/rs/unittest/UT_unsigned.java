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

public class UT_unsigned extends UnitTest {

    public UT_unsigned(Context ctx) {
        super("Unsigned", ctx);
    }

    private boolean initializeGlobals(ScriptC_unsigned s) {
        short pUC = s.get_uc();
        if (pUC != 5) {
            return false;
        }
        s.set_uc((short) 129);

        long pUI = s.get_ui();
        if (pUI != 37) {
            return false;
        }
        s.set_ui(0x7fffffff);

        return true;
    }

    public void run() {
        RenderScript pRS = createRenderScript(true);
        ScriptC_unsigned s = new ScriptC_unsigned(pRS);
        if (!initializeGlobals(s)) {
            failTest();
        } else {
            s.invoke_unsigned_test();
            pRS.finish();
        }
        s.destroy();
        pRS.destroy();
    }
}
