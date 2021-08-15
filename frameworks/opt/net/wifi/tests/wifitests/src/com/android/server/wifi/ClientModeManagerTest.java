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

import static android.net.wifi.WifiManager.EXTRA_PREVIOUS_WIFI_STATE;
import static android.net.wifi.WifiManager.EXTRA_SCAN_AVAILABLE;
import static android.net.wifi.WifiManager.EXTRA_WIFI_STATE;
import static android.net.wifi.WifiManager.WIFI_SCAN_AVAILABLE;
import static android.net.wifi.WifiManager.WIFI_STATE_CHANGED_ACTION;
import static android.net.wifi.WifiManager.WIFI_STATE_DISABLED;
import static android.net.wifi.WifiManager.WIFI_STATE_DISABLING;
import static android.net.wifi.WifiManager.WIFI_STATE_ENABLED;
import static android.net.wifi.WifiManager.WIFI_STATE_ENABLING;
import static android.net.wifi.WifiManager.WIFI_STATE_UNKNOWN;

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.*;

import android.content.Context;
import android.content.Intent;
import android.os.UserHandle;
import android.os.test.TestLooper;
import android.test.suitebuilder.annotation.SmallTest;
import android.util.Log;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.util.List;

/**
 * Unit tests for {@link ClientModeManager}.
 */
@SmallTest
public class ClientModeManagerTest {
    private static final String TAG = "ClientModeManagerTest";
    private static final String TEST_INTERFACE_NAME = "testif0";
    private static final String OTHER_INTERFACE_NAME = "notTestIf";

    TestLooper mLooper;

    ClientModeManager mClientModeManager;

    @Mock Context mContext;
    @Mock WifiMetrics mWifiMetrics;
    @Mock WifiNative mWifiNative;
    @Mock ClientModeManager.Listener mListener;
    @Mock WifiMonitor mWifiMonitor;
    @Mock ClientModeImpl mClientModeImpl;

    final ArgumentCaptor<WifiNative.InterfaceCallback> mInterfaceCallbackCaptor =
            ArgumentCaptor.forClass(WifiNative.InterfaceCallback.class);

    @Before
    public void setUp() {
        MockitoAnnotations.initMocks(this);
        mLooper = new TestLooper();

        mClientModeManager = createClientModeManager();
        mLooper.dispatchAll();
    }

    private ClientModeManager createClientModeManager() {
        return new ClientModeManager(mContext, mLooper.getLooper(), mWifiNative, mListener,
                mWifiMetrics, mClientModeImpl);
    }

    private void startClientModeAndVerifyEnabled() throws Exception {
        ArgumentCaptor<Intent> intentCaptor = ArgumentCaptor.forClass(Intent.class);

        when(mWifiNative.setupInterfaceForClientInConnectivityMode(any()))
                .thenReturn(TEST_INTERFACE_NAME);
        mClientModeManager.start();
        mLooper.dispatchAll();

        verify(mWifiNative).setupInterfaceForClientInConnectivityMode(
                mInterfaceCallbackCaptor.capture());

        // now mark the interface as up
        mInterfaceCallbackCaptor.getValue().onUp(TEST_INTERFACE_NAME);
        mLooper.dispatchAll();

        verify(mContext, atLeastOnce()).sendStickyBroadcastAsUser(intentCaptor.capture(),
                eq(UserHandle.ALL));

        List<Intent> intents = intentCaptor.getAllValues();
        assertEquals(2, intents.size());
        Log.d(TAG, "captured intents: " + intents);
        checkWifiStateChangedBroadcast(intents.get(0), WIFI_STATE_ENABLING, WIFI_STATE_DISABLED);
        checkWifiStateChangedBroadcast(intents.get(1), WIFI_STATE_ENABLED, WIFI_STATE_ENABLING);

        checkWifiStateChangeListenerUpdate(WIFI_STATE_ENABLED);
    }

    private void checkWifiScanStateChangedBroadcast(Intent intent, int expectedCurrentState) {
        String action = intent.getAction();
        assertEquals(WIFI_SCAN_AVAILABLE, action);
        int currentState = intent.getIntExtra(EXTRA_SCAN_AVAILABLE, WIFI_STATE_UNKNOWN);
        assertEquals(expectedCurrentState, currentState);
    }

    private void checkWifiStateChangedBroadcast(
            Intent intent, int expectedCurrentState, int expectedPrevState) {
        String action = intent.getAction();
        assertEquals(WIFI_STATE_CHANGED_ACTION, action);
        int currentState = intent.getIntExtra(EXTRA_WIFI_STATE, WIFI_STATE_UNKNOWN);
        assertEquals(expectedCurrentState, currentState);
        int prevState = intent.getIntExtra(EXTRA_PREVIOUS_WIFI_STATE, WIFI_STATE_UNKNOWN);
        assertEquals(expectedPrevState, prevState);

        verify(mClientModeImpl, atLeastOnce()).setWifiStateForApiCalls(expectedCurrentState);
    }


    private void checkWifiStateChangeListenerUpdate(int expectedCurrentState) {
        verify(mListener).onStateChanged(eq(expectedCurrentState));
    }

    private void verifyNotificationsForCleanShutdown(int fromState) {
        ArgumentCaptor<Intent> intentCaptor = ArgumentCaptor.forClass(Intent.class);
        verify(mContext, atLeastOnce())
                .sendStickyBroadcastAsUser(intentCaptor.capture(), eq(UserHandle.ALL));

        List<Intent> intents = intentCaptor.getAllValues();
        assertEquals(2, intents.size());
        checkWifiStateChangedBroadcast(intents.get(0), WIFI_STATE_DISABLING, fromState);
        checkWifiStateChangedBroadcast(intents.get(1), WIFI_STATE_DISABLED, WIFI_STATE_DISABLING);
    }

    private void verifyNotificationsForFailure() {
        ArgumentCaptor<Intent> intentCaptor = ArgumentCaptor.forClass(Intent.class);
        verify(mContext, atLeastOnce())
                .sendStickyBroadcastAsUser(intentCaptor.capture(), eq(UserHandle.ALL));

        List<Intent> intents = intentCaptor.getAllValues();
        assertEquals(2, intents.size());
        checkWifiStateChangedBroadcast(intents.get(0), WIFI_STATE_DISABLING, WIFI_STATE_UNKNOWN);
        checkWifiStateChangedBroadcast(intents.get(1), WIFI_STATE_DISABLED, WIFI_STATE_DISABLING);
        checkWifiStateChangeListenerUpdate(WIFI_STATE_UNKNOWN);
    }

    /**
     * ClientMode start sets up an interface in ClientMode.
     */
    @Test
    public void clientModeStartCreatesClientInterface() throws Exception {
        startClientModeAndVerifyEnabled();
    }

    /**
     * ClientMode increments failure metrics when failing to setup client mode.
     */
    @Test
    public void detectAndReportErrorWhenSetupForClientWifiNativeFailure() throws Exception {
        when(mWifiNative.setupInterfaceForClientInConnectivityMode(any())).thenReturn(null);
        mClientModeManager.start();
        mLooper.dispatchAll();

        ArgumentCaptor<Intent> intentCaptor = ArgumentCaptor.forClass(Intent.class);
        verify(mContext, atLeastOnce()).sendStickyBroadcastAsUser(intentCaptor.capture(),
                eq(UserHandle.ALL));
        List<Intent> intents = intentCaptor.getAllValues();
        assertEquals(2, intents.size());
        checkWifiStateChangedBroadcast(intents.get(0), WIFI_STATE_ENABLING, WIFI_STATE_DISABLED);
        checkWifiStateChangedBroadcast(intents.get(1), WIFI_STATE_DISABLED, WIFI_STATE_UNKNOWN);
        checkWifiStateChangeListenerUpdate(WIFI_STATE_UNKNOWN);
    }

    /**
     * ClientMode start does not indicate scanning is available when the interface name is empty.
     */
    @Test
    public void clientModeStartDoesNotSendScanningActiveWhenClientInterfaceNameIsEmpty()
            throws Exception {
        when(mWifiNative.setupInterfaceForClientInConnectivityMode(any())).thenReturn("");
        mClientModeManager.start();
        mLooper.dispatchAll();

        ArgumentCaptor<Intent> intentCaptor = ArgumentCaptor.forClass(Intent.class);
        verify(mContext, atLeastOnce()).sendStickyBroadcastAsUser(intentCaptor.capture(),
                eq(UserHandle.ALL));

        List<Intent> intents = intentCaptor.getAllValues();
        assertEquals(2, intents.size());
        checkWifiStateChangedBroadcast(intents.get(0), WIFI_STATE_ENABLING, WIFI_STATE_DISABLED);
        checkWifiStateChangedBroadcast(intents.get(1), WIFI_STATE_DISABLED, WIFI_STATE_UNKNOWN);
        checkWifiStateChangeListenerUpdate(WIFI_STATE_UNKNOWN);
    }

    /**
     * Calling ClientModeManager.start twice does not crash or restart client mode.
     */
    @Test
    public void clientModeStartCalledTwice() throws Exception {
        startClientModeAndVerifyEnabled();
        reset(mWifiNative, mContext);
        mClientModeManager.start();
        mLooper.dispatchAll();
        verifyNoMoreInteractions(mWifiNative, mContext);
    }

    /**
     * ClientMode stop properly cleans up state
     */
    @Test
    public void clientModeStopCleansUpState() throws Exception {
        startClientModeAndVerifyEnabled();
        reset(mContext, mListener);
        mClientModeManager.stop();
        mLooper.dispatchAll();

        verifyNotificationsForCleanShutdown(WIFI_STATE_ENABLED);

        // on an explicit stop, we should not trigger the callback
        verifyNoMoreInteractions(mListener);
    }

    /**
     * Calling stop when ClientMode is not started should not send scan state updates
     */
    @Test
    public void clientModeStopWhenNotStartedDoesNotUpdateScanStateUpdates() throws Exception {
        startClientModeAndVerifyEnabled();
        reset(mContext);
        mClientModeManager.stop();
        mLooper.dispatchAll();
        verifyNotificationsForCleanShutdown(WIFI_STATE_ENABLED);

        reset(mContext, mListener);
        // now call stop again
        mClientModeManager.stop();
        mLooper.dispatchAll();
        verify(mContext, never()).sendStickyBroadcastAsUser(any(), any());
        verify(mListener, never()).onStateChanged(anyInt());
    }

    /**
     * Triggering interface down when ClientMode is active properly exits the active state.
     */
    @Test
    public void clientModeStartedStopsWhenInterfaceDown() throws Exception {
        startClientModeAndVerifyEnabled();
        reset(mContext);
        when(mClientModeImpl.isConnectedMacRandomizationEnabled()).thenReturn(false);
        mInterfaceCallbackCaptor.getValue().onDown(TEST_INTERFACE_NAME);
        mLooper.dispatchAll();
        verify(mClientModeImpl).failureDetected(eq(SelfRecovery.REASON_STA_IFACE_DOWN));
        verifyNotificationsForFailure();
    }

    /**
     * Triggering interface down when ClientMode is active and Connected MacRandomization is enabled
     * does not exit the active state.
     */
    @Test
    public void clientModeStartedWithConnectedMacRandDoesNotStopWhenInterfaceDown()
            throws Exception {
        startClientModeAndVerifyEnabled();
        reset(mContext);
        when(mClientModeImpl.isConnectedMacRandomizationEnabled()).thenReturn(true);
        mInterfaceCallbackCaptor.getValue().onDown(TEST_INTERFACE_NAME);
        mLooper.dispatchAll();
        verify(mClientModeImpl, never()).failureDetected(eq(SelfRecovery.REASON_STA_IFACE_DOWN));
        verify(mContext, never()).sendStickyBroadcastAsUser(any(), any());
    }

    /**
     * Testing the handling of an interface destroyed notification.
     */
    @Test
    public void clientModeStartedStopsOnInterfaceDestroyed() throws Exception {
        startClientModeAndVerifyEnabled();
        reset(mContext);
        mInterfaceCallbackCaptor.getValue().onDestroyed(TEST_INTERFACE_NAME);
        mLooper.dispatchAll();
        verifyNotificationsForCleanShutdown(WIFI_STATE_ENABLED);
        verify(mClientModeImpl).handleIfaceDestroyed();
    }

    /**
     * Verify that onDestroyed after client mode is stopped doesn't trigger a callback.
     */
    @Test
    public void noCallbackOnInterfaceDestroyedWhenAlreadyStopped() throws Exception {
        startClientModeAndVerifyEnabled();

        reset(mListener);

        mClientModeManager.stop();
        mLooper.dispatchAll();

        // now trigger interface destroyed and make sure callback doesn't get called
        mInterfaceCallbackCaptor.getValue().onDestroyed(TEST_INTERFACE_NAME);
        mLooper.dispatchAll();

        verifyNoMoreInteractions(mListener);
        verify(mClientModeImpl, never()).handleIfaceDestroyed();
    }
}
