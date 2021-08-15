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
import static junit.framework.Assert.assertNull;
import static junit.framework.Assert.assertTrue;
import static junit.framework.Assert.fail;

import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;

import android.content.Context;
import android.content.Intent;
import android.os.RemoteException;
import android.telephony.ims.aidl.IImsMmTelFeature;
import android.telephony.ims.aidl.IImsServiceController;
import android.telephony.ims.feature.ImsFeature;
import android.telephony.ims.feature.MmTelFeature;
import android.telephony.ims.stub.ImsFeatureConfiguration;
import android.test.suitebuilder.annotation.SmallTest;
import android.util.SparseArray;

import androidx.test.runner.AndroidJUnit4;

import com.android.ims.internal.IImsFeatureStatusCallback;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;

/**
 * Unit tests for ImsService
 */
@RunWith(AndroidJUnit4.class)
public class ImsServiceTest {

    private static final int TEST_SLOT_0 = 0;
    private static final int TEST_SLOT_1 = 1;

    private TestImsService mTestImsService;
    private IImsServiceController mTestImsServiceBinder;

    private Context mMockContext;
    private IImsFeatureStatusCallback mTestCallback;

    @Before
    public void setUp() throws Exception {
        mMockContext = mock(Context.class);
        mTestCallback = mock(IImsFeatureStatusCallback.class);
        mTestImsService = new TestImsService(mMockContext);
        mTestImsServiceBinder = (IImsServiceController) mTestImsService.onBind(
                new Intent(ImsService.SERVICE_INTERFACE));
    }

    @After
    public void tearDown() throws Exception {
        mMockContext = null;
        mTestCallback = null;
        mTestImsService = null;
        mTestImsServiceBinder = null;
    }

    @Test
    @SmallTest
    public void testCreateMMTelFeature() throws RemoteException {
        IImsMmTelFeature f = mTestImsServiceBinder.createMmTelFeature(TEST_SLOT_0, mTestCallback);
        mTestImsService.mTestMmTelFeature.sendSetFeatureState(ImsFeature.STATE_READY);

        SparseArray<ImsFeature> features = mTestImsService.getFeatures(TEST_SLOT_0);
        ImsFeature featureToVerify = features.get(ImsFeature.FEATURE_MMTEL);
        MmTelFeature testMMTelFeature = null;
        if (featureToVerify instanceof MmTelFeature) {
            testMMTelFeature = (MmTelFeature) featureToVerify;
        } else {
            fail();
        }
        assertEquals(mTestImsService.mSpyMmTelFeature, testMMTelFeature);
        // Verify that upon creating a feature, we assign the callback and get the set feature state
        // when querying it.
        verify(mTestImsService.mSpyMmTelFeature).addImsFeatureStatusCallback(eq(mTestCallback));
        assertEquals(ImsFeature.STATE_READY, f.getFeatureState());
    }

    @Test
    @SmallTest
    public void testRemoveMMTelFeature() throws RemoteException {
        mTestImsServiceBinder.createMmTelFeature(TEST_SLOT_0, mTestCallback);

        mTestImsServiceBinder.removeImsFeature(TEST_SLOT_0, ImsFeature.FEATURE_MMTEL,
                mTestCallback);

        verify(mTestImsService.mSpyMmTelFeature).onFeatureRemoved();
        verify(mTestImsService.mSpyMmTelFeature).removeImsFeatureStatusCallback(mTestCallback);
        SparseArray<ImsFeature> features = mTestImsService.getFeatures(TEST_SLOT_0);
        assertNull(features.get(ImsFeature.FEATURE_MMTEL));
    }

    @Test
    @SmallTest
    public void testCallMethodOnCreatedFeature() throws RemoteException {
        IImsMmTelFeature f = mTestImsServiceBinder.createMmTelFeature(TEST_SLOT_0, mTestCallback);

        f.getUtInterface();

        assertTrue(mTestImsService.mTestMmTelFeature.isUtInterfaceCalled);
    }

    /**
     * Tests that the ImsService will return the correct ImsFeatureConfiguration when queried.
     */
    @Test
    @SmallTest
    public void testQuerySupportedImsFeatures() throws RemoteException {
        ImsFeatureConfiguration config = new ImsFeatureConfiguration.Builder()
                .addFeature(0, ImsFeature.FEATURE_MMTEL)
                .addFeature(0, ImsFeature.FEATURE_RCS)
                .build();
        mTestImsService.testFeatureConfig = config;

        ImsFeatureConfiguration result = mTestImsServiceBinder.querySupportedImsFeatures();

        assertEquals(config, result);
    }
}
