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
import static org.junit.Assert.assertNull;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.spy;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.hardware.wifi.supplicant.V1_0.ISupplicantStaIfaceCallback.WpsConfigError;
import android.hardware.wifi.supplicant.V1_0.ISupplicantStaIfaceCallback.WpsErrorIndication;
import android.net.wifi.SupplicantState;
import android.net.wifi.WifiEnterpriseConfig;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiSsid;
import android.os.Handler;
import android.os.Message;
import android.os.test.TestLooper;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.hotspot2.AnqpEvent;
import com.android.server.wifi.hotspot2.IconEvent;
import com.android.server.wifi.util.TelephonyUtil;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;

/**
 * Unit tests for {@link com.android.server.wifi.WifiMonitor}.
 */
@SmallTest
public class WifiMonitorTest {
    private static final String WLAN_IFACE_NAME = "wlan0";
    private static final String SECOND_WLAN_IFACE_NAME = "wlan1";
    private static final String[] GSM_AUTH_DATA = { "45adbc", "fead45", "0x3452"};
    private static final String[] UMTS_AUTH_DATA = { "fead45", "0x3452"};
    private static final String BSSID = "fe:45:23:12:12:0a";
    private static final int NETWORK_ID = 5;
    private static final String SSID = "\"test124\"";
    private WifiMonitor mWifiMonitor;
    private TestLooper mLooper;
    private Handler mHandlerSpy;
    private Handler mSecondHandlerSpy;

    @Before
    public void setUp() throws Exception {
        mWifiMonitor = new WifiMonitor(mock(WifiInjector.class));
        mLooper = new TestLooper();
        mHandlerSpy = spy(new Handler(mLooper.getLooper()));
        mSecondHandlerSpy = spy(new Handler(mLooper.getLooper()));
        mWifiMonitor.setMonitoring(WLAN_IFACE_NAME, true);
    }

    /**
     * Broadcast WPS failure event test.
     */
    @Test
    public void testBroadcastWpsEventFailDueToErrorTkipOnlyProhibhited() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.WPS_FAIL_EVENT, mHandlerSpy);
        mWifiMonitor.broadcastWpsFailEvent(
                WLAN_IFACE_NAME, WpsConfigError.NO_ERROR,
                WpsErrorIndication.SECURITY_TKIP_ONLY_PROHIBITED);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.WPS_FAIL_EVENT, messageCaptor.getValue().what);
        assertEquals(WifiManager.WPS_TKIP_ONLY_PROHIBITED, messageCaptor.getValue().arg1);
    }

    /**
     * Broadcast WPS failure event test.
     */
    @Test
    public void testBroadcastWpsEventFailDueToErrorWepProhibhited() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.WPS_FAIL_EVENT, mHandlerSpy);
        mWifiMonitor.broadcastWpsFailEvent(
                WLAN_IFACE_NAME, WpsConfigError.NO_ERROR,
                WpsErrorIndication.SECURITY_WEP_PROHIBITED);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.WPS_FAIL_EVENT, messageCaptor.getValue().what);
        assertEquals(WifiManager.WPS_WEP_PROHIBITED, messageCaptor.getValue().arg1);
    }

    /**
     * Broadcast WPS failure event test.
     */
    @Test
    public void testBroadcastWpsEventFailDueToConfigAuthError() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.WPS_FAIL_EVENT, mHandlerSpy);
        mWifiMonitor.broadcastWpsFailEvent(
                WLAN_IFACE_NAME, WpsConfigError.DEV_PASSWORD_AUTH_FAILURE,
                WpsErrorIndication.NO_ERROR);

        mLooper.dispatchAll();
        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.WPS_FAIL_EVENT, messageCaptor.getValue().what);
        assertEquals(WifiManager.WPS_AUTH_FAILURE, messageCaptor.getValue().arg1);
    }

    /**
     * Broadcast WPS failure event test.
     */
    @Test
    public void testBroadcastWpsEventFailDueToConfigPbcOverlapError() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.WPS_FAIL_EVENT, mHandlerSpy);
        mWifiMonitor.broadcastWpsFailEvent(
                WLAN_IFACE_NAME, WpsConfigError.MULTIPLE_PBC_DETECTED,
                WpsErrorIndication.NO_ERROR);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.WPS_FAIL_EVENT, messageCaptor.getValue().what);
        assertEquals(WifiManager.WPS_OVERLAP_ERROR, messageCaptor.getValue().arg1);
    }

    /**
     * Broadcast WPS failure event test.
     */
    @Test
    public void testBroadcastWpsEventFailDueToConfigError() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.WPS_FAIL_EVENT, mHandlerSpy);
        mWifiMonitor.broadcastWpsFailEvent(
                WLAN_IFACE_NAME, WpsConfigError.MSG_TIMEOUT,
                WpsErrorIndication.NO_ERROR);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.WPS_FAIL_EVENT, messageCaptor.getValue().what);
        assertEquals(WifiManager.ERROR, messageCaptor.getValue().arg1);
        assertEquals(WpsConfigError.MSG_TIMEOUT, messageCaptor.getValue().arg2);
    }

    /**
     * Broadcast WPS success event test.
     */
    @Test
    public void testBroadcastWpsEventSuccess() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.WPS_SUCCESS_EVENT, mHandlerSpy);
        mWifiMonitor.broadcastWpsSuccessEvent(WLAN_IFACE_NAME);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.WPS_SUCCESS_EVENT, messageCaptor.getValue().what);
    }

    /**
     * Broadcast WPS overlap event test.
     */
    @Test
    public void testBroadcastWpsEventOverlap() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.WPS_OVERLAP_EVENT, mHandlerSpy);
        mWifiMonitor.broadcastWpsOverlapEvent(WLAN_IFACE_NAME);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.WPS_OVERLAP_EVENT, messageCaptor.getValue().what);
    }

    /**
     * Broadcast WPS timeout event test.
     */
    @Test
    public void testBroadcastWpsEventTimeout() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.WPS_TIMEOUT_EVENT, mHandlerSpy);
        mWifiMonitor.broadcastWpsTimeoutEvent(WLAN_IFACE_NAME);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.WPS_TIMEOUT_EVENT, messageCaptor.getValue().what);
    }

    /**
     * Broadcast ANQP done event test.
     */
    @Test
    public void testBroadcastAnqpDoneEvent() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.ANQP_DONE_EVENT, mHandlerSpy);
        long bssid = 5;
        mWifiMonitor.broadcastAnqpDoneEvent(WLAN_IFACE_NAME, new AnqpEvent(bssid, null));
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.ANQP_DONE_EVENT, messageCaptor.getValue().what);
        assertEquals(bssid, ((AnqpEvent) messageCaptor.getValue().obj).getBssid());
        assertNull(((AnqpEvent) messageCaptor.getValue().obj).getElements());
    }

    /**
     * Broadcast Icon event test.
     */
    @Test
    public void testBroadcastIconDoneEvent() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.RX_HS20_ANQP_ICON_EVENT, mHandlerSpy);
        long bssid = 5;
        String fileName = "test";
        int fileSize = 0;
        mWifiMonitor.broadcastIconDoneEvent(
                WLAN_IFACE_NAME, new IconEvent(bssid, fileName, fileSize, null));
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.RX_HS20_ANQP_ICON_EVENT, messageCaptor.getValue().what);
        assertEquals(bssid, ((IconEvent) messageCaptor.getValue().obj).getBSSID());
        assertEquals(fileName, ((IconEvent) messageCaptor.getValue().obj).getFileName());
        assertEquals(fileSize, ((IconEvent) messageCaptor.getValue().obj).getSize());
        assertNull(((IconEvent) messageCaptor.getValue().obj).getData());
    }

    /**
     * Broadcast network Gsm auth request test.
     */
    @Test
    public void testBroadcastNetworkGsmAuthRequestEvent() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.SUP_REQUEST_SIM_AUTH, mHandlerSpy);
        int networkId = NETWORK_ID;
        String ssid = SSID;
        String[] data = GSM_AUTH_DATA;
        mWifiMonitor.broadcastNetworkGsmAuthRequestEvent(WLAN_IFACE_NAME, networkId, ssid, data);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.SUP_REQUEST_SIM_AUTH, messageCaptor.getValue().what);
        TelephonyUtil.SimAuthRequestData authData =
                (TelephonyUtil.SimAuthRequestData) messageCaptor.getValue().obj;
        assertEquals(networkId, authData.networkId);
        assertEquals(ssid, authData.ssid);
        assertEquals(WifiEnterpriseConfig.Eap.SIM, authData.protocol);
        assertArrayEquals(data, authData.data);
    }

    /**
     * Broadcast network Umts auth request test.
     */
    @Test
    public void testBroadcastNetworkUmtsAuthRequestEvent() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.SUP_REQUEST_SIM_AUTH, mHandlerSpy);
        int networkId = NETWORK_ID;
        String ssid = SSID;
        String[] data = UMTS_AUTH_DATA;
        mWifiMonitor.broadcastNetworkUmtsAuthRequestEvent(WLAN_IFACE_NAME, networkId, ssid, data);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.SUP_REQUEST_SIM_AUTH, messageCaptor.getValue().what);
        TelephonyUtil.SimAuthRequestData authData =
                (TelephonyUtil.SimAuthRequestData) messageCaptor.getValue().obj;
        assertEquals(networkId, authData.networkId);
        assertEquals(ssid, authData.ssid);
        assertEquals(WifiEnterpriseConfig.Eap.AKA, authData.protocol);
        assertArrayEquals(data, authData.data);
    }

    /**
     * Broadcast pno scan results event test.
     */
    @Test
    public void testBroadcastPnoScanResultsEvent() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.PNO_SCAN_RESULTS_EVENT, mHandlerSpy);
        mWifiMonitor.broadcastPnoScanResultEvent(WLAN_IFACE_NAME);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.PNO_SCAN_RESULTS_EVENT, messageCaptor.getValue().what);
    }

    /**
     * Broadcast Scan results event test.
     */
    @Test
    public void testBroadcastScanResultsEvent() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.SCAN_RESULTS_EVENT, mHandlerSpy);
        mWifiMonitor.broadcastScanResultEvent(WLAN_IFACE_NAME);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.SCAN_RESULTS_EVENT, messageCaptor.getValue().what);
    }

    /**
     * Broadcast Scan failed event test.
     */
    @Test
    public void testBroadcastScanFailedEvent() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.SCAN_FAILED_EVENT, mHandlerSpy);
        mWifiMonitor.broadcastScanFailedEvent(WLAN_IFACE_NAME);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());

        assertEquals(WifiMonitor.SCAN_FAILED_EVENT, messageCaptor.getValue().what);
    }

    /**
     * Broadcast authentication failure test.
     */
    @Test
    public void testBroadcastAuthenticationFailureEvent() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.AUTHENTICATION_FAILURE_EVENT, mHandlerSpy);
        int reason = WifiManager.ERROR_AUTH_FAILURE_WRONG_PSWD;
        mWifiMonitor.broadcastAuthenticationFailureEvent(WLAN_IFACE_NAME, reason, -1);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.AUTHENTICATION_FAILURE_EVENT, messageCaptor.getValue().what);
        assertEquals(reason, messageCaptor.getValue().arg1);
    }

    /**
     * Broadcast authentication failure test (EAP Error).
     */
    @Test
    public void testBroadcastAuthenticationFailureEapErrorEvent() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.AUTHENTICATION_FAILURE_EVENT, mHandlerSpy);
        int reason = WifiManager.ERROR_AUTH_FAILURE_EAP_FAILURE;
	int errorCode = WifiNative.EAP_SIM_VENDOR_SPECIFIC_CERT_EXPIRED;
        mWifiMonitor.broadcastAuthenticationFailureEvent(WLAN_IFACE_NAME, reason, errorCode);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.AUTHENTICATION_FAILURE_EVENT, messageCaptor.getValue().what);
        assertEquals(reason, messageCaptor.getValue().arg1);
	assertEquals(errorCode, messageCaptor.getValue().arg2);
    }

    /**
     * Broadcast association rejection test.
     */
    @Test
    public void testBroadcastAssociationRejectionEvent() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.ASSOCIATION_REJECTION_EVENT, mHandlerSpy);
        int status = 5;
        String bssid = BSSID;
        mWifiMonitor.broadcastAssociationRejectionEvent(WLAN_IFACE_NAME, status, false, bssid);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.ASSOCIATION_REJECTION_EVENT, messageCaptor.getValue().what);
        assertEquals(0, messageCaptor.getValue().arg1);
        assertEquals(status, messageCaptor.getValue().arg2);
        assertEquals(bssid, (String) messageCaptor.getValue().obj);
    }

    /**
     * Broadcast associated bssid test.
     */
    @Test
    public void testBroadcastAssociatedBssidEvent() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, ClientModeImpl.CMD_ASSOCIATED_BSSID, mHandlerSpy);
        String bssid = BSSID;
        mWifiMonitor.broadcastAssociatedBssidEvent(WLAN_IFACE_NAME, bssid);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(ClientModeImpl.CMD_ASSOCIATED_BSSID, messageCaptor.getValue().what);
        assertEquals(bssid, (String) messageCaptor.getValue().obj);
    }

    /**
     * Broadcast network connection test.
     */
    @Test
    public void testBroadcastNetworkConnectionEvent() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.NETWORK_CONNECTION_EVENT, mHandlerSpy);
        int networkId = NETWORK_ID;
        String bssid = BSSID;
        mWifiMonitor.broadcastNetworkConnectionEvent(WLAN_IFACE_NAME, networkId, bssid);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.NETWORK_CONNECTION_EVENT, messageCaptor.getValue().what);
        assertEquals(networkId, messageCaptor.getValue().arg1);
        assertEquals(bssid, (String) messageCaptor.getValue().obj);
    }

    /**
     * Broadcast network disconnection test.
     */
    @Test
    public void testBroadcastNetworkDisconnectionEvent() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.NETWORK_DISCONNECTION_EVENT, mHandlerSpy);
        int local = 1;
        int reason  = 5;
        String bssid = BSSID;
        mWifiMonitor.broadcastNetworkDisconnectionEvent(WLAN_IFACE_NAME, local, reason, bssid);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.NETWORK_DISCONNECTION_EVENT, messageCaptor.getValue().what);
        assertEquals(local, messageCaptor.getValue().arg1);
        assertEquals(reason, messageCaptor.getValue().arg2);
        assertEquals(bssid, (String) messageCaptor.getValue().obj);
    }

    /**
     * Broadcast supplicant state change test.
     */
    @Test
    public void testBroadcastSupplicantStateChangeEvent() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.SUPPLICANT_STATE_CHANGE_EVENT, mHandlerSpy);
        int networkId = NETWORK_ID;
        WifiSsid wifiSsid = WifiSsid.createFromAsciiEncoded(SSID);
        String bssid = BSSID;
        SupplicantState newState = SupplicantState.ASSOCIATED;
        mWifiMonitor.broadcastSupplicantStateChangeEvent(
                WLAN_IFACE_NAME, networkId, wifiSsid, bssid, newState);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.SUPPLICANT_STATE_CHANGE_EVENT, messageCaptor.getValue().what);
        StateChangeResult result = (StateChangeResult) messageCaptor.getValue().obj;
        assertEquals(networkId, result.networkId);
        assertEquals(wifiSsid, result.wifiSsid);
        assertEquals(bssid, result.BSSID);
        assertEquals(newState, result.state);
    }

    /**
     * Broadcast supplicant connection test.
     */
    @Test
    public void testBroadcastSupplicantConnectionEvent() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.SUP_CONNECTION_EVENT, mHandlerSpy);
        mWifiMonitor.broadcastSupplicantConnectionEvent(WLAN_IFACE_NAME);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.SUP_CONNECTION_EVENT, messageCaptor.getValue().what);
    }
    /**
     * Broadcast supplicant disconnection test.
     */
    @Test
    public void testBroadcastSupplicantDisconnectionEvent() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.SUP_DISCONNECTION_EVENT, mHandlerSpy);
        mWifiMonitor.broadcastSupplicantDisconnectionEvent(WLAN_IFACE_NAME);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.SUP_DISCONNECTION_EVENT, messageCaptor.getValue().what);
    }
    /**
     * Broadcast message to two handlers test.
     */
    @Test
    public void testBroadcastEventToTwoHandlers() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.SUP_CONNECTION_EVENT, mHandlerSpy);
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.SUP_CONNECTION_EVENT, mSecondHandlerSpy);
        mWifiMonitor.broadcastSupplicantConnectionEvent(WLAN_IFACE_NAME);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.SUP_CONNECTION_EVENT, messageCaptor.getValue().what);
        verify(mSecondHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.SUP_CONNECTION_EVENT, messageCaptor.getValue().what);
    }
    /**
     * Broadcast message when iface is null.
     */
    @Test
    public void testBroadcastEventWhenIfaceIsNull() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.SUP_DISCONNECTION_EVENT, mHandlerSpy);
        mWifiMonitor.broadcastSupplicantDisconnectionEvent(null);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.SUP_DISCONNECTION_EVENT, messageCaptor.getValue().what);
    }
    /**
     * Broadcast message when iface handler is null.
     */
    @Test
    public void testBroadcastEventWhenIfaceHandlerIsNull() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.SUP_DISCONNECTION_EVENT, mHandlerSpy);
        mWifiMonitor.broadcastSupplicantDisconnectionEvent(SECOND_WLAN_IFACE_NAME);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.SUP_DISCONNECTION_EVENT, messageCaptor.getValue().what);
    }

    @Test
    public void testDeregisterHandlerNotCrash() {
        mWifiMonitor.deregisterHandler(null, 0, null);
    }

    /**
     * Register a handler, send an event and then verify that the event is handled.
     * Unregister the handler, send an event and then verify the event is not handled.
     */
    @Test
    public void testDeregisterHandlerRemovesHandler() {
        mWifiMonitor.registerHandler(
                WLAN_IFACE_NAME, WifiMonitor.WPS_FAIL_EVENT, mHandlerSpy);
        mWifiMonitor.broadcastWpsFailEvent(
                WLAN_IFACE_NAME, WpsConfigError.NO_ERROR,
                WpsErrorIndication.SECURITY_TKIP_ONLY_PROHIBITED);
        mLooper.dispatchAll();

        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mHandlerSpy, times(1)).handleMessage(messageCaptor.capture());
        assertEquals(WifiMonitor.WPS_FAIL_EVENT, messageCaptor.getValue().what);
        assertEquals(WifiManager.WPS_TKIP_ONLY_PROHIBITED, messageCaptor.getValue().arg1);
        mWifiMonitor.deregisterHandler(
                WLAN_IFACE_NAME, WifiMonitor.WPS_FAIL_EVENT, mHandlerSpy);
        mWifiMonitor.broadcastWpsFailEvent(
                WLAN_IFACE_NAME, WpsConfigError.NO_ERROR,
                WpsErrorIndication.SECURITY_TKIP_ONLY_PROHIBITED);
        mLooper.dispatchAll();

        verify(mHandlerSpy, times(1)).handleMessage(messageCaptor.capture());
    }


}
