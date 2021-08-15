/*
 * Copyright (C) 2015 The Android Open Source Project
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

package com.example.android.rs.blasbenchmark;


import android.app.Activity;
import android.os.Bundle;
import android.test.ActivityInstrumentationTestCase2;
import android.test.suitebuilder.annotation.MediumTest;
import android.util.Log;

import com.example.android.rs.blasbenchmark.BlasTestList.TestName;

/**
 * BLAS benchmark test.
 * To run the test, please use command
 *
 * adb shell am instrument -w com.example.android.rs.blasbenchmark/androidx.test.runner.AndroidJUnitRunner
 *
 */
public class BlasTest extends ActivityInstrumentationTestCase2<BlasBenchmark> {
    private final String TAG = "BLAS Test";
    // Only run 1 iteration now to fit the MediumTest time requirement.
    // One iteration means running the tests continuous for 1s.
    private int mIteration = 1;
    private BlasBenchmark mActivity;

    public BlasTest() {
        super(BlasBenchmark.class);
    }

    // Initialize the parameter for ImageProcessingActivityJB.
    protected void prepareTest() {
        mActivity = getActivity();
        mActivity.prepareInstrumentationTest();
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        prepareTest();
        setActivityInitialTouchMode(false);
   }

    @Override
    public void tearDown() throws Exception {
        super.tearDown();
    }

    class TestAction implements Runnable {
        TestName mTestName;
        float mResult;
        public TestAction(TestName testName) {
            mTestName = testName;
        }
        public void run() {
            mResult = mActivity.mProcessor.getInstrumentationResult(mTestName);
            Log.v(TAG, "Benchmark for test \"" + mTestName.toString() + "\" is: " + mResult);
            synchronized(this) {
                this.notify();
            }
        }
        public float getBenchmark() {
            return mResult;
        }
    }

    // Set the benchmark thread to run on ui thread
    // Synchronized the thread such that the test will wait for the benchmark thread to finish
    public void runOnUiThread(Runnable action) {
        synchronized(action) {
            mActivity.runOnUiThread(action);
            try {
                action.wait();
            } catch (InterruptedException e) {
                Log.v(TAG, "waiting for action running on UI thread is interrupted: " +
                        e.toString());
            }
        }
    }

    public void runTest(TestAction ta, String testName) {
        float sum = 0;
        for (int i = 0; i < mIteration; i++) {
            runOnUiThread(ta);
            float bmValue = ta.getBenchmark();
            Log.v(TAG, "results for iteration " + i + " is " + bmValue);
            sum += bmValue;
        }
        float avgResult = sum/mIteration;

        // post result to INSTRUMENTATION_STATUS
        Bundle results = new Bundle();
        results.putFloat(testName + "_avg", avgResult);
        getInstrumentation().sendStatus(Activity.RESULT_OK, results);
    }

    // Test case 0: SGEMM Test Small
    @MediumTest
    public void testSGEMMSmall() {
        TestAction ta = new TestAction(TestName.SGEMM_SMALL);
        runTest(ta, TestName.SGEMM_SMALL.name());
    }

    // Test case 1: SGEMM Test Medium
    @MediumTest
    public void testSGEMMedium() {
        TestAction ta = new TestAction(TestName.SGEMM_MEDIUM);
        runTest(ta, TestName.SGEMM_MEDIUM.name());
    }

    // Test case 2: SGEMM Test Large
    @MediumTest
    public void testSGEMMLarge() {
        TestAction ta = new TestAction(TestName.SGEMM_LARGE);
        runTest(ta, TestName.SGEMM_LARGE.name());
    }

    // Test case 3: 8Bit GEMM Test Small
    @MediumTest
    public void testBNNMSmall() {
        TestAction ta = new TestAction(TestName.BNNM_SMALL);
        runTest(ta, TestName.BNNM_SMALL.name());
    }

    // Test case 4: 8Bit GEMM Test Medium
    @MediumTest
    public void testBNNMMMedium() {
        TestAction ta = new TestAction(TestName.BNNM_MEDIUM);
        runTest(ta, TestName.BNNM_MEDIUM.name());
    }

    // Test case 5: 8Bit GEMM Test Large
    @MediumTest
    public void testBNNMLarge() {
        TestAction ta = new TestAction(TestName.BNNM_LARGE);
        runTest(ta, TestName.BNNM_LARGE.name());
    }

    // Test case 6: SGEMM GoogLeNet Test
    @MediumTest
    public void testSGEMMGoogLeNet() {
        TestAction ta = new TestAction(TestName.SGEMM_GoogLeNet);
        runTest(ta, TestName.SGEMM_GoogLeNet.name());
    }

    // Test case 7: 8Bit GEMM GoogLeNet Test
    @MediumTest
    public void testBNNMGoogLeNet() {
        TestAction ta = new TestAction(TestName.BNNM_GoogLeNet);
        runTest(ta, TestName.BNNM_GoogLeNet.name());
    }

    // Test case 8: SGEMM GoogLeNet Test Padded
    @MediumTest
    public void testSGEMMGoogLeNetPadded() {
        TestAction ta = new TestAction(TestName.SGEMM_GoogLeNet_Padded);
        runTest(ta, TestName.SGEMM_GoogLeNet_Padded.name());
    }

    // Test case 9: 8Bit GEMM GoogLeNet Test Padded
    @MediumTest
    public void testBNNMGoogLeNetPadded() {
        TestAction ta = new TestAction(TestName.BNNM_GoogLeNet_Padded);
        runTest(ta, TestName.BNNM_GoogLeNet_Padded.name());
    }
}
