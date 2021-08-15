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

import static com.android.server.wifi.WifiConfigurationTestUtil.SECURITY_NONE;
import static com.android.server.wifi.WifiConfigurationTestUtil.SECURITY_PSK;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

import android.content.Context;
import android.content.res.Resources;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.os.SystemClock;
import android.telephony.SubscriptionManager;
import android.util.LocalLog;

import androidx.test.filters.SmallTest;

import com.android.internal.R;
import com.android.server.wifi.WifiNetworkSelector.NetworkEvaluator.OnConnectableListener;
import com.android.server.wifi.WifiNetworkSelectorTestUtil.ScanDetailsAndWifiConfigs;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.util.List;

/**
 * Unit tests for {@link com.android.server.wifi.SavedNetworkEvaluator}.
 */
@SmallTest
public class SavedNetworkEvaluatorTest {

    /** Sets up test. */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        setupContext();
        setupResource();
        setupWifiConfigManager();
        mLocalLog = new LocalLog(512);

        when(mWifiConnectivityHelper.isFirmwareRoamingSupported()).thenReturn(false);
        when(mClock.getElapsedSinceBootMillis()).thenReturn(SystemClock.elapsedRealtime());

        mThresholdMinimumRssi2G = mResource.getInteger(
                R.integer.config_wifi_framework_wifi_score_bad_rssi_threshold_24GHz);
        mThresholdMinimumRssi5G = mResource.getInteger(
                R.integer.config_wifi_framework_wifi_score_bad_rssi_threshold_5GHz);
        mThresholdQualifiedRssi2G = mResource.getInteger(
                R.integer.config_wifi_framework_wifi_score_low_rssi_threshold_24GHz);
        mThresholdQualifiedRssi5G = mResource.getInteger(
                R.integer.config_wifi_framework_wifi_score_low_rssi_threshold_5GHz);
        mThresholdSaturatedRssi2G = mResource.getInteger(
                R.integer.config_wifi_framework_wifi_score_good_rssi_threshold_24GHz);
        mThresholdSaturatedRssi5G = mResource.getInteger(
                R.integer.config_wifi_framework_wifi_score_good_rssi_threshold_5GHz);

        mSavedNetworkEvaluator = new SavedNetworkEvaluator(mContext,
                new ScoringParams(mContext), mWifiConfigManager,
                mClock, mLocalLog, mWifiConnectivityHelper, mSubscriptionManager);
        // SIM is absent
        when(mSubscriptionManager.getActiveSubscriptionIdList()).thenReturn(new int[0]);
    }

    /** Cleans up test. */
    @After
    public void cleanup() {
        validateMockitoUsage();
    }

    private SavedNetworkEvaluator mSavedNetworkEvaluator;
    @Mock private WifiConfigManager mWifiConfigManager;
    @Mock private WifiConnectivityHelper mWifiConnectivityHelper;
    @Mock private Context mContext;
    @Mock private Resources mResource;
    @Mock private Clock mClock;
    @Mock private OnConnectableListener mOnConnectableListener;
    @Mock private SubscriptionManager mSubscriptionManager;
    private LocalLog mLocalLog;
    private int mThresholdMinimumRssi2G;
    private int mThresholdMinimumRssi5G;
    private int mThresholdQualifiedRssi2G;
    private int mThresholdQualifiedRssi5G;
    private int mThresholdSaturatedRssi2G;
    private int mThresholdSaturatedRssi5G;

    private void setupContext() {
        when(mContext.getResources()).thenReturn(mResource);
    }

    private void setupResource() {
        when(mResource.getInteger(
                R.integer.config_wifi_framework_wifi_score_good_rssi_threshold_5GHz))
                .thenReturn(-57);
        when(mResource.getInteger(
                R.integer.config_wifi_framework_wifi_score_good_rssi_threshold_24GHz))
                .thenReturn(-60);
        when(mResource.getInteger(
                R.integer.config_wifi_framework_wifi_score_low_rssi_threshold_5GHz))
                .thenReturn(-70);
        when(mResource.getInteger(
                R.integer.config_wifi_framework_wifi_score_low_rssi_threshold_24GHz))
                .thenReturn(-73);
        when(mResource.getInteger(
                R.integer.config_wifi_framework_wifi_score_bad_rssi_threshold_5GHz))
                .thenReturn(-82);
        when(mResource.getInteger(
                R.integer.config_wifi_framework_wifi_score_bad_rssi_threshold_24GHz))
                .thenReturn(-85);
        when(mResource.getInteger(
                R.integer.config_wifi_framework_RSSI_SCORE_SLOPE))
                .thenReturn(4);
        when(mResource.getInteger(
                R.integer.config_wifi_framework_RSSI_SCORE_OFFSET))
                .thenReturn(85);
        when(mResource.getInteger(
                R.integer.config_wifi_framework_SAME_BSSID_AWARD))
                .thenReturn(24);
        when(mResource.getInteger(
                R.integer.config_wifi_framework_SECURITY_AWARD))
                .thenReturn(80);
        when(mResource.getInteger(
                R.integer.config_wifi_framework_5GHz_preference_boost_factor))
                .thenReturn(16);
        when(mResource.getInteger(
                R.integer.config_wifi_framework_current_network_boost))
                .thenReturn(16);
    }

    private void setupWifiConfigManager() {
        when(mWifiConfigManager.getLastSelectedNetwork())
                .thenReturn(WifiConfiguration.INVALID_NETWORK_ID);
    }

    /**
     * Do not evaluate networks that {@link WifiConfiguration#useExternalScores}.
     */
    @Test
    public void ignoreNetworksIfUseExternalScores() {
        String[] ssids = {"\"test1\"", "\"test2\""};
        String[] bssids = {"6c:f3:7f:ae:8c:f3", "6c:f3:7f:ae:8c:f4"};
        int[] freqs = {2470, 2437};
        String[] caps = {"[WPA2-PSK][ESS]", "[WPA2-PSK][ESS]"};
        int[] levels = {mThresholdQualifiedRssi2G + 8, mThresholdQualifiedRssi2G + 10};
        int[] securities = {SECURITY_PSK, SECURITY_PSK};

        ScanDetailsAndWifiConfigs scanDetailsAndConfigs =
                WifiNetworkSelectorTestUtil.setupScanDetailsAndConfigStore(ssids, bssids,
                        freqs, caps, levels, securities, mWifiConfigManager, mClock);
        List<ScanDetail> scanDetails = scanDetailsAndConfigs.getScanDetails();
        WifiConfiguration[] savedConfigs = scanDetailsAndConfigs.getWifiConfigs();
        for (WifiConfiguration wifiConfiguration : savedConfigs) {
            wifiConfiguration.useExternalScores = true;
        }

        WifiConfiguration candidate = mSavedNetworkEvaluator.evaluateNetworks(scanDetails,
                null, null, true, false, mOnConnectableListener);

        assertNull(candidate);
    }

    /**
     * Do not evaluate networks that {@link WifiConfiguration#isEphemeral}.
     */
    @Test
    public void ignoreEphemeralNetworks() {
        String[] ssids = {"\"test1\"", "\"test2\""};
        String[] bssids = {"6c:f3:7f:ae:8c:f3", "6c:f3:7f:ae:8c:f4"};
        int[] freqs = {2470, 2437};
        String[] caps = {"[ESS]", "[ESS]"};
        int[] levels = {mThresholdQualifiedRssi2G + 8, mThresholdQualifiedRssi2G + 10};
        int[] securities = {SECURITY_NONE, SECURITY_NONE};

        ScanDetailsAndWifiConfigs scanDetailsAndConfigs =
                WifiNetworkSelectorTestUtil.setupScanDetailsAndConfigStore(ssids, bssids,
                        freqs, caps, levels, securities, mWifiConfigManager, mClock);
        List<ScanDetail> scanDetails = scanDetailsAndConfigs.getScanDetails();
        WifiConfiguration[] savedConfigs = scanDetailsAndConfigs.getWifiConfigs();
        for (WifiConfiguration wifiConfiguration : savedConfigs) {
            wifiConfiguration.ephemeral = true;
        }

        WifiConfiguration candidate = mSavedNetworkEvaluator.evaluateNetworks(scanDetails,
                null, null, true, false, mOnConnectableListener);

        assertNull(candidate);
    }

    /**
     * Set the candidate {@link ScanResult} for all {@link WifiConfiguration}s regardless of
     * whether they are secure saved, open saved, or {@link WifiConfiguration#useExternalScores}.
     */
    @Test
    public void setCandidateScanResultsForAllSavedNetworks() {
        String[] ssids = {"\"test1\"", "\"test2\"", "\"test3\""};
        String[] bssids = {"6c:f3:7f:ae:8c:f3", "6c:f3:7f:ae:8c:f4", "6c:f3:7f:ae:8c:f5"};
        int[] freqs = {5200, 5220, 5240};
        String[] caps = {"[WPA2-PSK][ESS]", "[ESS]", "[WPA2-PSK][ESS]"};
        int[] levels =
                {mThresholdQualifiedRssi5G, mThresholdQualifiedRssi5G, mThresholdQualifiedRssi5G};
        int[] securities = {SECURITY_PSK, SECURITY_NONE, SECURITY_PSK};

        ScanDetailsAndWifiConfigs scanDetailsAndConfigs =
                WifiNetworkSelectorTestUtil.setupScanDetailsAndConfigStore(ssids, bssids,
                        freqs, caps, levels, securities, mWifiConfigManager, mClock);
        List<ScanDetail> scanDetails = scanDetailsAndConfigs.getScanDetails();
        WifiConfiguration useExternalScoresConfig = scanDetailsAndConfigs.getWifiConfigs()[0];
        useExternalScoresConfig.useExternalScores = true;
        WifiConfiguration openNetworkConfig = scanDetailsAndConfigs.getWifiConfigs()[1];
        WifiConfiguration secureNetworkConfig = scanDetailsAndConfigs.getWifiConfigs()[2];

        mSavedNetworkEvaluator.evaluateNetworks(scanDetails,
                null, null, true, false, mOnConnectableListener);

        verify(mWifiConfigManager, atLeastOnce()).setNetworkCandidateScanResult(
                eq(useExternalScoresConfig.networkId),
                eq(scanDetails.get(0).getScanResult()),
                anyInt());
        verify(mWifiConfigManager, atLeastOnce()).setNetworkCandidateScanResult(
                eq(openNetworkConfig.networkId),
                eq(scanDetails.get(1).getScanResult()),
                anyInt());
        verify(mWifiConfigManager, atLeastOnce()).setNetworkCandidateScanResult(
                eq(secureNetworkConfig.networkId),
                eq(scanDetails.get(2).getScanResult()),
                anyInt());
    }

    /**
     * Between two 2G networks, choose the one with stronger RSSI value if other conditions
     * are the same and the RSSI values are not saturated.
     */
    @Test
    public void chooseStrongerRssi2GNetwork() {
        String[] ssids = {"\"test1\"", "\"test2\""};
        String[] bssids = {"6c:f3:7f:ae:8c:f3", "6c:f3:7f:ae:8c:f4"};
        int[] freqs = {2470, 2437};
        String[] caps = {"[WPA2-PSK][ESS]", "[WPA2-PSK][ESS]"};
        int[] levels = {mThresholdQualifiedRssi2G + 8, mThresholdQualifiedRssi2G + 10};
        int[] securities = {SECURITY_PSK, SECURITY_PSK};

        ScanDetailsAndWifiConfigs scanDetailsAndConfigs =
                WifiNetworkSelectorTestUtil.setupScanDetailsAndConfigStore(ssids, bssids,
                    freqs, caps, levels, securities, mWifiConfigManager, mClock);
        List<ScanDetail> scanDetails = scanDetailsAndConfigs.getScanDetails();
        WifiConfiguration[] savedConfigs = scanDetailsAndConfigs.getWifiConfigs();

        WifiConfiguration candidate = mSavedNetworkEvaluator.evaluateNetworks(scanDetails,
                null, null, true, false, mOnConnectableListener);

        ScanResult chosenScanResult = scanDetails.get(1).getScanResult();
        WifiConfigurationTestUtil.assertConfigurationEqual(savedConfigs[1], candidate);
        WifiNetworkSelectorTestUtil.verifySelectedScanResult(mWifiConfigManager,
                chosenScanResult, candidate);
    }

    /**
     * Between two 5G networks, choose the one with stronger RSSI value if other conditions
     * are the same and the RSSI values are not saturated.
     */
    @Test
    public void chooseStrongerRssi5GNetwork() {
        String[] ssids = {"\"test1\"", "\"test2\""};
        String[] bssids = {"6c:f3:7f:ae:8c:f3", "6c:f3:7f:ae:8c:f4"};
        int[] freqs = {5200, 5240};
        String[] caps = {"[WPA2-PSK][ESS]", "[WPA2-PSK][ESS]"};
        int[] levels = {mThresholdQualifiedRssi5G + 8, mThresholdQualifiedRssi5G + 10};
        int[] securities = {SECURITY_PSK, SECURITY_PSK};

        ScanDetailsAndWifiConfigs scanDetailsAndConfigs =
                WifiNetworkSelectorTestUtil.setupScanDetailsAndConfigStore(ssids, bssids,
                    freqs, caps, levels, securities, mWifiConfigManager, mClock);
        List<ScanDetail> scanDetails = scanDetailsAndConfigs.getScanDetails();
        WifiConfiguration[] savedConfigs = scanDetailsAndConfigs.getWifiConfigs();

        WifiConfiguration candidate = mSavedNetworkEvaluator.evaluateNetworks(scanDetails,
                null, null, true, false, mOnConnectableListener);

        ScanResult chosenScanResult = scanDetails.get(1).getScanResult();
        WifiConfigurationTestUtil.assertConfigurationEqual(savedConfigs[1], candidate);
        WifiNetworkSelectorTestUtil.verifySelectedScanResult(mWifiConfigManager,
                chosenScanResult, candidate);
    }

    /**
     * Choose secure network over open network if other conditions are the same.
     */
    @Test
    public void chooseSecureNetworkOverOpenNetwork() {
        String[] ssids = {"\"test1\"", "\"test2\""};
        String[] bssids = {"6c:f3:7f:ae:8c:f3", "6c:f3:7f:ae:8c:f4"};
        int[] freqs = {5200, 5240};
        String[] caps = {"[ESS]", "[WPA2-PSK][ESS]"};
        int[] levels = {mThresholdQualifiedRssi5G, mThresholdQualifiedRssi5G};
        int[] securities = {SECURITY_NONE, SECURITY_PSK};

        ScanDetailsAndWifiConfigs scanDetailsAndConfigs =
                WifiNetworkSelectorTestUtil.setupScanDetailsAndConfigStore(ssids, bssids,
                    freqs, caps, levels, securities, mWifiConfigManager, mClock);
        List<ScanDetail> scanDetails = scanDetailsAndConfigs.getScanDetails();
        WifiConfiguration[] savedConfigs = scanDetailsAndConfigs.getWifiConfigs();

        WifiConfiguration candidate = mSavedNetworkEvaluator.evaluateNetworks(scanDetails,
                null, null, true, false, mOnConnectableListener);

        ScanResult chosenScanResult = scanDetails.get(1).getScanResult();
        WifiConfigurationTestUtil.assertConfigurationEqual(savedConfigs[1], candidate);
        WifiNetworkSelectorTestUtil.verifySelectedScanResult(mWifiConfigManager,
                chosenScanResult, candidate);
    }

    /**
     * Choose 5G network over 2G network if other conditions are the same.
     */
    @Test
    public void choose5GNetworkOver2GNetwork() {
        String[] ssids = {"\"test1\"", "\"test2\""};
        String[] bssids = {"6c:f3:7f:ae:8c:f3", "6c:f3:7f:ae:8c:f4"};
        int[] freqs = {2437, 5240};
        String[] caps = {"[WPA2-PSK][ESS]", "[WPA2-PSK][ESS]"};
        int[] levels = {mThresholdQualifiedRssi2G, mThresholdQualifiedRssi5G};
        int[] securities = {SECURITY_PSK, SECURITY_PSK};

        ScanDetailsAndWifiConfigs scanDetailsAndConfigs =
                WifiNetworkSelectorTestUtil.setupScanDetailsAndConfigStore(ssids, bssids,
                    freqs, caps, levels, securities, mWifiConfigManager, mClock);
        List<ScanDetail> scanDetails = scanDetailsAndConfigs.getScanDetails();
        WifiConfiguration[] savedConfigs = scanDetailsAndConfigs.getWifiConfigs();

        WifiConfiguration candidate = mSavedNetworkEvaluator.evaluateNetworks(scanDetails,
                null, null, true, false, mOnConnectableListener);

        ScanResult chosenScanResult = scanDetails.get(1).getScanResult();
        WifiConfigurationTestUtil.assertConfigurationEqual(savedConfigs[1], candidate);
        WifiNetworkSelectorTestUtil.verifySelectedScanResult(mWifiConfigManager,
                chosenScanResult, candidate);
    }

    /**
     * Verify that we stick to the currently connected network if the other one is
     * just slightly better scored.
     */
    @Test
    public void stickToCurrentNetwork() {
        String[] ssids = {"\"test1\"", "\"test2\""};
        String[] bssids = {"6c:f3:7f:ae:8c:f3", "6c:f3:7f:ae:8c:f4"};
        int[] freqs = {5200, 5240};
        String[] caps = {"[WPA2-PSK][ESS]", "[WPA2-PSK][ESS]"};
        // test2 has slightly stronger RSSI value than test1
        int[] levels = {mThresholdMinimumRssi5G + 2, mThresholdMinimumRssi5G + 4};
        int[] securities = {SECURITY_PSK, SECURITY_PSK};

        ScanDetailsAndWifiConfigs scanDetailsAndConfigs =
                WifiNetworkSelectorTestUtil.setupScanDetailsAndConfigStore(ssids, bssids,
                    freqs, caps, levels, securities, mWifiConfigManager, mClock);
        List<ScanDetail> scanDetails = scanDetailsAndConfigs.getScanDetails();
        WifiConfiguration[] savedConfigs = scanDetailsAndConfigs.getWifiConfigs();

        // Simuluate we are connected to SSID test1 already.
        WifiConfiguration candidate = mSavedNetworkEvaluator.evaluateNetworks(scanDetails,
                savedConfigs[0], null, true, false, mOnConnectableListener);

        // Even though test2 has higher RSSI value, test1 is chosen because of the
        // currently connected network bonus.
        ScanResult chosenScanResult = scanDetails.get(0).getScanResult();
        WifiConfigurationTestUtil.assertConfigurationEqual(savedConfigs[0], candidate);
        WifiNetworkSelectorTestUtil.verifySelectedScanResult(mWifiConfigManager,
                chosenScanResult, candidate);
    }

    /**
     * Verify that we stick to the currently connected BSSID if the other one is
     * just slightly better scored.
     */
    @Test
    public void stickToCurrentBSSID() {
        // Same SSID
        String[] ssids = {"\"test1\"", "\"test1\""};
        String[] bssids = {"6c:f3:7f:ae:8c:f3", "6c:f3:7f:ae:8c:f4"};
        int[] freqs = {5200, 5240};
        String[] caps = {"[WPA2-PSK][ESS]", "[WPA2-PSK][ESS]"};
        // test2 has slightly stronger RSSI value than test1
        int[] levels = {mThresholdMinimumRssi5G + 2, mThresholdMinimumRssi5G + 6};
        int[] securities = {SECURITY_PSK, SECURITY_PSK};

        ScanDetailsAndWifiConfigs scanDetailsAndConfigs =
                WifiNetworkSelectorTestUtil.setupScanDetailsAndConfigStore(ssids, bssids,
                    freqs, caps, levels, securities, mWifiConfigManager, mClock);
        List<ScanDetail> scanDetails = scanDetailsAndConfigs.getScanDetails();
        WifiConfiguration[] savedConfigs = scanDetailsAndConfigs.getWifiConfigs();

        // Simuluate we are connected to BSSID "6c:f3:7f:ae:8c:f3" already
        WifiConfiguration candidate = mSavedNetworkEvaluator.evaluateNetworks(scanDetails,
                null, bssids[0], true, false, mOnConnectableListener);

        // Even though test2 has higher RSSI value, test1 is chosen because of the
        // currently connected BSSID bonus.
        ScanResult chosenScanResult = scanDetails.get(0).getScanResult();
        WifiConfigurationTestUtil.assertConfigurationEqual(savedConfigs[0], candidate);
    }

    /**
     * Verify that the same BSSID award is applied to all the BSSIDs which are under the same
     * network as the currently connected BSSID.
     */
    @Test
    public void currentBssidAwardForAllBssidsWithinTheSameNetworkWhenFirmwareRoamingSupported() {
        // Three BSSIDs are carefully setup there:
        // BSSID_0 and BSSID_1 have the same SSID and security type, so they are considered under
        // the same 2.4 GHz network. BSSID_1 RSSI is stronger than BSSID_0.
        // BSSID_2 is under a 5GHz network different from BSSID_0 and BSSID_1. Its RSSI is
        // slightly stronger than BSSID_1.
        //
        // When firmware roaming is not supported, BSSID_2 has higher score than BSSID_0 and
        // BSSID_1.
        // When firmware roaming is suported, BSSID_1 has higher score than BSSID_2 because the
        // same BSSID award is now applied to both BSSID_0 and BSSID_1.
        String[] ssids = {"\"test1\"", "\"test1\"", "\"test2\""};
        String[] bssids = {"6c:f3:7f:ae:8c:f3", "6c:f3:7f:ae:8c:f4", "6c:f3:7f:ae:8c:f5"};
        int[] freqs = {2470, 2437, 5200};
        String[] caps = {"[WPA2-PSK][ESS]", "[WPA2-PSK][ESS]", "[WPA2-PSK][ESS]"};
        int[] levels = {mThresholdMinimumRssi2G + 2, mThresholdMinimumRssi2G + 5,
                mThresholdMinimumRssi5G + 7};
        int[] securities = {SECURITY_PSK, SECURITY_PSK, SECURITY_PSK};

        ScanDetailsAndWifiConfigs scanDetailsAndConfigs =
                WifiNetworkSelectorTestUtil.setupScanDetailsAndConfigStore(ssids, bssids,
                    freqs, caps, levels, securities, mWifiConfigManager, mClock);
        List<ScanDetail> scanDetails = scanDetailsAndConfigs.getScanDetails();
        WifiConfiguration[] savedConfigs = scanDetailsAndConfigs.getWifiConfigs();

        // Firmware roaming is not supported.
        when(mWifiConnectivityHelper.isFirmwareRoamingSupported()).thenReturn(false);
        // Simuluate we are connected to BSSID_0 already.
        WifiConfiguration candidate = mSavedNetworkEvaluator.evaluateNetworks(scanDetails,
                savedConfigs[0], bssids[0], true, false, mOnConnectableListener);
        // Verify that BSSID_2 is chosen.
        ScanResult chosenScanResult = scanDetails.get(2).getScanResult();
        WifiConfigurationTestUtil.assertConfigurationEqual(savedConfigs[2], candidate);
        WifiNetworkSelectorTestUtil.verifySelectedScanResult(mWifiConfigManager,
                chosenScanResult, candidate);

        // Firmware roaming is supported.
        when(mWifiConnectivityHelper.isFirmwareRoamingSupported()).thenReturn(true);
        // Simuluate we are connected to BSSID_0 already.
        candidate = mSavedNetworkEvaluator.evaluateNetworks(scanDetails,
                savedConfigs[0], bssids[0], true, false, mOnConnectableListener);
        // Verify that BSSID_1 is chosen.
        chosenScanResult = scanDetails.get(1).getScanResult();
        WifiConfigurationTestUtil.assertConfigurationEqual(savedConfigs[1], candidate);
        WifiNetworkSelectorTestUtil.verifySelectedScanResult(mWifiConfigManager,
                chosenScanResult, candidate);
    }

    /**
     * One 2.4GHz network and one 5GHz network have the same security type. Perform
     * the following tests to verify that once across the RSSI saturation threshold
     * stronger RSSI value doesn't increase network score.
     *
     * 1) Both 2.4GHz network and 5GHz network have the same RSSI value,
     *    mThresholdQualifiedRssi2G, which is below the saturation threshold. 5GHz
     *    network is chosen because of the 5G band award.
     * 2) Bump up 2.4GHz network RSSI 20dBm higher. Verify that it helps the 2.4GHz network
     *    score and it gets chosen over the 5GHz network.
     * 3) Bring both 2.4GHz network and 5GHz network RSSI value to mThresholdSaturatedRssi2G.
     *    Verify that 5GHz network is chosen because of the 5G band award.
     * 4) Bump up 2.4GHz network RSSI to be 20dBm higher than mThresholdSaturatedRssi2G.
     *    Verify that the incresed RSSI doesn't help 2.4GHz network score and 5GHz network
     *    is still chosen.
     */
    @Test
    public void saturatedRssiAddsNoWeightToNetwork() {
        String[] ssids = {"\"test1\"", "\"test2\""};
        String[] bssids = {"6c:f3:7f:ae:8c:f3", "6c:f3:7f:ae:8c:f4"};
        int[] freqs = {2437, 5400};
        String[] caps = {"[WPA2-PSK][ESS]", "[WPA2-PSK][ESS]"};
        int[] securities = {SECURITY_PSK, SECURITY_PSK};

        // 1) The RSSI of both networks is mThresholdQualifiedRssi2G
        int[] levels = {mThresholdQualifiedRssi2G, mThresholdQualifiedRssi2G};
        ScanDetailsAndWifiConfigs scanDetailsAndConfigs =
                WifiNetworkSelectorTestUtil.setupScanDetailsAndConfigStore(ssids, bssids,
                    freqs, caps, levels, securities, mWifiConfigManager, mClock);
        List<ScanDetail> scanDetails = scanDetailsAndConfigs.getScanDetails();
        WifiConfiguration[] savedConfigs = scanDetailsAndConfigs.getWifiConfigs();
        WifiConfiguration candidate = mSavedNetworkEvaluator.evaluateNetworks(scanDetails,
                null, null, false, false, mOnConnectableListener);
        // Verify that 5GHz network is chosen because of 5G band award
        ScanResult chosenScanResult = scanDetails.get(1).getScanResult();
        WifiConfigurationTestUtil.assertConfigurationEqual(savedConfigs[1], candidate);
        WifiNetworkSelectorTestUtil.verifySelectedScanResult(mWifiConfigManager,
                chosenScanResult, candidate);

        // 2) Bump up 2.4GHz network RSSI by 20dBm.
        levels[0] = mThresholdQualifiedRssi2G + 20;
        scanDetailsAndConfigs = WifiNetworkSelectorTestUtil.setupScanDetailsAndConfigStore(ssids,
                bssids, freqs, caps, levels, securities, mWifiConfigManager, mClock);
        scanDetails = scanDetailsAndConfigs.getScanDetails();
        savedConfigs = scanDetailsAndConfigs.getWifiConfigs();
        candidate = mSavedNetworkEvaluator.evaluateNetworks(scanDetails, null, null, false,
                false, mOnConnectableListener);
        // Verify that 2.4GHz network is chosen because of much higher RSSI value
        chosenScanResult = scanDetails.get(0).getScanResult();
        WifiConfigurationTestUtil.assertConfigurationEqual(savedConfigs[0], candidate);
        WifiNetworkSelectorTestUtil.verifySelectedScanResult(mWifiConfigManager,
                chosenScanResult, candidate);

        // 3) Bring both 2.4GHz network and 5GHz network RSSI to mThresholdSaturatedRssi2G
        levels[0] = levels[1] = mThresholdSaturatedRssi2G;
        scanDetailsAndConfigs = WifiNetworkSelectorTestUtil.setupScanDetailsAndConfigStore(ssids,
                bssids, freqs, caps, levels, securities, mWifiConfigManager, mClock);
        scanDetails = scanDetailsAndConfigs.getScanDetails();
        savedConfigs = scanDetailsAndConfigs.getWifiConfigs();
        candidate = mSavedNetworkEvaluator.evaluateNetworks(scanDetails, null, null, false,
                false, mOnConnectableListener);
        // Verify that 5GHz network is chosen because of 5G band award
        chosenScanResult = scanDetails.get(1).getScanResult();
        WifiConfigurationTestUtil.assertConfigurationEqual(savedConfigs[1], candidate);
        WifiNetworkSelectorTestUtil.verifySelectedScanResult(mWifiConfigManager,
                chosenScanResult, candidate);

        // 4) Bump 2.4GHz network RSSI to be 20dBm higher than mThresholdSaturatedRssi2G
        levels[0] = mThresholdSaturatedRssi2G + 20;
        scanDetailsAndConfigs = WifiNetworkSelectorTestUtil.setupScanDetailsAndConfigStore(ssids,
                bssids, freqs, caps, levels, securities, mWifiConfigManager, mClock);
        scanDetails = scanDetailsAndConfigs.getScanDetails();
        savedConfigs = scanDetailsAndConfigs.getWifiConfigs();
        candidate = mSavedNetworkEvaluator.evaluateNetworks(scanDetails, null, null, false,
                false, mOnConnectableListener);
        // Verify that the increased RSSI doesn't help 2.4GHz network and 5GHz network
        // is still chosen
        chosenScanResult = scanDetails.get(1).getScanResult();
        WifiConfigurationTestUtil.assertConfigurationEqual(savedConfigs[1], candidate);
        WifiNetworkSelectorTestUtil.verifySelectedScanResult(mWifiConfigManager,
                chosenScanResult, candidate);
    }
}
