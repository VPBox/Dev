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
 * limitations under the License
 */

package com.android.internal.telephony;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.ArgumentMatchers.nullable;
import static org.mockito.Mockito.spy;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.os.Looper;
import android.telephony.SmsMessage;
import android.telephony.ims.stub.ImsSmsImplBase;
import android.test.suitebuilder.annotation.SmallTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;

import java.util.HashMap;

public class ImsSmsDispatcherTest extends TelephonyTest {
    @Mock private SmsDispatchersController mSmsDispatchersController;
    @Mock private SMSDispatcher.SmsTracker mSmsTracker;
    private HashMap<String, Object> mTrackerData;
    private ImsSmsDispatcher mImsSmsDispatcher;

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());
        if (Looper.myLooper() == null) {
            Looper.prepare();
        }

        mImsSmsDispatcher = spy(new ImsSmsDispatcher(mPhone, mSmsDispatchersController));
        when(mSmsDispatchersController.isIms()).thenReturn(true);

        mTrackerData = new HashMap<>(1);
        when(mSmsTracker.getData()).thenReturn(mTrackerData);
    }

    /**
     * Send an SMS and verify that the token and PDU is correct.
     */
    @Test
    @SmallTest
    public void testSendSms() throws Exception {
        int token = mImsSmsDispatcher.mNextToken.get();
        int trackersSize = mImsSmsDispatcher.mTrackers.size();

        byte[] pdu = com.android.internal.telephony.gsm.SmsMessage.getSubmitPdu(null,
                "+15555551212", "Test", false).encodedMessage;
        mTrackerData.put("pdu", pdu);
        when(mImsManager.getSmsFormat()).thenReturn(SmsMessage.FORMAT_3GPP);
        when(mPhone.getPhoneType()).thenReturn(PhoneConstants.PHONE_TYPE_GSM);

        //Send an SMS
        mImsSmsDispatcher.sendSms(mSmsTracker);

        assertEquals(token + 1, mImsSmsDispatcher.mNextToken.get());
        assertEquals(trackersSize + 1, mImsSmsDispatcher.mTrackers.size());
        verify(mImsManager).sendSms(eq(token + 1), anyInt(), eq(SmsMessage.FORMAT_3GPP),
                nullable(String.class), eq(false), eq(pdu));
    }

    /**
     * Ensure that when sending a GSM text fails with SEND_STATUS_ERROR_FALLBACK, retry with
     * a non-zero retry count (set TP-RD).
     */
    @Test
    @SmallTest
    public void testFallbackGsmRetry() throws Exception {
        int token = mImsSmsDispatcher.mNextToken.get();
        mTrackerData.put("pdu", com.android.internal.telephony.gsm.SmsMessage.getSubmitPdu(null,
                "+15555551212", "Test", false).encodedMessage);
        mImsSmsDispatcher.mTrackers.put(token, mSmsTracker);
        when(mPhone.getPhoneType()).thenReturn(PhoneConstants.PHONE_TYPE_GSM);

        // Fallback over GSM
        mImsSmsDispatcher.getSmsListener().onSendSmsResult(token, 0,
                ImsSmsImplBase.SEND_STATUS_ERROR_FALLBACK, 0);
        ArgumentCaptor<SMSDispatcher.SmsTracker> captor =
                ArgumentCaptor.forClass(SMSDispatcher.SmsTracker.class);
        // Ensure GsmSmsDispatcher calls sendSms
        verify(mSmsDispatchersController).sendRetrySms(captor.capture());

        assertNotNull(captor.getValue());
        assertTrue(captor.getValue().mRetryCount > 0);

    }

    /**
     * Ensure that when an outgoing SMS has failed over IMS with SEND_STATUS_ERROR_RETRY, it is
     * sent over the IMS channel again with the TP-RD bit set.
     */
    @Test
    @SmallTest
    public void testErrorImsRetry() throws Exception {
        int token = mImsSmsDispatcher.mNextToken.get();
        mTrackerData.put("pdu", com.android.internal.telephony.gsm.SmsMessage.getSubmitPdu(null,
                "+15555551212", "Test", false).encodedMessage);
        when(mImsManager.getSmsFormat()).thenReturn(SmsMessage.FORMAT_3GPP);
        mImsSmsDispatcher.mTrackers.put(token, mSmsTracker);
        when(mPhone.getPhoneType()).thenReturn(PhoneConstants.PHONE_TYPE_GSM);

        // Fallback over GSM
        mImsSmsDispatcher.getSmsListener().onSendSmsResult(token, 0,
                ImsSmsImplBase.SEND_STATUS_ERROR_RETRY, 0);

        // Make sure retry bit set
        ArgumentCaptor<byte[]> byteCaptor = ArgumentCaptor.forClass(byte[].class);
        verify(mImsManager).sendSms(eq(token + 1), anyInt(), nullable(String.class),
                nullable(String.class), eq(true), byteCaptor.capture());
        byte[] pdu = byteCaptor.getValue();
        // Make sure that TP-RD is set for this message
        assertNotNull(pdu);
        assertEquals(0x04, (pdu[0] & 0x04));
    }

    @After
    public void tearDown() throws Exception {
        mImsSmsDispatcher = null;
        super.tearDown();
    }
}
