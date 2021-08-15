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
package com.android.internal.telephony;

import static org.junit.Assert.assertEquals;
import static org.mockito.Matchers.anyLong;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.content.Intent;
import android.database.ContentObserver;
import android.net.Uri;
import android.os.AsyncResult;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.provider.Settings;
import android.provider.Telephony;
import android.test.mock.MockContentResolver;
import android.test.suitebuilder.annotation.SmallTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;

public class CarrierActionAgentTest extends TelephonyTest {
    private CarrierActionAgent mCarrierActionAgentUT;
    private FakeContentResolver mFakeContentResolver;
    private static int DATA_CARRIER_ACTION_EVENT = 0;
    private static int RADIO_CARRIER_ACTION_EVENT = 1;
    private static int TEST_TIMEOUT = 5000;
    private CarrierActionAgentHandler mCarrierActionAgentHandler;
    @Mock
    private Handler mDataActionHandler;
    @Mock
    private Handler mRadioActionHandler;

    private class FakeContentResolver extends MockContentResolver {
        @Override
        public void notifyChange(Uri uri, ContentObserver observer, boolean syncToNetwork) {
            super.notifyChange(uri, observer, syncToNetwork);
            logd("onChanged(uri=" + uri + ")" + observer);
            if (observer != null) {
                observer.dispatchChange(false, uri);
            } else {
                mCarrierActionAgentUT.getContentObserver().dispatchChange(false, uri);
            }
        }
    }

    private class CarrierActionAgentHandler extends HandlerThread {

        private CarrierActionAgentHandler(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            mCarrierActionAgentUT = new CarrierActionAgent(mPhone);
            mCarrierActionAgentUT.registerForCarrierAction(
                    CarrierActionAgent.CARRIER_ACTION_SET_METERED_APNS_ENABLED, mDataActionHandler,
                    DATA_CARRIER_ACTION_EVENT, null, false);
            mCarrierActionAgentUT.registerForCarrierAction(
                    CarrierActionAgent.CARRIER_ACTION_SET_RADIO_ENABLED, mRadioActionHandler,
                    RADIO_CARRIER_ACTION_EVENT, null, false);
            setReady(true);
        }
    }

    @Before
    public void setUp() throws Exception {
        logd("CarrierActionAgentTest +Setup!");
        super.setUp(getClass().getSimpleName());
        mFakeContentResolver = new FakeContentResolver();
        doReturn(mFakeContentResolver).when(mContext).getContentResolver();
        mCarrierActionAgentHandler = new CarrierActionAgentHandler(getClass().getSimpleName());
        mCarrierActionAgentHandler.start();
        waitUntilReady();
        logd("CarrierActionAgentTest -Setup!");
    }

    @Test
    @SmallTest
    public void testCarrierActionResetOnAPM() {
        // setting observer register at sim loading
        final Intent intent = new Intent(TelephonyIntents.ACTION_SIM_STATE_CHANGED);
        intent.putExtra(IccCardConstants.INTENT_KEY_ICC_STATE,
                IccCardConstants.INTENT_VALUE_ICC_LOADED);
        mContext.sendBroadcast(intent);
        waitForHandlerAction(mCarrierActionAgentUT, TEST_TIMEOUT);

        // no carrier actions triggered from sim loading since there are same as the current one
        ArgumentCaptor<Message> message = ArgumentCaptor.forClass(Message.class);
        verify(mDataActionHandler, times(0)).sendMessageAtTime(message.capture(), anyLong());
        verify(mRadioActionHandler, times(0)).sendMessageAtTime(message.capture(), anyLong());

        // disable metered apns and radio
        mCarrierActionAgentUT.carrierActionSetRadioEnabled(false);
        mCarrierActionAgentUT.carrierActionSetMeteredApnsEnabled(false);
        waitForHandlerAction(mCarrierActionAgentUT, TEST_TIMEOUT);
        waitForHandlerAction(mCarrierActionAgentUT, TEST_TIMEOUT);
        verify(mDataActionHandler, times(1)).sendMessageAtTime(message.capture(), anyLong());
        assertEquals(DATA_CARRIER_ACTION_EVENT, message.getValue().what);
        assertEquals(false, ((AsyncResult) message.getValue().obj).result);
        verify(mRadioActionHandler, times(1)).sendMessageAtTime(message.capture(), anyLong());
        assertEquals(RADIO_CARRIER_ACTION_EVENT, message.getValue().what);
        assertEquals(false, ((AsyncResult) message.getValue().obj).result);

        // simulate APM change from off -> on
        Settings.Global.putInt(mFakeContentResolver, Settings.Global.AIRPLANE_MODE_ON, 1);
        mFakeContentResolver.notifyChange(
                Settings.Global.getUriFor(Settings.Global.AIRPLANE_MODE_ON), null);
        waitForHandlerAction(mCarrierActionAgentUT, TEST_TIMEOUT);
        waitForHandlerAction(mCarrierActionAgentUT, TEST_TIMEOUT);
        waitForHandlerAction(mCarrierActionAgentUT, TEST_TIMEOUT);

        // carrier actions triggered from APM
        verify(mDataActionHandler, times(2)).sendMessageAtTime(message.capture(), anyLong());
        assertEquals(DATA_CARRIER_ACTION_EVENT, message.getValue().what);
        assertEquals(true, ((AsyncResult) message.getValue().obj).result);

        verify(mRadioActionHandler, times(2)).sendMessageAtTime(message.capture(), anyLong());
        assertEquals(RADIO_CARRIER_ACTION_EVENT, message.getValue().what);
        assertEquals(true, ((AsyncResult) message.getValue().obj).result);
    }

    @Test
    @SmallTest
    public void testCarrierActionResetOnAPNChange() {
        // Setting observer register at sim loading
        final Intent intent = new Intent(TelephonyIntents.ACTION_SIM_STATE_CHANGED);
        intent.putExtra(IccCardConstants.INTENT_KEY_ICC_STATE,
                IccCardConstants.INTENT_VALUE_ICC_LOADED);
        mContext.sendBroadcast(intent);
        waitForHandlerAction(mCarrierActionAgentUT, TEST_TIMEOUT);

        // no carrier actions triggered from sim loading since there are same as the current one
        ArgumentCaptor<Message> message = ArgumentCaptor.forClass(Message.class);
        verify(mDataActionHandler, times(0)).sendMessageAtTime(message.capture(), anyLong());
        verify(mRadioActionHandler, times(0)).sendMessageAtTime(message.capture(), anyLong());

        // disable metered apns and radio
        mCarrierActionAgentUT.carrierActionSetRadioEnabled(false);
        mCarrierActionAgentUT.carrierActionSetMeteredApnsEnabled(false);
        waitForHandlerAction(mCarrierActionAgentUT, TEST_TIMEOUT);
        waitForHandlerAction(mCarrierActionAgentUT, TEST_TIMEOUT);

        verify(mDataActionHandler, times(1)).sendMessageAtTime(message.capture(), anyLong());
        assertEquals(DATA_CARRIER_ACTION_EVENT, message.getValue().what);
        assertEquals(false, ((AsyncResult) message.getValue().obj).result);

        verify(mRadioActionHandler, times(1)).sendMessageAtTime(message.capture(), anyLong());
        assertEquals(RADIO_CARRIER_ACTION_EVENT, message.getValue().what);
        assertEquals(false, ((AsyncResult) message.getValue().obj).result);

        // Simulate APN change
        mFakeContentResolver.notifyChange(Telephony.Carriers.CONTENT_URI, null);
        waitForHandlerAction(mCarrierActionAgentUT, TEST_TIMEOUT);
        waitForHandlerAction(mCarrierActionAgentUT, TEST_TIMEOUT);
        waitForHandlerAction(mCarrierActionAgentUT, TEST_TIMEOUT);

        // Carrier actions triggered from APN change
        verify(mDataActionHandler, times(2)).sendMessageAtTime(message.capture(), anyLong());
        assertEquals(DATA_CARRIER_ACTION_EVENT, message.getValue().what);
        assertEquals(true, ((AsyncResult) message.getValue().obj).result);

        verify(mRadioActionHandler, times(2)).sendMessageAtTime(message.capture(), anyLong());
        assertEquals(RADIO_CARRIER_ACTION_EVENT, message.getValue().what);
        assertEquals(true, ((AsyncResult) message.getValue().obj).result);
    }

    @After
    public void tearDown() throws Exception {
        Settings.Global.putInt(mFakeContentResolver, Settings.Global.AIRPLANE_MODE_ON, 0);
        mCarrierActionAgentHandler.quit();
        super.tearDown();
    }
}
