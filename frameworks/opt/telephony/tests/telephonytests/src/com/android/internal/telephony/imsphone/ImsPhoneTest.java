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

package com.android.internal.telephony.imsphone;

import static com.android.internal.telephony.CommandsInterface.CF_ACTION_ENABLE;
import static com.android.internal.telephony.CommandsInterface.CF_REASON_UNCONDITIONAL;
import static com.android.internal.telephony.TelephonyTestUtils.waitForMs;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.mockito.ArgumentMatchers.anyBoolean;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyChar;
import static org.mockito.Matchers.anyInt;
import static org.mockito.Matchers.anyLong;
import static org.mockito.Matchers.nullable;
import static org.mockito.Mockito.atLeast;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.reset;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.app.Activity;
import android.app.IApplicationThread;
import android.content.BroadcastReceiver;
import android.content.IIntentReceiver;
import android.content.Intent;
import android.os.AsyncResult;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.os.PersistableBundle;
import android.os.SystemProperties;
import android.telephony.CarrierConfigManager;
import android.telephony.ServiceState;
import android.telephony.ims.ImsCallProfile;
import android.telephony.ims.ImsReasonInfo;
import android.test.suitebuilder.annotation.SmallTest;

import androidx.test.filters.FlakyTest;

import com.android.ims.ImsEcbmStateListener;
import com.android.ims.ImsManager;
import com.android.ims.ImsUtInterface;
import com.android.internal.telephony.Call;
import com.android.internal.telephony.CallStateException;
import com.android.internal.telephony.CommandsInterface;
import com.android.internal.telephony.Connection;
import com.android.internal.telephony.Phone;
import com.android.internal.telephony.PhoneConstants;
import com.android.internal.telephony.TelephonyIntents;
import com.android.internal.telephony.TelephonyProperties;
import com.android.internal.telephony.TelephonyTest;
import com.android.internal.telephony.gsm.SuppServiceNotification;

import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;

import java.util.List;

public class ImsPhoneTest extends TelephonyTest {
    @Mock
    private ImsPhoneCall mForegroundCall;
    @Mock
    private ImsPhoneCall mBackgroundCall;
    @Mock
    private ImsPhoneCall mRingingCall;
    @Mock
    private Handler mTestHandler;
    @Mock
    Connection mConnection;
    @Mock
    ImsUtInterface mImsUtInterface;

    private ImsPhone mImsPhoneUT;
    private ImsPhoneTestHandler mImsPhoneTestHandler;
    private boolean mDoesRilSendMultipleCallRing;
    private static final int EVENT_SUPP_SERVICE_NOTIFICATION = 1;
    private static final int EVENT_SUPP_SERVICE_FAILED = 2;
    private static final int EVENT_INCOMING_RING = 3;
    private static final int EVENT_EMERGENCY_CALLBACK_MODE_EXIT = 4;

    private class ImsPhoneTestHandler extends HandlerThread {

        private ImsPhoneTestHandler(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            mImsPhoneUT = new ImsPhone(mContext, mNotifier, mPhone, true);
            setReady(true);
        }
    }

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());

        mImsCT.mForegroundCall = mForegroundCall;
        mImsCT.mBackgroundCall = mBackgroundCall;
        mImsCT.mRingingCall = mRingingCall;
        doReturn(Call.State.IDLE).when(mForegroundCall).getState();
        doReturn(Call.State.IDLE).when(mBackgroundCall).getState();
        doReturn(Call.State.IDLE).when(mRingingCall).getState();

        mContextFixture.putBooleanResource(com.android.internal.R.bool.config_voice_capable, true);

        mImsPhoneTestHandler = new ImsPhoneTestHandler(TAG);
        mImsPhoneTestHandler.start();
        waitUntilReady();

        mDoesRilSendMultipleCallRing = SystemProperties.getBoolean(
                TelephonyProperties.PROPERTY_RIL_SENDS_MULTIPLE_CALL_RING, true);
        replaceInstance(Handler.class, "mLooper", mTestHandler, mImsPhoneUT.getLooper());
        replaceInstance(Phone.class, "mLooper", mPhone, mImsPhoneUT.getLooper());
        mImsPhoneUT.registerForSuppServiceNotification(mTestHandler,
                EVENT_SUPP_SERVICE_NOTIFICATION, null);
        mImsPhoneUT.registerForSuppServiceFailed(mTestHandler,
                EVENT_SUPP_SERVICE_FAILED, null);
        mImsPhoneUT.registerForIncomingRing(mTestHandler,
                EVENT_INCOMING_RING, null);
        doReturn(mImsUtInterface).when(mImsCT).getUtInterface();
    }


    @After
    public void tearDown() throws Exception {
        mImsPhoneUT = null;
        mImsPhoneTestHandler.quit();
        super.tearDown();
    }

    @Test
    @SmallTest
    public void testHandleInCallMmiCommandCallDeflection() throws Exception {
        doReturn(Call.State.INCOMING).when(mRingingCall).getState();

        // dial string length > 1
        assertEquals(false, mImsPhoneUT.handleInCallMmiCommands("00"));

        // ringing call is not idle
        assertEquals(true, mImsPhoneUT.handleInCallMmiCommands("0"));
        verify(mImsCT).rejectCall();

        // ringing is idle, background call is not idle
        doReturn(Call.State.IDLE).when(mRingingCall).getState();
        doReturn(Call.State.ACTIVE).when(mBackgroundCall).getState();
        assertEquals(true, mImsPhoneUT.handleInCallMmiCommands("0"));
        verify(mImsCT).hangup(mBackgroundCall);
    }

    @Test
    @SmallTest
    public void testHandleInCallMmiCommandCallWaiting() throws Exception {
        doReturn(Call.State.ACTIVE).when(mForegroundCall).getState();

        // dial string length > 2
        assertEquals(false, mImsPhoneUT.handleInCallMmiCommands("100"));

        // dial string length > 1
        assertEquals(true, mImsPhoneUT.handleInCallMmiCommands("10"));
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler).sendMessageAtTime(messageArgumentCaptor.capture(), anyLong());
        assertEquals(EVENT_SUPP_SERVICE_FAILED, messageArgumentCaptor.getValue().what);
        assertEquals(Phone.SuppService.HANGUP,
                ((AsyncResult)messageArgumentCaptor.getValue().obj).result);

        // foreground call is not idle
        assertEquals(true, mImsPhoneUT.handleInCallMmiCommands("1"));
        verify(mImsCT).hangup(any(ImsPhoneCall.class));

        // foreground call is idle
        doReturn(Call.State.IDLE).when(mForegroundCall).getState();
        doReturn(Call.State.INCOMING).when(mRingingCall).getState();
        assertEquals(true, mImsPhoneUT.handleInCallMmiCommands("1"));
        verify(mImsCT).holdActiveCallForWaitingCall();
    }

    @Test
    @SmallTest
    public void testHandleInCallMmiCommandCallHold() throws Exception {
        doReturn(Call.State.ACTIVE).when(mForegroundCall).getState();

        // dial string length > 2
        assertEquals(false, mImsPhoneUT.handleInCallMmiCommands("200"));

        // dial string length > 1
        assertEquals(true, mImsPhoneUT.handleInCallMmiCommands("20"));
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler).sendMessageAtTime(messageArgumentCaptor.capture(), anyLong());
        assertEquals(EVENT_SUPP_SERVICE_FAILED, messageArgumentCaptor.getValue().what);
        assertEquals(Phone.SuppService.SEPARATE,
                ((AsyncResult) messageArgumentCaptor.getValue().obj).result);

        // ringing call is idle
        assertEquals(true, mImsPhoneUT.handleInCallMmiCommands("2"));
        verify(mImsCT).holdActiveCall();

        // ringing call is not idle
        doReturn(Call.State.INCOMING).when(mRingingCall).getState();
        assertEquals(true, mImsPhoneUT.handleInCallMmiCommands("2"));
        verify(mImsCT).acceptCall(ImsCallProfile.CALL_TYPE_VOICE);
    }

    @Test
    @SmallTest
    public void testHandleInCallMmiCommandMultiparty() {
        doReturn(Call.State.ACTIVE).when(mForegroundCall).getState();

        // dial string length > 1
        assertEquals(false, mImsPhoneUT.handleInCallMmiCommands("30"));

        // dial string length == 1
        assertEquals(true, mImsPhoneUT.handleInCallMmiCommands("3"));
        verify(mImsCT).conference();
    }

    @Test
    @SmallTest
    public void testHandleInCallMmiCommandCallEct() {
        doReturn(Call.State.ACTIVE).when(mForegroundCall).getState();

        // dial string length > 1
        assertEquals(false, mImsPhoneUT.handleInCallMmiCommands("40"));

        // dial string length == 1
        assertEquals(true, mImsPhoneUT.handleInCallMmiCommands("4"));
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler).sendMessageAtTime(messageArgumentCaptor.capture(), anyLong());
        assertEquals(EVENT_SUPP_SERVICE_FAILED, messageArgumentCaptor.getValue().what);
        assertEquals(Phone.SuppService.TRANSFER,
                ((AsyncResult) messageArgumentCaptor.getValue().obj).result);
    }

    @Test
    @SmallTest
    public void testHandleInCallMmiCommandCallCcbs() {
        doReturn(Call.State.ACTIVE).when(mForegroundCall).getState();

        // dial string length > 1
        assertEquals(false, mImsPhoneUT.handleInCallMmiCommands("50"));

        // dial string length == 1
        assertEquals(true, mImsPhoneUT.handleInCallMmiCommands("5"));
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler).sendMessageAtTime(messageArgumentCaptor.capture(), anyLong());
        assertEquals(EVENT_SUPP_SERVICE_FAILED, messageArgumentCaptor.getValue().what);
        assertEquals(Phone.SuppService.UNKNOWN,
                ((AsyncResult)messageArgumentCaptor.getValue().obj).result);
    }

    @Test
    @SmallTest
    public void testDispose() {
        // add MMI to verify that dispose removes it
        mImsPhoneUT.sendUssdResponse("1234");
        verify(mImsCT).sendUSSD(eq("1234"), any(Message.class));
        List<?> list = mImsPhoneUT.getPendingMmiCodes();
        assertNotNull(list);
        assertEquals(1, list.size());

        mImsPhoneUT.dispose();
        assertEquals(0, list.size());
        verify(mImsCT).dispose();
        verify(mSST, times(2)).unregisterForDataRegStateOrRatChanged(anyInt(), eq(mImsPhoneUT));
    }

    @Test
    @SmallTest
    public void testGettersAndPassThroughs() throws Exception {
        Message msg = mTestHandler.obtainMessage();

        assertNotNull(mImsPhoneUT.getServiceState());
        assertEquals(mImsCT, mImsPhoneUT.getCallTracker());

        mImsPhoneUT.acceptCall(0);
        verify(mImsCT).acceptCall(0);

        mImsPhoneUT.rejectCall();
        verify(mImsCT).rejectCall();

        assertEquals(false, mImsPhoneUT.canConference());
        doReturn(true).when(mImsCT).canConference();
        assertEquals(true, mImsPhoneUT.canConference());
        verify(mImsCT, times(2)).canConference();

        doNothing().when(mImsCT).checkForDialIssues();
        assertEquals(true, mImsPhoneUT.canDial());
        doThrow(CallStateException.class).when(mImsCT).checkForDialIssues();
        assertEquals(false, mImsPhoneUT.canDial());
        verify(mImsCT, times(2)).checkForDialIssues();

        mImsPhoneUT.conference();
        verify(mImsCT).conference();

        mImsPhoneUT.clearDisconnected();
        verify(mImsCT).clearDisconnected();

        assertEquals(false, mImsPhoneUT.canTransfer());
        doReturn(true).when(mImsCT).canTransfer();
        assertEquals(true, mImsPhoneUT.canTransfer());
        verify(mImsCT, times(2)).canTransfer();

        mImsPhoneUT.explicitCallTransfer();
        verify(mImsCT).explicitCallTransfer();

        assertEquals(mForegroundCall, mImsPhoneUT.getForegroundCall());
        assertEquals(mBackgroundCall, mImsPhoneUT.getBackgroundCall());
        assertEquals(mRingingCall, mImsPhoneUT.getRingingCall());

        mImsPhoneUT.notifyNewRingingConnection(mConnection);
        verify(mPhone).notifyNewRingingConnectionP(mConnection);

        mImsPhoneUT.notifyForVideoCapabilityChanged(true);
        verify(mPhone).notifyForVideoCapabilityChanged(true);

        mImsPhoneUT.setMute(true);
        verify(mImsCT).setMute(true);

        mImsPhoneUT.setUiTTYMode(1234, null);
        verify(mImsCT).setUiTTYMode(1234, null);

        doReturn(false).when(mImsCT).getMute();
        assertEquals(false, mImsPhoneUT.getMute());
        doReturn(true).when(mImsCT).getMute();
        assertEquals(true, mImsPhoneUT.getMute());
        verify(mImsCT, times(2)).getMute();

        doReturn(PhoneConstants.State.IDLE).when(mImsCT).getState();
        assertEquals(PhoneConstants.State.IDLE, mImsPhoneUT.getState());
        doReturn(PhoneConstants.State.RINGING).when(mImsCT).getState();
        assertEquals(PhoneConstants.State.RINGING, mImsPhoneUT.getState());

        mImsPhoneUT.sendUSSD("1234", msg);
        verify(mImsCT).sendUSSD("1234", msg);

        mImsPhoneUT.cancelUSSD(msg);
        verify(mImsCT).cancelUSSD(msg);

    }

    @Test
    @SmallTest
    public void testSuppServiceNotification() {
        SuppServiceNotification ssn = new SuppServiceNotification();
        mImsPhoneUT.notifySuppSvcNotification(ssn);

        // verify registrants are notified
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler, times(1)).sendMessageAtTime(messageArgumentCaptor.capture(),
                anyLong());
        Message message = messageArgumentCaptor.getValue();
        assertEquals(EVENT_SUPP_SERVICE_NOTIFICATION, message.what);
        assertEquals(ssn, ((AsyncResult) message.obj).result);
        assertEquals(null, ((AsyncResult) message.obj).userObj);
        assertEquals(null, ((AsyncResult) message.obj).exception);

        // verify no notification is received after unregister (verify() still sees only 1
        // notification)
        mImsPhoneUT.unregisterForSuppServiceNotification(mTestHandler);
        mImsPhoneUT.notifySuppSvcNotification(ssn);
        verify(mTestHandler, times(1)).sendMessageAtTime(any(Message.class), anyLong());
    }

    @Test
    @SmallTest
    public void testDial() throws Exception {
        String dialString = "1234567890";
        int videoState = 0;

        mImsPhoneUT.dial(dialString,
                new ImsPhone.ImsDialArgs.Builder().setVideoState(videoState).build());
        verify(mImsCT).dial(eq(dialString), any(ImsPhone.ImsDialArgs.class));
    }

    @Test
    @SmallTest
    public void testDtmf() {
        // case 1
        doReturn(PhoneConstants.State.IDLE).when(mImsCT).getState();
        mImsPhoneUT.sendDtmf('-');
        verify(mImsCT, times(0)).sendDtmf(anyChar(), nullable(Message.class));

        // case 2
        mImsPhoneUT.sendDtmf('0');
        verify(mImsCT, times(0)).sendDtmf(eq('0'), nullable(Message.class));

        // case 3
        doReturn(PhoneConstants.State.OFFHOOK).when(mImsCT).getState();
        mImsPhoneUT.sendDtmf('-');
        verify(mImsCT, times(0)).sendDtmf(eq('0'), nullable(Message.class));

        // case 4
        mImsPhoneUT.sendDtmf('0');
        verify(mImsCT, times(1)).sendDtmf(anyChar(), nullable(Message.class));

        mImsPhoneUT.startDtmf('-');
        verify(mImsCT, times(0)).startDtmf(anyChar());

        mImsPhoneUT.startDtmf('0');
        verify(mImsCT, times(1)).startDtmf('0');

        mImsPhoneUT.stopDtmf();
        verify(mImsCT).stopDtmf();
    }

    @Test
    @SmallTest
    public void testIncomingRing() {
        doReturn(PhoneConstants.State.IDLE).when(mImsCT).getState();
        mImsPhoneUT.notifyIncomingRing();
        waitForMs(100);
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler, times(1)).sendMessageAtTime(messageArgumentCaptor.capture(),
                anyLong());
        Message message = messageArgumentCaptor.getValue();
        assertEquals(EVENT_INCOMING_RING, message.what);
    }

    @Test
    @SmallTest
    public void testOutgoingCallerIdDisplay() throws Exception {
        Message msg = mTestHandler.obtainMessage();
        mImsPhoneUT.getOutgoingCallerIdDisplay(msg);

        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mImsUtInterface).queryCLIR(messageArgumentCaptor.capture());
        assertEquals(msg, messageArgumentCaptor.getValue().obj);

        mImsPhoneUT.setOutgoingCallerIdDisplay(1234, msg);
        verify(mImsUtInterface).updateCLIR(eq(1234), messageArgumentCaptor.capture());
        assertEquals(msg, messageArgumentCaptor.getValue().obj);
    }

    @FlakyTest
    @Test
    @Ignore
    public void testCallForwardingOption() throws Exception {
        Message msg = mTestHandler.obtainMessage();
        mImsPhoneUT.getCallForwardingOption(CF_REASON_UNCONDITIONAL, msg);

        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mImsUtInterface).queryCallForward(eq(ImsUtInterface.CDIV_CF_UNCONDITIONAL),
                (String) eq(null), messageArgumentCaptor.capture());
        assertEquals(msg, messageArgumentCaptor.getValue().obj);

        mImsPhoneUT.setCallForwardingOption(CF_ACTION_ENABLE, CF_REASON_UNCONDITIONAL, "1234", 0,
                msg);
        verify(mImsUtInterface).updateCallForward(eq(ImsUtInterface.ACTION_ACTIVATION),
                eq(ImsUtInterface.CDIV_CF_UNCONDITIONAL), eq("1234"),
                eq(CommandsInterface.SERVICE_CLASS_VOICE), eq(0), eq(msg));
    }

    @Test
    @SmallTest
    public void testCallWaiting() throws Exception {
        Message msg = mTestHandler.obtainMessage();
        mImsPhoneUT.getCallWaiting(msg);

        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mImsUtInterface).queryCallWaiting(messageArgumentCaptor.capture());
        assertEquals(msg, messageArgumentCaptor.getValue().obj);

        mImsPhoneUT.setCallWaiting(true, msg);
        verify(mImsUtInterface).updateCallWaiting(eq(true),
                eq(CommandsInterface.SERVICE_CLASS_VOICE), messageArgumentCaptor.capture());
        assertEquals(msg, messageArgumentCaptor.getValue().obj);
    }

    @Test
    public void testShouldSendNotificationWhenServiceStateIsChanged() {
        mImsPhoneUT.setServiceState(ServiceState.STATE_IN_SERVICE);
        reset(mSST);

        mImsPhoneUT.setServiceState(ServiceState.STATE_OUT_OF_SERVICE);
        verify(mSST).onImsServiceStateChanged();
    }

    @Test
    public void testShouldNotSendNotificationWhenServiceStateIsNotChanged() {
        mImsPhoneUT.setServiceState(ServiceState.STATE_IN_SERVICE);
        reset(mSST);

        mImsPhoneUT.setServiceState(ServiceState.STATE_IN_SERVICE);
        verify(mSST, never()).onImsServiceStateChanged();
    }

    @Test
    @SmallTest
    public void testCellBarring() throws Exception {
        Message msg = mTestHandler.obtainMessage();
        mImsPhoneUT.getCallBarring(CommandsInterface.CB_FACILITY_BAOC, msg,
                CommandsInterface.SERVICE_CLASS_NONE);

        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mImsUtInterface).queryCallBarring(eq(ImsUtInterface.CB_BAOC),
                messageArgumentCaptor.capture(), eq(CommandsInterface.SERVICE_CLASS_NONE));
        assertEquals(msg, messageArgumentCaptor.getValue().obj);

        mImsPhoneUT.setCallBarring(CommandsInterface.CB_FACILITY_BAOIC, true, "abc", msg,
                CommandsInterface.SERVICE_CLASS_NONE);
        verify(mImsUtInterface).updateCallBarring(eq(ImsUtInterface.CB_BOIC),
                eq(CommandsInterface.CF_ACTION_ENABLE), messageArgumentCaptor.capture(),
                (String[]) eq(null), eq(CommandsInterface.SERVICE_CLASS_NONE));
        assertEquals(msg, messageArgumentCaptor.getValue().obj);

        mImsPhoneUT.setCallBarring(CommandsInterface.CB_FACILITY_BAOICxH, false, "abc", msg,
                CommandsInterface.SERVICE_CLASS_NONE);
        verify(mImsUtInterface).updateCallBarring(eq(ImsUtInterface.CB_BOIC_EXHC),
                eq(CommandsInterface.CF_ACTION_DISABLE), messageArgumentCaptor.capture(),
                (String[])eq(null), eq(CommandsInterface.SERVICE_CLASS_NONE));
        assertEquals(msg, messageArgumentCaptor.getValue().obj);
    }

    @FlakyTest
    @Test
    @Ignore
    public void testEcbm() throws Exception {
        ImsEcbmStateListener imsEcbmStateListener = mImsPhoneUT.getImsEcbmStateListener();

        // verify handling of emergency callback mode
        imsEcbmStateListener.onECBMEntered();

        // verify ACTION_EMERGENCY_CALLBACK_MODE_CHANGED
        ArgumentCaptor<Intent> intentArgumentCaptor = ArgumentCaptor.forClass(Intent.class);
        verify(mIActivityManager, atLeast(1)).broadcastIntent(eq((IApplicationThread)null),
                intentArgumentCaptor.capture(),
                eq((String)null),
                eq((IIntentReceiver)null),
                eq(Activity.RESULT_OK),
                eq((String)null),
                eq((Bundle)null),
                eq((String[])null),
                anyInt(),
                eq((Bundle)null),
                eq(false),
                eq(true),
                anyInt());

        Intent intent = intentArgumentCaptor.getValue();
        assertEquals(TelephonyIntents.ACTION_EMERGENCY_CALLBACK_MODE_CHANGED, intent.getAction());
        assertEquals(true, intent.getBooleanExtra(PhoneConstants.PHONE_IN_ECM_STATE, false));

        // verify that wakeLock is acquired in ECM
        assertEquals(true, mImsPhoneUT.getWakeLock().isHeld());

        mImsPhoneUT.setOnEcbModeExitResponse(mTestHandler, EVENT_EMERGENCY_CALLBACK_MODE_EXIT,
                null);

        // verify handling of emergency callback mode exit
        imsEcbmStateListener.onECBMExited();

        // verify ACTION_EMERGENCY_CALLBACK_MODE_CHANGED
        verify(mIActivityManager, atLeast(2)).broadcastIntent(eq((IApplicationThread)null),
                intentArgumentCaptor.capture(),
                eq((String)null),
                eq((IIntentReceiver)null),
                eq(Activity.RESULT_OK),
                eq((String)null),
                eq((Bundle)null),
                eq((String[])null),
                anyInt(),
                eq((Bundle)null),
                eq(false),
                eq(true),
                anyInt());

        intent = intentArgumentCaptor.getValue();
        assertEquals(TelephonyIntents.ACTION_EMERGENCY_CALLBACK_MODE_CHANGED, intent.getAction());
        assertEquals(false, intent.getBooleanExtra(PhoneConstants.PHONE_IN_ECM_STATE, true));

        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);

        // verify EcmExitRespRegistrant is notified
        verify(mTestHandler).sendMessageAtTime(messageArgumentCaptor.capture(),
                anyLong());
        assertEquals(EVENT_EMERGENCY_CALLBACK_MODE_EXIT, messageArgumentCaptor.getValue().what);

        // verify wakeLock released
        assertEquals(false, mImsPhoneUT.getWakeLock().isHeld());
    }

    @FlakyTest
    @Test
    @SmallTest
    @Ignore
    public void testProcessDisconnectReason() throws Exception {
        // set up CarrierConfig
        PersistableBundle bundle = mContextFixture.getCarrierConfigBundle();
        bundle.putStringArray(CarrierConfigManager.KEY_WFC_OPERATOR_ERROR_CODES_STRING_ARRAY,
                new String[]{"REG09|0"});

        // set up overlays
        String title = "title";
        String messageAlert = "Alert!";
        String messageNotification = "Notification!";
        mContextFixture.putStringArrayResource(
                com.android.internal.R.array.wfcOperatorErrorAlertMessages,
                new String[]{messageAlert});
        mContextFixture.putStringArrayResource(
                com.android.internal.R.array.wfcOperatorErrorNotificationMessages,
                new String[]{messageNotification});
        mContextFixture.putResource(com.android.internal.R.string.wfcRegErrorTitle, title);

        mImsPhoneUT.processDisconnectReason(
                new ImsReasonInfo(ImsReasonInfo.CODE_REGISTRATION_ERROR, 0, "REG09"));

        // TODO: Verify that WFC has been turned off (can't do it right now because
        // setWfcSetting is static).
        //verify(mImsManager).setWfcSetting(any(), eq(false));

        ArgumentCaptor<Intent> intent = ArgumentCaptor.forClass(Intent.class);
        verify(mContext).sendOrderedBroadcast(
                intent.capture(), nullable(String.class), any(BroadcastReceiver.class),
                nullable(Handler.class), eq(Activity.RESULT_OK), nullable(String.class),
                nullable(Bundle.class));
        assertEquals(ImsManager.ACTION_IMS_REGISTRATION_ERROR, intent.getValue().getAction());
        assertEquals(title, intent.getValue().getStringExtra(Phone.EXTRA_KEY_ALERT_TITLE));
        assertEquals(messageAlert, intent.getValue().getStringExtra(Phone.EXTRA_KEY_ALERT_MESSAGE));
        assertEquals(messageNotification,
                intent.getValue().getStringExtra(Phone.EXTRA_KEY_NOTIFICATION_MESSAGE));
    }

    @Test
    @SmallTest
    public void testRoamingDuplicateMessages() throws Exception {
        doReturn(PhoneConstants.State.IDLE).when(mImsCT).getState();

        //roaming - data registration only on LTE
        Message m = getServiceStateChangedMessage(getServiceStateDataOnly(
                ServiceState.RIL_RADIO_TECHNOLOGY_LTE, ServiceState.STATE_IN_SERVICE, true));
        // Inject the message synchronously instead of waiting for the thread to do it.
        mImsPhoneUT.handleMessage(m);

        verify(mImsManager, times(1)).setWfcMode(anyInt(), eq(true));

        // Send a duplicate message
        mImsPhoneUT.handleMessage(m);
        m.recycle();

        // setWfcMode should not be called again.
        verify(mImsManager, times(1)).setWfcMode(anyInt(), anyBoolean());
    }


    @Test
    @SmallTest
    public void testRoamingToOutOfService() throws Exception {
        doReturn(true).when(mTransportManager).isInLegacyMode();
        doReturn(PhoneConstants.State.IDLE).when(mImsCT).getState();

        //roaming - data registration only on LTE
        Message m = getServiceStateChangedMessage(getServiceStateDataOnly(
                ServiceState.RIL_RADIO_TECHNOLOGY_LTE, ServiceState.STATE_IN_SERVICE, true));
        // Inject the message synchronously instead of waiting for the thread to do it.
        mImsPhoneUT.handleMessage(m);
        m.recycle();

        verify(mImsManager, times(1)).setWfcMode(anyInt(), eq(true));

        // move to out of service
        m = getServiceStateChangedMessage(getServiceStateOutOfService());
        mImsPhoneUT.handleMessage(m);
        m.recycle();

        // setWfcMode should not be called again, out_of_service should not trigger move out of
        // roaming.
        verify(mImsManager, times(1)).setWfcMode(anyInt(), anyBoolean());
    }

    @Test
    @SmallTest
    public void testRoamingChangeForLteInLegacyMode() throws Exception {
        doReturn(true).when(mTransportManager).isInLegacyMode();
        doReturn(PhoneConstants.State.IDLE).when(mImsCT).getState();

        //roaming - data registration only on LTE
        Message m = getServiceStateChangedMessage(getServiceStateDataOnly(
                ServiceState.RIL_RADIO_TECHNOLOGY_LTE, ServiceState.STATE_IN_SERVICE, true));
        // Inject the message synchronously instead of waiting for the thread to do it.
        mImsPhoneUT.handleMessage(m);
        m.recycle();

        verify(mImsManager, times(1)).setWfcMode(anyInt(), eq(true));

        // not roaming - data registration on LTE
        m = getServiceStateChangedMessage(getServiceStateDataOnly(
                ServiceState.RIL_RADIO_TECHNOLOGY_LTE, ServiceState.STATE_IN_SERVICE, false));
        mImsPhoneUT.handleMessage(m);
        m.recycle();

        verify(mImsManager, times(1)).setWfcMode(anyInt(), eq(false));
    }

    @Test
    @SmallTest
    public void testDataOnlyRoamingCellToIWlanInLegacyMode() throws Exception {
        doReturn(true).when(mTransportManager).isInLegacyMode();
        doReturn(PhoneConstants.State.IDLE).when(mImsCT).getState();

        //roaming - data registration only on LTE
        Message m = getServiceStateChangedMessage(getServiceStateDataOnly(
                ServiceState.RIL_RADIO_TECHNOLOGY_LTE, ServiceState.STATE_IN_SERVICE, true));
        // Inject the message synchronously instead of waiting for the thread to do it.
        mImsPhoneUT.handleMessage(m);
        m.recycle();

        verify(mImsManager, times(1)).setWfcMode(anyInt(), eq(true));

        // not roaming - data registration onto IWLAN
        m = getServiceStateChangedMessage(getServiceStateDataOnly(
                ServiceState.RIL_RADIO_TECHNOLOGY_IWLAN, ServiceState.STATE_IN_SERVICE, false));
        mImsPhoneUT.handleMessage(m);
        m.recycle();

        // Verify that it hasn't been called again.
        verify(mImsManager, times(1)).setWfcMode(anyInt(), anyBoolean());
    }

    @Test
    @SmallTest
    public void testCellVoiceDataChangeToWlanInLegacyMode() throws Exception {
        doReturn(true).when(mTransportManager).isInLegacyMode();
        doReturn(PhoneConstants.State.IDLE).when(mImsCT).getState();

        //roaming - voice/data registration on LTE
        ServiceState ss = getServiceStateDataAndVoice(
                ServiceState.RIL_RADIO_TECHNOLOGY_LTE, ServiceState.STATE_IN_SERVICE, true);
        Message m = getServiceStateChangedMessage(ss);
        // Inject the message synchronously instead of waiting for the thread to do it.
        mImsPhoneUT.handleMessage(m);

        verify(mImsManager, times(1)).setWfcMode(anyInt(), eq(true));

        // roaming - voice LTE, data registration onto IWLAN
        modifyServiceStateData(ss, ServiceState.RIL_RADIO_TECHNOLOGY_IWLAN,
                ServiceState.STATE_IN_SERVICE, false);
        mImsPhoneUT.handleMessage(m);
        m.recycle();

        // Verify that it hasn't been called again.
        verify(mImsManager, times(1)).setWfcMode(anyInt(), anyBoolean());
    }

    @Test
    public void testNonNullTrackersInImsPhone() throws Exception {
        assertNotNull(mImsPhoneUT.getEmergencyNumberTracker());
        assertNotNull(mImsPhoneUT.getServiceStateTracker());
    }

    private ServiceState getServiceStateDataAndVoice(int rat, int regState, boolean isRoaming) {
        ServiceState ss = new ServiceState();
        ss.setStateOutOfService();
        ss.setDataRegState(regState);
        ss.setDataRoaming(isRoaming);
        ss.setRilDataRadioTechnology(rat);
        ss.setVoiceRegState(regState);
        ss.setVoiceRoaming(isRoaming);
        ss.setRilVoiceRadioTechnology(rat);
        return ss;
    }

    private ServiceState getServiceStateDataOnly(int rat, int regState, boolean isRoaming) {
        ServiceState ss = new ServiceState();
        ss.setStateOutOfService();
        ss.setDataRegState(regState);
        ss.setDataRoaming(isRoaming);
        ss.setRilDataRadioTechnology(rat);
        return ss;
    }

    private ServiceState modifyServiceStateData(ServiceState ss, int rat, int regState,
            boolean isRoaming) {
        ss.setStateOutOfService();
        ss.setDataRegState(regState);
        ss.setDataRoaming(isRoaming);
        ss.setRilDataRadioTechnology(rat);
        return ss;
    }

    private ServiceState getServiceStateOutOfService() {
        ServiceState ss = new ServiceState();
        ss.setStateOutOfService();
        return ss;
    }

    private Message getServiceStateChangedMessage(ServiceState ss) {
        Message m = Message.obtain(mImsPhoneUT.getHandler(), ImsPhone.EVENT_SERVICE_STATE_CHANGED);
        m.obj = AsyncResult.forMessage(m, ss, null);
        return m;
    }
}
