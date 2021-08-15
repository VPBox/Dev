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

import android.os.AsyncResult;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.telephony.DisconnectCause;
import android.telephony.PhoneNumberUtils;
import android.telephony.ServiceState;
import android.telephony.ims.ImsCallProfile;
import android.test.suitebuilder.annotation.MediumTest;
import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.telephony.Call;
import com.android.internal.telephony.Connection;
import com.android.internal.telephony.GsmCdmaCall;
import com.android.internal.telephony.PhoneConstants;
import com.android.internal.telephony.TelephonyTest;

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

import java.lang.reflect.Field;

import static com.android.internal.telephony.TelephonyTestUtils.waitForMs;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyChar;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

public class ImsPhoneConnectionTest extends TelephonyTest {
    private ImsPhoneConnection mConnectionUT;
    private Bundle mBundle = new Bundle();
    @Mock
    private ImsPhoneCall mForeGroundCall;
    @Mock
    private ImsPhoneCall mBackGroundCall;
    @Mock
    private ImsPhoneCall mRingGroundCall;

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());
        replaceInstance(Handler.class, "mLooper", mImsCT, Looper.getMainLooper());
        replaceInstance(ImsPhoneCallTracker.class, "mForegroundCall", mImsCT, mForeGroundCall);
        replaceInstance(ImsPhoneCallTracker.class, "mBackgroundCall", mImsCT, mBackGroundCall);
        replaceInstance(ImsPhoneCallTracker.class, "mRingingCall", mImsCT, mRingGroundCall);
        replaceInstance(ImsPhoneCallTracker.class, "mPhone", mImsCT, mImsPhone);

        mImsCallProfile.mCallExtras = mBundle;
        doReturn(ImsPhoneCall.State.IDLE).when(mForeGroundCall).getState();
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
    }

    @Test
    @SmallTest
    public void testImsConnectionSanity() {
        logd("Testing initial state of MT ImsPhoneConnection");
        mConnectionUT = new ImsPhoneConnection(mImsPhone, mImsCall, mImsCT, mForeGroundCall, false);

        assertEquals(ImsPhoneCall.State.IDLE, mConnectionUT.getState());
        assertEquals(PhoneConstants.PRESENTATION_UNKNOWN, mConnectionUT.getNumberPresentation());
        assertEquals(PhoneConstants.PRESENTATION_UNKNOWN, mConnectionUT.getCnapNamePresentation());
        assertEquals(Connection.PostDialState.NOT_STARTED, mConnectionUT.getPostDialState());
        assertEquals(0, mConnectionUT.getDisconnectTime());
        assertEquals(0, mConnectionUT.getHoldDurationMillis());
        assertNull(mConnectionUT.getOrigDialString());
        assertFalse(mConnectionUT.isMultiparty());
        assertFalse(mConnectionUT.isConferenceHost());
        verify(mForeGroundCall, times(1)).attach((Connection) any(),
                eq(ImsPhoneCall.State.INCOMING));

        logd("Testing initial state of MO ImsPhoneConnection");
        mConnectionUT = new ImsPhoneConnection(mImsPhone, String.format("+1 (700).555-41NN%c1234",
                PhoneNumberUtils.PAUSE), mImsCT, mForeGroundCall, false);
        assertEquals(PhoneConstants.PRESENTATION_ALLOWED, mConnectionUT.getNumberPresentation());
        assertEquals(PhoneConstants.PRESENTATION_ALLOWED, mConnectionUT.getCnapNamePresentation());
        assertEquals("+1 (700).555-41NN,1234", mConnectionUT.getOrigDialString());
        verify(mForeGroundCall, times(1)).attachFake((Connection) any(),
                eq(ImsPhoneCall.State.DIALING));
    }

    @Test
    @SmallTest
    public void testImsUpdateStateForeGround() {
        // MO Foreground Connection dailing -> active
        mConnectionUT = new ImsPhoneConnection(mImsPhone, "+1 (700).555-41NN1234", mImsCT,
                mForeGroundCall, false);
        // initially in dialing state
        doReturn(Call.State.DIALING).when(mForeGroundCall).getState();
        assertTrue(mConnectionUT.update(mImsCall, Call.State.ACTIVE));
        // for Ringing/Dialing upadte postDialState
        assertEquals(Connection.PostDialState.COMPLETE, mConnectionUT.getPostDialState());
        verify(mForeGroundCall, times(1)).update(eq(mConnectionUT), eq(mImsCall),
                eq(Call.State.ACTIVE));
    }

    @Test
    @SmallTest
    public void testImsUpdateStateBackGround() {
        // MT background Connection dialing -> active
        mConnectionUT = new ImsPhoneConnection(mImsPhone, mImsCall, mImsCT, mBackGroundCall, false);
        doReturn(Call.State.HOLDING).when(mBackGroundCall).getState();
        assertFalse(mConnectionUT.update(mImsCall, Call.State.ACTIVE));
        verify(mBackGroundCall, times(1)).detach(eq(mConnectionUT));
        verify(mForeGroundCall, times(1)).attach(eq(mConnectionUT));
        verify(mForeGroundCall, times(1)).update(eq(mConnectionUT), eq(mImsCall),
                eq(Call.State.ACTIVE));
        assertEquals(Connection.PostDialState.NOT_STARTED, mConnectionUT.getPostDialState());
    }

    @Test
    @SmallTest
    public void testImsUpdateStatePendingHold() {
        mConnectionUT = new ImsPhoneConnection(mImsPhone, "+1 (700).555-41NN1234", mImsCT,
                mForeGroundCall, false);
        doReturn(true).when(mImsCall).isPendingHold();
        assertFalse(mConnectionUT.update(mImsCall, Call.State.ACTIVE));
        verify(mForeGroundCall, times(0)).update(eq(mConnectionUT), eq(mImsCall),
                eq(Call.State.ACTIVE));
        assertEquals(Connection.PostDialState.NOT_STARTED, mConnectionUT.getPostDialState());
    }

    @Test
    @SmallTest
    public void testUpdateAddressDisplay() {
        mConnectionUT = new ImsPhoneConnection(mImsPhone, mImsCall, mImsCT, mForeGroundCall, false);
        assertEquals(PhoneConstants.PRESENTATION_UNKNOWN, mConnectionUT.getNumberPresentation());
        assertEquals(PhoneConstants.PRESENTATION_UNKNOWN, mConnectionUT.getCnapNamePresentation());
        mImsCallProfile.setCallExtraInt(ImsCallProfile.EXTRA_CNAP,
                ImsCallProfile.OIR_PRESENTATION_PAYPHONE);
        mImsCallProfile.setCallExtraInt(ImsCallProfile.EXTRA_OIR,
                ImsCallProfile.OIR_PRESENTATION_NOT_RESTRICTED);

        mConnectionUT.updateAddressDisplay(mImsCall);
        assertEquals(ImsCallProfile.OIRToPresentation(ImsCallProfile.OIR_PRESENTATION_PAYPHONE),
                mConnectionUT.getCnapNamePresentation());
        assertEquals(ImsCallProfile.OIRToPresentation(
                        ImsCallProfile.OIR_PRESENTATION_NOT_RESTRICTED),
                mConnectionUT.getNumberPresentation());
    }

    @Test
    @SmallTest
    public void testConnectionDisconnect() {
        //Mock we have an active connection
        testImsUpdateStateForeGround();
        waitForMs(50);
        mConnectionUT.onDisconnect(DisconnectCause.LOCAL);
        assertEquals(DisconnectCause.LOCAL, mConnectionUT.getDisconnectCause());
        assertEquals(GsmCdmaCall.State.DISCONNECTED, mConnectionUT.getState());
        assertTrue(mConnectionUT.getDisconnectTime() <= System.currentTimeMillis());
        assertTrue(mConnectionUT.getDurationMillis() >= 50);
    }

    @Test
    @SmallTest
    public void testPostDialWait() {
        mConnectionUT = new ImsPhoneConnection(mImsPhone, String.format("+1 (700).555-41NN%c1234",
                PhoneNumberUtils.WAIT), mImsCT, mForeGroundCall, false);
        doReturn(Call.State.DIALING).when(mForeGroundCall).getState();
        doAnswer(new Answer() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                Message msg = (Message) invocation.getArguments()[1];
                AsyncResult.forMessage(msg);
                msg.sendToTarget();
                return  null;
            }
        }).when(mImsCT).sendDtmf(anyChar(), (Message) any());
        // process post dial string during update
        assertTrue(mConnectionUT.update(mImsCall, Call.State.ACTIVE));
        assertEquals(Connection.PostDialState.WAIT, mConnectionUT.getPostDialState());
        mConnectionUT.proceedAfterWaitChar();
        waitForMs(50);
        assertEquals(Connection.PostDialState.COMPLETE, mConnectionUT.getPostDialState());
    }

    @Test
    @MediumTest
    public void testPostDialPause() {
        mConnectionUT = new ImsPhoneConnection(mImsPhone, String.format("+1 (700).555-41NN%c1234",
                PhoneNumberUtils.PAUSE), mImsCT, mForeGroundCall, false);
        doReturn(Call.State.DIALING).when(mForeGroundCall).getState();
        doAnswer(new Answer() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                Message msg = (Message) invocation.getArguments()[1];
                AsyncResult.forMessage(msg);
                msg.sendToTarget();
                return null;
            }
        }).when(mImsCT).sendDtmf(anyChar(), (Message) any());

        // process post dial string during update
        assertTrue(mConnectionUT.update(mImsCall, Call.State.ACTIVE));
        assertEquals(Connection.PostDialState.STARTED, mConnectionUT.getPostDialState());
        try {
            Field field = ImsPhoneConnection.class.getDeclaredField("PAUSE_DELAY_MILLIS");
            field.setAccessible(true);
            waitForMs((Integer) field.get(null) + 50);
        } catch (Exception ex) {
            Assert.fail("unexpected exception thrown" + ex.getMessage());
        }
        assertEquals(Connection.PostDialState.COMPLETE, mConnectionUT.getPostDialState());
    }

    @Test
    @SmallTest
    public void testSetWifi() {
        mConnectionUT = new ImsPhoneConnection(mImsPhone, mImsCall, mImsCT, mForeGroundCall, false);
        assertFalse(mConnectionUT.isWifi());
        // ImsCall.getRadioTechnology is tested elsewhere
        doReturn(ServiceState.RIL_RADIO_TECHNOLOGY_IWLAN).when(mImsCall).getRadioTechnology();
        mBundle.putString(ImsCallProfile.EXTRA_CALL_RAT_TYPE,
                ServiceState.RIL_RADIO_TECHNOLOGY_IWLAN + "");
        assertTrue(mConnectionUT.update(mImsCall, Call.State.ACTIVE));
        assertTrue(mConnectionUT.isWifi());
    }

    @Test
    @SmallTest
    public void testSetWifi2() {
        mConnectionUT = new ImsPhoneConnection(mImsPhone, mImsCall, mImsCT, mForeGroundCall, false);
        assertFalse(mConnectionUT.isWifi());
        // ImsCall.getRadioTechnology is tested elsewhere
        doReturn(ServiceState.RIL_RADIO_TECHNOLOGY_IWLAN).when(mImsCall).getRadioTechnology();
        // Tests to make sure that the EXTRA_CALL_RAT_TYPE_ALT string is set correctly for newer
        // devices.
        mBundle.putString(ImsCallProfile.EXTRA_CALL_RAT_TYPE_ALT,
                ServiceState.RIL_RADIO_TECHNOLOGY_IWLAN + "");
        assertTrue(mConnectionUT.update(mImsCall, Call.State.ACTIVE));
        assertTrue(mConnectionUT.isWifi());
    }

    @Test
    @SmallTest
    public void testSetLTE() {
        mConnectionUT = new ImsPhoneConnection(mImsPhone, mImsCall, mImsCT, mForeGroundCall, false);
        assertNotEquals(mConnectionUT.getCallRadioTech(), ServiceState.RIL_RADIO_TECHNOLOGY_LTE);
        // ImsCall.getRadioTechnology is tested elsewhere
        doReturn(ServiceState.RIL_RADIO_TECHNOLOGY_LTE).when(mImsCall).getRadioTechnology();
        mBundle.putString(ImsCallProfile.EXTRA_CALL_RAT_TYPE,
                ServiceState.RIL_RADIO_TECHNOLOGY_LTE + "");
        assertTrue(mConnectionUT.update(mImsCall, Call.State.ACTIVE));
        assertEquals(mConnectionUT.getCallRadioTech(), ServiceState.RIL_RADIO_TECHNOLOGY_LTE);
    }

    @Test
    @SmallTest
    public void testSetLTE2() {
        mConnectionUT = new ImsPhoneConnection(mImsPhone, mImsCall, mImsCT, mForeGroundCall, false);
        assertNotEquals(mConnectionUT.getCallRadioTech(), ServiceState.RIL_RADIO_TECHNOLOGY_LTE);
        // ImsCall.getRadioTechnology is tested elsewhere
        doReturn(ServiceState.RIL_RADIO_TECHNOLOGY_LTE).when(mImsCall).getRadioTechnology();
        // Tests to make sure that the EXTRA_CALL_RAT_TYPE_ALT string is set correctly for newer
        // devices.
        mBundle.putString(ImsCallProfile.EXTRA_CALL_RAT_TYPE_ALT,
                ServiceState.RIL_RADIO_TECHNOLOGY_LTE + "");
        assertTrue(mConnectionUT.update(mImsCall, Call.State.ACTIVE));
        assertEquals(mConnectionUT.getCallRadioTech(), ServiceState.RIL_RADIO_TECHNOLOGY_LTE);
    }

    /**
     * Test updates to address for incoming calls.
     */
    @Test
    @SmallTest
    public void testAddressUpdate() {
        String[] testAddressMappingSet[] = {
                /* {"inputAddress", "updateAddress", "ExpectResult"} */
                {"12345", "12345", "12345"},
                {"12345", "67890", "67890"},
                {"12345*00000", "12345", "12345*00000"},
                {"12345*00000", "67890", "67890"},
                {"12345*00000", "12345*00000", "12345*00000"},
                {"12345;11111*00000", "12345", "12345"},
                {"12345*00000;11111", "12345", "12345*00000"},
                {"18412345*00000", "18412345", "18412345*00000"},
                {"+8112345*00000", "+8112345", "+8112345*00000"},
                {"12345*00000", "12346", "12346"}};
        for (String[] testAddress : testAddressMappingSet) {
            mConnectionUT = new ImsPhoneConnection(mImsPhone, testAddress[0], mImsCT,
                    mForeGroundCall, false);
            mConnectionUT.setIsIncoming(true);
            mImsCallProfile.setCallExtra(ImsCallProfile.EXTRA_OI, testAddress[1]);
            mConnectionUT.updateAddressDisplay(mImsCall);
            assertEquals(testAddress[2], mConnectionUT.getAddress());
        }
    }

    /**
     * Ensure updates to the address for outgoing calls are ignored.
     */
    @Test
    @SmallTest
    public void testSetAddressOnOutgoing() {
        String inputAddress = "12345";
        String updateAddress = "6789";

        mConnectionUT = new ImsPhoneConnection(mImsPhone, inputAddress, mImsCT, mForeGroundCall,
                false);
        mConnectionUT.setIsIncoming(false);
        mImsCallProfile.setCallExtra(ImsCallProfile.EXTRA_OI, updateAddress);
        mConnectionUT.updateAddressDisplay(mImsCall);
        assertEquals(inputAddress, mConnectionUT.getAddress());
    }
}
