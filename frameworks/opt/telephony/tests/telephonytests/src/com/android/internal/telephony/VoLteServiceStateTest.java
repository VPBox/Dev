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

import android.os.Bundle;
import android.os.Parcel;
import android.telephony.VoLteServiceState;
import android.test.suitebuilder.annotation.SmallTest;

import junit.framework.TestCase;


public class VoLteServiceStateTest extends TestCase {

    @SmallTest
    public void testGetState() {
        VoLteServiceState state = new VoLteServiceState(VoLteServiceState.HANDOVER_STARTED);
        assertEquals(VoLteServiceState.HANDOVER_STARTED, state.getSrvccState());
    }

    @SmallTest
    public void testParcel() {
        VoLteServiceState state = new VoLteServiceState(VoLteServiceState.HANDOVER_FAILED);
        Parcel p = Parcel.obtain();
        state.writeToParcel(p, 0);
        p.setDataPosition(0);
        VoLteServiceState newState = VoLteServiceState.CREATOR.createFromParcel(p);
        assertEquals(state, newState);
    }

    @SmallTest
    public void testCreateFromBundle() {
        Bundle b = new Bundle();
        b.putInt("mSrvccState", VoLteServiceState.HANDOVER_COMPLETED);
        assertEquals(VoLteServiceState.HANDOVER_COMPLETED,
                VoLteServiceState.newFromBundle(b).getSrvccState());
    }

    @SmallTest
    public void testFillInNotifierBundle() {
        Bundle b = new Bundle();
        VoLteServiceState state = new VoLteServiceState(VoLteServiceState.HANDOVER_CANCELED);
        state.fillInNotifierBundle(b);
        assertEquals(VoLteServiceState.HANDOVER_CANCELED, b.getInt("mSrvccState"));
    }

    @SmallTest
    public void testCopy() {
        VoLteServiceState state = new VoLteServiceState(VoLteServiceState.HANDOVER_STARTED);
        VoLteServiceState newState = new VoLteServiceState(state);
        assertEquals(state, newState);
    }
}

