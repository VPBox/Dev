/*
 * Copyright (C) 2019 The Android Open Source Project
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

import static junit.framework.Assert.assertEquals;

import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.atLeastOnce;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.when;

import android.content.Context;
import android.database.ContentObserver;
import android.net.wifi.WifiInfo;
import android.os.test.TestLooper;
import android.provider.Settings;

import androidx.test.filters.SmallTest;

import com.android.internal.R;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.util.HashSet;

/**
 * Unit tests for LinkProbeManager
 */
@SmallTest
public class LinkProbeManagerTest {

    private static final String TEST_IFACE_NAME = "testIfaceName";
    private static final String TEST_BSSID = "6c:f3:7f:ae:8c:f3";
    private static final int TEST_ELAPSED_TIME_MS = 100;

    private LinkProbeManager mLinkProbeManager;

    private WifiInfo mWifiInfo;
    private long mTimeMs;
    @Mock private Clock mClock;
    @Mock private WifiNative mWifiNative;
    @Mock private WifiMetrics mWifiMetrics;
    @Mock private FrameworkFacade mFrameworkFacade;
    @Mock private Context mContext;

    private TestLooper mLooper = new TestLooper();
    private ContentObserver mContentObserver;
    private MockResources mResources;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);

        mResources = new MockResources();
        mResources.setBoolean(R.bool.config_wifi_link_probing_supported, true);
        when(mContext.getResources()).thenReturn(mResources);

        initLinkProbeManager();

        mWifiInfo = new WifiInfo();
        mWifiInfo.setBSSID(TEST_BSSID);
        mTimeMs = 1000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);

        ArgumentCaptor<ContentObserver> observerCaptor = ArgumentCaptor.forClass(
                ContentObserver.class);
        verify(mFrameworkFacade).registerContentObserver(eq(mContext),
                eq(Settings.Global.getUriFor(Settings.Global.WIFI_LINK_PROBING_ENABLED)),
                eq(false), observerCaptor.capture());
        mContentObserver = observerCaptor.getValue();
        when(mFrameworkFacade.getIntegerSetting(eq(mContext),
                eq(Settings.Global.WIFI_LINK_PROBING_ENABLED), anyInt())).thenReturn(1);
        mContentObserver.onChange(false);
    }

    private void initLinkProbeManager() {
        mLinkProbeManager = new LinkProbeManager(mClock, mWifiNative, mWifiMetrics,
                mFrameworkFacade, mLooper.getLooper(), mContext);
    }

    /**
     * Tests that link probing is correctly triggered when the required conditions are met, and an
     * ACK was received from the AP.
     */
    @Test
    public void testLinkProbeTriggeredAndAcked() throws Exception {
        // initialize tx success counter
        mWifiInfo.txSuccess = 50;
        mTimeMs += 3000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
        mLinkProbeManager.updateConnectionStats(mWifiInfo, TEST_IFACE_NAME);
        // should not probe yet
        verify(mWifiNative, never()).probeLink(any(), any(), any(), anyInt());
        verify(mWifiMetrics, never()).incrementLinkProbeExperimentProbeCount(any());

        // tx success counter did not change since last update
        mWifiInfo.txSuccess = 50;
        // below RSSI threshold
        int rssi = LinkProbeManager.RSSI_THRESHOLD - 5;
        mWifiInfo.setRssi(rssi);
        // above link speed threshold
        int linkSpeed = LinkProbeManager.LINK_SPEED_THRESHOLD_MBPS + 10;
        mWifiInfo.setLinkSpeed(linkSpeed);
        // more than DELAY_AFTER_TX_SUCCESS_MS passed
        long timeDelta = LinkProbeManager.DELAY_AFTER_TX_SUCCESS_MS + 20000;
        mTimeMs += timeDelta;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
        mLinkProbeManager.updateConnectionStats(mWifiInfo, TEST_IFACE_NAME);
        ArgumentCaptor<WifiNative.SendMgmtFrameCallback> callbackCaptor =
                ArgumentCaptor.forClass(WifiNative.SendMgmtFrameCallback.class);
        verify(mWifiNative).probeLink(eq(TEST_IFACE_NAME), any(), callbackCaptor.capture(),
                anyInt());
        ArgumentCaptor<String> experimentIdCaptor = ArgumentCaptor.forClass(String.class);
        verify(mWifiMetrics, atLeastOnce()).incrementLinkProbeExperimentProbeCount(
                experimentIdCaptor.capture());
        int numExperimentIds = LinkProbeManager.EXPERIMENT_DELAYS_MS.length
                * LinkProbeManager.EXPERIMENT_RSSIS.length
                * LinkProbeManager.EXPERIMENT_LINK_SPEEDS.length;
        assertEquals(numExperimentIds, new HashSet<>(experimentIdCaptor.getAllValues()).size());

        callbackCaptor.getValue().onAck(TEST_ELAPSED_TIME_MS);
        verify(mWifiMetrics).logLinkProbeSuccess(timeDelta, rssi, linkSpeed, TEST_ELAPSED_TIME_MS);
    }

    /**
     * Tests that link probing is correctly triggered when the required conditions are met, but
     * no ACK was received from the AP.
     */
    @Test
    public void testLinkProbeTriggeredAndFailed() throws Exception {
        // initialize tx success counter
        mWifiInfo.txSuccess = 50;
        mTimeMs += 3000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
        mLinkProbeManager.updateConnectionStats(mWifiInfo, TEST_IFACE_NAME);
        // should not probe yet
        verify(mWifiNative, never()).probeLink(any(), any(), any(), anyInt());

        // tx success counter did not change since last update
        mWifiInfo.txSuccess = 50;
        // above RSSI threshold
        int rssi = LinkProbeManager.RSSI_THRESHOLD + 5;
        mWifiInfo.setRssi(rssi);
        // below link speed threshold
        int linkSpeed = LinkProbeManager.LINK_SPEED_THRESHOLD_MBPS - 2;
        mWifiInfo.setLinkSpeed(linkSpeed);
        // more than DELAY_BETWEEN_PROBES_MS passed
        long timeDelta = LinkProbeManager.DELAY_BETWEEN_PROBES_MS + 1000;
        mTimeMs += timeDelta;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
        mLinkProbeManager.updateConnectionStats(mWifiInfo, TEST_IFACE_NAME);
        ArgumentCaptor<WifiNative.SendMgmtFrameCallback> callbackCaptor =
                ArgumentCaptor.forClass(WifiNative.SendMgmtFrameCallback.class);
        verify(mWifiNative).probeLink(eq(TEST_IFACE_NAME), any(), callbackCaptor.capture(),
                anyInt());

        callbackCaptor.getValue().onFailure(WifiNative.SEND_MGMT_FRAME_ERROR_NO_ACK);
        verify(mWifiMetrics).logLinkProbeFailure(timeDelta, rssi, linkSpeed,
                WifiNative.SEND_MGMT_FRAME_ERROR_NO_ACK);
    }

    /**
     * Tests that link probing is not triggered more than once every DELAY_BETWEEN_PROBES_MS
     */
    @Test
    public void testLinkProbeNotTriggeredTooFrequently() throws Exception {
        testLinkProbeTriggeredAndAcked();

        // tx success counter did not change since last update
        mWifiInfo.txSuccess = 50;
        // below RSSI threshold
        mWifiInfo.setRssi(LinkProbeManager.RSSI_THRESHOLD - 5);
        // above link speed threshold
        mWifiInfo.setLinkSpeed(LinkProbeManager.LINK_SPEED_THRESHOLD_MBPS + 10);
        // *** but less than DELAY_BETWEEN_PROBES_MS has passed since last probe ***
        mTimeMs += LinkProbeManager.DELAY_BETWEEN_PROBES_MS - 1000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
        mLinkProbeManager.updateConnectionStats(mWifiInfo, TEST_IFACE_NAME);
        // should not probe
        verifyNoMoreInteractions(mWifiNative);
    }

    /**
     * Tests that link probing is not triggered when Tx has succeeded within the last
     * DELAY_AFTER_TX_SUCCESS_MS.
     */
    @Test
    public void testLinkProbeNotTriggeredWhenTxSucceeded() throws Exception {
        // initialize tx success counter
        mWifiInfo.txSuccess = 50;
        mTimeMs += 3000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
        mLinkProbeManager.updateConnectionStats(mWifiInfo, TEST_IFACE_NAME);
        // should not probe yet
        verify(mWifiNative, never()).probeLink(any(), any(), any(), anyInt());

        // tx succeeded
        mWifiInfo.txSuccess = 55;
        mTimeMs += 3000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
        mLinkProbeManager.updateConnectionStats(mWifiInfo, TEST_IFACE_NAME);
        // should not probe yet
        verify(mWifiNative, never()).probeLink(any(), any(), any(), anyInt());

        // tx success counter did not change since last update
        mWifiInfo.txSuccess = 55;
        // below RSSI threshold
        mWifiInfo.setRssi(LinkProbeManager.RSSI_THRESHOLD - 5);
        // above link speed threshold
        mWifiInfo.setLinkSpeed(LinkProbeManager.LINK_SPEED_THRESHOLD_MBPS + 10);
        // *** but less than DELAY_AFTER_TX_SUCCESS_MS has passed since last tx success ***
        mTimeMs += LinkProbeManager.DELAY_AFTER_TX_SUCCESS_MS - 1000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
        mLinkProbeManager.updateConnectionStats(mWifiInfo, TEST_IFACE_NAME);
        verify(mWifiNative, never()).probeLink(any(), any(), any(), anyInt());
    }

    /**
     * Tests that link probing is not triggered when screen was turned on within the last
     * {@link LinkProbeManager#SCREEN_ON_DELAY_MS}.
     */
    @Test
    public void testLinkProbeNotTriggeredWhenScreenJustTurnedOn() throws Exception {
        mTimeMs += 30 * 1000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
        mLinkProbeManager.resetOnScreenTurnedOn();
        // should not probe yet
        verify(mWifiNative, never()).probeLink(any(), any(), any(), anyInt());

        // tx success counter did not change since initialization
        mWifiInfo.txSuccess = 0;
        // below RSSI threshold
        mWifiInfo.setRssi(LinkProbeManager.RSSI_THRESHOLD - 5);
        // above link speed threshold
        mWifiInfo.setLinkSpeed(LinkProbeManager.LINK_SPEED_THRESHOLD_MBPS + 10);
        // *** but less than SCREEN_ON_DELAY_MS has passed since last screen on ***
        mTimeMs += LinkProbeManager.SCREEN_ON_DELAY_MS - 1000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
        mLinkProbeManager.updateConnectionStats(mWifiInfo, TEST_IFACE_NAME);
        verify(mWifiNative, never()).probeLink(any(), any(), any(), anyInt());
    }

    /**
     * Tests that link probing is triggered when screen was turned on more than
     * {@link LinkProbeManager#SCREEN_ON_DELAY_MS} ago.
     */
    @Test
    public void testLinkProbeTriggeredAfterScreenTurnedOn() throws Exception {
        mTimeMs += 30 * 1000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
        mLinkProbeManager.resetOnScreenTurnedOn();
        // should not probe yet
        verify(mWifiNative, never()).probeLink(any(), any(), any(), anyInt());

        // tx success counter did not change since initialization
        mWifiInfo.txSuccess = 0;
        // below RSSI threshold
        mWifiInfo.setRssi(LinkProbeManager.RSSI_THRESHOLD - 5);
        // above link speed threshold
        mWifiInfo.setLinkSpeed(LinkProbeManager.LINK_SPEED_THRESHOLD_MBPS + 10);
        // *** more than SCREEN_ON_DELAY_MS has passed since last screen on ***
        mTimeMs += LinkProbeManager.SCREEN_ON_DELAY_MS + 1000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
        mLinkProbeManager.updateConnectionStats(mWifiInfo, TEST_IFACE_NAME);
        verify(mWifiNative).probeLink(eq(TEST_IFACE_NAME), any(), any(), anyInt());
    }

    /**
     * Tests when link probing feature flag is disabled, no probes should run.
     */
    @Test
    public void testLinkProbeFeatureDisabled() throws Exception {
        when(mFrameworkFacade.getIntegerSetting(eq(mContext),
                eq(Settings.Global.WIFI_LINK_PROBING_ENABLED), anyInt())).thenReturn(0);
        mContentObserver.onChange(false);

        // initialize tx success counter
        mWifiInfo.txSuccess = 50;
        mTimeMs += 3000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
        mLinkProbeManager.updateConnectionStats(mWifiInfo, TEST_IFACE_NAME);
        // should not probe yet
        verify(mWifiNative, never()).probeLink(any(), any(), any(), anyInt());

        // tx success counter did not change since last update
        mWifiInfo.txSuccess = 50;
        // below RSSI threshold
        int rssi = LinkProbeManager.RSSI_THRESHOLD - 5;
        mWifiInfo.setRssi(rssi);
        // above link speed threshold
        int linkSpeed = LinkProbeManager.LINK_SPEED_THRESHOLD_MBPS + 10;
        mWifiInfo.setLinkSpeed(linkSpeed);
        // more than DELAY_AFTER_TX_SUCCESS_MS passed
        long timeDelta = LinkProbeManager.DELAY_AFTER_TX_SUCCESS_MS + 1000;
        mTimeMs += timeDelta;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
        mLinkProbeManager.updateConnectionStats(mWifiInfo, TEST_IFACE_NAME);
        verify(mWifiNative, never()).probeLink(any() , any(), any(), anyInt());
    }

    /**
     * Tests when link probing feature is not supported by the device, no probes should run.
     */
    @Test
    public void testLinkProbeFeatureUnsupported() throws Exception {
        mResources.setBoolean(R.bool.config_wifi_link_probing_supported, false);
        initLinkProbeManager();

        // initialize tx success counter
        mWifiInfo.txSuccess = 50;
        mTimeMs += 3000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
        mLinkProbeManager.updateConnectionStats(mWifiInfo, TEST_IFACE_NAME);
        // should not probe yet
        verify(mWifiNative, never()).probeLink(any(), any(), any(), anyInt());

        // tx success counter did not change since last update
        mWifiInfo.txSuccess = 50;
        // below RSSI threshold
        int rssi = LinkProbeManager.RSSI_THRESHOLD - 5;
        mWifiInfo.setRssi(rssi);
        // above link speed threshold
        int linkSpeed = LinkProbeManager.LINK_SPEED_THRESHOLD_MBPS + 10;
        mWifiInfo.setLinkSpeed(linkSpeed);
        // more than DELAY_AFTER_TX_SUCCESS_MS passed
        long timeDelta = LinkProbeManager.DELAY_AFTER_TX_SUCCESS_MS + 1000;
        mTimeMs += timeDelta;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
        mLinkProbeManager.updateConnectionStats(mWifiInfo, TEST_IFACE_NAME);
        verify(mWifiNative, never()).probeLink(any() , any(), any(), anyInt());
    }

    /**
     * Tests exhausting the daily link probe quota and verify that no more link probes are made
     * after the limit is reached. Tests that the quota is reset upon entering a new day.
     */
    @Test
    public void testLinkProbeQuotaExceeded() {
        mTimeMs += 30 * 1000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
        // should not probe yet
        verify(mWifiNative, never()).probeLink(any(), any(), any(), anyInt());

        // tx success counter did not change since initialization
        mWifiInfo.txSuccess = 0;
        // below RSSI threshold
        mWifiInfo.setRssi(LinkProbeManager.RSSI_THRESHOLD - 5);
        // above link speed threshold
        mWifiInfo.setLinkSpeed(LinkProbeManager.LINK_SPEED_THRESHOLD_MBPS + 10);

        // exhaust quota
        for (int i = 1; i <= LinkProbeManager.MAX_PROBE_COUNT_IN_PERIOD; i++) {
            mTimeMs += LinkProbeManager.DELAY_BETWEEN_PROBES_MS + 1000;
            when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
            mLinkProbeManager.updateConnectionStats(mWifiInfo, TEST_IFACE_NAME);
        }
        verify(mWifiNative, times((int) LinkProbeManager.MAX_PROBE_COUNT_IN_PERIOD))
                .probeLink(eq(TEST_IFACE_NAME), any(), any(), anyInt());
        // verify no more quota
        for (int i = 0; i < 10; i++) {
            mTimeMs += LinkProbeManager.DELAY_BETWEEN_PROBES_MS + 1000;
            when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
            mLinkProbeManager.updateConnectionStats(mWifiInfo, TEST_IFACE_NAME);
        }
        verify(mWifiNative, times((int) LinkProbeManager.MAX_PROBE_COUNT_IN_PERIOD))
                .probeLink(eq(TEST_IFACE_NAME), any(), any(), anyInt());

        // start new period
        mTimeMs += LinkProbeManager.PERIOD_MILLIS + 1000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);

        // exhaust quota again
        for (int i = 1; i <= LinkProbeManager.MAX_PROBE_COUNT_IN_PERIOD; i++) {
            mTimeMs += LinkProbeManager.DELAY_BETWEEN_PROBES_MS + 1000;
            when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
            mLinkProbeManager.updateConnectionStats(mWifiInfo, TEST_IFACE_NAME);
        }
        verify(mWifiNative, times((int) (2 * LinkProbeManager.MAX_PROBE_COUNT_IN_PERIOD)))
                .probeLink(eq(TEST_IFACE_NAME), any(), any(), anyInt());
        // verify no more quota again
        for (int i = 0; i < 10; i++) {
            mTimeMs += LinkProbeManager.DELAY_BETWEEN_PROBES_MS + 1000;
            when(mClock.getElapsedSinceBootMillis()).thenReturn(mTimeMs);
            mLinkProbeManager.updateConnectionStats(mWifiInfo, TEST_IFACE_NAME);
        }
        verify(mWifiNative, times((int) (2 * LinkProbeManager.MAX_PROBE_COUNT_IN_PERIOD)))
                .probeLink(eq(TEST_IFACE_NAME), any(), any(), anyInt());
    }
}
