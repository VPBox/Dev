/*
 * Copyright (C) 2019 The Android Open Source Project
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

import static com.google.common.truth.Truth.assertThat;

import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.Mockito.doReturn;

import android.content.Context;
import android.os.PersistableBundle;
import android.telephony.CarrierConfigManager;
import android.telephony.ServiceState;

import com.android.internal.telephony.cdnr.CarrierDisplayNameData;
import com.android.internal.telephony.cdnr.CarrierDisplayNameResolver;
import com.android.internal.telephony.uicc.IccCardApplicationStatus.AppState;
import com.android.internal.telephony.uicc.RuimRecords;
import com.android.internal.telephony.uicc.SIMRecords;
import com.android.internal.telephony.uicc.UiccCardApplication;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mockito;

public class CarrierDisplayNameResolverTest extends TelephonyTest {

    private static final String PLMN_1 = "310260";
    private static final String PLMN_2 = "480123";
    private static final String PLMN_3 = "586111";
    private static final String HOME_PLMN_NUMERIC = PLMN_1;
    private static final String NON_HOME_PLMN_NUMERIC = "123456";

    private static final String SPN_FROM_CC = "spn from carrier config";

    /** No PLMN in home, not SPN in roaming. */
    private static final int SPN_DISPLAY_CONDITION_FROM_USIM = 0;
    private static final String SPN_FROM_USIM = "spn from usim";
    private static final String PNN_HOME_NAME_FROM_USIM = "pnnHomeName";
    private static final String[] SPDI_FROM_USIM = new String[] { PLMN_1, PLMN_2 };
    private static final String[] EHPLMNS_FROM_USIM = new String[] {
            PLMN_1, PLMN_2, PLMN_3
    };

    private static final boolean ROAMING = true;
    private static final boolean NON_ROAMING = false;

    private PersistableBundle mConfig;

    private CarrierDisplayNameResolver mCdnr;

    private final ServiceState mSS = new ServiceState();

    @Before
    public void setUp() throws Exception {
        super.setUp("CDNRTest");

        mCdnr = new CarrierDisplayNameResolver(mPhone);

        UiccCardApplication uiccApp = Mockito.mock(UiccCardApplication.class);
        doReturn(uiccApp).when(mPhone).getUiccCardApplication();
        doReturn(AppState.APPSTATE_READY).when(uiccApp).getState();
        doReturn(mSS).when(mSST).getServiceState();

        doReturn(false).when(mPhone).isWifiCallingEnabled();
        doReturn(true).when(mPhone).isPhoneTypeGsm();

        mConfig = mContextFixture.getCarrierConfigBundle();
        CarrierConfigManager mockConfigManager = Mockito.mock(CarrierConfigManager.class);
        doReturn(mockConfigManager).when(mContext).getSystemService(Context.CARRIER_CONFIG_SERVICE);
        doReturn(mConfig).when(mockConfigManager).getConfigForSubId(anyInt());

        mSS.setEmergencyOnly(false /* emergencyCallOnly" */);
        mSS.setOperatorName("long name", "short name", HOME_PLMN_NUMERIC);
        mSS.setVoiceRegState(ServiceState.STATE_IN_SERVICE);
        mSS.setDataRegState(ServiceState.STATE_IN_SERVICE);

        SIMRecords usim = Mockito.mock(SIMRecords.class);
        doReturn(SPN_FROM_USIM).when(usim).getServiceProviderName();
        doReturn(PNN_HOME_NAME_FROM_USIM).when(usim).getPnnHomeName();
        doReturn(EHPLMNS_FROM_USIM).when(usim).getEhplmns();
        doReturn(SPDI_FROM_USIM).when(usim).getServiceProviderDisplayInformation();
        doReturn(SPN_DISPLAY_CONDITION_FROM_USIM).when(usim).getCarrierNameDisplayCondition();

        mCdnr.updateEfFromUsim(usim);
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
    }

    @Test
    public void testUpdateSPNFromHigherPrioritySource_shouldOverrideRecord() {
        // Carrier config source > sim record source
        mConfig.putString(CarrierConfigManager.KEY_CARRIER_NAME_STRING, SPN_FROM_CC);

        // Update ef records from carrier config
        mCdnr.updateEfFromCarrierConfig(mConfig);

        CarrierDisplayNameData data = mCdnr.getCarrierDisplayNameData();
        assertThat(data.getSpn()).isEqualTo(SPN_FROM_CC);
        assertThat(data.shouldShowSpn()).isTrue();
    }

    @Test
    public void testUpdateSPNFromLowerPrioritySource_shouldNotOverrideRecord() {
        // Ruim's priority < Usim's priority
        RuimRecords ruim = Mockito.mock(RuimRecords.class);
        doReturn("spn from ruim").when(ruim).getServiceProviderName();

        // Update ef records from Ruim
        CarrierDisplayNameData data = mCdnr.getCarrierDisplayNameData();
        assertThat(data.getSpn()).isEqualTo(SPN_FROM_USIM);
    }

    @Test
    public void testShouldShowSPN_nonRoaming_showSPN() {
        mSS.setRoaming(NON_ROAMING);

        CarrierDisplayNameData data = mCdnr.getCarrierDisplayNameData();
        assertThat(data.shouldShowSpn()).isTrue();
    }

    @Test
    public void testShouldShowSPN_plmnNotInProvidedList_notShowSPN() {
        mSS.setOperatorName("long", "short", NON_HOME_PLMN_NUMERIC);
        mSS.setRoaming(ROAMING);

        CarrierDisplayNameData data = mCdnr.getCarrierDisplayNameData();
        assertThat(data.shouldShowSpn()).isFalse();
    }

    @Test
    public void testShouldShowSPN_plmnInProvidedList_showSPN() {
        mSS.setOperatorName("long", "short", SPDI_FROM_USIM[0]);
        mSS.setRoaming(ROAMING);

        CarrierDisplayNameData data = mCdnr.getCarrierDisplayNameData();
        assertThat(data.shouldShowSpn()).isTrue();
    }

    @Test
    public void testShouldShowPLMNNetworkName_plmnNotInProvidedList_showPLMNNetworkName() {
        mSS.setOperatorName("long", "short", NON_HOME_PLMN_NUMERIC);
        mSS.setRoaming(ROAMING);

        CarrierDisplayNameData data = mCdnr.getCarrierDisplayNameData();
        assertThat(data.shouldShowPlmn()).isTrue();
    }

    @Test
    public void testGetPLMNNetworkName_oplNotPresent_returnTheFirstEntryOfPNNList() {
        // Set the roaming state to on roaming, we should show the plmn network name based on the
        // default settings.
        mSS.setRoaming(ROAMING);

        CarrierDisplayNameData data = mCdnr.getCarrierDisplayNameData();
        assertThat(data.getPlmn()).isEqualTo(PNN_HOME_NAME_FROM_USIM);
    }
}
