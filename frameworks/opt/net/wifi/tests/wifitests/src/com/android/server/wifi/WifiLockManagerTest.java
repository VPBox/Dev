/*
 * Copyright (C) 2010 The Android Open Source Project
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

import static org.hamcrest.CoreMatchers.not;
import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

import android.app.ActivityManager;
import android.content.Context;
import android.net.wifi.WifiManager;
import android.os.Binder;
import android.os.Handler;
import android.os.IBinder;
import android.os.WorkSource;
import android.os.test.TestLooper;

import androidx.test.filters.SmallTest;

import com.android.internal.app.IBatteryStats;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.InOrder;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.io.PrintWriter;
import java.io.StringWriter;

/** Unit tests for {@link WifiLockManager}. */
@SmallTest
public class WifiLockManagerTest {

    private static final int DEFAULT_TEST_UID_1 = 52;
    private static final int DEFAULT_TEST_UID_2 = 53;
    private static final int DEFAULT_TEST_UID_3 = 54;
    private static final int DEFAULT_TEST_UID_4 = 55;
    private static final int WIFI_LOCK_MODE_INVALID = -1;
    private static final String TEST_WIFI_LOCK_TAG = "TestTag";
    private static final String INTERFACE_NAME = "IfaceName";

    private ActivityManager.OnUidImportanceListener mUidImportanceListener;

    WifiLockManager mWifiLockManager;
    @Mock Clock mClock;
    @Mock IBatteryStats mBatteryStats;
    @Mock IBinder mBinder;
    @Mock IBinder mBinder2;
    WorkSource mWorkSource;
    WorkSource mChainedWorkSource;
    @Mock Context mContext;
    @Mock ClientModeImpl mClientModeImpl;
    @Mock FrameworkFacade mFrameworkFacade;
    @Mock ActivityManager mActivityManager;
    @Mock WifiMetrics mWifiMetrics;
    @Mock WifiNative mWifiNative;
    TestLooper mLooper;
    Handler mHandler;

    /**
     * Method to setup a WifiLockManager for the tests.
     * The WifiLockManager uses mocks for BatteryStats and Context.
     */
    @Before
    public void setUp() {
        mWorkSource = new WorkSource(DEFAULT_TEST_UID_1);
        mChainedWorkSource = new WorkSource();
        mChainedWorkSource.createWorkChain()
                .addNode(DEFAULT_TEST_UID_1, "tag1")
                .addNode(DEFAULT_TEST_UID_2, "tag2");

        MockitoAnnotations.initMocks(this);
        mLooper = new TestLooper();
        mHandler = new Handler(mLooper.getLooper());
        when(mContext.getSystemService(Context.ACTIVITY_SERVICE)).thenReturn(mActivityManager);
        when(mWifiNative.getClientInterfaceName()).thenReturn(INTERFACE_NAME);

        mWifiLockManager = new WifiLockManager(mContext, mBatteryStats,
                mClientModeImpl, mFrameworkFacade, mHandler, mWifiNative, mClock, mWifiMetrics);
    }

    private void acquireWifiLockSuccessful(int lockMode, String tag, IBinder binder, WorkSource ws)
            throws Exception {
        ArgumentCaptor<IBinder.DeathRecipient> deathRecipient =
                ArgumentCaptor.forClass(IBinder.DeathRecipient.class);
        mWifiLockManager.updateWifiClientConnected(true);
        assertTrue(mWifiLockManager.acquireWifiLock(lockMode, tag, binder, ws));
        assertThat(mWifiLockManager.getStrongestLockMode(),
                not(WifiManager.WIFI_MODE_NO_LOCKS_HELD));
        InOrder inOrder = inOrder(binder, mBatteryStats);

        inOrder.verify(binder).linkToDeath(deathRecipient.capture(), eq(0));
        if (lockMode == WifiManager.WIFI_MODE_FULL_LOW_LATENCY) {
            inOrder.verify(mBatteryStats).noteFullWifiLockAcquired(DEFAULT_TEST_UID_1);
        } else {
            inOrder.verify(mBatteryStats).noteFullWifiLockAcquiredFromSource(ws);
        }
    }

    private void captureUidImportanceListener() {
        ArgumentCaptor<ActivityManager.OnUidImportanceListener> uidImportanceListener =
                ArgumentCaptor.forClass(ActivityManager.OnUidImportanceListener.class);

        verify(mActivityManager).addOnUidImportanceListener(uidImportanceListener.capture(),
                anyInt());
        mUidImportanceListener = uidImportanceListener.getValue();
        assertNotNull(mUidImportanceListener);
    }

    private void releaseWifiLockSuccessful(IBinder binder) throws Exception {
        ArgumentCaptor<IBinder.DeathRecipient> deathRecipient =
                ArgumentCaptor.forClass(IBinder.DeathRecipient.class);

        assertTrue(mWifiLockManager.releaseWifiLock(binder));
        InOrder inOrder = inOrder(binder, mBatteryStats);
        inOrder.verify(binder).unlinkToDeath(deathRecipient.capture(), eq(0));
        inOrder.verify(mBatteryStats).noteFullWifiLockReleasedFromSource(any(WorkSource.class));
    }

    private void releaseWifiLockSuccessful_noBatteryStats(IBinder binder) throws Exception {
        ArgumentCaptor<IBinder.DeathRecipient> deathRecipient =
                ArgumentCaptor.forClass(IBinder.DeathRecipient.class);

        assertTrue(mWifiLockManager.releaseWifiLock(binder));
        InOrder inOrder = inOrder(binder, mBatteryStats);
        inOrder.verify(binder).unlinkToDeath(deathRecipient.capture(), eq(0));
    }

    private void releaseLowLatencyWifiLockSuccessful(IBinder binder) throws Exception {
        ArgumentCaptor<IBinder.DeathRecipient> deathRecipient =
                ArgumentCaptor.forClass(IBinder.DeathRecipient.class);

        assertTrue(mWifiLockManager.releaseWifiLock(binder));
        InOrder inOrder = inOrder(binder, mBatteryStats);
        inOrder.verify(binder).unlinkToDeath(deathRecipient.capture(), eq(0));
        inOrder.verify(mBatteryStats).noteFullWifiLockReleased(anyInt());
    }

    /**
     * Test to check that a new WifiLockManager should not be holding any locks.
     */
    @Test
    public void newWifiLockManagerShouldNotHaveAnyLocks() {
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD, mWifiLockManager.getStrongestLockMode());
    }

    /**
     * Test that a call to acquireWifiLock with valid parameters works.
     *
     * Steps: call acquireWifiLock on the empty WifiLockManager.
     * Expected: A subsequent call to getStrongestLockMode should reflect the type of the lock we
     * just added
     */
    @Test
    public void acquireWifiLockWithValidParamsShouldSucceed() throws Exception {
        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "", mBinder, mWorkSource);
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF, mWifiLockManager.getStrongestLockMode());
    }

    /**
     * Test that a call to acquireWifiLock will not succeed if there is already a lock for the same
     * binder instance.
     *
     * Steps: call acquireWifiLock twice
     * Expected: Second call should return false
     */
    @Test
    public void secondCallToAcquireWifiLockWithSameBinderShouldFail() throws Exception {
        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "", mBinder, mWorkSource);
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        assertFalse(mWifiLockManager.acquireWifiLock(
                WifiManager.WIFI_MODE_FULL_HIGH_PERF, "", mBinder, mWorkSource));
    }

    /**
     * After acquiring a lock, we should be able to remove it.
     *
     * Steps: acquire a WifiLock and then remove it.
     * Expected: Since a single lock was added, removing it should leave the WifiLockManager without
     * any locks.  We should not see any errors.
     */
    @Test
    public void releaseWifiLockShouldSucceed() throws Exception {
        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "", mBinder, mWorkSource);
        releaseWifiLockSuccessful(mBinder);
        verify(mWifiMetrics).addWifiLockAcqSession(eq(WifiManager.WIFI_MODE_FULL_HIGH_PERF),
                anyLong());
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD, mWifiLockManager.getStrongestLockMode());
    }

    /**
     * Releasing locks for one caller should not release locks for a different caller.
     *
     * Steps: acquire locks for two callers and remove locks for one.
     * Expected: locks for remaining caller should still be active.
     */
    @Test
    public void releaseLocksForOneCallerNotImpactOtherCallers() throws Exception {
        IBinder toReleaseBinder = mock(IBinder.class);
        WorkSource toReleaseWS = new WorkSource(DEFAULT_TEST_UID_1);
        WorkSource toKeepWS = new WorkSource(DEFAULT_TEST_UID_2);

        acquireWifiLockSuccessful(
                WifiManager.WIFI_MODE_FULL_HIGH_PERF, "", toReleaseBinder, toReleaseWS);
        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "", mBinder, toKeepWS);
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF, mWifiLockManager.getStrongestLockMode());
        releaseWifiLockSuccessful(toReleaseBinder);
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF, mWifiLockManager.getStrongestLockMode());
        releaseWifiLockSuccessful(mBinder);
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD, mWifiLockManager.getStrongestLockMode());
    }

    /**
     * Attempting to release a lock that we do not hold should return false.
     *
     * Steps: release a WifiLock
     * Expected: call to releaseWifiLock should return false.
     */
    @Test
    public void releaseWifiLockWithoutAcquireWillReturnFalse() {
        assertFalse(mWifiLockManager.releaseWifiLock(mBinder));
    }

    /**
     * Test used to verify call for getStrongestLockMode.
     *
     * Steps: The test first checks the return value for no held locks and then proceeds to test
     * with a single lock of each type.
     * Expected: getStrongestLockMode should reflect the type of lock we just added.
     * Note: getStrongestLockMode should not reflect deprecated lock types
     */
    @Test
    public void checkForProperValueForGetStrongestLockMode() throws Exception {
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD, mWifiLockManager.getStrongestLockMode());

        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "", mBinder, mWorkSource);
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF, mWifiLockManager.getStrongestLockMode());
        releaseWifiLockSuccessful(mBinder);

        assertTrue(mWifiLockManager.acquireWifiLock(WifiManager.WIFI_MODE_FULL, "",
                mBinder, mWorkSource));
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD, mWifiLockManager.getStrongestLockMode());
        releaseWifiLockSuccessful(mBinder);

        assertTrue(mWifiLockManager.acquireWifiLock(WifiManager.WIFI_MODE_SCAN_ONLY, "",
                mBinder, mWorkSource));
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD, mWifiLockManager.getStrongestLockMode());
    }

    /**
     * We should be able to create a merged WorkSource holding WorkSources for all active locks.
     *
     * Steps: call createMergedWorkSource and verify it is empty, add a lock and call again, it
     * should have one entry.
     * Expected: the first call should return a worksource with size 0 and the second should be size
     * 1.
     */
    @Test
    public void createMergedWorkSourceShouldSucceed() throws Exception {
        WorkSource checkMWS = mWifiLockManager.createMergedWorkSource();
        assertEquals(0, checkMWS.size());

        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "", mBinder, mWorkSource);
        checkMWS = mWifiLockManager.createMergedWorkSource();
        assertEquals(1, checkMWS.size());
    }

    /**
     * Checks that WorkChains are preserved when merged WorkSources are created.
     */
    @Test
    public void createMergedworkSourceWithChainsShouldSucceed() throws Exception {
        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "", mBinder, mWorkSource);
        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "", mBinder2,
                mChainedWorkSource);

        WorkSource merged = mWifiLockManager.createMergedWorkSource();
        assertEquals(1, merged.size());
        assertEquals(1, merged.getWorkChains().size());
    }

    /**
     * A smoke test for acquiring, updating and releasing WifiLocks with chained WorkSources.
     */
    @Test
    public void smokeTestLockLifecycleWithChainedWorkSource() throws Exception {
        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "", mBinder,
                mChainedWorkSource);

        WorkSource updated = new WorkSource();
        updated.set(mChainedWorkSource);
        updated.createWorkChain().addNode(
                DEFAULT_TEST_UID_1, "chain2");

        mWifiLockManager.updateWifiLockWorkSource(mBinder, updated);
        InOrder inOrder = inOrder(mBatteryStats);
        inOrder.verify(mBatteryStats).noteFullWifiLockAcquiredFromSource(eq(updated));
        inOrder.verify(mBatteryStats).noteFullWifiLockReleasedFromSource(mChainedWorkSource);

        releaseWifiLockSuccessful(mBinder);
    }

    /**
     * Test the ability to update a WifiLock WorkSource with a new WorkSource.
     *
     * Steps: acquire a WifiLock with the default test worksource, then attempt to update it.
     * Expected: Verify calls to release the original WorkSource and acquire with the new one to
     * BatteryStats.
     */
    @Test
    public void testUpdateWifiLockWorkSourceCalledWithWorkSource() throws Exception {
        WorkSource newWorkSource = new WorkSource(DEFAULT_TEST_UID_2);

        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "", mBinder, mWorkSource);

        mWifiLockManager.updateWifiLockWorkSource(mBinder, newWorkSource);
        InOrder inOrder = inOrder(mBatteryStats);
        inOrder.verify(mBatteryStats).noteFullWifiLockAcquiredFromSource(eq(newWorkSource));
        inOrder.verify(mBatteryStats).noteFullWifiLockReleasedFromSource(mWorkSource);
    }

    /**
     * Test the ability to update a WifiLock WorkSource with the callers UID.
     *
     * Steps: acquire a WifiLock with the default test worksource, then attempt to update it.
     * Expected: Verify calls to release the original WorkSource and acquire with the new one to
     * BatteryStats.
     */
    @Test
    public void testUpdateWifiLockWorkSourceCalledWithUID()  throws Exception {
        WorkSource newWorkSource = new WorkSource(Binder.getCallingUid());

        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "", mBinder, mWorkSource);

        mWifiLockManager.updateWifiLockWorkSource(mBinder, newWorkSource);
        InOrder inOrder = inOrder(mBatteryStats);
        inOrder.verify(mBatteryStats).noteFullWifiLockAcquiredFromSource(eq(newWorkSource));
        inOrder.verify(mBatteryStats).noteFullWifiLockReleasedFromSource(mWorkSource);
    }

    /**
     * Test an attempt to update a WifiLock that is not allocated.
     *
     * Steps: call updateWifiLockWorkSource
     * Expected: catch an IllegalArgumentException
     */
    @Test(expected = IllegalArgumentException.class)
    public void testUpdateWifiLockWorkSourceCalledWithoutActiveLock()  throws Exception {
        mWifiLockManager.updateWifiLockWorkSource(mBinder, null);
    }

    /**
     * Test when acquiring a hi-perf lock,
     * WifiLockManager calls to disable power save mechanism.
     */
    @Test
    public void testHiPerfLockAcquireCauseDisablePS() throws Exception {
        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "",
                mBinder, mWorkSource);
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        verify(mClientModeImpl).setPowerSave(false);
    }

    /**
     * Test when releasing a hi-perf lock,
     * WifiLockManager calls to enable power save mechanism.
     */
    @Test
    public void testHiPerfLockReleaseCauseEnablePS() throws Exception {
        InOrder inOrder = inOrder(mClientModeImpl);
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(true);

        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "",
                mBinder, mWorkSource);
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setPowerSave(false);

        releaseWifiLockSuccessful(mBinder);
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setPowerSave(true);
        verify(mWifiMetrics).addWifiLockActiveSession(eq(WifiManager.WIFI_MODE_FULL_HIGH_PERF),
                anyLong());
    }

    /**
     * Test when acquiring two hi-perf locks, then releasing them.
     * WifiLockManager calls to disable/enable power save mechanism only once.
     */
    @Test
    public void testHiPerfLockAcquireReleaseTwice() throws Exception {
        InOrder inOrder = inOrder(mClientModeImpl);
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(true);

        // Acquire the first lock
        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "",
                mBinder, mWorkSource);
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setPowerSave(false);

        // Now acquire another lock
        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "",
                mBinder2, mWorkSource);
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl, never()).setPowerSave(anyBoolean());

        // Release the first lock
        releaseWifiLockSuccessful(mBinder);
        verify(mWifiMetrics).addWifiLockAcqSession(eq(WifiManager.WIFI_MODE_FULL_HIGH_PERF),
                anyLong());

        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl, never()).setPowerSave(anyBoolean());

        // Release the second lock
        releaseWifiLockSuccessful(mBinder2);
        verify(mWifiMetrics).addWifiLockActiveSession(eq(WifiManager.WIFI_MODE_FULL_HIGH_PERF),
                anyLong());
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setPowerSave(true);
        verify(mWifiMetrics).addWifiLockActiveSession(eq(WifiManager.WIFI_MODE_FULL_HIGH_PERF),
                anyLong());
    }

    /**
     * Test when acquiring/releasing deprecated locks does not result in any action .
     */
    @Test
    public void testFullScanOnlyAcquireRelease() throws Exception {
        IBinder fullLockBinder = mock(IBinder.class);
        WorkSource fullLockWS = new WorkSource(DEFAULT_TEST_UID_1);
        IBinder scanOnlyLockBinder = mock(IBinder.class);
        WorkSource scanOnlyLockWS = new WorkSource(DEFAULT_TEST_UID_2);

        InOrder inOrder = inOrder(mClientModeImpl);
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(true);

        // Acquire the first lock as FULL
        assertTrue(mWifiLockManager.acquireWifiLock(WifiManager.WIFI_MODE_FULL, "",
                fullLockBinder, fullLockWS));
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl, never()).setPowerSave(anyBoolean());

        // Now acquire another lock with SCAN-ONLY
        assertTrue(mWifiLockManager.acquireWifiLock(WifiManager.WIFI_MODE_SCAN_ONLY, "",
                scanOnlyLockBinder, scanOnlyLockWS));
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl, never()).setPowerSave(anyBoolean());

        // Release the FULL lock
        releaseWifiLockSuccessful_noBatteryStats(fullLockBinder);
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl, never()).setPowerSave(anyBoolean());

        // Release the SCAN-ONLY lock
        releaseWifiLockSuccessful_noBatteryStats(scanOnlyLockBinder);
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl, never()).setPowerSave(anyBoolean());
    }

    /**
     * Test failure case when setPowerSave() fails, during acquisition of hi-perf lock
     * Note, the lock is still acquired despite the failure in setPowerSave().
     * On any new lock activity, the setPowerSave() will be attempted if still needed.
     */
    @Test
    public void testHiPerfLockAcquireFail() throws Exception {
        IBinder fullLockBinder = mock(IBinder.class);
        WorkSource fullLockWS = new WorkSource(DEFAULT_TEST_UID_1);

        InOrder inOrder = inOrder(mClientModeImpl);
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(false);
        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "",
                mBinder, mWorkSource);
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setPowerSave(false);

        // Now attempting adding some other lock, WifiLockManager should retry setPowerSave()
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(true);
        assertTrue(mWifiLockManager.acquireWifiLock(WifiManager.WIFI_MODE_FULL, "",
                fullLockBinder, fullLockWS));
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setPowerSave(false);
    }

    /**
     * Test failure case when setPowerSave() fails, during release of hi-perf lock
     * Note, the lock is still released despite the failure in setPowerSave().
     * On any new lock activity, the setPowerSave() will be re-attempted if still needed.
     */
    @Test
    public void testHiPerfLockReleaseFail() throws Exception {
        IBinder fullLockBinder = mock(IBinder.class);
        WorkSource fullLockWS = new WorkSource(DEFAULT_TEST_UID_1);

        InOrder inOrder = inOrder(mClientModeImpl);
        when(mClientModeImpl.setPowerSave(false)).thenReturn(true);
        when(mClientModeImpl.setPowerSave(true)).thenReturn(false);

        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "",
                mBinder, mWorkSource);
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setPowerSave(false);

        releaseWifiLockSuccessful(mBinder);
        verify(mWifiMetrics).addWifiLockAcqSession(eq(WifiManager.WIFI_MODE_FULL_HIGH_PERF),
                anyLong());
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setPowerSave(true);
        verify(mWifiMetrics, never()).addWifiLockActiveSession(anyInt(), anyLong());

        // Now attempting adding some other lock, WifiLockManager should retry setPowerSave()
        when(mClientModeImpl.setPowerSave(true)).thenReturn(true);
        assertTrue(mWifiLockManager.acquireWifiLock(WifiManager.WIFI_MODE_FULL, "",
                fullLockBinder, fullLockWS));
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setPowerSave(true);
        verify(mWifiMetrics).addWifiLockActiveSession(eq(WifiManager.WIFI_MODE_FULL_HIGH_PERF),
                anyLong());
    }

    /**
     * Test when forcing hi-perf mode, that it overrides apps requests
     * until it is no longer forced.
     */
    @Test
    public void testForceHiPerf() throws Exception {
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(true);
        InOrder inOrder = inOrder(mClientModeImpl);
        mWifiLockManager.updateWifiClientConnected(true);
        assertTrue(mWifiLockManager.acquireWifiLock(WifiManager.WIFI_MODE_SCAN_ONLY, "",
                mBinder, mWorkSource));
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
        assertTrue(mWifiLockManager.forceHiPerfMode(true));
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setPowerSave(false);
        assertTrue(mWifiLockManager.forceHiPerfMode(false));
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setPowerSave(true);
        verify(mWifiMetrics).addWifiLockActiveSession(eq(WifiManager.WIFI_MODE_FULL_HIGH_PERF),
                anyLong());
    }

    /**
     * Test when forcing hi-perf mode, and aquire/release of hi-perf locks
     */
    @Test
    public void testForceHiPerfAcqRelHiPerf() throws Exception {
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(true);
        InOrder inOrder = inOrder(mClientModeImpl);

        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "",
                mBinder, mWorkSource);
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setPowerSave(false);

        assertTrue(mWifiLockManager.forceHiPerfMode(true));
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl, never()).setPowerSave(anyBoolean());

        releaseWifiLockSuccessful(mBinder);
        verify(mWifiMetrics).addWifiLockAcqSession(eq(WifiManager.WIFI_MODE_FULL_HIGH_PERF),
                anyLong());
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl, never()).setPowerSave(anyBoolean());

        assertTrue(mWifiLockManager.forceHiPerfMode(false));
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setPowerSave(true);
        verify(mWifiMetrics).addWifiLockActiveSession(eq(WifiManager.WIFI_MODE_FULL_HIGH_PERF),
                anyLong());
    }

    /**
     * Test when trying to force hi-perf to true twice back to back
     */
    @Test
    public void testForceHiPerfTwice() throws Exception {
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(true);
        mWifiLockManager.updateWifiClientConnected(true);
        InOrder inOrder = inOrder(mClientModeImpl);

        assertTrue(mWifiLockManager.forceHiPerfMode(true));
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setPowerSave(false);

        assertTrue(mWifiLockManager.forceHiPerfMode(true));
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl, never()).setPowerSave(anyBoolean());
    }

    /**
     * Test when failure when forcing hi-perf mode
     */
    @Test
    public void testForceHiPerfFailure() throws Exception {
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(false);
        mWifiLockManager.updateWifiClientConnected(true);
        InOrder inOrder = inOrder(mClientModeImpl);

        assertTrue(mWifiLockManager.acquireWifiLock(WifiManager.WIFI_MODE_SCAN_ONLY, "",
                mBinder, mWorkSource));
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());

        assertFalse(mWifiLockManager.forceHiPerfMode(true));
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setPowerSave(false);
    }

    /**
     * Test if a foreground app acquires a low-latency lock, and screen is on,
     * then that lock becomes the strongest lock even with presence of other locks.
     */
    @Test
    public void testForegroundAppAcquireLowLatencyScreenOn() throws Exception {
        // Set screen on, and app foreground
        mWifiLockManager.handleScreenStateChanged(true);
        when(mFrameworkFacade.isAppForeground(anyInt())).thenReturn(true);

        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_LOW_LATENCY, "",
                mBinder, mWorkSource);
        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "",
                mBinder2, mWorkSource);
        assertEquals(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                mWifiLockManager.getStrongestLockMode());
    }

    /**
     * Test if foreground app acquires a low-latency lock, and screen is off,
     * then that lock becomes ineffective.
     */
    @Test
    public void testForegroundAppAcquireLowLatencyScreenOff() throws Exception {
        // Set screen off, and app is foreground
        mWifiLockManager.handleScreenStateChanged(false);
        when(mFrameworkFacade.isAppForeground(anyInt())).thenReturn(true);

        assertTrue(mWifiLockManager.acquireWifiLock(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                "", mBinder, mWorkSource));
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
    }

    /**
     * Test if an app in background acquires a low-latency lock, and screen is on,
     * then that lock becomes ineffective.
     */
    @Test
    public void testBackgroundAppAcquireLowLatencyScreenOn() throws Exception {
        // Set screen on, and app is background
        mWifiLockManager.handleScreenStateChanged(true);
        when(mFrameworkFacade.isAppForeground(anyInt())).thenReturn(false);

        assertTrue(mWifiLockManager.acquireWifiLock(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                "", mBinder, mWorkSource));
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
    }

    /**
     * Test when acquiring a low-latency lock from a foreground app, and screen is on, then,
     * WifiLockManager calls to enable low-latency mechanism for devices supporting this.
     */
    @Test
    public void testLatencyLockAcquireCauseLlEnableNew() throws Exception {
        // Set screen on, and app foreground
        mWifiLockManager.handleScreenStateChanged(true);
        when(mFrameworkFacade.isAppForeground(anyInt())).thenReturn(true);
        when(mClientModeImpl.setLowLatencyMode(anyBoolean())).thenReturn(true);
        when(mWifiNative.getSupportedFeatureSet(INTERFACE_NAME))
                .thenReturn((long) WifiManager.WIFI_FEATURE_LOW_LATENCY);

        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_LOW_LATENCY, "",
                mBinder, mWorkSource);

        verify(mClientModeImpl).setLowLatencyMode(true);
        verify(mClientModeImpl).setPowerSave(false);
    }

    /**
     * Test when acquiring a low-latency lock from a foreground app, and screen is on, then,
     * WifiLockManager calls to disable power save, when low-latency mechanism is not supported.
     */
    @Test
    public void testLatencyLockAcquireCauseLL_enableLegacy() throws Exception {
        // Set screen on, and app foreground
        mWifiLockManager.handleScreenStateChanged(true);
        when(mFrameworkFacade.isAppForeground(anyInt())).thenReturn(true);
        when(mWifiNative.getSupportedFeatureSet(INTERFACE_NAME))
                .thenReturn((long) WifiManager.WIFI_FEATURE_TX_POWER_LIMIT);

        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_LOW_LATENCY, "",
                mBinder, mWorkSource);

        verify(mClientModeImpl).setPowerSave(false);
        verify(mClientModeImpl, never()).setLowLatencyMode(anyBoolean());
    }

    /**
     * Test when releasing an acquired low-latency lock,
     * WifiLockManager calls to disable low-latency mechanism.
     */
    @Test
    public void testLatencyLockReleaseCauseLlDisable() throws Exception {
        // Set screen on, and app foreground
        mWifiLockManager.handleScreenStateChanged(true);
        when(mFrameworkFacade.isAppForeground(anyInt())).thenReturn(true);
        when(mWifiNative.getSupportedFeatureSet(INTERFACE_NAME))
                .thenReturn((long) WifiManager.WIFI_FEATURE_LOW_LATENCY);

        // Make sure setLowLatencyMode() is successful
        when(mClientModeImpl.setLowLatencyMode(anyBoolean())).thenReturn(true);
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(true);

        InOrder inOrder = inOrder(mClientModeImpl);

        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_LOW_LATENCY, "",
                mBinder, mWorkSource);
        inOrder.verify(mClientModeImpl).setLowLatencyMode(true);
        inOrder.verify(mClientModeImpl).setPowerSave(false);

        releaseLowLatencyWifiLockSuccessful(mBinder);
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setLowLatencyMode(false);
        inOrder.verify(mClientModeImpl).setPowerSave(true);
        verify(mWifiMetrics).addWifiLockActiveSession(eq(WifiManager.WIFI_MODE_FULL_LOW_LATENCY),
                anyLong());
    }

    /**
     * Test when acquire of low-latency lock fails to enable low-latency mode,
     * then release will not result in calling to disable low-latency.
     */
    @Test
    public void testLatencyLockReleaseFailure() throws Exception {
        InOrder inOrder = inOrder(mClientModeImpl);

        // Set screen on, and app is foreground
        mWifiLockManager.handleScreenStateChanged(true);
        when(mFrameworkFacade.isAppForeground(anyInt())).thenReturn(true);
        when(mWifiNative.getSupportedFeatureSet(INTERFACE_NAME))
                .thenReturn((long) WifiManager.WIFI_FEATURE_LOW_LATENCY);

        // Fail the call to ClientModeImpl
        when(mClientModeImpl.setLowLatencyMode(true)).thenReturn(false);

        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_LOW_LATENCY, "",
                mBinder, mWorkSource);
        assertEquals(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setLowLatencyMode(true);

        releaseLowLatencyWifiLockSuccessful(mBinder);
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl, never()).setLowLatencyMode(anyBoolean());
    }

    /**
     * Test when acquire of low-latency lock succeeds in enable low latency
     * but fails to disable power save, then low latency mode is reverted
     */
    @Test
    public void testLatencyfail2DisablePowerSave() throws Exception {
        InOrder inOrder = inOrder(mClientModeImpl);

        // Set screen on, and app is foreground
        mWifiLockManager.handleScreenStateChanged(true);
        when(mFrameworkFacade.isAppForeground(anyInt())).thenReturn(true);
        when(mWifiNative.getSupportedFeatureSet(INTERFACE_NAME))
                .thenReturn((long) WifiManager.WIFI_FEATURE_LOW_LATENCY);

        // Succeed to setLowLatencyMode()
        when(mClientModeImpl.setLowLatencyMode(anyBoolean())).thenReturn(true);
        // Fail to setPowerSave()
        when(mClientModeImpl.setPowerSave(false)).thenReturn(false);

        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_LOW_LATENCY, "",
                mBinder, mWorkSource);
        assertEquals(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setLowLatencyMode(true);
        inOrder.verify(mClientModeImpl).setPowerSave(false);
        inOrder.verify(mClientModeImpl).setLowLatencyMode(false);
    }

    /**
     * Test when a low-latency lock is acquired (foreground app, screen-on),
     * then, screen go off, then low-latency mode is turned off.
     */
    @Test
    public void testLatencyLockGoScreenOff() throws Exception {
        // Set screen on, app foreground
        mWifiLockManager.handleScreenStateChanged(true);
        when(mFrameworkFacade.isAppForeground(anyInt())).thenReturn(true);
        when(mWifiNative.getSupportedFeatureSet(INTERFACE_NAME))
                .thenReturn((long) WifiManager.WIFI_FEATURE_LOW_LATENCY);

        // Make sure setLowLatencyMode() is successful
        when(mClientModeImpl.setLowLatencyMode(anyBoolean())).thenReturn(true);
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(true);

        InOrder inOrder = inOrder(mClientModeImpl);

        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_LOW_LATENCY, "",
                mBinder, mWorkSource);
        assertEquals(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setLowLatencyMode(true);
        inOrder.verify(mClientModeImpl).setPowerSave(false);

        mWifiLockManager.handleScreenStateChanged(false);
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setLowLatencyMode(false);
        inOrder.verify(mClientModeImpl).setPowerSave(true);
        verify(mWifiMetrics).addWifiLockActiveSession(eq(WifiManager.WIFI_MODE_FULL_LOW_LATENCY),
                anyLong());
    }

    /**
     * Test when a low-latency lock is acquired (foreground app, screen-on),
     * then, app goes to background, then low-latency mode is turned off.
     */
    @Test
    public void testLatencyLockGoBackground() throws Exception {
        // Initially, set screen on, app foreground
        mWifiLockManager.handleScreenStateChanged(true);
        when(mFrameworkFacade.isAppForeground(anyInt())).thenReturn(true);
        when(mWifiNative.getSupportedFeatureSet(INTERFACE_NAME))
                .thenReturn((long) WifiManager.WIFI_FEATURE_LOW_LATENCY);

        // Make sure setLowLatencyMode() is successful
        when(mClientModeImpl.setLowLatencyMode(anyBoolean())).thenReturn(true);
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(true);

        InOrder inOrder = inOrder(mClientModeImpl);

        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_LOW_LATENCY, "",
                mBinder, mWorkSource);
        captureUidImportanceListener();

        assertEquals(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setLowLatencyMode(true);
        inOrder.verify(mClientModeImpl).setPowerSave(false);

        /* App going to background */
        mUidImportanceListener.onUidImportance(DEFAULT_TEST_UID_1,
                ActivityManager.RunningAppProcessInfo.IMPORTANCE_BACKGROUND);
        mLooper.dispatchAll();
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setLowLatencyMode(false);
        inOrder.verify(mClientModeImpl).setPowerSave(true);
        verify(mWifiMetrics).addWifiLockActiveSession(eq(WifiManager.WIFI_MODE_FULL_LOW_LATENCY),
                anyLong());
    }

    /**
     * Test when a low-latency lock is acquired (background app, screen-on),
     * then, lock is only effective when app goes to foreground.
     */
    @Test
    public void testLatencyLockGoForeground() throws Exception {
        // Initially, set screen on, and app background
        mWifiLockManager.handleScreenStateChanged(true);
        when(mFrameworkFacade.isAppForeground(anyInt())).thenReturn(false);
        when(mWifiNative.getSupportedFeatureSet(INTERFACE_NAME))
                .thenReturn((long) WifiManager.WIFI_FEATURE_LOW_LATENCY);
        // Make sure setLowLatencyMode() is successful
        when(mClientModeImpl.setLowLatencyMode(anyBoolean())).thenReturn(true);

        mWifiLockManager.updateWifiClientConnected(true);

        InOrder inOrder = inOrder(mClientModeImpl);

        assertTrue(mWifiLockManager.acquireWifiLock(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                "", mBinder, mWorkSource));
        captureUidImportanceListener();

        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl, never()).setLowLatencyMode(anyBoolean());
        inOrder.verify(mClientModeImpl, never()).setPowerSave(anyBoolean());

        /* App going to foreground */
        mUidImportanceListener.onUidImportance(DEFAULT_TEST_UID_1,
                ActivityManager.RunningAppProcessInfo.IMPORTANCE_FOREGROUND);
        mLooper.dispatchAll();

        assertEquals(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setLowLatencyMode(true);
        inOrder.verify(mClientModeImpl).setPowerSave(false);
    }

    /**
     * Test when both low-latency lock and hi-perf lock are  acquired
     * then, hi-perf is active when app is in background , while low-latency
     * is active when app is in foreground (and screen on).
     */
    @Test
    public void testLatencyHiPerfLocks() throws Exception {
        // Initially, set screen on, and app background
        mWifiLockManager.handleScreenStateChanged(true);
        when(mFrameworkFacade.isAppForeground(anyInt())).thenReturn(false);
        when(mWifiNative.getSupportedFeatureSet(INTERFACE_NAME))
                .thenReturn((long) WifiManager.WIFI_FEATURE_LOW_LATENCY);
        mWifiLockManager.updateWifiClientConnected(true);

        // Make sure setLowLatencyMode()/setPowerSave() is successful
        when(mClientModeImpl.setLowLatencyMode(anyBoolean())).thenReturn(true);
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(true);

        InOrder inOrder = inOrder(mClientModeImpl);

        assertTrue(mWifiLockManager.acquireWifiLock(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                "", mBinder, mWorkSource));
        assertTrue(mWifiLockManager.acquireWifiLock(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                "", mBinder2, mWorkSource));
        captureUidImportanceListener();

        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl, never()).setLowLatencyMode(anyBoolean());
        inOrder.verify(mClientModeImpl).setPowerSave(false);

        /* App going to foreground */
        mUidImportanceListener.onUidImportance(DEFAULT_TEST_UID_1,
                ActivityManager.RunningAppProcessInfo.IMPORTANCE_FOREGROUND);
        mLooper.dispatchAll();

        assertEquals(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                mWifiLockManager.getStrongestLockMode());

        inOrder.verify(mClientModeImpl).setPowerSave(true);
        verify(mWifiMetrics).addWifiLockActiveSession(eq(WifiManager.WIFI_MODE_FULL_HIGH_PERF),
                anyLong());
        inOrder.verify(mClientModeImpl).setLowLatencyMode(true);
        inOrder.verify(mClientModeImpl).setPowerSave(false);
    }

    /**
     * Test when forcing low-latency mode, that it overrides apps requests
     * until it is no longer forced.
     */
    @Test
    public void testForceLowLatency() throws Exception {
        when(mClientModeImpl.setLowLatencyMode(anyBoolean())).thenReturn(true);
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(true);
        when(mWifiNative.getSupportedFeatureSet(INTERFACE_NAME))
                .thenReturn((long) WifiManager.WIFI_FEATURE_LOW_LATENCY);

        InOrder inOrder = inOrder(mClientModeImpl);

        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "",
                mBinder, mWorkSource);
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        assertTrue(mWifiLockManager.forceLowLatencyMode(true));
        assertEquals(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setLowLatencyMode(true);
        inOrder.verify(mClientModeImpl).setPowerSave(false);
        assertTrue(mWifiLockManager.forceLowLatencyMode(false));
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setLowLatencyMode(false);
        inOrder.verify(mClientModeImpl).setPowerSave(true);
        verify(mWifiMetrics).addWifiLockActiveSession(eq(WifiManager.WIFI_MODE_FULL_LOW_LATENCY),
                anyLong());
    }

    /**
     * Test when forcing low-latency mode, that it is effective even if screen is off.
     */
    @Test
    public void testForceLowLatencyScreenOff() throws Exception {
        when(mClientModeImpl.setLowLatencyMode(anyBoolean())).thenReturn(true);
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(true);
        mWifiLockManager.handleScreenStateChanged(false);
        when(mWifiNative.getSupportedFeatureSet(INTERFACE_NAME))
                .thenReturn((long) WifiManager.WIFI_FEATURE_LOW_LATENCY);
        mWifiLockManager.updateWifiClientConnected(true);

        InOrder inOrder = inOrder(mClientModeImpl);

        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
        assertTrue(mWifiLockManager.forceLowLatencyMode(true));
        assertEquals(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setLowLatencyMode(true);
        inOrder.verify(mClientModeImpl).setPowerSave(false);

        mWifiLockManager.handleScreenStateChanged(true);
        assertEquals(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl, never()).setLowLatencyMode(anyBoolean());
        inOrder.verify(mClientModeImpl, never()).setPowerSave(anyBoolean());

        mWifiLockManager.handleScreenStateChanged(false);
        assertEquals(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl, never()).setLowLatencyMode(anyBoolean());
        inOrder.verify(mClientModeImpl, never()).setPowerSave(anyBoolean());
    }

    /**
     * Test when forcing low-latency mode, and aquire/release of low-latency locks
     */
    @Test
    public void testForceLowLatencyAcqRelLowLatency() throws Exception {
        when(mClientModeImpl.setLowLatencyMode(anyBoolean())).thenReturn(true);
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(true);
        mWifiLockManager.handleScreenStateChanged(true);
        when(mFrameworkFacade.isAppForeground(anyInt())).thenReturn(true);
        when(mWifiNative.getSupportedFeatureSet(INTERFACE_NAME))
                .thenReturn((long) WifiManager.WIFI_FEATURE_LOW_LATENCY);

        InOrder inOrder = inOrder(mClientModeImpl);

        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_LOW_LATENCY, "",
                mBinder, mWorkSource);
        assertEquals(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setLowLatencyMode(true);
        inOrder.verify(mClientModeImpl).setPowerSave(false);

        assertTrue(mWifiLockManager.forceLowLatencyMode(true));
        assertEquals(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl, never()).setLowLatencyMode(anyBoolean());
        inOrder.verify(mClientModeImpl, never()).setPowerSave(anyBoolean());

        releaseLowLatencyWifiLockSuccessful(mBinder);
        assertEquals(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl, never()).setLowLatencyMode(anyBoolean());
        inOrder.verify(mClientModeImpl, never()).setPowerSave(anyBoolean());

        assertTrue(mWifiLockManager.forceLowLatencyMode(false));
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setLowLatencyMode(false);
        inOrder.verify(mClientModeImpl).setPowerSave(true);
        verify(mWifiMetrics).addWifiLockActiveSession(eq(WifiManager.WIFI_MODE_FULL_LOW_LATENCY),
                anyLong());
    }

    /**
     * Test when trying to force low-latency to true twice back to back
     */
    @Test
    public void testForceLowLatencyTwice() throws Exception {
        when(mClientModeImpl.setLowLatencyMode(anyBoolean())).thenReturn(true);
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(true);
        when(mWifiNative.getSupportedFeatureSet(INTERFACE_NAME))
                .thenReturn((long) WifiManager.WIFI_FEATURE_LOW_LATENCY);
        mWifiLockManager.updateWifiClientConnected(true);

        InOrder inOrder = inOrder(mClientModeImpl);

        assertTrue(mWifiLockManager.forceLowLatencyMode(true));
        assertEquals(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setLowLatencyMode(true);
        inOrder.verify(mClientModeImpl).setPowerSave(false);

        assertTrue(mWifiLockManager.forceLowLatencyMode(true));
        assertEquals(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl, never()).setLowLatencyMode(anyBoolean());
        inOrder.verify(mClientModeImpl, never()).setPowerSave(anyBoolean());
    }

    /**
     * Test when forcing hi-perf mode then forcing low-latency mode
     */
    @Test
    public void testForceHiPerfLowLatency() throws Exception {
        when(mClientModeImpl.setLowLatencyMode(anyBoolean())).thenReturn(true);
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(true);
        when(mWifiNative.getSupportedFeatureSet(INTERFACE_NAME))
                .thenReturn((long) WifiManager.WIFI_FEATURE_LOW_LATENCY);
        mWifiLockManager.updateWifiClientConnected(true);
        InOrder inOrder = inOrder(mClientModeImpl);

        assertTrue(mWifiLockManager.forceHiPerfMode(true));
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setPowerSave(false);

        assertTrue(mWifiLockManager.forceLowLatencyMode(true));
        assertEquals(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                mWifiLockManager.getStrongestLockMode());

        inOrder.verify(mClientModeImpl).setPowerSave(true);
        verify(mWifiMetrics).addWifiLockActiveSession(eq(WifiManager.WIFI_MODE_FULL_HIGH_PERF),
                anyLong());
        inOrder.verify(mClientModeImpl).setLowLatencyMode(true);
        inOrder.verify(mClientModeImpl).setPowerSave(false);
    }

    /**
     * Test when forcing low-latency mode then forcing high-perf mode
     */
    @Test
    public void testForceLowLatencyHiPerf() throws Exception {
        when(mClientModeImpl.setLowLatencyMode(anyBoolean())).thenReturn(true);
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(true);
        when(mWifiNative.getSupportedFeatureSet(INTERFACE_NAME))
                .thenReturn((long) WifiManager.WIFI_FEATURE_LOW_LATENCY);
        mWifiLockManager.updateWifiClientConnected(true);

        InOrder inOrder = inOrder(mClientModeImpl);

        assertTrue(mWifiLockManager.forceLowLatencyMode(true));
        assertEquals(WifiManager.WIFI_MODE_FULL_LOW_LATENCY,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setLowLatencyMode(true);
        inOrder.verify(mClientModeImpl).setPowerSave(false);

        assertTrue(mWifiLockManager.forceHiPerfMode(true));
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setLowLatencyMode(false);
        inOrder.verify(mClientModeImpl).setPowerSave(true);
        verify(mWifiMetrics).addWifiLockActiveSession(eq(WifiManager.WIFI_MODE_FULL_LOW_LATENCY),
                anyLong());
        inOrder.verify(mClientModeImpl).setPowerSave(false);
    }

    /**
     * Test when failure when forcing low-latency mode
     */
    @Test
    public void testForceLowLatencyFailure() throws Exception {
        when(mClientModeImpl.setLowLatencyMode(anyBoolean())).thenReturn(false);
        when(mWifiNative.getSupportedFeatureSet(INTERFACE_NAME))
                .thenReturn((long) WifiManager.WIFI_FEATURE_LOW_LATENCY);

        InOrder inOrder = inOrder(mClientModeImpl);

        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "",
                mBinder, mWorkSource);
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());

        assertFalse(mWifiLockManager.forceLowLatencyMode(true));
        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF,
                mWifiLockManager.getStrongestLockMode());
        inOrder.verify(mClientModeImpl).setLowLatencyMode(true);
        // Since setLowLatencyMode() failed, no call to setPowerSave()
        inOrder.verify(mClientModeImpl, never()).setPowerSave(anyBoolean());
    }

    /**
     * Test acquiring locks while device is not connected
     * Expected: No locks are effective, and no call to other classes
     */
    @Test
    public void testAcquireLockWhileDisconnected() throws Exception {
        assertTrue(mWifiLockManager.acquireWifiLock(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "",
                mBinder, mWorkSource));
        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD, mWifiLockManager.getStrongestLockMode());
        verify(mBatteryStats, never()).noteFullWifiLockAcquired(anyInt());
        verify(mBatteryStats, never()).noteFullWifiLockAcquiredFromSource(any());
    }

    /**
     * Test acquiring locks while device is not connected, then connecting to an AP
     * Expected: Upon Connection, lock becomes effective
     */
    @Test
    public void testAcquireLockWhileDisconnectedConnect() throws Exception {
        assertTrue(mWifiLockManager.acquireWifiLock(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "",
                mBinder, mWorkSource));
        mWifiLockManager.updateWifiClientConnected(true);

        assertEquals(WifiManager.WIFI_MODE_FULL_HIGH_PERF, mWifiLockManager.getStrongestLockMode());
        verify(mBatteryStats).noteFullWifiLockAcquiredFromSource(eq(mWorkSource));
    }

    /**
     * Test acquiring locks while device is connected, then disconnecting from the AP
     * Expected: Upon disconnection, lock becomes ineffective
     */
    @Test
    public void testAcquireLockWhileConnectedDisconnect() throws Exception {
        // Set screen on, and app foreground
        mWifiLockManager.handleScreenStateChanged(true);
        when(mFrameworkFacade.isAppForeground(anyInt())).thenReturn(true);

        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_LOW_LATENCY, "", mBinder, mWorkSource);
        mWifiLockManager.updateWifiClientConnected(false);

        assertEquals(WifiManager.WIFI_MODE_NO_LOCKS_HELD, mWifiLockManager.getStrongestLockMode());
        verify(mBatteryStats).noteFullWifiLockReleased(DEFAULT_TEST_UID_1);
    }

    /**
     * Test that reporting of metrics for hi-perf lock acquistion is correct for both acquisition
     * time and active time.
     */
    @Test
    public void testHighPerfLockMetrics() throws Exception {
        long acquireTime      = 1000;
        long activationTime   = 2000;
        long deactivationTime = 3000;
        long releaseTime      = 4000;

        // Make sure setPowerSave() is successful
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(true);

        InOrder inOrder = inOrder(mWifiMetrics);

        // Acquire the lock
        when(mClock.getElapsedSinceBootMillis()).thenReturn(acquireTime);
        assertTrue(mWifiLockManager.acquireWifiLock(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "",
                mBinder, mWorkSource));

        // Activate the lock
        when(mClock.getElapsedSinceBootMillis()).thenReturn(activationTime);
        mWifiLockManager.updateWifiClientConnected(true);

        // Deactivate the lock
        when(mClock.getElapsedSinceBootMillis()).thenReturn(deactivationTime);
        mWifiLockManager.updateWifiClientConnected(false);

        inOrder.verify(mWifiMetrics).addWifiLockActiveSession(
                eq(WifiManager.WIFI_MODE_FULL_HIGH_PERF),
                eq(deactivationTime - activationTime));

        // Release the lock
        when(mClock.getElapsedSinceBootMillis()).thenReturn(releaseTime);
        releaseWifiLockSuccessful_noBatteryStats(mBinder);

        inOrder.verify(mWifiMetrics).addWifiLockAcqSession(
                eq(WifiManager.WIFI_MODE_FULL_HIGH_PERF),
                eq(releaseTime - acquireTime));
    }

    /**
     * Test that reporting of metrics for low-latency lock acquistion is correct for
     * both acquisition time and active time.
     */
    @Test
    public void testLowLatencyLockMetrics() throws Exception {
        long acquireTime      = 1000;
        long activationTime   = 2000;
        long deactivationTime = 3000;
        long releaseTime      = 4000;

        // Make sure setLowLatencyMode()/setPowerSave() is successful
        when(mClientModeImpl.setLowLatencyMode(anyBoolean())).thenReturn(true);
        when(mClientModeImpl.setPowerSave(anyBoolean())).thenReturn(true);

        // Set condition for activation of low-latency (except connection to AP)
        mWifiLockManager.handleScreenStateChanged(true);
        when(mFrameworkFacade.isAppForeground(anyInt())).thenReturn(true);
        when(mWifiNative.getSupportedFeatureSet(INTERFACE_NAME))
                .thenReturn((long) WifiManager.WIFI_FEATURE_LOW_LATENCY);

        InOrder inOrder = inOrder(mWifiMetrics);

        // Acquire the lock
        when(mClock.getElapsedSinceBootMillis()).thenReturn(acquireTime);
        assertTrue(mWifiLockManager.acquireWifiLock(WifiManager.WIFI_MODE_FULL_LOW_LATENCY, "",
                mBinder, mWorkSource));

        // Activate the lock
        when(mClock.getElapsedSinceBootMillis()).thenReturn(activationTime);
        mWifiLockManager.updateWifiClientConnected(true);

        // Deactivate the lock
        when(mClock.getElapsedSinceBootMillis()).thenReturn(deactivationTime);
        mWifiLockManager.updateWifiClientConnected(false);

        inOrder.verify(mWifiMetrics).addWifiLockActiveSession(
                eq(WifiManager.WIFI_MODE_FULL_LOW_LATENCY),
                eq(deactivationTime - activationTime));

        // Release the lock
        when(mClock.getElapsedSinceBootMillis()).thenReturn(releaseTime);
        releaseWifiLockSuccessful_noBatteryStats(mBinder);

        inOrder.verify(mWifiMetrics).addWifiLockAcqSession(
                eq(WifiManager.WIFI_MODE_FULL_LOW_LATENCY),
                eq(releaseTime - acquireTime));
    }

    /**
     * Verfies that dump() does not fail when no locks are held.
     */
    @Test
    public void dumpDoesNotFailWhenNoLocksAreHeld() {
        StringWriter sw = new StringWriter();
        PrintWriter pw = new PrintWriter(sw);
        mWifiLockManager.dump(pw);

        String wifiLockManagerDumpString = sw.toString();
        assertTrue(wifiLockManagerDumpString.contains(
                "Locks acquired: 0 full high perf, 0 full low latency"));
        assertTrue(wifiLockManagerDumpString.contains(
                "Locks released: 0 full high perf, 0 full low latency"));
        assertTrue(wifiLockManagerDumpString.contains("Locks held:"));
        assertFalse(wifiLockManagerDumpString.contains("WifiLock{"));
    }

    /**
     * Verifies that dump() contains lock information when there are locks held.
     */
    @Test
    public void dumpOutputsCorrectInformationWithActiveLocks() throws Exception {
        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, TEST_WIFI_LOCK_TAG,
                mBinder, mWorkSource);
        releaseWifiLockSuccessful(mBinder);
        verify(mWifiMetrics).addWifiLockAcqSession(eq(WifiManager.WIFI_MODE_FULL_HIGH_PERF),
                anyLong());
        acquireWifiLockSuccessful(WifiManager.WIFI_MODE_FULL_HIGH_PERF, TEST_WIFI_LOCK_TAG,
                mBinder, mWorkSource);

        StringWriter sw = new StringWriter();
        PrintWriter pw = new PrintWriter(sw);
        mWifiLockManager.dump(pw);

        String wifiLockManagerDumpString = sw.toString();
        assertTrue(wifiLockManagerDumpString.contains(
                "Locks acquired: 2 full high perf, 0 full low latency"));
        assertTrue(wifiLockManagerDumpString.contains(
                "Locks released: 1 full high perf, 0 full low latency"));
        assertTrue(wifiLockManagerDumpString.contains("Locks held:"));
        assertTrue(wifiLockManagerDumpString.contains(
                "WifiLock{" + TEST_WIFI_LOCK_TAG + " type=" + WifiManager.WIFI_MODE_FULL_HIGH_PERF
                + " uid=" + Binder.getCallingUid() + " workSource=WorkSource{"
                        + DEFAULT_TEST_UID_1 + "}"));
    }
}
