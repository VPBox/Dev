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
 * limitations under the License
 */

package com.android.internal.telephony;

import static com.android.internal.telephony.TelephonyTestUtils.waitForMs;

import android.os.Build;
import android.util.Log;
import android.telephony.Rlog;
import android.telephony.TelephonyHistogram;

import junit.framework.Assert;
import junit.framework.TestCase;

public class ClientWakelockAccountantTest extends TestCase {
    private final static String LOG_TAG = "ClientWakelockAccountantTest";
    ClientWakelockAccountant mClient;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        mClient = new ClientWakelockAccountant("Package Name");
    }

    @Override
    public void tearDown() throws Exception {
        super.tearDown();
    }

    public void testStartAttributingWakelock() throws Exception {

        Assert.assertEquals(0, mClient.getPendingRequestCount());
        mClient.startAttributingWakelock(15, 25, 1, 100);
        Assert.assertEquals(1, mClient.getPendingRequestCount());
        mClient.startAttributingWakelock(22, 26, 2, 150);
        Assert.assertEquals(2, mClient.getPendingRequestCount());
    }

    /* This test only tests ClientWakelockAccountant to make sure that it applies the
       multiplier as expected. Here we start 1 Client and add 2 requests.
       First request multiplier stays at 1 and goes on for 200ms and second request
       multiplier stays at 0.5 and request goes on for 500ms. So totally we
       expect the wakelock time for the Client to be 200*1 + 500*0.5 = 450ms
     */
    public void testStopAttributingWakelock() throws Exception {
        mClient.startAttributingWakelock(15, 25, 1, 100);
        mClient.startAttributingWakelock(25, 26, 2, 200);
        mClient.changeConcurrentRequests(2, 200);
        Assert.assertEquals(2, mClient.getPendingRequestCount());
        Assert.assertEquals(0, mClient.mRequestStats.getCompletedRequestsCount());
        mClient.stopAttributingWakelock(15, 25, 300);
        mClient.changeConcurrentRequests(1, 300);
        Assert.assertEquals(1, mClient.mRequestStats.getRequestHistograms().size());
        mClient.stopAttributingWakelock(25, 26, 700);
        Assert.assertEquals(0, mClient.getPendingRequestCount());
        Assert.assertEquals(2, mClient.mRequestStats.getCompletedRequestsCount());
        Assert.assertEquals(600, mClient.mRequestStats.getCompletedRequestsWakelockTime());
        Assert.assertEquals(0, mClient.updatePendingRequestWakelockTime(0));
        Assert.assertEquals(2, mClient.mRequestStats.getRequestHistograms().size());
    }

    public void testStopAllPendingRequests() throws Exception {

        mClient.startAttributingWakelock(15, 25, 1, 100);
        mClient.startAttributingWakelock(22, 26, 2, 150);
        Assert.assertEquals(2, mClient.getPendingRequestCount());
        mClient.stopAllPendingRequests(300);
        Assert.assertEquals(0, mClient.getPendingRequestCount());
        Assert.assertEquals(2, mClient.mRequestStats.getCompletedRequestsCount());
        Assert.assertEquals(275, mClient.mRequestStats.getCompletedRequestsWakelockTime());
        Assert.assertEquals(0, mClient.updatePendingRequestWakelockTime(0));
        Assert.assertEquals(2, mClient.mRequestStats.getRequestHistograms().size());
    }

    public void testStartAttributingWithZeroConcurrentRequests() throws Exception {
        if(Build.IS_DEBUGGABLE) {
            try {
                mClient.startAttributingWakelock(15, 25, 0, 100);
                fail("Expecting an illegal argument Exception to be thrown");
            } catch (IllegalArgumentException e) { }
        } else {
            mClient.startAttributingWakelock(15, 25, 0, 100);
            Assert.assertEquals(1, mClient.getPendingRequestCount());
        }
    }
}
