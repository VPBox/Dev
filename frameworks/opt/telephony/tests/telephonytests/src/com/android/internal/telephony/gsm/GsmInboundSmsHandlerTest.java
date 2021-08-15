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

package com.android.internal.telephony.gsm;

import static com.android.internal.telephony.TelephonyTestUtils.waitForMs;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyBoolean;
import static org.mockito.Matchers.anyInt;
import static org.mockito.Matchers.anyLong;
import static org.mockito.Matchers.nullable;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.content.BroadcastReceiver;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.database.Cursor;
import android.net.Uri;
import android.os.AsyncResult;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.RemoteException;
import android.os.UserHandle;
import android.os.UserManager;
import android.provider.Telephony;
import android.telephony.SubscriptionManager;
import android.test.mock.MockContentResolver;

import androidx.test.filters.FlakyTest;
import androidx.test.filters.MediumTest;

import com.android.internal.telephony.FakeSmsContentProvider;
import com.android.internal.telephony.InboundSmsHandler;
import com.android.internal.telephony.InboundSmsTracker;
import com.android.internal.telephony.PhoneConstants;
import com.android.internal.telephony.SmsBroadcastUndelivered;
import com.android.internal.telephony.SmsHeader;
import com.android.internal.telephony.SmsStorageMonitor;
import com.android.internal.telephony.TelephonyTest;
import com.android.internal.telephony.cdma.CdmaInboundSmsHandler;
import com.android.internal.util.HexDump;
import com.android.internal.util.IState;
import com.android.internal.util.StateMachine;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;

import java.lang.reflect.Method;

public class GsmInboundSmsHandlerTest extends TelephonyTest {
    @Mock
    private SmsStorageMonitor mSmsStorageMonitor;
    @Mock
    private android.telephony.SmsMessage mSmsMessage;
    @Mock
    private SmsMessage mGsmSmsMessage;
    @Mock
    private SmsHeader mSmsHeader;
    private InboundSmsTracker mInboundSmsTracker;
    private InboundSmsTracker mInboundSmsTrackerPart1;
    private InboundSmsTracker mInboundSmsTrackerPart2;
    @Mock
    private InboundSmsTracker mMockInboundSmsTracker;
    private ContentValues mInboundSmsTrackerCV;
    @Mock
    private InboundSmsTracker mMockInboundSmsTrackerSub1;
    private ContentValues mInboundSmsTrackerCVSub1;
    @Mock
    private CdmaInboundSmsHandler mCdmaInboundSmsHandler;

    private GsmInboundSmsHandler mGsmInboundSmsHandler;
    private GsmInboundSmsHandlerTestHandler mGsmInboundSmsHandlerTestHandler;

    private FakeSmsContentProvider mContentProvider;
    private static final String RAW_TABLE_NAME = "raw";
    private static final Uri sRawUri = Uri.withAppendedPath(Telephony.Sms.CONTENT_URI,
            RAW_TABLE_NAME);
    private static final int TEST_TIMEOUT = 5000;

    private String mMessageBody = "This is the message body of a single-part message";
    private String mMessageBodyPart1 = "This is the first part of a multi-part message";
    private String mMessageBodyPart2 = "This is the second part of a multi-part message";
    private int mSubId0 = 0;
    private int mSubId1 = 0;

    byte[] mSmsPdu = new byte[]{(byte)0xFF, (byte)0xFF, (byte)0xFF};

    private class GsmInboundSmsHandlerTestHandler extends HandlerThread {

        private GsmInboundSmsHandlerTestHandler(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            mGsmInboundSmsHandler = GsmInboundSmsHandler.makeInboundSmsHandler(mContext,
                    mSmsStorageMonitor, mPhone);
            setReady(true);
        }
    }

    private IState getCurrentState() {
        try {
            Method method = StateMachine.class.getDeclaredMethod("getCurrentState");
            method.setAccessible(true);
            return (IState) method.invoke(mGsmInboundSmsHandler);
        } catch (Exception e) {
            fail(e.toString());
            return null;
        }
    }

    /**
     * This is used only for InboundSmsTracker constructed through Cursor. For other cases
     * real objects should be used. This should be used only for tests related to
     * SmsBroadcastUndelivered.
     */
    private void createMockInboundSmsTracker() {
        mInboundSmsTrackerCV = new ContentValues();
        mInboundSmsTrackerCV.put("destination_port", InboundSmsTracker.DEST_PORT_FLAG_NO_PORT);
        mInboundSmsTrackerCV.put("pdu", HexDump.toHexString(mSmsPdu));
        mInboundSmsTrackerCV.put("address", "1234567890");
        mInboundSmsTrackerCV.put("reference_number", 1);
        mInboundSmsTrackerCV.put("sequence", 1);
        mInboundSmsTrackerCV.put("count", 1);
        mInboundSmsTrackerCV.put("date", System.currentTimeMillis());
        mInboundSmsTrackerCV.put("message_body", mMessageBody);
        mInboundSmsTrackerCV.put("display_originating_addr", "1234567890");
        mInboundSmsTrackerCV.put("sub_id", mSubId0);

        doReturn(1).when(mMockInboundSmsTracker).getMessageCount();
        doReturn(1).when(mMockInboundSmsTracker).getReferenceNumber();
        doReturn("1234567890").when(mMockInboundSmsTracker).getAddress();
        doReturn(1).when(mMockInboundSmsTracker).getSequenceNumber();
        doReturn(1).when(mMockInboundSmsTracker).getIndexOffset();
        doReturn(-1).when(mMockInboundSmsTracker).getDestPort();
        doReturn(mMessageBody).when(mMockInboundSmsTracker).getMessageBody();
        doReturn(mSmsPdu).when(mMockInboundSmsTracker).getPdu();
        doReturn(mInboundSmsTrackerCV.get("date")).when(mMockInboundSmsTracker).getTimestamp();
        doReturn(mInboundSmsTrackerCV).when(mMockInboundSmsTracker).getContentValues();
        doReturn(mSubId0).when(mMockInboundSmsTracker).getSubId();

        mInboundSmsTrackerCVSub1 = new ContentValues();
        mInboundSmsTrackerCVSub1.put("destination_port", InboundSmsTracker.DEST_PORT_FLAG_NO_PORT);
        mInboundSmsTrackerCVSub1.put("pdu", HexDump.toHexString(mSmsPdu));
        mInboundSmsTrackerCVSub1.put("address", "1234567890");
        mInboundSmsTrackerCVSub1.put("reference_number", 1);
        mInboundSmsTrackerCVSub1.put("sequence", 1);
        mInboundSmsTrackerCVSub1.put("count", 1);
        mInboundSmsTrackerCVSub1.put("date", System.currentTimeMillis());
        mInboundSmsTrackerCVSub1.put("message_body", mMessageBody);
        mInboundSmsTrackerCVSub1.put("display_originating_addr", "1234567890");
        mInboundSmsTrackerCVSub1.put("sub_id", mSubId1);

        doReturn(1).when(mMockInboundSmsTrackerSub1).getMessageCount();
        doReturn(1).when(mMockInboundSmsTrackerSub1).getReferenceNumber();
        doReturn("1234567890").when(mMockInboundSmsTrackerSub1).getAddress();
        doReturn(1).when(mMockInboundSmsTrackerSub1).getSequenceNumber();
        doReturn(1).when(mMockInboundSmsTrackerSub1).getIndexOffset();
        doReturn(-1).when(mMockInboundSmsTrackerSub1).getDestPort();
        doReturn(mMessageBody).when(mMockInboundSmsTrackerSub1).getMessageBody();
        doReturn(mSmsPdu).when(mMockInboundSmsTrackerSub1).getPdu();
        doReturn(mInboundSmsTrackerCVSub1.get("date")).when(mMockInboundSmsTrackerSub1)
                .getTimestamp();
        doReturn(mInboundSmsTrackerCVSub1).when(mMockInboundSmsTrackerSub1).getContentValues();
        doReturn(mSubId1).when(mMockInboundSmsTrackerSub1).getSubId();

        doReturn(mMockInboundSmsTracker).doReturn(mMockInboundSmsTrackerSub1)
                .when(mTelephonyComponentFactory)
                .makeInboundSmsTracker(nullable(Cursor.class), anyBoolean());
    }

    @Before
    public void setUp() throws Exception {
        super.setUp("GsmInboundSmsHandlerTest");

        doReturn(true).when(mTelephonyManager).getSmsReceiveCapableForPhone(anyInt(), anyBoolean());
        doReturn(true).when(mSmsStorageMonitor).isStorageAvailable();

        UserManager userManager = (UserManager)mContext.getSystemService(Context.USER_SERVICE);
        doReturn(true).when(userManager).isUserUnlocked();

        try {
            doReturn(new int[]{UserHandle.USER_SYSTEM}).when(mIActivityManager).getRunningUserIds();
        } catch (RemoteException re) {
            fail("Unexpected RemoteException: " + re.getStackTrace());
        }

        mSmsMessage.mWrappedSmsMessage = mGsmSmsMessage;

        mInboundSmsTracker = new InboundSmsTracker(
                mSmsPdu, /* pdu */
                System.currentTimeMillis(), /* timestamp */
                -1, /* destPort */
                false, /* is3gpp2 */
                false, /* is3gpp2WapPdu */
                "1234567890", /* address */
                "1234567890", /* displayAddress */
                mMessageBody, /* messageBody */
                false, /* isClass0 */
                mSubId0);
        doReturn(mInboundSmsTracker).when(mTelephonyComponentFactory)
                .makeInboundSmsTracker(nullable(byte[].class), anyLong(), anyInt(), anyBoolean(),
                anyBoolean(), nullable(String.class), nullable(String.class),
                nullable(String.class), anyBoolean(), anyInt());

        createMockInboundSmsTracker();

        mContentProvider = new FakeSmsContentProvider();
        ((MockContentResolver)mContext.getContentResolver()).addProvider(
                Telephony.Sms.CONTENT_URI.getAuthority(), mContentProvider);

        mGsmInboundSmsHandlerTestHandler = new GsmInboundSmsHandlerTestHandler(TAG);
        mGsmInboundSmsHandlerTestHandler.start();
        waitUntilReady();
    }

    @After
    public void tearDown() throws Exception {
        // wait for wakelock to be released; timeout at 10s
        int i = 0;
        while (mGsmInboundSmsHandler.getWakeLock().isHeld() && i < 100) {
            waitForMs(100);
            i++;
        }
        assertFalse(mGsmInboundSmsHandler.getWakeLock().isHeld());
        mGsmInboundSmsHandler = null;
        mContentProvider.shutdown();
        mGsmInboundSmsHandlerTestHandler.quit();
        mGsmInboundSmsHandlerTestHandler.join();
        super.tearDown();
    }

    private void transitionFromStartupToIdle() {
        // verify initially in StartupState
        assertEquals("StartupState", getCurrentState().getName());

        // trigger transition to IdleState
        mGsmInboundSmsHandler.sendMessage(InboundSmsHandler.EVENT_START_ACCEPTING_SMS);
        waitForHandlerAction(mGsmInboundSmsHandler.getHandler(), TEST_TIMEOUT);

        assertEquals("IdleState", getCurrentState().getName());
    }

    private void verifySmsIntentBroadcasts(int numPastBroadcasts) {
        verifySmsIntentBroadcasts(numPastBroadcasts, false /* allowBgActivityStarts */);
    }

    private void verifySmsIntentBroadcasts(int numPastBroadcasts, boolean allowBgActivityStarts) {
        verifySmsIntentBroadcasts(numPastBroadcasts, allowBgActivityStarts, mSubId0,
                false /* moreMessages */);
    }

    private void verifySmsIntentBroadcasts(int numPastBroadcasts, int subId, boolean moreMessages) {
        verifySmsIntentBroadcasts(numPastBroadcasts, false /* allowBgActivityStarts */, subId,
                moreMessages);
    }

    private void verifySmsIntentBroadcasts(int numPastBroadcasts, boolean allowBgActivityStarts,
            int subId, boolean moreMessages) {
        ArgumentCaptor<Intent> intentArgumentCaptor = ArgumentCaptor.forClass(Intent.class);
        verify(mContext, times(1 + numPastBroadcasts)).sendBroadcast(
                intentArgumentCaptor.capture());
        Intent intent = intentArgumentCaptor.getAllValues().get(numPastBroadcasts);
        assertEquals(Telephony.Sms.Intents.SMS_DELIVER_ACTION, intent.getAction());
        assertEquals(subId, intent.getIntExtra(PhoneConstants.SUBSCRIPTION_KEY,
                SubscriptionManager.INVALID_SUBSCRIPTION_ID));
        assertEquals(subId, intent.getIntExtra(SubscriptionManager.EXTRA_SUBSCRIPTION_INDEX,
                SubscriptionManager.INVALID_SUBSCRIPTION_ID));
        assertEquals("WaitingState", getCurrentState().getName());
        if (allowBgActivityStarts) {
            Bundle broadcastOptions = mContextFixture.getLastBroadcastOptions();
            assertTrue(broadcastOptions
                    .getBoolean("android:broadcast.allowBackgroundActivityStarts"));
        }

        mContextFixture.sendBroadcastToOrderedBroadcastReceivers();

        intentArgumentCaptor = ArgumentCaptor.forClass(Intent.class);
        verify(mContext, times(2 + numPastBroadcasts)).sendBroadcast(
                intentArgumentCaptor.capture());
        intent = intentArgumentCaptor.getAllValues().get(numPastBroadcasts + 1);
        assertEquals(Telephony.Sms.Intents.SMS_RECEIVED_ACTION, intent.getAction());
        assertEquals(subId, intent.getIntExtra(PhoneConstants.SUBSCRIPTION_KEY,
                SubscriptionManager.INVALID_SUBSCRIPTION_ID));
        assertEquals(subId, intent.getIntExtra(SubscriptionManager.EXTRA_SUBSCRIPTION_INDEX,
                SubscriptionManager.INVALID_SUBSCRIPTION_ID));
        assertEquals("WaitingState", getCurrentState().getName());

        mContextFixture.sendBroadcastToOrderedBroadcastReceivers();
        // handle broadcast complete msg
        waitForHandlerAction(mGsmInboundSmsHandler.getHandler(), TEST_TIMEOUT);
        // transition from waiting state to delivering state
        waitForHandlerAction(mGsmInboundSmsHandler.getHandler(), TEST_TIMEOUT);
        if (!moreMessages) {
            // transition from delivering state to idle state; if moreMessages are pending will
            // transition to WaitingState instead of IdleState
            waitForHandlerAction(mGsmInboundSmsHandler.getHandler(), TEST_TIMEOUT);
            assertEquals("IdleState", getCurrentState().getName());
        }
    }

    private void sendNewSms() {
        mGsmInboundSmsHandler.sendMessage(InboundSmsHandler.EVENT_NEW_SMS,
                new AsyncResult(null, mSmsMessage, null));
        // handle EVENT_NEW_SMS
        waitForHandlerAction(mGsmInboundSmsHandler.getHandler(), TEST_TIMEOUT);
        // handle EVENT_BROADCAST_SMS
        waitForHandlerAction(mGsmInboundSmsHandler.getHandler(), TEST_TIMEOUT);
    }

    @FlakyTest
    @Test
    @MediumTest
    public void testNewSms() {
        transitionFromStartupToIdle();

        // send new SMS to state machine and verify that triggers SMS_DELIVER_ACTION
        sendNewSms();

        verifySmsIntentBroadcasts(0);

        // send same SMS again, verify no broadcasts are sent
        sendNewSms();

        verify(mContext, times(2)).sendBroadcast(any(Intent.class));
        assertEquals("IdleState", getCurrentState().getName());
    }

    @Test
    @MediumTest
    public void testNewSmsFromBlockedNumber_noBroadcastsSent() {
        String blockedNumber = "1234567890";
        mFakeBlockedNumberContentProvider.mBlockedNumbers.add(blockedNumber);

        transitionFromStartupToIdle();

        sendNewSms();

        verify(mContext, never()).sendBroadcast(any(Intent.class));
        assertEquals("IdleState", getCurrentState().getName());
    }

    private void verifyDataSmsIntentBroadcasts(int numPastBroadcasts) {
        ArgumentCaptor<Intent> intentArgumentCaptor = ArgumentCaptor.forClass(Intent.class);
        verify(mContext, times(1 + numPastBroadcasts)).sendBroadcast(
                intentArgumentCaptor.capture());
        assertEquals(Telephony.Sms.Intents.DATA_SMS_RECEIVED_ACTION,
                intentArgumentCaptor.getAllValues().get(numPastBroadcasts).getAction());
        assertEquals("WaitingState", getCurrentState().getName());

        mContextFixture.sendBroadcastToOrderedBroadcastReceivers();
        // handle broadcast complete msg
        waitForHandlerAction(mGsmInboundSmsHandler.getHandler(), TEST_TIMEOUT);
        // transition from waiting state to delivering state
        waitForHandlerAction(mGsmInboundSmsHandler.getHandler(), TEST_TIMEOUT);
        // transition from delivering state to idle state
        waitForHandlerAction(mGsmInboundSmsHandler.getHandler(), TEST_TIMEOUT);

        assertEquals("IdleState", getCurrentState().getName());
    }

    @Test
    @MediumTest
    public void testClass0Sms() {
        transitionFromStartupToIdle();

        mInboundSmsTracker = new InboundSmsTracker(
                mSmsPdu, /* pdu */
                System.currentTimeMillis(), /* timestamp */
                -1, /* destPort */
                false, /* is3gpp2 */
                false, /* is3gpp2WapPdu */
                "1234567890", /* address */
                "1234567890", /* displayAddress */
                mMessageBody, /* messageBody */
                true, /* isClass0 */
                mSubId0);
        doReturn(mInboundSmsTracker).when(mTelephonyComponentFactory)
                .makeInboundSmsTracker(nullable(byte[].class), anyLong(), anyInt(), anyBoolean(),
                        anyBoolean(), nullable(String.class), nullable(String.class),
                        nullable(String.class), anyBoolean(), anyInt());
        mGsmInboundSmsHandler.sendMessage(InboundSmsHandler.EVENT_BROADCAST_SMS,
                mInboundSmsTracker);
        waitForHandlerAction(mGsmInboundSmsHandler.getHandler(), TEST_TIMEOUT);
        waitForHandlerAction(mGsmInboundSmsHandler.getHandler(), TEST_TIMEOUT);

        verifySmsIntentBroadcasts(0, true /* allowBgActivityStarts */);
    }

    @Test
    @MediumTest
    public void testBroadcastSms() {
        transitionFromStartupToIdle();

        mInboundSmsTracker = new InboundSmsTracker(
                mSmsPdu, /* pdu */
                System.currentTimeMillis(), /* timestamp */
                0, /* destPort */
                false, /* is3gpp2 */
                false, /* is3gpp2WapPdu */
                "1234567890", /* address */
                "1234567890", /* displayAddress */
                mMessageBody, /* messageBody */
                false, /* isClass0 */
                mSubId0);
        doReturn(mInboundSmsTracker).when(mTelephonyComponentFactory)
                .makeInboundSmsTracker(nullable(byte[].class), anyLong(), anyInt(), anyBoolean(),
                anyBoolean(), nullable(String.class), nullable(String.class),
                nullable(String.class), anyBoolean(), anyInt());
        mGsmInboundSmsHandler.sendMessage(InboundSmsHandler.EVENT_BROADCAST_SMS,
                mInboundSmsTracker);
        waitForHandlerAction(mGsmInboundSmsHandler.getHandler(), TEST_TIMEOUT);
        waitForHandlerAction(mGsmInboundSmsHandler.getHandler(), TEST_TIMEOUT);

        verifyDataSmsIntentBroadcasts(0);

        // send same data sms again, and since it's not text sms it should be broadcast again
        mGsmInboundSmsHandler.sendMessage(InboundSmsHandler.EVENT_BROADCAST_SMS,
                mInboundSmsTracker);
        waitForHandlerAction(mGsmInboundSmsHandler.getHandler(), TEST_TIMEOUT);
        waitForHandlerAction(mGsmInboundSmsHandler.getHandler(), TEST_TIMEOUT);

        verifyDataSmsIntentBroadcasts(1);
    }

    @FlakyTest
    @Test
    @MediumTest
    public void testInjectSms() {
        transitionFromStartupToIdle();

        mGsmInboundSmsHandler.sendMessage(InboundSmsHandler.EVENT_INJECT_SMS, new AsyncResult(null,
                mSmsMessage, null));
        waitForHandlerAction(mGsmInboundSmsHandler.getHandler(), TEST_TIMEOUT);
        waitForHandlerAction(mGsmInboundSmsHandler.getHandler(), TEST_TIMEOUT);

        verifySmsIntentBroadcasts(0);

        // inject same SMS again, verify no broadcasts are sent
        mGsmInboundSmsHandler.sendMessage(InboundSmsHandler.EVENT_INJECT_SMS, new AsyncResult(null,
                mSmsMessage, null));
        waitForHandlerAction(mGsmInboundSmsHandler.getHandler(), TEST_TIMEOUT);
        waitForHandlerAction(mGsmInboundSmsHandler.getHandler(), TEST_TIMEOUT);

        verify(mContext, times(2)).sendBroadcast(any(Intent.class));
        assertEquals("IdleState", getCurrentState().getName());
    }

    private void prepareMultiPartSms(boolean is3gpp2WapPush) {
        // Part 1
        mInboundSmsTrackerPart1 = new InboundSmsTracker(
                mSmsPdu, /* pdu */
                System.currentTimeMillis(), /* timestamp */
                -1, /* destPort */
                is3gpp2WapPush, /* is3gpp2 */
                "1234567890", /* address */
                "1234567890", /* displayAddress */
                1, /* referenceNumber */
                1, /* sequenceNumber */
                2, /* messageCount */
                is3gpp2WapPush, /* is3gpp2WapPdu */
                mMessageBodyPart1, /* messageBody */
                false, /* isClass0 */
                mSubId0);

        // Part 2
        mInboundSmsTrackerPart2 = new InboundSmsTracker(
                mSmsPdu, /* pdu */
                System.currentTimeMillis(), /* timestamp */
                -1, /* destPort */
                is3gpp2WapPush, /* is3gpp2 */
                "1234567890", /* address */
                "1234567890", /* displayAddress */
                1, /* referenceNumber */
                2, /* sequenceNumber */
                2, /* messageCount */
                is3gpp2WapPush, /* is3gpp2WapPdu */
                mMessageBodyPart2, /* messageBody */
                false, /* isClass0 */
                mSubId0);
    }

    @Test
    @MediumTest
    public void testMultiPartSmsWithIncompleteWAP() {
        /**
         * Test scenario: 3 messages are received with same address, ref number, count. two of the
         * messages are belonging to the same multi-part SMS and the other one is a 3GPP2WAP.
         * we should not try to merge 3gpp2wap with the multi-part SMS.
         */
        transitionFromStartupToIdle();

        // prepare SMS part 1 and part 2
        prepareMultiPartSms(false);

        mSmsHeader.concatRef = new SmsHeader.ConcatRef();
        doReturn(mSmsHeader).when(mGsmSmsMessage).getUserDataHeader();

        // part 2 of non-3gpp2wap arrives first
        doReturn(mInboundSmsTrackerPart2).when(mTelephonyComponentFactory)
                .makeInboundSmsTracker(nullable(byte[].class), anyLong(), anyInt(), anyBoolean(),
                        nullable(String.class), nullable(String.class), anyInt(), anyInt(),
                        anyInt(), anyBoolean(), nullable(String.class), anyBoolean(), anyInt());
        sendNewSms();

        // State machine should go back to idle and wait for second part
        assertEquals("IdleState", getCurrentState().getName());

        // mock a 3gpp2wap push
        prepareMultiPartSms(true);
        doReturn(mInboundSmsTrackerPart2).when(mTelephonyComponentFactory)
                .makeInboundSmsTracker(nullable(byte[].class), anyLong(), anyInt(), anyBoolean(),
                        nullable(String.class), nullable(String.class), anyInt(), anyInt(),
                        anyInt(), anyBoolean(), nullable(String.class), anyBoolean(), anyInt());
        sendNewSms();

        // State machine should go back to idle and wait for second part
        assertEquals("IdleState", getCurrentState().getName());

        // verify no broadcast sent.
        verify(mContext, times(0)).sendBroadcast(any(Intent.class));

        // additional copy of part 1 of non-3gpp2wap
        prepareMultiPartSms(false);
        doReturn(mInboundSmsTrackerPart1).when(mTelephonyComponentFactory)
                .makeInboundSmsTracker(nullable(byte[].class), anyLong(), anyInt(), anyBoolean(),
                        nullable(String.class), nullable(String.class), anyInt(), anyInt(),
                        anyInt(), anyBoolean(), nullable(String.class), anyBoolean(), anyInt());
        sendNewSms();

        // verify broadcast intents
        verifySmsIntentBroadcasts(0);
        assertEquals("IdleState", getCurrentState().getName());
        // verify there are three segments in the db and only one of them is not marked as deleted.
        assertEquals(3, mContentProvider.getNumRows());
        assertEquals(1, mContentProvider.query(sRawUri, null, "deleted=0", null, null).getCount());
    }

    @FlakyTest
    @Test
    @MediumTest
    public void testMultiPartSms() {
        transitionFromStartupToIdle();

        // prepare SMS part 1 and part 2
        prepareMultiPartSms(false);

        mSmsHeader.concatRef = new SmsHeader.ConcatRef();
        doReturn(mSmsHeader).when(mGsmSmsMessage).getUserDataHeader();

        doReturn(mInboundSmsTrackerPart1).when(mTelephonyComponentFactory)
                .makeInboundSmsTracker(nullable(byte[].class), anyLong(), anyInt(), anyBoolean(),
                        nullable(String.class), nullable(String.class), anyInt(), anyInt(),
                        anyInt(), anyBoolean(), nullable(String.class), anyBoolean(), anyInt());
        sendNewSms();

        // State machine should go back to idle and wait for second part
        assertEquals("IdleState", getCurrentState().getName());

        doReturn(mInboundSmsTrackerPart2).when(mTelephonyComponentFactory)
                .makeInboundSmsTracker(nullable(byte[].class), anyLong(), anyInt(), anyBoolean(),
                        nullable(String.class), nullable(String.class), anyInt(), anyInt(),
                        anyInt(), anyBoolean(), nullable(String.class), anyBoolean(), anyInt());
        sendNewSms();

        // verify broadcast intents
        verifySmsIntentBroadcasts(0);

        // if an additional copy of one of the segments above is received, it should not be kept in
        // the db and should not be combined with any subsequent messages received from the same
        // sender

        // additional copy of part 2 of message
        doReturn(mInboundSmsTrackerPart2).when(mTelephonyComponentFactory)
                .makeInboundSmsTracker(nullable(byte[].class), anyLong(), anyInt(), anyBoolean(),
                        nullable(String.class), nullable(String.class), anyInt(), anyInt(),
                        anyInt(), anyBoolean(), nullable(String.class), anyBoolean(), anyInt());
        sendNewSms();

        // verify no additional broadcasts sent
        verify(mContext, times(2)).sendBroadcast(any(Intent.class));

        // part 1 of new sms recieved from same sender with same parameters, just different
        // timestamps, should not be combined with the additional part 2 received above

        // call prepareMultiPartSms() to update timestamps
        prepareMultiPartSms(false);

        // part 1 of new sms
        doReturn(mInboundSmsTrackerPart1).when(mTelephonyComponentFactory)
                .makeInboundSmsTracker(nullable(byte[].class), anyLong(), anyInt(), anyBoolean(),
                        nullable(String.class), nullable(String.class), anyInt(), anyInt(),
                        anyInt(), anyBoolean(), nullable(String.class), anyBoolean(), anyInt());
        sendNewSms();

        // verify no additional broadcasts sent
        verify(mContext, times(2)).sendBroadcast(any(Intent.class));

        assertEquals("IdleState", getCurrentState().getName());
    }

    @Test
    @MediumTest
    public void testMultiPartIncompleteSms() {
        /**
         * Test scenario: 2 messages are received with same address, ref number, count, and
         * seqNumber, with count = 2 and seqNumber = 1. We should not try to merge these.
         */
        transitionFromStartupToIdle();

        // prepare SMS part 1 and part 2
        prepareMultiPartSms(false);
        // change seqNumber in part 2 to 1
        mInboundSmsTrackerPart2 = new InboundSmsTracker(
                mSmsPdu, /* pdu */
                System.currentTimeMillis(), /* timestamp */
                -1, /* destPort */
                false, /* is3gpp2 */
                "1234567890", /* address */
                "1234567890", /* displayAddress */
                1, /* referenceNumber */
                1, /* sequenceNumber */
                2, /* messageCount */
                false, /* is3gpp2WapPdu */
                mMessageBodyPart2, /* messageBody */
                false, /* isClass0 */
                mSubId0);

        mSmsHeader.concatRef = new SmsHeader.ConcatRef();
        doReturn(mSmsHeader).when(mGsmSmsMessage).getUserDataHeader();

        doReturn(mInboundSmsTrackerPart1).when(mTelephonyComponentFactory)
                .makeInboundSmsTracker(nullable(byte[].class), anyLong(), anyInt(), anyBoolean(),
                        nullable(String.class), nullable(String.class), anyInt(), anyInt(),
                        anyInt(), anyBoolean(), nullable(String.class), anyBoolean(), anyInt());
        sendNewSms();

        // State machine should go back to idle and wait for second part
        assertEquals("IdleState", getCurrentState().getName());

        doReturn(mInboundSmsTrackerPart2).when(mTelephonyComponentFactory)
                .makeInboundSmsTracker(nullable(byte[].class), anyLong(), anyInt(), anyBoolean(),
                        nullable(String.class), nullable(String.class), anyInt(), anyInt(),
                        anyInt(), anyBoolean(), nullable(String.class), anyBoolean(), anyInt());
        sendNewSms();

        // verify no broadcasts sent
        verify(mContext, never()).sendBroadcast(any(Intent.class));
        // verify there's only 1 of the segments in the db (other should be discarded as dup)
        assertEquals(1, mContentProvider.getNumRows());
        // verify the first one is discarded, and second message is present in the db
        Cursor c = mContentProvider.query(sRawUri, null, null, null, null);
        c.moveToFirst();
        assertEquals(mMessageBodyPart2, c.getString(c.getColumnIndex("message_body")));
        // State machine should go back to idle
        assertEquals("IdleState", getCurrentState().getName());
    }

    @Test
    @MediumTest
    public void testMultiPartSmsWithInvalidSeqNumber() {
        transitionFromStartupToIdle();

        // prepare SMS part 1 and part 2
        prepareMultiPartSms(false);

        mSmsHeader.concatRef = new SmsHeader.ConcatRef();
        doReturn(mSmsHeader).when(mGsmSmsMessage).getUserDataHeader();

        doReturn(mInboundSmsTrackerPart1).when(mTelephonyComponentFactory)
                .makeInboundSmsTracker(nullable(byte[].class), anyLong(), anyInt(), anyBoolean(),
                        nullable(String.class), nullable(String.class), anyInt(), anyInt(),
                        anyInt(), anyBoolean(), nullable(String.class), anyBoolean(), anyInt());
        sendNewSms();

        // verify the message is stored in the raw table
        assertEquals(1, mContentProvider.getNumRows());

        // State machine should go back to idle and wait for second part
        assertEquals("IdleState", getCurrentState().getName());

        // change seqNumber in part 2 to an invalid value
        int invalidSeqNumber = -1;
        mInboundSmsTrackerPart2 = new InboundSmsTracker(
                mSmsPdu, /* pdu */
                System.currentTimeMillis(), /* timestamp */
                -1, /* destPort */
                false, /* is3gpp2 */
                "1234567890", /* address */
                "1234567890", /* displayAddress */
                1, /* referenceNumber */
                invalidSeqNumber, /* sequenceNumber */
                2, /* messageCount */
                false, /* is3gpp2WapPdu */
                mMessageBodyPart2, /* messageBody */
                false, /* isClass0 */
                mSubId0);

        doReturn(mInboundSmsTrackerPart2).when(mTelephonyComponentFactory)
                .makeInboundSmsTracker(nullable(byte[].class), anyLong(), anyInt(), anyBoolean(),
                        nullable(String.class), nullable(String.class), anyInt(), anyInt(),
                        anyInt(), anyBoolean(), nullable(String.class), anyBoolean(), anyInt());
        sendNewSms();

        // verify no broadcasts sent
        verify(mContext, never()).sendBroadcast(any(Intent.class));
        // State machine should go back to idle
        assertEquals("IdleState", getCurrentState().getName());
    }

    @Test
    @MediumTest
    public void testMultipartSmsFromBlockedNumber_noBroadcastsSent() {
        mFakeBlockedNumberContentProvider.mBlockedNumbers.add("1234567890");

        transitionFromStartupToIdle();

        // prepare SMS part 1 and part 2
        prepareMultiPartSms(false);

        mSmsHeader.concatRef = new SmsHeader.ConcatRef();
        doReturn(mSmsHeader).when(mGsmSmsMessage).getUserDataHeader();
        doReturn(mInboundSmsTrackerPart1).when(mTelephonyComponentFactory)
                .makeInboundSmsTracker(nullable(byte[].class), anyLong(), anyInt(), anyBoolean(),
                        nullable(String.class), nullable(String.class), anyInt(), anyInt(),
                        anyInt(), anyBoolean(), nullable(String.class), anyBoolean(), anyInt());

        sendNewSms();

        // State machine should go back to idle and wait for second part
        assertEquals("IdleState", getCurrentState().getName());

        doReturn(mInboundSmsTrackerPart2).when(mTelephonyComponentFactory)
                .makeInboundSmsTracker(nullable(byte[].class), anyLong(), anyInt(), anyBoolean(),
                        nullable(String.class), nullable(String.class), anyInt(), anyInt(),
                        anyInt(), anyBoolean(), nullable(String.class), anyBoolean(), anyInt());
        sendNewSms();

        verify(mContext, never()).sendBroadcast(any(Intent.class));
        assertEquals("IdleState", getCurrentState().getName());
    }

    @Test
    @MediumTest
    public void testMultipartSmsFromBlockedEmail_noBroadcastsSent() {
        mFakeBlockedNumberContentProvider.mBlockedNumbers.add("1234567890@test.com");

        transitionFromStartupToIdle();

        // prepare SMS part 1 and part 2
        prepareMultiPartSms(false);
        // only the first SMS is configured with the display originating email address
        mInboundSmsTrackerPart1 = new InboundSmsTracker(
                mSmsPdu, /* pdu */
                System.currentTimeMillis(), /* timestamp */
                -1, /* destPort */
                false, /* is3gpp2 */
                "1234567890", /* address */
                "1234567890@test.com", /* displayAddress */
                1, /* referenceNumber */
                1, /* sequenceNumber */
                2, /* messageCount */
                false, /* is3gpp2WapPdu */
                mMessageBodyPart1, /* messageBody */
                false, /* isClass0 */
                mSubId0);

        mSmsHeader.concatRef = new SmsHeader.ConcatRef();
        doReturn(mSmsHeader).when(mGsmSmsMessage).getUserDataHeader();
        doReturn(mInboundSmsTrackerPart1).when(mTelephonyComponentFactory)
                .makeInboundSmsTracker(nullable(byte[].class), anyLong(), anyInt(), anyBoolean(),
                        nullable(String.class), nullable(String.class), anyInt(), anyInt(),
                        anyInt(), anyBoolean(), nullable(String.class), anyBoolean(), anyInt());

        sendNewSms();

        // State machine should go back to idle and wait for second part
        assertEquals("IdleState", getCurrentState().getName());

        doReturn(mInboundSmsTrackerPart2).when(mTelephonyComponentFactory)
                .makeInboundSmsTracker(nullable(byte[].class), anyLong(), anyInt(), anyBoolean(),
                        nullable(String.class), nullable(String.class), anyInt(), anyInt(),
                        anyInt(), anyBoolean(), nullable(String.class), anyBoolean(), anyInt());
        sendNewSms();

        verify(mContext, never()).sendBroadcast(any(Intent.class));
        assertEquals("IdleState", getCurrentState().getName());
    }

    @Test
    @MediumTest
    public void testBroadcastUndeliveredUserLocked() throws Exception {
        replaceInstance(SmsBroadcastUndelivered.class, "instance", null, null);
        doReturn(0).when(mMockInboundSmsTracker).getDestPort();

        // add a fake entry to db
        mContentProvider.insert(sRawUri, mMockInboundSmsTracker.getContentValues());

        // user locked
        UserManager userManager = (UserManager)mContext.getSystemService(Context.USER_SERVICE);
        doReturn(false).when(userManager).isUserUnlocked();

        SmsBroadcastUndelivered.initialize(mContext, mGsmInboundSmsHandler, mCdmaInboundSmsHandler);

        // verify that a broadcast receiver is registered for current user (user == null) based on
        // implementation in ContextFixture
        verify(mContext).registerReceiverAsUser(any(BroadcastReceiver.class), eq((UserHandle)null),
                any(IntentFilter.class), eq((String)null), eq((Handler)null));

        // wait for ScanRawTableThread
        waitForMs(100);

        // verify no broadcasts sent because due to !isUserUnlocked
        verify(mContext, never()).sendBroadcast(any(Intent.class));

        // when user unlocks the device, the message in db should be broadcast
        doReturn(true).when(userManager).isUserUnlocked();
        mContext.sendBroadcast(new Intent(Intent.ACTION_USER_UNLOCKED));
        // wait for ScanRawTableThread
        waitForMs(100);

        verifyDataSmsIntentBroadcasts(1);
    }

    @Test
    @MediumTest
    public void testBroadcastUndeliveredUserUnlocked() throws Exception {
        replaceInstance(SmsBroadcastUndelivered.class, "instance", null, null);
        doReturn(0).when(mMockInboundSmsTracker).getDestPort();

        // add a fake entry to db
        mContentProvider.insert(sRawUri, mMockInboundSmsTracker.getContentValues());

        SmsBroadcastUndelivered.initialize(mContext, mGsmInboundSmsHandler, mCdmaInboundSmsHandler);

        // wait for ScanRawTableThread
        waitForMs(100);

        // user is unlocked; intent should be broadcast right away
        verifyDataSmsIntentBroadcasts(0);
    }

    @Test
    @MediumTest
    public void testBroadcastUndeliveredDeleted() throws Exception {
        replaceInstance(SmsBroadcastUndelivered.class, "instance", null, null);
        SmsBroadcastUndelivered.initialize(mContext, mGsmInboundSmsHandler, mCdmaInboundSmsHandler);
        mInboundSmsTracker = new InboundSmsTracker(
                mSmsPdu, /* pdu */
                System.currentTimeMillis(), /* timestamp */
                0, /* destPort */
                false, /* is3gpp2 */
                false, /* is3gpp2WapPdu */
                "1234567890", /* address */
                "1234567890", /* displayAddress */
                mMessageBody, /* messageBody */
                false, /* isClass0 */
                mSubId0);
        doReturn(mInboundSmsTracker).when(mTelephonyComponentFactory)
                .makeInboundSmsTracker(nullable(byte[].class), anyLong(), anyInt(), anyBoolean(),
                anyBoolean(), nullable(String.class), nullable(String.class),
                nullable(String.class), anyBoolean(), anyInt());

        //add a fake entry to db
        ContentValues rawSms = new ContentValues();
        rawSms.put("deleted", 1);
        mContentProvider.insert(sRawUri, rawSms);

        //when user unlocks the device, broadcast should not be sent for new message
        mContext.sendBroadcast(new Intent(Intent.ACTION_USER_UNLOCKED));
        // wait for ScanRawTableThread
        waitForMs(100);

        verify(mContext, times(1)).sendBroadcast(any(Intent.class));
        assertEquals("IdleState", getCurrentState().getName());

    }

    @FlakyTest
    @Test
    @MediumTest
    public void testBroadcastUndeliveredMultiPart() throws Exception {
        replaceInstance(SmsBroadcastUndelivered.class, "instance", null, null);

        // prepare SMS part 1 and part 2
        prepareMultiPartSms(false);

        //add the 2 SMS parts to db
        mContentProvider.insert(sRawUri, mInboundSmsTrackerPart1.getContentValues());
        mContentProvider.insert(sRawUri, mInboundSmsTrackerPart2.getContentValues());

        //return InboundSmsTracker objects corresponding to the 2 parts
        doReturn(mInboundSmsTrackerPart1).doReturn(mInboundSmsTrackerPart2).
                when(mTelephonyComponentFactory).makeInboundSmsTracker(any(Cursor.class),
                anyBoolean());

        SmsBroadcastUndelivered.initialize(mContext, mGsmInboundSmsHandler, mCdmaInboundSmsHandler);
        // wait for ScanRawTableThread
        waitForMs(200);

        verifySmsIntentBroadcasts(0);
    }

    @Test
    @MediumTest
    public void testBroadcastUndeliveredMultiSim() throws Exception {
        replaceInstance(SmsBroadcastUndelivered.class, "instance", null, null);

        // add SMSs from different subs to db
        mContentProvider.insert(sRawUri, mMockInboundSmsTracker.getContentValues());
        mContentProvider.insert(sRawUri, mMockInboundSmsTrackerSub1.getContentValues());

        SmsBroadcastUndelivered.initialize(mContext, mGsmInboundSmsHandler, mCdmaInboundSmsHandler);
        // wait for ScanRawTableThread
        waitForMs(200);

        verifySmsIntentBroadcasts(0, mSubId0, true);
        verifySmsIntentBroadcasts(2, mSubId1, false);
    }
}
