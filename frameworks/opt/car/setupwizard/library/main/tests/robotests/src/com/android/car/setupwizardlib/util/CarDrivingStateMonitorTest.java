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

package com.android.car.setupwizardlib.util;

import static com.google.common.truth.Truth.assertThat;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.robolectric.RuntimeEnvironment.application;

import android.car.Car;
import android.car.CarNotConnectedException;
import android.car.drivingstate.CarUxRestrictions;
import android.car.drivingstate.CarUxRestrictionsManager;

import com.android.car.setupwizardlib.robolectric.BaseRobolectricTest;
import com.android.car.setupwizardlib.shadows.ShadowCar;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.robolectric.RobolectricTestRunner;
import org.robolectric.Shadows;
import org.robolectric.annotation.Config;
import org.robolectric.shadows.ShadowApplication;
import org.robolectric.shadows.ShadowLooper;

/**
 * Tests that the {@link CarDrivingStateMonitor} properly initiates the car restrictions monitoring
 * and calls back to exit listeners on exit.
 */
@RunWith(RobolectricTestRunner.class)
@Config(shadows = ShadowCar.class)
public class CarDrivingStateMonitorTest extends BaseRobolectricTest {
    @Mock
    private CarUxRestrictionsManager mMockRestrictionsManager;
    @Mock
    private CarUxRestrictions mMockRestrictions;

    private CarDrivingStateMonitor mCarDrivingStateMonitor;
    private ShadowApplication mShadowApplication;

    @Before
    public void setupCarState() throws CarNotConnectedException {
        ShadowCar.setIsConnected(false);
        ShadowCar.setCarManager(Car.CAR_UX_RESTRICTION_SERVICE, mMockRestrictionsManager);
        doReturn(mMockRestrictions).when(mMockRestrictionsManager).getCurrentCarUxRestrictions();
        mCarDrivingStateMonitor = CarDrivingStateMonitor.get(application);
        mShadowApplication = Shadows.shadowOf(application);
    }

    @After
    public void resetCarState() {
        ShadowCar.reset();
        CarDrivingStateMonitor.reset(application);
    }

    @Test
    public void testStartMonitor_registersCarDrivingStateMonitorAsListener()
            throws CarNotConnectedException {
        mCarDrivingStateMonitor.startMonitor();
        verify(mMockRestrictionsManager).registerListener(eq(mCarDrivingStateMonitor));
        assertThat(ShadowCar.hasConnected()).isTrue();
    }

    @Test
    public void testStartMonitor_withNoManagerFound_doesNothing()
            throws CarNotConnectedException {
        ShadowCar.setCarManager(Car.CAR_UX_RESTRICTION_SERVICE, null);
        mCarDrivingStateMonitor.startMonitor();
        verify(mMockRestrictionsManager, never()).registerListener(any());
    }

    @Test
    public void testStopMonitor_triggersDisconnectCallback() {
        mCarDrivingStateMonitor.startMonitor();
        ShadowCar.setIsConnected(true);
        mCarDrivingStateMonitor.stopMonitor();
        assertThat(mCarDrivingStateMonitor.mHandler.hasCallbacks(
                mCarDrivingStateMonitor.mDisconnectRunnable)).isTrue();
    }

    @Test
    public void testDisconnect_unregistersCarDrivingStateMonitorAsListener()
            throws CarNotConnectedException {
        mCarDrivingStateMonitor.startMonitor();
        ShadowCar.triggerDisconnect();
        verify(mMockRestrictionsManager).unregisterListener();
    }

    @Test
    public void testOnUxRestrictionsChanged_triggersExit() {
        mCarDrivingStateMonitor.startMonitor();
        doReturn(CarUxRestrictions.UX_RESTRICTIONS_NO_SETUP).when(mMockRestrictions)
                .getActiveRestrictions();
        mCarDrivingStateMonitor.onUxRestrictionsChanged(mMockRestrictions);
        assertThat(mShadowApplication.getBroadcastIntents().get(0).getAction())
                .isEqualTo(CarDrivingStateMonitor.EXIT_BROADCAST_ACTION);
    }

    @Test
    public void testStartMonitorWhileDriving_triggersExit() {
        doReturn(CarUxRestrictions.UX_RESTRICTIONS_NO_SETUP).when(mMockRestrictions)
                .getActiveRestrictions();
        mCarDrivingStateMonitor.startMonitor();
        assertThat(mShadowApplication.getBroadcastIntents().get(0).getAction())
                .isEqualTo(CarDrivingStateMonitor.EXIT_BROADCAST_ACTION);
    }

    @Test
    public void testStartMonitor_clearsStopMonitorRunnable() {
        mCarDrivingStateMonitor.startMonitor();
        ShadowCar.setIsConnected(true);
        mCarDrivingStateMonitor.stopMonitor();
        mCarDrivingStateMonitor.startMonitor();
        assertThat(mCarDrivingStateMonitor.mHandler.hasMessagesOrCallbacks()).isFalse();
    }

    @Test
    public void testDrivingMonitorDisconnects_afterDisconnectTimeoutDelay()
            throws CarNotConnectedException {
        mCarDrivingStateMonitor.startMonitor();
        mCarDrivingStateMonitor.stopMonitor();
        ShadowLooper.runUiThreadTasksIncludingDelayedTasks();
        verify(mMockRestrictionsManager).unregisterListener();
    }

    @Test
    public void testDrivingMonitorDoesNotDisconnect_unlessSameNumberOfStopCallsAsStart() {
        mCarDrivingStateMonitor.startMonitor();
        ShadowCar.setIsConnected(true);
        mCarDrivingStateMonitor.startMonitor();
        mCarDrivingStateMonitor.stopMonitor();
        ShadowLooper.runUiThreadTasksIncludingDelayedTasks();
        assertThat(ShadowCar.hasDisconnected()).isFalse();
        mCarDrivingStateMonitor.stopMonitor();
        ShadowLooper.runUiThreadTasksIncludingDelayedTasks();
        assertThat(ShadowCar.hasDisconnected()).isTrue();
    }
}
