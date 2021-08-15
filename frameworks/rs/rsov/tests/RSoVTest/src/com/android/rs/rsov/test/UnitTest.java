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

import android.content.Context;
import android.renderscript.RenderScript.RSMessageHandler;
import android.util.Log;

public class UnitTest extends Thread {
    public String name;
    private int result;
    private ScriptField_ListAllocs_s.Item mItem;
    private RSoVTestCore mRSTC;
    private boolean msgHandled;
    protected Context mCtx;

    /* These constants must match those in shared.rsh */
    public static final int RS_MSG_TEST_PASSED = 100;
    public static final int RS_MSG_TEST_FAILED = 101;
    public static final int TEST_PASSED = 1;
    public static final int TEST_FAILED = -1;

    private static int numTests = 0;
    public int testID;

    protected UnitTest(RSoVTestCore rstc, String n, int initResult, Context ctx) {
        super();
        mRSTC = rstc;
        name = n;
        msgHandled = false;
        mCtx = ctx;
        result = initResult;
        testID = numTests++;
    }

    protected UnitTest(RSoVTestCore rstc, String n, Context ctx) {
        this(rstc, n, 0, ctx);
    }

    protected UnitTest(RSoVTestCore rstc, Context ctx) {
        this(rstc, "<Unknown>", ctx);
    }

    protected UnitTest(Context ctx) {
        this(null, ctx);
    }

    protected void _RS_ASSERT(String message, boolean b) {
        if (b == false) {
            Log.e(name, message + " FAILED");
            failTest();
        }
    }

    private void updateUI() {
        msgHandled = true;
        if (mItem != null) {
            mItem.result = result;
            if (mRSTC != null) {
                // Add null check for mRSTC, for instrumentation tests.
                try {
                    mRSTC.refreshTestResults();
                } catch (IllegalStateException e) {
                /* Ignore the case where our message receiver has been
                   disconnected. This happens when we leave the application
                   before it finishes running all of the unit tests. */
                }
            }
        }
    }

    protected RSMessageHandler mRsMessage = new RSMessageHandler() {
        public void run() {
            if (result == 0) {
                switch (mID) {
                    case RS_MSG_TEST_PASSED:
                        result = TEST_PASSED;
                        break;
                    case RS_MSG_TEST_FAILED:
                        result = TEST_FAILED;
                        break;
                    default:
                        RSoVTest.log("Unit test got unexpected message");
                        return;
                }
            }

            updateUI();
        }
    };

    public void waitForMessage() {
        while (!msgHandled) {
            yield();
        }
    }

    public int getResult() {
        return result;
    }

    public void failTest() {
        result = TEST_FAILED;
        updateUI();
    }

    public void passTest() {
        if (result != TEST_FAILED) {
            result = TEST_PASSED;
        }
        updateUI();
    }

    public String toString() {
        String out = name;
        if (result == TEST_PASSED) {
            out += " - PASSED";
        } else if (result == TEST_FAILED) {
            out += " - FAILED";
        }
        return out;
    }

    public void setItem(ScriptField_ListAllocs_s.Item item) {
        mItem = item;
    }

    public void run() {
        /* This method needs to be implemented for each subclass */
        if (mRSTC != null) {
            mRSTC.refreshTestResults();
        }
    }
}
