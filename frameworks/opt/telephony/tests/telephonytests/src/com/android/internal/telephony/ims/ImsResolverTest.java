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

package com.android.internal.telephony.ims;

import static junit.framework.Assert.assertEquals;
import static junit.framework.Assert.assertNotNull;
import static junit.framework.Assert.assertNull;
import static junit.framework.Assert.assertTrue;
import static junit.framework.TestCase.assertFalse;

import static org.mockito.ArgumentMatchers.argThat;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyInt;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.atLeastOnce;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.Manifest;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.content.pm.ServiceInfo;
import android.net.Uri;
import android.os.Bundle;
import android.os.PersistableBundle;
import android.os.RemoteException;
import android.telephony.CarrierConfigManager;
import android.telephony.ims.ImsService;
import android.telephony.ims.feature.ImsFeature;
import android.telephony.ims.stub.ImsFeatureConfiguration;
import android.test.suitebuilder.annotation.SmallTest;

import androidx.test.runner.AndroidJUnit4;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.Mockito;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Objects;
import java.util.Set;
import java.util.stream.Collectors;

/**
 * Unit tests for ImsResolver
 */
@RunWith(AndroidJUnit4.class)
public class ImsResolverTest extends ImsTestBase {

    private static final int TEST_TIMEOUT = 200; //ms
    private static final ComponentName TEST_DEVICE_DEFAULT_NAME = new ComponentName("TestDevicePkg",
            "DeviceImsService");
    private static final ComponentName TEST_CARRIER_DEFAULT_NAME = new ComponentName(
            "TestCarrierPkg", "CarrierImsService");
    private static final ComponentName TEST_CARRIER_2_DEFAULT_NAME = new ComponentName(
            "TestCarrier2Pkg", "Carrier2ImsService");

    @Mock
    Context mMockContext;
    @Mock
    PackageManager mMockPM;
    @Mock
    ImsResolver.SubscriptionManagerProxy mTestSubscriptionManagerProxy;
    @Mock
    CarrierConfigManager mMockCarrierConfigManager;
    @Mock
    ImsResolver.ImsDynamicQueryManagerFactory mMockQueryManagerFactory;
    @Mock
    ImsServiceFeatureQueryManager mMockQueryManager;
    private ImsResolver mTestImsResolver;
    private BroadcastReceiver mTestPackageBroadcastReceiver;
    private BroadcastReceiver mTestCarrierConfigReceiver;
    private ImsServiceFeatureQueryManager.Listener mDynamicQueryListener;
    private PersistableBundle[] mCarrierConfigs;

    @Before
    @Override
    public void setUp() throws Exception {
        super.setUp();
    }

    @After
    @Override
    public void tearDown() throws Exception {
        mTestImsResolver = null;
        super.tearDown();
    }

    /**
     * Add a package to the package manager and make sure it is added to the cache of available
     * ImsServices in the ImsResolver.
     */
    @Test
    @SmallTest
    public void testAddDevicePackageToCache() {
        setupResolver(1/*numSlots*/);
        HashSet<String> features = new HashSet<>();
        features.add(ImsResolver.METADATA_EMERGENCY_MMTEL_FEATURE);
        features.add(ImsResolver.METADATA_MMTEL_FEATURE);
        features.add(ImsResolver.METADATA_RCS_FEATURE);
        setupPackageQuery(TEST_DEVICE_DEFAULT_NAME, features, true);
        setupController();

        // Complete package manager lookup and cache.
        startBind();

        ImsResolver.ImsServiceInfo testCachedService =
                mTestImsResolver.getImsServiceInfoFromCache(
                        TEST_DEVICE_DEFAULT_NAME.getPackageName());
        assertNotNull(testCachedService);
        assertTrue(isImsServiceInfoEqual(TEST_DEVICE_DEFAULT_NAME, features, testCachedService));
    }

    /**
     * Add a carrier ImsService to the package manager and make sure the features declared here are
     * ignored. We should only allow dynamic query for these services.
     */
    @Test
    @SmallTest
    public void testAddCarrierPackageToCache() {
        setupResolver(1/*numSlots*/);
        HashSet<String> features = new HashSet<>();
        features.add(ImsResolver.METADATA_EMERGENCY_MMTEL_FEATURE);
        features.add(ImsResolver.METADATA_MMTEL_FEATURE);
        features.add(ImsResolver.METADATA_RCS_FEATURE);
        setConfigCarrierString(0, TEST_CARRIER_DEFAULT_NAME.getPackageName());
        setupPackageQuery(TEST_CARRIER_DEFAULT_NAME, features, true);
        setupController();

        // Complete package manager lookup and cache.
        startBind();

        ImsResolver.ImsServiceInfo testCachedService =
                mTestImsResolver.getImsServiceInfoFromCache(
                        TEST_CARRIER_DEFAULT_NAME.getPackageName());
        assertNotNull(testCachedService);
        // none of the manifest features we added above should be reported for carrier package.
        assertTrue(testCachedService.getSupportedFeatures().isEmpty());
        // we should report that the service does not use metadata to report features.
        assertFalse(testCachedService.featureFromMetadata);
    }

    /**
     * Set the carrier config override value and ensure that ImsResolver calls .bind on that
     * package name with the correct ImsFeatures.
     */
    @Test
    @SmallTest
    public void testCarrierPackageBind() throws RemoteException {
        setupResolver(1/*numSlots*/);
        // Setup the carrier features
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> features = new HashSet<>();
        features.add(new ImsFeatureConfiguration.FeatureSlotPair(0, ImsFeature.FEATURE_MMTEL));
        features.add(new ImsFeatureConfiguration.FeatureSlotPair(0, ImsFeature.FEATURE_RCS));
        // Set CarrierConfig default package name and make it available as the CarrierConfig.
        setConfigCarrierString(0, TEST_CARRIER_DEFAULT_NAME.getPackageName());
        setupPackageQuery(TEST_CARRIER_DEFAULT_NAME, new HashSet<>(), true);
        ImsServiceController controller = setupController();

        // Start bind to carrier service
        startBind();
        // setup features response
        setupDynamicQueryFeatures(TEST_CARRIER_DEFAULT_NAME, features, 1);

        verify(controller).bind(features);
        verify(controller, never()).unbind();
        assertEquals(TEST_CARRIER_DEFAULT_NAME, controller.getComponentName());
    }

    /**
     * Creates a carrier ImsService that defines FEATURE_EMERGENCY_MMTEL and ensure that the
     * controller sets this capability.
     */
    @Test
    @SmallTest
    public void testCarrierPackageBindWithEmergencyCalling() throws RemoteException {
        setupResolver(1/*numSlots*/);
        // Set CarrierConfig default package name and make it available to the package manager
        setConfigCarrierString(0, TEST_CARRIER_DEFAULT_NAME.getPackageName());
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> features = new HashSet<>();
        features.add(new ImsFeatureConfiguration.FeatureSlotPair(0, ImsFeature.FEATURE_MMTEL));
        features.add(new ImsFeatureConfiguration.FeatureSlotPair(0, ImsFeature.FEATURE_RCS));
        setupPackageQuery(TEST_CARRIER_DEFAULT_NAME, new HashSet<>(), true);
        ImsServiceController controller = setupController();

        startBind();
        setupDynamicQueryFeatures(TEST_CARRIER_DEFAULT_NAME, features, 1);

        verify(controller).bind(features);
        verify(controller, never()).unbind();
        assertEquals(TEST_CARRIER_DEFAULT_NAME, controller.getComponentName());
    }

    /**
     * Creates a carrier ImsService that does not report FEATURE_EMERGENCY_MMTEL and then update the
     * ImsService to define it. Ensure that the controller sets this capability once enabled.
     */
    @Test
    @SmallTest
    public void testCarrierPackageChangeEmergencyCalling() throws RemoteException {
        setupResolver(1/*numSlots*/);
        // Set CarrierConfig default package name and make it available to the package manager
        setConfigCarrierString(0, TEST_CARRIER_DEFAULT_NAME.getPackageName());
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> features = new HashSet<>();
        features.add(new ImsFeatureConfiguration.FeatureSlotPair(0, ImsFeature.FEATURE_MMTEL));
        setupPackageQuery(TEST_CARRIER_DEFAULT_NAME, new HashSet<>(), true);
        ImsServiceController controller = setupController();

        // Bind without emergency calling
        startBind();
        setupDynamicQueryFeatures(TEST_CARRIER_DEFAULT_NAME, features, 1);
        verify(controller).bind(features);
        verify(controller, never()).unbind();
        assertEquals(TEST_CARRIER_DEFAULT_NAME, controller.getComponentName());

        packageChanged(TEST_CARRIER_DEFAULT_NAME.getPackageName());
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> newFeatures = new HashSet<>();
        newFeatures.add(new ImsFeatureConfiguration.FeatureSlotPair(0,
                ImsFeature.FEATURE_MMTEL));
        newFeatures.add(new ImsFeatureConfiguration.FeatureSlotPair(0,
                ImsFeature.FEATURE_EMERGENCY_MMTEL));
        setupDynamicQueryFeatures(TEST_CARRIER_DEFAULT_NAME, newFeatures, 2);

        //Verify new feature is added to the carrier override.
        // add all features for slot 0
        verify(controller, atLeastOnce()).changeImsServiceFeatures(newFeatures);
    }

    /**
     * Ensure that no ImsService is bound if there is no carrier or device package explicitly set.
     */
    @Test
    @SmallTest
    public void testDontBindWhenNullCarrierPackage() throws RemoteException {
        setupResolver(1/*numSlots*/);
        setupPackageQuery(TEST_CARRIER_DEFAULT_NAME, new HashSet<>(), true);
        ImsServiceController controller = setupController();

        // Set the CarrierConfig string to null so that ImsResolver will not bind to the available
        // Services
        setConfigCarrierString(0, null);
        startBind();

        waitForHandlerAction(mTestImsResolver.getHandler(), TEST_TIMEOUT);
        verify(mMockQueryManager, never()).startQuery(any(), any());
        verify(controller, never()).bind(any());
        verify(controller, never()).unbind();
    }

    /**
     * Test that the ImsService corresponding to the default device ImsService package name is
     * bound.
     */
    @Test
    @SmallTest
    public void testDevicePackageBind() throws RemoteException {
        setupResolver(1/*numSlots*/);
        List<ResolveInfo> info = new ArrayList<>();
        Set<String> features = new HashSet<>();
        features.add(ImsResolver.METADATA_MMTEL_FEATURE);
        features.add(ImsResolver.METADATA_RCS_FEATURE);
        // Use device default package, which will load the ImsService that the device provides
        info.add(getResolveInfo(TEST_DEVICE_DEFAULT_NAME, features, true));
        info.add(getResolveInfo(TEST_CARRIER_DEFAULT_NAME, new HashSet<>(), true));
        setupPackageQuery(info);
        ImsServiceController controller = setupController();


        startBind();
        // Wait to make sure that there are no dynamic queries that are being completed.
        waitForHandlerAction(mTestImsResolver.getHandler(), TEST_TIMEOUT);

        // There is no carrier override set, so make sure that the ImsServiceController binds
        // to all SIMs.
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> featureSet = convertToHashSet(features, 0);
        verify(controller).bind(featureSet);
        verify(controller, never()).unbind();
        verify(mMockQueryManager, never()).startQuery(any(), any());
        assertEquals(TEST_DEVICE_DEFAULT_NAME, controller.getComponentName());
    }

    /**
     * Test that when a device and carrier override package are set, both ImsServices are bound.
     * Verify that the carrier ImsService features are created and the device default features
     * are created for all features that are not covered by the carrier ImsService.
     */
    @Test
    @SmallTest
    public void testDeviceAndCarrierPackageBind() throws RemoteException {
        setupResolver(1/*numSlots*/);
        List<ResolveInfo> info = new ArrayList<>();
        Set<String> deviceFeatures = new HashSet<>();
        deviceFeatures.add(ImsResolver.METADATA_MMTEL_FEATURE);
        deviceFeatures.add(ImsResolver.METADATA_RCS_FEATURE);
        // Set the carrier override package for slot 0
        setConfigCarrierString(0, TEST_CARRIER_DEFAULT_NAME.getPackageName());
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> carrierFeatures = new HashSet<>();
        // Carrier service doesn't support the voice feature.
        carrierFeatures.add(new ImsFeatureConfiguration.FeatureSlotPair(0, ImsFeature.FEATURE_RCS));
        // Use device default package, which will load the ImsService that the device provides
        info.add(getResolveInfo(TEST_DEVICE_DEFAULT_NAME, deviceFeatures, true));
        info.add(getResolveInfo(TEST_CARRIER_DEFAULT_NAME, new HashSet<>(), true));
        // Only return info if not using the compat argument
        setupPackageQuery(info);
        ImsServiceController deviceController = mock(ImsServiceController.class);
        ImsServiceController carrierController = mock(ImsServiceController.class);
        setImsServiceControllerFactory(deviceController, carrierController);

        startBind();
        setupDynamicQueryFeatures(TEST_CARRIER_DEFAULT_NAME, carrierFeatures, 1);

        // Verify that all features that have been defined for the carrier override are bound
        verify(carrierController).bind(carrierFeatures);
        verify(carrierController, never()).unbind();
        assertEquals(TEST_CARRIER_DEFAULT_NAME, carrierController.getComponentName());
        // Verify that all features that are not defined in the carrier override are bound in the
        // device controller (including emergency voice for slot 0)
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> deviceFeatureSet =
                convertToHashSet(deviceFeatures, 0);
        verify(deviceController).bind(deviceFeatureSet);
        verify(deviceController, never()).unbind();
        assertEquals(TEST_DEVICE_DEFAULT_NAME, deviceController.getComponentName());
    }

    /**
     * Verify that the ImsServiceController is available for the feature specified
     * (carrier for VOICE/RCS and device for emergency).
     */
    @Test
    @SmallTest
    public void testGetDeviceCarrierFeatures() throws RemoteException {
        setupResolver(2/*numSlots*/);
        ImsServiceController deviceController = mock(ImsServiceController.class);
        ImsServiceController carrierController = mock(ImsServiceController.class);

        // Callback from mock ImsServiceControllers
        // All features on slot 1 should be the device default
        mTestImsResolver.imsServiceFeatureCreated(1, ImsFeature.FEATURE_MMTEL, deviceController);
        mTestImsResolver.imsServiceFeatureCreated(1, ImsFeature.FEATURE_RCS, deviceController);
        mTestImsResolver.imsServiceFeatureCreated(0, ImsFeature.FEATURE_MMTEL, deviceController);
        // The carrier override contains this feature
        mTestImsResolver.imsServiceFeatureCreated(0, ImsFeature.FEATURE_RCS, carrierController);
        // Get the IImsServiceControllers for each feature on each slot and verify they are correct.
        assertEquals(deviceController, mTestImsResolver.getImsServiceControllerAndListen(
                1 /*Slot id*/, ImsFeature.FEATURE_MMTEL, null));
        assertEquals(deviceController, mTestImsResolver.getImsServiceControllerAndListen(
                1 /*Slot id*/, ImsFeature.FEATURE_RCS, null));
        assertEquals(deviceController, mTestImsResolver.getImsServiceControllerAndListen(
                0 /*Slot id*/, ImsFeature.FEATURE_MMTEL, null));
        assertEquals(carrierController, mTestImsResolver.getImsServiceControllerAndListen(
                0 /*Slot id*/, ImsFeature.FEATURE_RCS, null));
    }

    /**
     * Bind to device ImsService and change the feature set. Verify that changeImsServiceFeature
     * is called with the new feature set.
     */
    @Test
    @SmallTest
    public void testAddDeviceFeatureNoCarrier() throws RemoteException {
        setupResolver(2/*numSlots*/);
        List<ResolveInfo> info = new ArrayList<>();
        Set<String> features = new HashSet<>();
        features.add(ImsResolver.METADATA_MMTEL_FEATURE);
        // Doesn't include RCS feature by default
        info.add(getResolveInfo(TEST_DEVICE_DEFAULT_NAME, features, true));
        setupPackageQuery(info);
        ImsServiceController controller = setupController();
        // Bind using default features
        startBind();
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> featureSet =
                convertToHashSet(features, 0);
        featureSet.addAll(convertToHashSet(features, 1));
        verify(controller).bind(featureSet);

        // add RCS to features list
        Set<String> newFeatures = new HashSet<>(features);
        newFeatures.add(ImsResolver.METADATA_RCS_FEATURE);
        info.clear();
        info.add(getResolveInfo(TEST_DEVICE_DEFAULT_NAME, newFeatures, true));

        packageChanged(TEST_DEVICE_DEFAULT_NAME.getPackageName());

        //Verify new feature is added to the device default.
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> newFeatureSet =
                convertToHashSet(newFeatures, 0);
        newFeatureSet.addAll(convertToHashSet(newFeatures, 1));
        verify(controller).changeImsServiceFeatures(newFeatureSet);
    }

    /**
     * Bind to device ImsService and change the feature set. Verify that changeImsServiceFeature
     * is called with the new feature set on the sub that doesn't include the carrier override.
     */
    @Test
    @SmallTest
    public void testAddDeviceFeatureWithCarrier() throws RemoteException {
        setupResolver(2/*numSlots*/);
        List<ResolveInfo> info = new ArrayList<>();
        Set<String> deviceFeatures = new HashSet<>();
        deviceFeatures.add(ImsResolver.METADATA_MMTEL_FEATURE);
        // Set the carrier override package for slot 0
        setConfigCarrierString(0, TEST_CARRIER_DEFAULT_NAME.getPackageName());
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> carrierFeatures = new HashSet<>();
        // Carrier service doesn't support the emergency voice feature.
        carrierFeatures.add(new ImsFeatureConfiguration.FeatureSlotPair(0,
                ImsFeature.FEATURE_MMTEL));
        carrierFeatures.add(new ImsFeatureConfiguration.FeatureSlotPair(0,
                ImsFeature.FEATURE_RCS));
        // Use device default package, which will load the ImsService that the device provides
        info.add(getResolveInfo(TEST_DEVICE_DEFAULT_NAME, deviceFeatures, true));
        info.add(getResolveInfo(TEST_CARRIER_DEFAULT_NAME, new HashSet<>(), true));
        setupPackageQuery(info);
        ImsServiceController deviceController = mock(ImsServiceController.class);
        ImsServiceController carrierController = mock(ImsServiceController.class);
        setImsServiceControllerFactory(deviceController, carrierController);

        startBind();
        setupDynamicQueryFeatures(TEST_CARRIER_DEFAULT_NAME, carrierFeatures, 1);

        // Verify that all features that have been defined for the carrier override are bound
        verify(carrierController).bind(carrierFeatures);
        verify(carrierController, never()).unbind();
        assertEquals(TEST_CARRIER_DEFAULT_NAME, carrierController.getComponentName());
        // Verify that all features that are not defined in the carrier override are bound in the
        // device controller (including emergency voice for slot 0)
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> deviceFeatureSet =
                convertToHashSet(deviceFeatures, 1);
        deviceFeatureSet.addAll(convertToHashSet(deviceFeatures, 0));
        deviceFeatureSet.removeAll(carrierFeatures);
        // we will first have bound to device and then the features will change once the dynamic
        // returns. So, instead of checking the bind parameters, we will check the change parameters
        verify(deviceController).changeImsServiceFeatures(deviceFeatureSet);
        verify(deviceController, never()).unbind();
        assertEquals(TEST_DEVICE_DEFAULT_NAME, deviceController.getComponentName());

        // add RCS to features list
        Set<String> newDeviceFeatures = new HashSet<>();
        newDeviceFeatures.add(ImsResolver.METADATA_MMTEL_FEATURE);
        newDeviceFeatures.add(ImsResolver.METADATA_RCS_FEATURE);
        info.clear();
        info.add(getResolveInfo(TEST_DEVICE_DEFAULT_NAME, newDeviceFeatures, true));

        // Tell the package manager that a new device feature is installed
        packageChanged(TEST_DEVICE_DEFAULT_NAME.getPackageName());

        //Verify new feature is added to the device default.
        // add all features for slot 1
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> newDeviceFeatureSet =
                convertToHashSet(newDeviceFeatures, 1);
        newDeviceFeatureSet.addAll(convertToHashSet(newDeviceFeatures, 0));
        // remove carrier overrides for slot 0
        newDeviceFeatureSet.removeAll(carrierFeatures);
        verify(deviceController).changeImsServiceFeatures(newDeviceFeatureSet);
        verify(carrierController, never()).changeImsServiceFeatures(any());
    }

    /**
     * Bind to device ImsService and change the feature set of the carrier overridden ImsService.
     * Verify that the device and carrier ImsServices are changed.
     */
    @Test
    @SmallTest
    public void testAddCarrierFeature() throws RemoteException {
        setupResolver(2/*numSlots*/);
        List<ResolveInfo> info = new ArrayList<>();
        Set<String> deviceFeatures = new HashSet<>();
        deviceFeatures.add(ImsResolver.METADATA_MMTEL_FEATURE);
        deviceFeatures.add(ImsResolver.METADATA_RCS_FEATURE);
        // Set the carrier override package for slot 0
        setConfigCarrierString(0, TEST_CARRIER_DEFAULT_NAME.getPackageName());
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> carrierFeatures = new HashSet<>();
        // Carrier service doesn't support the emergency voice feature.
        carrierFeatures.add(new ImsFeatureConfiguration.FeatureSlotPair(0,
                ImsFeature.FEATURE_MMTEL));
        // Use device default package, which will load the ImsService that the device provides
        info.add(getResolveInfo(TEST_DEVICE_DEFAULT_NAME, deviceFeatures, true));
        info.add(getResolveInfo(TEST_CARRIER_DEFAULT_NAME, new HashSet<>(), true));
        setupPackageQuery(info);
        ImsServiceController deviceController = mock(ImsServiceController.class);
        ImsServiceController carrierController = mock(ImsServiceController.class);
        setImsServiceControllerFactory(deviceController, carrierController);

        startBind();
        setupDynamicQueryFeatures(TEST_CARRIER_DEFAULT_NAME, carrierFeatures, 1);
        // Verify that all features that have been defined for the carrier override are bound
        verify(carrierController).bind(carrierFeatures);
        verify(carrierController, never()).unbind();
        assertEquals(TEST_CARRIER_DEFAULT_NAME, carrierController.getComponentName());
        // Verify that all features that are not defined in the carrier override are bound in the
        // device controller (including emergency voice for slot 0)
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> deviceFeatureSet =
                convertToHashSet(deviceFeatures, 1);
        deviceFeatureSet.addAll(convertToHashSet(deviceFeatures, 0));
        deviceFeatureSet.removeAll(carrierFeatures);
        // device ImsService will bind with all of its defined features first and then when the
        // carrier query comes back, it will change. So, checking change instead of bind here.
        verify(deviceController).changeImsServiceFeatures(deviceFeatureSet);
        verify(deviceController, never()).unbind();
        assertEquals(TEST_DEVICE_DEFAULT_NAME, deviceController.getComponentName());

        // add RCS to carrier features list
        carrierFeatures.add(new ImsFeatureConfiguration.FeatureSlotPair(0, ImsFeature.FEATURE_RCS));

        // Tell the package manager that a new device feature is installed
        packageChanged(TEST_CARRIER_DEFAULT_NAME.getPackageName());
        setupDynamicQueryFeatures(TEST_CARRIER_DEFAULT_NAME, carrierFeatures, 2);

        //Verify new feature is added to the carrier override.
        // add all features for slot 0
        verify(carrierController).changeImsServiceFeatures(carrierFeatures);
        deviceFeatureSet.removeAll(carrierFeatures);
        verify(deviceController).changeImsServiceFeatures(deviceFeatureSet);
    }

    /**
     * Bind to device ImsService and change the feature set of the carrier overridden ImsService by
     * removing a feature.
     * Verify that the device and carrier ImsServices are changed.
     */
    @Test
    @SmallTest
    public void testRemoveCarrierFeature() throws RemoteException {
        setupResolver(2/*numSlots*/);
        List<ResolveInfo> info = new ArrayList<>();
        Set<String> deviceFeatures = new HashSet<>();
        deviceFeatures.add(ImsResolver.METADATA_MMTEL_FEATURE);
        deviceFeatures.add(ImsResolver.METADATA_RCS_FEATURE);
        // Set the carrier override package for slot 0
        setConfigCarrierString(0, TEST_CARRIER_DEFAULT_NAME.getPackageName());
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> carrierFeatures = new HashSet<>();
        // Carrier service doesn't support the voice feature.
        carrierFeatures.add(new ImsFeatureConfiguration.FeatureSlotPair(0, ImsFeature.FEATURE_RCS));
        // Use device default package, which will load the ImsService that the device provides
        info.add(getResolveInfo(TEST_DEVICE_DEFAULT_NAME, deviceFeatures, true));
        info.add(getResolveInfo(TEST_CARRIER_DEFAULT_NAME, new HashSet<>(), true));
        setupPackageQuery(info);
        ImsServiceController deviceController = mock(ImsServiceController.class);
        ImsServiceController carrierController = mock(ImsServiceController.class);
        setImsServiceControllerFactory(deviceController, carrierController);

        startBind();
        setupDynamicQueryFeatures(TEST_CARRIER_DEFAULT_NAME, carrierFeatures, 1);
        // Verify that all features that have been defined for the carrier override are bound
        verify(carrierController).bind(carrierFeatures);
        verify(carrierController, never()).unbind();
        assertEquals(TEST_CARRIER_DEFAULT_NAME, carrierController.getComponentName());
        // Verify that all features that are not defined in the carrier override are bound in the
        // device controller (including emergency voice for slot 0)
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> deviceFeatureSet =
                convertToHashSet(deviceFeatures, 1);
        deviceFeatureSet.addAll(convertToHashSet(deviceFeatures, 0));
        deviceFeatureSet.removeAll(carrierFeatures);
        // we will first have bound to device and then the features will change once the dynamic
        // returns. So, instead of checking the bind parameters, we will check the change parameters
        verify(deviceController).changeImsServiceFeatures(deviceFeatureSet);
        verify(deviceController, never()).unbind();
        assertEquals(TEST_DEVICE_DEFAULT_NAME, deviceController.getComponentName());

        // change supported feature to MMTEL only
        carrierFeatures.clear();
        carrierFeatures.add(new ImsFeatureConfiguration.FeatureSlotPair(0,
                ImsFeature.FEATURE_MMTEL));
        // Tell the package manager that a new device feature is installed
        packageChanged(TEST_CARRIER_DEFAULT_NAME.getPackageName());
        setupDynamicQueryFeatures(TEST_CARRIER_DEFAULT_NAME, carrierFeatures, 2);

        //Verify new feature is added to the carrier override.
        verify(carrierController).changeImsServiceFeatures(carrierFeatures);
        Set<String> newDeviceFeatures = new HashSet<>();
        newDeviceFeatures.add(ImsResolver.METADATA_MMTEL_FEATURE);
        newDeviceFeatures.add(ImsResolver.METADATA_RCS_FEATURE);
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> newDeviceFeatureSet =
                convertToHashSet(newDeviceFeatures, 1);
        newDeviceFeatureSet.addAll(convertToHashSet(newDeviceFeatures, 0));
        newDeviceFeatureSet.removeAll(carrierFeatures);
        verify(deviceController).changeImsServiceFeatures(newDeviceFeatureSet);
    }

    /**
     * Inform the ImsResolver that a Carrier ImsService has been installed and must be bound.
     */
    @Test
    @SmallTest
    public void testInstallCarrierImsService() throws RemoteException {
        setupResolver(2/*numSlots*/);
        List<ResolveInfo> info = new ArrayList<>();
        Set<String> deviceFeatures = new HashSet<>();
        deviceFeatures.add(ImsResolver.METADATA_MMTEL_FEATURE);
        deviceFeatures.add(ImsResolver.METADATA_RCS_FEATURE);
        // Set the carrier override package for slot 0
        setConfigCarrierString(0, TEST_CARRIER_DEFAULT_NAME.getPackageName());
        // Use device default package, which will load the ImsService that the device provides
        info.add(getResolveInfo(TEST_DEVICE_DEFAULT_NAME, deviceFeatures, true));
        setupPackageQuery(info);
        ImsServiceController deviceController = mock(ImsServiceController.class);
        ImsServiceController carrierController = mock(ImsServiceController.class);
        setImsServiceControllerFactory(deviceController, carrierController);

        startBind();

        HashSet<ImsFeatureConfiguration.FeatureSlotPair> carrierFeatures = new HashSet<>();
        // Carrier service doesn't support the voice feature.
        carrierFeatures.add(new ImsFeatureConfiguration.FeatureSlotPair(0, ImsFeature.FEATURE_RCS));
        info.add(getResolveInfo(TEST_CARRIER_DEFAULT_NAME, new HashSet<>(), true));

        // Tell the package manager that a new carrier app is installed
        packageChanged(TEST_CARRIER_DEFAULT_NAME.getPackageName());
        setupDynamicQueryFeatures(TEST_CARRIER_DEFAULT_NAME, carrierFeatures, 1);

        // Verify that all features that have been defined for the carrier override are bound
        verify(carrierController).bind(carrierFeatures);
        // device features change
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> deviceFeatureSet =
                convertToHashSet(deviceFeatures, 1);
        deviceFeatureSet.addAll(convertToHashSet(deviceFeatures, 0));
        deviceFeatureSet.removeAll(carrierFeatures);
        verify(deviceController).changeImsServiceFeatures(deviceFeatureSet);
    }

    /**
     * Inform the ImsResolver that a carrier ImsService has been uninstalled and the device default
     * must now use those features.
     */
    @Test
    @SmallTest
    public void testUninstallCarrierImsService() throws RemoteException {
        setupResolver(2/*numSlots*/);
        List<ResolveInfo> info = new ArrayList<>();
        Set<String> deviceFeatures = new HashSet<>();
        deviceFeatures.add(ImsResolver.METADATA_MMTEL_FEATURE);
        deviceFeatures.add(ImsResolver.METADATA_RCS_FEATURE);
        // Set the carrier override package for slot 0
        setConfigCarrierString(0, TEST_CARRIER_DEFAULT_NAME.getPackageName());
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> carrierFeatures = new HashSet<>();
        // Carrier service doesn't support the voice feature.
        carrierFeatures.add(new ImsFeatureConfiguration.FeatureSlotPair(0, ImsFeature.FEATURE_RCS));
        info.add(getResolveInfo(TEST_CARRIER_DEFAULT_NAME, new HashSet<>(), true));
        // Use device default package, which will load the ImsService that the device provides
        info.add(getResolveInfo(TEST_DEVICE_DEFAULT_NAME, deviceFeatures, true));
        setupPackageQuery(info);
        ImsServiceController deviceController = mock(ImsServiceController.class);
        ImsServiceController carrierController = mock(ImsServiceController.class);
        setImsServiceControllerFactory(deviceController, carrierController);

        startBind();
        setupDynamicQueryFeatures(TEST_CARRIER_DEFAULT_NAME, carrierFeatures, 1);

        // Tell the package manager that carrier app is uninstalled
        info.clear();
        info.add(getResolveInfo(TEST_DEVICE_DEFAULT_NAME, deviceFeatures, true));
        packageRemoved(TEST_CARRIER_DEFAULT_NAME.getPackageName());

        // Verify that the carrier controller is unbound
        verify(carrierController).unbind();
        assertNull(mTestImsResolver.getImsServiceInfoFromCache(
                TEST_CARRIER_DEFAULT_NAME.getPackageName()));
        // device features change to include all supported functionality
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> deviceFeatureSet =
                convertToHashSet(deviceFeatures, 1);
        deviceFeatureSet.addAll(convertToHashSet(deviceFeatures, 0));
        verify(deviceController).changeImsServiceFeatures(deviceFeatureSet);
    }

    /**
     * Inform ImsResolver that the carrier config has changed to none, requiring the device
     * ImsService to be bound/set up and the previous carrier ImsService to be unbound.
     */
    @Test
    @SmallTest
    public void testCarrierConfigChangedToNone() throws RemoteException {
        setupResolver(2/*numSlots*/);
        List<ResolveInfo> info = new ArrayList<>();
        Set<String> deviceFeatures = new HashSet<>();
        deviceFeatures.add(ImsResolver.METADATA_MMTEL_FEATURE);
        deviceFeatures.add(ImsResolver.METADATA_RCS_FEATURE);
        // Set the carrier override package for slot 0
        setConfigCarrierString(0, TEST_CARRIER_DEFAULT_NAME.getPackageName());
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> carrierFeatures = new HashSet<>();
        // Carrier service doesn't support the voice feature.
        carrierFeatures.add(new ImsFeatureConfiguration.FeatureSlotPair(0, ImsFeature.FEATURE_RCS));
        info.add(getResolveInfo(TEST_CARRIER_DEFAULT_NAME, new HashSet<>(), true));
        // Use device default package, which will load the ImsService that the device provides
        info.add(getResolveInfo(TEST_DEVICE_DEFAULT_NAME, deviceFeatures, true));
        setupPackageQuery(info);
        ImsServiceController deviceController = mock(ImsServiceController.class);
        ImsServiceController carrierController = mock(ImsServiceController.class);
        setImsServiceControllerFactory(deviceController, carrierController);

        startBind();
        setupDynamicQueryFeatures(TEST_CARRIER_DEFAULT_NAME, carrierFeatures, 1);

        setConfigCarrierString(0, null);
        Intent carrierConfigIntent = new Intent();
        carrierConfigIntent.putExtra(CarrierConfigManager.EXTRA_SLOT_INDEX, 0);
        mTestCarrierConfigReceiver.onReceive(null, carrierConfigIntent);
        waitForHandlerAction(mTestImsResolver.getHandler(), TEST_TIMEOUT);

        // Verify that the carrier controller is unbound
        verify(carrierController).unbind();
        assertNotNull(mTestImsResolver.getImsServiceInfoFromCache(
                TEST_CARRIER_DEFAULT_NAME.getPackageName()));
        // device features change
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> deviceFeatureSet =
                convertToHashSet(deviceFeatures, 1);
        deviceFeatureSet.addAll(convertToHashSet(deviceFeatures, 0));
        verify(deviceController).changeImsServiceFeatures(deviceFeatureSet);
    }

    /**
     * Inform ImsResolver that the carrier config has changed to another, requiring the new carrier
     * ImsService to be bound/set up and the previous carrier ImsService to be unbound.
     */
    @Test
    @SmallTest
    public void testCarrierConfigChangedToAnotherService() throws RemoteException {
        setupResolver(2/*numSlots*/);
        List<ResolveInfo> info = new ArrayList<>();
        Set<String> deviceFeatures = new HashSet<>();
        deviceFeatures.add(ImsResolver.METADATA_MMTEL_FEATURE);
        deviceFeatures.add(ImsResolver.METADATA_RCS_FEATURE);
        // Set the carrier override package for slot 0
        setConfigCarrierString(0, TEST_CARRIER_DEFAULT_NAME.getPackageName());
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> carrierFeatures1 = new HashSet<>();
        // Carrier service 1
        carrierFeatures1.add(new ImsFeatureConfiguration.FeatureSlotPair(0,
                ImsFeature.FEATURE_MMTEL));
        carrierFeatures1.add(
                new ImsFeatureConfiguration.FeatureSlotPair(0, ImsFeature.FEATURE_RCS));
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> carrierFeatures2 = new HashSet<>();
        // Carrier service 2 doesn't support the voice feature.
        carrierFeatures2.add(
                new ImsFeatureConfiguration.FeatureSlotPair(0, ImsFeature.FEATURE_RCS));
        info.add(getResolveInfo(TEST_CARRIER_2_DEFAULT_NAME, new HashSet<>(), true));
        info.add(getResolveInfo(TEST_CARRIER_DEFAULT_NAME, new HashSet<>(), true));
        // Use device default package, which will load the ImsService that the device provides
        info.add(getResolveInfo(TEST_DEVICE_DEFAULT_NAME, deviceFeatures, true));
        setupPackageQuery(info);
        ImsServiceController deviceController = mock(ImsServiceController.class);
        ImsServiceController carrierController1 = mock(ImsServiceController.class);
        ImsServiceController carrierController2 = mock(ImsServiceController.class);
        setImsServiceControllerFactory(deviceController, carrierController1, carrierController2);

        startBind();
        setupDynamicQueryFeatures(TEST_CARRIER_DEFAULT_NAME, carrierFeatures1, 1);

        setConfigCarrierString(0, TEST_CARRIER_2_DEFAULT_NAME.getPackageName());
        Intent carrierConfigIntent = new Intent();
        carrierConfigIntent.putExtra(CarrierConfigManager.EXTRA_SLOT_INDEX, 0);
        mTestCarrierConfigReceiver.onReceive(null, carrierConfigIntent);
        waitForHandlerAction(mTestImsResolver.getHandler(), TEST_TIMEOUT);
        setupDynamicQueryFeatures(TEST_CARRIER_2_DEFAULT_NAME, carrierFeatures2, 1);

        // Verify that carrier 1 is unbound
        verify(carrierController1).unbind();
        assertNotNull(mTestImsResolver.getImsServiceInfoFromCache(
                TEST_CARRIER_DEFAULT_NAME.getPackageName()));
        // Verify that carrier 2 is bound
        verify(carrierController2).bind(carrierFeatures2);
        assertNotNull(mTestImsResolver.getImsServiceInfoFromCache(
                TEST_CARRIER_2_DEFAULT_NAME.getPackageName()));
        // device features change to accommodate for the features carrier 2 lacks
        HashSet<ImsFeatureConfiguration.FeatureSlotPair> deviceFeatureSet =
                convertToHashSet(deviceFeatures, 1);
        deviceFeatureSet.addAll(convertToHashSet(deviceFeatures, 0));
        deviceFeatureSet.removeAll(carrierFeatures2);
        verify(deviceController).changeImsServiceFeatures(deviceFeatureSet);
    }

    private void setupResolver(int numSlots) {
        when(mMockContext.getSystemService(eq(Context.CARRIER_CONFIG_SERVICE))).thenReturn(
                mMockCarrierConfigManager);
        when(mMockContext.getPackageManager()).thenReturn(mMockPM);
        mCarrierConfigs = new PersistableBundle[numSlots];
        for (int i = 0; i < numSlots; i++) {
            mCarrierConfigs[i] = new PersistableBundle();
            when(mMockCarrierConfigManager.getConfigForSubId(eq(i))).thenReturn(
                    mCarrierConfigs[i]);
            when(mTestSubscriptionManagerProxy.getSlotIndex(eq(i))).thenReturn(i);
            when(mTestSubscriptionManagerProxy.getSubId(eq(i))).thenReturn(i);
        }

        mTestImsResolver = new ImsResolver(mMockContext, TEST_DEVICE_DEFAULT_NAME.getPackageName(),
                numSlots, true);

        ArgumentCaptor<BroadcastReceiver> packageBroadcastCaptor =
                ArgumentCaptor.forClass(BroadcastReceiver.class);
        ArgumentCaptor<BroadcastReceiver> carrierConfigCaptor =
                ArgumentCaptor.forClass(BroadcastReceiver.class);
        verify(mMockContext).registerReceiverAsUser(packageBroadcastCaptor.capture(), any(),
                any(), any(), any());
        verify(mMockContext).registerReceiver(carrierConfigCaptor.capture(), any());
        mTestCarrierConfigReceiver = carrierConfigCaptor.getValue();
        mTestPackageBroadcastReceiver = packageBroadcastCaptor.getValue();
        mTestImsResolver.setSubscriptionManagerProxy(mTestSubscriptionManagerProxy);
        when(mMockQueryManagerFactory.create(any(Context.class),
                any(ImsServiceFeatureQueryManager.Listener.class))).thenReturn(mMockQueryManager);
        mTestImsResolver.setImsDynamicQueryManagerFactory(mMockQueryManagerFactory);
    }

    private void setupPackageQuery(List<ResolveInfo> infos) {
        // Only return info if not using the compat argument
        when(mMockPM.queryIntentServicesAsUser(
                argThat(argument -> ImsService.SERVICE_INTERFACE.equals(argument.getAction())),
                anyInt(), anyInt())).thenReturn(infos);
    }

    private void setupPackageQuery(ComponentName name, Set<String> features,
            boolean isPermissionGranted) {
        List<ResolveInfo> info = new ArrayList<>();
        info.add(getResolveInfo(name, features, isPermissionGranted));
        // Only return info if not using the compat argument
        when(mMockPM.queryIntentServicesAsUser(
                argThat(argument -> ImsService.SERVICE_INTERFACE.equals(argument.getAction())),
                anyInt(), anyInt())).thenReturn(info);
    }

    private ImsServiceController setupController() {
        ImsServiceController controller = mock(ImsServiceController.class);
        mTestImsResolver.setImsServiceControllerFactory(
                new ImsResolver.ImsServiceControllerFactory() {
                    @Override
                    public String getServiceInterface() {
                        return ImsService.SERVICE_INTERFACE;
                    }

                    @Override
                    public ImsServiceController create(Context context, ComponentName componentName,
                            ImsServiceController.ImsServiceControllerCallbacks callbacks) {
                        when(controller.getComponentName()).thenReturn(componentName);
                        return controller;
                    }
                });
        return controller;
    }

    private void startBind() {
        mTestImsResolver.initPopulateCacheAndStartBind();
        ArgumentCaptor<ImsServiceFeatureQueryManager.Listener> queryManagerCaptor =
                ArgumentCaptor.forClass(ImsServiceFeatureQueryManager.Listener.class);
        verify(mMockQueryManagerFactory).create(any(Context.class), queryManagerCaptor.capture());
        mDynamicQueryListener = queryManagerCaptor.getValue();
        waitForHandlerAction(mTestImsResolver.getHandler(), TEST_TIMEOUT);
    }

    private void setupDynamicQueryFeatures(ComponentName name,
            HashSet<ImsFeatureConfiguration.FeatureSlotPair> features, int times) {
        // wait for schedule to happen
        waitForHandlerAction(mTestImsResolver.getHandler(), TEST_TIMEOUT);
        // ensure that startQuery was called
        when(mMockQueryManager.startQuery(any(ComponentName.class), any(String.class)))
                .thenReturn(true);
        verify(mMockQueryManager, Mockito.times(times)).startQuery(eq(name), any(String.class));
        mDynamicQueryListener.onComplete(name, features);
        // wait for handling of onComplete
        waitForHandlerAction(mTestImsResolver.getHandler(), TEST_TIMEOUT);
    }

    public void packageChanged(String packageName) {
        // Tell the package manager that a new device feature is installed
        Intent addPackageIntent = new Intent();
        addPackageIntent.setAction(Intent.ACTION_PACKAGE_CHANGED);
        addPackageIntent.setData(new Uri.Builder().scheme("package").opaquePart(packageName)
                .build());
        mTestPackageBroadcastReceiver.onReceive(null, addPackageIntent);
        waitForHandlerAction(mTestImsResolver.getHandler(), TEST_TIMEOUT);
    }

    public void packageRemoved(String packageName) {
        Intent removePackageIntent = new Intent();
        removePackageIntent.setAction(Intent.ACTION_PACKAGE_REMOVED);
        removePackageIntent.setData(new Uri.Builder().scheme("package")
                .opaquePart(TEST_CARRIER_DEFAULT_NAME.getPackageName()).build());
        mTestPackageBroadcastReceiver.onReceive(null, removePackageIntent);
        waitForHandlerAction(mTestImsResolver.getHandler(), TEST_TIMEOUT);
    }

    private void setImsServiceControllerFactory(ImsServiceController deviceController,
            ImsServiceController carrierController) {
        mTestImsResolver.setImsServiceControllerFactory(
                new ImsResolver.ImsServiceControllerFactory() {
                    @Override
                    public String getServiceInterface() {
                        return ImsService.SERVICE_INTERFACE;
                    }

                    @Override
                    public ImsServiceController create(Context context, ComponentName componentName,
                            ImsServiceController.ImsServiceControllerCallbacks callbacks) {
                        if (TEST_DEVICE_DEFAULT_NAME.getPackageName().equals(
                                componentName.getPackageName())) {
                            when(deviceController.getComponentName()).thenReturn(componentName);
                            return deviceController;
                        } else if (TEST_CARRIER_DEFAULT_NAME.getPackageName().equals(
                                componentName.getPackageName())) {
                            when(carrierController.getComponentName()).thenReturn(componentName);
                            return carrierController;
                        }
                        return null;
                    }
                });
    }

    private void setImsServiceControllerFactory(ImsServiceController deviceController,
            ImsServiceController carrierController1, ImsServiceController carrierController2) {
        mTestImsResolver.setImsServiceControllerFactory(
                new ImsResolver.ImsServiceControllerFactory() {
                    @Override
                    public String getServiceInterface() {
                        return ImsService.SERVICE_INTERFACE;
                    }

                    @Override
                    public ImsServiceController create(Context context, ComponentName componentName,
                            ImsServiceController.ImsServiceControllerCallbacks callbacks) {
                        if (TEST_DEVICE_DEFAULT_NAME.getPackageName().equals(
                                componentName.getPackageName())) {
                            when(deviceController.getComponentName()).thenReturn(componentName);
                            return deviceController;
                        } else if (TEST_CARRIER_DEFAULT_NAME.getPackageName().equals(
                                componentName.getPackageName())) {
                            when(carrierController1.getComponentName()).thenReturn(componentName);
                            return carrierController1;
                        } else if (TEST_CARRIER_2_DEFAULT_NAME.getPackageName().equals(
                                componentName.getPackageName())) {
                            when(carrierController2.getComponentName()).thenReturn(componentName);
                            return carrierController2;
                        }
                        return null;
                    }
                });
    }


    private void setConfigCarrierString(int subId, String packageName) {
        mCarrierConfigs[subId].putString(
                CarrierConfigManager.KEY_CONFIG_IMS_PACKAGE_OVERRIDE_STRING, packageName);
    }

    private HashSet<ImsFeatureConfiguration.FeatureSlotPair> convertToHashSet(
            Set<String> features, int slotId) {
        return features.stream()
                // We do not count this as a valid feature set member.
                .filter(f -> !ImsResolver.METADATA_EMERGENCY_MMTEL_FEATURE.equals(f))
                .map(f -> new ImsFeatureConfiguration.FeatureSlotPair(slotId,
                        metadataStringToFeature(f)))
                .collect(Collectors.toCollection(HashSet::new));
    }

    private int metadataStringToFeature(String f) {
        switch (f) {
            case ImsResolver.METADATA_MMTEL_FEATURE:
                return ImsFeature.FEATURE_MMTEL;
            case ImsResolver.METADATA_RCS_FEATURE:
                return ImsFeature.FEATURE_RCS;
        }
        return -1;
    }

    // Make sure the metadata provided in the service definition creates the associated features in
    // the ImsServiceInfo. Note: this only tests for one slot.
    private boolean isImsServiceInfoEqual(ComponentName name, Set<String> features,
            ImsResolver.ImsServiceInfo sInfo) {
        if (!Objects.equals(sInfo.name, name)) {
            return false;
        }
        for (String f : features) {
            switch (f) {
                case ImsResolver.METADATA_EMERGENCY_MMTEL_FEATURE:
                    if (!sInfo.getSupportedFeatures().contains(
                            new ImsFeatureConfiguration.FeatureSlotPair(0,
                                    ImsFeature.FEATURE_EMERGENCY_MMTEL))) {
                        return false;
                    }
                    break;
                case ImsResolver.METADATA_MMTEL_FEATURE:
                    if (!sInfo.getSupportedFeatures().contains(
                            new ImsFeatureConfiguration.FeatureSlotPair(0,
                                    ImsFeature.FEATURE_MMTEL))) {
                        return false;
                    }
                    break;
                case ImsResolver.METADATA_RCS_FEATURE:
                    if (!sInfo.getSupportedFeatures().contains(
                            new ImsFeatureConfiguration.FeatureSlotPair(0,
                                    ImsFeature.FEATURE_RCS))) {
                        return false;
                    }
                    break;
            }
        }
        return true;
    }

    private ResolveInfo getResolveInfo(ComponentName name, Set<String> features,
            boolean isPermissionGranted) {
        ResolveInfo info = new ResolveInfo();
        info.serviceInfo = new ServiceInfo();
        info.serviceInfo.packageName = name.getPackageName();
        info.serviceInfo.name = name.getClassName();
        info.serviceInfo.metaData = new Bundle();
        for (String s : features) {
            info.serviceInfo.metaData.putBoolean(s, true);
        }
        if (isPermissionGranted) {
            info.serviceInfo.permission = Manifest.permission.BIND_IMS_SERVICE;
        }
        return info;
    }
}
