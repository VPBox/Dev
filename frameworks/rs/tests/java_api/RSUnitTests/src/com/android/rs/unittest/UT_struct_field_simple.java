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

public class UT_struct_field_simple extends UnitTest {

    public UT_struct_field_simple(Context ctx) {
        super("Structure Fields", ctx);
    }

    public void run() {
        RenderScript pRS = createRenderScript(true);
        ScriptC_struct_field_simple s = new ScriptC_struct_field_simple(pRS);

        ScriptField_Simple.Item simple = new ScriptField_Simple.Item();
        final int I = 0x12345678;
        final long L = 0x76543210fedcba98L;
        simple.I = I;
        simple.L = L;
        s.set_simple(simple);

        s.invoke_checkSimple(I, L);

        pRS.finish();
        pRS.destroy();
    }
}

// struct Simple {
//     int I;
//     long L;
// };
//
// Consistently padded, we expect:
//   I    = 0x12345678
//   L.lo = 0xfedcba98
//   L.hi = 0x76543210
//
// And indeed on N9 --abi armeabi-v7a (i.e., ARM-32) we see:
//
//   09-23 21:55:51.026  5990  6005 D RenderScript: argI        305419896  0x12345678
//   09-23 21:55:51.026  5990  6005 D RenderScript: simple.I    305419896  0x12345678
//   09-23 21:55:51.026  5990  6005 D RenderScript: argL.lo     4275878552  0xfedcba98
//   09-23 21:55:51.026  5990  6005 D RenderScript: simple.L.lo 4275878552  0xfedcba98
//   09-23 21:55:51.026  5990  6005 D RenderScript: argL.hi     1985229328  0x76543210
//   09-23 21:55:51.026  5990  6005 D RenderScript: simple.L.hi 1985229328  0x76543210
//   09-23 21:55:51.026  5990  6005 D RenderScript: struct_field_simple PASSED 0  0x0
//
// Inconsistently padded (Java pads, x86-32 script does not), we expect:
//   I    = 0x12345678  // fine
//   L.lo = 0           // oops, script read the padding
//   L.hi = 0xfedcba98  // oops, script read L.lo
//
// And indeed on fugu (i.e., x86-32) before the fix for http://b/29154200 and http://b/28070272
// we see:
//
//   09-23 21:39:19.775  4090  4107 D RenderScript: argI        305419896  0x12345678
//   09-23 21:39:19.775  4090  4107 D RenderScript: simple.I    305419896  0x12345678
//   09-23 21:39:19.775  4090  4107 D RenderScript: argL.lo     4275878552  0xfedcba98
//   09-23 21:39:19.775  4090  4107 D RenderScript: simple.L.lo 0  0x0
//   09-23 21:39:19.775  4090  4107 D RenderScript: argL.hi     1985229328  0x76543210
//   09-23 21:39:19.775  4090  4107 D RenderScript: simple.L.hi 4275878552  0xfedcba98
//   09-23 21:39:19.775  4090  4107 D RenderScript: simple.L == argL FAILED 0  0x0
//   09-23 21:39:19.775  4090  4107 D RenderScript: struct_field_simple FAILED 0  0x0
