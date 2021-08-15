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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.Matchers.anyBoolean;
import static org.mockito.Matchers.anyInt;
import static org.mockito.Matchers.eq;
import static org.mockito.Matchers.isA;
import static org.mockito.Matchers.isNull;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyChar;
import static org.mockito.Mockito.anyString;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.telephony.PhoneNumberUtils;
import android.telephony.ServiceState;
import android.test.suitebuilder.annotation.SmallTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;

import java.lang.reflect.Field;

public class CallManagerTest extends TelephonyTest {

    @Mock
    GsmCdmaCall mFgCall;
    @Mock
    GsmCdmaCall mBgCall;
    @Mock
    GsmCdmaCall mRingingCall;
    @Mock
    Phone mSecondPhone;

    private CallManagerHandlerThread mCallManagerHandlerThread;
    private Handler mHandler;
    private static final int PHONE_REGISTER_EVENT = 0;

    private class CallManagerHandlerThread extends HandlerThread {
        private CallManagerHandlerThread(String name) {
            super(name);
        }
        @Override
        public void onLooperPrepared() {
            /* CallManager is a static object with private constructor,no need call constructor */
            registerForPhone(mPhone);

            // create a custom handler for the Handler Thread
            mHandler = new Handler(mCallManagerHandlerThread.getLooper()) {
                @Override
                public void handleMessage(Message msg) {
                    switch (msg.what) {
                        case PHONE_REGISTER_EVENT:
                            logd("Phone registered with CallManager");
                            registerForPhone((Phone) msg.obj);
                            setReady(true);
                            break;
                        default:
                            logd("Unknown Event " + msg.what);
                    }
                }
            };

            setReady(true);
        }

        private void registerForPhone(Phone mPhone) {
            CallManager.getInstance().registerPhone(mPhone);
        }
    }

    @Before
    public void setUp() throws Exception {
        super.setUp(this.getClass().getSimpleName());
        restoreInstance(CallManager.class, "INSTANCE", null);
        /* Mock Phone and Call, initially all calls are idle */
        doReturn(ServiceState.STATE_IN_SERVICE).when(mServiceState).getState();
        doReturn(mBgCall).when(mPhone).getBackgroundCall();
        doReturn(mFgCall).when(mPhone).getForegroundCall();
        doReturn(mRingingCall).when(mPhone).getRingingCall();
        doReturn(mPhone).when(mBgCall).getPhone();
        doReturn(mPhone).when(mFgCall).getPhone();
        doReturn(mPhone).when(mRingingCall).getPhone();
        doReturn(Call.State.IDLE).when(mBgCall).getState();
        doReturn(Call.State.IDLE).when(mFgCall).getState();
        doReturn(Call.State.IDLE).when(mRingingCall).getState();
        doReturn(true).when(mBgCall).isIdle();
        doReturn(true).when(mFgCall).isIdle();
        doReturn(true).when(mRingingCall).isIdle();

        mCallManagerHandlerThread = new CallManagerHandlerThread(TAG);
        mCallManagerHandlerThread.start();
        waitUntilReady();
    }

    @After
    public void tearDown() throws Exception {
        CallManager.getInstance().unregisterPhone(mPhone);
        mCallManagerHandlerThread.quit();
        super.tearDown();
    }

    @SmallTest @Test
    public void testSanity() {
        assertEquals(mPhone, CallManager.getInstance().getDefaultPhone());
        assertFalse(CallManager.getInstance().hasActiveBgCall());
        assertFalse(CallManager.getInstance().hasActiveRingingCall());
        assertFalse(CallManager.getInstance().hasActiveFgCall());
        /* return the default phone if there is no any active call */
        assertEquals(mPhone, CallManager.getInstance().getRingingPhone());
        assertEquals(mPhone, CallManager.getInstance().getBgPhone());
        assertEquals(mPhone, CallManager.getInstance().getFgPhone());
    }

    @SmallTest @Test
    public void testBasicDial() throws Exception {
        //verify can dial and dial function of the phone is being triggered
        CallManager.getInstance().dial(mPhone,
                PhoneNumberUtils.stripSeparators("+17005554141"), 0);
        ArgumentCaptor<String> mCaptorString = ArgumentCaptor.forClass(String.class);
        ArgumentCaptor<PhoneInternalInterface.DialArgs> dialArgsCaptor =
                ArgumentCaptor.forClass(PhoneInternalInterface.DialArgs.class);
        verify(mPhone, times(1)).dial(mCaptorString.capture(), dialArgsCaptor.capture());
        assertEquals(PhoneNumberUtils.stripSeparators("+17005554141"),
                mCaptorString.getValue());
        assertEquals(0, dialArgsCaptor.getValue().videoState);
    }

    @SmallTest @Test
    public void testBasicRejectCall() throws Exception {
        //verify can dial and dial function of the phone is being triggered
        CallManager.getInstance().rejectCall(mRingingCall);
        verify(mPhone, times(1)).rejectCall();
    }

    @SmallTest @Test
    public void testSendDtmf() throws Exception {
        CallManager.getInstance().sendDtmf('a');
        verify(mPhone, times(0)).sendDtmf(eq('a'));

        //has active fg Call
        doReturn(false).when(mFgCall).isIdle();
        assertEquals(mFgCall, CallManager.getInstance().getActiveFgCall());
        CallManager.getInstance().sendDtmf('a');
        verify(mPhone, times(1)).sendDtmf(eq('a'));
    }

    @SmallTest @Test
    public void testStartDtmf() throws Exception {
        doReturn(true).when(mFgCall).isIdle();
        assertFalse(CallManager.getInstance().startDtmf('a'));
        verify(mPhone, times(0)).startDtmf(anyChar());

        //has active fg Call
        doReturn(false).when(mFgCall).isIdle();
        assertEquals(mFgCall, CallManager.getInstance().getActiveFgCall());
        assertTrue(CallManager.getInstance().startDtmf('a'));
        verify(mPhone, times(1)).startDtmf('a');
    }

    @SmallTest @Test
    public void testStopDtmf() throws Exception {
        doReturn(true).when(mFgCall).isIdle();
        CallManager.getInstance().stopDtmf();
        verify(mPhone, times(0)).stopDtmf();

        //has active fg Call
        doReturn(false).when(mFgCall).isIdle();
        assertEquals(mPhone, CallManager.getInstance().getFgPhone());
        CallManager.getInstance().stopDtmf();
        verify(mPhone, times(1)).stopDtmf();
    }

    @SmallTest @Test
    public void testSendBurstDtmf() throws Exception {
        doReturn(true).when(mFgCall).isIdle();
        assertFalse(CallManager.getInstance().sendBurstDtmf("12*#", 0, 0, null));
        verify(mPhone, times(0)).sendBurstDtmf(anyString(), anyInt(), anyInt(), (Message) any());

        //has active fg Call
        doReturn(false).when(mFgCall).isIdle();
        assertTrue(CallManager.getInstance().sendBurstDtmf("12*#", 0, 0, null));
        verify(mPhone, times(1)).sendBurstDtmf("12*#", 0, 0, null);
    }

    @SmallTest @Test
    public void testSetGetMute() throws Exception {
        CallManager.getInstance().setMute(false);
        verify(mPhone, times(0)).setMute(anyBoolean());

        //has active fg Call
        doReturn(false).when(mFgCall).isIdle();
        CallManager.getInstance().setMute(false);
        verify(mPhone, times(1)).setMute(false);

        CallManager.getInstance().setMute(true);
        verify(mPhone, times(1)).setMute(true);
    }

    @SmallTest @Test
    public void testFgCallActiveDial() throws Exception {
        /* set Fg/Bg Call state to active, verify CallManager Logical */
        doReturn(false).when(mFgCall).isIdle();
        doReturn(false).when(mBgCall).isIdle();
        assertTrue(CallManager.getInstance().hasActiveFgCall());
        assertTrue(CallManager.getInstance().hasActiveBgCall());
        assertTrue(CallManager.getInstance().hasActiveFgCall(mPhone.getSubId()));
        assertFalse(CallManager.getInstance().hasDisconnectedFgCall());
        /* try dial with non-idle foreground call and background call */
        CallManager.getInstance().dial(mPhone,
                PhoneNumberUtils.stripSeparators("+17005554141"), 0);
        ArgumentCaptor<String> mCaptorString = ArgumentCaptor.forClass(String.class);
        ArgumentCaptor<PhoneInternalInterface.DialArgs> dialArgsCaptor =
                ArgumentCaptor.forClass(PhoneInternalInterface.DialArgs.class);

        verify(mPhone, times(1)).dial(mCaptorString.capture(), dialArgsCaptor.capture());
        assertEquals(PhoneNumberUtils.stripSeparators("+17005554141"),
                mCaptorString.getValue());
        assertEquals(0, dialArgsCaptor.getValue().videoState);
    }

    @Test @SmallTest
    public void testRegisterEvent() throws Exception {
        Field field = CallManager.class.getDeclaredField("EVENT_CALL_WAITING");
        field.setAccessible(true);
        int mEvent = (Integer) field.get(CallManager.getInstance());
        verify(mPhone, times(1)).registerForCallWaiting(isA(Handler.class),
                eq(mEvent), isNull());

        field = CallManager.class.getDeclaredField("EVENT_PRECISE_CALL_STATE_CHANGED");
        field.setAccessible(true);
        mEvent = (Integer) field.get(CallManager.getInstance());
        verify(mPhone, times(1)).registerForPreciseCallStateChanged(isA(Handler.class),
                eq(mEvent), isA(Object.class));

        field = CallManager.class.getDeclaredField("EVENT_RINGBACK_TONE");
        field.setAccessible(true);
        mEvent = (Integer) field.get(CallManager.getInstance());
        verify(mPhone, times(1)).registerForRingbackTone(isA(Handler.class),
                eq(mEvent), isA(Object.class));
    }

    @Test @SmallTest
    public void testGetServiceState() throws Exception {
        // register for another phone
        ServiceState mSecondServiceState = mock(ServiceState.class);
        doReturn(mSecondServiceState).when(mSecondPhone).getServiceState();

        Message mRegisterPhone = mHandler.obtainMessage(PHONE_REGISTER_EVENT,
                mSecondPhone);
        setReady(false);
        mRegisterPhone.sendToTarget();

        waitUntilReady();

        // mPhone: STATE_IN_SERVICE > mPhoneSecond: state STATE_OUT_OF_SERVICE
        doReturn(ServiceState.STATE_OUT_OF_SERVICE).when(mSecondServiceState).getState();
        assertEquals(ServiceState.STATE_IN_SERVICE, CallManager.getInstance().getServiceState());

        // mPhone: STATE_IN_SERVICE > mPhoneSecond: state STATE_EMERGENCY_ONLY
        doReturn(ServiceState.STATE_EMERGENCY_ONLY).when(mSecondServiceState).getState();
        assertEquals(ServiceState.STATE_IN_SERVICE, CallManager.getInstance().getServiceState());

        // mPhone: STATE_IN_SERVICE > mPhoneSecond: state STATE_POWER_OFF
        doReturn(ServiceState.STATE_POWER_OFF).when(mSecondServiceState).getState();
        assertEquals(ServiceState.STATE_IN_SERVICE, CallManager.getInstance().getServiceState());

        // mPhone: STATE_EMERGENCY_ONLY < mPhoneSecond: state STATE_OUT_OF_SERVICE
        doReturn(ServiceState.STATE_OUT_OF_SERVICE).when(mSecondServiceState).getState();
        doReturn(ServiceState.STATE_EMERGENCY_ONLY).when(mServiceState).getState();
        assertEquals(ServiceState.STATE_OUT_OF_SERVICE,
                CallManager.getInstance().getServiceState());

        // mPhone: STATE_POWER_OFF < mPhoneSecond: state STATE_OUT_OF_SERVICE
        doReturn(ServiceState.STATE_POWER_OFF).when(mServiceState).getState();
        assertEquals(ServiceState.STATE_OUT_OF_SERVICE,
                CallManager.getInstance().getServiceState());

        /* mPhone: STATE_POWER_OFF < mPhoneSecond: state STATE_EMERGENCY_ONLY
           but OUT_OF_SERVICE will replaces EMERGENCY_ONLY and POWER_OFF */
        doReturn(ServiceState.STATE_EMERGENCY_ONLY).when(mSecondServiceState).getState();
        assertEquals(ServiceState.STATE_OUT_OF_SERVICE,
                CallManager.getInstance().getServiceState());
        CallManager.getInstance().unregisterPhone(mSecondPhone);
    }
}
