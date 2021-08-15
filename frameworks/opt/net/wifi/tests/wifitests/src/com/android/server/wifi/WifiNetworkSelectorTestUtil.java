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

import static com.android.server.wifi.WifiConfigurationTestUtil.generateWifiConfig;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

import android.app.test.MockAnswerUtil.AnswerWithArguments;
import android.net.NetworkKey;
import android.net.RssiCurve;
import android.net.ScoredNetwork;
import android.net.WifiKey;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiConfiguration.NetworkSelectionStatus;
import android.net.wifi.WifiNetworkScoreCache;
import android.net.wifi.WifiSsid;
import android.text.TextUtils;

import com.android.server.wifi.util.NativeUtil;
import com.android.server.wifi.util.ScanResultUtil;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Helper for WifiNetworkSelector unit tests.
 */
public class WifiNetworkSelectorTestUtil {

    /**
     * A class that holds a list of scanDetail and their associated WifiConfiguration.
     */
    public static class ScanDetailsAndWifiConfigs {
        List<ScanDetail> mScanDetails;
        WifiConfiguration[] mWifiConfigs;

        ScanDetailsAndWifiConfigs(List<ScanDetail> scanDetails, WifiConfiguration[] configs) {
            mScanDetails = scanDetails;
            mWifiConfigs = configs;
        }

        List<ScanDetail> getScanDetails() {
            return mScanDetails;
        }

        WifiConfiguration[] getWifiConfigs() {
            return mWifiConfigs;
        }
    }

    /**
     * Build a list of ScanDetail based on the caller supplied network SSID, BSSID,
     * frequency, capability and RSSI level information. Create the corresponding
     * WifiConfiguration for these networks and set up the mocked WifiConfigManager.
     *
     * @param ssids an array of SSIDs
     * @param bssids an array of BSSIDs
     * @param freqs an array of the network's frequency
     * @param caps an array of the network's capability
     * @param levels an array of the network's RSSI levels
     * @param securities an array of the network's security setting
     * @param wifiConfigManager the mocked WifiConfigManager
     * @return the constructed ScanDetail list and WifiConfiguration array
     */
    public static ScanDetailsAndWifiConfigs setupScanDetailsAndConfigStore(String[] ssids,
                String[] bssids, int[] freqs, String[] caps, int[] levels, int[] securities,
                WifiConfigManager wifiConfigManager, Clock clock) {
        List<ScanDetail> scanDetails = buildScanDetails(ssids, bssids, freqs, caps, levels, clock);
        WifiConfiguration[] savedConfigs = generateWifiConfigurations(ssids, securities);
        checkConsistencyOfScanDetailsAndWifiConfigs(scanDetails, savedConfigs);
        prepareConfigStore(wifiConfigManager, savedConfigs);
        scanResultLinkConfiguration(wifiConfigManager, savedConfigs, scanDetails);

        return new ScanDetailsAndWifiConfigs(scanDetails, savedConfigs);
    }

    private static void checkConsistencyOfScanDetailsAndWifiConfigs(
            List<ScanDetail> scanDetails,
            WifiConfiguration[] savedConfigs) {
        assertEquals(scanDetails.size(), savedConfigs.length);
        for (int i = 0; i < savedConfigs.length; i++) {
            ScanResult scanResult = scanDetails.get(i).getScanResult();
            WifiConfiguration config = savedConfigs[i];
            assertEquals("Problem in entry " + i,
                    ScanResultMatchInfo.fromScanResult(scanResult),
                    ScanResultMatchInfo.fromWifiConfiguration(config));
        }
    }

    /**
     * Verify whether the WifiConfiguration chosen by WifiNetworkSelector matches
     * with the chosen scan result.
     *
     * @param chosenScanResult the chosen scan result
     * @param chosenCandidate  the chosen configuration
     */
    public static void verifySelectedScanResult(WifiConfigManager wifiConfigManager,
            ScanResult chosenScanResult, WifiConfiguration chosenCandidate) {
        verify(wifiConfigManager, atLeastOnce()).setNetworkCandidateScanResult(
                eq(chosenCandidate.networkId), eq(chosenScanResult), anyInt());
    }


    /**
     * Build a list of scanDetails based on the caller supplied network SSID, BSSID,
     * frequency, capability and RSSI level information.
     *
     * @param ssids an array of SSIDs
     * @param bssids an array of BSSIDs
     * @param freqs an array of the network's frequency
     * @param caps an array of the network's capability
     * @param levels an array of the network's RSSI levels
     * @return the constructed list of ScanDetail
     */
    public static List<ScanDetail> buildScanDetails(String[] ssids, String[] bssids, int[] freqs,
                                            String[] caps, int[] levels, Clock clock) {
        List<ScanDetail> scanDetailList = new ArrayList<ScanDetail>();

        long timeStamp = clock.getElapsedSinceBootMillis();
        for (int index = 0; index < ssids.length; index++) {
            byte[] ssid = NativeUtil.byteArrayFromArrayList(NativeUtil.decodeSsid(ssids[index]));
            ScanDetail scanDetail = new ScanDetail(WifiSsid.createFromByteArray(ssid),
                    bssids[index], caps[index], levels[index], freqs[index], timeStamp, 0);
            scanDetailList.add(scanDetail);
        }
        return scanDetailList;
    }


    /**
     * Generate an array of {@link android.net.wifi.WifiConfiguration} based on the caller
     * supplied network SSID and security information.
     *
     * @param ssids an array of SSIDs
     * @param securities an array of the network's security setting
     * @return the constructed array of {@link android.net.wifi.WifiConfiguration}
     */
    public static WifiConfiguration[] generateWifiConfigurations(String[] ssids,
                int[] securities) {
        if (ssids == null || securities == null || ssids.length != securities.length) {
            throw new IllegalArgumentException();
        }

        Map<String, Integer> netIdMap = new HashMap<>();
        int netId = 0;

        WifiConfiguration[] configs = new WifiConfiguration[ssids.length];
        for (int index = 0; index < ssids.length; index++) {
            String configKey = ssids[index] + Integer.toString(securities[index]);
            Integer id;

            id = netIdMap.get(configKey);
            if (id == null) {
                id = new Integer(netId);
                netIdMap.put(configKey, id);
                netId++;
            }

            configs[index] = generateWifiConfig(id.intValue(), 0, ssids[index], false, true, null,
                    null, securities[index]);
            configs[index].preSharedKey = "\"PA55W0RD\""; // needed to validate with PSK
            if (!WifiConfigurationUtil.validate(configs[index], true)) {
                throw new IllegalArgumentException("Invalid generated config: " + configs[index]);
            }
        }

        return configs;
    }

    /**
     * Add the Configurations to WifiConfigManager (WifiConfigureStore can take them out according
     * to the networkd ID) and setup the WifiConfigManager mocks for these networks.
     * This simulates the WifiConfigManager class behaviour.
     *
     * @param wifiConfigManager the mocked WifiConfigManager
     * @param configs input configuration need to be added to WifiConfigureStore
     */
    private static void prepareConfigStore(final WifiConfigManager wifiConfigManager,
                final WifiConfiguration[] configs) {
        when(wifiConfigManager.getConfiguredNetwork(anyInt()))
                .then(new AnswerWithArguments() {
                    public WifiConfiguration answer(int netId) {
                        for (WifiConfiguration config : configs) {
                            if (netId == config.networkId) {
                                return new WifiConfiguration(config);
                            }
                        }
                        return null;
                    }
                });
        when(wifiConfigManager.getConfiguredNetwork(anyString()))
                .then(new AnswerWithArguments() {
                    public WifiConfiguration answer(String configKey) {
                        for (WifiConfiguration config : configs) {
                            if (TextUtils.equals(config.configKey(), configKey)) {
                                return new WifiConfiguration(config);
                            }
                        }
                        return null;
                    }
                });
        when(wifiConfigManager.getConfiguredNetworks())
                .then(new AnswerWithArguments() {
                    public List<WifiConfiguration> answer() {
                        List<WifiConfiguration> savedNetworks = new ArrayList<>();
                        for (int netId = 0; netId < configs.length; netId++) {
                            savedNetworks.add(new WifiConfiguration(configs[netId]));
                        }
                        return savedNetworks;
                    }
                });
        when(wifiConfigManager.clearNetworkCandidateScanResult(anyInt()))
                .then(new AnswerWithArguments() {
                    public boolean answer(int netId) {
                        if (netId >= 0 && netId < configs.length) {
                            configs[netId].getNetworkSelectionStatus().setCandidate(null);
                            configs[netId].getNetworkSelectionStatus()
                                    .setCandidateScore(Integer.MIN_VALUE);
                            configs[netId].getNetworkSelectionStatus()
                                    .setSeenInLastQualifiedNetworkSelection(false);
                            return true;
                        } else {
                            return false;
                        }
                    }
                });
        when(wifiConfigManager.setNetworkCandidateScanResult(
                anyInt(), any(ScanResult.class), anyInt()))
                .then(new AnswerWithArguments() {
                    public boolean answer(int netId, ScanResult scanResult, int score) {
                        if (netId >= 0 && netId < configs.length) {
                            configs[netId].getNetworkSelectionStatus().setCandidate(scanResult);
                            configs[netId].getNetworkSelectionStatus().setCandidateScore(score);
                            configs[netId].getNetworkSelectionStatus()
                                    .setSeenInLastQualifiedNetworkSelection(true);
                            return true;
                        } else {
                            return false;
                        }
                    }
                });
        when(wifiConfigManager.clearNetworkConnectChoice(anyInt()))
                .then(new AnswerWithArguments() {
                    public boolean answer(int netId) {
                        if (netId >= 0 && netId < configs.length) {
                            configs[netId].getNetworkSelectionStatus().setConnectChoice(null);
                            configs[netId].getNetworkSelectionStatus()
                                    .setConnectChoiceTimestamp(
                                            NetworkSelectionStatus
                                                    .INVALID_NETWORK_SELECTION_DISABLE_TIMESTAMP);
                            return true;
                        } else {
                            return false;
                        }
                    }
                });
        when(wifiConfigManager.setNetworkConnectChoice(anyInt(), anyString(), anyLong()))
                .then(new AnswerWithArguments() {
                    public boolean answer(int netId, String configKey, long timestamp) {
                        if (netId >= 0 && netId < configs.length) {
                            configs[netId].getNetworkSelectionStatus().setConnectChoice(configKey);
                            configs[netId].getNetworkSelectionStatus().setConnectChoiceTimestamp(
                                    timestamp);
                            return true;
                        } else {
                            return false;
                        }
                    }
                });
    }


    /**
     * Link scan results to the saved configurations.
     *
     * The shorter of the 2 input params will be used to loop over so the inputs don't
     * need to be of equal length. If there are more scan details then configs the remaining scan
     * details will be associated with a NULL config.
     *
     * @param wifiConfigManager the mocked WifiConfigManager
     * @param configs     saved configurations
     * @param scanDetails come in scan results
     */
    private static void scanResultLinkConfiguration(WifiConfigManager wifiConfigManager,
                WifiConfiguration[] configs, List<ScanDetail> scanDetails) {
        if (configs == null || scanDetails == null) {
            return;
        }

        if (scanDetails.size() <= configs.length) {
            for (int i = 0; i < scanDetails.size(); i++) {
                ScanDetail scanDetail = scanDetails.get(i);
                when(wifiConfigManager.getConfiguredNetworkForScanDetailAndCache(eq(scanDetail)))
                        .thenReturn(configs[i]);
            }
        } else {
            for (int i = 0; i < configs.length; i++) {
                ScanDetail scanDetail = scanDetails.get(i);
                when(wifiConfigManager.getConfiguredNetworkForScanDetailAndCache(eq(scanDetail)))
                        .thenReturn(configs[i]);
            }

            // associated the remaining scan details with a NULL config.
            for (int i = configs.length; i < scanDetails.size(); i++) {
                when(wifiConfigManager.getConfiguredNetworkForScanDetailAndCache(
                        eq(scanDetails.get(i)))).thenReturn(null);
            }
        }
    }


    /**
     * Configure the score cache for externally scored networks
     *
     * @param scoreCache   Wifi network score cache to be configured
     * @param scanDetails  a list of ScanDetail
     * @param scores       scores of the networks
     * @param meteredHints hints of if the networks are metered
     */
    public static void configureScoreCache(WifiNetworkScoreCache scoreCache,
            List<ScanDetail> scanDetails, Integer[] scores, boolean[] meteredHints) {
        List<ScoredNetwork> networks = new ArrayList<>();

        for (int i = 0; i < scanDetails.size(); i++) {
            ScanDetail scanDetail = scanDetails.get(i);
            ScanResult scanResult = scanDetail.getScanResult();
            WifiKey wifiKey = new WifiKey("\"" + scanResult.SSID + "\"", scanResult.BSSID);
            NetworkKey ntwkKey = new NetworkKey(wifiKey);
            RssiCurve rssiCurve;

            if (scores != null) { // fixed score
                byte rssiScore;
                Integer score = scores[i];

                if (scores[i] == null) {
                    rssiScore = WifiNetworkScoreCache.INVALID_NETWORK_SCORE;
                } else {
                    rssiScore = scores[i].byteValue();
                }
                rssiCurve = new RssiCurve(-100, 100, new byte[] {rssiScore});
            } else {
                rssiCurve = new RssiCurve(-80, 20, new byte[] {-10, 0, 10, 20, 30, 40});
            }
            ScoredNetwork scoredNetwork = new ScoredNetwork(ntwkKey, rssiCurve, meteredHints[i]);

            networks.add(scoredNetwork);
        }

        scoreCache.updateScores(networks);
    }

    /**
     * Setup WifiConfigManager mock for ephemeral networks.
     *
     * @param wifiConfigManager WifiConfigManager mock
     * @param networkId         ID of the ephemeral network
     * @param scanDetail        scanDetail of the ephemeral network
     * @param meteredHint       flag to indidate if the network has meteredHint
     */
    public static WifiConfiguration setupEphemeralNetwork(WifiConfigManager wifiConfigManager,
            int networkId, ScanDetail scanDetail, boolean meteredHint) {
        // Return the correct networkID for ephemeral network addition.
        when(wifiConfigManager.addOrUpdateNetwork(any(WifiConfiguration.class), anyInt()))
                .thenReturn(new NetworkUpdateResult(networkId));
        final WifiConfiguration config =
                ScanResultUtil.createNetworkFromScanResult(scanDetail.getScanResult());
        config.ephemeral = true;
        config.trusted = false;
        config.networkId = networkId;
        config.meteredHint = meteredHint;

        when(wifiConfigManager.getConfiguredNetworkForScanDetailAndCache(eq(scanDetail)))
                .thenReturn(new WifiConfiguration(config));
        when(wifiConfigManager.getConfiguredNetwork(eq(networkId)))
                .then(new AnswerWithArguments() {
                    public WifiConfiguration answer(int netId) {
                        return new WifiConfiguration(config);
                    }
                });
        when(wifiConfigManager.setNetworkCandidateScanResult(
                eq(networkId), any(ScanResult.class), anyInt()))
                .then(new AnswerWithArguments() {
                    public boolean answer(int netId, ScanResult scanResult, int score) {
                        config.getNetworkSelectionStatus().setCandidate(scanResult);
                        config.getNetworkSelectionStatus().setCandidateScore(score);
                        config.getNetworkSelectionStatus()
                                .setSeenInLastQualifiedNetworkSelection(true);
                        return true;
                    }
                });
        when(wifiConfigManager.updateNetworkSelectionStatus(eq(networkId),
                eq(WifiConfiguration.NetworkSelectionStatus.NETWORK_SELECTION_ENABLE)))
                .then(new AnswerWithArguments() {
                    public boolean answer(int netId, int status) {
                        config.getNetworkSelectionStatus().setNetworkSelectionStatus(
                                WifiConfiguration.NetworkSelectionStatus.NETWORK_SELECTION_ENABLE);
                        return true;
                    }
                });
        return config;
    }
}
