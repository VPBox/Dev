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

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.anyBoolean;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyString;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.app.test.MockAnswerUtil.AnswerWithArguments;
import android.content.Context;
import android.hardware.wifi.supplicant.V1_0.ISupplicantNetwork;
import android.hardware.wifi.supplicant.V1_0.ISupplicantStaNetwork;
import android.hardware.wifi.supplicant.V1_0.ISupplicantStaNetworkCallback;
import android.hardware.wifi.supplicant.V1_0.ISupplicantStaNetworkCallback.NetworkRequestEapSimGsmAuthParams;
import android.hardware.wifi.supplicant.V1_0.ISupplicantStaNetworkCallback.NetworkRequestEapSimUmtsAuthParams;
import android.hardware.wifi.supplicant.V1_0.SupplicantStatus;
import android.hardware.wifi.supplicant.V1_0.SupplicantStatusCode;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiEnterpriseConfig;
import android.os.RemoteException;
import android.text.TextUtils;

import androidx.test.filters.SmallTest;

import com.android.internal.R;
import com.android.server.wifi.util.NativeUtil;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;

/**
 * Unit tests for SupplicantStaNetworkHal
 */
@SmallTest
public class SupplicantStaNetworkHalTest {
    private static final String IFACE_NAME = "wlan0";
    private static final Map<String, String> NETWORK_EXTRAS_VALUES = new HashMap<>();
    static {
        NETWORK_EXTRAS_VALUES.put("key1", "value1");
        NETWORK_EXTRAS_VALUES.put("key2", "value2");
    }
    private static final String NETWORK_EXTRAS_SERIALIZED =
            "%7B%22key1%22%3A%22value1%22%2C%22key2%22%3A%22value2%22%7D";
    private static final String ANONYMOUS_IDENTITY = "aaa@bbb.cc.ddd";

    private SupplicantStaNetworkHal mSupplicantNetwork;
    private SupplicantStatus mStatusSuccess;
    private SupplicantStatus mStatusFailure;
    @Mock private ISupplicantStaNetwork mISupplicantStaNetworkMock;
    @Mock
    private android.hardware.wifi.supplicant.V1_2.ISupplicantStaNetwork mISupplicantStaNetworkV12;
    @Mock private Context mContext;
    @Mock private WifiMonitor mWifiMonitor;

    private SupplicantNetworkVariables mSupplicantVariables;
    private MockResources mResources;
    private ISupplicantStaNetworkCallback mISupplicantStaNetworkCallback;

    /**
     * Spy used to return the V1_2 ISupplicantStaNetwork mock object to simulate the 1.2 HAL running
     * on the device.
     */
    private class SupplicantStaNetworkHalSpyV1_2 extends SupplicantStaNetworkHal {
        SupplicantStaNetworkHalSpyV1_2(ISupplicantStaNetwork iSupplicantStaNetwork,
                String ifaceName,
                Context context, WifiMonitor monitor) {
            super(iSupplicantStaNetwork, ifaceName, context, monitor);
        }

        @Override
        protected android.hardware.wifi.supplicant.V1_2.ISupplicantStaNetwork
                getSupplicantStaNetworkForV1_2Mockable() {
            return mISupplicantStaNetworkV12;
        }
    }

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mStatusSuccess = createSupplicantStatus(SupplicantStatusCode.SUCCESS);
        mStatusFailure = createSupplicantStatus(SupplicantStatusCode.FAILURE_UNKNOWN);
        mSupplicantVariables = new SupplicantNetworkVariables();
        setupISupplicantNetworkMock();

        mResources = new MockResources();
        when(mContext.getResources()).thenReturn(mResources);
        createSupplicantStaNetwork();
    }

    /**
     * Tests the saving of WifiConfiguration to wpa_supplicant.
     */
    @Test
    public void testOweNetworkWifiConfigurationSaveLoad() throws Exception {
        // Now expose the V1.2 ISupplicantStaNetwork
        mSupplicantNetwork = new SupplicantStaNetworkHalSpyV1_2(mISupplicantStaNetworkMock,
                IFACE_NAME, mContext, mWifiMonitor);

        WifiConfiguration config = WifiConfigurationTestUtil.createOweNetwork();
        config.updateIdentifier = "46";
        testWifiConfigurationSaveLoad(config);
    }

    /**
     * Tests the saving of WifiConfiguration to wpa_supplicant.
     */
    @Test
    public void testOpenNetworkWifiConfigurationSaveLoad() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createOpenHiddenNetwork();
        config.updateIdentifier = "45";
        testWifiConfigurationSaveLoad(config);
    }

    /**
     * Tests the saving/loading of WifiConfiguration to wpa_supplicant with SAE password.
     */
    @Test
    public void testSaePasswordNetworkWifiConfigurationSaveLoad() throws Exception {
        // Now expose the V1.2 ISupplicantStaNetwork
        mSupplicantNetwork = new SupplicantStaNetworkHalSpyV1_2(mISupplicantStaNetworkMock,
                IFACE_NAME, mContext, mWifiMonitor);

        WifiConfiguration config = WifiConfigurationTestUtil.createSaeNetwork();
        testWifiConfigurationSaveLoad(config);
        verify(mISupplicantStaNetworkV12).setSaePassword(any(String.class));
        verify(mISupplicantStaNetworkV12, never())
                .getSaePassword(any(android.hardware.wifi.supplicant.V1_2.ISupplicantStaNetwork
                        .getSaePasswordCallback.class));
        verify(mISupplicantStaNetworkV12, never())
                .getPskPassphrase(any(ISupplicantStaNetwork.getPskPassphraseCallback.class));
        verify(mISupplicantStaNetworkV12, never()).setPsk(any(byte[].class));
        verify(mISupplicantStaNetworkV12, never())
                .getPsk(any(ISupplicantStaNetwork.getPskCallback.class));
    }

    /**
     * Tests the saving/loading of WifiConfiguration to wpa_supplicant with psk passphrase.
     */
    @Test
    public void testPskPassphraseNetworkWifiConfigurationSaveLoad() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createPskNetwork();
        config.requirePMF = true;
        testWifiConfigurationSaveLoad(config);
        verify(mISupplicantStaNetworkMock).setPskPassphrase(anyString());
        verify(mISupplicantStaNetworkMock)
                .getPskPassphrase(any(ISupplicantStaNetwork.getPskPassphraseCallback.class));
        verify(mISupplicantStaNetworkMock, never()).setPsk(any(byte[].class));
        verify(mISupplicantStaNetworkMock, never())
                .getPsk(any(ISupplicantStaNetwork.getPskCallback.class));
    }

    /**
     * Tests the saving/loading of WifiConfiguration to wpa_supplicant with raw psk.
     */
    @Test
    public void testPskNetworkWifiConfigurationSaveLoad() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createPskNetwork();
        config.preSharedKey = "945ef00c463c2a7c2496376b13263d1531366b46377179a4b17b393687450779";
        testWifiConfigurationSaveLoad(config);
        verify(mISupplicantStaNetworkMock).setPsk(any(byte[].class));
        verify(mISupplicantStaNetworkMock)
                .getPsk(any(ISupplicantStaNetwork.getPskCallback.class));
        verify(mISupplicantStaNetworkMock, never()).setPskPassphrase(anyString());
        verify(mISupplicantStaNetworkMock)
                .getPskPassphrase(any(ISupplicantStaNetwork.getPskPassphraseCallback.class));
    }

    /**
     * Tests the saving of WifiConfiguration to wpa_supplicant removes enclosing quotes of psk
     * passphrase
     */
    @Test
    public void testPskNetworkWifiConfigurationSaveRemovesPskQuotes() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createPskNetwork();
        config.preSharedKey = "\"quoted_psd\"";
        assertTrue(mSupplicantNetwork.saveWifiConfiguration(config));
        assertEquals(mSupplicantVariables.pskPassphrase,
                NativeUtil.removeEnclosingQuotes(config.preSharedKey));
    }

    /**
     * Tests the saving/loading of WifiConfiguration to wpa_supplicant.
     */
    @Test
    public void testWepNetworkWifiConfigurationSaveLoad() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createWepHiddenNetwork();
        config.BSSID = " *NOT USED* "; // we want the other bssid!
        config.getNetworkSelectionStatus().setNetworkSelectionBSSID("34:45:19:09:45:66");
        testWifiConfigurationSaveLoad(config);
    }

    /**
     * Tests the saving of WifiConfiguration to wpa_supplicant.
     */
    @Test
    public void testEapPeapGtcNetworkWifiConfigurationSaveLoad() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createEapNetwork();
        config.enterpriseConfig =
                WifiConfigurationTestUtil.createPEAPWifiEnterpriseConfigWithGTCPhase2();
        testWifiConfigurationSaveLoad(config);
    }

    /**
     * Tests the saving of WifiConfiguration to wpa_supplicant.
     */
    @Test
    public void testEapTlsNoneNetworkWifiConfigurationSaveLoad() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createEapNetwork();
        config.enterpriseConfig =
                WifiConfigurationTestUtil.createTLSWifiEnterpriseConfigWithNonePhase2();
        testWifiConfigurationSaveLoad(config);
    }

    /**
     * Tests the saving of WifiConfiguration to wpa_supplicant.
     */
    @Test
    public void testEapTlsNoneClientCertNetworkWifiConfigurationSaveLoad() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createEapNetwork();
        config.enterpriseConfig =
                WifiConfigurationTestUtil.createTLSWifiEnterpriseConfigWithNonePhase2();
        config.enterpriseConfig.setClientCertificateAlias("test_alias");
        testWifiConfigurationSaveLoad(config);
    }

    /**
     * Tests the saving of WifiConfiguration to wpa_supplicant.
     */
    @Test
    public void testEapTlsAkaNetworkWifiConfigurationSaveLoad() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createEapNetwork();
        config.enterpriseConfig =
                WifiConfigurationTestUtil.createTLSWifiEnterpriseConfigWithAkaPhase2();
        testWifiConfigurationSaveLoad(config);
    }

    /**
     * Tests the saving/loading of WifiConfiguration to wpa_supplicant with Suite-B-192
     */
    @Test
    public void testEapSuiteBRsaNetworkWifiConfigurationSaveLoad() throws Exception {
        // Now expose the V1.2 ISupplicantStaNetwork
        mSupplicantNetwork = new SupplicantStaNetworkHalSpyV1_2(mISupplicantStaNetworkMock,
                IFACE_NAME, mContext, mWifiMonitor);

        WifiConfiguration config = WifiConfigurationTestUtil.createEapSuiteBNetwork();
        config.allowedSuiteBCiphers.set(WifiConfiguration.SuiteBCipher.ECDHE_RSA);

        testWifiConfigurationSaveLoad(config);
        verify(mISupplicantStaNetworkV12, never()).enableSuiteBEapOpenSslCiphers();
        verify(mISupplicantStaNetworkV12).enableTlsSuiteBEapPhase1Param(anyBoolean());

        verify(mISupplicantStaNetworkV12, never()).setSaePassword(any(String.class));
        verify(mISupplicantStaNetworkV12, never())
                .getSaePassword(any(android.hardware.wifi.supplicant.V1_2.ISupplicantStaNetwork
                        .getSaePasswordCallback.class));
        verify(mISupplicantStaNetworkV12, never())
                .getPskPassphrase(any(ISupplicantStaNetwork.getPskPassphraseCallback.class));
        verify(mISupplicantStaNetworkV12, never()).setPsk(any(byte[].class));
        verify(mISupplicantStaNetworkV12, never())
                .getPsk(any(ISupplicantStaNetwork.getPskCallback.class));
    }

    /**
     * Tests the saving/loading of WifiConfiguration to wpa_supplicant with Suite-B-192
     */
    @Test
    public void testEapSuiteBEcdsaNetworkWifiConfigurationSaveLoad() throws Exception {
        // Now expose the V1.2 ISupplicantStaNetwork
        mSupplicantNetwork = new SupplicantStaNetworkHalSpyV1_2(mISupplicantStaNetworkMock,
                IFACE_NAME, mContext, mWifiMonitor);

        WifiConfiguration config = WifiConfigurationTestUtil.createEapSuiteBNetwork();
        config.allowedSuiteBCiphers.set(WifiConfiguration.SuiteBCipher.ECDHE_ECDSA);

        testWifiConfigurationSaveLoad(config);
        verify(mISupplicantStaNetworkV12).enableSuiteBEapOpenSslCiphers();
        verify(mISupplicantStaNetworkV12, never())
                .enableTlsSuiteBEapPhase1Param(any(boolean.class));

        verify(mISupplicantStaNetworkV12, never()).setSaePassword(any(String.class));
        verify(mISupplicantStaNetworkV12, never())
                .getSaePassword(any(android.hardware.wifi.supplicant.V1_2.ISupplicantStaNetwork
                        .getSaePasswordCallback.class));
        verify(mISupplicantStaNetworkV12, never())
                .getPskPassphrase(any(ISupplicantStaNetwork.getPskPassphraseCallback.class));
        verify(mISupplicantStaNetworkV12, never()).setPsk(any(byte[].class));
        verify(mISupplicantStaNetworkV12, never())
                .getPsk(any(ISupplicantStaNetwork.getPskCallback.class));
    }

    /**
     * Tests the loading of network ID.
     */
    @Test
    public void testNetworkIdLoad() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createWepHiddenNetwork();
        assertTrue(mSupplicantNetwork.saveWifiConfiguration(config));

        // Modify the supplicant variable directly.
        mSupplicantVariables.networkId = 5;
        WifiConfiguration loadConfig = new WifiConfiguration();
        Map<String, String> networkExtras = new HashMap<>();
        assertTrue(mSupplicantNetwork.loadWifiConfiguration(loadConfig, networkExtras));
        assertEquals(mSupplicantVariables.networkId, loadConfig.networkId);
    }

    /**
     * Tests the failure to load ssid aborts the loading of network variables.
     */
    @Test
    public void testSsidLoadFailure() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createWepHiddenNetwork();
        assertTrue(mSupplicantNetwork.saveWifiConfiguration(config));

        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getSsidCallback cb) throws RemoteException {
                cb.onValues(mStatusFailure, mSupplicantVariables.ssid);
            }
        }).when(mISupplicantStaNetworkMock)
                .getSsid(any(ISupplicantStaNetwork.getSsidCallback.class));

        WifiConfiguration loadConfig = new WifiConfiguration();
        Map<String, String> networkExtras = new HashMap<>();
        assertFalse(mSupplicantNetwork.loadWifiConfiguration(loadConfig, networkExtras));
    }

    /**
     * Tests the failure to save ssid.
     */
    @Test
    public void testSsidSaveFailure() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createWepHiddenNetwork();

        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(ArrayList<Byte> ssid) throws RemoteException {
                return mStatusFailure;
            }
        }).when(mISupplicantStaNetworkMock).setSsid(any(ArrayList.class));

        assertFalse(mSupplicantNetwork.saveWifiConfiguration(config));
    }

    /**
     * Tests the failure to save invalid key mgmt (unknown bit set in the
     * {@link WifiConfiguration#allowedKeyManagement} being saved).
     */
    @Test
    public void testInvalidKeyMgmtSaveFailure() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createWepHiddenNetwork();
        config.allowedKeyManagement.set(20);
        try {
            assertFalse(mSupplicantNetwork.saveWifiConfiguration(config));
        } catch (IllegalArgumentException e) {
            return;
        }
        assertTrue(false);
    }

    /**
     * Tests the failure to load invalid key mgmt (unknown bit set in the
     * supplicant network key_mgmt variable read).
     */
    @Test
    public void testInvalidKeyMgmtLoadFailure() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createWepHiddenNetwork();
        assertTrue(mSupplicantNetwork.saveWifiConfiguration(config));

        // Modify the supplicant variable directly.
        mSupplicantVariables.keyMgmtMask = 0xFFFFF;
        WifiConfiguration loadConfig = new WifiConfiguration();
        Map<String, String> networkExtras = new HashMap<>();
        try {
            assertFalse(mSupplicantNetwork.loadWifiConfiguration(loadConfig, networkExtras));
        } catch (IllegalArgumentException e) {
            return;
        }
        assertTrue(false);
    }

    /**
     * Tests the failure to save invalid bssid (less than 6 bytes in the
     * {@link WifiConfiguration#BSSID} being saved).
     */
    @Test
    public void testInvalidBssidSaveFailure() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createWepHiddenNetwork();
        config.getNetworkSelectionStatus().setNetworkSelectionBSSID("45:34:23:12");
        try {
            assertFalse(mSupplicantNetwork.saveWifiConfiguration(config));
        } catch (IllegalArgumentException e) {
            return;
        }
        assertTrue(false);
    }

    /**
     * Tests the failure to load invalid bssid (less than 6 bytes in the supplicant bssid variable
     * read).
     */
    @Test
    public void testInvalidBssidLoadFailure() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createWepHiddenNetwork();
        assertTrue(mSupplicantNetwork.saveWifiConfiguration(config));

        // Modify the supplicant variable directly.
        mSupplicantVariables.bssid = new byte[3];
        WifiConfiguration loadConfig = new WifiConfiguration();
        Map<String, String> networkExtras = new HashMap<>();
        try {
            assertFalse(mSupplicantNetwork.loadWifiConfiguration(loadConfig, networkExtras));
        } catch (IllegalArgumentException e) {
            return;
        }
        assertTrue(false);
    }

    /**
     * Tests the loading of invalid ssid from wpa_supplicant.
     */
    @Test
    public void testInvalidSsidLoadFailure() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createWepHiddenNetwork();
        assertTrue(mSupplicantNetwork.saveWifiConfiguration(config));

        // Modify the supplicant variable directly.
        mSupplicantVariables.ssid = null;

        WifiConfiguration loadConfig = new WifiConfiguration();
        Map<String, String> networkExtras = new HashMap<>();
        assertFalse(mSupplicantNetwork.loadWifiConfiguration(loadConfig, networkExtras));
    }

    /**
     * Tests the loading of invalid eap method from wpa_supplicant.
     * Invalid eap method is assumed to be a non enterprise network. So, the loading should
     * succeed as a non-enterprise network.
     */
    @Test
    public void testInvalidEapMethodLoadFailure() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createEapNetwork();
        config.enterpriseConfig =
                WifiConfigurationTestUtil.createPEAPWifiEnterpriseConfigWithGTCPhase2();
        assertTrue(mSupplicantNetwork.saveWifiConfiguration(config));

        // Modify the supplicant variable directly.
        mSupplicantVariables.eapMethod = -1;

        WifiConfiguration loadConfig = new WifiConfiguration();
        Map<String, String> networkExtras = new HashMap<>();
        assertTrue(mSupplicantNetwork.loadWifiConfiguration(loadConfig, networkExtras));
    }

    /**
     * Tests the loading of invalid eap phase2 method from wpa_supplicant.
     */
    @Test
    public void testInvalidEapPhase2MethodLoadFailure() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createEapNetwork();
        config.enterpriseConfig =
                WifiConfigurationTestUtil.createPEAPWifiEnterpriseConfigWithGTCPhase2();
        assertTrue(mSupplicantNetwork.saveWifiConfiguration(config));

        // Modify the supplicant variable directly.
        mSupplicantVariables.eapPhase2Method = -1;

        WifiConfiguration loadConfig = new WifiConfiguration();
        Map<String, String> networkExtras = new HashMap<>();
        assertFalse(mSupplicantNetwork.loadWifiConfiguration(loadConfig, networkExtras));
    }

    /**
     * Tests the parsing of GSM auth response parameters.
     */
    @Test
    public void testSendNetworkEapSimGsmAuthResponseWith2KcSresPair() throws Exception {
        final byte[] kc = new byte[]{0x45, 0x45, 0x32, 0x34, 0x45, 0x10, 0x34, 0x12};
        final byte[] sres = new byte[]{0x12, 0x10, 0x32, 0x23};
        // Send 2 kc/sres pair for this request.
        String paramsStr = ":" + NativeUtil.hexStringFromByteArray(kc)
                + ":" + NativeUtil.hexStringFromByteArray(sres)
                + ":" + NativeUtil.hexStringFromByteArray(kc)
                + ":" + NativeUtil.hexStringFromByteArray(sres);

        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(
                    ArrayList<ISupplicantStaNetwork.NetworkResponseEapSimGsmAuthParams> params)
                    throws RemoteException {
                assertEquals(2, params.size());
                assertArrayEquals(kc, params.get(0).kc);
                assertArrayEquals(sres, params.get(0).sres);
                assertArrayEquals(kc, params.get(1).kc);
                assertArrayEquals(sres, params.get(1).sres);
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).sendNetworkEapSimGsmAuthResponse(any(ArrayList.class));

        assertTrue(mSupplicantNetwork.sendNetworkEapSimGsmAuthResponse(paramsStr));
    }

    /**
     * Tests the parsing of GSM auth response parameters.
     */
    @Test
    public void testSendNetworkEapSimGsmAuthResponseWith3KcSresPair() throws Exception {
        final byte[] kc1 = new byte[]{0x45, 0x45, 0x32, 0x34, 0x45, 0x10, 0x34, 0x12};
        final byte[] sres1 = new byte[]{0x12, 0x10, 0x32, 0x23};
        final byte[] kc2 = new byte[]{0x45, 0x34, 0x12, 0x34, 0x45, 0x10, 0x34, 0x12};
        final byte[] sres2 = new byte[]{0x12, 0x23, 0x12, 0x23};
        final byte[] kc3 = new byte[]{0x25, 0x34, 0x12, 0x14, 0x45, 0x10, 0x34, 0x12};
        final byte[] sres3 = new byte[]{0x42, 0x23, 0x22, 0x23};
        // Send 3 kc/sres pair for this request.
        String paramsStr = ":" + NativeUtil.hexStringFromByteArray(kc1)
                + ":" + NativeUtil.hexStringFromByteArray(sres1)
                + ":" + NativeUtil.hexStringFromByteArray(kc2)
                + ":" + NativeUtil.hexStringFromByteArray(sres2)
                + ":" + NativeUtil.hexStringFromByteArray(kc3)
                + ":" + NativeUtil.hexStringFromByteArray(sres3);

        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(
                    ArrayList<ISupplicantStaNetwork.NetworkResponseEapSimGsmAuthParams> params)
                    throws RemoteException {
                assertEquals(3, params.size());
                assertArrayEquals(kc1, params.get(0).kc);
                assertArrayEquals(sres1, params.get(0).sres);
                assertArrayEquals(kc2, params.get(1).kc);
                assertArrayEquals(sres2, params.get(1).sres);
                assertArrayEquals(kc3, params.get(2).kc);
                assertArrayEquals(sres3, params.get(2).sres);
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).sendNetworkEapSimGsmAuthResponse(any(ArrayList.class));

        assertTrue(mSupplicantNetwork.sendNetworkEapSimGsmAuthResponse(paramsStr));
    }

    /**
     * Tests the parsing of invalid GSM auth response parameters (invalid kc & sres lengths).
     */
    @Test
    public void testSendInvalidKcSresLenNetworkEapSimGsmAuthResponse() throws Exception {
        final byte[] kc1 = new byte[]{0x45, 0x45, 0x32, 0x34, 0x45, 0x10, 0x34};
        final byte[] sres1 = new byte[]{0x12, 0x10, 0x23};
        final byte[] kc2 = new byte[]{0x45, 0x34, 0x12, 0x34, 0x45, 0x10, 0x34, 0x12};
        final byte[] sres2 = new byte[]{0x12, 0x23, 0x12, 0x23};
        // Send 2 kc/sres pair for this request.
        String paramsStr = ":" + NativeUtil.hexStringFromByteArray(kc1)
                + ":" + NativeUtil.hexStringFromByteArray(sres1)
                + ":" + NativeUtil.hexStringFromByteArray(kc2)
                + ":" + NativeUtil.hexStringFromByteArray(sres2);

        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(
                    ArrayList<ISupplicantStaNetwork.NetworkResponseEapSimGsmAuthParams> params)
                    throws RemoteException {
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).sendNetworkEapSimGsmAuthResponse(any(ArrayList.class));

        assertFalse(mSupplicantNetwork.sendNetworkEapSimGsmAuthResponse(paramsStr));
    }

    /**
     * Tests the parsing of invalid GSM auth response parameters (invalid number of kc/sres pairs).
     */
    @Test
    public void testSendInvalidKcSresPairNumNetworkEapSimGsmAuthResponse() throws Exception {
        final byte[] kc = new byte[]{0x45, 0x34, 0x12, 0x34, 0x45, 0x10, 0x34, 0x12};
        final byte[] sres = new byte[]{0x12, 0x23, 0x12, 0x23};
        // Send 1 kc/sres pair for this request.
        String paramsStr = ":" + NativeUtil.hexStringFromByteArray(kc)
                + ":" + NativeUtil.hexStringFromByteArray(sres);

        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(
                    ArrayList<ISupplicantStaNetwork.NetworkResponseEapSimGsmAuthParams> params)
                    throws RemoteException {
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).sendNetworkEapSimGsmAuthResponse(any(ArrayList.class));

        assertFalse(mSupplicantNetwork.sendNetworkEapSimGsmAuthResponse(paramsStr));
    }

    /**
     * Tests the parsing of UMTS auth response parameters.
     */
    @Test
    public void testSendNetworkEapSimUmtsAuthResponse() throws Exception {
        final byte[] ik = new byte[]{0x45, 0x45, 0x32, 0x34, 0x45, 0x10, 0x34, 0x12, 0x23, 0x34,
                0x33, 0x23, 0x34, 0x10, 0x40, 0x34};
        final byte[] ck = new byte[]{0x12, 0x10, 0x32, 0x23, 0x45, 0x10, 0x34, 0x12, 0x23, 0x34,
                0x33, 0x23, 0x34, 0x10, 0x40, 0x34};
        final byte[] res = new byte[]{0x12, 0x10, 0x32, 0x23, 0x45, 0x10, 0x34, 0x12, 0x23, 0x34};
        String paramsStr = ":" + NativeUtil.hexStringFromByteArray(ik)
                + ":" + NativeUtil.hexStringFromByteArray(ck)
                + ":" + NativeUtil.hexStringFromByteArray(res);

        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(
                    ISupplicantStaNetwork.NetworkResponseEapSimUmtsAuthParams params)
                    throws RemoteException {
                assertArrayEquals(ik, params.ik);
                assertArrayEquals(ck, params.ck);
                // Convert to arraylist before comparison.
                ArrayList<Byte> resList = new ArrayList<>();
                for (byte b : res) {
                    resList.add(b);
                }
                assertEquals(resList, params.res);
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).sendNetworkEapSimUmtsAuthResponse(
                any(ISupplicantStaNetwork.NetworkResponseEapSimUmtsAuthParams.class));

        assertTrue(mSupplicantNetwork.sendNetworkEapSimUmtsAuthResponse(paramsStr));
    }

    /**
     * Tests the parsing of invalid UMTS auth response parameters (invalid ik, ck lengths).
     */
    @Test
    public void testSendInvalidNetworkEapSimUmtsAuthResponse() throws Exception {
        final byte[] ik = new byte[]{0x45, 0x45, 0x32, 0x34, 0x45, 0x10, 0x34, 0x12};
        final byte[] ck = new byte[]{0x12, 0x10, 0x32, 0x23, 0x45, 0x10, 0x34, 0x12, 0x23, 0x34,
                0x33, 0x23, 0x34, 0x10, 0x40};
        final byte[] res = new byte[]{0x12, 0x10, 0x32, 0x23, 0x45, 0x10, 0x34, 0x12, 0x23, 0x34};
        String paramsStr = ":" + NativeUtil.hexStringFromByteArray(ik)
                + ":" + NativeUtil.hexStringFromByteArray(ck)
                + ":" + NativeUtil.hexStringFromByteArray(res);

        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(
                    ISupplicantStaNetwork.NetworkResponseEapSimUmtsAuthParams params)
                    throws RemoteException {
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).sendNetworkEapSimUmtsAuthResponse(
                any(ISupplicantStaNetwork.NetworkResponseEapSimUmtsAuthParams.class));

        assertFalse(mSupplicantNetwork.sendNetworkEapSimUmtsAuthResponse(paramsStr));
    }

    /**
     * Tests the parsing of UMTS auts response parameters.
     */
    @Test
    public void testSendNetworkEapSimUmtsAutsResponse() throws Exception {
        final byte[] auts = new byte[]{0x45, 0x45, 0x32, 0x34, 0x45, 0x10, 0x34, 0x12, 0x23, 0x34,
                0x33, 0x23, 0x34, 0x10};
        String paramsStr = ":" + NativeUtil.hexStringFromByteArray(auts);

        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(byte[] params)
                    throws RemoteException {
                assertArrayEquals(auts, params);
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).sendNetworkEapSimUmtsAutsResponse(any(byte[].class));

        assertTrue(mSupplicantNetwork.sendNetworkEapSimUmtsAutsResponse(paramsStr));
    }

    /**
     * Tests the parsing of invalid UMTS auts response parameters (invalid auts length).
     */
    @Test
    public void testSendInvalidNetworkEapSimUmtsAutsResponse() throws Exception {
        final byte[] auts = new byte[]{0x45, 0x45, 0x32, 0x34, 0x45, 0x10, 0x34, 0x12, 0x23};
        String paramsStr = ":" + NativeUtil.hexStringFromByteArray(auts);

        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(byte[] params)
                    throws RemoteException {
                assertArrayEquals(auts, params);
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).sendNetworkEapSimUmtsAutsResponse(any(byte[].class));

        assertFalse(mSupplicantNetwork.sendNetworkEapSimUmtsAutsResponse(paramsStr));
    }

    /**
     * Tests the parsing of identity string.
     */
    @Test
    public void testSendNetworkEapIdentityResponse() throws Exception {
        final String identityStr = "test@test.com";
        final String encryptedIdentityStr = "test2@test.com";
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(ArrayList<Byte> identity)
                    throws RemoteException {
                assertEquals(identityStr, NativeUtil.stringFromByteArrayList(identity));
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).sendNetworkEapIdentityResponse(any(ArrayList.class));

        assertTrue(mSupplicantNetwork.sendNetworkEapIdentityResponse(identityStr,
                encryptedIdentityStr));
        verify(mISupplicantStaNetworkV12, never()).sendNetworkEapIdentityResponse_1_1(
                any(ArrayList.class), any(ArrayList.class));

        // Now expose the V1.2 ISupplicantStaNetwork
        mSupplicantNetwork = new SupplicantStaNetworkHalSpyV1_2(mISupplicantStaNetworkMock,
                IFACE_NAME, mContext, mWifiMonitor);
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(ArrayList<Byte> identity,
                    ArrayList<Byte> encryptedIdentity)
                    throws RemoteException {
                assertEquals(identityStr, NativeUtil.stringFromByteArrayList(identity));
                assertEquals(encryptedIdentityStr,
                        NativeUtil.stringFromByteArrayList(encryptedIdentity));
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkV12).sendNetworkEapIdentityResponse_1_1(any(ArrayList.class),
                any(ArrayList.class));
        assertTrue(mSupplicantNetwork.sendNetworkEapIdentityResponse(identityStr,
                encryptedIdentityStr));
    }

    /**
     * Tests the addition of FT flags when the device supports it.
     */
    @Test
    public void testAddFtPskFlags() throws Exception {
        mResources.setBoolean(R.bool.config_wifi_fast_bss_transition_enabled, true);
        createSupplicantStaNetwork();

        WifiConfiguration config = WifiConfigurationTestUtil.createPskNetwork();
        assertTrue(mSupplicantNetwork.saveWifiConfiguration(config));

        // Check the supplicant variables to ensure that we have added the FT flags.
        assertTrue((mSupplicantVariables.keyMgmtMask & ISupplicantStaNetwork.KeyMgmtMask.FT_PSK)
                == ISupplicantStaNetwork.KeyMgmtMask.FT_PSK);

        WifiConfiguration loadConfig = new WifiConfiguration();
        Map<String, String> networkExtras = new HashMap<>();
        assertTrue(mSupplicantNetwork.loadWifiConfiguration(loadConfig, networkExtras));
        // The FT flags should be stripped out when reading it back.
        WifiConfigurationTestUtil.assertConfigurationEqualForSupplicant(config, loadConfig);
    }

    /**
     * Tests the addition of FT flags when the device supports it.
     */
    @Test
    public void testAddFtEapFlags() throws Exception {
        mResources.setBoolean(R.bool.config_wifi_fast_bss_transition_enabled, true);
        createSupplicantStaNetwork();

        WifiConfiguration config = WifiConfigurationTestUtil.createEapNetwork();
        assertTrue(mSupplicantNetwork.saveWifiConfiguration(config));

        // Check the supplicant variables to ensure that we have added the FT flags.
        assertTrue((mSupplicantVariables.keyMgmtMask & ISupplicantStaNetwork.KeyMgmtMask.FT_EAP)
                == ISupplicantStaNetwork.KeyMgmtMask.FT_EAP);

        WifiConfiguration loadConfig = new WifiConfiguration();
        Map<String, String> networkExtras = new HashMap<>();
        assertTrue(mSupplicantNetwork.loadWifiConfiguration(loadConfig, networkExtras));
        // The FT flags should be stripped out when reading it back.
        WifiConfigurationTestUtil.assertConfigurationEqualForSupplicant(config, loadConfig);
    }

    /**
     * Tests the addition of SHA256 flags (WPA_PSK_SHA256)
     */
    @Test
    public void testAddPskSha256Flags() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createPskNetwork();
        // Now expose the V1.2 ISupplicantStaNetwork
        mSupplicantNetwork = new SupplicantStaNetworkHalSpyV1_2(mISupplicantStaNetworkMock,
                IFACE_NAME, mContext, mWifiMonitor);
        assertTrue(mSupplicantNetwork.saveWifiConfiguration(config));

        // Check the supplicant variables to ensure that we have added the SHA256 flags.
        assertTrue((mSupplicantVariables.keyMgmtMask
                & android.hardware.wifi.supplicant.V1_2.ISupplicantStaNetwork.KeyMgmtMask
                .WPA_PSK_SHA256) == android.hardware.wifi.supplicant.V1_2.ISupplicantStaNetwork
                .KeyMgmtMask.WPA_PSK_SHA256);

        WifiConfiguration loadConfig = new WifiConfiguration();
        Map<String, String> networkExtras = new HashMap<>();
        assertTrue(mSupplicantNetwork.loadWifiConfiguration(loadConfig, networkExtras));
        // The SHA256 flags should be stripped out when reading it back.
        WifiConfigurationTestUtil.assertConfigurationEqualForSupplicant(config, loadConfig);
    }

    /**
     * Tests the addition of SHA256 flags (WPA_EAP_SHA256)
     */
    @Test
    public void testAddEapSha256Flags() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createEapNetwork();
        // Now expose the V1.2 ISupplicantStaNetwork
        mSupplicantNetwork = new SupplicantStaNetworkHalSpyV1_2(mISupplicantStaNetworkMock,
                IFACE_NAME, mContext, mWifiMonitor);
        assertTrue(mSupplicantNetwork.saveWifiConfiguration(config));

        // Check the supplicant variables to ensure that we have added the SHA256 flags.
        assertTrue((mSupplicantVariables.keyMgmtMask
                & android.hardware.wifi.supplicant.V1_2.ISupplicantStaNetwork.KeyMgmtMask
                .WPA_EAP_SHA256) == android.hardware.wifi.supplicant.V1_2.ISupplicantStaNetwork
                .KeyMgmtMask.WPA_EAP_SHA256);

        WifiConfiguration loadConfig = new WifiConfiguration();
        Map<String, String> networkExtras = new HashMap<>();
        assertTrue(mSupplicantNetwork.loadWifiConfiguration(loadConfig, networkExtras));
        // The SHA256 flags should be stripped out when reading it back.
        WifiConfigurationTestUtil.assertConfigurationEqualForSupplicant(config, loadConfig);
    }

    /**
     * Tests the addition of SHA256 flags (WPA_PSK_SHA256) is ignored on HAL v1.1 or lower
     */
    @Test
    public void testAddPskSha256FlagsHal1_1OrLower() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createPskNetwork();
        assertTrue(mSupplicantNetwork.saveWifiConfiguration(config));

        // Check the supplicant variables to ensure that we have NOT added the SHA256 flags.
        assertFalse((mSupplicantVariables.keyMgmtMask
                & android.hardware.wifi.supplicant.V1_2.ISupplicantStaNetwork.KeyMgmtMask
                .WPA_PSK_SHA256) == android.hardware.wifi.supplicant.V1_2.ISupplicantStaNetwork
                .KeyMgmtMask.WPA_PSK_SHA256);
    }

    /**
     * Tests the addition of SHA256 flags (WPA_EAP_SHA256) is ignored on HAL v1.1 or lower
     */
    @Test
    public void testAddEapSha256FlagsHal1_1OrLower() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createEapNetwork();
        assertTrue(mSupplicantNetwork.saveWifiConfiguration(config));

        // Check the supplicant variables to ensure that we have NOT added the SHA256 flags.
        assertFalse((mSupplicantVariables.keyMgmtMask
                & android.hardware.wifi.supplicant.V1_2.ISupplicantStaNetwork.KeyMgmtMask
                .WPA_EAP_SHA256) == android.hardware.wifi.supplicant.V1_2.ISupplicantStaNetwork
                .KeyMgmtMask.WPA_EAP_SHA256);
    }

    /**
     * Tests the retrieval of WPS NFC token.
     */
    @Test
    public void testGetWpsNfcConfigurationToken() throws Exception {
        final ArrayList<Byte> token = new ArrayList<>();
        token.add(Byte.valueOf((byte) 0x45));
        token.add(Byte.valueOf((byte) 0x34));

        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getWpsNfcConfigurationTokenCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, token);
            }
        }).when(mISupplicantStaNetworkMock)
                .getWpsNfcConfigurationToken(
                        any(ISupplicantStaNetwork.getWpsNfcConfigurationTokenCallback.class));

        assertEquals("4534", mSupplicantNetwork.getWpsNfcConfigurationToken());
    }

    /**
     * Tests that callback registration failure triggers a failure in saving network config.
     */
    @Test
    public void testSaveFailureDueToCallbackReg() throws Exception {
        when(mISupplicantStaNetworkMock.registerCallback(any(ISupplicantStaNetworkCallback.class)))
                .thenReturn(mStatusFailure);
        WifiConfiguration config = WifiConfigurationTestUtil.createPskNetwork();
        assertFalse(mSupplicantNetwork.saveWifiConfiguration(config));
    }

    /**
     * Tests the network gsm auth callback.
     */
    @Test
    public void testNetworkEapGsmAuthCallback() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createPskNetwork();
        assertTrue(mSupplicantNetwork.saveWifiConfiguration(config));
        assertNotNull(mISupplicantStaNetworkCallback);

        // Now trigger eap gsm callback and ensure that the event is broadcast via WifiMonitor.
        NetworkRequestEapSimGsmAuthParams params = new NetworkRequestEapSimGsmAuthParams();
        Random random = new Random();
        byte[] rand1 = new byte[16];
        byte[] rand2 = new byte[16];
        byte[] rand3 = new byte[16];
        random.nextBytes(rand1);
        random.nextBytes(rand2);
        random.nextBytes(rand3);
        params.rands.add(rand1);
        params.rands.add(rand2);
        params.rands.add(rand3);

        String[] expectedRands = {
                NativeUtil.hexStringFromByteArray(rand1), NativeUtil.hexStringFromByteArray(rand2),
                NativeUtil.hexStringFromByteArray(rand3)
        };

        mISupplicantStaNetworkCallback.onNetworkEapSimGsmAuthRequest(params);
        verify(mWifiMonitor).broadcastNetworkGsmAuthRequestEvent(
                eq(IFACE_NAME), eq(config.networkId), eq(config.SSID), eq(expectedRands));
    }

    /**
     * Tests the network umts auth callback.
     */
    @Test
    public void testNetworkEapUmtsAuthCallback() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createPskNetwork();
        assertTrue(mSupplicantNetwork.saveWifiConfiguration(config));
        assertNotNull(mISupplicantStaNetworkCallback);

        // Now trigger eap gsm callback and ensure that the event is broadcast via WifiMonitor.
        NetworkRequestEapSimUmtsAuthParams params = new NetworkRequestEapSimUmtsAuthParams();
        Random random = new Random();
        random.nextBytes(params.autn);
        random.nextBytes(params.rand);

        String[] expectedRands = {
                NativeUtil.hexStringFromByteArray(params.rand),
                NativeUtil.hexStringFromByteArray(params.autn)
        };

        mISupplicantStaNetworkCallback.onNetworkEapSimUmtsAuthRequest(params);
        verify(mWifiMonitor).broadcastNetworkUmtsAuthRequestEvent(
                eq(IFACE_NAME), eq(config.networkId), eq(config.SSID), eq(expectedRands));
    }

    /**
     * Tests the network identity callback.
     */
    @Test
    public void testNetworkIdentityCallback() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createPskNetwork();
        assertTrue(mSupplicantNetwork.saveWifiConfiguration(config));
        assertNotNull(mISupplicantStaNetworkCallback);

        // Now trigger identity request callback and ensure that the event is broadcast via
        // WifiMonitor.
        mISupplicantStaNetworkCallback.onNetworkEapIdentityRequest();
        verify(mWifiMonitor).broadcastNetworkIdentityRequestEvent(
                eq(IFACE_NAME), eq(config.networkId), eq(config.SSID));
    }

    private void testWifiConfigurationSaveLoad(WifiConfiguration config) {
        assertTrue(mSupplicantNetwork.saveWifiConfiguration(config));
        WifiConfiguration loadConfig = new WifiConfiguration();
        Map<String, String> networkExtras = new HashMap<>();
        assertTrue(mSupplicantNetwork.loadWifiConfiguration(loadConfig, networkExtras));
        WifiConfigurationTestUtil.assertConfigurationEqualForSupplicant(config, loadConfig);
        assertEquals(config.configKey(),
                networkExtras.get(SupplicantStaNetworkHal.ID_STRING_KEY_CONFIG_KEY));
        assertEquals(
                config.creatorUid,
                Integer.parseInt(networkExtras.get(
                        SupplicantStaNetworkHal.ID_STRING_KEY_CREATOR_UID)));
        // There is no getter for this one, so check the supplicant variable.
        if (!TextUtils.isEmpty(config.updateIdentifier)) {
            assertEquals(Integer.parseInt(config.updateIdentifier),
                    mSupplicantVariables.updateIdentifier);
        }
        // There is no getter for this one, so check the supplicant variable.
        String oppKeyCaching =
                config.enterpriseConfig.getFieldValue(WifiEnterpriseConfig.OPP_KEY_CACHING);
        if (!TextUtils.isEmpty(oppKeyCaching)) {
            assertEquals(
                    Integer.parseInt(oppKeyCaching) == 1 ? true : false,
                    mSupplicantVariables.eapProactiveKeyCaching);
        }
    }

    /**
     * Verifies that createNetworkExtra() & parseNetworkExtra correctly writes a serialized and
     * URL-encoded JSON object.
     */
    @Test
    public void testNetworkExtra() {
        assertEquals(NETWORK_EXTRAS_SERIALIZED,
                SupplicantStaNetworkHal.createNetworkExtra(NETWORK_EXTRAS_VALUES));
        assertEquals(NETWORK_EXTRAS_VALUES,
                SupplicantStaNetworkHal.parseNetworkExtra(NETWORK_EXTRAS_SERIALIZED));
    }

    /**
     * Verifies that fetachEapAnonymousIdentity() can get the anonymous identity from supplicant.
     */
    @Test
    public void testFetchEapAnonymousIdentity() {
        WifiConfiguration config = WifiConfigurationTestUtil.createEapNetwork();
        config.enterpriseConfig.setAnonymousIdentity(ANONYMOUS_IDENTITY);
        assertTrue(mSupplicantNetwork.saveWifiConfiguration(config));
        assertEquals(ANONYMOUS_IDENTITY, mSupplicantNetwork.fetchEapAnonymousIdentity());
    }

    /**
     * Sets up the HIDL interface mock with all the setters/getter values.
     * Note: This only sets up the mock to return success on all methods.
     */
    private void setupISupplicantNetworkMock() throws Exception {
        /** SSID */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(ArrayList<Byte> ssid) throws RemoteException {
                mSupplicantVariables.ssid = ssid;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setSsid(any(ArrayList.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getSsidCallback cb) throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.ssid);
            }
        }).when(mISupplicantStaNetworkMock)
                .getSsid(any(ISupplicantStaNetwork.getSsidCallback.class));

        /** Network Id */
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantNetwork.getIdCallback cb) throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.networkId);
            }
        }).when(mISupplicantStaNetworkMock).getId(any(ISupplicantNetwork.getIdCallback.class));

        /** BSSID */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(byte[] bssid) throws RemoteException {
                mSupplicantVariables.bssid = bssid;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setBssid(any(byte[].class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getBssidCallback cb) throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.bssid);
            }
        }).when(mISupplicantStaNetworkMock)
                .getBssid(any(ISupplicantStaNetwork.getBssidCallback.class));

        /** Scan SSID (Is Hidden Network?) */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(boolean enable) throws RemoteException {
                mSupplicantVariables.scanSsid = enable;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setScanSsid(any(boolean.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getScanSsidCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.scanSsid);
            }
        }).when(mISupplicantStaNetworkMock)
                .getScanSsid(any(ISupplicantStaNetwork.getScanSsidCallback.class));

        /** Require PMF*/
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(boolean enable) throws RemoteException {
                mSupplicantVariables.requirePmf = enable;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setRequirePmf(any(boolean.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getRequirePmfCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.requirePmf);
            }
        }).when(mISupplicantStaNetworkMock)
                .getRequirePmf(any(ISupplicantStaNetwork.getRequirePmfCallback.class));

        /** SAE password */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(String saePassword) throws RemoteException {
                mSupplicantVariables.pskPassphrase = saePassword;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkV12).setSaePassword(any(String.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getPskPassphraseCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.pskPassphrase);
            }
        }).when(mISupplicantStaNetworkV12)
                .getSaePassword(any(android.hardware.wifi.supplicant.V1_2.ISupplicantStaNetwork
                        .getSaePasswordCallback.class));

        /** PSK passphrase */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(String pskPassphrase) throws RemoteException {
                mSupplicantVariables.pskPassphrase = pskPassphrase;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setPskPassphrase(any(String.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getPskPassphraseCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.pskPassphrase);
            }
        }).when(mISupplicantStaNetworkMock)
                .getPskPassphrase(any(ISupplicantStaNetwork.getPskPassphraseCallback.class));

        /** PSK */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(byte[] psk) throws RemoteException {
                mSupplicantVariables.psk = psk;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setPsk(any(byte[].class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getPskCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.psk);
            }
        }).when(mISupplicantStaNetworkMock)
                .getPsk(any(ISupplicantStaNetwork.getPskCallback.class));

        /** WEP keys **/
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(int keyIdx, ArrayList<Byte> key) throws RemoteException {
                mSupplicantVariables.wepKey[keyIdx] = key;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setWepKey(any(int.class), any(ArrayList.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(int keyIdx, ISupplicantStaNetwork.getWepKeyCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.wepKey[keyIdx]);
            }
        }).when(mISupplicantStaNetworkMock)
                .getWepKey(any(int.class), any(ISupplicantStaNetwork.getWepKeyCallback.class));

        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(int keyIdx) throws RemoteException {
                mSupplicantVariables.wepTxKeyIdx = keyIdx;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setWepTxKeyIdx(any(int.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getWepTxKeyIdxCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.wepTxKeyIdx);
            }
        }).when(mISupplicantStaNetworkMock)
                .getWepTxKeyIdx(any(ISupplicantStaNetwork.getWepTxKeyIdxCallback.class));

        /** allowedKeyManagement */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(int mask) throws RemoteException {
                mSupplicantVariables.keyMgmtMask = mask;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setKeyMgmt(any(int.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getKeyMgmtCallback cb) throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.keyMgmtMask);
            }
        }).when(mISupplicantStaNetworkMock)
                .getKeyMgmt(any(ISupplicantStaNetwork.getKeyMgmtCallback.class));

        /** allowedKeyManagement v1.2 */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(int mask) throws RemoteException {
                mSupplicantVariables.keyMgmtMask = mask;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkV12).setKeyMgmt_1_2(any(int.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getKeyMgmtCallback cb) throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.keyMgmtMask);
            }
        }).when(mISupplicantStaNetworkV12)
                .getKeyMgmt_1_2(any(android.hardware.wifi.supplicant.V1_2.ISupplicantStaNetwork
                        .getKeyMgmt_1_2Callback.class));

        /** allowedProtocols */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(int mask) throws RemoteException {
                mSupplicantVariables.protoMask = mask;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setProto(any(int.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getProtoCallback cb) throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.protoMask);
            }
        }).when(mISupplicantStaNetworkMock)
                .getProto(any(ISupplicantStaNetwork.getProtoCallback.class));

        /** allowedAuthAlgorithms */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(int mask) throws RemoteException {
                mSupplicantVariables.authAlgMask = mask;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setAuthAlg(any(int.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getAuthAlgCallback cb) throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.authAlgMask);
            }
        }).when(mISupplicantStaNetworkMock)
                .getAuthAlg(any(ISupplicantStaNetwork.getAuthAlgCallback.class));

        /** allowedGroupCiphers */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(int mask) throws RemoteException {
                mSupplicantVariables.groupCipherMask = mask;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setGroupCipher(any(int.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getGroupCipherCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.groupCipherMask);
            }
        }).when(mISupplicantStaNetworkMock)
                .getGroupCipher(any(ISupplicantStaNetwork.getGroupCipherCallback.class));

        /** allowedGroupCiphers v1.2*/
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(int mask) throws RemoteException {
                mSupplicantVariables.groupCipherMask = mask;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkV12).setGroupCipher_1_2(any(int.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getGroupCipherCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.groupCipherMask);
            }
        }).when(mISupplicantStaNetworkV12)
                .getGroupCipher_1_2(any(android.hardware.wifi.supplicant.V1_2.ISupplicantStaNetwork
                        .getGroupCipher_1_2Callback.class));

        /** allowedPairwiseCiphers */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(int mask) throws RemoteException {
                mSupplicantVariables.pairwiseCipherMask = mask;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setPairwiseCipher(any(int.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getPairwiseCipherCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.pairwiseCipherMask);
            }
        }).when(mISupplicantStaNetworkMock)
                .getPairwiseCipher(any(ISupplicantStaNetwork.getPairwiseCipherCallback.class));

        /** allowedPairwiseCiphers v1.2 */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(int mask) throws RemoteException {
                mSupplicantVariables.pairwiseCipherMask = mask;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkV12).setPairwiseCipher_1_2(any(int.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getPairwiseCipherCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.pairwiseCipherMask);
            }
        }).when(mISupplicantStaNetworkV12)
                .getPairwiseCipher_1_2(any(android.hardware.wifi.supplicant.V1_2
                        .ISupplicantStaNetwork.getPairwiseCipher_1_2Callback.class));

        /** metadata: idstr */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(String idStr) throws RemoteException {
                mSupplicantVariables.idStr = idStr;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setIdStr(any(String.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getIdStrCallback cb) throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.idStr);
            }
        }).when(mISupplicantStaNetworkMock)
                .getIdStr(any(ISupplicantStaNetwork.getIdStrCallback.class));

        /** UpdateIdentifier */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(int identifier) throws RemoteException {
                mSupplicantVariables.updateIdentifier = identifier;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setUpdateIdentifier(any(int.class));

        /** EAP method */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(int method) throws RemoteException {
                mSupplicantVariables.eapMethod = method;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setEapMethod(any(int.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getEapMethodCallback cb)
                    throws RemoteException {
                // When not set, return failure.
                if (mSupplicantVariables.eapMethod == -1) {
                    cb.onValues(mStatusFailure, mSupplicantVariables.eapMethod);
                } else {
                    cb.onValues(mStatusSuccess, mSupplicantVariables.eapMethod);
                }
            }
        }).when(mISupplicantStaNetworkMock)
                .getEapMethod(any(ISupplicantStaNetwork.getEapMethodCallback.class));

        /** EAP Phase 2 method */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(int method) throws RemoteException {
                mSupplicantVariables.eapPhase2Method = method;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setEapPhase2Method(any(int.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getEapPhase2MethodCallback cb)
                    throws RemoteException {
                // When not set, return failure.
                if (mSupplicantVariables.eapPhase2Method == -1) {
                    cb.onValues(mStatusFailure, mSupplicantVariables.eapPhase2Method);
                } else {
                    cb.onValues(mStatusSuccess, mSupplicantVariables.eapPhase2Method);
                }
            }
        }).when(mISupplicantStaNetworkMock)
                .getEapPhase2Method(any(ISupplicantStaNetwork.getEapPhase2MethodCallback.class));

        /** EAP Identity */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(ArrayList<Byte> identity) throws RemoteException {
                mSupplicantVariables.eapIdentity = identity;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setEapIdentity(any(ArrayList.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getEapIdentityCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.eapIdentity);
            }
        }).when(mISupplicantStaNetworkMock)
                .getEapIdentity(any(ISupplicantStaNetwork.getEapIdentityCallback.class));

        /** EAP Anonymous Identity */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(ArrayList<Byte> identity) throws RemoteException {
                mSupplicantVariables.eapAnonymousIdentity = identity;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setEapAnonymousIdentity(any(ArrayList.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getEapAnonymousIdentityCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.eapAnonymousIdentity);
            }
        }).when(mISupplicantStaNetworkMock)
                .getEapAnonymousIdentity(
                        any(ISupplicantStaNetwork.getEapAnonymousIdentityCallback.class));

        /** EAP Password */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(ArrayList<Byte> password) throws RemoteException {
                mSupplicantVariables.eapPassword = password;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setEapPassword(any(ArrayList.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getEapPasswordCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.eapPassword);
            }
        }).when(mISupplicantStaNetworkMock)
                .getEapPassword(any(ISupplicantStaNetwork.getEapPasswordCallback.class));

        /** EAP Client Cert */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(String cert) throws RemoteException {
                mSupplicantVariables.eapClientCert = cert;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setEapClientCert(any(String.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getEapClientCertCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.eapClientCert);
            }
        }).when(mISupplicantStaNetworkMock)
                .getEapClientCert(any(ISupplicantStaNetwork.getEapClientCertCallback.class));

        /** EAP CA Cert */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(String cert) throws RemoteException {
                mSupplicantVariables.eapCACert = cert;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setEapCACert(any(String.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getEapCACertCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.eapCACert);
            }
        }).when(mISupplicantStaNetworkMock)
                .getEapCACert(any(ISupplicantStaNetwork.getEapCACertCallback.class));

        /** EAP Subject Match */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(String match) throws RemoteException {
                mSupplicantVariables.eapSubjectMatch = match;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setEapSubjectMatch(any(String.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getEapSubjectMatchCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.eapSubjectMatch);
            }
        }).when(mISupplicantStaNetworkMock)
                .getEapSubjectMatch(any(ISupplicantStaNetwork.getEapSubjectMatchCallback.class));

        /** EAP Engine */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(boolean enable) throws RemoteException {
                mSupplicantVariables.eapEngine = enable;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setEapEngine(any(boolean.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getEapEngineCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.eapEngine);
            }
        }).when(mISupplicantStaNetworkMock)
                .getEapEngine(any(ISupplicantStaNetwork.getEapEngineCallback.class));

        /** EAP Engine ID */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(String id) throws RemoteException {
                mSupplicantVariables.eapEngineID = id;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setEapEngineID(any(String.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getEapEngineIDCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.eapEngineID);
            }
        }).when(mISupplicantStaNetworkMock)
                .getEapEngineID(any(ISupplicantStaNetwork.getEapEngineIDCallback.class));

        /** EAP Private Key */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(String key) throws RemoteException {
                mSupplicantVariables.eapPrivateKeyId = key;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setEapPrivateKeyId(any(String.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getEapPrivateKeyIdCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.eapPrivateKeyId);
            }
        }).when(mISupplicantStaNetworkMock)
                .getEapPrivateKeyId(any(ISupplicantStaNetwork.getEapPrivateKeyIdCallback.class));

        /** EAP Alt Subject Match */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(String match) throws RemoteException {
                mSupplicantVariables.eapAltSubjectMatch = match;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setEapAltSubjectMatch(any(String.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getEapAltSubjectMatchCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.eapAltSubjectMatch);
            }
        }).when(mISupplicantStaNetworkMock)
                .getEapAltSubjectMatch(
                        any(ISupplicantStaNetwork.getEapAltSubjectMatchCallback.class));

        /** EAP Domain Suffix Match */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(String match) throws RemoteException {
                mSupplicantVariables.eapDomainSuffixMatch = match;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setEapDomainSuffixMatch(any(String.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getEapDomainSuffixMatchCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.eapDomainSuffixMatch);
            }
        }).when(mISupplicantStaNetworkMock)
                .getEapDomainSuffixMatch(
                        any(ISupplicantStaNetwork.getEapDomainSuffixMatchCallback.class));

        /** EAP CA Path*/
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(String path) throws RemoteException {
                mSupplicantVariables.eapCAPath = path;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setEapCAPath(any(String.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantStaNetwork.getEapCAPathCallback cb)
                    throws RemoteException {
                cb.onValues(mStatusSuccess, mSupplicantVariables.eapCAPath);
            }
        }).when(mISupplicantStaNetworkMock)
                .getEapCAPath(any(ISupplicantStaNetwork.getEapCAPathCallback.class));

        /** EAP Proactive Key Caching */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(boolean enable) throws RemoteException {
                mSupplicantVariables.eapProactiveKeyCaching = enable;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock).setProactiveKeyCaching(any(boolean.class));

        /** Callback registration */
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(ISupplicantStaNetworkCallback cb)
                    throws RemoteException {
                mISupplicantStaNetworkCallback = cb;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkMock)
                .registerCallback(any(ISupplicantStaNetworkCallback.class));

        /** Suite-B*/
        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(boolean enable) throws RemoteException {
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkV12).enableTlsSuiteBEapPhase1Param(any(boolean.class));

        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer() throws RemoteException {
                return mStatusSuccess;
            }
        }).when(mISupplicantStaNetworkV12).enableSuiteBEapOpenSslCiphers();
    }

    private SupplicantStatus createSupplicantStatus(int code) {
        SupplicantStatus status = new SupplicantStatus();
        status.code = code;
        return status;
    }

    /**
     * Need this for tests which wants to manipulate context before creating the instance.
     */
    private void createSupplicantStaNetwork() {
        mSupplicantNetwork =
                new SupplicantStaNetworkHal(mISupplicantStaNetworkMock, IFACE_NAME, mContext,
                        mWifiMonitor);
        mSupplicantNetwork.enableVerboseLogging(true);
    }

    // Private class to to store/inspect values set via the HIDL mock.
    private class SupplicantNetworkVariables {
        public ArrayList<Byte> ssid;
        public int networkId;
        public byte[/* 6 */] bssid;
        public int keyMgmtMask;
        public int protoMask;
        public int authAlgMask;
        public int groupCipherMask;
        public int pairwiseCipherMask;
        public boolean scanSsid;
        public boolean requirePmf;
        public String idStr;
        public int updateIdentifier;
        public String pskPassphrase;
        public byte[] psk;
        public ArrayList<Byte>[] wepKey = new ArrayList[4];
        public int wepTxKeyIdx;
        public int eapMethod = -1;
        public int eapPhase2Method = -1;
        public ArrayList<Byte> eapIdentity;
        public ArrayList<Byte> eapAnonymousIdentity;
        public ArrayList<Byte> eapPassword;
        public String eapCACert;
        public String eapCAPath;
        public String eapClientCert;
        public String eapPrivateKeyId;
        public String eapSubjectMatch;
        public String eapAltSubjectMatch;
        public boolean eapEngine;
        public String eapEngineID;
        public String eapDomainSuffixMatch;
        public boolean eapProactiveKeyCaching;
    }
}
