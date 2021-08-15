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

import static com.android.server.wifi.CarrierNetworkNotifier.DEFAULT_REPEAT_DELAY_SEC;
import static com.android.server.wifi.ConnectToNetworkNotificationBuilder.ACTION_CONNECT_TO_NETWORK;
import static com.android.server.wifi.ConnectToNetworkNotificationBuilder.ACTION_PICK_WIFI_NETWORK;
import static com.android.server.wifi.ConnectToNetworkNotificationBuilder.ACTION_PICK_WIFI_NETWORK_AFTER_CONNECT_FAILURE;
import static com.android.server.wifi.ConnectToNetworkNotificationBuilder.ACTION_USER_DISMISSED_NOTIFICATION;
import static com.android.server.wifi.ConnectToNetworkNotificationBuilder.AVAILABLE_NETWORK_NOTIFIER_TAG;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.app.NotificationManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.database.ContentObserver;
import android.net.Uri;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiManager;
import android.os.Message;
import android.os.RemoteException;
import android.os.UserHandle;
import android.os.UserManager;
import android.os.test.TestLooper;
import android.provider.Settings;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.nano.WifiMetricsProto;
import com.android.server.wifi.nano.WifiMetricsProto.ConnectToNetworkNotificationAndActionCount;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.util.ArrayList;
import java.util.List;

/**
 * Unit tests for {@link CarrierNetworkNotifier}.
 */
@SmallTest
public class CarrierNetworkNotifierTest {

    private static final String TEST_SSID_1 = "Test SSID 1";
    private static final String TEST_SSID_2 = "Test SSID 2";
    private static final int MIN_RSSI_LEVEL = -127;
    private static final String CARRIER_NET_NOTIFIER_TAG = CarrierNetworkNotifier.TAG;
    private static final int TEST_NETWORK_ID = 42;

    @Mock private Context mContext;
    @Mock private Resources mResources;
    @Mock private FrameworkFacade mFrameworkFacade;
    @Mock private WifiMetrics mWifiMetrics;
    @Mock private Clock mClock;
    @Mock private WifiConfigStore mWifiConfigStore;
    @Mock private WifiConfigManager mWifiConfigManager;
    @Mock private NotificationManager mNotificationManager;
    @Mock private ClientModeImpl mClientModeImpl;
    @Mock private ConnectToNetworkNotificationBuilder mNotificationBuilder;
    @Mock private UserManager mUserManager;
    private CarrierNetworkNotifier mNotificationController;
    private TestLooper mLooper;
    private BroadcastReceiver mBroadcastReceiver;
    private ContentObserver mContentObserver;
    private ScanResult mDummyNetwork;
    private List<ScanDetail> mCarrierNetworks;


    /** Initialize objects before each test run. */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        when(mContext.getSystemService(Context.NOTIFICATION_SERVICE))
                .thenReturn(mNotificationManager);
        when(mFrameworkFacade.getIntegerSetting(mContext,
                Settings.Global.WIFI_CARRIER_NETWORKS_AVAILABLE_NOTIFICATION_ON, 1)).thenReturn(1);
        when(mFrameworkFacade.getIntegerSetting(mContext,
                Settings.Global.WIFI_NETWORKS_AVAILABLE_REPEAT_DELAY, DEFAULT_REPEAT_DELAY_SEC))
                .thenReturn(DEFAULT_REPEAT_DELAY_SEC);
        when(mContext.getSystemService(Context.USER_SERVICE))
                .thenReturn(mUserManager);
        when(mContext.getResources()).thenReturn(mResources);
        mDummyNetwork = new ScanResult();
        mDummyNetwork.SSID = TEST_SSID_1;
        mDummyNetwork.capabilities = "[ESS]";
        mDummyNetwork.level = MIN_RSSI_LEVEL;
        mCarrierNetworks = new ArrayList<>();
        mCarrierNetworks.add(new ScanDetail(mDummyNetwork, null /* networkDetail */));

        mLooper = new TestLooper();
        mNotificationController = new CarrierNetworkNotifier(
                mContext, mLooper.getLooper(), mFrameworkFacade, mClock, mWifiMetrics,
                mWifiConfigManager, mWifiConfigStore, mClientModeImpl, mNotificationBuilder);
        ArgumentCaptor<BroadcastReceiver> broadcastReceiverCaptor =
                ArgumentCaptor.forClass(BroadcastReceiver.class);
        verify(mContext).registerReceiver(broadcastReceiverCaptor.capture(), any(), any(), any());
        mBroadcastReceiver = broadcastReceiverCaptor.getValue();
        ArgumentCaptor<ContentObserver> observerCaptor =
                ArgumentCaptor.forClass(ContentObserver.class);
        verify(mFrameworkFacade).registerContentObserver(eq(mContext), any(Uri.class), eq(true),
                observerCaptor.capture());
        mContentObserver = observerCaptor.getValue();
        mNotificationController.handleScreenStateChanged(true);
        when(mWifiConfigManager.addOrUpdateNetwork(any(), anyInt()))
                .thenReturn(new NetworkUpdateResult(TEST_NETWORK_ID));
    }

    /**
     * On {@link CarrierNetworkNotifier} construction, WifiMetrics should track setting state.
     */
    @Test
    public void onCreate_setWifiNetworksAvailableNotificationSettingState() {
        verify(mWifiMetrics).setIsWifiNetworksAvailableNotificationEnabled(CARRIER_NET_NOTIFIER_TAG,
                true);
    }

    /**
     * When feature setting is toggled, WifiMetrics should track the disabled setting state.
     */
    @Test
    public void onFeatureDisable_setWifiNetworksAvailableNotificationSettingDisabled() {
        when(mFrameworkFacade.getIntegerSetting(mContext,
                Settings.Global.WIFI_CARRIER_NETWORKS_AVAILABLE_NOTIFICATION_ON, 1)).thenReturn(0);
        mContentObserver.onChange(false);

        verify(mWifiMetrics).setIsWifiNetworksAvailableNotificationEnabled(CARRIER_NET_NOTIFIER_TAG,
                false);
    }

    /**
     * When scan results with carrier networks are handled, a notification is posted.
     */
    @Test
    public void handleScanResults_hasCarrierNetworks_notificationDisplayed() {
        mNotificationController.handleScanResults(mCarrierNetworks);

        verify(mNotificationBuilder).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK);
        verify(mNotificationManager).notify(anyInt(), any());
    }

    /**
     * When scan results with no carrier networks are handled, a notification is not posted.
     */
    @Test
    public void handleScanResults_emptyList_notificationNotDisplayed() {
        mNotificationController.handleScanResults(new ArrayList<>());

        verify(mNotificationManager, never()).notify(anyInt(), any());
    }

    /**
     * When the feature is disabled, no notifications are posted.
     */
    @Test
    public void handleScanResults_featureDisabled_notificationNotDisplayed() {
        when(mFrameworkFacade.getIntegerSetting(mContext,
                Settings.Global.WIFI_CARRIER_NETWORKS_AVAILABLE_NOTIFICATION_ON, 1)).thenReturn(0);
        mContentObserver.onChange(false);
        mNotificationController.handleScanResults(new ArrayList<>());

        verify(mNotificationManager, never()).notify(anyInt(), any());
    }

    /**
     * When a notification is showing and scan results with no carrier networks are handled, the
     * notification is cleared.
     */
    @Test
    public void handleScanResults_notificationShown_emptyList_notificationCleared() {
        mNotificationController.handleScanResults(mCarrierNetworks);

        verify(mNotificationBuilder).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK);
        verify(mNotificationManager).notify(anyInt(), any());

        mNotificationController.handleScanResults(new ArrayList<>());

        verify(mNotificationManager).cancel(anyInt());
    }

    /**
     * When a notification is showing and no recommendation is made for the new scan results, the
     * notification is cleared.
     */
    @Test
    public void handleScanResults_notificationShown_noRecommendation_notificationCleared() {
        mNotificationController.handleScanResults(mCarrierNetworks);

        verify(mNotificationBuilder).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK);
        verify(mNotificationManager).notify(anyInt(), any());

        mCarrierNetworks.clear();
        mNotificationController.handleScanResults(mCarrierNetworks);

        verify(mNotificationManager).cancel(anyInt());
    }

    /**
     * When a notification is showing, screen is off, and scan results with no carrier networks are
     * handled, the notification is cleared.
     */
    @Test
    public void handleScanResults_notificationShown_screenOff_emptyList_notificationCleared() {
        mNotificationController.handleScanResults(mCarrierNetworks);

        verify(mNotificationBuilder).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK);
        verify(mNotificationManager).notify(anyInt(), any());

        mNotificationController.handleScreenStateChanged(false);
        mNotificationController.handleScanResults(new ArrayList<>());

        verify(mNotificationManager).cancel(anyInt());
    }

    /**
     * When {@link CarrierNetworkNotifier#clearPendingNotification(boolean)} is called and a
     * notification is shown, clear the notification.
     */
    @Test
    public void clearPendingNotification_clearsNotificationIfOneIsShowing() {
        mNotificationController.handleScanResults(mCarrierNetworks);

        verify(mNotificationBuilder).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK);
        verify(mNotificationManager).notify(anyInt(), any());

        mNotificationController.clearPendingNotification(true);

        verify(mNotificationManager).cancel(anyInt());
    }

    /**
     * When {@link CarrierNetworkNotifier#clearPendingNotification(boolean)} is called and a
     * notification was not previously shown, do not clear the notification.
     */
    @Test
    public void clearPendingNotification_doesNotClearNotificationIfNoneShowing() {
        mNotificationController.clearPendingNotification(true);

        verify(mNotificationManager, never()).cancel(anyInt());
    }

    /**
     * When screen is off and notification is not displayed, notification is not posted on handling
     * new scan results with carrier networks.
     */
    @Test
    public void screenOff_notificationNotShowing_handleScanResults_notificationNotDisplayed() {
        mNotificationController.handleScreenStateChanged(false);
        mNotificationController.handleScanResults(mCarrierNetworks);

        verify(mNotificationManager, never()).notify(anyInt(), any());
    }

    /**
     * When screen is off and notification is displayed, the notification can be updated with a new
     * recommendation.
     */
    @Test
    public void screenOff_notificationShowing_handleScanResults_recommendationCanBeUpdated() {
        mNotificationController.handleScanResults(mCarrierNetworks);

        verify(mNotificationBuilder).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK);
        verify(mNotificationManager).notify(anyInt(), any());

        ScanResult newNetwork = new ScanResult();
        newNetwork.SSID = TEST_SSID_2;
        mDummyNetwork.capabilities = "[ESS]";
        mDummyNetwork.level = MIN_RSSI_LEVEL + 1;
        mCarrierNetworks.add(new ScanDetail(newNetwork, null /* networkDetail */));

        mNotificationController.handleScreenStateChanged(false);
        mNotificationController.handleScanResults(mCarrierNetworks);

        // Recommendation changed
        verify(mNotificationBuilder).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, newNetwork);
        verify(mWifiMetrics).incrementNumNetworkRecommendationUpdates(CARRIER_NET_NOTIFIER_TAG);
        verify(mNotificationManager, times(2)).notify(anyInt(), any());
    }

    /**
     * When a notification is posted and cleared without resetting delay, the next scan with carrier
     * networks should not post another notification.
     */
    @Test
    public void postNotification_clearNotificationWithoutDelayReset_shouldNotPostNotification() {
        mNotificationController.handleScanResults(mCarrierNetworks);

        verify(mNotificationBuilder).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK);
        verify(mNotificationManager).notify(anyInt(), any());

        mNotificationController.clearPendingNotification(false);

        verify(mNotificationManager).cancel(anyInt());

        mNotificationController.handleScanResults(mCarrierNetworks);

        // no new notification posted
        verify(mNotificationManager).notify(anyInt(), any());
    }

    /**
     * When a notification is posted and cleared without resetting delay, the next scan with carrier
     * networks should post a notification.
     */
    @Test
    public void postNotification_clearNotificationWithDelayReset_shouldPostNotification() {
        mNotificationController.handleScanResults(mCarrierNetworks);

        verify(mNotificationBuilder).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK);
        verify(mNotificationManager).notify(anyInt(), any());

        mNotificationController.clearPendingNotification(true);

        mNotificationController.handleScanResults(mCarrierNetworks);

        verify(mNotificationBuilder, times(2)).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, mDummyNetwork);
        verify(mWifiMetrics, times(2)).incrementConnectToNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK);
        verify(mNotificationManager, times(2)).notify(anyInt(), any());
    }

    private Intent createIntent(String action) {
        return new Intent(action).putExtra(AVAILABLE_NETWORK_NOTIFIER_TAG,
                CARRIER_NET_NOTIFIER_TAG);
    }

    /**
     * When user dismissed notification and there is a recommended network, network ssid should be
     * blacklisted.
     */
    @Test
    public void userDismissedNotification_shouldBlacklistNetwork() {
        mNotificationController.handleScanResults(mCarrierNetworks);

        verify(mNotificationBuilder).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK);
        verify(mNotificationManager).notify(anyInt(), any());

        mBroadcastReceiver.onReceive(mContext, createIntent(ACTION_USER_DISMISSED_NOTIFICATION));

        verify(mWifiConfigManager).saveToStore(false /* forceWrite */);

        mNotificationController.clearPendingNotification(true);
        List<ScanDetail> scanResults = mCarrierNetworks;
        mNotificationController.handleScanResults(scanResults);

        verify(mWifiMetrics).setNetworkRecommenderBlacklistSize(CARRIER_NET_NOTIFIER_TAG, 1);
    }

    /**
     * When the user chooses to connect to recommended network, network ssid should be
     * blacklisted so that if the user removes the network in the future the same notification
     * won't show up again.
     */
    @Test
    public void userConnectedNotification_shouldBlacklistNetwork() {
        mNotificationController.handleScanResults(mCarrierNetworks);

        verify(mNotificationBuilder).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK);
        verify(mNotificationManager).notify(anyInt(), any());

        mBroadcastReceiver.onReceive(mContext, createIntent(ACTION_CONNECT_TO_NETWORK));

        verify(mWifiConfigManager).saveToStore(false /* forceWrite */);
        verify(mWifiMetrics).setNetworkRecommenderBlacklistSize(CARRIER_NET_NOTIFIER_TAG, 1);

        List<ScanDetail> scanResults = mCarrierNetworks;
        mNotificationController.handleScanResults(scanResults);
    }

    /**
     * When a notification is posted and cleared without resetting delay, after the delay has passed
     * the next scan with carrier networks should post a notification.
     */
    @Test
    public void delaySet_delayPassed_shouldPostNotification() {
        mNotificationController.handleScanResults(mCarrierNetworks);

        verify(mNotificationBuilder).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK);
        verify(mNotificationManager).notify(anyInt(), any());

        mNotificationController.clearPendingNotification(false);

        // twice the delay time passed
        when(mClock.getWallClockMillis()).thenReturn(DEFAULT_REPEAT_DELAY_SEC * 1000L * 2);

        mNotificationController.handleScanResults(mCarrierNetworks);

        verify(mNotificationBuilder, times(2)).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, mDummyNetwork);
        verify(mWifiMetrics, times(2)).incrementConnectToNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK);
        verify(mNotificationManager, times(2)).notify(anyInt(), any());
    }

    /** Verifies that {@link UserManager#DISALLOW_CONFIG_WIFI} disables the feature. */
    @Test
    public void userHasDisallowConfigWifiRestriction_notificationNotDisplayed() {
        when(mUserManager.hasUserRestriction(UserManager.DISALLOW_CONFIG_WIFI, UserHandle.CURRENT))
                .thenReturn(true);

        mNotificationController.handleScanResults(mCarrierNetworks);

        verify(mNotificationManager, never()).notify(anyInt(), any());
    }

    /** Verifies that {@link UserManager#DISALLOW_CONFIG_WIFI} clears the showing notification. */
    @Test
    public void userHasDisallowConfigWifiRestriction_showingNotificationIsCleared() {
        mNotificationController.handleScanResults(mCarrierNetworks);

        verify(mNotificationBuilder).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK);
        verify(mNotificationManager).notify(anyInt(), any());

        when(mUserManager.hasUserRestriction(UserManager.DISALLOW_CONFIG_WIFI, UserHandle.CURRENT))
                .thenReturn(true);

        mNotificationController.handleScanResults(mCarrierNetworks);

        verify(mNotificationManager).cancel(anyInt());
    }

    /**
     * {@link ConnectToNetworkNotificationBuilder#ACTION_CONNECT_TO_NETWORK} does not connect to
     * any network if the initial notification is not showing.
     */
    @Test
    public void actionConnectToNetwork_notificationNotShowing_doesNothing() {
        mBroadcastReceiver.onReceive(mContext, createIntent(ACTION_CONNECT_TO_NETWORK));

        verify(mClientModeImpl, never()).sendMessage(any(Message.class));
    }

    /**
     * {@link ConnectToNetworkNotificationBuilder#ACTION_CONNECT_TO_NETWORK} connects to the
     * currently recommended network if it exists.
     */
    @Test
    public void actionConnectToNetwork_currentRecommendationExists_connectsAndPostsNotification() {
        mNotificationController.handleScanResults(mCarrierNetworks);

        // Initial Notification
        verify(mNotificationBuilder).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK);
        verify(mNotificationManager).notify(anyInt(), any());

        mBroadcastReceiver.onReceive(mContext, createIntent(ACTION_CONNECT_TO_NETWORK));

        verify(mClientModeImpl).sendMessage(any(Message.class));
        // Connecting Notification
        verify(mNotificationBuilder).createNetworkConnectingNotification(CARRIER_NET_NOTIFIER_TAG,
                mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_CONNECTING_TO_NETWORK);
        verify(mWifiMetrics).incrementConnectToNetworkNotificationAction(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK,
                ConnectToNetworkNotificationAndActionCount.ACTION_CONNECT_TO_NETWORK);
        verify(mNotificationManager, times(2)).notify(anyInt(), any());
    }

    /**
     * {@link ConnectToNetworkNotificationBuilder#ACTION_PICK_WIFI_NETWORK} opens Wi-Fi settings
     * if the recommendation notification is showing.
     */
    @Test
    public void actionPickWifiNetwork_currentRecommendationExists_opensWifiSettings() {
        mNotificationController.handleScanResults(mCarrierNetworks);

        // Initial Notification
        verify(mNotificationBuilder).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK);
        verify(mNotificationManager).notify(anyInt(), any());

        mBroadcastReceiver.onReceive(mContext, createIntent(ACTION_PICK_WIFI_NETWORK));

        ArgumentCaptor<Intent> pickerIntentCaptor = ArgumentCaptor.forClass(Intent.class);
        verify(mContext).startActivity(pickerIntentCaptor.capture());
        assertEquals(pickerIntentCaptor.getValue().getAction(), Settings.ACTION_WIFI_SETTINGS);
        verify(mWifiMetrics).incrementConnectToNetworkNotificationAction(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK,
                ConnectToNetworkNotificationAndActionCount.ACTION_PICK_WIFI_NETWORK);
    }

    /**
     * {@link CarrierNetworkNotifier#handleWifiConnected(String ssid)} does not post connected
     * notification if the connecting notification is not showing
     */
    @Test
    public void networkConnectionSuccess_wasNotInConnectingFlow_doesNothing() {
        mNotificationController.handleWifiConnected(TEST_SSID_1);

        verify(mNotificationManager, never()).notify(anyInt(), any());
        verify(mWifiMetrics, never()).incrementConnectToNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_CONNECTED_TO_NETWORK);
    }

    /**
     * {@link CarrierNetworkNotifier#handleWifiConnected(String ssid)} clears notification
     * that is not connecting.
     */
    @Test
    public void networkConnectionSuccess_wasShowingNotification_clearsNotification() {
        mNotificationController.handleScanResults(mCarrierNetworks);

        // Initial Notification
        verify(mNotificationBuilder).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK);
        verify(mNotificationManager).notify(anyInt(), any());

        mNotificationController.handleWifiConnected(TEST_SSID_1);

        verify(mNotificationManager).cancel(anyInt());
    }

    /**
     * {@link CarrierNetworkNotifier#handleWifiConnected(String ssid)} posts the connected
     * notification if the connecting notification is showing.
     */
    @Test
    public void networkConnectionSuccess_wasInConnectingFlow_postsConnectedNotification() {
        mNotificationController.handleScanResults(mCarrierNetworks);

        // Initial Notification
        verify(mNotificationBuilder).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK);
        verify(mNotificationManager).notify(anyInt(), any());

        mBroadcastReceiver.onReceive(mContext, createIntent(ACTION_CONNECT_TO_NETWORK));

        // Connecting Notification
        verify(mNotificationBuilder).createNetworkConnectingNotification(CARRIER_NET_NOTIFIER_TAG,
                mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_CONNECTING_TO_NETWORK);
        verify(mWifiMetrics).incrementConnectToNetworkNotificationAction(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK,
                ConnectToNetworkNotificationAndActionCount.ACTION_CONNECT_TO_NETWORK);
        verify(mNotificationManager, times(2)).notify(anyInt(), any());

        mNotificationController.handleWifiConnected(TEST_SSID_1);

        // Connected Notification
        verify(mNotificationBuilder).createNetworkConnectedNotification(CARRIER_NET_NOTIFIER_TAG,
                mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_CONNECTED_TO_NETWORK);
        verify(mNotificationManager, times(3)).notify(anyInt(), any());
    }

    /**
     * {@link CarrierNetworkNotifier#handleConnectionFailure()} posts the Failed to Connect
     * notification if the connecting notification is showing.
     */
    @Test
    public void networkConnectionFailure_wasNotInConnectingFlow_doesNothing() {
        mNotificationController.handleConnectionFailure();

        verify(mNotificationManager, never()).notify(anyInt(), any());
        verify(mWifiMetrics, never()).incrementConnectToNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_FAILED_TO_CONNECT);
    }

    /**
     * {@link CarrierNetworkNotifier#handleConnectionFailure()} posts the Failed to Connect
     * notification if the connecting notification is showing.
     */
    @Test
    public void networkConnectionFailure_wasInConnectingFlow_postsFailedToConnectNotification() {
        mNotificationController.handleScanResults(mCarrierNetworks);

        // Initial Notification
        verify(mNotificationBuilder).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK);
        verify(mNotificationManager).notify(anyInt(), any());

        mBroadcastReceiver.onReceive(mContext, createIntent(ACTION_CONNECT_TO_NETWORK));

        // Connecting Notification
        verify(mNotificationBuilder).createNetworkConnectingNotification(CARRIER_NET_NOTIFIER_TAG,
                mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_CONNECTING_TO_NETWORK);
        verify(mWifiMetrics).incrementConnectToNetworkNotificationAction(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK,
                ConnectToNetworkNotificationAndActionCount.ACTION_CONNECT_TO_NETWORK);
        verify(mNotificationManager, times(2)).notify(anyInt(), any());

        mNotificationController.handleConnectionFailure();

        // Failed to Connect Notification
        verify(mNotificationBuilder).createNetworkFailedNotification(CARRIER_NET_NOTIFIER_TAG);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_FAILED_TO_CONNECT);
        verify(mNotificationManager, times(3)).notify(anyInt(), any());
    }

    /**
     * When a {@link WifiManager#CONNECT_NETWORK_FAILED} is received from the connection callback
     * of {@link ClientModeImpl#sendMessage(Message)}, a Failed to Connect notification should
     * be posted. On tapping this notification, Wi-Fi Settings should be launched.
     */
    @Test
    public void connectionFailedCallback_postsFailedToConnectNotification() throws RemoteException {
        mNotificationController.handleScanResults(mCarrierNetworks);

        // Initial Notification
        verify(mNotificationBuilder).createConnectToAvailableNetworkNotification(
                CARRIER_NET_NOTIFIER_TAG, mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK);
        verify(mNotificationManager).notify(anyInt(), any());

        mBroadcastReceiver.onReceive(mContext, createIntent(ACTION_CONNECT_TO_NETWORK));

        verify(mWifiMetrics).setNominatorForNetwork(TEST_NETWORK_ID,
                WifiMetricsProto.ConnectionEvent.NOMINATOR_CARRIER);

        ArgumentCaptor<Message> connectMessageCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mClientModeImpl).sendMessage(connectMessageCaptor.capture());
        Message connectMessage = connectMessageCaptor.getValue();

        // Connecting Notification
        verify(mNotificationBuilder).createNetworkConnectingNotification(CARRIER_NET_NOTIFIER_TAG,
                mDummyNetwork);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_CONNECTING_TO_NETWORK);
        verify(mWifiMetrics).incrementConnectToNetworkNotificationAction(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_RECOMMEND_NETWORK,
                ConnectToNetworkNotificationAndActionCount.ACTION_CONNECT_TO_NETWORK);
        verify(mNotificationManager, times(2)).notify(anyInt(), any());

        Message connectFailedMsg = Message.obtain();
        connectFailedMsg.what = WifiManager.CONNECT_NETWORK_FAILED;
        connectMessage.replyTo.send(connectFailedMsg);
        mLooper.dispatchAll();

        // Failed to Connect Notification
        verify(mNotificationBuilder).createNetworkFailedNotification(CARRIER_NET_NOTIFIER_TAG);
        verify(mWifiMetrics).incrementConnectToNetworkNotification(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_FAILED_TO_CONNECT);
        verify(mWifiMetrics).incrementNumNetworkConnectMessageFailedToSend(
                CARRIER_NET_NOTIFIER_TAG);
        verify(mNotificationManager, times(3)).notify(anyInt(), any());

        mBroadcastReceiver.onReceive(mContext,
                createIntent(ACTION_PICK_WIFI_NETWORK_AFTER_CONNECT_FAILURE));

        ArgumentCaptor<Intent> pickerIntentCaptor = ArgumentCaptor.forClass(Intent.class);
        verify(mContext).startActivity(pickerIntentCaptor.capture());
        assertEquals(pickerIntentCaptor.getValue().getAction(), Settings.ACTION_WIFI_SETTINGS);
        verify(mWifiMetrics).incrementConnectToNetworkNotificationAction(CARRIER_NET_NOTIFIER_TAG,
                ConnectToNetworkNotificationAndActionCount.NOTIFICATION_FAILED_TO_CONNECT,
                ConnectToNetworkNotificationAndActionCount
                        .ACTION_PICK_WIFI_NETWORK_AFTER_CONNECT_FAILURE);
    }

    private List<ScanDetail> createCarrierScanResults(String... ssids) {
        List<ScanDetail> scanResults = new ArrayList<>();
        for (String ssid : ssids) {
            ScanResult scanResult = new ScanResult();
            scanResult.SSID = ssid;
            scanResult.capabilities = "[ESS]";
            scanResults.add(new ScanDetail(scanResult, null /* networkDetail */));
        }
        return scanResults;
    }

    /** If list of carrier networks contain only one network, that network should be returned. */
    @Test
    public void onlyNetworkIsRecommended() {
        List<ScanDetail> scanResults = createCarrierScanResults(TEST_SSID_1);
        scanResults.get(0).getScanResult().level = MIN_RSSI_LEVEL;

        ScanResult actual = mNotificationController.recommendNetwork(scanResults);
        ScanResult expected = scanResults.get(0).getScanResult();
        assertEquals(expected, actual);
    }

    /** Verifies that the network with the highest rssi is recommended. */
    @Test
    public void networkWithHighestRssiIsRecommended() {
        List<ScanDetail> scanResults = createCarrierScanResults(TEST_SSID_1, TEST_SSID_2);
        scanResults.get(0).getScanResult().level = MIN_RSSI_LEVEL;
        scanResults.get(1).getScanResult().level = MIN_RSSI_LEVEL + 1;

        ScanResult actual = mNotificationController.recommendNetwork(scanResults);
        ScanResult expected = scanResults.get(1).getScanResult();
        assertEquals(expected, actual);
    }

    /**
     * If the best available carrier network is blacklisted, no network should be recommended.
     */
    @Test
    public void blacklistBestNetworkSsid_shouldNeverRecommendNetwork() {
        // Add TEST_SSID_1 to blacklist
        userDismissedNotification_shouldBlacklistNetwork();

        List<ScanDetail> scanResults = createCarrierScanResults(mDummyNetwork.SSID, TEST_SSID_2);
        scanResults.get(0).getScanResult().level = MIN_RSSI_LEVEL + 1;
        scanResults.get(1).getScanResult().level = MIN_RSSI_LEVEL;

        ScanResult actual = mNotificationController.recommendNetwork(scanResults);
        assertNull(actual);
    }

    /**
     * Test null input is handled
     */
    @Test
    public void removeNetworkFromBlacklist_handlesNull() {
        mNotificationController.handleWifiConnected(null);
        verify(mWifiConfigManager, never()).saveToStore(false /* forceWrite */);
    }

    /**
     * If the blacklist didn't change then there is no need to continue further.
     */
    @Test
    public void removeNetworkFromBlacklist_returnsEarlyIfNothingIsRemoved() {
        mNotificationController.handleWifiConnected(TEST_SSID_1);
        verify(mWifiConfigManager, never()).saveToStore(false /* forceWrite */);
    }

    /**
     * If we connected to a blacklisted network, then remove it from the blacklist.
     */
    @Test
    public void connectToNetwork_shouldRemoveSsidFromBlacklist() {
        // Add TEST_SSID_1 to blacklist
        userDismissedNotification_shouldBlacklistNetwork();

        // Simulate the user connecting to TEST_SSID_1 and verify it is removed from the blacklist
        mNotificationController.handleWifiConnected(mDummyNetwork.SSID);
        verify(mWifiConfigManager, times(2)).saveToStore(false /* forceWrite */);
        verify(mWifiMetrics).setNetworkRecommenderBlacklistSize(CARRIER_NET_NOTIFIER_TAG, 0);
        ScanResult actual = mNotificationController.recommendNetwork(mCarrierNetworks);
        ScanResult expected = mCarrierNetworks.get(0).getScanResult();
        assertEquals(expected, actual);
    }
}
