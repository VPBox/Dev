/*
 * Copyright (C) 2017 The Android Open Source Project
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

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyInt;
import static org.mockito.Matchers.anyString;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.content.ComponentName;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ServiceInfo;
import android.os.Looper;
import android.os.RemoteException;
import android.service.carrier.CarrierMessagingService;
import android.service.carrier.ICarrierMessagingCallback;
import android.service.carrier.ICarrierMessagingService;
import android.service.carrier.MessagePdu;
import android.test.suitebuilder.annotation.SmallTest;
import android.util.LocalLog;

import com.android.internal.telephony.uicc.UiccCard;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

import java.util.ArrayList;
import java.util.List;

/**
 * Tests SMS filtering by carrier services.
 */
public class CarrierServicesSmsFilterTest extends TelephonyTest {
    private static final byte[] SMS_PDU = new byte[]{(byte) 0xFF, (byte) 0xFF, (byte) 0xFF};
    private static final String CARRIER_APP_PACKAGE_NAME = "com.android.carrier";
    private static final String SYSTEM_APP_PACKAGE_NAME = "com.android.system";

    private CarrierServicesSmsFilter mCarrierServicesSmsFilterUT;
    @Mock
    private CarrierServicesSmsFilter.CarrierServicesSmsFilterCallbackInterface mFilterCallback;
    @Mock
    private UiccCard mUiccCard;
    @Mock
    private ICarrierMessagingService.Stub mICarrierAppMessagingService;
    @Mock
    private ICarrierMessagingService.Stub mISystemCarrierMessagingService;

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());
        if (Looper.myLooper() == null) {
            Looper.prepare();
            Looper.loop();
        }
        mCarrierServicesSmsFilterUT = new CarrierServicesSmsFilter(
                mContext, mPhone, new byte[][]{SMS_PDU},
                0, "3gpp", mFilterCallback, getClass().getSimpleName(), new LocalLog(64)
        );
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
    }

    @Test
    @SmallTest
    public void testFilter_noCarrierServicesFilter_notHandled() throws Exception {
        assertFalse(mCarrierServicesSmsFilterUT.filter());
    }

    @Test
    @SmallTest
    public void testFilter_carrierAppPresent_handled() throws Exception {
        mockCarrierApp();
        mockCarrierAppStubResults(
                CarrierMessagingService.RECEIVE_OPTIONS_DROP, mICarrierAppMessagingService,
                true);
        assertTrue(mCarrierServicesSmsFilterUT.filter());

        verify(mFilterCallback, timeout(100))
                .onFilterComplete(eq(CarrierMessagingService.RECEIVE_OPTIONS_DROP));
    }

    @Test
    @SmallTest
    public void testFilter_systemAppPresent_handled() throws Exception {
        mockSystemApp();
        mockCarrierAppStubResults(
                CarrierMessagingService.RECEIVE_OPTIONS_DROP, mISystemCarrierMessagingService,
                true);

        assertTrue(mCarrierServicesSmsFilterUT.filter());

        verify(mFilterCallback, timeout(100))
                .onFilterComplete(eq(CarrierMessagingService.RECEIVE_OPTIONS_DROP));
    }

    @Test
    @SmallTest
    public void testFilter_bothCarrierAndSystemAppPresent_carrierAppDecides() throws Exception {
        mockCarrierApp();
        mockSystemApp();
        mockCarrierAppStubResults(
                CarrierMessagingService.RECEIVE_OPTIONS_DEFAULT, mICarrierAppMessagingService,
                true);
        mockCarrierAppStubResults(
                CarrierMessagingService.RECEIVE_OPTIONS_DROP, mISystemCarrierMessagingService,
                true);

        assertTrue(mCarrierServicesSmsFilterUT.filter());

        verify(mFilterCallback, timeout(100))
                .onFilterComplete(eq(CarrierMessagingService.RECEIVE_OPTIONS_DEFAULT));
    }

    @Test
    public void testFilterSmsShouldNotTimeout_whenOnFilterCompleteCalled() throws Exception {
        //This will make sure mCarrierServicesSmsFilterUT.filter() will return true, and therefore
        // filterSms() will return true
        mockCarrierApp();
        mockCarrierAppStubResults(
                CarrierMessagingService.RECEIVE_OPTIONS_DEFAULT, mICarrierAppMessagingService,
                true);
        assertTrue(mCarrierServicesSmsFilterUT.filter());

        verify(mFilterCallback, times(1))
                .onFilterComplete(eq(CarrierMessagingService.RECEIVE_OPTIONS_DEFAULT));
    }

    @Test
    public void testFilterSmsShouldTimeout_whenOnFilterCompleteNotCalled() throws Exception {
        //This will make sure mCarrierServicesSmsFilterUT.filter() will return true, and therefore
        // filterSms() will return true
        mockCarrierApp();
        mockCarrierAppStubResults(
                CarrierMessagingService.RECEIVE_OPTIONS_DEFAULT, mICarrierAppMessagingService,
                false);
        assertTrue(mCarrierServicesSmsFilterUT.filter());

        verify(mFilterCallback, times(0))
                .onFilterComplete(anyInt());
    }

    private void mockCarrierApp()
            throws RemoteException {
        mContextFixture.addService(
                CarrierMessagingService.SERVICE_INTERFACE,
                new ComponentName(CARRIER_APP_PACKAGE_NAME, "CarrierAppFilterClass"),
                CARRIER_APP_PACKAGE_NAME,
                mICarrierAppMessagingService,
                new ServiceInfo());
        mockUiccWithCarrierApp();
    }

    private void mockUiccWithCarrierApp() {
        when(mUiccController.getUiccCard(mPhone.getPhoneId())).thenReturn(mUiccCard);
        List<String> carrierPackages = new ArrayList<>();
        carrierPackages.add(CARRIER_APP_PACKAGE_NAME);
        when(mUiccCard.getCarrierPackageNamesForIntent(
                any(PackageManager.class), any(Intent.class))).thenReturn(carrierPackages);
    }

    private void mockSystemApp() {
        ServiceInfo serviceInfo = new ServiceInfo();
        serviceInfo.packageName = SYSTEM_APP_PACKAGE_NAME;
        mContextFixture.addService(
                CarrierMessagingService.SERVICE_INTERFACE,
                new ComponentName(SYSTEM_APP_PACKAGE_NAME, "SystemFilterClass"),
                SYSTEM_APP_PACKAGE_NAME,
                mISystemCarrierMessagingService,
                serviceInfo);
    }

    private void mockCarrierAppStubResults(final int result, ICarrierMessagingService.Stub stub,
            boolean callOnFilterComplete)
            throws RemoteException {
        when(stub.queryLocalInterface(anyString())).thenReturn(stub);
        when(stub.asBinder()).thenReturn(stub);
        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                Object[] args = invocation.getArguments();
                ICarrierMessagingCallback callback = (ICarrierMessagingCallback) args[4];
                if (callOnFilterComplete) {
                    callback.onFilterComplete(result);
                }
                return null;
            }
        }).when(stub).filterSms(
                any(MessagePdu.class), anyString(), anyInt(), anyInt(),
                any(ICarrierMessagingCallback.class));
    }
}
