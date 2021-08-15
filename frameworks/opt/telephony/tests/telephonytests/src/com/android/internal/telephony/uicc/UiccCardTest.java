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

import static com.android.internal.telephony.TelephonyTestUtils.waitForMs;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNull;

import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.telephony.TelephonyTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;

public class UiccCardTest extends TelephonyTest {
    private UiccCard mUicccard;

    public UiccCardTest() {
        super();
    }

    private IccIoResult mIccIoResult;

    private UiccCardHandlerThread mTestHandlerThread;
    private Handler mHandler;
    private static final int UICCCARD_UPDATE_CARD_STATE_EVENT = 1;
    private static final int UICCCARD_UPDATE_CARD_APPLICATION_EVENT = 2;
    private static final int UICCCARD_CARRIER_PRIVILEDGE_LOADED_EVENT = 3;

    @Mock
    private IccCardStatus mIccCardStatus;
    @Mock
    private Handler mMockedHandler;


    private class UiccCardHandlerThread extends HandlerThread {

        private UiccCardHandlerThread(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            mUicccard = new UiccCard(mContextFixture.getTestDouble(),
                                     mSimulatedCommands, mIccCardStatus, 0 /* phoneId */,
                                     new Object());
            /* create a custom handler for the Handler Thread */
            mHandler = new Handler(mTestHandlerThread.getLooper()) {
                @Override
                public void handleMessage(Message msg) {
                    switch (msg.what) {
                        case UICCCARD_UPDATE_CARD_STATE_EVENT:
                            /* Upon handling this event, new CarrierPrivilegeRule
                            will be created with the looper of HandlerThread */
                            logd("Update UICC Card State");
                            mUicccard.update(mContextFixture.getTestDouble(),
                                    mSimulatedCommands, mIccCardStatus);
                            setReady(true);
                            break;
                        case UICCCARD_UPDATE_CARD_APPLICATION_EVENT:
                            logd("Update UICC Card Applications");
                            mUicccard.update(mContextFixture.getTestDouble(),
                                    mSimulatedCommands, mIccCardStatus);
                            setReady(true);
                            break;
                        default:
                            logd("Unknown Event " + msg.what);
                    }
                }
            };

            setReady(true);
            logd("create UiccCard");
        }
    }

    private IccCardApplicationStatus composeUiccApplicationStatus(
            IccCardApplicationStatus.AppType appType,
            IccCardApplicationStatus.AppState appState, String aid) {
        IccCardApplicationStatus mIccCardAppStatus = new IccCardApplicationStatus();
        mIccCardAppStatus.aid = aid;
        mIccCardAppStatus.app_type = appType;
        mIccCardAppStatus.app_state = appState;
        mIccCardAppStatus.pin1 = mIccCardAppStatus.pin2 =
                IccCardStatus.PinState.PINSTATE_ENABLED_VERIFIED;
        return mIccCardAppStatus;
    }

    @Before
    public void setUp() throws Exception {

        super.setUp(getClass().getSimpleName());
        /* initially there are no application available */
        mIccCardStatus.mApplications = new IccCardApplicationStatus[]{};
        mIccCardStatus.mCdmaSubscriptionAppIndex =
                mIccCardStatus.mImsSubscriptionAppIndex =
                        mIccCardStatus.mGsmUmtsSubscriptionAppIndex = -1;
        mIccCardStatus.mCardState = IccCardStatus.CardState.CARDSTATE_PRESENT;

        mIccIoResult = new IccIoResult(0x90, 0x00, IccUtils.hexStringToBytes("FF40"));
        mSimulatedCommands.setIccIoResultForApduLogicalChannel(mIccIoResult);
        /* starting the Handler Thread */
        mTestHandlerThread = new UiccCardHandlerThread(getClass().getSimpleName());
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
    public void tesUiccCartdInfoSanity() {
        /* before update sanity test */
        assertEquals(0, mUicccard.getNumApplications());
        assertEquals(IccCardStatus.CardState.CARDSTATE_PRESENT, mUicccard.getCardState());
        assertNull(mUicccard.getUniversalPinState());
        assertNull(mUicccard.getOperatorBrandOverride());
        /* UiccProfile mock should return false */
        assertFalse(mUicccard.areCarrierPriviligeRulesLoaded());
        for (IccCardApplicationStatus.AppType mAppType :
                IccCardApplicationStatus.AppType.values()) {
            assertFalse(mUicccard.isApplicationOnIcc(mAppType));
        }
    }

    @Test @SmallTest
    public void testUpdateUiccCardState() {
        int mChannelId = 1;
        /* set card as present */
        mIccCardStatus.mCardState = IccCardStatus.CardState.CARDSTATE_PRESENT;
        /* Mock open Channel ID 1 */
        mSimulatedCommands.setOpenChannelId(mChannelId);
        Message mCardUpdate = mHandler.obtainMessage(UICCCARD_UPDATE_CARD_STATE_EVENT);
        setReady(false);
        mCardUpdate.sendToTarget();
        /* try to create a new CarrierPrivilege, loading state -> loaded state */
        /* wait till the async result and message delay */
        waitUntilReady();

        assertEquals(IccCardStatus.CardState.CARDSTATE_PRESENT, mUicccard.getCardState());

        waitForMs(50);

        /* todo: This part should move to UiccProfileTest
        assertTrue(mUicccard.areCarrierPriviligeRulesLoaded());
        verify(mSimulatedCommandsVerifier, times(2)).iccOpenLogicalChannel(isA(String.class),
                anyInt(), isA(Message.class));
        verify(mSimulatedCommandsVerifier, times(2)).iccTransmitApduLogicalChannel(
                eq(mChannelId), anyInt(), anyInt(), anyInt(), anyInt(), anyInt(), anyString(),
                isA(Message.class)
        );
        */
    }
}
