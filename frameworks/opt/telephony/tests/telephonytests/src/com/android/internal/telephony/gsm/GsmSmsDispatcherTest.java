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

import static android.telephony.SmsManager.RESULT_ERROR_SHORT_CODE_NEVER_ALLOWED;
import static android.telephony.SmsManager.SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE;

import static com.android.internal.telephony.SmsUsageMonitor.PREMIUM_SMS_PERMISSION_NEVER_ALLOW;
import static com.android.internal.telephony.TelephonyTestUtils.waitForMs;

import static org.junit.Assert.assertEquals;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyInt;
import static org.mockito.Matchers.anyString;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.app.ActivityManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.location.Country;
import android.location.CountryDetector;
import android.os.HandlerThread;
import android.os.Message;
import android.os.SystemProperties;
import android.provider.Settings;
import android.provider.Telephony;
import android.telephony.SmsManager;
import android.test.suitebuilder.annotation.MediumTest;
import android.test.suitebuilder.annotation.SmallTest;
import android.util.Singleton;

import androidx.test.filters.FlakyTest;

import com.android.internal.telephony.ContextFixture;
import com.android.internal.telephony.ISub;
import com.android.internal.telephony.SMSDispatcher;
import com.android.internal.telephony.SmsDispatchersController;
import com.android.internal.telephony.TelephonyTest;
import com.android.internal.telephony.TelephonyTestUtils;
import com.android.internal.telephony.TestApplication;

import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;

import java.util.ArrayList;
import java.util.HashMap;

public class GsmSmsDispatcherTest extends TelephonyTest {

    private static final long TIMEOUT_MS = 500;

    @Mock
    private android.telephony.SmsMessage mSmsMessage;
    @Mock
    private SmsMessage mGsmSmsMessage;
    @Mock
    private SmsDispatchersController mSmsDispatchersController;
    @Mock
    private GsmInboundSmsHandler mGsmInboundSmsHandler;
    @Mock
    private CountryDetector mCountryDetector;
    @Mock
    private SMSDispatcher.SmsTracker mSmsTracker;
    @Mock
    private ISub.Stub mISubStub;
    private Object mLock = new Object();
    private boolean mReceivedTestIntent;
    private static final String TEST_INTENT = "com.android.internal.telephony.TEST_INTENT";
    private BroadcastReceiver mTestReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            logd("onReceive");
            synchronized (mLock) {
                mReceivedTestIntent = true;
            }
        }
    };

    private GsmSMSDispatcher mGsmSmsDispatcher;
    private GsmSmsDispatcherTestHandler mGsmSmsDispatcherTestHandler;

    private class GsmSmsDispatcherTestHandler extends HandlerThread {

        private GsmSmsDispatcherTestHandler(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            mGsmSmsDispatcher = new GsmSMSDispatcher(mPhone, mSmsDispatchersController,
                    mGsmInboundSmsHandler);
            setReady(true);
        }
    }

    @Before
    public void setUp() throws Exception {

        super.setUp(getClass().getSimpleName());

        // Note that this replaces only cached services in ServiceManager. If a service is not found
        // in the cache, a real instance is used.
        mServiceManagerMockedServices.put("isub", mISubStub);

        doReturn(mSmsUsageMonitor).when(mSmsDispatchersController).getUsageMonitor();
        mGsmSmsDispatcherTestHandler = new GsmSmsDispatcherTestHandler(getClass().getSimpleName());
        mGsmSmsDispatcherTestHandler.start();
        waitUntilReady();
    }

    @After
    public void tearDown() throws Exception {
        mGsmSmsDispatcher = null;
        mGsmSmsDispatcherTestHandler.quit();
        super.tearDown();
    }

    @Test @SmallTest
    public void testSmsStatus() {
        mSimulatedCommands.notifySmsStatus(new byte[]{(byte)0xFF, (byte)0xFF, (byte)0xFF});
        TelephonyTestUtils.waitForMs(50);
        verify(mSimulatedCommandsVerifier).acknowledgeLastIncomingGsmSms(true,
                Telephony.Sms.Intents.RESULT_SMS_HANDLED, null);
    }

    @Test @MediumTest
    public void testSendSmsToRegularNumber_doesNotNotifyblockedNumberProvider() throws Exception {
        setupMockPackagePermissionChecks();

        mContextFixture.setSystemService(Context.COUNTRY_DETECTOR, mCountryDetector);
        when(mCountryDetector.detectCountry())
                .thenReturn(new Country("US", Country.COUNTRY_SOURCE_SIM));

        mGsmSmsDispatcher.sendText("6501002000", "121" /*scAddr*/, "test sms",
                null, null, null, null, false, -1, false, -1, false);

        verify(mSimulatedCommandsVerifier).sendSMS(anyString(), anyString(), any(Message.class));
        // Blocked number provider is notified about the emergency contact asynchronously.
        TelephonyTestUtils.waitForMs(50);
        assertEquals(0, mFakeBlockedNumberContentProvider.mNumEmergencyContactNotifications);
    }

    @FlakyTest
    @Ignore
    @Test @MediumTest
    public void testSendSmsToEmergencyNumber_notifiesBlockedNumberProvider() throws Exception {
        setupMockPackagePermissionChecks();

        mContextFixture.setSystemService(Context.COUNTRY_DETECTOR, mCountryDetector);
        when(mCountryDetector.detectCountry())
                .thenReturn(new Country("US", Country.COUNTRY_SOURCE_SIM));

        mGsmSmsDispatcher.sendText(
                getEmergencyNumberFromSystemPropertiesOrDefault(), "121" /*scAddr*/, "test sms",
                null, null, null, null, false, -1, false, -1, false);

        verify(mSimulatedCommandsVerifier).sendSMS(anyString(), anyString(), any(Message.class));
        // Blocked number provider is notified about the emergency contact asynchronously.
        TelephonyTestUtils.waitForMs(50);
        assertEquals(1, mFakeBlockedNumberContentProvider.mNumEmergencyContactNotifications);
    }

    @Test @SmallTest
    public void testSmsMessageValidityPeriod() throws Exception {
        int vp;
        vp = SmsMessage.getRelativeValidityPeriod(-5);
        assertEquals(-1, vp);

        vp = SmsMessage.getRelativeValidityPeriod(100);
        assertEquals(100 / 5 - 1, vp);
    }

    private String getEmergencyNumberFromSystemPropertiesOrDefault() {
        String systemEmergencyNumbers = SystemProperties.get("ril.ecclist");
        if (systemEmergencyNumbers == null) {
            return "911";
        } else {
            return systemEmergencyNumbers.split(",")[0];
        }
    }

    @Test
    @SmallTest
    @FlakyTest
    @Ignore
    public void testSendTextWithInvalidDestAddr() throws Exception {
        // unmock ActivityManager to be able to register receiver, create real PendingIntent and
        // receive TEST_INTENT
        restoreInstance(Singleton.class, "mInstance", mIActivityManagerSingleton);
        restoreInstance(ActivityManager.class, "IActivityManagerSingleton", null);
        Context realContext = TestApplication.getAppContext();
        realContext.registerReceiver(mTestReceiver, new IntentFilter(TEST_INTENT));
        PendingIntent pendingIntent = PendingIntent.getBroadcast(realContext, 0,
                new Intent(TEST_INTENT), 0);
        // send invalid dest address: +
        mReceivedTestIntent = false;
        mGsmSmsDispatcher.sendText("+", "222" /*scAddr*/, TAG,
                pendingIntent, null, null, null, false, -1, false, -1, false);
        waitForMs(500);
        verify(mSimulatedCommandsVerifier, times(0)).sendSMS(anyString(), anyString(),
                any(Message.class));
        synchronized (mLock) {
            assertEquals(true, mReceivedTestIntent);
            assertEquals(SmsManager.RESULT_ERROR_NULL_PDU, mTestReceiver.getResultCode());
        }
    }

    @Test
    public void testSendRawPduWithEventStopSending() throws Exception {
        setupMockPackagePermissionChecks();
        mContextFixture.removeCallingOrSelfPermission(ContextFixture.PERMISSION_ENABLE_ALL);

        // return a fake value to pass getData()
        HashMap data = new HashMap<String, String>();
        data.put("pdu", new byte[1]);
        when(mSmsTracker.getData()).thenReturn(data);

        // Set values to return to simulate EVENT_STOP_SENDING
        when(mSmsUsageMonitor.checkDestination(any(), any()))
                .thenReturn(SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE);
        when(mSmsUsageMonitor.getPremiumSmsPermission(any()))
                .thenReturn(PREMIUM_SMS_PERMISSION_NEVER_ALLOW);
        when(mSmsTracker.getAppPackageName()).thenReturn("");

        // Settings.Global.DEVICE_PROVISIONED to 1
        Settings.Global.putInt(mContext.getContentResolver(),
                Settings.Global.DEVICE_PROVISIONED, 1);

        mGsmSmsDispatcher.sendRawPdu(mSmsTracker);
        waitForHandlerAction(mGsmSmsDispatcher, TIMEOUT_MS);

        verify(mSmsUsageMonitor, times(1)).checkDestination(any(), any());
        verify(mSmsUsageMonitor, times(1)).getPremiumSmsPermission(any());
        ArgumentCaptor<Integer> argumentCaptor = ArgumentCaptor
                .forClass(Integer.class);
        verify(mSmsTracker, times(1)).onFailed(any(), argumentCaptor.capture(), anyInt());
        assertEquals(RESULT_ERROR_SHORT_CODE_NEVER_ALLOWED, (int) argumentCaptor.getValue());
    }

    @Test @SmallTest
    public void testSendMultipartTextWithInvalidText() throws Exception {
        // unmock ActivityManager to be able to register receiver, create real PendingIntent and
        // receive TEST_INTENT
        restoreInstance(Singleton.class, "mInstance", mIActivityManagerSingleton);
        restoreInstance(ActivityManager.class, "IActivityManagerSingleton", null);

        Context realContext = TestApplication.getAppContext();
        realContext.registerReceiver(mTestReceiver, new IntentFilter(TEST_INTENT));

        // initiate parameters for an invalid text MO SMS (the 2nd segmeant has 161 characters)
        ArrayList<String> parts = new ArrayList<>();
        parts.add("valid segment1");
        parts.add("too long segment2 12345678912345678912345678912345678912345678912345678912345678"
                + "91234567891234567891234567891234567891234567891234567891234567891234567891234567"
                + "8");

        ArrayList<PendingIntent> sentIntents = new ArrayList<>();
        PendingIntent sentIntent = PendingIntent.getBroadcast(realContext, 0,
                new Intent(TEST_INTENT), 0);
        sentIntents.add(sentIntent);
        sentIntents.add(sentIntent);

        // send SMS and check sentIntent
        mReceivedTestIntent = false;
        mGsmSmsDispatcher.sendMultipartText("+123" /*destAddr*/, "222" /*scAddr*/, parts,
                sentIntents, null, null, null, false, -1, false, -1);

        waitForMs(500);
        synchronized (mLock) {
            assertEquals(true, mReceivedTestIntent);
            assertEquals(SmsManager.RESULT_ERROR_GENERIC_FAILURE, mTestReceiver.getResultCode());
        }
    }
}
