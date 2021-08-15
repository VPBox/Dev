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
 * limitations under the License
 */
package com.android.server.wifi;

import static android.net.wifi.WifiManager.DEVICE_MOBILITY_STATE_HIGH_MVMT;
import static android.net.wifi.WifiManager.DEVICE_MOBILITY_STATE_LOW_MVMT;
import static android.net.wifi.WifiManager.DEVICE_MOBILITY_STATE_STATIONARY;
import static android.net.wifi.WifiManager.DEVICE_MOBILITY_STATE_UNKNOWN;

import static com.android.server.wifi.WifiMetricsTestUtil.assertDeviceMobilityStatePnoScanStatsEqual;
import static com.android.server.wifi.WifiMetricsTestUtil.assertExperimentProbeCountsEqual;
import static com.android.server.wifi.WifiMetricsTestUtil.assertHistogramBucketsEqual;
import static com.android.server.wifi.WifiMetricsTestUtil.assertKeyCountsEqual;
import static com.android.server.wifi.WifiMetricsTestUtil.assertLinkProbeFailureReasonCountsEqual;
import static com.android.server.wifi.WifiMetricsTestUtil.assertLinkProbeStaEventsEqual;
import static com.android.server.wifi.WifiMetricsTestUtil.buildDeviceMobilityStatePnoScanStats;
import static com.android.server.wifi.WifiMetricsTestUtil.buildExperimentProbeCounts;
import static com.android.server.wifi.WifiMetricsTestUtil.buildHistogramBucketInt32;
import static com.android.server.wifi.WifiMetricsTestUtil.buildInt32Count;
import static com.android.server.wifi.WifiMetricsTestUtil.buildLinkProbeFailureReasonCount;
import static com.android.server.wifi.WifiMetricsTestUtil.buildLinkProbeFailureStaEvent;
import static com.android.server.wifi.WifiMetricsTestUtil.buildLinkProbeSuccessStaEvent;
import static com.android.server.wifi.nano.WifiMetricsProto.StaEvent.TYPE_LINK_PROBE;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyBoolean;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.content.Context;
import android.net.NetworkAgent;
import android.net.wifi.EAPConstants;
import android.net.wifi.IOnWifiUsabilityStatsListener;
import android.net.wifi.ScanResult;
import android.net.wifi.SupplicantState;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiSsid;
import android.net.wifi.hotspot2.PasspointConfiguration;
import android.net.wifi.hotspot2.ProvisioningCallback;
import android.net.wifi.hotspot2.pps.Credential;
import android.os.Handler;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.test.TestLooper;
import android.provider.Settings;
import android.telephony.TelephonyManager;
import android.util.Base64;
import android.util.Pair;
import android.util.SparseIntArray;

import androidx.test.filters.MediumTest;
import androidx.test.filters.SmallTest;

import com.android.server.wifi.aware.WifiAwareMetrics;
import com.android.server.wifi.hotspot2.NetworkDetail;
import com.android.server.wifi.hotspot2.PasspointManager;
import com.android.server.wifi.hotspot2.PasspointMatch;
import com.android.server.wifi.hotspot2.PasspointProvider;
import com.android.server.wifi.nano.WifiMetricsProto;
import com.android.server.wifi.nano.WifiMetricsProto.ConnectToNetworkNotificationAndActionCount;
import com.android.server.wifi.nano.WifiMetricsProto.DeviceMobilityStatePnoScanStats;
import com.android.server.wifi.nano.WifiMetricsProto.HistogramBucketInt32;
import com.android.server.wifi.nano.WifiMetricsProto.Int32Count;
import com.android.server.wifi.nano.WifiMetricsProto.LinkProbeStats;
import com.android.server.wifi.nano.WifiMetricsProto.LinkProbeStats.ExperimentProbeCounts;
import com.android.server.wifi.nano.WifiMetricsProto.LinkProbeStats.LinkProbeFailureReasonCount;
import com.android.server.wifi.nano.WifiMetricsProto.NetworkSelectionExperimentDecisions;
import com.android.server.wifi.nano.WifiMetricsProto.PasspointProfileTypeCount;
import com.android.server.wifi.nano.WifiMetricsProto.PasspointProvisionStats;
import com.android.server.wifi.nano.WifiMetricsProto.PnoScanMetrics;
import com.android.server.wifi.nano.WifiMetricsProto.SoftApConnectedClientsEvent;
import com.android.server.wifi.nano.WifiMetricsProto.StaEvent;
import com.android.server.wifi.nano.WifiMetricsProto.WifiIsUnusableEvent;
import com.android.server.wifi.nano.WifiMetricsProto.WifiRadioUsage;
import com.android.server.wifi.nano.WifiMetricsProto.WifiUsabilityStats;
import com.android.server.wifi.nano.WifiMetricsProto.WifiUsabilityStatsEntry;
import com.android.server.wifi.nano.WifiMetricsProto.WpsMetrics;
import com.android.server.wifi.p2p.WifiP2pMetrics;
import com.android.server.wifi.rtt.RttMetrics;
import com.android.server.wifi.util.ExternalCallbackTracker;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.io.ByteArrayOutputStream;
import java.io.FileDescriptor;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.BitSet;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Unit tests for {@link com.android.server.wifi.WifiMetrics}.
 */
@SmallTest
public class WifiMetricsTest {

    WifiMetrics mWifiMetrics;
    WifiMetricsProto.WifiLog mDecodedProto;
    TestLooper mTestLooper;
    Random mRandom = new Random();
    private static final int TEST_WIFI_USABILITY_STATS_LISTENER_IDENTIFIER = 2;
    private static final int TEST_NETWORK_ID = 42;
    @Mock Context mContext;
    @Mock FrameworkFacade mFacade;
    @Mock Clock mClock;
    @Mock ScoringParams mScoringParams;
    @Mock WifiConfigManager mWcm;
    @Mock PasspointManager mPpm;
    @Mock WifiNetworkSelector mWns;
    @Mock WifiPowerMetrics mWifiPowerMetrics;
    @Mock WifiDataStall mWifiDataStall;
    @Mock IBinder mAppBinder;
    @Mock IOnWifiUsabilityStatsListener mOnWifiUsabilityStatsListener;
    @Mock ExternalCallbackTracker<IOnWifiUsabilityStatsListener> mListenerTracker;
    @Mock WifiP2pMetrics mWifiP2pMetrics;
    @Mock DppMetrics mDppMetrics;
    @Mock CellularLinkLayerStatsCollector mCellularLinkLayerStatsCollector;
    @Mock CellularLinkLayerStats mCellularLinkLayerStats;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mDecodedProto = null;
        when(mClock.getElapsedSinceBootMillis()).thenReturn((long) 0);
        when(mCellularLinkLayerStatsCollector.update()).thenReturn(mCellularLinkLayerStats);
        mTestLooper = new TestLooper();
        mWifiMetrics = new WifiMetrics(mContext, mFacade, mClock, mTestLooper.getLooper(),
                new WifiAwareMetrics(mClock), new RttMetrics(mClock), mWifiPowerMetrics,
                mWifiP2pMetrics, mDppMetrics, mCellularLinkLayerStatsCollector);
        mWifiMetrics.setWifiConfigManager(mWcm);
        mWifiMetrics.setPasspointManager(mPpm);
        mWifiMetrics.setScoringParams(mScoringParams);
        mWifiMetrics.setWifiNetworkSelector(mWns);
        mWifiMetrics.setWifiDataStall(mWifiDataStall);
    }

    /**
     * Test that startConnectionEvent and endConnectionEvent can be called repeatedly and out of
     * order. Only tests no exception occurs. Creates 3 ConnectionEvents.
     */
    @Test
    public void startAndEndConnectionEventSucceeds() throws Exception {
        //Start and end Connection event
        mWifiMetrics.startConnectionEvent(null, "RED",
                WifiMetricsProto.ConnectionEvent.ROAM_ENTERPRISE);
        mWifiMetrics.endConnectionEvent(
                WifiMetrics.ConnectionEvent.FAILURE_AUTHENTICATION_FAILURE,
                WifiMetricsProto.ConnectionEvent.HLF_DHCP,
                WifiMetricsProto.ConnectionEvent.FAILURE_REASON_UNKNOWN);
        //end Connection event without starting one
        mWifiMetrics.endConnectionEvent(
                WifiMetrics.ConnectionEvent.FAILURE_AUTHENTICATION_FAILURE,
                WifiMetricsProto.ConnectionEvent.HLF_DHCP,
                WifiMetricsProto.ConnectionEvent.FAILURE_REASON_UNKNOWN);
        //start two ConnectionEvents in a row
        mWifiMetrics.startConnectionEvent(null, "BLUE",
                WifiMetricsProto.ConnectionEvent.ROAM_ENTERPRISE);
        mWifiMetrics.startConnectionEvent(null, "GREEN",
                WifiMetricsProto.ConnectionEvent.ROAM_ENTERPRISE);
    }

    private static final long TEST_RECORD_DURATION_SEC = 12 * 60 * 60;
    private static final long TEST_RECORD_DURATION_MILLIS = TEST_RECORD_DURATION_SEC * 1000;
    /**
     * Simulate how dumpsys gets the proto from mWifiMetrics, filter the proto bytes out and
     * deserialize them into mDecodedProto
     */
    public void dumpProtoAndDeserialize() throws Exception {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        PrintWriter writer = new PrintWriter(stream);

        when(mClock.getElapsedSinceBootMillis()).thenReturn(TEST_RECORD_DURATION_MILLIS);
        //Test proto dump, by passing in proto arg option
        String[] args = {WifiMetrics.PROTO_DUMP_ARG};
        mWifiMetrics.dump(null, writer, args);
        writer.flush();
        Pattern pattern = Pattern.compile(
                "(?<=WifiMetrics:\\n)([\\s\\S]*)(?=EndWifiMetrics)");
        Matcher matcher = pattern.matcher(stream.toString());
        assertTrue("Proto Byte string found in WifiMetrics.dump():\n" + stream.toString(),
                matcher.find());
        String protoByteString = matcher.group(1);
        byte[] protoBytes = Base64.decode(protoByteString, Base64.DEFAULT);
        mDecodedProto = WifiMetricsProto.WifiLog.parseFrom(protoBytes);
    }

    /**
     * Gets the 'clean dump' proto bytes from mWifiMetrics & deserializes it into
     * mDecodedProto
     */
    public void cleanDumpProtoAndDeserialize() throws Exception {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        PrintWriter writer = new PrintWriter(stream);

        when(mClock.getElapsedSinceBootMillis()).thenReturn(TEST_RECORD_DURATION_MILLIS);
        //Test proto dump, by passing in proto arg option
        String[] args = {WifiMetrics.PROTO_DUMP_ARG, WifiMetrics.CLEAN_DUMP_ARG};
        mWifiMetrics.dump(null, writer, args);
        writer.flush();
        String protoByteString = stream.toString();
        byte[] protoBytes = Base64.decode(protoByteString, Base64.DEFAULT);
        mDecodedProto = WifiMetricsProto.WifiLog.parseFrom(protoBytes);
    }

    /** Verifies that dump() includes the expected header */
    @Test
    public void stateDumpIncludesHeader() throws Exception {
        assertStringContains(getStateDump(), "WifiMetrics");
    }

    /** Verifies that dump() includes correct alert count when there are no alerts. */
    @Test
    public void stateDumpAlertCountIsCorrectWithNoAlerts() throws Exception {
        assertStringContains(getStateDump(), "mWifiLogProto.alertReasonCounts=()");
    }

    /** Verifies that dump() includes correct alert count when there is one alert. */
    @Test
    public void stateDumpAlertCountIsCorrectWithOneAlert() throws Exception {
        mWifiMetrics.logFirmwareAlert(1);
        assertStringContains(getStateDump(), "mWifiLogProto.alertReasonCounts=(1,1)");
    }

    /** Verifies that dump() includes correct alert count when there are multiple alerts. */
    @Test
    public void stateDumpAlertCountIsCorrectWithMultipleAlerts() throws Exception {
        mWifiMetrics.logFirmwareAlert(1);
        mWifiMetrics.logFirmwareAlert(1);
        mWifiMetrics.logFirmwareAlert(16);
        assertStringContains(getStateDump(), "mWifiLogProto.alertReasonCounts=(1,2),(16,1)");
    }

    @Test
    public void testDumpProtoAndDeserialize() throws Exception {
        setAndIncrementMetrics();
        dumpProtoAndDeserialize();
        verify(mWifiP2pMetrics).consolidateProto();
        assertDeserializedMetricsCorrect();
    }

    private static final int NUM_OPEN_NETWORKS = 2;
    private static final int NUM_LEGACY_PERSONAL_NETWORKS = 3;
    private static final int NUM_LEGACY_ENTERPRISE_NETWORKS = 5;
    private static final int NUM_ENHANCED_OPEN_NETWORKS = 1;
    private static final int NUM_WPA3_PERSONAL_NETWORKS = 4;
    private static final int NUM_WPA3_ENTERPRISE_NETWORKS = 6;
    private static final int NUM_SAVED_NETWORKS = NUM_OPEN_NETWORKS + NUM_LEGACY_PERSONAL_NETWORKS
            + NUM_LEGACY_ENTERPRISE_NETWORKS + NUM_ENHANCED_OPEN_NETWORKS
            + NUM_WPA3_PERSONAL_NETWORKS + NUM_WPA3_ENTERPRISE_NETWORKS;
    private static final int NUM_HIDDEN_NETWORKS = NUM_OPEN_NETWORKS;
    private static final int NUM_PASSPOINT_NETWORKS = NUM_LEGACY_ENTERPRISE_NETWORKS;
    private static final int NUM_NETWORKS_ADDED_BY_USER = 1;
    private static final int NUM_NETWORKS_ADDED_BY_APPS = NUM_SAVED_NETWORKS
            - NUM_NETWORKS_ADDED_BY_USER;
    private static final boolean TEST_VAL_IS_LOCATION_ENABLED = true;
    private static final boolean IS_SCANNING_ALWAYS_ENABLED = true;
    private static final int NUM_EMPTY_SCAN_RESULTS = 19;
    private static final int NUM_NON_EMPTY_SCAN_RESULTS = 23;
    private static final int NUM_SCAN_UNKNOWN = 1;
    private static final int NUM_SCAN_SUCCESS = 2;
    private static final int NUM_SCAN_FAILURE_INTERRUPTED = 3;
    private static final int NUM_SCAN_FAILURE_INVALID_CONFIGURATION = 5;
    private static final int NUM_WIFI_UNKNOWN_SCREEN_OFF = 3;
    private static final int NUM_WIFI_UNKNOWN_SCREEN_ON = 5;
    private static final int NUM_WIFI_ASSOCIATED_SCREEN_OFF = 7;
    private static final int NUM_WIFI_ASSOCIATED_SCREEN_ON = 11;
    private static final int NUM_CONNECTIVITY_WATCHDOG_PNO_GOOD = 11;
    private static final int NUM_CONNECTIVITY_WATCHDOG_PNO_BAD = 12;
    private static final int NUM_CONNECTIVITY_WATCHDOG_BACKGROUND_GOOD = 13;
    private static final int NUM_CONNECTIVITY_WATCHDOG_BACKGROUND_BAD = 14;
    private static final int NUM_LAST_RESORT_WATCHDOG_TRIGGERS = 1;
    private static final int NUM_LAST_RESORT_WATCHDOG_BAD_ASSOCIATION_NETWORKS_TOTAL = 2;
    private static final int NUM_LAST_RESORT_WATCHDOG_BAD_AUTHENTICATION_NETWORKS_TOTAL = 3;
    private static final int NUM_LAST_RESORT_WATCHDOG_BAD_DHCP_NETWORKS_TOTAL = 4;
    private static final int NUM_LAST_RESORT_WATCHDOG_BAD_OTHER_NETWORKS_TOTAL = 5;
    private static final int NUM_LAST_RESORT_WATCHDOG_AVAILABLE_NETWORKS_TOTAL = 6;
    private static final int NUM_LAST_RESORT_WATCHDOG_TRIGGERS_WITH_BAD_ASSOCIATION = 7;
    private static final int NUM_LAST_RESORT_WATCHDOG_TRIGGERS_WITH_BAD_AUTHENTICATION = 8;
    private static final int NUM_LAST_RESORT_WATCHDOG_TRIGGERS_WITH_BAD_DHCP = 9;
    private static final int NUM_LAST_RESORT_WATCHDOG_TRIGGERS_WITH_BAD_OTHER = 10;
    private static final int NUM_LAST_RESORT_WATCHDOG_SUCCESSES = 5;
    private static final int WATCHDOG_TOTAL_CONNECTION_FAILURE_COUNT_AFTER_TRIGGER = 6;
    private static final int RSSI_POLL_FREQUENCY = 5150;
    private static final int NUM_RSSI_LEVELS_TO_INCREMENT = 20;
    private static final int NUM_OPEN_NETWORK_SCAN_RESULTS = 1;
    private static final int NUM_LEGACY_PERSONAL_NETWORK_SCAN_RESULTS = 4;
    private static final int NUM_LEGACY_ENTERPRISE_NETWORK_SCAN_RESULTS = 3;
    private static final int NUM_ENHANCED_OPEN_NETWORK_SCAN_RESULTS = 1;
    private static final int NUM_WPA3_PERSONAL_NETWORK_SCAN_RESULTS = 2;
    private static final int NUM_WPA3_ENTERPRISE_NETWORK_SCAN_RESULTS = 1;
    private static final int NUM_HIDDEN_NETWORK_SCAN_RESULTS = 1;
    private static final int NUM_HOTSPOT2_R1_NETWORK_SCAN_RESULTS = 1;
    private static final int NUM_HOTSPOT2_R2_NETWORK_SCAN_RESULTS = 2;
    private static final int NUM_SCANS = 5;
    private static final int NUM_CONNECTIVITY_ONESHOT_SCAN_EVENT = 4;
    private static final int NUM_EXTERNAL_APP_ONESHOT_SCAN_REQUESTS = 15;
    private static final int NUM_EXTERNAL_FOREGROUND_APP_ONESHOT_SCAN_REQUESTS_THROTTLED = 10;
    private static final int NUM_EXTERNAL_BACKGROUND_APP_ONESHOT_SCAN_REQUESTS_THROTTLED = 16;
    // Look at buildMockScanDetailList, this number needs to match the mocked results
    private static final int NUM_TOTAL_SCAN_RESULTS = NUM_OPEN_NETWORK_SCAN_RESULTS
            + NUM_LEGACY_PERSONAL_NETWORK_SCAN_RESULTS + NUM_LEGACY_ENTERPRISE_NETWORK_SCAN_RESULTS
            + NUM_ENHANCED_OPEN_NETWORK_SCAN_RESULTS + NUM_WPA3_PERSONAL_NETWORK_SCAN_RESULTS
            + NUM_WPA3_ENTERPRISE_NETWORK_SCAN_RESULTS;
    private static final int MIN_RSSI_LEVEL = -127;
    private static final int MAX_RSSI_LEVEL = 0;
    private static final int WIFI_SCORE_RANGE_MIN = 0;
    private static final int NUM_WIFI_SCORES_TO_INCREMENT = 20;
    private static final int WIFI_SCORE_RANGE_MAX = 60;
    private static final int NUM_OUT_OF_BOUND_ENTRIES = 10;
    private static final int MAX_NUM_SOFTAP_RETURN_CODES = 3;
    private static final int NUM_SOFTAP_START_SUCCESS = 3;
    private static final int NUM_SOFTAP_FAILED_GENERAL_ERROR = 2;
    private static final int NUM_SOFTAP_FAILED_NO_CHANNEL = 1;
    private static final int NUM_HAL_CRASHES = 11;
    private static final int NUM_WIFICOND_CRASHES = 12;
    private static final int NUM_SUPPLICANT_CRASHES = 23;
    private static final int NUM_HOSTAPD_CRASHES = 7;
    private static final int NUM_WIFI_ON_FAILURE_DUE_TO_HAL = 13;
    private static final int NUM_WIFI_ON_FAILURE_DUE_TO_WIFICOND = 14;
    private static final int NUM_WIFI_ON_FAILURE_DUE_TO_SUPPLICANT = 20;
    private static final int NUM_SOFTAP_ON_FAILURE_DUE_TO_HAL = 23;
    private static final int NUM_SOFTAP_ON_FAILURE_DUE_TO_WIFICOND = 19;
    private static final int NUM_SOFTAP_ON_FAILURE_DUE_TO_HOSTAPD = 31;
    private static final int NUM_SOFTAP_INTERFACE_DOWN = 65;
    private static final int NUM_CLIENT_INTERFACE_DOWN = 12;
    private static final int NUM_PASSPOINT_PROVIDERS = 7;
    private static final int NUM_PASSPOINT_PROVIDER_INSTALLATION = 5;
    private static final int NUM_PASSPOINT_PROVIDER_INSTALL_SUCCESS = 4;
    private static final int NUM_PASSPOINT_PROVIDER_UNINSTALLATION = 3;
    private static final int NUM_PASSPOINT_PROVIDER_UNINSTALL_SUCCESS = 2;
    private static final int NUM_PASSPOINT_PROVIDERS_SUCCESSFULLY_CONNECTED = 1;
    private static final int NUM_EAP_SIM_TYPE = 1;
    private static final int NUM_EAP_TTLS_TYPE = 2;
    private static final int NUM_EAP_TLS_TYPE = 3;
    private static final int NUM_EAP_AKA_TYPE = 4;
    private static final int NUM_EAP_AKA_PRIME_TYPE = 5;
    private static final SparseIntArray SAVED_PASSPOINT_PROVIDERS_TYPE = new SparseIntArray();
    static {
        SAVED_PASSPOINT_PROVIDERS_TYPE.put(EAPConstants.EAP_SIM, NUM_EAP_SIM_TYPE);
        SAVED_PASSPOINT_PROVIDERS_TYPE.put(EAPConstants.EAP_TTLS, NUM_EAP_TTLS_TYPE);
        SAVED_PASSPOINT_PROVIDERS_TYPE.put(EAPConstants.EAP_TLS, NUM_EAP_TLS_TYPE);
        SAVED_PASSPOINT_PROVIDERS_TYPE.put(EAPConstants.EAP_AKA, NUM_EAP_AKA_TYPE);
        SAVED_PASSPOINT_PROVIDERS_TYPE.put(EAPConstants.EAP_AKA_PRIME, NUM_EAP_AKA_PRIME_TYPE);
    }

    private static final int NUM_PARTIAL_SCAN_RESULTS = 73;
    private static final int NUM_PNO_SCAN_ATTEMPTS = 20;
    private static final int NUM_PNO_SCAN_FAILED = 5;
    private static final int NUM_PNO_SCAN_STARTED_OVER_OFFLOAD = 17;
    private static final int NUM_PNO_SCAN_FAILED_OVER_OFFLOAD = 8;
    private static final int NUM_PNO_FOUND_NETWORK_EVENTS = 10;
    private static final int NUM_WPS_ATTEMPTS = 17;
    private static final int NUM_WPS_SUCCESS = 21;
    private static final int NUM_WPS_START_FAILURE = 7;
    private static final int NUM_WPS_OVERLAP_FAILURE = 3;
    private static final int NUM_WPS_TIMEOUT_FAILURE = 8;
    private static final int NUM_WPS_OTHER_CONNECTION_FAILURE = 16;
    private static final int NUM_WPS_SUPPLICANT_FAILURE = 12;
    private static final int NUM_WPS_CANCELLATION = 11;
    private static final int NUM_RADIO_MODE_CHANGE_TO_MCC = 4;
    private static final int NUM_RADIO_MODE_CHANGE_TO_SCC = 13;
    private static final int NUM_RADIO_MODE_CHANGE_TO_SBS = 19;
    private static final int NUM_RADIO_MODE_CHANGE_TO_DBS = 34;
    private static final int NUM_SOFTAP_USER_BAND_PREFERENCE_UNSATISFIED = 14;
    private static final long NUM_WATCHDOG_SUCCESS_DURATION_MS = 65;
    private static final long WIFI_POWER_METRICS_LOGGING_DURATION = 280;
    private static final long WIFI_POWER_METRICS_SCAN_TIME = 33;
    private static final boolean WIFI_IS_UNUSABLE_EVENT_LOGGING_SETTING = true;
    private static final boolean LINK_SPEED_COUNTS_LOGGING_SETTING = true;
    private static final int DATA_STALL_MIN_TX_BAD_SETTING = 5;
    private static final int DATA_STALL_MIN_TX_SUCCESS_WITHOUT_RX_SETTING = 75;
    private static final int NUM_SAR_SENSOR_LISTENER_REGISTRATION_FAILURES = 5;
    private static final int NUM_ONESHOT_SCAN_REQUESTS_WITH_DFS_CHANNELS = 4;
    private static final int NUM_ADD_OR_UPDATE_NETWORK_CALLS = 5;
    private static final int NUM_ENABLE_NETWORK_CALLS = 6;

    /** Number of notifications per "Connect to Network" notification type. */
    private static final int[] NUM_CONNECT_TO_NETWORK_NOTIFICATIONS = {0, 10, 20, 30, 40};
    /** Number of notifications per "Connect to Network notification type and action type. */
    private static final int[][] NUM_CONNECT_TO_NETWORK_NOTIFICATION_ACTIONS = {
            {0, 1, 2, 3, 4},
            {10, 11, 12, 13, 14},
            {20, 21, 22, 23, 24},
            {30, 31, 32, 33, 34},
            {40, 41, 42, 43, 44}};
    private static final int SIZE_OPEN_NETWORK_RECOMMENDER_BLACKLIST = 10;
    private static final boolean IS_WIFI_NETWORKS_AVAILABLE_NOTIFICATION_ON = true;
    private static final int NUM_OPEN_NETWORK_CONNECT_MESSAGE_FAILED_TO_SEND = 5;
    private static final int NUM_OPEN_NETWORK_RECOMMENDATION_UPDATES = 8;
    private static final String OPEN_NET_NOTIFIER_TAG = OpenNetworkNotifier.TAG;

    private static final int NUM_SOFT_AP_EVENT_ENTRIES = 3;
    private static final int NUM_SOFT_AP_ASSOCIATED_STATIONS = 3;
    private static final int SOFT_AP_CHANNEL_FREQUENCY = 2437;
    private static final int SOFT_AP_CHANNEL_BANDWIDTH = SoftApConnectedClientsEvent.BANDWIDTH_20;
    private static final boolean IS_MAC_RANDOMIZATION_ON = true;
    private static final int NUM_LINK_SPEED_LEVELS_TO_INCREMENT = 30;
    private static final int TEST_RSSI_LEVEL = -80;

    private ScanDetail buildMockScanDetail(boolean hidden, NetworkDetail.HSRelease hSRelease,
            String capabilities) {
        ScanDetail mockScanDetail = mock(ScanDetail.class);
        NetworkDetail mockNetworkDetail = mock(NetworkDetail.class);
        ScanResult mockScanResult = mock(ScanResult.class);
        when(mockScanDetail.getNetworkDetail()).thenReturn(mockNetworkDetail);
        when(mockScanDetail.getScanResult()).thenReturn(mockScanResult);
        when(mockNetworkDetail.isHiddenBeaconFrame()).thenReturn(hidden);
        when(mockNetworkDetail.getHSRelease()).thenReturn(hSRelease);
        mockScanResult.capabilities = capabilities;
        return mockScanDetail;
    }

    private ScanDetail buildMockScanDetail(String ssid, String bssid, boolean isOpen,
            boolean isSaved, boolean isProvider, boolean isWeakRssi) {
        ScanDetail mockScanDetail = mock(ScanDetail.class);
        NetworkDetail mockNetworkDetail = mock(NetworkDetail.class);
        ScanResult scanResult = new ScanResult();
        scanResult.SSID = ssid;
        scanResult.BSSID = bssid;
        when(mockScanDetail.getNetworkDetail()).thenReturn(mockNetworkDetail);
        when(mockScanDetail.getScanResult()).thenReturn(scanResult);
        when(mWns.isSignalTooWeak(eq(scanResult))).thenReturn(isWeakRssi);
        scanResult.capabilities = isOpen ? "" : "PSK";
        if (isSaved) {
            when(mWcm.getConfiguredNetworkForScanDetail(eq(mockScanDetail)))
                    .thenReturn(mock(WifiConfiguration.class));
        }
        if (isProvider) {
            PasspointProvider provider = mock(PasspointProvider.class);
            Pair<PasspointProvider, PasspointMatch> providerMatch = Pair.create(provider, null);
            when(mockNetworkDetail.isInterworking()).thenReturn(true);
            when(mPpm.matchProvider(eq(scanResult))).thenReturn(providerMatch);
        }
        return mockScanDetail;
    }

    private ScanDetail buildMockScanDetailPasspoint(String ssid, String bssid, long hessid,
            int anqpDomainId, NetworkDetail.HSRelease hsRelease, boolean weakSignal) {
        ScanDetail mockScanDetail = mock(ScanDetail.class);
        NetworkDetail mockNetworkDetail = mock(NetworkDetail.class);
        ScanResult scanResult = new ScanResult();
        scanResult.SSID = ssid;
        scanResult.BSSID = bssid;
        scanResult.hessid = hessid;
        scanResult.capabilities = "PSK";
        when(mockScanDetail.getNetworkDetail()).thenReturn(mockNetworkDetail);
        when(mockScanDetail.getScanResult()).thenReturn(scanResult);
        when(mockNetworkDetail.getHSRelease()).thenReturn(hsRelease);
        when(mockNetworkDetail.getAnqpDomainID()).thenReturn(anqpDomainId);
        when(mockNetworkDetail.isInterworking()).thenReturn(true);
        when(mWns.isSignalTooWeak(eq(scanResult))).thenReturn(weakSignal);
        return mockScanDetail;
    }

    private List<ScanDetail> buildMockScanDetailList() {
        List<ScanDetail> mockScanDetails = new ArrayList<ScanDetail>();
        mockScanDetails.add(buildMockScanDetail(true, null, "[ESS]"));
        mockScanDetails.add(buildMockScanDetail(false, null, "[WPA2-PSK-CCMP][ESS]"));
        mockScanDetails.add(buildMockScanDetail(false, null, "[WPA-PSK-CCMP]"));
        mockScanDetails.add(buildMockScanDetail(false, null, "[WPA2-SAE-CCMP]"));
        mockScanDetails.add(buildMockScanDetail(false, null, "[WPA-PSK-CCMP]"));
        mockScanDetails.add(buildMockScanDetail(false, null, "[WEP]"));
        mockScanDetails.add(buildMockScanDetail(false, null, "[WPA2-SAE-CCMP]"));
        mockScanDetails.add(buildMockScanDetail(false, null, "[WPA2-OWE-CCMP]"));
        mockScanDetails.add(buildMockScanDetail(false, null, "[WPA2-EAP-SUITE-B-192]"));
        mockScanDetails.add(buildMockScanDetail(false, NetworkDetail.HSRelease.R2,
                "[WPA-EAP-CCMP]"));
        mockScanDetails.add(buildMockScanDetail(false, NetworkDetail.HSRelease.R2,
                "[WPA2-EAP+FT/EAP-CCMP]"));
        mockScanDetails.add(buildMockScanDetail(false, NetworkDetail.HSRelease.R1,
                "[WPA-EAP-CCMP]"));
        return mockScanDetails;
    }

    private List<WifiConfiguration> buildSavedNetworkList() {
        List<WifiConfiguration> testSavedNetworks = new ArrayList<WifiConfiguration>();
        for (int i = 0; i < NUM_OPEN_NETWORKS; i++) {
            testSavedNetworks.add(WifiConfigurationTestUtil.createOpenHiddenNetwork());
        }
        for (int i = 0; i < NUM_LEGACY_PERSONAL_NETWORKS; i++) {
            testSavedNetworks.add(WifiConfigurationTestUtil.createPskNetwork());
        }
        for (int i = 0; i < NUM_LEGACY_ENTERPRISE_NETWORKS; i++) {
            // Passpoint networks are counted in both Passpoint and Enterprise counters
            testSavedNetworks.add(WifiConfigurationTestUtil.createPasspointNetwork());
        }
        for (int i = 0; i < NUM_ENHANCED_OPEN_NETWORKS; i++) {
            testSavedNetworks.add(WifiConfigurationTestUtil.createOweNetwork());
        }
        for (int i = 0; i < NUM_WPA3_PERSONAL_NETWORKS; i++) {
            testSavedNetworks.add(WifiConfigurationTestUtil.createSaeNetwork());
        }
        for (int i = 0; i < NUM_WPA3_ENTERPRISE_NETWORKS; i++) {
            testSavedNetworks.add(WifiConfigurationTestUtil.createEapSuiteBNetwork());
        }
        testSavedNetworks.get(0).selfAdded = true;
        testSavedNetworks.get(0).macRandomizationSetting = WifiConfiguration.RANDOMIZATION_NONE;
        return testSavedNetworks;
    }

    private PasspointProvider createMockProvider(int eapType, boolean validateForR2) {
        PasspointProvider provider = mock(PasspointProvider.class);
        PasspointConfiguration config = mock(PasspointConfiguration.class);
        Credential credential = new Credential();

        switch (eapType) {
            case EAPConstants.EAP_TLS:
                credential.setCertCredential(new Credential.CertificateCredential());
                break;
            case EAPConstants.EAP_TTLS:
                credential.setUserCredential(new Credential.UserCredential());
                break;
            case EAPConstants.EAP_AKA:
            case EAPConstants.EAP_AKA_PRIME:
            case EAPConstants.EAP_SIM:
                Credential.SimCredential simCredential = new Credential.SimCredential();
                simCredential.setEapType(eapType);
                credential.setSimCredential(simCredential);
                break;
        }
        when(provider.getConfig()).thenReturn(config);
        when(config.getCredential()).thenReturn(credential);
        when(config.validateForR2()).thenReturn(validateForR2);
        return provider;
    }

    /**
     * Set simple metrics, increment others
     */
    public void setAndIncrementMetrics() throws Exception {
        Map<String, PasspointProvider> providers = new HashMap<>();
        mWifiMetrics.updateSavedNetworks(buildSavedNetworkList());
        mWifiMetrics.updateSavedPasspointProfiles(NUM_PASSPOINT_PROVIDERS,
                NUM_PASSPOINT_PROVIDERS_SUCCESSFULLY_CONNECTED);
        for (int i = 0; i < SAVED_PASSPOINT_PROVIDERS_TYPE.size(); i++) {
            int eapType = SAVED_PASSPOINT_PROVIDERS_TYPE.keyAt(i);
            int count = SAVED_PASSPOINT_PROVIDERS_TYPE.valueAt(i);
            for (int j = 0; j < count; j++) {
                providers.put(Integer.toString(eapType) + j, createMockProvider(eapType, false));
            }
            for (int j = count; j < count * 2; j++) {
                providers.put(Integer.toString(eapType) + j, createMockProvider(eapType, true));
            }
        }
        mWifiMetrics.updateSavedPasspointProfilesInfo(providers);

        mWifiMetrics.setIsLocationEnabled(TEST_VAL_IS_LOCATION_ENABLED);
        mWifiMetrics.setIsScanningAlwaysEnabled(IS_SCANNING_ALWAYS_ENABLED);

        for (int i = 0; i < NUM_EMPTY_SCAN_RESULTS; i++) {
            mWifiMetrics.incrementEmptyScanResultCount();
        }
        for (int i = 0; i < NUM_NON_EMPTY_SCAN_RESULTS; i++) {
            mWifiMetrics.incrementNonEmptyScanResultCount();
        }
        mWifiMetrics.incrementScanReturnEntry(WifiMetricsProto.WifiLog.SCAN_UNKNOWN,
                NUM_SCAN_UNKNOWN);
        mWifiMetrics.incrementScanReturnEntry(WifiMetricsProto.WifiLog.SCAN_SUCCESS,
                NUM_SCAN_SUCCESS);
        mWifiMetrics.incrementScanReturnEntry(
                WifiMetricsProto.WifiLog.SCAN_FAILURE_INTERRUPTED,
                NUM_SCAN_FAILURE_INTERRUPTED);
        mWifiMetrics.incrementScanReturnEntry(
                WifiMetricsProto.WifiLog.SCAN_FAILURE_INVALID_CONFIGURATION,
                NUM_SCAN_FAILURE_INVALID_CONFIGURATION);
        for (int i = 0; i < NUM_WIFI_UNKNOWN_SCREEN_OFF; i++) {
            mWifiMetrics.incrementWifiSystemScanStateCount(WifiMetricsProto.WifiLog.WIFI_UNKNOWN,
                    false);
        }
        for (int i = 0; i < NUM_WIFI_UNKNOWN_SCREEN_ON; i++) {
            mWifiMetrics.incrementWifiSystemScanStateCount(WifiMetricsProto.WifiLog.WIFI_UNKNOWN,
                    true);
        }
        for (int i = 0; i < NUM_WIFI_ASSOCIATED_SCREEN_OFF; i++) {
            mWifiMetrics.incrementWifiSystemScanStateCount(WifiMetricsProto.WifiLog.WIFI_ASSOCIATED,
                    false);
        }
        for (int i = 0; i < NUM_WIFI_ASSOCIATED_SCREEN_ON; i++) {
            mWifiMetrics.incrementWifiSystemScanStateCount(WifiMetricsProto.WifiLog.WIFI_ASSOCIATED,
                    true);
        }
        for (int i = 0; i < NUM_CONNECTIVITY_WATCHDOG_PNO_GOOD; i++) {
            mWifiMetrics.incrementNumConnectivityWatchdogPnoGood();
        }
        for (int i = 0; i < NUM_CONNECTIVITY_WATCHDOG_PNO_BAD; i++) {
            mWifiMetrics.incrementNumConnectivityWatchdogPnoBad();
        }
        for (int i = 0; i < NUM_CONNECTIVITY_WATCHDOG_BACKGROUND_GOOD; i++) {
            mWifiMetrics.incrementNumConnectivityWatchdogBackgroundGood();
        }
        for (int i = 0; i < NUM_CONNECTIVITY_WATCHDOG_BACKGROUND_BAD; i++) {
            mWifiMetrics.incrementNumConnectivityWatchdogBackgroundBad();
        }
        for (int i = 0; i < NUM_LAST_RESORT_WATCHDOG_TRIGGERS; i++) {
            mWifiMetrics.incrementNumLastResortWatchdogTriggers();
        }
        mWifiMetrics.addCountToNumLastResortWatchdogBadAssociationNetworksTotal(
                NUM_LAST_RESORT_WATCHDOG_BAD_ASSOCIATION_NETWORKS_TOTAL);
        mWifiMetrics.addCountToNumLastResortWatchdogBadAuthenticationNetworksTotal(
                NUM_LAST_RESORT_WATCHDOG_BAD_AUTHENTICATION_NETWORKS_TOTAL);
        mWifiMetrics.addCountToNumLastResortWatchdogBadDhcpNetworksTotal(
                NUM_LAST_RESORT_WATCHDOG_BAD_DHCP_NETWORKS_TOTAL);
        mWifiMetrics.addCountToNumLastResortWatchdogBadOtherNetworksTotal(
                NUM_LAST_RESORT_WATCHDOG_BAD_OTHER_NETWORKS_TOTAL);
        mWifiMetrics.addCountToNumLastResortWatchdogAvailableNetworksTotal(
                NUM_LAST_RESORT_WATCHDOG_AVAILABLE_NETWORKS_TOTAL);
        for (int i = 0; i < NUM_LAST_RESORT_WATCHDOG_TRIGGERS_WITH_BAD_ASSOCIATION; i++) {
            mWifiMetrics.incrementNumLastResortWatchdogTriggersWithBadAssociation();
        }
        for (int i = 0; i < NUM_LAST_RESORT_WATCHDOG_TRIGGERS_WITH_BAD_AUTHENTICATION; i++) {
            mWifiMetrics.incrementNumLastResortWatchdogTriggersWithBadAuthentication();
        }
        for (int i = 0; i < NUM_LAST_RESORT_WATCHDOG_TRIGGERS_WITH_BAD_DHCP; i++) {
            mWifiMetrics.incrementNumLastResortWatchdogTriggersWithBadDhcp();
        }
        for (int i = 0; i < NUM_LAST_RESORT_WATCHDOG_TRIGGERS_WITH_BAD_OTHER; i++) {
            mWifiMetrics.incrementNumLastResortWatchdogTriggersWithBadOther();
        }
        for (int i = 0; i < NUM_LAST_RESORT_WATCHDOG_SUCCESSES; i++) {
            mWifiMetrics.incrementNumLastResortWatchdogSuccesses();
        }
        for (int i = 0; i < WATCHDOG_TOTAL_CONNECTION_FAILURE_COUNT_AFTER_TRIGGER; i++) {
            mWifiMetrics.incrementWatchdogTotalConnectionFailureCountAfterTrigger();
        }
        for (int i = 0; i < NUM_RSSI_LEVELS_TO_INCREMENT; i++) {
            for (int j = 0; j <= i; j++) {
                mWifiMetrics.incrementRssiPollRssiCount(RSSI_POLL_FREQUENCY, MIN_RSSI_LEVEL + i);
            }
        }
        for (int i = 1; i < NUM_OUT_OF_BOUND_ENTRIES; i++) {
            mWifiMetrics.incrementRssiPollRssiCount(RSSI_POLL_FREQUENCY, MIN_RSSI_LEVEL - i);
        }
        for (int i = 1; i < NUM_OUT_OF_BOUND_ENTRIES; i++) {
            mWifiMetrics.incrementRssiPollRssiCount(RSSI_POLL_FREQUENCY, MAX_RSSI_LEVEL + i);
        }

        // Test alert-reason clamping.
        mWifiMetrics.logFirmwareAlert(WifiLoggerHal.WIFI_ALERT_REASON_MIN - 1);
        mWifiMetrics.logFirmwareAlert(WifiLoggerHal.WIFI_ALERT_REASON_MAX + 1);
        // Simple cases for alert reason.
        mWifiMetrics.logFirmwareAlert(1);
        mWifiMetrics.logFirmwareAlert(1);
        mWifiMetrics.logFirmwareAlert(1);
        mWifiMetrics.logFirmwareAlert(2);
        List<ScanDetail> mockScanDetails = buildMockScanDetailList();
        for (int i = 0; i < NUM_SCANS; i++) {
            mWifiMetrics.countScanResults(mockScanDetails);
        }
        // increment connectivity scan metrics
        for (int i = 0; i < NUM_CONNECTIVITY_ONESHOT_SCAN_EVENT; i++) {
            mWifiMetrics.incrementConnectivityOneshotScanCount();
        }
        for (int i = 0; i < NUM_EXTERNAL_APP_ONESHOT_SCAN_REQUESTS; i++) {
            mWifiMetrics.incrementExternalAppOneshotScanRequestsCount();
        }
        for (int i = 0; i < NUM_EXTERNAL_FOREGROUND_APP_ONESHOT_SCAN_REQUESTS_THROTTLED; i++) {
            mWifiMetrics.incrementExternalForegroundAppOneshotScanRequestsThrottledCount();
        }
        for (int i = 0; i < NUM_EXTERNAL_BACKGROUND_APP_ONESHOT_SCAN_REQUESTS_THROTTLED; i++) {
            mWifiMetrics.incrementExternalBackgroundAppOneshotScanRequestsThrottledCount();
        }
        for (int score = 0; score < NUM_WIFI_SCORES_TO_INCREMENT; score++) {
            for (int offset = 0; offset <= score; offset++) {
                mWifiMetrics.incrementWifiScoreCount(WIFI_SCORE_RANGE_MIN + score);
            }
        }
        for (int i = 1; i < NUM_OUT_OF_BOUND_ENTRIES; i++) {
            mWifiMetrics.incrementWifiScoreCount(WIFI_SCORE_RANGE_MIN - i);
        }
        for (int i = 1; i < NUM_OUT_OF_BOUND_ENTRIES; i++) {
            mWifiMetrics.incrementWifiScoreCount(WIFI_SCORE_RANGE_MAX + i);
        }
        for (int score = 0; score < NUM_WIFI_SCORES_TO_INCREMENT; score++) {
            for (int offset = 0; offset <= score; offset++) {
                mWifiMetrics.incrementWifiUsabilityScoreCount(1, WIFI_SCORE_RANGE_MIN + score, 15);
            }
        }
        for (int i = 1; i < NUM_OUT_OF_BOUND_ENTRIES; i++) {
            mWifiMetrics.incrementWifiUsabilityScoreCount(1, WIFI_SCORE_RANGE_MIN - i, 15);
        }
        for (int i = 1; i < NUM_OUT_OF_BOUND_ENTRIES; i++) {
            mWifiMetrics.incrementWifiUsabilityScoreCount(1, WIFI_SCORE_RANGE_MAX + i, 15);
        }

        // increment soft ap start return codes
        for (int i = 0; i < NUM_SOFTAP_START_SUCCESS; i++) {
            mWifiMetrics.incrementSoftApStartResult(true, 0);
        }
        for (int i = 0; i < NUM_SOFTAP_FAILED_GENERAL_ERROR; i++) {
            mWifiMetrics.incrementSoftApStartResult(false, WifiManager.SAP_START_FAILURE_GENERAL);
        }
        for (int i = 0; i < NUM_SOFTAP_FAILED_NO_CHANNEL; i++) {
            mWifiMetrics.incrementSoftApStartResult(false,
                    WifiManager.SAP_START_FAILURE_NO_CHANNEL);
        }
        for (int i = 0; i < NUM_HAL_CRASHES; i++) {
            mWifiMetrics.incrementNumHalCrashes();
        }
        for (int i = 0; i < NUM_WIFICOND_CRASHES; i++) {
            mWifiMetrics.incrementNumWificondCrashes();
        }
        for (int i = 0; i < NUM_SUPPLICANT_CRASHES; i++) {
            mWifiMetrics.incrementNumSupplicantCrashes();
        }
        for (int i = 0; i < NUM_HOSTAPD_CRASHES; i++) {
            mWifiMetrics.incrementNumHostapdCrashes();
        }
        for (int i = 0; i < NUM_WIFI_ON_FAILURE_DUE_TO_HAL; i++) {
            mWifiMetrics.incrementNumSetupClientInterfaceFailureDueToHal();
        }
        for (int i = 0; i < NUM_WIFI_ON_FAILURE_DUE_TO_WIFICOND; i++) {
            mWifiMetrics.incrementNumSetupClientInterfaceFailureDueToWificond();
        }
        for (int i = 0; i < NUM_WIFI_ON_FAILURE_DUE_TO_SUPPLICANT; i++) {
            mWifiMetrics.incrementNumSetupClientInterfaceFailureDueToSupplicant();
        }
        for (int i = 0; i < NUM_SOFTAP_ON_FAILURE_DUE_TO_HAL; i++) {
            mWifiMetrics.incrementNumSetupSoftApInterfaceFailureDueToHal();
        }
        for (int i = 0; i < NUM_SOFTAP_ON_FAILURE_DUE_TO_WIFICOND; i++) {
            mWifiMetrics.incrementNumSetupSoftApInterfaceFailureDueToWificond();
        }
        for (int i = 0; i < NUM_SOFTAP_ON_FAILURE_DUE_TO_HOSTAPD; i++) {
            mWifiMetrics.incrementNumSetupSoftApInterfaceFailureDueToHostapd();
        }
        for (int i = 0; i < NUM_SOFTAP_INTERFACE_DOWN; i++) {
            mWifiMetrics.incrementNumSoftApInterfaceDown();
        }
        for (int i = 0; i < NUM_CLIENT_INTERFACE_DOWN; i++) {
            mWifiMetrics.incrementNumClientInterfaceDown();
        }
        for (int i = 0; i < NUM_PASSPOINT_PROVIDER_INSTALLATION; i++) {
            mWifiMetrics.incrementNumPasspointProviderInstallation();
        }
        for (int i = 0; i < NUM_PASSPOINT_PROVIDER_INSTALL_SUCCESS; i++) {
            mWifiMetrics.incrementNumPasspointProviderInstallSuccess();
        }
        for (int i = 0; i < NUM_PASSPOINT_PROVIDER_UNINSTALLATION; i++) {
            mWifiMetrics.incrementNumPasspointProviderUninstallation();
        }
        for (int i = 0; i < NUM_PASSPOINT_PROVIDER_UNINSTALL_SUCCESS; i++) {
            mWifiMetrics.incrementNumPasspointProviderUninstallSuccess();
        }
        for (int i = 0; i < NUM_RADIO_MODE_CHANGE_TO_MCC; i++) {
            mWifiMetrics.incrementNumRadioModeChangeToMcc();
        }
        for (int i = 0; i < NUM_RADIO_MODE_CHANGE_TO_SCC; i++) {
            mWifiMetrics.incrementNumRadioModeChangeToScc();
        }
        for (int i = 0; i < NUM_RADIO_MODE_CHANGE_TO_SBS; i++) {
            mWifiMetrics.incrementNumRadioModeChangeToSbs();
        }
        for (int i = 0; i < NUM_RADIO_MODE_CHANGE_TO_DBS; i++) {
            mWifiMetrics.incrementNumRadioModeChangeToDbs();
        }
        for (int i = 0; i < NUM_SOFTAP_USER_BAND_PREFERENCE_UNSATISFIED; i++) {
            mWifiMetrics.incrementNumSoftApUserBandPreferenceUnsatisfied();
        }

        // increment pno scan metrics
        for (int i = 0; i < NUM_PNO_SCAN_ATTEMPTS; i++) {
            mWifiMetrics.incrementPnoScanStartAttempCount();
        }
        for (int i = 0; i < NUM_PNO_SCAN_FAILED; i++) {
            mWifiMetrics.incrementPnoScanFailedCount();
        }
        for (int i = 0; i < NUM_PNO_SCAN_STARTED_OVER_OFFLOAD; i++) {
            mWifiMetrics.incrementPnoScanStartedOverOffloadCount();
        }
        for (int i = 0; i < NUM_PNO_SCAN_FAILED_OVER_OFFLOAD; i++) {
            mWifiMetrics.incrementPnoScanFailedOverOffloadCount();
        }
        for (int i = 0; i < NUM_PNO_FOUND_NETWORK_EVENTS; i++) {
            mWifiMetrics.incrementPnoFoundNetworkEventCount();
        }

        // set and increment "connect to network" notification metrics
        for (int i = 0; i < NUM_CONNECT_TO_NETWORK_NOTIFICATIONS.length; i++) {
            int count = NUM_CONNECT_TO_NETWORK_NOTIFICATIONS[i];
            for (int j = 0; j < count; j++) {
                mWifiMetrics.incrementConnectToNetworkNotification(OPEN_NET_NOTIFIER_TAG, i);
            }
        }
        for (int i = 0; i < NUM_CONNECT_TO_NETWORK_NOTIFICATION_ACTIONS.length; i++) {
            int[] actions = NUM_CONNECT_TO_NETWORK_NOTIFICATION_ACTIONS[i];
            for (int j = 0; j < actions.length; j++) {
                int count = actions[j];
                for (int k = 0; k < count; k++) {
                    mWifiMetrics.incrementConnectToNetworkNotificationAction(OPEN_NET_NOTIFIER_TAG,
                            i, j);
                }
            }
        }
        mWifiMetrics.setNetworkRecommenderBlacklistSize(OPEN_NET_NOTIFIER_TAG,
                SIZE_OPEN_NETWORK_RECOMMENDER_BLACKLIST);
        mWifiMetrics.setIsWifiNetworksAvailableNotificationEnabled(OPEN_NET_NOTIFIER_TAG,
                IS_WIFI_NETWORKS_AVAILABLE_NOTIFICATION_ON);
        for (int i = 0; i < NUM_OPEN_NETWORK_RECOMMENDATION_UPDATES; i++) {
            mWifiMetrics.incrementNumNetworkRecommendationUpdates(OPEN_NET_NOTIFIER_TAG);
        }
        for (int i = 0; i < NUM_OPEN_NETWORK_CONNECT_MESSAGE_FAILED_TO_SEND; i++) {
            mWifiMetrics.incrementNumNetworkConnectMessageFailedToSend(OPEN_NET_NOTIFIER_TAG);
        }

        addSoftApEventsToMetrics();

        // increment wps metrics
        for (int i = 0; i < NUM_WPS_ATTEMPTS; i++) {
            mWifiMetrics.incrementWpsAttemptCount();
        }
        for (int i = 0; i < NUM_WPS_SUCCESS; i++) {
            mWifiMetrics.incrementWpsSuccessCount();
        }
        for (int i = 0; i < NUM_WPS_START_FAILURE; i++) {
            mWifiMetrics.incrementWpsStartFailureCount();
        }
        for (int i = 0; i < NUM_WPS_OVERLAP_FAILURE; i++) {
            mWifiMetrics.incrementWpsOverlapFailureCount();
        }
        for (int i = 0; i < NUM_WPS_TIMEOUT_FAILURE; i++) {
            mWifiMetrics.incrementWpsTimeoutFailureCount();
        }
        for (int i = 0; i < NUM_WPS_OTHER_CONNECTION_FAILURE; i++) {
            mWifiMetrics.incrementWpsOtherConnectionFailureCount();
        }
        for (int i = 0; i < NUM_WPS_SUPPLICANT_FAILURE; i++) {
            mWifiMetrics.incrementWpsSupplicantFailureCount();
        }
        for (int i = 0; i < NUM_WPS_CANCELLATION; i++) {
            mWifiMetrics.incrementWpsCancellationCount();
        }
        for (int i = 0; i < NUM_SAR_SENSOR_LISTENER_REGISTRATION_FAILURES; i++) {
            mWifiMetrics.incrementNumSarSensorRegistrationFailures();
        }
        for (int i = 0; i < NUM_ONESHOT_SCAN_REQUESTS_WITH_DFS_CHANNELS; i++) {
            mWifiMetrics.incrementOneshotScanWithDfsCount();
        }
        for (int i = 0; i < NUM_ADD_OR_UPDATE_NETWORK_CALLS; i++) {
            mWifiMetrics.incrementNumAddOrUpdateNetworkCalls();
        }
        for (int i = 0; i < NUM_ENABLE_NETWORK_CALLS; i++) {
            mWifiMetrics.incrementNumEnableNetworkCalls();
        }

        mWifiMetrics.setWatchdogSuccessTimeDurationMs(NUM_WATCHDOG_SUCCESS_DURATION_MS);
        mWifiMetrics.setIsMacRandomizationOn(IS_MAC_RANDOMIZATION_ON);

        addWifiPowerMetrics();

        mWifiMetrics.setWifiIsUnusableLoggingEnabled(WIFI_IS_UNUSABLE_EVENT_LOGGING_SETTING);
        mWifiMetrics.setLinkSpeedCountsLoggingEnabled(LINK_SPEED_COUNTS_LOGGING_SETTING);
        mWifiMetrics.setWifiDataStallMinTxBad(DATA_STALL_MIN_TX_BAD_SETTING);
        mWifiMetrics.setWifiDataStallMinRxWithoutTx(DATA_STALL_MIN_TX_SUCCESS_WITHOUT_RX_SETTING);
    }

    private void addWifiPowerMetrics() {
        WifiRadioUsage wifiRadioUsage = new WifiRadioUsage();
        wifiRadioUsage.loggingDurationMs = WIFI_POWER_METRICS_LOGGING_DURATION;
        wifiRadioUsage.scanTimeMs = WIFI_POWER_METRICS_SCAN_TIME;
        when(mWifiPowerMetrics.buildWifiRadioUsageProto()).thenReturn(wifiRadioUsage);
    }

    private void addSoftApEventsToMetrics() {
        // Total number of events recorded is NUM_SOFT_AP_EVENT_ENTRIES in both modes

        mWifiMetrics.addSoftApUpChangedEvent(true, WifiManager.IFACE_IP_MODE_TETHERED);
        mWifiMetrics.addSoftApNumAssociatedStationsChangedEvent(NUM_SOFT_AP_ASSOCIATED_STATIONS,
                WifiManager.IFACE_IP_MODE_TETHERED);
        mWifiMetrics.addSoftApNumAssociatedStationsChangedEvent(NUM_SOFT_AP_ASSOCIATED_STATIONS,
                WifiManager.IFACE_IP_MODE_UNSPECIFIED);  // Should be dropped.
        mWifiMetrics.addSoftApUpChangedEvent(false, WifiManager.IFACE_IP_MODE_TETHERED);
        // Channel switch info should be added to the last Soft AP UP event in the list
        mWifiMetrics.addSoftApChannelSwitchedEvent(SOFT_AP_CHANNEL_FREQUENCY,
                SOFT_AP_CHANNEL_BANDWIDTH, WifiManager.IFACE_IP_MODE_TETHERED);
        mWifiMetrics.addSoftApUpChangedEvent(true, WifiManager.IFACE_IP_MODE_LOCAL_ONLY);
        mWifiMetrics.addSoftApNumAssociatedStationsChangedEvent(NUM_SOFT_AP_ASSOCIATED_STATIONS,
                WifiManager.IFACE_IP_MODE_LOCAL_ONLY);
        // Should be dropped.
        mWifiMetrics.addSoftApUpChangedEvent(false, WifiManager.IFACE_IP_MODE_CONFIGURATION_ERROR);
        mWifiMetrics.addSoftApUpChangedEvent(false, WifiManager.IFACE_IP_MODE_LOCAL_ONLY);
    }

    private void verifySoftApEventsStoredInProto() {
        assertEquals(NUM_SOFT_AP_EVENT_ENTRIES,
                mDecodedProto.softApConnectedClientsEventsTethered.length);
        assertEquals(SoftApConnectedClientsEvent.SOFT_AP_UP,
                mDecodedProto.softApConnectedClientsEventsTethered[0].eventType);
        assertEquals(0, mDecodedProto.softApConnectedClientsEventsTethered[0].numConnectedClients);
        assertEquals(SOFT_AP_CHANNEL_FREQUENCY,
                mDecodedProto.softApConnectedClientsEventsTethered[0].channelFrequency);
        assertEquals(SOFT_AP_CHANNEL_BANDWIDTH,
                mDecodedProto.softApConnectedClientsEventsTethered[0].channelBandwidth);
        assertEquals(SoftApConnectedClientsEvent.NUM_CLIENTS_CHANGED,
                mDecodedProto.softApConnectedClientsEventsTethered[1].eventType);
        assertEquals(NUM_SOFT_AP_ASSOCIATED_STATIONS,
                mDecodedProto.softApConnectedClientsEventsTethered[1].numConnectedClients);
        assertEquals(SoftApConnectedClientsEvent.SOFT_AP_DOWN,
                mDecodedProto.softApConnectedClientsEventsTethered[2].eventType);
        assertEquals(0, mDecodedProto.softApConnectedClientsEventsTethered[2].numConnectedClients);

        assertEquals(SoftApConnectedClientsEvent.SOFT_AP_UP,
                mDecodedProto.softApConnectedClientsEventsLocalOnly[0].eventType);
        assertEquals(0, mDecodedProto.softApConnectedClientsEventsLocalOnly[0].numConnectedClients);
        assertEquals(SoftApConnectedClientsEvent.NUM_CLIENTS_CHANGED,
                mDecodedProto.softApConnectedClientsEventsLocalOnly[1].eventType);
        assertEquals(NUM_SOFT_AP_ASSOCIATED_STATIONS,
                mDecodedProto.softApConnectedClientsEventsLocalOnly[1].numConnectedClients);
        assertEquals(SoftApConnectedClientsEvent.SOFT_AP_DOWN,
                mDecodedProto.softApConnectedClientsEventsLocalOnly[2].eventType);
        assertEquals(0, mDecodedProto.softApConnectedClientsEventsLocalOnly[2].numConnectedClients);
    }

    /**
     * Assert that values in deserializedWifiMetrics match those set in 'setAndIncrementMetrics'
     */
    public void assertDeserializedMetricsCorrect() throws Exception {
        assertEquals("mDecodedProto.numSavedNetworks == NUM_SAVED_NETWORKS",
                NUM_SAVED_NETWORKS, mDecodedProto.numSavedNetworks);
        assertEquals("mDecodedProto.numSavedNetworksWithMacRandomization == NUM_SAVED_NETWORKS-1",
                NUM_SAVED_NETWORKS - 1, mDecodedProto.numSavedNetworksWithMacRandomization);
        assertEquals("mDecodedProto.numOpenNetworks == NUM_OPEN_NETWORKS",
                NUM_OPEN_NETWORKS, mDecodedProto.numOpenNetworks);
        assertEquals("mDecodedProto.numLegacyPersonalNetworks == NUM_LEGACY_PERSONAL_NETWORKS",
                NUM_LEGACY_PERSONAL_NETWORKS, mDecodedProto.numLegacyPersonalNetworks);
        assertEquals(
                "mDecodedProto.numLegacyEnterpriseNetworks == NUM_LEGACY_ENTERPRISE_NETWORKS",
                NUM_LEGACY_ENTERPRISE_NETWORKS, mDecodedProto.numLegacyEnterpriseNetworks);
        assertEquals("mDecodedProto.numEnhancedOpenNetworks == NUM_ENHANCED_OPEN_NETWORKS",
                NUM_ENHANCED_OPEN_NETWORKS, mDecodedProto.numEnhancedOpenNetworks);
        assertEquals("mDecodedProto.numWpa3PersonalNetworks == NUM_WPA3_PERSONAL_NETWORKS",
                NUM_WPA3_PERSONAL_NETWORKS, mDecodedProto.numWpa3PersonalNetworks);
        assertEquals("mDecodedProto.numWpa3EnterpriseNetworks == NUM_WPA3_ENTERPRISE_NETWORKS",
                NUM_WPA3_ENTERPRISE_NETWORKS, mDecodedProto.numWpa3EnterpriseNetworks);
        assertEquals("mDecodedProto.numNetworksAddedByUser == NUM_NETWORKS_ADDED_BY_USER",
                NUM_NETWORKS_ADDED_BY_USER, mDecodedProto.numNetworksAddedByUser);
        assertEquals(NUM_HIDDEN_NETWORKS, mDecodedProto.numHiddenNetworks);
        assertEquals(NUM_PASSPOINT_NETWORKS, mDecodedProto.numPasspointNetworks);
        assertEquals("mDecodedProto.numNetworksAddedByApps == NUM_NETWORKS_ADDED_BY_APPS",
                NUM_NETWORKS_ADDED_BY_APPS, mDecodedProto.numNetworksAddedByApps);
        assertEquals("mDecodedProto.isLocationEnabled == TEST_VAL_IS_LOCATION_ENABLED",
                TEST_VAL_IS_LOCATION_ENABLED, mDecodedProto.isLocationEnabled);
        assertEquals("mDecodedProto.isScanningAlwaysEnabled == IS_SCANNING_ALWAYS_ENABLED",
                IS_SCANNING_ALWAYS_ENABLED, mDecodedProto.isScanningAlwaysEnabled);
        assertEquals("mDecodedProto.numEmptyScanResults == NUM_EMPTY_SCAN_RESULTS",
                NUM_EMPTY_SCAN_RESULTS, mDecodedProto.numEmptyScanResults);
        assertEquals("mDecodedProto.numNonEmptyScanResults == NUM_NON_EMPTY_SCAN_RESULTS",
                NUM_NON_EMPTY_SCAN_RESULTS, mDecodedProto.numNonEmptyScanResults);
        assertScanReturnEntryEquals(WifiMetricsProto.WifiLog.SCAN_UNKNOWN, NUM_SCAN_UNKNOWN);
        assertScanReturnEntryEquals(WifiMetricsProto.WifiLog.SCAN_SUCCESS, NUM_SCAN_SUCCESS);
        assertScanReturnEntryEquals(WifiMetricsProto.WifiLog.SCAN_FAILURE_INTERRUPTED,
                NUM_SCAN_FAILURE_INTERRUPTED);
        assertScanReturnEntryEquals(WifiMetricsProto.WifiLog.SCAN_FAILURE_INVALID_CONFIGURATION,
                NUM_SCAN_FAILURE_INVALID_CONFIGURATION);
        assertSystemStateEntryEquals(WifiMetricsProto.WifiLog.WIFI_UNKNOWN, false,
                NUM_WIFI_UNKNOWN_SCREEN_OFF);
        assertSystemStateEntryEquals(WifiMetricsProto.WifiLog.WIFI_UNKNOWN, true,
                NUM_WIFI_UNKNOWN_SCREEN_ON);
        assertSystemStateEntryEquals(
                WifiMetricsProto.WifiLog.WIFI_ASSOCIATED, false, NUM_WIFI_ASSOCIATED_SCREEN_OFF);
        assertSystemStateEntryEquals(WifiMetricsProto.WifiLog.WIFI_ASSOCIATED, true,
                NUM_WIFI_ASSOCIATED_SCREEN_ON);
        assertEquals(NUM_CONNECTIVITY_WATCHDOG_PNO_GOOD,
                mDecodedProto.numConnectivityWatchdogPnoGood);
        assertEquals(NUM_CONNECTIVITY_WATCHDOG_PNO_BAD,
                mDecodedProto.numConnectivityWatchdogPnoBad);
        assertEquals(NUM_CONNECTIVITY_WATCHDOG_BACKGROUND_GOOD,
                mDecodedProto.numConnectivityWatchdogBackgroundGood);
        assertEquals(NUM_CONNECTIVITY_WATCHDOG_BACKGROUND_BAD,
                mDecodedProto.numConnectivityWatchdogBackgroundBad);
        assertEquals(NUM_LAST_RESORT_WATCHDOG_TRIGGERS,
                mDecodedProto.numLastResortWatchdogTriggers);
        assertEquals(NUM_LAST_RESORT_WATCHDOG_BAD_ASSOCIATION_NETWORKS_TOTAL,
                mDecodedProto.numLastResortWatchdogBadAssociationNetworksTotal);
        assertEquals(NUM_LAST_RESORT_WATCHDOG_BAD_AUTHENTICATION_NETWORKS_TOTAL,
                mDecodedProto.numLastResortWatchdogBadAuthenticationNetworksTotal);
        assertEquals(NUM_LAST_RESORT_WATCHDOG_BAD_DHCP_NETWORKS_TOTAL,
                mDecodedProto.numLastResortWatchdogBadDhcpNetworksTotal);
        assertEquals(NUM_LAST_RESORT_WATCHDOG_BAD_OTHER_NETWORKS_TOTAL,
                mDecodedProto.numLastResortWatchdogBadOtherNetworksTotal);
        assertEquals(NUM_LAST_RESORT_WATCHDOG_AVAILABLE_NETWORKS_TOTAL,
                mDecodedProto.numLastResortWatchdogAvailableNetworksTotal);
        assertEquals(NUM_LAST_RESORT_WATCHDOG_TRIGGERS_WITH_BAD_ASSOCIATION,
                mDecodedProto.numLastResortWatchdogTriggersWithBadAssociation);
        assertEquals(NUM_LAST_RESORT_WATCHDOG_TRIGGERS_WITH_BAD_AUTHENTICATION,
                mDecodedProto.numLastResortWatchdogTriggersWithBadAuthentication);
        assertEquals(NUM_LAST_RESORT_WATCHDOG_TRIGGERS_WITH_BAD_DHCP,
                mDecodedProto.numLastResortWatchdogTriggersWithBadDhcp);
        assertEquals(NUM_LAST_RESORT_WATCHDOG_TRIGGERS_WITH_BAD_OTHER,
                mDecodedProto.numLastResortWatchdogTriggersWithBadOther);
        assertEquals(NUM_LAST_RESORT_WATCHDOG_SUCCESSES,
                mDecodedProto.numLastResortWatchdogSuccesses);
        assertEquals(WATCHDOG_TOTAL_CONNECTION_FAILURE_COUNT_AFTER_TRIGGER,
                mDecodedProto.watchdogTotalConnectionFailureCountAfterTrigger);
        assertEquals(TEST_RECORD_DURATION_SEC,
                mDecodedProto.recordDurationSec);
        for (int i = 0; i < NUM_RSSI_LEVELS_TO_INCREMENT; i++) {
            assertEquals(RSSI_POLL_FREQUENCY,
                    mDecodedProto.rssiPollRssiCount[i].frequency);
            assertEquals(MIN_RSSI_LEVEL + i, mDecodedProto.rssiPollRssiCount[i].rssi);
            assertEquals(i + 1, mDecodedProto.rssiPollRssiCount[i].count);
        }
        StringBuilder sb_rssi = new StringBuilder();
        sb_rssi.append("Number of RSSIs = " + mDecodedProto.rssiPollRssiCount.length);
        assertTrue(sb_rssi.toString(), (mDecodedProto.rssiPollRssiCount.length
                     <= (MAX_RSSI_LEVEL - MIN_RSSI_LEVEL + 1)));
        assertEquals(2, mDecodedProto.alertReasonCount[0].count);  // Clamped reasons.
        assertEquals(3, mDecodedProto.alertReasonCount[1].count);
        assertEquals(1, mDecodedProto.alertReasonCount[2].count);
        assertEquals(3, mDecodedProto.alertReasonCount.length);
        assertEquals(NUM_TOTAL_SCAN_RESULTS * NUM_SCANS,
                mDecodedProto.numTotalScanResults);
        assertEquals(NUM_OPEN_NETWORK_SCAN_RESULTS * NUM_SCANS,
                mDecodedProto.numOpenNetworkScanResults);
        assertEquals(NUM_LEGACY_PERSONAL_NETWORK_SCAN_RESULTS * NUM_SCANS,
                mDecodedProto.numLegacyPersonalNetworkScanResults);
        assertEquals(NUM_LEGACY_ENTERPRISE_NETWORK_SCAN_RESULTS * NUM_SCANS,
                mDecodedProto.numLegacyEnterpriseNetworkScanResults);
        assertEquals(NUM_ENHANCED_OPEN_NETWORK_SCAN_RESULTS * NUM_SCANS,
                mDecodedProto.numEnhancedOpenNetworkScanResults);
        assertEquals(NUM_WPA3_PERSONAL_NETWORK_SCAN_RESULTS * NUM_SCANS,
                mDecodedProto.numWpa3PersonalNetworkScanResults);
        assertEquals(NUM_WPA3_ENTERPRISE_NETWORK_SCAN_RESULTS * NUM_SCANS,
                mDecodedProto.numWpa3EnterpriseNetworkScanResults);
        assertEquals(NUM_HIDDEN_NETWORK_SCAN_RESULTS * NUM_SCANS,
                mDecodedProto.numHiddenNetworkScanResults);
        assertEquals(NUM_HOTSPOT2_R1_NETWORK_SCAN_RESULTS * NUM_SCANS,
                mDecodedProto.numHotspot2R1NetworkScanResults);
        assertEquals(NUM_HOTSPOT2_R2_NETWORK_SCAN_RESULTS * NUM_SCANS,
                mDecodedProto.numHotspot2R2NetworkScanResults);
        assertEquals(NUM_SCANS,
                mDecodedProto.numScans);
        assertEquals(NUM_CONNECTIVITY_ONESHOT_SCAN_EVENT,
                mDecodedProto.numConnectivityOneshotScans);
        assertEquals(NUM_EXTERNAL_APP_ONESHOT_SCAN_REQUESTS,
                mDecodedProto.numExternalAppOneshotScanRequests);
        assertEquals(NUM_EXTERNAL_FOREGROUND_APP_ONESHOT_SCAN_REQUESTS_THROTTLED,
                mDecodedProto.numExternalForegroundAppOneshotScanRequestsThrottled);
        assertEquals(NUM_EXTERNAL_BACKGROUND_APP_ONESHOT_SCAN_REQUESTS_THROTTLED,
                mDecodedProto.numExternalBackgroundAppOneshotScanRequestsThrottled);

        for (int score_index = 0; score_index < NUM_WIFI_SCORES_TO_INCREMENT; score_index++) {
            assertEquals(WIFI_SCORE_RANGE_MIN + score_index,
                    mDecodedProto.wifiScoreCount[score_index].score);
            assertEquals(WIFI_SCORE_RANGE_MIN + score_index + 1,
                    mDecodedProto.wifiScoreCount[score_index].count);
            assertEquals(WIFI_SCORE_RANGE_MIN + score_index,
                    mDecodedProto.wifiUsabilityScoreCount[score_index].score);
            assertEquals(WIFI_SCORE_RANGE_MIN + score_index + 1,
                    mDecodedProto.wifiUsabilityScoreCount[score_index].count);
        }
        StringBuilder sb_wifi_score = new StringBuilder();
        sb_wifi_score.append("Number of wifi_scores = " + mDecodedProto.wifiScoreCount.length);
        assertTrue(sb_wifi_score.toString(), (mDecodedProto.wifiScoreCount.length
                <= (WIFI_SCORE_RANGE_MAX - WIFI_SCORE_RANGE_MIN + 1)));
        StringBuilder sb_wifi_limits = new StringBuilder();
        sb_wifi_limits.append("Wifi Score limit is " +  NetworkAgent.WIFI_BASE_SCORE
                + ">= " + WIFI_SCORE_RANGE_MAX);
        assertTrue(sb_wifi_limits.toString(), NetworkAgent.WIFI_BASE_SCORE <= WIFI_SCORE_RANGE_MAX);
        StringBuilder sb_wifi_usability_score = new StringBuilder();
        sb_wifi_usability_score.append("Number of wifi_usability_scores = "
                + mDecodedProto.wifiUsabilityScoreCount.length);
        assertTrue(sb_wifi_usability_score.toString(), (mDecodedProto.wifiUsabilityScoreCount.length
                <= (WIFI_SCORE_RANGE_MAX - WIFI_SCORE_RANGE_MIN + 1)));
        StringBuilder sb_wifi_usablity_limits = new StringBuilder();
        sb_wifi_limits.append("Wifi Usability Score limit is " +  NetworkAgent.WIFI_BASE_SCORE
                + ">= " + WIFI_SCORE_RANGE_MAX);
        assertTrue(sb_wifi_limits.toString(), NetworkAgent.WIFI_BASE_SCORE <= WIFI_SCORE_RANGE_MAX);
        assertEquals(MAX_NUM_SOFTAP_RETURN_CODES, mDecodedProto.softApReturnCode.length);
        assertEquals(WifiMetricsProto.SoftApReturnCodeCount.SOFT_AP_STARTED_SUCCESSFULLY,
                     mDecodedProto.softApReturnCode[0].startResult);
        assertEquals(NUM_SOFTAP_START_SUCCESS, mDecodedProto.softApReturnCode[0].count);
        assertEquals(WifiMetricsProto.SoftApReturnCodeCount.SOFT_AP_FAILED_GENERAL_ERROR,
                     mDecodedProto.softApReturnCode[1].startResult);
        assertEquals(NUM_SOFTAP_FAILED_GENERAL_ERROR,
                     mDecodedProto.softApReturnCode[1].count);
        assertEquals(WifiMetricsProto.SoftApReturnCodeCount.SOFT_AP_FAILED_NO_CHANNEL,
                     mDecodedProto.softApReturnCode[2].startResult);
        assertEquals(NUM_SOFTAP_FAILED_NO_CHANNEL,
                     mDecodedProto.softApReturnCode[2].count);
        assertEquals(NUM_HAL_CRASHES, mDecodedProto.numHalCrashes);
        assertEquals(NUM_WIFICOND_CRASHES, mDecodedProto.numWificondCrashes);
        assertEquals(NUM_SUPPLICANT_CRASHES, mDecodedProto.numSupplicantCrashes);
        assertEquals(NUM_HOSTAPD_CRASHES, mDecodedProto.numHostapdCrashes);
        assertEquals(NUM_WIFI_ON_FAILURE_DUE_TO_HAL,
                mDecodedProto.numSetupClientInterfaceFailureDueToHal);
        assertEquals(NUM_WIFI_ON_FAILURE_DUE_TO_WIFICOND,
                mDecodedProto.numSetupClientInterfaceFailureDueToWificond);
        assertEquals(NUM_WIFI_ON_FAILURE_DUE_TO_SUPPLICANT,
                mDecodedProto.numSetupClientInterfaceFailureDueToSupplicant);
        assertEquals(NUM_SOFTAP_ON_FAILURE_DUE_TO_HAL,
                mDecodedProto.numSetupSoftApInterfaceFailureDueToHal);
        assertEquals(NUM_SOFTAP_ON_FAILURE_DUE_TO_WIFICOND,
                mDecodedProto.numSetupSoftApInterfaceFailureDueToWificond);
        assertEquals(NUM_SOFTAP_ON_FAILURE_DUE_TO_HOSTAPD,
                mDecodedProto.numSetupSoftApInterfaceFailureDueToHostapd);
        assertEquals(NUM_CLIENT_INTERFACE_DOWN, mDecodedProto.numClientInterfaceDown);
        assertEquals(NUM_SOFTAP_INTERFACE_DOWN, mDecodedProto.numSoftApInterfaceDown);
        assertEquals(NUM_PASSPOINT_PROVIDERS, mDecodedProto.numPasspointProviders);
        assertPasspointProfileTypeCount(mDecodedProto.installedPasspointProfileTypeForR1);
        assertPasspointProfileTypeCount(mDecodedProto.installedPasspointProfileTypeForR2);
        assertEquals(NUM_PASSPOINT_PROVIDER_INSTALLATION,
                mDecodedProto.numPasspointProviderInstallation);
        assertEquals(NUM_PASSPOINT_PROVIDER_INSTALL_SUCCESS,
                mDecodedProto.numPasspointProviderInstallSuccess);
        assertEquals(NUM_PASSPOINT_PROVIDER_UNINSTALLATION,
                mDecodedProto.numPasspointProviderUninstallation);
        assertEquals(NUM_PASSPOINT_PROVIDER_UNINSTALL_SUCCESS,
                mDecodedProto.numPasspointProviderUninstallSuccess);
        assertEquals(NUM_PASSPOINT_PROVIDERS_SUCCESSFULLY_CONNECTED,
                mDecodedProto.numPasspointProvidersSuccessfullyConnected);
        assertEquals(NUM_RADIO_MODE_CHANGE_TO_MCC, mDecodedProto.numRadioModeChangeToMcc);
        assertEquals(NUM_RADIO_MODE_CHANGE_TO_SCC, mDecodedProto.numRadioModeChangeToScc);
        assertEquals(NUM_RADIO_MODE_CHANGE_TO_SBS, mDecodedProto.numRadioModeChangeToSbs);
        assertEquals(NUM_RADIO_MODE_CHANGE_TO_DBS, mDecodedProto.numRadioModeChangeToDbs);
        assertEquals(NUM_SOFTAP_USER_BAND_PREFERENCE_UNSATISFIED,
                mDecodedProto.numSoftApUserBandPreferenceUnsatisfied);

        PnoScanMetrics pno_metrics = mDecodedProto.pnoScanMetrics;
        assertNotNull(pno_metrics);
        assertEquals(NUM_PNO_SCAN_ATTEMPTS, pno_metrics.numPnoScanAttempts);
        assertEquals(NUM_PNO_SCAN_FAILED, pno_metrics.numPnoScanFailed);
        assertEquals(NUM_PNO_SCAN_STARTED_OVER_OFFLOAD, pno_metrics.numPnoScanStartedOverOffload);
        assertEquals(NUM_PNO_SCAN_FAILED_OVER_OFFLOAD, pno_metrics.numPnoScanFailedOverOffload);
        assertEquals(NUM_PNO_FOUND_NETWORK_EVENTS, pno_metrics.numPnoFoundNetworkEvents);

        for (ConnectToNetworkNotificationAndActionCount notificationCount
                : mDecodedProto.connectToNetworkNotificationCount) {
            assertEquals(NUM_CONNECT_TO_NETWORK_NOTIFICATIONS[notificationCount.notification],
                    notificationCount.count);
            assertEquals(ConnectToNetworkNotificationAndActionCount.RECOMMENDER_OPEN,
                    notificationCount.recommender);
        }
        for (ConnectToNetworkNotificationAndActionCount notificationActionCount
                : mDecodedProto.connectToNetworkNotificationActionCount) {
            assertEquals(NUM_CONNECT_TO_NETWORK_NOTIFICATION_ACTIONS
                            [notificationActionCount.notification]
                            [notificationActionCount.action],
                    notificationActionCount.count);
            assertEquals(ConnectToNetworkNotificationAndActionCount.RECOMMENDER_OPEN,
                    notificationActionCount.recommender);
        }

        assertEquals(SIZE_OPEN_NETWORK_RECOMMENDER_BLACKLIST,
                mDecodedProto.openNetworkRecommenderBlacklistSize);
        assertEquals(IS_WIFI_NETWORKS_AVAILABLE_NOTIFICATION_ON,
                mDecodedProto.isWifiNetworksAvailableNotificationOn);
        assertEquals(NUM_OPEN_NETWORK_RECOMMENDATION_UPDATES,
                mDecodedProto.numOpenNetworkRecommendationUpdates);
        assertEquals(NUM_OPEN_NETWORK_CONNECT_MESSAGE_FAILED_TO_SEND,
                mDecodedProto.numOpenNetworkConnectMessageFailedToSend);

        verifySoftApEventsStoredInProto();

        WpsMetrics wps_metrics = mDecodedProto.wpsMetrics;
        assertNotNull(wps_metrics);
        assertEquals(NUM_WPS_ATTEMPTS, wps_metrics.numWpsAttempts);
        assertEquals(NUM_WPS_SUCCESS, wps_metrics.numWpsSuccess);
        assertEquals(NUM_WPS_START_FAILURE, wps_metrics.numWpsStartFailure);
        assertEquals(NUM_WPS_OVERLAP_FAILURE, wps_metrics.numWpsOverlapFailure);
        assertEquals(NUM_WPS_TIMEOUT_FAILURE, wps_metrics.numWpsTimeoutFailure);
        assertEquals(NUM_WPS_OTHER_CONNECTION_FAILURE, wps_metrics.numWpsOtherConnectionFailure);
        assertEquals(NUM_WPS_SUPPLICANT_FAILURE, wps_metrics.numWpsSupplicantFailure);
        assertEquals(NUM_WPS_CANCELLATION, wps_metrics.numWpsCancellation);

        assertEquals(NUM_WATCHDOG_SUCCESS_DURATION_MS,
                mDecodedProto.watchdogTriggerToConnectionSuccessDurationMs);
        assertEquals(IS_MAC_RANDOMIZATION_ON, mDecodedProto.isMacRandomizationOn);
        assertEquals(WIFI_POWER_METRICS_LOGGING_DURATION,
                mDecodedProto.wifiRadioUsage.loggingDurationMs);
        assertEquals(WIFI_POWER_METRICS_SCAN_TIME,
                mDecodedProto.wifiRadioUsage.scanTimeMs);
        assertEquals(WIFI_IS_UNUSABLE_EVENT_LOGGING_SETTING,
                mDecodedProto.experimentValues.wifiIsUnusableLoggingEnabled);
        assertEquals(LINK_SPEED_COUNTS_LOGGING_SETTING,
                mDecodedProto.experimentValues.linkSpeedCountsLoggingEnabled);
        assertEquals(DATA_STALL_MIN_TX_BAD_SETTING,
                mDecodedProto.experimentValues.wifiDataStallMinTxBad);
        assertEquals(DATA_STALL_MIN_TX_SUCCESS_WITHOUT_RX_SETTING,
                mDecodedProto.experimentValues.wifiDataStallMinTxSuccessWithoutRx);

        assertEquals(NUM_SAR_SENSOR_LISTENER_REGISTRATION_FAILURES,
                mDecodedProto.numSarSensorRegistrationFailures);
        assertEquals(NUM_ONESHOT_SCAN_REQUESTS_WITH_DFS_CHANNELS,
                mDecodedProto.numOneshotHasDfsChannelScans);
        assertEquals(NUM_ADD_OR_UPDATE_NETWORK_CALLS, mDecodedProto.numAddOrUpdateNetworkCalls);
        assertEquals(NUM_ENABLE_NETWORK_CALLS, mDecodedProto.numEnableNetworkCalls);
    }

    /**
     *  Assert deserialized metrics Scan Return Entry equals count
     */
    public void assertScanReturnEntryEquals(int returnCode, int count) {
        for (int i = 0; i < mDecodedProto.scanReturnEntries.length; i++) {
            if (mDecodedProto.scanReturnEntries[i].scanReturnCode == returnCode) {
                assertEquals(count, mDecodedProto.scanReturnEntries[i].scanResultsCount);
                return;
            }
        }
        assertEquals(null, count);
    }

    /**
     *  Assert deserialized metrics SystemState entry equals count
     */
    public void assertSystemStateEntryEquals(int state, boolean screenOn, int count) {
        for (int i = 0; i < mDecodedProto.wifiSystemStateEntries.length; i++) {
            if (mDecodedProto.wifiSystemStateEntries[i].wifiState == state
                    && mDecodedProto.wifiSystemStateEntries[i].isScreenOn == screenOn) {
                assertEquals(count, mDecodedProto.wifiSystemStateEntries[i].wifiStateCount);
                return;
            }
        }
        assertEquals(null, count);
    }

    /**
     * Test the number of Passpoint provision with the failure code are collected correctly
     *
     * @throws Exception
     */
    @Test
    public void testPasspointProvisionMetrics() throws Exception {
        //Increment count for provisioning success.
        mWifiMetrics.incrementPasspointProvisionSuccess();

        // Increment count for provisioning unavailable
        mWifiMetrics.incrementPasspointProvisionFailure(
                ProvisioningCallback.OSU_FAILURE_PROVISIONING_NOT_AVAILABLE);
        mWifiMetrics.incrementPasspointProvisionFailure(
                ProvisioningCallback.OSU_FAILURE_PROVISIONING_NOT_AVAILABLE);

        // Increment count for server connection failure
        mWifiMetrics.incrementPasspointProvisionFailure(
                ProvisioningCallback.OSU_FAILURE_AP_CONNECTION);

        // Dump proto and deserialize
        dumpProtoAndDeserialize();

        assertEquals(mDecodedProto.passpointProvisionStats.numProvisionSuccess, 1);
        assertEquals(mDecodedProto.passpointProvisionStats.provisionFailureCount.length, 2);
        assertEquals(mDecodedProto.passpointProvisionStats.provisionFailureCount[0].failureCode,
                PasspointProvisionStats.OSU_FAILURE_AP_CONNECTION);
        assertEquals(mDecodedProto.passpointProvisionStats.provisionFailureCount[0].count, 1);
        assertEquals(mDecodedProto.passpointProvisionStats.provisionFailureCount[1].failureCode,
                PasspointProvisionStats.OSU_FAILURE_PROVISIONING_NOT_AVAILABLE);
        assertEquals(mDecodedProto.passpointProvisionStats.provisionFailureCount[1].count, 2);
    }

    /**
     * Combination of all other WifiMetrics unit tests, an internal-integration test, or functional
     * test
     */
    @Test
    public void setMetricsSerializeDeserializeAssertMetricsSame() throws Exception {
        setAndIncrementMetrics();
        startAndEndConnectionEventSucceeds();
        dumpProtoAndDeserialize();
        assertDeserializedMetricsCorrect();
        assertEquals("mDecodedProto.connectionEvent.length",
                2, mDecodedProto.connectionEvent.length);
        //<TODO> test individual connectionEvents for correctness,
        // check scanReturnEntries & wifiSystemStateEntries counts and individual elements
        // pending their implementation</TODO>
    }

    /**
     * Test that score breach events are properly generated
     */
    @Test
    public void testScoreBeachEvents() throws Exception {
        int upper = WifiMetrics.LOW_WIFI_SCORE + 7;
        int mid = WifiMetrics.LOW_WIFI_SCORE;
        int lower = WifiMetrics.LOW_WIFI_SCORE - 8;
        mWifiMetrics.setWifiState(WifiMetricsProto.WifiLog.WIFI_ASSOCIATED);
        for (int score = upper; score >= mid; score--) mWifiMetrics.incrementWifiScoreCount(score);
        mWifiMetrics.incrementWifiScoreCount(mid + 1);
        mWifiMetrics.incrementWifiScoreCount(lower); // First breach
        for (int score = lower; score <= mid; score++) mWifiMetrics.incrementWifiScoreCount(score);
        mWifiMetrics.incrementWifiScoreCount(mid - 1);
        mWifiMetrics.incrementWifiScoreCount(upper); // Second breach

        dumpProtoAndDeserialize();

        assertEquals(2, mDecodedProto.staEventList.length);
        assertEquals(StaEvent.TYPE_SCORE_BREACH, mDecodedProto.staEventList[0].type);
        assertEquals(lower, mDecodedProto.staEventList[0].lastScore);
        assertEquals(StaEvent.TYPE_SCORE_BREACH, mDecodedProto.staEventList[1].type);
        assertEquals(upper, mDecodedProto.staEventList[1].lastScore);
    }

    /**
     * Test that Wifi usability score breach events are properly generated
     */
    @Test
    public void testWifiUsabilityScoreBreachEvents() throws Exception {
        int upper = WifiMetrics.LOW_WIFI_USABILITY_SCORE + 7;
        int mid = WifiMetrics.LOW_WIFI_USABILITY_SCORE;
        int lower = WifiMetrics.LOW_WIFI_USABILITY_SCORE - 8;
        mWifiMetrics.setWifiState(WifiMetricsProto.WifiLog.WIFI_ASSOCIATED);
        for (int score = upper; score >= mid; score--) {
            mWifiMetrics.incrementWifiUsabilityScoreCount(1, score, 15);
        }
        mWifiMetrics.incrementWifiUsabilityScoreCount(1, mid + 1, 15);
        mWifiMetrics.incrementWifiUsabilityScoreCount(1, lower, 15); // First breach
        for (int score = lower; score <= mid; score++) {
            mWifiMetrics.incrementWifiUsabilityScoreCount(1, score, 15);
        }
        mWifiMetrics.incrementWifiUsabilityScoreCount(1, mid - 1, 15);
        mWifiMetrics.incrementWifiUsabilityScoreCount(1, upper, 15); // Second breach

        dumpProtoAndDeserialize();

        assertEquals(2, mDecodedProto.staEventList.length);
        assertEquals(StaEvent.TYPE_WIFI_USABILITY_SCORE_BREACH, mDecodedProto.staEventList[0].type);
        assertEquals(lower, mDecodedProto.staEventList[0].lastWifiUsabilityScore);
        assertEquals(StaEvent.TYPE_WIFI_USABILITY_SCORE_BREACH, mDecodedProto.staEventList[1].type);
        assertEquals(upper, mDecodedProto.staEventList[1].lastWifiUsabilityScore);
    }

    private static final String SSID = "red";
    private static final int CONFIG_DTIM = 3;
    private static final int NETWORK_DETAIL_WIFIMODE = 5;
    private static final int NETWORK_DETAIL_DTIM = 7;
    private static final int SCAN_RESULT_LEVEL = -30;
    /**
     * Test that WifiMetrics is correctly getting data from ScanDetail and WifiConfiguration
     */
    @Test
    public void testScanDetailAndWifiConfigurationUsage() throws Exception {
        //Setup mock configs and scan details
        NetworkDetail networkDetail = mock(NetworkDetail.class);
        when(networkDetail.getWifiMode()).thenReturn(NETWORK_DETAIL_WIFIMODE);
        when(networkDetail.getSSID()).thenReturn(SSID);
        when(networkDetail.getDtimInterval()).thenReturn(NETWORK_DETAIL_DTIM);
        ScanResult scanResult = mock(ScanResult.class);
        scanResult.level = SCAN_RESULT_LEVEL;
        WifiConfiguration config = mock(WifiConfiguration.class);
        config.SSID = "\"" + SSID + "\"";
        config.dtimInterval = CONFIG_DTIM;
        config.macRandomizationSetting = WifiConfiguration.RANDOMIZATION_PERSISTENT;
        WifiConfiguration.NetworkSelectionStatus networkSelectionStat =
                mock(WifiConfiguration.NetworkSelectionStatus.class);
        when(networkSelectionStat.getCandidate()).thenReturn(scanResult);
        when(config.getNetworkSelectionStatus()).thenReturn(networkSelectionStat);
        ScanDetail scanDetail = mock(ScanDetail.class);
        when(scanDetail.getNetworkDetail()).thenReturn(networkDetail);
        when(scanDetail.getScanResult()).thenReturn(scanResult);

        config.networkId = TEST_NETWORK_ID;
        mWifiMetrics.setNominatorForNetwork(TEST_NETWORK_ID,
                WifiMetricsProto.ConnectionEvent.NOMINATOR_MANUAL);

        //Create a connection event using only the config
        mWifiMetrics.startConnectionEvent(config, "Red",
                WifiMetricsProto.ConnectionEvent.ROAM_NONE);
        mWifiMetrics.endConnectionEvent(
                WifiMetrics.ConnectionEvent.FAILURE_NONE,
                WifiMetricsProto.ConnectionEvent.HLF_NONE,
                WifiMetricsProto.ConnectionEvent.FAILURE_REASON_UNKNOWN);

        config.macRandomizationSetting = WifiConfiguration.RANDOMIZATION_NONE;
        //Create a connection event using the config and a scan detail
        mWifiMetrics.startConnectionEvent(config, "Green",
                WifiMetricsProto.ConnectionEvent.ROAM_NONE);
        mWifiMetrics.setConnectionScanDetail(scanDetail);
        mWifiMetrics.endConnectionEvent(
                WifiMetrics.ConnectionEvent.FAILURE_NONE,
                WifiMetricsProto.ConnectionEvent.HLF_NONE,
                WifiMetricsProto.ConnectionEvent.FAILURE_REASON_UNKNOWN);

        //Dump proto from mWifiMetrics and deserialize it to mDecodedProto
        dumpProtoAndDeserialize();

        //Check that the correct values are being flowed through
        assertEquals(2, mDecodedProto.connectionEvent.length);
        assertEquals(CONFIG_DTIM, mDecodedProto.connectionEvent[0].routerFingerprint.dtim);
        assertEquals(SCAN_RESULT_LEVEL, mDecodedProto.connectionEvent[0].signalStrength);
        assertEquals(NETWORK_DETAIL_DTIM, mDecodedProto.connectionEvent[1].routerFingerprint.dtim);
        assertEquals(SCAN_RESULT_LEVEL, mDecodedProto.connectionEvent[1].signalStrength);
        assertEquals(NETWORK_DETAIL_WIFIMODE,
                mDecodedProto.connectionEvent[1].routerFingerprint.routerTechnology);
        assertTrue(mDecodedProto.connectionEvent[0].useRandomizedMac);
        assertFalse(mDecodedProto.connectionEvent[1].useRandomizedMac);
        assertEquals(WifiMetricsProto.ConnectionEvent.NOMINATOR_MANUAL,
                mDecodedProto.connectionEvent[0].connectionNominator);
    }

    /**
     * Tests that the mapping from networkId to nominatorId is not cleared.
     */
    @Test
    public void testNetworkToNominatorNotCleared() throws Exception {
        //Setup mock configs and scan details
        NetworkDetail networkDetail = mock(NetworkDetail.class);
        when(networkDetail.getWifiMode()).thenReturn(NETWORK_DETAIL_WIFIMODE);
        when(networkDetail.getSSID()).thenReturn(SSID);
        when(networkDetail.getDtimInterval()).thenReturn(NETWORK_DETAIL_DTIM);
        ScanResult scanResult = mock(ScanResult.class);
        scanResult.level = SCAN_RESULT_LEVEL;
        WifiConfiguration config = mock(WifiConfiguration.class);
        config.SSID = "\"" + SSID + "\"";
        config.dtimInterval = CONFIG_DTIM;
        config.macRandomizationSetting = WifiConfiguration.RANDOMIZATION_PERSISTENT;
        WifiConfiguration.NetworkSelectionStatus networkSelectionStat =
                mock(WifiConfiguration.NetworkSelectionStatus.class);
        when(networkSelectionStat.getCandidate()).thenReturn(scanResult);
        when(config.getNetworkSelectionStatus()).thenReturn(networkSelectionStat);
        ScanDetail scanDetail = mock(ScanDetail.class);
        when(scanDetail.getNetworkDetail()).thenReturn(networkDetail);
        when(scanDetail.getScanResult()).thenReturn(scanResult);

        config.networkId = TEST_NETWORK_ID;
        mWifiMetrics.setNominatorForNetwork(TEST_NETWORK_ID,
                WifiMetricsProto.ConnectionEvent.NOMINATOR_CARRIER);

        // dump() calls clear() internally
        mWifiMetrics.dump(null, new PrintWriter(new StringWriter()),
                new String[]{WifiMetrics.PROTO_DUMP_ARG});

        // Create a connection event using only the config
        mWifiMetrics.startConnectionEvent(config, "Red",
                WifiMetricsProto.ConnectionEvent.ROAM_NONE);
        mWifiMetrics.endConnectionEvent(
                WifiMetrics.ConnectionEvent.FAILURE_NONE,
                WifiMetricsProto.ConnectionEvent.HLF_NONE,
                WifiMetricsProto.ConnectionEvent.FAILURE_REASON_UNKNOWN);

        dumpProtoAndDeserialize();

        assertEquals(WifiMetricsProto.ConnectionEvent.NOMINATOR_CARRIER,
                mDecodedProto.connectionEvent[0].connectionNominator);
    }

    /**
     * Test that WifiMetrics is serializing/deserializing association time out events.
     */
    @Test
    public void testMetricsAssociationTimedOut() throws Exception {
        mWifiMetrics.startConnectionEvent(null, "RED",
                WifiMetricsProto.ConnectionEvent.ROAM_NONE);
        mWifiMetrics.endConnectionEvent(
                WifiMetrics.ConnectionEvent.FAILURE_ASSOCIATION_TIMED_OUT,
                WifiMetricsProto.ConnectionEvent.HLF_NONE,
                WifiMetricsProto.ConnectionEvent.FAILURE_REASON_UNKNOWN);

        //Dump proto and deserialize
        //This should clear all the metrics in mWifiMetrics,
        dumpProtoAndDeserialize();
        //Check there is only 1 connection events
        assertEquals(1, mDecodedProto.connectionEvent.length);
        assertEquals(WifiMetrics.ConnectionEvent.FAILURE_ASSOCIATION_TIMED_OUT,
                mDecodedProto.connectionEvent[0].level2FailureCode);
        assertEquals(WifiMetricsProto.ConnectionEvent.FAILURE_REASON_UNKNOWN,
                mDecodedProto.connectionEvent[0].level2FailureReason);
    }

    /**
     * Test that WifiMetrics is serializing/deserializing authentication failure events.
     */
    @Test
    public void testMetricsAuthenticationFailureReason() throws Exception {
        mWifiMetrics.startConnectionEvent(null, "RED",
                WifiMetricsProto.ConnectionEvent.ROAM_NONE);
        mWifiMetrics.endConnectionEvent(
                WifiMetrics.ConnectionEvent.FAILURE_AUTHENTICATION_FAILURE,
                WifiMetricsProto.ConnectionEvent.HLF_NONE,
                WifiMetricsProto.ConnectionEvent.AUTH_FAILURE_WRONG_PSWD);

        //Dump proto and deserialize
        //This should clear all the metrics in mWifiMetrics,
        dumpProtoAndDeserialize();
        //Check there is only 1 connection events
        assertEquals(1, mDecodedProto.connectionEvent.length);
        assertEquals(WifiMetrics.ConnectionEvent.FAILURE_AUTHENTICATION_FAILURE,
                mDecodedProto.connectionEvent[0].level2FailureCode);
        //Check the authentication failure reason
        assertEquals(WifiMetricsProto.ConnectionEvent.AUTH_FAILURE_WRONG_PSWD,
                mDecodedProto.connectionEvent[0].level2FailureReason);
    }

    /**
     * Test that WifiMetrics is being cleared after dumping via proto
     */
    @Test
    public void testMetricsClearedAfterProtoRequested() throws Exception {
        // Create 3 ConnectionEvents
        mWifiMetrics.startConnectionEvent(null, "RED",
                WifiMetricsProto.ConnectionEvent.ROAM_ENTERPRISE);
        mWifiMetrics.endConnectionEvent(
                WifiMetrics.ConnectionEvent.FAILURE_NONE,
                WifiMetricsProto.ConnectionEvent.HLF_NONE,
                WifiMetricsProto.ConnectionEvent.FAILURE_REASON_UNKNOWN);
        mWifiMetrics.startConnectionEvent(null, "YELLOW",
                WifiMetricsProto.ConnectionEvent.ROAM_ENTERPRISE);
        mWifiMetrics.endConnectionEvent(
                WifiMetrics.ConnectionEvent.FAILURE_NONE,
                WifiMetricsProto.ConnectionEvent.HLF_NONE,
                WifiMetricsProto.ConnectionEvent.FAILURE_REASON_UNKNOWN);
        mWifiMetrics.startConnectionEvent(null, "GREEN",
                WifiMetricsProto.ConnectionEvent.ROAM_ENTERPRISE);
        mWifiMetrics.endConnectionEvent(
                WifiMetrics.ConnectionEvent.FAILURE_NONE,
                WifiMetricsProto.ConnectionEvent.HLF_NONE,
                WifiMetricsProto.ConnectionEvent.FAILURE_REASON_UNKNOWN);
        mWifiMetrics.startConnectionEvent(null, "ORANGE",
                WifiMetricsProto.ConnectionEvent.ROAM_ENTERPRISE);
        mWifiMetrics.endConnectionEvent(
                WifiMetrics.ConnectionEvent.FAILURE_NONE,
                WifiMetricsProto.ConnectionEvent.HLF_NONE,
                WifiMetricsProto.ConnectionEvent.FAILURE_REASON_UNKNOWN);

        //Dump proto and deserialize
        //This should clear all the metrics in mWifiMetrics,
        dumpProtoAndDeserialize();
        //Check there are 4 connection events
        assertEquals(4, mDecodedProto.connectionEvent.length);
        assertEquals(0, mDecodedProto.rssiPollRssiCount.length);
        assertEquals(0, mDecodedProto.alertReasonCount.length);

        // Create 2 ConnectionEvents
        mWifiMetrics.startConnectionEvent(null,  "BLUE",
                WifiMetricsProto.ConnectionEvent.ROAM_ENTERPRISE);
        mWifiMetrics.endConnectionEvent(
                WifiMetrics.ConnectionEvent.FAILURE_NONE,
                WifiMetricsProto.ConnectionEvent.HLF_NONE,
                WifiMetricsProto.ConnectionEvent.FAILURE_REASON_UNKNOWN);
        mWifiMetrics.startConnectionEvent(null, "RED",
                WifiMetricsProto.ConnectionEvent.ROAM_ENTERPRISE);
        mWifiMetrics.endConnectionEvent(
                WifiMetrics.ConnectionEvent.FAILURE_NONE,
                WifiMetricsProto.ConnectionEvent.HLF_NONE,
                WifiMetricsProto.ConnectionEvent.FAILURE_REASON_UNKNOWN);

        //Dump proto and deserialize
        dumpProtoAndDeserialize();
        //Check there are only 2 connection events
        assertEquals(2, mDecodedProto.connectionEvent.length);
    }

    /**
     * Test that current ongoing ConnectionEvent is not cleared and logged
     * when proto is dumped
     */
    @Test
    public void testCurrentConnectionEventNotClearedAfterProtoRequested() throws Exception {
        // Create 2 complete ConnectionEvents and 1 ongoing un-ended ConnectionEvent
        mWifiMetrics.startConnectionEvent(null, "RED",
                WifiMetricsProto.ConnectionEvent.ROAM_ENTERPRISE);
        mWifiMetrics.endConnectionEvent(
                WifiMetrics.ConnectionEvent.FAILURE_NONE,
                WifiMetricsProto.ConnectionEvent.HLF_NONE,
                WifiMetricsProto.ConnectionEvent.FAILURE_REASON_UNKNOWN);
        mWifiMetrics.startConnectionEvent(null, "YELLOW",
                WifiMetricsProto.ConnectionEvent.ROAM_ENTERPRISE);
        mWifiMetrics.endConnectionEvent(
                WifiMetrics.ConnectionEvent.FAILURE_NONE,
                WifiMetricsProto.ConnectionEvent.HLF_NONE,
                WifiMetricsProto.ConnectionEvent.FAILURE_REASON_UNKNOWN);
        mWifiMetrics.startConnectionEvent(null, "GREEN",
                WifiMetricsProto.ConnectionEvent.ROAM_ENTERPRISE);

        // Dump proto and deserialize
        // This should clear the metrics in mWifiMetrics,
        dumpProtoAndDeserialize();
        assertEquals(2, mDecodedProto.connectionEvent.length);

        // End the ongoing ConnectionEvent
        mWifiMetrics.endConnectionEvent(
                WifiMetrics.ConnectionEvent.FAILURE_NONE,
                WifiMetricsProto.ConnectionEvent.HLF_NONE,
                WifiMetricsProto.ConnectionEvent.FAILURE_REASON_UNKNOWN);

        dumpProtoAndDeserialize();
        assertEquals(1, mDecodedProto.connectionEvent.length);
    }

    /**
     * Tests that after setting metrics values they can be serialized and deserialized with the
     *   $ adb shell dumpsys wifi wifiMetricsProto clean
     */
    @Test
    public void testClearMetricsDump() throws Exception {
        setAndIncrementMetrics();
        startAndEndConnectionEventSucceeds();
        cleanDumpProtoAndDeserialize();
        assertDeserializedMetricsCorrect();
        assertEquals("mDecodedProto.connectionEvent.length",
                2, mDecodedProto.connectionEvent.length);
    }

    private static final int NUM_REPEATED_DELTAS = 7;
    private static final int REPEATED_DELTA = 0;
    private static final int SINGLE_GOOD_DELTA = 1;
    private static final int SINGLE_TIMEOUT_DELTA = 2;
    private static final int NUM_REPEATED_BOUND_DELTAS = 2;
    private static final int MAX_DELTA_LEVEL = 127;
    private static final int MIN_DELTA_LEVEL = -127;
    private static final int ARBITRARY_DELTA_LEVEL = 20;

    /**
     * Sunny day RSSI delta logging scenario.
     * Logs one rssi delta value multiple times
     * Logs a different delta value a single time
     */
    @Test
    public void testRssiDeltasSuccessfulLogging() throws Exception {
        // Generate some repeated deltas
        for (int i = 0; i < NUM_REPEATED_DELTAS; i++) {
            generateRssiDelta(MIN_RSSI_LEVEL, REPEATED_DELTA,
                    WifiMetrics.TIMEOUT_RSSI_DELTA_MILLIS);
        }
        // Generate a single delta
        generateRssiDelta(MIN_RSSI_LEVEL, SINGLE_GOOD_DELTA,
                WifiMetrics.TIMEOUT_RSSI_DELTA_MILLIS);
        dumpProtoAndDeserialize();
        assertEquals(2, mDecodedProto.rssiPollDeltaCount.length);
        // Check the repeated deltas
        assertEquals(NUM_REPEATED_DELTAS, mDecodedProto.rssiPollDeltaCount[0].count);
        assertEquals(REPEATED_DELTA, mDecodedProto.rssiPollDeltaCount[0].rssi);
        // Check the single delta
        assertEquals(1, mDecodedProto.rssiPollDeltaCount[1].count);
        assertEquals(SINGLE_GOOD_DELTA, mDecodedProto.rssiPollDeltaCount[1].rssi);
    }

    /**
     * Tests that Rssi Delta events whose scanResult and Rssi Poll come too far apart, timeout,
     * and are not logged.
     */
    @Test
    public void testRssiDeltasTimeout() throws Exception {
        // Create timed out rssi deltas
        generateRssiDelta(MIN_RSSI_LEVEL, REPEATED_DELTA,
                WifiMetrics.TIMEOUT_RSSI_DELTA_MILLIS + 1);
        generateRssiDelta(MIN_RSSI_LEVEL, SINGLE_TIMEOUT_DELTA,
                WifiMetrics.TIMEOUT_RSSI_DELTA_MILLIS + 1);
        dumpProtoAndDeserialize();
        assertEquals(0, mDecodedProto.rssiPollDeltaCount.length);
    }

    /**
     * Tests the exact inclusive boundaries of RSSI delta logging.
     */
    @Test
    public void testRssiDeltaSuccessfulLoggingExactBounds() throws Exception {
        generateRssiDelta(MIN_RSSI_LEVEL, MAX_DELTA_LEVEL,
                WifiMetrics.TIMEOUT_RSSI_DELTA_MILLIS);
        generateRssiDelta(MAX_RSSI_LEVEL, MIN_DELTA_LEVEL,
                WifiMetrics.TIMEOUT_RSSI_DELTA_MILLIS);
        dumpProtoAndDeserialize();
        assertEquals(2, mDecodedProto.rssiPollDeltaCount.length);
        assertEquals(MIN_DELTA_LEVEL, mDecodedProto.rssiPollDeltaCount[0].rssi);
        assertEquals(1, mDecodedProto.rssiPollDeltaCount[0].count);
        assertEquals(MAX_DELTA_LEVEL, mDecodedProto.rssiPollDeltaCount[1].rssi);
        assertEquals(1, mDecodedProto.rssiPollDeltaCount[1].count);
    }

    /**
     * Tests the exact exclusive boundaries of RSSI delta logging.
     * This test ensures that too much data is not generated.
     */
    @Test
    public void testRssiDeltaOutOfBounds() throws Exception {
        generateRssiDelta(MIN_RSSI_LEVEL, MAX_DELTA_LEVEL + 1,
                WifiMetrics.TIMEOUT_RSSI_DELTA_MILLIS);
        generateRssiDelta(MAX_RSSI_LEVEL, MIN_DELTA_LEVEL - 1,
                WifiMetrics.TIMEOUT_RSSI_DELTA_MILLIS);
        dumpProtoAndDeserialize();
        assertEquals(0, mDecodedProto.rssiPollDeltaCount.length);
    }

    /**
     * This test ensures no rssi Delta is logged after an unsuccessful ConnectionEvent
     */
    @Test
    public void testUnsuccesfulConnectionEventRssiDeltaIsNotLogged() throws Exception {
        generateRssiDelta(MIN_RSSI_LEVEL, ARBITRARY_DELTA_LEVEL,
                WifiMetrics.TIMEOUT_RSSI_DELTA_MILLIS,
                false, // successfulConnectionEvent
                true, // completeConnectionEvent
                true, // useValidScanResult
                true // dontDeserializeBeforePoll
        );

        dumpProtoAndDeserialize();
        assertEquals(0, mDecodedProto.rssiPollDeltaCount.length);
    }

    /**
     * This test ensures rssi Deltas can be logged during a ConnectionEvent
     */
    @Test
    public void testIncompleteConnectionEventRssiDeltaIsLogged() throws Exception {
        generateRssiDelta(MIN_RSSI_LEVEL, ARBITRARY_DELTA_LEVEL,
                WifiMetrics.TIMEOUT_RSSI_DELTA_MILLIS,
                true, // successfulConnectionEvent
                false, // completeConnectionEvent
                true, // useValidScanResult
                true // dontDeserializeBeforePoll
        );
        dumpProtoAndDeserialize();
        assertEquals(1, mDecodedProto.rssiPollDeltaCount.length);
        assertEquals(ARBITRARY_DELTA_LEVEL, mDecodedProto.rssiPollDeltaCount[0].rssi);
        assertEquals(1, mDecodedProto.rssiPollDeltaCount[0].count);
    }

    /**
     * This test ensures that no delta is logged for a null ScanResult Candidate
     */
    @Test
    public void testRssiDeltaNotLoggedForNullCandidateScanResult() throws Exception {
        generateRssiDelta(MIN_RSSI_LEVEL, ARBITRARY_DELTA_LEVEL,
                WifiMetrics.TIMEOUT_RSSI_DELTA_MILLIS,
                true, // successfulConnectionEvent
                true, // completeConnectionEvent
                false, // useValidScanResult
                true // dontDeserializeBeforePoll
        );
        dumpProtoAndDeserialize();
        assertEquals(0, mDecodedProto.rssiPollDeltaCount.length);
    }

    /**
     * This test ensures that Rssi Deltas are not logged over a 'clear()' call (Metrics Serialized)
     */
    @Test
    public void testMetricsSerializedDuringRssiDeltaEventLogsNothing() throws Exception {
        generateRssiDelta(MIN_RSSI_LEVEL, ARBITRARY_DELTA_LEVEL,
                WifiMetrics.TIMEOUT_RSSI_DELTA_MILLIS,
                true, // successfulConnectionEvent
                true, // completeConnectionEvent
                true, // useValidScanResult
                false // dontDeserializeBeforePoll
        );
        dumpProtoAndDeserialize();
        assertEquals(0, mDecodedProto.rssiPollDeltaCount.length);
    }

    private static final int DEAUTH_REASON = 7;
    private static final int ASSOC_STATUS = 11;
    private static final int ASSOC_TIMEOUT = 1;
    private static final int LOCAL_GEN = 1;
    private static final int AUTH_FAILURE_REASON = WifiManager.ERROR_AUTH_FAILURE_WRONG_PSWD;
    private static final int NUM_TEST_STA_EVENTS = 19;
    private static final String   sSSID = "\"SomeTestSsid\"";
    private static final WifiSsid sWifiSsid = WifiSsid.createFromAsciiEncoded(sSSID);
    private static final String   sBSSID = "01:02:03:04:05:06";

    private final StateChangeResult mStateDisconnected =
            new StateChangeResult(0, sWifiSsid, sBSSID, SupplicantState.DISCONNECTED);
    private final StateChangeResult mStateCompleted =
            new StateChangeResult(0, sWifiSsid, sBSSID, SupplicantState.COMPLETED);
    // Test bitmasks of supplicant state changes
    private final int mSupBm1 = WifiMetrics.supplicantStateToBit(mStateDisconnected.state);
    private final int mSupBm2 = WifiMetrics.supplicantStateToBit(mStateDisconnected.state)
            | WifiMetrics.supplicantStateToBit(mStateCompleted.state);
    // An invalid but interesting wifiConfiguration that exercises the StaEvent.ConfigInfo encoding
    private final WifiConfiguration mTestWifiConfig = createComplexWifiConfig();
    // <msg.what> <msg.arg1> <msg.arg2>
    private int[][] mTestStaMessageInts = {
        {WifiMonitor.ASSOCIATION_REJECTION_EVENT,   ASSOC_TIMEOUT,       ASSOC_STATUS},
        {WifiMonitor.AUTHENTICATION_FAILURE_EVENT,  AUTH_FAILURE_REASON, -1},
        {WifiMonitor.NETWORK_CONNECTION_EVENT,      0,                   0},
        {WifiMonitor.NETWORK_DISCONNECTION_EVENT,   LOCAL_GEN,           DEAUTH_REASON},
        {WifiMonitor.SUPPLICANT_STATE_CHANGE_EVENT, 0,                   0},
        {ClientModeImpl.CMD_ASSOCIATED_BSSID,       0,                   0},
        {ClientModeImpl.CMD_TARGET_BSSID,           0,                   0},
        {WifiMonitor.SUPPLICANT_STATE_CHANGE_EVENT, 0,                   0},
        {WifiMonitor.SUPPLICANT_STATE_CHANGE_EVENT, 0,                   0}
    };
    private Object[] mTestStaMessageObjs = {
        null,
        null,
        null,
        null,
        mStateDisconnected,
        null,
        null,
        mStateDisconnected,
        mStateCompleted
    };
    // Values used to generate the StaEvent log calls from ClientModeImpl
    // <StaEvent.Type>, <StaEvent.FrameworkDisconnectReason>, <1|0>(testWifiConfiguration, null)
    private int[][] mTestStaLogInts = {
        {StaEvent.TYPE_CMD_IP_CONFIGURATION_SUCCESSFUL, 0,                          0},
        {StaEvent.TYPE_CMD_IP_CONFIGURATION_LOST,       0,                          0},
        {StaEvent.TYPE_CMD_IP_REACHABILITY_LOST,        0,                          0},
        {StaEvent.TYPE_CMD_START_CONNECT,               0,                          1},
        {StaEvent.TYPE_CMD_START_ROAM,                  0,                          1},
        {StaEvent.TYPE_CONNECT_NETWORK,                 0,                          1},
        {StaEvent.TYPE_NETWORK_AGENT_VALID_NETWORK,     0,                          0},
        {StaEvent.TYPE_FRAMEWORK_DISCONNECT,            StaEvent.DISCONNECT_API,    0},
        {StaEvent.TYPE_SCORE_BREACH,                    0,                          0},
        {StaEvent.TYPE_MAC_CHANGE,                      0,                          1},
        {StaEvent.TYPE_WIFI_ENABLED,                    0,                          0},
        {StaEvent.TYPE_WIFI_DISABLED,                   0,                          0},
        {StaEvent.TYPE_WIFI_USABILITY_SCORE_BREACH,     0,                          0}
    };
    // Values used to generate the StaEvent log calls from WifiMonitor
    // <type>, <reason>, <status>, <local_gen>,
    // <auth_fail_reason>, <assoc_timed_out> <supplicantStateChangeBitmask> <1|0>(has ConfigInfo)
    private int[][] mExpectedValues = {
        {StaEvent.TYPE_ASSOCIATION_REJECTION_EVENT,     -1,  ASSOC_STATUS,         0,
            /**/                               0, ASSOC_TIMEOUT,        0, 0},    /**/
        {StaEvent.TYPE_AUTHENTICATION_FAILURE_EVENT,    -1,            -1,         0,
            /**/StaEvent.AUTH_FAILURE_WRONG_PSWD,             0,        0, 0},    /**/
        {StaEvent.TYPE_NETWORK_CONNECTION_EVENT,        -1,            -1,         0,
            /**/                               0,             0,        0, 0},    /**/
        {StaEvent.TYPE_NETWORK_DISCONNECTION_EVENT, DEAUTH_REASON,     -1, LOCAL_GEN,
            /**/                               0,             0,        0, 0},    /**/
        {StaEvent.TYPE_CMD_ASSOCIATED_BSSID,            -1,            -1,         0,
            /**/                               0,             0,  mSupBm1, 0},    /**/
        {StaEvent.TYPE_CMD_TARGET_BSSID,                -1,            -1,         0,
            /**/                               0,             0,        0, 0},    /**/
        {StaEvent.TYPE_CMD_IP_CONFIGURATION_SUCCESSFUL, -1,            -1,         0,
            /**/                               0,             0,  mSupBm2, 0},    /**/
        {StaEvent.TYPE_CMD_IP_CONFIGURATION_LOST,       -1,            -1,         0,
            /**/                               0,             0,        0, 0},    /**/
        {StaEvent.TYPE_CMD_IP_REACHABILITY_LOST,        -1,            -1,         0,
            /**/                               0,             0,        0, 0},    /**/
        {StaEvent.TYPE_CMD_START_CONNECT,               -1,            -1,         0,
            /**/                               0,             0,        0, 1},    /**/
        {StaEvent.TYPE_CMD_START_ROAM,                  -1,            -1,         0,
            /**/                               0,             0,        0, 1},    /**/
        {StaEvent.TYPE_CONNECT_NETWORK,                 -1,            -1,         0,
            /**/                               0,             0,        0, 1},    /**/
        {StaEvent.TYPE_NETWORK_AGENT_VALID_NETWORK,     -1,            -1,         0,
            /**/                               0,             0,        0, 0},    /**/
        {StaEvent.TYPE_FRAMEWORK_DISCONNECT,            -1,            -1,         0,
            /**/                               0,             0,        0, 0},    /**/
        {StaEvent.TYPE_SCORE_BREACH,                    -1,            -1,         0,
            /**/                               0,             0,        0, 0},    /**/
        {StaEvent.TYPE_MAC_CHANGE,                      -1,            -1,         0,
            /**/                               0,             0,        0, 1},    /**/
        {StaEvent.TYPE_WIFI_ENABLED,                    -1,            -1,         0,
            /**/                               0,             0,        0, 0},    /**/
        {StaEvent.TYPE_WIFI_DISABLED,                   -1,            -1,         0,
            /**/                               0,             0,        0, 0},     /**/
        {StaEvent.TYPE_WIFI_USABILITY_SCORE_BREACH,     -1,            -1,         0,
            /**/                               0,             0,        0, 0}    /**/
    };

    /**
     * Generates events from all the rows in mTestStaMessageInts, and then mTestStaLogInts
     */
    private void generateStaEvents(WifiMetrics wifiMetrics) {
        Handler handler = wifiMetrics.getHandler();
        for (int i = 0; i < mTestStaMessageInts.length; i++) {
            int[] mia = mTestStaMessageInts[i];
            handler.sendMessage(
                    handler.obtainMessage(mia[0], mia[1], mia[2], mTestStaMessageObjs[i]));
        }
        mTestLooper.dispatchAll();
        for (int i = 0; i < mTestStaLogInts.length; i++) {
            int[] lia = mTestStaLogInts[i];
            wifiMetrics.logStaEvent(lia[0], lia[1], lia[2] == 1 ? mTestWifiConfig : null);
        }
    }
    private void verifyDeserializedStaEvents(WifiMetricsProto.WifiLog wifiLog) {
        assertNotNull(mTestWifiConfig);
        assertEquals(NUM_TEST_STA_EVENTS, wifiLog.staEventList.length);
        int j = 0; // De-serialized event index
        for (int i = 0; i < mTestStaMessageInts.length; i++) {
            StaEvent event = wifiLog.staEventList[j];
            int[] mia = mTestStaMessageInts[i];
            int[] evs = mExpectedValues[j];
            if (mia[0] != WifiMonitor.SUPPLICANT_STATE_CHANGE_EVENT) {
                assertEquals(evs[0], event.type);
                assertEquals(evs[1], event.reason);
                assertEquals(evs[2], event.status);
                assertEquals(evs[3] == 1 ? true : false, event.localGen);
                assertEquals(evs[4], event.authFailureReason);
                assertEquals(evs[5] == 1 ? true : false, event.associationTimedOut);
                assertEquals(evs[6], event.supplicantStateChangesBitmask);
                assertConfigInfoEqualsWifiConfig(
                        evs[7] == 1 ? mTestWifiConfig : null, event.configInfo);
                j++;
            }
        }
        for (int i = 0; i < mTestStaLogInts.length; i++) {
            StaEvent event = wifiLog.staEventList[j];
            int[] evs = mExpectedValues[j];
            assertEquals(evs[0], event.type);
            assertEquals(evs[1], event.reason);
            assertEquals(evs[2], event.status);
            assertEquals(evs[3] == 1 ? true : false, event.localGen);
            assertEquals(evs[4], event.authFailureReason);
            assertEquals(evs[5] == 1 ? true : false, event.associationTimedOut);
            assertEquals(evs[6], event.supplicantStateChangesBitmask);
            assertConfigInfoEqualsWifiConfig(
                    evs[7] == 1 ? mTestWifiConfig : null, event.configInfo);
            j++;
        }
        assertEquals(mExpectedValues.length, j);
    }

    /**
     * Generate StaEvents of each type, ensure all the different values are logged correctly,
     * and that they survive serialization & de-serialization
     */
    @Test
    public void testStaEventsLogSerializeDeserialize() throws Exception {
        generateStaEvents(mWifiMetrics);
        dumpProtoAndDeserialize();
        verifyDeserializedStaEvents(mDecodedProto);
    }

    /**
     * Ensure the number of StaEvents does not exceed MAX_STA_EVENTS by generating lots of events
     * and checking how many are deserialized
     */
    @Test
    public void testStaEventBounding() throws Exception {
        for (int i = 0; i < (WifiMetrics.MAX_STA_EVENTS + 10); i++) {
            mWifiMetrics.logStaEvent(StaEvent.TYPE_CMD_START_CONNECT);
        }
        dumpProtoAndDeserialize();
        assertEquals(WifiMetrics.MAX_STA_EVENTS, mDecodedProto.staEventList.length);
    }

    /**
     * Tests that link probe StaEvents do not exceed
     * {@link WifiMetrics#MAX_LINK_PROBE_STA_EVENTS}.
     */
    @Test
    public void testLinkProbeStaEventBounding() throws Exception {
        for (int i = 0; i < WifiMetrics.MAX_LINK_PROBE_STA_EVENTS; i++) {
            mWifiMetrics.logLinkProbeSuccess(0, 0, 0, 0);
            mWifiMetrics.logLinkProbeFailure(0, 0, 0, 0);
        }
        for (int i = 0; i < 10; i++) {
            mWifiMetrics.logStaEvent(StaEvent.TYPE_CMD_START_CONNECT);
        }

        dumpProtoAndDeserialize();

        long numLinkProbeStaEvents = Arrays.stream(mDecodedProto.staEventList)
                .filter(event -> event.type == TYPE_LINK_PROBE)
                .count();
        assertEquals(WifiMetrics.MAX_LINK_PROBE_STA_EVENTS, numLinkProbeStaEvents);
        assertEquals(WifiMetrics.MAX_LINK_PROBE_STA_EVENTS + 10, mDecodedProto.staEventList.length);
    }

    /**
     * Ensure WifiMetrics doesn't cause a null pointer exception when called with null args
     */
    @Test
    public void testDumpNullArg() {
        mWifiMetrics.dump(new FileDescriptor(), new PrintWriter(new StringWriter()), null);
    }

    /**
     * Test the generation of 'NumConnectableNetwork' histograms from two scans of different
     * ScanDetails produces the correct histogram values, and relevant bounds are observed
     */
    @MediumTest
    @Test
    public void testNumConnectableNetworksGeneration() throws Exception {
        List<ScanDetail> scan = new ArrayList<ScanDetail>();
        //                                ssid, bssid, isOpen, isSaved, isProvider, isWeakRssi)
        scan.add(buildMockScanDetail("PASSPOINT_1", "bssid0", false, false, true, false));
        scan.add(buildMockScanDetail("PASSPOINT_2", "bssid1", false, false, true, false));
        scan.add(buildMockScanDetail("SSID_B", "bssid2", true, true, false, false));
        scan.add(buildMockScanDetail("SSID_B", "bssid3", true, true, false, false));
        scan.add(buildMockScanDetail("SSID_C", "bssid4", true, false, false, false));
        scan.add(buildMockScanDetail("SSID_D", "bssid5", false, true, false, false));
        scan.add(buildMockScanDetail("SSID_E", "bssid6", false, true, false, false));
        scan.add(buildMockScanDetail("SSID_F", "bssid7", false, false, false, false));
        scan.add(buildMockScanDetail("SSID_G_WEAK", "bssid9", false, false, false, true));
        scan.add(buildMockScanDetail("SSID_H_WEAK", "bssid10", false, false, false, true));
        mWifiMetrics.incrementAvailableNetworksHistograms(scan, true);
        scan.add(buildMockScanDetail("SSID_B", "bssid8", true, true, false, false));
        mWifiMetrics.incrementAvailableNetworksHistograms(scan, true);
        for (int i = 0; i < NUM_PARTIAL_SCAN_RESULTS; i++) {
            mWifiMetrics.incrementAvailableNetworksHistograms(scan, false);
        }
        dumpProtoAndDeserialize();
        verifyHist(mDecodedProto.totalSsidsInScanHistogram, 1,                    a(7),    a(2));
        verifyHist(mDecodedProto.totalBssidsInScanHistogram, 2,                   a(8, 9), a(1, 1));
        verifyHist(mDecodedProto.availableOpenSsidsInScanHistogram, 1,            a(2),    a(2));
        verifyHist(mDecodedProto.availableOpenBssidsInScanHistogram, 2,           a(3, 4), a(1, 1));
        verifyHist(mDecodedProto.availableSavedSsidsInScanHistogram, 1,           a(3),    a(2));
        verifyHist(mDecodedProto.availableSavedBssidsInScanHistogram, 2,          a(4, 5), a(1, 1));
        verifyHist(mDecodedProto.availableOpenOrSavedSsidsInScanHistogram, 1,     a(4),    a(2));
        verifyHist(mDecodedProto.availableOpenOrSavedBssidsInScanHistogram, 2,    a(5, 6), a(1, 1));
        verifyHist(mDecodedProto.availableSavedPasspointProviderProfilesInScanHistogram, 1,
                                                                                  a(2),    a(2));
        verifyHist(mDecodedProto.availableSavedPasspointProviderBssidsInScanHistogram, 1,
                                                                                  a(2),    a(2));
        assertEquals(2, mDecodedProto.fullBandAllSingleScanListenerResults);
        assertEquals(NUM_PARTIAL_SCAN_RESULTS, mDecodedProto.partialAllSingleScanListenerResults);

        // Check Bounds
        scan.clear();
        int lotsOfSSids = Math.max(WifiMetrics.MAX_TOTAL_SCAN_RESULT_SSIDS_BUCKET,
                WifiMetrics.MAX_CONNECTABLE_SSID_NETWORK_BUCKET) + 5;
        for (int i = 0; i < lotsOfSSids; i++) {
            scan.add(buildMockScanDetail("SSID_" + i, "bssid_" + i, true, true, false, false));
        }
        mWifiMetrics.incrementAvailableNetworksHistograms(scan, true);
        dumpProtoAndDeserialize();
        verifyHist(mDecodedProto.totalSsidsInScanHistogram, 1,
                a(WifiMetrics.MAX_TOTAL_SCAN_RESULT_SSIDS_BUCKET), a(1));
        verifyHist(mDecodedProto.availableOpenSsidsInScanHistogram, 1,
                a(WifiMetrics.MAX_CONNECTABLE_SSID_NETWORK_BUCKET), a(1));
        verifyHist(mDecodedProto.availableSavedSsidsInScanHistogram, 1,
                a(WifiMetrics.MAX_CONNECTABLE_SSID_NETWORK_BUCKET), a(1));
        verifyHist(mDecodedProto.availableOpenOrSavedSsidsInScanHistogram, 1,
                a(WifiMetrics.MAX_CONNECTABLE_SSID_NETWORK_BUCKET), a(1));
        scan.clear();
        int lotsOfBssids = Math.max(WifiMetrics.MAX_TOTAL_SCAN_RESULTS_BUCKET,
                WifiMetrics.MAX_CONNECTABLE_BSSID_NETWORK_BUCKET) + 5;
        for (int i = 0; i < lotsOfBssids; i++) {
            scan.add(buildMockScanDetail("SSID", "bssid_" + i, true, true, false, false));
        }
        mWifiMetrics.incrementAvailableNetworksHistograms(scan, true);
        dumpProtoAndDeserialize();
        verifyHist(mDecodedProto.totalBssidsInScanHistogram, 1,
                a(WifiMetrics.MAX_TOTAL_SCAN_RESULTS_BUCKET), a(1));
        verifyHist(mDecodedProto.availableOpenBssidsInScanHistogram, 1,
                a(WifiMetrics.MAX_CONNECTABLE_BSSID_NETWORK_BUCKET), a(1));
        verifyHist(mDecodedProto.availableSavedBssidsInScanHistogram, 1,
                a(WifiMetrics.MAX_CONNECTABLE_BSSID_NETWORK_BUCKET), a(1));
        verifyHist(mDecodedProto.availableOpenOrSavedBssidsInScanHistogram, 1,
                a(WifiMetrics.MAX_CONNECTABLE_BSSID_NETWORK_BUCKET), a(1));
    }

    /**
     * Test that Hotspot 2.0 (Passpoint) scan results are collected correctly and that relevant
     * bounds are observed.
     */
    @Test
    public void testObservedHotspotAps() throws Exception {
        List<ScanDetail> scan = new ArrayList<ScanDetail>();
        // 2 R1 (Unknown AP isn't counted) passpoint APs belonging to a single provider: hessid1
        long hessid1 = 10;
        int anqpDomainId1 = 5;
        scan.add(buildMockScanDetailPasspoint("PASSPOINT_XX", "00:02:03:04:05:06", hessid1,
                anqpDomainId1, NetworkDetail.HSRelease.R1, true));
        scan.add(buildMockScanDetailPasspoint("PASSPOINT_XY", "01:02:03:04:05:06", hessid1,
                anqpDomainId1, NetworkDetail.HSRelease.R1, true));
        scan.add(buildMockScanDetailPasspoint("PASSPOINT_XYZ", "02:02:03:04:05:06", hessid1,
                anqpDomainId1, NetworkDetail.HSRelease.Unknown, true));
        // 2 R2 passpoint APs belonging to a single provider: hessid2
        long hessid2 = 12;
        int anqpDomainId2 = 6;
        scan.add(buildMockScanDetailPasspoint("PASSPOINT_Y", "AA:02:03:04:05:06", hessid2,
                anqpDomainId2, NetworkDetail.HSRelease.R2, true));
        scan.add(buildMockScanDetailPasspoint("PASSPOINT_Z", "AB:02:03:04:05:06", hessid2,
                anqpDomainId2, NetworkDetail.HSRelease.R2, true));
        mWifiMetrics.incrementAvailableNetworksHistograms(scan, true);
        scan = new ArrayList<ScanDetail>();
        // 3 R2 passpoint APs belonging to a single provider: hessid3 (in next scan)
        long hessid3 = 15;
        int anqpDomainId3 = 8;
        scan.add(buildMockScanDetailPasspoint("PASSPOINT_Y", "AA:02:03:04:05:06", hessid3,
                anqpDomainId3, NetworkDetail.HSRelease.R2, true));
        scan.add(buildMockScanDetailPasspoint("PASSPOINT_Y", "AA:02:03:04:05:06", hessid3,
                anqpDomainId3, NetworkDetail.HSRelease.R2, false));
        scan.add(buildMockScanDetailPasspoint("PASSPOINT_Z", "AB:02:03:04:05:06", hessid3,
                anqpDomainId3, NetworkDetail.HSRelease.R2, true));
        mWifiMetrics.incrementAvailableNetworksHistograms(scan, true);
        dumpProtoAndDeserialize();

        verifyHist(mDecodedProto.observedHotspotR1ApsInScanHistogram, 2, a(0, 2), a(1, 1));
        verifyHist(mDecodedProto.observedHotspotR2ApsInScanHistogram, 2, a(2, 3), a(1, 1));
        verifyHist(mDecodedProto.observedHotspotR1EssInScanHistogram, 2, a(0, 1), a(1, 1));
        verifyHist(mDecodedProto.observedHotspotR2EssInScanHistogram, 1, a(1), a(2));
        verifyHist(mDecodedProto.observedHotspotR1ApsPerEssInScanHistogram, 1, a(2), a(1));
        verifyHist(mDecodedProto.observedHotspotR2ApsPerEssInScanHistogram, 2, a(2, 3), a(1, 1));

        // check bounds
        scan.clear();
        int lotsOfSSids = Math.max(WifiMetrics.MAX_TOTAL_PASSPOINT_APS_BUCKET,
                WifiMetrics.MAX_TOTAL_PASSPOINT_UNIQUE_ESS_BUCKET) + 5;
        for (int i = 0; i < lotsOfSSids; i++) {
            scan.add(buildMockScanDetailPasspoint("PASSPOINT_XX" + i, "00:02:03:04:05:06", i,
                    i + 10, NetworkDetail.HSRelease.R1, true));
            scan.add(buildMockScanDetailPasspoint("PASSPOINT_XY" + i, "AA:02:03:04:05:06", 1000 * i,
                    i + 10, NetworkDetail.HSRelease.R2, false));
        }
        mWifiMetrics.incrementAvailableNetworksHistograms(scan, true);
        dumpProtoAndDeserialize();
        verifyHist(mDecodedProto.observedHotspotR1ApsInScanHistogram, 1,
                a(WifiMetrics.MAX_TOTAL_PASSPOINT_APS_BUCKET), a(1));
        verifyHist(mDecodedProto.observedHotspotR2ApsInScanHistogram, 1,
                a(WifiMetrics.MAX_TOTAL_PASSPOINT_APS_BUCKET), a(1));
        verifyHist(mDecodedProto.observedHotspotR1EssInScanHistogram, 1,
                a(WifiMetrics.MAX_TOTAL_PASSPOINT_UNIQUE_ESS_BUCKET), a(1));
        verifyHist(mDecodedProto.observedHotspotR2EssInScanHistogram, 1,
                a(WifiMetrics.MAX_TOTAL_PASSPOINT_UNIQUE_ESS_BUCKET), a(1));
    }

    /**
     * Test that IEEE 802.11mc scan results are collected correctly and that relevant
     * bounds are observed.
     */
    @Test
    public void testObserved80211mcAps() throws Exception {
        ScanDetail mockScanDetailNon80211mc = mock(ScanDetail.class);
        ScanDetail mockScanDetail80211mc = mock(ScanDetail.class);
        NetworkDetail mockNetworkDetailNon80211mc = mock(NetworkDetail.class);
        NetworkDetail mockNetworkDetail80211mc = mock(NetworkDetail.class);
        when(mockNetworkDetail80211mc.is80211McResponderSupport()).thenReturn(true);
        ScanResult mockScanResult = mock(ScanResult.class);
        mockScanResult.capabilities = "";
        when(mockScanDetailNon80211mc.getNetworkDetail()).thenReturn(mockNetworkDetailNon80211mc);
        when(mockScanDetail80211mc.getNetworkDetail()).thenReturn(mockNetworkDetail80211mc);
        when(mockScanDetailNon80211mc.getScanResult()).thenReturn(mockScanResult);
        when(mockScanDetail80211mc.getScanResult()).thenReturn(mockScanResult);
        when(mWns.isSignalTooWeak(eq(mockScanDetail80211mc.getScanResult()))).thenReturn(true);
        List<ScanDetail> scan = new ArrayList<ScanDetail>();

        // 4 scans (a few non-802.11mc supporting APs on each)
        //  scan1: no 802.11mc supporting APs

        scan.add(mockScanDetailNon80211mc);
        scan.add(mockScanDetailNon80211mc);
        mWifiMetrics.incrementAvailableNetworksHistograms(scan, true);

        //  scan2: 2 802.11mc supporting APs
        scan.clear();
        scan.add(mockScanDetailNon80211mc);
        scan.add(mockScanDetail80211mc);
        scan.add(mockScanDetail80211mc);
        mWifiMetrics.incrementAvailableNetworksHistograms(scan, true);

        //  scan3: 100 802.11mc supporting APs (> limit)
        scan.clear();
        scan.add(mockScanDetailNon80211mc);
        scan.add(mockScanDetailNon80211mc);
        scan.add(mockScanDetailNon80211mc);
        for (int i = 0; i < 100; ++i) {
            scan.add(mockScanDetail80211mc);
        }
        mWifiMetrics.incrementAvailableNetworksHistograms(scan, true);

        //  scan4: 2 802.11mc supporting APs
        scan.clear();
        scan.add(mockScanDetailNon80211mc);
        scan.add(mockScanDetail80211mc);
        scan.add(mockScanDetail80211mc);
        scan.add(mockScanDetailNon80211mc);
        mWifiMetrics.incrementAvailableNetworksHistograms(scan, true);

        dumpProtoAndDeserialize();

        verifyHist(mDecodedProto.observed80211McSupportingApsInScanHistogram, 3,
                a(0, 2, WifiMetrics.MAX_TOTAL_80211MC_APS_BUCKET), a(1, 2, 1));
    }

    /**
     * Test Open Network Notification blacklist size and feature state are not cleared when proto
     * is dumped.
     */
    @Test
    public void testOpenNetworkNotificationBlacklistSizeAndFeatureStateNotCleared()
            throws Exception {
        mWifiMetrics.setNetworkRecommenderBlacklistSize(OPEN_NET_NOTIFIER_TAG,
                SIZE_OPEN_NETWORK_RECOMMENDER_BLACKLIST);
        mWifiMetrics.setIsWifiNetworksAvailableNotificationEnabled(OPEN_NET_NOTIFIER_TAG,
                IS_WIFI_NETWORKS_AVAILABLE_NOTIFICATION_ON);
        for (int i = 0; i < NUM_OPEN_NETWORK_RECOMMENDATION_UPDATES; i++) {
            mWifiMetrics.incrementNumNetworkRecommendationUpdates(OPEN_NET_NOTIFIER_TAG);
        }

        // This should clear most metrics in mWifiMetrics
        dumpProtoAndDeserialize();
        assertEquals(SIZE_OPEN_NETWORK_RECOMMENDER_BLACKLIST,
                mDecodedProto.openNetworkRecommenderBlacklistSize);
        assertEquals(IS_WIFI_NETWORKS_AVAILABLE_NOTIFICATION_ON,
                mDecodedProto.isWifiNetworksAvailableNotificationOn);
        assertEquals(NUM_OPEN_NETWORK_RECOMMENDATION_UPDATES,
                mDecodedProto.numOpenNetworkRecommendationUpdates);

        // Check that blacklist size and feature state persist on next dump but
        // others do not.
        dumpProtoAndDeserialize();
        assertEquals(SIZE_OPEN_NETWORK_RECOMMENDER_BLACKLIST,
                mDecodedProto.openNetworkRecommenderBlacklistSize);
        assertEquals(IS_WIFI_NETWORKS_AVAILABLE_NOTIFICATION_ON,
                mDecodedProto.isWifiNetworksAvailableNotificationOn);
        assertEquals(0, mDecodedProto.numOpenNetworkRecommendationUpdates);
    }

    /**
     * Check network selector id
     */
    @Test
    public void testNetworkSelectorExperimentId() throws Exception {
        final int id = 42888888;
        mWifiMetrics.setNetworkSelectorExperimentId(id);
        mWifiMetrics.startConnectionEvent(mTestWifiConfig, "TestNetwork",
                WifiMetricsProto.ConnectionEvent.ROAM_ENTERPRISE);
        mWifiMetrics.endConnectionEvent(
                WifiMetrics.ConnectionEvent.FAILURE_NONE,
                WifiMetricsProto.ConnectionEvent.HLF_NONE,
                WifiMetricsProto.ConnectionEvent.FAILURE_REASON_UNKNOWN);
        dumpProtoAndDeserialize();
        assertEquals(id, mDecodedProto.connectionEvent[0].networkSelectorExperimentId);
    }

    /**
     * Check ScoringParams
     */
    @Test
    public void testExperimentId() throws Exception {
        final int id = 42;
        final String expectId = "x" + id;
        when(mScoringParams.getExperimentIdentifier()).thenReturn(id);
        dumpProtoAndDeserialize();
        assertEquals(expectId, mDecodedProto.scoreExperimentId);
    }

    /**
     * Check ScoringParams default case
     */
    @Test
    public void testDefaultExperimentId() throws Exception {
        final int id = 0;
        final String expectId = "";
        when(mScoringParams.getExperimentIdentifier()).thenReturn(id);
        dumpProtoAndDeserialize();
        assertEquals(expectId, mDecodedProto.scoreExperimentId);
    }

    /** short hand for instantiating an anonymous int array, instead of 'new int[]{a1, a2, ...}' */
    private int[] a(int... element) {
        return element;
    }

    private void verifyHist(WifiMetricsProto.NumConnectableNetworksBucket[] hist, int size,
            int[] keys, int[] counts) throws Exception {
        assertEquals(size, hist.length);
        for (int i = 0; i < keys.length; i++) {
            assertEquals(keys[i], hist[i].numConnectableNetworks);
            assertEquals(counts[i], hist[i].count);
        }
    }

    /**
     * Generate an RSSI delta event by creating a connection event and an RSSI poll within
     * 'interArrivalTime' milliseconds of each other.
     * Event will not be logged if interArrivalTime > mWifiMetrics.TIMEOUT_RSSI_DELTA_MILLIS
     * successfulConnectionEvent, completeConnectionEvent, useValidScanResult and
     * dontDeserializeBeforePoll
     * each create an anomalous condition when set to false.
     */
    private void generateRssiDelta(int scanRssi, int rssiDelta,
            long interArrivalTime, boolean successfulConnectionEvent,
            boolean completeConnectionEvent, boolean useValidScanResult,
            boolean dontDeserializeBeforePoll) throws Exception {
        when(mClock.getElapsedSinceBootMillis()).thenReturn((long) 0);
        ScanResult scanResult = null;
        if (useValidScanResult) {
            scanResult = mock(ScanResult.class);
            scanResult.level = scanRssi;
        }
        WifiConfiguration config = mock(WifiConfiguration.class);
        WifiConfiguration.NetworkSelectionStatus networkSelectionStat =
                mock(WifiConfiguration.NetworkSelectionStatus.class);
        when(networkSelectionStat.getCandidate()).thenReturn(scanResult);
        when(config.getNetworkSelectionStatus()).thenReturn(networkSelectionStat);
        mWifiMetrics.startConnectionEvent(config, "TestNetwork",
                WifiMetricsProto.ConnectionEvent.ROAM_ENTERPRISE);
        if (completeConnectionEvent) {
            if (successfulConnectionEvent) {
                mWifiMetrics.endConnectionEvent(
                        WifiMetrics.ConnectionEvent.FAILURE_NONE,
                        WifiMetricsProto.ConnectionEvent.HLF_NONE,
                        WifiMetricsProto.ConnectionEvent.FAILURE_REASON_UNKNOWN);
            } else {
                mWifiMetrics.endConnectionEvent(
                        WifiMetrics.ConnectionEvent.FAILURE_AUTHENTICATION_FAILURE,
                        WifiMetricsProto.ConnectionEvent.HLF_NONE,
                        WifiMetricsProto.ConnectionEvent.FAILURE_REASON_UNKNOWN);
            }
        }
        when(mClock.getElapsedSinceBootMillis()).thenReturn(interArrivalTime);
        if (!dontDeserializeBeforePoll) {
            dumpProtoAndDeserialize();
        }
        mWifiMetrics.incrementRssiPollRssiCount(RSSI_POLL_FREQUENCY, scanRssi + rssiDelta);
    }

    /**
     * Generate an RSSI delta event, with all extra conditions set to true.
     */
    private void generateRssiDelta(int scanRssi, int rssiDelta,
            long interArrivalTime) throws Exception {
        generateRssiDelta(scanRssi, rssiDelta, interArrivalTime, true, true, true, true);
    }

    private void assertStringContains(
            String actualString, String expectedSubstring) {
        assertTrue("Expected text not found in: " + actualString,
                actualString.contains(expectedSubstring));
    }

    private String getStateDump() {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        PrintWriter writer = new PrintWriter(stream);
        String[] args = new String[0];
        mWifiMetrics.dump(null, writer, args);
        writer.flush();
        return stream.toString();
    }

    private static final int TEST_ALLOWED_KEY_MANAGEMENT = 83;
    private static final int TEST_ALLOWED_PROTOCOLS = 22;
    private static final int TEST_ALLOWED_AUTH_ALGORITHMS = 11;
    private static final int TEST_ALLOWED_PAIRWISE_CIPHERS = 67;
    private static final int TEST_ALLOWED_GROUP_CIPHERS = 231;
    private static final int TEST_CANDIDATE_LEVEL = -80;
    private static final int TEST_CANDIDATE_FREQ = 2345;

    private WifiConfiguration createComplexWifiConfig() {
        WifiConfiguration config = new WifiConfiguration();
        config.allowedKeyManagement = intToBitSet(TEST_ALLOWED_KEY_MANAGEMENT);
        config.allowedProtocols = intToBitSet(TEST_ALLOWED_PROTOCOLS);
        config.allowedAuthAlgorithms = intToBitSet(TEST_ALLOWED_AUTH_ALGORITHMS);
        config.allowedPairwiseCiphers = intToBitSet(TEST_ALLOWED_PAIRWISE_CIPHERS);
        config.allowedGroupCiphers = intToBitSet(TEST_ALLOWED_GROUP_CIPHERS);
        config.hiddenSSID = true;
        config.ephemeral = true;
        config.getNetworkSelectionStatus().setHasEverConnected(true);
        ScanResult candidate = new ScanResult();
        candidate.level = TEST_CANDIDATE_LEVEL;
        candidate.frequency = TEST_CANDIDATE_FREQ;
        config.getNetworkSelectionStatus().setCandidate(candidate);
        return config;
    }

    private void assertConfigInfoEqualsWifiConfig(WifiConfiguration config,
            StaEvent.ConfigInfo info) {
        if (config == null && info == null) return;
        assertEquals(config.allowedKeyManagement,   intToBitSet(info.allowedKeyManagement));
        assertEquals(config.allowedProtocols,       intToBitSet(info.allowedProtocols));
        assertEquals(config.allowedAuthAlgorithms,  intToBitSet(info.allowedAuthAlgorithms));
        assertEquals(config.allowedPairwiseCiphers, intToBitSet(info.allowedPairwiseCiphers));
        assertEquals(config.allowedGroupCiphers,    intToBitSet(info.allowedGroupCiphers));
        assertEquals(config.hiddenSSID, info.hiddenSsid);
        assertEquals(config.ephemeral, info.isEphemeral);
        assertEquals(config.getNetworkSelectionStatus().getHasEverConnected(),
                info.hasEverConnected);
        assertEquals(config.getNetworkSelectionStatus().getCandidate().level, info.scanRssi);
        assertEquals(config.getNetworkSelectionStatus().getCandidate().frequency, info.scanFreq);
    }

    /**
     * Sets the values of bitSet to match an int mask
     */
    private static BitSet intToBitSet(int mask) {
        BitSet bitSet = new BitSet();
        for (int bitIndex = 0; mask > 0; mask >>>= 1, bitIndex++) {
            if ((mask & 1) != 0) bitSet.set(bitIndex);
        }
        return bitSet;
    }

    private static final int NUM_UNUSABLE_EVENT = 5;
    private static final int NUM_UNUSABLE_EVENT_TIME_THROTTLE = 3;

    /**
     * Values used to generate WifiIsUnusableEvent
     * <WifiIsUnusableEvent.TriggerType>, <last_score>, <tx_success_delta>, <tx_retries_delta>,
     * <tx_bad_delta>, <rx_success_delta>, <packet_update_time_delta>, <firmware_alert_code>,
     * <last_wifi_usability_score>
     */
    private int[][] mTestUnusableEvents = {
        {WifiIsUnusableEvent.TYPE_DATA_STALL_BAD_TX,        60,  60,  50,  40,  30,  1000,  -1, 51},
        {WifiIsUnusableEvent.TYPE_DATA_STALL_TX_WITHOUT_RX, 55,  40,  30,  0,   0,   500,   -1, 52},
        {WifiIsUnusableEvent.TYPE_DATA_STALL_BOTH,          60,  90,  30,  30,  0,   1000,  -1, 53},
        {WifiIsUnusableEvent.TYPE_FIRMWARE_ALERT,           55,  55,  30,  15,  10,  1000,   4, 54},
        {WifiIsUnusableEvent.TYPE_IP_REACHABILITY_LOST,     50,  56,  28,  17,  12,  1000,  -1, 45}
    };

    /**
     * Generate all WifiIsUnusableEvents from mTestUnusableEvents
     */
    private void generateAllUnusableEvents(WifiMetrics wifiMetrics) {
        for (int i = 0; i < mTestUnusableEvents.length; i++) {
            generateUnusableEventAtGivenTime(i, i * (WifiMetrics.MIN_DATA_STALL_WAIT_MS + 1000));
        }
    }

    /**
     * Generate a WifiIsUnusableEvent at the given timestamp with data from
     * mTestUnusableEvents[index]
     */
    private void generateUnusableEventAtGivenTime(int index, long eventTime) {
        when(mClock.getElapsedSinceBootMillis()).thenReturn(eventTime);
        int[] trigger = mTestUnusableEvents[index];
        mWifiMetrics.incrementWifiScoreCount(trigger[1]);
        mWifiMetrics.incrementWifiUsabilityScoreCount(1, trigger[8], 15);
        mWifiMetrics.updateWifiIsUnusableLinkLayerStats(trigger[2], trigger[3], trigger[4],
                trigger[5], trigger[6]);
        mWifiMetrics.setScreenState(true);
        switch(trigger[0]) {
            case WifiIsUnusableEvent.TYPE_DATA_STALL_BAD_TX:
            case WifiIsUnusableEvent.TYPE_DATA_STALL_TX_WITHOUT_RX:
            case WifiIsUnusableEvent.TYPE_DATA_STALL_BOTH:
                mWifiMetrics.logWifiIsUnusableEvent(trigger[0]);
                break;
            case WifiIsUnusableEvent.TYPE_FIRMWARE_ALERT:
                mWifiMetrics.logWifiIsUnusableEvent(trigger[0], trigger[7]);
                break;
            case WifiIsUnusableEvent.TYPE_IP_REACHABILITY_LOST:
                mWifiMetrics.logWifiIsUnusableEvent(trigger[0]);
                break;
            default:
                break;
        }
    }

    /**
     * Verify that WifiIsUnusableEvent in wifiLog matches mTestUnusableEvents
     */
    private void verifyDeserializedUnusableEvents(WifiMetricsProto.WifiLog wifiLog) {
        assertEquals(NUM_UNUSABLE_EVENT, wifiLog.wifiIsUnusableEventList.length);
        for (int i = 0; i < mTestUnusableEvents.length; i++) {
            WifiIsUnusableEvent event = wifiLog.wifiIsUnusableEventList[i];
            verifyUnusableEvent(event, i);
        }
    }

    /**
     * Verify that the given WifiIsUnusableEvent matches mTestUnusableEvents
     * at given index
     */
    private void verifyUnusableEvent(WifiIsUnusableEvent event, int index) {
        int[] expectedValues = mTestUnusableEvents[index];
        assertEquals(expectedValues[0], event.type);
        assertEquals(expectedValues[1], event.lastScore);
        assertEquals(expectedValues[2], event.txSuccessDelta);
        assertEquals(expectedValues[3], event.txRetriesDelta);
        assertEquals(expectedValues[4], event.txBadDelta);
        assertEquals(expectedValues[5], event.rxSuccessDelta);
        assertEquals(expectedValues[6], event.packetUpdateTimeDelta);
        assertEquals(expectedValues[7], event.firmwareAlertCode);
        assertEquals(expectedValues[8], event.lastWifiUsabilityScore);
        assertEquals(true, event.screenOn);
    }

    /**
     * Verify that no WifiIsUnusableEvent is generated when it is disabled in the settings
     */
    @Test
    public void testNoUnusableEventLogWhenDisabled() throws Exception {
        when(mFacade.getIntegerSetting(eq(mContext),
                eq(Settings.Global.WIFI_IS_UNUSABLE_EVENT_METRICS_ENABLED),
                anyInt())).thenReturn(0);
        mWifiMetrics.loadSettings();
        generateAllUnusableEvents(mWifiMetrics);
        dumpProtoAndDeserialize();
        assertEquals(0, mDecodedProto.wifiIsUnusableEventList.length);
    }

    /**
     * Generate WifiIsUnusableEvent and verify that they are logged correctly
     */
    @Test
    public void testUnusableEventLogSerializeDeserialize() throws Exception {
        when(mFacade.getIntegerSetting(eq(mContext),
                eq(Settings.Global.WIFI_IS_UNUSABLE_EVENT_METRICS_ENABLED),
                anyInt())).thenReturn(1);
        mWifiMetrics.loadSettings();
        generateAllUnusableEvents(mWifiMetrics);
        dumpProtoAndDeserialize();
        verifyDeserializedUnusableEvents(mDecodedProto);
    }

    /**
     * Verify that the number of WifiIsUnusableEvents does not exceed MAX_UNUSABLE_EVENTS
     */
    @Test
    public void testUnusableEventBounding() throws Exception {
        when(mFacade.getIntegerSetting(eq(mContext),
                eq(Settings.Global.WIFI_IS_UNUSABLE_EVENT_METRICS_ENABLED),
                anyInt())).thenReturn(1);
        mWifiMetrics.loadSettings();
        for (int i = 0; i < (WifiMetrics.MAX_UNUSABLE_EVENTS + 2); i++) {
            generateAllUnusableEvents(mWifiMetrics);
        }
        dumpProtoAndDeserialize();
        assertEquals(WifiMetrics.MAX_UNUSABLE_EVENTS, mDecodedProto.wifiIsUnusableEventList.length);
    }

    /**
     * Verify that we don't generate new WifiIsUnusableEvent from data stalls
     * until MIN_DATA_STALL_WAIT_MS has passed since the last data stall WifiIsUnusableEvent
     */
    @Test
    public void testUnusableEventTimeThrottleForDataStall() throws Exception {
        when(mFacade.getIntegerSetting(eq(mContext),
                eq(Settings.Global.WIFI_IS_UNUSABLE_EVENT_METRICS_ENABLED),
                anyInt())).thenReturn(1);
        mWifiMetrics.loadSettings();
        generateUnusableEventAtGivenTime(0, 0);
        // should be time throttled
        generateUnusableEventAtGivenTime(1, 1);
        generateUnusableEventAtGivenTime(2, WifiMetrics.MIN_DATA_STALL_WAIT_MS + 1000);
        // no time throttle for firmware alert
        generateUnusableEventAtGivenTime(3, WifiMetrics.MIN_DATA_STALL_WAIT_MS + 1001);
        dumpProtoAndDeserialize();
        assertEquals(NUM_UNUSABLE_EVENT_TIME_THROTTLE,
                mDecodedProto.wifiIsUnusableEventList.length);
        verifyUnusableEvent(mDecodedProto.wifiIsUnusableEventList[0], 0);
        verifyUnusableEvent(mDecodedProto.wifiIsUnusableEventList[1], 2);
        verifyUnusableEvent(mDecodedProto.wifiIsUnusableEventList[2], 3);
    }

    /**
     * Verify that LinkSpeedCounts is correctly logged in metrics
     */
    @Test
    public void testLinkSpeedCounts() throws Exception {
        when(mFacade.getIntegerSetting(eq(mContext),
                eq(Settings.Global.WIFI_LINK_SPEED_METRICS_ENABLED), anyInt())).thenReturn(1);
        mWifiMetrics.loadSettings();
        for (int i = 0; i < NUM_LINK_SPEED_LEVELS_TO_INCREMENT; i++) {
            for (int j = 0; j <= i; j++) {
                mWifiMetrics.incrementLinkSpeedCount(
                        WifiMetrics.MIN_LINK_SPEED_MBPS + i, TEST_RSSI_LEVEL);
            }
        }
        dumpProtoAndDeserialize();
        assertEquals(NUM_LINK_SPEED_LEVELS_TO_INCREMENT, mDecodedProto.linkSpeedCounts.length);
        for (int i = 0; i < NUM_LINK_SPEED_LEVELS_TO_INCREMENT; i++) {
            assertEquals("Incorrect link speed", WifiMetrics.MIN_LINK_SPEED_MBPS + i,
                    mDecodedProto.linkSpeedCounts[i].linkSpeedMbps);
            assertEquals("Incorrect count of link speed",
                    i + 1, mDecodedProto.linkSpeedCounts[i].count);
            assertEquals("Incorrect sum of absolute values of rssi values",
                    Math.abs(TEST_RSSI_LEVEL) * (i + 1),
                    mDecodedProto.linkSpeedCounts[i].rssiSumDbm);
            assertEquals("Incorrect sum of squares of rssi values",
                    TEST_RSSI_LEVEL * TEST_RSSI_LEVEL * (i + 1),
                    mDecodedProto.linkSpeedCounts[i].rssiSumOfSquaresDbmSq);
        }
    }

    /**
     * Verify that Tx and Rx per-band LinkSpeedCounts are correctly logged in metrics
     */
    @Test
    public void testTxRxLinkSpeedBandCounts() throws Exception {
        when(mFacade.getIntegerSetting(eq(mContext),
                eq(Settings.Global.WIFI_LINK_SPEED_METRICS_ENABLED), anyInt())).thenReturn(1);
        mWifiMetrics.loadSettings();
        for (int i = 0; i < NUM_LINK_SPEED_LEVELS_TO_INCREMENT; i++) {
            for (int j = 0; j <= i; j++) {
                mWifiMetrics.incrementTxLinkSpeedBandCount(
                        WifiMetrics.MIN_LINK_SPEED_MBPS + i, RSSI_POLL_FREQUENCY);
                mWifiMetrics.incrementRxLinkSpeedBandCount(
                        WifiMetrics.MIN_LINK_SPEED_MBPS + i + 1, RSSI_POLL_FREQUENCY);
            }
        }
        dumpProtoAndDeserialize();
        assertEquals(0, mDecodedProto.txLinkSpeedCount2G.length);
        assertEquals(0, mDecodedProto.rxLinkSpeedCount2G.length);
        assertEquals(NUM_LINK_SPEED_LEVELS_TO_INCREMENT,
                mDecodedProto.txLinkSpeedCount5GLow.length);
        assertEquals(NUM_LINK_SPEED_LEVELS_TO_INCREMENT,
                mDecodedProto.rxLinkSpeedCount5GLow.length);
        assertEquals(0, mDecodedProto.txLinkSpeedCount5GMid.length);
        assertEquals(0, mDecodedProto.rxLinkSpeedCount5GMid.length);
        assertEquals(0, mDecodedProto.txLinkSpeedCount5GHigh.length);
        assertEquals(0, mDecodedProto.rxLinkSpeedCount5GHigh.length);
        for (int i = 0; i < NUM_LINK_SPEED_LEVELS_TO_INCREMENT; i++) {
            assertEquals("Incorrect Tx link speed", WifiMetrics.MIN_LINK_SPEED_MBPS + i,
                    mDecodedProto.txLinkSpeedCount5GLow[i].key);
            assertEquals("Incorrect Rx link speed", WifiMetrics.MIN_LINK_SPEED_MBPS + i + 1,
                    mDecodedProto.rxLinkSpeedCount5GLow[i].key);
            assertEquals("Incorrect count of Tx link speed",
                    i + 1, mDecodedProto.txLinkSpeedCount5GLow[i].count);
            assertEquals("Incorrect count of Rx link speed",
                    i + 1, mDecodedProto.rxLinkSpeedCount5GLow[i].count);
        }
    }

    /**
     * Verify that LinkSpeedCounts is not logged when disabled in settings
     */
    @Test
    public void testNoLinkSpeedCountsWhenDisabled() throws Exception {
        when(mFacade.getIntegerSetting(eq(mContext),
                eq(Settings.Global.WIFI_LINK_SPEED_METRICS_ENABLED), anyInt())).thenReturn(0);
        mWifiMetrics.loadSettings();
        for (int i = 0; i < NUM_LINK_SPEED_LEVELS_TO_INCREMENT; i++) {
            for (int j = 0; j <= i; j++) {
                mWifiMetrics.incrementLinkSpeedCount(
                        WifiMetrics.MIN_LINK_SPEED_MBPS + i, TEST_RSSI_LEVEL);
                mWifiMetrics.incrementTxLinkSpeedBandCount(
                        WifiMetrics.MIN_LINK_SPEED_MBPS - i, RSSI_POLL_FREQUENCY);
                mWifiMetrics.incrementRxLinkSpeedBandCount(
                        WifiMetrics.MIN_LINK_SPEED_MBPS - i, RSSI_POLL_FREQUENCY);
            }
        }
        dumpProtoAndDeserialize();
        assertEquals("LinkSpeedCounts should not be logged when disabled in settings",
                0, mDecodedProto.linkSpeedCounts.length);
        assertEquals("Tx LinkSpeedCounts should not be logged when disabled in settings",
                0, mDecodedProto.txLinkSpeedCount5GLow.length);
        assertEquals("Rx LinkSpeedCounts should not be logged when disabled in settings",
                0, mDecodedProto.rxLinkSpeedCount5GLow.length);
    }

    /**
     * Verify that LinkSpeedCounts is not logged when the link speed value is lower than
     * MIN_LINK_SPEED_MBPS or when the rssi value is outside of
     * [MIN_RSSI_LEVEL, MAX_RSSI_LEVEL]
     */
    @Test
    public void testNoLinkSpeedCountsForOutOfBoundValues() throws Exception {
        when(mFacade.getIntegerSetting(eq(mContext),
                eq(Settings.Global.WIFI_LINK_SPEED_METRICS_ENABLED), anyInt())).thenReturn(1);
        mWifiMetrics.loadSettings();
        for (int i = 1; i < NUM_OUT_OF_BOUND_ENTRIES; i++) {
            mWifiMetrics.incrementLinkSpeedCount(
                    WifiMetrics.MIN_LINK_SPEED_MBPS - i, MIN_RSSI_LEVEL);
            mWifiMetrics.incrementTxLinkSpeedBandCount(
                    WifiMetrics.MIN_LINK_SPEED_MBPS - i, RSSI_POLL_FREQUENCY);
            mWifiMetrics.incrementRxLinkSpeedBandCount(
                    WifiMetrics.MIN_LINK_SPEED_MBPS - i, RSSI_POLL_FREQUENCY);
        }
        for (int i = 1; i < NUM_OUT_OF_BOUND_ENTRIES; i++) {
            mWifiMetrics.incrementLinkSpeedCount(
                    WifiMetrics.MIN_LINK_SPEED_MBPS, MIN_RSSI_LEVEL - i);
        }
        for (int i = 1; i < NUM_OUT_OF_BOUND_ENTRIES; i++) {
            mWifiMetrics.incrementLinkSpeedCount(
                    WifiMetrics.MIN_LINK_SPEED_MBPS, MAX_RSSI_LEVEL + i);
        }
        dumpProtoAndDeserialize();
        assertEquals("LinkSpeedCounts should not be logged for out of bound values",
                0, mDecodedProto.linkSpeedCounts.length);
        assertEquals("Tx LinkSpeedCounts should not be logged for out of bound values",
                0, mDecodedProto.txLinkSpeedCount5GLow.length);
        assertEquals("Rx LinkSpeedCounts should not be logged for out of bound values",
                0, mDecodedProto.rxLinkSpeedCount5GLow.length);
    }

    private int nextRandInt() {
        return mRandom.nextInt(1000);
    }

    private WifiLinkLayerStats nextRandomStats(WifiLinkLayerStats current) {
        WifiLinkLayerStats out = new WifiLinkLayerStats();
        out.timeStampInMs = current.timeStampInMs + nextRandInt();

        out.rxmpdu_be = current.rxmpdu_be + nextRandInt();
        out.txmpdu_be = current.txmpdu_be + nextRandInt();
        out.lostmpdu_be = current.lostmpdu_be + nextRandInt();
        out.retries_be = current.retries_be + nextRandInt();

        out.rxmpdu_bk = current.rxmpdu_bk + nextRandInt();
        out.txmpdu_bk = current.txmpdu_bk + nextRandInt();
        out.lostmpdu_bk = current.lostmpdu_bk + nextRandInt();
        out.retries_bk = current.retries_bk + nextRandInt();

        out.rxmpdu_vi = current.rxmpdu_vi + nextRandInt();
        out.txmpdu_vi = current.txmpdu_vi + nextRandInt();
        out.lostmpdu_vi = current.lostmpdu_vi + nextRandInt();
        out.retries_vi = current.retries_vi + nextRandInt();

        out.rxmpdu_vo = current.rxmpdu_vo + nextRandInt();
        out.txmpdu_vo = current.txmpdu_vo + nextRandInt();
        out.lostmpdu_vo = current.lostmpdu_vo + nextRandInt();
        out.retries_vo = current.retries_vo + nextRandInt();

        out.on_time = current.on_time + nextRandInt();
        out.tx_time = current.tx_time + nextRandInt();
        out.rx_time = current.rx_time + nextRandInt();
        out.on_time_scan = current.on_time_scan + nextRandInt();
        out.on_time_nan_scan = current.on_time_nan_scan + nextRandInt();
        out.on_time_background_scan = current.on_time_background_scan + nextRandInt();
        out.on_time_roam_scan = current.on_time_roam_scan + nextRandInt();
        out.on_time_pno_scan = current.on_time_pno_scan + nextRandInt();
        out.on_time_hs20_scan = current.on_time_hs20_scan + nextRandInt();
        return out;
    }

    private void assertWifiLinkLayerUsageHasDiff(WifiLinkLayerStats oldStats,
            WifiLinkLayerStats newStats) {
        assertEquals(newStats.timeStampInMs - oldStats.timeStampInMs,
                mDecodedProto.wifiLinkLayerUsageStats.loggingDurationMs);
        assertEquals(newStats.on_time - oldStats.on_time,
                mDecodedProto.wifiLinkLayerUsageStats.radioOnTimeMs);
        assertEquals(newStats.tx_time - oldStats.tx_time,
                mDecodedProto.wifiLinkLayerUsageStats.radioTxTimeMs);
        assertEquals(newStats.rx_time - oldStats.rx_time,
                mDecodedProto.wifiLinkLayerUsageStats.radioRxTimeMs);
        assertEquals(newStats.on_time_scan - oldStats.on_time_scan,
                mDecodedProto.wifiLinkLayerUsageStats.radioScanTimeMs);
        assertEquals(newStats.on_time_nan_scan - oldStats.on_time_nan_scan,
                mDecodedProto.wifiLinkLayerUsageStats.radioNanScanTimeMs);
        assertEquals(newStats.on_time_background_scan - oldStats.on_time_background_scan,
                mDecodedProto.wifiLinkLayerUsageStats.radioBackgroundScanTimeMs);
        assertEquals(newStats.on_time_roam_scan - oldStats.on_time_roam_scan,
                mDecodedProto.wifiLinkLayerUsageStats.radioRoamScanTimeMs);
        assertEquals(newStats.on_time_pno_scan - oldStats.on_time_pno_scan,
                mDecodedProto.wifiLinkLayerUsageStats.radioPnoScanTimeMs);
        assertEquals(newStats.on_time_hs20_scan - oldStats.on_time_hs20_scan,
                mDecodedProto.wifiLinkLayerUsageStats.radioHs20ScanTimeMs);

    }

    /**
     * Verify that WifiMetrics is counting link layer usage correctly when given a series of
     * valid input.
     * @throws Exception
     */
    @Test
    public void testWifiLinkLayerUsageStats() throws Exception {
        WifiLinkLayerStats stat1 = nextRandomStats(new WifiLinkLayerStats());
        WifiLinkLayerStats stat2 = nextRandomStats(stat1);
        WifiLinkLayerStats stat3 = nextRandomStats(stat2);
        mWifiMetrics.incrementWifiLinkLayerUsageStats(stat1);
        mWifiMetrics.incrementWifiLinkLayerUsageStats(stat2);
        mWifiMetrics.incrementWifiLinkLayerUsageStats(stat3);
        dumpProtoAndDeserialize();

        // After 2 increments, the counters should have difference between |stat1| and |stat3|
        assertWifiLinkLayerUsageHasDiff(stat1, stat3);
    }

    /**
     * Verify that null input is handled and wifi link layer usage stats are not incremented.
     * @throws Exception
     */
    @Test
    public void testWifiLinkLayerUsageStatsNullInput() throws Exception {
        WifiLinkLayerStats stat1 = nextRandomStats(new WifiLinkLayerStats());
        WifiLinkLayerStats stat2 = null;
        mWifiMetrics.incrementWifiLinkLayerUsageStats(stat1);
        mWifiMetrics.incrementWifiLinkLayerUsageStats(stat2);
        dumpProtoAndDeserialize();

        // Counter should be zero
        assertWifiLinkLayerUsageHasDiff(stat1, stat1);
    }

    /**
     * Verify that when the new data appears to be bad link layer usage stats are not being
     * incremented and the buffered WifiLinkLayerStats get cleared.
     * @throws Exception
     */
    @Test
    public void testWifiLinkLayerUsageStatsChipReset() throws Exception {
        WifiLinkLayerStats stat1 = nextRandomStats(new WifiLinkLayerStats());
        WifiLinkLayerStats stat2 = nextRandomStats(stat1);
        stat2.on_time = stat1.on_time - 1;
        WifiLinkLayerStats stat3 = nextRandomStats(stat2);
        WifiLinkLayerStats stat4 = nextRandomStats(stat3);
        mWifiMetrics.incrementWifiLinkLayerUsageStats(stat1);
        mWifiMetrics.incrementWifiLinkLayerUsageStats(stat2);
        mWifiMetrics.incrementWifiLinkLayerUsageStats(stat3);
        mWifiMetrics.incrementWifiLinkLayerUsageStats(stat4);
        dumpProtoAndDeserialize();

        // Should only count the difference between |stat3| and |stat4|
        assertWifiLinkLayerUsageHasDiff(stat3, stat4);
    }

    private void assertUsabilityStatsAssignment(WifiInfo info, WifiLinkLayerStats stats,
            WifiUsabilityStatsEntry usabilityStats) {
        assertEquals(info.getRssi(), usabilityStats.rssi);
        assertEquals(info.getLinkSpeed(), usabilityStats.linkSpeedMbps);
        assertEquals(info.getRxLinkSpeedMbps(), usabilityStats.rxLinkSpeedMbps);
        assertEquals(stats.timeStampInMs, usabilityStats.timeStampMs);
        assertEquals(stats.txmpdu_be + stats.txmpdu_bk + stats.txmpdu_vi + stats.txmpdu_vo,
                usabilityStats.totalTxSuccess);
        assertEquals(stats.retries_be + stats.retries_bk + stats.retries_vi + stats.retries_vo,
                usabilityStats.totalTxRetries);
        assertEquals(stats.lostmpdu_be + stats.lostmpdu_bk + stats.lostmpdu_vi + stats.lostmpdu_vo,
                usabilityStats.totalTxBad);
        assertEquals(stats.rxmpdu_be + stats.rxmpdu_bk + stats.rxmpdu_vi + stats.rxmpdu_vo,
                usabilityStats.totalRxSuccess);
        assertEquals(stats.on_time, usabilityStats.totalRadioOnTimeMs);
        assertEquals(stats.tx_time, usabilityStats.totalRadioTxTimeMs);
        assertEquals(stats.rx_time, usabilityStats.totalRadioRxTimeMs);
        assertEquals(stats.on_time_scan, usabilityStats.totalScanTimeMs);
        assertEquals(stats.on_time_nan_scan, usabilityStats.totalNanScanTimeMs);
        assertEquals(stats.on_time_background_scan, usabilityStats.totalBackgroundScanTimeMs);
        assertEquals(stats.on_time_roam_scan, usabilityStats.totalRoamScanTimeMs);
        assertEquals(stats.on_time_pno_scan, usabilityStats.totalPnoScanTimeMs);
        assertEquals(stats.on_time_hs20_scan, usabilityStats.totalHotspot2ScanTimeMs);
        assertEquals(stats.beacon_rx, usabilityStats.totalBeaconRx);
    }

    // Simulate adding a LABEL_GOOD WifiUsabilityStats
    private WifiLinkLayerStats addGoodWifiUsabilityStats(WifiLinkLayerStats start) {
        WifiInfo info = mock(WifiInfo.class);
        when(info.getRssi()).thenReturn(nextRandInt());
        when(info.getLinkSpeed()).thenReturn(nextRandInt());
        WifiLinkLayerStats stats = start;
        for (int i = 0; i < WifiMetrics.NUM_WIFI_USABILITY_STATS_ENTRIES_PER_WIFI_GOOD; i++) {
            mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats);
            stats = nextRandomStats(stats);
        }
        return stats;
    }

    // Simulate adding a LABEL_BAD WifiUsabilityStats
    private WifiLinkLayerStats addBadWifiUsabilityStats(WifiLinkLayerStats start) {
        WifiInfo info = mock(WifiInfo.class);
        when(info.getRssi()).thenReturn(nextRandInt());
        when(info.getLinkSpeed()).thenReturn(nextRandInt());
        WifiLinkLayerStats stats1 = start;
        WifiLinkLayerStats stats2 = nextRandomStats(stats1);
        mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats1);
        mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats2);
        mWifiMetrics.addToWifiUsabilityStatsList(WifiUsabilityStats.LABEL_BAD,
                WifiUsabilityStats.TYPE_DATA_STALL_BAD_TX, -1);
        return nextRandomStats(stats2);
    }

    /**
     * Verify that updateWifiUsabilityStatsEntries correctly converts the inputs into
     * a WifiUsabilityStatsEntry Object and then stores it.
     *
     * Verify that the converted metrics proto contains pairs of WifiUsabilityStats with
     * LABEL_GOOD and LABEL_BAD
     * @throws Exception
     */
    @Test
    public void testUpdateWifiUsabilityStatsEntries() throws Exception {
        WifiInfo info = mock(WifiInfo.class);
        when(info.getRssi()).thenReturn(nextRandInt());
        when(info.getLinkSpeed()).thenReturn(nextRandInt());
        when(info.getRxLinkSpeedMbps()).thenReturn(nextRandInt());
        when(info.getBSSID()).thenReturn("Wifi");
        when(info.getFrequency()).thenReturn(5745);

        int signalStrengthDbm = -50;
        int signalStrengthDb = -10;
        boolean isSameRegisteredCell = true;
        CellularLinkLayerStats cellularStats =  new CellularLinkLayerStats();
        cellularStats.setIsSameRegisteredCell(isSameRegisteredCell);
        cellularStats.setDataNetworkType(TelephonyManager.NETWORK_TYPE_LTE);
        cellularStats.setSignalStrengthDbm(signalStrengthDbm);
        cellularStats.setSignalStrengthDb(signalStrengthDb);
        when(mCellularLinkLayerStatsCollector.update()).thenReturn(cellularStats);

        WifiLinkLayerStats stats1 = nextRandomStats(new WifiLinkLayerStats());
        WifiLinkLayerStats stats2 = nextRandomStats(stats1);
        mWifiMetrics.incrementWifiScoreCount(60);
        mWifiMetrics.incrementWifiUsabilityScoreCount(2, 55, 15);
        mWifiMetrics.logLinkProbeSuccess(nextRandInt(), nextRandInt(), nextRandInt(), 12);
        mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats1);
        mWifiMetrics.incrementWifiScoreCount(58);
        mWifiMetrics.incrementWifiUsabilityScoreCount(3, 56, 15);
        mWifiMetrics.logLinkProbeFailure(nextRandInt(), nextRandInt(),
                nextRandInt(), nextRandInt());
        mWifiMetrics.enterDeviceMobilityState(DEVICE_MOBILITY_STATE_HIGH_MVMT);

        mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats2);
        mWifiMetrics.addToWifiUsabilityStatsList(WifiUsabilityStats.LABEL_BAD,
                WifiUsabilityStats.TYPE_DATA_STALL_BAD_TX, -1);

        // Add 2 LABEL_GOOD but only 1 should remain in the converted proto
        WifiLinkLayerStats statsGood = addGoodWifiUsabilityStats(nextRandomStats(stats2));
        statsGood.timeStampInMs += WifiMetrics.MIN_WIFI_GOOD_USABILITY_STATS_PERIOD_MS;
        addGoodWifiUsabilityStats(statsGood);

        dumpProtoAndDeserialize();
        assertEquals(2, mDecodedProto.wifiUsabilityStatsList.length);
        assertEquals(WifiUsabilityStats.LABEL_GOOD, mDecodedProto.wifiUsabilityStatsList[0].label);
        assertEquals(WifiUsabilityStats.LABEL_BAD, mDecodedProto.wifiUsabilityStatsList[1].label);
        assertUsabilityStatsAssignment(info, stats1,
                mDecodedProto.wifiUsabilityStatsList[1].stats[0]);
        assertUsabilityStatsAssignment(info, stats2,
                mDecodedProto.wifiUsabilityStatsList[1].stats[1]);

        assertEquals(2, mDecodedProto.wifiUsabilityStatsList[1].stats[0].seqNumToFramework);
        assertEquals(3, mDecodedProto.wifiUsabilityStatsList[1].stats[1].seqNumToFramework);
        assertEquals(0, mDecodedProto.wifiUsabilityStatsList[1].stats[0].seqNumInsideFramework);
        assertEquals(1, mDecodedProto.wifiUsabilityStatsList[1].stats[1].seqNumInsideFramework);
        assertEquals(60, mDecodedProto.wifiUsabilityStatsList[1].stats[0].wifiScore);
        assertEquals(58, mDecodedProto.wifiUsabilityStatsList[1].stats[1].wifiScore);
        assertEquals(55, mDecodedProto.wifiUsabilityStatsList[1].stats[0].wifiUsabilityScore);
        assertEquals(56, mDecodedProto.wifiUsabilityStatsList[1].stats[1].wifiUsabilityScore);
        assertEquals(15, mDecodedProto.wifiUsabilityStatsList[1].stats[0].predictionHorizonSec);
        assertEquals(true, mDecodedProto.wifiUsabilityStatsList[1].stats[0].isSameBssidAndFreq);
        assertEquals(android.net.wifi.WifiUsabilityStatsEntry.PROBE_STATUS_SUCCESS,
                mDecodedProto.wifiUsabilityStatsList[1].stats[0].probeStatusSinceLastUpdate);
        assertEquals(android.net.wifi.WifiUsabilityStatsEntry.PROBE_STATUS_FAILURE,
                mDecodedProto.wifiUsabilityStatsList[1].stats[1].probeStatusSinceLastUpdate);
        assertEquals(android.net.wifi.WifiUsabilityStatsEntry.PROBE_STATUS_NO_PROBE,
                mDecodedProto.wifiUsabilityStatsList[0].stats[0].probeStatusSinceLastUpdate);
        assertEquals(12,
                mDecodedProto.wifiUsabilityStatsList[1].stats[0].probeElapsedTimeSinceLastUpdateMs);
        assertEquals(Integer.MAX_VALUE, mDecodedProto.wifiUsabilityStatsList[1]
                .stats[1].probeElapsedTimeSinceLastUpdateMs);
        assertEquals(-1, mDecodedProto.wifiUsabilityStatsList[0]
                .stats[0].probeElapsedTimeSinceLastUpdateMs);
        assertEquals(WifiUsabilityStatsEntry.NETWORK_TYPE_LTE,
                mDecodedProto.wifiUsabilityStatsList[0].stats[0].cellularDataNetworkType);
        assertEquals(signalStrengthDbm,
                mDecodedProto.wifiUsabilityStatsList[0].stats[0].cellularSignalStrengthDbm);
        assertEquals(signalStrengthDb,
                mDecodedProto.wifiUsabilityStatsList[0].stats[0].cellularSignalStrengthDb);
        assertEquals(isSameRegisteredCell,
                mDecodedProto.wifiUsabilityStatsList[0].stats[0].isSameRegisteredCell);
        assertEquals(DEVICE_MOBILITY_STATE_HIGH_MVMT, mDecodedProto.wifiUsabilityStatsList[1]
                .stats[mDecodedProto.wifiUsabilityStatsList[1].stats.length - 1]
                .deviceMobilityState);
    }

    /**
     * Verify that when there are no WifiUsability events the generated proto also contains no
     * such information.
     * @throws Exception
     */
    @Test
    public void testWifiUsabilityStatsZeroEvents() throws Exception {
        dumpProtoAndDeserialize();
        assertEquals(0, mDecodedProto.wifiUsabilityStatsList.length);
    }

    /**
     * Verify that we discard a WifiUsabilityStats with LABEL_GOOD if there is no corresponding
     * LABEL_BAD
     * @throws Exception
     */
    @Test
    public void testWifiUsabilityStatsIgnoreSingleLabelGood() throws Exception {
        addGoodWifiUsabilityStats(new WifiLinkLayerStats());
        dumpProtoAndDeserialize();
        assertEquals(0, mDecodedProto.wifiUsabilityStatsList.length);
    }

    /**
     * Verify that we discard a WifiUsabilityStats with LABEL_BAD if there is no corresponding
     * LABEL_GOOD
     * @throws Exception
     */
    @Test
    public void testWifiUsabilityStatsIgnoreSingleLabelBad() throws Exception {
        addBadWifiUsabilityStats(new WifiLinkLayerStats());
        dumpProtoAndDeserialize();
        assertEquals(0, mDecodedProto.wifiUsabilityStatsList.length);
    }

    /**
     * Verify that the buffer for WifiUsabilityStats does not exceed the max length.
     * Do this by trying to add more WifiUsabilityStats than the max length and then
     * verifying that the decoded proto's length does not exceed the max length.
     *
     * Also verify that the length for the list of WifiUsabilityStatsEntry is capped.
     * @throws Exception
     */
    @Test
    public void testWifiUsabilityStatsBufferSizeIsCapped() throws Exception {
        // simulate adding LABEL_GOOD WifiUsabilityStats 1 time over the max limit
        WifiLinkLayerStats stats = new WifiLinkLayerStats();
        for (int j = 0; j < WifiMetrics.MAX_WIFI_USABILITY_STATS_LIST_SIZE_PER_TYPE + 1; j++) {
            stats = addGoodWifiUsabilityStats(stats);
            stats = addBadWifiUsabilityStats(stats);
            stats.timeStampInMs += WifiMetrics.MIN_WIFI_GOOD_USABILITY_STATS_PERIOD_MS;
        }
        dumpProtoAndDeserialize();
        assertEquals(2 * WifiMetrics.MAX_WIFI_USABILITY_STATS_PER_TYPE_TO_UPLOAD,
                mDecodedProto.wifiUsabilityStatsList.length);
        for (int i = 0; i < WifiMetrics.MAX_WIFI_USABILITY_STATS_PER_TYPE_TO_UPLOAD; i++) {
            assertEquals(WifiMetrics.MAX_WIFI_USABILITY_STATS_ENTRIES_LIST_SIZE,
                    mDecodedProto.wifiUsabilityStatsList[2 * i].stats.length);
            assertEquals(2, mDecodedProto.wifiUsabilityStatsList[2 * i + 1].stats.length);
        }
    }

    /**
     * Verify that LABEL_GOOD stats are not generated more frequently than
     * |MIN_WIFI_GOOD_USABILITY_STATS_PERIOD_MS|
     * @throws Exception
     */
    @Test
    public void testWifiUsabilityStatsLabelGoodHasMinimumPeriod() throws Exception {
        // simulate adding LABEL_GOOD WifiUsabilityStats 1 time over the max limit
        WifiLinkLayerStats stats = new WifiLinkLayerStats();
        for (int j = 0; j < 2; j++) {
            stats = addGoodWifiUsabilityStats(stats);
            stats = addBadWifiUsabilityStats(stats);
        }
        dumpProtoAndDeserialize();
        assertEquals(2, mDecodedProto.wifiUsabilityStatsList.length);
    }

    /**
     * Verify that LABEL_BAD stats are not generated more frequently than |MIN_DATA_STALL_WAIT_MS|
     * @throws Exception
     */
    @Test
    public void testWifiUsabilityStatsLabelBadNotGeneratedGapLessThanMinimum() throws Exception {
        // simulate adding two LABEL_GOOD WifiUsabilityStats
        WifiInfo info = mock(WifiInfo.class);
        when(info.getRssi()).thenReturn(nextRandInt());
        when(info.getLinkSpeed()).thenReturn(nextRandInt());
        WifiLinkLayerStats stats1 = new WifiLinkLayerStats();
        WifiLinkLayerStats stats2 = new WifiLinkLayerStats();
        stats1 = addGoodWifiUsabilityStats(stats1);
        stats2.timeStampInMs = stats1.timeStampInMs
                + WifiMetrics.MIN_WIFI_GOOD_USABILITY_STATS_PERIOD_MS;
        addGoodWifiUsabilityStats(stats2);

        WifiLinkLayerStats stats3 = new WifiLinkLayerStats();
        WifiLinkLayerStats stats4 = new WifiLinkLayerStats();
        for (int i = 0; i < WifiMetrics.MAX_WIFI_USABILITY_STATS_ENTRIES_LIST_SIZE - 1; i++) {
            mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats3);
            stats3 = nextRandomStats(stats3);
        }
        mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats3);
        mWifiMetrics.addToWifiUsabilityStatsList(WifiUsabilityStats.LABEL_BAD,
                WifiUsabilityStats.TYPE_DATA_STALL_BAD_TX, -1);
        for (int i = 0; i < WifiMetrics.MAX_WIFI_USABILITY_STATS_ENTRIES_LIST_SIZE - 1; i++) {
            mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats4);
            stats4 = nextRandomStats(stats4);
        }
        stats4.timeStampInMs = stats3.timeStampInMs - 1 + WifiMetrics.MIN_DATA_STALL_WAIT_MS;
        mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats4);
        mWifiMetrics.addToWifiUsabilityStatsList(WifiUsabilityStats.LABEL_BAD,
                WifiUsabilityStats.TYPE_DATA_STALL_BAD_TX, -1);
        dumpProtoAndDeserialize();
        assertEquals(2, mDecodedProto.wifiUsabilityStatsList.length);
    }

    /**
     * Verify that LABEL_BAD stats are generated if timestamp gap is larger than
     * |MIN_DATA_STALL_WAIT_MS|
     * @throws Exception
     */
    @Test
    public void testWifiUsabilityStatsLabelBadGeneratedGapLargerThanMinimum() throws Exception {
        // simulate adding two LABEL_GOOD WifiUsabilityStats
        WifiInfo info = mock(WifiInfo.class);
        when(info.getRssi()).thenReturn(nextRandInt());
        when(info.getLinkSpeed()).thenReturn(nextRandInt());
        WifiLinkLayerStats stats1 = new WifiLinkLayerStats();
        WifiLinkLayerStats stats2 = new WifiLinkLayerStats();
        stats1 = addGoodWifiUsabilityStats(stats1);
        stats2.timeStampInMs = stats1.timeStampInMs
                + WifiMetrics.MIN_WIFI_GOOD_USABILITY_STATS_PERIOD_MS;
        addGoodWifiUsabilityStats(stats2);

        WifiLinkLayerStats stats3 = new WifiLinkLayerStats();
        WifiLinkLayerStats stats4 = new WifiLinkLayerStats();
        for (int i = 0; i < WifiMetrics.MAX_WIFI_USABILITY_STATS_ENTRIES_LIST_SIZE - 1; i++) {
            mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats3);
            stats3 = nextRandomStats(stats3);
        }
        mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats3);
        mWifiMetrics.addToWifiUsabilityStatsList(WifiUsabilityStats.LABEL_BAD,
                WifiUsabilityStats.TYPE_DATA_STALL_BAD_TX, -1);
        for (int i = 0; i < WifiMetrics.MAX_WIFI_USABILITY_STATS_ENTRIES_LIST_SIZE - 1; i++) {
            mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats4);
            stats4 = nextRandomStats(stats4);
        }
        stats4.timeStampInMs = stats3.timeStampInMs + 1 + WifiMetrics.MIN_DATA_STALL_WAIT_MS;
        mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats4);
        mWifiMetrics.addToWifiUsabilityStatsList(WifiUsabilityStats.LABEL_BAD,
                WifiUsabilityStats.TYPE_DATA_STALL_BAD_TX, -1);
        dumpProtoAndDeserialize();
        assertEquals(4, mDecodedProto.wifiUsabilityStatsList.length);
    }

    /**
     * Tests device mobility state metrics as states are changed.
     */
    @Test
    public void testDeviceMobilityStateMetrics_changeState() throws Exception {
        // timeMs is initialized to 0 by the setUp() method
        long timeMs = 1000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(timeMs);
        mWifiMetrics.enterDeviceMobilityState(DEVICE_MOBILITY_STATE_STATIONARY);

        timeMs += 2000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(timeMs);
        mWifiMetrics.enterDeviceMobilityState(DEVICE_MOBILITY_STATE_LOW_MVMT);

        dumpProtoAndDeserialize();

        DeviceMobilityStatePnoScanStats[] expected = {
                buildDeviceMobilityStatePnoScanStats(DEVICE_MOBILITY_STATE_UNKNOWN, 1, 1000, 0),
                buildDeviceMobilityStatePnoScanStats(DEVICE_MOBILITY_STATE_STATIONARY, 1, 2000, 0),
                buildDeviceMobilityStatePnoScanStats(DEVICE_MOBILITY_STATE_LOW_MVMT, 1, 0, 0)
        };

        assertDeviceMobilityStatePnoScanStatsEqual(
                expected, mDecodedProto.mobilityStatePnoStatsList);
    }

    /**
     * Tests device mobility state metrics as PNO scans are started and stopped.
     */
    @Test
    public void testDeviceMobilityStateMetrics_startStopPnoScans() throws Exception {
        long timeMs = 1000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(timeMs);
        mWifiMetrics.logPnoScanStart();

        timeMs += 2000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(timeMs);
        mWifiMetrics.logPnoScanStop();
        mWifiMetrics.enterDeviceMobilityState(DEVICE_MOBILITY_STATE_STATIONARY);
        mWifiMetrics.logPnoScanStart();

        timeMs += 4000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(timeMs);
        mWifiMetrics.logPnoScanStop();

        timeMs += 8000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(timeMs);
        mWifiMetrics.enterDeviceMobilityState(DEVICE_MOBILITY_STATE_HIGH_MVMT);

        dumpProtoAndDeserialize();

        DeviceMobilityStatePnoScanStats[] expected = {
                buildDeviceMobilityStatePnoScanStats(DEVICE_MOBILITY_STATE_UNKNOWN,
                        1, 1000 + 2000, 2000),
                buildDeviceMobilityStatePnoScanStats(DEVICE_MOBILITY_STATE_STATIONARY,
                        1, 4000 + 8000, 4000),
                buildDeviceMobilityStatePnoScanStats(DEVICE_MOBILITY_STATE_HIGH_MVMT, 1, 0, 0)
        };

        assertDeviceMobilityStatePnoScanStatsEqual(
                expected, mDecodedProto.mobilityStatePnoStatsList);
    }

    /**
     * Tests that the initial state is set up correctly.
     */
    @Test
    public void testDeviceMobilityStateMetrics_initialState() throws Exception {
        dumpProtoAndDeserialize();

        DeviceMobilityStatePnoScanStats[] expected = {
                buildDeviceMobilityStatePnoScanStats(DEVICE_MOBILITY_STATE_UNKNOWN, 1, 0, 0)
        };

        assertDeviceMobilityStatePnoScanStatsEqual(
                expected, mDecodedProto.mobilityStatePnoStatsList);
    }

    /**
     * Tests that logPnoScanStart() updates the total duration in addition to the PNO duration.
     */
    @Test
    public void testDeviceMobilityStateMetrics_startPnoScansUpdatesTotalDuration()
            throws Exception {
        long timeMs = 1000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(timeMs);
        mWifiMetrics.logPnoScanStart();

        dumpProtoAndDeserialize();

        DeviceMobilityStatePnoScanStats[] expected = {
                buildDeviceMobilityStatePnoScanStats(DEVICE_MOBILITY_STATE_UNKNOWN, 1, 1000, 0)
        };

        assertDeviceMobilityStatePnoScanStatsEqual(
                expected, mDecodedProto.mobilityStatePnoStatsList);
    }

    /**
     * Tests that logPnoScanStop() updates the total duration in addition to the PNO duration.
     */
    @Test
    public void testDeviceMobilityStateMetrics_stopPnoScansUpdatesTotalDuration()
            throws Exception {
        long timeMs = 1000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(timeMs);
        mWifiMetrics.logPnoScanStart();

        timeMs += 2000;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(timeMs);
        mWifiMetrics.logPnoScanStop();

        dumpProtoAndDeserialize();

        DeviceMobilityStatePnoScanStats[] expected = {
                buildDeviceMobilityStatePnoScanStats(DEVICE_MOBILITY_STATE_UNKNOWN,
                        1, 1000 + 2000, 2000)
        };

        assertDeviceMobilityStatePnoScanStatsEqual(
                expected, mDecodedProto.mobilityStatePnoStatsList);
    }

    /**
     * Verify that clients should be notified of activity in case Wifi stats get updated.
     */
    @Test
    public void testClientNotification() throws RemoteException {
        // Register Client for verification.
        ArgumentCaptor<android.net.wifi.WifiUsabilityStatsEntry> usabilityStats =
                ArgumentCaptor.forClass(android.net.wifi.WifiUsabilityStatsEntry.class);
        mWifiMetrics.addOnWifiUsabilityListener(mAppBinder, mOnWifiUsabilityStatsListener,
                TEST_WIFI_USABILITY_STATS_LISTENER_IDENTIFIER);
        WifiInfo info = mock(WifiInfo.class);
        when(info.getRssi()).thenReturn(nextRandInt());
        when(info.getLinkSpeed()).thenReturn(nextRandInt());

        CellularLinkLayerStats cellularStats = new CellularLinkLayerStats();
        cellularStats.setIsSameRegisteredCell(false);
        cellularStats.setDataNetworkType(TelephonyManager.NETWORK_TYPE_UMTS);
        cellularStats.setSignalStrengthDbm(-100);
        cellularStats.setSignalStrengthDb(-20);
        when(mCellularLinkLayerStatsCollector.update()).thenReturn(cellularStats);

        WifiLinkLayerStats linkLayerStats = nextRandomStats(new WifiLinkLayerStats());
        mWifiMetrics.updateWifiUsabilityStatsEntries(info, linkLayerStats);

        // Client should get the stats.
        verify(mOnWifiUsabilityStatsListener).onWifiUsabilityStats(anyInt(), anyBoolean(),
                usabilityStats.capture());
        assertEquals(usabilityStats.getValue().getTotalRadioOnTimeMillis(), linkLayerStats.on_time);
        assertEquals(usabilityStats.getValue().getTotalTxBad(), linkLayerStats.lostmpdu_be
                + linkLayerStats.lostmpdu_bk + linkLayerStats.lostmpdu_vi
                + linkLayerStats.lostmpdu_vo);
        assertEquals(usabilityStats.getValue().getTimeStampMillis(), linkLayerStats.timeStampInMs);
        assertEquals(usabilityStats.getValue().getTotalRoamScanTimeMillis(),
                linkLayerStats.on_time_roam_scan);
        assertEquals(usabilityStats.getValue().getCellularDataNetworkType(),
                TelephonyManager.NETWORK_TYPE_UMTS);
        assertEquals(usabilityStats.getValue().getCellularSignalStrengthDbm(), -100);
        assertEquals(usabilityStats.getValue().getCellularSignalStrengthDb(), -20);
    }

    /**
     * Verify that remove client should be handled
     */
    @Test
    public void testRemoveClient() throws RemoteException {
        // Register Client for verification.
        mWifiMetrics.addOnWifiUsabilityListener(mAppBinder, mOnWifiUsabilityStatsListener,
                TEST_WIFI_USABILITY_STATS_LISTENER_IDENTIFIER);
        mWifiMetrics.removeOnWifiUsabilityListener(TEST_WIFI_USABILITY_STATS_LISTENER_IDENTIFIER);
        verify(mAppBinder).unlinkToDeath(any(), anyInt());

        WifiInfo info = mock(WifiInfo.class);
        when(info.getRssi()).thenReturn(nextRandInt());
        when(info.getLinkSpeed()).thenReturn(nextRandInt());
        WifiLinkLayerStats linkLayerStats = nextRandomStats(new WifiLinkLayerStats());
        mWifiMetrics.updateWifiUsabilityStatsEntries(info, linkLayerStats);

        verify(mOnWifiUsabilityStatsListener, never()).onWifiUsabilityStats(anyInt(),
                anyBoolean(), any());
    }

    /**
     * Verify that WifiMetrics adds for death notification on adding client.
     */
    @Test
    public void testAddsForBinderDeathOnAddClient() throws Exception {
        mWifiMetrics.addOnWifiUsabilityListener(mAppBinder, mOnWifiUsabilityStatsListener,
                TEST_WIFI_USABILITY_STATS_LISTENER_IDENTIFIER);
        verify(mAppBinder).linkToDeath(any(IBinder.DeathRecipient.class), anyInt());
    }

    /**
     * Verify that client fails to get message when listener add failed.
     */
    @Test
    public void testAddsListenerFailureOnLinkToDeath() throws Exception {
        doThrow(new RemoteException())
                .when(mAppBinder).linkToDeath(any(IBinder.DeathRecipient.class), anyInt());
        mWifiMetrics.addOnWifiUsabilityListener(mAppBinder, mOnWifiUsabilityStatsListener,
                TEST_WIFI_USABILITY_STATS_LISTENER_IDENTIFIER);
        verify(mAppBinder).linkToDeath(any(IBinder.DeathRecipient.class), anyInt());

        WifiInfo info = mock(WifiInfo.class);
        when(info.getRssi()).thenReturn(nextRandInt());
        when(info.getLinkSpeed()).thenReturn(nextRandInt());
        WifiLinkLayerStats linkLayerStats = nextRandomStats(new WifiLinkLayerStats());
        mWifiMetrics.updateWifiUsabilityStatsEntries(info, linkLayerStats);

        // Client should not get any message listener add failed.
        verify(mOnWifiUsabilityStatsListener, never()).onWifiUsabilityStats(anyInt(),
                anyBoolean(), any());
    }

    /**
     * Verify that the label and the triggerType of Wifi usability stats are saved correctly
     * during firmware alert is triggered.
     * @throws Exception
     */
    @Test
    public void verifyFirmwareAlertUpdatesWifiUsabilityMetrics() throws Exception {
        WifiInfo info = mock(WifiInfo.class);
        when(info.getRssi()).thenReturn(nextRandInt());
        when(info.getLinkSpeed()).thenReturn(nextRandInt());
        long eventTimeMs = nextRandInt();
        when(mClock.getElapsedSinceBootMillis()).thenReturn(eventTimeMs);
        WifiLinkLayerStats stats1 = nextRandomStats(new WifiLinkLayerStats());
        mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats1);

        // Add 1 LABEL_GOOD
        WifiLinkLayerStats statsGood = addGoodWifiUsabilityStats(nextRandomStats(stats1));
        // Firmware alert occurs
        mWifiMetrics.logFirmwareAlert(2);

        dumpProtoAndDeserialize();
        assertEquals(2, mDecodedProto.wifiUsabilityStatsList.length);

        WifiUsabilityStats[] statsList = mDecodedProto.wifiUsabilityStatsList;
        assertEquals(WifiUsabilityStats.LABEL_GOOD, statsList[0].label);
        assertEquals(WifiUsabilityStats.LABEL_BAD, statsList[1].label);
        assertEquals(WifiIsUnusableEvent.TYPE_FIRMWARE_ALERT, statsList[1].triggerType);
        assertEquals(eventTimeMs, statsList[1].timeStampMs);
        assertEquals(2, statsList[1].firmwareAlertCode);
    }

    /**
     * Verify that the label and the triggerType of Wifi usability stats are saved correctly
     * during Wifi data stall is triggered.
     * @throws Exception
     */
    @Test
    public void verifyWifiDataStallUpdatesWifiUsabilityMetrics() throws Exception {
        WifiInfo info = mock(WifiInfo.class);
        when(info.getRssi()).thenReturn(nextRandInt());
        when(info.getLinkSpeed()).thenReturn(nextRandInt());
        long eventTimeMs = nextRandInt();
        when(mClock.getElapsedSinceBootMillis()).thenReturn(eventTimeMs);
        WifiLinkLayerStats stats1 = nextRandomStats(new WifiLinkLayerStats());
        mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats1);

        // Add 1 LABEL_GOOD
        WifiLinkLayerStats statsGood = addGoodWifiUsabilityStats(nextRandomStats(stats1));
        // Wifi data stall occurs
        mWifiMetrics.addToWifiUsabilityStatsList(WifiUsabilityStats.LABEL_BAD,
                WifiIsUnusableEvent.TYPE_DATA_STALL_BAD_TX, -1);

        dumpProtoAndDeserialize();
        assertEquals(2, mDecodedProto.wifiUsabilityStatsList.length);
        WifiUsabilityStats[] statsList = mDecodedProto.wifiUsabilityStatsList;
        assertEquals(WifiUsabilityStats.LABEL_BAD, statsList[1].label);
        assertEquals(WifiIsUnusableEvent.TYPE_DATA_STALL_BAD_TX, statsList[1].triggerType);
        assertEquals(-1, statsList[1].firmwareAlertCode);
        assertEquals(eventTimeMs, statsList[1].timeStampMs);
    }

    /**
     * Test the generation of 'WifiConfigStoreIODuration' read histograms.
     */
    @Test
    public void testWifiConfigStoreReadDurationsHistogramGeneration() throws Exception {
        mWifiMetrics.noteWifiConfigStoreReadDuration(10);
        mWifiMetrics.noteWifiConfigStoreReadDuration(20);
        mWifiMetrics.noteWifiConfigStoreReadDuration(100);
        mWifiMetrics.noteWifiConfigStoreReadDuration(90);
        mWifiMetrics.noteWifiConfigStoreReadDuration(130);
        mWifiMetrics.noteWifiConfigStoreReadDuration(250);
        mWifiMetrics.noteWifiConfigStoreReadDuration(600);

        dumpProtoAndDeserialize();

        assertEquals(5, mDecodedProto.wifiConfigStoreIo.readDurations.length);
        assertEquals(0, mDecodedProto.wifiConfigStoreIo.writeDurations.length);

        assertEquals(Integer.MIN_VALUE,
                mDecodedProto.wifiConfigStoreIo.readDurations[0].rangeStartMs);
        assertEquals(50, mDecodedProto.wifiConfigStoreIo.readDurations[0].rangeEndMs);
        assertEquals(2, mDecodedProto.wifiConfigStoreIo.readDurations[0].count);

        assertEquals(50, mDecodedProto.wifiConfigStoreIo.readDurations[1].rangeStartMs);
        assertEquals(100, mDecodedProto.wifiConfigStoreIo.readDurations[1].rangeEndMs);
        assertEquals(1, mDecodedProto.wifiConfigStoreIo.readDurations[1].count);

        assertEquals(100, mDecodedProto.wifiConfigStoreIo.readDurations[2].rangeStartMs);
        assertEquals(150, mDecodedProto.wifiConfigStoreIo.readDurations[2].rangeEndMs);
        assertEquals(2, mDecodedProto.wifiConfigStoreIo.readDurations[2].count);

        assertEquals(200, mDecodedProto.wifiConfigStoreIo.readDurations[3].rangeStartMs);
        assertEquals(300, mDecodedProto.wifiConfigStoreIo.readDurations[3].rangeEndMs);
        assertEquals(1, mDecodedProto.wifiConfigStoreIo.readDurations[3].count);

        assertEquals(300, mDecodedProto.wifiConfigStoreIo.readDurations[4].rangeStartMs);
        assertEquals(Integer.MAX_VALUE,
                mDecodedProto.wifiConfigStoreIo.readDurations[4].rangeEndMs);
        assertEquals(1, mDecodedProto.wifiConfigStoreIo.readDurations[4].count);
    }

    /**
     * Test the generation of 'WifiConfigStoreIODuration' write histograms.
     */
    @Test
    public void testWifiConfigStoreWriteDurationsHistogramGeneration() throws Exception {
        mWifiMetrics.noteWifiConfigStoreWriteDuration(10);
        mWifiMetrics.noteWifiConfigStoreWriteDuration(40);
        mWifiMetrics.noteWifiConfigStoreWriteDuration(60);
        mWifiMetrics.noteWifiConfigStoreWriteDuration(90);
        mWifiMetrics.noteWifiConfigStoreWriteDuration(534);
        mWifiMetrics.noteWifiConfigStoreWriteDuration(345);

        dumpProtoAndDeserialize();

        assertEquals(0, mDecodedProto.wifiConfigStoreIo.readDurations.length);
        assertEquals(3, mDecodedProto.wifiConfigStoreIo.writeDurations.length);

        assertEquals(Integer.MIN_VALUE,
                mDecodedProto.wifiConfigStoreIo.writeDurations[0].rangeStartMs);
        assertEquals(50, mDecodedProto.wifiConfigStoreIo.writeDurations[0].rangeEndMs);
        assertEquals(2, mDecodedProto.wifiConfigStoreIo.writeDurations[0].count);

        assertEquals(50, mDecodedProto.wifiConfigStoreIo.writeDurations[1].rangeStartMs);
        assertEquals(100, mDecodedProto.wifiConfigStoreIo.writeDurations[1].rangeEndMs);
        assertEquals(2, mDecodedProto.wifiConfigStoreIo.writeDurations[1].count);

        assertEquals(300, mDecodedProto.wifiConfigStoreIo.writeDurations[2].rangeStartMs);
        assertEquals(Integer.MAX_VALUE,
                mDecodedProto.wifiConfigStoreIo.writeDurations[2].rangeEndMs);
        assertEquals(2, mDecodedProto.wifiConfigStoreIo.writeDurations[2].count);
    }

    /**
     * Test link probe metrics.
     */
    @Test
    public void testLogLinkProbeMetrics() throws Exception {
        mWifiMetrics.logLinkProbeSuccess(10000, -75, 50, 5);
        mWifiMetrics.logLinkProbeFailure(30000, -80, 10,
                WifiNative.SEND_MGMT_FRAME_ERROR_NO_ACK);
        mWifiMetrics.logLinkProbeSuccess(3000, -71, 160, 12);
        mWifiMetrics.logLinkProbeFailure(40000, -80, 6,
                WifiNative.SEND_MGMT_FRAME_ERROR_NO_ACK);
        mWifiMetrics.logLinkProbeSuccess(5000, -73, 160, 10);
        mWifiMetrics.logLinkProbeFailure(2000, -78, 6,
                WifiNative.SEND_MGMT_FRAME_ERROR_TIMEOUT);

        dumpProtoAndDeserialize();

        StaEvent[] expected = {
                buildLinkProbeSuccessStaEvent(5),
                buildLinkProbeFailureStaEvent(LinkProbeStats.LINK_PROBE_FAILURE_REASON_NO_ACK),
                buildLinkProbeSuccessStaEvent(12),
                buildLinkProbeFailureStaEvent(LinkProbeStats.LINK_PROBE_FAILURE_REASON_NO_ACK),
                buildLinkProbeSuccessStaEvent(10),
                buildLinkProbeFailureStaEvent(LinkProbeStats.LINK_PROBE_FAILURE_REASON_TIMEOUT)
        };
        assertLinkProbeStaEventsEqual(expected, mDecodedProto.staEventList);

        LinkProbeStats linkProbeStats = mDecodedProto.linkProbeStats;

        Int32Count[] expectedSuccessRssiHistogram = {
                buildInt32Count(-75, 1),
                buildInt32Count(-73, 1),
                buildInt32Count(-71, 1),
        };
        assertKeyCountsEqual(expectedSuccessRssiHistogram,
                linkProbeStats.successRssiCounts);

        Int32Count[] expectedFailureRssiHistogram = {
                buildInt32Count(-80, 2),
                buildInt32Count(-78, 1),
        };
        assertKeyCountsEqual(expectedFailureRssiHistogram,
                linkProbeStats.failureRssiCounts);

        Int32Count[] expectedSuccessLinkSpeedHistogram = {
                buildInt32Count(50, 1),
                buildInt32Count(160, 2)
        };
        assertKeyCountsEqual(expectedSuccessLinkSpeedHistogram,
                linkProbeStats.successLinkSpeedCounts);

        Int32Count[] expectedFailureLinkSpeedHistogram = {
                buildInt32Count(6, 2),
                buildInt32Count(10, 1)
        };
        assertKeyCountsEqual(expectedFailureLinkSpeedHistogram,
                linkProbeStats.failureLinkSpeedCounts);

        HistogramBucketInt32[] expectedSuccessTimeSinceLastTxSuccessSecondsHistogram = {
                buildHistogramBucketInt32(Integer.MIN_VALUE, 5, 1),
                buildHistogramBucketInt32(5, 15, 2)
        };
        assertHistogramBucketsEqual(expectedSuccessTimeSinceLastTxSuccessSecondsHistogram,
                linkProbeStats.successSecondsSinceLastTxSuccessHistogram);

        HistogramBucketInt32[] expectedFailureTimeSinceLastTxSuccessSecondsHistogram = {
                buildHistogramBucketInt32(Integer.MIN_VALUE, 5, 1),
                buildHistogramBucketInt32(15, 45, 2)
        };
        assertHistogramBucketsEqual(expectedFailureTimeSinceLastTxSuccessSecondsHistogram,
                linkProbeStats.failureSecondsSinceLastTxSuccessHistogram);

        HistogramBucketInt32[] expectedSuccessElapsedTimeMsHistogram = {
                buildHistogramBucketInt32(5, 10, 1),
                buildHistogramBucketInt32(10, 15, 2),
        };
        assertHistogramBucketsEqual(expectedSuccessElapsedTimeMsHistogram,
                linkProbeStats.successElapsedTimeMsHistogram);

        LinkProbeFailureReasonCount[] expectedFailureReasonCount = {
                buildLinkProbeFailureReasonCount(
                        LinkProbeStats.LINK_PROBE_FAILURE_REASON_NO_ACK, 2),
                buildLinkProbeFailureReasonCount(
                        LinkProbeStats.LINK_PROBE_FAILURE_REASON_TIMEOUT, 1),
        };
        assertLinkProbeFailureReasonCountsEqual(expectedFailureReasonCount,
                linkProbeStats.failureReasonCounts);
    }

    /**
     * Tests counting the number of link probes triggered per day for each experiment.
     */
    @Test
    public void testIncrementLinkProbeExperimentProbeCount() throws Exception {
        String experimentId1 = "screenOnDelay=6000,noTxDelay=3000,delayBetweenProbes=9000,"
                + "rssiThreshold=-70,linkSpeedThreshold=15,";
        mWifiMetrics.incrementLinkProbeExperimentProbeCount(experimentId1);

        String experimentId2 = "screenOnDelay=9000,noTxDelay=12000,delayBetweenProbes=15000,"
                + "rssiThreshold=-72,linkSpeedThreshold=20,";
        mWifiMetrics.incrementLinkProbeExperimentProbeCount(experimentId2);
        mWifiMetrics.incrementLinkProbeExperimentProbeCount(experimentId2);

        dumpProtoAndDeserialize();

        ExperimentProbeCounts[] actual = mDecodedProto.linkProbeStats.experimentProbeCounts;

        ExperimentProbeCounts[] expected = {
                buildExperimentProbeCounts(experimentId1, 1),
                buildExperimentProbeCounts(experimentId2, 2)
        };

        assertExperimentProbeCountsEqual(expected, actual);
    }

    /**
     * Tests logNetworkSelectionDecision()
     */
    @Test
    public void testLogNetworkSelectionDecision() throws Exception {
        mWifiMetrics.logNetworkSelectionDecision(1, 2, true, 6);
        mWifiMetrics.logNetworkSelectionDecision(1, 2, false, 1);
        mWifiMetrics.logNetworkSelectionDecision(1, 2, true, 6);
        mWifiMetrics.logNetworkSelectionDecision(1, 2, true, 2);
        mWifiMetrics.logNetworkSelectionDecision(3, 2, false, 15);
        mWifiMetrics.logNetworkSelectionDecision(1, 2, false, 6);
        mWifiMetrics.logNetworkSelectionDecision(1, 4, true, 2);

        dumpProtoAndDeserialize();

        assertEquals(3, mDecodedProto.networkSelectionExperimentDecisionsList.length);

        NetworkSelectionExperimentDecisions exp12 =
                findUniqueNetworkSelectionExperimentDecisions(1, 2);
        Int32Count[] exp12SameExpected = {
                buildInt32Count(2, 1),
                buildInt32Count(6, 2)
        };
        assertKeyCountsEqual(exp12SameExpected, exp12.sameSelectionNumChoicesCounter);
        Int32Count[] exp12DiffExpected = {
                buildInt32Count(1, 1),
                buildInt32Count(6, 1)
        };
        assertKeyCountsEqual(exp12DiffExpected, exp12.differentSelectionNumChoicesCounter);

        NetworkSelectionExperimentDecisions exp32 =
                findUniqueNetworkSelectionExperimentDecisions(3, 2);
        Int32Count[] exp32SameExpected = {};
        assertKeyCountsEqual(exp32SameExpected, exp32.sameSelectionNumChoicesCounter);
        Int32Count[] exp32DiffExpected = {
                buildInt32Count(
                        WifiMetrics.NetworkSelectionExperimentResults.MAX_CHOICES, 1)
        };
        assertKeyCountsEqual(exp32DiffExpected, exp32.differentSelectionNumChoicesCounter);

        NetworkSelectionExperimentDecisions exp14 =
                findUniqueNetworkSelectionExperimentDecisions(1, 4);
        Int32Count[] exp14SameExpected = {
                buildInt32Count(2, 1)
        };
        assertKeyCountsEqual(exp14SameExpected, exp14.sameSelectionNumChoicesCounter);
        Int32Count[] exp14DiffExpected = {};
        assertKeyCountsEqual(exp14DiffExpected, exp14.differentSelectionNumChoicesCounter);
    }

    /**
     * Test the generation of 'WifiNetworkRequestApiLog' message.
     */
    @Test
    public void testWifiNetworkRequestApiLog() throws Exception {
        mWifiMetrics.incrementNetworkRequestApiNumRequest();
        mWifiMetrics.incrementNetworkRequestApiNumRequest();
        mWifiMetrics.incrementNetworkRequestApiNumRequest();

        mWifiMetrics.incrementNetworkRequestApiMatchSizeHistogram(7);
        mWifiMetrics.incrementNetworkRequestApiMatchSizeHistogram(0);
        mWifiMetrics.incrementNetworkRequestApiMatchSizeHistogram(1);

        mWifiMetrics.incrementNetworkRequestApiNumConnectSuccess();

        mWifiMetrics.incrementNetworkRequestApiNumUserApprovalBypass();
        mWifiMetrics.incrementNetworkRequestApiNumUserApprovalBypass();

        mWifiMetrics.incrementNetworkRequestApiNumUserReject();

        mWifiMetrics.incrementNetworkRequestApiNumApps();

        dumpProtoAndDeserialize();

        assertEquals(3, mDecodedProto.wifiNetworkRequestApiLog.numRequest);
        assertEquals(1, mDecodedProto.wifiNetworkRequestApiLog.numConnectSuccess);
        assertEquals(2, mDecodedProto.wifiNetworkRequestApiLog.numUserApprovalBypass);
        assertEquals(1, mDecodedProto.wifiNetworkRequestApiLog.numUserReject);
        assertEquals(1, mDecodedProto.wifiNetworkRequestApiLog.numApps);

        HistogramBucketInt32[] expectedNetworkMatchSizeHistogram = {
                buildHistogramBucketInt32(0, 1, 1),
                buildHistogramBucketInt32(1, 5, 1),
                buildHistogramBucketInt32(5, 10, 1)
        };
        assertHistogramBucketsEqual(expectedNetworkMatchSizeHistogram,
                mDecodedProto.wifiNetworkRequestApiLog.networkMatchSizeHistogram);
    }

    /**
     * Test the generation of 'WifiNetworkSuggestionApiLog' message.
     */
    @Test
    public void testWifiNetworkSuggestionApiLog() throws Exception {
        mWifiMetrics.incrementNetworkSuggestionApiNumModification();
        mWifiMetrics.incrementNetworkSuggestionApiNumModification();
        mWifiMetrics.incrementNetworkSuggestionApiNumModification();
        mWifiMetrics.incrementNetworkSuggestionApiNumModification();

        mWifiMetrics.incrementNetworkSuggestionApiNumConnectSuccess();
        mWifiMetrics.incrementNetworkSuggestionApiNumConnectSuccess();

        mWifiMetrics.incrementNetworkSuggestionApiNumConnectFailure();

        mWifiMetrics.noteNetworkSuggestionApiListSizeHistogram(new ArrayList<Integer>() {{
                add(5);
                add(100);
                add(50);
                add(120);
            }});
        // Second update should overwrite the prevous write.
        mWifiMetrics.noteNetworkSuggestionApiListSizeHistogram(new ArrayList<Integer>() {{
                add(7);
                add(110);
                add(40);
                add(60);
            }});

        dumpProtoAndDeserialize();

        assertEquals(4, mDecodedProto.wifiNetworkSuggestionApiLog.numModification);
        assertEquals(2, mDecodedProto.wifiNetworkSuggestionApiLog.numConnectSuccess);
        assertEquals(1, mDecodedProto.wifiNetworkSuggestionApiLog.numConnectFailure);

        HistogramBucketInt32[] expectedNetworkListSizeHistogram = {
                buildHistogramBucketInt32(5, 20, 1),
                buildHistogramBucketInt32(20, 50, 1),
                buildHistogramBucketInt32(50, 100, 1),
                buildHistogramBucketInt32(100, 500, 1),
        };
        assertHistogramBucketsEqual(expectedNetworkListSizeHistogram,
                mDecodedProto.wifiNetworkSuggestionApiLog.networkListSizeHistogram);
    }

    private NetworkSelectionExperimentDecisions findUniqueNetworkSelectionExperimentDecisions(
            int experiment1Id, int experiment2Id) {
        NetworkSelectionExperimentDecisions result = null;
        for (NetworkSelectionExperimentDecisions d
                : mDecodedProto.networkSelectionExperimentDecisionsList) {
            if (d.experiment1Id == experiment1Id && d.experiment2Id == experiment2Id) {
                assertNull("duplicate found!", result);
                result = d;
            }
        }
        assertNotNull("not found!", result);
        return result;
    }

    /**
     * Verify that the label and the triggerType of Wifi usability stats are saved correctly
     * during IP reachability lost message is received.
     * @throws Exception
     */
    @Test
    public void verifyIpReachabilityLostUpdatesWifiUsabilityMetrics() throws Exception {
        WifiInfo info = mock(WifiInfo.class);
        when(info.getRssi()).thenReturn(nextRandInt());
        when(info.getLinkSpeed()).thenReturn(nextRandInt());
        long eventTimeMs = nextRandInt();
        when(mClock.getElapsedSinceBootMillis()).thenReturn(eventTimeMs);
        WifiLinkLayerStats stats1 = nextRandomStats(new WifiLinkLayerStats());
        mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats1);

        // Add 1 LABEL_GOOD
        WifiLinkLayerStats statsGood = addGoodWifiUsabilityStats(nextRandomStats(stats1));
        // IP reachability lost occurs
        mWifiMetrics.addToWifiUsabilityStatsList(WifiUsabilityStats.LABEL_BAD,
                WifiUsabilityStats.TYPE_IP_REACHABILITY_LOST, -1);

        dumpProtoAndDeserialize();
        assertEquals(2, mDecodedProto.wifiUsabilityStatsList.length);
        WifiUsabilityStats[] statsList = mDecodedProto.wifiUsabilityStatsList;
        assertEquals(WifiUsabilityStats.LABEL_BAD, statsList[1].label);
        assertEquals(WifiUsabilityStats.TYPE_IP_REACHABILITY_LOST, statsList[1].triggerType);
        assertEquals(eventTimeMs, statsList[1].timeStampMs);
    }

    /**
     * Test the WifiLock active session statistics
     */
    @Test
    public void testWifiLockActiveSession() throws Exception {
        mWifiMetrics.addWifiLockActiveSession(WifiManager.WIFI_MODE_FULL_HIGH_PERF, 100000);
        mWifiMetrics.addWifiLockActiveSession(WifiManager.WIFI_MODE_FULL_HIGH_PERF, 10000);
        mWifiMetrics.addWifiLockActiveSession(WifiManager.WIFI_MODE_FULL_HIGH_PERF, 10000000);
        mWifiMetrics.addWifiLockActiveSession(WifiManager.WIFI_MODE_FULL_HIGH_PERF, 1000);

        mWifiMetrics.addWifiLockActiveSession(WifiManager.WIFI_MODE_FULL_LOW_LATENCY, 90000);
        mWifiMetrics.addWifiLockActiveSession(WifiManager.WIFI_MODE_FULL_LOW_LATENCY, 900000);
        mWifiMetrics.addWifiLockActiveSession(WifiManager.WIFI_MODE_FULL_LOW_LATENCY, 9000);
        mWifiMetrics.addWifiLockActiveSession(WifiManager.WIFI_MODE_FULL_LOW_LATENCY, 20000000);

        dumpProtoAndDeserialize();

        assertEquals(10111000, mDecodedProto.wifiLockStats.highPerfActiveTimeMs);
        assertEquals(20999000, mDecodedProto.wifiLockStats.lowLatencyActiveTimeMs);

        HistogramBucketInt32[] expectedHighPerfHistogram = {
                buildHistogramBucketInt32(1, 10, 1),
                buildHistogramBucketInt32(10, 60, 1),
                buildHistogramBucketInt32(60, 600, 1),
                buildHistogramBucketInt32(3600, Integer.MAX_VALUE, 1),
        };

        HistogramBucketInt32[] expectedLowLatencyHistogram = {
                buildHistogramBucketInt32(1, 10, 1),
                buildHistogramBucketInt32(60, 600, 1),
                buildHistogramBucketInt32(600, 3600, 1),
                buildHistogramBucketInt32(3600, Integer.MAX_VALUE, 1),
        };

        assertHistogramBucketsEqual(expectedHighPerfHistogram,
                mDecodedProto.wifiLockStats.highPerfActiveSessionDurationSecHistogram);

        assertHistogramBucketsEqual(expectedLowLatencyHistogram,
                mDecodedProto.wifiLockStats.lowLatencyActiveSessionDurationSecHistogram);
    }

    /**
     * Test the WifiLock acquisition session statistics
     */
    @Test
    public void testWifiLockAcqSession() throws Exception {
        mWifiMetrics.addWifiLockAcqSession(WifiManager.WIFI_MODE_FULL_HIGH_PERF, 100000);
        mWifiMetrics.addWifiLockAcqSession(WifiManager.WIFI_MODE_FULL_HIGH_PERF, 10000);
        mWifiMetrics.addWifiLockAcqSession(WifiManager.WIFI_MODE_FULL_HIGH_PERF, 10000000);
        mWifiMetrics.addWifiLockAcqSession(WifiManager.WIFI_MODE_FULL_HIGH_PERF, 1000);

        mWifiMetrics.addWifiLockAcqSession(WifiManager.WIFI_MODE_FULL_LOW_LATENCY, 90000);
        mWifiMetrics.addWifiLockAcqSession(WifiManager.WIFI_MODE_FULL_LOW_LATENCY, 900000);
        mWifiMetrics.addWifiLockAcqSession(WifiManager.WIFI_MODE_FULL_LOW_LATENCY, 9000);
        mWifiMetrics.addWifiLockAcqSession(WifiManager.WIFI_MODE_FULL_LOW_LATENCY, 20000000);

        dumpProtoAndDeserialize();

        HistogramBucketInt32[] expectedHighPerfHistogram = {
                buildHistogramBucketInt32(1, 10, 1),
                buildHistogramBucketInt32(10, 60, 1),
                buildHistogramBucketInt32(60, 600, 1),
                buildHistogramBucketInt32(3600, Integer.MAX_VALUE, 1),
        };

        HistogramBucketInt32[] expectedLowLatencyHistogram = {
                buildHistogramBucketInt32(1, 10, 1),
                buildHistogramBucketInt32(60, 600, 1),
                buildHistogramBucketInt32(600, 3600, 1),
                buildHistogramBucketInt32(3600, Integer.MAX_VALUE, 1),
        };

        assertHistogramBucketsEqual(expectedHighPerfHistogram,
                mDecodedProto.wifiLockStats.highPerfLockAcqDurationSecHistogram);

        assertHistogramBucketsEqual(expectedLowLatencyHistogram,
                mDecodedProto.wifiLockStats.lowLatencyLockAcqDurationSecHistogram);
    }

    /**
     * Verify that LABEL_GOOD stats are generated if Wifi score breaches low and there
     * is no WifiIsUnusableEvent in MIN_SCORE_BREACH_TO_GOOD_STATS_WAIT_TIME_MS millis
     * @throws Exception
     */
    @Test
    public void testGoodStatsAreGeneratedByWifiScoreBreachLow() throws Exception {
        // The elapsed time falls into the interval for adding good stats
        createTestForDataCollectionByScoreBreach(
                WifiMetrics.MIN_SCORE_BREACH_TO_GOOD_STATS_WAIT_TIME_MS + 1,
                false, true);
        dumpProtoAndDeserialize();
        assertEquals(2, mDecodedProto.wifiUsabilityStatsList.length);
    }

    /**
     * Verify that LABEL_GOOD stats are not generated if Wifi score breaches low and the checking
     * time is less than MIN_SCORE_BREACH_TO_GOOD_STATS_WAIT_TIME_MS millis
     * @throws Exception
     */
    @Test
    public void testGoodStatsAreNotGeneratedByWifiScoreBreachLow() throws Exception {
        // The elapsed time is shorter than necessary to add good stats
        createTestForDataCollectionByScoreBreach(
                WifiMetrics.MIN_SCORE_BREACH_TO_GOOD_STATS_WAIT_TIME_MS - 1,
                false, true);
        dumpProtoAndDeserialize();
        assertEquals(0, mDecodedProto.wifiUsabilityStatsList.length);
    }

    /**
     * Verify that LABEL_GOOD stats are not generated if Wifi score breaches low and the checking
     * time is greater than VALIDITY_PERIOD_OF_SCORE_BREACH_LOW_MS
     * @throws Exception
     */
    @Test
    public void testGoodStatsAreNotGeneratedIfWifiScoreBreachExpires() throws Exception {
        // The Wifi score breaching expires for adding good stats
        createTestForDataCollectionByScoreBreach(
                WifiMetrics.VALIDITY_PERIOD_OF_SCORE_BREACH_LOW_MS + 1,
                false, true);
        dumpProtoAndDeserialize();
        assertEquals(0, mDecodedProto.wifiUsabilityStatsList.length);
    }

    /**
     * Verify that LABEL_GOOD stats are not generated if Wifi score breaches low and there is
     * WifiIsUnusableEvent occured within MIN_SCORE_BREACH_TO_GOOD_STATS_WAIT_TIME_MS millis
     * @throws Exception
     */
    @Test
    public void testGoodStatsAreNotGeneratedIfBadEventOccured() throws Exception {
        // The elapsed time falls into the interval for adding good stats and bad event occurs
        createTestForDataCollectionByScoreBreach(
                WifiMetrics.MIN_SCORE_BREACH_TO_GOOD_STATS_WAIT_TIME_MS + 1,
                true, true);
        dumpProtoAndDeserialize();
        assertEquals(0, mDecodedProto.wifiUsabilityStatsList.length);
    }

    /**
     * Verify that LABEL_GOOD stats are generated if Wifi usability score breaches low and there
     * is no WifiIsUnusableEvent in MIN_SCORE_BREACH_TO_GOOD_STATS_WAIT_TIME_MS millis
     * @throws Exception
     */
    @Test
    public void testGoodStatsAreGeneratedByWifiUsabilityScoreBreachLow() throws Exception {
        // The elapsed time falls into the interval for adding good stats
        createTestForDataCollectionByScoreBreach(
                WifiMetrics.MIN_SCORE_BREACH_TO_GOOD_STATS_WAIT_TIME_MS + 1,
                false, false);
        dumpProtoAndDeserialize();
        assertEquals(2, mDecodedProto.wifiUsabilityStatsList.length);
    }

    /**
     * Verify that LABEL_GOOD stats are not generated if Wifi usability score breaches low and
     * the checking time is less than MIN_SCORE_BREACH_TO_GOOD_STATS_WAIT_TIME_MS millis
     * @throws Exception
     */
    @Test
    public void testGoodStatsAreNotGeneratedByWifiUsabilityScoreBreachLow() throws Exception {
        // The elapsed time is shorter than necessary to add good stats
        createTestForDataCollectionByScoreBreach(
                WifiMetrics.MIN_SCORE_BREACH_TO_GOOD_STATS_WAIT_TIME_MS - 1,
                false, false);
        dumpProtoAndDeserialize();
        assertEquals(0, mDecodedProto.wifiUsabilityStatsList.length);
    }

    /**
     * Verify that LABEL_GOOD stats are not generated if Wifi usability score breaches low and
     * the checking time is greater than VALIDITY_PERIOD_OF_SCORE_BREACH_LOW_MS
     * @throws Exception
     */
    @Test
    public void testGoodStatsAreNotGeneratedIfWifiUsabilityScoreBreachExpires() throws Exception {
        // The Wifi usability score breaching expires for adding good stats
        createTestForDataCollectionByScoreBreach(
                WifiMetrics.VALIDITY_PERIOD_OF_SCORE_BREACH_LOW_MS + 1,
                false, false);
        dumpProtoAndDeserialize();
        assertEquals(0, mDecodedProto.wifiUsabilityStatsList.length);
    }

    /**
     * Verify that LABEL_GOOD stats are not generated if Wifi usability score breaches low and there
     * is WifiIsUnusableEvent occured within MIN_SCORE_BREACH_TO_GOOD_STATS_WAIT_TIME_MS millis
     * @throws Exception
     */
    @Test
    public void testGoodStatsAreNotGeneratedIfBadEventOccuredForUsabilityScore() throws Exception {
        // The elapsed time falls into the interval for adding good stats and bad event occurs
        createTestForDataCollectionByScoreBreach(
                WifiMetrics.MIN_SCORE_BREACH_TO_GOOD_STATS_WAIT_TIME_MS + 1,
                true, false);
        dumpProtoAndDeserialize();
        assertEquals(0, mDecodedProto.wifiUsabilityStatsList.length);
    }

    /**
     * Verify that incrementNumWifiToggles increments the corrects fields based on input.
     */
    @Test
    public void testIncrementNumWifiToggles() throws Exception {
        mWifiMetrics.incrementNumWifiToggles(true, true);
        for (int i = 0; i < 2; i++) {
            mWifiMetrics.incrementNumWifiToggles(true, false);
        }
        for (int i = 0; i < 3; i++) {
            mWifiMetrics.incrementNumWifiToggles(false, true);
        }
        for (int i = 0; i < 4; i++) {
            mWifiMetrics.incrementNumWifiToggles(false, false);
        }
        dumpProtoAndDeserialize();
        assertEquals(1, mDecodedProto.wifiToggleStats.numToggleOnPrivileged);
        assertEquals(2, mDecodedProto.wifiToggleStats.numToggleOffPrivileged);
        assertEquals(3, mDecodedProto.wifiToggleStats.numToggleOnNormal);
        assertEquals(4, mDecodedProto.wifiToggleStats.numToggleOffNormal);
    }


    /**
     * Create a test to verify data collection logic triggered by score breaching low
     * @param elapsedTimeAfterBreach The elapsed time after score breaches low
     * @param isThereBadEvent Whether there is a bad event happened after score breaches low
     * @param isWifiScore Whether it is Wifi score or not that breaches the threshold
     */
    private void createTestForDataCollectionByScoreBreach(
            long elapsedTimeAfterBreach, boolean isThereBadEvent, boolean isWifiScore) {
        WifiInfo info = mock(WifiInfo.class);
        when(info.getRssi()).thenReturn(nextRandInt());
        when(info.getLinkSpeed()).thenReturn(nextRandInt());
        WifiLinkLayerStats stats2 = new WifiLinkLayerStats();
        mWifiMetrics.setWifiState(WifiMetricsProto.WifiLog.WIFI_ASSOCIATED);

        addOneBadWifiUsabilityStats(info);
        if (isWifiScore) {
            stats2 = wifiScoreBreachesLow(info, stats2);
        } else {
            stats2 = wifiUsabilityScoreBreachesLow(info, stats2);
        }
        if (isThereBadEvent) {
            mWifiMetrics.logWifiIsUnusableEvent(WifiIsUnusableEvent.TYPE_DATA_STALL_BAD_TX, -1);
        }
        when(mClock.getElapsedSinceBootMillis()).thenReturn(elapsedTimeAfterBreach);
        mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats2);
    }

    // Simulate adding one LABEL_BAD WifiUsabilityStats
    private void addOneBadWifiUsabilityStats(WifiInfo info) {
        WifiLinkLayerStats stats1 = new WifiLinkLayerStats();
        mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats1);
        mWifiMetrics.addToWifiUsabilityStatsList(WifiUsabilityStats.LABEL_BAD,
                WifiUsabilityStats.TYPE_DATA_STALL_BAD_TX, -1);
    }

    // Simulate that Wifi score breaches low
    private WifiLinkLayerStats wifiScoreBreachesLow(WifiInfo info, WifiLinkLayerStats stats2) {
        int upper = WifiMetrics.LOW_WIFI_SCORE + 7;
        int lower = WifiMetrics.LOW_WIFI_SCORE - 8;
        mWifiMetrics.incrementWifiScoreCount(upper);
        mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats2);
        stats2 = nextRandomStats(stats2);
        long timeMs = 0;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(timeMs);
        // Wifi score breaches low
        mWifiMetrics.incrementWifiScoreCount(lower);
        mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats2);
        stats2 = nextRandomStats(stats2);
        return stats2;
    }

    // Simulate that Wifi usability score breaches low
    private WifiLinkLayerStats wifiUsabilityScoreBreachesLow(WifiInfo info,
            WifiLinkLayerStats stats2) {
        int upper = WifiMetrics.LOW_WIFI_USABILITY_SCORE + 7;
        int lower = WifiMetrics.LOW_WIFI_USABILITY_SCORE - 8;
        mWifiMetrics.incrementWifiUsabilityScoreCount(1, upper, 30);
        mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats2);
        stats2 = nextRandomStats(stats2);
        long timeMs = 0;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(timeMs);
        // Wifi usability score breaches low
        mWifiMetrics.incrementWifiUsabilityScoreCount(2, lower, 30);
        mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats2);
        stats2 = nextRandomStats(stats2);
        return stats2;
    }

    /**
     * Verify the counts of passpoint profile type are correct.
     * @param profileTypes type and count of installed passpoint profiles
     */
    private void assertPasspointProfileTypeCount(PasspointProfileTypeCount[] profileTypes) {
        for (PasspointProfileTypeCount passpointProfileType : profileTypes) {
            switch(passpointProfileType.eapMethodType) {
                case PasspointProfileTypeCount.TYPE_EAP_AKA:
                    assertEquals(NUM_EAP_AKA_TYPE, passpointProfileType.count);
                    break;
                case PasspointProfileTypeCount.TYPE_EAP_AKA_PRIME:
                    assertEquals(NUM_EAP_AKA_PRIME_TYPE, passpointProfileType.count);
                    break;
                case PasspointProfileTypeCount.TYPE_EAP_SIM:
                    assertEquals(NUM_EAP_SIM_TYPE, passpointProfileType.count);
                    break;
                case PasspointProfileTypeCount.TYPE_EAP_TLS:
                    assertEquals(NUM_EAP_TLS_TYPE, passpointProfileType.count);
                    break;
                case PasspointProfileTypeCount.TYPE_EAP_TTLS:
                    assertEquals(NUM_EAP_TTLS_TYPE, passpointProfileType.count);
                    break;
                default:
                    fail("unknown type counted");
            }
        }
    }

    /**
     * Verify that the LABEL_BAD Wifi usability stats are not saved if screen state is off.
     * @throws Exception
     */
    @Test
    public void verifyLabelBadStatsAreNotSavedIfScreenIsOff() throws Exception {
        mWifiMetrics.setScreenState(false);
        WifiInfo info = mock(WifiInfo.class);
        when(info.getRssi()).thenReturn(nextRandInt());
        when(info.getLinkSpeed()).thenReturn(nextRandInt());
        WifiLinkLayerStats stats1 = nextRandomStats(new WifiLinkLayerStats());
        mWifiMetrics.updateWifiUsabilityStatsEntries(info, stats1);

        // Add 1 LABEL_GOOD
        WifiLinkLayerStats statsGood = addGoodWifiUsabilityStats(nextRandomStats(stats1));
        // IP reachability lost occurs
        mWifiMetrics.addToWifiUsabilityStatsList(WifiUsabilityStats.LABEL_BAD,
                WifiUsabilityStats.TYPE_IP_REACHABILITY_LOST, -1);
        // Wifi data stall occurs
        mWifiMetrics.addToWifiUsabilityStatsList(WifiUsabilityStats.LABEL_BAD,
                WifiIsUnusableEvent.TYPE_DATA_STALL_BAD_TX, -1);
        // Firmware alert occurs
        mWifiMetrics.logFirmwareAlert(2);

        dumpProtoAndDeserialize();
        assertEquals(0, mDecodedProto.wifiUsabilityStatsList.length);
    }
}
