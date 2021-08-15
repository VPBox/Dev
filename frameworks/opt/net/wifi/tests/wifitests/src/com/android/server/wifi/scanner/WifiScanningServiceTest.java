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

import static android.content.pm.PackageManager.PERMISSION_DENIED;

import static com.android.server.wifi.ScanTestUtil.NativeScanSettingsBuilder;
import static com.android.server.wifi.ScanTestUtil.assertNativePnoSettingsEquals;
import static com.android.server.wifi.ScanTestUtil.assertNativeScanSettingsEquals;
import static com.android.server.wifi.ScanTestUtil.assertScanDatasEquals;
import static com.android.server.wifi.ScanTestUtil.assertScanResultsEquals;
import static com.android.server.wifi.ScanTestUtil.channelsToSpec;
import static com.android.server.wifi.ScanTestUtil.computeSingleScanNativeSettings;
import static com.android.server.wifi.ScanTestUtil.createRequest;
import static com.android.server.wifi.ScanTestUtil.createSingleScanNativeSettingsForChannels;
import static com.android.server.wifi.scanner.WifiScanningServiceImpl.WifiSingleScanStateMachine
        .CACHED_SCAN_RESULTS_MAX_AGE_IN_MILLIS;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyBoolean;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.anyString;
import static org.mockito.Mockito.argThat;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.inOrder;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.validateMockitoUsage;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.when;

import android.Manifest;
import android.app.test.MockAnswerUtil.AnswerWithArguments;
import android.app.test.TestAlarmManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiScanner;
import android.os.Binder;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.RemoteException;
import android.os.WorkSource;
import android.os.test.TestLooper;
import android.util.Pair;

import androidx.test.filters.SmallTest;

import com.android.internal.app.IBatteryStats;
import com.android.internal.util.AsyncChannel;
import com.android.internal.util.Protocol;
import com.android.internal.util.test.BidirectionalAsyncChannel;
import com.android.server.wifi.CellularLinkLayerStatsCollector;
import com.android.server.wifi.Clock;
import com.android.server.wifi.DppMetrics;
import com.android.server.wifi.FakeWifiLog;
import com.android.server.wifi.FrameworkFacade;
import com.android.server.wifi.ScanResults;
import com.android.server.wifi.WifiInjector;
import com.android.server.wifi.WifiMetrics;
import com.android.server.wifi.WifiNative;
import com.android.server.wifi.WifiPowerMetrics;
import com.android.server.wifi.aware.WifiAwareMetrics;
import com.android.server.wifi.nano.WifiMetricsProto;
import com.android.server.wifi.p2p.WifiP2pMetrics;
import com.android.server.wifi.rtt.RttMetrics;
import com.android.server.wifi.util.WifiAsyncChannel;
import com.android.server.wifi.util.WifiPermissionsUtil;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.ArgumentMatcher;
import org.mockito.InOrder;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.mockito.Spy;

import java.io.FileDescriptor;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.regex.Pattern;

/**
 * Unit tests for {@link com.android.server.wifi.scanner.WifiScanningServiceImpl}.
 */
@SmallTest
public class WifiScanningServiceTest {
    public static final String TAG = "WifiScanningServiceTest";

    private static final int TEST_MAX_SCAN_BUCKETS_IN_CAPABILITIES = 8;
    private static final String TEST_PACKAGE_NAME = "com.test.123";

    @Mock Context mContext;
    TestAlarmManager mAlarmManager;
    @Mock WifiScannerImpl mWifiScannerImpl;
    @Mock WifiScannerImpl.WifiScannerImplFactory mWifiScannerImplFactory;
    @Mock IBatteryStats mBatteryStats;
    @Mock WifiInjector mWifiInjector;
    @Mock FrameworkFacade mFrameworkFacade;
    @Mock Clock mClock;
    @Spy FakeWifiLog mLog;
    @Mock WifiPermissionsUtil mWifiPermissionsUtil;
    @Mock DppMetrics mDppMetrics;
    WifiMetrics mWifiMetrics;
    TestLooper mLooper;
    WifiScanningServiceImpl mWifiScanningServiceImpl;
    @Mock WifiP2pMetrics mWifiP2pMetrics;
    @Mock CellularLinkLayerStatsCollector mCellularLinkLayerStatsCollector;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);

        mAlarmManager = new TestAlarmManager();
        when(mContext.getSystemService(Context.ALARM_SERVICE))
                .thenReturn(mAlarmManager.getAlarmManager());
        when(mWifiInjector.getWifiPermissionsUtil())
                .thenReturn(mWifiPermissionsUtil);

        ChannelHelper channelHelper = new PresetKnownBandsChannelHelper(
                new int[]{2400, 2450},
                new int[]{5150, 5175},
                new int[]{5600, 5650, 5660});

        mLooper = new TestLooper();
        mWifiMetrics = new WifiMetrics(mContext, mFrameworkFacade, mClock, mLooper.getLooper(),
                new WifiAwareMetrics(mClock), new RttMetrics(mClock), new WifiPowerMetrics(),
                mWifiP2pMetrics, mDppMetrics, mCellularLinkLayerStatsCollector);
        when(mWifiScannerImplFactory
                .create(any(), any(), any()))
                .thenReturn(mWifiScannerImpl);
        when(mWifiScannerImpl.getChannelHelper()).thenReturn(channelHelper);
        when(mWifiInjector.getWifiMetrics()).thenReturn(mWifiMetrics);
        when(mWifiInjector.makeLog(anyString())).thenReturn(mLog);
        WifiAsyncChannel mWifiAsyncChannel = new WifiAsyncChannel("ScanningServiceTest");
        mWifiAsyncChannel.setWifiLog(mLog);
        when(mFrameworkFacade.makeWifiAsyncChannel(anyString())).thenReturn(mWifiAsyncChannel);
        when(mWifiInjector.getFrameworkFacade()).thenReturn(mFrameworkFacade);
        when(mWifiInjector.getClock()).thenReturn(mClock);
        mWifiScanningServiceImpl = new WifiScanningServiceImpl(mContext, mLooper.getLooper(),
                mWifiScannerImplFactory, mBatteryStats, mWifiInjector);
    }

    @After
    public void cleanup() {
        validateMockitoUsage();
    }

    /**
     * Internal BroadcastReceiver that WifiScanningServiceImpl uses to listen for broadcasts
     * this is initialized by calling startServiceAndLoadDriver
     */
    BroadcastReceiver mBroadcastReceiver;

    private WifiScanner.ScanSettings generateValidScanSettings() {
        return createRequest(WifiScanner.WIFI_BAND_BOTH, 30000, 0, 20,
                WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
    }

    private BidirectionalAsyncChannel connectChannel(Handler handler) {
        BidirectionalAsyncChannel controlChannel = new BidirectionalAsyncChannel();
        controlChannel.connect(mLooper.getLooper(), mWifiScanningServiceImpl.getMessenger(),
                handler);
        mLooper.dispatchAll();
        controlChannel.assertConnected();
        return controlChannel;
    }

    private static Message verifyHandleMessageAndGetMessage(InOrder order, Handler handler) {
        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        order.verify(handler).handleMessage(messageCaptor.capture());
        return messageCaptor.getValue();
    }

    private static class ConditionalMessageCaptor implements ArgumentMatcher<Message> {
        private Message mLastValue;
        private final int mWhat;

        private ConditionalMessageCaptor(int what) {
            mWhat = what;
        }

        public Message getLastValue() {
            assertNotNull("Nothing captured yet", mLastValue);

            return mLastValue;
        }

        public boolean matches(Message message) {
            boolean isMatch = message.what == mWhat;

            if (isMatch) {
                mLastValue = message;
            }

            return isMatch;
        }
    }

    private static Message verifyHandleMessageAndGetMessage(InOrder order, Handler handler,
            final int what) {
        ConditionalMessageCaptor messageMatcher = new ConditionalMessageCaptor(what);

        order.verify(handler).handleMessage(argThat(messageMatcher));
        return messageMatcher.getLastValue();
    }

    private static void verifyScanResultsReceived(InOrder order, Handler handler, int listenerId,
            WifiScanner.ScanData... expected) {
        Message scanResultMessage = verifyHandleMessageAndGetMessage(order, handler,
                WifiScanner.CMD_SCAN_RESULT);
        assertScanResultsMessage(listenerId, expected, scanResultMessage);
    }

    private static void assertScanResultsMessage(int listenerId, WifiScanner.ScanData[] expected,
            Message scanResultMessage) {
        assertEquals("what", WifiScanner.CMD_SCAN_RESULT, scanResultMessage.what);
        assertEquals("listenerId", listenerId, scanResultMessage.arg2);
        assertScanDatasEquals(expected,
                ((WifiScanner.ParcelableScanData) scanResultMessage.obj).getResults());
    }

    private static void verifySingleScanCompletedReceived(InOrder order, Handler handler,
            int listenerId) {
        Message completedMessage = verifyHandleMessageAndGetMessage(order, handler,
                WifiScanner.CMD_SINGLE_SCAN_COMPLETED);
        assertSingleScanCompletedMessage(listenerId, completedMessage);
    }

    private static void assertSingleScanCompletedMessage(int listenerId, Message completedMessage) {
        assertEquals("what", WifiScanner.CMD_SINGLE_SCAN_COMPLETED, completedMessage.what);
        assertEquals("listenerId", listenerId, completedMessage.arg2);
    }

    private static void sendBackgroundScanRequest(BidirectionalAsyncChannel controlChannel,
            int scanRequestId, WifiScanner.ScanSettings settings, WorkSource workSource) {
        Bundle scanParams = new Bundle();
        scanParams.putParcelable(WifiScanner.SCAN_PARAMS_SCAN_SETTINGS_KEY, settings);
        scanParams.putParcelable(WifiScanner.SCAN_PARAMS_WORK_SOURCE_KEY, workSource);
        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_START_BACKGROUND_SCAN, 0,
                        scanRequestId, scanParams));
    }

    private static void sendSingleScanRequest(BidirectionalAsyncChannel controlChannel,
            int scanRequestId, WifiScanner.ScanSettings settings, WorkSource workSource) {
        Bundle scanParams = new Bundle();
        scanParams.putParcelable(WifiScanner.SCAN_PARAMS_SCAN_SETTINGS_KEY, settings);
        scanParams.putParcelable(WifiScanner.SCAN_PARAMS_WORK_SOURCE_KEY, workSource);
        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_START_SINGLE_SCAN, 0,
                        scanRequestId, scanParams));
    }

    private static void registerScanListener(BidirectionalAsyncChannel controlChannel,
            int listenerRequestId) {
        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_REGISTER_SCAN_LISTENER, 0,
                        listenerRequestId, null));
    }

    private static void deregisterScanListener(BidirectionalAsyncChannel controlChannel,
            int listenerRequestId) {
        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_DEREGISTER_SCAN_LISTENER, 0,
                        listenerRequestId, null));
    }

    private static void verifySuccessfulResponse(InOrder order, Handler handler, int arg2) {
        Message response = verifyHandleMessageAndGetMessage(order, handler);
        assertSuccessfulResponse(arg2, response);
    }

    private static void assertSuccessfulResponse(int arg2, Message response) {
        if (response.what == WifiScanner.CMD_OP_FAILED) {
            WifiScanner.OperationResult result = (WifiScanner.OperationResult) response.obj;
            fail("response indicates failure, reason=" + result.reason
                    + ", description=" + result.description);
        } else {
            assertEquals("response.what", WifiScanner.CMD_OP_SUCCEEDED, response.what);
            assertEquals("response.arg2", arg2, response.arg2);
        }
    }

    /**
     * If multiple results are expected for a single hardware scan then the order that they are
     * dispatched is dependant on the order which they are iterated through internally. This
     * function validates that the order is either one way or the other. A scan listener can
     * optionally be provided as well and will be checked after the after the single scan requests.
     */
    private static void verifyMultipleSingleScanResults(InOrder handlerOrder, Handler handler,
            int requestId1, ScanResults results1, int requestId2, ScanResults results2,
            int listenerRequestId, ScanResults listenerResults) {
        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        handlerOrder.verify(handler, times(listenerResults == null ? 4 : 5))
                .handleMessage(messageCaptor.capture());
        int firstListenerId = messageCaptor.getAllValues().get(0).arg2;
        assertTrue(firstListenerId + " was neither " + requestId2 + " nor " + requestId1,
                firstListenerId == requestId2 || firstListenerId == requestId1);
        if (firstListenerId == requestId2) {
            assertScanResultsMessage(requestId2,
                    new WifiScanner.ScanData[] {results2.getScanData()},
                    messageCaptor.getAllValues().get(0));
            assertSingleScanCompletedMessage(requestId2, messageCaptor.getAllValues().get(1));
            assertScanResultsMessage(requestId1,
                    new WifiScanner.ScanData[] {results1.getScanData()},
                    messageCaptor.getAllValues().get(2));
            assertSingleScanCompletedMessage(requestId1, messageCaptor.getAllValues().get(3));
            if (listenerResults != null) {
                assertScanResultsMessage(listenerRequestId,
                        new WifiScanner.ScanData[] {listenerResults.getScanData()},
                        messageCaptor.getAllValues().get(4));
            }
        } else {
            assertScanResultsMessage(requestId1,
                    new WifiScanner.ScanData[] {results1.getScanData()},
                    messageCaptor.getAllValues().get(0));
            assertSingleScanCompletedMessage(requestId1, messageCaptor.getAllValues().get(1));
            assertScanResultsMessage(requestId2,
                    new WifiScanner.ScanData[] {results2.getScanData()},
                    messageCaptor.getAllValues().get(2));
            assertSingleScanCompletedMessage(requestId2, messageCaptor.getAllValues().get(3));
            if (listenerResults != null) {
                assertScanResultsMessage(listenerRequestId,
                        new WifiScanner.ScanData[] {listenerResults.getScanData()},
                        messageCaptor.getAllValues().get(4));
            }
        }
    }

    private static void verifyMultipleSingleScanResults(InOrder handlerOrder, Handler handler,
            int requestId1, ScanResults results1, int requestId2, ScanResults results2) {
        verifyMultipleSingleScanResults(handlerOrder, handler, requestId1, results1, requestId2,
                results2, -1, null);
    }

    private static void verifyFailedResponse(InOrder order, Handler handler, int arg2,
            int expectedErrorReason, String expectedErrorDescription) {
        Message response = verifyHandleMessageAndGetMessage(order, handler);
        assertFailedResponse(arg2, expectedErrorReason, expectedErrorDescription, response);
    }

    private static void assertFailedResponse(int arg2, int expectedErrorReason,
            String expectedErrorDescription, Message response) {
        if (response.what == WifiScanner.CMD_OP_SUCCEEDED) {
            fail("response indicates success");
        } else {
            assertEquals("response.what", WifiScanner.CMD_OP_FAILED, response.what);
            assertEquals("response.arg2", arg2, response.arg2);
            WifiScanner.OperationResult result = (WifiScanner.OperationResult) response.obj;
            assertEquals("response.obj.reason",
                    expectedErrorReason, result.reason);
            assertEquals("response.obj.description",
                    expectedErrorDescription, result.description);
        }
    }

    private WifiNative.ScanEventHandler verifyStartSingleScan(InOrder order,
            WifiNative.ScanSettings expected) {
        ArgumentCaptor<WifiNative.ScanSettings> scanSettingsCaptor =
                ArgumentCaptor.forClass(WifiNative.ScanSettings.class);
        ArgumentCaptor<WifiNative.ScanEventHandler> scanEventHandlerCaptor =
                ArgumentCaptor.forClass(WifiNative.ScanEventHandler.class);
        order.verify(mWifiScannerImpl).startSingleScan(scanSettingsCaptor.capture(),
                scanEventHandlerCaptor.capture());
        assertNativeScanSettingsEquals(expected, scanSettingsCaptor.getValue());
        return scanEventHandlerCaptor.getValue();
    }

    private WifiNative.ScanEventHandler verifyStartBackgroundScan(InOrder order,
            WifiNative.ScanSettings expected) {
        ArgumentCaptor<WifiNative.ScanSettings> scanSettingsCaptor =
                ArgumentCaptor.forClass(WifiNative.ScanSettings.class);
        ArgumentCaptor<WifiNative.ScanEventHandler> scanEventHandlerCaptor =
                ArgumentCaptor.forClass(WifiNative.ScanEventHandler.class);
        order.verify(mWifiScannerImpl).startBatchedScan(scanSettingsCaptor.capture(),
                scanEventHandlerCaptor.capture());
        assertNativeScanSettingsEquals(expected, scanSettingsCaptor.getValue());
        return scanEventHandlerCaptor.getValue();
    }

    private static final int MAX_AP_PER_SCAN = 16;
    private void startServiceAndLoadDriver() {
        mWifiScanningServiceImpl.startService();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);
        setupAndLoadDriver(TEST_MAX_SCAN_BUCKETS_IN_CAPABILITIES);
    }

    private void setupAndLoadDriver(int max_scan_buckets) {
        when(mWifiScannerImpl.getScanCapabilities(any(WifiNative.ScanCapabilities.class)))
                .thenAnswer(new AnswerWithArguments() {
                        public boolean answer(WifiNative.ScanCapabilities capabilities) {
                            capabilities.max_scan_cache_size = Integer.MAX_VALUE;
                            capabilities.max_scan_buckets = max_scan_buckets;
                            capabilities.max_ap_cache_per_scan = MAX_AP_PER_SCAN;
                            capabilities.max_rssi_sample_size = 8;
                            capabilities.max_scan_reporting_threshold = 10;
                            return true;
                        }
                    });
        BidirectionalAsyncChannel controlChannel = connectChannel(mock(Handler.class));
        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_ENABLE));
        mLooper.dispatchAll();
    }

    private String dumpService() {
        StringWriter stringWriter = new StringWriter();
        mWifiScanningServiceImpl.dump(new FileDescriptor(), new PrintWriter(stringWriter),
                new String[0]);
        return stringWriter.toString();
    }

    private void assertDumpContainsRequestLog(String type, int id) {
        String serviceDump = dumpService();
        Pattern logLineRegex = Pattern.compile("^.+" + type
                + ": ClientInfo\\[uid=\\d+,android\\.os\\.Messenger@[a-f0-9]+\\],Id=" + id
                + ".*$", Pattern.MULTILINE);
        assertTrue("dump did not contain log with type=" + type + ", id=" + id +
                ": " + serviceDump + "\n",
                logLineRegex.matcher(serviceDump).find());
    }

    private void assertDumpContainsCallbackLog(String callback, int id, String extra) {
        String serviceDump = dumpService();
        String extraPattern = extra == null ? "" : "," + extra;
        Pattern logLineRegex = Pattern.compile("^.+" + callback
                + ": ClientInfo\\[uid=\\d+,android\\.os\\.Messenger@[a-f0-9]+\\],Id=" + id
                + extraPattern + "$", Pattern.MULTILINE);
        assertTrue("dump did not contain callback log with callback=" + callback + ", id=" + id +
                ", extra=" + extra + ": " + serviceDump + "\n",
                logLineRegex.matcher(serviceDump).find());
    }

    @Test
    public void construct() throws Exception {
        verifyNoMoreInteractions(mWifiScannerImpl, mWifiScannerImpl,
                mWifiScannerImplFactory, mBatteryStats);
        dumpService(); // make sure this succeeds
    }

    @Test
    public void startService() throws Exception {
        mWifiScanningServiceImpl.startService();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);
        verifyNoMoreInteractions(mWifiScannerImplFactory);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler);
        sendBackgroundScanRequest(controlChannel, 122, generateValidScanSettings(), null);
        mLooper.dispatchAll();
        verifyFailedResponse(order, handler, 122, WifiScanner.REASON_UNSPECIFIED, "not available");
    }

    @Test
    public void disconnectClientBeforeWifiEnabled() throws Exception {
        mWifiScanningServiceImpl.startService();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);
        BidirectionalAsyncChannel controlChannel = connectChannel(mock(Handler.class));
        mLooper.dispatchAll();

        controlChannel.disconnect();
        mLooper.dispatchAll();
    }

    @Test
    public void loadDriver() throws Exception {
        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);
        verify(mWifiScannerImplFactory, times(1))
                .create(any(), any(), any());

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler);
        when(mWifiScannerImpl.startBatchedScan(any(WifiNative.ScanSettings.class),
                        any(WifiNative.ScanEventHandler.class))).thenReturn(true);
        sendBackgroundScanRequest(controlChannel, 192, generateValidScanSettings(), null);
        mLooper.dispatchAll();
        verifySuccessfulResponse(order, handler, 192);
        assertDumpContainsRequestLog("addBackgroundScanRequest", 192);
    }

    /**
     * Verifies that duplicate scan enable is ignored.
     */
    @Test
    public void duplicateScanEnableIsIgnored() throws RemoteException {
        startServiceAndLoadDriver();

        // Send scan enable again.
        BidirectionalAsyncChannel controlChannel = connectChannel(mock(Handler.class));
        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_ENABLE));
        mLooper.dispatchAll();

        // Ensure we didn't create scanner instance twice.
        verify(mWifiScannerImplFactory, times(1))
                .create(any(), any(), any());
    }

    @Test
    public void disconnectClientAfterStartingWifi() throws Exception {
        mWifiScanningServiceImpl.startService();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);
        BidirectionalAsyncChannel controlChannel = connectChannel(mock(Handler.class));
        mLooper.dispatchAll();

        setupAndLoadDriver(TEST_MAX_SCAN_BUCKETS_IN_CAPABILITIES);

        controlChannel.disconnect();
        mLooper.dispatchAll();
    }

    @Test
    public void connectAndDisconnectClientAfterStartingWifi() throws Exception {
        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        BidirectionalAsyncChannel controlChannel = connectChannel(mock(Handler.class));
        mLooper.dispatchAll();
        controlChannel.disconnect();
        mLooper.dispatchAll();
    }

    @Test
    public void sendInvalidCommand() throws Exception {
        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);
        controlChannel.sendMessage(Message.obtain(null, Protocol.BASE_WIFI_MANAGER));
        mLooper.dispatchAll();
        verifyFailedResponse(order, handler, 0, WifiScanner.REASON_INVALID_REQUEST,
                "Invalid request");
    }

    @Test
    public void rejectBackgroundScanRequestWhenHalReturnsInvalidCapabilities() throws Exception {
        mWifiScanningServiceImpl.startService();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        setupAndLoadDriver(0);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler);
        sendBackgroundScanRequest(controlChannel, 122, generateValidScanSettings(), null);
        mLooper.dispatchAll();
        verifyFailedResponse(order, handler, 122, WifiScanner.REASON_UNSPECIFIED, "not available");
    }

    @Test
    public void rejectBackgroundScanRequestWhenScannerImplCreateFails() throws Exception {
        // Fail scanner impl creation.
        when(mWifiScannerImplFactory.create(any(), any(), any())).thenReturn(null);

        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler);
        sendBackgroundScanRequest(controlChannel, 122, generateValidScanSettings(), null);
        mLooper.dispatchAll();
        verifyFailedResponse(order, handler, 122, WifiScanner.REASON_UNSPECIFIED, "not available");
    }

    private void doSuccessfulSingleScan(WifiScanner.ScanSettings requestSettings,
            WifiNative.ScanSettings nativeSettings, ScanResults results) throws RemoteException {
        int requestId = 12;
        WorkSource workSource = new WorkSource(2292);
        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                        any(WifiNative.ScanEventHandler.class))).thenReturn(true);

        sendSingleScanRequest(controlChannel, requestId, requestSettings, workSource);

        mLooper.dispatchAll();
        WifiNative.ScanEventHandler eventHandler = verifyStartSingleScan(order, nativeSettings);
        verifySuccessfulResponse(order, handler, requestId);
        verify(mBatteryStats).noteWifiScanStartedFromSource(eq(workSource));

        when(mWifiScannerImpl.getLatestSingleScanResults())
                .thenReturn(results.getRawScanData());
        eventHandler.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);

        mLooper.dispatchAll();
        verifyScanResultsReceived(order, handler, requestId, results.getScanData());
        verifySingleScanCompletedReceived(order, handler, requestId);
        verifyNoMoreInteractions(handler);
        verify(mBatteryStats).noteWifiScanStoppedFromSource(eq(workSource));
        assertDumpContainsRequestLog("addSingleScanRequest", requestId);
        assertDumpContainsCallbackLog("singleScanResults", requestId,
                "results=" + results.getScanData().getResults().length);
    }

    /**
     * Do a single scan for a band and verify that it is successful.
     */
    @Test
    public void sendSingleScanBandRequest() throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(WifiScanner.WIFI_BAND_BOTH_WITH_DFS,
                0, 0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        doSuccessfulSingleScan(requestSettings, computeSingleScanNativeSettings(requestSettings),
                ScanResults.create(0, WifiScanner.WIFI_BAND_BOTH_WITH_DFS, 2400, 5150, 5175));
    }

    /**
     * Do a single scan for a list of channels and verify that it is successful.
     */
    @Test
    public void sendSingleScanChannelsRequest() throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(channelsToSpec(2400, 5150, 5175),
                0, 0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        doSuccessfulSingleScan(requestSettings, computeSingleScanNativeSettings(requestSettings),
                ScanResults.create(0, WifiScanner.WIFI_BAND_UNSPECIFIED, 2400, 5150, 5175));
    }

    /**
     * Do a single scan for a list of all channels and verify that it is successful.
     */
    @Test
    public void sendSingleScanAllChannelsRequest() throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(
                channelsToSpec(2400, 2450, 5150, 5175, 5600, 5650, 5660),
                0, 0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        doSuccessfulSingleScan(requestSettings, computeSingleScanNativeSettings(requestSettings),
                ScanResults.create(0, WifiScanner.WIFI_BAND_UNSPECIFIED, 2400, 5150, 5175));
    }

    /**
     * Do a single scan with no results and verify that it is successful.
     */
    @Test
    public void sendSingleScanRequestWithNoResults() throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(WifiScanner.WIFI_BAND_BOTH, 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        doSuccessfulSingleScan(requestSettings, computeSingleScanNativeSettings(requestSettings),
                ScanResults.create(0, WifiScanner.WIFI_BAND_BOTH, new int[0]));
    }

    /**
     * Do a single scan with results that do not match the requested scan and verify that it is
     * still successful (and returns no results).
     */
    @Test
    public void sendSingleScanRequestWithBadRawResults() throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(WifiScanner.WIFI_BAND_24_GHZ, 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        // Create a set of scan results that has results not matching the request settings, but is
        // limited to zero results for the expected results.
        ScanResults results = ScanResults.createOverflowing(0, WifiScanner.WIFI_BAND_24_GHZ, 0,
                ScanResults.generateNativeResults(0, 5150, 5171));
        doSuccessfulSingleScan(requestSettings, computeSingleScanNativeSettings(requestSettings),
                results);
    }

    /**
     * Do a single scan from a non-privileged app with some privileged params set.
     * Expect a scan failure.
     */
    @Test
    public void sendSingleScanRequestWithPrivilegedTypeParamsSetFromNonPrivilegedApp()
            throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(WifiScanner.WIFI_BAND_BOTH, 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId = 33;
        requestSettings.type = WifiScanner.TYPE_HIGH_ACCURACY;
        WorkSource workSource = new WorkSource(Binder.getCallingUid()); // don't explicitly set

        when(mContext.checkPermission(
                Manifest.permission.NETWORK_STACK, -1, Binder.getCallingUid()))
                .thenReturn(PERMISSION_DENIED);

        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        // successful start
        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                any(WifiNative.ScanEventHandler.class))).thenReturn(true);

        sendSingleScanRequest(controlChannel, requestId, requestSettings, null);

        // Scan is successfully queued
        mLooper.dispatchAll();

        // but then fails to execute
        verifyFailedResponse(order, handler, requestId,
                WifiScanner.REASON_INVALID_REQUEST, "bad request");
        assertDumpContainsCallbackLog("singleScanInvalidRequest", requestId,
                "bad request");

        assertEquals(0, mWifiMetrics.getOneshotScanCount());
        assertEquals(mWifiMetrics.getScanReturnEntry(
                WifiMetricsProto.WifiLog.SCAN_FAILURE_INVALID_CONFIGURATION), 1);

        // Ensure that no scan was triggered to the lower layers.
        verify(mBatteryStats, never()).noteWifiScanStoppedFromSource(eq(workSource));
        verify(mWifiScannerImpl, never()).startSingleScan(any(WifiNative.ScanSettings.class),
                any(WifiNative.ScanEventHandler.class));
    }

    /**
     * Do a single scan from a non-privileged app with some privileged params set.
     * Expect a scan failure.
     */
    @Test
    public void sendSingleScanRequestWithPrivilegedHiddenNetworkParamsSetFromNonPrivilegedApp()
            throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(WifiScanner.WIFI_BAND_BOTH, 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId = 33;
        requestSettings.hiddenNetworks = new WifiScanner.ScanSettings.HiddenNetwork[] {
                new WifiScanner.ScanSettings.HiddenNetwork("Test1"),
                new WifiScanner.ScanSettings.HiddenNetwork("Test2")
        };
        WorkSource workSource = new WorkSource(Binder.getCallingUid()); // don't explicitly set

        when(mContext.checkPermission(
                Manifest.permission.NETWORK_STACK, -1, Binder.getCallingUid()))
                .thenReturn(PERMISSION_DENIED);

        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        // successful start
        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                any(WifiNative.ScanEventHandler.class))).thenReturn(true);

        sendSingleScanRequest(controlChannel, requestId, requestSettings, null);

        // Scan is successfully queued
        mLooper.dispatchAll();

        // but then fails to execute
        verifyFailedResponse(order, handler, requestId,
                WifiScanner.REASON_INVALID_REQUEST, "bad request");
        assertDumpContainsCallbackLog("singleScanInvalidRequest", requestId,
                "bad request");

        assertEquals(0, mWifiMetrics.getOneshotScanCount());
        assertEquals(mWifiMetrics.getScanReturnEntry(
                WifiMetricsProto.WifiLog.SCAN_FAILURE_INVALID_CONFIGURATION), 1);

        // Ensure that no scan was triggered to the lower layers.
        verify(mBatteryStats, never()).noteWifiScanStoppedFromSource(eq(workSource));
        verify(mWifiScannerImpl, never()).startSingleScan(any(WifiNative.ScanSettings.class),
                any(WifiNative.ScanEventHandler.class));
    }

    /**
     * Do a single scan with invalid scan type set.
     * Expect a scan failure.
     */
    @Test
    public void sendSingleScanRequestWithInvalidScanType()
            throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(WifiScanner.WIFI_BAND_BOTH, 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId = 33;
        requestSettings.type = 100; // invalid  scan type
        WorkSource workSource = new WorkSource(Binder.getCallingUid()); // don't explicitly set

        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        // successful start
        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                any(WifiNative.ScanEventHandler.class))).thenReturn(true);

        sendSingleScanRequest(controlChannel, requestId, requestSettings, null);

        // Scan is successfully queued
        mLooper.dispatchAll();

        // but then fails to execute
        verifyFailedResponse(order, handler, requestId,
                WifiScanner.REASON_INVALID_REQUEST, "bad request");
        assertDumpContainsCallbackLog("singleScanInvalidRequest", requestId,
                "bad request");

        assertEquals(0, mWifiMetrics.getOneshotScanCount());
        assertEquals(mWifiMetrics.getScanReturnEntry(
                WifiMetricsProto.WifiLog.SCAN_FAILURE_INVALID_CONFIGURATION), 1);

        // Ensure that no scan was triggered to the lower layers.
        verify(mBatteryStats, never()).noteWifiScanStoppedFromSource(eq(workSource));
        verify(mWifiScannerImpl, never()).startSingleScan(any(WifiNative.ScanSettings.class),
                any(WifiNative.ScanEventHandler.class));
    }

    /**
     * Do a single scan from a non-privileged app with no privileged params set.
     */
    @Test
    public void sendSingleScanRequestWithNoPrivilegedParamsSetFromNonPrivilegedApp()
            throws Exception {
        when(mContext.checkPermission(
                Manifest.permission.NETWORK_STACK, -1, Binder.getCallingUid()))
                .thenReturn(PERMISSION_DENIED);
        WifiScanner.ScanSettings requestSettings = createRequest(channelsToSpec(2400, 5150, 5175),
                0, 0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        doSuccessfulSingleScan(requestSettings, computeSingleScanNativeSettings(requestSettings),
                ScanResults.create(0, 2400, 5150, 5175));
    }

    /**
     * Do a single scan, which the hardware fails to start, and verify that a failure response is
     * delivered.
     */
    @Test
    public void sendSingleScanRequestWhichFailsToStart() throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(WifiScanner.WIFI_BAND_BOTH, 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId = 33;

        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        // scan fails
        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                        any(WifiNative.ScanEventHandler.class))).thenReturn(false);

        sendSingleScanRequest(controlChannel, requestId, requestSettings, null);

        mLooper.dispatchAll();
        // Scan is successfully queue, but then fails to execute
        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        order.verify(handler, times(2)).handleMessage(messageCaptor.capture());
        assertSuccessfulResponse(requestId, messageCaptor.getAllValues().get(0));
        assertFailedResponse(requestId, WifiScanner.REASON_UNSPECIFIED,
                "Failed to start single scan", messageCaptor.getAllValues().get(1));
        verifyNoMoreInteractions(mBatteryStats);

        assertEquals(mWifiMetrics.getOneshotScanCount(), 1);
        assertEquals(mWifiMetrics.getScanReturnEntry(WifiMetricsProto.WifiLog.SCAN_UNKNOWN), 1);
        assertDumpContainsRequestLog("addSingleScanRequest", requestId);
    }

    /**
     * Do a single scan, which successfully starts, but fails partway through and verify that a
     * failure response is delivered.
     */
    @Test
    public void sendSingleScanRequestWhichFailsAfterStart() throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(WifiScanner.WIFI_BAND_BOTH, 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId = 33;
        WorkSource workSource = new WorkSource(Binder.getCallingUid()); // don't explicitly set

        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        // successful start
        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                        any(WifiNative.ScanEventHandler.class))).thenReturn(true);

        sendSingleScanRequest(controlChannel, requestId, requestSettings, null);

        // Scan is successfully queue
        mLooper.dispatchAll();
        WifiNative.ScanEventHandler eventHandler =
                verifyStartSingleScan(order, computeSingleScanNativeSettings(requestSettings));
        verifySuccessfulResponse(order, handler, requestId);
        verify(mBatteryStats).noteWifiScanStartedFromSource(eq(workSource));

        // but then fails to execute
        eventHandler.onScanStatus(WifiNative.WIFI_SCAN_FAILED);
        mLooper.dispatchAll();
        verifyFailedResponse(order, handler, requestId,
                WifiScanner.REASON_UNSPECIFIED, "Scan failed");
        assertDumpContainsCallbackLog("singleScanFailed", requestId,
                "reason=" + WifiScanner.REASON_UNSPECIFIED + ", Scan failed");
        assertEquals(mWifiMetrics.getOneshotScanCount(), 1);
        assertEquals(mWifiMetrics.getScanReturnEntry(WifiMetricsProto.WifiLog.SCAN_UNKNOWN), 1);
        verify(mBatteryStats).noteWifiScanStoppedFromSource(eq(workSource));
    }

    /**
     * Do a single scan that includes DFS channels and verify that both oneshot scan count and
     * oneshot scan count with dfs are incremented.
     */
    @Test
    public void testMetricsForOneshotScanWithDFSIsIncremented() throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(
                WifiScanner.WIFI_BAND_BOTH_WITH_DFS, 0, 0, 20,
                WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId = 33;
        WorkSource workSource = new WorkSource(Binder.getCallingUid()); // don't explicitly set

        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        // successful start
        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                any(WifiNative.ScanEventHandler.class))).thenReturn(true);

        sendSingleScanRequest(controlChannel, requestId, requestSettings, null);

        assertEquals(0, mWifiMetrics.getOneshotScanCount());
        assertEquals(0, mWifiMetrics.getOneshotScanWithDfsCount());
        // Scan is successfully queue
        mLooper.dispatchAll();
        assertEquals(1, mWifiMetrics.getOneshotScanCount());
        assertEquals(1, mWifiMetrics.getOneshotScanWithDfsCount());
    }

    /**
     * Do a single scan that excludes DFS channels and verify that only oneshot scan count is
     * incremented.
     */
    @Test
    public void testMetricsForOneshotScanWithDFSIsNotIncremented() throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(
                WifiScanner.WIFI_BAND_5_GHZ, 0, 0, 20,
                WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId = 33;
        WorkSource workSource = new WorkSource(Binder.getCallingUid()); // don't explicitly set

        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        // successful start
        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                any(WifiNative.ScanEventHandler.class))).thenReturn(true);

        sendSingleScanRequest(controlChannel, requestId, requestSettings, null);

        assertEquals(0, mWifiMetrics.getOneshotScanCount());
        // Scan is successfully queue
        mLooper.dispatchAll();
        assertEquals(1, mWifiMetrics.getOneshotScanCount());
        assertEquals(0, mWifiMetrics.getOneshotScanWithDfsCount());
    }

    /**
     * Send a single scan request and then disable Wi-Fi before it completes
     */
    @Test
    public void sendSingleScanRequestThenDisableWifi() {
        WifiScanner.ScanSettings requestSettings = createRequest(channelsToSpec(2400), 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId = 2293;

        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                        any(WifiNative.ScanEventHandler.class))).thenReturn(true);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        // Run scan 1
        sendSingleScanRequest(controlChannel, requestId, requestSettings, null);
        mLooper.dispatchAll();
        verifySuccessfulResponse(order, handler, requestId);

        // disable wifi
        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_DISABLE));

        // validate failed response
        mLooper.dispatchAll();
        verifyFailedResponse(order, handler, requestId, WifiScanner.REASON_UNSPECIFIED,
                "Scan was interrupted");
        verifyNoMoreInteractions(handler);
    }

    /**
     * Send a single scan request, schedule a second pending scan and disable Wi-Fi before the first
     * scan completes.
     */
    @Test
    public void sendSingleScanAndPendingScanAndListenerThenDisableWifi() {
        WifiScanner.ScanSettings requestSettings1 = createRequest(channelsToSpec(2400), 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId1 = 2293;

        WifiScanner.ScanSettings requestSettings2 = createRequest(channelsToSpec(2450), 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId2 = 2294;

        int listenerRequestId = 2295;

        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                        any(WifiNative.ScanEventHandler.class))).thenReturn(true);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        // Request scan 1
        sendSingleScanRequest(controlChannel, requestId1, requestSettings1, null);
        mLooper.dispatchAll();
        verifySuccessfulResponse(order, handler, requestId1);

        // Request scan 2
        sendSingleScanRequest(controlChannel, requestId2, requestSettings2, null);
        mLooper.dispatchAll();
        verifySuccessfulResponse(order, handler, requestId2);

        // Setup scan listener
        registerScanListener(controlChannel, listenerRequestId);
        mLooper.dispatchAll();
        verifySuccessfulResponse(order, handler, listenerRequestId);

        // disable wifi
        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_DISABLE));

        // validate failed response
        mLooper.dispatchAll();
        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        order.verify(handler, times(2)).handleMessage(messageCaptor.capture());
        assertFailedResponse(requestId1, WifiScanner.REASON_UNSPECIFIED,
                "Scan was interrupted", messageCaptor.getAllValues().get(0));
        assertFailedResponse(requestId2, WifiScanner.REASON_UNSPECIFIED,
                "Scan was interrupted", messageCaptor.getAllValues().get(1));
        // No additional callbacks for scan listener
        verifyNoMoreInteractions(handler);
    }

    /**
     * Send a single scan request and then a second one after the first completes.
     */
    @Test
    public void sendSingleScanRequestAfterPreviousCompletes() {
        WifiScanner.ScanSettings requestSettings1 = createRequest(channelsToSpec(2400), 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId1 = 12;
        ScanResults results1 = ScanResults.create(0, WifiScanner.WIFI_BAND_UNSPECIFIED, 2400);


        WifiScanner.ScanSettings requestSettings2 = createRequest(channelsToSpec(2450, 5175), 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId2 = 13;
        ScanResults results2 = ScanResults.create(0, WifiScanner.WIFI_BAND_UNSPECIFIED, 2450);


        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                        any(WifiNative.ScanEventHandler.class))).thenReturn(true);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl, mContext);

        // Run scan 1
        sendSingleScanRequest(controlChannel, requestId1, requestSettings1, null);

        mLooper.dispatchAll();
        WifiNative.ScanEventHandler eventHandler1 = verifyStartSingleScan(order,
                computeSingleScanNativeSettings(requestSettings1));
        verifySuccessfulResponse(order, handler, requestId1);

        // dispatch scan 1 results
        when(mWifiScannerImpl.getLatestSingleScanResults())
                .thenReturn(results1.getScanData());
        eventHandler1.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);

        mLooper.dispatchAll();
        // Note: The order of the following verification calls looks out of order if you compare to
        // the source code of WifiScanningServiceImpl WifiSingleScanStateMachine.reportScanResults.
        // This is due to the fact that verifyScanResultsReceived and
        // verifySingleScanCompletedReceived require an additional call to handle the message that
        // is created in reportScanResults.  This handling is done in the two verify*Received calls
        // that is run AFTER the reportScanResults method in WifiScanningServiceImpl completes.
        verifyScanResultsReceived(order, handler, requestId1, results1.getScanData());
        verifySingleScanCompletedReceived(order, handler, requestId1);

        // Run scan 2
        sendSingleScanRequest(controlChannel, requestId2, requestSettings2, null);

        mLooper.dispatchAll();
        WifiNative.ScanEventHandler eventHandler2 = verifyStartSingleScan(order,
                computeSingleScanNativeSettings(requestSettings2));
        verifySuccessfulResponse(order, handler, requestId2);

        // dispatch scan 2 results
        when(mWifiScannerImpl.getLatestSingleScanResults())
                .thenReturn(results2.getScanData());
        eventHandler2.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);

        mLooper.dispatchAll();
        verifyScanResultsReceived(order, handler, requestId2, results2.getScanData());
        verifySingleScanCompletedReceived(order, handler, requestId2);
    }

    /**
     * Send a single scan request and then a second one not satisfied by the first before the first
     * completes. Verify that both are scheduled and succeed.
     */
    @Test
    public void sendSingleScanRequestWhilePreviousScanRunning() {
        WifiScanner.ScanSettings requestSettings1 = createRequest(channelsToSpec(2400), 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId1 = 12;
        ScanResults results1 = ScanResults.create(0, WifiScanner.WIFI_BAND_UNSPECIFIED, 2400);

        WifiScanner.ScanSettings requestSettings2 = createRequest(channelsToSpec(2450, 5175), 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId2 = 13;
        ScanResults results2 = ScanResults.create(0, WifiScanner.WIFI_BAND_UNSPECIFIED, 2450);


        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                        any(WifiNative.ScanEventHandler.class))).thenReturn(true);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder handlerOrder = inOrder(handler);
        InOrder nativeOrder = inOrder(mWifiScannerImpl);

        // Run scan 1
        sendSingleScanRequest(controlChannel, requestId1, requestSettings1, null);

        mLooper.dispatchAll();
        WifiNative.ScanEventHandler eventHandler1 = verifyStartSingleScan(nativeOrder,
                computeSingleScanNativeSettings(requestSettings1));
        verifySuccessfulResponse(handlerOrder, handler, requestId1);

        // Queue scan 2 (will not run because previous is in progress)
        sendSingleScanRequest(controlChannel, requestId2, requestSettings2, null);
        mLooper.dispatchAll();
        verifySuccessfulResponse(handlerOrder, handler, requestId2);

        // dispatch scan 1 results
        when(mWifiScannerImpl.getLatestSingleScanResults())
                .thenReturn(results1.getScanData());
        eventHandler1.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);

        mLooper.dispatchAll();
        verifyScanResultsReceived(handlerOrder, handler, requestId1, results1.getScanData());
        verifySingleScanCompletedReceived(handlerOrder, handler, requestId1);

        // now that the first scan completed we expect the second one to start
        WifiNative.ScanEventHandler eventHandler2 = verifyStartSingleScan(nativeOrder,
                computeSingleScanNativeSettings(requestSettings2));

        // dispatch scan 2 results
        when(mWifiScannerImpl.getLatestSingleScanResults())
                .thenReturn(results2.getScanData());
        eventHandler2.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);

        mLooper.dispatchAll();
        verifyScanResultsReceived(handlerOrder, handler, requestId2, results2.getScanData());
        verifySingleScanCompletedReceived(handlerOrder, handler, requestId2);
        assertEquals(mWifiMetrics.getOneshotScanCount(), 2);
        assertEquals(mWifiMetrics.getScanReturnEntry(WifiMetricsProto.WifiLog.SCAN_SUCCESS), 2);
    }

    /**
     * Send a single scan request and then a second one not satisfied by the first before the first
     * completes. Verify that both are scheduled and succeed.
     * Validates that a high accuracy scan request is not satisfied by an ongoing low latency scan,
     * while any other low latency/low power scan request is satisfied.
     */
    @Test
    public void sendSingleScanRequestWhilePreviousScanRunningWithTypesThatDoesNotSatisfy() {
        // Create identitical scan requests other than the types being different.
        WifiScanner.ScanSettings requestSettings1 = createRequest(channelsToSpec(2400), 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        requestSettings1.type = WifiScanner.TYPE_LOW_LATENCY;
        int requestId1 = 12;
        ScanResults results1 = ScanResults.create(0, WifiScanner.WIFI_BAND_UNSPECIFIED, 2400);

        WifiScanner.ScanSettings requestSettings2 = createRequest(channelsToSpec(2400), 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        requestSettings2.type = WifiScanner.TYPE_HIGH_ACCURACY;
        int requestId2 = 13;
        ScanResults results2 = ScanResults.create(0, WifiScanner.WIFI_BAND_UNSPECIFIED, 2400);

        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                        any(WifiNative.ScanEventHandler.class))).thenReturn(true);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder handlerOrder = inOrder(handler);
        InOrder nativeOrder = inOrder(mWifiScannerImpl);

        // Run scan 1
        sendSingleScanRequest(controlChannel, requestId1, requestSettings1, null);

        mLooper.dispatchAll();
        WifiNative.ScanEventHandler eventHandler1 = verifyStartSingleScan(nativeOrder,
                computeSingleScanNativeSettings(requestSettings1));
        verifySuccessfulResponse(handlerOrder, handler, requestId1);

        // Queue scan 2 (will not run because previous is in progress)
        sendSingleScanRequest(controlChannel, requestId2, requestSettings2, null);
        mLooper.dispatchAll();
        verifySuccessfulResponse(handlerOrder, handler, requestId2);

        // dispatch scan 1 results
        when(mWifiScannerImpl.getLatestSingleScanResults())
                .thenReturn(results1.getScanData());
        eventHandler1.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);

        mLooper.dispatchAll();
        verifyScanResultsReceived(handlerOrder, handler, requestId1, results1.getScanData());
        verifySingleScanCompletedReceived(handlerOrder, handler, requestId1);

        // now that the first scan completed we expect the second one to start
        WifiNative.ScanEventHandler eventHandler2 = verifyStartSingleScan(nativeOrder,
                computeSingleScanNativeSettings(requestSettings2));

        // dispatch scan 2 results
        when(mWifiScannerImpl.getLatestSingleScanResults())
                .thenReturn(results2.getScanData());
        eventHandler2.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);

        mLooper.dispatchAll();
        verifyScanResultsReceived(handlerOrder, handler, requestId2, results2.getScanData());
        verifySingleScanCompletedReceived(handlerOrder, handler, requestId2);
        assertEquals(mWifiMetrics.getOneshotScanCount(), 2);
        assertEquals(mWifiMetrics.getScanReturnEntry(WifiMetricsProto.WifiLog.SCAN_SUCCESS), 2);
    }


    /**
     * Send a single scan request and then two more before the first completes. Neither are
     * satisfied by the first scan. Verify that the first completes and the second two are merged.
     * Validates that a high accuracy scan is always preferred over the other types while merging.
     */
    @Test
    public void sendMultipleSingleScanRequestWhilePreviousScanRunning() throws RemoteException {
        WifiScanner.ScanSettings requestSettings1 = createRequest(channelsToSpec(2400), 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        requestSettings1.type = WifiScanner.TYPE_LOW_LATENCY;
        int requestId1 = 12;
        WorkSource workSource1 = new WorkSource(1121);
        ScanResults results1 = ScanResults.create(0, WifiScanner.WIFI_BAND_UNSPECIFIED, 2400);

        WifiScanner.ScanSettings requestSettings2 = createRequest(channelsToSpec(2450, 5175), 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        requestSettings2.type = WifiScanner.TYPE_HIGH_ACCURACY;
        int requestId2 = 13;
        WorkSource workSource2 = new WorkSource(Binder.getCallingUid()); // don't explicitly set
        ScanResults results2 =
                ScanResults.create(0, WifiScanner.WIFI_BAND_UNSPECIFIED, 2450, 5175, 2450);

        WifiScanner.ScanSettings requestSettings3 = createRequest(channelsToSpec(5150), 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        requestSettings3.type = WifiScanner.TYPE_LOW_POWER;
        int requestId3 = 15;
        // Let one of the WorkSources be a chained workSource.
        WorkSource workSource3 = new WorkSource();
        workSource3.createWorkChain()
                .addNode(2292, "tag1");
        ScanResults results3 =
                ScanResults.create(0, WifiScanner.WIFI_BAND_UNSPECIFIED, 5150, 5150, 5150, 5150);

        WifiNative.ScanSettings nativeSettings2and3 = createSingleScanNativeSettingsForChannels(
                WifiNative.SCAN_TYPE_HIGH_ACCURACY, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN,
                channelsToSpec(2450, 5175, 5150));
        ScanResults results2and3 =
                ScanResults.merge(WifiScanner.WIFI_BAND_UNSPECIFIED, results2, results3);
        WorkSource workSource2and3 = new WorkSource();
        workSource2and3.add(workSource2);
        workSource2and3.add(workSource3);


        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                        any(WifiNative.ScanEventHandler.class))).thenReturn(true);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder handlerOrder = inOrder(handler);
        InOrder nativeOrder = inOrder(mWifiScannerImpl);

        // Run scan 1
        sendSingleScanRequest(controlChannel, requestId1, requestSettings1, workSource1);

        mLooper.dispatchAll();
        WifiNative.ScanEventHandler eventHandler1 = verifyStartSingleScan(nativeOrder,
                computeSingleScanNativeSettings(requestSettings1));
        verifySuccessfulResponse(handlerOrder, handler, requestId1);
        verify(mBatteryStats).noteWifiScanStartedFromSource(eq(workSource1));


        // Queue scan 2 (will not run because previous is in progress)
        // uses uid of calling process
        sendSingleScanRequest(controlChannel, requestId2, requestSettings2, null);
        mLooper.dispatchAll();
        verifySuccessfulResponse(handlerOrder, handler, requestId2);

        // Queue scan 3 (will not run because previous is in progress)
        sendSingleScanRequest(controlChannel, requestId3, requestSettings3, workSource3);
        mLooper.dispatchAll();
        verifySuccessfulResponse(handlerOrder, handler, requestId3);

        // dispatch scan 1 results
        when(mWifiScannerImpl.getLatestSingleScanResults())
                .thenReturn(results1.getScanData());
        eventHandler1.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);

        mLooper.dispatchAll();
        verifyScanResultsReceived(handlerOrder, handler, requestId1, results1.getScanData());
        verifySingleScanCompletedReceived(handlerOrder, handler, requestId1);
        verify(mBatteryStats).noteWifiScanStoppedFromSource(eq(workSource1));
        verify(mBatteryStats).noteWifiScanStartedFromSource(eq(workSource2and3));

        // now that the first scan completed we expect the second and third ones to start
        WifiNative.ScanEventHandler eventHandler2and3 = verifyStartSingleScan(nativeOrder,
                nativeSettings2and3);

        // dispatch scan 2 and 3 results
        when(mWifiScannerImpl.getLatestSingleScanResults())
                .thenReturn(results2and3.getScanData());
        eventHandler2and3.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);

        mLooper.dispatchAll();

        verifyMultipleSingleScanResults(handlerOrder, handler, requestId2, results2, requestId3,
                results3);
        assertEquals(mWifiMetrics.getOneshotScanCount(), 3);
        assertEquals(mWifiMetrics.getScanReturnEntry(WifiMetricsProto.WifiLog.SCAN_SUCCESS), 3);

        verify(mBatteryStats).noteWifiScanStoppedFromSource(eq(workSource2and3));

        assertDumpContainsRequestLog("addSingleScanRequest", requestId1);
        assertDumpContainsRequestLog("addSingleScanRequest", requestId2);
        assertDumpContainsRequestLog("addSingleScanRequest", requestId3);
        assertDumpContainsCallbackLog("singleScanResults", requestId1,
                "results=" + results1.getRawScanResults().length);
        assertDumpContainsCallbackLog("singleScanResults", requestId2,
                "results=" + results2.getRawScanResults().length);
        assertDumpContainsCallbackLog("singleScanResults", requestId3,
                "results=" + results3.getRawScanResults().length);
    }

    /**
     * Send a single scan request and then a second one satisfied by the first before the first
     * completes. Verify that only one scan is scheduled.
     * Validates that a low latency scan type request is satisfied by an ongoing high accuracy
     * scan.
     */
    @Test
    public void sendSingleScanRequestWhilePreviousScanRunningAndMergeIntoFirstScan() {
        // Split by frequency to make it easier to determine which results each request is expecting
        ScanResults results24GHz =
                ScanResults.create(0, WifiScanner.WIFI_BAND_BOTH, 2400, 2400, 2400, 2450);
        ScanResults results5GHz =
                ScanResults.create(0, WifiScanner.WIFI_BAND_BOTH, 5150, 5150, 5175);
        ScanResults resultsBoth =
                ScanResults.merge(WifiScanner.WIFI_BAND_BOTH, results24GHz, results5GHz);

        WifiScanner.ScanSettings requestSettings1 = createRequest(
                WifiScanner.TYPE_HIGH_ACCURACY, WifiScanner.WIFI_BAND_BOTH, 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId1 = 12;
        ScanResults results1 = resultsBoth;

        WifiScanner.ScanSettings requestSettings2 = createRequest(
                WifiScanner.TYPE_LOW_LATENCY, WifiScanner.WIFI_BAND_24_GHZ, 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId2 = 13;
        ScanResults results2 = results24GHz;


        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                        any(WifiNative.ScanEventHandler.class))).thenReturn(true);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder handlerOrder = inOrder(handler);
        InOrder nativeOrder = inOrder(mWifiScannerImpl);

        // Run scan 1
        sendSingleScanRequest(controlChannel, requestId1, requestSettings1, null);

        mLooper.dispatchAll();
        WifiNative.ScanEventHandler eventHandler = verifyStartSingleScan(nativeOrder,
                computeSingleScanNativeSettings(requestSettings1));
        verifySuccessfulResponse(handlerOrder, handler, requestId1);

        // Queue scan 2 (will be folded into ongoing scan)
        sendSingleScanRequest(controlChannel, requestId2, requestSettings2, null);
        mLooper.dispatchAll();
        verifySuccessfulResponse(handlerOrder, handler, requestId2);

        // dispatch scan 1 results
        when(mWifiScannerImpl.getLatestSingleScanResults())
                .thenReturn(resultsBoth.getScanData());
        eventHandler.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);

        mLooper.dispatchAll();
        verifyMultipleSingleScanResults(handlerOrder, handler, requestId1, results1, requestId2,
                results2);

        assertEquals(mWifiMetrics.getOneshotScanCount(), 2);
        assertEquals(mWifiMetrics.getScanReturnEntry(WifiMetricsProto.WifiLog.SCAN_SUCCESS), 2);
    }

    /**
     * Send a single scan request and then two more before the first completes, one of which is
     * satisfied by the first scan. Verify that the first two complete together the second scan is
     * just for the other scan.
     * Validates that a high accuracy scan request is not satisfied by an ongoing low latency scan,
     * while any other low latency/low power scan request is satisfied.
     */
    @Test
    public void sendMultipleSingleScanRequestWhilePreviousScanRunningAndMergeOneIntoFirstScan()
          throws RemoteException {
        // Split by frequency to make it easier to determine which results each request is expecting
        ScanResults results2400 =
                ScanResults.create(0, WifiScanner.WIFI_BAND_UNSPECIFIED, 2400, 2400, 2400);
        ScanResults results2450 = ScanResults.create(0, WifiScanner.WIFI_BAND_UNSPECIFIED, 2450);
        ScanResults results1and3 =
                ScanResults.merge(WifiScanner.WIFI_BAND_UNSPECIFIED, results2400, results2450);

        WifiScanner.ScanSettings requestSettings1 = createRequest(channelsToSpec(2400, 2450), 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        requestSettings1.type = WifiScanner.TYPE_LOW_LATENCY;
        int requestId1 = 12;
        WorkSource workSource1 = new WorkSource(1121);
        ScanResults results1 = results1and3;

        WifiScanner.ScanSettings requestSettings2 = createRequest(channelsToSpec(2450, 5175), 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        requestSettings2.type = WifiScanner.TYPE_HIGH_ACCURACY;
        int requestId2 = 13;
        WorkSource workSource2 = new WorkSource(Binder.getCallingUid()); // don't explicitly set
        ScanResults results2 = ScanResults.create(0, 2450, 5175, 2450);

        WifiScanner.ScanSettings requestSettings3 = createRequest(channelsToSpec(2400), 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        requestSettings3.type = WifiScanner.TYPE_LOW_POWER;
        int requestId3 = 15;
        WorkSource workSource3 = new WorkSource(2292);
        ScanResults results3 = results2400;

        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                        any(WifiNative.ScanEventHandler.class))).thenReturn(true);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder handlerOrder = inOrder(handler);
        InOrder nativeOrder = inOrder(mWifiScannerImpl);

        // Run scan 1
        sendSingleScanRequest(controlChannel, requestId1, requestSettings1, workSource1);

        mLooper.dispatchAll();
        WifiNative.ScanEventHandler eventHandler1 = verifyStartSingleScan(nativeOrder,
                computeSingleScanNativeSettings(requestSettings1));
        verifySuccessfulResponse(handlerOrder, handler, requestId1);
        verify(mBatteryStats).noteWifiScanStartedFromSource(eq(workSource1));


        // Queue scan 2 (will not run because previous is in progress)
        // uses uid of calling process
        sendSingleScanRequest(controlChannel, requestId2, requestSettings2, null);
        mLooper.dispatchAll();
        verifySuccessfulResponse(handlerOrder, handler, requestId2);

        // Queue scan 3 (will be merged into the active scan)
        sendSingleScanRequest(controlChannel, requestId3, requestSettings3, workSource3);
        mLooper.dispatchAll();
        verifySuccessfulResponse(handlerOrder, handler, requestId3);

        // dispatch scan 1 results
        when(mWifiScannerImpl.getLatestSingleScanResults())
                .thenReturn(results1and3.getScanData());
        eventHandler1.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);

        mLooper.dispatchAll();
        verifyMultipleSingleScanResults(handlerOrder, handler, requestId1, results1, requestId3,
                results3);
        // only the requests know at the beginning of the scan get blamed
        verify(mBatteryStats).noteWifiScanStoppedFromSource(eq(workSource1));
        verify(mBatteryStats).noteWifiScanStartedFromSource(eq(workSource2));

        // now that the first scan completed we expect the second and third ones to start
        WifiNative.ScanEventHandler eventHandler2 = verifyStartSingleScan(nativeOrder,
                computeSingleScanNativeSettings(requestSettings2));

        // dispatch scan 2 and 3 results
        when(mWifiScannerImpl.getLatestSingleScanResults())
                .thenReturn(results2.getScanData());
        eventHandler2.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);

        mLooper.dispatchAll();

        verifyScanResultsReceived(handlerOrder, handler, requestId2, results2.getScanData());
        verifySingleScanCompletedReceived(handlerOrder, handler, requestId2);
        assertEquals(mWifiMetrics.getOneshotScanCount(), 3);
        assertEquals(mWifiMetrics.getScanReturnEntry(WifiMetricsProto.WifiLog.SCAN_SUCCESS), 3);

        verify(mBatteryStats).noteWifiScanStoppedFromSource(eq(workSource2));

        assertDumpContainsRequestLog("addSingleScanRequest", requestId1);
        assertDumpContainsRequestLog("addSingleScanRequest", requestId2);
        assertDumpContainsRequestLog("addSingleScanRequest", requestId3);
        assertDumpContainsCallbackLog("singleScanResults", requestId1,
                "results=" + results1.getRawScanResults().length);
        assertDumpContainsCallbackLog("singleScanResults", requestId2,
                "results=" + results2.getRawScanResults().length);
        assertDumpContainsCallbackLog("singleScanResults", requestId3,
                "results=" + results3.getRawScanResults().length);
    }

    /**
     * Verify that WifiService provides a way to get the most recent SingleScan results.
     */
    @Test
    public void retrieveSingleScanResults() throws Exception {
        WifiScanner.ScanSettings requestSettings =
                createRequest(WifiScanner.WIFI_BAND_BOTH_WITH_DFS,
                              0, 0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        ScanResults expectedResults =
                ScanResults.create(0, WifiScanner.WIFI_BAND_BOTH_WITH_DFS, 2400, 5150, 5175);
        doSuccessfulSingleScan(requestSettings,
                               computeSingleScanNativeSettings(requestSettings),
                               expectedResults);
        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        controlChannel.sendMessage(
                Message.obtain(null, WifiScanner.CMD_GET_SINGLE_SCAN_RESULTS, 0));
        mLooper.dispatchAll();
        Message response = verifyHandleMessageAndGetMessage(order, handler);
        List<ScanResult> results = Arrays.asList(
                ((WifiScanner.ParcelableScanResults) response.obj).getResults());
        assertEquals(results.size(), expectedResults.getRawScanResults().length);

        // Make sure that we logged the scan results in the dump method.
        String serviceDump = dumpService();
        Pattern logLineRegex = Pattern.compile("Latest scan results:");
        assertTrue("dump did not contain Latest scan results: " + serviceDump + "\n",
                logLineRegex.matcher(serviceDump).find());
    }

    /**
     * Verify that WifiService provides a way to get the most recent SingleScan results even when
     * they are empty.
     */
    @Test
    public void retrieveSingleScanResultsEmpty() throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(WifiScanner.WIFI_BAND_BOTH, 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        doSuccessfulSingleScan(requestSettings, computeSingleScanNativeSettings(requestSettings),
                ScanResults.create(0, WifiScanner.WIFI_BAND_BOTH, new int[0]));

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        controlChannel.sendMessage(
                Message.obtain(null, WifiScanner.CMD_GET_SINGLE_SCAN_RESULTS, 0));
        mLooper.dispatchAll();
        Message response = verifyHandleMessageAndGetMessage(order, handler);

        List<ScanResult> results = Arrays.asList(
                ((WifiScanner.ParcelableScanResults) response.obj).getResults());
        assertEquals(results.size(), 0);
    }

    /**
     * Verify that WifiService will return empty SingleScan results if a scan has not been
     * performed.
     */
    @Test
    public void retrieveSingleScanResultsBeforeAnySingleScans() throws Exception {
        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);
        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        controlChannel.sendMessage(
                Message.obtain(null, WifiScanner.CMD_GET_SINGLE_SCAN_RESULTS, 0));
        mLooper.dispatchAll();
        Message response = verifyHandleMessageAndGetMessage(order, handler);

        List<ScanResult> results = Arrays.asList(
                ((WifiScanner.ParcelableScanResults) response.obj).getResults());
        assertEquals(results.size(), 0);
    }

    /**
     * Verify that the newest full scan results are returned by WifiService.getSingleScanResults.
     */
    @Test
    public void retrieveMostRecentFullSingleScanResults() throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(WifiScanner.WIFI_BAND_BOTH, 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        ScanResults expectedResults = ScanResults.create(
                0, WifiScanner.WIFI_BAND_BOTH, 2400, 5150, 5175);
        doSuccessfulSingleScan(requestSettings,
                               computeSingleScanNativeSettings(requestSettings),
                               expectedResults);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        controlChannel.sendMessage(
                Message.obtain(null, WifiScanner.CMD_GET_SINGLE_SCAN_RESULTS, 0));
        mLooper.dispatchAll();
        Message response = verifyHandleMessageAndGetMessage(order, handler);

        List<ScanResult> results = Arrays.asList(
                ((WifiScanner.ParcelableScanResults) response.obj).getResults());
        assertEquals(results.size(), expectedResults.getRawScanResults().length);

        // now update with a new scan that only has one result
        int secondScanRequestId = 35;
        ScanResults expectedSingleResult = ScanResults.create(0, WifiScanner.WIFI_BAND_BOTH, 5150);
        sendSingleScanRequest(controlChannel, secondScanRequestId, requestSettings, null);

        mLooper.dispatchAll();
        WifiNative.ScanEventHandler eventHandler = verifyStartSingleScan(order,
                computeSingleScanNativeSettings(requestSettings));
        verifySuccessfulResponse(order, handler, secondScanRequestId);

        // dispatch scan 2 results
        when(mWifiScannerImpl.getLatestSingleScanResults())
                .thenReturn(expectedSingleResult.getScanData());
        eventHandler.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);

        mLooper.dispatchAll();
        verifyScanResultsReceived(order, handler, secondScanRequestId,
                expectedSingleResult.getScanData());
        verifySingleScanCompletedReceived(order, handler, secondScanRequestId);

        controlChannel.sendMessage(
                Message.obtain(null, WifiScanner.CMD_GET_SINGLE_SCAN_RESULTS, 0));
        mLooper.dispatchAll();
        Message response2 = verifyHandleMessageAndGetMessage(order, handler);

        List<ScanResult> results2 = Arrays.asList(
                ((WifiScanner.ParcelableScanResults) response2.obj).getResults());
        assertEquals(results2.size(), expectedSingleResult.getRawScanResults().length);
    }

    /**
     * Verify that the newest partial scan results are not returned by
     * WifiService.getSingleScanResults.
     */
    @Test
    public void doesNotRetrieveMostRecentPartialSingleScanResults() throws Exception {
        WifiScanner.ScanSettings fullRequestSettings = createRequest(WifiScanner.WIFI_BAND_BOTH, 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        ScanResults expectedFullResults =
                ScanResults.create(0, WifiScanner.WIFI_BAND_BOTH, 2400, 5150, 5175);
        doSuccessfulSingleScan(fullRequestSettings,
                computeSingleScanNativeSettings(fullRequestSettings),
                expectedFullResults);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        controlChannel.sendMessage(
                Message.obtain(null, WifiScanner.CMD_GET_SINGLE_SCAN_RESULTS, 0));
        mLooper.dispatchAll();
        Message response = verifyHandleMessageAndGetMessage(order, handler);

        List<ScanResult> results = Arrays.asList(
                ((WifiScanner.ParcelableScanResults) response.obj).getResults());
        assertEquals(results.size(), expectedFullResults.getRawScanResults().length);

        // now update with a new scan that only has one result
        int secondScanRequestId = 35;
        WifiScanner.ScanSettings partialRequestSettings = createRequest(WifiScanner.WIFI_BAND_BOTH,
                0, 0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        ScanResults expectedPartialResults =
                ScanResults.create(0, WifiScanner.WIFI_BAND_5_GHZ, 5150);
        sendSingleScanRequest(controlChannel, secondScanRequestId, partialRequestSettings, null);

        mLooper.dispatchAll();
        WifiNative.ScanEventHandler eventHandler = verifyStartSingleScan(order,
                computeSingleScanNativeSettings(partialRequestSettings));
        verifySuccessfulResponse(order, handler, secondScanRequestId);

        // dispatch scan 2 results
        when(mWifiScannerImpl.getLatestSingleScanResults())
                .thenReturn(expectedPartialResults.getScanData());
        eventHandler.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);

        mLooper.dispatchAll();
        verifyScanResultsReceived(order, handler, secondScanRequestId,
                expectedPartialResults.getScanData());
        verifySingleScanCompletedReceived(order, handler, secondScanRequestId);

        controlChannel.sendMessage(
                Message.obtain(null, WifiScanner.CMD_GET_SINGLE_SCAN_RESULTS, 0));
        mLooper.dispatchAll();
        Message response2 = verifyHandleMessageAndGetMessage(order, handler);

        List<ScanResult> results2 = Arrays.asList(
                ((WifiScanner.ParcelableScanResults) response2.obj).getResults());
        assertEquals(results2.size(), expectedFullResults.getRawScanResults().length);
    }

    /**
     * Verify that the scan results returned by WifiService.getSingleScanResults are not older
     * than {@link com.android.server.wifi.scanner.WifiScanningServiceImpl
     * .WifiSingleScanStateMachine#CACHED_SCAN_RESULTS_MAX_AGE_IN_MILLIS}.
     */
    @Test
    public void doesNotRetrieveStaleScanResultsFromLastFullSingleScan() throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(WifiScanner.WIFI_BAND_BOTH, 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        ScanResults scanResults =
                ScanResults.create(0, WifiScanner.WIFI_BAND_BOTH, 2400, 5150, 5175);

        // Out of the 3 scan results, modify the timestamp of 2 of them to be within the expiration
        // age and 1 out of it.
        long currentTimeInMillis = CACHED_SCAN_RESULTS_MAX_AGE_IN_MILLIS * 2;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTimeInMillis);
        scanResults.getRawScanResults()[0].timestamp = (currentTimeInMillis - 1) * 1000;
        scanResults.getRawScanResults()[1].timestamp = (currentTimeInMillis - 2) * 1000;
        scanResults.getRawScanResults()[2].timestamp =
                (currentTimeInMillis - CACHED_SCAN_RESULTS_MAX_AGE_IN_MILLIS) * 1000;
        List<ScanResult> expectedResults = new ArrayList<ScanResult>() {{
                add(scanResults.getRawScanResults()[0]);
                add(scanResults.getRawScanResults()[1]);
            }};

        doSuccessfulSingleScan(requestSettings,
                computeSingleScanNativeSettings(requestSettings), scanResults);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        controlChannel.sendMessage(
                Message.obtain(null, WifiScanner.CMD_GET_SINGLE_SCAN_RESULTS, 0));
        mLooper.dispatchAll();
        Message response = verifyHandleMessageAndGetMessage(order, handler);

        List<ScanResult> results = Arrays.asList(
                ((WifiScanner.ParcelableScanResults) response.obj).getResults());
        assertScanResultsEquals(expectedResults.toArray(new ScanResult[expectedResults.size()]),
                results.toArray(new ScanResult[results.size()]));
    }

    /**
     * Cached scan results should be cleared after the driver is unloaded.
     */
    @Test
    public void validateScanResultsClearedAfterDriverUnloaded() throws Exception {
        WifiScanner.ScanSettings requestSettings =
                createRequest(WifiScanner.WIFI_BAND_BOTH_WITH_DFS,
                              0, 0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        ScanResults expectedResults = ScanResults.create(
                0, WifiScanner.WIFI_BAND_BOTH_WITH_DFS, 2400, 5150, 5175);
        doSuccessfulSingleScan(requestSettings,
                               computeSingleScanNativeSettings(requestSettings),
                               expectedResults);
        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        controlChannel.sendMessage(
                Message.obtain(null, WifiScanner.CMD_GET_SINGLE_SCAN_RESULTS, 0));
        mLooper.dispatchAll();
        Message response = verifyHandleMessageAndGetMessage(order, handler);
        List<ScanResult> results = Arrays.asList(
                ((WifiScanner.ParcelableScanResults) response.obj).getResults());
        assertEquals(results.size(), expectedResults.getRawScanResults().length);

        // disable wifi
        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_DISABLE));
        // Now get scan results again. The returned list should be empty since we
        // clear the cache when exiting the DriverLoaded state.
        controlChannel.sendMessage(
                Message.obtain(null, WifiScanner.CMD_GET_SINGLE_SCAN_RESULTS, 0));
        mLooper.dispatchAll();
        Message response2 = verifyHandleMessageAndGetMessage(order, handler);
        List<ScanResult> results2 = Arrays.asList(
                ((WifiScanner.ParcelableScanResults) response2.obj).getResults());
        assertEquals(results2.size(), 0);
    }

    /**
     * Register a single scan listener and do a single scan
     */
    @Test
    public void registerScanListener() throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(WifiScanner.WIFI_BAND_BOTH, 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        WifiNative.ScanSettings nativeSettings = computeSingleScanNativeSettings(requestSettings);
        ScanResults results = ScanResults.create(0, 2400, 5150, 5175);

        int requestId = 12;
        int listenerRequestId = 13;

        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                        any(WifiNative.ScanEventHandler.class))).thenReturn(true);

        registerScanListener(controlChannel, listenerRequestId);
        mLooper.dispatchAll();
        verifySuccessfulResponse(order, handler, listenerRequestId);

        sendSingleScanRequest(controlChannel, requestId, requestSettings, null);

        mLooper.dispatchAll();
        WifiNative.ScanEventHandler eventHandler = verifyStartSingleScan(order, nativeSettings);
        verifySuccessfulResponse(order, handler, requestId);

        when(mWifiScannerImpl.getLatestSingleScanResults())
                .thenReturn(results.getRawScanData());
        eventHandler.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);

        mLooper.dispatchAll();
        verifyScanResultsReceived(order, handler, requestId, results.getScanData());
        verifySingleScanCompletedReceived(order, handler, requestId);
        verifyScanResultsReceived(order, handler, listenerRequestId, results.getScanData());
        verifyNoMoreInteractions(handler);

        assertDumpContainsRequestLog("registerScanListener", listenerRequestId);
        assertDumpContainsCallbackLog("singleScanResults", listenerRequestId,
                "results=" + results.getScanData().getResults().length);
    }

    /**
     * Register a single scan listener and do a single scan
     */
    @Test
    public void deregisterScanListener() throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(WifiScanner.WIFI_BAND_BOTH, 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        WifiNative.ScanSettings nativeSettings = computeSingleScanNativeSettings(requestSettings);
        ScanResults results = ScanResults.create(0, 2400, 5150, 5175);

        int requestId = 12;
        int listenerRequestId = 13;

        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                        any(WifiNative.ScanEventHandler.class))).thenReturn(true);

        registerScanListener(controlChannel, listenerRequestId);
        mLooper.dispatchAll();
        verifySuccessfulResponse(order, handler, listenerRequestId);

        sendSingleScanRequest(controlChannel, requestId, requestSettings, null);

        mLooper.dispatchAll();
        WifiNative.ScanEventHandler eventHandler = verifyStartSingleScan(order, nativeSettings);
        verifySuccessfulResponse(order, handler, requestId);

        deregisterScanListener(controlChannel, listenerRequestId);
        mLooper.dispatchAll();

        when(mWifiScannerImpl.getLatestSingleScanResults())
                .thenReturn(results.getRawScanData());
        eventHandler.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);

        mLooper.dispatchAll();
        verifyScanResultsReceived(order, handler, requestId, results.getScanData());
        verifySingleScanCompletedReceived(order, handler, requestId);
        verifyNoMoreInteractions(handler);

        assertDumpContainsRequestLog("registerScanListener", listenerRequestId);
        assertDumpContainsRequestLog("deregisterScanListener", listenerRequestId);
    }

    /**
     * Send a single scan request and then two more before the first completes. Neither are
     * satisfied by the first scan. Verify that the first completes and the second two are merged.
     */
    @Test
    public void scanListenerReceivesAllResults() throws RemoteException {
        WifiScanner.ScanSettings requestSettings1 = createRequest(channelsToSpec(2400), 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId1 = 12;
        ScanResults results1 = ScanResults.create(0, WifiScanner.WIFI_BAND_UNSPECIFIED, 2400);

        WifiScanner.ScanSettings requestSettings2 = createRequest(channelsToSpec(2450, 5175), 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId2 = 13;
        ScanResults results2 =
                ScanResults.create(0, WifiScanner.WIFI_BAND_UNSPECIFIED, 2450, 5175, 2450);

        WifiScanner.ScanSettings requestSettings3 = createRequest(channelsToSpec(5150), 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId3 = 15;
        ScanResults results3 =
                ScanResults.create(0, WifiScanner.WIFI_BAND_UNSPECIFIED, 5150, 5150, 5150, 5150);

        WifiNative.ScanSettings nativeSettings2and3 = createSingleScanNativeSettingsForChannels(
                WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN, channelsToSpec(2450, 5175, 5150));
        ScanResults results2and3 =
                ScanResults.merge(WifiScanner.WIFI_BAND_UNSPECIFIED, results2, results3);

        int listenerRequestId = 13;


        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                        any(WifiNative.ScanEventHandler.class))).thenReturn(true);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder handlerOrder = inOrder(handler);
        InOrder nativeOrder = inOrder(mWifiScannerImpl);

        // Run scan 1
        sendSingleScanRequest(controlChannel, requestId1, requestSettings1, null);

        mLooper.dispatchAll();
        WifiNative.ScanEventHandler eventHandler1 = verifyStartSingleScan(nativeOrder,
                computeSingleScanNativeSettings(requestSettings1));
        verifySuccessfulResponse(handlerOrder, handler, requestId1);


        // Queue scan 2 (will not run because previous is in progress)
        sendSingleScanRequest(controlChannel, requestId2, requestSettings2, null);
        mLooper.dispatchAll();
        verifySuccessfulResponse(handlerOrder, handler, requestId2);

        // Queue scan 3 (will not run because previous is in progress)
        sendSingleScanRequest(controlChannel, requestId3, requestSettings3, null);
        mLooper.dispatchAll();
        verifySuccessfulResponse(handlerOrder, handler, requestId3);

        // Register scan listener
        registerScanListener(controlChannel, listenerRequestId);
        mLooper.dispatchAll();
        verifySuccessfulResponse(handlerOrder, handler, listenerRequestId);

        // dispatch scan 1 results
        when(mWifiScannerImpl.getLatestSingleScanResults())
                .thenReturn(results1.getScanData());
        eventHandler1.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);

        mLooper.dispatchAll();
        verifyScanResultsReceived(handlerOrder, handler, requestId1, results1.getScanData());
        verifySingleScanCompletedReceived(handlerOrder, handler, requestId1);
        verifyScanResultsReceived(handlerOrder, handler, listenerRequestId, results1.getScanData());

        // now that the first scan completed we expect the second and third ones to start
        WifiNative.ScanEventHandler eventHandler2and3 = verifyStartSingleScan(nativeOrder,
                nativeSettings2and3);

        // dispatch scan 2 and 3 results
        when(mWifiScannerImpl.getLatestSingleScanResults())
                .thenReturn(results2and3.getScanData());
        eventHandler2and3.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);

        mLooper.dispatchAll();

        verifyMultipleSingleScanResults(handlerOrder, handler, requestId2, results2, requestId3,
                results3, listenerRequestId, results2and3);

        assertDumpContainsRequestLog("registerScanListener", listenerRequestId);
        assertDumpContainsCallbackLog("singleScanResults", listenerRequestId,
                "results=" + results1.getRawScanResults().length);
        assertDumpContainsCallbackLog("singleScanResults", listenerRequestId,
                "results=" + results2and3.getRawScanResults().length);
    }

    @Test
    public void rejectSingleScanRequestWhenScannerImplCreateFails() throws Exception {
        // Fail scanner impl creation.
        when(mWifiScannerImplFactory.create(any(), any(), any())).thenReturn(null);

        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler);
        sendSingleScanRequest(controlChannel, 122, generateValidScanSettings(), null);
        mLooper.dispatchAll();
        verifyFailedResponse(order, handler, 122, WifiScanner.REASON_UNSPECIFIED, "not available");
    }


    private void doSuccessfulBackgroundScan(WifiScanner.ScanSettings requestSettings,
            WifiNative.ScanSettings nativeSettings) {
        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        when(mWifiScannerImpl.startBatchedScan(any(WifiNative.ScanSettings.class),
                        any(WifiNative.ScanEventHandler.class))).thenReturn(true);

        sendBackgroundScanRequest(controlChannel, 12, requestSettings, null);
        mLooper.dispatchAll();
        verifyStartBackgroundScan(order, nativeSettings);
        verifySuccessfulResponse(order, handler, 12);
        verifyNoMoreInteractions(handler);
        assertDumpContainsRequestLog("addBackgroundScanRequest", 12);
    }

    /**
     * Do a background scan for a band and verify that it is successful.
     */
    @Test
    public void sendBackgroundScanBandRequest() throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(WifiScanner.WIFI_BAND_BOTH, 30000,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        WifiNative.ScanSettings nativeSettings = new NativeScanSettingsBuilder()
                .withBasePeriod(30000)
                .withMaxApPerScan(MAX_AP_PER_SCAN)
                .withMaxScansToCache(BackgroundScanScheduler.DEFAULT_MAX_SCANS_TO_BATCH)
                .addBucketWithBand(30000, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN,
                        WifiScanner.WIFI_BAND_BOTH)
                .build();
        doSuccessfulBackgroundScan(requestSettings, nativeSettings);
        assertEquals(mWifiMetrics.getBackgroundScanCount(), 1);
    }

    /**
     * Do a background scan for a list of channels and verify that it is successful.
     */
    @Test
    public void sendBackgroundScanChannelsRequest() throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(channelsToSpec(5150), 30000,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        WifiNative.ScanSettings nativeSettings = new NativeScanSettingsBuilder()
                .withBasePeriod(30000)
                .withMaxApPerScan(MAX_AP_PER_SCAN)
                .withMaxScansToCache(BackgroundScanScheduler.DEFAULT_MAX_SCANS_TO_BATCH)
                .addBucketWithChannels(30000, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN, 5150)
                .build();
        doSuccessfulBackgroundScan(requestSettings, nativeSettings);
    }

    private Pair<WifiScanner.ScanSettings, WifiNative.ScanSettings> createScanSettingsForHwPno()
            throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(
                channelsToSpec(0, 2400, 5150, 5175), 30000, 0, 20,
                WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        WifiNative.ScanSettings nativeSettings = new NativeScanSettingsBuilder()
                .withBasePeriod(30000)
                .withMaxApPerScan(MAX_AP_PER_SCAN)
                .withMaxScansToCache(BackgroundScanScheduler.DEFAULT_MAX_SCANS_TO_BATCH)
                .addBucketWithChannels(30000, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN,
                        0, 2400, 5150, 5175)
                .build();
        return Pair.create(requestSettings, nativeSettings);
    }

    private Pair<WifiScanner.PnoSettings, WifiNative.PnoSettings> createPnoSettings(
            ScanResults results)
            throws Exception {
        WifiScanner.PnoSettings requestPnoSettings = new WifiScanner.PnoSettings();
        requestPnoSettings.networkList =
                new WifiScanner.PnoSettings.PnoNetwork[results.getRawScanResults().length];
        int i = 0;
        for (ScanResult scanResult : results.getRawScanResults()) {
            requestPnoSettings.networkList[i++] =
                    new WifiScanner.PnoSettings.PnoNetwork(scanResult.SSID);
        }

        WifiNative.PnoSettings nativePnoSettings = new WifiNative.PnoSettings();
        nativePnoSettings.min5GHzRssi = requestPnoSettings.min5GHzRssi;
        nativePnoSettings.min24GHzRssi = requestPnoSettings.min24GHzRssi;
        nativePnoSettings.initialScoreMax = requestPnoSettings.initialScoreMax;
        nativePnoSettings.currentConnectionBonus = requestPnoSettings.currentConnectionBonus;
        nativePnoSettings.sameNetworkBonus = requestPnoSettings.sameNetworkBonus;
        nativePnoSettings.secureBonus = requestPnoSettings.secureBonus;
        nativePnoSettings.band5GHzBonus = requestPnoSettings.band5GHzBonus;
        nativePnoSettings.isConnected = requestPnoSettings.isConnected;
        nativePnoSettings.networkList =
                new WifiNative.PnoNetwork[requestPnoSettings.networkList.length];
        for (i = 0; i < requestPnoSettings.networkList.length; i++) {
            nativePnoSettings.networkList[i] = new WifiNative.PnoNetwork();
            nativePnoSettings.networkList[i].ssid = requestPnoSettings.networkList[i].ssid;
            nativePnoSettings.networkList[i].flags = requestPnoSettings.networkList[i].flags;
            nativePnoSettings.networkList[i].auth_bit_field =
                    requestPnoSettings.networkList[i].authBitField;
        }
        return Pair.create(requestPnoSettings, nativePnoSettings);
    }

    private ScanResults createScanResultsForPno() {
        return ScanResults.create(0, 2400, 5150, 5175);
    }

    private WifiNative.PnoEventHandler verifyHwPno(InOrder order,
            WifiNative.PnoSettings expected) {
        ArgumentCaptor<WifiNative.PnoSettings> pnoSettingsCaptor =
                ArgumentCaptor.forClass(WifiNative.PnoSettings.class);
        ArgumentCaptor<WifiNative.PnoEventHandler> pnoEventHandlerCaptor =
                ArgumentCaptor.forClass(WifiNative.PnoEventHandler.class);
        order.verify(mWifiScannerImpl).setHwPnoList(pnoSettingsCaptor.capture(),
                pnoEventHandlerCaptor.capture());
        assertNativePnoSettingsEquals(expected, pnoSettingsCaptor.getValue());
        return pnoEventHandlerCaptor.getValue();
    }

    private void sendPnoScanRequest(BidirectionalAsyncChannel controlChannel,
            int scanRequestId, WifiScanner.ScanSettings scanSettings,
            WifiScanner.PnoSettings pnoSettings) {
        Bundle pnoParams = new Bundle();
        scanSettings.isPnoScan = true;
        pnoParams.putParcelable(WifiScanner.PNO_PARAMS_SCAN_SETTINGS_KEY, scanSettings);
        pnoParams.putParcelable(WifiScanner.PNO_PARAMS_PNO_SETTINGS_KEY, pnoSettings);
        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_START_PNO_SCAN, 0,
                scanRequestId, pnoParams));
    }

    private void assertPnoNetworkFoundMessage(int listenerId, ScanResult[] expected,
            Message networkFoundMessage) {
        assertEquals("what", WifiScanner.CMD_PNO_NETWORK_FOUND, networkFoundMessage.what);
        assertEquals("listenerId", listenerId, networkFoundMessage.arg2);
        assertScanResultsEquals(expected,
                ((WifiScanner.ParcelableScanResults) networkFoundMessage.obj).getResults());
    }

    private void verifyPnoNetworkFoundReceived(InOrder order, Handler handler, int listenerId,
            ScanResult[] expected) {
        Message scanResultMessage = verifyHandleMessageAndGetMessage(order, handler,
                WifiScanner.CMD_PNO_NETWORK_FOUND);
        assertPnoNetworkFoundMessage(listenerId, expected, scanResultMessage);
    }

    private void expectSuccessfulBackgroundScan(InOrder order,
            WifiNative.ScanSettings nativeSettings, ScanResults results) {
        when(mWifiScannerImpl.startBatchedScan(any(WifiNative.ScanSettings.class),
                any(WifiNative.ScanEventHandler.class))).thenReturn(true);
        mLooper.dispatchAll();
        WifiNative.ScanEventHandler eventHandler = verifyStartBackgroundScan(order, nativeSettings);
        WifiScanner.ScanData[] scanDatas = new WifiScanner.ScanData[1];
        scanDatas[0] = results.getScanData();
        for (ScanResult fullScanResult : results.getRawScanResults()) {
            eventHandler.onFullScanResult(fullScanResult, 0);
        }
        when(mWifiScannerImpl.getLatestBatchedScanResults(anyBoolean())).thenReturn(scanDatas);
        eventHandler.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);
        mLooper.dispatchAll();
    }

    private void expectHwPnoScan(InOrder order, Handler handler, int requestId,
            WifiNative.PnoSettings nativeSettings, ScanResults results) {
        when(mWifiScannerImpl.isHwPnoSupported(anyBoolean())).thenReturn(true);

        when(mWifiScannerImpl.setHwPnoList(any(WifiNative.PnoSettings.class),
                any(WifiNative.PnoEventHandler.class))).thenReturn(true);
        mLooper.dispatchAll();
        WifiNative.PnoEventHandler eventHandler = verifyHwPno(order, nativeSettings);
        verifySuccessfulResponse(order, handler, requestId);
        eventHandler.onPnoNetworkFound(results.getRawScanResults());
        mLooper.dispatchAll();
    }

    /**
     * Tests wificond PNO scan when the PNO scan results contain IE info. This ensures that the
     * PNO scan results are plumbed back to the client as a PNO network found event.
     */
    @Test
    public void testSuccessfulHwPnoScanWithNoBackgroundScan() throws Exception {
        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);
        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);
        int requestId = 12;

        ScanResults scanResults = createScanResultsForPno();
        Pair<WifiScanner.ScanSettings, WifiNative.ScanSettings> scanSettings =
                createScanSettingsForHwPno();
        Pair<WifiScanner.PnoSettings, WifiNative.PnoSettings> pnoSettings =
                createPnoSettings(scanResults);

        sendPnoScanRequest(controlChannel, requestId, scanSettings.first, pnoSettings.first);
        expectHwPnoScan(order, handler, requestId, pnoSettings.second, scanResults);
        verifyPnoNetworkFoundReceived(order, handler, requestId, scanResults.getRawScanResults());
    }

    @Test
    public void rejectHwPnoScanRequestWhenScannerImplCreateFails() throws Exception {
        // Fail scanner impl creation.
        when(mWifiScannerImplFactory.create(any(), any(), any())).thenReturn(null);

        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        InOrder order = inOrder(handler, mWifiScannerImpl);

        ScanResults scanResults = createScanResultsForPno();
        Pair<WifiScanner.ScanSettings, WifiNative.ScanSettings> scanSettings =
                createScanSettingsForHwPno();
        Pair<WifiScanner.PnoSettings, WifiNative.PnoSettings> pnoSettings =
                createPnoSettings(scanResults);

        sendPnoScanRequest(controlChannel, 122, scanSettings.first, pnoSettings.first);
        mLooper.dispatchAll();
        verifyFailedResponse(order, handler, 122, WifiScanner.REASON_UNSPECIFIED, "not available");
    }

    /**
     * Tries to simulate the race scenario where a client is disconnected immediately after single
     * scan request is sent to |SingleScanStateMachine|.
     */
    @Test
    public void processSingleScanRequestAfterDisconnect() throws Exception {
        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);
        BidirectionalAsyncChannel controlChannel = connectChannel(mock(Handler.class));
        mLooper.dispatchAll();

        // Send the single scan request and then send the disconnect immediately after.
        WifiScanner.ScanSettings requestSettings = createRequest(WifiScanner.WIFI_BAND_BOTH, 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId = 10;

        sendSingleScanRequest(controlChannel, requestId, requestSettings, null);
        // Can't call |disconnect| here because that sends |CMD_CHANNEL_DISCONNECT| followed by
        // |CMD_CHANNEL_DISCONNECTED|.
        controlChannel.sendMessage(Message.obtain(null, AsyncChannel.CMD_CHANNEL_DISCONNECTED,
                        AsyncChannel.STATUS_REMOTE_DISCONNECTION, 0, null));

        // Now process the above 2 actions. This should result in first processing the single scan
        // request (which forwards the request to SingleScanStateMachine) and then processing the
        // disconnect after.
        mLooper.dispatchAll();

        // Now check that we logged the invalid request.
        String serviceDump = dumpService();
        Pattern logLineRegex = Pattern.compile("^.+" + "singleScanInvalidRequest: "
                + "ClientInfo\\[unknown\\],Id=" + requestId + ",bad request$", Pattern.MULTILINE);
        assertTrue("dump did not contain log with ClientInfo[unknown]: " + serviceDump + "\n",
                logLineRegex.matcher(serviceDump).find());
    }

    /**
     * Tries to simulate the race scenario where a client is disconnected immediately after single
     * scan request is sent to |SingleScanStateMachine|.
     */
    @Test
    public void sendScanRequestAfterUnsuccessfulSend() throws Exception {
        WifiScanner.ScanSettings requestSettings = createRequest(WifiScanner.WIFI_BAND_BOTH, 0,
                0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        int requestId = 9;

        startServiceAndLoadDriver();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);
        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        mLooper.dispatchAll();

        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                        any(WifiNative.ScanEventHandler.class))).thenReturn(true);
        ScanResults results = ScanResults.create(0, WifiScanner.WIFI_BAND_BOTH, 2400);
        when(mWifiScannerImpl.getLatestSingleScanResults())
                .thenReturn(results.getRawScanData());

        InOrder order = inOrder(mWifiScannerImpl, handler);

        sendSingleScanRequest(controlChannel, requestId, requestSettings, null);
        mLooper.dispatchAll();
        WifiNative.ScanEventHandler eventHandler1 = verifyStartSingleScan(order,
                computeSingleScanNativeSettings(requestSettings));
        verifySuccessfulResponse(order, handler, requestId);

        eventHandler1.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);
        mLooper.dispatchAll();
        verifyScanResultsReceived(order, handler, requestId, results.getScanData());
        verifySingleScanCompletedReceived(order, handler, requestId);
        verifyNoMoreInteractions(handler);

        controlChannel.sendMessage(Message.obtain(null, AsyncChannel.CMD_CHANNEL_DISCONNECTED,
                        AsyncChannel.STATUS_SEND_UNSUCCESSFUL, 0, null));
        mLooper.dispatchAll();

        sendSingleScanRequest(controlChannel, requestId, requestSettings, null);
        mLooper.dispatchAll();
        WifiNative.ScanEventHandler eventHandler2 = verifyStartSingleScan(order,
                computeSingleScanNativeSettings(requestSettings));
        verifySuccessfulResponse(order, handler, requestId);

        eventHandler2.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);
        mLooper.dispatchAll();
        verifyScanResultsReceived(order, handler, requestId, results.getScanData());
        verifySingleScanCompletedReceived(order, handler, requestId);
        verifyNoMoreInteractions(handler);
    }

    /**
     * Verifies that background scan works after duplicate scan enable.
     */
    @Test
    public void backgroundScanAfterDuplicateScanEnable() throws RemoteException {
        startServiceAndLoadDriver();

        // Send scan enable again.
        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_ENABLE));
        mLooper.dispatchAll();

        // Ensure we didn't create scanner instance twice.
        verify(mWifiScannerImplFactory, times(1))
                .create(any(), any(), any());

        InOrder order = inOrder(handler);
        when(mWifiScannerImpl.startBatchedScan(any(WifiNative.ScanSettings.class),
                any(WifiNative.ScanEventHandler.class))).thenReturn(true);
        sendBackgroundScanRequest(controlChannel, 192, generateValidScanSettings(), null);
        mLooper.dispatchAll();
        verifySuccessfulResponse(order, handler, 192);
        assertDumpContainsRequestLog("addBackgroundScanRequest", 192);
    }

    /**
     * Verifies that single scan works after duplicate scan enable.
     */
    @Test
    public void singleScanScanAfterDuplicateScanEnable() throws RemoteException {
        startServiceAndLoadDriver();

        // Send scan enable again.
        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_ENABLE));
        mLooper.dispatchAll();

        // Ensure we didn't create scanner instance twice.
        verify(mWifiScannerImplFactory, times(1))
                .create(any(), any(), any());

        InOrder order = inOrder(handler, mWifiScannerImpl);

        int requestId = 12;
        WorkSource workSource = new WorkSource(2292);
        WifiScanner.ScanSettings requestSettings = createRequest(channelsToSpec(2400, 5150, 5175),
                0, 0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
        ScanResults results =
                ScanResults.create(0, WifiScanner.WIFI_BAND_UNSPECIFIED, 2400, 5150, 5175);

        when(mWifiScannerImpl.startSingleScan(any(WifiNative.ScanSettings.class),
                any(WifiNative.ScanEventHandler.class))).thenReturn(true);

        sendSingleScanRequest(controlChannel, requestId, requestSettings, workSource);

        mLooper.dispatchAll();
        WifiNative.ScanEventHandler eventHandler =
                verifyStartSingleScan(order, computeSingleScanNativeSettings(requestSettings));
        verifySuccessfulResponse(order, handler, requestId);
        verify(mBatteryStats).noteWifiScanStartedFromSource(eq(workSource));

        when(mWifiScannerImpl.getLatestSingleScanResults())
                .thenReturn(results.getRawScanData());
        eventHandler.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);

        mLooper.dispatchAll();
        verifyScanResultsReceived(order, handler, requestId, results.getScanData());
        verifySingleScanCompletedReceived(order, handler, requestId);
        verifyNoMoreInteractions(handler);
        verify(mBatteryStats).noteWifiScanStoppedFromSource(eq(workSource));
        assertDumpContainsRequestLog("addSingleScanRequest", requestId);
        assertDumpContainsCallbackLog("singleScanResults", requestId,
                "results=" + results.getScanData().getResults().length);
    }

    /**
     * Verifies that pno scan works after duplicate scan enable.
     */
    @Test
    public void hwPnoScanScanAfterDuplicateScanEnable() throws Exception {
        startServiceAndLoadDriver();

        // Send scan enable again.
        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);
        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_ENABLE));
        mLooper.dispatchAll();

        // Ensure we didn't create scanner instance twice.
        verify(mWifiScannerImplFactory, times(1))
                .create(any(), any(), any());

        InOrder order = inOrder(handler, mWifiScannerImpl);
        int requestId = 12;

        ScanResults scanResults = createScanResultsForPno();
        Pair<WifiScanner.ScanSettings, WifiNative.ScanSettings> scanSettings =
                createScanSettingsForHwPno();
        Pair<WifiScanner.PnoSettings, WifiNative.PnoSettings> pnoSettings =
                createPnoSettings(scanResults);

        sendPnoScanRequest(controlChannel, requestId, scanSettings.first, pnoSettings.first);
        expectHwPnoScan(order, handler, requestId, pnoSettings.second, scanResults);
        verifyPnoNetworkFoundReceived(order, handler, requestId, scanResults.getRawScanResults());
    }

    /**
     * Verifies that only clients with NETWORK_STACK permission can issues restricted messages
     * (from API's).
     */
    @Test
    public void rejectRestrictedMessagesFromNonPrivilegedApps() throws Exception {
        mWifiScanningServiceImpl.startService();
        mWifiScanningServiceImpl.setWifiHandlerLogForTest(mLog);
        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);

        // Client doesn't have NETWORK_STACK permission.
        doThrow(new SecurityException()).when(mContext).enforcePermission(
                eq(Manifest.permission.NETWORK_STACK), anyInt(), eq(Binder.getCallingUid()), any());

        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_ENABLE));
        mLooper.dispatchAll();

        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_DISABLE));
        mLooper.dispatchAll();

        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_START_PNO_SCAN));
        mLooper.dispatchAll();

        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_STOP_PNO_SCAN));
        mLooper.dispatchAll();

        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_REGISTER_SCAN_LISTENER));
        mLooper.dispatchAll();

        // All 4 of the above messages should have been rejected because the app doesn't have
        // the required permissions.
        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(handler, times(5)).handleMessage(messageCaptor.capture());
        assertFailedResponse(0, WifiScanner.REASON_NOT_AUTHORIZED,
                "Not authorized", messageCaptor.getAllValues().get(0));
        assertFailedResponse(0, WifiScanner.REASON_NOT_AUTHORIZED,
                "Not authorized", messageCaptor.getAllValues().get(1));
        assertFailedResponse(0, WifiScanner.REASON_NOT_AUTHORIZED,
                "Not authorized", messageCaptor.getAllValues().get(2));
        assertFailedResponse(0, WifiScanner.REASON_NOT_AUTHORIZED,
                "Not authorized", messageCaptor.getAllValues().get(3));
        assertFailedResponse(0, WifiScanner.REASON_NOT_AUTHORIZED,
                "Not authorized", messageCaptor.getAllValues().get(4));

        // Ensure we didn't create scanner instance.
        verify(mWifiScannerImplFactory, never()).create(any(), any(), any());

    }

    /**
     * Verifies that clients without NETWORK_STACK permission cannot issue any messages when they
     * don't have the necessary location permissions & location is enabled.
     */
    @Test
    public void rejectAllMessagesFromNonPrivilegedAppsWithoutLocationPermission() throws Exception {
        // Start service & initialize it.
        startServiceAndLoadDriver();
        // Location permission or mode check fail.
        doThrow(new SecurityException()).when(mWifiPermissionsUtil)
                .enforceCanAccessScanResultsForWifiScanner(any(), eq(Binder.getCallingUid()),
                        eq(false), eq(false));

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);

        // Client doesn't have NETWORK_STACK permission.
        doThrow(new SecurityException()).when(mContext).enforcePermission(
                eq(Manifest.permission.NETWORK_STACK), anyInt(), eq(Binder.getCallingUid()), any());

        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_START_SINGLE_SCAN));
        mLooper.dispatchAll();

        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_GET_SCAN_RESULTS));
        mLooper.dispatchAll();

        controlChannel.sendMessage(Message.obtain(null, WifiScanner.CMD_START_BACKGROUND_SCAN));
        mLooper.dispatchAll();

        // All the above messages should have been rejected because the app doesn't have
        // the privileged permissions & location is turned off.
        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(handler, times(3)).handleMessage(messageCaptor.capture());
        assertFailedResponse(0, WifiScanner.REASON_NOT_AUTHORIZED,
                "Not authorized", messageCaptor.getAllValues().get(0));
        assertFailedResponse(0, WifiScanner.REASON_NOT_AUTHORIZED,
                "Not authorized", messageCaptor.getAllValues().get(1));
        assertFailedResponse(0, WifiScanner.REASON_NOT_AUTHORIZED,
                "Not authorized", messageCaptor.getAllValues().get(2));

        // Validate the initialization sequence.
        verify(mWifiScannerImpl).getChannelHelper();
        verify(mWifiScannerImpl).getScanCapabilities(any());

        // Ensure we didn't start any scans after.
        verifyNoMoreInteractions(mWifiScannerImpl);
    }

    /**
     * Verifies that we ignore location settings when the single scan request settings sets
     * {@link WifiScanner.ScanSettings#ignoreLocationSettings}
     */
    @Test
    public void verifyIgnoreLocationSettingsFromNonPrivilegedAppsForSingleScan() throws Exception {
        // Start service & initialize it.
        startServiceAndLoadDriver();

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);

        // Client doesn't have NETWORK_STACK permission.
        doThrow(new SecurityException()).when(mContext).enforcePermission(
                eq(Manifest.permission.NETWORK_STACK), anyInt(), eq(Binder.getCallingUid()), any());

        Bundle bundle = new Bundle();
        bundle.putString(WifiScanner.REQUEST_PACKAGE_NAME_KEY, TEST_PACKAGE_NAME);
        WifiScanner.ScanSettings scanSettings = new WifiScanner.ScanSettings();

        // send single scan request (ignoreLocationSettings == true).
        scanSettings.ignoreLocationSettings = true;
        bundle.putParcelable(WifiScanner.SCAN_PARAMS_SCAN_SETTINGS_KEY, scanSettings);
        Message message = Message.obtain();
        message.what = WifiScanner.CMD_START_SINGLE_SCAN;
        message.obj = bundle;
        controlChannel.sendMessage(message);
        mLooper.dispatchAll();

        // Verify the permission check params (ignoreLocationSettings == true).
        verify(mWifiPermissionsUtil).enforceCanAccessScanResultsForWifiScanner(
                eq(TEST_PACKAGE_NAME), eq(Binder.getCallingUid()), eq(true), eq(false));

        // send single scan request (ignoreLocationSettings == false).
        scanSettings.ignoreLocationSettings = false;
        bundle.putParcelable(WifiScanner.SCAN_PARAMS_SCAN_SETTINGS_KEY, scanSettings);
        message = Message.obtain();
        message.what = WifiScanner.CMD_START_SINGLE_SCAN;
        message.obj = bundle;
        controlChannel.sendMessage(message);
        mLooper.dispatchAll();

        // Verify the permission check params (ignoreLocationSettings == true).
        verify(mWifiPermissionsUtil).enforceCanAccessScanResultsForWifiScanner(
                eq(TEST_PACKAGE_NAME), eq(Binder.getCallingUid()), eq(false), eq(false));

        // send background scan request (ignoreLocationSettings == true).
        scanSettings.ignoreLocationSettings = true;
        bundle.putParcelable(WifiScanner.SCAN_PARAMS_SCAN_SETTINGS_KEY, scanSettings);
        message = Message.obtain();
        message.what = WifiScanner.CMD_START_BACKGROUND_SCAN;
        controlChannel.sendMessage(message);
        mLooper.dispatchAll();

        // Verify the permission check params (ignoreLocationSettings == false), the field
        // is ignored for any requests other than single scan.
        verify(mWifiPermissionsUtil).enforceCanAccessScanResultsForWifiScanner(
                eq(TEST_PACKAGE_NAME), eq(Binder.getCallingUid()), eq(false), eq(false));
    }

    /**
     * Verifies that we hide from app-ops when the single scan request settings sets
     * {@link WifiScanner.ScanSettings#hideFromAppOps}
     */
    @Test
    public void verifyHideFromAppOpsFromNonPrivilegedAppsForSingleScan() throws Exception {
        // Start service & initialize it.
        startServiceAndLoadDriver();

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);

        // Client doesn't have NETWORK_STACK permission.
        doThrow(new SecurityException()).when(mContext).enforcePermission(
                eq(Manifest.permission.NETWORK_STACK), anyInt(), eq(Binder.getCallingUid()), any());

        Bundle bundle = new Bundle();
        bundle.putString(WifiScanner.REQUEST_PACKAGE_NAME_KEY, TEST_PACKAGE_NAME);
        WifiScanner.ScanSettings scanSettings = new WifiScanner.ScanSettings();

        // send single scan request (hideFromAppOps == true).
        scanSettings.hideFromAppOps = true;
        bundle.putParcelable(WifiScanner.SCAN_PARAMS_SCAN_SETTINGS_KEY, scanSettings);
        Message message = Message.obtain();
        message.what = WifiScanner.CMD_START_SINGLE_SCAN;
        message.obj = bundle;
        controlChannel.sendMessage(message);
        mLooper.dispatchAll();

        // Verify the permission check params (hideFromAppOps == true).
        verify(mWifiPermissionsUtil).enforceCanAccessScanResultsForWifiScanner(
                eq(TEST_PACKAGE_NAME), eq(Binder.getCallingUid()), eq(false), eq(true));

        // send single scan request (hideFromAppOps == false).
        scanSettings.hideFromAppOps = false;
        bundle.putParcelable(WifiScanner.SCAN_PARAMS_SCAN_SETTINGS_KEY, scanSettings);
        message = Message.obtain();
        message.what = WifiScanner.CMD_START_SINGLE_SCAN;
        message.obj = bundle;
        controlChannel.sendMessage(message);
        mLooper.dispatchAll();

        // Verify the permission check params (hideFromAppOps == false).
        verify(mWifiPermissionsUtil).enforceCanAccessScanResultsForWifiScanner(
                eq(TEST_PACKAGE_NAME), eq(Binder.getCallingUid()), eq(false), eq(false));

        // send background scan request (hideFromAppOps == true).
        scanSettings.hideFromAppOps = true;
        bundle.putParcelable(WifiScanner.SCAN_PARAMS_SCAN_SETTINGS_KEY, scanSettings);
        message = Message.obtain();
        message.what = WifiScanner.CMD_START_BACKGROUND_SCAN;
        controlChannel.sendMessage(message);
        mLooper.dispatchAll();

        // Verify the permission check params (hideFromAppOps == false), the field
        // is ignored for any requests other than single scan.
        verify(mWifiPermissionsUtil).enforceCanAccessScanResultsForWifiScanner(
                eq(TEST_PACKAGE_NAME), eq(Binder.getCallingUid()), eq(false), eq(false));
    }

    /**
     * Verifies that we don't invoke {@link WifiPermissionsUtil#
     * enforceCanAccessScanResultsForWifiScanner(String, int, boolean, boolean)} for requests
     * from privileged clients (i.e wifi service).
     */
    @Test
    public void verifyLocationPermissionCheckIsSkippedFromPrivilegedClientsForSingleScan()
            throws Exception {
        // Start service & initialize it.
        startServiceAndLoadDriver();

        Handler handler = mock(Handler.class);
        BidirectionalAsyncChannel controlChannel = connectChannel(handler);

        // Client does have NETWORK_STACK permission.
        doNothing().when(mContext).enforcePermission(
                eq(Manifest.permission.NETWORK_STACK), anyInt(), eq(Binder.getCallingUid()), any());

        Bundle bundle = new Bundle();
        bundle.putString(WifiScanner.REQUEST_PACKAGE_NAME_KEY, TEST_PACKAGE_NAME);
        WifiScanner.ScanSettings scanSettings = new WifiScanner.ScanSettings();

        // send single scan request (hideFromAppOps == true, ignoreLocationSettings = true).
        scanSettings.hideFromAppOps = true;
        scanSettings.ignoreLocationSettings = true;
        bundle.putParcelable(WifiScanner.SCAN_PARAMS_SCAN_SETTINGS_KEY, scanSettings);
        Message message = Message.obtain();
        message.what = WifiScanner.CMD_START_SINGLE_SCAN;
        message.obj = bundle;
        controlChannel.sendMessage(message);
        mLooper.dispatchAll();

        // Verify that we didn't invoke the location permission check.
        verify(mWifiPermissionsUtil, never()).enforceCanAccessScanResultsForWifiScanner(
                eq(TEST_PACKAGE_NAME), eq(Binder.getCallingUid()), anyBoolean(), anyBoolean());
    }
}
