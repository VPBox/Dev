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

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.anyObject;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.os.Looper;
import android.os.Message;
import android.telephony.TelephonyManager;
import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.R;
import com.android.internal.telephony.TelephonyIntents;
import com.android.internal.telephony.TelephonyTest;
import com.android.internal.telephony.uicc.IccCardStatus.CardState;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

public class UiccStateChangedLauncherTest extends TelephonyTest {
    private static final String TAG = UiccStateChangedLauncherTest.class.getName();
    private static final int CARD_COUNT = 1;
    private static final String PROVISIONING_PACKAGE_NAME = "test.provisioning.package";

    @Mock
    private Context mContext;
    @Mock
    private Resources mResources;

    private IccCardStatus makeCardStatus(CardState state) {
        IccCardStatus status = new IccCardStatus();
        status.setCardState(state.ordinal());
        status.mApplications = new IccCardApplicationStatus[0];
        status.mCdmaSubscriptionAppIndex = -1;
        status.mGsmUmtsSubscriptionAppIndex = -1;
        status.mImsSubscriptionAppIndex = -1;
        return status;
    }

    @Before
    public void setUp() throws Exception {
        super.setUp(TAG);

        MockitoAnnotations.initMocks(this);
        when(mContext.getResources()).thenReturn(mResources);
        when(TelephonyManager.getDefault().getPhoneCount()).thenReturn(CARD_COUNT);
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
    }

    @Test @SmallTest
    public void testProvisioningPackageSet() {
        // deviceProvisioningPackage is set.
        when(mResources.getString(eq(R.string.config_deviceProvisioningPackage)))
                .thenReturn(PROVISIONING_PACKAGE_NAME);

        if (Looper.myLooper() == null) {
            Looper.prepare();
        }

        UiccStateChangedLauncher uiccLauncher =
                new UiccStateChangedLauncher(mContext, UiccController.getInstance());
        ArgumentCaptor<Integer> integerArgumentCaptor = ArgumentCaptor.forClass(Integer.class);
        verify(UiccController.getInstance(), times(1)).registerForIccChanged(eq(uiccLauncher),
                integerArgumentCaptor.capture(),
                anyObject());
        Message msg = Message.obtain();
        msg.what = integerArgumentCaptor.getValue();

        // The first broadcast should be sent after initialization.
        UiccCard card = new UiccCard(mContext, mSimulatedCommands,
                makeCardStatus(CardState.CARDSTATE_PRESENT), 0 /* phoneId */, new Object());
        when(UiccController.getInstance().getUiccCardForPhone(0)).thenReturn(card);
        uiccLauncher.handleMessage(msg);

        ArgumentCaptor<Intent> intentArgumentCaptor = ArgumentCaptor.forClass(Intent.class);

        // Amount of sent broadcasts to the device provisioning package.
        int broadcast_count = 1;
        verify(mContext, times(broadcast_count)).sendBroadcast(intentArgumentCaptor.capture());
        assertEquals(PROVISIONING_PACKAGE_NAME, intentArgumentCaptor.getValue().getPackage());
        assertEquals(TelephonyIntents.ACTION_SIM_STATE_CHANGED,
                intentArgumentCaptor.getValue().getAction());

        // Card state's changed to restricted. Broadcast should be sent.
        card.update(mContext, mSimulatedCommands,
                makeCardStatus(CardState.CARDSTATE_RESTRICTED));
        uiccLauncher.handleMessage(msg);

        broadcast_count++;
        verify(mContext, times(broadcast_count)).sendBroadcast(intentArgumentCaptor.capture());
        assertEquals(PROVISIONING_PACKAGE_NAME, intentArgumentCaptor.getValue().getPackage());
        assertEquals(TelephonyIntents.ACTION_SIM_STATE_CHANGED,
                intentArgumentCaptor.getValue().getAction());

        // Nothing's changed. Broadcast should not be sent.
        uiccLauncher.handleMessage(msg);
        verify(mContext, times(broadcast_count)).sendBroadcast(any(Intent.class));

        // Card state's changed from restricted. Broadcast should be sent.
        card.update(mContext, mSimulatedCommands,
                makeCardStatus(CardState.CARDSTATE_PRESENT));
        uiccLauncher.handleMessage(msg);

        broadcast_count++;
        verify(mContext, times(broadcast_count)).sendBroadcast(intentArgumentCaptor.capture());
        assertEquals(PROVISIONING_PACKAGE_NAME, intentArgumentCaptor.getValue().getPackage());
        assertEquals(TelephonyIntents.ACTION_SIM_STATE_CHANGED,
                intentArgumentCaptor.getValue().getAction());
    }

    @Test @SmallTest
    public void testProvisioningPackageUnset() {
        // deviceProvisionigPackage is not set.
        when(mResources.getString(eq(R.string.config_deviceProvisioningPackage)))
                .thenReturn(null);

        if (Looper.myLooper() == null) {
            Looper.prepare();
        }

        UiccStateChangedLauncher uiccLauncher =
                new UiccStateChangedLauncher(mContext, UiccController.getInstance());
        verify(UiccController.getInstance(), never()).registerForIccChanged(eq(uiccLauncher),
                anyInt(), anyObject());
    }
}
