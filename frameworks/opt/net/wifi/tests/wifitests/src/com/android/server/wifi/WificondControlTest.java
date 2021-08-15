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
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Matchers.argThat;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyLong;
import static org.mockito.Mockito.anyString;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.reset;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.when;

import android.app.AlarmManager;
import android.net.wifi.IApInterface;
import android.net.wifi.IApInterfaceEventCallback;
import android.net.wifi.IClientInterface;
import android.net.wifi.IPnoScanEvent;
import android.net.wifi.IScanEvent;
import android.net.wifi.ISendMgmtFrameEvent;
import android.net.wifi.IWifiScannerImpl;
import android.net.wifi.IWificond;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiEnterpriseConfig;
import android.net.wifi.WifiScanner;
import android.os.Handler;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.test.TestLooper;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.WifiNative.SendMgmtFrameCallback;
import com.android.server.wifi.util.NativeUtil;
import com.android.server.wifi.wificond.ChannelSettings;
import com.android.server.wifi.wificond.HiddenNetwork;
import com.android.server.wifi.wificond.NativeScanResult;
import com.android.server.wifi.wificond.PnoSettings;
import com.android.server.wifi.wificond.RadioChainInfo;
import com.android.server.wifi.wificond.SingleScanSettings;

import org.junit.Before;
import org.junit.Test;
import org.mockito.AdditionalMatchers;
import org.mockito.ArgumentCaptor;
import org.mockito.ArgumentMatcher;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.io.ByteArrayOutputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.BitSet;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

/**
 * Unit tests for {@link com.android.server.wifi.WificondControl}.
 */
@SmallTest
public class WificondControlTest {
    @Mock private WifiInjector mWifiInjector;
    @Mock private WifiMonitor mWifiMonitor;
    @Mock private WifiMetrics mWifiMetrics;
    @Mock private IWificond mWificond;
    @Mock private IBinder mWifiCondBinder;
    @Mock private IClientInterface mClientInterface;
    @Mock private IWifiScannerImpl mWifiScannerImpl;
    @Mock private CarrierNetworkConfig mCarrierNetworkConfig;
    @Mock private IApInterface mApInterface;
    @Mock private WifiNative.SoftApListener mSoftApListener;
    @Mock private AlarmManager mAlarmManager;
    @Mock private Clock mClock;
    @Mock private SendMgmtFrameCallback mSendMgmtFrameCallback;
    private TestLooper mLooper;
    private WificondControl mWificondControl;
    private static final String TEST_INTERFACE_NAME = "test_wlan_if";
    private static final String TEST_INTERFACE_NAME1 = "test_wlan_if1";
    private static final String TEST_INVALID_INTERFACE_NAME = "asdf";
    private static final byte[] TEST_SSID =
            new byte[] {'G', 'o', 'o', 'g', 'l', 'e', 'G', 'u', 'e', 's', 't'};
    private static final byte[] TEST_PSK =
            new byte[] {'T', 'e', 's', 't'};
    private static final byte[] TEST_BSSID =
            new byte[] {(byte) 0x12, (byte) 0xef, (byte) 0xa1,
                        (byte) 0x2c, (byte) 0x97, (byte) 0x8b};
    // This the IE buffer which is consistent with TEST_SSID.
    private static final byte[] TEST_INFO_ELEMENT_SSID =
            new byte[] {
                    // Element ID for SSID.
                    (byte) 0x00,
                    // Length of the SSID: 0x0b or 11.
                    (byte) 0x0b,
                    // This is string "GoogleGuest"
                    'G', 'o', 'o', 'g', 'l', 'e', 'G', 'u', 'e', 's', 't'};
    // RSN IE data indicating EAP key management.
    private static final byte[] TEST_INFO_ELEMENT_RSN =
            new byte[] {
                    // Element ID for RSN.
                    (byte) 0x30,
                    // Length of the element data.
                    (byte) 0x18,
                    (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x02,
                    (byte) 0x02, (byte) 0x00, (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x04,
                    (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x02, (byte) 0x01, (byte) 0x00,
                    (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x01, (byte) 0x00, (byte) 0x00 };

    private static final int TEST_FREQUENCY = 2456;
    private static final int TEST_SIGNAL_MBM = -4500;
    private static final long TEST_TSF = 34455441;
    private static final BitSet TEST_CAPABILITY = new BitSet(16) {{ set(2); set(5); }};
    private static final boolean TEST_ASSOCIATED = true;
    private static final NativeScanResult MOCK_NATIVE_SCAN_RESULT =
            new NativeScanResult() {{
                ssid = TEST_SSID;
                bssid = TEST_BSSID;
                infoElement = TEST_INFO_ELEMENT_SSID;
                frequency = TEST_FREQUENCY;
                signalMbm = TEST_SIGNAL_MBM;
                capability = TEST_CAPABILITY;
                associated = TEST_ASSOCIATED;
                radioChainInfos = new ArrayList<>();
            }};
    private static final RadioChainInfo MOCK_NATIVE_RADIO_CHAIN_INFO_1 =
            new RadioChainInfo() {{
                chainId = 1;
                level = -89;
            }};
    private static final RadioChainInfo MOCK_NATIVE_RADIO_CHAIN_INFO_2 =
            new RadioChainInfo() {{
                chainId = 0;
                level = -78;
            }};
    private static final Set<Integer> SCAN_FREQ_SET =
            new HashSet<Integer>() {{
                add(2410);
                add(2450);
                add(5050);
                add(5200);
            }};
    private static final String TEST_QUOTED_SSID_1 = "\"testSsid1\"";
    private static final String TEST_QUOTED_SSID_2 = "\"testSsid2\"";
    private static final int[] TEST_FREQUENCIES_1 = {};
    private static final int[] TEST_FREQUENCIES_2 = {2500, 5124};

    private static final List<String> SCAN_HIDDEN_NETWORK_SSID_LIST =
            new ArrayList<String>() {{
                add(TEST_QUOTED_SSID_1);
                add(TEST_QUOTED_SSID_2);
            }};


    private static final WifiNative.PnoSettings TEST_PNO_SETTINGS =
            new WifiNative.PnoSettings() {{
                isConnected = false;
                periodInMs = 6000;
                networkList = new WifiNative.PnoNetwork[2];
                networkList[0] = new WifiNative.PnoNetwork();
                networkList[1] = new WifiNative.PnoNetwork();
                networkList[0].ssid = TEST_QUOTED_SSID_1;
                networkList[0].flags = WifiScanner.PnoSettings.PnoNetwork.FLAG_DIRECTED_SCAN;
                networkList[0].frequencies = TEST_FREQUENCIES_1;
                networkList[1].ssid = TEST_QUOTED_SSID_2;
                networkList[1].flags = 0;
                networkList[1].frequencies = TEST_FREQUENCIES_2;
            }};

    private static final int TEST_MCS_RATE = 5;
    private static final int TEST_SEND_MGMT_FRAME_ELAPSED_TIME_MS = 100;
    private static final byte[] TEST_PROBE_FRAME = {
            0x40, 0x00, 0x3c, 0x00, (byte) 0xa8, (byte) 0xbd, 0x27, 0x5b,
            0x33, 0x72, (byte) 0xf4, (byte) 0xf5, (byte) 0xe8, 0x51, (byte) 0x9e, 0x09,
            (byte) 0xa8, (byte) 0xbd, 0x27, 0x5b, 0x33, 0x72, (byte) 0xb0, 0x66,
            0x00, 0x00
    };

    @Before
    public void setUp() throws Exception {
        // Setup mocks for successful WificondControl operation. Failure case mocks should be
        // created in specific tests
        MockitoAnnotations.initMocks(this);
        when(mWifiInjector.makeWificond()).thenReturn(mWificond);
        when(mWificond.asBinder()).thenReturn(mWifiCondBinder);
        when(mClientInterface.getWifiScannerImpl()).thenReturn(mWifiScannerImpl);
        when(mWificond.createClientInterface(any())).thenReturn(mClientInterface);
        when(mWificond.createApInterface(any())).thenReturn(mApInterface);
        when(mWificond.tearDownClientInterface(any())).thenReturn(true);
        when(mWificond.tearDownApInterface(any())).thenReturn(true);
        when(mClientInterface.getWifiScannerImpl()).thenReturn(mWifiScannerImpl);
        when(mClientInterface.getInterfaceName()).thenReturn(TEST_INTERFACE_NAME);
        when(mWifiInjector.getWifiMetrics()).thenReturn(mWifiMetrics);
        mLooper = new TestLooper();
        mWificondControl = new WificondControl(mWifiInjector, mWifiMonitor, mCarrierNetworkConfig,
                mAlarmManager, mLooper.getLooper(), mClock);
        assertEquals(mClientInterface, mWificondControl.setupInterfaceForClientMode(
                TEST_INTERFACE_NAME));
        verify(mWifiInjector).makeWificond();
        verify(mWifiCondBinder).linkToDeath(any(IBinder.DeathRecipient.class), anyInt());
    }

    /**
     * Verifies that setupInterfaceForClientMode(TEST_INTERFACE_NAME) calls Wificond.
     */
    @Test
    public void testSetupInterfaceForClientMode() throws Exception {
        when(mWifiInjector.makeWificond()).thenReturn(mWificond);
        when(mWificond.createClientInterface(TEST_INTERFACE_NAME)).thenReturn(mClientInterface);
        verify(mWificond).createClientInterface(TEST_INTERFACE_NAME);
    }

    /**
     * Verifies that setupInterfaceForClientMode(TEST_INTERFACE_NAME) calls subscribeScanEvents().
     */
    @Test
    public void testSetupInterfaceForClientModeCallsScanEventSubscripiton() throws Exception {
        verify(mWifiScannerImpl).subscribeScanEvents(any(IScanEvent.class));
    }

    /**
     * Verifies that setupInterfaceForClientMode(TEST_INTERFACE_NAME) returns null when wificond is
     * not started.
     */
    @Test
    public void testSetupInterfaceForClientModeErrorWhenWificondIsNotStarted() throws Exception {
        // Invoke wificond death handler to clear the handle.
        mWificondControl.binderDied();
        mLooper.dispatchAll();
        when(mWifiInjector.makeWificond()).thenReturn(null);
        IClientInterface returnedClientInterface =
                mWificondControl.setupInterfaceForClientMode(TEST_INTERFACE_NAME);
        assertEquals(null, returnedClientInterface);
        verify(mWifiInjector, times(2)).makeWificond();
    }

    /**
     * Verifies that setupInterfaceForClientMode(TEST_INTERFACE_NAME) returns null when wificond
     * failed to setup client interface.
     */
    @Test
    public void testSetupInterfaceForClientModeErrorWhenWificondFailedToSetupInterface()
            throws Exception {
        when(mWifiInjector.makeWificond()).thenReturn(mWificond);
        when(mWificond.createClientInterface(TEST_INTERFACE_NAME)).thenReturn(null);

        IClientInterface returnedClientInterface =
                mWificondControl.setupInterfaceForClientMode(TEST_INTERFACE_NAME);
        assertEquals(null, returnedClientInterface);
    }

    /**
     * Verifies that tearDownClientInterface(TEST_INTERFACE_NAME) calls Wificond.
     */
    @Test
    public void testTeardownClientInterface() throws Exception {
        when(mWificond.tearDownClientInterface(TEST_INTERFACE_NAME)).thenReturn(true);

        assertTrue(mWificondControl.tearDownClientInterface(TEST_INTERFACE_NAME));
        verify(mWifiScannerImpl).unsubscribeScanEvents();
        verify(mWifiScannerImpl).unsubscribePnoScanEvents();
        verify(mWificond).tearDownClientInterface(TEST_INTERFACE_NAME);
    }

    /**
     * Verifies that tearDownClientInterface(TEST_INTERFACE_NAME) calls Wificond.
     */
    @Test
    public void testTeardownClientInterfaceOnInvalidIface() throws Exception {
        when(mWificond.tearDownClientInterface(TEST_INTERFACE_NAME1)).thenReturn(true);

        assertFalse(mWificondControl.tearDownClientInterface(TEST_INTERFACE_NAME1));
        verify(mWifiScannerImpl, never()).unsubscribeScanEvents();
        verify(mWifiScannerImpl, never()).unsubscribePnoScanEvents();
        verify(mWificond, never()).tearDownClientInterface(any());
    }

    /**
     * Verifies that tearDownClientInterface(TEST_INTERFACE_NAME) calls Wificond.
     */
    @Test
    public void testTeardownClientInterfaceFailDueToExceptionScannerUnsubscribe() throws Exception {
        when(mWificond.tearDownClientInterface(TEST_INTERFACE_NAME)).thenReturn(true);
        doThrow(new RemoteException()).when(mWifiScannerImpl).unsubscribeScanEvents();

        assertFalse(mWificondControl.tearDownClientInterface(TEST_INTERFACE_NAME));
        verify(mWifiScannerImpl).unsubscribeScanEvents();
        verify(mWifiScannerImpl, never()).unsubscribePnoScanEvents();
        verify(mWificond, never()).tearDownClientInterface(TEST_INTERFACE_NAME);
    }
    /**
     * Verifies that tearDownClientInterface(TEST_INTERFACE_NAME) calls Wificond.
     */
    @Test
    public void testTeardownClientInterfaceErrorWhenWificondFailed() throws Exception {
        when(mWificond.tearDownClientInterface(TEST_INTERFACE_NAME)).thenReturn(false);

        assertFalse(mWificondControl.tearDownClientInterface(TEST_INTERFACE_NAME));
        verify(mWifiScannerImpl).unsubscribeScanEvents();
        verify(mWifiScannerImpl).unsubscribePnoScanEvents();
        verify(mWificond).tearDownClientInterface(TEST_INTERFACE_NAME);
    }

    /**
     * Verifies that the client handles are cleared after teardown.
     */
    @Test
    public void testTeardownClientInterfaceClearsHandles() throws Exception {
        testTeardownClientInterface();

        assertNull(mWificondControl.signalPoll(TEST_INTERFACE_NAME));
        verify(mClientInterface, never()).signalPoll();

        assertFalse(mWificondControl.scan(
                TEST_INTERFACE_NAME, WifiNative.SCAN_TYPE_LOW_LATENCY,
                SCAN_FREQ_SET, SCAN_HIDDEN_NETWORK_SSID_LIST));
        verify(mWifiScannerImpl, never()).scan(any());
    }

    /**
     * Verifies that setupInterfaceForSoftApMode(TEST_INTERFACE_NAME) calls wificond.
     */
    @Test
    public void testSetupInterfaceForSoftApMode() throws Exception {
        when(mWifiInjector.makeWificond()).thenReturn(mWificond);
        when(mWificond.createApInterface(TEST_INTERFACE_NAME)).thenReturn(mApInterface);

        IApInterface returnedApInterface =
                mWificondControl.setupInterfaceForSoftApMode(TEST_INTERFACE_NAME);
        assertEquals(mApInterface, returnedApInterface);
        verify(mWifiInjector).makeWificond();
        verify(mWifiCondBinder).linkToDeath(any(IBinder.DeathRecipient.class), anyInt());
        verify(mWificond).createApInterface(TEST_INTERFACE_NAME);
    }

    /**
     * Verifies that setupInterfaceForSoftAp() returns null when wificond is not started.
     */
    @Test
    public void testSetupInterfaceForSoftApModeErrorWhenWificondIsNotStarted() throws Exception {
        // Invoke wificond death handler to clear the handle.
        mWificondControl.binderDied();
        mLooper.dispatchAll();
        when(mWifiInjector.makeWificond()).thenReturn(null);

        IApInterface returnedApInterface =
                mWificondControl.setupInterfaceForSoftApMode(TEST_INTERFACE_NAME);

        assertEquals(null, returnedApInterface);
        verify(mWifiInjector, times(2)).makeWificond();
    }

    /**
     * Verifies that setupInterfaceForSoftApMode(TEST_INTERFACE_NAME) returns null when wificond
     * failed to setup AP interface.
     */
    @Test
    public void testSetupInterfaceForSoftApModeErrorWhenWificondFailedToSetupInterface()
            throws Exception {
        when(mWifiInjector.makeWificond()).thenReturn(mWificond);
        when(mWificond.createApInterface(TEST_INTERFACE_NAME)).thenReturn(null);

        IApInterface returnedApInterface =
                mWificondControl.setupInterfaceForSoftApMode(TEST_INTERFACE_NAME);
        assertEquals(null, returnedApInterface);
    }

    /**
     * Verifies that tearDownClientInterface(TEST_INTERFACE_NAME) calls Wificond.
     */
    @Test
    public void testTeardownSoftApInterface() throws Exception {
        testSetupInterfaceForSoftApMode();
        when(mWificond.tearDownApInterface(TEST_INTERFACE_NAME)).thenReturn(true);

        assertTrue(mWificondControl.tearDownSoftApInterface(TEST_INTERFACE_NAME));
        verify(mWificond).tearDownApInterface(TEST_INTERFACE_NAME);
    }

    /**
     * Verifies that tearDownSoftapInterface(TEST_INTERFACE_NAME) calls Wificond.
     */
    @Test
    public void testTeardownSoftApInterfaceOnInvalidIface() throws Exception {
        testSetupInterfaceForSoftApMode();
        when(mWificond.tearDownApInterface(TEST_INTERFACE_NAME1)).thenReturn(true);

        assertFalse(mWificondControl.tearDownSoftApInterface(TEST_INTERFACE_NAME1));
        verify(mWificond, never()).tearDownApInterface(any());
    }

    /**
     * Verifies that tearDownClientInterface(TEST_INTERFACE_NAME) calls Wificond.
     */
    @Test
    public void testTeardownSoftApInterfaceErrorWhenWificondFailed() throws Exception {
        testSetupInterfaceForSoftApMode();
        when(mWificond.tearDownApInterface(TEST_INTERFACE_NAME)).thenReturn(false);

        assertFalse(mWificondControl.tearDownSoftApInterface(TEST_INTERFACE_NAME));
        verify(mWificond).tearDownApInterface(TEST_INTERFACE_NAME);
    }

    /**
     * Verifies that the SoftAp handles are cleared after teardown.
     */
    @Test
    public void testTeardownSoftApInterfaceClearsHandles() throws Exception {
        testTeardownSoftApInterface();

        assertFalse(mWificondControl.registerApListener(
                TEST_INTERFACE_NAME, mSoftApListener));
        verify(mApInterface, never()).registerCallback(any());
    }

    /**
     * Verifies that we can setup concurrent interfaces.
     */
    @Test
    public void testSetupMulitpleInterfaces() throws Exception {
        when(mWificond.createApInterface(TEST_INTERFACE_NAME1)).thenReturn(mApInterface);

        IApInterface returnedApInterface =
                mWificondControl.setupInterfaceForSoftApMode(TEST_INTERFACE_NAME1);
        assertEquals(mApInterface, returnedApInterface);
        verify(mWifiInjector).makeWificond();
        verify(mWifiCondBinder).linkToDeath(any(IBinder.DeathRecipient.class), anyInt());

        verify(mWificond).createClientInterface(TEST_INTERFACE_NAME);
        verify(mWificond).createApInterface(TEST_INTERFACE_NAME1);
    }

    /**
     * Verifies that we can setup concurrent interfaces.
     */
    @Test
    public void testTeardownMulitpleInterfaces() throws Exception {
        testSetupMulitpleInterfaces();
        assertTrue(mWificondControl.tearDownClientInterface(TEST_INTERFACE_NAME));
        assertTrue(mWificondControl.tearDownSoftApInterface(TEST_INTERFACE_NAME1));

        verify(mWificond).tearDownClientInterface(TEST_INTERFACE_NAME);
        verify(mWificond).tearDownApInterface(TEST_INTERFACE_NAME1);
    }

    /**
     * Verifies that tearDownInterfaces() calls wificond.
     */
    @Test
    public void testTearDownInterfaces() throws Exception {
        when(mWifiInjector.makeWificond()).thenReturn(mWificond);
        assertTrue(mWificondControl.tearDownInterfaces());
        verify(mWificond).tearDownInterfaces();
    }

    /**
     * Verifies that tearDownInterfaces() calls unsubscribeScanEvents() when there was
     * a configured client interface.
     */
    @Test
    public void testTearDownInterfacesRemovesScanEventSubscription() throws Exception {
        assertTrue(mWificondControl.tearDownInterfaces());
        verify(mWifiScannerImpl).unsubscribeScanEvents();
    }


    /**
     * Verifies that tearDownInterfaces() returns false when wificond is not started.
     */
    @Test
    public void testTearDownInterfacesErrorWhenWificondIsNotStarterd() throws Exception {
        // Invoke wificond death handler to clear the handle.
        mWificondControl.binderDied();
        mLooper.dispatchAll();
        when(mWifiInjector.makeWificond()).thenReturn(null);
        assertFalse(mWificondControl.tearDownInterfaces());
    }

    /**
     * Verifies that signalPoll() calls wificond.
     */
    @Test
    public void testSignalPoll() throws Exception {
        when(mWifiInjector.makeWificond()).thenReturn(mWificond);
        when(mWificond.createClientInterface(TEST_INTERFACE_NAME)).thenReturn(mClientInterface);

        mWificondControl.setupInterfaceForClientMode(TEST_INTERFACE_NAME);
        mWificondControl.signalPoll(TEST_INTERFACE_NAME);
        verify(mClientInterface).signalPoll();
    }

    /**
     * Verifies that signalPoll() returns null when there is no configured client interface.
     */
    @Test
    public void testSignalPollErrorWhenNoClientInterfaceConfigured() throws Exception {
        when(mWifiInjector.makeWificond()).thenReturn(mWificond);
        when(mWificond.createClientInterface(TEST_INTERFACE_NAME)).thenReturn(mClientInterface);

        // Configure client interface.
        IClientInterface returnedClientInterface =
                mWificondControl.setupInterfaceForClientMode(TEST_INTERFACE_NAME);
        assertEquals(mClientInterface, returnedClientInterface);

        // Tear down interfaces.
        assertTrue(mWificondControl.tearDownInterfaces());

        // Signal poll should fail.
        assertEquals(null, mWificondControl.signalPoll(TEST_INTERFACE_NAME));
    }

    /**
     * Verifies that getTxPacketCounters() calls wificond.
     */
    @Test
    public void testGetTxPacketCounters() throws Exception {
        when(mWifiInjector.makeWificond()).thenReturn(mWificond);
        when(mWificond.createClientInterface(TEST_INTERFACE_NAME)).thenReturn(mClientInterface);

        mWificondControl.setupInterfaceForClientMode(TEST_INTERFACE_NAME);
        mWificondControl.getTxPacketCounters(TEST_INTERFACE_NAME);
        verify(mClientInterface).getPacketCounters();
    }

    /**
     * Verifies that getTxPacketCounters() returns null when there is no configured client
     * interface.
     */
    @Test
    public void testGetTxPacketCountersErrorWhenNoClientInterfaceConfigured() throws Exception {
        when(mWifiInjector.makeWificond()).thenReturn(mWificond);
        when(mWificond.createClientInterface(TEST_INTERFACE_NAME)).thenReturn(mClientInterface);

        // Configure client interface.
        IClientInterface returnedClientInterface =
                mWificondControl.setupInterfaceForClientMode(TEST_INTERFACE_NAME);
        assertEquals(mClientInterface, returnedClientInterface);

        // Tear down interfaces.
        assertTrue(mWificondControl.tearDownInterfaces());

        // Signal poll should fail.
        assertEquals(null, mWificondControl.getTxPacketCounters(TEST_INTERFACE_NAME));
    }

    /**
     * Verifies that getScanResults() returns null when there is no configured client
     * interface.
     */
    @Test
    public void testGetScanResultsErrorWhenNoClientInterfaceConfigured() throws Exception {
        when(mWifiInjector.makeWificond()).thenReturn(mWificond);
        when(mWificond.createClientInterface(TEST_INTERFACE_NAME)).thenReturn(mClientInterface);

        // Configure client interface.
        IClientInterface returnedClientInterface =
                mWificondControl.setupInterfaceForClientMode(TEST_INTERFACE_NAME);
        assertEquals(mClientInterface, returnedClientInterface);

        // Tear down interfaces.
        assertTrue(mWificondControl.tearDownInterfaces());

        // getScanResults should fail.
        assertEquals(0,
                mWificondControl.getScanResults(TEST_INTERFACE_NAME,
                        WificondControl.SCAN_TYPE_SINGLE_SCAN).size());
    }

    /**
     * Verifies that getScanResults() can parse NativeScanResult from wificond correctly,
     */
    @Test
    public void testGetScanResults() throws Exception {
        assertNotNull(mWifiScannerImpl);

        // Mock the returned array of NativeScanResult.
        NativeScanResult[] mockScanResults = {MOCK_NATIVE_SCAN_RESULT};
        when(mWifiScannerImpl.getScanResults()).thenReturn(mockScanResults);

        ArrayList<ScanDetail> returnedScanResults = mWificondControl.getScanResults(
                TEST_INTERFACE_NAME, WificondControl.SCAN_TYPE_SINGLE_SCAN);
        // The test IEs {@link #TEST_INFO_ELEMENT} doesn't contained RSN IE, which means non-EAP
        // AP. So verify carrier network is not checked, since EAP is currently required for a
        // carrier network.
        verify(mCarrierNetworkConfig, never()).isCarrierNetwork(anyString());
        assertEquals(mockScanResults.length, returnedScanResults.size());
        // Since NativeScanResult is organized differently from ScanResult, this only checks
        // a few fields.
        for (int i = 0; i < mockScanResults.length; i++) {
            assertArrayEquals(mockScanResults[i].ssid,
                              returnedScanResults.get(i).getScanResult().SSID.getBytes());
            assertEquals(mockScanResults[i].frequency,
                         returnedScanResults.get(i).getScanResult().frequency);
            assertEquals(mockScanResults[i].tsf,
                         returnedScanResults.get(i).getScanResult().timestamp);
        }
    }

    /**
     * Verifies that scan result's carrier network info {@link ScanResult#isCarrierAp} and
     * {@link ScanResult#getCarrierApEapType} is set appropriated based on the carrier network
     * config.
     *
     * @throws Exception
     */
    @Test
    public void testGetScanResultsForCarrierAp() throws Exception {
        assertNotNull(mWifiScannerImpl);

        // Include RSN IE to indicate EAP key management.
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        out.write(TEST_INFO_ELEMENT_SSID);
        out.write(TEST_INFO_ELEMENT_RSN);
        NativeScanResult nativeScanResult = new NativeScanResult(MOCK_NATIVE_SCAN_RESULT);
        nativeScanResult.infoElement = out.toByteArray();
        when(mWifiScannerImpl.getScanResults()).thenReturn(
                new NativeScanResult[] {nativeScanResult});

        // AP associated with a carrier network.
        int eapType = WifiEnterpriseConfig.Eap.SIM;
        String carrierName = "Test Carrier";
        when(mCarrierNetworkConfig.isCarrierNetwork(new String(nativeScanResult.ssid)))
                .thenReturn(true);
        when(mCarrierNetworkConfig.getNetworkEapType(new String(nativeScanResult.ssid)))
                .thenReturn(eapType);
        when(mCarrierNetworkConfig.getCarrierName(new String(nativeScanResult.ssid)))
                .thenReturn(carrierName);
        ArrayList<ScanDetail> returnedScanResults = mWificondControl.getScanResults(
                TEST_INTERFACE_NAME, WificondControl.SCAN_TYPE_SINGLE_SCAN);
        assertEquals(1, returnedScanResults.size());
        // Verify returned scan result.
        ScanResult scanResult = returnedScanResults.get(0).getScanResult();
        assertArrayEquals(nativeScanResult.ssid, scanResult.SSID.getBytes());
        assertTrue(scanResult.isCarrierAp);
        assertEquals(eapType, scanResult.carrierApEapType);
        assertEquals(carrierName, scanResult.carrierName);
        reset(mCarrierNetworkConfig);

        // AP not associated with a carrier network.
        when(mCarrierNetworkConfig.isCarrierNetwork(new String(nativeScanResult.ssid)))
                .thenReturn(false);
        returnedScanResults = mWificondControl.getScanResults(
                TEST_INTERFACE_NAME, WificondControl.SCAN_TYPE_SINGLE_SCAN);
        assertEquals(1, returnedScanResults.size());
        // Verify returned scan result.
        scanResult = returnedScanResults.get(0).getScanResult();
        assertArrayEquals(nativeScanResult.ssid, scanResult.SSID.getBytes());
        assertFalse(scanResult.isCarrierAp);
        assertEquals(ScanResult.UNSPECIFIED, scanResult.carrierApEapType);
        assertEquals(null, scanResult.carrierName);
    }

    /**
     * Verifies that getScanResults() can parse NativeScanResult from wificond correctly,
     * when there is radio chain info.
     */
    @Test
    public void testGetScanResultsWithRadioChainInfo() throws Exception {
        assertNotNull(mWifiScannerImpl);

        // Mock the returned array of NativeScanResult.
        NativeScanResult nativeScanResult = new NativeScanResult(MOCK_NATIVE_SCAN_RESULT);
        // Add radio chain info
        ArrayList<RadioChainInfo> nativeRadioChainInfos = new ArrayList<RadioChainInfo>() {{
                add(MOCK_NATIVE_RADIO_CHAIN_INFO_1);
                add(MOCK_NATIVE_RADIO_CHAIN_INFO_2);
            }};
        nativeScanResult.radioChainInfos = nativeRadioChainInfos;
        NativeScanResult[] mockScanResults = { nativeScanResult };

        when(mWifiScannerImpl.getScanResults()).thenReturn(mockScanResults);

        ArrayList<ScanDetail> returnedScanResults = mWificondControl.getScanResults(
                TEST_INTERFACE_NAME, WificondControl.SCAN_TYPE_SINGLE_SCAN);
        // The test IEs {@link #TEST_INFO_ELEMENT} doesn't contained RSN IE, which means non-EAP
        // AP. So verify carrier network is not checked, since EAP is currently required for a
        // carrier network.
        verify(mCarrierNetworkConfig, never()).isCarrierNetwork(anyString());
        assertEquals(mockScanResults.length, returnedScanResults.size());
        // Since NativeScanResult is organized differently from ScanResult, this only checks
        // a few fields.
        for (int i = 0; i < mockScanResults.length; i++) {
            assertArrayEquals(mockScanResults[i].ssid,
                    returnedScanResults.get(i).getScanResult().SSID.getBytes());
            assertEquals(mockScanResults[i].frequency,
                    returnedScanResults.get(i).getScanResult().frequency);
            assertEquals(mockScanResults[i].tsf,
                    returnedScanResults.get(i).getScanResult().timestamp);
            assertRadioChainInfosEqual(nativeRadioChainInfos,
                    returnedScanResults.get(i).getScanResult().radioChainInfos);
        }
    }

    /**
     * Verifies that Scan() can convert input parameters to SingleScanSettings correctly.
     */
    @Test
    public void testScan() throws Exception {
        when(mWifiScannerImpl.scan(any(SingleScanSettings.class))).thenReturn(true);
        assertTrue(mWificondControl.scan(
                TEST_INTERFACE_NAME, WifiNative.SCAN_TYPE_LOW_POWER,
                SCAN_FREQ_SET, SCAN_HIDDEN_NETWORK_SSID_LIST));
        verify(mWifiScannerImpl).scan(argThat(new ScanMatcher(
                IWifiScannerImpl.SCAN_TYPE_LOW_POWER,
                SCAN_FREQ_SET, SCAN_HIDDEN_NETWORK_SSID_LIST)));
    }

    /**
     * Verifies that Scan() removes duplicates hiddenSsids passed in from input.
     */
    @Test
    public void testScanWithDuplicateHiddenSsids() throws Exception {
        when(mWifiScannerImpl.scan(any(SingleScanSettings.class))).thenReturn(true);
        // Create a list of hiddenSsid that has a duplicate element
        List<String> hiddenSsidWithDup = new ArrayList<>(SCAN_HIDDEN_NETWORK_SSID_LIST);
        hiddenSsidWithDup.add(SCAN_HIDDEN_NETWORK_SSID_LIST.get(0));
        assertEquals(hiddenSsidWithDup.get(0),
                hiddenSsidWithDup.get(hiddenSsidWithDup.size() - 1));
        // Pass the List with duplicate elements into scan()
        assertTrue(mWificondControl.scan(
                TEST_INTERFACE_NAME, WifiNative.SCAN_TYPE_LOW_POWER,
                SCAN_FREQ_SET, hiddenSsidWithDup));
        // But the argument passed down should have the duplicate removed.
        verify(mWifiScannerImpl).scan(argThat(new ScanMatcher(
                IWifiScannerImpl.SCAN_TYPE_LOW_POWER,
                SCAN_FREQ_SET, SCAN_HIDDEN_NETWORK_SSID_LIST)));
    }

    /**
     * Verifies that Scan() can handle null input parameters correctly.
     */
    @Test
    public void testScanNullParameters() throws Exception {
        when(mWifiScannerImpl.scan(any(SingleScanSettings.class))).thenReturn(true);
        assertTrue(mWificondControl.scan(
                TEST_INTERFACE_NAME, WifiNative.SCAN_TYPE_HIGH_ACCURACY, null, null));
        verify(mWifiScannerImpl).scan(argThat(new ScanMatcher(
                IWifiScannerImpl.SCAN_TYPE_HIGH_ACCURACY, null, null)));
    }

    /**
     * Verifies that Scan() can handle wificond scan failure.
     */
    @Test
    public void testScanFailure() throws Exception {
        when(mWifiScannerImpl.scan(any(SingleScanSettings.class))).thenReturn(false);
        assertFalse(mWificondControl.scan(
                TEST_INTERFACE_NAME, WifiNative.SCAN_TYPE_LOW_LATENCY,
                SCAN_FREQ_SET, SCAN_HIDDEN_NETWORK_SSID_LIST));
        verify(mWifiScannerImpl).scan(any(SingleScanSettings.class));
    }

    /**
     * Verifies that Scan() can handle invalid type.
     */
    @Test
    public void testScanFailureDueToInvalidType() throws Exception {
        assertFalse(mWificondControl.scan(
                TEST_INTERFACE_NAME, 100,
                SCAN_FREQ_SET, SCAN_HIDDEN_NETWORK_SSID_LIST));
        verify(mWifiScannerImpl, never()).scan(any(SingleScanSettings.class));
    }

    /**
     * Verifies that startPnoScan() can convert input parameters to PnoSettings correctly.
     */
    @Test
    public void testStartPnoScan() throws Exception {
        when(mWifiScannerImpl.startPnoScan(any(PnoSettings.class))).thenReturn(true);
        assertTrue(mWificondControl.startPnoScan(TEST_INTERFACE_NAME, TEST_PNO_SETTINGS));
        verify(mWifiScannerImpl).startPnoScan(argThat(new PnoScanMatcher(TEST_PNO_SETTINGS)));
    }

    /**
     * Verifies that stopPnoScan() calls underlying wificond.
     */
    @Test
    public void testStopPnoScan() throws Exception {
        when(mWifiScannerImpl.stopPnoScan()).thenReturn(true);
        assertTrue(mWificondControl.stopPnoScan(TEST_INTERFACE_NAME));
        verify(mWifiScannerImpl).stopPnoScan();
    }

    /**
     * Verifies that stopPnoScan() can handle wificond failure.
     */
    @Test
    public void testStopPnoScanFailure() throws Exception {

        when(mWifiScannerImpl.stopPnoScan()).thenReturn(false);
        assertFalse(mWificondControl.stopPnoScan(TEST_INTERFACE_NAME));
        verify(mWifiScannerImpl).stopPnoScan();
    }

    /**
     * Verifies that WificondControl can invoke WifiMonitor broadcast methods upon scan
     * reuslt event.
     */
    @Test
    public void testScanResultEvent() throws Exception {
        ArgumentCaptor<IScanEvent> messageCaptor = ArgumentCaptor.forClass(IScanEvent.class);
        verify(mWifiScannerImpl).subscribeScanEvents(messageCaptor.capture());
        IScanEvent scanEvent = messageCaptor.getValue();
        assertNotNull(scanEvent);
        scanEvent.OnScanResultReady();

        verify(mWifiMonitor).broadcastScanResultEvent(any(String.class));
    }

    /**
     * Verifies that WificondControl can invoke WifiMonitor broadcast methods upon scan
     * failed event.
     */
    @Test
    public void testScanFailedEvent() throws Exception {

        ArgumentCaptor<IScanEvent> messageCaptor = ArgumentCaptor.forClass(IScanEvent.class);
        verify(mWifiScannerImpl).subscribeScanEvents(messageCaptor.capture());
        IScanEvent scanEvent = messageCaptor.getValue();
        assertNotNull(scanEvent);
        scanEvent.OnScanFailed();

        verify(mWifiMonitor).broadcastScanFailedEvent(any(String.class));
    }

    /**
     * Verifies that WificondControl can invoke WifiMonitor broadcast methods upon pno scan
     * result event.
     */
    @Test
    public void testPnoScanResultEvent() throws Exception {
        ArgumentCaptor<IPnoScanEvent> messageCaptor = ArgumentCaptor.forClass(IPnoScanEvent.class);
        verify(mWifiScannerImpl).subscribePnoScanEvents(messageCaptor.capture());
        IPnoScanEvent pnoScanEvent = messageCaptor.getValue();
        assertNotNull(pnoScanEvent);
        pnoScanEvent.OnPnoNetworkFound();
        verify(mWifiMonitor).broadcastPnoScanResultEvent(any(String.class));
    }

    /**
     * Verifies that WificondControl can invoke WifiMetrics pno scan count methods upon pno event.
     */
    @Test
    public void testPnoScanEventsForMetrics() throws Exception {
        ArgumentCaptor<IPnoScanEvent> messageCaptor = ArgumentCaptor.forClass(IPnoScanEvent.class);
        verify(mWifiScannerImpl).subscribePnoScanEvents(messageCaptor.capture());
        IPnoScanEvent pnoScanEvent = messageCaptor.getValue();
        assertNotNull(pnoScanEvent);

        pnoScanEvent.OnPnoNetworkFound();
        verify(mWifiMetrics).incrementPnoFoundNetworkEventCount();

        pnoScanEvent.OnPnoScanFailed();
        verify(mWifiMetrics).incrementPnoScanFailedCount();

        pnoScanEvent.OnPnoScanOverOffloadStarted();
        verify(mWifiMetrics).incrementPnoScanStartedOverOffloadCount();

        pnoScanEvent.OnPnoScanOverOffloadFailed(0);
        verify(mWifiMetrics).incrementPnoScanFailedOverOffloadCount();
    }

    /**
     * Verifies that startPnoScan() can invoke WifiMetrics pno scan count methods correctly.
     */
    @Test
    public void testStartPnoScanForMetrics() throws Exception {
        when(mWifiScannerImpl.startPnoScan(any(PnoSettings.class))).thenReturn(false);
        assertFalse(mWificondControl.startPnoScan(TEST_INTERFACE_NAME, TEST_PNO_SETTINGS));
        verify(mWifiMetrics).incrementPnoScanStartAttempCount();
        verify(mWifiMetrics).incrementPnoScanFailedCount();
    }

    /**
     * Verifies that abortScan() calls underlying wificond.
     */
    @Test
    public void testAbortScan() throws Exception {
        mWificondControl.abortScan(TEST_INTERFACE_NAME);
        verify(mWifiScannerImpl).abortScan();
    }

    /**
     * Ensures that the Ap interface callbacks are forwarded to the
     * SoftApListener used for starting soft AP.
     */
    @Test
    public void testSoftApListenerInvocation() throws Exception {
        testSetupInterfaceForSoftApMode();

        WifiConfiguration config = new WifiConfiguration();
        config.SSID = new String(TEST_SSID, StandardCharsets.UTF_8);

        when(mApInterface.registerCallback(any())).thenReturn(true);

        final ArgumentCaptor<IApInterfaceEventCallback> apInterfaceCallbackCaptor =
                ArgumentCaptor.forClass(IApInterfaceEventCallback.class);

        assertTrue(mWificondControl.registerApListener(
                TEST_INTERFACE_NAME, mSoftApListener));
        verify(mApInterface).registerCallback(apInterfaceCallbackCaptor.capture());

        int numStations = 5;
        apInterfaceCallbackCaptor.getValue().onNumAssociatedStationsChanged(numStations);
        verify(mSoftApListener).onNumAssociatedStationsChanged(eq(numStations));

        int channelFrequency = 2437;
        int channelBandwidth = IApInterfaceEventCallback.BANDWIDTH_20;
        apInterfaceCallbackCaptor.getValue().onSoftApChannelSwitched(channelFrequency,
                channelBandwidth);
        verify(mSoftApListener).onSoftApChannelSwitched(eq(channelFrequency), eq(channelBandwidth));
    }

    /**
     * Verifies registration and invocation of wificond death handler.
     */
    @Test
    public void testRegisterDeathHandler() throws Exception {
        WifiNative.WificondDeathEventHandler handler =
                mock(WifiNative.WificondDeathEventHandler.class);
        assertTrue(mWificondControl.initialize(handler));
        verify(mWificond).tearDownInterfaces();
        mWificondControl.binderDied();
        mLooper.dispatchAll();
        verify(handler).onDeath();
    }

    /**
     * Verifies handling of wificond death and ensures that all internal state is cleared and
     * handlers are invoked.
     */
    @Test
    public void testDeathHandling() throws Exception {
        WifiNative.WificondDeathEventHandler handler =
                mock(WifiNative.WificondDeathEventHandler.class);
        assertTrue(mWificondControl.initialize(handler));

        testSetupInterfaceForClientMode();

        mWificondControl.binderDied();
        mLooper.dispatchAll();
        verify(handler).onDeath();

        // The handles should be cleared after death.
        assertNull(mWificondControl.getChannelsForBand(WifiScanner.WIFI_BAND_5_GHZ));
        verify(mWificond, never()).getAvailable5gNonDFSChannels();
    }

    /**
     * sendMgmtFrame() should fail if a null callback is passed in.
     */
    @Test
    public void testSendMgmtFrameNullCallback() throws Exception {
        mWificondControl.sendMgmtFrame(TEST_INTERFACE_NAME, TEST_PROBE_FRAME, null, TEST_MCS_RATE);

        verify(mClientInterface, never()).SendMgmtFrame(any(), any(), anyInt());
    }

    /**
     * sendMgmtFrame() should fail if a null frame is passed in.
     */
    @Test
    public void testSendMgmtFrameNullFrame() throws Exception {
        mWificondControl.sendMgmtFrame(TEST_INTERFACE_NAME, null,
                mSendMgmtFrameCallback, TEST_MCS_RATE);

        verify(mClientInterface, never()).SendMgmtFrame(any(), any(), anyInt());
        verify(mSendMgmtFrameCallback).onFailure(anyInt());
    }

    /**
     * sendMgmtFrame() should fail if an interface name that does not exist is passed in.
     */
    @Test
    public void testSendMgmtFrameInvalidInterfaceName() throws Exception {
        mWificondControl.sendMgmtFrame(TEST_INVALID_INTERFACE_NAME, TEST_PROBE_FRAME,
                mSendMgmtFrameCallback, TEST_MCS_RATE);

        verify(mClientInterface, never()).SendMgmtFrame(any(), any(), anyInt());
        verify(mSendMgmtFrameCallback).onFailure(anyInt());
    }

    /**
     * sendMgmtFrame() should fail if it is called a second time before the first call completed.
     */
    @Test
    public void testSendMgmtFrameCalledTwiceBeforeFinished() throws Exception {
        SendMgmtFrameCallback cb1 = mock(SendMgmtFrameCallback.class);
        SendMgmtFrameCallback cb2 = mock(SendMgmtFrameCallback.class);

        mWificondControl.sendMgmtFrame(TEST_INTERFACE_NAME, TEST_PROBE_FRAME, cb1, TEST_MCS_RATE);
        verify(cb1, never()).onFailure(anyInt());
        verify(mClientInterface, times(1))
                .SendMgmtFrame(AdditionalMatchers.aryEq(TEST_PROBE_FRAME),
                        any(), eq(TEST_MCS_RATE));

        mWificondControl.sendMgmtFrame(TEST_INTERFACE_NAME, TEST_PROBE_FRAME, cb2, TEST_MCS_RATE);
        verify(cb2).onFailure(WifiNative.SEND_MGMT_FRAME_ERROR_ALREADY_STARTED);
        // verify SendMgmtFrame() still was only called once i.e. not called again
        verify(mClientInterface, times(1))
                .SendMgmtFrame(any(), any(), anyInt());
    }

    /**
     * Tests that when a RemoteException is triggered on AIDL call, onFailure() is called only once.
     */
    @Test
    public void testSendMgmtFrameThrowsException() throws Exception {
        SendMgmtFrameCallback cb = mock(SendMgmtFrameCallback.class);

        final ArgumentCaptor<ISendMgmtFrameEvent> sendMgmtFrameEventCaptor =
                ArgumentCaptor.forClass(ISendMgmtFrameEvent.class);

        doThrow(new RemoteException()).when(mClientInterface)
                .SendMgmtFrame(any(), sendMgmtFrameEventCaptor.capture(), anyInt());

        final ArgumentCaptor<AlarmManager.OnAlarmListener> alarmListenerCaptor =
                ArgumentCaptor.forClass(AlarmManager.OnAlarmListener.class);
        final ArgumentCaptor<Handler> handlerCaptor = ArgumentCaptor.forClass(Handler.class);
        doNothing().when(mAlarmManager).set(anyInt(), anyLong(), any(),
                alarmListenerCaptor.capture(), handlerCaptor.capture());

        mWificondControl.sendMgmtFrame(TEST_INTERFACE_NAME, TEST_PROBE_FRAME,
                cb, TEST_MCS_RATE);
        mLooper.dispatchAll();

        verify(cb).onFailure(anyInt());
        verify(mAlarmManager).cancel(eq(alarmListenerCaptor.getValue()));

        sendMgmtFrameEventCaptor.getValue().OnFailure(WifiNative.SEND_MGMT_FRAME_ERROR_UNKNOWN);
        mLooper.dispatchAll();

        handlerCaptor.getValue().post(() -> alarmListenerCaptor.getValue().onAlarm());
        mLooper.dispatchAll();

        verifyNoMoreInteractions(cb);
    }

    /**
     * Tests that the onAck() callback is triggered correctly.
     */
    @Test
    public void testSendMgmtFrameSuccess() throws Exception {
        SendMgmtFrameCallback cb = mock(SendMgmtFrameCallback.class);

        final ArgumentCaptor<ISendMgmtFrameEvent> sendMgmtFrameEventCaptor =
                ArgumentCaptor.forClass(ISendMgmtFrameEvent.class);
        doNothing().when(mClientInterface)
                .SendMgmtFrame(any(), sendMgmtFrameEventCaptor.capture(), anyInt());
        final ArgumentCaptor<AlarmManager.OnAlarmListener> alarmListenerCaptor =
                ArgumentCaptor.forClass(AlarmManager.OnAlarmListener.class);
        final ArgumentCaptor<Handler> handlerCaptor = ArgumentCaptor.forClass(Handler.class);
        doNothing().when(mAlarmManager).set(anyInt(), anyLong(), any(),
                alarmListenerCaptor.capture(), handlerCaptor.capture());
        mWificondControl.sendMgmtFrame(TEST_INTERFACE_NAME, TEST_PROBE_FRAME, cb, TEST_MCS_RATE);

        sendMgmtFrameEventCaptor.getValue().OnAck(TEST_SEND_MGMT_FRAME_ELAPSED_TIME_MS);
        mLooper.dispatchAll();
        verify(cb).onAck(eq(TEST_SEND_MGMT_FRAME_ELAPSED_TIME_MS));
        verify(cb, never()).onFailure(anyInt());
        verify(mAlarmManager).cancel(eq(alarmListenerCaptor.getValue()));

        // verify that even if timeout is triggered afterwards, SendMgmtFrameCallback is not
        // triggered again
        handlerCaptor.getValue().post(() -> alarmListenerCaptor.getValue().onAlarm());
        mLooper.dispatchAll();
        verify(cb, times(1)).onAck(anyInt());
        verify(cb, never()).onFailure(anyInt());
    }

    /**
     * Tests that the onFailure() callback is triggered correctly.
     */
    @Test
    public void testSendMgmtFrameFailure() throws Exception {
        SendMgmtFrameCallback cb = mock(SendMgmtFrameCallback.class);

        final ArgumentCaptor<ISendMgmtFrameEvent> sendMgmtFrameEventCaptor =
                ArgumentCaptor.forClass(ISendMgmtFrameEvent.class);
        doNothing().when(mClientInterface)
                .SendMgmtFrame(any(), sendMgmtFrameEventCaptor.capture(), anyInt());
        final ArgumentCaptor<AlarmManager.OnAlarmListener> alarmListenerCaptor =
                ArgumentCaptor.forClass(AlarmManager.OnAlarmListener.class);
        final ArgumentCaptor<Handler> handlerCaptor = ArgumentCaptor.forClass(Handler.class);
        doNothing().when(mAlarmManager).set(anyInt(), anyLong(), any(),
                alarmListenerCaptor.capture(), handlerCaptor.capture());
        mWificondControl.sendMgmtFrame(TEST_INTERFACE_NAME, TEST_PROBE_FRAME, cb, TEST_MCS_RATE);

        sendMgmtFrameEventCaptor.getValue().OnFailure(WifiNative.SEND_MGMT_FRAME_ERROR_UNKNOWN);
        mLooper.dispatchAll();
        verify(cb, never()).onAck(anyInt());
        verify(cb).onFailure(eq(WifiNative.SEND_MGMT_FRAME_ERROR_UNKNOWN));
        verify(mAlarmManager).cancel(eq(alarmListenerCaptor.getValue()));

        // verify that even if timeout is triggered afterwards, SendMgmtFrameCallback is not
        // triggered again
        handlerCaptor.getValue().post(() -> alarmListenerCaptor.getValue().onAlarm());
        mLooper.dispatchAll();
        verify(cb, never()).onAck(anyInt());
        verify(cb, times(1)).onFailure(anyInt());
    }

    /**
     * Tests that the onTimeout() callback is triggered correctly.
     */
    @Test
    public void testSendMgmtFrameTimeout() throws Exception {
        SendMgmtFrameCallback cb = mock(SendMgmtFrameCallback.class);

        final ArgumentCaptor<ISendMgmtFrameEvent> sendMgmtFrameEventCaptor =
                ArgumentCaptor.forClass(ISendMgmtFrameEvent.class);
        doNothing().when(mClientInterface)
                .SendMgmtFrame(any(), sendMgmtFrameEventCaptor.capture(), anyInt());
        final ArgumentCaptor<AlarmManager.OnAlarmListener> alarmListenerCaptor =
                ArgumentCaptor.forClass(AlarmManager.OnAlarmListener.class);
        final ArgumentCaptor<Handler> handlerCaptor = ArgumentCaptor.forClass(Handler.class);
        doNothing().when(mAlarmManager).set(anyInt(), anyLong(), any(),
                alarmListenerCaptor.capture(), handlerCaptor.capture());
        mWificondControl.sendMgmtFrame(TEST_INTERFACE_NAME, TEST_PROBE_FRAME, cb, TEST_MCS_RATE);

        handlerCaptor.getValue().post(() -> alarmListenerCaptor.getValue().onAlarm());
        mLooper.dispatchAll();
        verify(cb, never()).onAck(anyInt());
        verify(cb).onFailure(eq(WifiNative.SEND_MGMT_FRAME_ERROR_TIMEOUT));

        // verify that even if onAck() callback is triggered after timeout,
        // SendMgmtFrameCallback is not triggered again
        sendMgmtFrameEventCaptor.getValue().OnAck(TEST_SEND_MGMT_FRAME_ELAPSED_TIME_MS);
        mLooper.dispatchAll();
        verify(cb, never()).onAck(anyInt());
        verify(cb, times(1)).onFailure(anyInt());
    }

    /**
     * Tests every possible test outcome followed by every other test outcome to ensure that the
     * internal state is reset correctly between calls.
     * i.e. (success, success), (success, failure), (success, timeout),
     * (failure, failure), (failure, success), (failure, timeout),
     * (timeout, timeout), (timeout, success), (timeout, failure)
     *
     * Also tests that internal state is reset correctly after a transient AIDL RemoteException.
     */
    @Test
    public void testSendMgmtFrameMixed() throws Exception {
        testSendMgmtFrameThrowsException();
        testSendMgmtFrameSuccess();
        testSendMgmtFrameSuccess();
        testSendMgmtFrameFailure();
        testSendMgmtFrameFailure();
        testSendMgmtFrameTimeout();
        testSendMgmtFrameTimeout();
        testSendMgmtFrameSuccess();
        testSendMgmtFrameTimeout();
        testSendMgmtFrameFailure();
        testSendMgmtFrameSuccess();
    }

    /**
     * Tests that OnAck() does not perform any non-thread-safe operations on the binder thread.
     *
     * The sequence of instructions are:
     * 1. post onAlarm() onto main thread
     * 2. OnAck()
     * 3. mLooper.dispatchAll()
     *
     * The actual order of execution is:
     * 1. binder thread portion of OnAck()
     * 2. onAlarm() (which purely executes on the main thread)
     * 3. main thread portion of OnAck()
     *
     * If the binder thread portion of OnAck() is not thread-safe, it can possibly mess up
     * onAlarm(). Tests that this does not occur.
     */
    @Test
    public void testSendMgmtFrameTimeoutAckThreadSafe() throws Exception {
        final ArgumentCaptor<ISendMgmtFrameEvent> sendMgmtFrameEventCaptor =
                ArgumentCaptor.forClass(ISendMgmtFrameEvent.class);
        doNothing().when(mClientInterface)
                .SendMgmtFrame(any(), sendMgmtFrameEventCaptor.capture(), anyInt());
        final ArgumentCaptor<AlarmManager.OnAlarmListener> alarmListenerCaptor =
                ArgumentCaptor.forClass(AlarmManager.OnAlarmListener.class);
        final ArgumentCaptor<Handler> handlerCaptor = ArgumentCaptor.forClass(Handler.class);
        doNothing().when(mAlarmManager).set(anyInt(), anyLong(), any(),
                alarmListenerCaptor.capture(), handlerCaptor.capture());
        mWificondControl.sendMgmtFrame(TEST_INTERFACE_NAME, TEST_PROBE_FRAME,
                mSendMgmtFrameCallback, TEST_MCS_RATE);

        // AlarmManager should post the onAlarm() callback onto the handler, but since we are
        // triggering onAlarm() ourselves during the test, manually post onto handler
        handlerCaptor.getValue().post(() -> alarmListenerCaptor.getValue().onAlarm());
        // OnAck posts to the handler
        sendMgmtFrameEventCaptor.getValue().OnAck(TEST_SEND_MGMT_FRAME_ELAPSED_TIME_MS);
        mLooper.dispatchAll();
        verify(mSendMgmtFrameCallback, never()).onAck(anyInt());
        verify(mSendMgmtFrameCallback).onFailure(eq(WifiNative.SEND_MGMT_FRAME_ERROR_TIMEOUT));
    }

    /**
     * See {@link #testSendMgmtFrameTimeoutAckThreadSafe()}. This test replaces OnAck() with
     * OnFailure().
     */
    @Test
    public void testSendMgmtFrameTimeoutFailureThreadSafe() throws Exception {
        final ArgumentCaptor<ISendMgmtFrameEvent> sendMgmtFrameEventCaptor =
                ArgumentCaptor.forClass(ISendMgmtFrameEvent.class);
        doNothing().when(mClientInterface)
                .SendMgmtFrame(any(), sendMgmtFrameEventCaptor.capture(), anyInt());
        final ArgumentCaptor<AlarmManager.OnAlarmListener> alarmListenerCaptor =
                ArgumentCaptor.forClass(AlarmManager.OnAlarmListener.class);
        final ArgumentCaptor<Handler> handlerCaptor = ArgumentCaptor.forClass(Handler.class);
        doNothing().when(mAlarmManager).set(anyInt(), anyLong(), any(),
                alarmListenerCaptor.capture(), handlerCaptor.capture());
        mWificondControl.sendMgmtFrame(TEST_INTERFACE_NAME, TEST_PROBE_FRAME,
                mSendMgmtFrameCallback, TEST_MCS_RATE);

        // AlarmManager should post the onAlarm() callback onto the handler, but since we are
        // triggering onAlarm() ourselves during the test, manually post onto handler
        handlerCaptor.getValue().post(() -> alarmListenerCaptor.getValue().onAlarm());
        // OnFailure posts to the handler
        sendMgmtFrameEventCaptor.getValue().OnFailure(WifiNative.SEND_MGMT_FRAME_ERROR_UNKNOWN);
        mLooper.dispatchAll();
        verify(mSendMgmtFrameCallback).onFailure(eq(WifiNative.SEND_MGMT_FRAME_ERROR_TIMEOUT));
    }

    private void assertRadioChainInfosEqual(
            List<RadioChainInfo> expected, android.net.wifi.ScanResult.RadioChainInfo[] actual) {
        assertEquals(expected.size(), actual.length);
        for (int i = 0; i < actual.length; i++) {
            RadioChainInfo nativeRadioChainInfo =
                    new RadioChainInfo(actual[i].id, actual[i].level);
            assertTrue(expected.contains(nativeRadioChainInfo));
        }
    }

    // Create a ArgumentMatcher which captures a SingleScanSettings parameter and checks if it
    // matches the provided frequency set and ssid set.
    private class ScanMatcher implements ArgumentMatcher<SingleScanSettings> {
        int mExpectedScanType;
        private final Set<Integer> mExpectedFreqs;
        private final List<String> mExpectedSsids;
        ScanMatcher(int expectedScanType, Set<Integer> expectedFreqs, List<String> expectedSsids) {
            this.mExpectedScanType = expectedScanType;
            this.mExpectedFreqs = expectedFreqs;
            this.mExpectedSsids = expectedSsids;
        }

        @Override
        public boolean matches(SingleScanSettings settings) {
            if (settings.scanType != mExpectedScanType) {
                return false;
            }
            ArrayList<ChannelSettings> channelSettings = settings.channelSettings;
            ArrayList<HiddenNetwork> hiddenNetworks = settings.hiddenNetworks;
            if (mExpectedFreqs != null) {
                Set<Integer> freqSet = new HashSet<Integer>();
                for (ChannelSettings channel : channelSettings) {
                    freqSet.add(channel.frequency);
                }
                if (!mExpectedFreqs.equals(freqSet)) {
                    return false;
                }
            } else {
                if (channelSettings != null && channelSettings.size() > 0) {
                    return false;
                }
            }

            if (mExpectedSsids != null) {
                List<String> ssidSet = new ArrayList<String>();
                for (HiddenNetwork network : hiddenNetworks) {
                    ssidSet.add(NativeUtil.encodeSsid(
                            NativeUtil.byteArrayToArrayList(network.ssid)));
                }
                if (!mExpectedSsids.equals(ssidSet)) {
                    return false;
                }

            } else {
                if (hiddenNetworks != null && hiddenNetworks.size() > 0) {
                    return false;
                }
            }
            return true;
        }

        @Override
        public String toString() {
            return "ScanMatcher{mExpectedFreqs=" + mExpectedFreqs
                    + ", mExpectedSsids=" + mExpectedSsids + '}';
        }
    }

    // Create a ArgumentMatcher which captures a PnoSettings parameter and checks if it
    // matches the WifiNative.PnoSettings;
    private class PnoScanMatcher implements ArgumentMatcher<PnoSettings> {
        private final WifiNative.PnoSettings mExpectedPnoSettings;
        PnoScanMatcher(WifiNative.PnoSettings expectedPnoSettings) {
            this.mExpectedPnoSettings = expectedPnoSettings;
        }
        @Override
        public boolean matches(PnoSettings settings) {
            if (mExpectedPnoSettings == null) {
                return false;
            }
            if (settings.intervalMs != mExpectedPnoSettings.periodInMs
                    || settings.min2gRssi != mExpectedPnoSettings.min24GHzRssi
                    || settings.min5gRssi != mExpectedPnoSettings.min5GHzRssi) {
                return false;
            }
            if (settings.pnoNetworks == null || mExpectedPnoSettings.networkList == null) {
                return false;
            }
            if (settings.pnoNetworks.size() != mExpectedPnoSettings.networkList.length) {
                return false;
            }

            for (int i = 0; i < settings.pnoNetworks.size(); i++) {
                if (!mExpectedPnoSettings.networkList[i].ssid.equals(NativeUtil.encodeSsid(
                         NativeUtil.byteArrayToArrayList(settings.pnoNetworks.get(i).ssid)))) {
                    return false;
                }
                boolean isNetworkHidden = (mExpectedPnoSettings.networkList[i].flags
                        & WifiScanner.PnoSettings.PnoNetwork.FLAG_DIRECTED_SCAN) != 0;
                if (isNetworkHidden != settings.pnoNetworks.get(i).isHidden) {
                    return false;
                }
                if (!Arrays.equals(mExpectedPnoSettings.networkList[i].frequencies,
                        settings.pnoNetworks.get(i).frequencies)) {
                    return false;
                }
            }
            return true;
        }

        @Override
        public String toString() {
            return "PnoScanMatcher{" + "mExpectedPnoSettings=" + mExpectedPnoSettings + '}';
        }
    }
}
