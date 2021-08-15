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

package com.android.server.wifi;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.*;
import static org.mockito.MockitoAnnotations.*;

import android.content.Context;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiSsid;
import android.os.Handler;
import android.os.test.TestLooper;
import android.provider.DeviceConfig.OnPropertiesChangedListener;
import android.util.Pair;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * Unit tests for {@link com.android.server.wifi.WifiLastResortWatchdog}.
 */
@SmallTest
public class WifiLastResortWatchdogTest {
    final ArgumentCaptor<OnPropertiesChangedListener> mOnPropertiesChangedListenerCaptor =
            ArgumentCaptor.forClass(OnPropertiesChangedListener.class);
    WifiLastResortWatchdog mLastResortWatchdog;
    @Mock WifiInjector mWifiInjector;
    @Mock WifiMetrics mWifiMetrics;
    @Mock SelfRecovery mSelfRecovery;
    @Mock ClientModeImpl mClientModeImpl;
    @Mock Clock mClock;
    @Mock WifiInfo mWifiInfo;
    @Mock Context mContext;
    @Mock DeviceConfigFacade mDeviceConfigFacade;

    private String[] mSsids = {"\"test1\"", "\"test2\"", "\"test3\"", "\"test4\""};
    private String[] mBssids = {"6c:f3:7f:ae:8c:f3", "6c:f3:7f:ae:8c:f4", "de:ad:ba:b1:e5:55",
            "c0:ff:ee:ee:e3:ee"};
    private int[] mFrequencies = {2437, 5180, 5180, 2437};
    private String[] mCaps = {"[WPA2-EAP-CCMP][ESS]", "[WPA2-EAP-CCMP][ESS]",
            "[WPA2-EAP-CCMP][ESS]", "[WPA2-EAP-CCMP][ESS]"};
    private int[] mLevels = {-60, -86, -50, -62};
    private boolean[] mIsEphemeral = {false, false, false, false};
    private boolean[] mHasEverConnected = {false, false, false, false};
    private TestLooper mLooper;
    private static final String TEST_NETWORK_SSID = "\"test_ssid\"";
    private static final int DEFAULT_ABNORMAL_CONNECTION_DURATION_MS = 30000;

    @Before
    public void setUp() throws Exception {
        initMocks(this);
        mLooper = new TestLooper();
        when(mWifiInjector.getSelfRecovery()).thenReturn(mSelfRecovery);
        when(mDeviceConfigFacade.isAbnormalConnectionBugreportEnabled()).thenReturn(true);
        when(mDeviceConfigFacade.getAbnormalConnectionDurationMs()).thenReturn(
                        DEFAULT_ABNORMAL_CONNECTION_DURATION_MS);
        mLastResortWatchdog = new WifiLastResortWatchdog(mWifiInjector, mContext, mClock,
                mWifiMetrics, mClientModeImpl, mLooper.getLooper(), mDeviceConfigFacade);
        mLastResortWatchdog.setBugReportProbability(1);
        when(mClientModeImpl.getWifiInfo()).thenReturn(mWifiInfo);
        when(mWifiInfo.getSSID()).thenReturn(TEST_NETWORK_SSID);
        when(mWifiInjector.getClientModeImplHandler()).thenReturn(mLastResortWatchdog.getHandler());
        verify(mDeviceConfigFacade).addOnPropertiesChangedListener(any(),
                mOnPropertiesChangedListenerCaptor.capture());
    }

    private List<Pair<ScanDetail, WifiConfiguration>> createFilteredQnsCandidates(String[] ssids,
            String[] bssids, int[] frequencies, String[] caps, int[] levels,
            boolean[] isEphemeral) {
        List<Pair<ScanDetail, WifiConfiguration>> candidates = new ArrayList<>();
        long timeStamp = System.currentTimeMillis();
        for (int index = 0; index < ssids.length; index++) {
            String ssid = ssids[index].replaceAll("^\"+", "").replaceAll("\"+$", "");
            ScanDetail scanDetail = new ScanDetail(WifiSsid.createFromAsciiEncoded(ssid),
                    bssids[index], caps[index], levels[index], frequencies[index], timeStamp,
                    0);
            WifiConfiguration config = null;
            if (!isEphemeral[index]) {
                config = mock(WifiConfiguration.class);
                WifiConfiguration.NetworkSelectionStatus networkSelectionStatus =
                        mock(WifiConfiguration.NetworkSelectionStatus.class);
                when(config.getNetworkSelectionStatus()).thenReturn(networkSelectionStatus);
                when(networkSelectionStatus.getHasEverConnected()).thenReturn(true);
            }
            candidates.add(Pair.create(scanDetail, config));
        }
        return candidates;
    }

    private List<Pair<ScanDetail, WifiConfiguration>> createFilteredQnsCandidates(String[] ssids,
            String[] bssids, int[] frequencies, String[] caps, int[] levels,
            boolean[] isEphemeral, boolean[] hasEverConnected) {
        List<Pair<ScanDetail, WifiConfiguration>> candidates =
                new ArrayList<Pair<ScanDetail, WifiConfiguration>>();
        long timeStamp = System.currentTimeMillis();
        for (int index = 0; index < ssids.length; index++) {
            String ssid = ssids[index].replaceAll("^\"+", "").replaceAll("\"+$", "");
            ScanDetail scanDetail = new ScanDetail(WifiSsid.createFromAsciiEncoded(ssid),
                    bssids[index], caps[index], levels[index], frequencies[index], timeStamp,
                    0);
            WifiConfiguration config = null;
            if (!isEphemeral[index]) {
                config = mock(WifiConfiguration.class);
                WifiConfiguration.NetworkSelectionStatus networkSelectionStatus =
                        mock(WifiConfiguration.NetworkSelectionStatus.class);
                when(config.getNetworkSelectionStatus()).thenReturn(networkSelectionStatus);
                when(networkSelectionStatus.getHasEverConnected())
                        .thenReturn(hasEverConnected[index]);
            }
            candidates.add(Pair.create(scanDetail, config));
        }
        return candidates;
    }

    private void assertFailureCountEquals(
            String bssid, int associationRejections, int authenticationFailures, int dhcpFailures) {
        assertEquals(associationRejections, mLastResortWatchdog.getFailureCount(bssid,
                WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION));
        assertEquals(authenticationFailures, mLastResortWatchdog.getFailureCount(bssid,
                WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION));
        assertEquals(dhcpFailures, mLastResortWatchdog.getFailureCount(bssid,
                WifiLastResortWatchdog.FAILURE_CODE_DHCP));
    }

    /**
     * Case #1: Test aging works in available network buffering
     * This test simulates 4 networks appearing in a scan result, and then only the first 2
     * appearing in successive scans results.
     * Expected Behavior:
     * 4 networks appear in recentAvailalbeNetworks, after N=MAX_BSSID_AGE scans, only 2 remain
     */
    @Test
    public void testAvailableNetworkBuffering_ageCullingWorks() throws Exception {
        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral);
        mLastResortWatchdog.updateAvailableNetworks(candidates);
        assertEquals(mLastResortWatchdog.getRecentAvailableNetworks().size(), 4);

        // Repeatedly buffer candidates 1 & 2, MAX_BSSID_AGE - 1 times
        candidates = createFilteredQnsCandidates(Arrays.copyOfRange(mSsids, 0, 2),
                Arrays.copyOfRange(mBssids, 0, 2),
                Arrays.copyOfRange(mFrequencies, 0, 2),
                Arrays.copyOfRange(mCaps, 0, 2),
                Arrays.copyOfRange(mLevels, 0, 2),
                Arrays.copyOfRange(mIsEphemeral, 0, 2));
        for (int i = 0; i < WifiLastResortWatchdog.MAX_BSSID_AGE - 1; i++) {
            mLastResortWatchdog.updateAvailableNetworks(candidates);
            assertEquals(mLastResortWatchdog.getRecentAvailableNetworks().get(mBssids[0]).age, 0);
            assertEquals(mLastResortWatchdog.getRecentAvailableNetworks().get(mBssids[1]).age, 0);
            assertEquals(mLastResortWatchdog.getRecentAvailableNetworks().get(mBssids[2]).age,
                    i + 1);
            assertEquals(mLastResortWatchdog.getRecentAvailableNetworks().get(mBssids[3]).age,
                    i + 1);
        }
        assertEquals(mLastResortWatchdog.getRecentAvailableNetworks().size(), 4);

        // One more buffering should age and cull candidates 2 & 3
        mLastResortWatchdog.updateAvailableNetworks(candidates);
        assertEquals(mLastResortWatchdog.getRecentAvailableNetworks().size(), 2);
    };

    /**
     * Case #2: Culling of old networks
     * Part 1:
     * This test starts with 4 networks seen, it then buffers N=MAX_BSSID_AGE empty scans
     * Expected behaviour: All networks are culled from recentAvailableNetworks
     *
     * Part 2:
     * Buffer some more empty scans just to make sure nothing breaks
     */
    @Test
    public void testAvailableNetworkBuffering_emptyBufferWithEmptyScanResults() throws Exception {
        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral);
        mLastResortWatchdog.updateAvailableNetworks(candidates);
        assertEquals(mLastResortWatchdog.getRecentAvailableNetworks().size(), 4);

        // Repeatedly buffer with no candidates
        candidates = createFilteredQnsCandidates(Arrays.copyOfRange(mSsids, 0, 0),
                Arrays.copyOfRange(mBssids, 0, 0),
                Arrays.copyOfRange(mFrequencies, 0, 0),
                Arrays.copyOfRange(mCaps, 0, 0),
                Arrays.copyOfRange(mLevels, 0, 0),
                Arrays.copyOfRange(mIsEphemeral, 0, 0));
        for (int i = 0; i < WifiLastResortWatchdog.MAX_BSSID_AGE; i++) {
            mLastResortWatchdog.updateAvailableNetworks(candidates);
        }
        assertEquals(mLastResortWatchdog.getRecentAvailableNetworks().size(), 0);
        for (int i = 0; i < WifiLastResortWatchdog.MAX_BSSID_AGE; i++) {
            mLastResortWatchdog.updateAvailableNetworks(candidates);
        }
        assertEquals(mLastResortWatchdog.getRecentAvailableNetworks().size(), 0);
    };

    /**
     * Case 3: Adding more networks over time
     * In this test, each successive (4 total) scan result buffers one more network.
     * Expected behavior: recentAvailableNetworks grows with number of scan results
     */
    @Test
    public void testAvailableNetworkBuffering_addNewNetworksOverTime() throws Exception {
        List<Pair<ScanDetail, WifiConfiguration>> candidates;
        // Buffer (i) scan results with each successive scan result
        for (int i = 1; i <= mSsids.length; i++) {
            candidates = createFilteredQnsCandidates(Arrays.copyOfRange(mSsids, 0, i),
                    Arrays.copyOfRange(mBssids, 0, i),
                    Arrays.copyOfRange(mFrequencies, 0, i),
                    Arrays.copyOfRange(mCaps, 0, i),
                    Arrays.copyOfRange(mLevels, 0, i),
                    Arrays.copyOfRange(mIsEphemeral, 0, i));
            mLastResortWatchdog.updateAvailableNetworks(candidates);
            assertEquals(mLastResortWatchdog.getRecentAvailableNetworks().size(), i);
            for (int j = 0; j < i; j++) {
                assertEquals(
                        mLastResortWatchdog.getRecentAvailableNetworks().get(mBssids[j]).age, 0);
            }
        }
    };

    /**
     *  Case 4: Test buffering with ephemeral networks & toString()
     *  This test is the same as Case 1, but it also includes ephemeral networks. toString is also
     *  smoke tested at various places in this test
     *  Expected behaviour: 4 networks added initially (2 ephemeral). After MAX_BSSID_AGE more
     *  bufferings, 2 are culled (leaving 1 ephemeral, one normal). toString method should execute
     *  without breaking anything.
     */
    @Test
    public void testAvailableNetworkBuffering_multipleNetworksSomeEphemeral() throws Exception {
        boolean[] isEphemeral = {true, false, true, false};

        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, isEphemeral);
        mLastResortWatchdog.updateAvailableNetworks(candidates);
        assertEquals(mLastResortWatchdog.getRecentAvailableNetworks().size(), 4);

        // Repeatedly buffer candidates 1 & 2, MAX_BSSID_AGE - 1 times
        candidates = createFilteredQnsCandidates(Arrays.copyOfRange(mSsids, 0, 2),
                Arrays.copyOfRange(mBssids, 0, 2),
                Arrays.copyOfRange(mFrequencies, 0, 2),
                Arrays.copyOfRange(mCaps, 0, 2),
                Arrays.copyOfRange(mLevels, 0, 2),
                Arrays.copyOfRange(isEphemeral, 0, 2));
        for (int i = 0; i < WifiLastResortWatchdog.MAX_BSSID_AGE - 1; i++) {
            mLastResortWatchdog.updateAvailableNetworks(candidates);
            mLastResortWatchdog.toString();
            assertEquals(mLastResortWatchdog.getRecentAvailableNetworks().get(mBssids[0]).age, 0);
            assertEquals(mLastResortWatchdog.getRecentAvailableNetworks().get(mBssids[1]).age, 0);
            assertEquals(mLastResortWatchdog.getRecentAvailableNetworks().get(mBssids[2]).age,
                    i + 1);
            assertEquals(mLastResortWatchdog.getRecentAvailableNetworks().get(mBssids[3]).age,
                    i + 1);
        }
        assertEquals(mLastResortWatchdog.getRecentAvailableNetworks().size(), 4);

        // One more buffering should age and cull candidates 2 & 3
        mLastResortWatchdog.updateAvailableNetworks(candidates);
        assertEquals(mLastResortWatchdog.getRecentAvailableNetworks().size(), 2);
        mLastResortWatchdog.toString();
    };

    /**
     * Case 5: Test failure counting, incrementing a specific BSSID
     * Test has 4 networks buffered, increment each different failure type on one of them
     * Expected behaviour: See failure counts for the specific failures rise to the appropriate
     * level for the specific network
     */
    @Test
    public void testFailureCounting_countFailuresForSingleBssid() throws Exception {
        int associationRejections = 5;
        int authenticationFailures = 9;
        int dhcpFailures = 11;
        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, mHasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < mSsids.length; i++) {
            assertFailureCountEquals(mBssids[i], 0, 0, 0);
        }

        //Increment failure count for each network and failure type
        int net = 0;
        for (int i = 0; i < associationRejections; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            assertEquals(i + 1, mLastResortWatchdog.getRecentAvailableNetworks()
                    .get(mBssids[net]).associationRejection);
        }
        net = 1;
        for (int i = 0; i < authenticationFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
            assertEquals(i + 1, mLastResortWatchdog.getRecentAvailableNetworks()
                    .get(mBssids[net]).authenticationFailure);
        }
        net = 2;
        for (int i = 0; i < dhcpFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_DHCP);
            assertEquals(i + 1, mLastResortWatchdog.getRecentAvailableNetworks()
                    .get(mBssids[net]).dhcpFailure);
        }
        assertFailureCountEquals(mBssids[0], associationRejections, 0, 0);
        assertFailureCountEquals(mBssids[1], 0, authenticationFailures, 0);
        assertFailureCountEquals(mBssids[2], 0, 0, dhcpFailures);
        assertFailureCountEquals(mBssids[3], 0, 0, 0);
    }

    /**
     * Case 6: Test failure counting, incrementing a specific BSSID, with some ephemeral networks
     * Almost identical to test case 5.
     * Test has 4 networks buffered (two are ephemeral), increment each different failure type on
     * one of them.
     * Expected behavior: See failure counts for the specific failures rise to the appropriate
     * level for the specific network
     */
    @Test
    public void testFailureCounting_countFailuresForSingleBssidWithEphemeral() throws Exception {
        int associationRejections = 5;
        int authenticationFailures = 9;
        int dhcpFailures = 11;
        boolean[] mIsEphemeral = {false, true, false, true};
        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, mHasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < mSsids.length; i++) {
            assertFailureCountEquals(mBssids[i], 0, 0, 0);
        }

        //Increment failure count for each network and failure type
        int net = 0;
        for (int i = 0; i < associationRejections; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            assertEquals(mLastResortWatchdog.getRecentAvailableNetworks()
                    .get(mBssids[net]).associationRejection, i + 1);
        }
        net = 1;
        for (int i = 0; i < authenticationFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
            assertEquals(mLastResortWatchdog.getRecentAvailableNetworks()
                    .get(mBssids[net]).authenticationFailure, i + 1);
        }
        net = 2;
        for (int i = 0; i < dhcpFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_DHCP);
            assertEquals(mLastResortWatchdog.getRecentAvailableNetworks()
                    .get(mBssids[net]).dhcpFailure, i + 1);
        }
        assertFailureCountEquals(mBssids[0], associationRejections, 0, 0);
        assertFailureCountEquals(mBssids[1], 0, authenticationFailures, 0);
        assertFailureCountEquals(mBssids[2], 0, 0, dhcpFailures);
        assertFailureCountEquals(mBssids[3], 0, 0, 0);
    }

    /**
     * Case 7: Test failure counting, incrementing a specific BSSID but with the wrong SSID given
     * Test has 4 networks buffered, increment each different failure type on one of them but using
     * the wrong ssid.
     * Expected behavior: Failure counts will remain at zero for all networks
     */
    @Test
    public void testFailureCounting_countFailuresForSingleBssidWrongSsid() throws Exception {
        String badSsid = "ItHertzWhenIP";
        int associationRejections = 5;
        int authenticationFailures = 9;
        int dhcpFailures = 11;
        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, mHasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < mSsids.length; i++) {
            assertFailureCountEquals(mBssids[i], 0, 0, 0);
        }

        //Increment failure count for each network and failure type
        int net = 0;
        for (int i = 0; i < associationRejections; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(badSsid, mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        }
        net = 1;
        for (int i = 0; i < authenticationFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(badSsid, mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
        }
        net = 2;
        for (int i = 0; i < dhcpFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(badSsid, mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_DHCP);
        }

        // Ensure all networks still have zero failure count
        for (int i = 0; i < mSsids.length; i++) {
            assertFailureCountEquals(mBssids[i], 0, 0, 0);
        }
    }

    /**
     * Case 8: Test failure counting, increment a bssid that does not exist
     * Test has 4 networks buffered, increment each failure type, but using the wrong bssid
     * Expected behavior: Failure counts will remain at zero for all networks
     */
    @Test
    public void testFailureCounting_countFailuresForNonexistentBssid() throws Exception {
        String badBssid = "de:ad:be:ee:e3:ef";
        int associationRejections = 5;
        int authenticationFailures = 9;
        int dhcpFailures = 11;
        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, mHasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < mSsids.length; i++) {
            assertFailureCountEquals(mBssids[i], 0, 0, 0);
        }

        //Increment failure count for each network and failure type
        int net = 0;
        for (int i = 0; i < associationRejections; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], badBssid,
                    WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        }
        net = 1;
        for (int i = 0; i < authenticationFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], badBssid,
                    WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
        }
        net = 2;
        for (int i = 0; i < dhcpFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], badBssid,
                    WifiLastResortWatchdog.FAILURE_CODE_DHCP);
        }

        // Ensure all networks still have zero failure count
        for (int i = 0; i < mSsids.length; i++) {
            assertFailureCountEquals(mBssids[i], 0, 0, 0);
        }
    }

    /**
     * Case 9: Test Failure Counting, using the "Any" BSSID
     * Test has 4 buffered networks, two of which share the same SSID (different mBssids)
     * Each failure type is incremented for the shared SSID, but with BSSID "any"
     * Expected Behavior: Both networks increment their counts in tandem
     */
    @Test
    public void testFailureCounting_countFailuresForAnyBssid() throws Exception {
        String[] ssids = {"\"test1\"", "\"test2\"", "\"test1\"", "\"test4\""};
        int associationRejections = 5;
        int authenticationFailures = 9;
        int dhcpFailures = 11;
        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(ssids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, mHasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < ssids.length; i++) {
            assertFailureCountEquals(mBssids[i], 0, 0, 0);
        }

        //Increment failure count for each network and failure type
        int net = 0;
        for (int i = 0; i < associationRejections; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[0], WifiLastResortWatchdog.BSSID_ANY,
                    WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        }
        net = 1;
        for (int i = 0; i < authenticationFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[0], WifiLastResortWatchdog.BSSID_ANY,
                    WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
        }
        net = 2;
        for (int i = 0; i < dhcpFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[0], WifiLastResortWatchdog.BSSID_ANY,
                    WifiLastResortWatchdog.FAILURE_CODE_DHCP);
        }
        assertFailureCountEquals(mBssids[0], associationRejections, authenticationFailures,
                dhcpFailures);
        assertFailureCountEquals(mBssids[1], 0, 0, 0);
        assertFailureCountEquals(mBssids[2], associationRejections, authenticationFailures,
                dhcpFailures);
        assertFailureCountEquals(mBssids[3], 0, 0, 0);
    }

    /**
     * Case 10: Test Failure Counting, using the "Any" BSSID for nonexistent SSID
     * Test has 4 buffered networks, two of which share the same SSID (different mBssids)
     * Each failure type is incremented for a bad SSID (doesn't exist), but with BSSID "any"
     * Expected Behavior: No Failures counted
     */
    @Test
    public void testFailureCounting_countFailuresForAnyBssidNonexistentSsid() throws Exception {
        int associationRejections = 5;
        int authenticationFailures = 9;
        int dhcpFailures = 11;
        String badSsid = "DropItLikeIt'sHotSpot";
        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, mHasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < mSsids.length; i++) {
            assertFailureCountEquals(mBssids[i], 0, 0, 0);
        }

        //Increment failure count for each network and failure type
        int net = 0;
        for (int i = 0; i < associationRejections; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    badSsid, WifiLastResortWatchdog.BSSID_ANY,
                    WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        }
        net = 1;
        for (int i = 0; i < authenticationFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    badSsid, WifiLastResortWatchdog.BSSID_ANY,
                    WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
        }
        net = 2;
        for (int i = 0; i < dhcpFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    badSsid, WifiLastResortWatchdog.BSSID_ANY,
                    WifiLastResortWatchdog.FAILURE_CODE_DHCP);
        }
        // Check that all network failure counts are still zero
        for (int i = 0; i < mSsids.length; i++) {
            assertFailureCountEquals(mBssids[i], 0, 0, 0);
        }
    }

    /**
     * Case 11: Test Failure Counting, over failure Threshold check
     * Test has 4 buffered networks, cause FAILURE_THRESHOLD failures for each failure type to one
     * of each network (leaving one unfailed).
     * Expected Behavior: 3 of the Available Networks report OverFailureThreshold
     */
    @Test
    public void testFailureCounting_failureOverThresholdCheck() throws Exception {
        int associationRejections = WifiLastResortWatchdog.FAILURE_THRESHOLD;
        int authenticationFailures = WifiLastResortWatchdog.FAILURE_THRESHOLD;
        int dhcpFailures = WifiLastResortWatchdog.FAILURE_THRESHOLD;
        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, mHasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < mSsids.length; i++) {
            assertFailureCountEquals(mBssids[i], 0, 0, 0);
        }

        //Increment failure count for each network and failure type
        int net = 0;
        for (int i = 0; i < associationRejections; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        }
        net = 1;
        for (int i = 0; i < authenticationFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
        }
        net = 2;
        for (int i = 0; i < dhcpFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_DHCP);
        }
        assertEquals(true, mLastResortWatchdog.isOverFailureThreshold(mBssids[0]));
        assertEquals(true, mLastResortWatchdog.isOverFailureThreshold(mBssids[1]));
        assertEquals(true, mLastResortWatchdog.isOverFailureThreshold(mBssids[2]));
        assertEquals(false, mLastResortWatchdog.isOverFailureThreshold(mBssids[3]));
    }

    /**
     * Case 12: Test Failure Counting, under failure Threshold check
     * Test has 4 buffered networks, cause FAILURE_THRESHOLD - 1 failures for each failure type to
     * one of each network (leaving one unfailed).
     * Expected Behavior: 0 of the Available Networks report OverFailureThreshold
     */
    @Test
    public void testFailureCounting_failureUnderThresholdCheck() throws Exception {
        int associationRejections = WifiLastResortWatchdog.FAILURE_THRESHOLD - 1;
        int authenticationFailures = WifiLastResortWatchdog.FAILURE_THRESHOLD - 1;
        int dhcpFailures = WifiLastResortWatchdog.FAILURE_THRESHOLD - 1;
        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, mHasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < mSsids.length; i++) {
            assertFailureCountEquals(mBssids[i], 0, 0, 0);
        }

        //Increment failure count for each network and failure type
        int net = 0;
        for (int i = 0; i < associationRejections; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        }
        net = 1;
        for (int i = 0; i < authenticationFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
        }
        net = 2;
        for (int i = 0; i < dhcpFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_DHCP);
        }
        assertEquals(false, mLastResortWatchdog.isOverFailureThreshold(mBssids[0]));
        assertEquals(false, mLastResortWatchdog.isOverFailureThreshold(mBssids[1]));
        assertEquals(false, mLastResortWatchdog.isOverFailureThreshold(mBssids[2]));
        assertEquals(false, mLastResortWatchdog.isOverFailureThreshold(mBssids[3]));
    }

    /**
     * Case 13: Test Failure Counting, available network buffering does not affect counts
     * In this test:
     *   4 networks are buffered
     *   Some number of failures are counted
     *   networks are buffered again
     * Expected Behavior: Failure counts are not modified by buffering
     */
    @Test
    public void testAvailableNetworkBuffering_doesNotAffectFailureCounts() throws Exception {
        int associationRejections = 5;
        int authenticationFailures = 9;
        int dhcpFailures = 11;
        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, mHasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < mSsids.length; i++) {
            assertFailureCountEquals(mBssids[i], 0, 0, 0);
        }

        //Increment failure count for each network and failure type
        int net = 0;
        for (int i = 0; i < associationRejections; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            assertEquals(i + 1, mLastResortWatchdog.getRecentAvailableNetworks()
                    .get(mBssids[net]).associationRejection);
        }
        net = 1;
        for (int i = 0; i < authenticationFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
            assertEquals(i + 1, mLastResortWatchdog.getRecentAvailableNetworks()
                    .get(mBssids[net]).authenticationFailure);
        }
        net = 2;
        for (int i = 0; i < dhcpFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_DHCP);
            assertEquals(i + 1, mLastResortWatchdog.getRecentAvailableNetworks()
                    .get(mBssids[net]).dhcpFailure);
        }
        // Check Each Network has appropriate failure count
        assertFailureCountEquals(mBssids[0], associationRejections, 0, 0);
        assertFailureCountEquals(mBssids[1], 0, authenticationFailures, 0);
        assertFailureCountEquals(mBssids[2], 0, 0, dhcpFailures);
        assertFailureCountEquals(mBssids[3], 0, 0, 0);

        // Re-buffer all networks
        for (int i = 0; i < WifiLastResortWatchdog.MAX_BSSID_AGE; i++) {
            mLastResortWatchdog.updateAvailableNetworks(candidates);
        }

        // Check Each Network still has appropriate failure count
        assertFailureCountEquals(mBssids[0], associationRejections, 0, 0);
        assertFailureCountEquals(mBssids[1], 0, authenticationFailures, 0);
        assertFailureCountEquals(mBssids[2], 0, 0, dhcpFailures);
        assertFailureCountEquals(mBssids[3], 0, 0, 0);
    }

    /**
     * Case 14: Test Failure Counting, culling of an old network will remove its failure counts
     * In this test:
     *   4 networks are buffered
     *   Some number of failures are counted for all networks
     *   3 of the networks are buffered until the 4th dies of old age
     *   The 4th network is re-buffered
     * Expected Behavior: Failure counts for the 4th network are cleared after re-buffering
     */
    @Test
    public void testAvailableNetworkBuffering_rebufferWipesCounts() throws Exception {
        int associationRejections = 5;
        int authenticationFailures = 9;
        int dhcpFailures = 11;
        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, mHasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < mSsids.length; i++) {
            assertFailureCountEquals(mBssids[i], 0, 0, 0);
        }

        //Increment failure count for each network and failure type
        int net = 0;
        for (int i = 0; i < associationRejections; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            assertEquals(i + 1, mLastResortWatchdog.getRecentAvailableNetworks()
                    .get(mBssids[net]).associationRejection);
        }
        net = 1;
        for (int i = 0; i < authenticationFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
            assertEquals(i + 1, mLastResortWatchdog.getRecentAvailableNetworks()
                    .get(mBssids[net]).authenticationFailure);
        }
        net = 2;
        for (int i = 0; i < dhcpFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_DHCP);
            assertEquals(i + 1, mLastResortWatchdog.getRecentAvailableNetworks()
                    .get(mBssids[net]).dhcpFailure);
        }
        // Check Each Network has appropriate failure count
        assertFailureCountEquals(mBssids[0], associationRejections, 0, 0);
        assertFailureCountEquals(mBssids[1], 0, authenticationFailures, 0);
        assertFailureCountEquals(mBssids[2], 0, 0, dhcpFailures);
        assertFailureCountEquals(mBssids[3], 0, 0, 0);

        // Re-buffer all networks except 'test1' until it dies of old age
        candidates = createFilteredQnsCandidates(Arrays.copyOfRange(mSsids, 1, 4),
                Arrays.copyOfRange(mBssids, 1, 4),
                Arrays.copyOfRange(mFrequencies, 1, 4),
                Arrays.copyOfRange(mCaps, 1, 4),
                Arrays.copyOfRange(mLevels, 1, 4),
                Arrays.copyOfRange(mIsEphemeral, 1, 4));
        for (int i = 0; i < WifiLastResortWatchdog.MAX_BSSID_AGE; i++) {
            mLastResortWatchdog.updateAvailableNetworks(candidates);
        }
        assertEquals(3, mLastResortWatchdog.getRecentAvailableNetworks().size());
        // Re-buffer All networks, with 'test1' again
        candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, mHasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Check Each Network has appropriate failure count (network 1 should be zero'd)
        assertFailureCountEquals(mBssids[0], 0, 0, 0);
        assertFailureCountEquals(mBssids[1], 0, authenticationFailures, 0);
        assertFailureCountEquals(mBssids[2], 0, 0, dhcpFailures);
        assertFailureCountEquals(mBssids[3], 0, 0, 0);
    }

    /**
     * Case 26: Test Failure Counting, null failure incrementation
     * In this test:
     *   4 networks are buffered
     *   Attempt to increment failures with null BSSID & SSID
     * Expected behavior: Nothing breaks, no counts incremented
     */
    @Test
    public void testFailureCounting_nullInputsNoBreaky() {
        int associationRejections = 5;
        int authenticationFailures = 9;
        int dhcpFailures = 11;
        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, mHasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < mSsids.length; i++) {
            assertFailureCountEquals(mBssids[i], 0, 0, 0);
        }

        //Increment failure count for each network and failure type
        int net = 0;
        for (int i = 0; i < associationRejections; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(null, mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        }
        net = 1;
        for (int i = 0; i < authenticationFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], null,
                    WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
        }
        net = 2;
        for (int i = 0; i < dhcpFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(null, null,
                    WifiLastResortWatchdog.FAILURE_CODE_DHCP);
        }

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < mSsids.length; i++) {
            assertFailureCountEquals(mBssids[i], 0, 0, 0);
        }
    }

    /**
     * Case 27: Test Failure Counting, test all failures are counted across SSID
     * In this test there are 8 networks,
     * the first 4 networks have unique SSIDs amongst themselves,
     * the last 4 networks share these SSIDs respectively, so there are 2 networks per SSID
     * In this test we increment failure counts for the 'test1' ssid for a specific BSSID, and for
     * the 'test2' ssid for BSSID_ANY.
     * Expected behaviour: Failure counts for both networks on the same SSID are mirrored via both
     * incrementation methods
     */
    @Test
    public void testFailureCounting_countFailuresAcrossSsids() throws Exception {
        String[] ssids = {"\"test1\"", "\"test2\"", "\"test3\"", "\"test4\"",
                "\"test1\"", "\"test2\"", "\"test3\"", "\"test4\""};
        String[] bssids = {"6c:f3:7f:ae:8c:f3", "6c:f3:7f:ae:8c:f4", "de:ad:ba:b1:e5:55",
                "c0:ff:ee:ee:e3:ee", "6c:f3:7f:ae:3c:f3", "6c:f3:7f:ae:3c:f4", "d3:ad:ba:b1:35:55",
                "c0:ff:ee:ee:33:ee"};
        int[] frequencies = {2437, 5180, 5180, 2437, 2437, 5180, 5180, 2437};
        String[] caps = {"[WPA2-EAP-CCMP][ESS]", "[WPA2-EAP-CCMP][ESS]",
                "[WPA2-EAP-CCMP][ESS]", "[WPA2-EAP-CCMP][ESS]", "[WPA2-EAP-CCMP][ESS]",
                "[WPA2-EAP-CCMP][ESS]", "[WPA2-EAP-CCMP][ESS]", "[WPA2-EAP-CCMP][ESS]"};
        int[] levels = {-60, -86, -50, -62, -60, -86, -50, -62};
        boolean[] isEphemeral = {false, false, false, false, false, false, false, false};
        boolean[] hasEverConnected = {false, false, false, false, false, false, false,
                false};
        int firstNetFails = 13;
        int secondNetFails = 8;
        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(ssids,
                bssids, frequencies, caps, levels, isEphemeral, hasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < ssids.length; i++) {
            assertFailureCountEquals(bssids[i], 0, 0, 0);
        }

        //Increment failure count for the first test network ssid & bssid
        for (int i = 0; i < firstNetFails; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[0], bssids[0], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[0], bssids[0], WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[0], bssids[0], WifiLastResortWatchdog.FAILURE_CODE_DHCP);
        }
        //Increment failure count for the first test network ssid & BSSID_ANY
        for (int i = 0; i < secondNetFails; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[1], WifiLastResortWatchdog.BSSID_ANY,
                    WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[1], WifiLastResortWatchdog.BSSID_ANY,
                    WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[1], WifiLastResortWatchdog.BSSID_ANY,
                    WifiLastResortWatchdog.FAILURE_CODE_DHCP);
        }
        assertFailureCountEquals(bssids[0], firstNetFails, firstNetFails, firstNetFails);
        assertFailureCountEquals(bssids[1], secondNetFails, secondNetFails, secondNetFails);
        assertFailureCountEquals(bssids[2], 0, 0, 0);
        assertFailureCountEquals(bssids[3], 0, 0, 0);
        assertFailureCountEquals(bssids[4], firstNetFails, firstNetFails, firstNetFails);
        assertFailureCountEquals(bssids[5], secondNetFails, secondNetFails, secondNetFails);
        assertFailureCountEquals(bssids[6], 0, 0, 0);
        assertFailureCountEquals(bssids[7], 0, 0, 0);
    }

    /**
     * Case 15: Test failure counting, ensure failures still counted while connected
     * Although failures should not occur while wifi is connected, race conditions are a thing, and
     * I'd like the count to be incremented even while connected (Later test verifies that this
     * can't cause a trigger though)
     * Expected behavior: Failure counts increment like normal
     */
    @Test
    public void testFailureCounting_wifiIsConnectedDoesNotAffectCounting() throws Exception {
        int associationRejections = 5;
        int authenticationFailures = 9;
        int dhcpFailures = 11;

        // Set Watchdogs internal wifi state tracking to 'connected'
        mLastResortWatchdog.connectedStateTransition(true);

        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, mHasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < mSsids.length; i++) {
            assertFailureCountEquals(mBssids[i], 0, 0, 0);
        }

        //Increment failure count for each network and failure type
        int net = 0;
        for (int i = 0; i < associationRejections; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            assertEquals(i + 1, mLastResortWatchdog.getRecentAvailableNetworks()
                    .get(mBssids[net]).associationRejection);
        }
        net = 1;
        for (int i = 0; i < authenticationFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
            assertEquals(i + 1, mLastResortWatchdog.getRecentAvailableNetworks()
                    .get(mBssids[net]).authenticationFailure);
        }
        net = 2;
        for (int i = 0; i < dhcpFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_DHCP);
            assertEquals(i + 1, mLastResortWatchdog.getRecentAvailableNetworks()
                    .get(mBssids[net]).dhcpFailure);
        }
        assertFailureCountEquals(mBssids[0], associationRejections, 0, 0);
        assertFailureCountEquals(mBssids[1], 0, authenticationFailures, 0);
        assertFailureCountEquals(mBssids[2], 0, 0, dhcpFailures);
        assertFailureCountEquals(mBssids[3], 0, 0, 0);
    }

    /**
     * Case 16: Test Failure Counting, entering ConnectedState clears all failure counts
     * 4 Networks are buffered, cause various failures to 3 of them. Transition to ConnectedState
     * Expected behavior: After transitioning, failure counts are reset to 0
     */
    @Test
    public void testFailureCounting_enteringWifiConnectedStateClearsCounts() throws Exception {
        int associationRejections = 5;
        int authenticationFailures = 9;
        int dhcpFailures = 11;

        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, mHasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        //Increment failure count for each network and failure type
        int net = 0;
        for (int i = 0; i < associationRejections; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        }
        net = 1;
        for (int i = 0; i < authenticationFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
        }
        net = 2;
        for (int i = 0; i < dhcpFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_DHCP);
        }

        // Check that we have Failures
        assertFailureCountEquals(mBssids[0], associationRejections, 0, 0);
        assertFailureCountEquals(mBssids[1], 0, authenticationFailures, 0);
        assertFailureCountEquals(mBssids[2], 0, 0, dhcpFailures);

        // Transition to 'ConnectedState'
        mLastResortWatchdog.connectedStateTransition(true);

        // Check that we have no failures
        for (int i = 0; i < mSsids.length; i++) {
            assertFailureCountEquals(mBssids[i], 0, 0, 0);
        }
    }

    /**
     * Case 17: Test Trigger Condition, only some networks over threshold
     * We have 4 buffered networks, increment failure counts on 3 of them, until all 3 are over
     * threshold.
     * Expected Behavior: Watchdog does not trigger
     */
    @Test
    public void testTriggerCondition_someNetworksOverFailureThreshold_allHaveEverConnected()
            throws Exception {
        int associationRejections = WifiLastResortWatchdog.FAILURE_THRESHOLD;
        int authenticationFailures = WifiLastResortWatchdog.FAILURE_THRESHOLD + 2;
        int dhcpFailures = WifiLastResortWatchdog.FAILURE_THRESHOLD + 3;
        boolean[] hasEverConnected = {true, true, true, true};

        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, hasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Increment failure count for 3 networks and failure types, asserting each time that it
        // does not trigger, with only 3 over threshold
        boolean watchdogTriggered = false;
        int net = 0;
        for (int i = 0; i < associationRejections; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            assertEquals(false, watchdogTriggered);
        }
        net = 1;
        for (int i = 0; i < authenticationFailures; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
            assertEquals(false, watchdogTriggered);
        }
        net = 2;
        for (int i = 0; i < dhcpFailures; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_DHCP);
            assertEquals(false, watchdogTriggered);
        }

        // Check that we have Failures
        assertFailureCountEquals(mBssids[0], associationRejections, 0, 0);
        assertFailureCountEquals(mBssids[1], 0, authenticationFailures, 0);
        assertFailureCountEquals(mBssids[2], 0, 0, dhcpFailures);

        // Add one more failure to one of the already over threshold networks, assert that it
        // does not trigger
        watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[0], mBssids[0], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        assertEquals(false, watchdogTriggered);
    }

    /**
     * Case 18: Test Trigger Condition, watchdog fires once, then deactivates
     * In this test we have 4 networks, which we have connected to in the past. Failures are
     * incremented until all networks but one are over failure threshold, and then a few more times.
     *
     * Expected behavior: The watchdog triggers once as soon as all failures are over threshold,
     * but stops triggering for subsequent failures
     */
    @Test
    public void testTriggerCondition_allNetworksOverFailureThreshold_allHaveEverConnected()
            throws Exception {
        int associationRejections = WifiLastResortWatchdog.FAILURE_THRESHOLD;
        int authenticationFailures = WifiLastResortWatchdog.FAILURE_THRESHOLD + 2;
        int dhcpFailures = WifiLastResortWatchdog.FAILURE_THRESHOLD + 3;
        boolean[] hasEverConnected = {true, true, true, true};

        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, hasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Bring 3 of the 4 networks over failure Threshold without triggering watchdog
        boolean watchdogTriggered = false;
        int net = 0;
        for (int i = 0; i < associationRejections; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            assertEquals(false, watchdogTriggered);
        }
        net = 1;
        for (int i = 0; i < authenticationFailures; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
            assertEquals(false, watchdogTriggered);
        }
        net = 2;
        for (int i = 0; i < dhcpFailures; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_DHCP);
            assertEquals(false, watchdogTriggered);
        }

        // Bring the remaining unfailed network upto 1 less than the failure threshold
        net = 3;
        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD - 1; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            assertEquals(false, watchdogTriggered);
        }
        // Increment failure count once more, check that watchdog triggered this time
        watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        mLastResortWatchdog.updateAvailableNetworks(candidates);
        assertEquals(true, watchdogTriggered);

        // Increment failure count 5 more times, watchdog should not trigger
        for (int i = 0; i < 5; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                        mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            assertEquals(false, watchdogTriggered);
        }
    }

    /**
     * Case 19: Test Trigger Condition, all networks over failure threshold, one has ever connected
     * In this test we have 4 networks, only one has connected in the past. Failures are
     * incremented until all networks but one are over failure threshold, and then a few more times.
     *
     * Expected behavior: The watchdog triggers once as soon as all failures are over threshold,
     * but stops triggering for subsequent failures
     */
    @Test
    public void testTriggerCondition_allNetworksOverFailureThreshold_oneHaveEverConnected()
            throws Exception {
        int associationRejections = WifiLastResortWatchdog.FAILURE_THRESHOLD;
        int authenticationFailures = WifiLastResortWatchdog.FAILURE_THRESHOLD + 2;
        int dhcpFailures = WifiLastResortWatchdog.FAILURE_THRESHOLD + 3;
        boolean[] hasEverConnected = {false, true, false, false};

        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, hasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Bring 3 of the 4 networks over failure Threshold without triggering watchdog
        boolean watchdogTriggered = false;
        int net = 0;
        for (int i = 0; i < associationRejections; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            assertEquals(false, watchdogTriggered);
        }
        net = 1;
        for (int i = 0; i < authenticationFailures; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
            assertEquals(false, watchdogTriggered);
        }
        net = 2;
        for (int i = 0; i < dhcpFailures; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_DHCP);
            assertEquals(false, watchdogTriggered);
        }

        // Bring the remaining unfailed network upto 1 less than the failure threshold
        net = 3;
        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD - 1; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            assertEquals(false, watchdogTriggered);
        }
        // Increment failure count once more, check that watchdog triggered this time
        watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        mLastResortWatchdog.updateAvailableNetworks(candidates);
        assertEquals(true, watchdogTriggered);

        // Increment failure count 5 more times, watchdog should not trigger
        for (int i = 0; i < 5; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                        mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            assertEquals(false, watchdogTriggered);
        }
    }

    /**
     * Case 20: Test Trigger Condition, all networks over failure threshold, 0 have ever connected
     * In this test we have 4 networks, none have ever connected. Failures are
     * incremented until all networks but one are over failure threshold, and then a few more times.
     *
     * Expected behavior: The watchdog does not trigger
     */
    @Test
    public void testTriggerCondition_allNetworksOverFailureThreshold_zeroHaveEverConnected()
            throws Exception {
        int associationRejections = WifiLastResortWatchdog.FAILURE_THRESHOLD + 1;
        int authenticationFailures = WifiLastResortWatchdog.FAILURE_THRESHOLD + 2;
        int dhcpFailures = WifiLastResortWatchdog.FAILURE_THRESHOLD + 3;

        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, mHasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Count failures on all 4 networks until all of them are over the failure threshold
        boolean watchdogTriggered = false;
        int net = 0;
        for (int i = 0; i < associationRejections; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            assertEquals(false, watchdogTriggered);
        }
        net = 1;
        for (int i = 0; i < authenticationFailures; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
            assertEquals(false, watchdogTriggered);
        }
        net = 2;
        for (int i = 0; i < dhcpFailures; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_DHCP);
            assertEquals(false, watchdogTriggered);
        }
        net = 3;
        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD + 1; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            assertEquals(false, watchdogTriggered);
        }
    }

    /**
     * Case 21: Test Trigger Condition, Conditions right to trigger, but wifi is connected
     * In this test we have 4 networks, all have connected in the past
     * incremented until all networks but one are over failure threshold, and then a few more times.
     *
     * Expected behavior: The watchdog does not trigger
     */
    @Test
    public void testTriggerCondition_allNetworksOverFailureThreshold_isConnected()
        throws Exception {
        int associationRejections = WifiLastResortWatchdog.FAILURE_THRESHOLD + 1;
        int authenticationFailures = WifiLastResortWatchdog.FAILURE_THRESHOLD + 2;
        int dhcpFailures = WifiLastResortWatchdog.FAILURE_THRESHOLD + 3;

        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, mHasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Set Watchdogs internal wifi state tracking to 'connected'
        mLastResortWatchdog.connectedStateTransition(true);

        // Count failures on all 4 networks until all of them are over the failure threshold
        boolean watchdogTriggered = false;
        int net = 0;
        for (int i = 0; i < associationRejections; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            assertEquals(false, watchdogTriggered);
        }
        net = 1;
        for (int i = 0; i < authenticationFailures; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
            assertEquals(false, watchdogTriggered);
        }
        net = 2;
        for (int i = 0; i < dhcpFailures; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_DHCP);
            assertEquals(false, watchdogTriggered);
        }
        net = 3;
        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD + 1; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[net], mBssids[net], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            assertEquals(false, watchdogTriggered);
        }
    }

    private void incrementFailuresUntilTrigger(String[] ssids, String[] bssids) {
        // Bring 3 of the 4 networks over failure Threshold without triggering watchdog
        boolean watchdogTriggered = false;
        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD; i++) {
            for (int j = 0; j < ssids.length - 1; j++) {
                watchdogTriggered = mLastResortWatchdog
                        .noteConnectionFailureAndTriggerIfNeeded(ssids[j], bssids[j],
                        WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
                assertEquals(false, watchdogTriggered);
            }
        }
        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD - 1; i++) {
            watchdogTriggered = mLastResortWatchdog
                    .noteConnectionFailureAndTriggerIfNeeded(ssids[ssids.length - 1],
                    bssids[ssids.length - 1], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            assertEquals(false, watchdogTriggered);
        }

        // Increment failure count once more, check that watchdog triggered this time
        watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[ssids.length - 1], bssids[ssids.length - 1],
                    WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        assertEquals(true, watchdogTriggered);
        verify(mSelfRecovery).trigger(eq(SelfRecovery.REASON_LAST_RESORT_WATCHDOG));
        reset(mSelfRecovery);
    }

    /**
     * Case 22: Test enabling/disabling of Watchdog Trigger, disabled after triggering
     * In this test, we have 4 networks. Increment failures until Watchdog triggers. Increment some
     * more failures.
     * Expected behavior: Watchdog trigger gets deactivated after triggering, and stops triggering
     */
    @Test
    public void testTriggerEnabling_disabledAfterTriggering() throws Exception {
        int associationRejections = WifiLastResortWatchdog.FAILURE_THRESHOLD;
        int authenticationFailures = WifiLastResortWatchdog.FAILURE_THRESHOLD + 2;
        int dhcpFailures = WifiLastResortWatchdog.FAILURE_THRESHOLD + 3;
        boolean[] hasEverConnected = {false, true, false, false};

        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, hasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        incrementFailuresUntilTrigger(mSsids, mBssids);

        // Increment failure count 5 more times, watchdog should not trigger
        for (int i = 0; i < 5; i++) {
            boolean watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                        mSsids[3], mBssids[3], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            assertEquals(false, watchdogTriggered);
        }
    }

    /**
     * Case 23: Test enabling/disabling of Watchdog Trigger, trigger re-enabled after connecting
     * In this test, we have 4 networks. Increment failures until Watchdog triggers and deactivates,
     * transition wifi to connected state, then increment failures until all networks over threshold
     * Expected behavior: Watchdog able to trigger again after transitioning to and from connected
     * state
     */
    @Test
    public void testTriggerEnabling_enabledAfterConnecting() throws Exception {
        int associationRejections = WifiLastResortWatchdog.FAILURE_THRESHOLD;
        int authenticationFailures = WifiLastResortWatchdog.FAILURE_THRESHOLD + 2;
        int dhcpFailures = WifiLastResortWatchdog.FAILURE_THRESHOLD + 3;
        boolean[] hasEverConnected = {false, true, false, false};
        boolean watchdogTriggered;
        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, hasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        incrementFailuresUntilTrigger(mSsids, mBssids);

        // Increment failure count 5 more times, ensure trigger is deactivated
        for (int i = 0; i < 5; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                        mSsids[3], mBssids[3], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            mLastResortWatchdog.updateAvailableNetworks(candidates);
            assertEquals(false, watchdogTriggered);
        }

        // transition Watchdog wifi state tracking to 'connected' then back to 'disconnected'
        mLastResortWatchdog.connectedStateTransition(true);
        mLastResortWatchdog.connectedStateTransition(false);

        // Fail 3/4 networks until they're over threshold
        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD + 1; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[0], mBssids[0], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            assertEquals(false, watchdogTriggered);
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[1], mBssids[1], WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
            assertEquals(false, watchdogTriggered);
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[2], mBssids[2], WifiLastResortWatchdog.FAILURE_CODE_DHCP);
            assertEquals(false, watchdogTriggered);
        }

        // Bring the remaining unfailed network upto 1 less than the failure threshold
        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD - 1; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[3], mBssids[3], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            assertEquals(false, watchdogTriggered);
        }
        // Increment failure count once more, check that watchdog triggered this time
        watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[3], mBssids[3], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        assertEquals(true, watchdogTriggered);
    }

    /**
     * Case 24: Test enabling/disabling of Watchdog Trigger, trigger re-enabled after
     * timeout mechanism with new network available.
     * In this test, we have 3 networks. Increment failures until Watchdog triggers and deactivates,
     * we then buffer a new network (network 4), then increment failures until all networks over
     * threshold Expected behavior: Watchdog able to trigger again after discovering a new network
     */
    @Test
    public void testTriggerEnabling_enabledAfterNewNetwork() {
        int associationRejections = WifiLastResortWatchdog.FAILURE_THRESHOLD;
        int authenticationFailures = WifiLastResortWatchdog.FAILURE_THRESHOLD + 2;
        int dhcpFailures = WifiLastResortWatchdog.FAILURE_THRESHOLD + 3;
        boolean[] hasEverConnected = {false, true, false, false};
        boolean watchdogTriggered;

        final long timeAtFailure = 100;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(timeAtFailure);

        // Buffer potential candidates 1,2,3
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(
                Arrays.copyOfRange(mSsids, 0, 3),
                Arrays.copyOfRange(mBssids, 0, 3),
                Arrays.copyOfRange(mFrequencies, 0, 3),
                Arrays.copyOfRange(mCaps, 0, 3),
                Arrays.copyOfRange(mLevels, 0, 3),
                Arrays.copyOfRange(mIsEphemeral, 0, 3),
                Arrays.copyOfRange(hasEverConnected, 0, 3));
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        incrementFailuresUntilTrigger(Arrays.copyOfRange(mSsids, 0, 3),
                Arrays.copyOfRange(mBssids, 0, 3));

        // Increment failure count 5 more times, ensure trigger is deactivated
        for (int i = 0; i < 5; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                        mSsids[2], mBssids[2], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
            mLastResortWatchdog.updateAvailableNetworks(candidates);
            assertEquals(false, watchdogTriggered);
        }

        when(mClock.getElapsedSinceBootMillis())
                .thenReturn(timeAtFailure + WifiLastResortWatchdog.LAST_TRIGGER_TIMEOUT_MILLIS);

        candidates = createFilteredQnsCandidates(mSsids, mBssids, mFrequencies, mCaps, mLevels,
                mIsEphemeral, hasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        incrementFailuresUntilTrigger(mSsids, mBssids);
    }

    /**
     * Case 28: Test Metrics collection
     * Setup 5 networks (unique SSIDs). Fail them until watchdog triggers, with 1 network failing
     * association, 1 failing authentication, 2 failing dhcp and one failing both authentication and
     * dhcp, (over threshold for all these failures)
     * Expected behavior: Metrics are updated as follows
     *  Triggers++
     *  # of Networks += 5
     *  Triggers with Bad association++
     *  Triggers with Bad authentication++
     *  Triggers with Bad dhcp++
     *  Number of networks with bad association += 1
     *  Number of networks with bad authentication += 2
     *  Number of networks with bad dhcp += 3
     */
    @Test
    public void testMetricsCollection() {
        String[] ssids = {"\"test1\"", "\"test2\"", "\"test3\"", "\"test4\"", "\"test5\""};
        String[] bssids = {"6c:f3:7f:ae:8c:f3", "6c:f3:7f:ae:8c:f4", "de:ad:ba:b1:e5:55",
                "c0:ff:ee:ee:e3:ee", "6c:f3:7f:ae:3c:f3"};
        int[] frequencies = {2437, 5180, 5180, 2437, 2437};
        String[] caps = {"[WPA2-EAP-CCMP][ESS]", "[WPA2-EAP-CCMP][ESS]",
                "[WPA2-EAP-CCMP][ESS]", "[WPA2-EAP-CCMP][ESS]", "[WPA2-EAP-CCMP][ESS]"};
        int[] levels = {-60, -86, -50, -62, -60};
        boolean[] isEphemeral = {false, false, false, false, false};
        boolean[] hasEverConnected = {true, false, false, false, false};
        // Buffer potential candidates 1,2,3,4 & 5
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(ssids,
                bssids, frequencies, caps, levels, isEphemeral, hasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < ssids.length; i++) {
            assertFailureCountEquals(bssids[i], 0, 0, 0);
        }

        long timeAtFailure = 100;
        long timeAtReconnect = 5000;
        long expectedDuration = timeAtReconnect - timeAtFailure;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(timeAtFailure, timeAtReconnect);

        //Increment failure counts
        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[1], bssids[1], WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[2], bssids[2], WifiLastResortWatchdog.FAILURE_CODE_DHCP);
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[3], bssids[3], WifiLastResortWatchdog.FAILURE_CODE_DHCP);
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[4], bssids[4], WifiLastResortWatchdog.FAILURE_CODE_DHCP);
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[4], bssids[4], WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[0], bssids[0], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        }

        // Verify relevant WifiMetrics calls were made once with appropriate arguments
        verify(mWifiMetrics, times(1)).incrementNumLastResortWatchdogTriggers();
        verify(mWifiMetrics, times(1)).addCountToNumLastResortWatchdogAvailableNetworksTotal(5);
        verify(mWifiMetrics, times(1))
                .addCountToNumLastResortWatchdogBadAuthenticationNetworksTotal(2);
        verify(mWifiMetrics, times(1))
                .incrementNumLastResortWatchdogTriggersWithBadAuthentication();
        verify(mWifiMetrics, times(1))
                .addCountToNumLastResortWatchdogBadAssociationNetworksTotal(1);
        verify(mWifiMetrics, times(1)).incrementNumLastResortWatchdogTriggersWithBadAssociation();
        verify(mWifiMetrics, times(1)).addCountToNumLastResortWatchdogBadDhcpNetworksTotal(3);
        verify(mWifiMetrics, times(1)).incrementNumLastResortWatchdogTriggersWithBadDhcp();

        // set connection to ssids[0]
        when(mWifiInfo.getSSID()).thenReturn(ssids[0]);

        // Simulate wifi connecting after triggering
        mLastResortWatchdog.connectedStateTransition(true);

        // Verify that WifiMetrics counted this as a Watchdog success
        verify(mWifiMetrics, times(1)).incrementNumLastResortWatchdogSuccesses();
        verify(mWifiMetrics, times(1)).setWatchdogSuccessTimeDurationMs(eq(expectedDuration));

        // Verify takeBugReport is called
        mLooper.dispatchAll();
        verify(mClientModeImpl, times(1)).takeBugReport(anyString(), anyString());

        // Simulate wifi disconnecting
        mLastResortWatchdog.connectedStateTransition(false);

        // Verify that WifiMetrics has still only counted one success
        verify(mWifiMetrics, times(1)).incrementNumLastResortWatchdogSuccesses();
        verify(mWifiMetrics, times(1)).setWatchdogSuccessTimeDurationMs(eq(expectedDuration));
        // Verify takeBugReport not called again
        mLooper.dispatchAll();
        verify(mClientModeImpl, times(1)).takeBugReport(anyString(), anyString());

        // Remove the fifth network from candidates
        candidates = createFilteredQnsCandidates(Arrays.copyOfRange(mSsids, 0, 4),
            Arrays.copyOfRange(mBssids, 0, 4),
            Arrays.copyOfRange(mFrequencies, 0, 4),
            Arrays.copyOfRange(mCaps, 0, 4),
            Arrays.copyOfRange(mLevels, 0, 4),
            Arrays.copyOfRange(mIsEphemeral, 0, 4));

        // Age out the fifth network
        for (int i = 0; i < WifiLastResortWatchdog.MAX_BSSID_AGE; i++) {
            mLastResortWatchdog.updateAvailableNetworks(candidates);
        }

        timeAtFailure = 10100;
        timeAtReconnect = 15000;
        expectedDuration = timeAtReconnect - timeAtFailure;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(timeAtFailure, timeAtReconnect);

        //Increment failure counts
        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[1], bssids[1], WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[2], bssids[2], WifiLastResortWatchdog.FAILURE_CODE_DHCP);
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[3], bssids[3], WifiLastResortWatchdog.FAILURE_CODE_DHCP);
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[0], bssids[0], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        }

        // increment the timer to enable Watchdog
        when(mClock.getElapsedSinceBootMillis())
                .thenReturn(timeAtFailure + WifiLastResortWatchdog.LAST_TRIGGER_TIMEOUT_MILLIS);

        // Add network #5 back into the candidates
        candidates = createFilteredQnsCandidates(ssids,
                bssids, frequencies, caps, levels, isEphemeral, hasEverConnected);

        // LastResortWatchdog should reactivate if there is a new network (#5)
        // available and time interval after last Watchdog trigger is over
        // LAST_TRIGGER_TIMEOUT_MILLIS.
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Simulate wifi connecting
        mLastResortWatchdog.connectedStateTransition(true);

        // Verify that WifiMetrics did not count another success, as the connection could be due
        // to the newly available network #5
        verify(mWifiMetrics, times(1)).incrementNumLastResortWatchdogSuccesses();
        verify(mWifiMetrics, times(1)).setWatchdogSuccessTimeDurationMs(eq(expectedDuration));
    }

    /**
     * Case 21: Test config updates where new config is null.
     * Create a scan result with an associated config and update the available networks list.
     * Repeat this with a second scan result where the config is null.
     * Expected behavior: The stored config should not be lost overwritten.
     */
    @Test
    public void testUpdateNetworkWithNullConfig() {
        List<Pair<ScanDetail, WifiConfiguration>> candidates =
                new ArrayList<Pair<ScanDetail, WifiConfiguration>>();
        String ssid = mSsids[0].replaceAll("^\"+", "").replaceAll("\"+$", "");
        ScanDetail scanDetail = new ScanDetail(WifiSsid.createFromAsciiEncoded(ssid),
                mBssids[0], mCaps[0], mLevels[0], mFrequencies[0], System.currentTimeMillis(), 0);
        WifiConfiguration config = mock(WifiConfiguration.class);
        WifiConfiguration.NetworkSelectionStatus networkSelectionStatus =
                mock(WifiConfiguration.NetworkSelectionStatus.class);
        when(config.getNetworkSelectionStatus()).thenReturn(networkSelectionStatus);
        when(networkSelectionStatus.getHasEverConnected())
                .thenReturn(true);
        candidates.add(Pair.create(scanDetail, config));
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        candidates.clear();

        candidates.add(Pair.create(scanDetail, null));
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        boolean watchdogTriggered = false;
        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[0], mBssids[0], WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
        }
        assertEquals(true, watchdogTriggered);
    }

    /**
     * Case 22: Test config updates where hasEverConnected goes from false to true.
     * Create a scan result with an associated config and update the available networks list.
     * Repeat this with a second scan result where the config value for hasEverConnected
     * is true.
     * Expected behavior: The stored config should not be lost overwritten.
     */
    @Test
    public void testUpdateNetworkWithHasEverConnectedTrue() {
        List<Pair<ScanDetail, WifiConfiguration>> candidates =
                new ArrayList<Pair<ScanDetail, WifiConfiguration>>();
        String ssid = mSsids[0].replaceAll("^\"+", "").replaceAll("\"+$", "");
        ScanDetail scanDetail = new ScanDetail(WifiSsid.createFromAsciiEncoded(ssid),
                mBssids[0], mCaps[0], mLevels[0], mFrequencies[0], System.currentTimeMillis(), 0);
        WifiConfiguration configHasEverConnectedFalse = mock(WifiConfiguration.class);
        WifiConfiguration.NetworkSelectionStatus networkSelectionStatusFalse =
                mock(WifiConfiguration.NetworkSelectionStatus.class);
        when(configHasEverConnectedFalse.getNetworkSelectionStatus())
                .thenReturn(networkSelectionStatusFalse);
        when(networkSelectionStatusFalse.getHasEverConnected())
                .thenReturn(false);
        candidates.add(Pair.create(scanDetail, configHasEverConnectedFalse));
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        boolean watchdogTriggered = false;
        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[0], mBssids[0], WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
        }
        assertEquals(false, watchdogTriggered);

        candidates.clear();

        WifiConfiguration configHasEverConnectedTrue = mock(WifiConfiguration.class);
        WifiConfiguration.NetworkSelectionStatus networkSelectionStatusTrue =
                mock(WifiConfiguration.NetworkSelectionStatus.class);
        when(configHasEverConnectedTrue.getNetworkSelectionStatus())
                .thenReturn(networkSelectionStatusTrue);
        when(networkSelectionStatusTrue.getHasEverConnected())
                .thenReturn(true);
        candidates.add(Pair.create(scanDetail, configHasEverConnectedTrue));
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                mSsids[0], mBssids[0], WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
        assertEquals(true, watchdogTriggered);
    }

    /**
     * Case 23: Test config updates where hasEverConnected goes from true to false.
     * Create a scan result with an associated config and update the available networks list.
     * Repeat this with a second scan result where hasEverConnected is false.
     * Expected behavior: The stored config should not be lost overwritten.
     */
    @Test
    public void testUpdateNetworkWithHasEverConnectedFalse() {
        List<Pair<ScanDetail, WifiConfiguration>> candidates =
                new ArrayList<Pair<ScanDetail, WifiConfiguration>>();
        String ssid = mSsids[0].replaceAll("^\"+", "").replaceAll("\"+$", "");
        ScanDetail scanDetail = new ScanDetail(WifiSsid.createFromAsciiEncoded(ssid),
                mBssids[0], mCaps[0], mLevels[0], mFrequencies[0], System.currentTimeMillis(), 0);

        WifiConfiguration configHasEverConnectedTrue = mock(WifiConfiguration.class);
        WifiConfiguration.NetworkSelectionStatus networkSelectionStatusTrue =
                mock(WifiConfiguration.NetworkSelectionStatus.class);
        when(configHasEverConnectedTrue.getNetworkSelectionStatus())
                .thenReturn(networkSelectionStatusTrue);
        when(networkSelectionStatusTrue.getHasEverConnected())
                .thenReturn(true);
        candidates.add(Pair.create(scanDetail, configHasEverConnectedTrue));
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        boolean watchdogTriggered = false;
        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[0], mBssids[0], WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
        }
        assertEquals(true, watchdogTriggered);

        candidates.clear();

        WifiConfiguration configHasEverConnectedFalse = mock(WifiConfiguration.class);
        WifiConfiguration.NetworkSelectionStatus networkSelectionStatusFalse =
                mock(WifiConfiguration.NetworkSelectionStatus.class);
        when(configHasEverConnectedFalse.getNetworkSelectionStatus())
                .thenReturn(networkSelectionStatusFalse);
        when(networkSelectionStatusFalse.getHasEverConnected())
                .thenReturn(false);
        candidates.add(Pair.create(scanDetail, configHasEverConnectedFalse));
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD; i++) {
            watchdogTriggered = mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    mSsids[0], mBssids[0], WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
        }
        assertEquals(false, watchdogTriggered);
    }

    /**
     * Case 24: Check toString method for accurate hasEverConnected value in
     * AvailableNetworkFailureCount objects.
     * Create an AvailableNetworkFailureCount instance and check output of toString method.
     * Expected behavior:  String contains HasEverConnected setting or null_config if there is not
     * an associated config.
     */
    @Test
    public void testHasEverConnectedValueInAvailableNetworkFailureCountToString() {
        // Check with HasEverConnected true
        WifiConfiguration configHasEverConnectedTrue = mock(WifiConfiguration.class);
        WifiConfiguration.NetworkSelectionStatus networkSelectionStatusTrue =
                mock(WifiConfiguration.NetworkSelectionStatus.class);
        when(configHasEverConnectedTrue.getNetworkSelectionStatus())
                .thenReturn(networkSelectionStatusTrue);
        when(networkSelectionStatusTrue.getHasEverConnected()).thenReturn(true);
        WifiLastResortWatchdog.AvailableNetworkFailureCount withConfigHECTrue =
                new WifiLastResortWatchdog.AvailableNetworkFailureCount(configHasEverConnectedTrue);
        String output = withConfigHECTrue.toString();
        assertTrue(output.contains("HasEverConnected: true"));

        // check with HasEverConnected false
        WifiConfiguration configHasEverConnectedFalse = mock(WifiConfiguration.class);
        WifiConfiguration.NetworkSelectionStatus networkSelectionStatusFalse =
                mock(WifiConfiguration.NetworkSelectionStatus.class);
        when(configHasEverConnectedFalse.getNetworkSelectionStatus())
                .thenReturn(networkSelectionStatusFalse);
        when(networkSelectionStatusFalse.getHasEverConnected()).thenReturn(false);
        WifiLastResortWatchdog.AvailableNetworkFailureCount withConfigHECFalse =
                new WifiLastResortWatchdog.AvailableNetworkFailureCount(
                        configHasEverConnectedFalse);
        output = withConfigHECFalse.toString();
        assertTrue(output.contains("HasEverConnected: false"));

        // Check with a null config
        WifiLastResortWatchdog.AvailableNetworkFailureCount withNullConfig =
                new WifiLastResortWatchdog.AvailableNetworkFailureCount(null);
        output = withNullConfig.toString();
        assertTrue(output.contains("HasEverConnected: null_config"));
    }

    /**
     * Test metrics incrementing connection failure count after watchdog has already been triggered
     */
    @Test
    public void testIncrementingWatchdogConnectionFailuresAfterTrigger() {
        String[] ssids = {"\"test1\""};
        String[] bssids = {"6c:f3:7f:ae:8c:f3"};
        int[] frequencies = {2437};
        String[] caps = {"[WPA2-EAP-CCMP][ESS]"};
        int[] levels = {-60};
        boolean[] isEphemeral = {false};
        boolean[] hasEverConnected = {true};
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(ssids,
                bssids, frequencies, caps, levels, isEphemeral, hasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < ssids.length; i++) {
            assertFailureCountEquals(bssids[i], 0, 0, 0);
        }

        //Increment failure counts
        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[0], bssids[0], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        }

        // Verify relevant WifiMetrics calls were made once with appropriate arguments
        verify(mWifiMetrics, times(1)).incrementNumLastResortWatchdogTriggers();

        // Verify that failure count after trigger is not incremented yet
        verify(mWifiMetrics, never()).incrementWatchdogTotalConnectionFailureCountAfterTrigger();

        // Fail 1 more time and verify this time it's counted
        mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                ssids[0], bssids[0], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        verify(mWifiMetrics, times(1)).incrementWatchdogTotalConnectionFailureCountAfterTrigger();
    }

    /**
     * Test that LRWD success is only declared when the first connection after restarting wifi
     * is successful.
     *
     * First tests the failure case: check success metric is not incremented when the first
     * connection is a failure.
     * Then test state transition and the success case: check success metric is incremented
     * when the first connection is a success.
     */
    @Test
    public void testWatchdogAssumesSuccessOnlyIfFirstConnectionAfterRestartSucceeds() {
        String[] ssids = {"\"test1\""};
        String[] bssids = {"6c:f3:7f:ae:8c:f3"};
        int[] frequencies = {2437};
        String[] caps = {"[WPA2-EAP-CCMP][ESS]"};
        int[] levels = {-60};
        boolean[] isEphemeral = {false};
        boolean[] hasEverConnected = {true};
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(ssids,
                bssids, frequencies, caps, levels, isEphemeral, hasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < ssids.length; i++) {
            assertFailureCountEquals(bssids[i], 0, 0, 0);
        }

        //Increment failure counts
        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[0], bssids[0], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        }

        // Verify watchdog has triggered a restart
        verify(mWifiMetrics, times(1)).incrementNumLastResortWatchdogTriggers();

        // Fail 1 more time and verify this time it's counted
        mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                ssids[0], bssids[0], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);

        // Simulate wifi connecting after triggering
        mLastResortWatchdog.connectedStateTransition(true);
        // Verify takeBugReport is not called again
        mLooper.dispatchAll();
        verify(mClientModeImpl, never()).takeBugReport(anyString(), anyString());
        verify(mWifiMetrics, never()).incrementNumLastResortWatchdogSuccesses();

        // Simulate wifi disconnecting
        mLastResortWatchdog.connectedStateTransition(false);

        // set connection to ssids[0]
        when(mWifiInfo.getSSID()).thenReturn(ssids[0]);

        // Test another round, and this time successfully connect after restart trigger
        for (int i = 0; i < ssids.length; i++) {
            assertFailureCountEquals(bssids[i], 0, 0, 0);
        }
        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[0], bssids[0], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        }

        // Verify watchdog has triggered a restart
        verify(mWifiMetrics, times(2)).incrementNumLastResortWatchdogTriggers();
        // Simulate wifi connecting after triggering
        mLastResortWatchdog.connectedStateTransition(true);
        // Verify takeBugReport is not called again
        mLooper.dispatchAll();
        verify(mClientModeImpl, times(1)).takeBugReport(anyString(), anyString());
        verify(mWifiMetrics, times(1)).incrementNumLastResortWatchdogSuccesses();
    }

    /**
     * If the user changes the configuration and then we have a successful connection, don't
     * trigger bugreport.
     * Tests this specific path:
     * 1. watchdog triggers restart
     * 2. wifi configuration changes
     * 3. wifi successfully connects immedietly after
     * Expected result: bugreport should not trigger
     */
    @Test
    public void testWatchdogVerifiesAtLeastOneNetworkIsConnectedBeforeTriggeringBugreport() {
        String[] ssids = {"\"test1\""};
        String[] bssids = {"6c:f3:7f:ae:8c:f3"};
        int[] frequencies = {2437};
        String[] caps = {"[WPA2-EAP-CCMP][ESS]"};
        int[] levels = {-60};
        boolean[] isEphemeral = {false};
        boolean[] hasEverConnected = {true};
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(ssids,
                bssids, frequencies, caps, levels, isEphemeral, hasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < ssids.length; i++) {
            assertFailureCountEquals(bssids[i], 0, 0, 0);
        }

        //Increment failure counts
        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[0], bssids[0], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        }

        // Verify watchdog has triggered a restart
        verify(mWifiMetrics, times(1)).incrementNumLastResortWatchdogTriggers();

        // Simulate user changing the configuration
        when(candidates.get(0).second.getNetworkSelectionStatus().getHasEverConnected())
                .thenReturn(false);

        mLastResortWatchdog.connectedStateTransition(true);
        // Verify takeBugReport is not called again
        mLooper.dispatchAll();
        verify(mClientModeImpl, never()).takeBugReport(anyString(), anyString());
        verify(mWifiMetrics, never()).incrementNumLastResortWatchdogSuccesses();
    }

    /**
     * Test Failure Counting, over failure Threshold check with mixture reason.
     * Test has 4 buffered networks, cause FAILURE_THRESHOLD failures for mixture failure type
     *  (leaving one unfailed).
     * Expected Behavior: 3 of the Available Networks report OverFailureThreshold
     */
    @Test
    public void testMixtureFailureCounting_failureOverThresholdCheck() throws Exception {
        int associationRejections = 3;
        int authenticationFailures = 4;
        int dhcpFailures = 5;
        // Buffer potential candidates 1,2,3 & 4
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(mSsids,
                mBssids, mFrequencies, mCaps, mLevels, mIsEphemeral, mHasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < mSsids.length; i++) {
            assertFailureCountEquals(mBssids[i], 0, 0, 0);
        }

        //Increment failure count for each network and failure type
        int net = 0;
        for (int i = 0; i < associationRejections; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        }
        for (int i = 0; i < authenticationFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
        }
        net = 1;
        for (int i = 0; i < associationRejections; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        }
        for (int i = 0; i < dhcpFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_DHCP);
        }
        net = 2;
        for (int i = 0; i < authenticationFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
        }
        for (int i = 0; i < dhcpFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(mSsids[net], mBssids[net],
                    WifiLastResortWatchdog.FAILURE_CODE_DHCP);
        }

        assertEquals(true, mLastResortWatchdog.isOverFailureThreshold(mBssids[0]));
        assertEquals(true, mLastResortWatchdog.isOverFailureThreshold(mBssids[1]));
        assertEquals(true, mLastResortWatchdog.isOverFailureThreshold(mBssids[2]));
        assertEquals(false, mLastResortWatchdog.isOverFailureThreshold(mBssids[3]));
    }

    /**
     * Test Metrics collection with Mixture Reason.
     * Setup 1 networks. Fail them until watchdog triggers
     * with 2 authenticationFailures and 5 dhcpFailures.
     * Expected behavior: Metrics are updated as follows
     *  Triggers++
     *  # of Networks += 1
     *  Triggers with Bad dhcp++
     *  Number of networks with bad dhcp += 1
     */
    @Test
    public void testMetricsCollectionWithMixtureReason() {
        String[] ssids = {"\"test1\""};
        String[] bssids = {"6c:f3:7f:ae:8c:f3"};
        int[] frequencies = {2437};
        String[] caps = {"[WPA2-EAP-CCMP][ESS]"};
        int[] levels = {-60};
        boolean[] isEphemeral = {false};
        boolean[] hasEverConnected = {true};
        int authenticationFailures = 2;
        int dhcpFailures = 5;

        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(ssids,
                bssids, frequencies, caps, levels, isEphemeral, hasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < ssids.length; i++) {
            assertFailureCountEquals(bssids[i], 0, 0, 0);
        }

        //Increment failure counts
        for (int i = 0; i < authenticationFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[0], bssids[0], WifiLastResortWatchdog.FAILURE_CODE_AUTHENTICATION);
        }
        for (int i = 0; i < dhcpFailures; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[0], bssids[0], WifiLastResortWatchdog.FAILURE_CODE_DHCP);
        }

        // Verify relevant WifiMetrics calls were made once with appropriate arguments
        verify(mWifiMetrics, times(1)).incrementNumLastResortWatchdogTriggers();
        verify(mWifiMetrics, times(1)).addCountToNumLastResortWatchdogAvailableNetworksTotal(1);
        verify(mWifiMetrics, never())
                .addCountToNumLastResortWatchdogBadAuthenticationNetworksTotal(anyInt());
        verify(mWifiMetrics, never())
                .incrementNumLastResortWatchdogTriggersWithBadAuthentication();
        verify(mWifiMetrics, never())
                .addCountToNumLastResortWatchdogBadAssociationNetworksTotal(anyInt());
        verify(mWifiMetrics, never()).incrementNumLastResortWatchdogTriggersWithBadAssociation();
        verify(mWifiMetrics, times(1)).addCountToNumLastResortWatchdogBadDhcpNetworksTotal(1);
        verify(mWifiMetrics, times(1)).incrementNumLastResortWatchdogTriggersWithBadDhcp();
    }

    /**
     * Test Watchdog with time based logic.
     *
     * After Watchdog has triggered once and still fail to connect,
     * Watchdog will not be allowed to work in 2 hours.
     * Expected result: Watchdog won't be trigger again
     */
    @Test
    public void testWatchdogWithTimeBasedLogic() {
        String[] ssids = {"\"test1\""};
        String[] bssids = {"6c:f3:7f:ae:8c:f3"};
        int[] frequencies = {2437};
        String[] caps = {"[WPA2-EAP-CCMP][ESS]"};
        int[] levels = {-60};
        boolean[] isEphemeral = {false};
        boolean[] hasEverConnected = {true};
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(ssids,
                bssids, frequencies, caps, levels, isEphemeral, hasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < mSsids.length; i++) {
            assertFailureCountEquals(mBssids[i], 0, 0, 0);
        }

        final long timeAtFailure = 100;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(timeAtFailure);

        incrementFailuresUntilTrigger(ssids, bssids);

        // Verify watchdog has triggered a restart
        verify(mWifiMetrics, times(1)).incrementNumLastResortWatchdogTriggers();

        // Age out network
        for (int i = 0; i < WifiLastResortWatchdog.MAX_BSSID_AGE; i++) {
            mLastResortWatchdog.updateAvailableNetworks(null);
        }
        assertEquals(mLastResortWatchdog.getRecentAvailableNetworks().size(), 0);

        when(mClock.getElapsedSinceBootMillis()).thenReturn(timeAtFailure + (300 * 1000));

        // network back into the candidates
        candidates = createFilteredQnsCandidates(ssids,
                bssids, frequencies, caps, levels, isEphemeral, hasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < mSsids.length; i++) {
            assertFailureCountEquals(mBssids[i], 0, 0, 0);
        }

        // Increment failure counts
        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[0], bssids[0], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        }

        // Watchdog should not be triggerred since time based logic.
        verify(mWifiMetrics, times(1)).incrementNumLastResortWatchdogTriggers();
    }

    /**
     * Test that LRWD success is only declared when connected back to same SSID.
     *
     * First tests the failure case: check success metric is not incremented when the first
     * connection on different SSID.
     * Then test state transition and the success case: check success metric is incremented
     * when the first connection on same SSID.
     */
    @Test
    public void testWatchdogAssumesSuccessOnlyIfConnectedOnSameSsid() {
        String[] ssids = {"\"test1\""};
        String[] bssids = {"6c:f3:7f:ae:8c:f3"};
        int[] frequencies = {2437};
        String[] caps = {"[WPA2-EAP-CCMP][ESS]"};
        int[] levels = {-60};
        boolean[] isEphemeral = {false};
        boolean[] hasEverConnected = {true};
        List<Pair<ScanDetail, WifiConfiguration>> candidates = createFilteredQnsCandidates(ssids,
                bssids, frequencies, caps, levels, isEphemeral, hasEverConnected);
        mLastResortWatchdog.updateAvailableNetworks(candidates);

        // Ensure new networks have zero'ed failure counts
        for (int i = 0; i < ssids.length; i++) {
            assertFailureCountEquals(bssids[i], 0, 0, 0);
        }

        //Increment failure counts
        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[0], bssids[0], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        }

        // Verify watchdog has triggered a restart
        verify(mWifiMetrics, times(1)).incrementNumLastResortWatchdogTriggers();

        // Simulate wifi connecting after triggering
        mLastResortWatchdog.connectedStateTransition(true);
        // Verify takeBugReport is not called because connected on different SSID
        mLooper.dispatchAll();
        verify(mClientModeImpl, never()).takeBugReport(anyString(), anyString());
        verify(mWifiMetrics, never()).incrementNumLastResortWatchdogSuccesses();

        // Simulate wifi disconnecting
        mLastResortWatchdog.connectedStateTransition(false);

        // set connection to ssids[0]
        when(mWifiInfo.getSSID()).thenReturn(ssids[0]);

        // Test another round, and this time successfully connect after restart trigger
        for (int i = 0; i < ssids.length; i++) {
            assertFailureCountEquals(bssids[i], 0, 0, 0);
        }
        for (int i = 0; i < WifiLastResortWatchdog.FAILURE_THRESHOLD; i++) {
            mLastResortWatchdog.noteConnectionFailureAndTriggerIfNeeded(
                    ssids[0], bssids[0], WifiLastResortWatchdog.FAILURE_CODE_ASSOCIATION);
        }

        // Verify watchdog has triggered a restart
        verify(mWifiMetrics, times(2)).incrementNumLastResortWatchdogTriggers();
        // Simulate wifi connecting after triggering
        mLastResortWatchdog.connectedStateTransition(true);
        // Verify takeBugReport is called because connected back on same SSID
        mLooper.dispatchAll();
        verify(mClientModeImpl, times(1)).takeBugReport(anyString(), anyString());
        verify(mWifiMetrics, times(1)).incrementNumLastResortWatchdogSuccesses();
    }

    /**
     * Verifies that when a connection takes too long (time difference between
     * StaEvent.TYPE_CMD_START_CONNECT and StaEvent.TYPE_NETWORK_CONNECTION_EVENT) a bugreport is
     * taken.
     */
    @Test
    public void testAbnormalConnectionTimeTriggersBugreport() throws Exception {
        // first verifies that bugreports are not taken when connection takes less than
        // DEFAULT_ABNORMAL_CONNECTION_DURATION_MS
        when(mClock.getElapsedSinceBootMillis()).thenReturn(1L);
        mLastResortWatchdog.noteStartConnectTime();
        mLooper.dispatchAll();
        when(mClock.getElapsedSinceBootMillis()).thenReturn(
                (long) DEFAULT_ABNORMAL_CONNECTION_DURATION_MS);
        Handler handler = mLastResortWatchdog.getHandler();
        handler.sendMessage(
                handler.obtainMessage(WifiMonitor.NETWORK_CONNECTION_EVENT, 0, 0, null));
        mLooper.dispatchAll();
        verify(mClientModeImpl, never()).takeBugReport(anyString(), anyString());

        // Now verify that bugreport is taken
        mLastResortWatchdog.noteStartConnectTime();
        mLooper.dispatchAll();
        when(mClock.getElapsedSinceBootMillis()).thenReturn(
                2L * DEFAULT_ABNORMAL_CONNECTION_DURATION_MS + 1);
        handler.sendMessage(
                handler.obtainMessage(WifiMonitor.NETWORK_CONNECTION_EVENT, 0, 0, null));
        mLooper.dispatchAll();
        verify(mClientModeImpl).takeBugReport(anyString(), anyString());

        // Verify additional connections (without more TYPE_CMD_START_CONNECT) don't trigger more
        // bugreports.
        when(mClock.getElapsedSinceBootMillis()).thenReturn(
                4L * DEFAULT_ABNORMAL_CONNECTION_DURATION_MS);
        handler.sendMessage(
                handler.obtainMessage(WifiMonitor.NETWORK_CONNECTION_EVENT, 0, 0, null));
        mLooper.dispatchAll();
        verify(mClientModeImpl).takeBugReport(anyString(), anyString());
    }

    /**
     * Changes |mAbnormalConnectionDurationMs| to a new value, and then verify that a bugreport is
     * taken for a connection that takes longer than the new threshold.
     * @throws Exception
     */
    @Test
    public void testGServicesSetDuration() throws Exception {
        final int testDurationMs = 10 * 1000; // 10 seconds
        // changes the abnormal connection duration to |testDurationMs|.
        when(mDeviceConfigFacade.getAbnormalConnectionDurationMs()).thenReturn(testDurationMs);
        mOnPropertiesChangedListenerCaptor.getValue().onPropertiesChanged(null);

        // verifies that bugreport is taken for connections that take longer than |testDurationMs|.
        when(mClock.getElapsedSinceBootMillis()).thenReturn(1L);
        mLastResortWatchdog.noteStartConnectTime();
        mLooper.dispatchAll();
        when(mClock.getElapsedSinceBootMillis()).thenReturn((long) testDurationMs + 2);
        Handler handler = mLastResortWatchdog.getHandler();
        handler.sendMessage(
                handler.obtainMessage(WifiMonitor.NETWORK_CONNECTION_EVENT, 0, 0, null));
        mLooper.dispatchAll();
        verify(mClientModeImpl).takeBugReport(anyString(), anyString());
    }

    /**
     * Verifies that bugreports are not triggered even when conditions are met after the
     * |mAbnormalConnectionBugreportEnabled| flag is changed to false.
     * @throws Exception
     */
    @Test
    public void testGServicesFlagDisable() throws Exception {
        // changes |mAbnormalConnectionBugreportEnabled| to false.
        when(mDeviceConfigFacade.isAbnormalConnectionBugreportEnabled()).thenReturn(false);
        mOnPropertiesChangedListenerCaptor.getValue().onPropertiesChanged(null);

        // verifies that bugreports are not taken.
        when(mClock.getElapsedSinceBootMillis()).thenReturn(1L);
        mLastResortWatchdog.noteStartConnectTime();
        mLooper.dispatchAll();
        when(mClock.getElapsedSinceBootMillis()).thenReturn(
                (long) DEFAULT_ABNORMAL_CONNECTION_DURATION_MS + 2);
        Handler handler = mLastResortWatchdog.getHandler();
        handler.sendMessage(
                handler.obtainMessage(WifiMonitor.NETWORK_CONNECTION_EVENT, 0, 0, null));
        mLooper.dispatchAll();
        verify(mClientModeImpl, never()).takeBugReport(anyString(), anyString());
    }
}
