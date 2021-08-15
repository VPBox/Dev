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

import static android.net.wifi.WifiManager.WIFI_FEATURE_CONTROL_ROAMING;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

import android.app.test.MockAnswerUtil.AnswerWithArguments;

import androidx.test.filters.SmallTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Captor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.util.ArrayList;

/**
 * Unit tests for {@link com.android.server.wifi.WifiConnectivityHelper}.
 */
@SmallTest
public class WifiConnectivityHelperTest {
    /** Sets up test. */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        setupWifiNative();

        mWifiConnectivityHelper = new WifiConnectivityHelper(mWifiNative);
    }

    /** Cleans up test. */
    @After
    public void cleanup() {
        validateMockitoUsage();
    }

    private WifiConnectivityHelper mWifiConnectivityHelper;
    @Mock private WifiNative mWifiNative;
    @Captor ArgumentCaptor<WifiNative.RoamingConfig> mRoamingConfigCaptor;
    private int mFeatureSetValue;
    private static final String TAG = "WifiConnectivityHelperTest";
    private static final int MAX_BSSID_BLACKLIST_SIZE = 16;
    private static final int MAX_SSID_WHITELIST_SIZE = 8;

    private void setupWifiNative() {
        // Return firmware roaming feature as supported by default.
        when(mWifiNative.getSupportedFeatureSet(any()))
                .thenReturn((long) WIFI_FEATURE_CONTROL_ROAMING);

        doAnswer(new AnswerWithArguments() {
            public boolean answer(String ifaceName, WifiNative.RoamingCapabilities roamCap)
                    throws Exception {
                roamCap.maxBlacklistSize = MAX_BSSID_BLACKLIST_SIZE;
                roamCap.maxWhitelistSize = MAX_SSID_WHITELIST_SIZE;
                return true;
            }}).when(mWifiNative).getRoamingCapabilities(any(), anyObject());

        when(mWifiNative.configureRoaming(any(), anyObject())).thenReturn(true);
    }

    private ArrayList<String> buildBssidBlacklist(int size) {
        ArrayList<String> bssidBlacklist = new ArrayList<String>();

        for (int i = 0; i < size; i++) {
            StringBuilder bssid = new StringBuilder("11:22:33:44:55:66");
            bssid.setCharAt(16, (char) ('0' + i));
            bssidBlacklist.add(bssid.toString());
        }

        return bssidBlacklist;
    }

    private ArrayList<String> buildSsidWhitelist(int size) {
        ArrayList<String> ssidWhitelist = new ArrayList<String>();

        for (int i = 0; i < size; i++) {
            StringBuilder ssid = new StringBuilder("\"Test_Ap_0\"");
            ssid.setCharAt(9, (char) ('0' + i));
            ssidWhitelist.add(ssid.toString());
        }

        return ssidWhitelist;
    }

    /**
     * When WifiNative has WIFI_FEATURE_CONTROL_ROAMING set, verify that
     * WifiConnectivityHelper#isFirmwareRoamingSupported returns true.
     */
    @Test
    public void returnFirmwareRoamingSupported() {
        //By default WifiNative has WIFI_FEATURE_CONTROL_ROAMING set in its feature set.
        assertTrue(mWifiConnectivityHelper.getFirmwareRoamingInfo());
        assertTrue(mWifiConnectivityHelper.isFirmwareRoamingSupported());
    }

    /**
     * When WifiNative doesn't have WIFI_FEATURE_CONTROL_ROAMING set, verify that
     * WifiConnectivityHelper#isFirmwareRoamingSupported returns false.
     */
    @Test
    public void returnFirmwareRoamingNotSupported() {
        when(mWifiNative.getSupportedFeatureSet(any()))
                .thenReturn((long) ~WIFI_FEATURE_CONTROL_ROAMING);
        assertTrue(mWifiConnectivityHelper.getFirmwareRoamingInfo());
        assertFalse(mWifiConnectivityHelper.isFirmwareRoamingSupported());
    }

    /**
     * Verify that correct firmware roaming capability values are returned if querying
     * WifiNative for roaming capability succeeded.
     */
    @Test
    public void verifyFirmwareRoamingCapabilityWithSuccessfulNativeCall() {
        assertTrue(mWifiConnectivityHelper.getFirmwareRoamingInfo());
        assertTrue(mWifiConnectivityHelper.isFirmwareRoamingSupported());
        assertEquals(MAX_BSSID_BLACKLIST_SIZE, mWifiConnectivityHelper.getMaxNumBlacklistBssid());
        assertEquals(MAX_SSID_WHITELIST_SIZE, mWifiConnectivityHelper.getMaxNumWhitelistSsid());
    }

    /**
     * Verify that firmware roaming is set to not supported if WifiNative returned firmware roaming
     * is supported but failed to return roaming capabilities. Firmware roaming capabilty values
     * should be reset to INVALID_LIST_SIZE.
     */
    @Test
    public void verifyFirmwareRoamingCapabilityWithFailureNativeCall() {
        doAnswer(new AnswerWithArguments() {
            public boolean answer(String ifaceName, WifiNative.RoamingCapabilities roamCap)
                    throws Exception {
                return false;
            }}).when(mWifiNative).getRoamingCapabilities(any(), anyObject());
        assertFalse(mWifiConnectivityHelper.getFirmwareRoamingInfo());
        assertFalse(mWifiConnectivityHelper.isFirmwareRoamingSupported());
        assertEquals(WifiConnectivityHelper.INVALID_LIST_SIZE,
                mWifiConnectivityHelper.getMaxNumBlacklistBssid());
        assertEquals(WifiConnectivityHelper.INVALID_LIST_SIZE,
                mWifiConnectivityHelper.getMaxNumWhitelistSsid());
    }

    /**
     * Verify that firmware roaming is set to not supported if WifiNative returned firmware roaming
     * is supported but returned invalid max BSSID balcklist size. Firmware roaming capabilty values
     * should be reset to INVALID_LIST_SIZE.
     */
    @Test
    public void verifyFirmwareRoamingCapabilityWithInvalidMaxBssidBlacklistSize() {
        doAnswer(new AnswerWithArguments() {
            public boolean answer(String ifaceName, WifiNative.RoamingCapabilities roamCap)
                    throws Exception {
                roamCap.maxBlacklistSize = -5;
                roamCap.maxWhitelistSize = MAX_SSID_WHITELIST_SIZE;
                return true;
            }}).when(mWifiNative).getRoamingCapabilities(any(), anyObject());
        assertFalse(mWifiConnectivityHelper.getFirmwareRoamingInfo());
        assertFalse(mWifiConnectivityHelper.isFirmwareRoamingSupported());
        assertEquals(WifiConnectivityHelper.INVALID_LIST_SIZE,
                mWifiConnectivityHelper.getMaxNumBlacklistBssid());
        assertEquals(WifiConnectivityHelper.INVALID_LIST_SIZE,
                mWifiConnectivityHelper.getMaxNumWhitelistSsid());
    }

    /**
     * Verify that firmware roaming is set to not supported if WifiNative returned firmware roaming
     * is supported but returned invalid max SSID whitelist size. Firmware roaming capabilty values
     * should be reset to INVALID_LIST_SIZE.
     */
    @Test
    public void verifyFirmwareRoamingCapabilityWithInvalidMaxSsidWhitelistSize() {
        doAnswer(new AnswerWithArguments() {
            public boolean answer(String ifaceName, WifiNative.RoamingCapabilities roamCap)
                    throws Exception {
                roamCap.maxBlacklistSize = MAX_BSSID_BLACKLIST_SIZE;
                roamCap.maxWhitelistSize = -2;
                return true;
            }}).when(mWifiNative).getRoamingCapabilities(any(), anyObject());
        assertFalse(mWifiConnectivityHelper.getFirmwareRoamingInfo());
        assertFalse(mWifiConnectivityHelper.isFirmwareRoamingSupported());
        assertEquals(WifiConnectivityHelper.INVALID_LIST_SIZE,
                mWifiConnectivityHelper.getMaxNumBlacklistBssid());
        assertEquals(WifiConnectivityHelper.INVALID_LIST_SIZE,
                mWifiConnectivityHelper.getMaxNumWhitelistSsid());
    }

    /**
     * Verify that correct size BSSID blacklist and SSID whitelist are accepted.
     */
    @Test
    public void verifySetFirmwareRoamingConfigurationWithGoodInput() {
        assertTrue(mWifiConnectivityHelper.getFirmwareRoamingInfo());
        ArrayList<String> blacklist = buildBssidBlacklist(MAX_BSSID_BLACKLIST_SIZE);
        ArrayList<String> whitelist = buildSsidWhitelist(MAX_SSID_WHITELIST_SIZE);
        assertTrue(mWifiConnectivityHelper.setFirmwareRoamingConfiguration(blacklist, whitelist));

        blacklist = buildBssidBlacklist(MAX_BSSID_BLACKLIST_SIZE - 2);
        whitelist = buildSsidWhitelist(MAX_SSID_WHITELIST_SIZE - 3);
        assertTrue(mWifiConnectivityHelper.setFirmwareRoamingConfiguration(blacklist, whitelist));
    }

    /**
     * Verify that null BSSID blacklist or SSID whitelist is rejected.
     */
    @Test
    public void verifySetFirmwareRoamingConfigurationWithNullInput() {
        assertTrue(mWifiConnectivityHelper.getFirmwareRoamingInfo());
        ArrayList<String> blacklist = buildBssidBlacklist(MAX_BSSID_BLACKLIST_SIZE);
        ArrayList<String> whitelist = buildSsidWhitelist(MAX_SSID_WHITELIST_SIZE);
        assertFalse(mWifiConnectivityHelper.setFirmwareRoamingConfiguration(null, whitelist));
        assertFalse(mWifiConnectivityHelper.setFirmwareRoamingConfiguration(blacklist, null));
    }

    /**
     * Verify that incorrect size BSSID blacklist is rejected.
     */
    @Test
    public void verifySetFirmwareRoamingConfigurationWithIncorrectBlacklist() {
        assertTrue(mWifiConnectivityHelper.getFirmwareRoamingInfo());
        ArrayList<String> blacklist = buildBssidBlacklist(MAX_BSSID_BLACKLIST_SIZE + 1);
        ArrayList<String> whitelist = buildSsidWhitelist(MAX_SSID_WHITELIST_SIZE);
        assertFalse(mWifiConnectivityHelper.setFirmwareRoamingConfiguration(blacklist, whitelist));
    }

    /**
     * Verify that incorrect size SSID whitelist is rejected.
     */
    @Test
    public void verifySetFirmwareRoamingConfigurationWithIncorrectWhitelist() {
        assertTrue(mWifiConnectivityHelper.getFirmwareRoamingInfo());
        ArrayList<String> blacklist = buildBssidBlacklist(MAX_BSSID_BLACKLIST_SIZE);
        ArrayList<String> whitelist = buildSsidWhitelist(MAX_SSID_WHITELIST_SIZE + 1);
        assertFalse(mWifiConnectivityHelper.setFirmwareRoamingConfiguration(blacklist, whitelist));
    }

    /**
     * Verify that empty BSSID blacklist and SSID whitelist are sent to WifiNative
     * to reset the firmware roaming configuration.
     */
    @Test
    public void verifySetFirmwareRoamingConfigurationWithEmptyBlacklistAndWhitelist() {
        assertTrue(mWifiConnectivityHelper.getFirmwareRoamingInfo());
        ArrayList<String> blacklist = buildBssidBlacklist(0);
        ArrayList<String> whitelist = buildSsidWhitelist(0);
        assertTrue(mWifiConnectivityHelper.setFirmwareRoamingConfiguration(blacklist, whitelist));
        verify(mWifiNative).configureRoaming(any(), mRoamingConfigCaptor.capture());
        assertEquals(0, mRoamingConfigCaptor.getValue().blacklistBssids.size());
        assertEquals(0, mRoamingConfigCaptor.getValue().whitelistSsids.size());
    }
}
