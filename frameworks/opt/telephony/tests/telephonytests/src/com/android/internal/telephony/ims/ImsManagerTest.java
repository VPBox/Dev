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

package com.android.internal.telephony.ims;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.anyString;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.content.pm.PackageManager;
import android.os.IBinder;
import android.os.PersistableBundle;
import android.telephony.CarrierConfigManager;
import android.telephony.SubscriptionManager;
import android.telephony.ims.ImsMmTelManager;
import android.telephony.ims.ProvisioningManager;
import android.telephony.ims.stub.ImsConfigImplBase;

import androidx.test.filters.SmallTest;
import androidx.test.runner.AndroidJUnit4;

import com.android.ims.ImsConfig;
import com.android.ims.ImsManager;
import com.android.ims.MmTelFeatureConnection;
import com.android.internal.telephony.TelephonyTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;

import java.util.Hashtable;

@RunWith(AndroidJUnit4.class)
public class ImsManagerTest extends TelephonyTest {
    private static final String UNSET_PROVISIONED_STRING = "unset";
    private static final boolean ENHANCED_4G_MODE_DEFAULT_VAL = true;
    private static final boolean ENHANCED_4G_MODE_EDITABLE = true;
    private static final boolean WFC_IMS_ENABLE_DEFAULT_VAL = false;
    private static final boolean WFC_IMS_ROAMING_ENABLE_DEFAULT_VAL = true;
    private static final boolean VT_IMS_ENABLE_DEFAULT_VAL = true;
    private static final boolean WFC_IMS_EDITABLE_VAL = true;
    private static final boolean WFC_IMS_NOT_EDITABLE_VAL = false;
    private static final boolean WFC_IMS_ROAMING_EDITABLE_VAL = true;
    private static final boolean WFC_IMS_ROAMING_NOT_EDITABLE_VAL = false;
    private static final int WFC_IMS_MODE_DEFAULT_VAL =
            ImsConfig.WfcModeFeatureValueConstants.CELLULAR_PREFERRED;
    private static final int WFC_IMS_ROAMING_MODE_DEFAULT_VAL =
            ImsConfig.WfcModeFeatureValueConstants.WIFI_PREFERRED;
    private static final boolean WFC_USE_HOME_MODE_FOR_ROAMING_VAL = true;
    private static final boolean WFC_NOT_USE_HOME_MODE_FOR_ROAMING_VAL = false;

    PersistableBundle mBundle;
    @Mock IBinder mBinder;
    @Mock ImsConfigImplBase mImsConfigImplBaseMock;
    Hashtable<Integer, Integer> mProvisionedIntVals = new Hashtable<>();
    Hashtable<Integer, String> mProvisionedStringVals = new Hashtable<>();
    ImsConfigImplBase.ImsConfigStub mImsConfigStub;
    @Mock MmTelFeatureConnection mMmTelFeatureConnection;

    private final int[] mSubId = {0};
    private int mPhoneId;

    @Before
    public void setUp() throws Exception {
        super.setUp("ImsManagerTest");
        mPhoneId = mPhone.getPhoneId();
        mBundle = mContextFixture.getCarrierConfigBundle();
        // Force MmTelFeatureConnection to create an executor using Looper.myLooper().
        doReturn(null).when(mContext).getMainLooper();

        doReturn(mSubId).when(mSubscriptionController).getSubId(mPhoneId);

        doReturn(mSubscriptionController).when(mBinder).queryLocalInterface(anyString());
        mServiceManagerMockedServices.put("isub", mBinder);
        // Stick to the CarrierConfig defaults unless explicitly overwritten.
        doReturn("-1").when(mSubscriptionController)
                .getSubscriptionProperty(anyInt(), anyString(), anyString());


        doReturn(true).when(mMmTelFeatureConnection).isBinderAlive();
        mContextFixture.addSystemFeature(PackageManager.FEATURE_TELEPHONY_IMS);

        mImsManagerInstances.remove(mPhoneId);

        setDefaultValues();
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
    }

    private void setDefaultValues() {
        mBundle.putBoolean(CarrierConfigManager.KEY_EDITABLE_ENHANCED_4G_LTE_BOOL,
                ENHANCED_4G_MODE_EDITABLE);
        mBundle.putBoolean(CarrierConfigManager.KEY_EDITABLE_WFC_MODE_BOOL,
                WFC_IMS_EDITABLE_VAL);
        mBundle.putBoolean(CarrierConfigManager.KEY_EDITABLE_WFC_ROAMING_MODE_BOOL,
                WFC_IMS_ROAMING_EDITABLE_VAL);
        mBundle.putBoolean(CarrierConfigManager.KEY_CARRIER_DEFAULT_WFC_IMS_ENABLED_BOOL,
                WFC_IMS_ENABLE_DEFAULT_VAL);
        mBundle.putBoolean(CarrierConfigManager.KEY_CARRIER_DEFAULT_WFC_IMS_ROAMING_ENABLED_BOOL,
                WFC_IMS_ROAMING_ENABLE_DEFAULT_VAL);
        mBundle.putInt(CarrierConfigManager.KEY_CARRIER_DEFAULT_WFC_IMS_MODE_INT,
                WFC_IMS_MODE_DEFAULT_VAL);
        mBundle.putInt(CarrierConfigManager.KEY_CARRIER_DEFAULT_WFC_IMS_ROAMING_MODE_INT,
                WFC_IMS_ROAMING_MODE_DEFAULT_VAL);
        mBundle.putBoolean(CarrierConfigManager.KEY_ENHANCED_4G_LTE_ON_BY_DEFAULT_BOOL,
                ENHANCED_4G_MODE_DEFAULT_VAL);
        mBundle.putBoolean(CarrierConfigManager.KEY_CARRIER_VOLTE_PROVISIONING_REQUIRED_BOOL, true);
        mBundle.putBoolean(
                CarrierConfigManager.KEY_USE_WFC_HOME_NETWORK_MODE_IN_ROAMING_NETWORK_BOOL,
                WFC_NOT_USE_HOME_MODE_FOR_ROAMING_VAL);
    }

    @Test @SmallTest
    public void testGetDefaultValues() {
        ImsManager imsManager = getImsManagerAndInitProvisionedValues();

        assertEquals(WFC_IMS_ENABLE_DEFAULT_VAL, imsManager.isWfcEnabledByUser());
        verify(mSubscriptionController, times(1)).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_ENABLED),
                anyString());

        assertEquals(WFC_IMS_ROAMING_ENABLE_DEFAULT_VAL, imsManager.isWfcRoamingEnabledByUser());
        verify(mSubscriptionController, times(1)).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_ROAMING_ENABLED),
                anyString());

        assertEquals(ENHANCED_4G_MODE_DEFAULT_VAL,
                imsManager.isEnhanced4gLteModeSettingEnabledByUser());
        verify(mSubscriptionController, times(1)).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.ENHANCED_4G_MODE_ENABLED),
                anyString());

        assertEquals(WFC_IMS_MODE_DEFAULT_VAL, imsManager.getWfcMode(false));
        verify(mSubscriptionController, times(1)).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_MODE),
                anyString());

        assertEquals(WFC_IMS_ROAMING_MODE_DEFAULT_VAL, imsManager.getWfcMode(true));
        verify(mSubscriptionController, times(1)).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_ROAMING_MODE),
                anyString());

        assertEquals(VT_IMS_ENABLE_DEFAULT_VAL, imsManager.isVtEnabledByUser());
        verify(mSubscriptionController, times(1)).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.VT_IMS_ENABLED),
                anyString());
    }

    @Test @SmallTest
    public void testSetValues() {
        ImsManager imsManager = getImsManagerAndInitProvisionedValues();

        imsManager.setWfcMode(ImsConfig.WfcModeFeatureValueConstants.CELLULAR_PREFERRED);
        verify(mSubscriptionController, times(1)).setSubscriptionProperty(
                eq(mSubId[0]),
                eq(SubscriptionManager.WFC_IMS_MODE),
                eq("1"));

        imsManager.setWfcMode(ImsConfig.WfcModeFeatureValueConstants.CELLULAR_PREFERRED, true);
        verify(mSubscriptionController, times(1)).setSubscriptionProperty(
                eq(mSubId[0]),
                eq(SubscriptionManager.WFC_IMS_ROAMING_MODE),
                eq("1"));

        imsManager.setVtSetting(false);
        verify(mSubscriptionController, times(1)).setSubscriptionProperty(
                eq(mSubId[0]),
                eq(SubscriptionManager.VT_IMS_ENABLED),
                eq("0"));

        // enhanced 4g mode must be editable to use setEnhanced4gLteModeSetting
        mBundle.putBoolean(CarrierConfigManager.KEY_EDITABLE_ENHANCED_4G_LTE_BOOL,
                ENHANCED_4G_MODE_EDITABLE);
        imsManager.setEnhanced4gLteModeSetting(true);
        verify(mSubscriptionController, times(1)).setSubscriptionProperty(
                eq(mSubId[0]),
                eq(SubscriptionManager.ENHANCED_4G_MODE_ENABLED),
                eq("1"));

        imsManager.setWfcSetting(true);
        verify(mSubscriptionController, times(1)).setSubscriptionProperty(
                eq(mSubId[0]),
                eq(SubscriptionManager.WFC_IMS_ENABLED),
                eq("1"));
    }
    @Test
    public void testGetProvisionedValues() throws Exception {
        ImsManager imsManager = getImsManagerAndInitProvisionedValues();

        assertEquals(true, imsManager.isWfcProvisionedOnDevice());
        verify(mImsConfigImplBaseMock, times(1)).getConfigInt(
                eq(ImsConfig.ConfigConstants.VOICE_OVER_WIFI_SETTING_ENABLED));

        assertEquals(true, imsManager.isVtProvisionedOnDevice());
        verify(mImsConfigImplBaseMock, times(1)).getConfigInt(
                eq(ImsConfig.ConfigConstants.LVC_SETTING_ENABLED));

        assertEquals(true, imsManager.isVolteProvisionedOnDevice());
        verify(mImsConfigImplBaseMock, times(1)).getConfigInt(
                eq(ImsConfig.ConfigConstants.VLT_SETTING_ENABLED));

        // If we call get again, times should still be one because the value should be fetched
        // from cache.
        assertEquals(true, imsManager.isWfcProvisionedOnDevice());
        verify(mImsConfigImplBaseMock, times(1)).getConfigInt(
                eq(ImsConfig.ConfigConstants.VOICE_OVER_WIFI_SETTING_ENABLED));

        assertEquals(true, imsManager.isVtProvisionedOnDevice());
        verify(mImsConfigImplBaseMock, times(1)).getConfigInt(
                eq(ImsConfig.ConfigConstants.LVC_SETTING_ENABLED));

        assertEquals(true, imsManager.isVolteProvisionedOnDevice());
        verify(mImsConfigImplBaseMock, times(1)).getConfigInt(
                eq(ImsConfig.ConfigConstants.VLT_SETTING_ENABLED));
    }

    @Test
    public void testSetProvisionedValues() throws Exception {
        ImsManager imsManager = getImsManagerAndInitProvisionedValues();

        assertEquals(true, imsManager.isWfcProvisionedOnDevice());
        verify(mImsConfigImplBaseMock, times(1)).getConfigInt(
                eq(ImsConfig.ConfigConstants.VOICE_OVER_WIFI_SETTING_ENABLED));

        imsManager.getConfigInterface().setProvisionedValue(
                ImsConfig.ConfigConstants.VOICE_OVER_WIFI_SETTING_ENABLED,
                ImsConfig.FeatureValueConstants.OFF);

        assertEquals(0, (int) mProvisionedIntVals.get(
                ImsConfig.ConfigConstants.VOICE_OVER_WIFI_SETTING_ENABLED));

        assertEquals(false, imsManager.isWfcProvisionedOnDevice());

        verify(mImsConfigImplBaseMock, times(1)).setConfig(
                eq(ImsConfig.ConfigConstants.VOICE_OVER_WIFI_SETTING_ENABLED),
                eq(0));
        verify(mImsConfigImplBaseMock, times(1)).getConfigInt(
                eq(ImsConfig.ConfigConstants.VOICE_OVER_WIFI_SETTING_ENABLED));

    }

    /**
     * Tests that when WFC is enabled/disabled for home/roaming, that setting is sent to the
     * ImsService correctly.
     *
     * Preconditions:
     *  - CarrierConfigManager.KEY_EDITABLE_WFC_MODE_BOOL = true
     *  - CarrierConfigManager.KEY_CARRIER_DEFAULT_WFC_IMS_ROAMING_ENABLED_BOOL = true
     */
    @Test @SmallTest
    public void testSetWfcSetting_true_shouldSetWfcModeWrtRoamingState() throws Exception {
        // First, Set WFC home/roaming mode that is not the Carrier Config default.
        doReturn(String.valueOf(ImsConfig.WfcModeFeatureValueConstants.WIFI_PREFERRED))
                .when(mSubscriptionController).getSubscriptionProperty(
                        anyInt(),
                        eq(SubscriptionManager.WFC_IMS_MODE),
                        anyString());
        doReturn(String.valueOf(ImsConfig.WfcModeFeatureValueConstants.CELLULAR_PREFERRED))
                .when(mSubscriptionController).getSubscriptionProperty(
                        anyInt(),
                        eq(SubscriptionManager.WFC_IMS_ROAMING_MODE),
                        anyString());
        ImsManager imsManager = getImsManagerAndInitProvisionedValues();

        // Roaming
        doReturn(true).when(mTelephonyManager).isNetworkRoaming(eq(mSubId[0]));
        // Turn on WFC
        imsManager.setWfcSetting(true);
        // Roaming mode (CELLULAR_PREFERRED) should be set.
        verify(mImsConfigImplBaseMock).setConfig(
                eq(ImsConfig.ConfigConstants.VOICE_OVER_WIFI_MODE),
                eq(ImsConfig.WfcModeFeatureValueConstants.CELLULAR_PREFERRED));
        // WFC is enabled, so we should set user roaming setting
        verify(mImsConfigImplBaseMock).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_ROAMING_ENABLED_OVERRIDE),
                eq(ProvisioningManager.PROVISIONING_VALUE_ENABLED));

        // Not roaming
        doReturn(false).when(mTelephonyManager).isNetworkRoaming(eq(mSubId[0]));
        // Turn on WFC
        imsManager.setWfcSetting(true);
        // Home mode (WIFI_PREFERRED) should be set.
        verify(mImsConfigImplBaseMock).setConfig(
                eq(ImsConfig.ConfigConstants.VOICE_OVER_WIFI_MODE),
                eq(ImsConfig.WfcModeFeatureValueConstants.WIFI_PREFERRED));
        // WFC is enabled, so we should set user roaming setting
        verify(mImsConfigImplBaseMock, times(2)).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_ROAMING_ENABLED_OVERRIDE),
                eq(ProvisioningManager.PROVISIONING_VALUE_ENABLED));


        // Turn off WFC and ensure that roaming setting is disabled.
        doReturn(false).when(mTelephonyManager).isNetworkRoaming(eq(mSubId[0]));
        imsManager.setWfcSetting(false);
        verify(mImsConfigImplBaseMock).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_ROAMING_ENABLED_OVERRIDE),
                eq(ProvisioningManager.PROVISIONING_VALUE_DISABLED));
    }


    /**
     * Tests that when user changed WFC setting while NOT roaming, the home WFC mode is sent to the
     * modem and the roaming enabled configuration is pushed.
     *
     * Preconditions:
     *  - CarrierConfigManager.KEY_EDITABLE_WFC_MODE_BOOL = true
     *  - CarrierConfigManager.KEY_CARRIER_DEFAULT_WFC_IMS_ROAMING_ENABLED_BOOL = true
     */
    @Test @SmallTest
    public void testSetWfcSetting_shouldSetWfcModeRoamingDisabledUserEnabled() throws Exception {
        // The user has previously enabled "WFC while roaming" setting in UI and then turned WFC
        // off.
        doReturn(String.valueOf(1 /*true*/))
                .when(mSubscriptionController).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_ROAMING_ENABLED),
                anyString());

        ImsManager imsManager = getImsManagerAndInitProvisionedValues();

        // We are currently on the home network, not roaming.
        doReturn(false).when(mTelephonyManager).isNetworkRoaming(eq(mSubId[0]));

        // User enables WFC from UI
        imsManager.setWfcSetting(true /*enabled*/);
        verify(mImsConfigImplBaseMock).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_MODE_OVERRIDE),
                eq(ImsMmTelManager.WIFI_MODE_CELLULAR_PREFERRED));
        verify(mImsConfigImplBaseMock).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_ROAMING_ENABLED_OVERRIDE),
                // Should be enabled because the user enabled the "WFC while roaming" setting
                // independent of whether or not we are roaming.
                eq(ProvisioningManager.PROVISIONING_VALUE_ENABLED));
    }

    /**
     * Tests that when user changed WFC setting while roaming, that the correct user setting
     * is sent to the ImsService when changing the roaming mode.
     *
     * Preconditions:
     *  - CarrierConfigManager.KEY_EDITABLE_WFC_MODE_BOOL = true
     *  - CarrierConfigManager.KEY_CARRIER_DEFAULT_WFC_IMS_ROAMING_ENABLED_BOOL = true
     */
    @Test @SmallTest
    public void testSetWfcSetting_shouldSetWfcModeRoamingEnabledUserEnabled() throws Exception {
        // The user has previously enabled "WFC while roaming" setting in UI and then turned WFC
        // off.
        doReturn(String.valueOf(1 /*true*/))
                .when(mSubscriptionController).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_ROAMING_ENABLED),
                anyString());

        ImsManager imsManager = getImsManagerAndInitProvisionedValues();

        //  The device is currently roaming
        doReturn(true).when(mTelephonyManager).isNetworkRoaming(eq(mSubId[0]));

        // The user has enabled WFC in the UI while the device is roaming.
        imsManager.setWfcSetting(true /*enabled*/);

        verify(mImsConfigImplBaseMock).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_MODE_OVERRIDE),
                // Default for roaming is WFC_IMS_ROAMING_MODE_DEFAULT_VAL
                eq(ImsMmTelManager.WIFI_MODE_WIFI_PREFERRED));
        verify(mImsConfigImplBaseMock).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_ROAMING_ENABLED_OVERRIDE),
                // Should be enabled because user enabled the setting in the UI previously.
                eq(ProvisioningManager.PROVISIONING_VALUE_ENABLED));
    }

    /**
     * Tests that when a WFC mode is updated for home, that setting is sent to the
     * ImsService correctly or ignored if the roaming mode is changed.
     *
     * Preconditions:
     *  - CarrierConfigManager.KEY_EDITABLE_WFC_MODE_BOOL = true
     *  - CarrierConfigManager.KEY_CARRIER_DEFAULT_WFC_IMS_ROAMING_ENABLED_BOOL = true
     */
    @Test @SmallTest
    public void testSetWfcMode_shouldSetWfcModeRoamingDisabled() throws Exception {
        ImsManager imsManager = getImsManagerAndInitProvisionedValues();

        // the device is not currently roaming
        doReturn(false).when(mTelephonyManager).isNetworkRoaming(eq(mSubId[0]));

        // set the WFC roaming mode while the device is not roaming, so any changes to roaming mode
        // should be ignored
        imsManager.setWfcMode(ImsMmTelManager.WIFI_MODE_CELLULAR_PREFERRED, true /*IsRoaming*/);
        verify(mImsConfigImplBaseMock, never()).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_MODE_OVERRIDE),
                anyInt());
        verify(mImsConfigImplBaseMock, never()).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_ROAMING_ENABLED_OVERRIDE),
                anyInt());

        // set home WFC mode setting while not roaming, the configuration should be set correctly.
        imsManager.setWfcMode(ImsMmTelManager.WIFI_MODE_CELLULAR_PREFERRED, false /*IsRoaming*/);
        verify(mImsConfigImplBaseMock).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_MODE_OVERRIDE),
                eq(ImsMmTelManager.WIFI_MODE_CELLULAR_PREFERRED));
        // WiFi Roaming enabled setting is not related to WFC mode
        verify(mImsConfigImplBaseMock, never()).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_ROAMING_ENABLED_OVERRIDE),
                anyInt());
    }

    /**
     * Tests that when a WFC mode is updated for roaming while WFC is enabled, that setting is sent
     * to the ImsService correctly when changing the roaming mode or ignored if the home setting is
     * changed.
     *
     * Preconditions:
     *  - CarrierConfigManager.KEY_EDITABLE_WFC_MODE_BOOL = true
     *  - CarrierConfigManager.KEY_CARRIER_DEFAULT_WFC_IMS_ROAMING_ENABLED_BOOL = true
     */
    @Test @SmallTest
    public void testSetWfcMode_wfcEnabledShouldSetWfcModeRoamingEnabled() throws Exception {
        ImsManager imsManager = getImsManagerAndInitProvisionedValues();

        // The user has previously enabled WFC in the settings UI.
        doReturn(String.valueOf(1 /*true*/))
                .when(mSubscriptionController).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_ENABLED),
                anyString());

        // The device is roaming
        doReturn(true).when(mTelephonyManager).isNetworkRoaming(eq(mSubId[0]));

        // The carrier app has changed the WFC mode for roaming while the device is home. The
        // result of this operation is that the neither the WFC mode or the roaming enabled
        // configuration should change.
        imsManager.setWfcMode(ImsMmTelManager.WIFI_MODE_CELLULAR_PREFERRED, false /*IsRoaming*/);
        verify(mImsConfigImplBaseMock, never()).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_MODE_OVERRIDE),
                anyInt());
        verify(mImsConfigImplBaseMock, never()).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_ROAMING_ENABLED_OVERRIDE),
                anyInt());

        //  The carrier app has set the WFC mode for roaming while the device is roaming. The
        // WFC mode should be updated to reflect the roaming setting and the roaming enabled
        // configuration should be changed to enabled.
        imsManager.setWfcMode(ImsMmTelManager.WIFI_MODE_CELLULAR_PREFERRED, true /*IsRoaming*/);
        verify(mImsConfigImplBaseMock).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_MODE_OVERRIDE),
                eq(ImsMmTelManager.WIFI_MODE_CELLULAR_PREFERRED));
        // WiFi Roaming enabled setting is not related to WFC mode
        verify(mImsConfigImplBaseMock, never()).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_ROAMING_ENABLED_OVERRIDE),
                anyInt());
    }

    /**
     * Tests that when a WFC mode is updated for roaming while WFC is disabled, the WFC roaming
     * setting is always set to disabled.
     *
     * Preconditions:
     *  - CarrierConfigManager.KEY_EDITABLE_WFC_MODE_BOOL = true
     *  - CarrierConfigManager.KEY_CARRIER_DEFAULT_WFC_IMS_ROAMING_ENABLED_BOOL = true
     */
    @Test @SmallTest
    public void testSetWfcMode_WfcDisabledShouldNotSetWfcModeRoamingEnabled() throws Exception {
        ImsManager imsManager = getImsManagerAndInitProvisionedValues();

        // The user has previously disabled WFC in the settings UI.
        doReturn(String.valueOf(0 /*false*/))
                .when(mSubscriptionController).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_ENABLED),
                anyString());

        // The device is roaming
        doReturn(true).when(mTelephonyManager).isNetworkRoaming(eq(mSubId[0]));

        // WFC is disabled and the carrier app has set the WFC mode for roaming while the device is
        // roaming. The WFC mode should be updated to reflect the roaming setting and the roaming
        // enabled configuration should be disabled because WFC is disabled.
        imsManager.setWfcMode(ImsMmTelManager.WIFI_MODE_CELLULAR_PREFERRED, true /*IsRoaming*/);
        verify(mImsConfigImplBaseMock).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_MODE_OVERRIDE),
                eq(ImsMmTelManager.WIFI_MODE_CELLULAR_PREFERRED));
        // WiFi Roaming enabled setting is not related to WFC mode
        verify(mImsConfigImplBaseMock, never()).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_ROAMING_ENABLED_OVERRIDE),
                anyInt());
    }

    /**
     * Tests that when user changed WFC mode while not roaming, the new mode is sent to the modem
     * and roaming enabled indication is sent to the ImsService correctly when changing the roaming
     * mode.
     *
     * Preconditions:
     *  - CarrierConfigManager.KEY_EDITABLE_WFC_MODE_BOOL = true
     *  - CarrierConfigManager.KEY_CARRIER_DEFAULT_WFC_IMS_ROAMING_ENABLED_BOOL = true
     */
    @Test @SmallTest
    public void testSetWfcMode_shouldSetWfcModeRoamingDisabledUserEnabled() throws Exception {
        // The user has enabled the WFC setting in the UI.
        doReturn(String.valueOf(1 /*true*/))
                .when(mSubscriptionController).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_ENABLED),
                anyString());
        // The user has enabled the "WFC while roaming" setting in the UI while WFC was enabled
        doReturn(String.valueOf(1 /*true*/))
                .when(mSubscriptionController).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_ROAMING_ENABLED),
                anyString());

        ImsManager imsManager = getImsManagerAndInitProvisionedValues();

        // The device is currently on the home network
        doReturn(false).when(mTelephonyManager).isNetworkRoaming(eq(mSubId[0]));

        // The user has changed the WFC mode in the UI for the non-roaming configuration
        imsManager.setWfcMode(ImsMmTelManager.WIFI_MODE_CELLULAR_PREFERRED, false /*IsRoaming*/);
        verify(mImsConfigImplBaseMock).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_MODE_OVERRIDE),
                // ensure that the correct cellular preferred config change is sent
                eq(ImsMmTelManager.WIFI_MODE_CELLULAR_PREFERRED));
        // WiFi Roaming enabled setting is not related to WFC mode
        verify(mImsConfigImplBaseMock, never()).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_ROAMING_ENABLED_OVERRIDE),
                anyInt());
    }

    /**
     * Tests that when user changed WFC mode while roaming, that setting is sent to the
     * ImsService correctly when changing the roaming mode.
     *
     * Preconditions:
     *  - CarrierConfigManager.KEY_EDITABLE_WFC_MODE_BOOL = true
     *  - CarrierConfigManager.KEY_CARRIER_DEFAULT_WFC_IMS_ROAMING_ENABLED_BOOL = true
     */
    @Test @SmallTest
    public void testSetWfcMode_shouldSetWfcModeRoamingEnabledUserDisabled() throws Exception {
        // The user disabled "WFC while roaming" setting in the UI
        doReturn(String.valueOf(0 /*false*/))
                .when(mSubscriptionController).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_ROAMING_ENABLED),
                anyString());

        ImsManager imsManager = getImsManagerAndInitProvisionedValues();

        // the device is currently roaming
        doReturn(true).when(mTelephonyManager).isNetworkRoaming(eq(mSubId[0]));

        // The carrier app has changed the WFC mode while roaming, so we must set the WFC mode
        // to the new configuration.
        imsManager.setWfcMode(ImsMmTelManager.WIFI_MODE_CELLULAR_PREFERRED, true /*IsRoaming*/);
        verify(mImsConfigImplBaseMock).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_MODE_OVERRIDE),
                eq(ImsMmTelManager.WIFI_MODE_CELLULAR_PREFERRED));
        // WiFi Roaming enabled setting is not related to WFC mode
        verify(mImsConfigImplBaseMock, never()).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_ROAMING_ENABLED_OVERRIDE),
                anyInt());
    }

    /**
     * Tests that the settings for WFC mode are ignored if the Carrier sets the settings to not
     * editable.
     *
     * Preconditions:
     *  - CarrierConfigManager.KEY_EDITABLE_WFC_MODE_BOOL = false
     */
    @Test @SmallTest
    public void testSetWfcSetting_wfcNotEditable() throws Exception {
        mBundle.putBoolean(CarrierConfigManager.KEY_EDITABLE_WFC_MODE_BOOL,
                WFC_IMS_NOT_EDITABLE_VAL);
        // Set some values that are different than the defaults for WFC mode.
        doReturn(String.valueOf(ImsConfig.WfcModeFeatureValueConstants.WIFI_ONLY))
                .when(mSubscriptionController).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_MODE),
                anyString());
        doReturn(String.valueOf(ImsConfig.WfcModeFeatureValueConstants.WIFI_ONLY))
                .when(mSubscriptionController).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_ROAMING_MODE),
                anyString());
        ImsManager imsManager = getImsManagerAndInitProvisionedValues();

        // Roaming
        doReturn(true).when(mTelephonyManager).isNetworkRoaming(eq(mSubId[0]));
        // Turn on WFC
        imsManager.setWfcSetting(true);
        // User defined setting for Roaming mode (WIFI_ONLY) should be set independent of whether or
        // not WFC mode is editable. With 1000 ms timeout.
        verify(mImsConfigImplBaseMock).setConfig(
                eq(ImsConfig.ConfigConstants.VOICE_OVER_WIFI_MODE),
                eq(ImsConfig.WfcModeFeatureValueConstants.WIFI_ONLY));

        // Not roaming
        doReturn(false).when(mTelephonyManager).isNetworkRoaming(eq(mSubId[0]));
        // Turn on WFC
        imsManager.setWfcSetting(true);
        // Default Home mode (CELLULAR_PREFERRED) should be set. With 1000 ms timeout.
        verify(mImsConfigImplBaseMock).setConfig(
                eq(ImsConfig.ConfigConstants.VOICE_OVER_WIFI_MODE),
                eq(WFC_IMS_MODE_DEFAULT_VAL));
    }

    /**
     * Tests that the CarrierConfig defaults will be used if no setting is set in the Subscription
     * Manager.
     *
     * Preconditions:
     *  - CarrierConfigManager.KEY_EDITABLE_WFC_MODE_BOOL = true
     *  - CarrierConfigManager.KEY_CARRIER_DEFAULT_WFC_IMS_MODE_INT = Carrier preferred
     *  - CarrierConfigManager.KEY_CARRIER_DEFAULT_WFC_IMS_ROAMING_MODE_INT = WiFi preferred
     */
    @Test @SmallTest
    public void testSetWfcSetting_noUserSettingSet() throws Exception {
        ImsManager imsManager = getImsManagerAndInitProvisionedValues();

        // Roaming
        doReturn(true).when(mTelephonyManager).isNetworkRoaming(eq(mSubId[0]));
        // Turn on WFC
        imsManager.setWfcSetting(true);

        // Default Roaming mode (WIFI_PREFERRED) for carrier should be set. With 1000 ms timeout.
        verify(mImsConfigImplBaseMock).setConfig(
                eq(ImsConfig.ConfigConstants.VOICE_OVER_WIFI_MODE),
                eq(WFC_IMS_ROAMING_MODE_DEFAULT_VAL));

        // Not roaming
        doReturn(false).when(mTelephonyManager).isNetworkRoaming(eq(mSubId[0]));
        // Turn on WFC
        imsManager.setWfcSetting(true);

        // Default Home mode (CELLULAR_PREFERRED) for carrier should be set. With 1000 ms timeout.
        verify(mImsConfigImplBaseMock).setConfig(
                eq(ImsConfig.ConfigConstants.VOICE_OVER_WIFI_MODE),
                eq(WFC_IMS_MODE_DEFAULT_VAL));
    }

    /**
     * Tests the operation of getWfcMode when the configuration to use the home network mode when
     * roaming for WFC is false. First, it checks that the user setting for WFC_IMS_ROAMING_MODE is
     * returned when WFC roaming is set to editable. Then, it switches the WFC roaming mode to not
     * editable and ensures that the default WFC roaming mode is returned.
     *
     * Preconditions:
     *  - CarrierConfigManager.KEY_USE_WFC_HOME_NETWORK_MODE_IN_ROAMING_NETWORK_BOOL = false
     */
    @Test @SmallTest
    public void getWfcMode_useWfcHomeModeConfigFalse_shouldUseWfcRoamingMode() {
        // Set some values that are different than the defaults for WFC mode.
        doReturn(String.valueOf(ImsConfig.WfcModeFeatureValueConstants.WIFI_ONLY))
                .when(mSubscriptionController).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_MODE),
                anyString());
        doReturn(String.valueOf(ImsConfig.WfcModeFeatureValueConstants.CELLULAR_PREFERRED))
                .when(mSubscriptionController).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_ROAMING_MODE),
                anyString());

        ImsManager imsManager = getImsManagerAndInitProvisionedValues();

        // Check that use the WFC roaming network mode.
        assertEquals(ImsConfig.WfcModeFeatureValueConstants.CELLULAR_PREFERRED,
                imsManager.getWfcMode(true));
        verify(mSubscriptionController, times(1)).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_ROAMING_MODE),
                anyString());

        // Set WFC roaming network mode to not editable.
        mBundle.putBoolean(CarrierConfigManager.KEY_EDITABLE_WFC_ROAMING_MODE_BOOL,
                WFC_IMS_ROAMING_NOT_EDITABLE_VAL);

        // Check that use the default WFC roaming network mode.
        assertEquals(WFC_IMS_ROAMING_MODE_DEFAULT_VAL, imsManager.getWfcMode(true));
        verify(mSubscriptionController, times(1)).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_ROAMING_MODE),
                anyString());
    }

    /**
     * Tests the operation of getWfcMode when the configuration to use the home network mode when
     * roaming for WFC is true independent of whether or not the WFC roaming mode is editable.
     *
     * Preconditions:
     *  - CarrierConfigManager.KEY_USE_WFC_HOME_NETWORK_MODE_IN_ROAMING_NETWORK_BOOL = true
     */
    @Test @SmallTest
    public void getWfcMode_useWfcHomeModeConfigTrue_shouldUseWfcHomeMode() {
        // Set some values that are different than the defaults for WFC mode.
        doReturn(String.valueOf(ImsConfig.WfcModeFeatureValueConstants.WIFI_ONLY))
                .when(mSubscriptionController).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_MODE),
                anyString());
        doReturn(String.valueOf(ImsConfig.WfcModeFeatureValueConstants.CELLULAR_PREFERRED))
                .when(mSubscriptionController).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_ROAMING_MODE),
                anyString());

        // Set to use WFC home network mode in roaming network.
        mBundle.putBoolean(
                CarrierConfigManager.KEY_USE_WFC_HOME_NETWORK_MODE_IN_ROAMING_NETWORK_BOOL,
                WFC_USE_HOME_MODE_FOR_ROAMING_VAL);

        ImsManager imsManager = getImsManagerAndInitProvisionedValues();

        // Check that use the WFC home network mode.
        assertEquals(ImsConfig.WfcModeFeatureValueConstants.WIFI_ONLY, imsManager.getWfcMode(true));
        verify(mSubscriptionController, times(1)).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_MODE),
                anyString());

        // Set WFC home network mode to not editable.
        mBundle.putBoolean(CarrierConfigManager.KEY_EDITABLE_WFC_MODE_BOOL,
                WFC_IMS_NOT_EDITABLE_VAL);

        // Check that use the default WFC home network mode.
        assertEquals(WFC_IMS_MODE_DEFAULT_VAL, imsManager.getWfcMode(true));
        verify(mSubscriptionController, times(1)).getSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_MODE),
                anyString());
    }

    /**
     * Tests the operation of setWfcRoamingSetting and ensures that the user setting for WFC roaming
     * and the ImsConfig setting are both called properly.
     */
    @Test @SmallTest
    public void setWfcRoamingSettingTest() {
        ImsManager imsManager = getImsManagerAndInitProvisionedValues();

        imsManager.setWfcRoamingSetting(true);
        verify(mSubscriptionController, times(1)).setSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_ROAMING_ENABLED),
                eq("1"));
        verify(mImsConfigImplBaseMock).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_ROAMING_ENABLED_OVERRIDE),
                eq(ProvisioningManager.PROVISIONING_VALUE_ENABLED));

        imsManager.setWfcRoamingSetting(false);
        verify(mSubscriptionController, times(1)).setSubscriptionProperty(
                anyInt(),
                eq(SubscriptionManager.WFC_IMS_ROAMING_ENABLED),
                eq("0"));
        verify(mImsConfigImplBaseMock).setConfig(
                eq(ProvisioningManager.KEY_VOICE_OVER_WIFI_ROAMING_ENABLED_OVERRIDE),
                eq(ProvisioningManager.PROVISIONING_VALUE_DISABLED));

    }

    private ImsManager getImsManagerAndInitProvisionedValues() {
        when(mImsConfigImplBaseMock.getConfigInt(anyInt()))
                .thenAnswer(invocation ->  {
                    return getProvisionedInt((Integer) (invocation.getArguments()[0]));
                });

        when(mImsConfigImplBaseMock.setConfig(anyInt(), anyInt()))
                .thenAnswer(invocation ->  {
                    mProvisionedIntVals.put((Integer) (invocation.getArguments()[0]),
                            (Integer) (invocation.getArguments()[1]));
                    return ImsConfig.OperationStatusConstants.SUCCESS;
                });


        // Configure ImsConfigStub
        mImsConfigStub = new ImsConfigImplBase.ImsConfigStub(mImsConfigImplBaseMock);
        doReturn(mImsConfigStub).when(mMmTelFeatureConnection).getConfigInterface();

        // Configure ImsManager
        ImsManager imsManager = ImsManager.getInstance(mContext, mPhoneId);
        try {
            replaceInstance(ImsManager.class, "mMmTelFeatureConnection", imsManager,
                    mMmTelFeatureConnection);
        } catch (Exception ex) {
            fail("failed with " + ex);
        }

        imsManager.mExecutorFactory = Runnable::run;

        return imsManager;
    }

    // If the value is ever set, return the set value. If not, return a constant value 1000.
    private int getProvisionedInt(int item) {
        if (mProvisionedIntVals.containsKey(item)) {
            return mProvisionedIntVals.get(item);
        } else {
            return ImsConfig.FeatureValueConstants.ON;
        }
    }

    // If the value is ever set, return the set value. If not, return a constant value "unset".
    private String getProvisionedString(int item) {
        if (mProvisionedStringVals.containsKey(item)) {
            return mProvisionedStringVals.get(item);
        } else {
            return UNSET_PROVISIONED_STRING;
        }
    }
}
