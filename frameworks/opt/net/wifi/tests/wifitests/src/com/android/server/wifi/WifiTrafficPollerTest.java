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

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;

import android.net.wifi.ITrafficStateCallback;
import android.net.wifi.WifiManager;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.test.TestLooper;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.util.ExternalCallbackTracker;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

/**
 * Unit tests for {@link com.android.server.wifi.WifiTrafficPoller}.
 */
@SmallTest
public class WifiTrafficPollerTest {
    public static final String TAG = "WifiTrafficPollerTest";

    private TestLooper mLooper;
    private WifiTrafficPoller mWifiTrafficPoller;
    private final static long DEFAULT_PACKET_COUNT = 10;
    private final static long TX_PACKET_COUNT = 40;
    private final static long RX_PACKET_COUNT = 50;
    private static final int TEST_TRAFFIC_STATE_CALLBACK_IDENTIFIER = 14;

    @Mock IBinder mAppBinder;
    @Mock ITrafficStateCallback mTrafficStateCallback;
    @Mock ExternalCallbackTracker<ITrafficStateCallback> mCallbackTracker;

    /**
     * Called before each test
     */
    @Before
    public void setUp() throws Exception {
        // Ensure looper exists
        mLooper = new TestLooper();
        MockitoAnnotations.initMocks(this);

        mWifiTrafficPoller = new WifiTrafficPoller(mLooper.getLooper());

        // Set the current mTxPkts and mRxPkts to DEFAULT_PACKET_COUNT
        mWifiTrafficPoller.notifyOnDataActivity(DEFAULT_PACKET_COUNT, DEFAULT_PACKET_COUNT);
    }

    /**
     * Verify that clients should be notified of activity in case Tx/Rx packet count changes.
     */
    @Test
    public void testClientNotification() throws RemoteException {
        // Register Client to verify that Tx/RX packet message is properly received.
        mWifiTrafficPoller.addCallback(
                mAppBinder, mTrafficStateCallback, TEST_TRAFFIC_STATE_CALLBACK_IDENTIFIER);
        mWifiTrafficPoller.notifyOnDataActivity(TX_PACKET_COUNT, RX_PACKET_COUNT);

        // Client should get the DATA_ACTIVITY_NOTIFICATION
        verify(mTrafficStateCallback).onStateChanged(
                WifiManager.TrafficStateCallback.DATA_ACTIVITY_INOUT);
    }

    /**
     * Verify that remove client should be handled
     */
    @Test
    public void testRemoveClient() throws RemoteException {
        // Register Client to verify that Tx/RX packet message is properly received.
        mWifiTrafficPoller.addCallback(
                mAppBinder, mTrafficStateCallback, TEST_TRAFFIC_STATE_CALLBACK_IDENTIFIER);
        mWifiTrafficPoller.removeCallback(TEST_TRAFFIC_STATE_CALLBACK_IDENTIFIER);
        verify(mAppBinder).unlinkToDeath(any(), anyInt());

        mWifiTrafficPoller.notifyOnDataActivity(TX_PACKET_COUNT, RX_PACKET_COUNT);

        // Client should not get any message after the client is removed.
        verify(mTrafficStateCallback, never()).onStateChanged(anyInt());
    }

    /**
     * Verify that remove client ignores when callback identifier is wrong.
     */
    @Test
    public void testRemoveClientWithWrongIdentifier() throws RemoteException {
        // Register Client to verify that Tx/RX packet message is properly received.
        mWifiTrafficPoller.addCallback(
                mAppBinder, mTrafficStateCallback, TEST_TRAFFIC_STATE_CALLBACK_IDENTIFIER);
        mWifiTrafficPoller.removeCallback(TEST_TRAFFIC_STATE_CALLBACK_IDENTIFIER + 5);
        mLooper.dispatchAll();

        mWifiTrafficPoller.notifyOnDataActivity(TX_PACKET_COUNT, RX_PACKET_COUNT);

        // Client should get the DATA_ACTIVITY_NOTIFICATION
        verify(mTrafficStateCallback).onStateChanged(
                WifiManager.TrafficStateCallback.DATA_ACTIVITY_INOUT);
    }

    /**
     *
     * Verify that traffic poller registers for death notification on adding client.
     */
    @Test
    public void registersForBinderDeathOnAddClient() throws Exception {
        mWifiTrafficPoller.addCallback(
                mAppBinder, mTrafficStateCallback, TEST_TRAFFIC_STATE_CALLBACK_IDENTIFIER);
        verify(mAppBinder).linkToDeath(any(IBinder.DeathRecipient.class), anyInt());
    }

    /**
     *
     * Verify that traffic poller registers for death notification on adding client.
     */
    @Test
    public void addCallbackFailureOnLinkToDeath() throws Exception {
        doThrow(new RemoteException())
                .when(mAppBinder).linkToDeath(any(IBinder.DeathRecipient.class), anyInt());
        mWifiTrafficPoller.addCallback(
                mAppBinder, mTrafficStateCallback, TEST_TRAFFIC_STATE_CALLBACK_IDENTIFIER);
        verify(mAppBinder).linkToDeath(any(IBinder.DeathRecipient.class), anyInt());

        mWifiTrafficPoller.notifyOnDataActivity(TX_PACKET_COUNT, RX_PACKET_COUNT);

        // Client should not get any message callback add failed.
        verify(mTrafficStateCallback, never()).onStateChanged(anyInt());
    }
}
