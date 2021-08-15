/*
 * Copyright 2018 The Android Open Source Project
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
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.Matchers.any;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.content.Context;
import android.content.Intent;
import android.net.wifi.WifiManager;
import android.os.AsyncResult;
import android.os.HandlerThread;
import android.os.Message;
import android.telephony.CellIdentityGsm;
import android.telephony.CellInfo;
import android.telephony.CellInfoGsm;
import android.telephony.ServiceState;
import android.telephony.TelephonyManager;
import android.test.suitebuilder.annotation.SmallTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class LocaleTrackerTest extends TelephonyTest {

    private static final String US_MCC = "310";
    private static final String LIECHTENSTEIN_MCC = "295";

    private static final String FAKE_MNC = "123";

    private static final String COUNTRY_CODE_UNAVAILABLE = "";
    private static final String US_COUNTRY_CODE = "us";
    private static final String LIECHTENSTEIN_COUNTRY_CODE = "li";

    private LocaleTracker mLocaleTracker;

    private CellInfoGsm mCellInfo;
    private WifiManager mWifiManager;

    private class LocaleTrackerTestHandler extends HandlerThread {

        private LocaleTrackerTestHandler(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            mLocaleTracker = new LocaleTracker(mPhone, mNitzStateMachine, this.getLooper());
            setReady(true);
        }
    }

    private LocaleTrackerTestHandler mHandlerThread;

    @Before
    public void setUp() throws Exception {
        logd("LocaleTrackerTest +Setup!");
        super.setUp(getClass().getSimpleName());

        mHandlerThread = new LocaleTrackerTestHandler("LocaleTrackerTestHandler");
        mHandlerThread.start();
        waitUntilReady();


        // This is a workaround to bypass setting system properties, which causes access violation.
        doReturn(-1).when(mPhone).getPhoneId();
        mWifiManager = (WifiManager) mContext.getSystemService(Context.WIFI_SERVICE);

        mCellInfo = new CellInfoGsm();
        mCellInfo.setCellIdentity(new CellIdentityGsm(
                    CellInfo.UNAVAILABLE, CellInfo.UNAVAILABLE,
                    CellInfo.UNAVAILABLE, CellInfo.UNAVAILABLE,
                    US_MCC, FAKE_MNC, null, null));
        doAnswer(invocation -> {
            Message m = invocation.getArgument(1);
            AsyncResult.forMessage(m, Arrays.asList(mCellInfo), null);
            m.sendToTarget();
            return null; }).when(mPhone).requestCellInfoUpdate(any(), any());

        logd("LocaleTrackerTest -Setup!");
    }

    @After
    public void tearDown() throws Exception {
        mHandlerThread.quit();
        mHandlerThread.join();
        super.tearDown();
    }

    private void sendServiceState(int state) {
        ServiceState ss = new ServiceState();
        ss.setState(state);
        AsyncResult ar = new AsyncResult(null, ss, null);
        mLocaleTracker.sendMessage(
                mLocaleTracker.obtainMessage(2 /*SERVICE_STATE_CHANGED*/, ar));
        waitForHandlerAction(mLocaleTracker, 100);
    }

    private void sendGsmCellInfo() {
        // send an unsol cell info
        mLocaleTracker
                .obtainMessage(4 /*UNSOL_CELL_INFO*/,
                        new AsyncResult(null, Arrays.asList(mCellInfo), null))
                .sendToTarget();
        waitForHandlerAction(mLocaleTracker, 100);
    }

    private void verifyCountryCodeNotified(String[] countryCodes) {
        ArgumentCaptor<String> stringArgumentCaptor = ArgumentCaptor.forClass(String.class);
        verify(mWifiManager, times(countryCodes.length)).setCountryCode(
                stringArgumentCaptor.capture());
        List<String> strs = stringArgumentCaptor.getAllValues();

        ArgumentCaptor<Intent> intentArgumentCaptor = ArgumentCaptor.forClass(Intent.class);
        verify(mContext, times(countryCodes.length)).sendBroadcast(intentArgumentCaptor.capture());
        List<Intent> intents = intentArgumentCaptor.getAllValues();

        for (int i = 0; i < countryCodes.length; i++) {
            assertEquals(countryCodes[i], strs.get(i));
            assertEquals(TelephonyManager.ACTION_NETWORK_COUNTRY_CHANGED,
                    intents.get(i).getAction());
            assertEquals(countryCodes[i], intents.get(i).getStringExtra(
                    TelephonyManager.EXTRA_NETWORK_COUNTRY));
        }
    }

    @Test
    @SmallTest
    public void testUpdateOperatorNumericSync() throws Exception {
        mLocaleTracker.updateOperatorNumeric(US_MCC + FAKE_MNC);
        assertEquals(US_COUNTRY_CODE, mLocaleTracker.getCurrentCountry());
        verifyCountryCodeNotified(new String[]{US_COUNTRY_CODE});
    }

    @Test
    @SmallTest
    public void testNoSim() throws Exception {
        // updateOperatorNumeric("") will not trigger an instantaneous country change
        mLocaleTracker.updateOperatorNumeric("");
        sendGsmCellInfo();
        sendServiceState(ServiceState.STATE_EMERGENCY_ONLY);
        assertEquals(US_COUNTRY_CODE, mLocaleTracker.getCurrentCountry());
        verifyCountryCodeNotified(new String[]{US_COUNTRY_CODE});
        assertTrue(mLocaleTracker.isTracking());
    }

    @Test
    @SmallTest
    public void testBootupInAirplaneModeOn() throws Exception {
        mLocaleTracker.updateOperatorNumeric("");
        sendServiceState(ServiceState.STATE_POWER_OFF);
        assertEquals(COUNTRY_CODE_UNAVAILABLE, mLocaleTracker.getCurrentCountry());
        verifyCountryCodeNotified(new String[]{COUNTRY_CODE_UNAVAILABLE});
        assertFalse(mLocaleTracker.isTracking());
    }

    @Test
    @SmallTest
    public void testToggleAirplaneModeOn() throws Exception {
        sendServiceState(ServiceState.STATE_IN_SERVICE);
        mLocaleTracker.updateOperatorNumeric(US_MCC + FAKE_MNC);
        assertEquals(US_COUNTRY_CODE, mLocaleTracker.getCurrentCountry());
        verifyCountryCodeNotified(new String[]{COUNTRY_CODE_UNAVAILABLE, US_COUNTRY_CODE});
        assertFalse(mLocaleTracker.isTracking());

        // updateOperatorNumeric("") will not trigger an instantaneous country change
        mLocaleTracker.updateOperatorNumeric("");
        waitForHandlerAction(mLocaleTracker, 100);
        assertEquals(US_COUNTRY_CODE, mLocaleTracker.getCurrentCountry());
        verifyCountryCodeNotified(new String[]{COUNTRY_CODE_UNAVAILABLE, US_COUNTRY_CODE});
        sendServiceState(ServiceState.STATE_POWER_OFF);
        assertFalse(mLocaleTracker.isTracking());
    }

    @Test
    @SmallTest
    public void testToggleAirplaneModeOff() throws Exception {
        sendServiceState(ServiceState.STATE_POWER_OFF);
        mLocaleTracker.updateOperatorNumeric("");
        waitForHandlerAction(mLocaleTracker, 100);
        assertEquals(COUNTRY_CODE_UNAVAILABLE, mLocaleTracker.getCurrentCountry());
        verifyCountryCodeNotified(new String[]{COUNTRY_CODE_UNAVAILABLE});
        assertFalse(mLocaleTracker.isTracking());

        sendServiceState(ServiceState.STATE_OUT_OF_SERVICE);
        waitForHandlerAction(mLocaleTracker, 100);
        assertTrue(mLocaleTracker.isTracking());
        waitForHandlerAction(mLocaleTracker, 100);
        assertEquals(US_COUNTRY_CODE, mLocaleTracker.getCurrentCountry());
    }

    @Test
    @SmallTest
    public void testToggleAirplaneModeOosPlmn() throws Exception {
        sendServiceState(ServiceState.STATE_POWER_OFF);
        mLocaleTracker.updateOperatorNumeric("");
        waitForHandlerAction(mLocaleTracker, 100);
        assertEquals(COUNTRY_CODE_UNAVAILABLE, mLocaleTracker.getCurrentCountry());
        verifyCountryCodeNotified(new String[]{COUNTRY_CODE_UNAVAILABLE});
        assertFalse(mLocaleTracker.isTracking());

        // Override the setUp() function and return an empty list for CellInfo
        doAnswer(invocation -> {
            Message m = invocation.getArgument(1);
            AsyncResult.forMessage(m, Collections.emptyList(), null);
            m.sendToTarget();
            return null; }).when(mPhone).requestCellInfoUpdate(any(), any());

        sendServiceState(ServiceState.STATE_OUT_OF_SERVICE);
        waitForHandlerAction(mLocaleTracker, 100);
        assertTrue(mLocaleTracker.isTracking());
        waitForHandlerAction(mLocaleTracker, 100);
        assertEquals(COUNTRY_CODE_UNAVAILABLE, mLocaleTracker.getCurrentCountry());

        mLocaleTracker.updateOperatorNumeric(US_MCC + FAKE_MNC);
        assertEquals(US_COUNTRY_CODE, mLocaleTracker.getCurrentCountry());
        verifyCountryCodeNotified(new String[]{COUNTRY_CODE_UNAVAILABLE, US_COUNTRY_CODE});

        mLocaleTracker.updateOperatorNumeric("");
        waitForHandlerAction(mLocaleTracker, 100);
        assertEquals(US_COUNTRY_CODE, mLocaleTracker.getCurrentCountry());
        verifyCountryCodeNotified(new String[]{COUNTRY_CODE_UNAVAILABLE, US_COUNTRY_CODE});

        mLocaleTracker.updateOperatorNumeric(LIECHTENSTEIN_MCC + FAKE_MNC);
        waitForHandlerAction(mLocaleTracker, 100);
        assertEquals(LIECHTENSTEIN_COUNTRY_CODE, mLocaleTracker.getCurrentCountry());
        verifyCountryCodeNotified(new String[]{
                COUNTRY_CODE_UNAVAILABLE, US_COUNTRY_CODE, LIECHTENSTEIN_COUNTRY_CODE});
    }

    @Test
    @SmallTest
    public void testToggleAirplaneModeNoCellInfo() throws Exception {
        sendServiceState(ServiceState.STATE_POWER_OFF);
        mLocaleTracker.updateOperatorNumeric("");
        waitForHandlerAction(mLocaleTracker, 100);
        assertEquals(COUNTRY_CODE_UNAVAILABLE, mLocaleTracker.getCurrentCountry());
        verifyCountryCodeNotified(new String[]{COUNTRY_CODE_UNAVAILABLE});
        assertFalse(mLocaleTracker.isTracking());

        // Override the setUp() function and return an empty list for CellInfo
        doAnswer(invocation -> {
            Message m = invocation.getArgument(1);
            AsyncResult.forMessage(m, Collections.emptyList(), null);
            m.sendToTarget();
            return null; }).when(mPhone).requestCellInfoUpdate(any(), any());

        sendServiceState(ServiceState.STATE_OUT_OF_SERVICE);
        waitForHandlerAction(mLocaleTracker, 100);
        assertTrue(mLocaleTracker.isTracking());
        waitForHandlerAction(mLocaleTracker, 100);
        assertEquals(COUNTRY_CODE_UNAVAILABLE, mLocaleTracker.getCurrentCountry());
    }


    @Test
    @SmallTest
    public void testGetCellInfoDelayTime() throws Exception {
        assertEquals(2000, LocaleTracker.getCellInfoDelayTime(0));
        assertEquals(2000, LocaleTracker.getCellInfoDelayTime(1));
        assertEquals(4000, LocaleTracker.getCellInfoDelayTime(2));
        assertEquals(8000, LocaleTracker.getCellInfoDelayTime(3));
        assertEquals(16000, LocaleTracker.getCellInfoDelayTime(4));
        assertEquals(32000, LocaleTracker.getCellInfoDelayTime(5));
        assertEquals(64000, LocaleTracker.getCellInfoDelayTime(6));
        assertEquals(128000, LocaleTracker.getCellInfoDelayTime(7));
        assertEquals(256000, LocaleTracker.getCellInfoDelayTime(8));
        assertEquals(512000, LocaleTracker.getCellInfoDelayTime(9));

        for (int i = 10; i <= 2000; i++) {
            assertEquals(600000, LocaleTracker.getCellInfoDelayTime(i));
        }
    }
}
