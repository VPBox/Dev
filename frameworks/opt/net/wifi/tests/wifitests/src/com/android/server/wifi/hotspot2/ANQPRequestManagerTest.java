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

package com.android.server.wifi.hotspot2;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.anyLong;
import static org.mockito.Mockito.anyObject;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.reset;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;
import static org.mockito.MockitoAnnotations.initMocks;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.Clock;
import com.android.server.wifi.hotspot2.anqp.Constants;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;

import java.util.Arrays;
import java.util.List;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.ANQPRequestManager}.
 */
@SmallTest
public class ANQPRequestManagerTest {
    private static final long TEST_BSSID = 0x123456L;
    private static final ANQPNetworkKey TEST_ANQP_KEY =
            new ANQPNetworkKey("TestSSID", TEST_BSSID, 0, 0);

    private static final List<Constants.ANQPElementType> R1_ANQP_WITHOUT_RC = Arrays.asList(
            Constants.ANQPElementType.ANQPVenueName,
            Constants.ANQPElementType.ANQPIPAddrAvailability,
            Constants.ANQPElementType.ANQPNAIRealm,
            Constants.ANQPElementType.ANQP3GPPNetwork,
            Constants.ANQPElementType.ANQPDomName);

    private static final List<Constants.ANQPElementType> R1_ANQP_WITH_RC = Arrays.asList(
            Constants.ANQPElementType.ANQPVenueName,
            Constants.ANQPElementType.ANQPIPAddrAvailability,
            Constants.ANQPElementType.ANQPNAIRealm,
            Constants.ANQPElementType.ANQP3GPPNetwork,
            Constants.ANQPElementType.ANQPDomName,
            Constants.ANQPElementType.ANQPRoamingConsortium);

    private static final List<Constants.ANQPElementType> R1R2_ANQP_WITHOUT_RC = Arrays.asList(
            Constants.ANQPElementType.ANQPVenueName,
            Constants.ANQPElementType.ANQPIPAddrAvailability,
            Constants.ANQPElementType.ANQPNAIRealm,
            Constants.ANQPElementType.ANQP3GPPNetwork,
            Constants.ANQPElementType.ANQPDomName,
            Constants.ANQPElementType.HSFriendlyName,
            Constants.ANQPElementType.HSWANMetrics,
            Constants.ANQPElementType.HSConnCapability,
            Constants.ANQPElementType.HSOSUProviders);

    private static final List<Constants.ANQPElementType> R1R2_ANQP_WITH_RC = Arrays.asList(
            Constants.ANQPElementType.ANQPVenueName,
            Constants.ANQPElementType.ANQPIPAddrAvailability,
            Constants.ANQPElementType.ANQPNAIRealm,
            Constants.ANQPElementType.ANQP3GPPNetwork,
            Constants.ANQPElementType.ANQPDomName,
            Constants.ANQPElementType.ANQPRoamingConsortium,
            Constants.ANQPElementType.HSFriendlyName,
            Constants.ANQPElementType.HSWANMetrics,
            Constants.ANQPElementType.HSConnCapability,
            Constants.ANQPElementType.HSOSUProviders);

    @Mock PasspointEventHandler mHandler;
    @Mock Clock mClock;
    ANQPRequestManager mManager;

    /**
     * Test setup.
     */
    @Before
    public void setUp() throws Exception {
        initMocks(this);
        mManager = new ANQPRequestManager(mHandler, mClock);
    }

    /**
     * Verify that the expected set of ANQP elements are being requested when the targeted AP
     * doesn't provide roaming consortium OIs and doesn't support Hotspot 2.0 Release 2 ANQP
     * elements, based on the IEs in the scan result .
     *
     * @throws Exception
     */
    @Test
    public void requestR1ANQPElementsWithoutRC() throws Exception {
        when(mHandler.requestANQP(TEST_BSSID, R1_ANQP_WITHOUT_RC)).thenReturn(true);
        assertTrue(mManager.requestANQPElements(TEST_BSSID, TEST_ANQP_KEY, false, false));
    }

    /**
     * Verify that the expected set of ANQP elements are being requested when the targeted AP
     * does provide roaming consortium OIs and doesn't support Hotspot 2.0 Release ANQP elements,
     * based on the IEs in the scan result.
     *
     * @throws Exception
     */
    @Test
    public void requestR1ANQPElementsWithRC() throws Exception {
        when(mHandler.requestANQP(TEST_BSSID, R1_ANQP_WITH_RC)).thenReturn(true);
        assertTrue(mManager.requestANQPElements(TEST_BSSID, TEST_ANQP_KEY, true, false));
    }

    /**
     * Verify that the expected set of ANQP elements are being requested when the targeted AP
     * doesn't provide roaming consortium OIs and does support Hotspot 2.0 Release ANQP elements,
     * based on the IEs in the scan result.
     *
     * @throws Exception
     */
    @Test
    public void requestR1R2ANQPElementsWithoutRC() throws Exception {
        when(mHandler.requestANQP(TEST_BSSID, R1R2_ANQP_WITHOUT_RC)).thenReturn(true);
        assertTrue(mManager.requestANQPElements(TEST_BSSID, TEST_ANQP_KEY, false, true));
    }

    /**
     * Verify that the expected set of ANQP elements are being requested when the targeted AP
     * does provide roaming consortium OIs and support Hotspot 2.0 Release ANQP elements,
     * based on the IEs in the scan result.
     *
     * @throws Exception
     */
    @Test
    public void requestR1R2ANQPElementsWithRC() throws Exception {
        when(mHandler.requestANQP(TEST_BSSID, R1R2_ANQP_WITH_RC)).thenReturn(true);
        assertTrue(mManager.requestANQPElements(TEST_BSSID, TEST_ANQP_KEY, true, true));
    }

    /**
     * Verify that attempt to request ANQP elements from an AP will fail when there is a request
     * already pending.  The request will succeed when the hold off time is up.
     *
     * @throws Exception
     */
    @Test
    public void requestANQPElementsWithPendingRequest() throws Exception {
        // Send the initial request.
        long startTime = 0;
        when(mHandler.requestANQP(TEST_BSSID, R1_ANQP_WITHOUT_RC)).thenReturn(true);
        when(mClock.getElapsedSinceBootMillis()).thenReturn(startTime);
        assertTrue(mManager.requestANQPElements(TEST_BSSID, TEST_ANQP_KEY, false, false));
        reset(mHandler);

        // Attempt another request will fail while one is still pending and hold off time is not up
        // yet.
        when(mClock.getElapsedSinceBootMillis()).thenReturn(startTime + 1);
        assertFalse(mManager.requestANQPElements(TEST_BSSID, TEST_ANQP_KEY, false, false));
        verify(mHandler, never()).requestANQP(anyLong(), anyObject());
        reset(mHandler);

        // Attempt other request will succeed after the hold off time is up.
        when(mHandler.requestANQP(TEST_BSSID, R1_ANQP_WITHOUT_RC)).thenReturn(true);
        when(mClock.getElapsedSinceBootMillis())
                .thenReturn(startTime + ANQPRequestManager.BASE_HOLDOFF_TIME_MILLISECONDS);
        assertTrue(mManager.requestANQPElements(TEST_BSSID, TEST_ANQP_KEY, false, false));
    }

    /**
     * Verify that an immediate attempt to request ANQP elements from an AP will succeed when
     * the previous request is failed on sending.
     *
     * @throws Exception
     */
    @Test
    public void requestANQPElementsAfterRequestSendFailure() throws Exception {
        // Initial request failed to send.
        long startTime = 0;
        when(mHandler.requestANQP(TEST_BSSID, R1_ANQP_WITHOUT_RC)).thenReturn(false);
        when(mClock.getElapsedSinceBootMillis()).thenReturn(startTime);
        assertFalse(mManager.requestANQPElements(TEST_BSSID, TEST_ANQP_KEY, false, false));
        reset(mHandler);

        // Verify that new request is not being held off after previous send failure.
        when(mHandler.requestANQP(TEST_BSSID, R1_ANQP_WITHOUT_RC)).thenReturn(true);
        when(mClock.getElapsedSinceBootMillis()).thenReturn(startTime);
        assertTrue(mManager.requestANQPElements(TEST_BSSID, TEST_ANQP_KEY, false, false));
    }

    /**
     * Verify that an immediate attempt to request ANQP elements from an AP will succeed when
     * the previous request is completed with success.
     *
     * @throws Exception
     */
    @Test
    public void requestANQPElementsAfterRequestSucceeded() throws Exception {
        // Send the initial request.
        long startTime = 0;
        when(mHandler.requestANQP(TEST_BSSID, R1_ANQP_WITHOUT_RC)).thenReturn(true);
        when(mClock.getElapsedSinceBootMillis()).thenReturn(startTime);
        assertTrue(mManager.requestANQPElements(TEST_BSSID, TEST_ANQP_KEY, false, false));
        reset(mHandler);

        // Request completed with success. Verify that the key associated with the request
        // is returned.
        assertEquals(TEST_ANQP_KEY, mManager.onRequestCompleted(TEST_BSSID, true));

        when(mHandler.requestANQP(TEST_BSSID, R1_ANQP_WITHOUT_RC)).thenReturn(true);
        when(mClock.getElapsedSinceBootMillis()).thenReturn(startTime + 1);
        assertTrue(mManager.requestANQPElements(TEST_BSSID, TEST_ANQP_KEY, false, false));
    }

    /**
     * Verify that an immediate attempt to request ANQP elements from an AP will fail when
     * the previous request is completed with failure.  The request will succeed after the
     * hold off time is up.
     *
     * @throws Exception
     */
    @Test
    public void requestANQPElementsAfterRequestFailed() throws Exception {
        // Send the initial request.
        long startTime = 0;
        when(mHandler.requestANQP(TEST_BSSID, R1_ANQP_WITHOUT_RC)).thenReturn(true);
        when(mClock.getElapsedSinceBootMillis()).thenReturn(startTime);
        assertTrue(mManager.requestANQPElements(TEST_BSSID, TEST_ANQP_KEY, false, false));
        reset(mHandler);

        // Request completed with failure.  Verify that the key associated with the request
        // is returned
        assertEquals(TEST_ANQP_KEY, mManager.onRequestCompleted(TEST_BSSID, false));

        // Attempt another request will fail since the hold off time is not up yet.
        when(mClock.getElapsedSinceBootMillis()).thenReturn(startTime + 1);
        assertFalse(mManager.requestANQPElements(TEST_BSSID, TEST_ANQP_KEY, false, false));
        verify(mHandler, never()).requestANQP(anyLong(), anyObject());

        // Attempt another request will succeed after the hold off time is up.
        when(mHandler.requestANQP(TEST_BSSID, R1_ANQP_WITHOUT_RC)).thenReturn(true);
        when(mClock.getElapsedSinceBootMillis())
                .thenReturn(startTime + ANQPRequestManager.BASE_HOLDOFF_TIME_MILLISECONDS);
        assertTrue(mManager.requestANQPElements(TEST_BSSID, TEST_ANQP_KEY, false, false));
    }

    /**
     * Verify the hold off time for each unanswered query, and that it will stay the same after
     * reaching the max hold off count {@link ANQPRequestManager#MAX_HOLDOFF_COUNT}.
     *
     * @throws Exception
     */
    @Test
    public void requestANQPElementsWithMaxRetries() throws Exception {
        long currentTime = 0;

        // Initial request.
        when(mHandler.requestANQP(TEST_BSSID, R1_ANQP_WITHOUT_RC)).thenReturn(true);
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTime);
        assertTrue(mManager.requestANQPElements(TEST_BSSID, TEST_ANQP_KEY, false, false));
        reset(mHandler);

        // Sending the request with the hold off time based on the current hold off count.
        for (int i = 0; i <= ANQPRequestManager.MAX_HOLDOFF_COUNT; i++) {
            long currentHoldOffTime = ANQPRequestManager.BASE_HOLDOFF_TIME_MILLISECONDS * (1 << i);
            currentTime += (currentHoldOffTime - 1);

            // Request will fail before the hold off time is up.
            when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTime);
            assertFalse(mManager.requestANQPElements(TEST_BSSID, TEST_ANQP_KEY, false, false));
            verify(mHandler, never()).requestANQP(anyLong(), anyObject());

            // Request will succeed when the hold off time is up.
            currentTime += 1;
            when(mHandler.requestANQP(TEST_BSSID, R1_ANQP_WITHOUT_RC)).thenReturn(true);
            when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTime);
            assertTrue(mManager.requestANQPElements(TEST_BSSID, TEST_ANQP_KEY, false, false));
            reset(mHandler);
        }

        // Verify that the hold off time is max out at the maximum hold off count.
        currentTime += (ANQPRequestManager.BASE_HOLDOFF_TIME_MILLISECONDS
                * (1 << ANQPRequestManager.MAX_HOLDOFF_COUNT) - 1);

        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTime);
        assertFalse(mManager.requestANQPElements(TEST_BSSID, TEST_ANQP_KEY, false, false));
        verify(mHandler, never()).requestANQP(anyLong(), anyObject());

        currentTime += 1;
        when(mHandler.requestANQP(TEST_BSSID, R1_ANQP_WITHOUT_RC)).thenReturn(true);
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTime);
        assertTrue(mManager.requestANQPElements(TEST_BSSID, TEST_ANQP_KEY, false, false));
        reset(mHandler);
    }
}
