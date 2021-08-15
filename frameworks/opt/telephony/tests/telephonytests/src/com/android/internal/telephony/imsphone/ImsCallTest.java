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
 * limitations under the License
 */

package com.android.internal.telephony.imsphone;

import android.os.Bundle;
import android.telephony.ServiceState;
import android.test.suitebuilder.annotation.SmallTest;

import com.android.ims.ImsCall;
import android.telephony.ims.ImsCallProfile;

import com.android.internal.telephony.TelephonyTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.doReturn;

public class ImsCallTest extends TelephonyTest {

    private Bundle mBundle;
    private ImsCallProfile mTestCallProfile;

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());
        mTestCallProfile = new ImsCallProfile();
        mBundle = mTestCallProfile.mCallExtras;
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
    }

    @Test
    @SmallTest
    public void testSetWifi() {
        ImsCall mTestImsCall = new ImsCall(mContext, mTestCallProfile);
        assertFalse(mTestImsCall.isWifiCall());
        assertNotEquals(mTestImsCall.getRadioTechnology(), ServiceState.RIL_RADIO_TECHNOLOGY_LTE);
        mBundle.putString(ImsCallProfile.EXTRA_CALL_RAT_TYPE,
                ServiceState.RIL_RADIO_TECHNOLOGY_IWLAN + "");
        assertTrue(mTestImsCall.isWifiCall());
        assertNotEquals(mTestImsCall.getRadioTechnology(), ServiceState.RIL_RADIO_TECHNOLOGY_LTE);
    }

    @Test
    @SmallTest
    public void testSetWifiAlt() {
        ImsCall mTestImsCall = new ImsCall(mContext, mTestCallProfile);
        assertFalse(mTestImsCall.isWifiCall());
        assertNotEquals(mTestImsCall.getRadioTechnology(), ServiceState.RIL_RADIO_TECHNOLOGY_LTE);
        mBundle.putString(ImsCallProfile.EXTRA_CALL_RAT_TYPE_ALT,
                ServiceState.RIL_RADIO_TECHNOLOGY_IWLAN + "");
        assertTrue(mTestImsCall.isWifiCall());
        assertNotEquals(mTestImsCall.getRadioTechnology(), ServiceState.RIL_RADIO_TECHNOLOGY_LTE);
    }

    @Test
    @SmallTest
    public void testSetLteNoWifi() {
        ImsCall mTestImsCall = new ImsCall(mContext, mTestCallProfile);
        assertFalse(mTestImsCall.isWifiCall());
        assertNotEquals(mTestImsCall.getRadioTechnology(), ServiceState.RIL_RADIO_TECHNOLOGY_LTE);
        mBundle.putString(ImsCallProfile.EXTRA_CALL_RAT_TYPE,
                ServiceState.RIL_RADIO_TECHNOLOGY_LTE + "");
        assertFalse(mTestImsCall.isWifiCall());
        assertEquals(mTestImsCall.getRadioTechnology(), ServiceState.RIL_RADIO_TECHNOLOGY_LTE);
    }

    @Test
    @SmallTest
    public void testSetLteNoWifiAlt() {
        ImsCall mTestImsCall = new ImsCall(mContext, mTestCallProfile);
        assertFalse(mTestImsCall.isWifiCall());
        assertNotEquals(mTestImsCall.getRadioTechnology(), ServiceState.RIL_RADIO_TECHNOLOGY_LTE);
        mBundle.putString(ImsCallProfile.EXTRA_CALL_RAT_TYPE_ALT,
                ServiceState.RIL_RADIO_TECHNOLOGY_LTE + "");
        assertFalse(mTestImsCall.isWifiCall());
        assertEquals(mTestImsCall.getRadioTechnology(), ServiceState.RIL_RADIO_TECHNOLOGY_LTE);
    }
}
