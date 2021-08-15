/*
 * Copyright (C) 2018 The Android Open Source Project
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

package com.android.internal.telephony.emergency;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.doReturn;

import android.os.AsyncResult;
import android.os.HandlerThread;
import android.telephony.emergency.EmergencyNumber;

import com.android.internal.telephony.Phone;
import com.android.internal.telephony.PhoneFactory;
import com.android.internal.telephony.TelephonyTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Tests for EmergencyNumberTracker.java
 */
public class EmergencyNumberTrackerTest extends TelephonyTest {

    @Mock
    private Phone mPhone2; // mPhone as phone 1 is already defined in TelephonyTest.

    // mEmergencyNumberTrackerMock for mPhone
    private EmergencyNumberTracker mEmergencyNumberTrackerMock;
    // mEmergencyNumberTrackerMock2 for mPhone2
    private EmergencyNumberTracker mEmergencyNumberTrackerMock2;

    private List<EmergencyNumber> mEmergencyNumberListTestSample = new ArrayList<>();
    private EmergencyNumber mUsEmergencyNumber;
    private String[] mEmergencyNumberPrefixTestSample = {"123", "456"};
    private static final long TIMEOUT_MS = 500;

    private class EmergencyNumberTrackerTestHandler extends HandlerThread {
        private EmergencyNumberTrackerTestHandler(String name) {
            super(name);
        }
        @Override
        public void onLooperPrepared() {
            mEmergencyNumberTrackerMock = new EmergencyNumberTracker(mPhone, mSimulatedCommands);
            mEmergencyNumberTrackerMock2 = new EmergencyNumberTracker(mPhone2, mSimulatedCommands);
            doReturn(mEmergencyNumberTrackerMock2).when(mPhone2).getEmergencyNumberTracker();
            mEmergencyNumberTrackerMock.DBG = true;
            setReady(true);
        }
    }

    private EmergencyNumberTrackerTestHandler mHandlerThread;

    @Before
    public void setUp() throws Exception {
        logd("EmergencyNumberTrackerTest +Setup!");
        super.setUp("EmergencyNumberTrackerTest");
        doReturn(mContext).when(mPhone).getContext();
        doReturn(0).when(mPhone).getPhoneId();

        doReturn(mContext).when(mPhone2).getContext();
        doReturn(1).when(mPhone2).getPhoneId();

        initializeEmergencyNumberListTestSamples();
        mHandlerThread = new EmergencyNumberTrackerTestHandler("EmergencyNumberTrackerTestHandler");
        mHandlerThread.start();
        waitUntilReady();
        logd("EmergencyNumberTrackerTest -Setup!");
    }

    @After
    public void tearDown() throws Exception {
        // Set back to single sim mode
        setSinglePhone();

        mHandlerThread.quit();
        mHandlerThread.join();
        super.tearDown();
    }

    private void initializeEmergencyNumberListTestSamples() {
        EmergencyNumber emergencyNumberForTest = new EmergencyNumber("119", "jp", "30",
                EmergencyNumber.EMERGENCY_SERVICE_CATEGORY_FIRE_BRIGADE,
                new ArrayList<String>(),
                EmergencyNumber.EMERGENCY_NUMBER_SOURCE_NETWORK_SIGNALING,
                EmergencyNumber.EMERGENCY_CALL_ROUTING_UNKNOWN);
        mUsEmergencyNumber = new EmergencyNumber("911", "us", "",
            EmergencyNumber.EMERGENCY_SERVICE_CATEGORY_POLICE
                | EmergencyNumber.EMERGENCY_SERVICE_CATEGORY_AMBULANCE
                | EmergencyNumber.EMERGENCY_SERVICE_CATEGORY_FIRE_BRIGADE, new ArrayList<String>(),
            EmergencyNumber.EMERGENCY_NUMBER_SOURCE_DATABASE,
            EmergencyNumber.EMERGENCY_CALL_ROUTING_UNKNOWN);
        mEmergencyNumberListTestSample.add(emergencyNumberForTest);
    }

    private void sendEmergencyNumberListFromRadio() {
        mEmergencyNumberTrackerMock.sendMessage(
                mEmergencyNumberTrackerMock.obtainMessage(
                        1 /* EVENT_UNSOL_EMERGENCY_NUMBER_LIST */,
                        new AsyncResult(null, mEmergencyNumberListTestSample, null)));
        waitForHandlerAction(mEmergencyNumberTrackerMock, TIMEOUT_MS);
    }

    private void cacheEmergencyNumberListFromDatabaseByCountry(String countryIso) {
        mEmergencyNumberTrackerMock.updateEmergencyNumberDatabaseCountryChange(countryIso);
        waitForHandlerAction(mEmergencyNumberTrackerMock, TIMEOUT_MS);
    }

    private void sendEmergencyNumberPrefix(EmergencyNumberTracker emergencyNumberTrackerMock) {
        emergencyNumberTrackerMock.obtainMessage(
        	4 /* EVENT_UPDATE_EMERGENCY_NUMBER_PREFIX */,
                mEmergencyNumberPrefixTestSample).sendToTarget();
        waitForHandlerAction(emergencyNumberTrackerMock, TIMEOUT_MS);
    }

    private void setDsdsPhones() throws Exception {
        mPhones = new Phone[] {mPhone, mPhone2};
        replaceInstance(PhoneFactory.class, "sPhones", null, mPhones);
    }

    private void setSinglePhone() throws Exception {
        mPhones = new Phone[] {mPhone};
        replaceInstance(PhoneFactory.class, "sPhones", null, mPhones);
    }

    @Test
    public void testEmergencyNumberListFromRadio() throws Exception {
        sendEmergencyNumberListFromRadio();
        assertEquals(mEmergencyNumberListTestSample,
                mEmergencyNumberTrackerMock.getRadioEmergencyNumberList());
    }

    @Test
    public void testUpdateEmergencyCountryIso() throws Exception {
        sendEmergencyNumberPrefix(mEmergencyNumberTrackerMock);
        mEmergencyNumberTrackerMock.updateEmergencyNumberDatabaseCountryChange("us");
        waitForHandlerAction(mEmergencyNumberTrackerMock, TIMEOUT_MS);

        assertTrue(mEmergencyNumberTrackerMock.getEmergencyCountryIso().equals("us"));
    }

    @Test
    public void testUpdateEmergencyCountryIsoMultiSim() throws Exception {
        setDsdsPhones();
        sendEmergencyNumberPrefix(mEmergencyNumberTrackerMock);
        sendEmergencyNumberPrefix(mEmergencyNumberTrackerMock2);
        mEmergencyNumberTrackerMock.updateEmergencyCountryIsoAllPhones("jp");
        waitForHandlerAction(mEmergencyNumberTrackerMock, TIMEOUT_MS);
        waitForHandlerAction(mEmergencyNumberTrackerMock2, TIMEOUT_MS);

        assertTrue(mEmergencyNumberTrackerMock.getEmergencyCountryIso().equals("jp"));
        assertTrue(mEmergencyNumberTrackerMock2.getEmergencyCountryIso().equals("jp"));
    }

    @Test
    public void testUpdateEmergencyCountryIsoFromAnotherSimOrNot() throws Exception {
        setDsdsPhones();
        sendEmergencyNumberPrefix(mEmergencyNumberTrackerMock);
        sendEmergencyNumberPrefix(mEmergencyNumberTrackerMock2);

        // First, both slots have empty country iso, trigger a country change to "jp".
        // We should expect both sims have "jp" country iso.
        mEmergencyNumberTrackerMock.updateEmergencyCountryIsoAllPhones("jp");
        waitForHandlerAction(mEmergencyNumberTrackerMock, TIMEOUT_MS);
        waitForHandlerAction(mEmergencyNumberTrackerMock2, TIMEOUT_MS);
        assertTrue(mEmergencyNumberTrackerMock.getEmergencyCountryIso().equals("jp"));
        assertTrue(mEmergencyNumberTrackerMock2.getEmergencyCountryIso().equals("jp"));

        // Second, both slots now have "jp" country iso, trigger a country change to "us".
        // We should expect both sims have "us" country iso.
        mEmergencyNumberTrackerMock.updateEmergencyCountryIsoAllPhones("us");
        waitForHandlerAction(mEmergencyNumberTrackerMock, TIMEOUT_MS);
        waitForHandlerAction(mEmergencyNumberTrackerMock2, TIMEOUT_MS);
        assertTrue(mEmergencyNumberTrackerMock.getEmergencyCountryIso().equals("us"));
        assertTrue(mEmergencyNumberTrackerMock2.getEmergencyCountryIso().equals("us"));

        // Third, both slots now have "us" country iso, manually configure
        // "mIsCountrySetByAnotherSub" flag in "mPhone2" as false, and trigger a country
        // change to "ca". We should expect the current phone to change the country iso
        // to "ca", and should expect the other phone *not* to change their country iso
        // to "ca".
        mEmergencyNumberTrackerMock2.mIsCountrySetByAnotherSub = false;
        mEmergencyNumberTrackerMock.updateEmergencyCountryIsoAllPhones("ca");
        waitForHandlerAction(mEmergencyNumberTrackerMock, TIMEOUT_MS);
        waitForHandlerAction(mEmergencyNumberTrackerMock2, TIMEOUT_MS);
        assertTrue(mEmergencyNumberTrackerMock.getEmergencyCountryIso().equals("ca"));
        assertTrue(mEmergencyNumberTrackerMock2.getEmergencyCountryIso().equals("us"));
    }

    @Test
    public void testEmergencyNumberListPrefix() throws Exception {
        sendEmergencyNumberListFromRadio();
        sendEmergencyNumberPrefix(mEmergencyNumberTrackerMock);
        List<EmergencyNumber> resultToVerify = mEmergencyNumberListTestSample;
        resultToVerify.add(new EmergencyNumber("123119", "jp", "30",
                EmergencyNumber.EMERGENCY_SERVICE_CATEGORY_FIRE_BRIGADE,
                new ArrayList<String>(),
                EmergencyNumber.EMERGENCY_NUMBER_SOURCE_NETWORK_SIGNALING,
                EmergencyNumber.EMERGENCY_CALL_ROUTING_UNKNOWN));
        resultToVerify.add(new EmergencyNumber("456119", "jp", "30",
                EmergencyNumber.EMERGENCY_SERVICE_CATEGORY_FIRE_BRIGADE,
                new ArrayList<String>(),
                EmergencyNumber.EMERGENCY_NUMBER_SOURCE_NETWORK_SIGNALING,
                EmergencyNumber.EMERGENCY_CALL_ROUTING_UNKNOWN));
        Collections.sort(resultToVerify);

        List<EmergencyNumber> resultFromRadio = mEmergencyNumberTrackerMock
                .getRadioEmergencyNumberList();
        Collections.sort(resultFromRadio);

        assertEquals(resultToVerify, resultFromRadio);
    }
}
