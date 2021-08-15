/*
 * Copyright (C) 2019 The Android Open Source Project
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

package com.android.server.wifi;

import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_FAILURE_AUTHENTICATION;
import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_FAILURE_BUSY;
import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_FAILURE_CONFIGURATION;
import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_FAILURE_GENERIC;
import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_FAILURE_INVALID_NETWORK;
import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_FAILURE_INVALID_URI;
import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_FAILURE_NOT_COMPATIBLE;
import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_FAILURE_NOT_SUPPORTED;
import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_FAILURE_TIMEOUT;
import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_SUCCESS_CONFIGURATION_SENT;

import static com.android.server.wifi.DppMetrics.DPP_OPERATION_TIME;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import android.test.suitebuilder.annotation.SmallTest;

import com.android.server.wifi.nano.WifiMetricsProto;
import com.android.server.wifi.nano.WifiMetricsProto.HistogramBucketInt32;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.util.Random;

/**
 * Unit tests for {@link com.android.server.wifi.DppMetrics}.
 */
@SmallTest
public class DppMetricsTest {
    private static final int MAX_ITERATIONS = 30;

    private DppMetrics mDppMetrics = new DppMetrics();

    @Before
    public void setUp() throws Exception {
        mDppMetrics.clear();
    }

    @After
    public void cleanUp() {
        mDppMetrics.clear();
    }

    /**
     * Helper function that matches histogram buckets to an expected value
     *
     * @param index Bucket index
     * @param value Expected value
     */
    private void checkOperationBucketEqualsTo(int index, int value) {
        // Confirm that the consolidated log has the expected value
        WifiMetricsProto.WifiDppLog mWifiDppLogProto = mDppMetrics.consolidateProto();
        HistogramBucketInt32 hb = mWifiDppLogProto.dppOperationTime[index];
        assertEquals(hb.count, value);
    }

    /**
     * Helper function that returns a number to be used to call a counter update method
     *
     * @param max Upper limit for the random number
     * @return returns a random number between 0 and max
     */
    private int getNumOfTimes(int max) {
        Random random = new Random();
        return random.nextInt(max);
    }

    /**
     * Helper function to check DPP Failures in the histogram bucket
     *
     * @param key   Key to verify
     * @param value Expected value
     */
    private void checkDppFailures(int key, int value) {
        boolean found = false;

        // Confirm that the consolidated log has the expected value
        WifiMetricsProto.WifiDppLog mWifiDppLogProto = mDppMetrics.consolidateProto();

        for (WifiMetricsProto.WifiDppLog.DppFailureStatusHistogramBucket hb :
                mWifiDppLogProto.dppFailureCode) {
            if (hb.dppStatusType == key) {
                assertEquals(hb.count, value);
                found = true;
                break;
            }
        }

        assertTrue(found);
    }

    /**
     * Helper function to check DPP Successes in the histogram bucket
     *
     * @param key   Key to verify
     * @param value Expected value
     */
    private void checkDppSuccesses(int key, int value) {
        boolean found = false;

        // Confirm that the consolidated log has the expected value
        WifiMetricsProto.WifiDppLog mWifiDppLogProto = mDppMetrics.consolidateProto();

        for (WifiMetricsProto.WifiDppLog.DppConfiguratorSuccessStatusHistogramBucket hb :
                mWifiDppLogProto.dppConfiguratorSuccessCode) {
            if (hb.dppStatusType == key) {
                assertEquals(hb.count, value);
                found = true;
                break;
            }
        }

        assertTrue(found);
    }

    /**
     * Test numDppConfiguratorInitiatorRequests
     *
     * @throws Exception
     */
    @Test
    public void testUpdateDppConfiguratorInitiatorRequests() throws Exception {
        // Get a random value and call the update method 'value' times
        int value = getNumOfTimes(MAX_ITERATIONS) + 1;

        for (int i = 0; i < value; i++) {
            mDppMetrics.updateDppConfiguratorInitiatorRequests();
        }

        // Confirm that the consolidated log has the expected value
        WifiMetricsProto.WifiDppLog mWifiDppLogProto = mDppMetrics.consolidateProto();
        assertEquals(mWifiDppLogProto.numDppConfiguratorInitiatorRequests, value);
    }

    /**
     * Test numDppEnrolleeInitiatorRequests
     *
     * @throws Exception
     */
    @Test
    public void testUpdateDppEnrolleeInitiatorRequests() throws Exception {
        // Get a random value and call the update method 'value' times
        int value = getNumOfTimes(MAX_ITERATIONS) + 1;

        for (int i = 0; i < value; i++) {
            mDppMetrics.updateDppEnrolleeInitiatorRequests();
        }

        // Confirm that the consolidated log has the expected value
        WifiMetricsProto.WifiDppLog mWifiDppLogProto = mDppMetrics.consolidateProto();
        assertEquals(mWifiDppLogProto.numDppEnrolleeInitiatorRequests, value);
    }

    /**
     * Test EASY_CONNECT_EVENT_SUCCESS_CONFIGURATION_SENT
     *
     * @throws Exception
     */
    @Test
    public void testUpdateDppConfiguratorSuccess() throws Exception {
        // Get a random value and call the update method 'value' times
        int value = getNumOfTimes(MAX_ITERATIONS) + 1;

        for (int i = 0; i < value; i++) {
            mDppMetrics.updateDppConfiguratorSuccess(EASY_CONNECT_EVENT_SUCCESS_CONFIGURATION_SENT);
        }

        // Confirm that the consolidated log has the expected value
        checkDppSuccesses(WifiMetricsProto.WifiDppLog.EASY_CONNECT_EVENT_SUCCESS_CONFIGURATION_SENT,
                value);
    }

    /**
     * Test numDppEnrolleeSuccess
     *
     * @throws Exception
     */
    @Test
    public void testUpdateDppEnrolleeSuccess() throws Exception {
        // Get a random value and call the update method 'value' times
        int value = getNumOfTimes(MAX_ITERATIONS) + 1;

        for (int i = 0; i < value; i++) {
            mDppMetrics.updateDppEnrolleeSuccess();
        }

        // Confirm that the consolidated log has the expected value
        WifiMetricsProto.WifiDppLog mWifiDppLogProto = mDppMetrics.consolidateProto();
        assertEquals(mWifiDppLogProto.numDppEnrolleeSuccess, value);
    }

    /**
     * Test EASY_CONNECT_EVENT_FAILURE_INVALID_URI
     *
     * @throws Exception
     */
    @Test
    public void testUpdateDppFailureInvalidUri() throws Exception {
        // Get a random value and call the update method 'value' times
        int value = getNumOfTimes(MAX_ITERATIONS) + 1;

        for (int i = 0; i < value; i++) {
            mDppMetrics.updateDppFailure(EASY_CONNECT_EVENT_FAILURE_INVALID_URI);
        }

        // Confirm that the consolidated log has the expected value
        checkDppFailures(WifiMetricsProto.WifiDppLog.EASY_CONNECT_EVENT_FAILURE_INVALID_URI, value);
    }

    /**
     * Test EASY_CONNECT_EVENT_FAILURE_AUTHENTICATION
     *
     * @throws Exception
     */
    @Test
    public void testUpdateDppFailureAuthentication() throws Exception {
        // Get a random value and call the update method 'value' times
        int value = getNumOfTimes(MAX_ITERATIONS) + 1;

        for (int i = 0; i < value; i++) {
            mDppMetrics.updateDppFailure(EASY_CONNECT_EVENT_FAILURE_AUTHENTICATION);
        }

        // Confirm that the consolidated log has the expected value
        checkDppFailures(WifiMetricsProto.WifiDppLog.EASY_CONNECT_EVENT_FAILURE_AUTHENTICATION,
                value);
    }

    /**
     * Test EASY_CONNECT_EVENT_FAILURE_NOT_COMPATIBLE
     *
     * @throws Exception
     */
    @Test
    public void testUpdateDppFailureNotCompatible() throws Exception {
        // Get a random value and call the update method 'value' times
        int value = getNumOfTimes(MAX_ITERATIONS) + 1;

        for (int i = 0; i < value; i++) {
            mDppMetrics.updateDppFailure(EASY_CONNECT_EVENT_FAILURE_NOT_COMPATIBLE);
        }

        // Confirm that the consolidated log has the expected value
        checkDppFailures(WifiMetricsProto.WifiDppLog.EASY_CONNECT_EVENT_FAILURE_NOT_COMPATIBLE,
                value);
    }

    /**
     * Test EASY_CONNECT_EVENT_FAILURE_CONFIGURATION
     *
     * @throws Exception
     */
    @Test
    public void testUpdateDppFailureConfiguration() throws Exception {
        // Get a random value and call the update method 'value' times
        int value = getNumOfTimes(MAX_ITERATIONS) + 1;

        for (int i = 0; i < value; i++) {
            mDppMetrics.updateDppFailure(EASY_CONNECT_EVENT_FAILURE_CONFIGURATION);
        }

        // Confirm that the consolidated log has the expected value
        checkDppFailures(WifiMetricsProto.WifiDppLog.EASY_CONNECT_EVENT_FAILURE_CONFIGURATION,
                value);
    }

    /**
     * Test EASY_CONNECT_EVENT_FAILURE_BUSY
     *
     * @throws Exception
     */
    @Test
    public void testUpdateDppFailureBusy() throws Exception {
        // Get a random value and call the update method 'value' times
        int value = getNumOfTimes(MAX_ITERATIONS) + 1;

        for (int i = 0; i < value; i++) {
            mDppMetrics.updateDppFailure(EASY_CONNECT_EVENT_FAILURE_BUSY);
        }

        // Confirm that the consolidated log has the expected value
        checkDppFailures(WifiMetricsProto.WifiDppLog.EASY_CONNECT_EVENT_FAILURE_BUSY, value);
    }

    /**
     * Test EASY_CONNECT_EVENT_FAILURE_TIMEOUT
     *
     * @throws Exception
     */
    @Test
    public void testUpdateDppFailureTimeout() throws Exception {
        // Get a random value and call the update method 'value' times
        int value = getNumOfTimes(MAX_ITERATIONS) + 1;

        for (int i = 0; i < value; i++) {
            mDppMetrics.updateDppFailure(EASY_CONNECT_EVENT_FAILURE_TIMEOUT);
        }

        // Confirm that the consolidated log has the expected value
        checkDppFailures(WifiMetricsProto.WifiDppLog.EASY_CONNECT_EVENT_FAILURE_TIMEOUT, value);
    }

    /**
     * Test EASY_CONNECT_EVENT_FAILURE_GENERIC
     *
     * @throws Exception
     */
    @Test
    public void testUpdateDppFailureGeneric() throws Exception {
        // Get a random value and call the update method 'value' times
        int value = getNumOfTimes(MAX_ITERATIONS) + 1;

        for (int i = 0; i < value; i++) {
            mDppMetrics.updateDppFailure(EASY_CONNECT_EVENT_FAILURE_GENERIC);
        }

        // Confirm that the consolidated log has the expected value
        checkDppFailures(WifiMetricsProto.WifiDppLog.EASY_CONNECT_EVENT_FAILURE_GENERIC, value);
    }

    /**
     * Test EASY_CONNECT_EVENT_FAILURE_NOT_SUPPORTED
     *
     * @throws Exception
     */
    @Test
    public void testUpdateDppFailureNotSupported() throws Exception {
        // Get a random value and call the update method 'value' times
        int value = getNumOfTimes(MAX_ITERATIONS) + 1;

        for (int i = 0; i < value; i++) {
            mDppMetrics.updateDppFailure(EASY_CONNECT_EVENT_FAILURE_NOT_SUPPORTED);
        }

        // Confirm that the consolidated log has the expected value
        checkDppFailures(WifiMetricsProto.WifiDppLog.EASY_CONNECT_EVENT_FAILURE_NOT_SUPPORTED,
                value);
    }

    /**
     * Test EASY_CONNECT_EVENT_FAILURE_INVALID_NETWORK
     *
     * @throws Exception
     */
    @Test
    public void testUpdateDppFailureInvalidNetwork() throws Exception {
        // Get a random value and call the update method 'value' times
        int value = getNumOfTimes(MAX_ITERATIONS) + 1;

        for (int i = 0; i < value; i++) {
            mDppMetrics.updateDppFailure(EASY_CONNECT_EVENT_FAILURE_INVALID_NETWORK);
        }

        // Confirm that the consolidated log has the expected value
        checkDppFailures(WifiMetricsProto.WifiDppLog.EASY_CONNECT_EVENT_FAILURE_INVALID_NETWORK,
                value);
    }

    /**
     * Test DPP operation time histogram. Pick a single time value from each bucket by selecting
     * the max value minus 1, and call the update method random amount of times with this value.
     * Then confirm that the output histogram has the expected value in each target bucket.
     *
     * @throws Exception
     */
    @Test
    public void testUpdateHistogramDppOperationTime() throws Exception {
        // Iterate through the histogram array
        for (int i = 0; i <= DPP_OPERATION_TIME.length; i++) {
            // Get a random value and call the operation time update method
            int value = getNumOfTimes(MAX_ITERATIONS) + 1;
            int timeMs;

            if (i < DPP_OPERATION_TIME.length) {
                timeMs = DPP_OPERATION_TIME[i] - 1;
            } else {
                timeMs = DPP_OPERATION_TIME[i - 1];
            }

            // Framework uses milliseconds when it gets the time, convert to milliseconds
            timeMs *= 1000;

            for (int j = 0; j < value; j++) {
                // Add a time value that would fall in the i'th bucket
                mDppMetrics.updateDppOperationTime(timeMs);
            }

            // Confirm that output matches the expected value
            checkOperationBucketEqualsTo(i, value);
        }
    }
}
