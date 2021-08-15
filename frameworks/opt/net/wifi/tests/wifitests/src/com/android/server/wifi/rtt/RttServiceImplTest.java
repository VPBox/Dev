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


package com.android.server.wifi.rtt;

import static com.android.server.wifi.rtt.RttTestUtils.compareListContentsNoOrdering;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyBoolean;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.anyLong;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.atLeastOnce;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.inOrder;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.when;

import android.app.ActivityManager;
import android.app.AlarmManager;
import android.app.test.MockAnswerUtil;
import android.app.test.TestAlarmManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.hardware.wifi.V1_0.RttResult;
import android.location.LocationManager;
import android.net.MacAddress;
import android.net.wifi.aware.IWifiAwareMacAddressProvider;
import android.net.wifi.aware.IWifiAwareManager;
import android.net.wifi.aware.PeerHandle;
import android.net.wifi.rtt.IRttCallback;
import android.net.wifi.rtt.RangingRequest;
import android.net.wifi.rtt.RangingResult;
import android.net.wifi.rtt.RangingResultCallback;
import android.net.wifi.rtt.ResponderConfig;
import android.net.wifi.rtt.WifiRttManager;
import android.os.Handler;
import android.os.IBinder;
import android.os.IPowerManager;
import android.os.PowerManager;
import android.os.RemoteException;
import android.os.UserHandle;
import android.os.WorkSource;
import android.os.test.TestLooper;
import android.provider.Settings;
import android.util.Pair;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.Clock;
import com.android.server.wifi.FrameworkFacade;
import com.android.server.wifi.nano.WifiMetricsProto;
import com.android.server.wifi.util.WifiPermissionsUtil;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.InOrder;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;


/**
 * Unit test harness for the RttServiceImpl class.
 */
@SmallTest
public class RttServiceImplTest {

    private static final long BACKGROUND_PROCESS_EXEC_GAP_MS = 10 * 60 * 1000;  // 10 minutes.

    private RttServiceImplSpy mDut;
    private TestLooper mMockLooper;
    private TestAlarmManager mAlarmManager;
    private PowerManager mMockPowerManager;
    private BroadcastReceiver mPowerBcastReceiver;
    private BroadcastReceiver mLocationModeReceiver;

    private final String mPackageName = "some.package.name.for.rtt.app";
    private int mDefaultUid = 1500;
    private WorkSource mDefaultWs = new WorkSource(mDefaultUid);

    private ArgumentCaptor<Integer> mIntCaptor = ArgumentCaptor.forClass(Integer.class);
    private ArgumentCaptor<IBinder.DeathRecipient> mDeathRecipientCaptor = ArgumentCaptor
            .forClass(IBinder.DeathRecipient.class);
    private ArgumentCaptor<RangingRequest> mRequestCaptor = ArgumentCaptor.forClass(
            RangingRequest.class);
    private ArgumentCaptor<List> mListCaptor = ArgumentCaptor.forClass(List.class);

    private BinderLinkToDeathAnswer mBinderLinkToDeathCounter = new BinderLinkToDeathAnswer();
    private BinderUnlinkToDeathAnswer mBinderUnlinkToDeathCounter = new BinderUnlinkToDeathAnswer();

    private InOrder mInOrder;

    @Mock
    public Context mockContext;

    @Mock
    public ActivityManager mockActivityManager;

    @Mock
    public Clock mockClock;

    @Mock
    public RttNative mockNative;

    @Mock
    public RttMetrics mockMetrics;

    @Mock
    public IWifiAwareManager mockAwareManagerBinder;

    @Mock
    public WifiPermissionsUtil mockPermissionUtil;

    @Mock
    public IBinder mockIbinder;

    @Mock
    public IRttCallback mockCallback;

    @Mock
    FrameworkFacade mFrameworkFacade;

    /**
     * Using instead of spy to avoid native crash failures - possibly due to
     * spy's copying of state.
     */
    private class RttServiceImplSpy extends RttServiceImpl {
        public int fakeUid;

        RttServiceImplSpy(Context context) {
            super(context);
        }

        /**
         * Return the fake UID instead of the real one: pseudo-spy
         * implementation.
         */
        @Override
        public int getMockableCallingUid() {
            return fakeUid;
        }
    }

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);

        mDut = new RttServiceImplSpy(mockContext);
        mDut.fakeUid = mDefaultUid;
        mMockLooper = new TestLooper();

        when(mockContext.checkCallingOrSelfPermission(
                android.Manifest.permission.LOCATION_HARDWARE)).thenReturn(
                PackageManager.PERMISSION_GRANTED);

        mAlarmManager = new TestAlarmManager();
        doNothing().when(mFrameworkFacade).registerContentObserver(eq(mockContext), any(),
                anyBoolean(), any());
        when(mFrameworkFacade.getLongSetting(
            eq(mockContext),
            eq(Settings.Global.WIFI_RTT_BACKGROUND_EXEC_GAP_MS),
            anyLong()))
            .thenReturn(BACKGROUND_PROCESS_EXEC_GAP_MS);
        when(mockContext.getSystemService(Context.ALARM_SERVICE))
                .thenReturn(mAlarmManager.getAlarmManager());
        mInOrder = inOrder(mAlarmManager.getAlarmManager(), mockContext);

        when(mockContext.getSystemService(Context.ACTIVITY_SERVICE)).thenReturn(
                mockActivityManager);
        when(mockActivityManager.getUidImportance(anyInt())).thenReturn(
                ActivityManager.RunningAppProcessInfo.IMPORTANCE_FOREGROUND_SERVICE);

        when(mockPermissionUtil.checkCallersLocationPermission(eq(mPackageName),
                anyInt(), anyBoolean())).thenReturn(true);
        when(mockPermissionUtil.isLocationModeEnabled()).thenReturn(true);
        when(mockNative.isReady()).thenReturn(true);
        when(mockNative.rangeRequest(anyInt(), any(RangingRequest.class), anyBoolean())).thenReturn(
                true);

        mMockPowerManager = new PowerManager(mockContext, mock(IPowerManager.class),
                new Handler(mMockLooper.getLooper()));
        when(mMockPowerManager.isDeviceIdleMode()).thenReturn(false);
        when(mockContext.getSystemServiceName(PowerManager.class)).thenReturn(
                Context.POWER_SERVICE);
        when(mockContext.getSystemService(PowerManager.class)).thenReturn(mMockPowerManager);

        doAnswer(mBinderLinkToDeathCounter).when(mockIbinder).linkToDeath(any(), anyInt());
        doAnswer(mBinderUnlinkToDeathCounter).when(mockIbinder).unlinkToDeath(any(), anyInt());

        mDut.start(mMockLooper.getLooper(), mockClock, mockAwareManagerBinder, mockNative,
                mockMetrics, mockPermissionUtil, mFrameworkFacade);
        mMockLooper.dispatchAll();
        ArgumentCaptor<BroadcastReceiver> bcastRxCaptor = ArgumentCaptor.forClass(
                BroadcastReceiver.class);
        verify(mockContext, times(2)).registerReceiver(bcastRxCaptor.capture(),
                any(IntentFilter.class));
        verify(mockNative).start(any());
        mPowerBcastReceiver = bcastRxCaptor.getAllValues().get(0);
        mLocationModeReceiver = bcastRxCaptor.getAllValues().get(1);

        assertTrue(mDut.isAvailable());
    }

    @After
    public void tearDown() throws Exception {
        assertEquals("Binder links != unlinks to death (size)",
                mBinderLinkToDeathCounter.mUniqueExecs.size(),
                mBinderUnlinkToDeathCounter.mUniqueExecs.size());
        assertEquals("Binder links != unlinks to death", mBinderLinkToDeathCounter.mUniqueExecs,
                mBinderUnlinkToDeathCounter.mUniqueExecs);
    }

    /**
     * Validate successful ranging flow.
     */
    @Test
    public void testRangingFlow() throws Exception {
        int numIter = 10;
        RangingRequest[] requests = new RangingRequest[numIter];
        List<Pair<List<RttResult>, List<RangingResult>>> results = new ArrayList<>();

        for (int i = 0; i < numIter; ++i) { // even: MC, non-MC, Aware, odd: MC only
            if (i % 2 == 0) {
                requests[i] = RttTestUtils.getDummyRangingRequestMcOnly((byte) i);
            } else {
                requests[i] = RttTestUtils.getDummyRangingRequest((byte) i);
            }
            results.add(RttTestUtils.getDummyRangingResults(requests[i]));
        }

        // (1) request 10 ranging operations
        for (int i = 0; i < numIter; ++i) {
            mDut.startRanging(mockIbinder, mPackageName, null, requests[i], mockCallback);
        }
        mMockLooper.dispatchAll();

        for (int i = 0; i < numIter; ++i) {
            // (2) verify that request issued to native
            verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(requests[i]), eq(true));
            verifyWakeupSet(i % 2 != 0, 0);

            // (3) native calls back with result
            mDut.onRangingResults(mIntCaptor.getValue(), results.get(i).first);
            mMockLooper.dispatchAll();

            // (4) verify that results dispatched
            verify(mockCallback).onRangingResults(results.get(i).second);
            verifyWakeupCancelled();

            // (5) replicate results - shouldn't dispatch another callback
            mDut.onRangingResults(mIntCaptor.getValue(), results.get(i).first);
            mMockLooper.dispatchAll();
        }

        // verify metrics
        for (int i = 0; i < numIter; ++i) {
            verify(mockMetrics).recordRequest(eq(mDefaultWs), eq(requests[i]));
            verify(mockMetrics).recordResult(eq(requests[i]), eq(results.get(i).first));
        }
        verify(mockMetrics, times(numIter)).recordOverallStatus(
                WifiMetricsProto.WifiRttLog.OVERALL_SUCCESS);

        verify(mockNative, atLeastOnce()).isReady();
        verifyNoMoreInteractions(mockNative, mockMetrics, mockCallback,
                mAlarmManager.getAlarmManager());
    }

    /**
     * Validate a successful ranging flow with PeerHandles (i.e. verify translations)
     */
    @Test
    public void testRangingFlowUsingAwarePeerHandles() throws Exception {
        RangingRequest request = RttTestUtils.getDummyRangingRequest((byte) 0xA);
        PeerHandle peerHandle1 = new PeerHandle(1022);
        PeerHandle peerHandle2 = new PeerHandle(1023);
        PeerHandle peerHandle3 = new PeerHandle(1024);
        request.mRttPeers.add(ResponderConfig.fromWifiAwarePeerHandleWithDefaults(peerHandle1));
        request.mRttPeers.add(ResponderConfig.fromWifiAwarePeerHandleWithDefaults(peerHandle2));
        request.mRttPeers.add(ResponderConfig.fromWifiAwarePeerHandleWithDefaults(peerHandle3));
        Map<Integer, MacAddress> peerHandleToMacMap = new HashMap<>();
        MacAddress macAwarePeer1 = MacAddress.fromString("AA:BB:CC:DD:EE:FF");
        MacAddress macAwarePeer2 = MacAddress.fromString("BB:BB:BB:EE:EE:EE");
        peerHandleToMacMap.put(peerHandle1.peerId, macAwarePeer1);
        peerHandleToMacMap.put(peerHandle2.peerId, macAwarePeer2);
        peerHandleToMacMap.put(peerHandle3.peerId, null); // bad answer from Aware (expired?)

        AwareTranslatePeerHandlesToMac answer = new AwareTranslatePeerHandlesToMac(mDefaultUid,
                peerHandleToMacMap);
        doAnswer(answer).when(mockAwareManagerBinder).requestMacAddresses(anyInt(), any(), any());

        // issue request
        mDut.startRanging(mockIbinder, mPackageName, null, request, mockCallback);
        mMockLooper.dispatchAll();

        // verify that requested with MAC address translated from the PeerHandle issued to Native
        verify(mockNative).rangeRequest(mIntCaptor.capture(), mRequestCaptor.capture(), eq(true));
        verifyWakeupSet(true, 0);

        RangingRequest finalRequest = mRequestCaptor.getValue();
        assertNotEquals("Request to native is not null", null, finalRequest);
        assertEquals("Size of request", request.mRttPeers.size() - 1,
                finalRequest.mRttPeers.size());
        assertEquals("Aware peer 1 MAC", macAwarePeer1,
                finalRequest.mRttPeers.get(finalRequest.mRttPeers.size() - 2).macAddress);
        assertEquals("Aware peer 2 MAC", macAwarePeer2,
                finalRequest.mRttPeers.get(finalRequest.mRttPeers.size() - 1).macAddress);

        // issue results - but remove the one for peer #2
        Pair<List<RttResult>, List<RangingResult>> results =
                RttTestUtils.getDummyRangingResults(mRequestCaptor.getValue());
        results.first.remove(results.first.size() - 1);
        RangingResult removed = results.second.remove(results.second.size() - 1);
        results.second.add(
                new RangingResult(RangingResult.STATUS_FAIL, removed.getPeerHandle(), 0, 0, 0, 0, 0,
                        null, null, null, 0));
        mDut.onRangingResults(mIntCaptor.getValue(), results.first);
        mMockLooper.dispatchAll();

        // verify that results with MAC addresses filtered out and replaced by PeerHandles issued
        // to callback
        verify(mockCallback).onRangingResults(mListCaptor.capture());
        verifyWakeupCancelled();

        assertTrue(compareListContentsNoOrdering(results.second, mListCaptor.getValue()));

        // verify metrics
        verify(mockMetrics).recordRequest(eq(mDefaultWs), eq(request));
        verify(mockMetrics).recordResult(eq(finalRequest), eq(results.first));
        verify(mockMetrics).recordOverallStatus(WifiMetricsProto.WifiRttLog.OVERALL_SUCCESS);

        verify(mockNative, atLeastOnce()).isReady();
        verifyNoMoreInteractions(mockNative, mockMetrics, mockCallback,
                mAlarmManager.getAlarmManager());
    }

    /**
     * Validate failed ranging flow (native failure).
     */
    @Test
    public void testRangingFlowNativeFailure() throws Exception {
        int numIter = 10;
        RangingRequest[] requests = new RangingRequest[numIter];
        List<Pair<List<RttResult>, List<RangingResult>>> results = new ArrayList<>();

        for (int i = 0; i < numIter; ++i) {
            requests[i] = RttTestUtils.getDummyRangingRequest((byte) i);
            results.add(RttTestUtils.getDummyRangingResults(requests[i]));
        }

        // (1) request 10 ranging operations: fail the first one
        when(mockNative.rangeRequest(anyInt(), any(RangingRequest.class), anyBoolean())).thenReturn(
                false);
        mDut.startRanging(mockIbinder, mPackageName, null, requests[0], mockCallback);
        mMockLooper.dispatchAll();

        when(mockNative.rangeRequest(anyInt(), any(RangingRequest.class), anyBoolean())).thenReturn(
                true);
        for (int i = 1; i < numIter; ++i) {
            mDut.startRanging(mockIbinder, mPackageName, null, requests[i], mockCallback);
        }
        mMockLooper.dispatchAll();

        for (int i = 0; i < numIter; ++i) {
            // (2) verify that request issued to native
            verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(requests[i]), eq(true));

            // (3) verify that failure callback dispatched (for the HAL failure)
            if (i == 0) {
                verify(mockCallback).onRangingFailure(RangingResultCallback.STATUS_CODE_FAIL);
            } else {
                verifyWakeupSet(true, 0);
            }

            // (4) on failed HAL: even if native calls back with result we shouldn't dispatch
            // callback, otherwise expect result
            mDut.onRangingResults(mIntCaptor.getValue(), results.get(i).first);
            mMockLooper.dispatchAll();

            if (i != 0) {
                verify(mockCallback).onRangingResults(results.get(i).second);
                verifyWakeupCancelled();
            }
        }

        // verify metrics
        for (int i = 0; i < numIter; ++i) {
            verify(mockMetrics).recordRequest(eq(mDefaultWs), eq(requests[i]));
            if (i != 0) {
                verify(mockMetrics).recordResult(eq(requests[i]), eq(results.get(i).first));
            }
        }
        verify(mockMetrics).recordOverallStatus(WifiMetricsProto.WifiRttLog.OVERALL_HAL_FAILURE);
        verify(mockMetrics, times(numIter - 1)).recordOverallStatus(
                WifiMetricsProto.WifiRttLog.OVERALL_SUCCESS);

        verify(mockNative, atLeastOnce()).isReady();
        verifyNoMoreInteractions(mockNative, mockMetrics, mockCallback,
                mAlarmManager.getAlarmManager());
    }

    /**
     * Validate a ranging flow for an app whose LOCATION runtime permission is revoked.
     */
    @Test
    public void testRangingRequestWithoutRuntimePermission() throws Exception {
        RangingRequest request = RttTestUtils.getDummyRangingRequest((byte) 0);
        Pair<List<RttResult>, List<RangingResult>> results = RttTestUtils.getDummyRangingResults(
                request);

        // (1) request ranging operation
        mDut.startRanging(mockIbinder, mPackageName, null, request, mockCallback);
        mMockLooper.dispatchAll();

        // (2) verify that request issued to native
        verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(request), eq(true));
        verifyWakeupSet(true, 0);

        // (3) native calls back with result - should get a FAILED callback
        when(mockPermissionUtil.checkCallersLocationPermission(eq(mPackageName),
                anyInt(), anyBoolean())).thenReturn(false);

        mDut.onRangingResults(mIntCaptor.getValue(), results.first);
        mMockLooper.dispatchAll();

        verify(mockCallback).onRangingFailure(eq(RangingResultCallback.STATUS_CODE_FAIL));
        verifyWakeupCancelled();

        // verify metrics
        verify(mockMetrics).recordRequest(eq(mDefaultWs), eq(request));
        verify(mockMetrics).recordOverallStatus(
                WifiMetricsProto.WifiRttLog.OVERALL_LOCATION_PERMISSION_MISSING);

        verify(mockNative, atLeastOnce()).isReady();
        verifyNoMoreInteractions(mockNative, mockMetrics, mockCallback,
                mAlarmManager.getAlarmManager());
    }

    /**
     * Validate that the ranging app's binder death clears record of request - no callbacks are
     * attempted.
     */
    @Test
    public void testBinderDeathOfRangingApp() throws Exception {
        int numIter = 10;
        RangingRequest[] requests = new RangingRequest[numIter];
        List<Pair<List<RttResult>, List<RangingResult>>> results = new ArrayList<>();

        for (int i = 0; i < numIter; ++i) {
            requests[i] = RttTestUtils.getDummyRangingRequest((byte) i);
            results.add(RttTestUtils.getDummyRangingResults(requests[i]));
        }

        // (1) request 10 ranging operations: even/odd with different UIDs
        for (int i = 0; i < numIter; ++i) {
            mDut.fakeUid = mDefaultUid + i % 2;
            mDut.startRanging(mockIbinder, mPackageName, null, requests[i], mockCallback);
        }
        mMockLooper.dispatchAll();

        // (2) capture death listeners
        verify(mockIbinder, times(numIter)).linkToDeath(mDeathRecipientCaptor.capture(), anyInt());

        for (int i = 0; i < numIter; ++i) {
            // (3) verify first request and all odd requests issued to HAL
            if (i == 0 || i % 2 == 1) {
                verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(requests[i]), eq(true));
                verifyWakeupSet(true, 0);
            }

            // (4) trigger first death recipient (which will map to the even UID)
            if (i == 0) {
                mDeathRecipientCaptor.getAllValues().get(0).binderDied();
                mMockLooper.dispatchAll();

                verify(mockNative).rangeCancel(eq(mIntCaptor.getValue()),
                        (ArrayList) mListCaptor.capture());
                RangingRequest request0 = requests[0];
                assertEquals(request0.mRttPeers.size(), mListCaptor.getValue().size());
                assertArrayEquals(MacAddress.fromString("00:01:02:03:04:00").toByteArray(),
                        (byte[]) mListCaptor.getValue().get(0));
                assertArrayEquals(MacAddress.fromString("0A:0B:0C:0D:0E:00").toByteArray(),
                        (byte[]) mListCaptor.getValue().get(1));
                assertArrayEquals(MacAddress.fromString("08:09:08:07:06:05").toByteArray(),
                        (byte[]) mListCaptor.getValue().get(2));
            }

            // (5) native calls back with all results - should get requests for the odd attempts and
            // should only get callbacks for the odd attempts (the non-dead UID), but this simulates
            // invalid results (or possibly the firmware not cancelling some requests)
            mDut.onRangingResults(mIntCaptor.getValue(), results.get(i).first);
            mMockLooper.dispatchAll();
            if (i == 0) {
                verifyWakeupCancelled(); // as the first (dispatched) request is aborted
            }
            if (i % 2 == 1) {
                verify(mockCallback).onRangingResults(results.get(i).second);
                verifyWakeupCancelled();
            }
        }

        // verify metrics
        WorkSource oddWs = new WorkSource(mDefaultUid + 1);
        for (int i = 0; i < numIter; ++i) {
            verify(mockMetrics).recordRequest(eq((i % 2) == 0 ? mDefaultWs : oddWs),
                    eq(requests[i]));
            if (i % 2 == 1) {
                verify(mockMetrics).recordResult(eq(requests[i]), eq(results.get(i).first));
            }
        }
        verify(mockMetrics, times(numIter / 2)).recordOverallStatus(
                WifiMetricsProto.WifiRttLog.OVERALL_SUCCESS);

        verify(mockNative, atLeastOnce()).isReady();
        verifyNoMoreInteractions(mockNative, mockMetrics, mockCallback,
                mAlarmManager.getAlarmManager());
    }

    /**
     * Validate that a ranging app which uses WorkSource and dies (binder death) results in the
     * request cleanup.
     */
    @Test
    public void testBinderDeathWithWorkSource() throws Exception {
        WorkSource ws = new WorkSource(100);

        RangingRequest request = RttTestUtils.getDummyRangingRequest((byte) 0);
        Pair<List<RttResult>, List<RangingResult>> results = RttTestUtils.getDummyRangingResults(
                request);

        // (1) request ranging operation
        mDut.startRanging(mockIbinder, mPackageName, ws, request, mockCallback);
        mMockLooper.dispatchAll();

        verify(mockIbinder).linkToDeath(mDeathRecipientCaptor.capture(), anyInt());
        verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(request), eq(true));
        verifyWakeupSet(true, 0);

        // (2) execute binder death
        mDeathRecipientCaptor.getValue().binderDied();
        mMockLooper.dispatchAll();

        verify(mockNative).rangeCancel(eq(mIntCaptor.getValue()), any());
        verifyWakeupCancelled();

        // (3) provide results back - should be ignored
        mDut.onRangingResults(mIntCaptor.getValue(), results.first);
        mMockLooper.dispatchAll();

        // verify metrics
        verify(mockMetrics).recordRequest(eq(ws), eq(request));

        verify(mockNative, atLeastOnce()).isReady();
        verifyNoMoreInteractions(mockNative, mockMetrics, mockCallback,
                mAlarmManager.getAlarmManager());
    }

    /**
     * Validate that when a cancelRanging is called, using the same work source specification as the
     * request, that the request is cancelled.
     */
    @Test
    public void testCancelRangingFullMatch() throws Exception {
        int uid1 = 10;
        int uid2 = 20;
        int uid3 = 30;
        int uid4 = 40;
        WorkSource worksourceRequest = new WorkSource(uid1);
        worksourceRequest.add(uid2);
        worksourceRequest.add(uid3);
        worksourceRequest.createWorkChain().addNode(uid4, "foo");
        WorkSource worksourceCancel = new WorkSource(uid2);
        worksourceCancel.add(uid3);
        worksourceCancel.add(uid1);
        worksourceCancel.createWorkChain().addNode(uid4, "foo");

        RangingRequest request = RttTestUtils.getDummyRangingRequest((byte) 0);
        Pair<List<RttResult>, List<RangingResult>> results = RttTestUtils.getDummyRangingResults(
                request);

        // (1) request ranging operation
        mDut.startRanging(mockIbinder, mPackageName, worksourceRequest, request, mockCallback);
        mMockLooper.dispatchAll();

        // (2) verify that request issued to native
        verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(request), eq(true));
        verifyWakeupSet(true, 0);

        // (3) cancel the request
        mDut.cancelRanging(worksourceCancel);
        mMockLooper.dispatchAll();

        verify(mockNative).rangeCancel(eq(mIntCaptor.getValue()), any());
        verifyWakeupCancelled();

        // (4) send results back from native
        mDut.onRangingResults(mIntCaptor.getValue(), results.first);
        mMockLooper.dispatchAll();

        // verify metrics
        verify(mockMetrics).recordRequest(eq(worksourceRequest), eq(request));

        verify(mockNative, atLeastOnce()).isReady();
        verifyNoMoreInteractions(mockNative, mockMetrics, mockCallback,
                mAlarmManager.getAlarmManager());
    }

    /**
     * Validate that when a cancelRanging is called - but specifies a subset of the WorkSource
     * uids then the ranging proceeds.
     */
    @Test
    public void testCancelRangingPartialMatch() throws Exception {
        int uid1 = 10;
        int uid2 = 20;
        int uid3 = 30;
        WorkSource worksourceRequest = new WorkSource(uid1);
        worksourceRequest.add(uid2);
        worksourceRequest.createWorkChain().addNode(uid3, null);
        WorkSource worksourceCancel = new WorkSource(uid1);
        worksourceCancel.add(uid2);

        RangingRequest request = RttTestUtils.getDummyRangingRequest((byte) 0);
        Pair<List<RttResult>, List<RangingResult>> results = RttTestUtils.getDummyRangingResults(
                request);

        // (1) request ranging operation
        mDut.startRanging(mockIbinder, mPackageName, worksourceRequest, request, mockCallback);
        mMockLooper.dispatchAll();

        // (2) verify that request issued to native
        verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(request), eq(true));
        verifyWakeupSet(true, 0);

        // (3) cancel the request
        mDut.cancelRanging(worksourceCancel);

        // (4) send results back from native
        mDut.onRangingResults(mIntCaptor.getValue(), results.first);
        mMockLooper.dispatchAll();

        verify(mockCallback).onRangingResults(results.second);
        verifyWakeupCancelled();

        // verify metrics
        verify(mockMetrics).recordRequest(eq(worksourceRequest), eq(request));
        verify(mockMetrics).recordResult(eq(request), eq(results.first));
        verify(mockMetrics).recordOverallStatus(WifiMetricsProto.WifiRttLog.OVERALL_SUCCESS);

        verify(mockNative, atLeastOnce()).isReady();
        verifyNoMoreInteractions(mockNative, mockMetrics, mockCallback,
                mAlarmManager.getAlarmManager());
    }

    /**
     * Validate that when an unexpected result is provided by the Native it is not propagated to
     * caller (unexpected = different command ID).
     */
    @Test
    public void testUnexpectedResult() throws Exception {
        RangingRequest request = RttTestUtils.getDummyRangingRequest((byte) 0);
        Pair<List<RttResult>, List<RangingResult>> results = RttTestUtils.getDummyRangingResults(
                request);

        // (1) request ranging operation
        mDut.startRanging(mockIbinder, mPackageName, null, request, mockCallback);
        mMockLooper.dispatchAll();

        // (2) verify that request issued to native
        verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(request), eq(true));
        verifyWakeupSet(true, 0);

        // (3) native calls back with result - but wrong ID
        mDut.onRangingResults(mIntCaptor.getValue() + 1,
                RttTestUtils.getDummyRangingResults(null).first);
        mMockLooper.dispatchAll();

        // (4) now send results with correct ID (different set of results to differentiate)
        mDut.onRangingResults(mIntCaptor.getValue(), results.first);
        mMockLooper.dispatchAll();

        // (5) verify that results dispatched
        verify(mockCallback).onRangingResults(results.second);
        verifyWakeupCancelled();

        // verify metrics
        verify(mockMetrics).recordRequest(eq(mDefaultWs), eq(request));
        verify(mockMetrics).recordResult(eq(request), eq(results.first));
        verify(mockMetrics).recordOverallStatus(WifiMetricsProto.WifiRttLog.OVERALL_SUCCESS);

        verify(mockNative, atLeastOnce()).isReady();
        verifyNoMoreInteractions(mockNative, mockMetrics, mockCallback,
                mAlarmManager.getAlarmManager());
    }

    /**
     * Validate that the HAL returns results with "missing" entries (i.e. some requests don't get
     * results) they are filled-in with FAILED results.
     */
    @Test
    public void testMissingResults() throws Exception {
        RangingRequest request = RttTestUtils.getDummyRangingRequest((byte) 0);
        Pair<List<RttResult>, List<RangingResult>> results = RttTestUtils.getDummyRangingResults(
                request);
        results.first.remove(2); // remove a direct AWARE request
        RangingResult removed = results.second.remove(2);
        results.second.add(
                new RangingResult(RangingResult.STATUS_FAIL, removed.getMacAddress(), 0, 0, 0, 0, 0,
                        null, null, null, 0));
        results.first.remove(0); // remove an AP request
        removed = results.second.remove(0);
        results.second.add(
                new RangingResult(RangingResult.STATUS_FAIL, removed.getMacAddress(), 0, 0, 0, 0, 0,
                        null, null, null, 0));

        // (1) request ranging operation
        mDut.startRanging(mockIbinder, mPackageName, null, request, mockCallback);
        mMockLooper.dispatchAll();

        // (2) verify that request issued to native
        verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(request), eq(true));
        verifyWakeupSet(true, 0);

        // (3) return results with missing entries
        mDut.onRangingResults(mIntCaptor.getValue(), results.first);
        mMockLooper.dispatchAll();

        // (5) verify that (full) results dispatched
        verify(mockCallback).onRangingResults(mListCaptor.capture());
        assertTrue(compareListContentsNoOrdering(results.second, mListCaptor.getValue()));
        verifyWakeupCancelled();

        // verify metrics
        verify(mockMetrics).recordRequest(eq(mDefaultWs), eq(request));
        verify(mockMetrics).recordResult(eq(request), eq(results.first));
        verify(mockMetrics).recordOverallStatus(WifiMetricsProto.WifiRttLog.OVERALL_SUCCESS);

        verify(mockNative, atLeastOnce()).isReady();
        verifyNoMoreInteractions(mockNative, mockMetrics, mockCallback,
                mAlarmManager.getAlarmManager());
    }

    /**
     * Validate that when HAL returns an empty result set (completely empty) - they are filled-in
     * with FAILED results.
     */
    @Test
    public void testMissingAllResults() throws Exception {
        RangingRequest request = RttTestUtils.getDummyRangingRequest((byte) 0);
        Pair<List<RttResult>, List<RangingResult>> results = RttTestUtils.getDummyRangingResults(
                request);
        List<RangingResult> allFailResults = new ArrayList<>();
        for (RangingResult result : results.second) {
            allFailResults.add(
                    new RangingResult(RangingResult.STATUS_FAIL, result.getMacAddress(), 0, 0, 0, 0,
                            0, null, null, null, 0));
        }

        // (1) request ranging operation
        mDut.startRanging(mockIbinder, mPackageName, null, request, mockCallback);
        mMockLooper.dispatchAll();

        // (2) verify that request issued to native
        verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(request), eq(true));
        verifyWakeupSet(true, 0);

        // (3) return results with ALL results missing
        mDut.onRangingResults(mIntCaptor.getValue(), new ArrayList<>());
        mMockLooper.dispatchAll();

        // (5) verify that (full) results dispatched
        verify(mockCallback).onRangingResults(mListCaptor.capture());
        assertTrue(compareListContentsNoOrdering(allFailResults, mListCaptor.getValue()));
        verifyWakeupCancelled();

        // verify metrics
        verify(mockMetrics).recordRequest(eq(mDefaultWs), eq(request));
        verify(mockMetrics).recordResult(eq(request), eq(new ArrayList<RttResult>()));
        verify(mockMetrics).recordOverallStatus(WifiMetricsProto.WifiRttLog.OVERALL_SUCCESS);

        verify(mockNative, atLeastOnce()).isReady();
        verifyNoMoreInteractions(mockNative, mockMetrics, mockCallback,
                mAlarmManager.getAlarmManager());
    }

    /**
     * Validate that when the HAL returns results with "missing" entries (i.e. some requests
     * don't get results) AND these correspond to peers which do not support 802.11mc AND the
     * request is from a non-privileged context: they are filled-in with FAILED results.
     */
    @Test
    public void testMissingResultsForNonSupportOf80211mc() throws Exception {
        RangingRequest request = RttTestUtils.getDummyRangingRequest((byte) 0);
        Pair<List<RttResult>, List<RangingResult>> results = RttTestUtils.getDummyRangingResults(
                request);
        results.first.remove(1); // remove the entry which doesn't support 802.11mc
        RangingResult removed = results.second.remove(1);
        results.second.add(
                new RangingResult(RangingResult.STATUS_RESPONDER_DOES_NOT_SUPPORT_IEEE80211MC,
                        removed.getMacAddress(), 0, 0, 0, 0, 0, null, null, null, 0));
        results.first.remove(
                0); // remove an AP request (i.e. test combo of missing for different reasons)
        removed = results.second.remove(0);
        results.second.add(
                new RangingResult(RangingResult.STATUS_FAIL, removed.getMacAddress(), 0, 0, 0, 0, 0,
                        null, null, null, 0));

        when(mockContext.checkCallingOrSelfPermission(
                android.Manifest.permission.LOCATION_HARDWARE)).thenReturn(
                PackageManager.PERMISSION_DENIED);

        // (1) request ranging operation
        mDut.startRanging(mockIbinder, mPackageName, null, request, mockCallback);
        mMockLooper.dispatchAll();

        // (2) verify that request issued to native
        verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(request), eq(false));
        verifyWakeupSet(true, 0);

        // (3) return results with missing entries
        mDut.onRangingResults(mIntCaptor.getValue(), results.first);
        mMockLooper.dispatchAll();

        // (5) verify that (full) results dispatched
        verify(mockCallback).onRangingResults(mListCaptor.capture());
        assertTrue(compareListContentsNoOrdering(results.second, mListCaptor.getValue()));
        verifyWakeupCancelled();

        // verify metrics
        verify(mockMetrics).recordRequest(eq(mDefaultWs), eq(request));
        verify(mockMetrics).recordResult(eq(request), eq(results.first));
        verify(mockMetrics).recordOverallStatus(WifiMetricsProto.WifiRttLog.OVERALL_SUCCESS);

        verify(mockNative, atLeastOnce()).isReady();
        verifyNoMoreInteractions(mockNative, mockMetrics, mockCallback,
                mAlarmManager.getAlarmManager());
    }

    /**
     * Validate that when the HAL times out we fail, clean-up the queue and move to the next
     * request.
     */
    @Test
    public void testRangingTimeout() throws Exception {
        RangingRequest request1 = RttTestUtils.getDummyRangingRequest((byte) 1);
        RangingRequest request2 = RttTestUtils.getDummyRangingRequest((byte) 2);
        Pair<List<RttResult>, List<RangingResult>> result1 = RttTestUtils.getDummyRangingResults(
                request1);
        Pair<List<RttResult>, List<RangingResult>> result2 = RttTestUtils.getDummyRangingResults(
                request2);

        // (1) request 2 ranging operation
        mDut.startRanging(mockIbinder, mPackageName, null, request1, mockCallback);
        mDut.startRanging(mockIbinder, mPackageName, null, request2, mockCallback);
        mMockLooper.dispatchAll();

        // verify that request 1 issued to native
        verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(request1), eq(true));
        int cmdId1 = mIntCaptor.getValue();
        verifyWakeupSet(true, 0);

        // (2) time-out
        mAlarmManager.dispatch(RttServiceImpl.HAL_RANGING_TIMEOUT_TAG);
        mMockLooper.dispatchAll();

        // verify that: failure callback + request 2 issued to native
        verify(mockNative).rangeCancel(eq(cmdId1), any());
        verify(mockCallback).onRangingFailure(RangingResultCallback.STATUS_CODE_FAIL);
        verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(request2), eq(true));
        verifyWakeupSet(true, 0);

        // (3) send both result 1 and result 2
        mDut.onRangingResults(cmdId1, result1.first);
        mDut.onRangingResults(mIntCaptor.getValue(), result2.first);
        mMockLooper.dispatchAll();

        // verify that only result 2 is forwarded to client
        verify(mockCallback).onRangingResults(result2.second);
        verifyWakeupCancelled();

        // verify metrics
        verify(mockMetrics).recordRequest(eq(mDefaultWs), eq(request1));
        verify(mockMetrics).recordRequest(eq(mDefaultWs), eq(request2));
        verify(mockMetrics).recordResult(eq(request2), eq(result2.first));
        verify(mockMetrics).recordOverallStatus(WifiMetricsProto.WifiRttLog.OVERALL_TIMEOUT);
        verify(mockMetrics).recordOverallStatus(WifiMetricsProto.WifiRttLog.OVERALL_SUCCESS);

        verify(mockNative, atLeastOnce()).isReady();
        verifyNoMoreInteractions(mockNative, mockMetrics, mockCallback,
                mAlarmManager.getAlarmManager());
    }

    /**
     * Validate that ranging requests from background apps are throttled. The sequence is:
     * - Time 1: Background request -> ok
     * - Time 2 = t1 + 0.5gap: Background request -> fail (throttled)
     * - Time 3 = t1 + 1.1gap: Background request -> ok
     * - Time 4 = t3 + small: Foreground request -> ok
     * - Time 5 = t4 + small: Background request -> fail (throttled)
     */
    @Test
    public void testRangingThrottleBackground() throws Exception {
        RangingRequest request1 = RttTestUtils.getDummyRangingRequest((byte) 1);
        RangingRequest request2 = RttTestUtils.getDummyRangingRequest((byte) 2);
        RangingRequest request3 = RttTestUtils.getDummyRangingRequest((byte) 3);
        RangingRequest request4 = RttTestUtils.getDummyRangingRequest((byte) 4);
        RangingRequest request5 = RttTestUtils.getDummyRangingRequest((byte) 5);

        Pair<List<RttResult>, List<RangingResult>> result1 = RttTestUtils.getDummyRangingResults(
                request1);
        Pair<List<RttResult>, List<RangingResult>> result3 = RttTestUtils.getDummyRangingResults(
                request3);
        Pair<List<RttResult>, List<RangingResult>> result4 = RttTestUtils.getDummyRangingResults(
                request4);

        InOrder cbInorder = inOrder(mockCallback);

        ClockAnswer clock = new ClockAnswer();
        doAnswer(clock).when(mockClock).getElapsedSinceBootMillis();
        when(mockActivityManager.getUidImportance(anyInt())).thenReturn(
                ActivityManager.RunningAppProcessInfo.IMPORTANCE_GONE); // far background

        // (1) issue a request at time t1: should be dispatched since first one!
        clock.time = 100;
        mDut.startRanging(mockIbinder, mPackageName, null, request1, mockCallback);
        mMockLooper.dispatchAll();

        verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(request1), eq(true));
        verifyWakeupSet(true, clock.time);

        // (1.1) get result
        mDut.onRangingResults(mIntCaptor.getValue(), result1.first);
        mMockLooper.dispatchAll();

        cbInorder.verify(mockCallback).onRangingResults(result1.second);
        verifyWakeupCancelled();

        // (2) issue a request at time t2 = t1 + 0.5 gap: should be rejected (throttled)
        clock.time = 100 + BACKGROUND_PROCESS_EXEC_GAP_MS / 2;
        mDut.startRanging(mockIbinder, mPackageName, null, request2, mockCallback);
        mMockLooper.dispatchAll();

        cbInorder.verify(mockCallback).onRangingFailure(RangingResultCallback.STATUS_CODE_FAIL);

        // (3) issue a request at time t3 = t1 + 1.1 gap: should be dispatched since enough time
        clock.time = 100 + BACKGROUND_PROCESS_EXEC_GAP_MS * 11 / 10;
        mDut.startRanging(mockIbinder, mPackageName, null, request3, mockCallback);
        mMockLooper.dispatchAll();

        verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(request3), eq(true));
        verifyWakeupSet(true, clock.time);

        // (3.1) get result
        mDut.onRangingResults(mIntCaptor.getValue(), result3.first);
        mMockLooper.dispatchAll();

        cbInorder.verify(mockCallback).onRangingResults(result3.second);
        verifyWakeupCancelled();

        // (4) issue a foreground request at t4 = t3 + small: should be dispatched (foreground!)
        when(mockActivityManager.getUidImportance(anyInt())).thenReturn(
                ActivityManager.RunningAppProcessInfo.IMPORTANCE_FOREGROUND);

        clock.time = clock.time + 5;
        mDut.startRanging(mockIbinder, mPackageName, null, request4, mockCallback);
        mMockLooper.dispatchAll();

        verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(request4), eq(true));
        verifyWakeupSet(true, clock.time);

        // (4.1) get result
        mDut.onRangingResults(mIntCaptor.getValue(), result4.first);
        mMockLooper.dispatchAll();

        cbInorder.verify(mockCallback).onRangingResults(result4.second);
        verifyWakeupCancelled();

        // (5) issue a background request at t5 = t4 + small: should be rejected (throttled)
        when(mockActivityManager.getUidImportance(anyInt())).thenReturn(
                ActivityManager.RunningAppProcessInfo.IMPORTANCE_GONE);

        clock.time = clock.time + 5;
        mDut.startRanging(mockIbinder, mPackageName, null, request5, mockCallback);
        mMockLooper.dispatchAll();

        cbInorder.verify(mockCallback).onRangingFailure(RangingResultCallback.STATUS_CODE_FAIL);

        // verify metrics
        verify(mockMetrics).recordRequest(eq(mDefaultWs), eq(request1));
        verify(mockMetrics).recordRequest(eq(mDefaultWs), eq(request2));
        verify(mockMetrics).recordRequest(eq(mDefaultWs), eq(request3));
        verify(mockMetrics).recordRequest(eq(mDefaultWs), eq(request4));
        verify(mockMetrics).recordRequest(eq(mDefaultWs), eq(request5));
        verify(mockMetrics).recordResult(eq(request1), eq(result1.first));
        verify(mockMetrics).recordResult(eq(request3), eq(result3.first));
        verify(mockMetrics).recordResult(eq(request4), eq(result4.first));
        verify(mockMetrics, times(2)).recordOverallStatus(
                WifiMetricsProto.WifiRttLog.OVERALL_THROTTLE);
        verify(mockMetrics, times(3)).recordOverallStatus(
                WifiMetricsProto.WifiRttLog.OVERALL_SUCCESS);

        verify(mockNative, atLeastOnce()).isReady();
        verifyNoMoreInteractions(mockNative, mockMetrics, mockCallback,
                mAlarmManager.getAlarmManager());
    }

    /**
     * Validate that throttling of background request handles multiple work source correctly:
     * - Time t1: background request uid=10: ok
     * - Time t2 = t1+small: background request ws={10,20}: ok
     * - Time t3 = t1+gap: background request uid=10: fail (throttled)
     */
    @Test
    public void testRangingThrottleBackgroundWorkSources() throws Exception {
        runTestRangingThrottleBackgroundWorkSources(false);
    }

    @Test
    public void testRangingThrottleBackgroundChainedWorkSources() throws Exception {
        runTestRangingThrottleBackgroundWorkSources(true);
    }

    private void runTestRangingThrottleBackgroundWorkSources(boolean isChained) throws Exception {
        final WorkSource wsReq1 = new WorkSource();
        final WorkSource wsReq2 = new WorkSource();
        if (isChained) {
            wsReq1.createWorkChain().addNode(10, "foo");

            wsReq2.createWorkChain().addNode(10, "foo");
            wsReq2.createWorkChain().addNode(20, "bar");
        } else {
            wsReq1.add(10);

            wsReq2.add(10);
            wsReq2.add(20);
        }

        RangingRequest request1 = RttTestUtils.getDummyRangingRequest((byte) 1);
        RangingRequest request2 = RttTestUtils.getDummyRangingRequest((byte) 2);
        RangingRequest request3 = RttTestUtils.getDummyRangingRequest((byte) 3);

        Pair<List<RttResult>, List<RangingResult>> result1 = RttTestUtils.getDummyRangingResults(
                request1);
        Pair<List<RttResult>, List<RangingResult>> result2 = RttTestUtils.getDummyRangingResults(
                request2);

        InOrder cbInorder = inOrder(mockCallback);

        ClockAnswer clock = new ClockAnswer();
        doAnswer(clock).when(mockClock).getElapsedSinceBootMillis();
        when(mockActivityManager.getUidImportance(anyInt())).thenReturn(
                ActivityManager.RunningAppProcessInfo.IMPORTANCE_GONE); // far background

        // (1) issue a request at time t1 for {10}: should be dispatched since first one!
        clock.time = 100;
        mDut.startRanging(mockIbinder, mPackageName, wsReq1, request1, mockCallback);
        mMockLooper.dispatchAll();

        verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(request1), eq(true));
        verifyWakeupSet(true, clock.time);

        // (1.1) get result
        mDut.onRangingResults(mIntCaptor.getValue(), result1.first);
        mMockLooper.dispatchAll();

        cbInorder.verify(mockCallback).onRangingResults(result1.second);
        verifyWakeupCancelled();

        // (2) issue a request at time t2 = t1 + 0.5 gap for {10,20}: should be dispatched since
        //     uid=20 should not be throttled
        clock.time = 100 + BACKGROUND_PROCESS_EXEC_GAP_MS / 2;
        mDut.startRanging(mockIbinder, mPackageName, wsReq2, request2, mockCallback);
        mMockLooper.dispatchAll();

        verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(request2), eq(true));
        verifyWakeupSet(true, clock.time);

        // (2.1) get result
        mDut.onRangingResults(mIntCaptor.getValue(), result2.first);
        mMockLooper.dispatchAll();

        cbInorder.verify(mockCallback).onRangingResults(result2.second);
        verifyWakeupCancelled();

        // (3) issue a request at t3 = t1 + 1.1 * gap for {10}: should be rejected (throttled)
        clock.time = 100 + BACKGROUND_PROCESS_EXEC_GAP_MS * 11 / 10;
        mDut.startRanging(mockIbinder, mPackageName, wsReq1, request3, mockCallback);
        mMockLooper.dispatchAll();

        cbInorder.verify(mockCallback).onRangingFailure(RangingResultCallback.STATUS_CODE_FAIL);

        // verify metrics
        verify(mockMetrics).recordRequest(eq(wsReq1), eq(request1));
        verify(mockMetrics).recordRequest(eq(wsReq2), eq(request2));
        verify(mockMetrics).recordRequest(eq(wsReq1), eq(request3));
        verify(mockMetrics).recordResult(eq(request1), eq(result1.first));
        verify(mockMetrics).recordResult(eq(request2), eq(result2.first));
        verify(mockMetrics).recordOverallStatus(WifiMetricsProto.WifiRttLog.OVERALL_THROTTLE);
        verify(mockMetrics, times(2)).recordOverallStatus(
                WifiMetricsProto.WifiRttLog.OVERALL_SUCCESS);

        verify(mockNative, atLeastOnce()).isReady();
        verifyNoMoreInteractions(mockNative, mockMetrics, mockCallback,
                mAlarmManager.getAlarmManager());
    }

    /**
     * Validate that flooding the service with ranging requests will cause it to start rejecting
     * rejects from the flooding uid. Single UID.
     */
    @Test
    public void testRejectFloodingRequestsSingleUid() throws Exception {
        runFloodRequestsTest(true, false);
    }

    /**
     * Validate that flooding the service with ranging requests will cause it to start rejecting
     * rejects from the flooding uid. WorkSource (all identical).
     */
    @Test
    public void testRejectFloodingRequestsIdenticalWorksources() throws Exception {
        runFloodRequestsTest(false, false);
    }

    @Test
    public void testRejectFloodingRequestsIdenticalChainedWorksources() throws Exception {
        runFloodRequestsTest(false, true);
    }

    /**
     * Validate that flooding the service with ranging requests will cause it to start rejecting
     * rejects from the flooding uid. WorkSource (with one constant UID but other varying UIDs -
     * the varying UIDs should prevent the flood throttle)
     */
    @Test
    public void testDontRejectFloodingRequestsVariousUids() throws Exception {
        RangingRequest request = RttTestUtils.getDummyRangingRequest((byte) 1);
        WorkSource ws = new WorkSource(10);

        // 1. issue a request
        mDut.startRanging(mockIbinder, mPackageName, ws, request, mockCallback);
        mMockLooper.dispatchAll();

        verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(request), eq(true));
        verifyWakeupSet(true, 0);

        // 2. issue FLOOD LEVEL requests + 10 at various UIDs - no failure expected
        for (int i = 0; i < RttServiceImpl.MAX_QUEUED_PER_UID + 10; ++i) {
            WorkSource wsExtra = new WorkSource(ws);
            wsExtra.add(11 + i);
            mDut.startRanging(mockIbinder, mPackageName, wsExtra, request, mockCallback);
        }
        mMockLooper.dispatchAll();

        // 3. clear queue
        mDut.disable();
        mMockLooper.dispatchAll();

        verifyWakeupCancelled();
        verify(mockNative).rangeCancel(eq(mIntCaptor.getValue()), any());
        verify(mockCallback, times(RttServiceImpl.MAX_QUEUED_PER_UID + 11)).onRangingFailure(
                RangingResultCallback.STATUS_CODE_FAIL_RTT_NOT_AVAILABLE);

        // verify metrics
        for (int i = 0; i < RttServiceImpl.MAX_QUEUED_PER_UID + 11; ++i) {
            WorkSource wsExtra = new WorkSource(10);
            if (i != 0) {
                wsExtra.add(10 + i);
            }
            verify(mockMetrics).recordRequest(eq(wsExtra), eq(request));
        }
        verify(mockMetrics, times(RttServiceImpl.MAX_QUEUED_PER_UID + 11)).recordOverallStatus(
                WifiMetricsProto.WifiRttLog.OVERALL_RTT_NOT_AVAILABLE);

        verify(mockNative, atLeastOnce()).isReady();
        verifyNoMoreInteractions(mockNative, mockMetrics, mockCallback,
                mAlarmManager.getAlarmManager());
    }

    /**
     * Utility to run configurable tests for flooding range requests.
     * - Execute a single request
     * - Flood service with requests: using same ID or same WorkSource
     * - Provide results (to clear queue) and execute another test: validate succeeds
     */
    private void runFloodRequestsTest(boolean useUids, boolean useChainedWorkSources)
            throws Exception {
        RangingRequest request = RttTestUtils.getDummyRangingRequest((byte) 1);
        Pair<List<RttResult>, List<RangingResult>> result = RttTestUtils.getDummyRangingResults(
                request);

        WorkSource ws = new WorkSource();
        if (useChainedWorkSources) {
            ws.createWorkChain().addNode(10, "foo");
            ws.createWorkChain().addNode(20, "bar");
            ws.createWorkChain().addNode(30, "baz");
        } else {
            ws.add(10);
            ws.add(20);
            ws.add(30);
        }

        InOrder cbInorder = inOrder(mockCallback);
        InOrder nativeInorder = inOrder(mockNative);

        // 1. issue a request
        mDut.startRanging(mockIbinder, mPackageName, useUids ? null : ws, request, mockCallback);
        mMockLooper.dispatchAll();

        nativeInorder.verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(request), eq(true));
        verifyWakeupSet(true, 0);

        // 2. issue FLOOD LEVEL requests + 10: should get 11 failures (10 extra + 1 original)
        for (int i = 0; i < RttServiceImpl.MAX_QUEUED_PER_UID + 10; ++i) {
            mDut.startRanging(mockIbinder, mPackageName, useUids ? null : ws, request,
                    mockCallback);
        }
        mMockLooper.dispatchAll();

        cbInorder.verify(mockCallback, times(11)).onRangingFailure(
                RangingResultCallback.STATUS_CODE_FAIL);

        // 3. provide results
        mDut.onRangingResults(mIntCaptor.getValue(), result.first);
        mMockLooper.dispatchAll();

        cbInorder.verify(mockCallback).onRangingResults(result.second);
        verifyWakeupCancelled();

        nativeInorder.verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(request), eq(true));
        verifyWakeupSet(true, 0);

        // 4. issue a request: don't expect a failure
        mDut.startRanging(mockIbinder, mPackageName, useUids ? null : ws, request, mockCallback);
        mMockLooper.dispatchAll();

        // 5. clear queue
        mDut.disable();
        mMockLooper.dispatchAll();

        verifyWakeupCancelled();
        nativeInorder.verify(mockNative).rangeCancel(eq(mIntCaptor.getValue()), any());
        cbInorder.verify(mockCallback, times(RttServiceImpl.MAX_QUEUED_PER_UID)).onRangingFailure(
                RangingResultCallback.STATUS_CODE_FAIL_RTT_NOT_AVAILABLE);

        // verify metrics
        verify(mockMetrics, times(RttServiceImpl.MAX_QUEUED_PER_UID + 12)).recordRequest(
                eq(useUids ? mDefaultWs : ws), eq(request));
        verify(mockMetrics).recordResult(eq(request), eq(result.first));
        verify(mockMetrics, times(11)).recordOverallStatus(
                WifiMetricsProto.WifiRttLog.OVERALL_THROTTLE);
        verify(mockMetrics, times(RttServiceImpl.MAX_QUEUED_PER_UID)).recordOverallStatus(
                WifiMetricsProto.WifiRttLog.OVERALL_RTT_NOT_AVAILABLE);
        verify(mockMetrics).recordOverallStatus(WifiMetricsProto.WifiRttLog.OVERALL_SUCCESS);

        verify(mockNative, atLeastOnce()).isReady();
        verifyNoMoreInteractions(mockNative, mockMetrics, mockCallback,
                mAlarmManager.getAlarmManager());
    }

    /**
     * Validate that when Wi-Fi gets disabled (HAL level) the ranging queue gets cleared.
     */
    @Test
    public void testDisableWifiFlow() throws Exception {
        runDisableRttFlow(FAILURE_MODE_DISABLE_WIFI);
    }

    /**
     * Validate that when Doze mode starts, RTT gets disabled and the ranging queue gets cleared.
     */
    @Test
    public void testDozeModeFlow() throws Exception {
        runDisableRttFlow(FAILURE_MODE_ENABLE_DOZE);
    }

    /**
     * Validate that when locationing is disabled, RTT gets disabled and the ranging queue gets
     * cleared.
     */
    @Test
    public void testLocationingOffFlow() throws Exception {
        runDisableRttFlow(FAILURE_MODE_DISABLE_LOCATIONING);
    }


    private static final int FAILURE_MODE_DISABLE_WIFI = 0;
    private static final int FAILURE_MODE_ENABLE_DOZE = 1;
    private static final int FAILURE_MODE_DISABLE_LOCATIONING = 2;

    /**
     * Actually execute the disable RTT flow: either by disabling Wi-Fi or enabling doze.
     *
     * @param failureMode The mechanism by which RTT is to be disabled. One of the FAILURE_MODE_*
     *                    constants above.
     */
    private void runDisableRttFlow(int failureMode) throws Exception {
        RangingRequest request1 = RttTestUtils.getDummyRangingRequest((byte) 1);
        RangingRequest request2 = RttTestUtils.getDummyRangingRequest((byte) 2);
        RangingRequest request3 = RttTestUtils.getDummyRangingRequest((byte) 3);

        IRttCallback mockCallback2 = mock(IRttCallback.class);
        IRttCallback mockCallback3 = mock(IRttCallback.class);

        // (1) request 2 ranging operations: request 1 should be sent to HAL
        mDut.startRanging(mockIbinder, mPackageName, null, request1, mockCallback);
        mDut.startRanging(mockIbinder, mPackageName, null, request2, mockCallback2);
        mMockLooper.dispatchAll();

        verify(mockNative).rangeRequest(mIntCaptor.capture(), eq(request1), eq(true));
        verifyWakeupSet(true, 0);

        // (2) disable RTT: all requests should "fail"
        if (failureMode == FAILURE_MODE_DISABLE_WIFI) {
            when(mockNative.isReady()).thenReturn(false);
            mDut.disable();
        } else if (failureMode == FAILURE_MODE_ENABLE_DOZE) {
            simulatePowerStateChangeDoze(true);
        } else if (failureMode == FAILURE_MODE_DISABLE_LOCATIONING) {
            when(mockPermissionUtil.isLocationModeEnabled()).thenReturn(false);
            simulateLocationModeChange();
        }
        mMockLooper.dispatchAll();

        assertFalse(mDut.isAvailable());
        validateCorrectRttStatusChangeBroadcast(false);
        verify(mockNative).rangeCancel(eq(mIntCaptor.getValue()), any());
        verify(mockCallback).onRangingFailure(
                RangingResultCallback.STATUS_CODE_FAIL_RTT_NOT_AVAILABLE);
        verify(mockCallback2).onRangingFailure(
                RangingResultCallback.STATUS_CODE_FAIL_RTT_NOT_AVAILABLE);
        verifyWakeupCancelled();

        // (3) issue another request: it should fail
        mDut.startRanging(mockIbinder, mPackageName, null, request3, mockCallback3);
        mMockLooper.dispatchAll();

        verify(mockCallback3).onRangingFailure(
                RangingResultCallback.STATUS_CODE_FAIL_RTT_NOT_AVAILABLE);

        // (4) enable RTT: nothing should happen (no requests in queue!)
        if (failureMode == FAILURE_MODE_DISABLE_WIFI) {
            when(mockNative.isReady()).thenReturn(true);
            mDut.enableIfPossible();
        } else if (failureMode == FAILURE_MODE_ENABLE_DOZE) {
            simulatePowerStateChangeDoze(false);
        } else if (failureMode == FAILURE_MODE_DISABLE_LOCATIONING) {
            when(mockPermissionUtil.isLocationModeEnabled()).thenReturn(true);
            simulateLocationModeChange();
        }
        mMockLooper.dispatchAll();

        assertTrue(mDut.isAvailable());
        validateCorrectRttStatusChangeBroadcast(true);

        // verify metrics
        verify(mockMetrics).recordRequest(eq(mDefaultWs), eq(request1));
        verify(mockMetrics).recordRequest(eq(mDefaultWs), eq(request2));
        verify(mockMetrics, times(3)).recordOverallStatus(
                WifiMetricsProto.WifiRttLog.OVERALL_RTT_NOT_AVAILABLE);

        verify(mockNative, atLeastOnce()).isReady();
        verifyNoMoreInteractions(mockNative, mockMetrics, mockCallback, mockCallback2,
                mockCallback3, mAlarmManager.getAlarmManager());
    }

    /*
     * Utilities
     */

    /**
     * Simulate power state change due to doze. Changes the power manager return values and
     * dispatches a broadcast.
     */
    private void simulatePowerStateChangeDoze(boolean isDozeOn) {
        when(mMockPowerManager.isDeviceIdleMode()).thenReturn(isDozeOn);

        Intent intent = new Intent(PowerManager.ACTION_DEVICE_IDLE_MODE_CHANGED);
        mPowerBcastReceiver.onReceive(mockContext, intent);
    }

    /**
     * Simulate the broadcast which is dispatched when a LOCATION_MODE is modified.
     */
    private void simulateLocationModeChange() {
        Intent intent = new Intent(LocationManager.MODE_CHANGED_ACTION);
        mLocationModeReceiver.onReceive(mockContext, intent);
    }

    private void verifyWakeupSet(boolean useAwareTimeout, long baseTime) {
        ArgumentCaptor<Long> longCaptor = ArgumentCaptor.forClass(Long.class);

        mInOrder.verify(mAlarmManager.getAlarmManager()).setExact(anyInt(), longCaptor.capture(),
                eq(RttServiceImpl.HAL_RANGING_TIMEOUT_TAG), any(AlarmManager.OnAlarmListener.class),
                any(Handler.class));

        assertEquals(baseTime + (useAwareTimeout ? RttServiceImpl.HAL_AWARE_RANGING_TIMEOUT_MS
                : RttServiceImpl.HAL_RANGING_TIMEOUT_MS), longCaptor.getValue().longValue());
    }

    private void verifyWakeupCancelled() {
        mInOrder.verify(mAlarmManager.getAlarmManager()).cancel(
                any(AlarmManager.OnAlarmListener.class));
    }

    /**
     * Validates that the broadcast sent on RTT status change is correct.
     *
     * @param expectedEnabled The expected change status - i.e. are we expected to announce that
     *                        RTT is enabled (true) or disabled (false).
     */
    private void validateCorrectRttStatusChangeBroadcast(boolean expectedEnabled) {
        ArgumentCaptor<Intent> intent = ArgumentCaptor.forClass(Intent.class);

        mInOrder.verify(mockContext).sendBroadcastAsUser(intent.capture(), eq(UserHandle.ALL));
        assertEquals(intent.getValue().getAction(), WifiRttManager.ACTION_WIFI_RTT_STATE_CHANGED);
    }

    private class AwareTranslatePeerHandlesToMac extends MockAnswerUtil.AnswerWithArguments {
        private int mExpectedUid;
        private Map<Integer, MacAddress> mPeerIdToMacMap;

        AwareTranslatePeerHandlesToMac(int expectedUid, Map<Integer, MacAddress> peerIdToMacMap) {
            mExpectedUid = expectedUid;
            mPeerIdToMacMap = peerIdToMacMap;
        }

        public void answer(int uid, List<Integer> peerIds, IWifiAwareMacAddressProvider callback) {
            assertEquals("Invalid UID", mExpectedUid, uid);

            Map<Integer, byte[]> result = new HashMap<>();
            for (Integer peerId: peerIds) {
                byte[] macBytes = null;
                MacAddress macAddr = mPeerIdToMacMap.get(peerId);
                if (macAddr != null) {
                    macBytes = macAddr.toByteArray();
                }

                result.put(peerId, macBytes);
            }

            try {
                callback.macAddress(result);
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }
    }

    private class BinderDeathAnswerBase extends MockAnswerUtil.AnswerWithArguments {
        protected Set<IBinder.DeathRecipient> mUniqueExecs = new HashSet<>();
    }

    private class BinderLinkToDeathAnswer extends BinderDeathAnswerBase {
        public void answer(IBinder.DeathRecipient recipient, int flags) {
            mUniqueExecs.add(recipient);
        }
    }

    private class BinderUnlinkToDeathAnswer extends BinderDeathAnswerBase {
        public boolean answer(IBinder.DeathRecipient recipient, int flags) {
            mUniqueExecs.add(recipient);
            return true;
        }
    }

    private class ClockAnswer extends MockAnswerUtil.AnswerWithArguments {
        public long time;

        public long answer() {
            return time;
        }
    }
}
