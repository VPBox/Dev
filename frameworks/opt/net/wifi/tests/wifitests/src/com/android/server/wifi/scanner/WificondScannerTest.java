/*
 * Copyright (C) 2015 The Android Open Source Project
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

package com.android.server.wifi.scanner;

import static com.android.server.wifi.ScanTestUtil.NativeScanSettingsBuilder;
import static com.android.server.wifi.ScanTestUtil.setupMockChannels;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.net.wifi.WifiScanner;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.ScanResults;
import com.android.server.wifi.WifiMonitor;
import com.android.server.wifi.WifiNative;
import com.android.server.wifi.scanner.ChannelHelper.ChannelCollection;

import org.junit.Before;
import org.junit.Test;

import java.io.FileDescriptor;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;

/**
 * Unit tests for {@link com.android.server.wifi.scanner.WificondScannerImpl}.
 */
@SmallTest
public class WificondScannerTest extends BaseWifiScannerImplTest {
    WifiMonitor mWifiMonitorSpy;
    @Before
    public void setup() throws Exception {
        setupMockChannels(mWifiNative,
                new int[]{2400, 2450},
                new int[]{5150, 5175},
                new int[]{5600, 5650});
        mWifiMonitorSpy = spy(mWifiMonitor);
        mScanner = new WificondScannerImpl(mContext, BaseWifiScannerImplTest.IFACE_NAME,
                mWifiNative, mWifiMonitorSpy, new WificondChannelHelper(mWifiNative),
                mLooper.getLooper(), mClock);
    }

    /**
     * Test that WificondScannerImpl will not issue a scan and report scan failure
     * when there is no channel to scan for.
     */
    @Test
    public void singleScanNotIssuedIfNoAvailableChannels() {
        // Use mocked ChannelHelper and ChannelCollection to simulate the scenario
        // that no channel is available for this request.
        ChannelHelper channelHelper = mock(ChannelHelper.class);
        ChannelCollection channelCollection = mock(ChannelCollection.class);
        when(channelHelper.createChannelCollection()).thenReturn(channelCollection);
        when(channelCollection.isEmpty()).thenReturn(true);

        mScanner = new WificondScannerImpl(mContext, BaseWifiScannerImplTest.IFACE_NAME,
                mWifiNative, mWifiMonitor, channelHelper, mLooper.getLooper(), mClock);

        WifiNative.ScanSettings settings = new NativeScanSettingsBuilder()
                .withBasePeriod(10000) // ms
                .withMaxApPerScan(10)
                .addBucketWithBand(10000 /* ms */, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN,
                        WifiScanner.WIFI_BAND_5_GHZ)
                .build();
        WifiNative.ScanEventHandler eventHandler = mock(WifiNative.ScanEventHandler.class);
        mScanner.startSingleScan(settings, eventHandler);

        mLooper.dispatchAll();

        // No scan is issued to WifiNative.
        verify(mWifiNative, never()).scan(any(), anyInt(), any(), any(List.class));
        // A scan failed event must be reported.
        verify(eventHandler).onScanStatus(WifiNative.WIFI_SCAN_FAILED);
    }

    @Test
    public void externalScanResultsDoNotCauseSpuriousTimerCancellationOrCrash() {
        mWifiMonitor.sendMessage(IFACE_NAME, WifiMonitor.SCAN_RESULTS_EVENT);
        mLooper.dispatchAll();
        verify(mAlarmManager.getAlarmManager(), never()).cancel(any(PendingIntent.class));
        verify(mAlarmManager.getAlarmManager(), never())
                .cancel(any(AlarmManager.OnAlarmListener.class));
        verify(mAlarmManager.getAlarmManager(), never()).cancel(isNull(PendingIntent.class));
        verify(mAlarmManager.getAlarmManager(), never())
                .cancel(isNull(AlarmManager.OnAlarmListener.class));
    }

    @Test
    public void externalScanResultsAfterOurScanDoNotCauseSpuriousTimerCancellationOrCrash() {
        WifiNative.ScanSettings settings = new NativeScanSettingsBuilder()
                .withBasePeriod(10000) // ms
                .withMaxApPerScan(10)
                .addBucketWithBand(10000 /* ms */, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN,
                        WifiScanner.WIFI_BAND_24_GHZ)
                .build();

        doSuccessfulSingleScanTest(settings,
                expectedBandScanFreqs(WifiScanner.WIFI_BAND_24_GHZ),
                new ArrayList<String>(),
                ScanResults.create(0, WifiScanner.WIFI_BAND_24_GHZ,
                        2400, 2450, 2450, 2400, 2450, 2450, 2400, 2450, 2450), false);

        mWifiMonitor.sendMessage(IFACE_NAME, WifiMonitor.SCAN_RESULTS_EVENT);
        mLooper.dispatchAll();
        verify(mAlarmManager.getAlarmManager(), never()).cancel(any(PendingIntent.class));
        verify(mAlarmManager.getAlarmManager(), times(1))
                .cancel(any(AlarmManager.OnAlarmListener.class));
        verify(mAlarmManager.getAlarmManager(), never()).cancel(isNull(PendingIntent.class));
        verify(mAlarmManager.getAlarmManager(), never())
                .cancel(isNull(AlarmManager.OnAlarmListener.class));
    }

    @Test
    public void lateScanResultsDoNotCauseSpuriousTimerCancellationOrCrash() {
        WifiNative.ScanSettings settings = new NativeScanSettingsBuilder()
                .withBasePeriod(10000) // ms
                .withMaxApPerScan(10)
                .addBucketWithBand(10000 /* ms */, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN,
                        WifiScanner.WIFI_BAND_24_GHZ)
                .build();

        // Kick off a scan
        when(mWifiNative.scan(eq(IFACE_NAME), anyInt(), any(), any(List.class))).thenReturn(true);
        WifiNative.ScanEventHandler eventHandler = mock(WifiNative.ScanEventHandler.class);
        assertTrue(mScanner.startSingleScan(settings, eventHandler));
        mLooper.dispatchAll();

        // Report a timeout
        mAlarmManager.dispatch(WificondScannerImpl.TIMEOUT_ALARM_TAG);
        mLooper.dispatchAll();

        // Now report scan results (results lost the race with timeout)
        mWifiMonitor.sendMessage(IFACE_NAME, WifiMonitor.SCAN_RESULTS_EVENT);
        mLooper.dispatchAll();

        verify(mAlarmManager.getAlarmManager(), never()).cancel(any(PendingIntent.class));
        verify(mAlarmManager.getAlarmManager(), never())
                .cancel(any(AlarmManager.OnAlarmListener.class));
        verify(mAlarmManager.getAlarmManager(), never()).cancel(isNull(PendingIntent.class));
        verify(mAlarmManager.getAlarmManager(), never())
                .cancel(isNull(AlarmManager.OnAlarmListener.class));
    }

    /**
     * Test that dump() of WificondScannerImpl dumps native scan results.
     */
    @Test
    public void dumpContainsNativeScanResults() {
        assertDumpContainsRequestLog("Latest native scan results:");
    }

    @Test
    public void cleanupDeregistersHandlers() {
        mScanner.cleanup();
        verify(mWifiMonitorSpy, times(1)).deregisterHandler(anyString(),
                eq(WifiMonitor.SCAN_FAILED_EVENT), any());
        verify(mWifiMonitorSpy, times(1)).deregisterHandler(anyString(),
                eq(WifiMonitor.PNO_SCAN_RESULTS_EVENT), any());
        verify(mWifiMonitorSpy, times(1)).deregisterHandler(anyString(),
                eq(WifiMonitor.SCAN_RESULTS_EVENT), any());
    }

    private void assertDumpContainsRequestLog(String log) {
        String objectDump = dumpObject();
        Pattern logLineRegex = Pattern.compile(".*" + log + ".*");
        assertTrue("dump did not contain log = " + log + "\n " + objectDump + "\n",
                logLineRegex.matcher(objectDump).find());
    }

    private String dumpObject() {
        StringWriter stringWriter = new StringWriter();
        mScanner.dump(new FileDescriptor(), new PrintWriter(stringWriter),
                new String[0]);
        return stringWriter.toString();
    }
}
