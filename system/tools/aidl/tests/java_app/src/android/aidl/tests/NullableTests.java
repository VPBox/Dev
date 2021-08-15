/*
 * Copyright (C) 2016, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package android.aidl.tests;

import android.aidl.tests.TestFailException;
import android.os.IBinder;
import java.util.ArrayList;
import java.util.List;

class NullableTests {
    private static final String TAG = "TestServiceClient";
    private ITestService mService;
    private TestLogger mLog;

    public NullableTests(ITestService service, TestLogger logger) {
        mService = service;
        mLog = logger;
    }

    public void checkNullHandling() throws TestFailException {
        mLog.log("Checking that sending null strings reports an error...");
        try {
            String response = mService.RepeatString(null);
            mLog.logAndThrow("Expected to fail on null string input!");
        } catch (NullPointerException ex) {
            mLog.log("Caught an exception on null string parameter (expected)");
            mLog.log("null strings behave as expected");
            return;
        } catch (Exception ex) {
            mLog.logAndThrow("Expected to receive NullPointerException on " +
                             "null parameter, but got " + ex.toString());
        }
        mLog.logAndThrow("Expected to receive NullPointerException on " +
                         "null parameter, but nothing was thrown??");
    }

    public void checkNullBinderDetection() throws TestFailException {
        mLog.log("Checking that service handles @nullable IBinder...");
        try {
            mService.TakesAnIBinder(null);
            mLog.logAndThrow("Expected to fail on null Binder!");
        } catch (NullPointerException ex) {
            mLog.log("Caught an exception on null Binder parameter (expected)");
            return;
        } catch (Exception ex) {
            mLog.logAndThrow("Expected to receive NullPointerException," +
                             "but got " + ex.toString());
        }
        mLog.logAndThrow("Expected to receive NullPointerException on " +
                         "null parameter, but nothing was thrown??");
    }

    public void checkNullBinderInListDetection() throws TestFailException {
        List<IBinder> listWithNulls = new ArrayList<IBinder>();
        listWithNulls.add(null);
        try {
            mService.TakesAnIBinderList(listWithNulls);
            mLog.logAndThrow("Expected to fail on list with null Binder!");
        } catch (NullPointerException ex) {
            mLog.log("Caught an exception on list with null Binder (expected)");
            return;
        } catch (Exception ex) {
            mLog.logAndThrow("Expected to receive NullPointerException," +
                             "but got " + ex.toString());
        }
        mLog.logAndThrow("Expected to receive NullPointerException on " +
                         "null parameter, but nothing was thrown??");
    }

    public void checkNullInterfaceHandling() throws TestFailException {
        mLog.log("Checking @nullable IInterface handling...");
        try {
            INamedCallback callback  = mService.GetCallback(false);
            if (callback == null) {
                mLog.logAndThrow("Expected to get non-null INamedCallback.");
            }
            callback  = mService.GetCallback(true);
            if (callback != null) {
                mLog.logAndThrow("Expected to get null INamedCallback.");
            }
        } catch (Exception ex) {
            mLog.logAndThrow("Unexpected exception during @nullable IInterface test: " +
                             ex.toString());
        }
        mLog.log("@nullable IInterface handling works as expected.");
    }

    public void runTests() throws TestFailException {
        checkNullHandling();
        checkNullBinderDetection();
        checkNullBinderInListDetection();
        checkNullInterfaceHandling();
    }
}
