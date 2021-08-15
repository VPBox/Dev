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

import static com.android.internal.telephony.TelephonyTestUtils.waitForMs;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.fail;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.isA;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.telephony.DisconnectCause;
import android.telephony.PhoneNumberUtils;
import android.telephony.ServiceState;
import android.test.suitebuilder.annotation.MediumTest;
import android.test.suitebuilder.annotation.SmallTest;

import androidx.test.filters.FlakyTest;
import androidx.test.runner.AndroidJUnit4;

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;

@RunWith(AndroidJUnit4.class)
public class GsmCdmaCallTrackerTest extends TelephonyTest {
    private static final int VOICE_CALL_STARTED_EVENT = 0;
    private static final int VOICE_CALL_ENDED_EVENT = 1;
    private static final int TEST_TIMEOUT = 5000;
    private String mDialString = PhoneNumberUtils.stripSeparators("+17005554141");
    /* Handler class initiated at the HandlerThread */
    private GsmCdmaCallTracker mCTUT;
    private GsmCdmaCTHandlerThread mGsmCdmaCTHandlerThread;
    @Mock
    GsmCdmaConnection mConnection;
    @Mock
    private Handler mHandler;

    private class GsmCdmaCTHandlerThread extends HandlerThread {

        private GsmCdmaCTHandlerThread(String name) {
            super(name);
        }
        @Override
        public void onLooperPrepared() {
            mCTUT = new GsmCdmaCallTracker(mPhone);
            setReady(true);
        }
    }

    @Before
    public void setUp() throws Exception {
        super.setUp(this.getClass().getSimpleName());
        mSimulatedCommands.setRadioPower(true, null);
        mPhone.mCi = this.mSimulatedCommands;

        mGsmCdmaCTHandlerThread = new GsmCdmaCTHandlerThread(TAG);
        mGsmCdmaCTHandlerThread.start();

        waitUntilReady();
        logd("GsmCdmaCallTracker initiated, waiting for Power on");
        /* Make sure radio state is power on before dial.
         * When radio state changed from off to on, CallTracker
         * will poll result from RIL. Avoid dialing triggered at the same*/
        waitForMs(100);
    }

    @After
    public void tearDown() throws Exception {
        mCTUT = null;
        mGsmCdmaCTHandlerThread.quit();
        super.tearDown();
    }

    @Test
    @SmallTest
    public void testMOCallDial() {
        doReturn(ServiceState.STATE_IN_SERVICE).when(mServiceState).getState();
        assertEquals(PhoneConstants.State.IDLE, mCTUT.getState());
        assertEquals(GsmCdmaCall.State.IDLE, mCTUT.mForegroundCall.getState());
        assertEquals(GsmCdmaCall.State.IDLE, mCTUT.mBackgroundCall.getState());
        assertEquals(0, mCTUT.mForegroundCall.getConnections().size());
        try {
            mCTUT.dial(mDialString, new Bundle());
            waitForMs(100);
        } catch(Exception ex) {
            ex.printStackTrace();
            Assert.fail("unexpected exception thrown"+ex.getMessage()+ex.getStackTrace());
        }

        assertEquals(PhoneConstants.State.OFFHOOK, mCTUT.getState());
        assertEquals(GsmCdmaCall.State.DIALING, mCTUT.mForegroundCall.getState());
        assertEquals(1, mCTUT.mForegroundCall.getConnections().size());
        /* verify the command is sent out to RIL */
        verify(mSimulatedCommandsVerifier).dial(
                eq(PhoneNumberUtils.extractNetworkPortionAlt(mDialString)), eq(false),
                eq(null), eq(false), anyInt(), eq((UUSInfo) null), isA(Message.class));
    }

    @Test
    @SmallTest
    public void testMOCallPickUp() {
        testMOCallDial();
        logd("Waiting for POLL CALL response from RIL");
        TelephonyTestUtils.waitForMs(50);
        logd("Pick Up MO call, expecting call state change event ");
        mSimulatedCommands.progressConnectingToActive();
        waitForMs(100);
        assertEquals(GsmCdmaCall.State.ACTIVE, mCTUT.mForegroundCall.getState());
        assertEquals(GsmCdmaCall.State.IDLE, mCTUT.mBackgroundCall.getState());
    }

    @FlakyTest
    @Ignore
    @Test
    @MediumTest
    public void testMOCallHangup() {
        testMOCallDial();
        logd("Waiting for POLL CALL response from RIL ");
        TelephonyTestUtils.waitForMs(50);
        assertEquals(GsmCdmaCall.State.DIALING, mCTUT.mForegroundCall.getState());
        assertEquals(PhoneConstants.State.OFFHOOK, mCTUT.getState());
        assertEquals(1, mCTUT.mForegroundCall.getConnections().size());
        logd("Hang up MO call after MO call established ");
        try {
            mCTUT.hangup(mCTUT.mForegroundCall);
        } catch(Exception ex) {
            ex.printStackTrace();
            Assert.fail("unexpected exception thrown" + ex.getMessage());
        }
        waitForMs(300);
        assertEquals(GsmCdmaCall.State.IDLE, mCTUT.mForegroundCall.getState());
        assertEquals(0, mCTUT.mForegroundCall.getConnections().size());
        assertEquals(PhoneConstants.State.IDLE, mCTUT.getState());
    }

    @FlakyTest
    @Ignore
    @Test
    @MediumTest
    public void testMOCallDialPickUpHangup() {
        testMOCallPickUp();
        assertEquals(GsmCdmaCall.State.ACTIVE, mCTUT.mForegroundCall.getState());
        assertEquals(PhoneConstants.State.OFFHOOK, mCTUT.getState());
        assertEquals(1, mCTUT.mForegroundCall.getConnections().size());
         /* get the reference of the connection before reject */
        Connection connection = mCTUT.mForegroundCall.getConnections().get(0);
        assertEquals(DisconnectCause.NOT_DISCONNECTED, connection.getDisconnectCause());
        logd("hang up MO call after pickup");
        try {
            mCTUT.hangup(mCTUT.mForegroundCall);
        } catch(Exception ex) {
            ex.printStackTrace();
            Assert.fail("unexpected exception thrown" + ex.getMessage());
        }
        /* request send to RIL still in disconnecting state */
        waitForMs(300);
        assertEquals(GsmCdmaCall.State.IDLE, mCTUT.mForegroundCall.getState());
        assertEquals(0, mCTUT.mForegroundCall.getConnections().size());
        assertEquals(PhoneConstants.State.IDLE, mCTUT.getState());
        assertEquals(DisconnectCause.LOCAL, connection.getDisconnectCause());

    }

    @FlakyTest
    @Ignore
    @Test
    @MediumTest
    public void testMOCallPendingHangUp() {
        testMOCallDial();
        logd("MO call hangup before established[ getting result from RIL ]");
        /* poll call result from RIL, find that there is a pendingMO call,
         * Didn't do anything for hangup, clear during handle poll result */
        try {
            mCTUT.hangup(mCTUT.mForegroundCall);
        } catch(Exception ex) {
            ex.printStackTrace();
            Assert.fail("unexpected exception thrown" + ex.getMessage());
        }
        waitForMs(300);
        assertEquals(GsmCdmaCall.State.IDLE, mCTUT.mForegroundCall.getState());
        assertEquals(0, mCTUT.mForegroundCall.getConnections().size());
        assertEquals(PhoneConstants.State.IDLE, mCTUT.getState());
    }

    @Test
    @MediumTest
    public void testMOCallSwitch() {
        testMOCallPickUp();
        logd("MO call picked up, initiating a new MO call");
        assertEquals(GsmCdmaCall.State.ACTIVE, mCTUT.mForegroundCall.getState());
        assertEquals(GsmCdmaCall.State.IDLE, mCTUT.mBackgroundCall.getState());
        assertEquals(1, mCTUT.mForegroundCall.getConnections().size());
        assertEquals(0, mCTUT.mBackgroundCall.getConnections().size());

        String mDialString = PhoneNumberUtils.stripSeparators("+17005554142");
        try {
            mCTUT.dial(mDialString, new Bundle());
        } catch(Exception ex) {
            ex.printStackTrace();
            Assert.fail("unexpected exception thrown" + ex.getMessage());
        }
        waitForMs(200);
        assertEquals(GsmCdmaCall.State.DIALING, mCTUT.mForegroundCall.getState());
        assertEquals(GsmCdmaCall.State.HOLDING, mCTUT.mBackgroundCall.getState());
        assertEquals(1, mCTUT.mForegroundCall.getConnections().size());
        assertEquals(1, mCTUT.mBackgroundCall.getConnections().size());

    }

    /**
     * Test that we can set up a ringing call. If a timeout occurs something probably went wrong.
     */
    @Test(timeout = 100)
    @SmallTest
    public void testMTCallRinging() {
        setupRingingCall();
    }

    private void setupRingingCall() {
        /* Mock there is a MT call mRinging call and try to accept this MT call */
        /* if we got a active state followed by another MT call-> move to background call */
        assertEquals(PhoneConstants.State.IDLE, mCTUT.getState());
        assertEquals(0, mCTUT.mRingingCall.getConnections().size());
        assertEquals(GsmCdmaCall.State.IDLE, mCTUT.mForegroundCall.getState());
        String mDialString = PhoneNumberUtils.stripSeparators("+17005554141");
        logd("MT call Ringing");
        mSimulatedCommands.triggerRing(mDialString);
        // handle EVENT_CALL_STATE_CHANGE
        waitForHandlerAction(mCTUT, TEST_TIMEOUT);
        // handle EVENT_POLL_CALLS_RESULT
        waitForHandlerAction(mCTUT, TEST_TIMEOUT);
        assertEquals(PhoneConstants.State.RINGING, mCTUT.getState());
        assertEquals(1, mCTUT.mRingingCall.getConnections().size());
    }

    /**
     * Set up a ringing call, then test that we can accept the call. If a timeout occurs something
     * probably went wrong.
     */
    @Test(timeout = 200) // longer timeout because we first need to setup a ringing call
    @MediumTest
    public void testMTCallAccept() {
        setupRingingCall();
        assertEquals(mCTUT.mForegroundCall.getConnections().size(),0);
        logd("accept the MT call");
        try{
            mCTUT.acceptCall();
        } catch(Exception ex) {
            ex.printStackTrace();
            Assert.fail("unexpected exception thrown" + ex.getMessage());
        }
        /* send to the RIL */
        verify(mSimulatedCommandsVerifier).acceptCall(isA(Message.class));
        // handle EVENT_OPERATION_COMPLETE
        waitForHandlerAction(mCTUT, TEST_TIMEOUT);
        // handle waitForHandlerAction
        waitForHandlerAction(mCTUT, TEST_TIMEOUT);
        assertEquals(PhoneConstants.State.OFFHOOK, mCTUT.getState());
        assertEquals(GsmCdmaCall.State.ACTIVE, mCTUT.mForegroundCall.getState());
        assertEquals(1, mCTUT.mForegroundCall.getConnections().size());
        assertEquals(0, mCTUT.mRingingCall.getConnections().size());
    }

    /**
     * Set up a ringing call, then test that we can reject the call. If a timeout occurs something
     * probably went wrong.
     */
    @Test(timeout = 200) // longer timeout because we first need to setup a ringing call
    @MediumTest
    public void testMTCallReject() {
        setupRingingCall();
        logd("MT call ringing and rejected ");
        /* get the reference of the connection before reject */
        Connection connection = mCTUT.mRingingCall.getConnections().get(0);
        assertNotNull(connection);
        assertEquals(DisconnectCause.NOT_DISCONNECTED, connection.getDisconnectCause());
        try {
            mCTUT.rejectCall();
        } catch(Exception ex) {
            ex.printStackTrace();
            Assert.fail("unexpected exception thrown" + ex.getMessage());
        }
        // handle EVENT_OPERATION_COMPLETE
        waitForHandlerAction(mCTUT, TEST_TIMEOUT);
        // handle EVENT_POLL_CALLS_RESULT
        waitForHandlerAction(mCTUT, TEST_TIMEOUT);
        assertEquals(PhoneConstants.State.IDLE, mCTUT.getState());
        assertEquals(GsmCdmaCall.State.IDLE, mCTUT.mForegroundCall.getState());
        assertEquals(0, mCTUT.mForegroundCall.getConnections().size());
        /* ? why rejectCall didnt -> hang up locally to set the cause to LOCAL? */
        assertEquals(DisconnectCause.INCOMING_MISSED, connection.getDisconnectCause());

    }

    @FlakyTest
    @Ignore
    @Test
    @MediumTest
    public void testMOCallSwitchHangupForeGround() {
        testMOCallSwitch();
        logd("Hang up the foreground MO call while dialing ");
        try {
            mCTUT.hangup(mCTUT.mForegroundCall);
        } catch(Exception ex) {
            ex.printStackTrace();
            Assert.fail("unexpected exception thrown" + ex.getMessage());
        }
        waitForMs(300);
        logd(" Foreground Call is IDLE and BackGround Call is still HOLDING ");
        /* if we want to hang up foreground call which is alerting state, hangup all */
        assertEquals(GsmCdmaCall.State.IDLE, mCTUT.mForegroundCall.getState());
        assertEquals(GsmCdmaCall.State.HOLDING, mCTUT.mBackgroundCall.getState());
    }

    @FlakyTest
    @Ignore
    @Test
    @MediumTest
    public void testMOCallPickUpHangUpResumeBackGround() {
        testMOCallSwitch();
        logd("Pick up the new MO Call");
        try{
            mSimulatedCommands.progressConnectingToActive();
        } catch(Exception ex) {
            ex.printStackTrace();
            Assert.fail("unexpected exception thrown" + ex.getMessage());
        }

        waitForMs(200);
        assertEquals(GsmCdmaCall.State.ACTIVE, mCTUT.mForegroundCall.getState());
        assertEquals(GsmCdmaCall.State.HOLDING, mCTUT.mBackgroundCall.getState());

        logd("Hang up the new MO Call");
        try {
            mCTUT.hangup(mCTUT.mForegroundCall);
        } catch(Exception ex) {
            ex.printStackTrace();
            Assert.fail("unexpected exception thrown" + ex.getMessage());
        }

        waitForMs(300);
        logd(" BackGround Call switch to ForeGround Call ");
        assertEquals(GsmCdmaCall.State.ACTIVE, mCTUT.mForegroundCall.getState());
        assertEquals(GsmCdmaCall.State.IDLE, mCTUT.mBackgroundCall.getState());
    }

    @Test @SmallTest
    public void testVoiceCallStartListener(){
        logd("register for voice call started event");
        mCTUT.registerForVoiceCallStarted(mHandler, VOICE_CALL_STARTED_EVENT, null);
        logd("voice call started");
        testMOCallPickUp();
        ArgumentCaptor<Message> mCaptorMessage = ArgumentCaptor.forClass(Message.class);
        ArgumentCaptor<Long> mCaptorLong = ArgumentCaptor.forClass(Long.class);
        verify(mHandler, times(1))
                .sendMessageAtTime(mCaptorMessage.capture(), mCaptorLong.capture());
        assertEquals(VOICE_CALL_STARTED_EVENT, mCaptorMessage.getValue().what);

    }

    @FlakyTest
    @Ignore
    @Test @SmallTest
    public void testVoiceCallEndedListener(){
        logd("register for voice call ended event");
        mCTUT.registerForVoiceCallEnded(mHandler, VOICE_CALL_ENDED_EVENT, null);
        ArgumentCaptor<Message> mCaptorMessage = ArgumentCaptor.forClass(Message.class);
        ArgumentCaptor<Long> mCaptorLong = ArgumentCaptor.forClass(Long.class);
        testMOCallHangup();
        verify(mHandler, times(1))
                .sendMessageAtTime(mCaptorMessage.capture(), mCaptorLong.capture());
        assertEquals(VOICE_CALL_ENDED_EVENT, mCaptorMessage.getValue().what);
    }

    @Test @SmallTest
    public void testUpdatePhoneType() {
        // verify getCurrentCalls is called on init
        verify(mSimulatedCommandsVerifier).getCurrentCalls(any(Message.class));

        // update phone type (call the function on same thread as the call tracker)
        Handler updatePhoneTypeHandler = new Handler(mCTUT.getLooper()) {
            @Override
            public void handleMessage(Message msg) {
                mCTUT.updatePhoneType();
            }
        };
        updatePhoneTypeHandler.sendEmptyMessage(0);
        waitForMs(100);

        // verify getCurrentCalls is called on updating phone type
        verify(mSimulatedCommandsVerifier, times(2)).getCurrentCalls(any(Message.class));

        // we'd like to verify that if phone type is updated, calls and callTracker go to idle.
        // However, as soon as phone type is updated, call tracker queries for calls from RIL and
        // will go back to OFFHOOK

        // call tracker goes to OFFHOOK
        testMOCallPickUp();

        // update phone type - call tracker goes to IDLE and then due to getCurrentCalls(),
        // goes back to OFFHOOK
        updatePhoneTypeHandler.sendEmptyMessage(0);
        waitForMs(100);

        // verify CT and calls go to idle
        assertEquals(PhoneConstants.State.OFFHOOK, mCTUT.getState());
        assertEquals(GsmCdmaCall.State.ACTIVE, mCTUT.mForegroundCall.getState());
        assertEquals(GsmCdmaCall.State.IDLE, mCTUT.mBackgroundCall.getState());
        assertEquals(GsmCdmaCall.State.IDLE, mCTUT.mRingingCall.getState());
    }

    @Test
    @SmallTest
    public void testUpdatePhoneTypeWithActiveCall() {
        // verify getCurrentCalls is called on init
        verify(mSimulatedCommandsVerifier).getCurrentCalls(any(Message.class));

        // fake connection
        mCTUT.mConnections[0] = mConnection;

        // update phone type (call the function on same thread as the call tracker)
        Handler updatePhoneTypeHandler = new Handler(mCTUT.getLooper()) {
            @Override
            public void handleMessage(Message msg) {
                mCTUT.updatePhoneType();
            }
        };
        updatePhoneTypeHandler.sendEmptyMessage(0);
        waitForMs(100);

        // verify that the active call is disconnected
        verify(mConnection).onDisconnect(DisconnectCause.ERROR_UNSPECIFIED);
    }

    @Test
    @SmallTest
    public void testDispatchCsCallRadioTech() {
        // fake connection
        mCTUT.mConnections[0] = mConnection;

        // dispatch umts
        mCTUT.dispatchCsCallRadioTech(ServiceState.RIL_RADIO_TECHNOLOGY_UMTS);
        // verify that call radio tech is set
        verify(mConnection).setCallRadioTech(ServiceState.RIL_RADIO_TECHNOLOGY_UMTS);

        // dispatch unknown
        mCTUT.dispatchCsCallRadioTech(ServiceState.RIL_RADIO_TECHNOLOGY_UNKNOWN);
        // verify that call radio tech is set
        verify(mConnection).setCallRadioTech(ServiceState.RIL_RADIO_TECHNOLOGY_UNKNOWN);
    }

    @Test
    @SmallTest
    public void testCantCallOtaspInProgress() {
        mDialString = "*22899";
        testMOCallDial();
        waitForHandlerAction(mSimulatedCommands.getHandler(), 5000);
        mSimulatedCommands.progressConnectingToActive();
        waitForHandlerAction(mSimulatedCommands.getHandler(), 5000);
        // Try to place another call.
        try {
            mCTUT.dial("650-555-1212", new Bundle());
        } catch (CallStateException cse) {
            assertEquals(CallStateException.ERROR_OTASP_PROVISIONING_IN_PROCESS, cse.getError());
            return;
        }
        fail("Expected otasp call state exception");
    }
}

