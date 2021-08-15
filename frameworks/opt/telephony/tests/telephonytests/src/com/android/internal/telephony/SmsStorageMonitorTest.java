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

import android.content.Intent;
import android.os.HandlerThread;
import android.os.Message;
import android.provider.Telephony;
import android.test.suitebuilder.annotation.SmallTest;
import android.util.Log;

import com.android.internal.telephony.test.SimulatedCommands;
import com.android.internal.telephony.test.SimulatedCommandsVerifier;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.lang.reflect.Field;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

public class SmsStorageMonitorTest extends TelephonyTest {

    private SmsStorageMonitor mSmsStorageMonitor;
    private SmsStorageMonitorTestHandler mSmsStorageMonitorTestHandler;

    private class SmsStorageMonitorTestHandler extends HandlerThread {

        private SmsStorageMonitorTestHandler(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            mSmsStorageMonitor = new SmsStorageMonitor(mPhone);
            setReady(true);
        }
    }

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());
        mSmsStorageMonitorTestHandler = new SmsStorageMonitorTestHandler(TAG);
        mSmsStorageMonitorTestHandler.start();
        waitUntilReady();
    }

    @After
    public void tearDown() throws Exception {
        mSmsStorageMonitor = null;
        mSmsStorageMonitorTestHandler.quit();
        super.tearDown();
    }

    @Test @SmallTest
    public void testEventIccFull() {
        // Notify icc sms full
        mSimulatedCommands.notifyIccSmsFull();
        TelephonyTestUtils.waitForMs(50);

        // SIM_FULL_ACTION intent should be broadcast
        ArgumentCaptor<Intent> intentArgumentCaptor = ArgumentCaptor.forClass(Intent.class);
        verify(mContextFixture.getTestDouble()).sendBroadcast(intentArgumentCaptor.capture());
        assertEquals(Telephony.Sms.Intents.SIM_FULL_ACTION,
                intentArgumentCaptor.getValue().getAction());
    }

    @Test @SmallTest
    public void testSmsMemoryStatus() {
        // Notify radio on
        mSimulatedCommands.notifyRadioOn();
        TelephonyTestUtils.waitForMs(50);

        verify(mSimulatedCommandsVerifier, never()).reportSmsMemoryStatus(anyBoolean(),
                any(Message.class));

        // Send DEVICE_STORAGE_FULL
        mContextFixture.getTestDouble().sendBroadcast(
                new Intent(Intent.ACTION_DEVICE_STORAGE_FULL));
        TelephonyTestUtils.waitForMs(50);

        verify(mSimulatedCommandsVerifier).reportSmsMemoryStatus(eq(false), any(Message.class));
        assertFalse(mSmsStorageMonitor.isStorageAvailable());

        mSimulatedCommands.notifyRadioOn();
        TelephonyTestUtils.waitForMs(50);

        verify(mSimulatedCommandsVerifier).reportSmsMemoryStatus(eq(false), any(Message.class));

        // Send DEVICE_STORAGE_NOT_FULL
        mContextFixture.getTestDouble().sendBroadcast(
                new Intent(Intent.ACTION_DEVICE_STORAGE_NOT_FULL));
        TelephonyTestUtils.waitForMs(50);

        verify(mSimulatedCommandsVerifier).reportSmsMemoryStatus(eq(true), any(Message.class));
        assertTrue(mSmsStorageMonitor.isStorageAvailable());

        mSimulatedCommands.notifyRadioOn();
        TelephonyTestUtils.waitForMs(50);

        verify(mSimulatedCommandsVerifier).reportSmsMemoryStatus(eq(true), any(Message.class));
    }
}