/*
 * Copyright (C) 2011 The Android Open Source Project
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

package com.android.rs.imagejb;


import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.test.ActivityInstrumentationTestCase2;
import android.test.suitebuilder.annotation.MediumTest;
import android.util.Log;

import com.android.rs.imagejb.IPTestListJB.TestName;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

/**
 * ImageProcessing benchmark test.
 * To run the test, please use command
 *
 * adb shell am instrument -w com.android.rs.imagejb/androidx.test.runner.AndroidJUnitRunner
 *
 */
public class ImageProcessingTest extends ActivityInstrumentationTestCase2<ImageProcessingActivityJB> {
    private final String TAG = "ImageProcessingTest";
    // Only run 1 iteration now to fit the MediumTest time requirement.
    // One iteration means running the tests continuous for 1s.
    private ImageProcessingActivityJB mActivity;

    public ImageProcessingTest() {
        super(ImageProcessingActivityJB.class);
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
        if (mActivity.mProcessor != null) {
            mActivity.mProcessor.exit();
            mActivity.mProcessor = null;
        }
        super.tearDown();
    }

    class TestAction implements Runnable {
        private TestName mTestName;
        private Result mResult;
        public TestAction(TestName testName) {
            mTestName = testName;
        }
        public void run() {
            mResult = mActivity.mProcessor.getInstrumentationResult(mTestName);
            Log.v(TAG, "Benchmark for test \"" + mTestName.toString() + "\" is: " +
                    mResult.getAvg() * 1000.f);
            synchronized(this) {
                this.notify();
            }
        }
        public Result getBenchmark() {
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

    // TODO: Report more info: mean, median, std, etc.
    public void runTest(TestAction ta, String testName) {
        runOnUiThread(ta);
        Result times = ta.getBenchmark();

        // post result to INSTRUMENTATION_STATUS
        Bundle results = new Bundle();
        results.putFloat(testName + "_avg", times.getAvg() * 1000.0f); // ms
        results.putFloat(testName + "_stdevp", times.getStdevp() * 1000.0f); // ms
        results.putFloat(testName + "_stdcoef", times.getStdCoef() * 100.0f); // %
        getInstrumentation().sendStatus(Activity.RESULT_OK, results);

        // save the runtime distribution to a file on the sdcard so a script can plot it
        writeResults("rsTimes/", testName + "_DATA.txt", times);
    }

    private void writeResults(String directory, String filename, Result times) {
        // write result into a file
        File externalStorage = Environment.getExternalStorageDirectory();
        if (!externalStorage.canWrite()) {
            Log.v(TAG, "sdcard is not writable");
            return;
        }
        File resultDirectory = new File(externalStorage, directory);
        resultDirectory.mkdirs();
        File resultFile = new File(externalStorage, directory + filename);
        resultFile.setWritable(true, false);
        try {
            BufferedWriter rsWriter = new BufferedWriter(new FileWriter(resultFile));
            Log.v(TAG, "Saved results in: " + resultFile.getAbsolutePath());

            float[] datapoints = times.getTimes();
            for (int i = 0; i < times.getIterations(); i++) {
                rsWriter.write(String.format("%d %f\n", i, datapoints[i] * 1000.0));
            }
            rsWriter.close();
        } catch (IOException e) {
            Log.v(TAG, "Unable to write result file " + e.getMessage());
        }
    }

    // Test case 0: Levels Vec3 Relaxed
    @MediumTest
    public void testLevelsVec3Relaxed() {
        TestAction ta = new TestAction(TestName.LEVELS_VEC3_RELAXED);
        runTest(ta, TestName.LEVELS_VEC3_RELAXED.name());
    }

    // Test case 1: Levels Vec4 Relaxed
    @MediumTest
    public void testLevelsVec4Relaxed() {
        TestAction ta = new TestAction(TestName.LEVELS_VEC4_RELAXED);
        runTest(ta, TestName.LEVELS_VEC4_RELAXED.name());
    }

    // Test case 2: Levels Vec3 Full
    @MediumTest
    public void testLevelsVec3Full() {
        TestAction ta = new TestAction(TestName.LEVELS_VEC3_FULL);
        runTest(ta, TestName.LEVELS_VEC3_FULL.name());
    }

    // Test case 3: Levels Vec4 Full
    @MediumTest
    public void testLevelsVec4Full() {
        TestAction ta = new TestAction(TestName.LEVELS_VEC4_FULL);
        runTest(ta, TestName.LEVELS_VEC4_FULL.name());
    }

    // Test case 4: Blur Radius 25
    @MediumTest
    public void testBlurRadius25() {
        TestAction ta = new TestAction(TestName.BLUR_RADIUS_25);
        runTest(ta, TestName.BLUR_RADIUS_25.name());
    }

    // Test case 5: Intrinsic Blur Radius 25
    @MediumTest
    public void testIntrinsicBlurRadius25() {
        TestAction ta = new TestAction(TestName.INTRINSIC_BLUR_RADIUS_25);
        runTest(ta, TestName.INTRINSIC_BLUR_RADIUS_25.name());
    }

    // Test case 6: Greyscale
    @MediumTest
    public void testGreyscale() {
        TestAction ta = new TestAction(TestName.GREYSCALE);
        runTest(ta, TestName.GREYSCALE.name());
    }

    // Test case 7: Grain
    @MediumTest
    public void testGrain() {
        TestAction ta = new TestAction(TestName.GRAIN);
        runTest(ta, TestName.GRAIN.name());
    }

    // Test case 8: Fisheye Full
    @MediumTest
    public void testFisheyeFull() {
        TestAction ta = new TestAction(TestName.FISHEYE_FULL);
        runTest(ta, TestName.FISHEYE_FULL.name());
    }

    // Test case 9: Fisheye Relaxed
    @MediumTest
    public void testFishEyeRelaxed() {
        TestAction ta = new TestAction(TestName.FISHEYE_RELAXED);
        runTest(ta, TestName.FISHEYE_RELAXED.name());
    }

    // Test case 10: Fisheye Approximate Full
    @MediumTest
    public void testFisheyeApproximateFull() {
        TestAction ta = new TestAction(TestName.FISHEYE_APPROXIMATE_FULL);
        runTest(ta, TestName.FISHEYE_APPROXIMATE_FULL.name());
    }

    // Test case 11: Fisheye Approximate Relaxed
    @MediumTest
    public void testFisheyeApproximateRelaxed() {
        TestAction ta = new TestAction(TestName.FISHEYE_APPROXIMATE_RELAXED);
        runTest(ta, TestName.FISHEYE_APPROXIMATE_RELAXED.name());
    }

    // Test case 12: Vignette Full
    @MediumTest
    public void testVignetteFull() {
        TestAction ta = new TestAction(TestName.VIGNETTE_FULL);
        runTest(ta, TestName.VIGNETTE_FULL.name());
    }

    // Test case 13: Vignette Relaxed
    @MediumTest
    public void testVignetteRelaxed() {
        TestAction ta = new TestAction(TestName.VIGNETTE_RELAXED);
        runTest(ta, TestName.VIGNETTE_RELAXED.name());
    }

    // Test case 14: Vignette Approximate Full
    @MediumTest
    public void testVignetteApproximateFull() {
        TestAction ta = new TestAction(TestName.VIGNETTE_APPROXIMATE_FULL);
        runTest(ta, TestName.VIGNETTE_APPROXIMATE_FULL.name());
    }

    // Test case 15: Vignette Approximate Relaxed
    @MediumTest
    public void testVignetteApproximateRelaxed() {
        TestAction ta = new TestAction(TestName.VIGNETTE_APPROXIMATE_RELAXED);
        runTest(ta, TestName.VIGNETTE_APPROXIMATE_RELAXED.name());
    }

    // Test case 16: Group Test (emulated)
    @MediumTest
    public void testGroupTestEmulated() {
        TestAction ta = new TestAction(TestName.GROUP_TEST_EMULATED);
        runTest(ta, TestName.GROUP_TEST_EMULATED.name());
    }

    // Test case 17: Group Test (native)
    @MediumTest
    public void testGroupTestNative() {
        TestAction ta = new TestAction(TestName.GROUP_TEST_NATIVE);
        runTest(ta, TestName.GROUP_TEST_NATIVE.name());
    }

    // Test case 18: Convolve 3x3
    @MediumTest
    public void testConvolve3x3() {
        TestAction ta = new TestAction(TestName.CONVOLVE_3X3);
        runTest(ta, TestName.CONVOLVE_3X3.name());
    }

    // Test case 19: Intrinsics Convolve 3x3
    @MediumTest
    public void testIntrinsicsConvolve3x3() {
        TestAction ta = new TestAction(TestName.INTRINSICS_CONVOLVE_3X3);
        runTest(ta, TestName.INTRINSICS_CONVOLVE_3X3.name());
    }

    // Test case 20: ColorMatrix
    @MediumTest
    public void testColorMatrix() {
        TestAction ta = new TestAction(TestName.COLOR_MATRIX);
        runTest(ta, TestName.COLOR_MATRIX.name());
    }

    // Test case 21: Intrinsics ColorMatrix
    @MediumTest
    public void testIntrinsicsColorMatrix() {
        TestAction ta = new TestAction(TestName.INTRINSICS_COLOR_MATRIX);
        runTest(ta, TestName.INTRINSICS_COLOR_MATRIX.name());
    }

    // Test case 22: Intrinsics ColorMatrix Grey
    @MediumTest
    public void testIntrinsicsColorMatrixGrey() {
        TestAction ta = new TestAction(TestName.INTRINSICS_COLOR_MATRIX_GREY);
        runTest(ta, TestName.INTRINSICS_COLOR_MATRIX_GREY.name());
    }

    // Test case 23: Copy
    @MediumTest
    public void testCopy() {
        TestAction ta = new TestAction(TestName.COPY);
        runTest(ta, TestName.COPY.name());
    }

    // Test case 24: CrossProcess (using LUT)
    @MediumTest
    public void testCrossProcessUsingLUT() {
        TestAction ta = new TestAction(TestName.CROSS_PROCESS_USING_LUT);
        runTest(ta, TestName.CROSS_PROCESS_USING_LUT.name());
    }

    // Test case 25: Convolve 5x5
    @MediumTest
    public void testConvolve5x5() {
        TestAction ta = new TestAction(TestName.CONVOLVE_5X5);
        runTest(ta, TestName.CONVOLVE_5X5.name());
    }

    // Test case 26: Intrinsics Convolve 5x5
    @MediumTest
    public void testIntrinsicsConvolve5x5() {
        TestAction ta = new TestAction(TestName.INTRINSICS_CONVOLVE_5X5);
        runTest(ta, TestName.INTRINSICS_CONVOLVE_5X5.name());
    }

    // Test case 27: Mandelbrot
    @MediumTest
    public void testMandelbrot() {
        TestAction ta = new TestAction(TestName.MANDELBROT_FLOAT);
        runTest(ta, TestName.MANDELBROT_FLOAT.name());
    }

    // Test case 28: Intrinsics Blend
    @MediumTest
    public void testIntrinsicsBlend() {
        TestAction ta = new TestAction(TestName.INTRINSICS_BLEND);
        runTest(ta, TestName.INTRINSICS_BLEND.name());
    }

    // Test case 29: Intrinsics Blur 25 uchar
    @MediumTest
    public void testIntrinsicsBlur25G() {
        TestAction ta = new TestAction(TestName.INTRINSICS_BLUR_25G);
        runTest(ta, TestName.INTRINSICS_BLUR_25G.name());
    }

    // Test case 30: Vibrance
    @MediumTest
    public void testVibrance() {
        TestAction ta = new TestAction(TestName.VIBRANCE);
        runTest(ta, TestName.VIBRANCE.name());
    }

    // Test case 31: BWFilter
    @MediumTest
    public void testBWFilter() {
        TestAction ta = new TestAction(TestName.BW_FILTER);
        runTest(ta, TestName.BW_FILTER.name());
    }

    // Test case 32: Shadows
    @MediumTest
    public void testShadows() {
        TestAction ta = new TestAction(TestName.SHADOWS);
        runTest(ta, TestName.SHADOWS.name());
    }

    // Test case 33: Contrast
    @MediumTest
    public void testContrast() {
        TestAction ta = new TestAction(TestName.CONTRAST);
        runTest(ta, TestName.CONTRAST.name());
    }

    // Test case 34: Exposure
    @MediumTest
    public void testExposure(){
        TestAction ta = new TestAction(TestName.EXPOSURE);
        runTest(ta, TestName.EXPOSURE.name());
    }

    // Test case 35: White Balance
    @MediumTest
    public void testWhiteBalance() {
        TestAction ta = new TestAction(TestName.WHITE_BALANCE);
        runTest(ta, TestName.WHITE_BALANCE.name());
    }

    // Test case 36: Color Cube
    @MediumTest
    public void testColorCube() {
        TestAction ta = new TestAction(TestName.COLOR_CUBE);
        runTest(ta, TestName.COLOR_CUBE.name());
    }

    // Test case 37: Color Cube (3D Intrinsic)
    @MediumTest
    public void testColorCube3DIntrinsic() {
        TestAction ta = new TestAction(TestName.COLOR_CUBE_3D_INTRINSIC);
        runTest(ta, TestName.COLOR_CUBE_3D_INTRINSIC.name());
    }

    // Test case 38: Artistic 1
    @MediumTest
    public void testArtistic1() {
        TestAction ta = new TestAction(TestName.ARTISTIC1);
        runTest(ta, TestName.ARTISTIC1.name());
    }

    // Test case 39: Resize BiCubic Script
    @MediumTest
    public void testResizeBiCubicScript() {
        TestAction ta = new TestAction(TestName.RESIZE_BI_SCRIPT);
        runTest(ta, TestName.RESIZE_BI_SCRIPT.name());
    }

    // Test case 40: Resize BiCubic Intrinsic
    @MediumTest
    public void testResizeBiCubicIntrinsic() {
        TestAction ta = new TestAction(TestName.RESIZE_BI_INTRINSIC);
        runTest(ta, TestName.RESIZE_BI_INTRINSIC.name());
    }

    // Test case 41: Posterize with invoke
    @MediumTest
    public void testPosterizeInvoke() {
        TestAction ta = new TestAction(TestName.POSTERIZE_INVOKE);
        runTest(ta, TestName.POSTERIZE_INVOKE.name());
    }

    // Test case 42: Posterize with set
    @MediumTest
    public void testPosterizeSet() {
        TestAction ta = new TestAction(TestName.POSTERIZE_SET);
        runTest(ta, TestName.POSTERIZE_SET.name());
    }

    // Test case 43 Histogram intrinsic
    @MediumTest
    public void testHistogramIntrinsic() {
        TestAction ta = new TestAction(TestName.HISTOGRAM_INTRINSIC);
        runTest(ta, TestName.HISTOGRAM_INTRINSIC.name());
    }

    // Test case 44 Histogram script
    @MediumTest
    public void testHistogramScript() {
        TestAction ta = new TestAction(TestName.HISTOGRAM_SCRIPT);
        runTest(ta, TestName.HISTOGRAM_SCRIPT.name());
    }

    // Test case 45: Mandelbrot fp64
    @MediumTest
    public void testMandelbrotfp64() {
        TestAction ta = new TestAction(TestName.MANDELBROT_DOUBLE);
        runTest(ta, TestName.MANDELBROT_DOUBLE.name());
    }

    // Test case 46: Blur Radius 25 Half Precision
    @MediumTest
    public void testBlurRadius25Half() {
        TestAction ta = new TestAction(TestName.BLUR_RADIUS_25_HALF);
        runTest(ta, TestName.BLUR_RADIUS_25_HALF.name());
    }
}
