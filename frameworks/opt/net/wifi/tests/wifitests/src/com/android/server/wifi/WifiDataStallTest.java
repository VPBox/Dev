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
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.anyLong;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.content.Context;
import android.net.wifi.WifiInfo;
import android.os.Looper;
import android.provider.DeviceConfig.OnPropertiesChangedListener;
import android.provider.Settings;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.nano.WifiMetricsProto.WifiIsUnusableEvent;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

/**
 * Unit tests for {@link com.android.server.wifi.WifiDataStall}.
 */
@SmallTest
public class WifiDataStallTest {

    @Mock Context mContext;
    @Mock FrameworkFacade mFacade;
    @Mock WifiMetrics mWifiMetrics;
    WifiDataStall mWifiDataStall;
    @Mock Clock mClock;
    @Mock DeviceConfigFacade mDeviceConfigFacade;
    @Mock Looper mClientModeImplLooper;
    @Mock WifiInfo mWifiInfo;

    private final WifiLinkLayerStats mOldLlStats = new WifiLinkLayerStats();
    private final WifiLinkLayerStats mNewLlStats = new WifiLinkLayerStats();
    final ArgumentCaptor<OnPropertiesChangedListener> mOnPropertiesChangedListenerCaptor =
            ArgumentCaptor.forClass(OnPropertiesChangedListener.class);

    /**
     * Sets up for unit test
     */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        when(mFacade.getIntegerSetting(mContext,
                Settings.Global.WIFI_DATA_STALL_MIN_TX_BAD,
                WifiDataStall.MIN_TX_BAD_DEFAULT))
                .thenReturn(WifiDataStall.MIN_TX_BAD_DEFAULT);
        when(mFacade.getIntegerSetting(mContext,
                Settings.Global.WIFI_DATA_STALL_MIN_TX_SUCCESS_WITHOUT_RX,
                WifiDataStall.MIN_TX_SUCCESS_WITHOUT_RX_DEFAULT))
                .thenReturn(WifiDataStall.MIN_TX_SUCCESS_WITHOUT_RX_DEFAULT);
        when(mDeviceConfigFacade.getDataStallDurationMs()).thenReturn(
                DeviceConfigFacade.DEFAULT_DATA_STALL_DURATION_MS);
        when(mDeviceConfigFacade.getDataStallTxTputThrMbps()).thenReturn(
                DeviceConfigFacade.DEFAULT_DATA_STALL_TX_TPUT_THR_MBPS);
        when(mDeviceConfigFacade.getDataStallRxTputThrMbps()).thenReturn(
                DeviceConfigFacade.DEFAULT_DATA_STALL_RX_TPUT_THR_MBPS);
        when(mDeviceConfigFacade.getDataStallTxPerThr()).thenReturn(
                DeviceConfigFacade.DEFAULT_DATA_STALL_TX_PER_THR);
        when(mDeviceConfigFacade.getDataStallCcaLevelThr()).thenReturn(
                DeviceConfigFacade.DEFAULT_DATA_STALL_CCA_LEVEL_THR);
        when(mWifiInfo.getLinkSpeed()).thenReturn(100);
        when(mWifiInfo.getRxLinkSpeedMbps()).thenReturn(100);
        when(mWifiInfo.getFrequency()).thenReturn(5850);
        when(mWifiInfo.getBSSID()).thenReturn("5G_WiFi");

        mWifiDataStall = new WifiDataStall(mContext, mFacade, mWifiMetrics, mDeviceConfigFacade,
                mClientModeImplLooper, mClock);

        mOldLlStats.txmpdu_be = 1000;
        mOldLlStats.retries_be = 1000;
        mOldLlStats.lostmpdu_be = 3000;
        mOldLlStats.rxmpdu_be = 4000;
        mOldLlStats.timeStampInMs = 10000;

        mNewLlStats.txmpdu_be = 2 * mOldLlStats.txmpdu_be;
        mNewLlStats.retries_be = 10 * mOldLlStats.retries_be;
        mNewLlStats.lostmpdu_be = mOldLlStats.lostmpdu_be;
        mNewLlStats.rxmpdu_be = mOldLlStats.rxmpdu_be;
        mNewLlStats.timeStampInMs = mOldLlStats.timeStampInMs
                + WifiDataStall.MAX_MS_DELTA_FOR_DATA_STALL - 1;
        verify(mDeviceConfigFacade).addOnPropertiesChangedListener(any(),
                mOnPropertiesChangedListenerCaptor.capture());
    }

    /**
     * Verify that LinkLayerStats for WifiIsUnusableEvent is correctly updated
     */
    private void verifyUpdateWifiIsUnusableLinkLayerStats() {
        verify(mWifiMetrics).updateWifiIsUnusableLinkLayerStats(
                mNewLlStats.txmpdu_be - mOldLlStats.txmpdu_be,
                mNewLlStats.retries_be - mOldLlStats.retries_be,
                mNewLlStats.lostmpdu_be - mOldLlStats.lostmpdu_be,
                mNewLlStats.rxmpdu_be - mOldLlStats.rxmpdu_be,
                mNewLlStats.timeStampInMs - mOldLlStats.timeStampInMs);
    }

    /**
     * Verify there is data stall from tx failures
     */
    @Test
    public void verifyDataStallTxFailure() throws Exception {
        when(mClock.getElapsedSinceBootMillis()).thenReturn(10L);

        assertEquals(WifiIsUnusableEvent.TYPE_UNKNOWN,
                mWifiDataStall.checkForDataStall(mOldLlStats, mNewLlStats, mWifiInfo));
        verifyUpdateWifiIsUnusableLinkLayerStats();
        when(mClock.getElapsedSinceBootMillis()).thenReturn(
                10L + DeviceConfigFacade.DEFAULT_DATA_STALL_DURATION_MS);
        assertEquals(WifiIsUnusableEvent.TYPE_DATA_STALL_BAD_TX,
                mWifiDataStall.checkForDataStall(mOldLlStats, mNewLlStats, mWifiInfo));
        verify(mWifiMetrics).logWifiIsUnusableEvent(WifiIsUnusableEvent.TYPE_DATA_STALL_BAD_TX);
    }

    /**
     * Verify there is no data stall from tx failures if tx failures are not consecutively bad
     */
    @Test
    public void verifyNoDataStallWhenTxFailureIsNotConsecutive() throws Exception {
        when(mClock.getElapsedSinceBootMillis()).thenReturn(10L);

        assertEquals(WifiIsUnusableEvent.TYPE_UNKNOWN,
                mWifiDataStall.checkForDataStall(mOldLlStats, mNewLlStats, mWifiInfo));
        verifyUpdateWifiIsUnusableLinkLayerStats();
        when(mClock.getElapsedSinceBootMillis()).thenReturn(
                10L + DeviceConfigFacade.DEFAULT_DATA_STALL_DURATION_MS);
        mNewLlStats.retries_be = 2 * mOldLlStats.retries_be;
        assertEquals(WifiIsUnusableEvent.TYPE_UNKNOWN,
                mWifiDataStall.checkForDataStall(mOldLlStats, mNewLlStats, mWifiInfo));
        verify(mWifiMetrics, never()).logWifiIsUnusableEvent(
                WifiIsUnusableEvent.TYPE_DATA_STALL_BAD_TX);
    }

    /**
     * Verify there is data stall from rx failures
     */
    @Test
    public void verifyDataStallRxFailure() throws Exception {
        when(mWifiInfo.getRxLinkSpeedMbps()).thenReturn(1);
        mNewLlStats.retries_be = 2 * mOldLlStats.retries_be;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(10L);

        assertEquals(WifiIsUnusableEvent.TYPE_UNKNOWN,
                mWifiDataStall.checkForDataStall(mOldLlStats, mNewLlStats, mWifiInfo));
        verifyUpdateWifiIsUnusableLinkLayerStats();
        when(mClock.getElapsedSinceBootMillis()).thenReturn(
                10L + DeviceConfigFacade.DEFAULT_DATA_STALL_DURATION_MS);
        assertEquals(WifiIsUnusableEvent.TYPE_DATA_STALL_TX_WITHOUT_RX,
                mWifiDataStall.checkForDataStall(mOldLlStats, mNewLlStats, mWifiInfo));
        verify(mWifiMetrics).logWifiIsUnusableEvent(
                WifiIsUnusableEvent.TYPE_DATA_STALL_TX_WITHOUT_RX);
    }

    /**
     * Verify there is data stall from both tx and rx failures
     */
    @Test
    public void verifyDataStallBothTxRxFailure() throws Exception {
        when(mWifiInfo.getRxLinkSpeedMbps()).thenReturn(1);
        when(mClock.getElapsedSinceBootMillis()).thenReturn(10L);

        assertEquals(WifiIsUnusableEvent.TYPE_UNKNOWN,
                mWifiDataStall.checkForDataStall(mOldLlStats, mNewLlStats, mWifiInfo));
        verifyUpdateWifiIsUnusableLinkLayerStats();
        when(mClock.getElapsedSinceBootMillis()).thenReturn(
                10L + DeviceConfigFacade.DEFAULT_DATA_STALL_DURATION_MS);
        assertEquals(WifiIsUnusableEvent.TYPE_DATA_STALL_BOTH,
                mWifiDataStall.checkForDataStall(mOldLlStats, mNewLlStats, mWifiInfo));
        verify(mWifiMetrics).logWifiIsUnusableEvent(WifiIsUnusableEvent.TYPE_DATA_STALL_BOTH);
    }

    /**
     * Verify that we can change the duration of evaluating Wifi conditions
     * to trigger data stall from DeviceConfigFacade
     */
    @Test
    public void verifyDataStallDurationDeviceConfigChange() throws Exception {
        when(mClock.getElapsedSinceBootMillis()).thenReturn(10L);
        when(mDeviceConfigFacade.getDataStallDurationMs()).thenReturn(
                DeviceConfigFacade.DEFAULT_DATA_STALL_DURATION_MS + 1);
        mOnPropertiesChangedListenerCaptor.getValue().onPropertiesChanged(null);

        assertEquals(WifiIsUnusableEvent.TYPE_UNKNOWN,
                mWifiDataStall.checkForDataStall(mOldLlStats, mNewLlStats, mWifiInfo));
        verifyUpdateWifiIsUnusableLinkLayerStats();
        when(mClock.getElapsedSinceBootMillis()).thenReturn(
                10L + DeviceConfigFacade.DEFAULT_DATA_STALL_DURATION_MS);
        assertEquals(WifiIsUnusableEvent.TYPE_UNKNOWN,
                mWifiDataStall.checkForDataStall(mOldLlStats, mNewLlStats, mWifiInfo));
        verify(mWifiMetrics, never()).logWifiIsUnusableEvent(
                WifiIsUnusableEvent.TYPE_DATA_STALL_BAD_TX);
    }

    /**
     * Verify that we can change the threshold of Tx packet error rate to trigger a data stall
     * from DeviceConfigFacade
     */
    @Test
    public void verifyDataStallTxPerThrDeviceConfigChange() throws Exception {
        when(mClock.getElapsedSinceBootMillis()).thenReturn(10L);
        when(mDeviceConfigFacade.getDataStallTxPerThr()).thenReturn(
                DeviceConfigFacade.DEFAULT_DATA_STALL_TX_PER_THR + 1);
        mOnPropertiesChangedListenerCaptor.getValue().onPropertiesChanged(null);

        assertEquals(WifiIsUnusableEvent.TYPE_UNKNOWN,
                mWifiDataStall.checkForDataStall(mOldLlStats, mNewLlStats, mWifiInfo));
        verifyUpdateWifiIsUnusableLinkLayerStats();
        when(mClock.getElapsedSinceBootMillis()).thenReturn(
                10L + DeviceConfigFacade.DEFAULT_DATA_STALL_DURATION_MS);
        assertEquals(WifiIsUnusableEvent.TYPE_UNKNOWN,
                mWifiDataStall.checkForDataStall(mOldLlStats, mNewLlStats, mWifiInfo));
        verify(mWifiMetrics, never()).logWifiIsUnusableEvent(
                WifiIsUnusableEvent.TYPE_DATA_STALL_BAD_TX);
    }

    /**
     * Verify there is no data stall when there are no new tx/rx failures
     */
    @Test
    public void verifyNoDataStallWhenNoFail() throws Exception {
        assertEquals(WifiIsUnusableEvent.TYPE_UNKNOWN,
                mWifiDataStall.checkForDataStall(mOldLlStats, mNewLlStats, mWifiInfo));
        verify(mWifiMetrics, never()).resetWifiIsUnusableLinkLayerStats();
        verifyUpdateWifiIsUnusableLinkLayerStats();
        verify(mWifiMetrics, never()).logWifiIsUnusableEvent(anyInt());
    }

    /**
     * Verify there is no data stall when the time difference between
     * two WifiLinkLayerStats is too big.
     */
    @Test
    public void verifyNoDataStallBigTimeGap() throws Exception {
        mNewLlStats.timeStampInMs = mOldLlStats.timeStampInMs
                + WifiDataStall.MAX_MS_DELTA_FOR_DATA_STALL + 1;
        assertEquals(WifiIsUnusableEvent.TYPE_UNKNOWN,
                mWifiDataStall.checkForDataStall(mOldLlStats, mNewLlStats, mWifiInfo));
        verifyUpdateWifiIsUnusableLinkLayerStats();
        verify(mWifiMetrics, never()).logWifiIsUnusableEvent(anyInt());
    }

    /**
     * Verify that metrics get reset when there is a reset in WifiLinkLayerStats
     */
    @Test
    public void verifyReset() throws Exception {
        mNewLlStats.lostmpdu_be = mOldLlStats.lostmpdu_be - 1;
        assertEquals(WifiIsUnusableEvent.TYPE_UNKNOWN,
                mWifiDataStall.checkForDataStall(mOldLlStats, mNewLlStats, mWifiInfo));
        verify(mWifiMetrics).resetWifiIsUnusableLinkLayerStats();
        verify(mWifiMetrics, never()).updateWifiIsUnusableLinkLayerStats(
                anyLong(), anyLong(), anyLong(), anyLong(), anyLong());
        verify(mWifiMetrics, never()).logWifiIsUnusableEvent(anyInt());
    }
}
