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

import androidx.test.filters.FlakyTest;

import junit.framework.TestCase;

public class ClientWakelockTrackerTest extends TestCase {
    ClientWakelockTracker myTracker;

    public void setUp() throws Exception {
        super.setUp();
        myTracker = new ClientWakelockTracker();
    }

    public void tearDown() throws Exception {
        super.tearDown();
    }

    /* This test has client "ABC" send 1 message at time t and gets response at t+40,
    client "PQR" sends a message at t+20 and gets response at t+120. Verify that
    "ABC" is attributed 30ms and "PQR" 90ms of the total wakelock time of 120ms
     */
    @FlakyTest /* flakes 0.37% of the time */
    public void testTwoClients() throws Exception {
        myTracker.startTracking("ABC", 101, 1, 1);
        waitForMs(20);
        assertEquals(1, myTracker.mActiveClients.size());
        myTracker.startTracking("PQR", 102, 2, 2);
        assertEquals(2, myTracker.mActiveClients.size());
        ClientWakelockAccountant abc = myTracker.mClients.get("ABC");
        ClientWakelockAccountant pqr = myTracker.mClients.get("PQR");
        assertEquals(2, abc.mPendingRilWakelocks.get(0).getConcurrentRequests());
        assertEquals(2, pqr.mPendingRilWakelocks.get(0).getConcurrentRequests());
        waitForMs(20);
        myTracker.stopTracking("ABC", 101, 1, 1);
        assertEquals(1, myTracker.mActiveClients.size());
        assertEquals(0, abc.getPendingRequestCount());
        assertEquals(1, pqr.mPendingRilWakelocks.get(0).getConcurrentRequests());
        waitForMs(80);
        myTracker.stopTracking("PQR", 102, 2, 0);
        assertEquals(0, myTracker.mActiveClients.size());
        assertEquals(0, abc.getPendingRequestCount());
        assertEquals(0, pqr.getPendingRequestCount());

        assertTimeTaken(abc, 30);
        assertTimeTaken(pqr, 90);
    }

    private void assertTimeTaken(ClientWakelockAccountant abc, int time) {
        assertTrue(abc.mRequestStats.getCompletedRequestsWakelockTime() > (time - 1));
        assertTrue(abc.mRequestStats.getCompletedRequestsWakelockTime() < (time + 19));
    }

    /* This test has client "ABC" send 1 message at time t and gets response at t+40,
    and sends another message at t+20 and gets response at t+120. Verify that
    "ABC" is attributed 120ms
     */
    @FlakyTest /* flakes 0.37% of the time */
    public void testOneClient() throws Exception {
        myTracker.startTracking("ABC", 101, 1, 1);
        waitForMs(20);
        assertEquals(1, myTracker.mActiveClients.size());
        myTracker.startTracking("ABC", 102, 2, 2);
        assertEquals(1, myTracker.mActiveClients.size());
        ClientWakelockAccountant abc = myTracker.mClients.get("ABC");
        assertEquals(2, abc.mPendingRilWakelocks.get(0).getConcurrentRequests());
        assertEquals(2, abc.mPendingRilWakelocks.get(1).getConcurrentRequests());
        waitForMs(20);
        myTracker.stopTracking("ABC", 101, 1, 1);
        assertEquals(1, myTracker.mActiveClients.size());
        assertEquals(1, abc.getPendingRequestCount());
        assertEquals(1, abc.mPendingRilWakelocks.get(0).getConcurrentRequests());
        waitForMs(80);
        myTracker.stopTracking("ABC", 102, 2, 0);
        assertEquals(0, myTracker.mActiveClients.size());
        assertEquals(0, abc.getPendingRequestCount());
        assertEquals(2, abc.mRequestStats.getCompletedRequestsCount());

        assertTimeTaken(abc, 120);
    }

    /* This test has client "ABC" send 1 message at time t and another at time t+20
    and gets response for all at t+40. Verify that "ABC" is attributed 40ms
     */
    @FlakyTest /* flakes 0.37% of the time */
    public void testStopTrackingAllOneClient() throws Exception {
        myTracker.startTracking("ABC", 101, 1, 1);
        waitForMs(20);
        assertEquals(1, myTracker.mActiveClients.size());
        myTracker.startTracking("ABC", 102, 2, 2);
        ClientWakelockAccountant abc = myTracker.mClients.get("ABC");
        assertEquals(1, myTracker.mActiveClients.size());
        assertEquals(2, abc.mPendingRilWakelocks.get(0).getConcurrentRequests());
        assertEquals(2, abc.mPendingRilWakelocks.get(1).getConcurrentRequests());
        waitForMs(20);
        myTracker.stopTrackingAll();
        assertEquals(0, myTracker.mActiveClients.size());
        assertEquals(0, abc.getPendingRequestCount());
        assertEquals(2, abc.mRequestStats.getCompletedRequestsCount());

        assertTimeTaken(abc, 40);
    }

    /* This test has client "ABC" send 1 message at time t and client "PQR" sends 1 message
     at time (t+20)ms. Both of them get response at (t+40). Verify that
    "ABC" is attributed 30ms amd PQR is attributed 10ms
     */
    public void testStopTrackingAllTwoClients() throws Exception {
        myTracker.startTracking("ABC", 101, 1, 1);
        waitForMs(20);
        assertEquals(1, myTracker.mActiveClients.size());
        myTracker.startTracking("PQR", 102, 2, 2);
        ClientWakelockAccountant abc = myTracker.mClients.get("ABC");
        ClientWakelockAccountant pqr = myTracker.mClients.get("PQR");
        assertEquals(2, myTracker.mActiveClients.size());
        assertEquals(2, abc.mPendingRilWakelocks.get(0).getConcurrentRequests());
        assertEquals(2, pqr.mPendingRilWakelocks.get(0).getConcurrentRequests());
        waitForMs(20);
        myTracker.stopTrackingAll();
        assertEquals(0, myTracker.mActiveClients.size());
        assertEquals(0, abc.getPendingRequestCount());
        assertEquals(1, abc.mRequestStats.getCompletedRequestsCount());
        assertEquals(0, pqr.getPendingRequestCount());

        assertTimeTaken(pqr, 10);
        assertTimeTaken(abc, 30);
    }
}
