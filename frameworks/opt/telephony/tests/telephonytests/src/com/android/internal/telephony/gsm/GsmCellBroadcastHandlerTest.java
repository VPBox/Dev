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

import static android.provider.Settings.Secure.CMAS_ADDITIONAL_BROADCAST_PKG;

import static org.junit.Assert.assertEquals;
import static org.mockito.Matchers.nullable;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyBoolean;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.Manifest;
import android.app.Activity;
import android.app.AppOpsManager;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.UserHandle;
import android.provider.Settings;
import android.provider.Telephony;

import com.android.internal.telephony.SmsStorageMonitor;
import com.android.internal.telephony.TelephonyTest;
import com.android.internal.telephony.TelephonyTestUtils;

import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;

import java.util.List;

public class GsmCellBroadcastHandlerTest extends TelephonyTest {
    @Mock
    private SmsStorageMonitor mSmsStorageMonitor;
    @Mock
    private android.telephony.SmsMessage mSmsMessage;
    @Mock
    private SmsMessage mGsmSmsMessage;

    private GsmCellBroadcastHandler mGsmCellBroadcastHandler;
    private GsmCellBroadcastHandlerTestHandler mGsmCellBroadcastHandlerTestHandler;

    private class GsmCellBroadcastHandlerTestHandler extends HandlerThread {

        private GsmCellBroadcastHandlerTestHandler(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            mGsmCellBroadcastHandler = GsmCellBroadcastHandler.makeGsmCellBroadcastHandler(
                    mContextFixture.getTestDouble(), mPhone);
            setReady(true);
        }
    }

    @Before
    public void setUp() throws Exception {

        super.setUp(getClass().getSimpleName());

        doReturn(true).when(mTelephonyManager).getSmsReceiveCapableForPhone(anyInt(), anyBoolean());
        doReturn(true).when(mSmsStorageMonitor).isStorageAvailable();

        mGsmCellBroadcastHandlerTestHandler =
                new GsmCellBroadcastHandlerTestHandler(getClass().getSimpleName());
        mGsmCellBroadcastHandlerTestHandler.start();
        waitUntilReady();
    }

    @After
    public void tearDown() throws Exception {
        mGsmCellBroadcastHandler = null;
        mGsmCellBroadcastHandlerTestHandler.quit();
        super.tearDown();
    }

    @Ignore
    public void testBroadcastSms() {
        mContextFixture.putStringArrayResource(
                com.android.internal.R.array.config_defaultCellBroadcastReceiverPkgs,
                new String[]{"fake.cellbroadcastreceiver"});

        Settings.Secure.putString(mContext.getContentResolver(),
                CMAS_ADDITIONAL_BROADCAST_PKG, "another.fake.pkg");
        mSimulatedCommands.notifyGsmBroadcastSms(new byte[] {
                (byte)0xc0, //geographical scope
                (byte)0x01, //serial number
                (byte)0x01, //serial number
                (byte)0x01, //message identifier
                (byte)0x01, //message identifier
                (byte)0x01
        });
        TelephonyTestUtils.waitForMs(100);
        ArgumentCaptor<Intent> intentArgumentCaptor = ArgumentCaptor.forClass(Intent.class);
        verify(mContextFixture.getTestDouble(), times(2)).sendOrderedBroadcastAsUser(
                intentArgumentCaptor.capture(), eq(UserHandle.ALL),
                eq(Manifest.permission.RECEIVE_SMS), eq(AppOpsManager.OP_RECEIVE_SMS),
                nullable(BroadcastReceiver.class), any(Handler.class), eq(Activity.RESULT_OK),
                eq(null), eq(null));

        List<Intent> intentList = intentArgumentCaptor.getAllValues();

        assertEquals(Telephony.Sms.Intents.SMS_CB_RECEIVED_ACTION,
                intentList.get(0).getAction());
        assertEquals("another.fake.pkg", intentList.get(0).getPackage());

        assertEquals(Telephony.Sms.Intents.SMS_CB_RECEIVED_ACTION,
                intentList.get(1).getAction());
        assertEquals("fake.cellbroadcastreceiver", intentList.get(1).getPackage());
    }
}
