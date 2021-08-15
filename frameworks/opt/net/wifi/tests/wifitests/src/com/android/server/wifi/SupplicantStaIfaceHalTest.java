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

import static android.net.wifi.WifiManager.WIFI_FEATURE_DPP;
import static android.net.wifi.WifiManager.WIFI_FEATURE_OWE;
import static android.net.wifi.WifiManager.WIFI_FEATURE_WPA3_SAE;
import static android.net.wifi.WifiManager.WIFI_FEATURE_WPA3_SUITE_B;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyBoolean;
import static org.mockito.Matchers.anyInt;
import static org.mockito.Matchers.anyLong;
import static org.mockito.Matchers.anyShort;
import static org.mockito.Matchers.anyString;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.inOrder;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.reset;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.when;

import android.annotation.NonNull;
import android.app.test.MockAnswerUtil;
import android.content.Context;
import android.hardware.wifi.supplicant.V1_0.ISupplicant;
import android.hardware.wifi.supplicant.V1_0.ISupplicantIface;
import android.hardware.wifi.supplicant.V1_0.ISupplicantStaIface;
import android.hardware.wifi.supplicant.V1_0.ISupplicantStaIfaceCallback;
import android.hardware.wifi.supplicant.V1_0.ISupplicantStaIfaceCallback.BssidChangeReason;
import android.hardware.wifi.supplicant.V1_0.ISupplicantStaNetwork;
import android.hardware.wifi.supplicant.V1_0.IfaceType;
import android.hardware.wifi.supplicant.V1_0.SupplicantStatus;
import android.hardware.wifi.supplicant.V1_0.SupplicantStatusCode;
import android.hardware.wifi.supplicant.V1_0.WpsConfigMethods;
import android.hidl.manager.V1_0.IServiceManager;
import android.hidl.manager.V1_0.IServiceNotification;
import android.net.IpConfiguration;
import android.net.wifi.SupplicantState;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiSsid;
import android.os.IHwBinder;
import android.os.RemoteException;
import android.os.test.TestLooper;
import android.text.TextUtils;
import android.util.SparseArray;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.hotspot2.AnqpEvent;
import com.android.server.wifi.hotspot2.IconEvent;
import com.android.server.wifi.hotspot2.WnmData;
import com.android.server.wifi.util.NativeUtil;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.InOrder;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;

/**
 * Unit tests for SupplicantStaIfaceHal
 */
@SmallTest
public class SupplicantStaIfaceHalTest {
    private static final String TAG = "SupplicantStaIfaceHalTest";
    private static final Map<Integer, String> NETWORK_ID_TO_SSID = new HashMap<Integer, String>() {{
            put(1, "\"ssid1\"");
            put(2, "\"ssid2\"");
            put(3, "\"ssid3\"");
        }};
    private static final int SUPPLICANT_NETWORK_ID = 2;
    private static final String SUPPLICANT_SSID = NETWORK_ID_TO_SSID.get(SUPPLICANT_NETWORK_ID);
    private static final int ROAM_NETWORK_ID = 4;
    private static final String BSSID = "fa:45:23:23:12:12";
    private static final String WLAN0_IFACE_NAME = "wlan0";
    private static final String WLAN1_IFACE_NAME = "wlan1";
    private static final String P2P_IFACE_NAME = "p2p0";
    private static final String ICON_FILE_NAME  = "blahblah";
    private static final int ICON_FILE_SIZE = 72;
    private static final String HS20_URL = "http://blahblah";

    private @Mock IServiceManager mServiceManagerMock;
    private @Mock ISupplicant mISupplicantMock;
    private android.hardware.wifi.supplicant.V1_1.ISupplicant mISupplicantMockV1_1;
    private android.hardware.wifi.supplicant.V1_2.ISupplicant mISupplicantMockV1_2;
    private @Mock ISupplicantIface mISupplicantIfaceMock;
    private @Mock ISupplicantStaIface mISupplicantStaIfaceMock;
    private @Mock android.hardware.wifi.supplicant.V1_1.ISupplicantStaIface
            mISupplicantStaIfaceMockV1_1;
    private @Mock android.hardware.wifi.supplicant.V1_2.ISupplicantStaIface
            mISupplicantStaIfaceMockV1_2;
    private @Mock Context mContext;
    private @Mock WifiMonitor mWifiMonitor;
    private @Mock PropertyService mPropertyService;
    private @Mock SupplicantStaNetworkHal mSupplicantStaNetworkMock;
    private @Mock WifiNative.SupplicantDeathEventHandler mSupplicantHalDeathHandler;

    SupplicantStatus mStatusSuccess;
    SupplicantStatus mStatusFailure;
    ISupplicant.IfaceInfo mStaIface0;
    ISupplicant.IfaceInfo mStaIface1;
    ISupplicant.IfaceInfo mP2pIface;
    ArrayList<ISupplicant.IfaceInfo> mIfaceInfoList;
    ISupplicantStaIfaceCallback mISupplicantStaIfaceCallback;
    android.hardware.wifi.supplicant.V1_1.ISupplicantStaIfaceCallback
            mISupplicantStaIfaceCallbackV1_1;
    android.hardware.wifi.supplicant.V1_2.ISupplicantStaIfaceCallback
            mISupplicantStaIfaceCallbackV1_2;
    private TestLooper mLooper = new TestLooper();
    private SupplicantStaIfaceHal mDut;
    private ArgumentCaptor<IHwBinder.DeathRecipient> mServiceManagerDeathCaptor =
            ArgumentCaptor.forClass(IHwBinder.DeathRecipient.class);
    private ArgumentCaptor<IHwBinder.DeathRecipient> mSupplicantDeathCaptor =
            ArgumentCaptor.forClass(IHwBinder.DeathRecipient.class);
    private ArgumentCaptor<IHwBinder.DeathRecipient> mSupplicantStaIfaceDeathCaptor =
            ArgumentCaptor.forClass(IHwBinder.DeathRecipient.class);
    private ArgumentCaptor<IServiceNotification.Stub> mServiceNotificationCaptor =
            ArgumentCaptor.forClass(IServiceNotification.Stub.class);
    private ArgumentCaptor<Long> mDeathRecipientCookieCaptor = ArgumentCaptor.forClass(Long.class);
    private InOrder mInOrder;

    private class SupplicantStaIfaceHalSpy extends SupplicantStaIfaceHal {
        SupplicantStaIfaceHalSpy() {
            super(mContext, mWifiMonitor, mPropertyService, mLooper.getLooper());
        }

        @Override
        protected IServiceManager getServiceManagerMockable() throws RemoteException {
            return mServiceManagerMock;
        }

        @Override
        protected ISupplicant getSupplicantMockable() throws RemoteException {
            return mISupplicantMock;
        }

        @Override
        protected android.hardware.wifi.supplicant.V1_1.ISupplicant getSupplicantMockableV1_1()
                throws RemoteException {
            return mISupplicantMockV1_1;
        }

        @Override
        protected ISupplicantStaIface getStaIfaceMockable(ISupplicantIface iface) {
            return mISupplicantStaIfaceMock;
        }

        @Override
        protected android.hardware.wifi.supplicant.V1_1.ISupplicantStaIface
                getStaIfaceMockableV1_1(ISupplicantIface iface) {
            return mISupplicantStaIfaceMockV1_1;
        }

        @Override
        protected android.hardware.wifi.supplicant.V1_2.ISupplicantStaIface
                getStaIfaceMockableV1_2(ISupplicantIface iface) {
            return mISupplicantStaIfaceMockV1_2;
        }

        @Override
        protected SupplicantStaNetworkHal getStaNetworkMockable(
                @NonNull String ifaceName,
                ISupplicantStaNetwork iSupplicantStaNetwork) {
            return mSupplicantStaNetworkMock;
        }
    }

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mStatusSuccess = createSupplicantStatus(SupplicantStatusCode.SUCCESS);
        mStatusFailure = createSupplicantStatus(SupplicantStatusCode.FAILURE_UNKNOWN);
        mStaIface0 = createIfaceInfo(IfaceType.STA, WLAN0_IFACE_NAME);
        mStaIface1 = createIfaceInfo(IfaceType.STA, WLAN1_IFACE_NAME);
        mP2pIface = createIfaceInfo(IfaceType.P2P, P2P_IFACE_NAME);

        mIfaceInfoList = new ArrayList<>();
        mIfaceInfoList.add(mStaIface0);
        mIfaceInfoList.add(mStaIface1);
        mIfaceInfoList.add(mP2pIface);

        when(mServiceManagerMock.getTransport(anyString(), anyString()))
                .thenReturn(IServiceManager.Transport.EMPTY);
        when(mServiceManagerMock.linkToDeath(any(IHwBinder.DeathRecipient.class),
                anyLong())).thenReturn(true);
        when(mServiceManagerMock.registerForNotifications(anyString(), anyString(),
                any(IServiceNotification.Stub.class))).thenReturn(true);
        when(mISupplicantMock.linkToDeath(any(IHwBinder.DeathRecipient.class),
                anyLong())).thenReturn(true);
        mDut = new SupplicantStaIfaceHalSpy();
    }

    /**
     * Sunny day scenario for SupplicantStaIfaceHal initialization
     * Asserts successful initialization
     */
    @Test
    public void testInitialize_success() throws Exception {
        executeAndValidateInitializationSequence(false, false, false, false);
    }

    /**
     * Tests the initialization flow, with a RemoteException occurring when 'getInterface' is called
     * Ensures initialization fails.
     */
    @Test
    public void testInitialize_remoteExceptionFailure() throws Exception {
        executeAndValidateInitializationSequence(true, false, false, false);
    }

    /**
     * Tests the initialization flow, with listInterfaces returning 0 interfaces.
     * Ensures failure
     */
    @Test
    public void testInitialize_zeroInterfacesFailure() throws Exception {
        executeAndValidateInitializationSequence(false, true, false, false);
    }

    /**
     * Tests the initialization flow, with a null interface being returned by getInterface.
     * Ensures initialization fails.
     */
    @Test
    public void testInitialize_nullInterfaceFailure() throws Exception {
        executeAndValidateInitializationSequence(false, false, true, false);
    }

    /**
     * Tests the initialization flow, with a callback registration failure.
     * Ensures initialization fails.
     */
    @Test
    public void testInitialize_callbackRegistrationFailure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false, true);
    }

    /**
     * Sunny day scenario for SupplicantStaIfaceHal initialization
     * Asserts successful initialization
     */
    @Test
    public void testInitialize_successV1_1() throws Exception {
        when(mServiceManagerMock.getTransport(eq(android.hardware.wifi.supplicant.V1_1.ISupplicant
                .kInterfaceName), anyString()))
                .thenReturn(IServiceManager.Transport.HWBINDER);
        mISupplicantMockV1_1 = mock(android.hardware.wifi.supplicant.V1_1.ISupplicant.class);
        executeAndValidateInitializationSequenceV1_1(false, false);
    }

    /**
     * Sunny day scenario for SupplicantStaIfaceHal initialization
     * Asserts successful initialization
     */
    @Test
    public void testInitialize_successV1_2() throws Exception {
        when(mServiceManagerMock.getTransport(eq(android.hardware.wifi.supplicant.V1_2.ISupplicant
                .kInterfaceName), anyString()))
                .thenReturn(IServiceManager.Transport.HWBINDER);
        mISupplicantMockV1_2 = mock(android.hardware.wifi.supplicant.V1_2.ISupplicant.class);
        when(mServiceManagerMock.getTransport(eq(android.hardware.wifi.supplicant.V1_1.ISupplicant
                .kInterfaceName), anyString()))
                .thenReturn(IServiceManager.Transport.HWBINDER);
        mISupplicantMockV1_1 = mock(android.hardware.wifi.supplicant.V1_1.ISupplicant.class);
        executeAndValidateInitializationSequenceV1_2();
    }

    /**
     * Tests the initialization flow, with a RemoteException occurring when 'getInterface' is called
     * Ensures initialization fails.
     */
    @Test
    public void testInitialize_remoteExceptionFailureV1_1() throws Exception {
        when(mServiceManagerMock.getTransport(eq(android.hardware.wifi.supplicant.V1_1.ISupplicant
                .kInterfaceName), anyString()))
                .thenReturn(IServiceManager.Transport.HWBINDER);
        mISupplicantMockV1_1 = mock(android.hardware.wifi.supplicant.V1_1.ISupplicant.class);
        executeAndValidateInitializationSequenceV1_1(true, false);
    }

    /**
     * Tests the initialization flow, with a null interface being returned by getInterface.
     * Ensures initialization fails.
     */
    @Test
    public void testInitialize_nullInterfaceFailureV1_1() throws Exception {
        when(mServiceManagerMock.getTransport(eq(android.hardware.wifi.supplicant.V1_1.ISupplicant
                .kInterfaceName), anyString()))
                .thenReturn(IServiceManager.Transport.HWBINDER);
        mISupplicantMockV1_1 = mock(android.hardware.wifi.supplicant.V1_1.ISupplicant.class);
        executeAndValidateInitializationSequenceV1_1(false, true);
    }

    /**
     * Ensures that we do not allow operations on an interface until it's setup.
     */
    @Test
    public void testEnsureOperationFailsUntilSetupInterfaces() throws Exception {
        executeAndValidateInitializationSequence(false, false, false, false);

        // Ensure that the cancel wps operation is failed because wlan1 interface is not yet setup.
        assertFalse(mDut.cancelWps(WLAN1_IFACE_NAME));
        verify(mISupplicantStaIfaceMock, never()).cancelWps();

        // Now setup the wlan1 interface and Ensure that the cancel wps operation is successful.
        assertTrue(mDut.setupIface(WLAN1_IFACE_NAME));
        when(mISupplicantStaIfaceMock.cancelWps()).thenReturn(mStatusSuccess);
        assertTrue(mDut.cancelWps(WLAN1_IFACE_NAME));
        verify(mISupplicantStaIfaceMock).cancelWps();
    }

    /**
     * Ensures that reject addition of an existing iface.
     */
    @Test
    public void testDuplicateSetupIfaceV1_1_Fails() throws Exception {
        when(mServiceManagerMock.getTransport(eq(android.hardware.wifi.supplicant.V1_1.ISupplicant
                .kInterfaceName), anyString()))
                .thenReturn(IServiceManager.Transport.HWBINDER);
        mISupplicantMockV1_1 = mock(android.hardware.wifi.supplicant.V1_1.ISupplicant.class);
        executeAndValidateInitializationSequenceV1_1(false, false);

        // Trying setting up the wlan0 interface again & ensure it fails.
        assertFalse(mDut.setupIface(WLAN0_IFACE_NAME));
        verifyNoMoreInteractions(mISupplicantMockV1_1);
    }

    /**
     * Sunny day scenario for SupplicantStaIfaceHal interface teardown.
     */
    @Test
    public void testTeardownInterface() throws Exception {
        testInitialize_success();
        assertTrue(mDut.teardownIface(WLAN0_IFACE_NAME));

        // Ensure that the cancel wps operation is failed because there are no interfaces setup.
        assertFalse(mDut.cancelWps(WLAN0_IFACE_NAME));
        verify(mISupplicantStaIfaceMock, never()).cancelWps();
    }

    /**
     * Sunny day scenario for SupplicantStaIfaceHal interface teardown.
     */
    @Test
    public void testTeardownInterfaceV1_1() throws Exception {
        testInitialize_successV1_1();

        when(mISupplicantMockV1_1.removeInterface(any())).thenReturn(mStatusSuccess);
        assertTrue(mDut.teardownIface(WLAN0_IFACE_NAME));
        verify(mISupplicantMockV1_1).removeInterface(any());

        // Ensure that the cancel wps operation is failed because there are no interfaces setup.
        assertFalse(mDut.cancelWps(WLAN0_IFACE_NAME));
        verify(mISupplicantStaIfaceMock, never()).cancelWps();
    }

    /**
     * Ensures that we reject removal of an invalid iface.
     */
    @Test
    public void testInvalidTeardownInterfaceV1_1_Fails() throws Exception {
        assertFalse(mDut.teardownIface(WLAN0_IFACE_NAME));
        verifyNoMoreInteractions(mISupplicantMock);
    }

    /**
     * Sunny day scenario for SupplicantStaIfaceHal initialization
     * Asserts successful initialization of second interface
     */
    @Test
    public void testSetupTwoInterfaces() throws Exception {
        executeAndValidateInitializationSequence(false, false, false, false);
        assertTrue(mDut.setupIface(WLAN1_IFACE_NAME));
    }

    /**
     * Sunny day scenario for SupplicantStaIfaceHal interface teardown.
     * Asserts successful initialization of second interface
     */
    @Test
    public void testTeardownTwoInterfaces() throws Exception {
        testSetupTwoInterfaces();
        assertTrue(mDut.teardownIface(WLAN0_IFACE_NAME));
        assertTrue(mDut.teardownIface(WLAN1_IFACE_NAME));

        // Ensure that the cancel wps operation is failed because there are no interfaces setup.
        assertFalse(mDut.cancelWps(WLAN0_IFACE_NAME));
        verify(mISupplicantStaIfaceMock, never()).cancelWps();
    }


    /**
     * Tests the loading of networks using {@link SupplicantStaNetworkHal}.
     * Fills up only the SSID field of configs and uses it as a configKey as well.
     */
    @Test
    public void testLoadNetworks() throws Exception {
        executeAndValidateInitializationSequence();
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public void answer(ISupplicantStaIface.listNetworksCallback cb) {
                cb.onValues(mStatusSuccess, new ArrayList<>(NETWORK_ID_TO_SSID.keySet()));
            }
        }).when(mISupplicantStaIfaceMock)
                .listNetworks(any(ISupplicantStaIface.listNetworksCallback.class));
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public void answer(final int networkId, ISupplicantStaIface.getNetworkCallback cb) {
                // Reset the |mSupplicantStaNetwork| mock for each network.
                doAnswer(new MockAnswerUtil.AnswerWithArguments() {
                    public boolean answer(
                            WifiConfiguration config, Map<String, String> networkExtra) {
                        config.SSID = NETWORK_ID_TO_SSID.get(networkId);
                        config.networkId = networkId;
                        networkExtra.put(
                                SupplicantStaNetworkHal.ID_STRING_KEY_CONFIG_KEY, config.SSID);
                        return true;
                    }
                }).when(mSupplicantStaNetworkMock)
                        .loadWifiConfiguration(any(WifiConfiguration.class), any(Map.class));
                cb.onValues(mStatusSuccess, mock(ISupplicantStaNetwork.class));
                return;
            }
        }).when(mISupplicantStaIfaceMock)
                .getNetwork(anyInt(), any(ISupplicantStaIface.getNetworkCallback.class));

        Map<String, WifiConfiguration> configs = new HashMap<>();
        SparseArray<Map<String, String>> extras = new SparseArray<>();
        assertTrue(mDut.loadNetworks(WLAN0_IFACE_NAME, configs, extras));

        assertEquals(3, configs.size());
        assertEquals(3, extras.size());
        for (Map.Entry<Integer, String> network : NETWORK_ID_TO_SSID.entrySet()) {
            WifiConfiguration config = configs.get(network.getValue());
            assertTrue(config != null);
            assertEquals(network.getKey(), Integer.valueOf(config.networkId));
            assertEquals(network.getValue(), config.SSID);
            assertEquals(IpConfiguration.IpAssignment.DHCP, config.getIpAssignment());
            assertEquals(IpConfiguration.ProxySettings.NONE, config.getProxySettings());
        }
    }

    /**
     * Tests the loading of networks using {@link SupplicantStaNetworkHal} removes any networks
     * with duplicate config key.
     * Fills up only the SSID field of configs and uses it as a configKey as well.
     */
    @Test
    public void testLoadNetworksRemovesDuplicates() throws Exception {
        // Network ID which will have the same config key as the previous one.
        final int duplicateNetworkId = 2;
        final int toRemoveNetworkId = duplicateNetworkId - 1;
        executeAndValidateInitializationSequence();
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public void answer(ISupplicantStaIface.listNetworksCallback cb) {
                cb.onValues(mStatusSuccess, new ArrayList<>(NETWORK_ID_TO_SSID.keySet()));
            }
        }).when(mISupplicantStaIfaceMock)
                .listNetworks(any(ISupplicantStaIface.listNetworksCallback.class));
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public SupplicantStatus answer(int id) {
                return mStatusSuccess;
            }
        }).when(mISupplicantStaIfaceMock).removeNetwork(eq(toRemoveNetworkId));
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public void answer(final int networkId, ISupplicantStaIface.getNetworkCallback cb) {
                // Reset the |mSupplicantStaNetwork| mock for each network.
                doAnswer(new MockAnswerUtil.AnswerWithArguments() {
                    public boolean answer(
                            WifiConfiguration config, Map<String, String> networkExtra) {
                        config.SSID = NETWORK_ID_TO_SSID.get(networkId);
                        config.networkId = networkId;
                        // Duplicate network gets the same config key as the to removed one.
                        if (networkId == duplicateNetworkId) {
                            networkExtra.put(
                                    SupplicantStaNetworkHal.ID_STRING_KEY_CONFIG_KEY,
                                    NETWORK_ID_TO_SSID.get(toRemoveNetworkId));
                        } else {
                            networkExtra.put(
                                    SupplicantStaNetworkHal.ID_STRING_KEY_CONFIG_KEY,
                                    NETWORK_ID_TO_SSID.get(networkId));
                        }
                        return true;
                    }
                }).when(mSupplicantStaNetworkMock)
                        .loadWifiConfiguration(any(WifiConfiguration.class), any(Map.class));
                cb.onValues(mStatusSuccess, mock(ISupplicantStaNetwork.class));
                return;
            }
        }).when(mISupplicantStaIfaceMock)
                .getNetwork(anyInt(), any(ISupplicantStaIface.getNetworkCallback.class));

        Map<String, WifiConfiguration> configs = new HashMap<>();
        SparseArray<Map<String, String>> extras = new SparseArray<>();
        assertTrue(mDut.loadNetworks(WLAN0_IFACE_NAME, configs, extras));

        assertEquals(2, configs.size());
        assertEquals(2, extras.size());
        for (Map.Entry<Integer, String> network : NETWORK_ID_TO_SSID.entrySet()) {
            if (network.getKey() == toRemoveNetworkId) {
                continue;
            }
            WifiConfiguration config;
            // Duplicate network gets the same config key as the to removed one. So, use that to
            // lookup the map.
            if (network.getKey() == duplicateNetworkId) {
                config = configs.get(NETWORK_ID_TO_SSID.get(toRemoveNetworkId));
            } else {
                config = configs.get(network.getValue());
            }
            assertTrue(config != null);
            assertEquals(network.getKey(), Integer.valueOf(config.networkId));
            assertEquals(network.getValue(), config.SSID);
            assertEquals(IpConfiguration.IpAssignment.DHCP, config.getIpAssignment());
            assertEquals(IpConfiguration.ProxySettings.NONE, config.getProxySettings());
        }
    }

    /**
     * Tests the failure to load networks because of listNetworks failure.
     */
    @Test
    public void testLoadNetworksFailedDueToListNetworks() throws Exception {
        executeAndValidateInitializationSequence();
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public void answer(ISupplicantStaIface.listNetworksCallback cb) {
                cb.onValues(mStatusFailure, null);
            }
        }).when(mISupplicantStaIfaceMock)
                .listNetworks(any(ISupplicantStaIface.listNetworksCallback.class));

        Map<String, WifiConfiguration> configs = new HashMap<>();
        SparseArray<Map<String, String>> extras = new SparseArray<>();
        assertFalse(mDut.loadNetworks(WLAN0_IFACE_NAME, configs, extras));
    }

    /**
     * Tests the failure to load networks because of getNetwork failure.
     */
    @Test
    public void testLoadNetworksFailedDueToGetNetwork() throws Exception {
        executeAndValidateInitializationSequence();
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public void answer(ISupplicantStaIface.listNetworksCallback cb) {
                cb.onValues(mStatusSuccess, new ArrayList<>(NETWORK_ID_TO_SSID.keySet()));
            }
        }).when(mISupplicantStaIfaceMock)
                .listNetworks(any(ISupplicantStaIface.listNetworksCallback.class));
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public void answer(final int networkId, ISupplicantStaIface.getNetworkCallback cb) {
                cb.onValues(mStatusFailure, mock(ISupplicantStaNetwork.class));
                return;
            }
        }).when(mISupplicantStaIfaceMock)
                .getNetwork(anyInt(), any(ISupplicantStaIface.getNetworkCallback.class));

        Map<String, WifiConfiguration> configs = new HashMap<>();
        SparseArray<Map<String, String>> extras = new SparseArray<>();
        assertFalse(mDut.loadNetworks(WLAN0_IFACE_NAME, configs, extras));
    }

    /**
     * Tests the failure to load networks because of loadWifiConfiguration failure.
     */
    @Test
    public void testLoadNetworksFailedDueToLoadWifiConfiguration() throws Exception {
        executeAndValidateInitializationSequence();
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public void answer(ISupplicantStaIface.listNetworksCallback cb) {
                cb.onValues(mStatusSuccess, new ArrayList<>(NETWORK_ID_TO_SSID.keySet()));
            }
        }).when(mISupplicantStaIfaceMock)
                .listNetworks(any(ISupplicantStaIface.listNetworksCallback.class));
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public void answer(final int networkId, ISupplicantStaIface.getNetworkCallback cb) {
                cb.onValues(mStatusSuccess, mock(ISupplicantStaNetwork.class));
                return;
            }
        }).when(mISupplicantStaIfaceMock)
                .getNetwork(anyInt(), any(ISupplicantStaIface.getNetworkCallback.class));
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public boolean answer(WifiConfiguration config, Map<String, String> networkExtra) {
                return false;
            }
        }).when(mSupplicantStaNetworkMock)
                .loadWifiConfiguration(any(WifiConfiguration.class), any(Map.class));

        Map<String, WifiConfiguration> configs = new HashMap<>();
        SparseArray<Map<String, String>> extras = new SparseArray<>();
        assertTrue(mDut.loadNetworks(WLAN0_IFACE_NAME, configs, extras));
        assertTrue(configs.isEmpty());
    }

    /**
     * Tests the failure to load networks because of loadWifiConfiguration exception.
     */
    @Test
    public void testLoadNetworksFailedDueToExceptionInLoadWifiConfiguration() throws Exception {
        executeAndValidateInitializationSequence();
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public void answer(ISupplicantStaIface.listNetworksCallback cb) {
                cb.onValues(mStatusSuccess, new ArrayList<>(NETWORK_ID_TO_SSID.keySet()));
            }
        }).when(mISupplicantStaIfaceMock)
                .listNetworks(any(ISupplicantStaIface.listNetworksCallback.class));
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public void answer(final int networkId, ISupplicantStaIface.getNetworkCallback cb) {
                cb.onValues(mStatusSuccess, mock(ISupplicantStaNetwork.class));
                return;
            }
        }).when(mISupplicantStaIfaceMock)
                .getNetwork(anyInt(), any(ISupplicantStaIface.getNetworkCallback.class));
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public boolean answer(WifiConfiguration config, Map<String, String> networkExtra)
                    throws Exception {
                throw new IllegalArgumentException();
            }
        }).when(mSupplicantStaNetworkMock)
                .loadWifiConfiguration(any(WifiConfiguration.class), any(Map.class));

        Map<String, WifiConfiguration> configs = new HashMap<>();
        SparseArray<Map<String, String>> extras = new SparseArray<>();
        assertTrue(mDut.loadNetworks(WLAN0_IFACE_NAME, configs, extras));
        assertTrue(configs.isEmpty());
    }

    /**
     * Tests connection to a specified network with empty existing network.
     */
    @Test
    public void testConnectWithEmptyExistingNetwork() throws Exception {
        executeAndValidateInitializationSequence();
        executeAndValidateConnectSequence(0, false);
    }

    @Test
    public void testConnectToNetworkWithDifferentConfigReplacesNetworkInSupplicant()
            throws Exception {
        executeAndValidateInitializationSequence();
        WifiConfiguration config = executeAndValidateConnectSequence(
                SUPPLICANT_NETWORK_ID, false);
        // Reset mocks for mISupplicantStaIfaceMock because we finished the first connection.
        reset(mISupplicantStaIfaceMock);
        setupMocksForConnectSequence(true /*haveExistingNetwork*/);
        // Make this network different by changing SSID.
        config.SSID = "AnDifferentSSID";
        assertTrue(mDut.connectToNetwork(WLAN0_IFACE_NAME, config));
        verify(mISupplicantStaIfaceMock).removeNetwork(SUPPLICANT_NETWORK_ID);
        verify(mISupplicantStaIfaceMock)
                .addNetwork(any(ISupplicantStaIface.addNetworkCallback.class));
    }

    @Test
    public void connectToNetworkWithSameNetworkDoesNotRemoveNetworkFromSupplicant()
            throws Exception {
        executeAndValidateInitializationSequence();
        WifiConfiguration config = executeAndValidateConnectSequence(SUPPLICANT_NETWORK_ID, false);
        // Reset mocks for mISupplicantStaIfaceMock because we finished the first connection.
        reset(mISupplicantStaIfaceMock);
        setupMocksForConnectSequence(true /*haveExistingNetwork*/);
        assertTrue(mDut.connectToNetwork(WLAN0_IFACE_NAME, config));
        verify(mISupplicantStaIfaceMock, never()).removeNetwork(anyInt());
        verify(mISupplicantStaIfaceMock, never())
                .addNetwork(any(ISupplicantStaIface.addNetworkCallback.class));
    }

    @Test
    public void connectToNetworkWithSameNetworkButDifferentBssidUpdatesNetworkFromSupplicant()
            throws Exception {
        executeAndValidateInitializationSequence();
        WifiConfiguration config = executeAndValidateConnectSequence(SUPPLICANT_NETWORK_ID, false);
        String testBssid = "11:22:33:44:55:66";
        when(mSupplicantStaNetworkMock.setBssid(eq(testBssid))).thenReturn(true);

        // Reset mocks for mISupplicantStaIfaceMock because we finished the first connection.
        reset(mISupplicantStaIfaceMock);
        setupMocksForConnectSequence(true /*haveExistingNetwork*/);
        // Change the BSSID and connect to the same network.
        assertFalse(TextUtils.equals(
                testBssid, config.getNetworkSelectionStatus().getNetworkSelectionBSSID()));
        config.getNetworkSelectionStatus().setNetworkSelectionBSSID(testBssid);
        assertTrue(mDut.connectToNetwork(WLAN0_IFACE_NAME, config));
        verify(mSupplicantStaNetworkMock).setBssid(eq(testBssid));
        verify(mISupplicantStaIfaceMock, never()).removeNetwork(anyInt());
        verify(mISupplicantStaIfaceMock, never())
                .addNetwork(any(ISupplicantStaIface.addNetworkCallback.class));
    }

    /**
     * Tests connection to a specified network failure due to network add.
     */
    @Test
    public void testConnectFailureDueToNetworkAddFailure() throws Exception {
        executeAndValidateInitializationSequence();
        setupMocksForConnectSequence(false);
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public void answer(ISupplicantStaIface.addNetworkCallback cb) throws RemoteException {
                cb.onValues(mStatusFailure, mock(ISupplicantStaNetwork.class));
                return;
            }
        }).when(mISupplicantStaIfaceMock).addNetwork(
                any(ISupplicantStaIface.addNetworkCallback.class));

        assertFalse(mDut.connectToNetwork(WLAN0_IFACE_NAME, createTestWifiConfiguration()));
    }

    /**
     * Tests connection to a specified network failure due to network save.
     */
    @Test
    public void testConnectFailureDueToNetworkSaveFailure() throws Exception {
        executeAndValidateInitializationSequence();
        setupMocksForConnectSequence(true);

        when(mSupplicantStaNetworkMock.saveWifiConfiguration(any(WifiConfiguration.class)))
                .thenReturn(false);

        assertFalse(mDut.connectToNetwork(WLAN0_IFACE_NAME, createTestWifiConfiguration()));
        // We should have removed the existing network once before connection and once more
        // on failure to save network configuration.
        verify(mISupplicantStaIfaceMock, times(2)).removeNetwork(anyInt());
    }

    /**
     * Tests connection to a specified network failure due to exception in network save.
     */
    @Test
    public void testConnectFailureDueToNetworkSaveException() throws Exception {
        executeAndValidateInitializationSequence();
        setupMocksForConnectSequence(true);

        doThrow(new IllegalArgumentException("Some error!!!"))
                .when(mSupplicantStaNetworkMock).saveWifiConfiguration(
                        any(WifiConfiguration.class));

        assertFalse(mDut.connectToNetwork(WLAN0_IFACE_NAME, createTestWifiConfiguration()));
        // We should have removed the existing network once before connection and once more
        // on failure to save network configuration.
        verify(mISupplicantStaIfaceMock, times(2)).removeNetwork(anyInt());
    }

    /**
     * Tests connection to a specified network failure due to network select.
     */
    @Test
    public void testConnectFailureDueToNetworkSelectFailure() throws Exception {
        executeAndValidateInitializationSequence();
        setupMocksForConnectSequence(false);

        when(mSupplicantStaNetworkMock.select()).thenReturn(false);

        assertFalse(mDut.connectToNetwork(WLAN0_IFACE_NAME, createTestWifiConfiguration()));
    }

    /**
     * Tests roaming to the same network as the currently connected one.
     */
    @Test
    public void testRoamToSameNetwork() throws Exception {
        executeAndValidateInitializationSequence();
        executeAndValidateRoamSequence(true);
        assertTrue(mDut.connectToNetwork(WLAN0_IFACE_NAME, createTestWifiConfiguration()));
    }

    /**
     * Tests roaming to a different network.
     */
    @Test
    public void testRoamToDifferentNetwork() throws Exception {
        executeAndValidateInitializationSequence();
        executeAndValidateRoamSequence(false);
    }

    /**
     * Tests roaming failure because of unable to set bssid.
     */
    @Test
    public void testRoamFailureDueToBssidSet() throws Exception {
        executeAndValidateInitializationSequence();
        int connectedNetworkId = 5;
        executeAndValidateConnectSequence(connectedNetworkId, false);
        when(mSupplicantStaNetworkMock.setBssid(anyString())).thenReturn(false);

        WifiConfiguration roamingConfig = new WifiConfiguration();
        roamingConfig.networkId = connectedNetworkId;
        roamingConfig.getNetworkSelectionStatus().setNetworkSelectionBSSID("45:34:23:23:ab:ed");
        assertFalse(mDut.roamToNetwork(WLAN0_IFACE_NAME, roamingConfig));
    }

    /**
     * Tests removal of all configured networks from wpa_supplicant.
     */
    @Test
    public void testRemoveAllNetworks() throws Exception {
        executeAndValidateInitializationSequence();
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public void answer(ISupplicantStaIface.listNetworksCallback cb) {
                cb.onValues(mStatusSuccess, new ArrayList<>(NETWORK_ID_TO_SSID.keySet()));
            }
        }).when(mISupplicantStaIfaceMock)
                .listNetworks(any(ISupplicantStaIface.listNetworksCallback.class));
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public SupplicantStatus answer(int id) {
                assertTrue(NETWORK_ID_TO_SSID.containsKey(id));
                return mStatusSuccess;
            }
        }).when(mISupplicantStaIfaceMock).removeNetwork(anyInt());

        assertTrue(mDut.removeAllNetworks(WLAN0_IFACE_NAME));
        verify(mISupplicantStaIfaceMock, times(NETWORK_ID_TO_SSID.size())).removeNetwork(anyInt());
    }

    /**
     * Remove all networks while connected, verify that the current network info is resetted.
     */
    @Test
    public void testRemoveAllNetworksWhileConnected() throws Exception {
        String testBssid = "11:22:33:44:55:66";
        when(mSupplicantStaNetworkMock.setBssid(eq(testBssid))).thenReturn(true);

        executeAndValidateInitializationSequence();

        // Connect to a network and verify current network is set.
        executeAndValidateConnectSequence(4, false);
        assertTrue(mDut.setCurrentNetworkBssid(WLAN0_IFACE_NAME, testBssid));
        verify(mSupplicantStaNetworkMock).setBssid(eq(testBssid));
        reset(mSupplicantStaNetworkMock);

        // Remove all networks and verify current network info is resetted.
        assertTrue(mDut.removeAllNetworks(WLAN0_IFACE_NAME));
        assertFalse(mDut.setCurrentNetworkBssid(WLAN0_IFACE_NAME, testBssid));
        verify(mSupplicantStaNetworkMock, never()).setBssid(eq(testBssid));
    }

    /**
     * Tests roaming failure because of unable to reassociate.
     */
    @Test
    public void testRoamFailureDueToReassociate() throws Exception {
        executeAndValidateInitializationSequence();
        int connectedNetworkId = 5;
        executeAndValidateConnectSequence(connectedNetworkId, false);

        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public SupplicantStatus answer() throws RemoteException {
                return mStatusFailure;
            }
        }).when(mISupplicantStaIfaceMock).reassociate();
        when(mSupplicantStaNetworkMock.setBssid(anyString())).thenReturn(true);

        WifiConfiguration roamingConfig = new WifiConfiguration();
        roamingConfig.networkId = connectedNetworkId;
        roamingConfig.getNetworkSelectionStatus().setNetworkSelectionBSSID("45:34:23:23:ab:ed");
        assertFalse(mDut.roamToNetwork(WLAN0_IFACE_NAME, roamingConfig));
    }

    /**
     * Tests the retrieval of WPS NFC token.
     */
    @Test
    public void testGetCurrentNetworkWpsNfcConfigurationToken() throws Exception {
        String token = "45adbc1";
        when(mSupplicantStaNetworkMock.getWpsNfcConfigurationToken()).thenReturn(token);

        executeAndValidateInitializationSequence();
        // Return null when not connected to the network.
        assertTrue(mDut.getCurrentNetworkWpsNfcConfigurationToken(WLAN0_IFACE_NAME) == null);
        verify(mSupplicantStaNetworkMock, never()).getWpsNfcConfigurationToken();
        executeAndValidateConnectSequence(4, false);
        assertEquals(token, mDut.getCurrentNetworkWpsNfcConfigurationToken(WLAN0_IFACE_NAME));
        verify(mSupplicantStaNetworkMock).getWpsNfcConfigurationToken();
    }

    /**
     * Tests the setting of BSSID.
     */
    @Test
    public void testSetCurrentNetworkBssid() throws Exception {
        String bssidStr = "34:34:12:12:12:90";
        when(mSupplicantStaNetworkMock.setBssid(eq(bssidStr))).thenReturn(true);

        executeAndValidateInitializationSequence();
        // Fail when not connected to a network.
        assertFalse(mDut.setCurrentNetworkBssid(WLAN0_IFACE_NAME, bssidStr));
        verify(mSupplicantStaNetworkMock, never()).setBssid(eq(bssidStr));
        executeAndValidateConnectSequence(4, false);
        assertTrue(mDut.setCurrentNetworkBssid(WLAN0_IFACE_NAME, bssidStr));
        verify(mSupplicantStaNetworkMock).setBssid(eq(bssidStr));
    }

    /**
     * Tests the sending identity response for the current network.
     */
    @Test
    public void testSetCurrentNetworkEapIdentityResponse() throws Exception {
        String identity = "blah@blah.com";
        String encryptedIdentity = "blah2@blah.com";
        when(mSupplicantStaNetworkMock.sendNetworkEapIdentityResponse(eq(identity),
                eq(encryptedIdentity)))
                .thenReturn(true);

        executeAndValidateInitializationSequence();
        // Fail when not connected to a network.
        assertFalse(mDut.sendCurrentNetworkEapIdentityResponse(WLAN0_IFACE_NAME, identity,
                encryptedIdentity));
        verify(mSupplicantStaNetworkMock, never()).sendNetworkEapIdentityResponse(eq(identity),
                eq(encryptedIdentity));
        executeAndValidateConnectSequence(4, false);
        assertTrue(mDut.sendCurrentNetworkEapIdentityResponse(WLAN0_IFACE_NAME, identity,
                encryptedIdentity));
        verify(mSupplicantStaNetworkMock).sendNetworkEapIdentityResponse(eq(identity),
                eq(encryptedIdentity));
    }

    /**
     * Tests the getting of anonymous identity for the current network.
     */
    @Test
    public void testGetCurrentNetworkEapAnonymousIdentity() throws Exception {
        String anonymousIdentity = "aaa@bbb.ccc";
        when(mSupplicantStaNetworkMock.fetchEapAnonymousIdentity())
                .thenReturn(anonymousIdentity);
        executeAndValidateInitializationSequence();

        // Return null when not connected to the network.
        assertEquals(null, mDut.getCurrentNetworkEapAnonymousIdentity(WLAN0_IFACE_NAME));
        executeAndValidateConnectSequence(4, false);
        // Return anonymous identity for the current network.
        assertEquals(
                anonymousIdentity, mDut.getCurrentNetworkEapAnonymousIdentity(WLAN0_IFACE_NAME));
    }

    /**
     * Tests the sending gsm auth response for the current network.
     */
    @Test
    public void testSetCurrentNetworkEapSimGsmAuthResponse() throws Exception {
        String params = "test";
        when(mSupplicantStaNetworkMock.sendNetworkEapSimGsmAuthResponse(eq(params)))
                .thenReturn(true);

        executeAndValidateInitializationSequence();
        // Fail when not connected to a network.
        assertFalse(mDut.sendCurrentNetworkEapSimGsmAuthResponse(WLAN0_IFACE_NAME, params));
        verify(mSupplicantStaNetworkMock, never()).sendNetworkEapSimGsmAuthResponse(eq(params));
        executeAndValidateConnectSequence(4, false);
        assertTrue(mDut.sendCurrentNetworkEapSimGsmAuthResponse(WLAN0_IFACE_NAME, params));
        verify(mSupplicantStaNetworkMock).sendNetworkEapSimGsmAuthResponse(eq(params));
    }

    /**
     * Tests the sending umts auth response for the current network.
     */
    @Test
    public void testSetCurrentNetworkEapSimUmtsAuthResponse() throws Exception {
        String params = "test";
        when(mSupplicantStaNetworkMock.sendNetworkEapSimUmtsAuthResponse(eq(params)))
                .thenReturn(true);

        executeAndValidateInitializationSequence();
        // Fail when not connected to a network.
        assertFalse(mDut.sendCurrentNetworkEapSimUmtsAuthResponse(WLAN0_IFACE_NAME, params));
        verify(mSupplicantStaNetworkMock, never()).sendNetworkEapSimUmtsAuthResponse(eq(params));
        executeAndValidateConnectSequence(4, false);
        assertTrue(mDut.sendCurrentNetworkEapSimUmtsAuthResponse(WLAN0_IFACE_NAME, params));
        verify(mSupplicantStaNetworkMock).sendNetworkEapSimUmtsAuthResponse(eq(params));
    }

    /**
     * Tests the sending umts auts response for the current network.
     */
    @Test
    public void testSetCurrentNetworkEapSimUmtsAutsResponse() throws Exception {
        String params = "test";
        when(mSupplicantStaNetworkMock.sendNetworkEapSimUmtsAutsResponse(eq(params)))
                .thenReturn(true);

        executeAndValidateInitializationSequence();
        // Fail when not connected to a network.
        assertFalse(mDut.sendCurrentNetworkEapSimUmtsAutsResponse(WLAN0_IFACE_NAME, params));
        verify(mSupplicantStaNetworkMock, never()).sendNetworkEapSimUmtsAutsResponse(eq(params));
        executeAndValidateConnectSequence(4, false);
        assertTrue(mDut.sendCurrentNetworkEapSimUmtsAutsResponse(WLAN0_IFACE_NAME, params));
        verify(mSupplicantStaNetworkMock).sendNetworkEapSimUmtsAutsResponse(eq(params));
    }

    /**
     * Tests the setting of WPS device type.
     */
    @Test
    public void testSetWpsDeviceType() throws Exception {
        String validDeviceTypeStr = "10-0050F204-5";
        byte[] expectedDeviceType = { 0x0, 0xa, 0x0, 0x50, (byte) 0xf2, 0x04, 0x0, 0x05};
        String invalidDeviceType1Str = "10-02050F204-5";
        String invalidDeviceType2Str = "10-0050F204-534";
        when(mISupplicantStaIfaceMock.setWpsDeviceType(any(byte[].class)))
                .thenReturn(mStatusSuccess);

        executeAndValidateInitializationSequence();

        // This should work.
        assertTrue(mDut.setWpsDeviceType(WLAN0_IFACE_NAME, validDeviceTypeStr));
        verify(mISupplicantStaIfaceMock).setWpsDeviceType(eq(expectedDeviceType));

        // This should not work
        assertFalse(mDut.setWpsDeviceType(WLAN0_IFACE_NAME, invalidDeviceType1Str));
        // This should not work
        assertFalse(mDut.setWpsDeviceType(WLAN0_IFACE_NAME, invalidDeviceType2Str));
    }

    /**
     * Tests the setting of WPS config methods.
     */
    @Test
    public void testSetWpsConfigMethods() throws Exception {
        String validConfigMethodsStr = "physical_display virtual_push_button";
        Short expectedConfigMethods =
                WpsConfigMethods.PHY_DISPLAY | WpsConfigMethods.VIRT_PUSHBUTTON;
        String invalidConfigMethodsStr = "physical_display virtual_push_button test";
        when(mISupplicantStaIfaceMock.setWpsConfigMethods(anyShort())).thenReturn(mStatusSuccess);

        executeAndValidateInitializationSequence();

        // This should work.
        assertTrue(mDut.setWpsConfigMethods(WLAN0_IFACE_NAME, validConfigMethodsStr));
        verify(mISupplicantStaIfaceMock).setWpsConfigMethods(eq(expectedConfigMethods));

        // This should throw an illegal argument exception.
        try {
            assertFalse(mDut.setWpsConfigMethods(WLAN0_IFACE_NAME, invalidConfigMethodsStr));
        } catch (IllegalArgumentException e) {
            return;
        }
        assertTrue(false);
    }

    /**
     * Tests the handling of ANQP done callback.
     * Note: Since the ANQP element parsing methods are static, this can only test the negative test
     * where all the parsing fails because the data is empty. It'll be non-trivial and unnecessary
     * to test out the parsing logic here.
     */
    @Test
    public void testAnqpDoneCallback() throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mISupplicantStaIfaceCallback);
        byte[] bssid = NativeUtil.macAddressToByteArray(BSSID);
        mISupplicantStaIfaceCallback.onAnqpQueryDone(
                bssid, new ISupplicantStaIfaceCallback.AnqpData(),
                new ISupplicantStaIfaceCallback.Hs20AnqpData());

        ArgumentCaptor<AnqpEvent> anqpEventCaptor = ArgumentCaptor.forClass(AnqpEvent.class);
        verify(mWifiMonitor).broadcastAnqpDoneEvent(
                eq(WLAN0_IFACE_NAME), anqpEventCaptor.capture());
        assertEquals(
                ByteBufferReader.readInteger(
                        ByteBuffer.wrap(bssid), ByteOrder.BIG_ENDIAN, bssid.length),
                anqpEventCaptor.getValue().getBssid());
    }

    /**
     * Tests the handling of Icon done callback.
     */
    @Test
    public void testIconDoneCallback() throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mISupplicantStaIfaceCallback);

        byte[] bssid = NativeUtil.macAddressToByteArray(BSSID);
        byte[] iconData = new byte[ICON_FILE_SIZE];
        new Random().nextBytes(iconData);
        mISupplicantStaIfaceCallback.onHs20IconQueryDone(
                bssid, ICON_FILE_NAME, NativeUtil.byteArrayToArrayList(iconData));

        ArgumentCaptor<IconEvent> iconEventCaptor = ArgumentCaptor.forClass(IconEvent.class);
        verify(mWifiMonitor).broadcastIconDoneEvent(
                eq(WLAN0_IFACE_NAME), iconEventCaptor.capture());
        assertEquals(
                ByteBufferReader.readInteger(
                        ByteBuffer.wrap(bssid), ByteOrder.BIG_ENDIAN, bssid.length),
                iconEventCaptor.getValue().getBSSID());
        assertEquals(ICON_FILE_NAME, iconEventCaptor.getValue().getFileName());
        assertArrayEquals(iconData, iconEventCaptor.getValue().getData());
    }

    /**
     * Tests the handling of HS20 subscription remediation callback.
     */
    @Test
    public void testHs20SubscriptionRemediationCallback() throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mISupplicantStaIfaceCallback);

        byte[] bssid = NativeUtil.macAddressToByteArray(BSSID);
        byte osuMethod = ISupplicantStaIfaceCallback.OsuMethod.OMA_DM;
        mISupplicantStaIfaceCallback.onHs20SubscriptionRemediation(
                bssid, osuMethod, HS20_URL);

        ArgumentCaptor<WnmData> wnmDataCaptor = ArgumentCaptor.forClass(WnmData.class);
        verify(mWifiMonitor).broadcastWnmEvent(eq(WLAN0_IFACE_NAME), wnmDataCaptor.capture());
        assertEquals(
                ByteBufferReader.readInteger(
                        ByteBuffer.wrap(bssid), ByteOrder.BIG_ENDIAN, bssid.length),
                wnmDataCaptor.getValue().getBssid());
        assertEquals(osuMethod, wnmDataCaptor.getValue().getMethod());
        assertEquals(HS20_URL, wnmDataCaptor.getValue().getUrl());
    }

    /**
     * Tests the handling of HS20 deauth imminent callback.
     */
    @Test
    public void testHs20DeauthImminentCallbackWithEssReasonCode() throws Exception {
        executeAndValidateHs20DeauthImminentCallback(true);
    }

    /**
     * Tests the handling of HS20 deauth imminent callback.
     */
    @Test
    public void testHs20DeauthImminentCallbackWithNonEssReasonCode() throws Exception {
        executeAndValidateHs20DeauthImminentCallback(false);

    }

    /**
     * Tests the handling of state change notification without any configured network.
     */
    @Test
    public void testStateChangeCallbackWithNoConfiguredNetwork() throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mISupplicantStaIfaceCallback);

        mISupplicantStaIfaceCallback.onStateChanged(
                ISupplicantStaIfaceCallback.State.INACTIVE,
                NativeUtil.macAddressToByteArray(BSSID), SUPPLICANT_NETWORK_ID,
                NativeUtil.decodeSsid(SUPPLICANT_SSID));

        // Can't compare WifiSsid instances because they lack an equals.
        verify(mWifiMonitor).broadcastSupplicantStateChangeEvent(
                eq(WLAN0_IFACE_NAME), eq(WifiConfiguration.INVALID_NETWORK_ID),
                any(WifiSsid.class), eq(BSSID), eq(SupplicantState.INACTIVE));
    }

    /**
     * Tests the handling of state change notification to associated after configuring a network.
     */
    @Test
    public void testStateChangeToAssociatedCallback() throws Exception {
        executeAndValidateInitializationSequence();
        int frameworkNetworkId = 6;
        executeAndValidateConnectSequence(frameworkNetworkId, false);
        assertNotNull(mISupplicantStaIfaceCallback);

        mISupplicantStaIfaceCallback.onStateChanged(
                ISupplicantStaIfaceCallback.State.ASSOCIATED,
                NativeUtil.macAddressToByteArray(BSSID), SUPPLICANT_NETWORK_ID,
                NativeUtil.decodeSsid(SUPPLICANT_SSID));

        verify(mWifiMonitor).broadcastSupplicantStateChangeEvent(
                eq(WLAN0_IFACE_NAME), eq(frameworkNetworkId),
                any(WifiSsid.class), eq(BSSID), eq(SupplicantState.ASSOCIATED));
    }

    /**
     * Tests the handling of state change notification to completed after configuring a network.
     */
    @Test
    public void testStateChangeToCompletedCallback() throws Exception {
        InOrder wifiMonitorInOrder = inOrder(mWifiMonitor);
        executeAndValidateInitializationSequence();
        int frameworkNetworkId = 6;
        executeAndValidateConnectSequence(frameworkNetworkId, false);
        assertNotNull(mISupplicantStaIfaceCallback);

        mISupplicantStaIfaceCallback.onStateChanged(
                ISupplicantStaIfaceCallback.State.COMPLETED,
                NativeUtil.macAddressToByteArray(BSSID), SUPPLICANT_NETWORK_ID,
                NativeUtil.decodeSsid(SUPPLICANT_SSID));

        wifiMonitorInOrder.verify(mWifiMonitor).broadcastNetworkConnectionEvent(
                eq(WLAN0_IFACE_NAME), eq(frameworkNetworkId), eq(BSSID));
        wifiMonitorInOrder.verify(mWifiMonitor).broadcastSupplicantStateChangeEvent(
                eq(WLAN0_IFACE_NAME), eq(frameworkNetworkId),
                any(WifiSsid.class), eq(BSSID), eq(SupplicantState.COMPLETED));
    }

    /**
     * Tests the handling of network disconnected notification.
     */
    @Test
    public void testDisconnectedCallback() throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mISupplicantStaIfaceCallback);

        int reasonCode = 5;
        mISupplicantStaIfaceCallback.onDisconnected(
                NativeUtil.macAddressToByteArray(BSSID), true, reasonCode);
        verify(mWifiMonitor).broadcastNetworkDisconnectionEvent(
                eq(WLAN0_IFACE_NAME), eq(1), eq(reasonCode), eq(BSSID));

        mISupplicantStaIfaceCallback.onDisconnected(
                NativeUtil.macAddressToByteArray(BSSID), false, reasonCode);
        verify(mWifiMonitor).broadcastNetworkDisconnectionEvent(
                eq(WLAN0_IFACE_NAME), eq(0), eq(reasonCode), eq(BSSID));
    }

    /**
     * Tests the handling of incorrect network passwords.
     */
    @Test
    public void testAuthFailurePassword() throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mISupplicantStaIfaceCallback);

        int reasonCode = 3;
        mISupplicantStaIfaceCallback.onDisconnected(
                NativeUtil.macAddressToByteArray(BSSID), true, reasonCode);
        verify(mWifiMonitor, times(0))
                .broadcastAuthenticationFailureEvent(any(), anyInt(), anyInt());

        mISupplicantStaIfaceCallback.onDisconnected(
                NativeUtil.macAddressToByteArray(BSSID), false, reasonCode);
        verify(mWifiMonitor, times(0))
                .broadcastAuthenticationFailureEvent(any(), anyInt(), anyInt());

        mISupplicantStaIfaceCallback.onStateChanged(
                ISupplicantStaIfaceCallback.State.FOURWAY_HANDSHAKE,
                NativeUtil.macAddressToByteArray(BSSID),
                SUPPLICANT_NETWORK_ID,
                NativeUtil.decodeSsid(SUPPLICANT_SSID));
        mISupplicantStaIfaceCallback.onDisconnected(
                NativeUtil.macAddressToByteArray(BSSID), true, reasonCode);
        mISupplicantStaIfaceCallback.onDisconnected(
                NativeUtil.macAddressToByteArray(BSSID), false, reasonCode);

        verify(mWifiMonitor, times(2))
                .broadcastAuthenticationFailureEvent(eq(WLAN0_IFACE_NAME),
                        eq(WifiManager.ERROR_AUTH_FAILURE_WRONG_PSWD), eq(-1));
    }

    /**
     * Tests the handling of incorrect network passwords for WPA3-Personal networks
     */
    @Test
    public void testAuthRejectionPassword() throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mISupplicantStaIfaceCallback);

        executeAndValidateConnectSequenceWithKeyMgmt(SUPPLICANT_NETWORK_ID, false,
                WifiConfiguration.KeyMgmt.SAE);

        int statusCode = ISupplicantStaIfaceCallback.StatusCode.UNSPECIFIED_FAILURE;

        mISupplicantStaIfaceCallback.onAssociationRejected(
                NativeUtil.macAddressToByteArray(BSSID), statusCode, false);
        verify(mWifiMonitor).broadcastAuthenticationFailureEvent(eq(WLAN0_IFACE_NAME),
                eq(WifiManager.ERROR_AUTH_FAILURE_WRONG_PSWD), eq(-1));
        verify(mWifiMonitor).broadcastAssociationRejectionEvent(
                eq(WLAN0_IFACE_NAME), eq(statusCode), eq(false), eq(BSSID));
    }

    /**
     * Tests the handling of incorrect network passwords, edge case.
     *
     * If the network is removed during 4-way handshake, do not call it a password mismatch.
     */
    @Test
    public void testNetworkRemovedDuring4way() throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mISupplicantStaIfaceCallback);

        int reasonCode = 3;

        mISupplicantStaIfaceCallback.onStateChanged(
                ISupplicantStaIfaceCallback.State.FOURWAY_HANDSHAKE,
                NativeUtil.macAddressToByteArray(BSSID),
                SUPPLICANT_NETWORK_ID,
                NativeUtil.decodeSsid(SUPPLICANT_SSID));
        mISupplicantStaIfaceCallback.onNetworkRemoved(SUPPLICANT_NETWORK_ID);
        mISupplicantStaIfaceCallback.onDisconnected(
                NativeUtil.macAddressToByteArray(BSSID), true, reasonCode);
        verify(mWifiMonitor, times(0)).broadcastAuthenticationFailureEvent(any(), anyInt(),
                anyInt());
    }

     /**
      * Tests the handling of incorrect network passwords, edge case.
      *
      * If the disconnect reason is "IE in 4way differs", do not call it a password mismatch.
      */
    @Test
    public void testIeDiffers() throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mISupplicantStaIfaceCallback);

        int reasonCode = ISupplicantStaIfaceCallback.ReasonCode.IE_IN_4WAY_DIFFERS;

        mISupplicantStaIfaceCallback.onStateChanged(
                ISupplicantStaIfaceCallback.State.FOURWAY_HANDSHAKE,
                NativeUtil.macAddressToByteArray(BSSID),
                SUPPLICANT_NETWORK_ID,
                NativeUtil.decodeSsid(SUPPLICANT_SSID));
        mISupplicantStaIfaceCallback.onDisconnected(
                NativeUtil.macAddressToByteArray(BSSID), true, reasonCode);
        verify(mWifiMonitor, times(0)).broadcastAuthenticationFailureEvent(any(), anyInt(),
                anyInt());
    }

    /**
     * Tests the handling of eap failure during disconnect.
     */
    @Test
    public void testEapFailure() throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mISupplicantStaIfaceCallback);

        int reasonCode = ISupplicantStaIfaceCallback.ReasonCode.IEEE_802_1X_AUTH_FAILED;
        mISupplicantStaIfaceCallback.onDisconnected(
                NativeUtil.macAddressToByteArray(BSSID), false, reasonCode);
        verify(mWifiMonitor, times(0)).broadcastAuthenticationFailureEvent(any(), anyInt(),
                anyInt());
    }

    /**
     * Tests the handling of association rejection notification.
     */
    @Test
    public void testAssociationRejectionCallback() throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mISupplicantStaIfaceCallback);

        int statusCode = 7;
        mISupplicantStaIfaceCallback.onAssociationRejected(
                NativeUtil.macAddressToByteArray(BSSID), statusCode, false);
        verify(mWifiMonitor).broadcastAssociationRejectionEvent(
                eq(WLAN0_IFACE_NAME), eq(statusCode), eq(false), eq(BSSID));
    }

    /**
     * Tests the handling of authentification timeout notification.
     */
    @Test
    public void testAuthenticationTimeoutCallback() throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mISupplicantStaIfaceCallback);

        mISupplicantStaIfaceCallback.onAuthenticationTimeout(
                NativeUtil.macAddressToByteArray(BSSID));
        verify(mWifiMonitor).broadcastAuthenticationFailureEvent(eq(WLAN0_IFACE_NAME),
                eq(WifiManager.ERROR_AUTH_FAILURE_TIMEOUT), eq(-1));
    }

    /**
     * Tests the handling of bssid change notification.
     */
    @Test
    public void testBssidChangedCallback() throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mISupplicantStaIfaceCallback);

        mISupplicantStaIfaceCallback.onBssidChanged(
                BssidChangeReason.ASSOC_START, NativeUtil.macAddressToByteArray(BSSID));
        verify(mWifiMonitor).broadcastTargetBssidEvent(eq(WLAN0_IFACE_NAME), eq(BSSID));
        verify(mWifiMonitor, never()).broadcastAssociatedBssidEvent(
                eq(WLAN0_IFACE_NAME), eq(BSSID));

        reset(mWifiMonitor);
        mISupplicantStaIfaceCallback.onBssidChanged(
                BssidChangeReason.ASSOC_COMPLETE, NativeUtil.macAddressToByteArray(BSSID));
        verify(mWifiMonitor, never()).broadcastTargetBssidEvent(eq(WLAN0_IFACE_NAME), eq(BSSID));
        verify(mWifiMonitor).broadcastAssociatedBssidEvent(eq(WLAN0_IFACE_NAME), eq(BSSID));

        reset(mWifiMonitor);
        mISupplicantStaIfaceCallback.onBssidChanged(
                BssidChangeReason.DISASSOC, NativeUtil.macAddressToByteArray(BSSID));
        verify(mWifiMonitor, never()).broadcastTargetBssidEvent(eq(WLAN0_IFACE_NAME), eq(BSSID));
        verify(mWifiMonitor, never()).broadcastAssociatedBssidEvent(
                eq(WLAN0_IFACE_NAME), eq(BSSID));
    }

    /**
     * Tests the handling of EAP failure notification.
     */
    @Test
    public void testEapFailureCallback() throws Exception {
        int eapFailureCode = WifiNative.EAP_SIM_VENDOR_SPECIFIC_CERT_EXPIRED;
        testInitialize_successV1_1();
        assertNotNull(mISupplicantStaIfaceCallbackV1_1);

        mISupplicantStaIfaceCallbackV1_1.onEapFailure_1_1(eapFailureCode);
        verify(mWifiMonitor).broadcastAuthenticationFailureEvent(
                eq(WLAN0_IFACE_NAME), eq(WifiManager.ERROR_AUTH_FAILURE_EAP_FAILURE),
                eq(eapFailureCode));
    }

    /**
     * Tests the handling of Wps success notification.
     */
    @Test
    public void testWpsSuccessCallback() throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mISupplicantStaIfaceCallback);

        mISupplicantStaIfaceCallback.onWpsEventSuccess();
        verify(mWifiMonitor).broadcastWpsSuccessEvent(eq(WLAN0_IFACE_NAME));
    }

    /**
     * Tests the handling of Wps fail notification.
     */
    @Test
    public void testWpsFailureCallback() throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mISupplicantStaIfaceCallback);

        short cfgError = ISupplicantStaIfaceCallback.WpsConfigError.MULTIPLE_PBC_DETECTED;
        short errorInd = ISupplicantStaIfaceCallback.WpsErrorIndication.SECURITY_WEP_PROHIBITED;
        mISupplicantStaIfaceCallback.onWpsEventFail(
                NativeUtil.macAddressToByteArray(BSSID), cfgError, errorInd);
        verify(mWifiMonitor).broadcastWpsFailEvent(eq(WLAN0_IFACE_NAME),
                eq((int) cfgError), eq((int) errorInd));
    }

    /**
     * Tests the handling of Wps fail notification.
     */
    @Test
    public void testWpsTimeoutCallback() throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mISupplicantStaIfaceCallback);

        short cfgError = ISupplicantStaIfaceCallback.WpsConfigError.MSG_TIMEOUT;
        short errorInd = ISupplicantStaIfaceCallback.WpsErrorIndication.NO_ERROR;
        mISupplicantStaIfaceCallback.onWpsEventFail(
                NativeUtil.macAddressToByteArray(BSSID), cfgError, errorInd);
        verify(mWifiMonitor).broadcastWpsTimeoutEvent(eq(WLAN0_IFACE_NAME));
    }

    /**
     * Tests the handling of Wps pbc overlap notification.
     */
    @Test
    public void testWpsPbcOverlapCallback() throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mISupplicantStaIfaceCallback);

        mISupplicantStaIfaceCallback.onWpsEventPbcOverlap();
        verify(mWifiMonitor).broadcastWpsOverlapEvent(eq(WLAN0_IFACE_NAME));
    }

    /**
     * Tests the handling of service manager death notification.
     */
    @Test
    public void testServiceManagerDeathCallback() throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mServiceManagerDeathCaptor.getValue());
        assertTrue(mDut.isInitializationComplete());
        assertTrue(mDut.registerDeathHandler(mSupplicantHalDeathHandler));

        mServiceManagerDeathCaptor.getValue().serviceDied(5L);
        mLooper.dispatchAll();

        assertFalse(mDut.isInitializationComplete());
        verify(mWifiMonitor).broadcastSupplicantDisconnectionEvent(eq(WLAN0_IFACE_NAME));
        verify(mSupplicantHalDeathHandler).onDeath();
    }

    /**
     * Tests the handling of supplicant death notification.
     */
    @Test
    public void testSupplicantDeathCallback() throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mSupplicantDeathCaptor.getValue());
        assertTrue(mDut.isInitializationComplete());
        assertTrue(mDut.registerDeathHandler(mSupplicantHalDeathHandler));

        mSupplicantDeathCaptor.getValue().serviceDied(mDeathRecipientCookieCaptor.getValue());
        mLooper.dispatchAll();

        assertFalse(mDut.isInitializationComplete());
        verify(mWifiMonitor).broadcastSupplicantDisconnectionEvent(eq(WLAN0_IFACE_NAME));
        verify(mSupplicantHalDeathHandler).onDeath();
    }

    /**
     * Tests the handling of supplicant death notification.
     */
    @Test
    public void testSupplicantStaleDeathCallback() throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mSupplicantDeathCaptor.getValue());
        assertTrue(mDut.isInitializationComplete());
        assertTrue(mDut.registerDeathHandler(mSupplicantHalDeathHandler));

        mSupplicantDeathCaptor.getValue().serviceDied(mDeathRecipientCookieCaptor.getValue() - 1);
        mLooper.dispatchAll();

        assertTrue(mDut.isInitializationComplete());
        verify(mWifiMonitor, never()).broadcastSupplicantDisconnectionEvent(eq(WLAN0_IFACE_NAME));
        verify(mSupplicantHalDeathHandler, never()).onDeath();
    }

    /**
     * When wpa_supplicant is dead, we could end up getting a remote exception on a hwbinder call
     * and then the death notification.
     */
    @Test
    public void testHandleRemoteExceptionAndDeathNotification() throws Exception {
        executeAndValidateInitializationSequence();
        assertTrue(mDut.registerDeathHandler(mSupplicantHalDeathHandler));
        assertTrue(mDut.isInitializationComplete());

        // Throw remote exception on hwbinder call.
        when(mISupplicantStaIfaceMock.setPowerSave(anyBoolean()))
                .thenThrow(new RemoteException());
        assertFalse(mDut.setPowerSave(WLAN0_IFACE_NAME, true));
        verify(mISupplicantStaIfaceMock).setPowerSave(true);

        // Check that remote exception cleared all internal state.
        assertFalse(mDut.isInitializationComplete());

        // Ensure that further calls fail because the remote exception clears any state.
        assertFalse(mDut.setPowerSave(WLAN0_IFACE_NAME, true));
        //.. No call to ISupplicantStaIface object

        // Now trigger a death notification and ensure it's handled.
        assertNotNull(mSupplicantDeathCaptor.getValue());
        mSupplicantDeathCaptor.getValue().serviceDied(mDeathRecipientCookieCaptor.getValue());
        mLooper.dispatchAll();

        // External death notification fires only once!
        verify(mSupplicantHalDeathHandler).onDeath();
    }

    /**
     * Tests the setting of log level.
     */
    @Test
    public void testSetLogLevel() throws Exception {
        when(mISupplicantMock.setDebugParams(anyInt(), anyBoolean(), anyBoolean()))
                .thenReturn(mStatusSuccess);

        // Fail before initialization is performed.
        assertFalse(mDut.setLogLevel(true));

        executeAndValidateInitializationSequence();

        // This should work.
        assertTrue(mDut.setLogLevel(true));
        verify(mISupplicantMock)
                .setDebugParams(eq(ISupplicant.DebugLevel.DEBUG), eq(false), eq(false));
    }

    /**
     * Tests the setting of concurrency priority.
     */
    @Test
    public void testConcurrencyPriority() throws Exception {
        when(mISupplicantMock.setConcurrencyPriority(anyInt())).thenReturn(mStatusSuccess);

        // Fail before initialization is performed.
        assertFalse(mDut.setConcurrencyPriority(false));

        executeAndValidateInitializationSequence();

        // This should work.
        assertTrue(mDut.setConcurrencyPriority(false));
        verify(mISupplicantMock).setConcurrencyPriority(eq(IfaceType.P2P));
        assertTrue(mDut.setConcurrencyPriority(true));
        verify(mISupplicantMock).setConcurrencyPriority(eq(IfaceType.STA));
    }

    /**
     * Tests the start of wps registrar.
     */
    @Test
    public void testStartWpsRegistrar() throws Exception {
        when(mISupplicantStaIfaceMock.startWpsRegistrar(any(byte[].class), anyString()))
                .thenReturn(mStatusSuccess);

        // Fail before initialization is performed.
        assertFalse(mDut.startWpsRegistrar(WLAN0_IFACE_NAME, null, null));

        executeAndValidateInitializationSequence();

        assertFalse(mDut.startWpsRegistrar(WLAN0_IFACE_NAME, null, null));
        verify(mISupplicantStaIfaceMock, never()).startWpsRegistrar(any(byte[].class), anyString());

        assertFalse(mDut.startWpsRegistrar(WLAN0_IFACE_NAME, new String(), "452233"));
        verify(mISupplicantStaIfaceMock, never()).startWpsRegistrar(any(byte[].class), anyString());

        assertTrue(mDut.startWpsRegistrar(WLAN0_IFACE_NAME, "45:23:12:12:12:98", "562535"));
        verify(mISupplicantStaIfaceMock).startWpsRegistrar(any(byte[].class), anyString());
    }

    /**
     * Tests the start of wps PBC.
     */
    @Test
    public void testStartWpsPbc() throws Exception {
        when(mISupplicantStaIfaceMock.startWpsPbc(any(byte[].class))).thenReturn(mStatusSuccess);
        String bssid = "45:23:12:12:12:98";
        byte[] bssidBytes = {0x45, 0x23, 0x12, 0x12, 0x12, (byte) 0x98};
        byte[] anyBssidBytes = {0, 0, 0, 0, 0, 0};

        // Fail before initialization is performed.
        assertFalse(mDut.startWpsPbc(WLAN0_IFACE_NAME, bssid));
        verify(mISupplicantStaIfaceMock, never()).startWpsPbc(any(byte[].class));

        executeAndValidateInitializationSequence();

        assertTrue(mDut.startWpsPbc(WLAN0_IFACE_NAME, bssid));
        verify(mISupplicantStaIfaceMock).startWpsPbc(eq(bssidBytes));

        assertTrue(mDut.startWpsPbc(WLAN0_IFACE_NAME, null));
        verify(mISupplicantStaIfaceMock).startWpsPbc(eq(anyBssidBytes));
    }

    /**
     * Tests country code setter
     */
    @Test
    public void testSetCountryCode() throws Exception {
        when(mISupplicantStaIfaceMock.setCountryCode(any(byte[].class))).thenReturn(mStatusSuccess);
        String testCountryCode = "US";

        // Fail before initialization is performed.
        assertFalse(mDut.setCountryCode(WLAN0_IFACE_NAME, testCountryCode));
        verify(mISupplicantStaIfaceMock, never()).setCountryCode(any(byte[].class));

        executeAndValidateInitializationSequence();

        assertTrue(mDut.setCountryCode(WLAN0_IFACE_NAME, testCountryCode));
        verify(mISupplicantStaIfaceMock).setCountryCode(eq(testCountryCode.getBytes()));

        // Bad input values should fail the call.
        reset(mISupplicantStaIfaceMock);

        assertFalse(mDut.setCountryCode(WLAN0_IFACE_NAME, null));
        verify(mISupplicantStaIfaceMock, never()).setCountryCode(any(byte[].class));

        assertFalse(mDut.setCountryCode(WLAN0_IFACE_NAME, "U"));
        verify(mISupplicantStaIfaceMock, never()).setCountryCode(any(byte[].class));
    }

    /**
     * Tests the start daemon for V1_0 service.
     */
    @Test
    public void testStartDaemonV1_0() throws Exception {
        executeAndValidateInitializationSequence();
        assertTrue(mDut.startDaemon());
        verify(mPropertyService).set(
                SupplicantStaIfaceHal.INIT_START_PROPERTY, SupplicantStaIfaceHal.INIT_SERVICE_NAME);
    }

    /**
     * Tests the start daemon for V1_1 service.
     */
    @Test
    public void testStartDaemonV1_1() throws Exception {
        when(mServiceManagerMock.getTransport(eq(android.hardware.wifi.supplicant.V1_1.ISupplicant
                .kInterfaceName), anyString()))
                .thenReturn(IServiceManager.Transport.HWBINDER);
        mISupplicantMockV1_1 = mock(android.hardware.wifi.supplicant.V1_1.ISupplicant.class);

        executeAndValidateInitializationSequenceV1_1(false, false);
        assertTrue(mDut.startDaemon());
        verify(mPropertyService, never()).set(any(), any());
    }

    /**
     * Tests the terminate for V1_0 service.
     */
    @Test
    public void testTerminateV1_0() throws Exception {
        executeAndValidateInitializationSequence();
        mDut.terminate();
        verify(mPropertyService).set(
                SupplicantStaIfaceHal.INIT_STOP_PROPERTY, SupplicantStaIfaceHal.INIT_SERVICE_NAME);
    }

    /**
     * Tests the start daemon for V1_1 service.
     */
    @Test
    public void testTerminateV1_1() throws Exception {
        when(mServiceManagerMock.getTransport(eq(android.hardware.wifi.supplicant.V1_1.ISupplicant
                .kInterfaceName), anyString()))
                .thenReturn(IServiceManager.Transport.HWBINDER);
        mISupplicantMockV1_1 = mock(android.hardware.wifi.supplicant.V1_1.ISupplicant.class);

        executeAndValidateInitializationSequenceV1_1(false, false);
        mDut.terminate();
        verify(mPropertyService, never()).set(any(), any());
        verify(mISupplicantMockV1_1).terminate();
    }

    private class GetKeyMgmtCapabilitiesAnswer extends MockAnswerUtil.AnswerWithArguments {
        private int mKeyMgmtCapabilities;

        GetKeyMgmtCapabilitiesAnswer(int keyMgmtCapabilities) {
            mKeyMgmtCapabilities = keyMgmtCapabilities;
        }

        public void answer(android.hardware.wifi.supplicant.V1_2.ISupplicantStaIface
                .getKeyMgmtCapabilitiesCallback cb) {
            cb.onValues(mStatusSuccess, mKeyMgmtCapabilities);
        }
    }

    /**
     * Test get key management capabilities API on old HAL, should return 0 (not supported)
     */
    @Test
    public void testGetKeyMgmtCapabilitiesOldHal() throws Exception {
        when(mServiceManagerMock.getTransport(eq(android.hardware.wifi.supplicant.V1_1.ISupplicant
                .kInterfaceName), anyString()))
                .thenReturn(IServiceManager.Transport.HWBINDER);
        mISupplicantMockV1_1 = mock(android.hardware.wifi.supplicant.V1_1.ISupplicant.class);

        executeAndValidateInitializationSequenceV1_1(false, false);

        assertTrue(mDut.getAdvancedKeyMgmtCapabilities(WLAN0_IFACE_NAME) == 0);
    }

    /**
     * Test WPA3-Personal SAE key may management support
     */
    @Test
    public void testGetKeyMgmtCapabilitiesWpa3Sae() throws Exception {
        when(mServiceManagerMock.getTransport(eq(android.hardware.wifi.supplicant.V1_2.ISupplicant
                .kInterfaceName), anyString()))
                .thenReturn(IServiceManager.Transport.HWBINDER);
        mISupplicantMockV1_2 = mock(android.hardware.wifi.supplicant.V1_2.ISupplicant.class);
        when(mServiceManagerMock.getTransport(eq(android.hardware.wifi.supplicant.V1_1.ISupplicant
                .kInterfaceName), anyString()))
                .thenReturn(IServiceManager.Transport.HWBINDER);
        mISupplicantMockV1_1 = mock(android.hardware.wifi.supplicant.V1_1.ISupplicant.class);

        executeAndValidateInitializationSequenceV1_2();

        doAnswer(new GetKeyMgmtCapabilitiesAnswer(android.hardware.wifi.supplicant.V1_2
                .ISupplicantStaNetwork.KeyMgmtMask.SAE))
                .when(mISupplicantStaIfaceMockV1_2).getKeyMgmtCapabilities(any(
                android.hardware.wifi.supplicant.V1_2.ISupplicantStaIface
                        .getKeyMgmtCapabilitiesCallback.class));

        assertEquals(WIFI_FEATURE_WPA3_SAE, mDut.getAdvancedKeyMgmtCapabilities(WLAN0_IFACE_NAME));
    }

    /**
     * Test WPA3-Enterprise Suite-B-192 key may management support
     */
    @Test
    public void testGetKeyMgmtCapabilitiesWpa3SuiteB() throws Exception {
        when(mServiceManagerMock.getTransport(eq(android.hardware.wifi.supplicant.V1_2.ISupplicant
                .kInterfaceName), anyString()))
                .thenReturn(IServiceManager.Transport.HWBINDER);
        mISupplicantMockV1_2 = mock(android.hardware.wifi.supplicant.V1_2.ISupplicant.class);
        when(mServiceManagerMock.getTransport(eq(android.hardware.wifi.supplicant.V1_1.ISupplicant
                .kInterfaceName), anyString()))
                .thenReturn(IServiceManager.Transport.HWBINDER);
        mISupplicantMockV1_1 = mock(android.hardware.wifi.supplicant.V1_1.ISupplicant.class);

        executeAndValidateInitializationSequenceV1_2();

        doAnswer(new GetKeyMgmtCapabilitiesAnswer(android.hardware.wifi.supplicant.V1_2
                .ISupplicantStaNetwork.KeyMgmtMask.SUITE_B_192))
                .when(mISupplicantStaIfaceMockV1_2).getKeyMgmtCapabilities(any(
                android.hardware.wifi.supplicant.V1_2.ISupplicantStaIface
                        .getKeyMgmtCapabilitiesCallback.class));

        assertEquals(WIFI_FEATURE_WPA3_SUITE_B,
                mDut.getAdvancedKeyMgmtCapabilities(WLAN0_IFACE_NAME));
    }

    /**
     * Test Enhanced Open (OWE) key may management support
     */
    @Test
    public void testGetKeyMgmtCapabilitiesOwe() throws Exception {
        when(mServiceManagerMock.getTransport(eq(android.hardware.wifi.supplicant.V1_2.ISupplicant
                .kInterfaceName), anyString()))
                .thenReturn(IServiceManager.Transport.HWBINDER);
        mISupplicantMockV1_2 = mock(android.hardware.wifi.supplicant.V1_2.ISupplicant.class);
        when(mServiceManagerMock.getTransport(eq(android.hardware.wifi.supplicant.V1_1.ISupplicant
                .kInterfaceName), anyString()))
                .thenReturn(IServiceManager.Transport.HWBINDER);
        mISupplicantMockV1_1 = mock(android.hardware.wifi.supplicant.V1_1.ISupplicant.class);

        executeAndValidateInitializationSequenceV1_2();

        doAnswer(new GetKeyMgmtCapabilitiesAnswer(android.hardware.wifi.supplicant.V1_2
                .ISupplicantStaNetwork.KeyMgmtMask.OWE))
                .when(mISupplicantStaIfaceMockV1_2).getKeyMgmtCapabilities(any(
                android.hardware.wifi.supplicant.V1_2.ISupplicantStaIface
                        .getKeyMgmtCapabilitiesCallback.class));

        assertEquals(WIFI_FEATURE_OWE, mDut.getAdvancedKeyMgmtCapabilities(WLAN0_IFACE_NAME));
    }

    /**
     * Test Enhanced Open (OWE) and SAE key may management support
     */
    @Test
    public void testGetKeyMgmtCapabilitiesOweAndSae() throws Exception {
        when(mServiceManagerMock.getTransport(eq(android.hardware.wifi.supplicant.V1_2.ISupplicant
                .kInterfaceName), anyString()))
                .thenReturn(IServiceManager.Transport.HWBINDER);
        mISupplicantMockV1_2 = mock(android.hardware.wifi.supplicant.V1_2.ISupplicant.class);
        when(mServiceManagerMock.getTransport(eq(android.hardware.wifi.supplicant.V1_1.ISupplicant
                .kInterfaceName), anyString()))
                .thenReturn(IServiceManager.Transport.HWBINDER);
        mISupplicantMockV1_1 = mock(android.hardware.wifi.supplicant.V1_1.ISupplicant.class);

        executeAndValidateInitializationSequenceV1_2();

        doAnswer(new GetKeyMgmtCapabilitiesAnswer(android.hardware.wifi.supplicant.V1_2
                .ISupplicantStaNetwork.KeyMgmtMask.OWE
                | android.hardware.wifi.supplicant.V1_2.ISupplicantStaNetwork.KeyMgmtMask.SAE))
                .when(mISupplicantStaIfaceMockV1_2).getKeyMgmtCapabilities(any(
                android.hardware.wifi.supplicant.V1_2.ISupplicantStaIface
                        .getKeyMgmtCapabilitiesCallback.class));

        assertEquals(WIFI_FEATURE_OWE | WIFI_FEATURE_WPA3_SAE,
                mDut.getAdvancedKeyMgmtCapabilities(WLAN0_IFACE_NAME));
    }

    /**
     * Test Easy Connect (DPP) key may management support
     */
    @Test
    public void testGetKeyMgmtCapabilitiesDpp() throws Exception {
        when(mServiceManagerMock.getTransport(eq(android.hardware.wifi.supplicant.V1_2.ISupplicant
                .kInterfaceName), anyString()))
                .thenReturn(IServiceManager.Transport.HWBINDER);
        mISupplicantMockV1_2 = mock(android.hardware.wifi.supplicant.V1_2.ISupplicant.class);
        when(mServiceManagerMock.getTransport(eq(android.hardware.wifi.supplicant.V1_1.ISupplicant
                .kInterfaceName), anyString()))
                .thenReturn(IServiceManager.Transport.HWBINDER);
        mISupplicantMockV1_1 = mock(android.hardware.wifi.supplicant.V1_1.ISupplicant.class);

        executeAndValidateInitializationSequenceV1_2();

        doAnswer(new GetKeyMgmtCapabilitiesAnswer(android.hardware.wifi.supplicant.V1_2
                .ISupplicantStaNetwork.KeyMgmtMask.DPP))
                .when(mISupplicantStaIfaceMockV1_2).getKeyMgmtCapabilities(any(
                android.hardware.wifi.supplicant.V1_2.ISupplicantStaIface
                        .getKeyMgmtCapabilitiesCallback.class));

        assertEquals(WIFI_FEATURE_DPP, mDut.getAdvancedKeyMgmtCapabilities(WLAN0_IFACE_NAME));
    }

    /**
     * Test Easy Connect (DPP) calls return failure if hal version is less than 1_2
     */
    @Test
    public void testDppFailsWithOldHal() throws Exception {
        assertEquals(-1, mDut.addDppPeerUri(WLAN0_IFACE_NAME, "/blah"));
        assertFalse(mDut.removeDppUri(WLAN0_IFACE_NAME, 0));
        assertFalse(mDut.stopDppInitiator(WLAN0_IFACE_NAME));
        assertFalse(mDut.startDppConfiguratorInitiator(WLAN0_IFACE_NAME,
                1, 2, "Buckle", "My", "Shoe",
                3, 4));
        assertFalse(mDut.startDppEnrolleeInitiator(WLAN0_IFACE_NAME, 3, 14));
    }

    private WifiConfiguration createTestWifiConfiguration() {
        WifiConfiguration config = new WifiConfiguration();
        config.networkId = SUPPLICANT_NETWORK_ID;
        return config;
    }

    private void executeAndValidateHs20DeauthImminentCallback(boolean isEss) throws Exception {
        executeAndValidateInitializationSequence();
        assertNotNull(mISupplicantStaIfaceCallback);

        byte[] bssid = NativeUtil.macAddressToByteArray(BSSID);
        int reasonCode = isEss ? WnmData.ESS : WnmData.ESS + 1;
        int reauthDelay = 5;
        mISupplicantStaIfaceCallback.onHs20DeauthImminentNotice(
                bssid, reasonCode, reauthDelay, HS20_URL);

        ArgumentCaptor<WnmData> wnmDataCaptor = ArgumentCaptor.forClass(WnmData.class);
        verify(mWifiMonitor).broadcastWnmEvent(eq(WLAN0_IFACE_NAME), wnmDataCaptor.capture());
        assertEquals(
                ByteBufferReader.readInteger(
                        ByteBuffer.wrap(bssid), ByteOrder.BIG_ENDIAN, bssid.length),
                wnmDataCaptor.getValue().getBssid());
        assertEquals(isEss, wnmDataCaptor.getValue().isEss());
        assertEquals(reauthDelay, wnmDataCaptor.getValue().getDelay());
        assertEquals(HS20_URL, wnmDataCaptor.getValue().getUrl());
    }

    private void executeAndValidateInitializationSequence() throws  Exception {
        executeAndValidateInitializationSequence(false, false, false, false);
    }

    /**
     * Calls.initialize(), mocking various call back answers and verifying flow, asserting for the
     * expected result. Verifies if ISupplicantStaIface manager is initialized or reset.
     * Each of the arguments will cause a different failure mode when set true.
     */
    private void executeAndValidateInitializationSequence(boolean causeRemoteException,
                                                          boolean getZeroInterfaces,
                                                          boolean getNullInterface,
                                                          boolean causeCallbackRegFailure)
            throws Exception {
        boolean shouldSucceed =
                !causeRemoteException && !getZeroInterfaces && !getNullInterface
                        && !causeCallbackRegFailure;
        // Setup callback mock answers
        ArrayList<ISupplicant.IfaceInfo> interfaces;
        if (getZeroInterfaces) {
            interfaces = new ArrayList<>();
        } else {
            interfaces = mIfaceInfoList;
        }
        doAnswer(new GetListInterfacesAnswer(interfaces)).when(mISupplicantMock)
                .listInterfaces(any(ISupplicant.listInterfacesCallback.class));
        if (causeRemoteException) {
            doThrow(new RemoteException("Some error!!!"))
                    .when(mISupplicantMock).getInterface(any(ISupplicant.IfaceInfo.class),
                    any(ISupplicant.getInterfaceCallback.class));
        } else {
            doAnswer(new GetGetInterfaceAnswer(getNullInterface))
                    .when(mISupplicantMock).getInterface(any(ISupplicant.IfaceInfo.class),
                    any(ISupplicant.getInterfaceCallback.class));
        }
        /** Callback registration */
        if (causeCallbackRegFailure) {
            doAnswer(new MockAnswerUtil.AnswerWithArguments() {
                public SupplicantStatus answer(ISupplicantStaIfaceCallback cb)
                        throws RemoteException {
                    return mStatusFailure;
                }
            }).when(mISupplicantStaIfaceMock)
                    .registerCallback(any(ISupplicantStaIfaceCallback.class));
        } else {
            doAnswer(new MockAnswerUtil.AnswerWithArguments() {
                public SupplicantStatus answer(ISupplicantStaIfaceCallback cb)
                        throws RemoteException {
                    mISupplicantStaIfaceCallback = cb;
                    return mStatusSuccess;
                }
            }).when(mISupplicantStaIfaceMock)
                    .registerCallback(any(ISupplicantStaIfaceCallback.class));
        }

        mInOrder = inOrder(mServiceManagerMock, mISupplicantMock, mISupplicantStaIfaceMock,
                mWifiMonitor);
        // Initialize SupplicantStaIfaceHal, should call serviceManager.registerForNotifications
        assertTrue(mDut.initialize());
        // verify: service manager initialization sequence
        mInOrder.verify(mServiceManagerMock).linkToDeath(mServiceManagerDeathCaptor.capture(),
                anyLong());
        mInOrder.verify(mServiceManagerMock).registerForNotifications(
                eq(ISupplicant.kInterfaceName), eq(""), mServiceNotificationCaptor.capture());
        // act: cause the onRegistration(...) callback to execute
        mServiceNotificationCaptor.getValue().onRegistration(ISupplicant.kInterfaceName, "", true);

        assertTrue(mDut.isInitializationComplete());
        assertEquals(shouldSucceed, mDut.setupIface(WLAN0_IFACE_NAME));
        mInOrder.verify(mISupplicantMock).linkToDeath(mSupplicantDeathCaptor.capture(),
                mDeathRecipientCookieCaptor.capture());
        // verify: listInterfaces is called
        mInOrder.verify(mISupplicantMock).listInterfaces(
                any(ISupplicant.listInterfacesCallback.class));
        if (!getZeroInterfaces) {
            mInOrder.verify(mISupplicantMock)
                    .getInterface(any(ISupplicant.IfaceInfo.class),
                            any(ISupplicant.getInterfaceCallback.class));
        }
        if (!causeRemoteException && !getZeroInterfaces && !getNullInterface) {
            mInOrder.verify(mISupplicantStaIfaceMock)
                    .registerCallback(any(ISupplicantStaIfaceCallback.class));
        }
    }

    /**
     * Calls.initialize(), mocking various call back answers and verifying flow, asserting for the
     * expected result. Verifies if ISupplicantStaIface manager is initialized or reset.
     * Each of the arguments will cause a different failure mode when set true.
     */
    private void executeAndValidateInitializationSequenceV1_1(boolean causeRemoteException,
                                                               boolean getNullInterface)
            throws Exception {
        boolean shouldSucceed = !causeRemoteException && !getNullInterface;
        // Setup callback mock answers
        if (causeRemoteException) {
            doThrow(new RemoteException("Some error!!!"))
                    .when(mISupplicantMockV1_1).addInterface(any(ISupplicant.IfaceInfo.class),
                    any(android.hardware.wifi.supplicant.V1_1.ISupplicant
                            .addInterfaceCallback.class));
        } else {
            doAnswer(new GetAddInterfaceAnswer(getNullInterface))
                    .when(mISupplicantMockV1_1).addInterface(any(ISupplicant.IfaceInfo.class),
                    any(android.hardware.wifi.supplicant.V1_1.ISupplicant
                            .addInterfaceCallback.class));
        }
        /** Callback registration */
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public SupplicantStatus answer(
                    android.hardware.wifi.supplicant.V1_1.ISupplicantStaIfaceCallback cb)
                    throws RemoteException {
                mISupplicantStaIfaceCallbackV1_1 = cb;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaIfaceMockV1_1)
                .registerCallback_1_1(
                any(android.hardware.wifi.supplicant.V1_1.ISupplicantStaIfaceCallback.class));

        mInOrder = inOrder(mServiceManagerMock, mISupplicantMock, mISupplicantMockV1_1,
                mISupplicantStaIfaceMockV1_1, mWifiMonitor);
        // Initialize SupplicantStaIfaceHal, should call serviceManager.registerForNotifications
        assertTrue(mDut.initialize());
        // verify: service manager initialization sequence
        mInOrder.verify(mServiceManagerMock).linkToDeath(mServiceManagerDeathCaptor.capture(),
                anyLong());
        mInOrder.verify(mServiceManagerMock).registerForNotifications(
                eq(ISupplicant.kInterfaceName), eq(""), mServiceNotificationCaptor.capture());
        // act: cause the onRegistration(...) callback to execute
        mServiceNotificationCaptor.getValue().onRegistration(ISupplicant.kInterfaceName, "", true);

        assertTrue(mDut.isInitializationComplete());
        assertTrue(mDut.setupIface(WLAN0_IFACE_NAME) == shouldSucceed);
        mInOrder.verify(mISupplicantMock).linkToDeath(mSupplicantDeathCaptor.capture(),
                anyLong());
        // verify: addInterface is called
        mInOrder.verify(mISupplicantMockV1_1)
                .addInterface(any(ISupplicant.IfaceInfo.class),
                        any(android.hardware.wifi.supplicant.V1_1.ISupplicant
                                .addInterfaceCallback.class));
        if (!causeRemoteException && !getNullInterface) {
            mInOrder.verify(mISupplicantStaIfaceMockV1_1)
                    .registerCallback_1_1(
                    any(android.hardware.wifi.supplicant.V1_1.ISupplicantStaIfaceCallback.class));
        }

        // Ensure we don't try to use the listInterfaces method from 1.0 version.
        verify(mISupplicantMock, never()).listInterfaces(
                any(ISupplicant.listInterfacesCallback.class));
        verify(mISupplicantMock, never()).getInterface(any(ISupplicant.IfaceInfo.class),
                        any(ISupplicant.getInterfaceCallback.class));
    }

    /**
     * Calls.initialize(), mocking various call back answers and verifying flow, asserting for the
     * expected result. Verifies if ISupplicantStaIface manager is initialized or reset.
     * Each of the arguments will cause a different failure mode when set true.
     */
    private void executeAndValidateInitializationSequenceV1_2()
            throws Exception {
        // Setup callback mock answers
        doAnswer(new GetAddInterfaceAnswerV1_2(false))
                .when(mISupplicantMockV1_1).addInterface(any(ISupplicant.IfaceInfo.class),
                any(android.hardware.wifi.supplicant.V1_2.ISupplicant
                        .addInterfaceCallback.class));

        /** Callback registration */
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public SupplicantStatus answer(
                    android.hardware.wifi.supplicant.V1_1.ISupplicantStaIfaceCallback cb)
                    throws RemoteException {
                mISupplicantStaIfaceCallbackV1_1 = cb;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaIfaceMockV1_2)
                .registerCallback_1_1(
                        any(android.hardware.wifi.supplicant.V1_1.ISupplicantStaIfaceCallback
                                .class));

        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public SupplicantStatus answer(
                    android.hardware.wifi.supplicant.V1_2.ISupplicantStaIfaceCallback cb)
                    throws RemoteException {
                mISupplicantStaIfaceCallbackV1_2 = cb;
                return mStatusSuccess;
            }
        }).when(mISupplicantStaIfaceMockV1_2)
                .registerCallback_1_2(
                        any(android.hardware.wifi.supplicant.V1_2.ISupplicantStaIfaceCallback
                                .class));

        mInOrder = inOrder(mServiceManagerMock, mISupplicantMock, mISupplicantMockV1_1,
                mISupplicantStaIfaceMockV1_2, mWifiMonitor);
        // Initialize SupplicantStaIfaceHal, should call serviceManager.registerForNotifications
        assertTrue(mDut.initialize());
        // verify: service manager initialization sequence
        mInOrder.verify(mServiceManagerMock).linkToDeath(mServiceManagerDeathCaptor.capture(),
                anyLong());
        mInOrder.verify(mServiceManagerMock).registerForNotifications(
                eq(ISupplicant.kInterfaceName), eq(""), mServiceNotificationCaptor.capture());
        // act: cause the onRegistration(...) callback to execute
        mServiceNotificationCaptor.getValue().onRegistration(ISupplicant.kInterfaceName, "", true);

        assertTrue(mDut.isInitializationComplete());
        assertTrue(mDut.setupIface(WLAN0_IFACE_NAME));
        mInOrder.verify(mISupplicantMock).linkToDeath(mSupplicantDeathCaptor.capture(),
                anyLong());
        // verify: addInterface is called
        mInOrder.verify(mISupplicantMockV1_1)
                .addInterface(any(ISupplicant.IfaceInfo.class),
                        any(android.hardware.wifi.supplicant.V1_2.ISupplicant
                                .addInterfaceCallback.class));

        mInOrder.verify(mISupplicantStaIfaceMockV1_2)
                .registerCallback_1_2(
                        any(android.hardware.wifi.supplicant.V1_2.ISupplicantStaIfaceCallback
                                .class));

        // Ensure we don't try to use the listInterfaces method from 1.0 version.
//        verify(mISupplicantMock, never()).listInterfaces(
//                any(ISupplicant.listInterfacesCallback.class));
//        verify(mISupplicantMock, never()).getInterface(any(ISupplicant.IfaceInfo.class),
//                any(ISupplicant.getInterfaceCallback.class));
    }

    private SupplicantStatus createSupplicantStatus(int code) {
        SupplicantStatus status = new SupplicantStatus();
        status.code = code;
        return status;
    }

    /**
     * Create an IfaceInfo with given type and name
     */
    private ISupplicant.IfaceInfo createIfaceInfo(int type, String name) {
        ISupplicant.IfaceInfo info = new ISupplicant.IfaceInfo();
        info.type = type;
        info.name = name;
        return info;
    }

    private class GetListInterfacesAnswer extends MockAnswerUtil.AnswerWithArguments {
        private ArrayList<ISupplicant.IfaceInfo> mInterfaceList;

        GetListInterfacesAnswer(ArrayList<ISupplicant.IfaceInfo> ifaces) {
            mInterfaceList = ifaces;
        }

        public void answer(ISupplicant.listInterfacesCallback cb) {
            cb.onValues(mStatusSuccess, mInterfaceList);
        }
    }

    private class GetGetInterfaceAnswer extends MockAnswerUtil.AnswerWithArguments {
        boolean mGetNullInterface;

        GetGetInterfaceAnswer(boolean getNullInterface) {
            mGetNullInterface = getNullInterface;
        }

        public void answer(ISupplicant.IfaceInfo iface, ISupplicant.getInterfaceCallback cb) {
            if (mGetNullInterface) {
                cb.onValues(mStatusSuccess, null);
            } else {
                cb.onValues(mStatusSuccess, mISupplicantIfaceMock);
            }
        }
    }

    private class GetAddInterfaceAnswer extends MockAnswerUtil.AnswerWithArguments {
        boolean mGetNullInterface;

        GetAddInterfaceAnswer(boolean getNullInterface) {
            mGetNullInterface = getNullInterface;
        }

        public void answer(ISupplicant.IfaceInfo iface,
                           android.hardware.wifi.supplicant.V1_1.ISupplicant
                                   .addInterfaceCallback cb) {
            if (mGetNullInterface) {
                cb.onValues(mStatusSuccess, null);
            } else {
                cb.onValues(mStatusSuccess, mISupplicantIfaceMock);
            }
        }
    }

    private class GetAddInterfaceAnswerV1_2 extends MockAnswerUtil.AnswerWithArguments {
        boolean mGetNullInterface;

        GetAddInterfaceAnswerV1_2(boolean getNullInterface) {
            mGetNullInterface = getNullInterface;
        }

        public void answer(ISupplicant.IfaceInfo iface,
                android.hardware.wifi.supplicant.V1_2.ISupplicant
                        .addInterfaceCallback cb) {
            if (mGetNullInterface) {
                cb.onValues(mStatusSuccess, null);
            } else {
                cb.onValues(mStatusSuccess, mISupplicantIfaceMock);
            }
        }
    }

    /**
     * Setup mocks for connect sequence.
     */
    private void setupMocksForConnectSequence(final boolean haveExistingNetwork) throws Exception {
        final int existingNetworkId = SUPPLICANT_NETWORK_ID;
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public SupplicantStatus answer() throws RemoteException {
                return mStatusSuccess;
            }
        }).when(mISupplicantStaIfaceMock).disconnect();
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public void answer(ISupplicantStaIface.listNetworksCallback cb) throws RemoteException {
                if (haveExistingNetwork) {
                    cb.onValues(mStatusSuccess, new ArrayList<>(Arrays.asList(existingNetworkId)));
                } else {
                    cb.onValues(mStatusSuccess, new ArrayList<>());
                }
            }
        }).when(mISupplicantStaIfaceMock)
                .listNetworks(any(ISupplicantStaIface.listNetworksCallback.class));
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public SupplicantStatus answer(int id) throws RemoteException {
                return mStatusSuccess;
            }
        }).when(mISupplicantStaIfaceMock).removeNetwork(eq(existingNetworkId));
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public void answer(ISupplicantStaIface.addNetworkCallback cb) throws RemoteException {
                cb.onValues(mStatusSuccess, mock(ISupplicantStaNetwork.class));
                return;
            }
        }).when(mISupplicantStaIfaceMock).addNetwork(
                any(ISupplicantStaIface.addNetworkCallback.class));
        when(mSupplicantStaNetworkMock.saveWifiConfiguration(any(WifiConfiguration.class)))
                .thenReturn(true);
        when(mSupplicantStaNetworkMock.select()).thenReturn(true);
    }

    /**
     * Helper function to validate the connect sequence.
     */
    private void validateConnectSequence(
            final boolean haveExistingNetwork, int numNetworkAdditions) throws Exception {
        if (haveExistingNetwork) {
            verify(mISupplicantStaIfaceMock).removeNetwork(anyInt());
        }
        verify(mISupplicantStaIfaceMock, times(numNetworkAdditions))
                .addNetwork(any(ISupplicantStaIface.addNetworkCallback.class));
        verify(mSupplicantStaNetworkMock, times(numNetworkAdditions))
                .saveWifiConfiguration(any(WifiConfiguration.class));
        verify(mSupplicantStaNetworkMock, times(numNetworkAdditions)).select();
    }

    /**
     * Helper function to execute all the actions to perform connection to the network.
     *
     * @param newFrameworkNetworkId Framework Network Id of the new network to connect.
     * @param haveExistingNetwork Removes the existing network.
     * @return the WifiConfiguration object of the new network to connect.
     */
    private WifiConfiguration executeAndValidateConnectSequence(
            final int newFrameworkNetworkId, final boolean haveExistingNetwork) throws Exception {
        return executeAndValidateConnectSequenceWithKeyMgmt(newFrameworkNetworkId,
                haveExistingNetwork, WifiConfiguration.KeyMgmt.WPA_PSK);
    }

    /**
     * Helper function to execute all the actions to perform connection to the network.
     *
     * @param newFrameworkNetworkId Framework Network Id of the new network to connect.
     * @param haveExistingNetwork Removes the existing network.
     * @param keyMgmt Key management of the new network
     * @return the WifiConfiguration object of the new network to connect.
     */
    private WifiConfiguration executeAndValidateConnectSequenceWithKeyMgmt(
            final int newFrameworkNetworkId, final boolean haveExistingNetwork,
            int keyMgmt) throws Exception {
        setupMocksForConnectSequence(haveExistingNetwork);
        WifiConfiguration config = new WifiConfiguration();
        config.networkId = newFrameworkNetworkId;
        config.allowedKeyManagement.set(keyMgmt);
        assertTrue(mDut.connectToNetwork(WLAN0_IFACE_NAME, config));
        validateConnectSequence(haveExistingNetwork, 1);
        return config;
    }

    /**
     * Setup mocks for roam sequence.
     */
    private void setupMocksForRoamSequence(String roamBssid) throws Exception {
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public SupplicantStatus answer() throws RemoteException {
                return mStatusSuccess;
            }
        }).when(mISupplicantStaIfaceMock).reassociate();
        when(mSupplicantStaNetworkMock.setBssid(eq(roamBssid))).thenReturn(true);
    }

    /**
     * Helper function to execute all the actions to perform roaming to the network.
     *
     * @param sameNetwork Roam to the same network or not.
     */
    private void executeAndValidateRoamSequence(boolean sameNetwork) throws Exception {
        int connectedNetworkId = ROAM_NETWORK_ID;
        String roamBssid = BSSID;
        int roamNetworkId;
        if (sameNetwork) {
            roamNetworkId = connectedNetworkId;
        } else {
            roamNetworkId = connectedNetworkId + 1;
        }
        executeAndValidateConnectSequence(connectedNetworkId, false);
        setupMocksForRoamSequence(roamBssid);

        WifiConfiguration roamingConfig = new WifiConfiguration();
        roamingConfig.networkId = roamNetworkId;
        roamingConfig.getNetworkSelectionStatus().setNetworkSelectionBSSID(roamBssid);
        assertTrue(mDut.roamToNetwork(WLAN0_IFACE_NAME, roamingConfig));

        if (!sameNetwork) {
            validateConnectSequence(false, 2);
            verify(mSupplicantStaNetworkMock, never()).setBssid(anyString());
            verify(mISupplicantStaIfaceMock, never()).reassociate();
        } else {
            verify(mSupplicantStaNetworkMock).setBssid(eq(roamBssid));
            verify(mISupplicantStaIfaceMock).reassociate();
        }
    }
}
