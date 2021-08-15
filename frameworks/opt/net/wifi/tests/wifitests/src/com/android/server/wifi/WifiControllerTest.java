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

import static com.android.server.wifi.WifiController.CMD_AP_STOPPED;
import static com.android.server.wifi.WifiController.CMD_EMERGENCY_CALL_STATE_CHANGED;
import static com.android.server.wifi.WifiController.CMD_EMERGENCY_MODE_CHANGED;
import static com.android.server.wifi.WifiController.CMD_RECOVERY_DISABLE_WIFI;
import static com.android.server.wifi.WifiController.CMD_RECOVERY_RESTART_WIFI;
import static com.android.server.wifi.WifiController.CMD_SCANNING_STOPPED;
import static com.android.server.wifi.WifiController.CMD_SCAN_ALWAYS_MODE_CHANGED;
import static com.android.server.wifi.WifiController.CMD_SET_AP;
import static com.android.server.wifi.WifiController.CMD_STA_STOPPED;
import static com.android.server.wifi.WifiController.CMD_WIFI_TOGGLED;

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.*;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Resources;
import android.location.LocationManager;
import android.net.wifi.WifiManager;
import android.os.test.TestLooper;
import android.util.Log;

import androidx.test.filters.SmallTest;

import com.android.internal.R;
import com.android.internal.util.IState;
import com.android.internal.util.StateMachine;
import com.android.server.wifi.util.WifiPermissionsUtil;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.InOrder;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.io.ByteArrayOutputStream;
import java.io.PrintWriter;
import java.lang.reflect.Method;

/**
 * Test WifiController for changes in and out of ECM and SoftAP modes.
 */
@SmallTest
public class WifiControllerTest {

    private static final String TAG = "WifiControllerTest";

    private static final int TEST_WIFI_RECOVERY_DELAY_MS = 2000;

    private void dumpState() {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        PrintWriter writer = new PrintWriter(stream);
        mWifiController.dump(null, writer, null);
        writer.flush();
        Log.d(TAG, "ClientModeImpl state -" + stream.toString());
    }

    private IState getCurrentState() throws Exception {
        Method method = StateMachine.class.getDeclaredMethod("getCurrentState");
        method.setAccessible(true);
        return (IState) method.invoke(mWifiController);
    }

    private void initializeSettingsStore() throws Exception {
        when(mSettingsStore.isAirplaneModeOn()).thenReturn(false);
        when(mSettingsStore.isWifiToggleEnabled()).thenReturn(false);
        when(mSettingsStore.isScanAlwaysAvailable()).thenReturn(false);
    }

    TestLooper mLooper;
    @Mock Context mContext;
    @Mock Resources mResources;
    @Mock FrameworkFacade mFacade;
    @Mock WifiSettingsStore mSettingsStore;
    @Mock ClientModeImpl mClientModeImpl;
    @Mock ActiveModeWarden mActiveModeWarden;
    @Mock WifiPermissionsUtil mWifiPermissionsUtil;

    WifiController mWifiController;

    private BroadcastReceiver mBroadcastReceiver;

    private ClientModeManager.Listener mClientModeCallback;
    private ScanOnlyModeManager.Listener mScanOnlyModeCallback;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);

        mLooper = new TestLooper();

        initializeSettingsStore();

        when(mContext.getResources()).thenReturn(mResources);

        when(mResources.getInteger(R.integer.config_wifi_framework_recovery_timeout_delay))
                .thenReturn(TEST_WIFI_RECOVERY_DELAY_MS);
        when(mWifiPermissionsUtil.isLocationModeEnabled()).thenReturn(true);

        mWifiController = new WifiController(mContext, mClientModeImpl, mLooper.getLooper(),
                mSettingsStore, mLooper.getLooper(), mFacade, mActiveModeWarden,
                mWifiPermissionsUtil);
        mWifiController.start();
        mLooper.dispatchAll();

        ArgumentCaptor<BroadcastReceiver> bcastRxCaptor = ArgumentCaptor.forClass(
                BroadcastReceiver.class);
        verify(mContext).registerReceiver(bcastRxCaptor.capture(), any(IntentFilter.class));
        mBroadcastReceiver = bcastRxCaptor.getValue();

        ArgumentCaptor<ClientModeManager.Listener> clientModeCallbackCaptor =
                ArgumentCaptor.forClass(ClientModeManager.Listener.class);
        verify(mActiveModeWarden).registerClientModeCallback(clientModeCallbackCaptor.capture());
        mClientModeCallback = clientModeCallbackCaptor.getValue();

        ArgumentCaptor<ScanOnlyModeManager.Listener> scanOnlyModeCallbackCaptor =
                ArgumentCaptor.forClass(ScanOnlyModeManager.Listener.class);
        verify(mActiveModeWarden).registerScanOnlyCallback(scanOnlyModeCallbackCaptor.capture());
        mScanOnlyModeCallback = scanOnlyModeCallbackCaptor.getValue();

    }

    @After
    public void cleanUp() {
        mLooper.dispatchAll();
    }

    /**
     * Verify that toggling wifi from disabled starts client mode.
     */
    @Test
    public void enableWifi() throws Exception {
        assertEquals("StaDisabledState", getCurrentState().getName());

        when(mSettingsStore.isWifiToggleEnabled()).thenReturn(true);
        mWifiController.sendMessage(CMD_WIFI_TOGGLED);
        mLooper.dispatchAll();
        assertEquals("StaEnabledState", getCurrentState().getName());
    }

    /**
     * Test verifying that we can enter scan mode when the scan mode changes
     */
    @Test
    public void enableScanMode() throws Exception {
        when(mSettingsStore.isScanAlwaysAvailable()).thenReturn(true);
        mWifiController.sendMessage(CMD_SCAN_ALWAYS_MODE_CHANGED);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).enterScanOnlyMode();
    }

    /**
     * Verify that if scanning is enabled at startup, we enter scan mode
     */
    @Test
    public void testEnterScanModeAtStartWhenSet() throws Exception {
        when(mSettingsStore.isScanAlwaysAvailable()).thenReturn(true);

        // reset to avoid the default behavior
        reset(mActiveModeWarden);

        WifiController wifiController =
                new WifiController(mContext, mClientModeImpl, mLooper.getLooper(),
                                   mSettingsStore, mLooper.getLooper(), mFacade,
                                   mActiveModeWarden, mWifiPermissionsUtil);

        wifiController.start();
        mLooper.dispatchAll();

        verify(mActiveModeWarden, never()).disableWifi();
        verify(mActiveModeWarden).enterScanOnlyMode();
    }

    /**
     * Do not enter scan mode if location mode disabled.
     */
    @Test
    public void testDoesNotEnterScanModeWhenLocationModeDisabled() throws Exception {
        // Start a new WifiController with wifi disabled
        when(mSettingsStore.isAirplaneModeOn()).thenReturn(false);
        when(mSettingsStore.isWifiToggleEnabled()).thenReturn(false);
        when(mSettingsStore.isScanAlwaysAvailable()).thenReturn(false);
        when(mWifiPermissionsUtil.isLocationModeEnabled()).thenReturn(false);

        mWifiController = new WifiController(mContext, mClientModeImpl, mLooper.getLooper(),
                mSettingsStore, mLooper.getLooper(), mFacade, mActiveModeWarden,
                mWifiPermissionsUtil);

        reset(mActiveModeWarden);
        mWifiController.start();
        mLooper.dispatchAll();

        verify(mActiveModeWarden).disableWifi();

        // toggling scan always available is not sufficient for scan mode
        when(mSettingsStore.isScanAlwaysAvailable()).thenReturn(true);
        mWifiController.sendMessage(CMD_SCAN_ALWAYS_MODE_CHANGED);
        mLooper.dispatchAll();

        verify(mActiveModeWarden, never()).enterScanOnlyMode();

    }

    /**
     * Only enter scan mode if location mode enabled
     */
    @Test
    public void testEnterScanModeWhenLocationModeEnabled() throws Exception {
        when(mSettingsStore.isScanAlwaysAvailable()).thenReturn(true);
        when(mWifiPermissionsUtil.isLocationModeEnabled()).thenReturn(false);

        reset(mContext, mActiveModeWarden);
        when(mContext.getResources()).thenReturn(mResources);
        mWifiController = new WifiController(mContext, mClientModeImpl, mLooper.getLooper(),
                mSettingsStore, mLooper.getLooper(), mFacade, mActiveModeWarden,
                mWifiPermissionsUtil);

        mWifiController.start();
        mLooper.dispatchAll();

        ArgumentCaptor<BroadcastReceiver> bcastRxCaptor = ArgumentCaptor.forClass(
                BroadcastReceiver.class);
        verify(mContext).registerReceiver(bcastRxCaptor.capture(), any(IntentFilter.class));
        mBroadcastReceiver = bcastRxCaptor.getValue();

        verify(mActiveModeWarden).disableWifi();

        when(mWifiPermissionsUtil.isLocationModeEnabled()).thenReturn(true);
        Intent intent = new Intent(LocationManager.MODE_CHANGED_ACTION);

        mBroadcastReceiver.onReceive(mContext, intent);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).enterScanOnlyMode();
    }



    /**
     * Disabling location mode when in scan mode will disable wifi
     */
    @Test
    public void testExitScanModeWhenLocationModeDisabled() throws Exception {
        when(mSettingsStore.isScanAlwaysAvailable()).thenReturn(true);
        when(mWifiPermissionsUtil.isLocationModeEnabled()).thenReturn(true);

        reset(mContext, mActiveModeWarden);
        when(mContext.getResources()).thenReturn(mResources);
        mWifiController = new WifiController(mContext, mClientModeImpl, mLooper.getLooper(),
                mSettingsStore, mLooper.getLooper(), mFacade, mActiveModeWarden,
                mWifiPermissionsUtil);
        mWifiController.start();
        mLooper.dispatchAll();

        ArgumentCaptor<BroadcastReceiver> bcastRxCaptor = ArgumentCaptor.forClass(
                BroadcastReceiver.class);
        verify(mContext).registerReceiver(bcastRxCaptor.capture(), any(IntentFilter.class));
        mBroadcastReceiver = bcastRxCaptor.getValue();

        verify(mActiveModeWarden).enterScanOnlyMode();

        when(mWifiPermissionsUtil.isLocationModeEnabled()).thenReturn(false);
        Intent intent = new Intent(LocationManager.MODE_CHANGED_ACTION);

        mBroadcastReceiver.onReceive(mContext, intent);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).disableWifi();
    }

    /**
     * When in Client mode, make sure ECM triggers wifi shutdown.
     */
    @Test
    public void testEcmOnFromClientMode() throws Exception {
        when(mSettingsStore.isScanAlwaysAvailable()).thenReturn(false);
        enableWifi();

        // Test with WifiDisableInECBM turned on:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);

        // test ecm changed
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();

        verify(mActiveModeWarden).shutdownWifi();
    }

    /**
     * ECM disabling messages, when in client mode (not expected) do not trigger state changes.
     */
    @Test
    public void testEcmOffInClientMode() throws Exception {
        when(mSettingsStore.isScanAlwaysAvailable()).thenReturn(false);
        enableWifi();

        // Test with WifiDisableInECBM turned off
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(false);

        // test ecm changed
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();

        verify(mActiveModeWarden, never()).shutdownWifi();
        verify(mActiveModeWarden).stopSoftAPMode(WifiManager.IFACE_IP_MODE_UNSPECIFIED);
    }

    /**
     * When ECM activates and we are in client mode, disabling ECM should return us to client mode.
     */
    @Test
    public void testEcmDisabledReturnsToClientMode() throws Exception {
        enableWifi();
        verify(mActiveModeWarden).enterClientMode();

        reset(mActiveModeWarden);

        // Test with WifiDisableInECBM turned on:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);

        // test ecm changed
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();

        verify(mActiveModeWarden).shutdownWifi();

        // test ecm changed
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 0);
        mLooper.dispatchAll();

        verify(mActiveModeWarden).enterClientMode();
    }

    /**
     * When Ecm mode is enabled, we should shut down wifi when we get an emergency mode changed
     * update.
     */
    @Test
    public void testEcmOnFromScanMode() throws Exception {
        when(mSettingsStore.isScanAlwaysAvailable()).thenReturn(true);
        mWifiController.sendMessage(CMD_SCAN_ALWAYS_MODE_CHANGED);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).enterScanOnlyMode();

        // Test with WifiDisableInECBM turned on:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);
        verify(mActiveModeWarden).enterScanOnlyMode();

        // test ecm changed
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();

        verify(mActiveModeWarden).shutdownWifi();
    }

    /**
     * When Ecm mode is disabled, we should not shut down scan mode if we get an emergency mode
     * changed update, but we should turn off soft AP
     */
    @Test
    public void testEcmOffInScanMode() throws Exception {
        when(mSettingsStore.isScanAlwaysAvailable()).thenReturn(true);
        mWifiController.sendMessage(CMD_SCAN_ALWAYS_MODE_CHANGED);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).enterScanOnlyMode();

        // Test with WifiDisableInECBM turned off:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(false);
        verify(mActiveModeWarden).enterScanOnlyMode();

        // test ecm changed
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();

        verify(mActiveModeWarden, never()).shutdownWifi();
        verify(mActiveModeWarden).stopSoftAPMode(WifiManager.IFACE_IP_MODE_UNSPECIFIED);
    }

    /**
     * When ECM is disabled, we should return to scan mode
     */
    @Test
    public void testEcmDisabledReturnsToScanMode() throws Exception {
        when(mSettingsStore.isScanAlwaysAvailable()).thenReturn(true);
        mWifiController.sendMessage(CMD_SCAN_ALWAYS_MODE_CHANGED);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).enterScanOnlyMode();

        reset(mActiveModeWarden);

        // Test with WifiDisableInECBM turned on:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);

        // test ecm changed
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();

        verify(mActiveModeWarden).shutdownWifi();

        // test ecm changed
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 0);
        mLooper.dispatchAll();

        verify(mActiveModeWarden).enterScanOnlyMode();
    }

    /**
     * When Ecm mode is enabled, we should shut down wifi when we get an emergency mode changed
     * update.
     */
    @Test
    public void testEcmOnFromSoftApMode() throws Exception {
        mWifiController.obtainMessage(CMD_SET_AP, 1, 0).sendToTarget();
        mLooper.dispatchAll();

        // Test with WifiDisableInECBM turned on:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);
        verify(mActiveModeWarden).enterSoftAPMode(any());

        // test ecm changed
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();

        verify(mActiveModeWarden).shutdownWifi();
    }

    /**
     * When Ecm mode is disabled, we should shut down softap mode if we get an emergency mode
     * changed update
     */
    @Test
    public void testEcmOffInSoftApMode() throws Exception {
        mWifiController.obtainMessage(CMD_SET_AP, 1, 0).sendToTarget();
        mLooper.dispatchAll();
        verify(mActiveModeWarden).enterSoftAPMode(any());

        // Test with WifiDisableInECBM turned off:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(false);

        // test ecm changed
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();

        verify(mActiveModeWarden).stopSoftAPMode(WifiManager.IFACE_IP_MODE_UNSPECIFIED);
    }

    /**
     * When ECM is activated and we were in softap mode, we should just return to wifi off when ECM
     * ends
     */
    @Test
    public void testEcmDisabledRemainsDisabledWhenSoftApHadBeenOn() throws Exception {
        verify(mActiveModeWarden).disableWifi();

        mWifiController.obtainMessage(CMD_SET_AP, 1, 0).sendToTarget();
        mLooper.dispatchAll();
        verify(mActiveModeWarden).enterSoftAPMode(any());

        // Test with WifiDisableInECBM turned on:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);

        // test ecm changed
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).shutdownWifi();

        reset(mActiveModeWarden);

        // test ecm changed
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 0);
        mLooper.dispatchAll();

        verify(mActiveModeWarden).disableWifi();
        // no additional calls to enable softap
        verify(mActiveModeWarden, never()).enterSoftAPMode(any());
    }

    /**
     * Wifi should remain off when already disabled and we enter ECM.
     */
    @Test
    public void testEcmOnFromDisabledMode() throws Exception {
        verify(mActiveModeWarden, never()).enterSoftAPMode(any());
        verify(mActiveModeWarden, never()).enterClientMode();
        verify(mActiveModeWarden, never()).enterScanOnlyMode();

        // Test with WifiDisableInECBM turned on:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);

        // test ecm changed
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();
        assertEquals("EcmState", getCurrentState().getName());

        verify(mActiveModeWarden).shutdownWifi();
    }


    /**
     * Updates about call state change also trigger entry of ECM mode.
     */
    @Test
    public void testEnterEcmOnEmergencyCallStateChange() throws Exception {
        verify(mActiveModeWarden).disableWifi();

        enableWifi();
        verify(mActiveModeWarden).enterClientMode();

        reset(mActiveModeWarden);

        // Test with WifiDisableInECBM turned on:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);

        // test call state changed
        mWifiController.sendMessage(CMD_EMERGENCY_CALL_STATE_CHANGED, 1);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).shutdownWifi();

        mWifiController.sendMessage(CMD_EMERGENCY_CALL_STATE_CHANGED, 0);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).enterClientMode();
    }

    /**
     * Updates about call state change with an invalid state do not change modes.
     */
    @Test
    public void testEnterEcmOnEmergencyCallStateChangeAndUpdateWithInvalidState() throws Exception {
        verify(mActiveModeWarden).disableWifi();

        enableWifi();
        verify(mActiveModeWarden).enterClientMode();

        reset(mActiveModeWarden);

        // Test with WifiDisableInECBM turned on:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);

        // test call state changed
        mWifiController.sendMessage(CMD_EMERGENCY_CALL_STATE_CHANGED, 1);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).shutdownWifi();

        reset(mActiveModeWarden);
        mWifiController.sendMessage(CMD_EMERGENCY_CALL_STATE_CHANGED, 2);
        mLooper.dispatchAll();
        verifyNoMoreInteractions(mActiveModeWarden);
    }

    /**
     * Updates about emergency mode change with an invalid state do not change modes.
     */
    @Test
    public void testEnterEcmOnEmergencyModeChangeAndUpdateWithInvalidState() throws Exception {
        verify(mActiveModeWarden).disableWifi();

        enableWifi();
        verify(mActiveModeWarden).enterClientMode();

        reset(mActiveModeWarden);

        // Test with WifiDisableInECBM turned on:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);

        // test call state changed
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).shutdownWifi();

        reset(mActiveModeWarden);
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 2);
        mLooper.dispatchAll();
        verifyNoMoreInteractions(mActiveModeWarden);
    }

    /**
     * Verify when both ECM and call state changes arrive, we enter ECM mode
     */
    @Test
    public void testEnterEcmWithBothSignals() throws Exception {
        verify(mActiveModeWarden).disableWifi();

        enableWifi();
        verify(mActiveModeWarden).enterClientMode();

        reset(mActiveModeWarden);

        // Test with WifiDisableInECBM turned on:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);

        mWifiController.sendMessage(CMD_EMERGENCY_CALL_STATE_CHANGED, 1);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).shutdownWifi();

        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();
        // still only 1 shutdown
        verify(mActiveModeWarden).shutdownWifi();

        mWifiController.sendMessage(CMD_EMERGENCY_CALL_STATE_CHANGED, 0);
        mLooper.dispatchAll();
        // stay in ecm, do not send an additional client mode trigger
        verify(mActiveModeWarden, never()).enterClientMode();

        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 0);
        mLooper.dispatchAll();
        // now we can re-enable wifi
        verify(mActiveModeWarden).enterClientMode();
    }

    /**
     * Verify when both ECM and call state changes arrive but out of order, we enter ECM mode
     */
    @Test
    public void testEnterEcmWithBothSignalsOutOfOrder() throws Exception {
        verify(mActiveModeWarden).disableWifi();

        enableWifi();
        verify(mActiveModeWarden).enterClientMode();

        reset(mActiveModeWarden);

        // Test with WifiDisableInECBM turned on:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);

        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).shutdownWifi();

        mWifiController.sendMessage(CMD_EMERGENCY_CALL_STATE_CHANGED, 1);
        mLooper.dispatchAll();
        // still only 1 shutdown
        verify(mActiveModeWarden).shutdownWifi();

        mWifiController.sendMessage(CMD_EMERGENCY_CALL_STATE_CHANGED, 0);
        mLooper.dispatchAll();
        // stay in ecm, do not send an additional client mode trigger
        verify(mActiveModeWarden, never()).enterClientMode();

        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 0);
        mLooper.dispatchAll();
        // now we can re-enable wifi
        verify(mActiveModeWarden).enterClientMode();
    }

    /**
     * Verify when both ECM and call state changes arrive but completely out of order,
     * we still enter and properly exit ECM mode
     */
    @Test
    public void testEnterEcmWithBothSignalsOppositeOrder() throws Exception {
        verify(mActiveModeWarden).disableWifi();

        enableWifi();
        verify(mActiveModeWarden).enterClientMode();

        reset(mActiveModeWarden);

        // Test with WifiDisableInECBM turned on:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);

        mWifiController.sendMessage(CMD_EMERGENCY_CALL_STATE_CHANGED, 1);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).shutdownWifi();

        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();
        // still only 1 shutdown
        verify(mActiveModeWarden).shutdownWifi();

        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 0);
        mLooper.dispatchAll();
        // stay in ecm, do not send an additional client mode trigger
        verify(mActiveModeWarden, never()).enterClientMode();

        mWifiController.sendMessage(CMD_EMERGENCY_CALL_STATE_CHANGED, 0);
        mLooper.dispatchAll();
        // now we can re-enable wifi
        verify(mActiveModeWarden).enterClientMode();
    }


    /**
     * When ECM is active, we might get addition signals of ECM mode, we must not exit until they
     * are all cleared.
     */
    @Test
    public void testProperExitFromEcmModeWithMultipleMessages() throws Exception {
        verify(mActiveModeWarden).disableWifi();

        enableWifi();
        verify(mActiveModeWarden).enterClientMode();

        reset(mActiveModeWarden);

        // Test with WifiDisableInECBM turned on:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);

        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mWifiController.sendMessage(CMD_EMERGENCY_CALL_STATE_CHANGED, 1);
        mWifiController.sendMessage(CMD_EMERGENCY_CALL_STATE_CHANGED, 1);
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).shutdownWifi();

        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 0);
        mLooper.dispatchAll();
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 0);
        mLooper.dispatchAll();
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 0);
        mLooper.dispatchAll();
        mWifiController.sendMessage(CMD_EMERGENCY_CALL_STATE_CHANGED, 0);
        mLooper.dispatchAll();
        verify(mActiveModeWarden, never()).enterClientMode();

        // now we will exit ECM
        mWifiController.sendMessage(CMD_EMERGENCY_CALL_STATE_CHANGED, 0);
        mLooper.dispatchAll();

        // now we can re-enable wifi
        verify(mActiveModeWarden).enterClientMode();
    }

    /**
     * Toggling wifi when in ECM does not exit ecm mode and enable wifi
     */
    @Test
    public void testWifiDoesNotToggleOnWhenInEcm() throws Exception {

        // Test with WifiDisableInECBM turned on:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);

        // test ecm changed
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();
        assertEquals("EcmState", getCurrentState().getName());

        verify(mActiveModeWarden).shutdownWifi();

        // now toggle wifi and verify we do not start wifi
        when(mSettingsStore.isWifiToggleEnabled()).thenReturn(true);
        mWifiController.sendMessage(CMD_WIFI_TOGGLED);
        mLooper.dispatchAll();

        verify(mActiveModeWarden, never()).enterClientMode();
    }

    /**
     * Toggling scan mode when in ECM does not exit ecm mode and enable scan mode
     */
    @Test
    public void testScanModeDoesNotToggleOnWhenInEcm() throws Exception {

        // Test with WifiDisableInECBM turned on:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);

        // test ecm changed
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();
        assertEquals("EcmState", getCurrentState().getName());

        verify(mActiveModeWarden).shutdownWifi();

        // now enable scanning and verify we do not start wifi
        when(mSettingsStore.isWifiToggleEnabled()).thenReturn(true);
        when(mSettingsStore.isScanAlwaysAvailable()).thenReturn(true);

        mWifiController.sendMessage(CMD_SCAN_ALWAYS_MODE_CHANGED);
        mLooper.dispatchAll();

        verify(mActiveModeWarden, never()).enterScanOnlyMode();
    }

    /**
     * Toggling softap mode when in ECM does not exit ecm mode and enable softap
     */
    @Test
    public void testSoftApModeDoesNotToggleOnWhenInEcm() throws Exception {

        // Test with WifiDisableInECBM turned on:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);

        // test ecm changed
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();
        assertEquals("EcmState", getCurrentState().getName());

        verify(mActiveModeWarden).shutdownWifi();

        mWifiController.sendMessage(CMD_SET_AP);
        mLooper.dispatchAll();

        verify(mActiveModeWarden, never()).enterSoftAPMode(any());
    }

    /**
     * Toggling off softap mode when in ECM does not induce a mode change
     */
    @Test
    public void testSoftApStoppedDoesNotSwitchModesWhenInEcm() throws Exception {

        // Test with WifiDisableInECBM turned on:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);

        // test ecm changed
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();
        assertEquals("EcmState", getCurrentState().getName());

        verify(mActiveModeWarden).shutdownWifi();

        reset(mActiveModeWarden);
        mWifiController.sendMessage(CMD_AP_STOPPED);
        mLooper.dispatchAll();

        verifyNoMoreInteractions(mActiveModeWarden);
    }

    /**
     * Toggling softap mode when in airplane mode needs to enable softap
     */
    @Test
    public void testSoftApModeToggleWhenInAirplaneMode() throws Exception {
        // Test with airplane mode turned on:
        when(mSettingsStore.isAirplaneModeOn()).thenReturn(true);

        // Turn on SoftAp.
        mWifiController.sendMessage(CMD_SET_AP, 1);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).enterSoftAPMode(any());

        // Turn off SoftAp.
        mWifiController.sendMessage(CMD_SET_AP, 0, WifiManager.IFACE_IP_MODE_UNSPECIFIED);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).stopSoftAPMode(WifiManager.IFACE_IP_MODE_UNSPECIFIED);
    }

    /**
     * Toggling off scan mode when in ECM does not induce a mode change
     */
    @Test
    public void testScanModeStoppedDoesNotSwitchModesWhenInEcm() throws Exception {

        // Test with WifiDisableInECBM turned on:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);

        // test ecm changed
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();
        assertEquals("EcmState", getCurrentState().getName());

        verify(mActiveModeWarden).shutdownWifi();

        reset(mActiveModeWarden);
        mWifiController.sendMessage(CMD_SCANNING_STOPPED);
        mLooper.dispatchAll();

        verifyNoMoreInteractions(mActiveModeWarden);
    }

    /**
     * Toggling off client mode when in ECM does not induce a mode change
     */
    @Test
    public void testClientModeStoppedDoesNotSwitchModesWhenInEcm() throws Exception {

        // Test with WifiDisableInECBM turned on:
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);

        // test ecm changed
        mWifiController.sendMessage(CMD_EMERGENCY_MODE_CHANGED, 1);
        mLooper.dispatchAll();
        assertEquals("EcmState", getCurrentState().getName());

        verify(mActiveModeWarden).shutdownWifi();

        reset(mActiveModeWarden);
        mWifiController.sendMessage(CMD_STA_STOPPED);
        mLooper.dispatchAll();

        verifyNoMoreInteractions(mActiveModeWarden);
    }


    /**
     * When AP mode is enabled and wifi was previously in AP mode, we should return to
     * StaEnabledState after the AP is disabled.
     * Enter StaEnabledState, activate AP mode, disable AP mode.
     * <p>
     * Expected: AP should successfully start and exit, then return to StaEnabledState.
     */
    @Test
    public void testReturnToStaEnabledStateAfterAPModeShutdown() throws Exception {
        enableWifi();
        assertEquals("StaEnabledState", getCurrentState().getName());

        mWifiController.obtainMessage(CMD_SET_AP, 1, 0).sendToTarget();
        // add an "unexpected" sta mode stop to simulate a single interface device
        mClientModeCallback.onStateChanged(WifiManager.WIFI_STATE_DISABLED);
        mLooper.dispatchAll();

        when(mSettingsStore.getWifiSavedState()).thenReturn(1);
        mWifiController.obtainMessage(CMD_AP_STOPPED).sendToTarget();
        mLooper.dispatchAll();

        InOrder inOrder = inOrder(mActiveModeWarden);
        inOrder.verify(mActiveModeWarden).enterClientMode();
        assertEquals("StaEnabledState", getCurrentState().getName());
    }

    /**
     * When AP mode is enabled and wifi is toggled on, we should transition to
     * StaEnabledState after the AP is disabled.
     * Enter StaEnabledState, activate AP mode, toggle WiFi.
     * <p>
     * Expected: AP should successfully start and exit, then return to StaEnabledState.
     */
    @Test
    public void testReturnToStaEnabledStateAfterWifiEnabledShutdown() throws Exception {
        enableWifi();
        assertEquals("StaEnabledState", getCurrentState().getName());

        mWifiController.obtainMessage(CMD_SET_AP, 1, 0).sendToTarget();
        mLooper.dispatchAll();

        when(mSettingsStore.isWifiToggleEnabled()).thenReturn(true);
        mWifiController.obtainMessage(CMD_WIFI_TOGGLED).sendToTarget();
        mWifiController.obtainMessage(CMD_AP_STOPPED).sendToTarget();
        mLooper.dispatchAll();

        InOrder inOrder = inOrder(mActiveModeWarden);
        inOrder.verify(mActiveModeWarden).enterClientMode();
        assertEquals("StaEnabledState", getCurrentState().getName());
    }

    @Test
    public void testRestartWifiStackInStaEnabledStateTriggersBugReport() throws Exception {
        enableWifi();
        mWifiController.sendMessage(CMD_RECOVERY_RESTART_WIFI,
                                    SelfRecovery.REASON_WIFINATIVE_FAILURE);
        mLooper.dispatchAll();
        verify(mClientModeImpl).takeBugReport(anyString(), anyString());
    }

    @Test
    public void testRestartWifiWatchdogDoesNotTriggerBugReport() throws Exception {
        enableWifi();
        mWifiController.sendMessage(CMD_RECOVERY_RESTART_WIFI,
                                    SelfRecovery.REASON_LAST_RESORT_WATCHDOG);
        mLooper.dispatchAll();
        verify(mClientModeImpl, never()).takeBugReport(anyString(), anyString());
    }

    /**
     * When in sta mode, CMD_RECOVERY_DISABLE_WIFI messages should trigger wifi to disable.
     */
    @Test
    public void testRecoveryDisabledTurnsWifiOff() throws Exception {
        enableWifi();
        reset(mActiveModeWarden);
        mWifiController.sendMessage(CMD_RECOVERY_DISABLE_WIFI);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).disableWifi();
    }

    /**
     * When wifi is disabled, CMD_RECOVERY_DISABLE_WIFI should not trigger a state change.
     */
    @Test
    public void testRecoveryDisabledWhenWifiAlreadyOff() throws Exception {
        assertEquals("StaDisabledState", getCurrentState().getName());
        mWifiController.sendMessage(CMD_RECOVERY_DISABLE_WIFI);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).shutdownWifi();
    }

    /**
     * The command to trigger a WiFi reset should not trigger any action by WifiController if we
     * are not in STA mode.
     * WiFi is not in connect mode, so any calls to reset the wifi stack due to connection failures
     * should be ignored.
     * Create and start WifiController in StaDisabledState, send command to restart WiFi
     * <p>
     * Expected: WiFiController should not call ActiveModeWarden.disableWifi()
     */
    @Test
    public void testRestartWifiStackInStaDisabledState() throws Exception {
        // Start a new WifiController with wifi disabled
        when(mSettingsStore.isAirplaneModeOn()).thenReturn(false);
        when(mSettingsStore.isWifiToggleEnabled()).thenReturn(false);
        when(mSettingsStore.isScanAlwaysAvailable()).thenReturn(false);

        mWifiController = new WifiController(mContext, mClientModeImpl, mLooper.getLooper(),
                mSettingsStore, mLooper.getLooper(), mFacade, mActiveModeWarden,
                mWifiPermissionsUtil);

        mWifiController.start();
        mLooper.dispatchAll();

        reset(mClientModeImpl);
        assertEquals("StaDisabledState", getCurrentState().getName());

        reset(mActiveModeWarden);
        mWifiController.sendMessage(CMD_RECOVERY_RESTART_WIFI);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).disableWifi();
    }

    /**
     * The command to trigger a WiFi reset should not trigger any action by WifiController if we
     * are not in STA mode, even if scans are allowed.
     * WiFi is not in connect mode, so any calls to reset the wifi stack due to connection failures
     * should be ignored.
     * Create and start WifiController in StaDisabledState, send command to restart WiFi
     * <p>
     * Expected: WiFiController should not call ActiveModeWarden.disableWifi() or
     * ActiveModeWarden.shutdownWifi().
     */
    @Test
    public void testRestartWifiStackInStaDisabledWithScanState() throws Exception {
        when(mSettingsStore.isScanAlwaysAvailable()).thenReturn(true);
        mWifiController.sendMessage(CMD_SCAN_ALWAYS_MODE_CHANGED);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).enterScanOnlyMode();

        reset(mActiveModeWarden);
        mWifiController.sendMessage(CMD_RECOVERY_RESTART_WIFI);
        mLooper.dispatchAll();
        mLooper.moveTimeForward(TEST_WIFI_RECOVERY_DELAY_MS);
        mLooper.dispatchAll();
        InOrder inOrder = inOrder(mActiveModeWarden);
        verify(mActiveModeWarden).disableWifi();
        verify(mActiveModeWarden).enterScanOnlyMode();
    }

    /**
     * The command to trigger a WiFi reset should trigger a wifi reset in ClientModeImpl through
     * the ActiveModeWarden.shutdownWifi() call when in STA mode.
     * WiFi is in connect mode, calls to reset the wifi stack due to connection failures
     * should trigger a supplicant stop, and subsequently, a driver reload.
     * Create and start WifiController in StaEnabledState, send command to restart WiFi
     * <p>
     * Expected: WiFiController should call ActiveModeWarden.shutdownWifi() and
     * ActiveModeWarden should enter CONNECT_MODE and the wifi driver should be started.
     */
    @Test
    public void testRestartWifiStackInStaEnabledState() throws Exception {
        enableWifi();
        assertEquals("StaEnabledState", getCurrentState().getName());

        mWifiController.sendMessage(CMD_RECOVERY_RESTART_WIFI);
        mLooper.dispatchAll();
        mLooper.moveTimeForward(TEST_WIFI_RECOVERY_DELAY_MS);
        mLooper.dispatchAll();

        InOrder inOrder = inOrder(mActiveModeWarden);
        inOrder.verify(mActiveModeWarden).shutdownWifi();
        inOrder.verify(mActiveModeWarden).enterClientMode();
        assertEquals("StaEnabledState", getCurrentState().getName());
    }

    /**
     * The command to trigger a WiFi reset should not trigger a reset when in ECM mode.
     * Enable wifi and enter ECM state, send command to restart wifi.
     * <p>
     * Expected: The command to trigger a wifi reset should be ignored and we should remain in ECM
     * mode.
     */
    @Test
    public void testRestartWifiStackDoesNotExitECMMode() throws Exception {
        enableWifi();
        assertEquals("StaEnabledState", getCurrentState().getName());
        when(mFacade.getConfigWiFiDisableInECBM(mContext)).thenReturn(true);

        mWifiController.sendMessage(CMD_EMERGENCY_CALL_STATE_CHANGED, 1);
        mLooper.dispatchAll();
        assertEquals("EcmState", getCurrentState().getName());
        verify(mActiveModeWarden).shutdownWifi();

        reset(mActiveModeWarden);
        mWifiController.sendMessage(CMD_RECOVERY_RESTART_WIFI);
        mLooper.dispatchAll();
        assertEquals("EcmState", getCurrentState().getName());

        verifyZeroInteractions(mActiveModeWarden);
    }

    /**
     * The command to trigger a WiFi reset should trigger a reset when in AP mode.
     * Enter AP mode, send command to restart wifi.
     * <p>
     * Expected: The command to trigger a wifi reset should trigger wifi shutdown.
     */
    @Test
    public void testRestartWifiStackFullyStopsWifi() throws Exception {
        mWifiController.obtainMessage(CMD_SET_AP, 1).sendToTarget();
        mLooper.dispatchAll();
        verify(mActiveModeWarden).enterSoftAPMode(any());

        reset(mActiveModeWarden);
        mWifiController.sendMessage(CMD_RECOVERY_RESTART_WIFI);
        mLooper.dispatchAll();
        verify(mActiveModeWarden).shutdownWifi();
    }
}
