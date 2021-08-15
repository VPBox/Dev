/*
 * Copyright (C) 2019 The Android Open Source Project
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

package com.android.internal.telephony.dataconnection;

import static com.android.internal.telephony.TelephonyTestUtils.waitForMs;

import static org.junit.Assert.assertEquals;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Matchers.anyLong;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.os.AsyncResult;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.telephony.AccessNetworkConstants;
import android.telephony.AccessNetworkConstants.AccessNetworkType;
import android.telephony.data.ApnSetting;
import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.telephony.TelephonyTest;
import com.android.internal.telephony.dataconnection.AccessNetworksManager.QualifiedNetworks;
import com.android.internal.telephony.dataconnection.TransportManager.HandoverParams;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;

import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

public class TransportManagerTest extends TelephonyTest {
    private static final int EVENT_HANDOVER_NEEDED = 1;

    @Mock
    private Handler mTestHandler;

    private TransportManager mTransportManager;

    private TransportManagerTestHandler mTransportManagerTestHandler;

    private class TransportManagerTestHandler extends HandlerThread {

        private TransportManagerTestHandler(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            mTransportManager = new TransportManager(mPhone);
            setReady(true);
        }
    }

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());
        mTransportManagerTestHandler = new TransportManagerTestHandler(TAG);
        mTransportManagerTestHandler.start();
        waitUntilReady();
    }

    @After
    public void tearDown() throws Exception {
        mTransportManagerTestHandler.quit();
        mTransportManagerTestHandler.join();
        super.tearDown();
    }

    @Test
    @SmallTest
    public void testHandoverNeeded() throws Exception {
        mTransportManager.registerForHandoverNeededEvent(mTestHandler, EVENT_HANDOVER_NEEDED);

        // Initial qualified networks
        List<QualifiedNetworks> networkList = new ArrayList<>(Arrays.asList(
                new QualifiedNetworks(ApnSetting.TYPE_IMS,
                        new int[]{AccessNetworkType.EUTRAN, AccessNetworkType.UTRAN,
                                AccessNetworkType.IWLAN})));
        mTransportManager.obtainMessage(1 /* EVENT_QUALIFIED_NETWORKS_CHANGED */,
                new AsyncResult(null, networkList, null)).sendToTarget();
        waitForMs(100);
        // Verify handover needed event was not sent
        verify(mTestHandler, never()).sendMessageAtTime(any(Message.class), anyLong());

        assertEquals(AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                mTransportManager.getCurrentTransport(ApnSetting.TYPE_IMS));

        // Now change the order of qualified networks by putting IWLAN first
        networkList = new ArrayList<>(Arrays.asList(
                new QualifiedNetworks(ApnSetting.TYPE_IMS,
                        new int[]{AccessNetworkType.IWLAN, AccessNetworkType.UTRAN,
                                AccessNetworkType.EUTRAN})));
        mTransportManager.obtainMessage(1 /* EVENT_QUALIFIED_NETWORKS_CHANGED */,
                new AsyncResult(null, networkList, null)).sendToTarget();
        waitForMs(100);

        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);

        // Verify handover needed event was sent and the the target transport is WLAN.
        verify(mTestHandler, times(1)).sendMessageAtTime(messageArgumentCaptor.capture(),
                anyLong());
        Message message = messageArgumentCaptor.getValue();
        assertEquals(EVENT_HANDOVER_NEEDED, message.what);
        AsyncResult ar = (AsyncResult) message.obj;
        HandoverParams params = (HandoverParams) ar.result;
        assertEquals(ApnSetting.TYPE_IMS, params.apnType);
        assertEquals(AccessNetworkConstants.TRANSPORT_TYPE_WLAN, params.targetTransport);

        // Notify handover succeeded.
        params.callback.onCompleted(true, false);
        assertEquals(AccessNetworkConstants.TRANSPORT_TYPE_WLAN,
                mTransportManager.getCurrentTransport(ApnSetting.TYPE_IMS));

        // Now change the order of qualified networks by putting UTRAN first
        networkList = new ArrayList<>(Arrays.asList(
                new QualifiedNetworks(ApnSetting.TYPE_IMS,
                        new int[]{AccessNetworkType.UTRAN, AccessNetworkType.EUTRAN,
                                AccessNetworkType.IWLAN})));
        mTransportManager.obtainMessage(1 /* EVENT_QUALIFIED_NETWORKS_CHANGED */,
                new AsyncResult(null, networkList, null)).sendToTarget();
        waitForMs(100);

        messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);

        // Verify handover needed event was sent and the the target transport is WWAN.
        verify(mTestHandler, times(2)).sendMessageAtTime(messageArgumentCaptor.capture(),
                anyLong());
        message = messageArgumentCaptor.getValue();
        assertEquals(EVENT_HANDOVER_NEEDED, message.what);
        ar = (AsyncResult) message.obj;
        params = (HandoverParams) ar.result;
        assertEquals(ApnSetting.TYPE_IMS, params.apnType);
        assertEquals(AccessNetworkConstants.TRANSPORT_TYPE_WWAN, params.targetTransport);

        // The transport should not change before handover complete callback is called.
        assertEquals(AccessNetworkConstants.TRANSPORT_TYPE_WLAN,
                mTransportManager.getCurrentTransport(ApnSetting.TYPE_IMS));
        // Notify handover succeeded.
        params.callback.onCompleted(true, false);
        assertEquals(AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                mTransportManager.getCurrentTransport(ApnSetting.TYPE_IMS));
    }

    @Test
    @SmallTest
    public void testHandoverNotNeeded() throws Exception {
        mTransportManager.registerForHandoverNeededEvent(mTestHandler, EVENT_HANDOVER_NEEDED);

        // Initial qualified networks
        List<QualifiedNetworks> networkList = new ArrayList<>(Arrays.asList(
                new QualifiedNetworks(ApnSetting.TYPE_IMS,
                        new int[]{AccessNetworkType.EUTRAN, AccessNetworkType.UTRAN,
                                AccessNetworkType.IWLAN})));
        mTransportManager.obtainMessage(1 /* EVENT_QUALIFIED_NETWORKS_CHANGED */,
                new AsyncResult(null, networkList, null)).sendToTarget();
        waitForMs(100);
        // Verify handover needed event was not sent
        verify(mTestHandler, never()).sendMessageAtTime(any(Message.class), anyLong());

        // Now change the order of qualified networks by swapping EUTRAN and UTRAN.
        networkList = new ArrayList<>(Arrays.asList(
                new QualifiedNetworks(ApnSetting.TYPE_IMS,
                        new int[]{AccessNetworkType.UTRAN, AccessNetworkType.EUTRAN,
                                AccessNetworkType.IWLAN})));
        mTransportManager.obtainMessage(1 /* EVENT_QUALIFIED_NETWORKS_CHANGED */,
                new AsyncResult(null, networkList, null)).sendToTarget();
        waitForMs(100);
        // Verify handover needed event was not sent
        verify(mTestHandler, never()).sendMessageAtTime(any(Message.class), anyLong());
    }

    private LinkedList<List<QualifiedNetworks>> getAvailableNetworksList() throws Exception {
        Field f = TransportManager.class.getDeclaredField("mAvailableNetworksList");
        f.setAccessible(true);
        return (LinkedList<List<QualifiedNetworks>>) f.get(mTransportManager);
    }

    @Test
    @SmallTest
    public void testBackToBackHandoverNeeded() throws Exception {
        mTransportManager.registerForHandoverNeededEvent(mTestHandler, EVENT_HANDOVER_NEEDED);

        // Initial qualified networks
        List<QualifiedNetworks> networkList = new ArrayList<>(Arrays.asList(
                new QualifiedNetworks(ApnSetting.TYPE_IMS,
                        new int[]{AccessNetworkType.EUTRAN, AccessNetworkType.UTRAN,
                                AccessNetworkType.IWLAN})));
        mTransportManager.obtainMessage(1 /* EVENT_QUALIFIED_NETWORKS_CHANGED */,
                new AsyncResult(null, networkList, null)).sendToTarget();
        waitForMs(100);
        // Verify handover needed event was not sent
        verify(mTestHandler, never()).sendMessageAtTime(any(Message.class), anyLong());

        assertEquals(AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                mTransportManager.getCurrentTransport(ApnSetting.TYPE_IMS));

        // Now change the order of qualified networks by putting IWLAN first
        networkList = new ArrayList<>(Arrays.asList(
                new QualifiedNetworks(ApnSetting.TYPE_IMS,
                        new int[]{AccessNetworkType.IWLAN, AccessNetworkType.UTRAN,
                                AccessNetworkType.EUTRAN})));
        mTransportManager.obtainMessage(1 /* EVENT_QUALIFIED_NETWORKS_CHANGED */,
                new AsyncResult(null, networkList, null)).sendToTarget();
        waitForMs(100);

        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);

        // Verify handover needed event was sent and the the target transport is WLAN.
        verify(mTestHandler, times(1)).sendMessageAtTime(messageArgumentCaptor.capture(),
                anyLong());
        Message message = messageArgumentCaptor.getValue();
        assertEquals(EVENT_HANDOVER_NEEDED, message.what);
        AsyncResult ar = (AsyncResult) message.obj;
        HandoverParams params = (HandoverParams) ar.result;
        assertEquals(ApnSetting.TYPE_IMS, params.apnType);
        assertEquals(AccessNetworkConstants.TRANSPORT_TYPE_WLAN, params.targetTransport);

        // Before handover is completed, update the available networks again.
        // This time change the order of qualified networks by putting EUTRAN first
        networkList = new ArrayList<>(Arrays.asList(
                new QualifiedNetworks(ApnSetting.TYPE_IMS,
                        new int[]{AccessNetworkType.EUTRAN, AccessNetworkType.UTRAN,
                                AccessNetworkType.IWLAN})));
        mTransportManager.obtainMessage(1 /* EVENT_QUALIFIED_NETWORKS_CHANGED */,
                new AsyncResult(null, networkList, null)).sendToTarget();
        waitForMs(100);

        // Verify handover needed event was sent only once (for the previous change)
        verify(mTestHandler, times(1)).sendMessageAtTime(messageArgumentCaptor.capture(),
                anyLong());

        LinkedList<List<QualifiedNetworks>> listQueue = getAvailableNetworksList();
        // Verify the list has been queued.
        assertEquals(1, listQueue.size());

        // Notify handover succeeded.
        params.callback.onCompleted(true, false);
        assertEquals(AccessNetworkConstants.TRANSPORT_TYPE_WLAN,
                mTransportManager.getCurrentTransport(ApnSetting.TYPE_IMS));
        waitForMs(100);

        listQueue = getAvailableNetworksList();
        // Verify the queue is empty.
        assertEquals(0, listQueue.size());

        // Verify handover 2nd needed event was sent
        verify(mTestHandler, times(2)).sendMessageAtTime(messageArgumentCaptor.capture(),
                anyLong());
    }
}
