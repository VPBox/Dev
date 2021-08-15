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

package com.android.server.wifi.scanner;

import static com.android.server.wifi.ScanTestUtil.NativeScanSettingsBuilder;
import static com.android.server.wifi.ScanTestUtil.setupMockChannels;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

import android.app.test.TestAlarmManager;
import android.content.Context;
import android.net.wifi.WifiScanner;
import android.os.SystemClock;
import android.os.test.TestLooper;

import androidx.test.filters.SmallTest;

import com.android.internal.R;
import com.android.server.wifi.Clock;
import com.android.server.wifi.MockResources;
import com.android.server.wifi.MockWifiMonitor;
import com.android.server.wifi.ScanResults;
import com.android.server.wifi.WifiMonitor;
import com.android.server.wifi.WifiNative;
import com.android.server.wifi.scanner.ChannelHelper.ChannelCollection;

import org.junit.Before;
import org.junit.Test;
import org.mockito.InOrder;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.util.List;
import java.util.Set;

/**
 * Unit tests for {@link com.android.server.wifi.scanner.WificondScannerImpl.setPnoList}.
 */
@SmallTest
public class WificondPnoScannerTest {
    private static final String IFACE_NAME = "a_test_interface_name";

    @Mock Context mContext;
    TestAlarmManager mAlarmManager;
    MockWifiMonitor mWifiMonitor;
    TestLooper mLooper;
    @Mock WifiNative mWifiNative;
    MockResources mResources;
    @Mock Clock mClock;
    WificondScannerImpl mScanner;

    @Before
    public void setup() throws Exception {
        MockitoAnnotations.initMocks(this);

        mLooper = new TestLooper();
        mAlarmManager = new TestAlarmManager();
        mWifiMonitor = new MockWifiMonitor();
        mResources = new MockResources();

        setupMockChannels(mWifiNative,
                new int[]{2400, 2450},
                new int[]{5150, 5175},
                new int[]{5600, 5650});

        when(mWifiNative.getClientInterfaceName()).thenReturn(IFACE_NAME);
        when(mContext.getSystemService(Context.ALARM_SERVICE))
                .thenReturn(mAlarmManager.getAlarmManager());
        when(mContext.getResources()).thenReturn(mResources);
        when(mClock.getElapsedSinceBootMillis()).thenReturn(SystemClock.elapsedRealtime());
    }

    /**
     * Verify that the HW disconnected PNO scan triggers a wificond PNO scan and invokes the
     * OnPnoNetworkFound callback when the scan results are received.
     */
    @Test
    public void startHwDisconnectedPnoScan() {
        createScannerWithHwPnoScanSupport();

        WifiNative.PnoEventHandler pnoEventHandler = mock(WifiNative.PnoEventHandler.class);
        WifiNative.PnoSettings pnoSettings = createDummyPnoSettings(false);
        ScanResults scanResults = createDummyScanResults();

        InOrder order = inOrder(pnoEventHandler, mWifiNative);
        // Start PNO scan
        startSuccessfulPnoScan(null, pnoSettings, null, pnoEventHandler);
        expectSuccessfulHwDisconnectedPnoScan(order, pnoSettings, pnoEventHandler, scanResults);
        verifyNoMoreInteractions(pnoEventHandler);
    }

    /**
     * Verify that the HW PNO scan stop failure still resets the PNO scan state.
     * 1. Start Hw PNO.
     * 2. Stop Hw PNO scan which raises a stop command to WifiNative which is failed.
     * 3. Now restart a new PNO scan to ensure that the failure was cleanly handled.
     */
    @Test
    public void ignoreHwDisconnectedPnoScanStopFailure() {
        createScannerWithHwPnoScanSupport();

        WifiNative.PnoEventHandler pnoEventHandler = mock(WifiNative.PnoEventHandler.class);
        WifiNative.PnoSettings pnoSettings = createDummyPnoSettings(false);

        // Start PNO scan
        startSuccessfulPnoScan(null, pnoSettings, null, pnoEventHandler);

        // Fail the PNO stop.
        when(mWifiNative.stopPnoScan(IFACE_NAME)).thenReturn(false);
        assertTrue(mScanner.resetHwPnoList());
        mLooper.dispatchAll();
        verify(mWifiNative).stopPnoScan(IFACE_NAME);

        // Add a new PNO scan request and ensure it runs successfully.
        startSuccessfulPnoScan(null, pnoSettings, null, pnoEventHandler);
        mLooper.dispatchAll();
        InOrder order = inOrder(pnoEventHandler, mWifiNative);
        ScanResults scanResults = createDummyScanResults();
        expectSuccessfulHwDisconnectedPnoScan(order, pnoSettings, pnoEventHandler, scanResults);
        verifyNoMoreInteractions(pnoEventHandler);
    }

    private void createScannerWithHwPnoScanSupport() {
        mResources.setBoolean(R.bool.config_wifi_background_scan_support, true);
        mScanner = new WificondScannerImpl(mContext, IFACE_NAME, mWifiNative, mWifiMonitor,
                new WificondChannelHelper(mWifiNative), mLooper.getLooper(), mClock);
    }

    private WifiNative.PnoNetwork createDummyPnoNetwork(String ssid) {
        WifiNative.PnoNetwork pnoNetwork = new WifiNative.PnoNetwork();
        pnoNetwork.ssid = ssid;
        return pnoNetwork;
    }

    private WifiNative.PnoSettings createDummyPnoSettings(boolean isConnected) {
        WifiNative.PnoSettings pnoSettings = new WifiNative.PnoSettings();
        pnoSettings.isConnected = isConnected;
        pnoSettings.networkList = new WifiNative.PnoNetwork[2];
        pnoSettings.networkList[0] = createDummyPnoNetwork("ssid_pno_1");
        pnoSettings.networkList[1] = createDummyPnoNetwork("ssid_pno_2");
        return pnoSettings;
    }

    private WifiNative.ScanSettings createDummyScanSettings(boolean allChannelsScanned) {
        WifiNative.ScanSettings settings = new NativeScanSettingsBuilder()
                .withBasePeriod(10000)
                .withMaxApPerScan(10)
                .addBucketWithBand(
                    10000,
                    WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN,
                    allChannelsScanned
                            ? WifiScanner.WIFI_BAND_BOTH_WITH_DFS : WifiScanner.WIFI_BAND_24_GHZ)
                .build();
        return settings;
    }

    private ScanResults createDummyScanResults() {
        return ScanResults.create(0, WifiScanner.WIFI_BAND_UNSPECIFIED,  2400, 2450, 2450, 2400,
                2450, 2450, 2400, 2450, 2450);
    }

    private void startSuccessfulPnoScan(WifiNative.ScanSettings scanSettings,
            WifiNative.PnoSettings pnoSettings, WifiNative.ScanEventHandler scanEventHandler,
            WifiNative.PnoEventHandler pnoEventHandler) {
        // Scans succeed
        when(mWifiNative.scan(eq(IFACE_NAME), anyInt(), any(), any(List.class))).thenReturn(true);
        when(mWifiNative.startPnoScan(eq(IFACE_NAME), any(WifiNative.PnoSettings.class)))
                .thenReturn(true);
        when(mWifiNative.stopPnoScan(IFACE_NAME)).thenReturn(true);

        assertTrue(mScanner.setHwPnoList(pnoSettings, pnoEventHandler));
    }

    private Set<Integer> expectedBandScanFreqs(int band) {
        ChannelCollection collection = mScanner.getChannelHelper().createChannelCollection();
        collection.addBand(band);
        return collection.getScanFreqs();
    }

    /**
     * Verify that the PNO scan was successfully started.
     */
    private void expectHwDisconnectedPnoScanStart(InOrder order,
            WifiNative.PnoSettings pnoSettings) {
        // Verify  HW PNO scan started
        order.verify(mWifiNative).startPnoScan(any(), any(WifiNative.PnoSettings.class));
    }

    /**
     *
     * 1. Verify that the PNO scan was successfully started.
     * 2. Send scan results and ensure that the |onPnoNetworkFound| callback was called.
     */
    private void expectSuccessfulHwDisconnectedPnoScan(InOrder order,
            WifiNative.PnoSettings pnoSettings, WifiNative.PnoEventHandler eventHandler,
            ScanResults scanResults) {
        expectHwDisconnectedPnoScanStart(order, pnoSettings);

        // Setup scan results
        when(mWifiNative.getPnoScanResults(IFACE_NAME))
                .thenReturn(scanResults.getScanDetailArrayList());
        when(mWifiNative.getScanResults(IFACE_NAME))
                .thenReturn(scanResults.getScanDetailArrayList());

        // Notify scan has finished
        mWifiMonitor.sendMessage(IFACE_NAME, WifiMonitor.PNO_SCAN_RESULTS_EVENT);
        assertEquals("dispatch message after results event", 1, mLooper.dispatchAll());

        order.verify(eventHandler).onPnoNetworkFound(scanResults.getRawScanResults());
    }

}
