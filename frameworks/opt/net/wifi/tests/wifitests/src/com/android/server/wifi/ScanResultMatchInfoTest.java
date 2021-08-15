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
package com.android.server.wifi;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.*;

import android.net.wifi.WifiConfiguration;

import androidx.test.filters.SmallTest;

import org.junit.Test;

/**
 * Unit tests for {@link com.android.server.wifi.ScanResultMatchInfoTest}.
 */
@SmallTest
public class ScanResultMatchInfoTest {
    /**
     * Tests that equivalent ScanResultMatchInfo objects are created for WifiConfigurations and
     * their associated ScanResult
     */
    @Test
    public void testScanResultMatchesWifiConfiguration() {
        WifiConfiguration conf =
                WifiConfigurationTestUtil.createPskNetwork("\"PrettyFlyForAWifi\"");
        ScanDetail scan = createScanDetailForNetwork(conf, "AA:AA:AA:AA:AA:AA");
        assertEquals(ScanResultMatchInfo.fromWifiConfiguration(conf),
                ScanResultMatchInfo.fromScanResult(scan.getScanResult()));

        conf = WifiConfigurationTestUtil.createOpenNetwork("\"WIFIght the inevitable\"");
        scan = createScanDetailForNetwork(conf, "BB:BB:BB:BB:BB:BB");
        assertEquals(ScanResultMatchInfo.fromWifiConfiguration(conf),
                ScanResultMatchInfo.fromScanResult(scan.getScanResult()));
    }

    /**
     * Tests that multiple ScanResults with different BSSIDs will produce equivalent
     * ScanResultMatchInfo objects to their associated WifiConfiguration
     */
    @Test
    public void testDifferentBssidScanResultsMatch() {
        WifiConfiguration conf =
                WifiConfigurationTestUtil.createPskNetwork("\"PrettyFlyForAWifi-5G\"");
        ScanDetail scan1 = createScanDetailForNetwork(conf, "AA:AA:AA:AA:AA:AA");
        ScanDetail scan2 = createScanDetailForNetwork(conf, "BB:BB:BB:BB:BB:BB");
        assertFalse(scan1.getScanResult().BSSID.equals(scan2.getScanResult().BSSID));
        assertEquals(ScanResultMatchInfo.fromScanResult(scan1.getScanResult()),
                ScanResultMatchInfo.fromScanResult(scan2.getScanResult()));
    }

    /**
     * Tests that ScanResultMatchInfo objects created for different SSIDs or security types are not
     * equivalent
     */
    @Test
    public void testDifferentNetworkScanResultsDontMatch() {
        WifiConfiguration psk =
                WifiConfigurationTestUtil.createPskNetwork("\"Series Of Tubes\"");
        WifiConfiguration open1 =
                WifiConfigurationTestUtil.createOpenNetwork("\"Series Of Tubes\"");
        WifiConfiguration open2 =
                WifiConfigurationTestUtil.createOpenNetwork("\"Mom, Click Here For Internet\"");
        ScanDetail scanOpen1 = createScanDetailForNetwork(open1, "AA:AA:AA:AA:AA:AA");
        ScanDetail scanOpen2 = createScanDetailForNetwork(open2, "BB:BB:BB:BB:BB:BB");
        ScanDetail scanPsk =   createScanDetailForNetwork(psk,   "CC:CC:CC:CC:CC:CC");
        assertTrue(ScanResultMatchInfo.fromScanResult(scanOpen1.getScanResult())
                != ScanResultMatchInfo.fromScanResult(scanOpen2.getScanResult()));
        assertTrue(ScanResultMatchInfo.fromScanResult(scanOpen1.getScanResult())
                != ScanResultMatchInfo.fromScanResult(scanPsk.getScanResult()));
    }

    /**
     * Tests equality properties, reflexive, symmetric, transitive, consistent for transition mode
     * network.
     */
    @Test
    public void testEqualityRulesForTransitionMode() {
        WifiConfiguration wifiConfiguration =
                WifiConfigurationTestUtil.createPskNetwork("\"Transition is Hard\"");
        ScanDetail scanDetail = createScanDetailForWpa2Wpa3TransitionModeNetwork(wifiConfiguration,
                "AA:BB:CC:DD:CC:BB");

        ScanResultMatchInfo key1 = ScanResultMatchInfo.fromWifiConfiguration(wifiConfiguration);
        ScanResultMatchInfo key2 = ScanResultMatchInfo.fromScanResult(scanDetail.getScanResult());
        ScanResultMatchInfo key3 = ScanResultMatchInfo.fromWifiConfiguration(wifiConfiguration);

        // Test a.equals(a)
        assertTrue(key1.equals(key1));

        // Test if a.equals(b) then b.equals(a)
        assertTrue(key1.equals(key2));
        assertTrue(key2.equals(key1));

        // Test transitivity
        assertTrue(key1.equals(key2));
        assertTrue(key2.equals(key3));
        assertTrue(key1.equals(key3));

        // Test consistency
        assertTrue(key1.equals(key2));
        assertTrue(key1.equals(key2));
        assertTrue(key1.equals(key2));
        assertTrue(key1.equals(key2));
    }

    /**
     * Tests equality properties, reflexive, symmetric, transitive, consistent for PSK network.
     */
    @Test
    public void testEqualityRulesForPsk() {
        WifiConfiguration wifiConfiguration =
                WifiConfigurationTestUtil.createPskNetwork("\"Psk Tsk\"");
        ScanDetail scanDetail = createScanDetailForNetwork(wifiConfiguration,
                "AA:BB:CC:DD:CC:BB");

        ScanResultMatchInfo key1 = ScanResultMatchInfo.fromWifiConfiguration(wifiConfiguration);
        ScanResultMatchInfo key2 = ScanResultMatchInfo.fromScanResult(scanDetail.getScanResult());
        ScanResultMatchInfo key3 = ScanResultMatchInfo.fromWifiConfiguration(wifiConfiguration);

        // Test a.equals(a)
        assertTrue(key1.equals(key1));

        // Test if a.equals(b) then b.equals(a)
        assertTrue(key1.equals(key2));
        assertTrue(key2.equals(key1));

        // Test transitivity
        assertTrue(key1.equals(key2));
        assertTrue(key2.equals(key3));
        assertTrue(key1.equals(key3));

        // Test consistency
        assertTrue(key1.equals(key2));
        assertTrue(key1.equals(key2));
        assertTrue(key1.equals(key2));
        assertTrue(key1.equals(key2));
    }


    /**
     * Tests equality properties, reflexive, symmetric, transitive, consistent for SAE network.
     */
    @Test
    public void testEqualityRulesForSae() {
        WifiConfiguration wifiConfiguration =
                WifiConfigurationTestUtil.createSaeNetwork();
        ScanDetail scanDetail = createScanDetailForNetwork(wifiConfiguration,
                "AC:AB:AD:AE:AF:FC");

        ScanResultMatchInfo key1 = ScanResultMatchInfo.fromWifiConfiguration(wifiConfiguration);
        ScanResultMatchInfo key2 = ScanResultMatchInfo.fromScanResult(scanDetail.getScanResult());
        ScanResultMatchInfo key3 = ScanResultMatchInfo.fromWifiConfiguration(wifiConfiguration);

        // Test a.equals(a)
        assertTrue(key1.equals(key1));

        // Test if a.equals(b) then b.equals(a)
        assertTrue(key1.equals(key2));
        assertTrue(key2.equals(key1));

        // Test transitivity
        assertTrue(key1.equals(key2));
        assertTrue(key2.equals(key3));
        assertTrue(key1.equals(key3));

        // Test consistency
        assertTrue(key1.equals(key2));
        assertTrue(key1.equals(key2));
        assertTrue(key1.equals(key2));
        assertTrue(key1.equals(key2));
    }

    /**
     * Tests that hashes of various configurations are equal
     */
    @Test
    public void testHashForTransitionMode() {
        WifiConfiguration wifiConfigurationPsk =
                WifiConfigurationTestUtil.createPskNetwork("\"Transition is Hard\"");
        WifiConfiguration wifiConfigurationSae =
                WifiConfigurationTestUtil.createSaeNetwork("\"Transition is Hard\"");
        ScanDetail scanDetail = createScanDetailForWpa2Wpa3TransitionModeNetwork(
                wifiConfigurationPsk, "AA:BB:CC:DD:CC:BB");

        ScanResultMatchInfo key1 = ScanResultMatchInfo.fromWifiConfiguration(wifiConfigurationPsk);
        ScanResultMatchInfo key2 = ScanResultMatchInfo.fromScanResult(scanDetail.getScanResult());
        ScanResultMatchInfo key3 = ScanResultMatchInfo.fromWifiConfiguration(wifiConfigurationSae);

        // Assert that all hashes are equal
        assertEquals(key1.hashCode(), key2.hashCode());
        assertEquals(key1.hashCode(), key3.hashCode());
        assertEquals(key2.hashCode(), key3.hashCode());
    }

    /**
     * Creates a scan detail corresponding to the provided network and given BSSID
     */
    private ScanDetail createScanDetailForNetwork(
            WifiConfiguration configuration, String bssid) {
        return WifiConfigurationTestUtil.createScanDetailForNetwork(configuration, bssid, -40,
                2402, 0, 0);
    }

    /**
     * Creates a scan detail corresponding to the provided network and given BSSID
     */
    private ScanDetail createScanDetailForWpa2Wpa3TransitionModeNetwork(
            WifiConfiguration configuration, String bssid) {
        return WifiConfigurationTestUtil.createScanDetailForWpa2Wpa3TransitionModeNetwork(
                configuration, bssid, -40, 2402, 0, 0);
    }
}
