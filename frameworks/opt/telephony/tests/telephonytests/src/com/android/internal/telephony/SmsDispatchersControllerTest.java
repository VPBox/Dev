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
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyString;
import static org.mockito.Matchers.isNull;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.app.ActivityManager;
import android.os.HandlerThread;
import android.os.Message;
import android.provider.Telephony.Sms.Intents;
import android.test.FlakyTest;
import android.test.suitebuilder.annotation.SmallTest;
import android.util.Singleton;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;

public class SmsDispatchersControllerTest extends TelephonyTest {
    @Mock
    private SMSDispatcher.SmsTracker mTracker;

    private SmsDispatchersController mSmsDispatchersController;
    private ImsSmsDispatcherTestHandler mImsSmsDispatcherTestHandler;
    private boolean mInjectionCallbackTriggered = false;
    private static final String TEST_INTENT = "com.android.internal.telephony.TEST_INTENT";
    private static final int TEST_TIMEOUT = 5000;

    private class ImsSmsDispatcherTestHandler extends HandlerThread {

        private ImsSmsDispatcherTestHandler(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            mSmsDispatchersController = new SmsDispatchersController(mPhone, mSmsStorageMonitor,
                    mSmsUsageMonitor);
            //Initial state of RIL is power on, need to wait util RADIO_ON msg get handled
            waitForMs(200);
            setReady(true);
        }
    }

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());
        setupMockPackagePermissionChecks();

        mImsSmsDispatcherTestHandler = new ImsSmsDispatcherTestHandler(getClass().getSimpleName());
        mImsSmsDispatcherTestHandler.start();
        waitUntilReady();
    }

    @After
    public void tearDown() throws Exception {
        mSmsDispatchersController = null;
        mImsSmsDispatcherTestHandler.quit();
        super.tearDown();
    }

    @Test @SmallTest @FlakyTest
    public void testSmsHandleStateUpdate() throws Exception {
        assertEquals(SmsConstants.FORMAT_UNKNOWN, mSmsDispatchersController.getImsSmsFormat());
        //Mock ImsNetWorkStateChange with GSM phone type
        switchImsSmsFormat(PhoneConstants.PHONE_TYPE_GSM);
        assertEquals(SmsConstants.FORMAT_3GPP, mSmsDispatchersController.getImsSmsFormat());
        assertTrue(mSmsDispatchersController.isIms());

        //Mock ImsNetWorkStateChange with Cdma Phone type
        switchImsSmsFormat(PhoneConstants.PHONE_TYPE_CDMA);
        assertEquals(SmsConstants.FORMAT_3GPP2, mSmsDispatchersController.getImsSmsFormat());
        assertTrue(mSmsDispatchersController.isIms());
    }

    @Test @SmallTest @FlakyTest
    public void testSendImsGmsTest() throws Exception {
        switchImsSmsFormat(PhoneConstants.PHONE_TYPE_GSM);
        mSmsDispatchersController.sendText("111"/* desAddr*/, "222" /*scAddr*/, TAG,
                null, null, null, null, false, -1, false, -1, false);
        verify(mSimulatedCommandsVerifier).sendImsGsmSms(eq("038122F2"),
                eq("0100038111F100001CD3F69C989EC3C3F431BA2C9F0FDF6EBAFCCD6697E5D4F29C0E"), eq(0), eq(0),
                any(Message.class));
    }

    @Test @SmallTest
    public void testSendImsGmsTestWithOutDesAddr() throws Exception {
        switchImsSmsFormat(PhoneConstants.PHONE_TYPE_GSM);
        mSmsDispatchersController.sendText(null, "222" /*scAddr*/, TAG,
                null, null, null, null, false, -1, false, -1, false);
        verify(mSimulatedCommandsVerifier, times(0)).sendImsGsmSms(anyString(), anyString(),
                anyInt(), anyInt(), any(Message.class));
    }

    @Test @SmallTest @FlakyTest /* flakes 0.73% of the time on gce, 0.57% on marlin */
    public void testSendImsCdmaTest() throws Exception {
        switchImsSmsFormat(PhoneConstants.PHONE_TYPE_CDMA);
        mSmsDispatchersController.sendText("111"/* desAddr*/, "222" /*scAddr*/, TAG,
                null, null, null, null, false, -1, false, -1, false);
        verify(mSimulatedCommandsVerifier).sendImsCdmaSms((byte[])any(), eq(0), eq(0),
                any(Message.class));
    }

    @Test @SmallTest @FlakyTest /* flakes 0.71% of the time on marlin, 0.61% on gce */
    public void testSendRetrySmsCdmaTest() throws Exception {
        // newFormat will be based on voice technology
        ArgumentCaptor<byte[]> captor = ArgumentCaptor.forClass(byte[].class);
        switchImsSmsFormat(PhoneConstants.PHONE_TYPE_CDMA);
        replaceInstance(SMSDispatcher.SmsTracker.class, "mFormat", mTracker,
                SmsConstants.FORMAT_3GPP2);
        doReturn(PhoneConstants.PHONE_TYPE_CDMA).when(mPhone).getPhoneType();
        mSmsDispatchersController.sendRetrySms(mTracker);
        verify(mSimulatedCommandsVerifier).sendImsCdmaSms(captor.capture(), eq(0), eq(0),
                any(Message.class));
        assertEquals(1, captor.getAllValues().size());
        assertNull(captor.getAllValues().get(0));
    }

    @Test @SmallTest @FlakyTest /* flakes 0.85% of the time on gce, 0.43% on marlin */
    public void testSendRetrySmsGsmTest() throws Exception {
        // newFormat will be based on voice technology will be GSM if phone type is not CDMA
        switchImsSmsFormat(PhoneConstants.PHONE_TYPE_GSM);
        replaceInstance(SMSDispatcher.SmsTracker.class, "mFormat", mTracker,
                SmsConstants.FORMAT_3GPP);
        mSmsDispatchersController.sendRetrySms(mTracker);
        verify(mSimulatedCommandsVerifier).sendImsGsmSms((String)isNull(), (String)isNull(), eq(0),
                eq(0), any(Message.class));
    }

    @Test @SmallTest
    public void testInjectNullSmsPdu() throws Exception {
        // unmock ActivityManager to be able to register receiver, create real PendingIntent and
        // receive TEST_INTENT
        restoreInstance(Singleton.class, "mInstance", mIActivityManagerSingleton);
        restoreInstance(ActivityManager.class, "IActivityManagerSingleton", null);

        // inject null sms pdu. This should cause intent to be received since pdu is null.
        mSmsDispatchersController.injectSmsPdu(null, SmsConstants.FORMAT_3GPP,
                (SmsDispatchersController.SmsInjectionCallback) result -> {
                    mInjectionCallbackTriggered = true;
                   assertEquals(Intents.RESULT_SMS_GENERIC_ERROR, result);
                }
        );
        waitForMs(100);
        assertEquals(true, mInjectionCallbackTriggered);
    }

    private void switchImsSmsFormat(int phoneType) {
        mSimulatedCommands.setImsRegistrationState(new int[]{1, phoneType});
        mSimulatedCommands.notifyImsNetworkStateChanged();
        /* wait for async msg get handled */
        waitForHandlerAction(mSmsDispatchersController, TEST_TIMEOUT);
        /* handle EVENT_IMS_STATE_DONE */
        waitForHandlerAction(mSmsDispatchersController, TEST_TIMEOUT);
        assertTrue(mSmsDispatchersController.isIms());
    }
}
