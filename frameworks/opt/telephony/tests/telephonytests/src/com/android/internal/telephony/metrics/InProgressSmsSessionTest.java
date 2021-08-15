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

package com.android.internal.telephony.metrics;

import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.telephony.TelephonyTest;
import com.android.internal.telephony.nano.TelephonyProto;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

public class InProgressSmsSessionTest extends TelephonyTest {

    private InProgressSmsSession mSmsSession;

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());
        mSmsSession = new InProgressSmsSession(mPhone.getPhoneId());
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
    }

    // Test add event
    @Test
    @SmallTest
    public void testAddEvent() {
        SmsSessionEventBuilder builder = new SmsSessionEventBuilder(
                TelephonyProto.SmsSession.Event.Type.SMS_SEND_RESULT)
                .setRilRequestId(2);
        mSmsSession.addEvent(builder);
        assertEquals(builder.build(), mSmsSession.events.getFirst());
        assertFalse(mSmsSession.isEventsDropped());
    }

    // Test dropped event scenario
    @Test
    @SmallTest
    public void testEventDropped() {
        for (int i = 0; i < 25; i++) {
            SmsSessionEventBuilder builder = new SmsSessionEventBuilder(
                    TelephonyProto.SmsSession.Event.Type.SMS_SEND_RESULT)
                    .setRilRequestId(i + 1);
            mSmsSession.addEvent(builder);
        }

        assertTrue(mSmsSession.isEventsDropped());
        assertEquals(6, mSmsSession.events.getFirst().rilRequestId);
    }

    // Test dropped event scenario
    @Test
    @SmallTest
    public void testExpectedResponse() {
        mSmsSession.increaseExpectedResponse();
        assertEquals(1, mSmsSession.getNumExpectedResponses());
        mSmsSession.decreaseExpectedResponse();
        assertEquals(0, mSmsSession.getNumExpectedResponses());

        for (int i = 0; i < 100; i++) {
            mSmsSession.increaseExpectedResponse();
        }

        assertEquals(100, mSmsSession.getNumExpectedResponses());

        for (int i = 0; i < 50; i++) {
            mSmsSession.decreaseExpectedResponse();
        }

        assertEquals(50, mSmsSession.getNumExpectedResponses());
    }
}
