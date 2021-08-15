/*
 * Copyright 2017 The Android Open Source Project
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

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.*;

import android.net.wifi.WifiConfiguration;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.List;

/**
 * Unit tests for {@link WakeupLock}.
 */
@SmallTest
public class WakeupLockTest {

    private static final String SSID_1 = "ssid1";
    private static final String SSID_2 = "ssid2";

    @Mock private WifiConfigManager mWifiConfigManager;
    @Mock private WifiWakeMetrics mWifiWakeMetrics;
    @Mock private Clock mClock;

    private ScanResultMatchInfo mNetwork1;
    private ScanResultMatchInfo mNetwork2;
    private WakeupLock mWakeupLock;

    /**
     * Initialize objects before each test run.
     */
    @Before
    public void setUp() {
        MockitoAnnotations.initMocks(this);

        when(mClock.getElapsedSinceBootMillis()).thenReturn(0L);

        mNetwork1 = new ScanResultMatchInfo();
        mNetwork1.networkSsid = SSID_1;
        mNetwork1.networkType = WifiConfiguration.SECURITY_TYPE_OPEN;

        mNetwork2 = new ScanResultMatchInfo();
        mNetwork2.networkSsid = SSID_2;
        mNetwork2.networkType = WifiConfiguration.SECURITY_TYPE_EAP;

        mWakeupLock = new WakeupLock(mWifiConfigManager, mWifiWakeMetrics, mClock);
    }

    /**
     * Updates the lock enough times to evict any networks not passed in.
     *
     * <p>It calls update {@link WakeupLock#CONSECUTIVE_MISSED_SCANS_REQUIRED_TO_EVICT} times with
     * the given network list. It asserts that the lock isn't empty prior to each call to update.
     */
    private void updateEnoughTimesToEvictWithAsserts(Collection<ScanResultMatchInfo> networks) {
        for (int i = 0; i < WakeupLock.CONSECUTIVE_MISSED_SCANS_REQUIRED_TO_EVICT; i++) {
            assertFalse("Lock empty after " + i + " scans", mWakeupLock.isUnlocked());
            mWakeupLock.update(networks);
        }
    }

    /**
     * Updates the lock enough times to evict any networks not passed in.
     *
     * <p>It calls update {@link WakeupLock#CONSECUTIVE_MISSED_SCANS_REQUIRED_TO_EVICT} times with
     * the given network list. It does not make any assertions about the state of the lock.
     */
    private void updateEnoughTimesToEvictWithoutAsserts(Collection<ScanResultMatchInfo> networks) {
        for (int i = 0; i < WakeupLock.CONSECUTIVE_MISSED_SCANS_REQUIRED_TO_EVICT; i++) {
            mWakeupLock.update(networks);
        }
    }

    /**
     * Verify that calling update {@link WakeupLock#CONSECUTIVE_MISSED_SCANS_REQUIRED_TO_EVICT}
     * times sets the lock to be initialized.
     */
    @Test
    public void verifyInitializingLockByScans() {
        List<ScanResultMatchInfo> networks = Collections.singletonList(mNetwork1);
        mWakeupLock.setLock(networks);
        assertFalse(mWakeupLock.isInitialized());

        for (int i = 0; i < WakeupLock.CONSECUTIVE_MISSED_SCANS_REQUIRED_TO_EVICT - 1; i++) {
            mWakeupLock.update(networks);
            assertFalse(mWakeupLock.isInitialized());
        }
        mWakeupLock.update(networks);
        assertTrue(mWakeupLock.isInitialized());
    }

    /**
     * Verify that calling update after {@link WakeupLock#MAX_LOCK_TIME_MILLIS} milliseconds sets
     * the lock to be initialized and does not add the scans to the lock.
     */
    @Test
    public void verifyInitializingLockByTimeout() {
        when(mClock.getElapsedSinceBootMillis())
                .thenReturn(0L, WakeupLock.MAX_LOCK_TIME_MILLIS + 1);
        mWakeupLock.setLock(Collections.emptyList());
        assertFalse(mWakeupLock.isInitialized());

        List<ScanResultMatchInfo> networks = Collections.singletonList(mNetwork1);
        mWakeupLock.update(networks);
        assertTrue(mWakeupLock.isInitialized());
        assertTrue(mWakeupLock.isUnlocked());
    }

    /**
     * Verify that addToLock saves to the store if it changes the contents of the lock.
     */
    @Test
    public void addToLockSavesToStore() {
        mWakeupLock.setLock(Collections.emptyList());

        List<ScanResultMatchInfo> networks = Collections.singletonList(mNetwork1);
        mWakeupLock.update(networks);

        // want 2 invocations, once for setLock(), once for addToLock
        verify(mWifiConfigManager, times(2)).saveToStore(false);
    }

    /**
     * Verify that the WakeupLock is not empty immediately after being initialized with networks.
     */
    @Test
    public void verifyNotEmptyWhenSetWithNetworkList() {
        setLockAndInitializeByTimeout(Arrays.asList(mNetwork1, mNetwork2));
        assertFalse(mWakeupLock.isUnlocked());
    }

    /**
     * Verify that the WakeupLock is unlocked when initialized with an empty list.
     */
    @Test
    public void isEmptyWhenInitializedWithEmptyList() {
        setLockAndInitializeByTimeout(Collections.emptyList());
        assertTrue(mWakeupLock.isUnlocked());
    }

    /**
     * Verify that setting the lock clears out previous entries.
     */
    @Test
    public void setLockClearsPreviousNetworks() {
        setLockAndInitializeByTimeout(Collections.singletonList(mNetwork1));
        assertFalse(mWakeupLock.isUnlocked());

        setLockAndInitializeByTimeout(Collections.emptyList());
        assertTrue(mWakeupLock.isUnlocked());
    }

    /**
     * Updating the lock should evict scan results that haven't been seen in
     * {@link WakeupLock#CONSECUTIVE_MISSED_SCANS_REQUIRED_TO_EVICT} scans.
     */
    @Test
    public void updateShouldRemoveNetworksAfterConsecutiveMissedScans() {
        setLockAndInitializeByTimeout(Collections.singletonList(mNetwork1));

        updateEnoughTimesToEvictWithAsserts(Collections.singletonList(mNetwork2));

        assertTrue(mWakeupLock.isUnlocked());
    }

    /**
     * Ensure that missed scans must be consecutive in order to evict networks from lock.
     */
    @Test
    public void updateWithLockedNetworkShouldResetRequiredNumberOfScans() {
        List<ScanResultMatchInfo> lockedNetworks = Collections.singletonList(mNetwork1);
        List<ScanResultMatchInfo> updateNetworks = Collections.singletonList(mNetwork2);

        setLockAndInitializeByTimeout(lockedNetworks);

        // one update without network
        mWakeupLock.update(updateNetworks);
        // one update with network
        mWakeupLock.update(lockedNetworks);

        updateEnoughTimesToEvictWithAsserts(updateNetworks);

        assertTrue(mWakeupLock.isUnlocked());
    }

    /**
     * Once a network is removed from the lock, it should not be reset even if it's seen again.
     */
    @Test
    public void updateWithLockedNetworkAfterItIsRemovedDoesNotReset() {
        List<ScanResultMatchInfo> lockedNetworks = Collections.singletonList(mNetwork1);
        setLockAndInitializeByTimeout(lockedNetworks);

        updateEnoughTimesToEvictWithAsserts(Collections.emptyList());

        assertTrue(mWakeupLock.isUnlocked());
        mWakeupLock.update(lockedNetworks);
        assertTrue(mWakeupLock.isUnlocked());
    }

    /**
     * Verify that networks can be incrementally removed from the lock. Their counters should be
     * independent.
     */
    @Test
    public void networksCanBeRemovedIncrementallyFromLock() {
        List<ScanResultMatchInfo> lockedNetworks = Arrays.asList(mNetwork1, mNetwork2);
        setLockAndInitializeByTimeout(lockedNetworks);

        updateEnoughTimesToEvictWithAsserts(Collections.singletonList(mNetwork1));
        assertFalse(mWakeupLock.isUnlocked());

        updateEnoughTimesToEvictWithAsserts(Collections.singletonList(mNetwork2));
        assertTrue(mWakeupLock.isUnlocked());
    }

    /**
     * Verify that initializing the lock persists the SSID list to the config store.
     */
    @Test
    public void initializeShouldSaveSsidsToStore() {
        setLockAndInitializeByTimeout(Collections.singletonList(mNetwork1));
        verify(mWifiConfigManager).saveToStore(eq(false));
    }

    /**
     * Verify that update saves to store if the lock changes.
     */
    @Test
    public void updateShouldOnlySaveIfLockChanges() {
        setLockAndInitializeByTimeout(Collections.singletonList(mNetwork1));
        updateEnoughTimesToEvictWithoutAsserts(Collections.emptyList());

        // need exactly 2 invocations: 1 for initialize, 1 for successful update
        verify(mWifiConfigManager, times(2)).saveToStore(eq(false));
    }

    /**
     * Verify that update does not save to store if the lock does not change.
     */
    @Test
    public void updateShouldNotSaveIfLockDoesNotChange() {
        List<ScanResultMatchInfo> networks = Collections.singletonList(mNetwork1);
        setLockAndInitializeByTimeout(networks);
        verify(mWifiConfigManager, times(1)).saveToStore(anyBoolean());
        mWakeupLock.update(networks);
    }

    /**
     * Verify that on unlock, records the unlock event with WifiWakeMetrics with the correct number
     * of scans.
     */
    @Test
    public void unlockingShouldRecordEventInMetrics() {
        when(mClock.getElapsedSinceBootMillis())
                .thenReturn(0L, WakeupLock.MAX_LOCK_TIME_MILLIS + 1);
        List<ScanResultMatchInfo> networks = Collections.singletonList(mNetwork1);
        mWakeupLock.setLock(networks);
        for (int i = 0; i < WakeupLock.CONSECUTIVE_MISSED_SCANS_REQUIRED_TO_EVICT; i++) {
            mWakeupLock.update(Collections.emptyList());
        }
        verify(mWifiWakeMetrics).recordUnlockEvent(
                WakeupLock.CONSECUTIVE_MISSED_SCANS_REQUIRED_TO_EVICT /* numScans */);
    }

    private void setLockAndInitializeByTimeout(Collection<ScanResultMatchInfo> networks) {
        when(mClock.getElapsedSinceBootMillis())
                .thenReturn(0L, WakeupLock.MAX_LOCK_TIME_MILLIS + 1);
        mWakeupLock.setLock(networks);
        mWakeupLock.update(networks);
        assertTrue(mWakeupLock.isInitialized());
    }
}
