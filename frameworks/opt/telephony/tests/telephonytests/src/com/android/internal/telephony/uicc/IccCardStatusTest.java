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
package com.android.internal.telephony.uicc;

import android.test.suitebuilder.annotation.SmallTest;
import android.util.Log;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import static org.junit.Assert.assertEquals;

public class IccCardStatusTest {

    private IccCardStatus mIccCardStatus;
    private static final String TAG = "ICCCardStatusTest";
    @Mock
    private IccCardApplicationStatus mApplicationStatus;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mIccCardStatus = new IccCardStatus();
        /* set only one applications for cdma */
        mIccCardStatus.mApplications = new IccCardApplicationStatus[]{mApplicationStatus};
        mIccCardStatus.mCdmaSubscriptionAppIndex = 0;
        mIccCardStatus.mGsmUmtsSubscriptionAppIndex = -1;
        mIccCardStatus.mCdmaSubscriptionAppIndex = -1;

    }

    @After
    public void tearDown() throws Exception {
        mIccCardStatus = null;
    }

    @Test
    @SmallTest
    public void testSetCardState() {
        mIccCardStatus.setCardState(IccCardStatus.CardState.CARDSTATE_ABSENT.ordinal());
        assertEquals(IccCardStatus.CardState.CARDSTATE_ABSENT, mIccCardStatus.mCardState);
        logd(mIccCardStatus.toString());

        mIccCardStatus.setCardState(IccCardStatus.CardState.CARDSTATE_PRESENT.ordinal());
        assertEquals(IccCardStatus.CardState.CARDSTATE_PRESENT, mIccCardStatus.mCardState);
        logd(mIccCardStatus.toString());

        mIccCardStatus.setCardState(IccCardStatus.CardState.CARDSTATE_ERROR.ordinal());
        assertEquals(IccCardStatus.CardState.CARDSTATE_ERROR, mIccCardStatus.mCardState);
        logd(mIccCardStatus.toString());
    }

    @Test
    @SmallTest
    public void testSetPinState() {
        mIccCardStatus.setUniversalPinState(IccCardStatus.PinState.PINSTATE_UNKNOWN.ordinal());
        assertEquals(IccCardStatus.PinState.PINSTATE_UNKNOWN, mIccCardStatus.mUniversalPinState);
        logd(mIccCardStatus.toString());

        mIccCardStatus.setUniversalPinState(
                IccCardStatus.PinState.PINSTATE_ENABLED_BLOCKED.ordinal());
        assertEquals(IccCardStatus.PinState.PINSTATE_ENABLED_BLOCKED,
                     mIccCardStatus.mUniversalPinState);
        logd(mIccCardStatus.toString());
    }

    private static void logd(String s) {
        Log.d(TAG, s);
    }
}
