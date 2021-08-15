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

public class UT_single_source_alloc extends UnitTest {
    private int dimX = 3;
    private int dimY = 4;
    private int dimZ = 5;
    private int start = 23;

    // Mimicking enum rs_data_type in frameworks/rs/script_api/include/rs_object_types.rsh
    private enum rsDataType {
        RS_TYPE_FLOAT_16    (1),
        RS_TYPE_FLOAT_32    (2),
        RS_TYPE_FLOAT_64    (3),
        RS_TYPE_SIGNED_8    (4),
        RS_TYPE_SIGNED_16   (5),
        RS_TYPE_SIGNED_32   (6),
        RS_TYPE_SIGNED_64   (7),
        RS_TYPE_UNSIGNED_8  (8),
        RS_TYPE_UNSIGNED_16 (9),
        RS_TYPE_UNSIGNED_32 (10),
        RS_TYPE_UNSIGNED_64 (11);

        private int value;

        rsDataType(int value) { this.value = value; }
        public int Value() { return value; }
    }

    public UT_single_source_alloc(Context ctx) {
        super("SingleSourceAllocation", ctx);
    }

    private void initializeGlobals(RenderScript RS, ScriptC_single_source_alloc s, int nDims) {
        s.set_gDimX(dimX);
        s.set_gDimY(nDims > 1 ? dimY : 0);
        s.set_gDimZ(nDims > 2 ? dimZ : 0);
        s.set_gStart(start);

        return;
    }

    public void run() {
        RenderScript pRS = createRenderScript(true);
        ScriptC_single_source_alloc s = new ScriptC_single_source_alloc(pRS);

        // Test 1-D, 2-D and 3-D Allocations of basic RenderScript types by creating Allocations and
        // invoking a kernel on them.
        for (rsDataType dataType : rsDataType.values()) {
            for (int vecSize = 1; vecSize <= 4; vecSize++) {
                for (int nDims = 1; nDims <= 3; nDims++) {
                    initializeGlobals(pRS, s, nDims);
                    s.invoke_CreateAndTestAlloc(dataType.Value(), vecSize);
                }
            }
        }

        // Exhaustively test valid and invalid calls to rs_* creation functions.  (These tests don't
        // walk the created allocations, though.)
        s.invoke_TestAllCases();

        s.invoke_single_source_alloc_test();
        pRS.finish();
        s.destroy();
        pRS.destroy();
    }
}
