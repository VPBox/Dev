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

import static android.net.wifi.WifiConfiguration.NetworkSelectionStatus.DISABLED_AUTHENTICATION_FAILURE;
import static android.net.wifi.WifiConfiguration.NetworkSelectionStatus.DISABLED_NO_INTERNET_TEMPORARY;

import static com.android.server.wifi.ClientModeImpl.WIFI_WORK_SOURCE;
import static com.android.server.wifi.WifiConfigurationTestUtil.generateWifiConfig;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

import android.app.test.MockAnswerUtil.AnswerWithArguments;
import android.app.test.TestAlarmManager;
import android.content.Context;
import android.content.pm.PackageManager;
import android.content.res.Resources;
import android.net.NetworkScoreManager;
import android.net.wifi.ScanResult;
import android.net.wifi.ScanResult.InformationElement;
import android.net.wifi.SupplicantState;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiNetworkScoreCache;
import android.net.wifi.WifiScanner;
import android.net.wifi.WifiScanner.PnoScanListener;
import android.net.wifi.WifiScanner.PnoSettings;
import android.net.wifi.WifiScanner.ScanData;
import android.net.wifi.WifiScanner.ScanListener;
import android.net.wifi.WifiScanner.ScanSettings;
import android.net.wifi.WifiSsid;
import android.os.Process;
import android.os.SystemClock;
import android.os.WorkSource;
import android.os.test.TestLooper;
import android.util.LocalLog;

import androidx.test.filters.SmallTest;

import com.android.internal.R;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Captor;
import org.mockito.InOrder;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.io.FileDescriptor;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.stream.Collectors;

/**
 * Unit tests for {@link com.android.server.wifi.WifiConnectivityManager}.
 */
@SmallTest
public class WifiConnectivityManagerTest {
    /**
     * Called before each test
     */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mResource = mockResource();
        mAlarmManager = new TestAlarmManager();
        mContext = mockContext();
        mLocalLog = new LocalLog(512);
        mClientModeImpl = mockClientModeImpl();
        mWifiConfigManager = mockWifiConfigManager();
        mWifiInfo = getWifiInfo();
        mScanData = mockScanData();
        mWifiScanner = mockWifiScanner();
        mWifiConnectivityHelper = mockWifiConnectivityHelper();
        mWifiNS = mockWifiNetworkSelector();
        when(mWifiInjector.getWifiScanner()).thenReturn(mWifiScanner);
        when(mWifiInjector.getWifiNetworkSuggestionsManager())
                .thenReturn(mWifiNetworkSuggestionsManager);
        when(mWifiNetworkSuggestionsManager.retrieveHiddenNetworkList())
                .thenReturn(new ArrayList<>());
        mWifiConnectivityManager = createConnectivityManager();
        verify(mWifiConfigManager).setOnSavedNetworkUpdateListener(anyObject());
        mWifiConnectivityManager.setTrustedConnectionAllowed(true);
        mWifiConnectivityManager.setWifiEnabled(true);
        when(mClock.getElapsedSinceBootMillis()).thenReturn(SystemClock.elapsedRealtime());
        mFullScanMaxTxPacketRate = mResource.getInteger(
                R.integer.config_wifi_framework_max_tx_rate_for_full_scan);
        mFullScanMaxRxPacketRate = mResource.getInteger(
                R.integer.config_wifi_framework_max_rx_rate_for_full_scan);
        when(mCarrierNetworkConfig.isCarrierEncryptionInfoAvailable()).thenReturn(true);
        when(mWifiLastResortWatchdog.shouldIgnoreBssidUpdate(anyString())).thenReturn(false);
    }

    /**
     * Called after each test
     */
    @After
    public void cleanup() {
        validateMockitoUsage();
    }

    private Resources mResource;

    private Context mContext;
    private TestAlarmManager mAlarmManager;
    private TestLooper mLooper = new TestLooper();
    private WifiConnectivityManager mWifiConnectivityManager;
    private WifiNetworkSelector mWifiNS;
    private ClientModeImpl mClientModeImpl;
    private WifiScanner mWifiScanner;
    private WifiConnectivityHelper mWifiConnectivityHelper;
    private ScanData mScanData;
    private WifiConfigManager mWifiConfigManager;
    private WifiInfo mWifiInfo;
    private LocalLog mLocalLog;
    @Mock private WifiInjector mWifiInjector;
    @Mock private NetworkScoreManager mNetworkScoreManager;
    @Mock private Clock mClock;
    @Mock private WifiLastResortWatchdog mWifiLastResortWatchdog;
    @Mock private OpenNetworkNotifier mOpenNetworkNotifier;
    @Mock private CarrierNetworkNotifier mCarrierNetworkNotifier;
    @Mock private CarrierNetworkConfig mCarrierNetworkConfig;
    @Mock private WifiMetrics mWifiMetrics;
    @Mock private WifiNetworkScoreCache mScoreCache;
    @Mock private WifiNetworkSuggestionsManager mWifiNetworkSuggestionsManager;
    @Captor ArgumentCaptor<ScanResult> mCandidateScanResultCaptor;
    @Captor ArgumentCaptor<ArrayList<String>> mBssidBlacklistCaptor;
    @Captor ArgumentCaptor<ArrayList<String>> mSsidWhitelistCaptor;
    @Captor ArgumentCaptor<WifiConfigManager.OnSavedNetworkUpdateListener>
            mSavedNetworkUpdateListenerCaptor;
    private MockResources mResources;
    private int mFullScanMaxTxPacketRate;
    private int mFullScanMaxRxPacketRate;

    private static final int CANDIDATE_NETWORK_ID = 0;
    private static final String CANDIDATE_SSID = "\"AnSsid\"";
    private static final String CANDIDATE_BSSID = "6c:f3:7f:ae:8c:f3";
    private static final String INVALID_SCAN_RESULT_BSSID = "6c:f3:7f:ae:8c:f4";
    private static final long CURRENT_SYSTEM_TIME_MS = 1000;
    private static final int MAX_BSSID_BLACKLIST_SIZE = 16;


    Resources mockResource() {
        Resources resource = mock(Resources.class);

        when(resource.getInteger(R.integer.config_wifi_framework_SECURITY_AWARD)).thenReturn(80);
        when(resource.getInteger(R.integer.config_wifi_framework_SAME_BSSID_AWARD)).thenReturn(24);
        when(resource.getBoolean(
                R.bool.config_wifi_framework_enable_associated_network_selection)).thenReturn(true);
        when(resource.getInteger(
                R.integer.config_wifi_framework_wifi_score_good_rssi_threshold_24GHz))
                .thenReturn(-60);
        when(resource.getInteger(
                R.integer.config_wifi_framework_current_network_boost)).thenReturn(16);
        when(resource.getInteger(
                R.integer.config_wifi_framework_max_tx_rate_for_full_scan)).thenReturn(8);
        when(resource.getInteger(
                R.integer.config_wifi_framework_max_rx_rate_for_full_scan)).thenReturn(16);
        return resource;
    }

    Context mockContext() {
        Context context = mock(Context.class);

        when(context.getResources()).thenReturn(mResource);
        when(context.getSystemService(Context.ALARM_SERVICE)).thenReturn(
                mAlarmManager.getAlarmManager());
        when(context.getPackageManager()).thenReturn(mock(PackageManager.class));

        return context;
    }

    ScanData mockScanData() {
        ScanData scanData = mock(ScanData.class);

        when(scanData.getBandScanned()).thenReturn(WifiScanner.WIFI_BAND_BOTH_WITH_DFS);

        return scanData;
    }

    WifiScanner mockWifiScanner() {
        WifiScanner scanner = mock(WifiScanner.class);
        ArgumentCaptor<ScanListener> allSingleScanListenerCaptor =
                ArgumentCaptor.forClass(ScanListener.class);

        doNothing().when(scanner).registerScanListener(allSingleScanListenerCaptor.capture());

        ScanData[] scanDatas = new ScanData[1];
        scanDatas[0] = mScanData;

        // do a synchronous answer for the ScanListener callbacks
        doAnswer(new AnswerWithArguments() {
            public void answer(ScanSettings settings, ScanListener listener,
                    WorkSource workSource) throws Exception {
                listener.onResults(scanDatas);
            }}).when(scanner).startBackgroundScan(anyObject(), anyObject(), anyObject());

        doAnswer(new AnswerWithArguments() {
            public void answer(ScanSettings settings, ScanListener listener,
                    WorkSource workSource) throws Exception {
                listener.onResults(scanDatas);
                // WCM processes scan results received via onFullResult (even though they're the
                // same as onResult for single scans).
                if (mScanData != null && mScanData.getResults() != null) {
                    for (int i = 0; i < mScanData.getResults().length; i++) {
                        allSingleScanListenerCaptor.getValue().onFullResult(
                                mScanData.getResults()[i]);
                    }
                }
                allSingleScanListenerCaptor.getValue().onResults(scanDatas);
            }}).when(scanner).startScan(anyObject(), anyObject(), anyObject());

        // This unfortunately needs to be a somewhat valid scan result, otherwise
        // |ScanDetailUtil.toScanDetail| raises exceptions.
        final ScanResult[] scanResults = new ScanResult[1];
        scanResults[0] = new ScanResult(WifiSsid.createFromAsciiEncoded(CANDIDATE_SSID),
                CANDIDATE_SSID, CANDIDATE_BSSID, 1245, 0, "some caps",
                -78, 2450, 1025, 22, 33, 20, 0, 0, true);
        scanResults[0].informationElements = new InformationElement[1];
        scanResults[0].informationElements[0] = new InformationElement();
        scanResults[0].informationElements[0].id = InformationElement.EID_SSID;
        scanResults[0].informationElements[0].bytes =
            CANDIDATE_SSID.getBytes(StandardCharsets.UTF_8);

        doAnswer(new AnswerWithArguments() {
            public void answer(ScanSettings settings, PnoSettings pnoSettings,
                    PnoScanListener listener) throws Exception {
                listener.onPnoNetworkFound(scanResults);
            }}).when(scanner).startDisconnectedPnoScan(anyObject(), anyObject(), anyObject());

        doAnswer(new AnswerWithArguments() {
            public void answer(ScanSettings settings, PnoSettings pnoSettings,
                    PnoScanListener listener) throws Exception {
                listener.onPnoNetworkFound(scanResults);
            }}).when(scanner).startConnectedPnoScan(anyObject(), anyObject(), anyObject());

        return scanner;
    }

    WifiConnectivityHelper mockWifiConnectivityHelper() {
        WifiConnectivityHelper connectivityHelper = mock(WifiConnectivityHelper.class);

        when(connectivityHelper.isFirmwareRoamingSupported()).thenReturn(false);
        when(connectivityHelper.getMaxNumBlacklistBssid()).thenReturn(MAX_BSSID_BLACKLIST_SIZE);

        return connectivityHelper;
    }

    ClientModeImpl mockClientModeImpl() {
        ClientModeImpl stateMachine = mock(ClientModeImpl.class);

        when(stateMachine.isConnected()).thenReturn(false);
        when(stateMachine.isDisconnected()).thenReturn(true);
        when(stateMachine.isSupplicantTransientState()).thenReturn(false);

        return stateMachine;
    }

    WifiNetworkSelector mockWifiNetworkSelector() {
        WifiNetworkSelector ns = mock(WifiNetworkSelector.class);

        WifiConfiguration candidate = generateWifiConfig(
                0, CANDIDATE_NETWORK_ID, CANDIDATE_SSID, false, true, null, null);
        candidate.BSSID = ClientModeImpl.SUPPLICANT_BSSID_ANY;
        ScanResult candidateScanResult = new ScanResult();
        candidateScanResult.SSID = CANDIDATE_SSID;
        candidateScanResult.BSSID = CANDIDATE_BSSID;
        candidate.getNetworkSelectionStatus().setCandidate(candidateScanResult);

        when(ns.selectNetwork(anyObject(), anyObject(), anyObject(), anyBoolean(),
                anyBoolean(), anyBoolean())).thenReturn(candidate);
        return ns;
    }

    WifiInfo getWifiInfo() {
        WifiInfo wifiInfo = new WifiInfo();

        wifiInfo.setNetworkId(WifiConfiguration.INVALID_NETWORK_ID);
        wifiInfo.setBSSID(null);
        wifiInfo.setSupplicantState(SupplicantState.DISCONNECTED);

        return wifiInfo;
    }

    WifiConfigManager mockWifiConfigManager() {
        WifiConfigManager wifiConfigManager = mock(WifiConfigManager.class);

        when(wifiConfigManager.getConfiguredNetwork(anyInt())).thenReturn(null);

        // Pass dummy pno network list, otherwise Pno scan requests will not be triggered.
        PnoSettings.PnoNetwork pnoNetwork = new PnoSettings.PnoNetwork(CANDIDATE_SSID);
        ArrayList<PnoSettings.PnoNetwork> pnoNetworkList = new ArrayList<>();
        pnoNetworkList.add(pnoNetwork);
        when(wifiConfigManager.retrievePnoNetworkList()).thenReturn(pnoNetworkList);
        when(wifiConfigManager.retrievePnoNetworkList()).thenReturn(pnoNetworkList);
        doNothing().when(wifiConfigManager).setOnSavedNetworkUpdateListener(
                mSavedNetworkUpdateListenerCaptor.capture());

        return wifiConfigManager;
    }

    WifiConnectivityManager createConnectivityManager() {
        return new WifiConnectivityManager(mContext,
                new ScoringParams(mContext),
                mClientModeImpl, mWifiInjector,
                mWifiConfigManager, mWifiInfo, mWifiNS, mWifiConnectivityHelper,
                mWifiLastResortWatchdog, mOpenNetworkNotifier, mCarrierNetworkNotifier,
                mCarrierNetworkConfig, mWifiMetrics, mLooper.getLooper(), mClock, mLocalLog);
    }

    /**
     *  Wifi enters disconnected state while screen is on.
     *
     * Expected behavior: WifiConnectivityManager calls
     * ClientModeImpl.startConnectToNetwork() with the
     * expected candidate network ID and BSSID.
     */
    @Test
    public void enterWifiDisconnectedStateWhenScreenOn() {
        // Set screen to on
        mWifiConnectivityManager.handleScreenStateChanged(true);

        // Set WiFi to disconnected state
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        verify(mClientModeImpl).startConnectToNetwork(
                CANDIDATE_NETWORK_ID, Process.WIFI_UID, CANDIDATE_BSSID);
    }

    /**
     *  Wifi enters connected state while screen is on.
     *
     * Expected behavior: WifiConnectivityManager calls
     * ClientModeImpl.startConnectToNetwork() with the
     * expected candidate network ID and BSSID.
     */
    @Test
    public void enterWifiConnectedStateWhenScreenOn() {
        // Set screen to on
        mWifiConnectivityManager.handleScreenStateChanged(true);

        // Set WiFi to connected state
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_CONNECTED);

        verify(mClientModeImpl).startConnectToNetwork(
                CANDIDATE_NETWORK_ID, Process.WIFI_UID, CANDIDATE_BSSID);
    }

    /**
     *  Screen turned on while WiFi in disconnected state.
     *
     * Expected behavior: WifiConnectivityManager calls
     * ClientModeImpl.startConnectToNetwork() with the
     * expected candidate network ID and BSSID.
     */
    @Test
    public void turnScreenOnWhenWifiInDisconnectedState() {
        // Set WiFi to disconnected state
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        // Set screen to on
        mWifiConnectivityManager.handleScreenStateChanged(true);

        verify(mClientModeImpl, atLeastOnce()).startConnectToNetwork(
                CANDIDATE_NETWORK_ID, Process.WIFI_UID, CANDIDATE_BSSID);
    }

    /**
     *  Screen turned on while WiFi in connected state.
     *
     * Expected behavior: WifiConnectivityManager calls
     * ClientModeImpl.startConnectToNetwork() with the
     * expected candidate network ID and BSSID.
     */
    @Test
    public void turnScreenOnWhenWifiInConnectedState() {
        // Set WiFi to connected state
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_CONNECTED);

        // Set screen to on
        mWifiConnectivityManager.handleScreenStateChanged(true);

        verify(mClientModeImpl, atLeastOnce()).startConnectToNetwork(
                CANDIDATE_NETWORK_ID, Process.WIFI_UID, CANDIDATE_BSSID);
    }

    /**
     *  Screen turned on while WiFi in connected state but
     *  auto roaming is disabled.
     *
     * Expected behavior: WifiConnectivityManager doesn't invoke
     * ClientModeImpl.startConnectToNetwork() because roaming
     * is turned off.
     */
    @Test
    public void turnScreenOnWhenWifiInConnectedStateRoamingDisabled() {
        // Turn off auto roaming
        when(mResource.getBoolean(
                R.bool.config_wifi_framework_enable_associated_network_selection))
                .thenReturn(false);
        mWifiConnectivityManager = createConnectivityManager();
        mWifiConnectivityManager.setTrustedConnectionAllowed(true);

        // Set WiFi to connected state
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_CONNECTED);

        // Set screen to on
        mWifiConnectivityManager.handleScreenStateChanged(true);

        verify(mClientModeImpl, times(0)).startConnectToNetwork(
                CANDIDATE_NETWORK_ID, Process.WIFI_UID, CANDIDATE_BSSID);
    }

    /**
     * Multiple back to back connection attempts within the rate interval should be rate limited.
     *
     * Expected behavior: WifiConnectivityManager calls ClientModeImpl.startConnectToNetwork()
     * with the expected candidate network ID and BSSID for only the expected number of times within
     * the given interval.
     */
    @Test
    public void connectionAttemptRateLimitedWhenScreenOff() {
        int maxAttemptRate = WifiConnectivityManager.MAX_CONNECTION_ATTEMPTS_RATE;
        int timeInterval = WifiConnectivityManager.MAX_CONNECTION_ATTEMPTS_TIME_INTERVAL_MS;
        int numAttempts = 0;
        int connectionAttemptIntervals = timeInterval / maxAttemptRate;

        mWifiConnectivityManager.handleScreenStateChanged(false);

        // First attempt the max rate number of connections within the rate interval.
        long currentTimeStamp = 0;
        for (int attempt = 0; attempt < maxAttemptRate; attempt++) {
            currentTimeStamp += connectionAttemptIntervals;
            when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);
            // Set WiFi to disconnected state to trigger PNO scan
            mWifiConnectivityManager.handleConnectionStateChanged(
                    WifiConnectivityManager.WIFI_STATE_DISCONNECTED);
            numAttempts++;
        }
        // Now trigger another connection attempt before the rate interval, this should be
        // skipped because we've crossed rate limit.
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);
        // Set WiFi to disconnected state to trigger PNO scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        // Verify that we attempt to connect upto the rate.
        verify(mClientModeImpl, times(numAttempts)).startConnectToNetwork(
                CANDIDATE_NETWORK_ID, Process.WIFI_UID, CANDIDATE_BSSID);
    }

    /**
     * Multiple back to back connection attempts outside the rate interval should not be rate
     * limited.
     *
     * Expected behavior: WifiConnectivityManager calls ClientModeImpl.startConnectToNetwork()
     * with the expected candidate network ID and BSSID for only the expected number of times within
     * the given interval.
     */
    @Test
    public void connectionAttemptNotRateLimitedWhenScreenOff() {
        int maxAttemptRate = WifiConnectivityManager.MAX_CONNECTION_ATTEMPTS_RATE;
        int timeInterval = WifiConnectivityManager.MAX_CONNECTION_ATTEMPTS_TIME_INTERVAL_MS;
        int numAttempts = 0;
        int connectionAttemptIntervals = timeInterval / maxAttemptRate;

        mWifiConnectivityManager.handleScreenStateChanged(false);

        // First attempt the max rate number of connections within the rate interval.
        long currentTimeStamp = 0;
        for (int attempt = 0; attempt < maxAttemptRate; attempt++) {
            currentTimeStamp += connectionAttemptIntervals;
            when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);
            // Set WiFi to disconnected state to trigger PNO scan
            mWifiConnectivityManager.handleConnectionStateChanged(
                    WifiConnectivityManager.WIFI_STATE_DISCONNECTED);
            numAttempts++;
        }
        // Now trigger another connection attempt after the rate interval, this should not be
        // skipped because we should've evicted the older attempt.
        when(mClock.getElapsedSinceBootMillis()).thenReturn(
                currentTimeStamp + connectionAttemptIntervals * 2);
        // Set WiFi to disconnected state to trigger PNO scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);
        numAttempts++;

        // Verify that all the connection attempts went through
        verify(mClientModeImpl, times(numAttempts)).startConnectToNetwork(
                CANDIDATE_NETWORK_ID, Process.WIFI_UID, CANDIDATE_BSSID);
    }

    /**
     * Multiple back to back connection attempts after a user selection should not be rate limited.
     *
     * Expected behavior: WifiConnectivityManager calls ClientModeImpl.startConnectToNetwork()
     * with the expected candidate network ID and BSSID for only the expected number of times within
     * the given interval.
     */
    @Test
    public void connectionAttemptNotRateLimitedWhenScreenOffAfterUserSelection() {
        int maxAttemptRate = WifiConnectivityManager.MAX_CONNECTION_ATTEMPTS_RATE;
        int timeInterval = WifiConnectivityManager.MAX_CONNECTION_ATTEMPTS_TIME_INTERVAL_MS;
        int numAttempts = 0;
        int connectionAttemptIntervals = timeInterval / maxAttemptRate;

        mWifiConnectivityManager.handleScreenStateChanged(false);

        // First attempt the max rate number of connections within the rate interval.
        long currentTimeStamp = 0;
        for (int attempt = 0; attempt < maxAttemptRate; attempt++) {
            currentTimeStamp += connectionAttemptIntervals;
            when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);
            // Set WiFi to disconnected state to trigger PNO scan
            mWifiConnectivityManager.handleConnectionStateChanged(
                    WifiConnectivityManager.WIFI_STATE_DISCONNECTED);
            numAttempts++;
        }

        mWifiConnectivityManager.setUserConnectChoice(CANDIDATE_NETWORK_ID);
        mWifiConnectivityManager.prepareForForcedConnection(CANDIDATE_NETWORK_ID);

        for (int attempt = 0; attempt < maxAttemptRate; attempt++) {
            currentTimeStamp += connectionAttemptIntervals;
            when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);
            // Set WiFi to disconnected state to trigger PNO scan
            mWifiConnectivityManager.handleConnectionStateChanged(
                    WifiConnectivityManager.WIFI_STATE_DISCONNECTED);
            numAttempts++;
        }

        // Verify that all the connection attempts went through
        verify(mClientModeImpl, times(numAttempts)).startConnectToNetwork(
                CANDIDATE_NETWORK_ID, Process.WIFI_UID, CANDIDATE_BSSID);
    }

    /**
     *  PNO retry for low RSSI networks.
     *
     * Expected behavior: WifiConnectivityManager doubles the low RSSI
     * network retry delay value after QNS skips the PNO scan results
     * because of their low RSSI values.
     */
    @Test
    public void pnoRetryForLowRssiNetwork() {
        when(mWifiNS.selectNetwork(anyObject(), anyObject(), anyObject(), anyBoolean(),
                anyBoolean(), anyBoolean())).thenReturn(null);

        // Set screen to off
        mWifiConnectivityManager.handleScreenStateChanged(false);

        // Get the current retry delay value
        int lowRssiNetworkRetryDelayStartValue = mWifiConnectivityManager
                .getLowRssiNetworkRetryDelay();

        // Set WiFi to disconnected state to trigger PNO scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        // Get the retry delay value after QNS didn't select a
        // network candicate from the PNO scan results.
        int lowRssiNetworkRetryDelayAfterPnoValue = mWifiConnectivityManager
                .getLowRssiNetworkRetryDelay();

        assertEquals(lowRssiNetworkRetryDelayStartValue * 2,
                lowRssiNetworkRetryDelayAfterPnoValue);
    }

    /**
     * Ensure that the watchdog bite increments the "Pno bad" metric.
     *
     * Expected behavior: WifiConnectivityManager detects that the PNO scan failed to find
     * a candidate while watchdog single scan did.
     */
    @Test
    public void watchdogBitePnoBadIncrementsMetrics() {
        // Set screen to off
        mWifiConnectivityManager.handleScreenStateChanged(false);

        // Set WiFi to disconnected state to trigger PNO scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        // Now fire the watchdog alarm and verify the metrics were incremented.
        mAlarmManager.dispatch(WifiConnectivityManager.WATCHDOG_TIMER_TAG);
        mLooper.dispatchAll();

        verify(mWifiMetrics).incrementNumConnectivityWatchdogPnoBad();
        verify(mWifiMetrics, never()).incrementNumConnectivityWatchdogPnoGood();
    }

    /**
     * Ensure that the watchdog bite increments the "Pno good" metric.
     *
     * Expected behavior: WifiConnectivityManager detects that the PNO scan failed to find
     * a candidate which was the same with watchdog single scan.
     */
    @Test
    public void watchdogBitePnoGoodIncrementsMetrics() {
        // Qns returns no candidate after watchdog single scan.
        when(mWifiNS.selectNetwork(anyObject(), anyObject(), anyObject(), anyBoolean(),
                anyBoolean(), anyBoolean())).thenReturn(null);

        // Set screen to off
        mWifiConnectivityManager.handleScreenStateChanged(false);

        // Set WiFi to disconnected state to trigger PNO scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        // Now fire the watchdog alarm and verify the metrics were incremented.
        mAlarmManager.dispatch(WifiConnectivityManager.WATCHDOG_TIMER_TAG);
        mLooper.dispatchAll();

        verify(mWifiMetrics).incrementNumConnectivityWatchdogPnoGood();
        verify(mWifiMetrics, never()).incrementNumConnectivityWatchdogPnoBad();
    }

    /**
     * {@link OpenNetworkNotifier} handles scan results on network selection.
     *
     * Expected behavior: ONA handles scan results
     */
    @Test
    public void wifiDisconnected_noConnectionCandidate_openNetworkNotifierScanResultsHandled() {
        // no connection candidate selected
        when(mWifiNS.selectNetwork(anyObject(), anyObject(), anyObject(), anyBoolean(),
                anyBoolean(), anyBoolean())).thenReturn(null);

        List<ScanDetail> expectedOpenNetworks = new ArrayList<>();
        expectedOpenNetworks.add(
                new ScanDetail(
                        new ScanResult(WifiSsid.createFromAsciiEncoded(CANDIDATE_SSID),
                                CANDIDATE_SSID, CANDIDATE_BSSID, 1245, 0, "some caps", -78, 2450,
                                1025, 22, 33, 20, 0, 0, true), null));

        when(mWifiNS.getFilteredScanDetailsForOpenUnsavedNetworks())
                .thenReturn(expectedOpenNetworks);

        // Set WiFi to disconnected state to trigger PNO scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        verify(mOpenNetworkNotifier).handleScanResults(expectedOpenNetworks);
    }

    /**
     * When wifi is connected, {@link OpenNetworkNotifier} handles the Wi-Fi connected behavior.
     *
     * Expected behavior: ONA handles connected behavior
     */
    @Test
    public void wifiConnected_openNetworkNotifierHandlesConnection() {
        // Set WiFi to connected state
        mWifiInfo.setSSID(WifiSsid.createFromAsciiEncoded(CANDIDATE_SSID));
        mWifiConnectivityManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_NONE);
        verify(mOpenNetworkNotifier).handleWifiConnected(CANDIDATE_SSID);
    }

    /**
     * When wifi is connected, {@link OpenNetworkNotifier} handles connection state
     * change.
     *
     * Expected behavior: ONA does not clear pending notification.
     */
    @Test
    public void wifiDisconnected_openNetworkNotifierDoesNotClearPendingNotification() {
        // Set WiFi to disconnected state
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        verify(mOpenNetworkNotifier, never()).clearPendingNotification(anyBoolean());
    }

    /**
     * When a Wi-Fi connection attempt ends, {@link OpenNetworkNotifier} handles the connection
     * failure. A failure code that is not {@link WifiMetrics.ConnectionEvent#FAILURE_NONE}
     * represents a connection failure.
     *
     * Expected behavior: ONA handles connection failure.
     */
    @Test
    public void wifiConnectionEndsWithFailure_openNetworkNotifierHandlesConnectionFailure() {
        mWifiConnectivityManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_CONNECT_NETWORK_FAILED);

        verify(mOpenNetworkNotifier).handleConnectionFailure();
    }

    /**
     * When a Wi-Fi connection attempt ends, {@link OpenNetworkNotifier} does not handle connection
     * failure after a successful connection. {@link WifiMetrics.ConnectionEvent#FAILURE_NONE}
     * represents a successful connection.
     *
     * Expected behavior: ONA does nothing.
     */
    @Test
    public void wifiConnectionEndsWithSuccess_openNetworkNotifierDoesNotHandleConnectionFailure() {
        mWifiConnectivityManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_NONE);

        verify(mOpenNetworkNotifier, never()).handleConnectionFailure();
    }

    /**
     * When Wi-Fi is disabled, clear the pending notification and reset notification repeat delay.
     *
     * Expected behavior: clear pending notification and reset notification repeat delay
     * */
    @Test
    public void openNetworkNotifierClearsPendingNotificationOnWifiDisabled() {
        mWifiConnectivityManager.setWifiEnabled(false);

        verify(mOpenNetworkNotifier).clearPendingNotification(true /* resetRepeatDelay */);
    }

    /**
     * Verify that the ONA controller tracks screen state changes.
     */
    @Test
    public void openNetworkNotifierTracksScreenStateChanges() {
        mWifiConnectivityManager.handleScreenStateChanged(false);

        verify(mOpenNetworkNotifier).handleScreenStateChanged(false);

        mWifiConnectivityManager.handleScreenStateChanged(true);

        verify(mOpenNetworkNotifier).handleScreenStateChanged(true);
    }

    /**
     * {@link CarrierNetworkNotifier} handles scan results on network selection.
     *
     * Expected behavior: CarrierNetworkNotifier handles scan results
     */
    @Test
    public void wifiDisconnected_noConnectionCandidate_CarrierNetworkNotifierScanResultsHandled() {
        // no connection candidate selected
        when(mWifiNS.selectNetwork(anyObject(), anyObject(), anyObject(), anyBoolean(),
                anyBoolean(), anyBoolean())).thenReturn(null);

        List<ScanDetail> expectedCarrierNetworks = new ArrayList<>();
        expectedCarrierNetworks.add(
                new ScanDetail(
                        new ScanResult(WifiSsid.createFromAsciiEncoded(CANDIDATE_SSID),
                                CANDIDATE_SSID, CANDIDATE_BSSID, 1245, 0, "[EAP][ESS]", -78, 2450,
                                1025, 22, 33, 20, 0, 0, true), null));

        when(mWifiNS.getFilteredScanDetailsForCarrierUnsavedNetworks(any()))
                .thenReturn(expectedCarrierNetworks);

        // Set WiFi to disconnected state to trigger PNO scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        verify(mCarrierNetworkNotifier).handleScanResults(expectedCarrierNetworks);
    }

    /**
     * {@link CarrierNetworkNotifier} does not handle scan results on network selection if carrier
     * encryption info is not available.
     *
     * Expected behavior: CarrierNetworkNotifier does not handle scan results
     */
    @Test
    public void whenNoEncryptionInfoAvailable_CarrierNetworkNotifierDoesNotHandleScanResults() {
        // no connection candidate selected
        when(mWifiNS.selectNetwork(anyObject(), anyObject(), anyObject(), anyBoolean(),
                anyBoolean(), anyBoolean())).thenReturn(null);

        List<ScanDetail> expectedCarrierNetworks = new ArrayList<>();
        expectedCarrierNetworks.add(
                new ScanDetail(
                        new ScanResult(WifiSsid.createFromAsciiEncoded(CANDIDATE_SSID),
                                CANDIDATE_SSID, CANDIDATE_BSSID, 1245, 0, "[EAP][ESS]", -78, 2450,
                                1025, 22, 33, 20, 0, 0, true), null));

        when(mWifiNS.getFilteredScanDetailsForCarrierUnsavedNetworks(any()))
                .thenReturn(expectedCarrierNetworks);
        when(mCarrierNetworkConfig.isCarrierEncryptionInfoAvailable()).thenReturn(false);

        // Set WiFi to disconnected state to trigger PNO scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        verify(mCarrierNetworkNotifier, never()).handleScanResults(expectedCarrierNetworks);
    }

    /**
     * When wifi is connected, {@link CarrierNetworkNotifier} handles the Wi-Fi connected behavior.
     *
     * Expected behavior: CarrierNetworkNotifier handles connected behavior
     */
    @Test
    public void wifiConnected_carrierNetworkNotifierHandlesConnection() {
        // Set WiFi to connected state
        mWifiInfo.setSSID(WifiSsid.createFromAsciiEncoded(CANDIDATE_SSID));
        mWifiConnectivityManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_NONE);
        verify(mCarrierNetworkNotifier).handleWifiConnected(CANDIDATE_SSID);
    }

    /**
     * When wifi is connected, {@link CarrierNetworkNotifier} handles connection state
     * change.
     *
     * Expected behavior: CarrierNetworkNotifer does not clear pending notification.
     */
    @Test
    public void wifiDisconnected_carrierNetworkNotifierDoesNotClearPendingNotification() {
        // Set WiFi to disconnected state
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        verify(mCarrierNetworkNotifier, never()).clearPendingNotification(anyBoolean());
    }

    /**
     * When a Wi-Fi connection attempt ends, {@link CarrierNetworkNotifier} handles the connection
     * failure. A failure code that is not {@link WifiMetrics.ConnectionEvent#FAILURE_NONE}
     * represents a connection failure.
     *
     * Expected behavior: CarrierNetworkNotifier handles connection failure.
     */
    @Test
    public void wifiConnectionEndsWithFailure_carrierNetworkNotifierHandlesConnectionFailure() {
        mWifiConnectivityManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_CONNECT_NETWORK_FAILED);

        verify(mCarrierNetworkNotifier).handleConnectionFailure();
    }

    /**
     * When a Wi-Fi connection attempt ends, {@link CarrierNetworkNotifier} does not handle
     * connection failure after a successful connection.
     * {@link WifiMetrics.ConnectionEvent#FAILURE_NONE} represents a successful connection.
     *
     * Expected behavior: CarrierNetworkNotifier does nothing.
     */
    @Test
    public void
            wifiConnectionEndsWithSuccess_carrierNetworkNotifierDoesNotHandleConnectionFailure() {
        mWifiConnectivityManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_NONE);

        verify(mCarrierNetworkNotifier, never()).handleConnectionFailure();
    }

    /**
     * When Wi-Fi is disabled, clear the pending notification and reset notification repeat delay.
     *
     * Expected behavior: clear pending notification and reset notification repeat delay
     * */
    @Test
    public void carrierNetworkNotifierClearsPendingNotificationOnWifiDisabled() {
        mWifiConnectivityManager.setWifiEnabled(false);

        verify(mCarrierNetworkNotifier).clearPendingNotification(true /* resetRepeatDelay */);
    }

    /**
     * Verify that the CarrierNetworkNotifier tracks screen state changes.
     */
    @Test
    public void carrierNetworkNotifierTracksScreenStateChanges() {
        mWifiConnectivityManager.handleScreenStateChanged(false);

        verify(mCarrierNetworkNotifier).handleScreenStateChanged(false);

        mWifiConnectivityManager.handleScreenStateChanged(true);

        verify(mCarrierNetworkNotifier).handleScreenStateChanged(true);
    }

    /**
     *  Verify that scan interval for screen on and wifi disconnected scenario
     *  is in the exponential backoff fashion.
     *
     * Expected behavior: WifiConnectivityManager doubles periodic
     * scan interval.
     */
    @Test
    public void checkPeriodicScanIntervalWhenDisconnected() {
        long currentTimeStamp = CURRENT_SYSTEM_TIME_MS;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);

        // Set screen to ON
        mWifiConnectivityManager.handleScreenStateChanged(true);

        // Wait for MAX_PERIODIC_SCAN_INTERVAL_MS so that any impact triggered
        // by screen state change can settle
        currentTimeStamp += WifiConnectivityManager.MAX_PERIODIC_SCAN_INTERVAL_MS;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);

        // Set WiFi to disconnected state to trigger periodic scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        // Get the first periodic scan interval
        long firstIntervalMs = mAlarmManager
                .getTriggerTimeMillis(WifiConnectivityManager.PERIODIC_SCAN_TIMER_TAG)
                - currentTimeStamp;
        assertEquals(firstIntervalMs, WifiConnectivityManager.PERIODIC_SCAN_INTERVAL_MS);

        currentTimeStamp += firstIntervalMs;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);

        // Now fire the first periodic scan alarm timer
        mAlarmManager.dispatch(WifiConnectivityManager.PERIODIC_SCAN_TIMER_TAG);
        mLooper.dispatchAll();

        // Get the second periodic scan interval
        long secondIntervalMs = mAlarmManager
                .getTriggerTimeMillis(WifiConnectivityManager.PERIODIC_SCAN_TIMER_TAG)
                - currentTimeStamp;

        // Verify the intervals are exponential back off
        assertEquals(firstIntervalMs * 2, secondIntervalMs);

        currentTimeStamp += secondIntervalMs;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);

        // Make sure we eventually stay at the maximum scan interval.
        long intervalMs = 0;
        for (int i = 0; i < 5; i++) {
            mAlarmManager.dispatch(WifiConnectivityManager.PERIODIC_SCAN_TIMER_TAG);
            mLooper.dispatchAll();
            intervalMs = mAlarmManager
                    .getTriggerTimeMillis(WifiConnectivityManager.PERIODIC_SCAN_TIMER_TAG)
                    - currentTimeStamp;
            currentTimeStamp += intervalMs;
            when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);
        }

        assertEquals(intervalMs, WifiConnectivityManager.MAX_PERIODIC_SCAN_INTERVAL_MS);
    }

    /**
     *  Verify that scan interval for screen on and wifi connected scenario
     *  is in the exponential backoff fashion.
     *
     * Expected behavior: WifiConnectivityManager doubles periodic
     * scan interval.
     */
    @Test
    public void checkPeriodicScanIntervalWhenConnected() {
        long currentTimeStamp = CURRENT_SYSTEM_TIME_MS;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);

        // Set screen to ON
        mWifiConnectivityManager.handleScreenStateChanged(true);

        // Wait for MAX_PERIODIC_SCAN_INTERVAL_MS so that any impact triggered
        // by screen state change can settle
        currentTimeStamp += WifiConnectivityManager.MAX_PERIODIC_SCAN_INTERVAL_MS;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);

        // Set WiFi to connected state to trigger periodic scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_CONNECTED);

        // Get the first periodic scan interval
        long firstIntervalMs = mAlarmManager
                .getTriggerTimeMillis(WifiConnectivityManager.PERIODIC_SCAN_TIMER_TAG)
                - currentTimeStamp;
        assertEquals(firstIntervalMs, WifiConnectivityManager.PERIODIC_SCAN_INTERVAL_MS);

        currentTimeStamp += firstIntervalMs;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);

        // Now fire the first periodic scan alarm timer
        mAlarmManager.dispatch(WifiConnectivityManager.PERIODIC_SCAN_TIMER_TAG);
        mLooper.dispatchAll();

        // Get the second periodic scan interval
        long secondIntervalMs = mAlarmManager
                .getTriggerTimeMillis(WifiConnectivityManager.PERIODIC_SCAN_TIMER_TAG)
                - currentTimeStamp;

        // Verify the intervals are exponential back off
        assertEquals(firstIntervalMs * 2, secondIntervalMs);

        currentTimeStamp += secondIntervalMs;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);

        // Make sure we eventually stay at the maximum scan interval.
        long intervalMs = 0;
        for (int i = 0; i < 5; i++) {
            mAlarmManager.dispatch(WifiConnectivityManager.PERIODIC_SCAN_TIMER_TAG);
            mLooper.dispatchAll();
            intervalMs = mAlarmManager
                    .getTriggerTimeMillis(WifiConnectivityManager.PERIODIC_SCAN_TIMER_TAG)
                    - currentTimeStamp;
            currentTimeStamp += intervalMs;
            when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);
        }

        assertEquals(intervalMs, WifiConnectivityManager.MAX_PERIODIC_SCAN_INTERVAL_MS);
    }

    /**
     *  When screen on trigger a disconnected state change event then a connected state
     *  change event back to back to verify that the minium scan interval is enforced.
     *
     * Expected behavior: WifiConnectivityManager start the second periodic single
     * scan PERIODIC_SCAN_INTERVAL_MS after the first one.
     */
    @Test
    public void checkMinimumPeriodicScanIntervalWhenScreenOnAndConnected() {
        long currentTimeStamp = CURRENT_SYSTEM_TIME_MS;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);

        // Set screen to ON
        mWifiConnectivityManager.handleScreenStateChanged(true);

        // Wait for MAX_PERIODIC_SCAN_INTERVAL_MS so that any impact triggered
        // by screen state change can settle
        currentTimeStamp += WifiConnectivityManager.MAX_PERIODIC_SCAN_INTERVAL_MS;
        long scanForDisconnectedTimeStamp = currentTimeStamp;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);

        // Set WiFi to disconnected state which triggers a scan immediately
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);
        verify(mWifiScanner, times(1)).startScan(anyObject(), anyObject(), anyObject());

        // Set up time stamp for when entering CONNECTED state
        currentTimeStamp += 2000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);

        // Set WiFi to connected state to trigger its periodic scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_CONNECTED);

        // The very first scan triggered for connected state is actually via the alarm timer
        // and it obeys the minimum scan interval
        long firstScanForConnectedTimeStamp = mAlarmManager
                .getTriggerTimeMillis(WifiConnectivityManager.PERIODIC_SCAN_TIMER_TAG);

        // Verify that the first scan for connected state is scheduled PERIODIC_SCAN_INTERVAL_MS
        // after the scan for disconnected state
        assertEquals(firstScanForConnectedTimeStamp, scanForDisconnectedTimeStamp
                + WifiConnectivityManager.PERIODIC_SCAN_INTERVAL_MS);
    }

    /**
     *  When screen on trigger a connected state change event then a disconnected state
     *  change event back to back to verify that a scan is fired immediately for the
     *  disconnected state change event.
     *
     * Expected behavior: WifiConnectivityManager directly starts the periodic immediately
     * for the disconnected state change event. The second scan for disconnected state is
     * via alarm timer.
     */
    @Test
    public void scanImmediatelyWhenScreenOnAndDisconnected() {
        long currentTimeStamp = CURRENT_SYSTEM_TIME_MS;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);

        // Set screen to ON
        mWifiConnectivityManager.handleScreenStateChanged(true);

        // Wait for MAX_PERIODIC_SCAN_INTERVAL_MS so that any impact triggered
        // by screen state change can settle
        currentTimeStamp += WifiConnectivityManager.MAX_PERIODIC_SCAN_INTERVAL_MS;
        long scanForConnectedTimeStamp = currentTimeStamp;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);

        // Set WiFi to connected state to trigger the periodic scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_CONNECTED);
        verify(mWifiScanner, times(1)).startScan(anyObject(), anyObject(), anyObject());

        // Set up the time stamp for when entering DISCONNECTED state
        currentTimeStamp += 2000;
        long enteringDisconnectedStateTimeStamp = currentTimeStamp;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);

        // Set WiFi to disconnected state to trigger its periodic scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        // Verify the very first scan for DISCONNECTED state is fired immediately
        verify(mWifiScanner, times(2)).startScan(anyObject(), anyObject(), anyObject());
        long secondScanForDisconnectedTimeStamp = mAlarmManager
                .getTriggerTimeMillis(WifiConnectivityManager.PERIODIC_SCAN_TIMER_TAG);

        // Verify that the second scan is scheduled PERIODIC_SCAN_INTERVAL_MS after
        // entering DISCONNECTED state.
        assertEquals(secondScanForDisconnectedTimeStamp, enteringDisconnectedStateTimeStamp
                + WifiConnectivityManager.PERIODIC_SCAN_INTERVAL_MS);
    }

    /**
     *  When screen on trigger a connection state change event and a forced connectivity
     *  scan event back to back to verify that the minimum scan interval is not applied
     *  in this scenario.
     *
     * Expected behavior: WifiConnectivityManager starts the second periodic single
     * scan immediately.
     */
    @Test
    public void checkMinimumPeriodicScanIntervalNotEnforced() {
        long currentTimeStamp = CURRENT_SYSTEM_TIME_MS;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);

        // Set screen to ON
        mWifiConnectivityManager.handleScreenStateChanged(true);

        // Wait for MAX_PERIODIC_SCAN_INTERVAL_MS so that any impact triggered
        // by screen state change can settle
        currentTimeStamp += WifiConnectivityManager.MAX_PERIODIC_SCAN_INTERVAL_MS;
        long firstScanTimeStamp = currentTimeStamp;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);

        // Set WiFi to connected state to trigger the periodic scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_CONNECTED);

        // Set the second scan attempt time stamp
        currentTimeStamp += 2000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);

        // Allow untrusted networks so WifiConnectivityManager starts a periodic scan
        // immediately.
        mWifiConnectivityManager.setUntrustedConnectionAllowed(true);

        // Get the second periodic scan actual time stamp. Note, this scan is not
        // started from the AlarmManager.
        long secondScanTimeStamp = mWifiConnectivityManager.getLastPeriodicSingleScanTimeStamp();

        // Verify that the second scan is fired immediately
        assertEquals(secondScanTimeStamp, currentTimeStamp);
    }

    /**
     * Verify that we perform full band scan when the currently connected network's tx/rx success
     * rate is low.
     *
     * Expected behavior: WifiConnectivityManager does full band scan.
     */
    @Test
    public void checkSingleScanSettingsWhenConnectedWithLowDataRate() {
        mWifiInfo.txSuccessRate = 0;
        mWifiInfo.rxSuccessRate = 0;

        final HashSet<Integer> channelList = new HashSet<>();
        channelList.add(1);
        channelList.add(2);
        channelList.add(3);

        when(mClientModeImpl.getCurrentWifiConfiguration())
                .thenReturn(new WifiConfiguration());
        when(mWifiConfigManager.fetchChannelSetForNetworkForPartialScan(anyInt(), anyLong(),
                anyInt())).thenReturn(channelList);

        doAnswer(new AnswerWithArguments() {
            public void answer(ScanSettings settings, ScanListener listener,
                    WorkSource workSource) throws Exception {
                assertEquals(settings.band, WifiScanner.WIFI_BAND_BOTH_WITH_DFS);
                assertNull(settings.channels);
            }}).when(mWifiScanner).startScan(anyObject(), anyObject(), anyObject());

        // Set screen to ON
        mWifiConnectivityManager.handleScreenStateChanged(true);

        // Set WiFi to connected state to trigger periodic scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_CONNECTED);

        verify(mWifiScanner).startScan(anyObject(), anyObject(), anyObject());
    }

    /**
     * Verify that we perform partial scan when the currently connected network's tx/rx success
     * rate is high and when the currently connected network is present in scan
     * cache in WifiConfigManager.
     * WifiConnectivityManager does partial scan only when firmware roaming is not supported.
     *
     * Expected behavior: WifiConnectivityManager does partial scan.
     */
    @Test
    public void checkPartialScanRequestedWithHighDataRateWithoutFwRoaming() {
        mWifiInfo.txSuccessRate = mFullScanMaxTxPacketRate * 2;
        mWifiInfo.rxSuccessRate = mFullScanMaxRxPacketRate * 2;

        final HashSet<Integer> channelList = new HashSet<>();
        channelList.add(1);
        channelList.add(2);
        channelList.add(3);

        when(mClientModeImpl.getCurrentWifiConfiguration())
                .thenReturn(new WifiConfiguration());
        when(mWifiConfigManager.fetchChannelSetForNetworkForPartialScan(anyInt(), anyLong(),
                anyInt())).thenReturn(channelList);
        when(mWifiConnectivityHelper.isFirmwareRoamingSupported()).thenReturn(false);

        doAnswer(new AnswerWithArguments() {
            public void answer(ScanSettings settings, ScanListener listener,
                    WorkSource workSource) throws Exception {
                assertEquals(settings.band, WifiScanner.WIFI_BAND_UNSPECIFIED);
                assertEquals(settings.channels.length, channelList.size());
                for (int chanIdx = 0; chanIdx < settings.channels.length; chanIdx++) {
                    assertTrue(channelList.contains(settings.channels[chanIdx].frequency));
                }
            }}).when(mWifiScanner).startScan(anyObject(), anyObject(), anyObject());

        // Set screen to ON
        mWifiConnectivityManager.handleScreenStateChanged(true);

        // Set WiFi to connected state to trigger periodic scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_CONNECTED);

        verify(mWifiScanner).startScan(anyObject(), anyObject(), anyObject());
    }

    /**
     * Verify that we skip the partial scan when:
     * 1. The currently connected network's tx/rx success rate is high.
     * 2. When the currently connected network is present in scan
     * cache in WifiConfigManager.
     * 3. When firmware roaming is supported.
     * Expected behavior: WifiConnectivityManager does no scan, but periodic scans
     * are still scheduled.
     */
    @Test
    public void checkPartialScanSkippedWithHighDataRateWithFwRoaming() {
        mWifiInfo.txSuccessRate = mFullScanMaxTxPacketRate * 2;
        mWifiInfo.rxSuccessRate = mFullScanMaxRxPacketRate * 2;

        long currentTimeStamp = CURRENT_SYSTEM_TIME_MS;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeStamp);

        final HashSet<Integer> channelList = new HashSet<>();
        channelList.add(1);
        channelList.add(2);
        channelList.add(3);

        when(mClientModeImpl.getCurrentWifiConfiguration())
                .thenReturn(new WifiConfiguration());
        when(mWifiConfigManager.fetchChannelSetForNetworkForPartialScan(anyInt(), anyLong(),
                anyInt())).thenReturn(channelList);
        // No scan will be requested when firmware roaming control is not supported.
        when(mWifiConnectivityHelper.isFirmwareRoamingSupported()).thenReturn(true);

        // Set screen to ON
        mWifiConnectivityManager.handleScreenStateChanged(true);

        // Set WiFi to connected state to trigger periodic scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_CONNECTED);

        verify(mWifiScanner, never()).startScan(anyObject(), anyObject(), anyObject());

        // Get the first periodic scan interval to check that we are still scheduling
        // periodic scans.
        long firstIntervalMs = mAlarmManager
                .getTriggerTimeMillis(WifiConnectivityManager.PERIODIC_SCAN_TIMER_TAG)
                - currentTimeStamp;
        assertEquals(firstIntervalMs, WifiConnectivityManager.PERIODIC_SCAN_INTERVAL_MS);
    }

    /**
     * Verify that we fall back to full band scan when the currently connected network's tx/rx
     * success rate is high and the currently connected network is not present in scan cache in
     * WifiConfigManager. This is simulated by returning an empty hashset in |makeChannelList|.
     *
     * Expected behavior: WifiConnectivityManager does full band scan.
     */
    @Test
    public void checkSingleScanSettingsWhenConnectedWithHighDataRateNotInCache() {
        mWifiInfo.txSuccessRate = mFullScanMaxTxPacketRate * 2;
        mWifiInfo.rxSuccessRate = mFullScanMaxRxPacketRate * 2;

        final HashSet<Integer> channelList = new HashSet<>();

        when(mClientModeImpl.getCurrentWifiConfiguration())
                .thenReturn(new WifiConfiguration());
        when(mWifiConfigManager.fetchChannelSetForNetworkForPartialScan(anyInt(), anyLong(),
                anyInt())).thenReturn(channelList);

        doAnswer(new AnswerWithArguments() {
            public void answer(ScanSettings settings, ScanListener listener,
                    WorkSource workSource) throws Exception {
                assertEquals(settings.band, WifiScanner.WIFI_BAND_BOTH_WITH_DFS);
                assertNull(settings.channels);
            }}).when(mWifiScanner).startScan(anyObject(), anyObject(), anyObject());

        // Set screen to ON
        mWifiConnectivityManager.handleScreenStateChanged(true);

        // Set WiFi to connected state to trigger periodic scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_CONNECTED);

        verify(mWifiScanner).startScan(anyObject(), anyObject(), anyObject());
    }

    /**
     *  Verify that we retry connectivity scan up to MAX_SCAN_RESTART_ALLOWED times
     *  when Wifi somehow gets into a bad state and fails to scan.
     *
     * Expected behavior: WifiConnectivityManager schedules connectivity scan
     * MAX_SCAN_RESTART_ALLOWED times.
     */
    @Test
    public void checkMaximumScanRetry() {
        // Set screen to ON
        mWifiConnectivityManager.handleScreenStateChanged(true);

        doAnswer(new AnswerWithArguments() {
            public void answer(ScanSettings settings, ScanListener listener,
                    WorkSource workSource) throws Exception {
                listener.onFailure(-1, "ScanFailure");
            }}).when(mWifiScanner).startScan(anyObject(), anyObject(), anyObject());

        // Set WiFi to disconnected state to trigger the single scan based periodic scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        // Fire the alarm timer 2x timers
        for (int i = 0; i < (WifiConnectivityManager.MAX_SCAN_RESTART_ALLOWED * 2); i++) {
            mAlarmManager.dispatch(WifiConnectivityManager.RESTART_SINGLE_SCAN_TIMER_TAG);
            mLooper.dispatchAll();
        }

        // Verify that the connectivity scan has been retried for MAX_SCAN_RESTART_ALLOWED
        // times. Note, WifiScanner.startScan() is invoked MAX_SCAN_RESTART_ALLOWED + 1 times.
        // The very first scan is the initial one, and the other MAX_SCAN_RESTART_ALLOWED
        // are the retrial ones.
        verify(mWifiScanner, times(WifiConnectivityManager.MAX_SCAN_RESTART_ALLOWED + 1)).startScan(
                anyObject(), anyObject(), anyObject());
    }

    /**
     * Verify that a successful scan result resets scan retry counter
     *
     * Steps
     * 1. Trigger a scan that fails
     * 2. Let the retry succeed
     * 3. Trigger a scan again and have it and all subsequent retries fail
     * 4. Verify that there are MAX_SCAN_RESTART_ALLOWED + 3 startScan calls. (2 are from the
     * original scans, and MAX_SCAN_RESTART_ALLOWED + 1 from retries)
     */
    @Test
    public void verifyScanFailureCountIsResetAfterOnResult() {
        // Setup WifiScanner to fail
        doAnswer(new AnswerWithArguments() {
            public void answer(ScanSettings settings, ScanListener listener,
                    WorkSource workSource) throws Exception {
                listener.onFailure(-1, "ScanFailure");
            }}).when(mWifiScanner).startScan(anyObject(), anyObject(), anyObject());

        mWifiConnectivityManager.forceConnectivityScan(null);
        // make the retry succeed
        doAnswer(new AnswerWithArguments() {
            public void answer(ScanSettings settings, ScanListener listener,
                    WorkSource workSource) throws Exception {
                listener.onResults(null);
            }}).when(mWifiScanner).startScan(anyObject(), anyObject(), anyObject());
        mAlarmManager.dispatch(WifiConnectivityManager.RESTART_SINGLE_SCAN_TIMER_TAG);
        mLooper.dispatchAll();

        // Verify that startScan is called once for the original scan, plus once for the retry.
        // The successful retry should have now cleared the restart count
        verify(mWifiScanner, times(2)).startScan(anyObject(), anyObject(), anyObject());

        // Now force a new scan and verify we retry MAX_SCAN_RESTART_ALLOWED times
        doAnswer(new AnswerWithArguments() {
            public void answer(ScanSettings settings, ScanListener listener,
                    WorkSource workSource) throws Exception {
                listener.onFailure(-1, "ScanFailure");
            }}).when(mWifiScanner).startScan(anyObject(), anyObject(), anyObject());
        mWifiConnectivityManager.forceConnectivityScan(null);
        // Fire the alarm timer 2x timers
        for (int i = 0; i < (WifiConnectivityManager.MAX_SCAN_RESTART_ALLOWED * 2); i++) {
            mAlarmManager.dispatch(WifiConnectivityManager.RESTART_SINGLE_SCAN_TIMER_TAG);
            mLooper.dispatchAll();
        }

        // Verify that the connectivity scan has been retried for MAX_SCAN_RESTART_ALLOWED + 3
        // times. Note, WifiScanner.startScan() is invoked 2 times by the first part of this test,
        // and additionally MAX_SCAN_RESTART_ALLOWED + 1 times from forceConnectivityScan and
        // subsequent retries.
        verify(mWifiScanner, times(WifiConnectivityManager.MAX_SCAN_RESTART_ALLOWED + 3)).startScan(
                anyObject(), anyObject(), anyObject());
    }

    /**
     * Listen to scan results not requested by WifiConnectivityManager and
     * act on them.
     *
     * Expected behavior: WifiConnectivityManager calls
     * ClientModeImpl.startConnectToNetwork() with the
     * expected candidate network ID and BSSID.
     */
    @Test
    public void listenToAllSingleScanResults() {
        ScanSettings settings = new ScanSettings();
        ScanListener scanListener = mock(ScanListener.class);

        // Request a single scan outside of WifiConnectivityManager.
        mWifiScanner.startScan(settings, scanListener, WIFI_WORK_SOURCE);

        // Verify that WCM receives the scan results and initiates a connection
        // to the network.
        verify(mClientModeImpl).startConnectToNetwork(
                CANDIDATE_NETWORK_ID, Process.WIFI_UID, CANDIDATE_BSSID);
    }

    /**
     *  Verify that a forced connectivity scan waits for full band scan
     *  results.
     *
     * Expected behavior: WifiConnectivityManager doesn't invoke
     * ClientModeImpl.startConnectToNetwork() when full band scan
     * results are not available.
     */
    @Test
    public void waitForFullBandScanResults() {
        // Set WiFi to connected state.
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_CONNECTED);

        // Set up as partial scan results.
        when(mScanData.getBandScanned()).thenReturn(WifiScanner.WIFI_BAND_5_GHZ);

        // Force a connectivity scan which enables WifiConnectivityManager
        // to wait for full band scan results.
        mWifiConnectivityManager.forceConnectivityScan(WIFI_WORK_SOURCE);

        // No roaming because no full band scan results.
        verify(mClientModeImpl, times(0)).startConnectToNetwork(
                CANDIDATE_NETWORK_ID, Process.WIFI_UID, CANDIDATE_BSSID);

        // Set up as full band scan results.
        when(mScanData.getBandScanned()).thenReturn(WifiScanner.WIFI_BAND_BOTH_WITH_DFS);

        // Force a connectivity scan which enables WifiConnectivityManager
        // to wait for full band scan results.
        mWifiConnectivityManager.forceConnectivityScan(WIFI_WORK_SOURCE);

        // Roaming attempt because full band scan results are available.
        verify(mClientModeImpl).startConnectToNetwork(
                CANDIDATE_NETWORK_ID, Process.WIFI_UID, CANDIDATE_BSSID);
    }

    /**
     *  Verify the BSSID blacklist implementation.
     *
     * Expected behavior: A BSSID gets blacklisted after being disabled
     * for 3 times, and becomes available after being re-enabled. Firmware
     * controlled roaming is supported, its roaming configuration needs to be
     * updated as well.
     */
    @Test
    public void blacklistAndReenableBssid() {
        String bssid = "6c:f3:7f:ae:8c:f3";

        when(mWifiConnectivityHelper.isFirmwareRoamingSupported()).thenReturn(true);
        // Verify that a BSSID gets blacklisted only after being disabled
        // for BSSID_BLACKLIST_THRESHOLD times for reasons other than
        // REASON_CODE_AP_UNABLE_TO_HANDLE_NEW_STA.
        for (int i = 0; i < WifiConnectivityManager.BSSID_BLACKLIST_THRESHOLD; i++) {
            assertFalse(mWifiConnectivityManager.isBssidDisabled(bssid));
            mWifiConnectivityManager.trackBssid(bssid, false, 1);
        }

        // Verify the BSSID is now blacklisted.
        assertTrue(mWifiConnectivityManager.isBssidDisabled(bssid));
        // Verify the BSSID gets sent to firmware.
        verify(mWifiConnectivityHelper).setFirmwareRoamingConfiguration(
                mBssidBlacklistCaptor.capture(), mSsidWhitelistCaptor.capture());
        assertTrue(mBssidBlacklistCaptor.getValue().contains(bssid));
        assertTrue(mSsidWhitelistCaptor.getValue().isEmpty());

        // Re-enable the bssid.
        mWifiConnectivityManager.trackBssid(bssid, true, 1);

        // Verify the bssid is no longer blacklisted.
        assertFalse(mWifiConnectivityManager.isBssidDisabled(bssid));
        // Verify the BSSID gets cleared from firmware.
        verify(mWifiConnectivityHelper, times(2)).setFirmwareRoamingConfiguration(
                mBssidBlacklistCaptor.capture(), mSsidWhitelistCaptor.capture());
        assertFalse(mBssidBlacklistCaptor.getValue().contains(bssid));
        assertTrue(mSsidWhitelistCaptor.getValue().isEmpty());
    }

    /**
     *  Verify that a network gets blacklisted immediately if it is unable
     *  to handle new stations.
     */
    @Test
    public void blacklistNetworkImmediatelyIfApHasNoCapacityForNewStation() {
        String bssid = "6c:f3:7f:ae:8c:f3";

        when(mWifiConnectivityHelper.isFirmwareRoamingSupported()).thenReturn(true);
        // Blacklist the BSSID
        mWifiConnectivityManager.trackBssid(bssid, false,
                WifiConnectivityManager.REASON_CODE_AP_UNABLE_TO_HANDLE_NEW_STA);

        // Verify the BSSID is now blacklisted.
        assertTrue(mWifiConnectivityManager.isBssidDisabled(bssid));
        // Verify the BSSID gets sent to firmware.
        verify(mWifiConnectivityHelper).setFirmwareRoamingConfiguration(
                mBssidBlacklistCaptor.capture(), mSsidWhitelistCaptor.capture());
        assertTrue(mBssidBlacklistCaptor.getValue().contains(bssid));
        assertTrue(mSsidWhitelistCaptor.getValue().isEmpty());
    }

    /**
     *  Verify that a blacklisted BSSID becomes available only after
     *  BSSID_BLACKLIST_EXPIRE_TIME_MS.
     */
    @Test
    public void verifyBlacklistRefreshedAfterScanResults() {
        String bssid = "6c:f3:7f:ae:8c:f3";

        when(mWifiConnectivityHelper.isFirmwareRoamingSupported()).thenReturn(true);
        // Blacklist the BSSID.
        mWifiConnectivityManager.trackBssid(bssid, false,
                WifiConnectivityManager.REASON_CODE_AP_UNABLE_TO_HANDLE_NEW_STA);

        // Verify the BSSID is now blacklisted.
        assertTrue(mWifiConnectivityManager.isBssidDisabled(bssid));
        // Verify the BSSID gets sent to firmware.
        verify(mWifiConnectivityHelper).setFirmwareRoamingConfiguration(
                mBssidBlacklistCaptor.capture(), mSsidWhitelistCaptor.capture());
        assertTrue(mBssidBlacklistCaptor.getValue().contains(bssid));
        assertTrue(mSsidWhitelistCaptor.getValue().isEmpty());

        // Force a connectivity scan in less than BSSID_BLACKLIST_EXPIRE_TIME_MS.
        // Arrival of scan results will trigger WifiConnectivityManager to refresh its
        // BSSID blacklist. Verify that the blacklisted BSSId is not freed because
        // its blacklist expiration time hasn't reached yet.
        when(mClock.getElapsedSinceBootMillis()).thenReturn(SystemClock.elapsedRealtime()
                + WifiConnectivityManager.BSSID_BLACKLIST_EXPIRE_TIME_MS / 2);
        mWifiConnectivityManager.forceConnectivityScan(WIFI_WORK_SOURCE);
        assertTrue(mWifiConnectivityManager.isBssidDisabled(bssid));

        // Force another connectivity scan at BSSID_BLACKLIST_EXPIRE_TIME_MS from when the
        // BSSID was blacklisted. Verify that the blacklisted BSSId is freed.
        when(mClock.getElapsedSinceBootMillis()).thenReturn(SystemClock.elapsedRealtime()
                + WifiConnectivityManager.BSSID_BLACKLIST_EXPIRE_TIME_MS);
        mWifiConnectivityManager.forceConnectivityScan(WIFI_WORK_SOURCE);

        // Verify the BSSID is no longer blacklisted.
        assertFalse(mWifiConnectivityManager.isBssidDisabled(bssid));
        // Verify the BSSID gets cleared from firmware.
        verify(mWifiConnectivityHelper, times(2)).setFirmwareRoamingConfiguration(
                mBssidBlacklistCaptor.capture(), mSsidWhitelistCaptor.capture());
        assertFalse(mBssidBlacklistCaptor.getValue().contains(bssid));
        assertTrue(mSsidWhitelistCaptor.getValue().isEmpty());
    }

    /**
     *  Verify that BSSID blacklist gets cleared when exiting Wifi client mode.
     */
    @Test
    public void clearBssidBlacklistWhenExitingWifiClientMode() {
        String bssid = "6c:f3:7f:ae:8c:f3";

        when(mWifiConnectivityHelper.isFirmwareRoamingSupported()).thenReturn(true);

        // Blacklist the BSSID.
        mWifiConnectivityManager.trackBssid(bssid, false,
                WifiConnectivityManager.REASON_CODE_AP_UNABLE_TO_HANDLE_NEW_STA);

        // Verify the BSSID is now blacklisted.
        assertTrue(mWifiConnectivityManager.isBssidDisabled(bssid));
        // Verify the BSSID gets sent to firmware.
        verify(mWifiConnectivityHelper).setFirmwareRoamingConfiguration(
                mBssidBlacklistCaptor.capture(), mSsidWhitelistCaptor.capture());
        assertTrue(mBssidBlacklistCaptor.getValue().contains(bssid));
        assertTrue(mSsidWhitelistCaptor.getValue().isEmpty());

        // Exit Wifi client mode.
        mWifiConnectivityManager.setWifiEnabled(false);

        // Verify the BSSID blacklist is empty.
        assertFalse(mWifiConnectivityManager.isBssidDisabled(bssid));
        verify(mWifiConnectivityHelper, times(2)).setFirmwareRoamingConfiguration(
                mBssidBlacklistCaptor.capture(), mSsidWhitelistCaptor.capture());
        assertTrue(mBssidBlacklistCaptor.getValue().isEmpty());
        assertTrue(mSsidWhitelistCaptor.getValue().isEmpty());
    }

    /**
     *  Verify that BSSID blacklist gets cleared when preparing for a forced connection
     *  initiated by user/app.
     */
    @Test
    public void clearBssidBlacklistWhenPreparingForForcedConnection() {
        String bssid = "6c:f3:7f:ae:8c:f3";

        when(mWifiConnectivityHelper.isFirmwareRoamingSupported()).thenReturn(true);

        // Blacklist the BSSID.
        mWifiConnectivityManager.trackBssid(bssid, false,
                WifiConnectivityManager.REASON_CODE_AP_UNABLE_TO_HANDLE_NEW_STA);

        // Verify the BSSID is now blacklisted.
        assertTrue(mWifiConnectivityManager.isBssidDisabled(bssid));
        // Verify the BSSID gets sent to firmware.
        verify(mWifiConnectivityHelper).setFirmwareRoamingConfiguration(
                mBssidBlacklistCaptor.capture(), mSsidWhitelistCaptor.capture());
        assertTrue(mBssidBlacklistCaptor.getValue().contains(bssid));
        assertTrue(mSsidWhitelistCaptor.getValue().isEmpty());

        // Prepare for a forced connection attempt.
        mWifiConnectivityManager.prepareForForcedConnection(1);

        // Verify the BSSID blacklist is empty.
        assertFalse(mWifiConnectivityManager.isBssidDisabled(bssid));
        verify(mWifiConnectivityHelper, times(2)).setFirmwareRoamingConfiguration(
                mBssidBlacklistCaptor.capture(), mSsidWhitelistCaptor.capture());
        assertTrue(mBssidBlacklistCaptor.getValue().isEmpty());
        assertTrue(mSsidWhitelistCaptor.getValue().isEmpty());
    }

    /**
    /**
     *  Verify that BSSID blacklist gets trimmed down to fit firmware capability.
     */
    @Test
    public void trimDownBssidBlacklistForFirmware() {
        when(mWifiConnectivityHelper.isFirmwareRoamingSupported()).thenReturn(true);

        // Blacklist more than MAX_BSSID_BLACKLIST_SIZE BSSIDs.
        for (int i = 0; i < MAX_BSSID_BLACKLIST_SIZE + 6; i++) {
            StringBuilder bssid = new StringBuilder("55:44:33:22:11:00");
            bssid.setCharAt(16, (char) ('0' + i));
            mWifiConnectivityManager.trackBssid(bssid.toString(), false,
                    WifiConnectivityManager.REASON_CODE_AP_UNABLE_TO_HANDLE_NEW_STA);
            // Verify that up to MAX_BSSID_BLACKLIST_SIZE BSSIDs gets sent to firmware.
            verify(mWifiConnectivityHelper, times(i + 1)).setFirmwareRoamingConfiguration(
                    mBssidBlacklistCaptor.capture(), mSsidWhitelistCaptor.capture());
            assertEquals((i + 1) <  MAX_BSSID_BLACKLIST_SIZE ? (i + 1) : MAX_BSSID_BLACKLIST_SIZE,
                    mBssidBlacklistCaptor.getValue().size());
            assertTrue(mSsidWhitelistCaptor.getValue().isEmpty());
        }
    }

    /**
     * When WifiConnectivityManager is on and Wifi client mode is enabled, framework
     * queries firmware via WifiConnectivityHelper to check if firmware roaming is
     * supported and its capability.
     *
     * Expected behavior: WifiConnectivityManager#setWifiEnabled calls into
     * WifiConnectivityHelper#getFirmwareRoamingInfo
     */
    @Test
    public void verifyGetFirmwareRoamingInfoIsCalledWhenEnableWiFiAndWcmOn() {
        // WifiConnectivityManager is on by default
        mWifiConnectivityManager.setWifiEnabled(true);
        verify(mWifiConnectivityHelper).getFirmwareRoamingInfo();
    }

    /**
     * When WifiConnectivityManager is off,  verify that framework does not
     * query firmware via WifiConnectivityHelper to check if firmware roaming is
     * supported and its capability when enabling Wifi client mode.
     *
     * Expected behavior: WifiConnectivityManager#setWifiEnabled does not call into
     * WifiConnectivityHelper#getFirmwareRoamingInfo
     */
    @Test
    public void verifyGetFirmwareRoamingInfoIsNotCalledWhenEnableWiFiAndWcmOff() {
        reset(mWifiConnectivityHelper);
        mWifiConnectivityManager.enable(false);
        mWifiConnectivityManager.setWifiEnabled(true);
        verify(mWifiConnectivityHelper, times(0)).getFirmwareRoamingInfo();
    }

    /*
     * Firmware supports controlled roaming.
     * Connect to a network which doesn't have a config specified BSSID.
     *
     * Expected behavior: WifiConnectivityManager calls
     * ClientModeImpl.startConnectToNetwork() with the
     * expected candidate network ID, and the BSSID value should be
     * 'any' since firmware controls the roaming.
     */
    @Test
    public void useAnyBssidToConnectWhenFirmwareRoamingOnAndConfigHasNoBssidSpecified() {
        // Firmware controls roaming
        when(mWifiConnectivityHelper.isFirmwareRoamingSupported()).thenReturn(true);

        // Set screen to on
        mWifiConnectivityManager.handleScreenStateChanged(true);

        // Set WiFi to disconnected state
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        verify(mClientModeImpl).startConnectToNetwork(
                CANDIDATE_NETWORK_ID, Process.WIFI_UID, ClientModeImpl.SUPPLICANT_BSSID_ANY);
    }

    /*
     * Firmware supports controlled roaming.
     * Connect to a network which has a config specified BSSID.
     *
     * Expected behavior: WifiConnectivityManager calls
     * ClientModeImpl.startConnectToNetwork() with the
     * expected candidate network ID, and the BSSID value should be
     * the config specified one.
     */
    @Test
    public void useConfigSpecifiedBssidToConnectWhenFirmwareRoamingOn() {
        // Firmware controls roaming
        when(mWifiConnectivityHelper.isFirmwareRoamingSupported()).thenReturn(true);

        // Set up the candidate configuration such that it has a BSSID specified.
        WifiConfiguration candidate = generateWifiConfig(
                0, CANDIDATE_NETWORK_ID, CANDIDATE_SSID, false, true, null, null);
        candidate.BSSID = CANDIDATE_BSSID; // config specified
        ScanResult candidateScanResult = new ScanResult();
        candidateScanResult.SSID = CANDIDATE_SSID;
        candidateScanResult.BSSID = CANDIDATE_BSSID;
        candidate.getNetworkSelectionStatus().setCandidate(candidateScanResult);

        when(mWifiNS.selectNetwork(anyObject(), anyObject(), anyObject(), anyBoolean(),
                anyBoolean(), anyBoolean())).thenReturn(candidate);

        // Set screen to on
        mWifiConnectivityManager.handleScreenStateChanged(true);

        // Set WiFi to disconnected state
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        verify(mClientModeImpl).startConnectToNetwork(
                CANDIDATE_NETWORK_ID, Process.WIFI_UID, CANDIDATE_BSSID);
    }

    /*
     * Firmware does not support controlled roaming.
     * Connect to a network which doesn't have a config specified BSSID.
     *
     * Expected behavior: WifiConnectivityManager calls
     * ClientModeImpl.startConnectToNetwork() with the expected candidate network ID,
     * and the BSSID value should be the candidate scan result specified.
     */
    @Test
    public void useScanResultBssidToConnectWhenFirmwareRoamingOffAndConfigHasNoBssidSpecified() {
        // Set screen to on
        mWifiConnectivityManager.handleScreenStateChanged(true);

        // Set WiFi to disconnected state
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        verify(mClientModeImpl).startConnectToNetwork(
                CANDIDATE_NETWORK_ID, Process.WIFI_UID, CANDIDATE_BSSID);
    }

    /*
     * Firmware does not support controlled roaming.
     * Connect to a network which has a config specified BSSID.
     *
     * Expected behavior: WifiConnectivityManager calls
     * ClientModeImpl.startConnectToNetwork() with the expected candidate network ID,
     * and the BSSID value should be the config specified one.
     */
    @Test
    public void useConfigSpecifiedBssidToConnectionWhenFirmwareRoamingOff() {
        // Set up the candidate configuration such that it has a BSSID specified.
        WifiConfiguration candidate = generateWifiConfig(
                0, CANDIDATE_NETWORK_ID, CANDIDATE_SSID, false, true, null, null);
        candidate.BSSID = CANDIDATE_BSSID; // config specified
        ScanResult candidateScanResult = new ScanResult();
        candidateScanResult.SSID = CANDIDATE_SSID;
        candidateScanResult.BSSID = CANDIDATE_BSSID;
        candidate.getNetworkSelectionStatus().setCandidate(candidateScanResult);

        when(mWifiNS.selectNetwork(anyObject(), anyObject(), anyObject(), anyBoolean(),
                anyBoolean(), anyBoolean())).thenReturn(candidate);

        // Set screen to on
        mWifiConnectivityManager.handleScreenStateChanged(true);

        // Set WiFi to disconnected state
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        verify(mClientModeImpl).startConnectToNetwork(
                CANDIDATE_NETWORK_ID, Process.WIFI_UID, CANDIDATE_BSSID);
    }

    /**
     * Firmware does not support controlled roaming.
     * WiFi in connected state, framework triggers roaming.
     *
     * Expected behavior: WifiConnectivityManager invokes
     * ClientModeImpl.startRoamToNetwork().
     */
    @Test
    public void frameworkInitiatedRoaming() {
        // Mock the currently connected network which has the same networkID and
        // SSID as the one to be selected.
        WifiConfiguration currentNetwork = generateWifiConfig(
                0, CANDIDATE_NETWORK_ID, CANDIDATE_SSID, false, true, null, null);
        when(mWifiConfigManager.getConfiguredNetwork(anyInt())).thenReturn(currentNetwork);

        // Set WiFi to connected state
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_CONNECTED);

        // Set screen to on
        mWifiConnectivityManager.handleScreenStateChanged(true);

        verify(mClientModeImpl).startRoamToNetwork(eq(CANDIDATE_NETWORK_ID),
                mCandidateScanResultCaptor.capture());
        assertEquals(mCandidateScanResultCaptor.getValue().BSSID, CANDIDATE_BSSID);
    }

    /**
     * Firmware supports controlled roaming.
     * WiFi in connected state, framework does not trigger roaming
     * as it's handed off to the firmware.
     *
     * Expected behavior: WifiConnectivityManager doesn't invoke
     * ClientModeImpl.startRoamToNetwork().
     */
    @Test
    public void noFrameworkRoamingIfConnectedAndFirmwareRoamingSupported() {
        // Mock the currently connected network which has the same networkID and
        // SSID as the one to be selected.
        WifiConfiguration currentNetwork = generateWifiConfig(
                0, CANDIDATE_NETWORK_ID, CANDIDATE_SSID, false, true, null, null);
        when(mWifiConfigManager.getConfiguredNetwork(anyInt())).thenReturn(currentNetwork);

        // Firmware controls roaming
        when(mWifiConnectivityHelper.isFirmwareRoamingSupported()).thenReturn(true);

        // Set WiFi to connected state
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_CONNECTED);

        // Set screen to on
        mWifiConnectivityManager.handleScreenStateChanged(true);

        verify(mClientModeImpl, times(0)).startRoamToNetwork(anyInt(), anyObject());
    }

    /*
     * Wifi in disconnected state. Drop the connection attempt if the recommended
     * network configuration has a BSSID specified but the scan result BSSID doesn't
     * match it.
     *
     * Expected behavior: WifiConnectivityManager doesn't invoke
     * ClientModeImpl.startConnectToNetwork().
     */
    @Test
    public void dropConnectAttemptIfConfigSpecifiedBssidDifferentFromScanResultBssid() {
        // Set up the candidate configuration such that it has a BSSID specified.
        WifiConfiguration candidate = generateWifiConfig(
                0, CANDIDATE_NETWORK_ID, CANDIDATE_SSID, false, true, null, null);
        candidate.BSSID = CANDIDATE_BSSID; // config specified
        ScanResult candidateScanResult = new ScanResult();
        candidateScanResult.SSID = CANDIDATE_SSID;
        // Set up the scan result BSSID to be different from the config specified one.
        candidateScanResult.BSSID = INVALID_SCAN_RESULT_BSSID;
        candidate.getNetworkSelectionStatus().setCandidate(candidateScanResult);

        when(mWifiNS.selectNetwork(anyObject(), anyObject(), anyObject(), anyBoolean(),
                anyBoolean(), anyBoolean())).thenReturn(candidate);

        // Set screen to on
        mWifiConnectivityManager.handleScreenStateChanged(true);

        // Set WiFi to disconnected state
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        verify(mClientModeImpl, times(0)).startConnectToNetwork(
                CANDIDATE_NETWORK_ID, Process.WIFI_UID, CANDIDATE_BSSID);
    }

    /*
     * Wifi in connected state. Drop the roaming attempt if the recommended
     * network configuration has a BSSID specified but the scan result BSSID doesn't
     * match it.
     *
     * Expected behavior: WifiConnectivityManager doesn't invoke
     * ClientModeImpl.startRoamToNetwork().
     */
    @Test
    public void dropRoamingAttemptIfConfigSpecifiedBssidDifferentFromScanResultBssid() {
        // Mock the currently connected network which has the same networkID and
        // SSID as the one to be selected.
        WifiConfiguration currentNetwork = generateWifiConfig(
                0, CANDIDATE_NETWORK_ID, CANDIDATE_SSID, false, true, null, null);
        when(mWifiConfigManager.getConfiguredNetwork(anyInt())).thenReturn(currentNetwork);

        // Set up the candidate configuration such that it has a BSSID specified.
        WifiConfiguration candidate = generateWifiConfig(
                0, CANDIDATE_NETWORK_ID, CANDIDATE_SSID, false, true, null, null);
        candidate.BSSID = CANDIDATE_BSSID; // config specified
        ScanResult candidateScanResult = new ScanResult();
        candidateScanResult.SSID = CANDIDATE_SSID;
        // Set up the scan result BSSID to be different from the config specified one.
        candidateScanResult.BSSID = INVALID_SCAN_RESULT_BSSID;
        candidate.getNetworkSelectionStatus().setCandidate(candidateScanResult);

        when(mWifiNS.selectNetwork(anyObject(), anyObject(), anyObject(), anyBoolean(),
                anyBoolean(), anyBoolean())).thenReturn(candidate);

        // Set WiFi to connected state
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_CONNECTED);

        // Set screen to on
        mWifiConnectivityManager.handleScreenStateChanged(true);

        verify(mClientModeImpl, times(0)).startRoamToNetwork(anyInt(), anyObject());
    }

    /**
     *  Dump local log buffer.
     *
     * Expected behavior: Logs dumped from WifiConnectivityManager.dump()
     * contain the message we put in mLocalLog.
     */
    @Test
    public void dumpLocalLog() {
        final String localLogMessage = "This is a message from the test";
        mLocalLog.log(localLogMessage);

        StringWriter sw = new StringWriter();
        PrintWriter pw = new PrintWriter(sw);
        mWifiConnectivityManager.dump(new FileDescriptor(), pw, new String[]{});
        assertTrue(sw.toString().contains(localLogMessage));
    }

    /**
     *  Dump ONA controller.
     *
     * Expected behavior: {@link OpenNetworkNotifier#dump(FileDescriptor, PrintWriter,
     * String[])} is invoked.
     */
    @Test
    public void dumpNotificationController() {
        StringWriter sw = new StringWriter();
        PrintWriter pw = new PrintWriter(sw);
        mWifiConnectivityManager.dump(new FileDescriptor(), pw, new String[]{});

        verify(mOpenNetworkNotifier).dump(any(), any(), any());
    }

    /**
     * Create scan data with different radio chain infos:
     * First scan result has null radio chain info (No DBS support).
     * Second scan result has empty radio chain info (No DBS support).
     * Third scan result has 1 radio chain info (DBS scan).
     * Fourth scan result has 2 radio chain info (non-DBS scan).
     */
    private ScanData createScanDataWithDifferentRadioChainInfos() {
        // Create 4 scan results.
        ScanData[] scanDatas =
                ScanTestUtil.createScanDatas(new int[][]{{5150, 5175, 2412, 2400}}, new int[]{0});
        // WCM barfs if the scan result does not have an IE.
        scanDatas[0].getResults()[0].informationElements = new InformationElement[0];
        scanDatas[0].getResults()[1].informationElements = new InformationElement[0];
        scanDatas[0].getResults()[2].informationElements = new InformationElement[0];
        scanDatas[0].getResults()[3].informationElements = new InformationElement[0];
        scanDatas[0].getResults()[0].radioChainInfos = null;
        scanDatas[0].getResults()[1].radioChainInfos = new ScanResult.RadioChainInfo[0];
        scanDatas[0].getResults()[2].radioChainInfos = new ScanResult.RadioChainInfo[1];
        scanDatas[0].getResults()[3].radioChainInfos = new ScanResult.RadioChainInfo[2];

        return scanDatas[0];
    }

    /**
     * If |config_wifi_framework_use_single_radio_chain_scan_results_network_selection| flag is
     * false, WifiConnectivityManager should filter scan results which contain scans from a single
     * radio chain (i.e DBS scan).
     * Note:
     * a) ScanResult with no radio chain indicates a lack of DBS support on the device.
     * b) ScanResult with 2 radio chain info indicates a scan done using both the radio chains
     * on a DBS supported device.
     *
     * Expected behavior: WifiConnectivityManager invokes
     * {@link WifiNetworkSelector#selectNetwork(List, HashSet, WifiInfo, boolean, boolean, boolean)}
     * after filtering out the scan results obtained via DBS scan.
     */
    @Test
    public void filterScanResultsWithOneRadioChainInfoForNetworkSelectionIfConfigDisabled() {
        when(mResource.getBoolean(
                R.bool.config_wifi_framework_use_single_radio_chain_scan_results_network_selection))
                .thenReturn(false);
        when(mWifiNS.selectNetwork(any(), any(), any(), anyBoolean(), anyBoolean(), anyBoolean()))
                .thenReturn(null);
        mWifiConnectivityManager = createConnectivityManager();

        mScanData = createScanDataWithDifferentRadioChainInfos();

        // Capture scan details which were sent to network selector.
        final List<ScanDetail> capturedScanDetails = new ArrayList<>();
        doAnswer(new AnswerWithArguments() {
            public WifiConfiguration answer(
                    List<ScanDetail> scanDetails, HashSet<String> bssidBlacklist, WifiInfo wifiInfo,
                    boolean connected, boolean disconnected, boolean untrustedNetworkAllowed)
                    throws Exception {
                capturedScanDetails.addAll(scanDetails);
                return null;
            }}).when(mWifiNS).selectNetwork(
                    any(), any(), any(), anyBoolean(), anyBoolean(), anyBoolean());

        mWifiConnectivityManager.setTrustedConnectionAllowed(true);
        // Set WiFi to disconnected state with screen on which triggers a scan immediately.
        mWifiConnectivityManager.setWifiEnabled(true);
        mWifiConnectivityManager.handleScreenStateChanged(true);
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        // We should have filtered out the 3rd scan result.
        assertEquals(3, capturedScanDetails.size());
        List<ScanResult> capturedScanResults =
                capturedScanDetails.stream().map(ScanDetail::getScanResult)
                        .collect(Collectors.toList());

        assertEquals(3, capturedScanResults.size());
        assertTrue(capturedScanResults.contains(mScanData.getResults()[0]));
        assertTrue(capturedScanResults.contains(mScanData.getResults()[1]));
        assertFalse(capturedScanResults.contains(mScanData.getResults()[2]));
        assertTrue(capturedScanResults.contains(mScanData.getResults()[3]));
    }

    /**
     * If |config_wifi_framework_use_single_radio_chain_scan_results_network_selection| flag is
     * true, WifiConnectivityManager should not filter scan results which contain scans from a
     * single radio chain (i.e DBS scan).
     * Note:
     * a) ScanResult with no radio chain indicates a lack of DBS support on the device.
     * b) ScanResult with 2 radio chain info indicates a scan done using both the radio chains
     * on a DBS supported device.
     *
     * Expected behavior: WifiConnectivityManager invokes
     * {@link WifiNetworkSelector#selectNetwork(List, HashSet, WifiInfo, boolean, boolean, boolean)}
     * after filtering out the scan results obtained via DBS scan.
     */
    @Test
    public void dontFilterScanResultsWithOneRadioChainInfoForNetworkSelectionIfConfigEnabled() {
        when(mResource.getBoolean(
                R.bool.config_wifi_framework_use_single_radio_chain_scan_results_network_selection))
                .thenReturn(true);
        when(mWifiNS.selectNetwork(any(), any(), any(), anyBoolean(), anyBoolean(), anyBoolean()))
                .thenReturn(null);
        mWifiConnectivityManager = createConnectivityManager();

        mScanData = createScanDataWithDifferentRadioChainInfos();

        // Capture scan details which were sent to network selector.
        final List<ScanDetail> capturedScanDetails = new ArrayList<>();
        doAnswer(new AnswerWithArguments() {
            public WifiConfiguration answer(
                    List<ScanDetail> scanDetails, HashSet<String> bssidBlacklist, WifiInfo wifiInfo,
                    boolean connected, boolean disconnected, boolean untrustedNetworkAllowed)
                    throws Exception {
                capturedScanDetails.addAll(scanDetails);
                return null;
            }}).when(mWifiNS).selectNetwork(
                any(), any(), any(), anyBoolean(), anyBoolean(), anyBoolean());

        mWifiConnectivityManager.setTrustedConnectionAllowed(true);
        // Set WiFi to disconnected state with screen on which triggers a scan immediately.
        mWifiConnectivityManager.setWifiEnabled(true);
        mWifiConnectivityManager.handleScreenStateChanged(true);
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        // We should not filter any of the scan results.
        assertEquals(4, capturedScanDetails.size());
        List<ScanResult> capturedScanResults =
                capturedScanDetails.stream().map(ScanDetail::getScanResult)
                        .collect(Collectors.toList());

        assertEquals(4, capturedScanResults.size());
        assertTrue(capturedScanResults.contains(mScanData.getResults()[0]));
        assertTrue(capturedScanResults.contains(mScanData.getResults()[1]));
        assertTrue(capturedScanResults.contains(mScanData.getResults()[2]));
        assertTrue(capturedScanResults.contains(mScanData.getResults()[3]));
    }

    /**
     * Disabling the network temporarily due to lack of internet is a special reason for which we
     * don't want WCM to trigger a disconnect (by removing the network from supplicant).
     */
    @Test
    public void dontDisconnectIfNetworkTemporarilyDisabledDueToNoInternet() {
        assertNotNull(mSavedNetworkUpdateListenerCaptor.getValue());

        mSavedNetworkUpdateListenerCaptor.getValue()
                .onSavedNetworkPermanentlyDisabled(0, DISABLED_AUTHENTICATION_FAILURE);
        verify(mWifiConnectivityHelper).removeNetworkIfCurrent(0);

        mSavedNetworkUpdateListenerCaptor.getValue()
                .onSavedNetworkPermanentlyDisabled(0, DISABLED_NO_INTERNET_TEMPORARY);
        // Don't remove network.
    }

    /**
     * Verify the various WifiConnectivityManager enable/disable sequences.
     *
     * Expected behavior: WifiConnectivityManager is turned on as a long as there is
     *  - No specific network request being processed.
     *    And
     *    - Pending generic Network request for trusted wifi connection.
     *      OR
     *    - Pending generic Network request for untrused wifi connection.
     */
    @Test
    public void verifyEnableAndDisable() {
        mWifiConnectivityManager = createConnectivityManager();

        // set wifi on & disconnected to trigger pno scans when auto-join is enabled.
        mWifiConnectivityManager.setWifiEnabled(true);
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);

        // Enable trusted connection. This should trigger a pno scan for auto-join.
        mWifiConnectivityManager.setTrustedConnectionAllowed(true);
        verify(mWifiScanner).startDisconnectedPnoScan(any(), any(), any());

        // Start of processing a specific request. This should stop any pno scan for auto-join.
        mWifiConnectivityManager.setSpecificNetworkRequestInProgress(true);
        verify(mWifiScanner).stopPnoScan(any());

        // End of processing a specific request. This should now trigger a new pno scan for
        // auto-join.
        mWifiConnectivityManager.setSpecificNetworkRequestInProgress(false);
        verify(mWifiScanner, times(2)).startDisconnectedPnoScan(any(), any(), any());

        // Disable trusted connection. This should stop any pno scan for auto-join.
        mWifiConnectivityManager.setTrustedConnectionAllowed(false);
        verify(mWifiScanner, times(2)).stopPnoScan(any());

        // Enable untrusted connection. This should trigger a pno scan for auto-join.
        mWifiConnectivityManager.setUntrustedConnectionAllowed(true);
        verify(mWifiScanner, times(3)).startDisconnectedPnoScan(any(), any(), any());
    }

    /**
     * Change device mobility state in the middle of a PNO scan. PNO scan should stop, then restart
     * with the updated scan period.
     */
    @Test
    public void changeDeviceMobilityStateDuringScan() {
        mWifiConnectivityManager.setWifiEnabled(true);

        // starts a PNO scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);
        mWifiConnectivityManager.setTrustedConnectionAllowed(true);

        ArgumentCaptor<ScanSettings> scanSettingsCaptor = ArgumentCaptor.forClass(
                ScanSettings.class);
        InOrder inOrder = inOrder(mWifiScanner);

        inOrder.verify(mWifiScanner).startDisconnectedPnoScan(
                scanSettingsCaptor.capture(), any(), any());
        assertEquals(scanSettingsCaptor.getValue().periodInMs,
                WifiConnectivityManager.MOVING_PNO_SCAN_INTERVAL_MS);

        // initial connectivity state uses moving PNO scan interval, now set it to stationary
        mWifiConnectivityManager.setDeviceMobilityState(
                WifiManager.DEVICE_MOBILITY_STATE_STATIONARY);

        inOrder.verify(mWifiScanner).stopPnoScan(any());
        inOrder.verify(mWifiScanner).startDisconnectedPnoScan(
                scanSettingsCaptor.capture(), any(), any());
        assertEquals(scanSettingsCaptor.getValue().periodInMs,
                WifiConnectivityManager.STATIONARY_PNO_SCAN_INTERVAL_MS);
    }

    /**
     * Change device mobility state in the middle of a PNO scan, but it is changed to another
     * mobility state with the same scan period. Original PNO scan should continue.
     */
    @Test
    public void changeDeviceMobilityStateDuringScanWithSameScanPeriod() {
        mWifiConnectivityManager.setWifiEnabled(true);

        // starts a PNO scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);
        mWifiConnectivityManager.setTrustedConnectionAllowed(true);

        ArgumentCaptor<ScanSettings> scanSettingsCaptor = ArgumentCaptor.forClass(
                ScanSettings.class);
        InOrder inOrder = inOrder(mWifiScanner);
        inOrder.verify(mWifiScanner, never()).stopPnoScan(any());
        inOrder.verify(mWifiScanner).startDisconnectedPnoScan(
                scanSettingsCaptor.capture(), any(), any());
        assertEquals(scanSettingsCaptor.getValue().periodInMs,
                WifiConnectivityManager.MOVING_PNO_SCAN_INTERVAL_MS);

        mWifiConnectivityManager.setDeviceMobilityState(
                WifiManager.DEVICE_MOBILITY_STATE_LOW_MVMT);

        inOrder.verifyNoMoreInteractions();
    }

    /**
     * Device is already connected, setting device mobility state should do nothing since no PNO
     * scans are running. Then, when PNO scan is started afterwards, should use the new scan period.
     */
    @Test
    public void setDeviceMobilityStateBeforePnoScan() {
        // ensure no PNO scan running
        mWifiConnectivityManager.setWifiEnabled(true);
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_CONNECTED);

        // initial connectivity state uses moving PNO scan interval, now set it to stationary
        mWifiConnectivityManager.setDeviceMobilityState(
                WifiManager.DEVICE_MOBILITY_STATE_STATIONARY);

        // no scans should start or stop because no PNO scan is running
        verify(mWifiScanner, never()).startDisconnectedPnoScan(any(), any(), any());
        verify(mWifiScanner, never()).stopPnoScan(any());

        // starts a PNO scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);
        mWifiConnectivityManager.setTrustedConnectionAllowed(true);

        ArgumentCaptor<ScanSettings> scanSettingsCaptor = ArgumentCaptor.forClass(
                ScanSettings.class);

        verify(mWifiScanner).startDisconnectedPnoScan(scanSettingsCaptor.capture(), any(), any());
        // check that now the PNO scan uses the stationary interval, even though it was set before
        // the PNO scan started
        assertEquals(scanSettingsCaptor.getValue().periodInMs,
                WifiConnectivityManager.STATIONARY_PNO_SCAN_INTERVAL_MS);
    }

    /**
     * Tests the metrics collection of PNO scans through changes to device mobility state and
     * starting and stopping of PNO scans.
     */
    @Test
    public void deviceMobilityStateMetricsChangeStateAndStopStart() {
        InOrder inOrder = inOrder(mWifiMetrics);

        mWifiConnectivityManager = createConnectivityManager();
        mWifiConnectivityManager.setWifiEnabled(true);

        // change mobility state while no PNO scans running
        mWifiConnectivityManager.setDeviceMobilityState(
                WifiManager.DEVICE_MOBILITY_STATE_LOW_MVMT);
        inOrder.verify(mWifiMetrics).enterDeviceMobilityState(
                WifiManager.DEVICE_MOBILITY_STATE_LOW_MVMT);

        // starts a PNO scan
        mWifiConnectivityManager.handleConnectionStateChanged(
                WifiConnectivityManager.WIFI_STATE_DISCONNECTED);
        mWifiConnectivityManager.setTrustedConnectionAllowed(true);
        inOrder.verify(mWifiMetrics).logPnoScanStart();

        // change to High Movement, which has the same scan interval as Low Movement
        mWifiConnectivityManager.setDeviceMobilityState(
                WifiManager.DEVICE_MOBILITY_STATE_HIGH_MVMT);
        inOrder.verify(mWifiMetrics).logPnoScanStop();
        inOrder.verify(mWifiMetrics).enterDeviceMobilityState(
                WifiManager.DEVICE_MOBILITY_STATE_HIGH_MVMT);
        inOrder.verify(mWifiMetrics).logPnoScanStart();

        // change to Stationary, which has a different scan interval from High Movement
        mWifiConnectivityManager.setDeviceMobilityState(
                WifiManager.DEVICE_MOBILITY_STATE_STATIONARY);
        inOrder.verify(mWifiMetrics).logPnoScanStop();
        inOrder.verify(mWifiMetrics).enterDeviceMobilityState(
                WifiManager.DEVICE_MOBILITY_STATE_STATIONARY);
        inOrder.verify(mWifiMetrics).logPnoScanStart();

        // stops PNO scan
        mWifiConnectivityManager.setTrustedConnectionAllowed(false);
        inOrder.verify(mWifiMetrics).logPnoScanStop();

        // change mobility state while no PNO scans running
        mWifiConnectivityManager.setDeviceMobilityState(
                WifiManager.DEVICE_MOBILITY_STATE_HIGH_MVMT);
        inOrder.verify(mWifiMetrics).enterDeviceMobilityState(
                WifiManager.DEVICE_MOBILITY_STATE_HIGH_MVMT);

        inOrder.verifyNoMoreInteractions();
    }

    /**
     * Verifies BSSID blacklist consistent with Watchdog trigger.
     *
     * Expected behavior: A BSSID won't gets blacklisted if there only BSSID
     * of its SSID be observed and Watchdog trigger is activated.
     */
    @Test
    public void verifyConsistentWatchdogAndBssidBlacklist() {
        String bssid = "6c:f3:7f:ae:8c:f3";

        // If there only one BSSID is available and Watchdog trigger is activated.
        when(mWifiLastResortWatchdog.shouldIgnoreBssidUpdate(anyString())).thenReturn(true);
        when(mWifiConnectivityHelper.isFirmwareRoamingSupported()).thenReturn(true);

        // Verify that a BSSID won't gets blacklisted if there only one BSSID is available
        // and watchdog recover is not triggered.
        for (int i = 0; i < WifiConnectivityManager.BSSID_BLACKLIST_THRESHOLD; i++) {
            assertFalse(mWifiConnectivityManager.isBssidDisabled(bssid));
            mWifiConnectivityManager.trackBssid(bssid, false, 1);
        }

        // Verify the BSSID is not blacklisted.
        assertFalse(mWifiConnectivityManager.isBssidDisabled(bssid));
    }
}
