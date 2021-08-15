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

package android.telephony.ims;

import static junit.framework.Assert.assertEquals;
import static junit.framework.Assert.assertTrue;

import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.verify;

import android.os.Parcel;
import android.os.RemoteException;
import android.telephony.ims.aidl.IImsCapabilityCallback;
import android.telephony.ims.feature.CapabilityChangeRequest;
import android.telephony.ims.feature.ImsFeature;
import android.telephony.ims.feature.MmTelFeature;
import android.telephony.ims.stub.ImsRegistrationImplBase;
import android.test.suitebuilder.annotation.SmallTest;

import androidx.test.runner.AndroidJUnit4;

import com.android.ims.internal.IImsFeatureStatusCallback;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.mockito.MockitoAnnotations;

@RunWith(AndroidJUnit4.class)
public class ImsFeatureTest {
    // Public for Mockito testing
    public class CapabilityCallback extends IImsCapabilityCallback.Stub {

        @Override
        public void onQueryCapabilityConfiguration(int capability, int radioTech, boolean enabled)
                throws RemoteException {

        }

        @Override
        public void onChangeCapabilityConfigurationError(int capability, int radioTech, int reason)
                throws RemoteException {

        }

        @Override
        public void onCapabilitiesStatusChanged(int config) throws RemoteException {

        }
    }

    private TestImsFeature mTestImsFeature;

    @Mock
    private IImsFeatureStatusCallback mTestStatusCallback;
    @Mock
    private IImsFeatureStatusCallback mTestStatusCallback2;

    @Before
    public void setUp() {
        MockitoAnnotations.initMocks(this);
        mTestImsFeature = new TestImsFeature();
    }

    @After
    public void tearDown() {
        mTestImsFeature = null;
    }

    @Test
    @SmallTest
    public void testSetCallbackAndNotify() throws Exception {
        mTestImsFeature.addImsFeatureStatusCallback(mTestStatusCallback);
        mTestImsFeature.addImsFeatureStatusCallback(mTestStatusCallback2);

        verify(mTestStatusCallback).notifyImsFeatureStatus(eq(ImsFeature.STATE_UNAVAILABLE));
        verify(mTestStatusCallback2).notifyImsFeatureStatus(eq(ImsFeature.STATE_UNAVAILABLE));
    }

    @Test
    @SmallTest
    public void testSetFeatureAndCheckCallback() throws Exception {
        mTestImsFeature.addImsFeatureStatusCallback(mTestStatusCallback);
        mTestImsFeature.addImsFeatureStatusCallback(mTestStatusCallback2);

        mTestImsFeature.testSetFeatureState(ImsFeature.STATE_READY);

        verify(mTestStatusCallback).notifyImsFeatureStatus(eq(ImsFeature.STATE_READY));
        verify(mTestStatusCallback2).notifyImsFeatureStatus(eq(ImsFeature.STATE_READY));
        assertEquals(ImsFeature.STATE_READY, mTestImsFeature.getFeatureState());
    }

    @SmallTest
    @Test
    public void testCapabilityConfigAdd() throws Exception {
        ImsFeature.Capabilities c = new ImsFeature.Capabilities();
        c.addCapabilities(TestImsFeature.CAPABILITY_TEST_1);

        assertTrue(c.isCapable(TestImsFeature.CAPABILITY_TEST_1));
    }

    @SmallTest
    @Test
    public void testCapabilityConfigAddMultiple() throws Exception {
        ImsFeature.Capabilities c = new ImsFeature.Capabilities();
        c.addCapabilities(TestImsFeature.CAPABILITY_TEST_1);
        c.addCapabilities(TestImsFeature.CAPABILITY_TEST_2);

        assertTrue(c.isCapable(TestImsFeature.CAPABILITY_TEST_2));
    }

    @SmallTest
    @Test
    public void testCapabilityConfigHasMultiple() throws Exception {
        ImsFeature.Capabilities c = new ImsFeature.Capabilities();
        c.addCapabilities(TestImsFeature.CAPABILITY_TEST_1);
        c.addCapabilities(TestImsFeature.CAPABILITY_TEST_2);

        assertTrue(c.isCapable(
                TestImsFeature.CAPABILITY_TEST_1 | TestImsFeature.CAPABILITY_TEST_2));
    }

    @SmallTest
    @Test
    public void testCapabilityConfigRemove() throws Exception {
        ImsFeature.Capabilities c = new ImsFeature.Capabilities();
        c.addCapabilities(TestImsFeature.CAPABILITY_TEST_1);
        c.addCapabilities(TestImsFeature.CAPABILITY_TEST_2);
        c.removeCapabilities(TestImsFeature.CAPABILITY_TEST_1);

        assertTrue(c.isCapable(TestImsFeature.CAPABILITY_TEST_2));
    }

    @SmallTest
    @Test
    public void testSetCapabilityConfig() throws Exception {
        CapabilityChangeRequest request = new CapabilityChangeRequest();
        request.addCapabilitiesToEnableForTech(TestImsFeature.CAPABILITY_TEST_1,
                ImsRegistrationImplBase.REGISTRATION_TECH_IWLAN);

        mTestImsFeature.requestChangeEnabledCapabilities(request, null);

        assertEquals(request, mTestImsFeature.lastRequest);
    }


    @SmallTest
    @Test
    public void testSetCapabilityConfigError() throws Exception {
        CapabilityCallback capabilityCallback = Mockito.spy(new CapabilityCallback());
        mTestImsFeature.addCapabilityCallback(capabilityCallback);

        CapabilityChangeRequest request = new CapabilityChangeRequest();
        request.addCapabilitiesToEnableForTech(TestImsFeature.CAPABILITY_TEST_1,
                ImsRegistrationImplBase.REGISTRATION_TECH_IWLAN);

        mTestImsFeature.setCapabilitiesResult = ImsFeature.CAPABILITY_ERROR_GENERIC;
        mTestImsFeature.requestChangeEnabledCapabilities(request, capabilityCallback);

        verify(capabilityCallback).onChangeCapabilityConfigurationError(
                eq(TestImsFeature.CAPABILITY_TEST_1),
                eq(ImsRegistrationImplBase.REGISTRATION_TECH_IWLAN),
                eq(ImsFeature.CAPABILITY_ERROR_GENERIC));
        assertEquals(request, mTestImsFeature.lastRequest);
    }

    @SmallTest
    @Test
    public void testNotifyCapabilityStatusChanged() throws Exception {
        ImsFeature.Capabilities status =
                new ImsFeature.Capabilities();
        status.addCapabilities(TestImsFeature.CAPABILITY_TEST_1);
        status.addCapabilities(TestImsFeature.CAPABILITY_TEST_2);

        mTestImsFeature.capabilitiesStatusChanged(status);

        assertEquals(status.getMask(), mTestImsFeature.queryCapabilityStatus().getMask());
    }

    @SmallTest
    @Test
    public void testNotifyCapabilityStatusChangedCallback() throws Exception {
        CapabilityCallback capabilityCallback = Mockito.spy(new CapabilityCallback());
        mTestImsFeature.addCapabilityCallback(capabilityCallback);

        ImsFeature.Capabilities status =
                new ImsFeature.Capabilities();
        status.addCapabilities(TestImsFeature.CAPABILITY_TEST_1);
        status.addCapabilities(TestImsFeature.CAPABILITY_TEST_2);

        mTestImsFeature.capabilitiesStatusChanged(status);

        assertEquals(status.getMask(), mTestImsFeature.queryCapabilityStatus().getMask());
        verify(capabilityCallback).onCapabilitiesStatusChanged(
                eq(TestImsFeature.CAPABILITY_TEST_1 | TestImsFeature.CAPABILITY_TEST_2));
    }

    @SmallTest
    @Test
    public void testCapabilityChangeContainsFullSets() throws Exception {
        CapabilityChangeRequest request = new CapabilityChangeRequest();
        request.addCapabilitiesToEnableForTech(TestImsFeature.CAPABILITY_TEST_1
                        | TestImsFeature.CAPABILITY_TEST_2,
                ImsRegistrationImplBase.REGISTRATION_TECH_IWLAN);
        request.addCapabilitiesToEnableForTech(TestImsFeature.CAPABILITY_TEST_2,
                ImsRegistrationImplBase.REGISTRATION_TECH_LTE);
        request.addCapabilitiesToDisableForTech(TestImsFeature.CAPABILITY_TEST_1,
                ImsRegistrationImplBase.REGISTRATION_TECH_LTE);

        mTestImsFeature.changeEnabledCapabilities(request, /*Callback*/null);

        assertTrue(request.getCapabilitiesToDisable().containsAll(
                mTestImsFeature.lastRequest.getCapabilitiesToDisable()));
        assertTrue(request.getCapabilitiesToEnable().containsAll(
                mTestImsFeature.lastRequest.getCapabilitiesToEnable()));
    }

    @SmallTest
    @Test
    public void testCapabilityChangeRequestParcel() throws Exception {
        CapabilityChangeRequest request = new CapabilityChangeRequest();
        // add some capabilities
        request.addCapabilitiesToEnableForTech(MmTelFeature.MmTelCapabilities.CAPABILITY_TYPE_VOICE,
                ImsRegistrationImplBase.REGISTRATION_TECH_IWLAN);
        request.addCapabilitiesToEnableForTech(
                MmTelFeature.MmTelCapabilities.CAPABILITY_TYPE_VIDEO
                        | MmTelFeature.MmTelCapabilities.CAPABILITY_TYPE_VOICE,
                ImsRegistrationImplBase.REGISTRATION_TECH_LTE);
        request.addCapabilitiesToDisableForTech(MmTelFeature.MmTelCapabilities.CAPABILITY_TYPE_UT,
                ImsRegistrationImplBase.REGISTRATION_TECH_LTE);
        request.addCapabilitiesToDisableForTech(MmTelFeature.MmTelCapabilities.CAPABILITY_TYPE_UT,
                ImsRegistrationImplBase.REGISTRATION_TECH_IWLAN);

        Parcel p = Parcel.obtain();
        request.writeToParcel(p, 0);
        p.setDataPosition(0);
        CapabilityChangeRequest result =
                CapabilityChangeRequest.CREATOR.createFromParcel(p);
        p.recycle();

        assertEquals(request, result);
    }

    @SmallTest
    @Test
    public void testCapabilityCallbackWhenRegistering() throws Exception {
        CapabilityCallback capabilityCallback = Mockito.spy(new CapabilityCallback());

        // Signal the status has changed
        ImsFeature.Capabilities status =
                new ImsFeature.Capabilities();
        status.addCapabilities(TestImsFeature.CAPABILITY_TEST_1);
        status.addCapabilities(TestImsFeature.CAPABILITY_TEST_2);
        mTestImsFeature.capabilitiesStatusChanged(status);

        // addCapabilityCallback should cause capabilityCallback to call back with status.
        mTestImsFeature.addCapabilityCallback(capabilityCallback);
        assertEquals(status.getMask(), mTestImsFeature.queryCapabilityStatus().getMask());
        verify(capabilityCallback).onCapabilitiesStatusChanged(
                eq(TestImsFeature.CAPABILITY_TEST_1 | TestImsFeature.CAPABILITY_TEST_2));
    }
}
