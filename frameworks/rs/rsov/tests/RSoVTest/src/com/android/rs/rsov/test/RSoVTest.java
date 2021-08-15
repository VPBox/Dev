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

package com.android.rs.rsov.test;

import android.app.ListActivity;
import android.os.Bundle;
import android.renderscript.RenderScript;
import android.util.Log;

public class RSoVTest extends ListActivity {

    private static final String LOG_TAG = "RSTest";
    private static final boolean DEBUG = false;
    private static final boolean LOG_ENABLED = false;

    private RenderScript mRS;
    private RSoVTestCore RSTC;

    @Override
    public void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        mRS = RenderScript.create(this);

        RSTC = new RSoVTestCore(this);
        RSTC.init(mRS);
    }

    static void log(String message) {
        if (LOG_ENABLED) {
            Log.v(LOG_TAG, message);
        }
    }


}
