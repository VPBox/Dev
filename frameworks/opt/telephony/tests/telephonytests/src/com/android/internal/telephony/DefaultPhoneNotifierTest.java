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

import static org.junit.Assert.assertEquals;
import static org.mockito.Matchers.anyString;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.os.Bundle;
import android.telephony.CellInfo;
import android.telephony.DisconnectCause;
import android.telephony.PreciseCallState;
import android.telephony.PreciseDisconnectCause;
import android.telephony.SignalStrength;
import android.telephony.TelephonyManager;
import android.telephony.gsm.GsmCellLocation;
import android.test.suitebuilder.annotation.SmallTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;

import java.util.ArrayList;
import java.util.List;

public class DefaultPhoneNotifierTest extends TelephonyTest {

    private DefaultPhoneNotifier mDefaultPhoneNotifierUT;
    @Mock
    ITelephonyRegistry.Stub mTelephonyRegisteryMock;
    @Mock
    SignalStrength mSignalStrength;
    @Mock
    CellInfo mCellInfo;
    @Mock
    GsmCdmaCall mForeGroundCall;
    @Mock
    GsmCdmaCall mBackGroundCall;
    @Mock
    GsmCdmaCall mRingingCall;

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());
        mServiceManagerMockedServices.put("telephony.registry", mTelephonyRegisteryMock);
        doReturn(mTelephonyRegisteryMock).when(mTelephonyRegisteryMock)
                .queryLocalInterface(anyString());

        mDefaultPhoneNotifierUT = new DefaultPhoneNotifier();
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
    }

    @Test @SmallTest
    public void testNotifyCallForwarding() throws Exception {
        mDefaultPhoneNotifierUT.notifyCallForwardingChanged(mPhone);
        verify(mTelephonyRegisteryMock).notifyCallForwardingChangedForSubscriber(eq(0), eq(false));

        doReturn(true).when(mPhone).getCallForwardingIndicator();
        doReturn(1).when(mPhone).getSubId();
        mDefaultPhoneNotifierUT.notifyCallForwardingChanged(mPhone);
        verify(mTelephonyRegisteryMock).notifyCallForwardingChangedForSubscriber(eq(1), eq(true));
    }

    @Test @SmallTest
    public void testNotifyDataActivity() throws Exception {
        //mock data activity state
        doReturn(Phone.DataActivityState.NONE).when(mPhone).getDataActivityState();
        mDefaultPhoneNotifierUT.notifyDataActivity(mPhone);
        verify(mTelephonyRegisteryMock).notifyDataActivityForSubscriber(eq(0),
                eq(TelephonyManager.DATA_ACTIVITY_NONE));

        doReturn(1).when(mPhone).getSubId();
        doReturn(Phone.DataActivityState.DATAIN).when(mPhone).getDataActivityState();
        mDefaultPhoneNotifierUT.notifyDataActivity(mPhone);
        verify(mTelephonyRegisteryMock).notifyDataActivityForSubscriber(eq(1),
                eq(TelephonyManager.DATA_ACTIVITY_IN));
    }

    @Test @SmallTest
    public void testNotifySignalStrength() throws Exception {
        //mock signal strength value
        doReturn(99).when(mSignalStrength).getGsmSignalStrength();
        doReturn(mSignalStrength).when(mPhone).getSignalStrength();
        ArgumentCaptor<SignalStrength> signalStrengthArgumentCaptor =
                ArgumentCaptor.forClass(SignalStrength.class);

        mDefaultPhoneNotifierUT.notifySignalStrength(mPhone);
        verify(mTelephonyRegisteryMock).notifySignalStrengthForPhoneId(eq(0), eq(0),
                signalStrengthArgumentCaptor.capture());
        assertEquals(99, signalStrengthArgumentCaptor.getValue().getGsmSignalStrength());

        doReturn(1).when(mPhone).getSubId();
        doReturn(2).when(mPhone).getPhoneId();
        mDefaultPhoneNotifierUT.notifySignalStrength(mPhone);
        verify(mTelephonyRegisteryMock).notifySignalStrengthForPhoneId(eq(2), eq(1),
                signalStrengthArgumentCaptor.capture());
        assertEquals(99, signalStrengthArgumentCaptor.getValue().getGsmSignalStrength());
    }

    @Test @SmallTest
    public void testNotifyCellInfo() throws Exception {
        //mock cellinfo
        List<CellInfo> mCellInfoList = new ArrayList<>();
        mCellInfoList.add(mCellInfo);
        ArgumentCaptor<List> cellInfoArgumentCaptor = ArgumentCaptor.forClass(List.class);

        mDefaultPhoneNotifierUT.notifyCellInfo(mPhone, mCellInfoList);

        verify(mTelephonyRegisteryMock).notifyCellInfoForSubscriber(eq(0),
                cellInfoArgumentCaptor.capture());
        assertEquals(mCellInfo, cellInfoArgumentCaptor.getValue().get(0));
    }

    @Test @SmallTest
    public void testNotifyMessageWaiting() throws Exception {
        doReturn(1).when(mPhone).getPhoneId();
        mDefaultPhoneNotifierUT.notifyMessageWaitingChanged(mPhone);
        verify(mTelephonyRegisteryMock).notifyMessageWaitingChangedForPhoneId(1, 0, false);

        doReturn(2).when(mPhone).getPhoneId();
        mDefaultPhoneNotifierUT.notifyMessageWaitingChanged(mPhone);
        verify(mTelephonyRegisteryMock).notifyMessageWaitingChangedForPhoneId(2, 0, false);

        doReturn(1).when(mPhone).getSubId();
        mDefaultPhoneNotifierUT.notifyMessageWaitingChanged(mPhone);
        verify(mTelephonyRegisteryMock).notifyMessageWaitingChangedForPhoneId(2, 1, false);

        doReturn(true).when(mPhone).getMessageWaitingIndicator();
        mDefaultPhoneNotifierUT.notifyMessageWaitingChanged(mPhone);
        verify(mTelephonyRegisteryMock).notifyMessageWaitingChangedForPhoneId(2, 1, true);
    }

    @Test @SmallTest
    public void testNotifyDisconnectCause() throws Exception {
        doReturn(1).when(mPhone).getPhoneId();
        doReturn(0).when(mPhone).getSubId();
        mDefaultPhoneNotifierUT.notifyDisconnectCause(mPhone, DisconnectCause.NOT_VALID,
                PreciseDisconnectCause.FDN_BLOCKED);
        verify(mTelephonyRegisteryMock).notifyDisconnectCause(1, 0, DisconnectCause.NOT_VALID,
                PreciseDisconnectCause.FDN_BLOCKED);

        mDefaultPhoneNotifierUT.notifyDisconnectCause(mPhone, DisconnectCause.LOCAL,
                PreciseDisconnectCause.CHANNEL_NOT_AVAIL);
        verify(mTelephonyRegisteryMock).notifyDisconnectCause(1, 0, DisconnectCause.LOCAL,
                PreciseDisconnectCause.CHANNEL_NOT_AVAIL);
    }

    @Test @SmallTest
    public void testNotifyDataConnectionFailed() throws Exception {
        mDefaultPhoneNotifierUT.notifyDataConnectionFailed(mPhone, "APN_0");
        verify(mTelephonyRegisteryMock).notifyDataConnectionFailedForSubscriber(0, 0, "APN_0");

        mDefaultPhoneNotifierUT.notifyDataConnectionFailed(mPhone, "APN_1");
        verify(mTelephonyRegisteryMock).notifyDataConnectionFailedForSubscriber(0, 0, "APN_1");

        doReturn(1).when(mPhone).getSubId();
        mDefaultPhoneNotifierUT.notifyDataConnectionFailed(mPhone, "APN_1");
        verify(mTelephonyRegisteryMock).notifyDataConnectionFailedForSubscriber(0,1, "APN_1");
    }

    @Test @SmallTest
    public void testNotifyPreciseCallState() throws Exception {

        //mock forground/background/ringing call and call state
        doReturn(Call.State.IDLE).when(mForeGroundCall).getState();
        doReturn(Call.State.IDLE).when(mBackGroundCall).getState();
        doReturn(Call.State.IDLE).when(mRingingCall).getState();

        mDefaultPhoneNotifierUT.notifyPreciseCallState(mPhone);
        verify(mTelephonyRegisteryMock, times(0)).notifyPreciseCallState(
                anyInt(), anyInt(), anyInt(), anyInt(), anyInt());

        doReturn(mForeGroundCall).when(mPhone).getForegroundCall();
        mDefaultPhoneNotifierUT.notifyPreciseCallState(mPhone);
        verify(mTelephonyRegisteryMock, times(0)).notifyPreciseCallState(
                anyInt(), anyInt(), anyInt(), anyInt(), anyInt());

        doReturn(mBackGroundCall).when(mPhone).getBackgroundCall();
        mDefaultPhoneNotifierUT.notifyPreciseCallState(mPhone);
        verify(mTelephonyRegisteryMock, times(0)).notifyPreciseCallState(
                anyInt(), anyInt(), anyInt(), anyInt(), anyInt());

        doReturn(mRingingCall).when(mPhone).getRingingCall();
        mDefaultPhoneNotifierUT.notifyPreciseCallState(mPhone);
        verify(mTelephonyRegisteryMock, times(1)).notifyPreciseCallState(
                mPhone.getPhoneId(),
                mPhone.getSubId(),
                PreciseCallState.PRECISE_CALL_STATE_IDLE,
                PreciseCallState.PRECISE_CALL_STATE_IDLE,
                PreciseCallState.PRECISE_CALL_STATE_IDLE);

        doReturn(Call.State.ACTIVE).when(mForeGroundCall).getState();
        mDefaultPhoneNotifierUT.notifyPreciseCallState(mPhone);
        verify(mTelephonyRegisteryMock, times(1)).notifyPreciseCallState(
                mPhone.getPhoneId(),
                mPhone.getSubId(),
                PreciseCallState.PRECISE_CALL_STATE_IDLE,
                PreciseCallState.PRECISE_CALL_STATE_ACTIVE,
                PreciseCallState.PRECISE_CALL_STATE_IDLE);

        doReturn(Call.State.HOLDING).when(mBackGroundCall).getState();
        mDefaultPhoneNotifierUT.notifyPreciseCallState(mPhone);
        verify(mTelephonyRegisteryMock, times(1)).notifyPreciseCallState(
                mPhone.getPhoneId(),
                mPhone.getSubId(),
                PreciseCallState.PRECISE_CALL_STATE_IDLE,
                PreciseCallState.PRECISE_CALL_STATE_ACTIVE,
                PreciseCallState.PRECISE_CALL_STATE_HOLDING);

        doReturn(Call.State.ALERTING).when(mRingingCall).getState();
        mDefaultPhoneNotifierUT.notifyPreciseCallState(mPhone);
        verify(mTelephonyRegisteryMock, times(1)).notifyPreciseCallState(
                mPhone.getPhoneId(),
                mPhone.getSubId(),
                PreciseCallState.PRECISE_CALL_STATE_ALERTING,
                PreciseCallState.PRECISE_CALL_STATE_ACTIVE,
                PreciseCallState.PRECISE_CALL_STATE_HOLDING);
    }

    @Test @SmallTest
    public void testNotifyCellLocation() throws Exception {
        // mock gsm cell location
        GsmCellLocation mGsmCellLocation = new GsmCellLocation();
        mGsmCellLocation.setLacAndCid(2, 3);
        doReturn(mGsmCellLocation).when(mPhone).getCellLocation();
        ArgumentCaptor<Bundle> cellLocationCapture =
                ArgumentCaptor.forClass(Bundle.class);

        mDefaultPhoneNotifierUT.notifyCellLocation(mPhone, mGsmCellLocation);
        verify(mTelephonyRegisteryMock).notifyCellLocationForSubscriber(eq(0),
                cellLocationCapture.capture());
        assertEquals(2, cellLocationCapture.getValue().getInt("lac"));
        assertEquals(3, cellLocationCapture.getValue().getInt("cid"));
        assertEquals(-1, cellLocationCapture.getValue().getInt("psc"));

        doReturn(1).when(mPhone).getSubId();
        mGsmCellLocation.setPsc(5);
        mDefaultPhoneNotifierUT.notifyCellLocation(mPhone, mGsmCellLocation);
        verify(mTelephonyRegisteryMock).notifyCellLocationForSubscriber(eq(1),
                cellLocationCapture.capture());
        assertEquals(2, cellLocationCapture.getValue().getInt("lac"));
        assertEquals(3, cellLocationCapture.getValue().getInt("cid"));
        assertEquals(5, cellLocationCapture.getValue().getInt("psc"));
    }

    @Test @SmallTest
    public void testNotifyOtaspChanged() throws Exception {
        mDefaultPhoneNotifierUT.notifyOtaspChanged(mPhone, TelephonyManager.OTASP_NEEDED);
        verify(mTelephonyRegisteryMock).notifyOtaspChanged(eq(mPhone.getSubId()),
                eq(TelephonyManager.OTASP_NEEDED));

        mDefaultPhoneNotifierUT.notifyOtaspChanged(mPhone, TelephonyManager.OTASP_UNKNOWN);
        verify(mTelephonyRegisteryMock).notifyOtaspChanged(eq(mPhone.getSubId()),
                eq(TelephonyManager.OTASP_UNKNOWN));
    }
}
