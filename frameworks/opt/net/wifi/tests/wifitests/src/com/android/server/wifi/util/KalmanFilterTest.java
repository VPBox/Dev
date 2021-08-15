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

package com.android.server.wifi.util;

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import androidx.test.filters.SmallTest;

import org.junit.Test;

import java.util.Random;

/**
 * Unit tests for {@link com.android.server.wifi.util.KalmanFilter}.
 */
@SmallTest
public class KalmanFilterTest {
    /**
     * Test that constructor works
     */
    @Test
    public void testConstructor() throws Exception {
        KalmanFilter kf = new KalmanFilter();
        assertNotNull(kf);
    }

    /**
     * Sets up a Kalman filter to behave as as a phase-locked loop
     * <p>
     * Set up a 2-D model that generates sinusoidal output at a fixed frequency; the state
     * transformation is just a rotation by a fixed angle, and the output matrix projects oen
     * of the dimensions.
     */
    private KalmanFilter initializePll(double stepSizeRadians,
                                       double modelStandardDeviation,
                                       double measurementStandardDeviation) {
        KalmanFilter kf = new KalmanFilter();
        double cos = Math.cos(stepSizeRadians);
        double sin = Math.sin(stepSizeRadians);
        kf.mF = new Matrix(2, new double[]{
                cos, sin,
                -sin, cos});
        double modelVariance = modelStandardDeviation * modelStandardDeviation;
        kf.mQ = new Matrix(2, new double[]{
                modelVariance, 0.0,
                0.0, modelVariance});
        kf.mH = new Matrix(2, new double[]{1.0, 0.0});
        double measurementVariance = measurementStandardDeviation * measurementStandardDeviation;
        kf.mR = new Matrix(1, new double[]{measurementVariance});
        double initialAPosterioriVariance = 10000.0;
        kf.mP = new Matrix(2, new double[]{
                initialAPosterioriVariance, 0.0,
                0.0, initialAPosterioriVariance});
        kf.mx = new Matrix(2, 1);
        return kf;
    }

    private double mAmplitude = 10.0;
    private double mStepSizeRadians = Math.PI / 17.01;
    private int mTransitionPoint = 300;

    /**
     * Generates the ideal signal at time step i.
     * <p>
     * Sinusoid, with an abrupt phase shift thrown in to test transient response
     */
    private double idealSignal(int i) {
        double phi = mStepSizeRadians * i;
        if (i > mTransitionPoint) {
            phi = phi + Math.PI * .75;
        }
        return mAmplitude * Math.sin(phi);
    }

    private double mNoiseAmplitude = 3.0;

    private int mSteps = 500;
    private int mSeed = 271828;

    /**
     * Test that using the phase locked loop Kalman filter results in a residual that is
     * a lot smaller than the noise in the signal.
     */
    @Test
    public void testPhaseLockedLoop() throws Exception {
        Random random = new Random(mSeed);
        double modelStandardDeviation = 0.5;
        double [] noise = new double[mSteps];
        for (int i = 0; i < mSteps; i++) {
            noise[i] = random.nextGaussian() * mNoiseAmplitude;
        }
        double [] filtered = new double[mSteps];
        double [] residual = new double[mSteps];
        KalmanFilter kf = initializePll(mStepSizeRadians, modelStandardDeviation, mNoiseAmplitude);
        for (int i = 0; i < mSteps; i++) {
            kf.predict();
            kf.update(new Matrix(1, new double[] {idealSignal(i) + noise[i]}));
            filtered[i] = kf.mx.get(0, 0);
            residual[i] = filtered[i] - idealSignal(i);
        }
        double sum = 0.0;
        double sumSquares = 0.0;
        double n = 0.0;
        for (int i = 0; i < mSteps; i++) {
            if (i < 5 || (i > mTransitionPoint && i < mTransitionPoint + 20)) continue;
            sum += residual[i];
            sumSquares += residual[i] * residual[i];
            n += 1.0;
        }
        double mean = sum / n;
        double variance = (sumSquares - sum * sum) / (n * n);
        assertTrue(mean < 0.1);
        assertTrue(variance < 1.5);
        assertNotNull(kf.toString());
    }

    /**
     * Test that the toString method works even if the matrices have not been set.
     */
    @Test
    public void testToStrinWithNullsInside() throws Exception {
        assertNotNull(new KalmanFilter().toString());
    }
}
