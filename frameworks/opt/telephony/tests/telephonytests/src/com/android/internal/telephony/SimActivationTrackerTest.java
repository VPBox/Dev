
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

import junit.framework.AssertionFailedError;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.security.InvalidParameterException;

import static org.junit.Assert.assertEquals;
import static android.telephony.TelephonyManager.SIM_ACTIVATION_STATE_ACTIVATED;
import static android.telephony.TelephonyManager.SIM_ACTIVATION_STATE_UNKNOWN;
import static android.telephony.TelephonyManager.SIM_ACTIVATION_STATE_RESTRICTED;
import static android.telephony.TelephonyManager.SIM_ACTIVATION_STATE_DEACTIVATED;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

public class SimActivationTrackerTest extends TelephonyTest {
    private SimActivationTracker mSAT;

    @Before
    public void setUp() throws Exception {
        super.setUp("SimActivaitonTrackerTest");
        mSAT = new SimActivationTracker(mPhone);
    }

    @Test
    public void testSetVoiceActivationState() {
        // verify initial state is unknown
        assertEquals(SIM_ACTIVATION_STATE_UNKNOWN, mSAT.getVoiceActivationState());

        // verify activated state is set successfully
        try {
            mSAT.setVoiceActivationState(SIM_ACTIVATION_STATE_ACTIVATED);
        } catch (IllegalArgumentException ex) {
            fail("Exception in setVoiceActivationState: " + ex);
        }

        assertEquals(SIM_ACTIVATION_STATE_ACTIVATED, mSAT.getVoiceActivationState());
        verify(mPhone, times(1)).notifyVoiceActivationStateChanged(
                eq(SIM_ACTIVATION_STATE_ACTIVATED));

        // verify fails to set restricted voice activation state
        try {
            mSAT.setVoiceActivationState(SIM_ACTIVATION_STATE_RESTRICTED);
            fail("Expect exception in setVoiceActivationState with wrong state: "
                    + SIM_ACTIVATION_STATE_RESTRICTED);
        } catch (IllegalArgumentException ex) {
            //test pass
        }
        assertEquals(SIM_ACTIVATION_STATE_ACTIVATED, mSAT.getVoiceActivationState());
        verify(mPhone, times(0)).notifyVoiceActivationStateChanged(
                eq(SIM_ACTIVATION_STATE_RESTRICTED));
    }

    @Test
    public void testSetDataActivationState() {
        // verify initial state is unknown
        assertEquals(SIM_ACTIVATION_STATE_UNKNOWN, mSAT.getDataActivationState());

        // verify deactivated state is set successfully
        try {
            mSAT.setDataActivationState(SIM_ACTIVATION_STATE_DEACTIVATED);
        } catch (InvalidParameterException ex) {
            fail("Exception in setDataActivationState: " + ex);
        }
        assertEquals(SIM_ACTIVATION_STATE_DEACTIVATED, mSAT.getDataActivationState());
        verify(mPhone, times(1)).notifyDataActivationStateChanged(
                eq(SIM_ACTIVATION_STATE_DEACTIVATED));

        // verify set restricted data activation state successfully
        try {
            mSAT.setDataActivationState(SIM_ACTIVATION_STATE_RESTRICTED);
        } catch (InvalidParameterException ex) {
            fail("Exception in setDataActivationState: " + ex);
        }

        assertEquals(SIM_ACTIVATION_STATE_RESTRICTED, mSAT.getDataActivationState());
        verify(mPhone, times(1)).notifyDataActivationStateChanged(
                eq(SIM_ACTIVATION_STATE_RESTRICTED));
    }

    @After
    public void tearDown() throws Exception {
        mSAT = null;
        super.tearDown();
    }

    /**
     * Fails a test with the given message.
     */
    static public void fail(String message) {
        if (message == null) {
            throw new AssertionFailedError();
        }
        throw new AssertionFailedError(message);
    }
}