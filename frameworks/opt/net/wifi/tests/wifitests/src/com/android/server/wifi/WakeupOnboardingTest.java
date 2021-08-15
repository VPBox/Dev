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

import static com.android.server.wifi.WakeupNotificationFactory.ACTION_DISMISS_NOTIFICATION;
import static com.android.server.wifi.WakeupNotificationFactory.ACTION_OPEN_WIFI_PREFERENCES;
import static com.android.server.wifi.WakeupNotificationFactory.ACTION_TURN_OFF_WIFI_WAKE;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.app.NotificationManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Handler;
import android.os.test.TestLooper;
import android.provider.Settings;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.InOrder;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.mockito.MockitoAnnotations;

/** Unit tests for {@link com.android.server.wifi.WakeupOnboarding} */
@SmallTest
public class WakeupOnboardingTest {

    @Mock private Context mContext;
    @Mock private WifiConfigManager mWifiConfigManager;
    @Mock private FrameworkFacade mFrameworkFacade;
    @Mock private WakeupNotificationFactory mWakeupNotificationFactory;
    @Mock private NotificationManager mNotificationManager;

    private TestLooper mLooper;
    private WakeupOnboarding mWakeupOnboarding;

    // convenience method for resetting onboarded status
    private void setOnboardedStatus(boolean isOnboarded) {
        mWakeupOnboarding.getIsOnboadedDataSource().setData(isOnboarded);
    }

    private void setNotificationsShown(int numNotifications) {
        mWakeupOnboarding.getNotificationsDataSource().setData(numNotifications);
    }

    @Before
    public void setUp() {
        MockitoAnnotations.initMocks(this);

        when(mContext.getSystemService(Context.NOTIFICATION_SERVICE))
                .thenReturn(mNotificationManager);

        mLooper = new TestLooper();
        mWakeupOnboarding = new WakeupOnboarding(mContext, mWifiConfigManager, mLooper.getLooper(),
                mFrameworkFacade, mWakeupNotificationFactory);
    }

    /**
     * Verify that the notification shows if the user isn't onboarded.
     */
    @Test
    public void showsNotificationIfNotOnboarded() {
        setOnboardedStatus(false);
        mWakeupOnboarding.maybeShowNotification();

        verify(mNotificationManager).notify(eq(WakeupNotificationFactory.ONBOARD_ID), any());
    }

    /**
     * Verify that the notification does not show if the user is onboarded.
     */
    @Test
    public void doesNotShowNotificationIfAlreadyOnboarded() {
        setOnboardedStatus(true);
        mWakeupOnboarding.maybeShowNotification();

        verify(mNotificationManager, never())
                .notify(eq(WakeupNotificationFactory.ONBOARD_ID), any());
    }

    /**
     * Verify that the notification does not relaunch if it's already showing.
     */
    @Test
    public void doesNotShowNotificationIfAlreadyShowing() {
        setOnboardedStatus(false);
        mWakeupOnboarding.maybeShowNotification();
        mWakeupOnboarding.maybeShowNotification();

        InOrder inOrder = Mockito.inOrder(mNotificationManager);
        inOrder.verify(mNotificationManager)
                .notify(eq(WakeupNotificationFactory.ONBOARD_ID), any());
        inOrder.verifyNoMoreInteractions();
    }

    /**
     * Verify that the user is onboarded when the notification is dismissed.
     */
    @Test
    public void dismissNotificationAction_setsOnboarded() {
        setOnboardedStatus(false);
        assertFalse(mWakeupOnboarding.isOnboarded());

        mWakeupOnboarding.maybeShowNotification();
        ArgumentCaptor<BroadcastReceiver> captor = ArgumentCaptor.forClass(BroadcastReceiver.class);
        verify(mContext).registerReceiver(captor.capture(), any(IntentFilter.class), any(),
                any(Handler.class));
        BroadcastReceiver broadcastReceiver = captor.getValue();

        broadcastReceiver.onReceive(mContext, new Intent(ACTION_DISMISS_NOTIFICATION));

        verify(mNotificationManager).cancel(WakeupNotificationFactory.ONBOARD_ID);
        assertTrue(mWakeupOnboarding.isOnboarded());
    }

    /**
     * Verify that the user is onboarded and Wifi Wake is turned off when the user selects the
     * ACTION_TURN_OFF_WIFI_WAKE action.
     */
    @Test
    public void turnOffWifiWakeAction_setsOnboardedAndTurnsOffWifiWake() {
        setOnboardedStatus(false);
        assertFalse(mWakeupOnboarding.isOnboarded());

        mWakeupOnboarding.maybeShowNotification();
        ArgumentCaptor<BroadcastReceiver> captor = ArgumentCaptor.forClass(BroadcastReceiver.class);
        verify(mContext).registerReceiver(captor.capture(), any(IntentFilter.class), any(),
                any(Handler.class));
        BroadcastReceiver broadcastReceiver = captor.getValue();

        broadcastReceiver.onReceive(mContext, new Intent(ACTION_TURN_OFF_WIFI_WAKE));

        verify(mFrameworkFacade).setIntegerSetting(mContext,
                Settings.Global.WIFI_WAKEUP_ENABLED, 0);

        verify(mNotificationManager).cancel(WakeupNotificationFactory.ONBOARD_ID);
        assertTrue(mWakeupOnboarding.isOnboarded());
    }

    /**
     * Verify that the user is onboarded and sent to WifiSettings when the user selects the
     * ACTION_OPEN_WIFI_SETTINGS action.
     */
    @Test
    public void openWifiSettingsAction_setsOnboardedAndOpensWifiSettings() {
        setOnboardedStatus(false);
        assertFalse(mWakeupOnboarding.isOnboarded());

        mWakeupOnboarding.maybeShowNotification();
        ArgumentCaptor<BroadcastReceiver> captor = ArgumentCaptor.forClass(BroadcastReceiver.class);
        verify(mContext).registerReceiver(captor.capture(), any(IntentFilter.class), any(),
                any(Handler.class));
        BroadcastReceiver broadcastReceiver = captor.getValue();

        broadcastReceiver.onReceive(mContext, new Intent(ACTION_OPEN_WIFI_PREFERENCES));

        verify(mContext).startActivity(any());

        verify(mNotificationManager).cancel(WakeupNotificationFactory.ONBOARD_ID);
        assertTrue(mWakeupOnboarding.isOnboarded());
    }

    /**
     * Verify that onStop() doesn't onboard the user.
     */
    @Test
    public void onStopDismissesNotificationWithoutOnboarding() {
        setOnboardedStatus(false);
        assertFalse(mWakeupOnboarding.isOnboarded());

        mWakeupOnboarding.maybeShowNotification();
        mWakeupOnboarding.onStop();

        verify(mNotificationManager).cancel(WakeupNotificationFactory.ONBOARD_ID);
        assertFalse(mWakeupOnboarding.isOnboarded());
    }

    /**
     * Verify that incrementing the notification count saves to store.
     */
    @Test
    public void setOnboardedSavesToStore() {
        setOnboardedStatus(false);
        mWakeupOnboarding.setOnboarded();
        verify(mWifiConfigManager).saveToStore(false /* forceWrite */);
        assertTrue(mWakeupOnboarding.isOnboarded());
    }

    /**
     * Verify that incrementing the notification count saves to store.
     */
    @Test
    public void incrementingNotificationCountSavesToStore() {
        setOnboardedStatus(false);
        setNotificationsShown(0);
        mWakeupOnboarding.maybeShowNotification();
        verify(mWifiConfigManager).saveToStore(false /* forceWrite */);
    }

    /**
     * Verify that the notification does not show multiple times within 24 hours.
     */
    @Test
    public void doesNotShowMultipleNotificationsWithin24Hours() {
        setOnboardedStatus(false);
        setNotificationsShown(0);

        mWakeupOnboarding.maybeShowNotification(0 /* timestamp */);
        mWakeupOnboarding.onStop();
        mWakeupOnboarding.maybeShowNotification(0 /* timestamp */);

        InOrder inOrder = Mockito.inOrder(mNotificationManager);
        inOrder.verify(mNotificationManager)
                .notify(eq(WakeupNotificationFactory.ONBOARD_ID), any());
        inOrder.verify(mNotificationManager).cancel(WakeupNotificationFactory.ONBOARD_ID);
        inOrder.verifyNoMoreInteractions();
    }

    /**
     * Verify that notification reappears after 24 hours if not onboarded.
     */
    @Test
    public void showsNotificationsOutsideOf24Hours() {
        setOnboardedStatus(false);
        setNotificationsShown(0);

        mWakeupOnboarding.maybeShowNotification(0 /* timestamp */);
        assertFalse(mWakeupOnboarding.isOnboarded());

        mWakeupOnboarding.onStop();
        mWakeupOnboarding.maybeShowNotification(WakeupOnboarding.REQUIRED_NOTIFICATION_DELAY + 1);

        verify(mNotificationManager, times(2))
                .notify(eq(WakeupNotificationFactory.ONBOARD_ID), any());
    }

    /**
     * Verify that the user is onboarded after
     * {@link WakeupOnboarding#NOTIFICATIONS_UNTIL_ONBOARDED} notifications are shown.
     */
    @Test
    public void onboardsUserAfterThreeNotifications() {
        setOnboardedStatus(false);
        setNotificationsShown(WakeupOnboarding.NOTIFICATIONS_UNTIL_ONBOARDED - 1);

        mWakeupOnboarding.maybeShowNotification(0 /* timestamp */);
        assertTrue(mWakeupOnboarding.isOnboarded());
    }
}
