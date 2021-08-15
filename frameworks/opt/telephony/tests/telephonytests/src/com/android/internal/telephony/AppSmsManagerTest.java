/*
 * Copyright 2018 The Android Open Source Project
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

import static junit.framework.Assert.fail;

import android.content.Intent;
import android.provider.Telephony.Sms.Intents;
import android.telephony.SmsMessage;

import com.android.internal.telephony.uicc.IccUtils;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class AppSmsManagerTest extends TelephonyTest {

    /**
     * The sms message information represent by the {@link #PDU}.
     *
     * SMSC#+31624000000
     * Sender:+31641600986
     * TimeStamp:26/08/02 19:37:41
     * TP_PID:00
     * TP_DCS:00
     * TP_DCS-popis:Uncompressed Text
     * class:0
     * Alphabet:Default
     * How are you?
     * Length:12
     */
    private static final String PDU =
            "07911326040000F0040B911346610089F60000208062917314080CC8F71D14969741F977FD07";

    private AppSmsManager mAppSmsManagerUT;

    @Before
    public void setUp() throws Exception {
        super.setUp("AppSmsManagerTest");
        mAppSmsManagerUT = new AppSmsManager(mContextFixture.getTestDouble());
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
    }

    @Test
    public void testHandleSmsReceivedIntent() {
        Intent intent = new Intent();
        intent.setAction(Intents.SMS_DELIVER_ACTION);
        intent.putExtra("pdus", new byte[][]{IccUtils.hexStringToBytes(PDU), null, null});
        intent.putExtra("format", SmsMessage.FORMAT_3GPP);

        try {
            // Assumes the AppSmsManager can handle the null pdu.
            mAppSmsManagerUT.handleSmsReceivedIntent(intent);
        } catch (NullPointerException ex) {
            fail("Test failed because of null pointer exception " + ex);
        }
    }
}
