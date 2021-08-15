/*
 * Copyright 2017 The Android Open Source Project
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

package com.android.server.wifi.hotspot2;

import static android.net.NetworkCapabilities.NET_CAPABILITY_TRUSTED;

import static org.junit.Assert.*;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.LinkAddress;
import android.net.LinkProperties;
import android.net.Network;
import android.net.NetworkRequest;
import android.net.NetworkUtils;
import android.net.RouteInfo;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiSsid;
import android.os.Handler;
import android.os.test.TestLooper;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.TestUtil;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.net.InetAddress;

/**
 * Unit tests for {@link OsuNetworkConnection}.
 */
@SmallTest
public class OsuNetworkConnectionTest {
    private static final String TAG = "OsuNetworkConnectionTest";
    private static final int ENABLE_LOGGING = 1;
    private static final int DISABLE_LOGGING = 0;

    private static final int TEST_NETWORK_ID = 6;
    private static final String TEST_NAI = null;
    private static final String TEST_NAI_OSEN = "access.test.com";
    private static final String TEST_PROVIDER_NAME = "testService";
    private static final WifiSsid TEST_SSID = WifiSsid.createFromAsciiEncoded("Test SSID");

    private OsuNetworkConnection mNetworkConnection;
    private TestLooper mLooper;
    private Handler mHandler;

    @Mock Context mContext;
    @Mock WifiManager mWifiManager;
    @Mock ConnectivityManager mConnectivityManager;
    @Mock OsuNetworkConnection.Callbacks mNetworkCallbacks;
    @Mock WifiInfo mWifiInfo;
    @Mock Network mCurrentNetwork;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        doReturn(mWifiManager).when(mContext)
                .getSystemService(eq(Context.WIFI_SERVICE));
        doReturn(mConnectivityManager).when(mContext)
                .getSystemService(eq(Context.CONNECTIVITY_SERVICE));
        when(mWifiManager.isWifiEnabled()).thenReturn(true);
        when(mWifiManager.enableNetwork(TEST_NETWORK_ID, true)).thenReturn(true);
        when(mWifiManager.addNetwork(any(WifiConfiguration.class))).thenReturn(TEST_NETWORK_ID);
        when(mWifiManager.getConnectionInfo()).thenReturn(mWifiInfo);
        when(mWifiInfo.getNetworkId()).thenReturn(TEST_NETWORK_ID);
        mLooper = new TestLooper();
        mHandler = new Handler(mLooper.getLooper());
        mNetworkConnection = new OsuNetworkConnection(mContext);
        mNetworkConnection.enableVerboseLogging(ENABLE_LOGGING);
    }

    private LinkProperties createProvisionedLinkProperties() {
        InetAddress addrV4 = NetworkUtils.numericToInetAddress("75.208.6.1");
        InetAddress dns1 = NetworkUtils.numericToInetAddress("75.208.7.1");
        LinkAddress linkAddrV4 = new LinkAddress(addrV4, 32);
        InetAddress gateway1 = NetworkUtils.numericToInetAddress("75.208.8.1");
        LinkProperties lp4 = new LinkProperties();
        lp4.addLinkAddress(linkAddrV4);
        lp4.addDnsServer(dns1);
        lp4.addRoute(new RouteInfo(gateway1));
        return lp4;
    }

    /**
     * Verify that the class registers for receiving the necessary broadcast intents upon init.
     * Verify that the initialization only occurs once even if init() is called  multiple times.
     */
    @Test
    public void verifyBroadcastIntentRegistration() {
        mNetworkConnection.init(mHandler);

        ArgumentCaptor<IntentFilter> intentFilterCaptor =
                ArgumentCaptor.forClass(IntentFilter.class);
        verify(mContext).registerReceiver(any(BroadcastReceiver.class),
                intentFilterCaptor.capture(), any(), eq(mHandler));
        verify(mWifiManager).isWifiEnabled();
        IntentFilter intentFilter = intentFilterCaptor.getValue();
        assertEquals(intentFilter.countActions(), 1);
    }

    /**
     * Verifies that onWifiEnabled() callback is invoked when the relevant intent is
     * received and the caller is subscribed to receive the callback.
     */
    @Test
    public void verifyWifiStateCallbacks() {
        when(mWifiManager.isWifiEnabled()).thenReturn(false);
        mNetworkConnection.init(mHandler);
        ArgumentCaptor<BroadcastReceiver> broadcastReceiverCaptor =
                ArgumentCaptor.forClass(BroadcastReceiver.class);
        verify(mContext).registerReceiver(broadcastReceiverCaptor.capture(),
                any(IntentFilter.class), any(), eq(mHandler));
        BroadcastReceiver broadcastReceiver = broadcastReceiverCaptor.getValue();
        mLooper.dispatchAll();
        mNetworkConnection.setEventCallback(mNetworkCallbacks);
        TestUtil.sendWifiStateChanged(broadcastReceiver, mContext,
                WifiManager.WIFI_STATE_ENABLED);
        TestUtil.sendWifiStateChanged(broadcastReceiver, mContext,
                WifiManager.WIFI_STATE_DISABLED);
        mNetworkConnection.setEventCallback(null);
        TestUtil.sendWifiStateChanged(broadcastReceiver, mContext,
                WifiManager.WIFI_STATE_ENABLED);
        TestUtil.sendWifiStateChanged(broadcastReceiver, mContext,
                WifiManager.WIFI_STATE_DISABLED);
        verify(mNetworkCallbacks, times(1)).onWifiEnabled();
        verify(mNetworkCallbacks, times(1)).onWifiDisabled();
    }

    /**
     * Verifies that connect() API returns false when Wifi is not enabled
     */
    @Test
    public void verifyNetworkConnectionWhenWifiIsDisabled() {
        when(mWifiManager.isWifiEnabled()).thenReturn(false);
        mNetworkConnection.init(mHandler);
        assertEquals(false, mNetworkConnection.connect(TEST_SSID, TEST_NAI, TEST_PROVIDER_NAME));
    }

    /**
     * Verifies that connect() API returns false when OSU AP is a part of an OSEN
     */
    @Test
    public void verifyOSENUnsupported() {
        mNetworkConnection.init(mHandler);
        assertEquals(false,
                mNetworkConnection.connect(TEST_SSID, TEST_NAI_OSEN, TEST_PROVIDER_NAME));
    }

    /**
     * Verifies that connect() API returns false when WifiManager's addNetwork()
     * returns an invalid network ID
     */
    @Test
    public void verifyNetworkConnectionWhenAddNetworkFails() {
        when(mWifiManager.addNetwork(any(WifiConfiguration.class))).thenReturn(-1);
        mNetworkConnection.init(mHandler);
        assertEquals(false, mNetworkConnection.connect(TEST_SSID, TEST_NAI, TEST_PROVIDER_NAME));
        verify(mWifiManager, never()).removeNetwork(TEST_NETWORK_ID);
    }

    /**
     * Verifies that connect() API returns false when WifiManager's enableNetwork()
     * fails for the given network ID corresponding to the OSU AP
     */
    @Test
    public void verifyNetworkConnectionWhenEnableNetworkFails() {
        when(mWifiManager.enableNetwork(TEST_NETWORK_ID, true)).thenReturn(false);
        mNetworkConnection.init(mHandler);
        assertEquals(false, mNetworkConnection.connect(TEST_SSID, TEST_NAI, TEST_PROVIDER_NAME));
        verify(mWifiManager).removeNetwork(TEST_NETWORK_ID);
    }

    /**
     * Verifies that network state callbacks are invoked when network callbacks
     * are received and when WifiManager has successfully requested connection to the OSU AP.
     * Ensure IP connectivity is available before invoking onConnected callback.
     */
    @Test
    public void verifyNetworkCallbackInvokedWhenConnected() {
        mNetworkConnection.init(mHandler);

        mNetworkConnection.setEventCallback(mNetworkCallbacks);
        assertEquals(true, mNetworkConnection.connect(TEST_SSID, TEST_NAI, TEST_PROVIDER_NAME));

        ArgumentCaptor<ConnectivityManager.NetworkCallback> networkCallbackCaptor =
                ArgumentCaptor.forClass(ConnectivityManager.NetworkCallback.class);
        verify(mConnectivityManager).requestNetwork(any(NetworkRequest.class),
                networkCallbackCaptor.capture(), any(Handler.class), anyInt());
        ConnectivityManager.NetworkCallback callback = networkCallbackCaptor.getValue();
        callback.onAvailable(mCurrentNetwork);
        callback.onLinkPropertiesChanged(mCurrentNetwork, createProvisionedLinkProperties());
        verify(mNetworkCallbacks).onConnected(mCurrentNetwork);

        callback.onLost(mCurrentNetwork);
        verify(mNetworkCallbacks).onDisconnected();

        mNetworkConnection.disconnectIfNeeded();

        verify(mConnectivityManager).unregisterNetworkCallback(any(ConnectivityManager
                .NetworkCallback.class));
        verify(mWifiManager).removeNetwork(TEST_NETWORK_ID);
    }

    /**
     * Verifies that onConnected callback are never invoked when onLinkPropertiesChanged is invoked
     * without onAvailable of NetworkCallback.
     */
    @Test
    public void verifyNetworkCallbackWithoutOnAvailable() {
        mNetworkConnection.init(mHandler);

        mNetworkConnection.setEventCallback(mNetworkCallbacks);
        assertEquals(true, mNetworkConnection.connect(TEST_SSID, TEST_NAI, TEST_PROVIDER_NAME));

        ArgumentCaptor<ConnectivityManager.NetworkCallback> networkCallbackCaptor =
                ArgumentCaptor.forClass(ConnectivityManager.NetworkCallback.class);
        verify(mConnectivityManager).requestNetwork(any(NetworkRequest.class),
                networkCallbackCaptor.capture(), any(Handler.class), anyInt());
        ConnectivityManager.NetworkCallback callback = networkCallbackCaptor.getValue();

        callback.onLinkPropertiesChanged(mCurrentNetwork, createProvisionedLinkProperties());

        verify(mNetworkCallbacks, never()).onConnected(mCurrentNetwork);
    }

    /**
     * Verifies that network state callbacks are invoked when the network callbacks
     * are received and when WifiManager has successfully requested connection to the OSU AP.
     * If IP connectivity is not provisioned, do not invoke onConnected callback.
     */
    @Test
    public void verifyNetworkConnectionTimeout() {
        mNetworkConnection.init(mHandler);

        mNetworkConnection.setEventCallback(mNetworkCallbacks);
        assertEquals(true, mNetworkConnection.connect(TEST_SSID, TEST_NAI, TEST_PROVIDER_NAME));

        ArgumentCaptor<ConnectivityManager.NetworkCallback> networkCallbackCaptor =
                ArgumentCaptor.forClass(ConnectivityManager.NetworkCallback.class);
        verify(mConnectivityManager).requestNetwork(any(NetworkRequest.class),
                networkCallbackCaptor.capture(), any(Handler.class), anyInt());
        ConnectivityManager.NetworkCallback callback = networkCallbackCaptor.getValue();
        callback.onLinkPropertiesChanged(mCurrentNetwork, new LinkProperties());
        verify(mNetworkCallbacks, never()).onConnected(mCurrentNetwork);

        callback.onUnavailable();
        verify(mNetworkCallbacks).onTimeOut();

        mNetworkConnection.disconnectIfNeeded();

        verify(mConnectivityManager).unregisterNetworkCallback(any(ConnectivityManager
                .NetworkCallback.class));
        verify(mWifiManager).removeNetwork(TEST_NETWORK_ID);
    }

    /**
     * Verifies that WifiManager's removeNetwork() is called when disconnectIfNeeded() is called
     * on the OSU AP's network ID.
     */
    @Test
    public void verifyNetworkDisconnect() {
        mNetworkConnection.init(mHandler);
        assertEquals(true, mNetworkConnection.connect(TEST_SSID, TEST_NAI, TEST_PROVIDER_NAME));

        mNetworkConnection.disconnectIfNeeded();

        verify(mConnectivityManager).unregisterNetworkCallback(any(ConnectivityManager
                .NetworkCallback.class));
        verify(mWifiManager).removeNetwork(TEST_NETWORK_ID);
    }

    /**
     * Verifies that {@link WifiConfiguration} has been created properly for OSU network.
     * It is supposed to create a network as ephemeral network and suppress no internet access
     * notification.
     */
    @Test
    public void verifyWifiConfigurationForOsuNetwork() {
        mNetworkConnection.init(mHandler);

        assertEquals(true, mNetworkConnection.connect(TEST_SSID, TEST_NAI, TEST_PROVIDER_NAME));

        ArgumentCaptor<WifiConfiguration> wifiConfigurationCaptor = ArgumentCaptor.forClass(
                WifiConfiguration.class);
        verify(mWifiManager, times(1)).addNetwork(wifiConfigurationCaptor.capture());
        WifiConfiguration wifiConfiguration = wifiConfigurationCaptor.getValue();
        assertTrue(wifiConfiguration.isNoInternetAccessExpected());
        assertTrue(wifiConfiguration.isEphemeral());
        assertTrue(wifiConfiguration.osu);

        ArgumentCaptor<NetworkRequest> networkRequestCaptor = ArgumentCaptor.forClass(
                NetworkRequest.class);
        verify(mConnectivityManager, times(1)).requestNetwork(networkRequestCaptor.capture(),
                any(ConnectivityManager.NetworkCallback.class), any(Handler.class), anyInt());
        assertFalse(networkRequestCaptor.getValue().hasCapability(NET_CAPABILITY_TRUSTED));

    }
}
