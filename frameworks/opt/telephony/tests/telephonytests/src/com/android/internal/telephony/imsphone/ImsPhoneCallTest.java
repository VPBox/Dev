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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.fail;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.telephony.ims.ImsStreamMediaProfile;
import android.test.suitebuilder.annotation.SmallTest;

import androidx.test.filters.FlakyTest;

import com.android.internal.telephony.Call;
import com.android.internal.telephony.TelephonyTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.mockito.Mock;

public class ImsPhoneCallTest extends TelephonyTest {
    @Mock
    ImsPhoneConnection mConnection1;
    @Mock
    ImsPhoneConnection mConnection2;

    ImsStreamMediaProfile mMediaProfile;

    private ImsPhoneCall mImsCallUT;

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());
        replaceInstance(ImsPhoneCallTracker.class, "mPhone", mImsCT, mImsPhone);

        mImsCallUT = new ImsPhoneCall(mImsCT, ImsPhoneCall.CONTEXT_FOREGROUND);
        when(mImsCT.getPhone()).thenReturn(mImsPhone);
        mMediaProfile = new ImsStreamMediaProfile();
    }

    @After
    public void tearDown() throws Exception {
        mImsCallUT = null;
        super.tearDown();
    }

    @FlakyTest
    @Ignore
    @Test
    @SmallTest
    public void testAttachDetach() {
        //verify mConnections has 0 connections and is in IDLE state
        assertEquals(0, mImsCallUT.mConnections.size());
        assertEquals(Call.State.IDLE, mImsCallUT.getState());

        //attach
        mImsCallUT.attach(mConnection1, Call.State.ACTIVE);

        //verify mConnections has 1 connection and is not in idle
        assertEquals(1, mImsCallUT.mConnections.size());
        assertEquals(Call.State.ACTIVE, mImsCallUT.getState());

        //detach
        mImsCallUT.detach(mConnection1);

        //verify mConnections has 0 connections and is in IDLE state
        assertEquals(0, mImsCallUT.mConnections.size());
        assertEquals(Call.State.IDLE, mImsCallUT.getState());
    }

    @FlakyTest
    @Ignore
    @Test
    @SmallTest
    public void testConnectionDisconnected() {
        mImsCallUT.attach(mConnection1, Call.State.ACTIVE);
        mImsCallUT.attach(mConnection2, Call.State.ACTIVE);
        //both connections are active, state not change
        mImsCallUT.connectionDisconnected(null);
        assertEquals(Call.State.ACTIVE, mImsCallUT.getState());
        // only one attached connection get disconnected, state not changed
        doReturn(Call.State.DISCONNECTED).when(mConnection1).getState();
        mImsCallUT.connectionDisconnected(null);
        assertEquals(Call.State.ACTIVE, mImsCallUT.getState());
        doReturn(Call.State.DISCONNECTED).when(mConnection2).getState();
        mImsCallUT.connectionDisconnected(null);
        assertEquals(Call.State.DISCONNECTED, mImsCallUT.getState());
    }

    @FlakyTest
    @Ignore
    @Test
    @SmallTest
    public void testHangup() {
        try {
            mImsCallUT.hangup();
            verify(mImsCT).hangup(eq(mImsCallUT));
        } catch (Exception e) {
            fail("Exception " + e + " not expected");
        }
    }

    @Test
    @SmallTest
    public void testUpdateRingBackTone() {
        //Mock local tone
        mMediaProfile.mAudioDirection = ImsStreamMediaProfile.DIRECTION_INACTIVE;
        mImsCallProfile.mMediaProfile = mMediaProfile;

        mImsCallUT.update(null, mImsCall, Call.State.ALERTING);
        verify(mImsPhone, times(1)).startRingbackTone();
        assertEquals(Call.State.ALERTING, mImsCallUT.getState());
        mImsCallUT.update(null, mImsCall, Call.State.ACTIVE);
        verify(mImsPhone, times(1)).stopRingbackTone();
        assertEquals(Call.State.ACTIVE, mImsCallUT.getState());
    }

    @Test
    @SmallTest
    public void testStopRingingOnHandover() {
        //Mock local tone
        mMediaProfile.mAudioDirection = ImsStreamMediaProfile.DIRECTION_INACTIVE;
        mImsCallProfile.mMediaProfile = mMediaProfile;

        mImsCallUT.update(null, mImsCall, Call.State.ALERTING);
        verify(mImsPhone, times(1)).startRingbackTone();
        assertEquals(Call.State.ALERTING, mImsCallUT.getState());

        // Emulate ringback terminate on handover.
        mImsCallUT.maybeStopRingback();
        verify(mImsPhone, times(1)).stopRingbackTone();
    }

    @Test
    @SmallTest
    public void testSwitchWith() {
        // this call in active state with connection 1 attached
        mImsCallUT.attach(mConnection1, Call.State.ACTIVE);
        // that call in idle state with connection 2 attached
        ImsPhoneCall mImsCallThat = new ImsPhoneCall(mImsCT, ImsPhoneCall.CONTEXT_FOREGROUND);
        mImsCallThat.attach(mConnection2, Call.State.IDLE);

        mImsCallUT.switchWith(mImsCallThat);
        assertEquals(Call.State.ACTIVE, mImsCallThat.getState());
        assertEquals(Call.State.IDLE, mImsCallUT.getState());
        assertEquals(mConnection1, mImsCallThat.getConnections().get(0));
        assertEquals(mConnection2, mImsCallUT.getConnections().get(0));
    }

    @FlakyTest
    @Ignore
    @Test
    @SmallTest
    public void testMultiParty() {
        doReturn(mImsCall).when(mConnection1).getImsCall();
        assertFalse(mImsCallUT.isMultiparty());
        verify(mImsCall, times(0)).isMultiparty();

        //get the ImsCall from the first Connection
        mImsCallUT.attach(mConnection1, Call.State.ACTIVE);
        mImsCallUT.isMultiparty();
        verify(mImsCall, times(1)).isMultiparty();
    }
}
