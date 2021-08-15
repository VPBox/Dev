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

import static android.hardware.wifi.supplicant.V1_2.DppAkm.PSK;
import static android.hardware.wifi.supplicant.V1_2.DppAkm.SAE;
import static android.hardware.wifi.supplicant.V1_2.DppFailureCode.AUTHENTICATION;
import static android.hardware.wifi.supplicant.V1_2.DppFailureCode.BUSY;
import static android.hardware.wifi.supplicant.V1_2.DppFailureCode.CONFIGURATION;
import static android.hardware.wifi.supplicant.V1_2.DppFailureCode.FAILURE;
import static android.hardware.wifi.supplicant.V1_2.DppFailureCode.NOT_COMPATIBLE;
import static android.hardware.wifi.supplicant.V1_2.DppFailureCode.NOT_SUPPORTED;
import static android.hardware.wifi.supplicant.V1_2.DppFailureCode.TIMEOUT;
import static android.hardware.wifi.supplicant.V1_2.DppProgressCode.AUTHENTICATION_SUCCESS;
import static android.hardware.wifi.supplicant.V1_2.DppProgressCode.RESPONSE_PENDING;
import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_FAILURE_AUTHENTICATION;
import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_FAILURE_BUSY;
import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_FAILURE_CONFIGURATION;
import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_FAILURE_GENERIC;
import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_FAILURE_INVALID_NETWORK;
import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_FAILURE_NOT_COMPATIBLE;
import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_FAILURE_NOT_SUPPORTED;
import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_FAILURE_TIMEOUT;
import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_PROGRESS_AUTHENTICATION_SUCCESS;
import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_PROGRESS_RESPONSE_PENDING;
import static android.net.wifi.EasyConnectStatusCallback.EASY_CONNECT_EVENT_SUCCESS_CONFIGURATION_SENT;
import static android.net.wifi.WifiManager.EASY_CONNECT_NETWORK_ROLE_STA;

import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.anyString;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.when;

import android.content.Context;
import android.net.wifi.EasyConnectStatusCallback;
import android.net.wifi.IDppCallback;
import android.net.wifi.WifiConfiguration;
import android.os.IBinder;
import android.os.test.TestLooper;
import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.util.WakeupMessage;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

/**
 * Unit tests for {@link DppManager}.
 */
@SmallTest
public class DppManagerTest {
    private static final String TAG = "DppManagerTest";
    private static final String TEST_INTERFACE_NAME = "testif0";
    private static final int TEST_PEER_ID = 1;
    private static final String TEST_SSID = "\"Test_SSID\"";
    private static final String TEST_SSID_ENCODED = "546573745f53534944";
    private static final String TEST_PASSWORD = "\"secretPassword\"";
    private static final String TEST_PASSWORD_ENCODED = "73656372657450617373776f7264";
    private static final int TEST_NETWORK_ID = 1;

    TestLooper mLooper;

    DppManager mDppManager;

    @Mock
    Context mContext;
    @Mock
    WifiMetrics mWifiMetrics;
    @Mock
    WifiNative mWifiNative;
    @Mock
    WifiConfigManager mWifiConfigManager;
    @Mock
    IBinder mBinder;
    @Mock
    IDppCallback mDppCallback;
    @Mock
    IDppCallback mDppCallbackConcurrent;
    @Mock
    WakeupMessage mWakeupMessage;
    @Mock
    DppMetrics mDppMetrics;

    String mUri =
            "DPP:C:81/1;I:DPP_TESTER;K:MDkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDIgADebGHMJoCcE7OZP/aek5muaJo"
                    + "zGy2FVKPRjA/I/qyC8Q=;;";

    @Before
    public void setUp() {
        MockitoAnnotations.initMocks(this);
        mLooper = new TestLooper();
        mDppManager = createDppManager();
        mLooper.dispatchAll();

        // Add Peer URI successful, return peer ID 1
        when(mWifiNative.addDppPeerUri(anyString(), anyString()))
                .thenReturn(1);
        when(mWifiNative.removeDppUri(anyString(), anyInt()))
                .thenReturn(true);

        // Return test interface name
        when(mWifiNative.getClientInterfaceName()).thenReturn(TEST_INTERFACE_NAME);

        // Successfully start enrollee
        when(mWifiNative.startDppEnrolleeInitiator(anyString(), anyInt(), anyInt())).thenReturn(
                true);

        // Successfully start configurator
        when(mWifiNative.startDppConfiguratorInitiator(anyString(), anyInt(), anyInt(), anyString(),
                any(), any(), anyInt(), anyInt())).thenReturn(true);
    }

    private DppManager createDppManager() {
        DppManager dppManger = new DppManager(mLooper.getLooper(), mWifiNative, mWifiConfigManager,
                mContext, mDppMetrics);
        dppManger.mDppTimeoutMessage = mWakeupMessage;
        dppManger.enableVerboseLogging(1);
        return dppManger;
    }

    /**
     * DPP Tests
     */
    @Test
    public void testStartDppAsConfiguratorInitiatorWithBadNetworkId() throws Exception {
        // Return NULL when for the selected network (invalid network)
        when(mWifiConfigManager.getConfiguredNetworkWithoutMasking(anyInt())).thenReturn(null);

        mDppManager.startDppAsConfiguratorInitiator(0, mBinder, mUri, 1,
                EASY_CONNECT_NETWORK_ROLE_STA,
                mDppCallback);
        verify(mDppCallback).onFailure(
                EasyConnectStatusCallback.EASY_CONNECT_EVENT_FAILURE_INVALID_NETWORK);
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mDppMetrics).updateDppConfiguratorInitiatorRequests();
        verify(mDppMetrics).updateDppFailure(EasyConnectStatusCallback
                .EASY_CONNECT_EVENT_FAILURE_INVALID_NETWORK);
        verifyNoMoreInteractions(mDppMetrics);
    }

    @Test
    public void testStartDppAsConfiguratorInitiatorFailAddPeer() throws Exception {
        // Generate a mock WifiConfiguration object
        WifiConfiguration selectedNetwork = new WifiConfiguration();
        selectedNetwork.SSID = "\"Test_SSID\"";
        selectedNetwork.networkId = TEST_NETWORK_ID;
        selectedNetwork.preSharedKey = "\"secretPassword\"";
        selectedNetwork.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);

        when(mWifiConfigManager.getConfiguredNetworkWithoutMasking(anyInt())).thenReturn(
                selectedNetwork);

        // Fail to add Peer URI
        when(mWifiNative.addDppPeerUri(anyString(), anyString())).thenReturn(-1);

        mDppManager.startDppAsConfiguratorInitiator(0, mBinder, mUri, 1,
                EASY_CONNECT_NETWORK_ROLE_STA,
                mDppCallback);
        verify(mDppCallback).onFailure(
                EasyConnectStatusCallback.EASY_CONNECT_EVENT_FAILURE_INVALID_URI);
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mDppMetrics).updateDppConfiguratorInitiatorRequests();
        verify(mDppMetrics).updateDppFailure(EasyConnectStatusCallback
                .EASY_CONNECT_EVENT_FAILURE_INVALID_URI);
        verify(mDppMetrics).updateDppOperationTime(anyInt());
        verifyNoMoreInteractions(mDppMetrics);
    }

    @Test
    public void testStartDppAsEnrolleeInitiatorFailAddPeer() throws Exception {
        // Fail to add Peer URI
        when(mWifiNative.addDppPeerUri(anyString(), anyString())).thenReturn(-1);

        mDppManager.startDppAsEnrolleeInitiator(0, mBinder, mUri, mDppCallback);
        verify(mDppCallback).onFailure(
                EasyConnectStatusCallback.EASY_CONNECT_EVENT_FAILURE_INVALID_URI);
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mDppMetrics).updateDppEnrolleeInitiatorRequests();
        verify(mDppMetrics).updateDppFailure(EasyConnectStatusCallback
                .EASY_CONNECT_EVENT_FAILURE_INVALID_URI);
        verify(mDppMetrics).updateDppOperationTime(anyInt());
        verifyNoMoreInteractions(mDppMetrics);
    }

    @Test
    public void testStartDppAsConfiguratorInitiatorFailStart() throws Exception {
        // Generate a mock WifiConfiguration object
        WifiConfiguration selectedNetwork = new WifiConfiguration();
        selectedNetwork.SSID = "\"Test_SSID\"";
        selectedNetwork.networkId = TEST_NETWORK_ID;
        selectedNetwork.preSharedKey = "\"secretPassword\"";
        selectedNetwork.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);

        when(mWifiConfigManager.getConfiguredNetworkWithoutMasking(anyInt())).thenReturn(
                selectedNetwork);

        // Fail to start
        when(mWifiNative.startDppConfiguratorInitiator(anyString(), anyInt(), anyInt(), anyString(),
                any(), any(), anyInt(), anyInt())).thenReturn(false);

        mDppManager.startDppAsConfiguratorInitiator(0, mBinder, mUri, 1,
                EASY_CONNECT_NETWORK_ROLE_STA,
                mDppCallback);
        verify(mDppCallback).onFailure(EASY_CONNECT_EVENT_FAILURE_GENERIC);
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mDppMetrics).updateDppConfiguratorInitiatorRequests();
        verify(mDppMetrics).updateDppFailure(EASY_CONNECT_EVENT_FAILURE_GENERIC);
        verify(mDppMetrics).updateDppOperationTime(anyInt());
        verifyNoMoreInteractions(mDppMetrics);
    }

    @Test
    public void testStartDppAsEnrolleeInitiatorFailStart() throws Exception {
        // Fail to start
        when(mWifiNative.startDppEnrolleeInitiator(anyString(), anyInt(), anyInt())).thenReturn(
                false);

        mDppManager.startDppAsEnrolleeInitiator(0, mBinder, mUri, mDppCallback);
        verify(mDppCallback).onFailure(EASY_CONNECT_EVENT_FAILURE_GENERIC);
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mDppMetrics).updateDppEnrolleeInitiatorRequests();
        verify(mDppMetrics).updateDppFailure(EASY_CONNECT_EVENT_FAILURE_GENERIC);
        verify(mDppMetrics).updateDppOperationTime(anyInt());
        verifyNoMoreInteractions(mDppMetrics);
    }

    @Test
    public void testStartDppAsConfiguratorInitiatorStartCorrectlyWpa2Psk() throws Exception {
        // Generate a mock WifiConfiguration object
        WifiConfiguration selectedNetwork = new WifiConfiguration();
        selectedNetwork.SSID = TEST_SSID;
        selectedNetwork.networkId = TEST_NETWORK_ID;
        selectedNetwork.preSharedKey = TEST_PASSWORD;
        selectedNetwork.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);

        when(mWifiConfigManager.getConfiguredNetworkWithoutMasking(anyInt())).thenReturn(
                selectedNetwork);

        mDppManager.startDppAsConfiguratorInitiator(0, mBinder, mUri, 1,
                EASY_CONNECT_NETWORK_ROLE_STA,
                mDppCallback);
        verify(mDppCallback, never()).onFailure(anyInt());
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mWifiNative).startDppConfiguratorInitiator(eq(TEST_INTERFACE_NAME),
                eq(TEST_PEER_ID), anyInt(), eq(TEST_SSID_ENCODED), eq(TEST_PASSWORD_ENCODED), any(),
                eq(EASY_CONNECT_NETWORK_ROLE_STA), eq(PSK));
        verify(mDppMetrics).updateDppConfiguratorInitiatorRequests();
        verifyNoMoreInteractions(mDppMetrics);
    }

    @Test
    public void testStartDppAsConfiguratorInitiatorStartCorrectlyWpa3Sae() throws Exception {
        // Generate a mock WifiConfiguration object
        WifiConfiguration selectedNetwork = new WifiConfiguration();
        selectedNetwork.SSID = TEST_SSID;
        selectedNetwork.networkId = TEST_NETWORK_ID;
        selectedNetwork.preSharedKey = TEST_PASSWORD;
        selectedNetwork.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.SAE);

        when(mWifiConfigManager.getConfiguredNetworkWithoutMasking(anyInt())).thenReturn(
                selectedNetwork);

        mDppManager.startDppAsConfiguratorInitiator(0, mBinder, mUri, 1,
                EASY_CONNECT_NETWORK_ROLE_STA,
                mDppCallback);
        verify(mDppCallback, never()).onFailure(anyInt());
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mWifiNative).startDppConfiguratorInitiator(eq(TEST_INTERFACE_NAME),
                eq(TEST_PEER_ID), anyInt(), eq(TEST_SSID_ENCODED), eq(TEST_PASSWORD_ENCODED), any(),
                eq(EASY_CONNECT_NETWORK_ROLE_STA), eq(SAE));
        verify(mDppMetrics).updateDppConfiguratorInitiatorRequests();
        verifyNoMoreInteractions(mDppMetrics);
    }

    @Test
    public void testStartDppAsConfiguratorInitiatorFailOpenNetwork() throws Exception {
        // Generate a mock WifiConfiguration object
        WifiConfiguration selectedNetwork = new WifiConfiguration();
        selectedNetwork.SSID = TEST_SSID;
        selectedNetwork.networkId = TEST_NETWORK_ID;
        selectedNetwork.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);

        when(mWifiConfigManager.getConfiguredNetworkWithoutMasking(anyInt())).thenReturn(
                selectedNetwork);

        mDppManager.startDppAsConfiguratorInitiator(0, mBinder, mUri, 1,
                EASY_CONNECT_NETWORK_ROLE_STA,
                mDppCallback);
        verify(mDppCallback).onFailure(EASY_CONNECT_EVENT_FAILURE_INVALID_NETWORK);
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mDppMetrics).updateDppConfiguratorInitiatorRequests();
        verify(mDppMetrics).updateDppFailure(EasyConnectStatusCallback
                .EASY_CONNECT_EVENT_FAILURE_INVALID_NETWORK);
        verifyNoMoreInteractions(mDppMetrics);
    }

    @Test
    public void testStartDppAsConfiguratorInitiatorFailEapNetwork() throws Exception {
        // Generate a mock WifiConfiguration object
        WifiConfiguration selectedNetwork = new WifiConfiguration();
        selectedNetwork.SSID = TEST_SSID;
        selectedNetwork.networkId = TEST_NETWORK_ID;
        selectedNetwork.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_EAP);

        when(mWifiConfigManager.getConfiguredNetworkWithoutMasking(anyInt())).thenReturn(
                selectedNetwork);

        mDppManager.startDppAsConfiguratorInitiator(0, mBinder, mUri, 1,
                EASY_CONNECT_NETWORK_ROLE_STA,
                mDppCallback);
        verify(mDppCallback).onFailure(EASY_CONNECT_EVENT_FAILURE_INVALID_NETWORK);
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mDppMetrics).updateDppConfiguratorInitiatorRequests();
        verify(mDppMetrics).updateDppFailure(EasyConnectStatusCallback
                .EASY_CONNECT_EVENT_FAILURE_INVALID_NETWORK);
        verifyNoMoreInteractions(mDppMetrics);
    }

    @Test
    public void testStartDppAsEnrolleeInitiatorStartCorrectly() throws Exception {
        mDppManager.startDppAsEnrolleeInitiator(0, mBinder, mUri, mDppCallback);
        verify(mDppCallback, never()).onFailure(anyInt());
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mWifiNative).startDppEnrolleeInitiator(eq(TEST_INTERFACE_NAME),
                eq(TEST_PEER_ID), anyInt());
        verify(mDppMetrics).updateDppEnrolleeInitiatorRequests();
        verifyNoMoreInteractions(mDppMetrics);
    }

    @Test
    public void testStartDppAsConfiguratorInitiatorStartCorrectlyAndRejectConcurrentRequest()
            throws Exception {
        // Generate a mock WifiConfiguration object
        WifiConfiguration selectedNetwork = new WifiConfiguration();
        selectedNetwork.SSID = TEST_SSID;
        selectedNetwork.networkId = 1;
        selectedNetwork.preSharedKey = TEST_PASSWORD;
        selectedNetwork.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.SAE);

        when(mWifiConfigManager.getConfiguredNetworkWithoutMasking(anyInt())).thenReturn(
                selectedNetwork);

        mDppManager.startDppAsConfiguratorInitiator(0, mBinder, mUri, 1,
                EASY_CONNECT_NETWORK_ROLE_STA,
                mDppCallback);
        verify(mDppCallback, never()).onFailure(anyInt());
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mWifiNative).startDppConfiguratorInitiator(eq(TEST_INTERFACE_NAME),
                eq(TEST_PEER_ID), anyInt(), eq(TEST_SSID_ENCODED), eq(TEST_PASSWORD_ENCODED), any(),
                eq(EASY_CONNECT_NETWORK_ROLE_STA), eq(SAE));

        mDppManager.startDppAsConfiguratorInitiator(1, mBinder, mUri, 1,
                EASY_CONNECT_NETWORK_ROLE_STA,
                mDppCallbackConcurrent);
        verify(mDppCallbackConcurrent).onFailure(EASY_CONNECT_EVENT_FAILURE_BUSY);
        verify(mDppCallbackConcurrent, never()).onSuccess(anyInt());
        verify(mDppCallbackConcurrent, never()).onSuccessConfigReceived(anyInt());
        verify(mDppMetrics, times(2)).updateDppConfiguratorInitiatorRequests();
        verify(mDppMetrics).updateDppFailure(EASY_CONNECT_EVENT_FAILURE_BUSY);
        verifyNoMoreInteractions(mDppMetrics);
    }

    @Test
    public void testStartDppAsEnrolleeInitiatorStartCorrectlyAndRejectConcurrentRequest()
            throws Exception {
        mDppManager.startDppAsEnrolleeInitiator(0, mBinder, mUri, mDppCallback);
        verify(mDppCallback, never()).onFailure(anyInt());
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mWifiNative).startDppEnrolleeInitiator(eq(TEST_INTERFACE_NAME),
                eq(TEST_PEER_ID), anyInt());

        mDppManager.startDppAsEnrolleeInitiator(1, mBinder, mUri, mDppCallbackConcurrent);
        verify(mDppCallbackConcurrent).onFailure(EASY_CONNECT_EVENT_FAILURE_BUSY);
        verify(mDppCallbackConcurrent, never()).onSuccess(anyInt());
        verify(mDppCallbackConcurrent, never()).onSuccessConfigReceived(anyInt());
        verify(mDppMetrics, times(2)).updateDppEnrolleeInitiatorRequests();
        verify(mDppMetrics).updateDppFailure(EASY_CONNECT_EVENT_FAILURE_BUSY);
        verifyNoMoreInteractions(mDppMetrics);
    }

    @Test
    public void testStartDppAsConfiguratorInitiatorStartOnSuccessCallback() throws Exception {
        // Generate a mock WifiConfiguration object
        WifiConfiguration selectedNetwork = new WifiConfiguration();
        selectedNetwork.SSID = TEST_SSID;
        selectedNetwork.networkId = 1;
        selectedNetwork.preSharedKey = TEST_PASSWORD;
        selectedNetwork.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.SAE);

        when(mWifiConfigManager.getConfiguredNetworkWithoutMasking(anyInt())).thenReturn(
                selectedNetwork);

        ArgumentCaptor<WifiNative.DppEventCallback> dppEventCallbackCaptor =
                ArgumentCaptor.forClass(
                        WifiNative.DppEventCallback.class);

        mDppManager.startDppAsConfiguratorInitiator(0, mBinder, mUri, 1,
                EASY_CONNECT_NETWORK_ROLE_STA, mDppCallback);
        verify(mWifiNative).registerDppEventCallback(dppEventCallbackCaptor.capture());
        verify(mDppCallback, never()).onFailure(anyInt());
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mWifiNative).startDppConfiguratorInitiator(eq(TEST_INTERFACE_NAME),
                eq(TEST_PEER_ID), anyInt(), eq(TEST_SSID_ENCODED), eq(TEST_PASSWORD_ENCODED), any(),
                eq(EASY_CONNECT_NETWORK_ROLE_STA), eq(
                        SAE));

        WifiNative.DppEventCallback dppEventCallback = dppEventCallbackCaptor.getValue();

        // Generate a progress event
        dppEventCallback.onProgress(AUTHENTICATION_SUCCESS);
        mLooper.dispatchAll();
        verify(mDppCallback).onProgress(EASY_CONNECT_EVENT_PROGRESS_AUTHENTICATION_SUCCESS);

        // Generate an onSuccess callback
        dppEventCallback.onSuccessConfigSent();
        mLooper.dispatchAll();
        verify(mDppCallback).onSuccess(EASY_CONNECT_EVENT_SUCCESS_CONFIGURATION_SENT);
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mDppCallback, never()).onFailure(anyInt());
        verify(mDppMetrics).updateDppConfiguratorInitiatorRequests();
        verify(mDppMetrics).updateDppConfiguratorSuccess(EasyConnectStatusCallback
                .EASY_CONNECT_EVENT_SUCCESS_CONFIGURATION_SENT);
        verify(mDppMetrics).updateDppOperationTime(anyInt());
        verifyNoMoreInteractions(mDppMetrics);
    }

    @Test
    public void testStartDppAsEnrolleeInitiatorStartCorrectlyOnSuccessCallback() throws Exception {
        ArgumentCaptor<WifiNative.DppEventCallback> dppEventCallbackCaptor =
                ArgumentCaptor.forClass(
                        WifiNative.DppEventCallback.class);

        mDppManager.startDppAsEnrolleeInitiator(0, mBinder, mUri, mDppCallback);
        verify(mWifiNative).registerDppEventCallback(dppEventCallbackCaptor.capture());
        verify(mDppCallback, never()).onFailure(anyInt());
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mWifiNative).startDppEnrolleeInitiator(eq(TEST_INTERFACE_NAME),
                eq(TEST_PEER_ID), anyInt());

        // Generate an onSuccessConfigReceived callback
        WifiNative.DppEventCallback dppEventCallback = dppEventCallbackCaptor.getValue();

        // Generate a mock WifiConfiguration object
        WifiConfiguration selectedNetwork = new WifiConfiguration();
        selectedNetwork.SSID = TEST_SSID;
        selectedNetwork.networkId = TEST_NETWORK_ID;
        selectedNetwork.preSharedKey = TEST_PASSWORD;
        selectedNetwork.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.SAE);

        // Generate a progress event
        dppEventCallback.onProgress(AUTHENTICATION_SUCCESS);
        mLooper.dispatchAll();
        verify(mDppCallback).onProgress(EASY_CONNECT_EVENT_PROGRESS_AUTHENTICATION_SUCCESS);

        // Generate result
        NetworkUpdateResult networkUpdateResult = new NetworkUpdateResult(TEST_NETWORK_ID);
        when(mWifiConfigManager.addOrUpdateNetwork(any(WifiConfiguration.class),
                anyInt())).thenReturn(networkUpdateResult);

        dppEventCallback.onSuccessConfigReceived(selectedNetwork);
        mLooper.dispatchAll();
        verify(mDppCallback).onSuccessConfigReceived(TEST_NETWORK_ID);
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onFailure(anyInt());
        verify(mDppMetrics).updateDppEnrolleeInitiatorRequests();
        verify(mDppMetrics).updateDppEnrolleeSuccess();
        verify(mDppMetrics).updateDppOperationTime(anyInt());
        verifyNoMoreInteractions(mDppMetrics);
        verifyCleanUpResources();
    }

    @Test
    public void testStartDppAsConfiguratorInitiatorStartOnFailureCallback() throws Exception {
        // Generate a mock WifiConfiguration object
        WifiConfiguration selectedNetwork = new WifiConfiguration();
        selectedNetwork.SSID = TEST_SSID;
        selectedNetwork.networkId = TEST_NETWORK_ID;
        selectedNetwork.preSharedKey = TEST_PASSWORD;
        selectedNetwork.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.SAE);

        when(mWifiConfigManager.getConfiguredNetworkWithoutMasking(anyInt())).thenReturn(
                selectedNetwork);
        ArgumentCaptor<WifiNative.DppEventCallback> dppEventCallbackCaptor =
                ArgumentCaptor.forClass(
                        WifiNative.DppEventCallback.class);

        mDppManager.startDppAsConfiguratorInitiator(0, mBinder, mUri, 1,
                EASY_CONNECT_NETWORK_ROLE_STA,
                mDppCallback);
        verify(mWifiNative).registerDppEventCallback(dppEventCallbackCaptor.capture());
        verify(mDppCallback, never()).onFailure(anyInt());
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mWifiNative).startDppConfiguratorInitiator(eq(TEST_INTERFACE_NAME),
                eq(TEST_PEER_ID), anyInt(), eq(TEST_SSID_ENCODED), eq(TEST_PASSWORD_ENCODED), any(),
                eq(EASY_CONNECT_NETWORK_ROLE_STA), eq(
                        SAE));

        WifiNative.DppEventCallback dppEventCallback = dppEventCallbackCaptor.getValue();

        // Generate a progress event
        dppEventCallback.onProgress(RESPONSE_PENDING);
        mLooper.dispatchAll();
        verify(mDppCallback).onProgress(EASY_CONNECT_EVENT_PROGRESS_RESPONSE_PENDING);

        // Generate an onFailure callback
        dppEventCallback.onFailure(AUTHENTICATION);
        mLooper.dispatchAll();
        verify(mDppCallback).onFailure(EASY_CONNECT_EVENT_FAILURE_AUTHENTICATION);
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mDppMetrics).updateDppConfiguratorInitiatorRequests();
        verify(mDppMetrics).updateDppFailure(EasyConnectStatusCallback
                .EASY_CONNECT_EVENT_FAILURE_AUTHENTICATION);
        verify(mDppMetrics).updateDppOperationTime(anyInt());
        verifyNoMoreInteractions(mDppMetrics);
        verifyCleanUpResources();
    }

    @Test
    public void testStartDppAsEnrolleeInitiatorStartCorrectlyOnFailureCallback() throws Exception {
        // Add Peer URI successful, return peer ID 1
        when(mWifiNative.addDppPeerUri(anyString(), anyString()))
                .thenReturn(1);
        when(mWifiNative.removeDppUri(anyString(), anyInt()))
                .thenReturn(true);

        // Return test interface name
        when(mWifiNative.getClientInterfaceName()).thenReturn(TEST_INTERFACE_NAME);

        // Successful start
        when(mWifiNative.startDppEnrolleeInitiator(anyString(), anyInt(), anyInt())).thenReturn(
                true);

        ArgumentCaptor<WifiNative.DppEventCallback> dppEventCallbackCaptor =
                ArgumentCaptor.forClass(
                        WifiNative.DppEventCallback.class);

        mDppManager.startDppAsEnrolleeInitiator(0, mBinder, mUri, mDppCallback);
        verify(mWifiNative).registerDppEventCallback(dppEventCallbackCaptor.capture());
        verify(mDppCallback, never()).onFailure(anyInt());
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mWifiNative).startDppEnrolleeInitiator(eq(TEST_INTERFACE_NAME),
                eq(TEST_PEER_ID), anyInt());

        WifiNative.DppEventCallback dppEventCallback = dppEventCallbackCaptor.getValue();

        // Generate a progress event
        dppEventCallback.onProgress(RESPONSE_PENDING);
        mLooper.dispatchAll();
        verify(mDppCallback).onProgress(EASY_CONNECT_EVENT_PROGRESS_RESPONSE_PENDING);

        // Generate an onFailure callback
        dppEventCallback.onFailure(AUTHENTICATION);
        mLooper.dispatchAll();
        verify(mDppCallback).onFailure(EASY_CONNECT_EVENT_FAILURE_AUTHENTICATION);
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mDppMetrics).updateDppEnrolleeInitiatorRequests();
        verify(mDppMetrics).updateDppFailure(EasyConnectStatusCallback
                .EASY_CONNECT_EVENT_FAILURE_AUTHENTICATION);
        verify(mDppMetrics).updateDppOperationTime(anyInt());
        verifyNoMoreInteractions(mDppMetrics);
        verifyCleanUpResources();
    }

    @Test
    public void testDppStopSessionNotStarted() throws Exception {
        // Check that nothing happens if no DPP session started.
        mDppManager.stopDppSession(0);

        // Check that nothing is removed or canceled
        mDppManager.stopDppSession(0);
        verify(mWifiNative, never()).removeDppUri(eq(TEST_INTERFACE_NAME), anyInt());
        verify(mWakeupMessage, never()).cancel();
    }

    @Test
    public void testDppStopSessionIncorrectUid() throws Exception {
        // Check that nothing happens if UID is incorrect
        // Add Peer URI successful, return peer ID 1
        when(mWifiNative.addDppPeerUri(anyString(), anyString()))
                .thenReturn(1);
        when(mWifiNative.removeDppUri(anyString(), anyInt()))
                .thenReturn(true);

        // Return test interface name
        when(mWifiNative.getClientInterfaceName()).thenReturn(TEST_INTERFACE_NAME);

        // Successful start
        when(mWifiNative.startDppEnrolleeInitiator(anyString(), anyInt(), anyInt())).thenReturn(
                true);

        ArgumentCaptor<WifiNative.DppEventCallback> dppEventCallbackCaptor =
                ArgumentCaptor.forClass(
                        WifiNative.DppEventCallback.class);

        // Start with UID 10
        mDppManager.startDppAsEnrolleeInitiator(10, mBinder, mUri, mDppCallback);
        verify(mWifiNative).registerDppEventCallback(dppEventCallbackCaptor.capture());
        verify(mDppCallback, never()).onFailure(anyInt());
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mWifiNative).startDppEnrolleeInitiator(eq(TEST_INTERFACE_NAME),
                eq(TEST_PEER_ID), anyInt());

        // Check that nothing is removed or canceled
        mDppManager.stopDppSession(0);
        verify(mWifiNative, never()).removeDppUri(eq(TEST_INTERFACE_NAME), anyInt());
        verify(mWakeupMessage, never()).cancel();
    }

    @Test
    public void testDppStopSession() throws Exception {
        // Check that nothing happens if UID is incorrect
        // Add Peer URI successful, return peer ID 1
        when(mWifiNative.addDppPeerUri(anyString(), anyString()))
                .thenReturn(1);
        when(mWifiNative.removeDppUri(anyString(), anyInt()))
                .thenReturn(true);

        // Return test interface name
        when(mWifiNative.getClientInterfaceName()).thenReturn(TEST_INTERFACE_NAME);

        // Successful start
        when(mWifiNative.startDppEnrolleeInitiator(anyString(), anyInt(), anyInt())).thenReturn(
                true);

        ArgumentCaptor<WifiNative.DppEventCallback> dppEventCallbackCaptor =
                ArgumentCaptor.forClass(
                        WifiNative.DppEventCallback.class);

        // Start with UID 10
        mDppManager.startDppAsEnrolleeInitiator(10, mBinder, mUri, mDppCallback);
        verify(mWifiNative).registerDppEventCallback(dppEventCallbackCaptor.capture());
        verify(mDppCallback, never()).onFailure(anyInt());
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mWifiNative).startDppEnrolleeInitiator(eq(TEST_INTERFACE_NAME),
                eq(TEST_PEER_ID), anyInt());

        // Check that WifiNative is called to stop the DPP session
        mDppManager.stopDppSession(10);
        verify(mWifiNative).stopDppInitiator(TEST_INTERFACE_NAME);
        verifyCleanUpResources();
    }

    private void verifyCleanUpResources() {
        verify(mWifiNative).removeDppUri(eq(TEST_INTERFACE_NAME), anyInt());
        verify(mWakeupMessage).cancel();
    }

    @Test
    public void testOnFailureCallbackNotCompatible() throws Exception {
        testOnFailureCallback(NOT_COMPATIBLE, EASY_CONNECT_EVENT_FAILURE_NOT_COMPATIBLE);
    }

    @Test
    public void testOnFailureCallbackConfiguration() throws Exception {
        testOnFailureCallback(CONFIGURATION, EASY_CONNECT_EVENT_FAILURE_CONFIGURATION);
    }

    @Test
    public void testOnFailureCallbackTimeout() throws Exception {
        testOnFailureCallback(TIMEOUT, EASY_CONNECT_EVENT_FAILURE_TIMEOUT);
    }

    @Test
    public void testOnFailureCallbackNotSupported() throws Exception {
        testOnFailureCallback(NOT_SUPPORTED, EASY_CONNECT_EVENT_FAILURE_NOT_SUPPORTED);
    }

    @Test
    public void testOnFailureCallbackBusy() throws Exception {
        testOnFailureCallback(BUSY, EASY_CONNECT_EVENT_FAILURE_BUSY);
    }

    @Test
    public void testOnFailureCallbackFailure() throws Exception {
        testOnFailureCallback(FAILURE, EASY_CONNECT_EVENT_FAILURE_GENERIC);
    }

    private void testOnFailureCallback(int internalFailure, int appFailure) throws Exception {
        // Generate a mock WifiConfiguration object
        WifiConfiguration selectedNetwork = new WifiConfiguration();
        selectedNetwork.SSID = TEST_SSID;
        selectedNetwork.networkId = TEST_NETWORK_ID;
        selectedNetwork.preSharedKey = TEST_PASSWORD;
        selectedNetwork.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.SAE);

        when(mWifiConfigManager.getConfiguredNetworkWithoutMasking(anyInt())).thenReturn(
                selectedNetwork);
        ArgumentCaptor<WifiNative.DppEventCallback> dppEventCallbackCaptor =
                ArgumentCaptor.forClass(
                        WifiNative.DppEventCallback.class);

        mDppManager.startDppAsConfiguratorInitiator(0, mBinder, mUri, 1,
                EASY_CONNECT_NETWORK_ROLE_STA,
                mDppCallback);
        verify(mWifiNative).registerDppEventCallback(dppEventCallbackCaptor.capture());
        verify(mDppCallback, never()).onFailure(anyInt());
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mWifiNative).startDppConfiguratorInitiator(eq(TEST_INTERFACE_NAME),
                eq(TEST_PEER_ID), anyInt(), eq(TEST_SSID_ENCODED), eq(TEST_PASSWORD_ENCODED), any(),
                eq(EASY_CONNECT_NETWORK_ROLE_STA), eq(
                        SAE));

        WifiNative.DppEventCallback dppEventCallback = dppEventCallbackCaptor.getValue();

        // Generate an onFailure callback
        dppEventCallback.onFailure(internalFailure);
        mLooper.dispatchAll();
        verify(mDppCallback).onFailure(eq(appFailure));
        verify(mDppCallback, never()).onSuccess(anyInt());
        verify(mDppCallback, never()).onSuccessConfigReceived(anyInt());
        verify(mDppMetrics).updateDppConfiguratorInitiatorRequests();
        verify(mDppMetrics).updateDppFailure(eq(appFailure));
        verify(mDppMetrics).updateDppOperationTime(anyInt());
        verifyNoMoreInteractions(mDppMetrics);
        verifyCleanUpResources();
    }
}
