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
package com.android.internal.telephony;

import static com.android.internal.telephony.TelephonyIntents.ACTION_CARRIER_SIGNAL_PCO_VALUE;
import static com.android.internal.telephony.TelephonyIntents
        .ACTION_CARRIER_SIGNAL_REQUEST_NETWORK_FAILED;
import static com.android.internal.telephony.TelephonyTestUtils.waitForMs;

import static org.junit.Assert.assertEquals;
import static org.mockito.ArgumentMatchers.anyLong;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyInt;
import static org.mockito.Matchers.argThat;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.content.Intent;
import android.content.pm.ResolveInfo;
import android.os.HandlerThread;
import android.os.Message;
import android.os.PersistableBundle;
import android.telephony.CarrierConfigManager;
import android.test.suitebuilder.annotation.SmallTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Objects;

public class CarrierSignalAgentTest extends TelephonyTest {

    private CarrierSignalAgent mCarrierSignalAgentUT;
    private PersistableBundle mBundle;
    private CarrierSignalAgentHandler mCarrierSignalAgentHandler;
    private static final String PCO_RECEIVER = "pak/PCO_RECEIVER";
    private static final String DC_ERROR_RECEIVER = "pak/DC_ERROR_RECEIVER";
    @Mock
    ResolveInfo mResolveInfo;

    private class CarrierSignalAgentHandler extends HandlerThread {

        private CarrierSignalAgentHandler(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            mCarrierSignalAgentUT = new CarrierSignalAgent(mPhone);
            setReady(true);
        }
    }

    @Before
    public void setUp() throws Exception {
        logd("CarrierSignalAgentTest +Setup!");
        super.setUp(getClass().getSimpleName());
        mBundle = mContextFixture.getCarrierConfigBundle();
        mCarrierSignalAgentHandler = new CarrierSignalAgentHandler(getClass().getSimpleName());
        mCarrierSignalAgentHandler.start();
        waitUntilReady();
        logd("CarrierSignalAgentTest -Setup!");
    }

    @After
    public void tearDown() throws Exception {
        mCarrierSignalAgentHandler.quit();
        super.tearDown();
    }

    @Test
    @SmallTest
    public void testNotifyManifestReceivers() throws Exception {
        // Broadcast count
        int count = 0;
        Intent intent = new Intent(ACTION_CARRIER_SIGNAL_PCO_VALUE);
        mBundle.putStringArray(
                CarrierConfigManager.KEY_CARRIER_APP_WAKE_SIGNAL_CONFIG_STRING_ARRAY,
                new String[]{PCO_RECEIVER + ":" + ACTION_CARRIER_SIGNAL_PCO_VALUE,
                        DC_ERROR_RECEIVER + ":" + ACTION_CARRIER_SIGNAL_PCO_VALUE
                });

        // Verify no broadcast has been sent without carrier config
        mCarrierSignalAgentUT.notifyCarrierSignalReceivers(intent);
        ArgumentCaptor<Intent> mCaptorIntent = ArgumentCaptor.forClass(Intent.class);
        verify(mContext, times(count)).sendBroadcast(mCaptorIntent.capture());

        // Trigger carrier config reloading
        mContext.sendBroadcast(new Intent(CarrierConfigManager.ACTION_CARRIER_CONFIG_CHANGED));
        waitForMs(50);
        count++;

        // Verify no broadcast has been sent due to no manifest receivers
        mCarrierSignalAgentUT.notifyCarrierSignalReceivers(intent);
        verify(mContext, times(count)).sendBroadcast(mCaptorIntent.capture());

        // Verify broadcast has been sent to two different registered manifest receivers
        doReturn(new ArrayList<>(Arrays.asList(mResolveInfo)))
                .when(mPackageManager).queryBroadcastReceivers((Intent) any(), anyInt());
        mCarrierSignalAgentUT.notifyCarrierSignalReceivers(intent);
        count += 2;
        mCaptorIntent = ArgumentCaptor.forClass(Intent.class);
        verify(mContext, times(count)).sendBroadcast(mCaptorIntent.capture());

        logd(mCaptorIntent.getAllValues().toString());
        Intent capturedIntent = mCaptorIntent.getAllValues().get(1);
        assertEquals(ACTION_CARRIER_SIGNAL_PCO_VALUE, capturedIntent.getAction());
        assertEquals(DC_ERROR_RECEIVER, capturedIntent.getComponent().flattenToString());

        capturedIntent = mCaptorIntent.getAllValues().get(2);
        assertEquals(ACTION_CARRIER_SIGNAL_PCO_VALUE, capturedIntent.getAction());
        assertEquals(PCO_RECEIVER, capturedIntent.getComponent().flattenToString());
    }

    @Test
    @SmallTest
    public void testNotifyRuntimeReceivers() throws Exception {
        // Broadcast count
        int count = 0;
        Intent intent = new Intent(ACTION_CARRIER_SIGNAL_PCO_VALUE);
        mBundle.putStringArray(
                CarrierConfigManager.KEY_CARRIER_APP_NO_WAKE_SIGNAL_CONFIG_STRING_ARRAY,
                new String[]{PCO_RECEIVER + ":" + ACTION_CARRIER_SIGNAL_PCO_VALUE});

        // Verify no broadcast without carrier configs
        mCarrierSignalAgentUT.notifyCarrierSignalReceivers(intent);
        ArgumentCaptor<Intent> mCaptorIntent = ArgumentCaptor.forClass(Intent.class);
        verify(mContext, times(count)).sendBroadcast(mCaptorIntent.capture());

        // Trigger carrier config reloading
        mContext.sendBroadcast(new Intent(CarrierConfigManager.ACTION_CARRIER_CONFIG_CHANGED));
        waitForMs(50);
        count++;

        // Verify broadcast has been sent to registered components
        mCarrierSignalAgentUT.notifyCarrierSignalReceivers(intent);
        verify(mContext, times(++count)).sendBroadcast(mCaptorIntent.capture());
        assertEquals(ACTION_CARRIER_SIGNAL_PCO_VALUE,
                mCaptorIntent.getValue().getAction());
        assertEquals(PCO_RECEIVER, mCaptorIntent.getValue().getComponent().flattenToString());

        // Verify no broadcast has been sent to manifest receivers (bad config)
        doReturn(new ArrayList<>(Arrays.asList(mResolveInfo)))
                .when(mPackageManager).queryBroadcastReceivers((Intent) any(), anyInt());
        mCaptorIntent = ArgumentCaptor.forClass(Intent.class);
        mCarrierSignalAgentUT.notifyCarrierSignalReceivers(intent);
        verify(mContext, times(count)).sendBroadcast(mCaptorIntent.capture());
    }

    @Test
    @SmallTest
    public void testNotify() {
        // Broadcast count
        int count = 0;
        mBundle.putStringArray(
                CarrierConfigManager.KEY_CARRIER_APP_WAKE_SIGNAL_CONFIG_STRING_ARRAY,
                new String[]{ PCO_RECEIVER + ":" + ACTION_CARRIER_SIGNAL_PCO_VALUE });
        mBundle.putStringArray(
                CarrierConfigManager.KEY_CARRIER_APP_NO_WAKE_SIGNAL_CONFIG_STRING_ARRAY,
                new String[]{ PCO_RECEIVER + ":"
                        + ACTION_CARRIER_SIGNAL_REQUEST_NETWORK_FAILED });
        // Only wake signal is declared in the manifest
        doReturn(new ArrayList<>(Arrays.asList(mResolveInfo)))
                .when(mPackageManager).queryBroadcastReceivers(
                argThat(o -> Objects.equals(o.getAction(), ACTION_CARRIER_SIGNAL_PCO_VALUE)),
                anyInt());

        mContext.sendBroadcast(new Intent(CarrierConfigManager.ACTION_CARRIER_CONFIG_CHANGED));
        count++;
        waitForMs(50);

        // Wake signal for PAK_PCO_RECEIVER
        mCarrierSignalAgentUT.notifyCarrierSignalReceivers(
                new Intent(ACTION_CARRIER_SIGNAL_PCO_VALUE));
        ArgumentCaptor<Intent> mCaptorIntent = ArgumentCaptor.forClass(Intent.class);
        verify(mContext, times(++count)).sendBroadcast(mCaptorIntent.capture());
        assertEquals(ACTION_CARRIER_SIGNAL_PCO_VALUE, mCaptorIntent.getValue().getAction());
        assertEquals(PCO_RECEIVER, mCaptorIntent.getValue().getComponent().flattenToString());

        // No wake signal for PAK_PCO_RECEIVER
        mCarrierSignalAgentUT.notifyCarrierSignalReceivers(
                new Intent(ACTION_CARRIER_SIGNAL_REQUEST_NETWORK_FAILED));
        mCaptorIntent = ArgumentCaptor.forClass(Intent.class);
        verify(mContext, times(++count)).sendBroadcast(mCaptorIntent.capture());
        assertEquals(ACTION_CARRIER_SIGNAL_REQUEST_NETWORK_FAILED,
                mCaptorIntent.getValue().getAction());
        assertEquals(PCO_RECEIVER, mCaptorIntent.getValue().getComponent().flattenToString());

        // Both wake and no-wake signals are declared in the manifest
        doReturn(new ArrayList<>(Arrays.asList(mResolveInfo)))
                .when(mPackageManager).queryBroadcastReceivers((Intent) any(), anyInt());
        mCarrierSignalAgentUT.notifyCarrierSignalReceivers(
                new Intent(ACTION_CARRIER_SIGNAL_PCO_VALUE));
        mCaptorIntent = ArgumentCaptor.forClass(Intent.class);
        verify(mContext, times(++count)).sendBroadcast(mCaptorIntent.capture());
        mCarrierSignalAgentUT.notifyCarrierSignalReceivers(
                new Intent(ACTION_CARRIER_SIGNAL_REQUEST_NETWORK_FAILED));
        mCaptorIntent = ArgumentCaptor.forClass(Intent.class);
        verify(mContext, times(count)).sendBroadcast(mCaptorIntent.capture());

        // Neither wake nor no-wake signals are declared in the manifest
        doReturn(new ArrayList<>()).when(mPackageManager).queryBroadcastReceivers((Intent) any(),
                anyInt());
        mCarrierSignalAgentUT.notifyCarrierSignalReceivers(
                new Intent(ACTION_CARRIER_SIGNAL_PCO_VALUE));
        mCaptorIntent = ArgumentCaptor.forClass(Intent.class);
        verify(mContext, times(count)).sendBroadcast(mCaptorIntent.capture());
        mCarrierSignalAgentUT.notifyCarrierSignalReceivers(
                new Intent(ACTION_CARRIER_SIGNAL_REQUEST_NETWORK_FAILED));
        mCaptorIntent = ArgumentCaptor.forClass(Intent.class);
        verify(mContext, times(++count)).sendBroadcast(mCaptorIntent.capture());
    }


    @Test
    @SmallTest
    public void testCarrierConfigChange() {
        // default config value
        mBundle.putStringArray(
                CarrierConfigManager.KEY_CARRIER_APP_WAKE_SIGNAL_CONFIG_STRING_ARRAY,
                new String[]{ PCO_RECEIVER + ":" + ACTION_CARRIER_SIGNAL_PCO_VALUE + ","
                        + ACTION_CARRIER_SIGNAL_REQUEST_NETWORK_FAILED });
        mContext.sendBroadcast(new Intent(CarrierConfigManager.ACTION_CARRIER_CONFIG_CHANGED));
        waitForMs(50);
        // verify no reset action on initial config load
        verify(mCarrierActionAgent, times(0)).sendMessageAtTime(any(Message.class), anyLong());

        // new carrier config with different receiver intent order
        mBundle.putStringArray(
                CarrierConfigManager.KEY_CARRIER_APP_WAKE_SIGNAL_CONFIG_STRING_ARRAY,
                new String[]{ PCO_RECEIVER + ":" + ACTION_CARRIER_SIGNAL_REQUEST_NETWORK_FAILED
                        + "," + ACTION_CARRIER_SIGNAL_PCO_VALUE});
        mContext.sendBroadcast(new Intent(CarrierConfigManager.ACTION_CARRIER_CONFIG_CHANGED));
        waitForMs(50);
        // verify no reset action for the same config (different order)
        verify(mCarrierActionAgent, times(0)).sendMessageAtTime(any(Message.class), anyLong());

        // new different config value
        mBundle.putStringArray(
                CarrierConfigManager.KEY_CARRIER_APP_WAKE_SIGNAL_CONFIG_STRING_ARRAY,
                new String[]{ DC_ERROR_RECEIVER + ":" + ACTION_CARRIER_SIGNAL_REQUEST_NETWORK_FAILED
                        + "," + ACTION_CARRIER_SIGNAL_PCO_VALUE});
        mContext.sendBroadcast(new Intent(CarrierConfigManager.ACTION_CARRIER_CONFIG_CHANGED));
        waitForMs(50);
        // verify there is no reset action
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mCarrierActionAgent, times(1))
                .sendMessageAtTime(messageArgumentCaptor.capture(), anyLong());
        assertEquals(CarrierActionAgent.CARRIER_ACTION_RESET,
                messageArgumentCaptor.getValue().what);
    }
}
