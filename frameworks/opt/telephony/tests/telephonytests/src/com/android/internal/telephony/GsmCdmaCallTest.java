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

import android.test.suitebuilder.annotation.SmallTest;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;

public class GsmCdmaCallTest extends TelephonyTest {

    @Mock GsmCdmaConnection mConnection1;
    @Mock GsmCdmaConnection mConnection2;
    @Mock DriverCall mDriverCall;

    private GsmCdmaCall mCallUnderTest;

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());
        mCallUnderTest = new GsmCdmaCall(mCT);
    }

    @After
    public void tearDown() throws Exception {
        mCallUnderTest = null;
        super.tearDown();
    }

    @Test @SmallTest
    public void testAttachDetach() {
        //verify mConnections has 0 connections and is in IDLE state
        assertEquals(0, mCallUnderTest.mConnections.size());
        assertEquals(Call.State.IDLE, mCallUnderTest.getState());

        //attach
        mDriverCall.state = DriverCall.State.ACTIVE;
        mCallUnderTest.attach(mConnection1, mDriverCall);

        //verify mConnections has 1 connection and is not in idle
        assertEquals(1, mCallUnderTest.mConnections.size());
        assertEquals(Call.State.ACTIVE, mCallUnderTest.getState());

        //detach
        mCallUnderTest.detach(mConnection1);

        //verify mConnections has 0 connections and is in IDLE state
        assertEquals(0, mCallUnderTest.mConnections.size());
        assertEquals(Call.State.IDLE, mCallUnderTest.getState());
    }

    @Test @SmallTest
    public void testMultiparty() {
        //verify mConnections has 0 connections and is in IDLE state
        assertEquals(0, mCallUnderTest.mConnections.size());
        assertEquals(Call.State.IDLE, mCallUnderTest.getState());

        //verify isMultiparty is false
        assertEquals(false, mCallUnderTest.isMultiparty());

        //attach
        mDriverCall.state = DriverCall.State.ACTIVE;
        mCallUnderTest.attach(mConnection1, mDriverCall);

        //verify isMultiparty is false
        assertEquals(false, mCallUnderTest.isMultiparty());

        //attach
        mCallUnderTest.attach(mConnection2, mDriverCall);

        //verify isMultiparty is true
        assertEquals(true, mCallUnderTest.isMultiparty());
    }

    @Test @SmallTest
    public void testHangup() {
        //verify hangup calls mCT.hangup
        try {
            mCallUnderTest.hangup();
            verify(mCT).hangup(mCallUnderTest);
        } catch (Exception e) {
            fail("Exception " + e + " not expected");
        }
    }

    @Test @SmallTest
    public void testConnectionDisconnected() {
        //attach
        mDriverCall.state = DriverCall.State.ACTIVE;
        mCallUnderTest.attach(mConnection1, mDriverCall);
        mCallUnderTest.attach(mConnection2, mDriverCall);

        //both connections are active, state not change
        mCallUnderTest.connectionDisconnected(null);
        assertEquals(Call.State.ACTIVE, mCallUnderTest.getState());

        // only one attached connection get disconnected, state not changed
        doReturn(Call.State.DISCONNECTED).when(mConnection1).getState();
        mCallUnderTest.connectionDisconnected(null);
        assertEquals(Call.State.ACTIVE, mCallUnderTest.getState());

        doReturn(Call.State.DISCONNECTED).when(mConnection2).getState();
        mCallUnderTest.connectionDisconnected(null);
        assertEquals(Call.State.DISCONNECTED, mCallUnderTest.getState());
    }

}