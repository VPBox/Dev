/*
 * Copyright 2018 The Android Open Source Project
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

import static com.android.server.wifi.util.NativeUtil.removeEnclosingQuotes;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

import android.net.MacAddress;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

/**
 * Unit tests for {@link com.android.server.wifi.WifiCandidates}.
 */
@SmallTest
public class WifiCandidatesTest {

    @Mock ScanDetail mScanDetail1;
    @Mock ScanDetail mScanDetail2;
    @Mock WifiScoreCard mWifiScoreCard;
    @Mock WifiScoreCard.PerBssid mPerBssid;

    ScanResult mScanResult1;
    ScanResult mScanResult2;

    WifiConfiguration mConfig1;
    WifiConfiguration mConfig2;

    WifiCandidates mWifiCandidates;

    /**
     * Sets up for unit test
     */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mWifiCandidates = new WifiCandidates(mWifiScoreCard);
        mConfig1 = WifiConfigurationTestUtil.createOpenNetwork();
        mScanResult1 = new ScanResult() {{
                SSID = removeEnclosingQuotes(mConfig1.SSID);
                capabilities = "[ESS]";
                BSSID = "00:00:00:00:00:01";
            }};
        mConfig2 = WifiConfigurationTestUtil.createEphemeralNetwork();
        mScanResult2 = new ScanResult() {{
                SSID = removeEnclosingQuotes(mConfig2.SSID);
                capabilities = "[ESS]";
            }};
        doReturn(mScanResult1).when(mScanDetail1).getScanResult();
        doReturn(mScanResult2).when(mScanDetail2).getScanResult();
        doReturn(mPerBssid).when(mWifiScoreCard).lookupBssid(any(), any());
    }

    /**
     * Test for absence of null pointer exceptions
     */
    @Test
    public void testDontDieFromNulls() throws Exception {
        mWifiCandidates.add(null, mConfig1, 1, 42);
        mWifiCandidates.add(mScanDetail1, null, 2, 16);
        doReturn(null).when(mScanDetail2).getScanResult();
        mWifiCandidates.add(mScanDetail2, mConfig2, 3, 314, 1.0);
        assertFalse(mWifiCandidates.remove(null));

        assertEquals(0, mWifiCandidates.size());
    }

    /**
     * Add just one thing
     */
    @Test
    public void testAddJustOne() throws Exception {
        assertTrue(mWifiCandidates.add(mScanDetail1, mConfig1, 2, 14, 0.0));

        assertEquals(1, mWifiCandidates.size());
        assertEquals(0, mWifiCandidates.getFaultCount());
        assertNull(mWifiCandidates.getLastFault());
        verify(mPerBssid).setNetworkConfigId(eq(mConfig1.networkId));
    }

    /**
     * Make sure we catch SSID mismatch due to quoting error
     */
    @Test
    public void testQuotingBotch() throws Exception {
        // Unfortunately ScanResult.SSID is not quoted; make sure we catch that
        mScanResult1.SSID = mConfig1.SSID;
        mWifiCandidates.add(mScanDetail1, mConfig1, 2, 14, 0.0);

        // Should not have added this one
        assertEquals(0, mWifiCandidates.size());
        // The failure should have been recorded
        assertEquals(1, mWifiCandidates.getFaultCount());
        // The record of the failure should contain the culprit
        String blah = mWifiCandidates.getLastFault().toString();
        assertTrue(blah, blah.contains(mConfig1.SSID));

        // Now check that we can clear the faults
        mWifiCandidates.clearFaults();

        assertEquals(0, mWifiCandidates.getFaultCount());
        assertNull(mWifiCandidates.getLastFault());
    }

    /**
     * Test Key equals and hashCode methods
     */
    @Test
    public void testKeyEquivalence() throws Exception {
        ScanResultMatchInfo matchInfo1 = ScanResultMatchInfo.fromWifiConfiguration(mConfig1);
        ScanResultMatchInfo matchInfo1Prime = ScanResultMatchInfo.fromWifiConfiguration(mConfig1);
        ScanResultMatchInfo matchInfo2 = ScanResultMatchInfo.fromWifiConfiguration(mConfig2);
        assertFalse(matchInfo1 == matchInfo1Prime); // Checking assumption
        MacAddress mac1 = MacAddress.createRandomUnicastAddress();
        MacAddress mac2 = MacAddress.createRandomUnicastAddress();
        assertNotEquals(mac1, mac2); // really tiny probablility of failing here

        WifiCandidates.Key key1 = new WifiCandidates.Key(matchInfo1, mac1, 1);

        assertFalse(key1.equals(null));
        assertFalse(key1.equals((Integer) 0));
        // Same inputs should give equal results
        assertEquals(key1, new WifiCandidates.Key(matchInfo1, mac1, 1));
        // Equal inputs should give equal results
        assertEquals(key1, new WifiCandidates.Key(matchInfo1Prime, mac1, 1));
        // Hash codes of equal things should be equal
        assertEquals(key1.hashCode(), key1.hashCode());
        assertEquals(key1.hashCode(), new WifiCandidates.Key(matchInfo1, mac1, 1).hashCode());
        assertEquals(key1.hashCode(), new WifiCandidates.Key(matchInfo1Prime, mac1, 1).hashCode());

        // Unequal inputs should give unequal results
        assertFalse(key1.equals(new WifiCandidates.Key(matchInfo2, mac1, 1)));
        assertFalse(key1.equals(new WifiCandidates.Key(matchInfo1, mac2, 1)));
        assertFalse(key1.equals(new WifiCandidates.Key(matchInfo1, mac1, 2)));
    }

    /**
     * Test that picky mode works
     */
    @Test
    public void testPickyMode() throws Exception {
        // Set picky mode, make sure that it returns the object itself (so that
        // method chaining may be used).
        assertTrue(mWifiCandidates == mWifiCandidates.setPicky(true));
        try {
            mScanResult1.SSID = mConfig1.SSID; // As in testQuotingBotch()
            mWifiCandidates.add(mScanDetail1, mConfig1, 2, 14, 0.0);
            fail("Exception not raised in picky mode");
        } catch (IllegalArgumentException e) {
            assertEquals(1, mWifiCandidates.getFaultCount());
            assertEquals(e, mWifiCandidates.getLastFault());
        }
    }

    /**
     * Try cases where we don't overwrite existing candidates
     */
    @Test
    public void testNoOverwriteCases() throws Exception {
        // Setup is to add the first candidate
        mWifiCandidates.add(mScanDetail1, mConfig1, 2, 14, 0.0);
        assertEquals(1, mWifiCandidates.size());

        // Same evaluator, same score. Should not add.
        assertFalse(mWifiCandidates.add(mScanDetail1, mConfig1, 2, 14, 0.0));
        assertEquals(0, mWifiCandidates.getFaultCount()); // But not considered a fault
        // Same evaluator, lower score. Should not add.
        assertFalse(mWifiCandidates.add(mScanDetail1, mConfig1, 2, 13, 0.0));
        assertEquals(0, mWifiCandidates.getFaultCount()); // Also not a fault
        // Later evaluator. Should not add (regardless of score).
        assertFalse(mWifiCandidates.add(mScanDetail1, mConfig1, 5, 13));
        assertFalse(mWifiCandidates.add(mScanDetail1, mConfig1, 5, 15));
        assertEquals(0, mWifiCandidates.getFaultCount()); // Still no faults
        // Evaluator out of order. Should not add (regardless of score).
        assertFalse(mWifiCandidates.add(mScanDetail1, mConfig1, 1, 12));
        assertNotNull(mWifiCandidates.getLastFault()); // This one is considered a caller error
        assertFalse(mWifiCandidates.add(mScanDetail1, mConfig1, 1, 15));
        assertEquals(2, mWifiCandidates.getFaultCount());
        // After all that, only one candidate should be there.
        assertEquals(1, mWifiCandidates.size());
    }

    /**
     * BSSID validation
     */
    @Test
    public void testBssidValidation() throws Exception {
        // Null BSSID.
        mScanResult1.BSSID = null;
        mWifiCandidates.add(mScanDetail1, mConfig1, 2, 14);
        assertTrue("Expecting NPE, got " + mWifiCandidates.getLastFault(),
                mWifiCandidates.getLastFault() instanceof NullPointerException);
        // Malformed BSSID
        mScanResult1.BSSID = "NotaBssid!";
        mWifiCandidates.add(mScanDetail1, mConfig1, 2, 14);
        assertTrue("Expecting IAE, got " + mWifiCandidates.getLastFault(),
                mWifiCandidates.getLastFault() instanceof IllegalArgumentException);
        assertEquals(0, mWifiCandidates.size());
    }

    /**
    * Add candidate BSSIDs in the same network, then remove them
    */
    @Test
    public void testTwoBssids() throws Exception {
        // Make a duplicate of the first config
        mConfig2 = new WifiConfiguration(mConfig1);
        // Make a second scan result, same network, different BSSID.
        mScanResult2.SSID = mScanResult1.SSID;
        mScanResult2.BSSID = mScanResult1.BSSID.replace('1', '2');
        // Add both
        mWifiCandidates.add(mScanDetail1, mConfig1, 2, 14);
        mWifiCandidates.add(mScanDetail2, mConfig2, 2, 14);
        // We expect them both to be there
        assertEquals(2, mWifiCandidates.size());
        // But just one group
        assertEquals(1, mWifiCandidates.getGroupedCandidates().size());
        // Now remove them one at a time
        WifiCandidates.Candidate c1, c2;
        c1 = mWifiCandidates.getGroupedCandidates().iterator().next().iterator().next();
        assertTrue(mWifiCandidates.remove(c1));
        assertEquals(1, mWifiCandidates.size());
        assertEquals(1, mWifiCandidates.getGroupedCandidates().size());
        // Should not be able to remove the one that isn't there
        assertFalse(mWifiCandidates.remove(c1));
        // Remove the other one, too
        c2 = mWifiCandidates.getGroupedCandidates().iterator().next().iterator().next();
        assertTrue(mWifiCandidates.remove(c2));
        assertFalse(mWifiCandidates.remove(c2));
        assertEquals(0, mWifiCandidates.size());
        assertEquals(0, mWifiCandidates.getGroupedCandidates().size());
        // Check that we have the right scan details, in either order
        assertTrue((mScanDetail1 == c1.getScanDetail() && mScanDetail2 == c2.getScanDetail())
                || (mScanDetail2 == c1.getScanDetail() && mScanDetail1 == c2.getScanDetail()));
    }

    /**
     * Test replacing a candidate with a higher scoring one
     */
    @Test
    public void testReplace() throws Exception {
        // Make a duplicate of the first config
        mConfig2 = new WifiConfiguration(mConfig1);
        // And the scan result
        mScanResult2.SSID = mScanResult1.SSID;
        mScanResult2.BSSID = mScanResult1.BSSID;
        // Try adding them both, the higher-scoring one second
        assertTrue(mWifiCandidates.add(mScanDetail2, mConfig2, 2, 14));
        assertTrue(mWifiCandidates.add(mScanDetail1, mConfig1, 2, 15));
        // Only one should survive
        assertEquals(1, mWifiCandidates.size());
        // And no faults
        assertEquals(0, mWifiCandidates.getFaultCount());
        // Make sure we kept the one with a higher evaluatorScore
        WifiCandidates.Candidate c;
        c = mWifiCandidates.getGroupedCandidates().iterator().next().iterator().next();
        assertEquals(15, c.getEvaluatorScore());
    }

}
