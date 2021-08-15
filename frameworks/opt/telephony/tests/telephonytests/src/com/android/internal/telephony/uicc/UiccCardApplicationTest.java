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
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.verify;

import android.os.AsyncResult;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.telephony.CommandException;
import com.android.internal.telephony.TelephonyTest;
import com.android.internal.telephony.test.SimulatedCommands;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;

public class UiccCardApplicationTest extends TelephonyTest {
    private UiccCardApplication mUiccCardApplication;
    @Mock
    private IccCardApplicationStatus mUiccCardAppStatus;
    @Mock
    private UiccProfile mUiccProfile;
    private Handler mHandler;
    private UiccCardAppTestHandlerThread mTestHandlerThread;
    private int mAttemptsRemaining = -1;
    private CommandException mException = null;
    private static final int UICCCARDAPP_UPDATE_EVENT = 1;
    private static final int UICCCARDAPP_ENABLE_FDN_EVENT = 2;
    private static final int UICCCARDAPP_ENABLE_LOCK_EVENT = 3;
    private static final int UICCCARDAPP_CHANGE_PSW_EVENT = 4;
    private static final int UICCCARDAPP_SUPPLY_PIN_EVENT = 5;
    private class UiccCardAppTestHandlerThread extends HandlerThread {

        private UiccCardAppTestHandlerThread(String name) {
            super(name);
        }
        @Override
        public void onLooperPrepared() {
            mUiccCardApplication = new UiccCardApplication(mUiccProfile, mUiccCardAppStatus,
                    mContext, mSimulatedCommands);
            mHandler = new Handler(mTestHandlerThread.getLooper()) {
                @Override
                public void handleMessage(Message msg) {
                    switch (msg.what) {
                        case UICCCARDAPP_UPDATE_EVENT:
                            logd("Update UiccCardApplication Status");
                            mUiccCardApplication.update(mUiccCardAppStatus,
                                    mContext,
                                    mSimulatedCommands);
                            setReady(true);
                            break;
                        case UICCCARDAPP_SUPPLY_PIN_EVENT:
                        case UICCCARDAPP_CHANGE_PSW_EVENT:
                        case UICCCARDAPP_ENABLE_LOCK_EVENT:
                        case UICCCARDAPP_ENABLE_FDN_EVENT:
                            mAttemptsRemaining = msg.arg1;
                            mException = (CommandException) ((AsyncResult) msg.obj).exception;
                            if (mAttemptsRemaining != -1) {
                                logd("remaining Attempt:" + mAttemptsRemaining);
                            }
                            setReady(true);
                            break;
                        default:
                            logd("Unknown Event " + msg.what);
                    }
                }
            };
            setReady(true);
        }
    }

    @Before
    public void setUp() throws Exception {
        super.setUp(this.getClass().getSimpleName());
        //set initial state of app status
        mUiccCardAppStatus.app_type = IccCardApplicationStatus.AppType.APPTYPE_SIM;
        mUiccCardAppStatus.aid = TAG;
        mUiccCardAppStatus.app_state = IccCardApplicationStatus.AppState.APPSTATE_PIN;
        mUiccCardAppStatus.pin1 = IccCardStatus.PinState.PINSTATE_ENABLED_NOT_VERIFIED;
        mUiccCardAppStatus.pin2 = IccCardStatus.PinState.PINSTATE_ENABLED_VERIFIED;

        mTestHandlerThread = new UiccCardAppTestHandlerThread(TAG);
        mTestHandlerThread.start();

        waitUntilReady();
    }

    @After
    public void tearDown() throws Exception {
        mTestHandlerThread.quit();
        super.tearDown();
    }

    @Test
    @SmallTest
    public void testGetSetAppType() {
        assertEquals(IccCardApplicationStatus.AppType.APPTYPE_SIM, mUiccCardApplication.getType());
        mUiccCardAppStatus.app_type = IccCardApplicationStatus.AppType.APPTYPE_USIM;
        Message mCardAppUpdate = mHandler.obtainMessage(UICCCARDAPP_UPDATE_EVENT);
        setReady(false);
        mCardAppUpdate.sendToTarget();

        waitUntilReady();
        assertEquals(IccCardApplicationStatus.AppType.APPTYPE_USIM, mUiccCardApplication.getType());
    }

    @Test
    @SmallTest
    public void testGetSetAppState() {
        assertEquals(IccCardApplicationStatus.AppState.APPSTATE_PIN,
                mUiccCardApplication.getState());
        mUiccCardAppStatus.app_state = IccCardApplicationStatus.AppState.APPSTATE_PUK;
        Message mCardAppUpdate = mHandler.obtainMessage(UICCCARDAPP_UPDATE_EVENT);
        setReady(false);
        mCardAppUpdate.sendToTarget();

        waitUntilReady();
        assertEquals(IccCardApplicationStatus.AppState.APPSTATE_PUK,
                mUiccCardApplication.getState());
    }

    @Test
    @SmallTest
    public void testGetSetIccFdnEnabled() {
        assertFalse(mUiccCardApplication.getIccFdnEnabled());
        //enable FDN
        Message mFDNenabled = mHandler.obtainMessage(UICCCARDAPP_ENABLE_FDN_EVENT);
        //wrong PIN2Code
        setReady(false);
        mUiccCardApplication.setIccFdnEnabled(true, "XXXX", mFDNenabled);
        waitUntilReady();
        assertFalse(mUiccCardApplication.getIccFdnEnabled());

        setReady(false);
        mFDNenabled = mHandler.obtainMessage(UICCCARDAPP_ENABLE_FDN_EVENT);
        mUiccCardApplication.setIccFdnEnabled(true, mSimulatedCommands.DEFAULT_SIM_PIN2_CODE,
                mFDNenabled);
        waitUntilReady();
        assertTrue(mUiccCardApplication.getIccFdnEnabled());
    }

    @Test
    @SmallTest
    public void testGetSetIccLockedEnabled() {
        assertFalse(mUiccCardApplication.getIccLockEnabled());
        Message mLockEnabled = mHandler.obtainMessage(UICCCARDAPP_ENABLE_LOCK_EVENT);
        setReady(false);
        mUiccCardApplication.setIccLockEnabled(true, "XXXX", mLockEnabled);
        waitUntilReady();
        assertFalse(mUiccCardApplication.getIccLockEnabled());

        mLockEnabled = mHandler.obtainMessage(UICCCARDAPP_ENABLE_LOCK_EVENT);
        setReady(false);
        mUiccCardApplication.setIccLockEnabled(true, mSimulatedCommands.DEFAULT_SIM_PIN_CODE,
                mLockEnabled);
        waitUntilReady();
        assertTrue(mUiccCardApplication.getIccLockEnabled());
    }

    @Test
    @SmallTest
    public void testChangeIccLockPassword() {
        Message mChangePsw = mHandler.obtainMessage(UICCCARDAPP_CHANGE_PSW_EVENT);
        setReady(false);
        mUiccCardApplication.changeIccLockPassword(mSimulatedCommands.DEFAULT_SIM_PIN_CODE,
                "1111", mChangePsw);
        waitUntilReady();
        verify(mSimulatedCommandsVerifier).changeIccPinForApp(
                eq(mSimulatedCommands.DEFAULT_SIM_PIN_CODE), eq("1111"), eq(TAG), (Message) any());
        assertNull(mException);
    }

    @Test
    @SmallTest
    public void testSupplyPin() {
        //Supply with default PIN1
        Message mSupplyPin = mHandler.obtainMessage(UICCCARDAPP_SUPPLY_PIN_EVENT);
        setReady(false);
        mUiccCardApplication.supplyPin(mSimulatedCommands.DEFAULT_SIM_PIN_CODE, mSupplyPin);
        waitUntilReady();
        assertEquals(-1, mAttemptsRemaining);
        verify(mSimulatedCommandsVerifier).supplyIccPinForApp(
                eq(SimulatedCommands.DEFAULT_SIM_PIN_CODE), eq(TAG), (Message) any());

        //Supply with wrong PIN1
        mSupplyPin = mHandler.obtainMessage(UICCCARDAPP_SUPPLY_PIN_EVENT);
        setReady(false);
        mUiccCardApplication.supplyPin("1111", mSupplyPin);
        waitUntilReady();
        assertEquals(mSimulatedCommands.DEFAULT_PIN1_ATTEMPT - 1, mAttemptsRemaining);
        assertNotNull(mException);
        assertEquals(CommandException.Error.PASSWORD_INCORRECT, mException.getCommandError());

        testChangeIccLockPassword();
        //Supply with the updated PIN1
        mSupplyPin = mHandler.obtainMessage(UICCCARDAPP_SUPPLY_PIN_EVENT);
        setReady(false);
        mUiccCardApplication.supplyPin("1111", mSupplyPin);
        waitUntilReady();
        assertEquals(-1, mAttemptsRemaining);
    }
}
