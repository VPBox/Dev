/*
 * Copyright (C) 2018 The Android Open Source Project
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
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.validateMockitoUsage;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiEnterpriseConfig;
import android.os.Process;
import android.telephony.TelephonyManager;
import android.util.LocalLog;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.util.ScanResultUtil;
import com.android.server.wifi.util.TelephonyUtil;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

import java.util.HashMap;
import java.util.List;
import java.util.Map;


/**
 * Unit tests for CarrierNeteworkEvaluator
 */
@SmallTest
public class CarrierNetworkEvaluatorTest {
    private static final String CARRIER1_SSID = "\"carrier1\"";
    private static final String CARRIER2_SSID = "\"carrier2\"";
    private static final String CARRIER_SAVED_SSID = "\"carrier3-saved\"";
    private static final String CARRIER_SAVED_EPH_SSID = "\"carrier4-saved-ephemeral\"";
    private static final String NON_CARRIER_SSID = "\"non-carrier\"";
    private static final String TEST_MCC_MNC = "123456";

    private static final int CARRIER1_NET_ID = 1;
    private static final int CARRIER2_NET_ID = 2;
    private static final int CARRIER_SAVED_NET_ID = 3;
    private static final int CARRIER_SAVED_EPH_NET_ID = 4;
    private static final int NON_CARRIER_NET_ID = 5;

    private CarrierNetworkEvaluator mDut;

    @Mock private WifiConfigManager mWifiConfigManager;
    @Mock private CarrierNetworkConfig mCarrierNetworkConfig;
    @Mock private LocalLog mLocalLog;
    @Mock private Clock mClock;
    @Mock private WifiNetworkSelector.NetworkEvaluator.OnConnectableListener mConnectableListener;
    @Mock private WifiInjector mWifiInjector;
    @Mock private TelephonyManager mTelephonyManager;
    @Mock private TelephonyManager mDataTelephonyManager;

    private ArgumentCaptor<ScanDetail> mScanDetailCaptor = ArgumentCaptor.forClass(
            ScanDetail.class);
    private ArgumentCaptor<WifiConfiguration> mWifiConfigCaptor = ArgumentCaptor.forClass(
            WifiConfiguration.class);

    private class GetConfiguredNetworkForScanDetailsAnswer implements Answer<WifiConfiguration> {
        private Map<String, WifiConfiguration> mConfig = new HashMap<>();

        public void addConfig(ScanDetail scanDetail, WifiConfiguration config) {
            mConfig.put(scanDetail.toKeyString(), config);
        }

        @Override
        public WifiConfiguration answer(InvocationOnMock invocation) throws Throwable {
            ScanDetail scanDetail = invocation.getArgument(0);
            return mConfig.get(scanDetail.toKeyString());
        }
    }

    private GetConfiguredNetworkForScanDetailsAnswer mGetConfiguredNetworkForScanDetailsAnswer;

    private class AddOrUpdateNetworkAnswer implements Answer<NetworkUpdateResult> {
        private Map<String, Integer> mConfigs = new HashMap<>();

        public void addConfig(WifiConfiguration config, int networkId) {
            mConfigs.put(config.configKey(), networkId);
        }

        @Override
        public NetworkUpdateResult answer(InvocationOnMock invocation) throws Throwable {
            WifiConfiguration config = invocation.getArgument(0);
            Integer networkId = mConfigs.get(config.configKey());
            if (networkId == null) return null;

            when(mWifiConfigManager.getConfiguredNetwork(networkId)).thenReturn(config);

            NetworkUpdateResult networkUpdateResult = mock(NetworkUpdateResult.class);
            when(networkUpdateResult.isSuccess()).thenReturn(true);
            when(networkUpdateResult.getNetworkId()).thenReturn(networkId);

            return networkUpdateResult;
        }
    }

    private AddOrUpdateNetworkAnswer mAddOrUpdateNetworkAnswer;

    private WifiConfiguration configureNewSsid(int networkId, ScanDetail scanDetail,
            boolean isEphemeral, boolean isSaved) {
        WifiConfiguration newConfig = ScanResultUtil.createNetworkFromScanResult(
                scanDetail.getScanResult());
        assertTrue("" + newConfig, WifiConfigurationUtil.validate(newConfig, true));
        assertEquals(ScanResultMatchInfo.fromScanResult(scanDetail.getScanResult()),
                ScanResultMatchInfo.fromWifiConfiguration(newConfig));
        newConfig.ephemeral = isEphemeral;

        if (isSaved) {
            mGetConfiguredNetworkForScanDetailsAnswer.addConfig(scanDetail, newConfig);
        }

        when(mWifiConfigManager.enableNetwork(networkId, false, Process.WIFI_UID)).thenReturn(true);
        when(mWifiConfigManager.setNetworkCandidateScanResult(eq(networkId), any(),
                anyInt())).thenReturn(true);
        when(mWifiConfigManager.getConfiguredNetwork(networkId)).thenReturn(newConfig);
        mAddOrUpdateNetworkAnswer.addConfig(newConfig, networkId);
        return newConfig;
    }

    /** Sets up test. */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);

        mDut = new CarrierNetworkEvaluator(mWifiConfigManager, mCarrierNetworkConfig, mLocalLog,
                mWifiInjector);

        when(mWifiInjector.makeTelephonyManager()).thenReturn(mTelephonyManager);
        when(mTelephonyManager.createForSubscriptionId(anyInt())).thenReturn(mDataTelephonyManager);
        when(mDataTelephonyManager.getSimOperator()).thenReturn(TEST_MCC_MNC);
        when(mDataTelephonyManager.getSimState()).thenReturn(TelephonyManager.SIM_STATE_READY);
        when(mCarrierNetworkConfig.isCarrierEncryptionInfoAvailable()).thenReturn(true);

        when(mCarrierNetworkConfig.isCarrierNetwork(eq(CARRIER1_SSID.replace("\"", ""))))
                .thenReturn(true);
        when(mCarrierNetworkConfig.isCarrierNetwork(eq(CARRIER2_SSID.replace("\"", ""))))
                .thenReturn(true);
        when(mCarrierNetworkConfig.isCarrierNetwork(eq(CARRIER_SAVED_SSID.replace("\"", ""))))
                .thenReturn(true);
        when(mCarrierNetworkConfig.isCarrierNetwork(eq(CARRIER_SAVED_EPH_SSID.replace("\"", ""))))
                .thenReturn(true);

        when(mCarrierNetworkConfig.getNetworkEapType(eq(CARRIER1_SSID.replace("\"", ""))))
                .thenReturn(WifiEnterpriseConfig.Eap.AKA);
        when(mCarrierNetworkConfig.getNetworkEapType(eq(CARRIER2_SSID.replace("\"", ""))))
                .thenReturn(WifiEnterpriseConfig.Eap.AKA_PRIME);
        when(mCarrierNetworkConfig.getNetworkEapType(eq(CARRIER_SAVED_SSID.replace("\"", ""))))
                .thenReturn(WifiEnterpriseConfig.Eap.SIM);
        when(mCarrierNetworkConfig.getNetworkEapType(eq(CARRIER_SAVED_EPH_SSID.replace("\"", ""))))
                .thenReturn(WifiEnterpriseConfig.Eap.AKA);

        mAddOrUpdateNetworkAnswer = new AddOrUpdateNetworkAnswer();
        when(mWifiConfigManager.addOrUpdateNetwork(any(), eq(Process.WIFI_UID))).thenAnswer(
                mAddOrUpdateNetworkAnswer);

        mGetConfiguredNetworkForScanDetailsAnswer = new GetConfiguredNetworkForScanDetailsAnswer();
        when(mWifiConfigManager.getConfiguredNetworkForScanDetail(any())).thenAnswer(
                mGetConfiguredNetworkForScanDetailsAnswer);
    }

    /** Cleans up test. */
    @After
    public void cleanup() {
        validateMockitoUsage();
    }

    /**
     * Baseline positive test case: carrier Wi-Fi enabled (have cert), present >1 Carrier networks
     * of varying RSSI, include some none carrier networks with even better RSSI and some saved
     * carrier networks (one of which is ephemeral).
     *
     * Desired behavior:
     * - all Carrier Wi-Fi (including all saved networks) as connectable
     * - best Carrier Wi-Fi (highest RSSI) as return value
     */
    @Test
    public void testSelectOneFromMultiple() {
        String[] ssids = {CARRIER1_SSID, CARRIER2_SSID, CARRIER_SAVED_SSID, CARRIER_SAVED_EPH_SSID,
                NON_CARRIER_SSID};
        String[] bssids = {"6c:f3:7f:ae:8c:f3", "6c:f3:7f:ae:8c:f4", "6c:f3:7f:ae:8c:f5",
                "6c:f3:7f:ae:8c:f6", "6c:f3:7f:ae:8c:f7"};
        int[] freqs = {2470, 2437, 2470, 2470, 2470};
        String[] caps = {"[WPA2-EAP-CCMP]", "[WPA2-EAP-CCMP]", "[WPA2-EAP-CCMP]",
                "[WPA2-EAP-CCMP]", "[]"};
        int[] levels = {10, 20, 11, 15, 50};

        List<ScanDetail> scanDetails = WifiNetworkSelectorTestUtil.buildScanDetails(ssids, bssids,
                freqs, caps, levels, mClock);
        configureNewSsid(CARRIER1_NET_ID, scanDetails.get(0), true, false);
        configureNewSsid(CARRIER2_NET_ID, scanDetails.get(1), true, false);
        configureNewSsid(CARRIER_SAVED_NET_ID, scanDetails.get(2), false, true);
        configureNewSsid(CARRIER_SAVED_EPH_NET_ID, scanDetails.get(3), true, false);
        configureNewSsid(NON_CARRIER_NET_ID, scanDetails.get(4), false, true);

        WifiConfiguration selected = mDut.evaluateNetworks(scanDetails, null, null, false, false,
                mConnectableListener);

        verify(mConnectableListener, times(4)).onConnectable(mScanDetailCaptor.capture(),
                mWifiConfigCaptor.capture(), anyInt());

        assertEquals(4, mScanDetailCaptor.getAllValues().size());
        assertEquals(CARRIER1_SSID.replace("\"", ""),
                mScanDetailCaptor.getAllValues().get(0).getSSID());
        assertEquals(CARRIER2_SSID.replace("\"", ""),
                mScanDetailCaptor.getAllValues().get(1).getSSID());
        assertEquals(CARRIER_SAVED_SSID.replace("\"", ""),
                mScanDetailCaptor.getAllValues().get(2).getSSID());
        assertEquals(CARRIER_SAVED_EPH_SSID.replace("\"", ""),
                mScanDetailCaptor.getAllValues().get(3).getSSID());

        assertEquals(4, mWifiConfigCaptor.getAllValues().size());
        WifiConfiguration config1 = mWifiConfigCaptor.getAllValues().get(0);
        assertEquals(CARRIER1_SSID, config1.SSID);
        assertTrue(config1.isEphemeral());
        assertTrue(config1.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.WPA_EAP));
        WifiConfiguration config2 = mWifiConfigCaptor.getAllValues().get(1);
        assertEquals(CARRIER2_SSID, config2.SSID);
        assertTrue(config2.isEphemeral());
        assertTrue(config2.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.WPA_EAP));
        WifiConfiguration config3 = mWifiConfigCaptor.getAllValues().get(2);
        assertEquals(CARRIER_SAVED_SSID, config3.SSID);
        // all configs returned by CarrierNetworkEvaluator are ephemeral.
        assertTrue(config3.isEphemeral());
        assertTrue(config3.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.WPA_EAP));
        WifiConfiguration config4 = mWifiConfigCaptor.getAllValues().get(3);
        assertEquals(CARRIER_SAVED_EPH_SSID, config4.SSID);
        assertTrue(config4.isEphemeral());
        assertTrue(config4.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.WPA_EAP));

        assertEquals(config2.configKey(), selected.configKey()); // SSID2 has the highest RSSI
        assertEquals("", selected.enterpriseConfig.getAnonymousIdentity());
        assertTrue(TelephonyUtil.isSimEapMethod(selected.enterpriseConfig.getEapMethod()));
    }

    /**
     * Cert installed and no Carrier Wi-Fi visible
     *
     * Desired behavior: no networks connectable or selected
     */
    @Test
    public void testSelectFromNoneAvailable() {
        String[] ssids = {NON_CARRIER_SSID};
        String[] bssids = {"6c:f3:7f:ae:8c:f6"};
        int[] freqs = {2470};
        String[] caps = {"[]"};
        int[] levels = {40};

        List<ScanDetail> scanDetails = WifiNetworkSelectorTestUtil.buildScanDetails(ssids, bssids,
                freqs, caps, levels, mClock);
        configureNewSsid(NON_CARRIER_NET_ID, scanDetails.get(0), false, true);

        WifiConfiguration selected = mDut.evaluateNetworks(scanDetails, null, null, false, false,
                mConnectableListener);

        verify(mConnectableListener, never()).onConnectable(any(), any(), anyInt());
        assertNull(selected);
    }

    /**
     * Multiple carrier Wi-Fi networks visible but no cert installed.
     *
     * Desired behavior: no networks connectable or selected
     */
    @Test
    public void testNoCarrierCert() {
        String[] ssids = {CARRIER1_SSID, CARRIER2_SSID, CARRIER_SAVED_SSID, CARRIER_SAVED_EPH_SSID,
                NON_CARRIER_SSID};
        String[] bssids = {"6c:f3:7f:ae:8c:f3", "6c:f3:7f:ae:8c:f4", "6c:f3:7f:ae:8c:f5",
                "6c:f3:7f:ae:8c:f6", "6c:f3:7f:ae:8c:f7"};
        int[] freqs = {2470, 2437, 2470, 2470, 2470};
        String[] caps = {"[WPA2-EAP-CCMP]", "[WPA2-EAP-CCMP]", "[WPA2-EAP-CCMP]",
                "[WPA2-EAP-CCMP]", "[]"};
        int[] levels = {10, 20, 30, 40, 50};

        when(mCarrierNetworkConfig.isCarrierEncryptionInfoAvailable()).thenReturn(false);

        List<ScanDetail> scanDetails = WifiNetworkSelectorTestUtil.buildScanDetails(ssids, bssids,
                freqs, caps, levels, mClock);
        configureNewSsid(CARRIER1_NET_ID, scanDetails.get(0), true, false);
        configureNewSsid(CARRIER2_NET_ID, scanDetails.get(1), true, false);
        configureNewSsid(CARRIER_SAVED_NET_ID, scanDetails.get(2), false, true);
        configureNewSsid(CARRIER_SAVED_EPH_NET_ID, scanDetails.get(3), true, false);
        configureNewSsid(NON_CARRIER_NET_ID, scanDetails.get(4), false, true);

        WifiConfiguration selected = mDut.evaluateNetworks(scanDetails, null, null, false, false,
                mConnectableListener);

        verify(mConnectableListener, never()).onConnectable(any(), any(), anyInt());
        assertNull(selected);
    }

    /**
     * One carrier Wi-Fi networks visible and cert installed but user has previously forgotten the
     * network.
     *
     * Desired behavior: no networks connectable or selected
     */
    @Test
    public void testAvailableButPreviouslyUserDeleted() {
        String[] ssids = {CARRIER1_SSID};
        String[] bssids = {"6c:f3:7f:ae:8c:f3"};
        int[] freqs = {2470};
        String[] caps = {"[WPA2-EAP-CCMP]"};
        int[] levels = {10};

        when(mCarrierNetworkConfig.isCarrierEncryptionInfoAvailable()).thenReturn(true);
        when(mWifiConfigManager.wasEphemeralNetworkDeleted(CARRIER1_SSID))
                .thenReturn(true);

        List<ScanDetail> scanDetails = WifiNetworkSelectorTestUtil.buildScanDetails(ssids, bssids,
                freqs, caps, levels, mClock);
        configureNewSsid(CARRIER1_NET_ID, scanDetails.get(0), true, false);

        WifiConfiguration selected = mDut.evaluateNetworks(scanDetails, null, null, false, false,
                mConnectableListener);

        verify(mConnectableListener, never()).onConnectable(any(), any(), anyInt());
        assertNull(selected);
    }

    /**
     * One carrier Wi-Fi networks visible and cert installed but ssid is blacklisted.
     *
     * Desired behavior: no networks connectable or selected
     */
    @Test
    public void testAvailableButBlacklisted() {
        String[] ssids = {CARRIER1_SSID};
        String[] bssids = {"6c:f3:7f:ae:8c:f3"};
        int[] freqs = {2470};
        String[] caps = {"[WPA2-EAP-CCMP]"};
        int[] levels = {10};

        when(mCarrierNetworkConfig.isCarrierEncryptionInfoAvailable()).thenReturn(true);

        List<ScanDetail> scanDetails = WifiNetworkSelectorTestUtil.buildScanDetails(ssids, bssids,
                freqs, caps, levels, mClock);
        WifiConfiguration blacklisted =
                configureNewSsid(CARRIER1_NET_ID, scanDetails.get(0), true, false);
        blacklisted.getNetworkSelectionStatus()
                .setNetworkSelectionStatus(
                        WifiConfiguration.NetworkSelectionStatus
                                .NETWORK_SELECTION_PERMANENTLY_DISABLED);
        when(mWifiConfigManager.getConfiguredNetwork(eq(blacklisted.configKey())))
                .thenReturn(blacklisted);
        when(mWifiConfigManager.tryEnableNetwork(CARRIER1_NET_ID))
                .thenReturn(false);

        WifiConfiguration selected = mDut.evaluateNetworks(scanDetails, null, null, false, false,
                mConnectableListener);
        verify(mWifiConfigManager).getConfiguredNetwork(eq(blacklisted.configKey()));

        verify(mConnectableListener, never()).onConnectable(any(), any(), anyInt());
        assertNull(selected);
    }

    /**
     * One carrier Wi-Fi network that is visible and supports encrypted IMSI.
     *
     * Desired behavior: selected network supports encrypted IMSI by using EAP-SIM/AKA/AKA'
     * and has an empty anonymous identity. The anonymous identity will be populated with
     * {@code anonymous@<realm>} by ClientModeImpl's handling of the
     * {@link ClientModeImpl#CMD_START_CONNECT} event.
     */
    @Test
    public void testSupportsEncryptedImsi() {
        String[] ssids = {CARRIER1_SSID};
        String[] bssids = {"6c:f3:7f:ae:8c:f3"};
        int[] freqs = {2470};
        String[] caps = {"[WPA2-EAP-CCMP]"};
        int[] levels = {10};
        when(mCarrierNetworkConfig.isCarrierEncryptionInfoAvailable()).thenReturn(true);
        List<ScanDetail> scanDetails = WifiNetworkSelectorTestUtil.buildScanDetails(ssids, bssids,
                freqs, caps, levels, mClock);
        WifiConfiguration carrierConfig = configureNewSsid(CARRIER1_NET_ID, scanDetails.get(0),
                true, false);

        WifiConfiguration selected = mDut.evaluateNetworks(scanDetails, null, null, false, false,
                mConnectableListener);

        assertEquals(carrierConfig.configKey(), selected.configKey());
        assertEquals("", selected.enterpriseConfig.getAnonymousIdentity());
        assertTrue(TelephonyUtil.isSimEapMethod(selected.enterpriseConfig.getEapMethod()));
    }
}
