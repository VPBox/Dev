/*
 * Copyright 2017 The Android Open Source Project
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
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.anyString;
import static org.mockito.Mockito.atLeast;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.isA;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.os.PersistableBundle;
import android.telephony.CarrierConfigManager;

import androidx.test.filters.SmallTest;

import com.android.internal.telephony.IccCardConstants.State;
import com.android.internal.telephony.TelephonyTest;
import com.android.internal.telephony.cat.CatService;
import com.android.internal.telephony.uicc.IccCardApplicationStatus.AppState;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;

import java.util.Map;

public class UiccProfileTest extends TelephonyTest {
    private UiccProfile mUiccProfile;

    public UiccProfileTest() {
        super();
    }

    private IccIoResult mIccIoResult;
    private static final int SCARY_SLEEP_MS = 200;

    private UiccProfileHandlerThread mTestHandlerThread;
    private Handler mHandler;
    private static final int UICCPROFILE_UPDATE_PROFILE_EVENT = 1;
    private static final int UICCPROFILE_UPDATE_APPLICATION_EVENT = 2;
    private static final int UICCPROFILE_CARRIER_PRIVILEDGE_LOADED_EVENT = 3;

    @Mock
    private CatService mCAT;
    @Mock
    private IccCardStatus mIccCardStatus;
    @Mock
    private Handler mMockedHandler;
    @Mock
    private UiccCard mUiccCard;

    private class UiccProfileHandlerThread extends HandlerThread {

        private UiccProfileHandlerThread(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            mUiccProfile = new UiccProfile(mContextFixture.getTestDouble(),
                                           mSimulatedCommands, mIccCardStatus, 0 /* phoneId */,
                                           mUiccCard, new Object());
            /* create a custom handler for the Handler Thread */
            mHandler = new Handler(mTestHandlerThread.getLooper()) {
                @Override
                public void handleMessage(Message msg) {
                    switch (msg.what) {
                        case UICCPROFILE_UPDATE_PROFILE_EVENT:
                            /* Upon handling this event, new CarrierPrivilegeRule
                            will be created with the looper of HandlerThread */
                            logd("Update UICC Profile");
                            mUiccProfile.update(mContextFixture.getTestDouble(),
                                    mSimulatedCommands, mIccCardStatus);
                            setReady(true);
                            break;
                        case UICCPROFILE_UPDATE_APPLICATION_EVENT:
                            logd("Update UICC Profile Applications");
                            mUiccProfile.update(mContextFixture.getTestDouble(),
                                    mSimulatedCommands, mIccCardStatus);
                            setReady(true);
                            break;
                        default:
                            logd("Unknown Event " + msg.what);
                    }
                }
            };
            /* wait for the carrier privilege rules to be loaded */
            waitForMs(50);
            setReady(true);
            logd("Create UiccProfile");
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
         /* initially there are no application available, but the array should not be empty. */
        IccCardApplicationStatus umtsApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_USIM,
                IccCardApplicationStatus.AppState.APPSTATE_UNKNOWN, "0xA2");
        mIccCardStatus.mApplications = new IccCardApplicationStatus[]{umtsApp};
        mIccCardStatus.mCdmaSubscriptionAppIndex =
                mIccCardStatus.mImsSubscriptionAppIndex =
                        mIccCardStatus.mGsmUmtsSubscriptionAppIndex = -1;
        mIccIoResult = new IccIoResult(0x90, 0x00, IccUtils.hexStringToBytes("FF40"));
        mSimulatedCommands.setIccIoResultForApduLogicalChannel(mIccIoResult);
        /* starting the Handler Thread */
        mTestHandlerThread = new UiccProfileHandlerThread(TAG);
        mTestHandlerThread.start();

        waitUntilReady();

        /* wait for the carrier privilege rules to be loaded */
        waitForMs(50);

        replaceInstance(UiccProfile.class, "mCatService", mUiccProfile, mCAT);
    }

    @After
    public void tearDown() throws Exception {
        mTestHandlerThread.quit();
        super.tearDown();
    }

    @Test
    @SmallTest
    public void tesUiccProfileInfoSanity() {
        assertEquals(1, mUiccProfile.getNumApplications());
        assertNull(mUiccProfile.getUniversalPinState());
        assertNull(mUiccProfile.getOperatorBrandOverride());
        for (IccCardApplicationStatus.AppType mAppType :
                IccCardApplicationStatus.AppType.values()) {
            if (mAppType == IccCardApplicationStatus.AppType.APPTYPE_USIM) {
                assertTrue(mUiccProfile.isApplicationOnIcc(mAppType));
            } else {
                assertFalse(mUiccProfile.isApplicationOnIcc(mAppType));
            }

        }
    }

    @Test
    @SmallTest
    public void testParseWhitelistMapFromString() {
        String whitelist = "";
        Map<String, String> parsedMap = UiccProfile.parseToCertificateToPackageMap(whitelist);
        assertTrue(parsedMap.isEmpty());

        whitelist = "nokey;value;separation";
        parsedMap = UiccProfile.parseToCertificateToPackageMap(whitelist);
        assertTrue(parsedMap.isEmpty());

        whitelist = "KEY1:value1";
        parsedMap = UiccProfile.parseToCertificateToPackageMap(whitelist);
        assertEquals("value1", parsedMap.get("KEY1"));

        whitelist = "KEY1:value1;   KEY2:value2  ;KEY3:value3";
        parsedMap = UiccProfile.parseToCertificateToPackageMap(whitelist);
        assertEquals("value1", parsedMap.get("KEY1"));
        assertEquals("value2", parsedMap.get("KEY2"));
        assertEquals("value3", parsedMap.get("KEY3"));
    }

    @Test
    @SmallTest
    public void testUpdateUiccProfileApplication() {
        /* update app status and index */
        IccCardApplicationStatus cdmaApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_CSIM,
                IccCardApplicationStatus.AppState.APPSTATE_UNKNOWN, "0xA0");
        IccCardApplicationStatus imsApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_ISIM,
                IccCardApplicationStatus.AppState.APPSTATE_UNKNOWN, "0xA1");
        IccCardApplicationStatus umtsApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_USIM,
                IccCardApplicationStatus.AppState.APPSTATE_UNKNOWN, "0xA2");
        mIccCardStatus.mApplications = new IccCardApplicationStatus[]{cdmaApp, imsApp, umtsApp};
        mIccCardStatus.mCdmaSubscriptionAppIndex = 0;
        mIccCardStatus.mImsSubscriptionAppIndex = 1;
        mIccCardStatus.mGsmUmtsSubscriptionAppIndex = 2;
        Message mProfileUpdate = mHandler.obtainMessage(UICCPROFILE_UPDATE_APPLICATION_EVENT);
        setReady(false);
        mProfileUpdate.sendToTarget();

        waitUntilReady();

        /* wait for the carrier privilege rules to be loaded */
        waitForMs(50);

        assertEquals(3, mUiccProfile.getNumApplications());
        assertTrue(mUiccProfile.isApplicationOnIcc(IccCardApplicationStatus.AppType.APPTYPE_CSIM));
        assertTrue(mUiccProfile.isApplicationOnIcc(IccCardApplicationStatus.AppType.APPTYPE_ISIM));
        assertTrue(mUiccProfile.isApplicationOnIcc(IccCardApplicationStatus.AppType.APPTYPE_USIM));
    }

    @Test
    @SmallTest
    public void testUpdateUiccProfile() {
        int mChannelId = 1;
        mIccCardStatus.mCardState = IccCardStatus.CardState.CARDSTATE_PRESENT;
        mSimulatedCommands.setOpenChannelId(mChannelId);
        Message mCardUpdate = mHandler.obtainMessage(UICCPROFILE_UPDATE_PROFILE_EVENT);
        setReady(false);
        mCardUpdate.sendToTarget();
        /* try to create a new CarrierPrivilege, loading state -> loaded state */
        /* wait till the async result and message delay */
        waitUntilReady();
        /* wait for the carrier privilege rules to be loaded */
        waitForMs(50);

        assertTrue(mUiccProfile.areCarrierPriviligeRulesLoaded());
        verify(mSimulatedCommandsVerifier, times(2)).iccOpenLogicalChannel(isA(String.class),
                anyInt(), isA(Message.class));
        verify(mSimulatedCommandsVerifier, times(2)).iccTransmitApduLogicalChannel(
                anyInt(), anyInt(), anyInt(), anyInt(), anyInt(), anyInt(), anyString(),
                isA(Message.class)
        );
    }

    @Test
    @SmallTest
    public void testUpdateUiccProfilePinState() {
        mIccCardStatus.mUniversalPinState = IccCardStatus.PinState.PINSTATE_ENABLED_VERIFIED;
        mUiccProfile.update(mContextFixture.getTestDouble(), mSimulatedCommands, mIccCardStatus);
        assertEquals(IccCardStatus.PinState.PINSTATE_ENABLED_VERIFIED,
                mUiccProfile.getUniversalPinState());
    }

    @Test
    @SmallTest
    public void testCarrierPriviledgeLoadedListener() {
        mUiccProfile.registerForCarrierPrivilegeRulesLoaded(mMockedHandler,
                UICCPROFILE_CARRIER_PRIVILEDGE_LOADED_EVENT, null);
        ArgumentCaptor<Message> mCaptorMessage = ArgumentCaptor.forClass(Message.class);
        ArgumentCaptor<Long> mCaptorLong = ArgumentCaptor.forClass(Long.class);
        testUpdateUiccProfile();
        verify(mMockedHandler, atLeast(1)).sendMessageDelayed(mCaptorMessage.capture(),
                mCaptorLong.capture());
        assertEquals(UICCPROFILE_CARRIER_PRIVILEDGE_LOADED_EVENT, mCaptorMessage.getValue().what);
    }

    @Test
    @SmallTest
    public void testInitialCardState() {
        // after updateExternalState() is called, the state will not be UNKNOWN
        assertEquals(mUiccProfile.getState(), State.NOT_READY);
    }

    @Test
    @SmallTest
    public void testUpdateUiccProfileApplicationNotReady() {
        /* update app status and index */
        IccCardApplicationStatus cdmaApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_CSIM,
                IccCardApplicationStatus.AppState.APPSTATE_READY, "0xA0");
        IccCardApplicationStatus imsApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_ISIM,
                IccCardApplicationStatus.AppState.APPSTATE_READY, "0xA1");
        IccCardApplicationStatus umtsApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_USIM,
                IccCardApplicationStatus.AppState.APPSTATE_UNKNOWN, "0xA2");
        mIccCardStatus.mApplications = new IccCardApplicationStatus[]{cdmaApp, imsApp, umtsApp};
        mIccCardStatus.mCdmaSubscriptionAppIndex = 0;
        mIccCardStatus.mImsSubscriptionAppIndex = 1;
        mIccCardStatus.mGsmUmtsSubscriptionAppIndex = 2;
        Message mProfileUpdate = mHandler.obtainMessage(UICCPROFILE_UPDATE_APPLICATION_EVENT);
        setReady(false);
        mProfileUpdate.sendToTarget();

        waitUntilReady();

        /* wait for the carrier privilege rules to be loaded */
        waitForMs(50);
        assertEquals(3, mUiccProfile.getNumApplications());

        mUiccProfile.mHandler.sendMessage(
                mUiccProfile.mHandler.obtainMessage(UiccProfile.EVENT_APP_READY));
        waitForMs(SCARY_SLEEP_MS);
        assertEquals(mUiccProfile.getState(), State.NOT_READY);
    }

    @Test
    @SmallTest
    public void testUpdateUiccProfileApplicationAllReady() {
        /* update app status and index */
        IccCardApplicationStatus cdmaApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_CSIM,
                IccCardApplicationStatus.AppState.APPSTATE_READY, "0xA0");
        IccCardApplicationStatus imsApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_ISIM,
                IccCardApplicationStatus.AppState.APPSTATE_READY, "0xA1");
        IccCardApplicationStatus umtsApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_USIM,
                IccCardApplicationStatus.AppState.APPSTATE_READY, "0xA2");
        mIccCardStatus.mApplications = new IccCardApplicationStatus[]{cdmaApp, imsApp, umtsApp};
        mIccCardStatus.mCdmaSubscriptionAppIndex = 0;
        mIccCardStatus.mImsSubscriptionAppIndex = 1;
        mIccCardStatus.mGsmUmtsSubscriptionAppIndex = 2;
        Message mProfileUpdate = mHandler.obtainMessage(UICCPROFILE_UPDATE_APPLICATION_EVENT);
        setReady(false);
        mProfileUpdate.sendToTarget();

        waitUntilReady();

        /* wait for the carrier privilege rules to be loaded */
        waitForMs(50);
        assertEquals(3, mUiccProfile.getNumApplications());

        mUiccProfile.mHandler.sendMessage(
                mUiccProfile.mHandler.obtainMessage(UiccProfile.EVENT_APP_READY));
        waitForMs(SCARY_SLEEP_MS);
        // state is loaded as all records are loaded right away as SimulatedCommands returns
        // response for them right away. Ideally applications and records should be mocked.
        assertEquals(State.LOADED, mUiccProfile.getState());
    }

    @Test
    @SmallTest
    public void testUpdateUiccProfileApplicationAllSupportedAppsReady() {
        /* update app status and index */
        IccCardApplicationStatus umtsApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_USIM,
                IccCardApplicationStatus.AppState.APPSTATE_READY, "0xA2");
        IccCardApplicationStatus imsApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_ISIM,
                IccCardApplicationStatus.AppState.APPSTATE_READY, "0xA1");
        IccCardApplicationStatus unknownApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_UNKNOWN,
                IccCardApplicationStatus.AppState.APPSTATE_UNKNOWN, "0xA2");
        mIccCardStatus.mApplications = new IccCardApplicationStatus[]{imsApp, umtsApp, unknownApp};
        mIccCardStatus.mCdmaSubscriptionAppIndex = -1;
        mIccCardStatus.mImsSubscriptionAppIndex = 0;
        mIccCardStatus.mGsmUmtsSubscriptionAppIndex = 1;
        Message mProfileUpdate = mHandler.obtainMessage(UICCPROFILE_UPDATE_APPLICATION_EVENT);
        setReady(false);
        mProfileUpdate.sendToTarget();

        waitUntilReady();

        /* wait for the carrier privilege rules to be loaded */
        waitForMs(50);
        assertEquals(3, mUiccProfile.getNumApplications());

        mUiccProfile.mHandler.sendMessage(
                mUiccProfile.mHandler.obtainMessage(UiccProfile.EVENT_APP_READY));
        waitForMs(SCARY_SLEEP_MS);
        // state is loaded as all records are loaded right away as SimulatedCommands returns
        // response for them right away. Ideally applications and records should be mocked.
        assertEquals(State.LOADED, mUiccProfile.getState());
    }

    @Test
    @SmallTest
    public void testUpdateUiccProfileApplicationWithDuplicateApps() {
        /* update app status and index */
        IccCardApplicationStatus umtsApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_USIM,
                IccCardApplicationStatus.AppState.APPSTATE_READY, "0xA2");
        IccCardApplicationStatus imsApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_ISIM,
                IccCardApplicationStatus.AppState.APPSTATE_READY, "0xA1");
        IccCardApplicationStatus unknownApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_UNKNOWN,
                IccCardApplicationStatus.AppState.APPSTATE_UNKNOWN, "0xA2");
        IccCardApplicationStatus umtsAppDup = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_USIM,
                AppState.APPSTATE_DETECTED, "0xA2");
        mIccCardStatus.mApplications = new IccCardApplicationStatus[]{imsApp, umtsApp, unknownApp,
                umtsAppDup};
        mIccCardStatus.mCdmaSubscriptionAppIndex = -1;
        mIccCardStatus.mImsSubscriptionAppIndex = 0;
        mIccCardStatus.mGsmUmtsSubscriptionAppIndex = 1;
        Message mProfileUpdate = mHandler.obtainMessage(UICCPROFILE_UPDATE_APPLICATION_EVENT);
        setReady(false);
        mProfileUpdate.sendToTarget();

        waitUntilReady();

        /* wait for the carrier privilege rules to be loaded */
        waitForMs(50);
        assertEquals(4, mUiccProfile.getNumApplications());

        mUiccProfile.mHandler.sendMessage(
                mUiccProfile.mHandler.obtainMessage(UiccProfile.EVENT_APP_READY));
        waitForMs(SCARY_SLEEP_MS);
        // state is loaded as all records are loaded right away as SimulatedCommands returns
        // response for them right away. Ideally applications and records should be mocked.
        assertEquals(State.LOADED, mUiccProfile.getState());
    }

    @Test
    @SmallTest
    public void testUpdateUiccProfileApplicationWithDuplicateAppsInDifferentOrder() {
        /* update app status and index */
        IccCardApplicationStatus umtsApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_USIM,
                IccCardApplicationStatus.AppState.APPSTATE_READY, "0xA2");
        IccCardApplicationStatus imsApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_ISIM,
                IccCardApplicationStatus.AppState.APPSTATE_READY, "0xA1");
        IccCardApplicationStatus unknownApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_UNKNOWN,
                IccCardApplicationStatus.AppState.APPSTATE_UNKNOWN, "0xA2");
        IccCardApplicationStatus umtsAppDup = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_USIM,
                AppState.APPSTATE_DETECTED, "0xA2");
        mIccCardStatus.mApplications = new IccCardApplicationStatus[]{umtsAppDup, imsApp, umtsApp,
                unknownApp};
        mIccCardStatus.mCdmaSubscriptionAppIndex = -1;
        mIccCardStatus.mImsSubscriptionAppIndex = 0;
        mIccCardStatus.mGsmUmtsSubscriptionAppIndex = 2;
        Message mProfileUpdate = mHandler.obtainMessage(UICCPROFILE_UPDATE_APPLICATION_EVENT);
        setReady(false);
        mProfileUpdate.sendToTarget();

        waitUntilReady();

        /* wait for the carrier privilege rules to be loaded */
        waitForMs(50);
        assertEquals(4, mUiccProfile.getNumApplications());

        mUiccProfile.mHandler.sendMessage(
                mUiccProfile.mHandler.obtainMessage(UiccProfile.EVENT_APP_READY));
        waitForMs(SCARY_SLEEP_MS);
        // state is loaded as all records are loaded right away as SimulatedCommands returns
        // response for them right away. Ideally applications and records should be mocked.
        assertEquals(State.LOADED, mUiccProfile.getState());
    }

    @Test
    @SmallTest
    public void testUpdateUiccProfileApplicationNoApplication() {
        mIccCardStatus.mApplications = new IccCardApplicationStatus[]{};
        mIccCardStatus.mCdmaSubscriptionAppIndex = -1;
        mIccCardStatus.mImsSubscriptionAppIndex = -1;
        mIccCardStatus.mGsmUmtsSubscriptionAppIndex = -1;
        Message mProfileUpdate = mHandler.obtainMessage(UICCPROFILE_UPDATE_APPLICATION_EVENT);
        setReady(false);
        mProfileUpdate.sendToTarget();

        waitUntilReady();

        /* wait for the carrier privilege rules to be loaded */
        waitForMs(50);
        assertEquals(0, mUiccProfile.getNumApplications());

        mUiccProfile.mHandler.sendMessage(
                mUiccProfile.mHandler.obtainMessage(UiccProfile.EVENT_APP_READY));
        waitForMs(SCARY_SLEEP_MS);
        // state is loaded since there is no applications.
        assertEquals(State.NOT_READY, mUiccProfile.getState());
    }

    @Test
    @SmallTest
    public void testUpdateUiccProfileApplicationNoSupportApplication() {
        IccCardApplicationStatus unknownApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_UNKNOWN,
                IccCardApplicationStatus.AppState.APPSTATE_UNKNOWN, "");
        mIccCardStatus.mApplications = new IccCardApplicationStatus[]{unknownApp};
        mIccCardStatus.mCdmaSubscriptionAppIndex = -1;
        mIccCardStatus.mImsSubscriptionAppIndex = -1;
        mIccCardStatus.mGsmUmtsSubscriptionAppIndex = -1;
        Message mProfileUpdate = mHandler.obtainMessage(UICCPROFILE_UPDATE_APPLICATION_EVENT);
        setReady(false);
        mProfileUpdate.sendToTarget();

        waitUntilReady();

        /* wait for the carrier privilege rules to be loaded */
        waitForMs(50);
        assertEquals(1, mUiccProfile.getNumApplications());

        mUiccProfile.mHandler.sendMessage(
                mUiccProfile.mHandler.obtainMessage(UiccProfile.EVENT_APP_READY));
        waitForMs(SCARY_SLEEP_MS);
        // state is loaded since there is no applications.
        assertEquals(State.NOT_READY, mUiccProfile.getState());
    }

    private void testWithCsimApp() {
        /* update app status and index */
        IccCardApplicationStatus umtsApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_USIM,
                AppState.APPSTATE_READY, "0xA2");
        IccCardApplicationStatus imsApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_ISIM,
                AppState.APPSTATE_READY, "0xA1");
        IccCardApplicationStatus cdmaApp = composeUiccApplicationStatus(
                IccCardApplicationStatus.AppType.APPTYPE_CSIM,
                AppState.APPSTATE_DETECTED, "0xA2");
        mIccCardStatus.mApplications = new IccCardApplicationStatus[]{imsApp, umtsApp, cdmaApp};
        mIccCardStatus.mCdmaSubscriptionAppIndex = 2;
        mIccCardStatus.mImsSubscriptionAppIndex = 0;
        mIccCardStatus.mGsmUmtsSubscriptionAppIndex = 1;

        Message mProfileUpdate = mHandler.obtainMessage(UICCPROFILE_UPDATE_APPLICATION_EVENT);
        setReady(false);
        mProfileUpdate.sendToTarget();

        waitUntilReady();

        /* wait for the carrier privilege rules to be loaded */
        waitForMs(50);
        assertEquals(3, mUiccProfile.getNumApplications());

        mUiccProfile.mHandler.sendMessage(
                mUiccProfile.mHandler.obtainMessage(UiccProfile.EVENT_APP_READY));
        waitForMs(SCARY_SLEEP_MS);
    }

    @Test
    @SmallTest
    public void testUpdateUiccProfileApplicationCdmaSupported() {
        // CDMA supported
        doReturn(true)
            .when(mPackageManager).hasSystemFeature(PackageManager.FEATURE_TELEPHONY_CDMA);

        testWithCsimApp();

        // CDMA is supported and CSIM app is not ready, so state should be NOT_READY
        assertEquals(State.NOT_READY, mUiccProfile.getState());
    }

    @Test
    @SmallTest
    public void testUpdateUiccProfileApplicationCdmaNotSupported() {
        // CDMA not supported
        doReturn(false)
            .when(mPackageManager).hasSystemFeature(PackageManager.FEATURE_TELEPHONY_CDMA);

        testWithCsimApp();

        // state is loaded as all records are loaded right away as SimulatedCommands returns
        // response for them right away. Ideally applications and records should be mocked.
        // CSIM is not ready but that should not matter since CDMA is not supported.
        assertEquals(State.LOADED, mUiccProfile.getState());
    }

    @Test
    @SmallTest
    public void testUpdateExternalState() {
        // IO_ERROR
        doReturn(IccCardStatus.CardState.CARDSTATE_ERROR).when(mUiccCard).getCardState();
        mUiccProfile.updateExternalState();
        assertEquals(State.CARD_IO_ERROR, mUiccProfile.getState());

        // RESTRICTED
        doReturn(IccCardStatus.CardState.CARDSTATE_RESTRICTED).when(mUiccCard).getCardState();
        mUiccProfile.updateExternalState();
        assertEquals(State.CARD_RESTRICTED, mUiccProfile.getState());

        // CARD PRESENT; no mUiccApplication - state should be NOT_READY
        doReturn(IccCardStatus.CardState.CARDSTATE_PRESENT).when(mUiccCard).getCardState();
        mUiccProfile.updateExternalState();
        assertEquals(State.NOT_READY, mUiccProfile.getState());

        // set mUiccApplication
        testUpdateUiccProfileApplicationAllReady();
        mUiccProfile.updateExternalState();
        assertEquals(State.LOADED, mUiccProfile.getState());
    }

    @Test
    @SmallTest
    public void testCarrierConfigHandling() {
        testUpdateUiccProfileApplication();

        // Fake carrier name
        String fakeCarrierName = "fakeCarrierName";
        PersistableBundle carrierConfigBundle = mContextFixture.getCarrierConfigBundle();
        carrierConfigBundle.putBoolean(CarrierConfigManager.KEY_CARRIER_NAME_OVERRIDE_BOOL, true);
        carrierConfigBundle.putString(CarrierConfigManager.KEY_CARRIER_NAME_STRING,
                fakeCarrierName);

        // broadcast CARRIER_CONFIG_CHANGED
        mContext.sendBroadcast(new Intent(CarrierConfigManager.ACTION_CARRIER_CONFIG_CHANGED));
        waitForMs(200);

        // verify that setSimOperatorNameForPhone() is called with fakeCarrierName
        ArgumentCaptor<String> stringArgumentCaptor = ArgumentCaptor.forClass(String.class);
        verify(mTelephonyManager, atLeast(1)).setSimOperatorNameForPhone(anyInt(),
                stringArgumentCaptor.capture());
        boolean carrierFound = false;
        for (String carrierName : stringArgumentCaptor.getAllValues()) {
            if (fakeCarrierName.equals(carrierName)) {
                carrierFound = true;
                break;
            }
        }
        assertTrue(carrierFound);
    }

    @Test
    @SmallTest
    public void testIsEmptyProfile() {
        testUpdateUiccProfileApplication();
        assertFalse(mUiccProfile.isEmptyProfile());

        // Manually resetting app shouldn't indicate we are on empty profile.
        mUiccProfile.resetAppWithAid("", true);
        assertFalse(mUiccProfile.isEmptyProfile());

        // If we update there's no application, then we are on empty profile.
        testUpdateUiccProfileApplicationNoApplication();
        assertTrue(mUiccProfile.isEmptyProfile());

    }
}
