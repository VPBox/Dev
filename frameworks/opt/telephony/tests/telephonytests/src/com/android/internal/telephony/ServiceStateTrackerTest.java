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

package com.android.internal.telephony;

import static com.android.internal.telephony.TelephonyTestUtils.waitForMs;

import static com.google.common.truth.Truth.assertThat;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyLong;
import static org.mockito.Matchers.nullable;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.anyString;
import static org.mockito.Mockito.atLeast;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.spy;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.app.IAlarmManager;
import android.app.Notification;
import android.app.NotificationManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ApplicationInfo;
import android.content.pm.ServiceInfo;
import android.content.res.Resources;
import android.graphics.drawable.Drawable;
import android.os.AsyncResult;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.os.PersistableBundle;
import android.os.Process;
import android.os.SystemClock;
import android.os.UserHandle;
import android.os.WorkSource;
import android.telephony.AccessNetworkConstants;
import android.telephony.CarrierConfigManager;
import android.telephony.CellIdentity;
import android.telephony.CellIdentityCdma;
import android.telephony.CellIdentityGsm;
import android.telephony.CellIdentityLte;
import android.telephony.CellInfo;
import android.telephony.CellInfoGsm;
import android.telephony.CellSignalStrength;
import android.telephony.CellSignalStrengthCdma;
import android.telephony.CellSignalStrengthGsm;
import android.telephony.CellSignalStrengthLte;
import android.telephony.CellSignalStrengthNr;
import android.telephony.CellSignalStrengthTdscdma;
import android.telephony.CellSignalStrengthWcdma;
import android.telephony.INetworkService;
import android.telephony.LteVopsSupportInfo;
import android.telephony.NetworkRegistrationInfo;
import android.telephony.NetworkService;
import android.telephony.PhysicalChannelConfig;
import android.telephony.ServiceState;
import android.telephony.SignalStrength;
import android.telephony.SubscriptionInfo;
import android.telephony.SubscriptionManager;
import android.telephony.TelephonyManager;
import android.telephony.cdma.CdmaCellLocation;
import android.telephony.gsm.GsmCellLocation;
import android.test.suitebuilder.annotation.MediumTest;
import android.test.suitebuilder.annotation.SmallTest;
import android.util.Pair;
import android.util.TimestampedValue;

import androidx.test.filters.FlakyTest;

import com.android.internal.R;
import com.android.internal.telephony.cdma.CdmaSubscriptionSourceManager;
import com.android.internal.telephony.test.SimulatedCommands;
import com.android.internal.telephony.uicc.IccCardApplicationStatus;
import com.android.internal.telephony.uicc.IccRecords;

import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.Mockito;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;

public class ServiceStateTrackerTest extends TelephonyTest {
    @Mock
    private ProxyController mProxyController;
    @Mock
    private Handler mTestHandler;
    @Mock
    protected IAlarmManager mAlarmManager;

    private CellularNetworkService mCellularNetworkService;

    @Mock
    private NetworkService mIwlanNetworkService;
    @Mock
    private INetworkService.Stub mIwlanNetworkServiceStub;

    @Mock
    private SubscriptionInfo mSubInfo;

    private ServiceStateTracker sst;
    private ServiceStateTrackerTestHandler mSSTTestHandler;
    private PersistableBundle mBundle;

    private static final int EVENT_REGISTERED_TO_NETWORK = 1;
    private static final int EVENT_SUBSCRIPTION_INFO_READY = 2;
    private static final int EVENT_DATA_ROAMING_ON = 3;
    private static final int EVENT_DATA_ROAMING_OFF = 4;
    private static final int EVENT_DATA_CONNECTION_ATTACHED = 5;
    private static final int EVENT_DATA_CONNECTION_DETACHED = 6;
    private static final int EVENT_DATA_RAT_CHANGED = 7;
    private static final int EVENT_PS_RESTRICT_ENABLED = 8;
    private static final int EVENT_PS_RESTRICT_DISABLED = 9;
    private static final int EVENT_VOICE_ROAMING_ON = 10;
    private static final int EVENT_VOICE_ROAMING_OFF = 11;
    private static final int EVENT_VOICE_RAT_CHANGED = 12;

    private static final int PHONE_ID = 0;

    private static final String CARRIER_NAME_DISPLAY_NO_SERVICE = "no service";
    private static final String CARRIER_NAME_DISPLAY_EMERGENCY_CALL = "emergency call";
    private static final String WIFI_CALLING_VOICE_FORMAT = "%s wifi calling";
    private static final String WIFI_CALLING_DATA_FORMAT = "%s wifi data";
    private static final String WIFI_CALLING_FLIGHT_MODE_FORMAT = "%s flight mode";

    private static final String[] WIFI_CALLING_FORMATTERS = {
            WIFI_CALLING_VOICE_FORMAT,
            WIFI_CALLING_DATA_FORMAT,
            WIFI_CALLING_FLIGHT_MODE_FORMAT };

    private static final String HOME_PLMN = "310260";
    private static final String PLMN1 = "480123";
    private static final String PLMN2 = "586111";
    private static final String HOME_PNN = "home pnn";
    private static final String[] CARRIER_CONFIG_SPDI = new String[] {HOME_PLMN, PLMN2};
    private static final String[] CARRIER_CONFIG_EHPLMN = new String[] {HOME_PLMN, PLMN1};
    private static final String[] CARRIER_CONFIG_PNN = new String[] {
            String.format("%s,%s", HOME_PNN, "short"), "f2,s2"
    };

    private class ServiceStateTrackerTestHandler extends HandlerThread {

        private ServiceStateTrackerTestHandler(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            sst = new ServiceStateTracker(mPhone, mSimulatedCommands);
            setReady(true);
        }
    }

    private void addNetworkService() {
        mCellularNetworkService = new CellularNetworkService();
        ServiceInfo CellularServiceInfo = new ServiceInfo();
        CellularServiceInfo.packageName = "com.android.phone";
        CellularServiceInfo.name = "CellularNetworkService";
        CellularServiceInfo.permission = "android.permission.BIND_TELEPHONY_NETWORK_SERVICE";
        IntentFilter cellularIntentfilter = new IntentFilter();
        mContextFixture.addService(
                NetworkService.SERVICE_INTERFACE,
                new ComponentName("com.android.phone",
                        "com.android.internal.telephony.CellularNetworkService"),
                "com.android.phone",
                mCellularNetworkService.mBinder,
                CellularServiceInfo,
                cellularIntentfilter);

        ServiceInfo iwlanServiceInfo = new ServiceInfo();
        iwlanServiceInfo.packageName = "com.xyz.iwlan.networkservice";
        iwlanServiceInfo.name = "IwlanNetworkService";
        iwlanServiceInfo.permission = "android.permission.BIND_TELEPHONY_NETWORK_SERVICE";
        IntentFilter iwlanIntentFilter = new IntentFilter();
        mContextFixture.addService(
                NetworkService.SERVICE_INTERFACE,
                new ComponentName("com.xyz.iwlan.networkservice",
                        "com.xyz.iwlan.IwlanNetworkService"),
                "com.xyz.iwlan.networkservice",
                mIwlanNetworkServiceStub,
                iwlanServiceInfo,
                iwlanIntentFilter);
    }

    @Before
    public void setUp() throws Exception {

        logd("ServiceStateTrackerTest +Setup!");
        super.setUp("ServiceStateTrackerTest");

        mContextFixture.putResource(R.string.config_wwan_network_service_package,
                "com.android.phone");
        mContextFixture.putResource(R.string.config_wlan_network_service_package,
                "com.xyz.iwlan.networkservice");
        doReturn(mIwlanNetworkServiceStub).when(mIwlanNetworkServiceStub).asBinder();
        addNetworkService();

        doReturn(true).when(mDcTracker).isDisconnected();

        doReturn(new ServiceState()).when(mPhone).getServiceState();

        replaceInstance(ProxyController.class, "sProxyController", null, mProxyController);
        mBundle = mContextFixture.getCarrierConfigBundle();
        mBundle.putStringArray(
                CarrierConfigManager.KEY_ROAMING_OPERATOR_STRING_ARRAY, new String[]{"123456"});

        mBundle.putStringArray(
                CarrierConfigManager.KEY_NON_ROAMING_OPERATOR_STRING_ARRAY, new String[]{"123456"});

        mBundle.putStringArray(CarrierConfigManager.KEY_RATCHET_RAT_FAMILIES,
                // UMTS < GPRS < EDGE
                new String[]{"3,1,2"});

        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME);
        mSimulatedCommands.setVoiceRadioTech(ServiceState.RIL_RADIO_TECHNOLOGY_HSPA);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME);
        mSimulatedCommands.setDataRadioTech(ServiceState.RIL_RADIO_TECHNOLOGY_HSPA);

        doReturn(PHONE_ID).when(mPhone).getPhoneId();

        int dds = SubscriptionManager.getDefaultDataSubscriptionId();
        doReturn(dds).when(mPhone).getSubId();
        doReturn(true).when(mPhone).areAllDataDisconnected();

        mSSTTestHandler = new ServiceStateTrackerTestHandler(getClass().getSimpleName());
        mSSTTestHandler.start();
        waitUntilReady();
        waitForMs(600);

        Intent intent = new Intent(CarrierConfigManager.ACTION_CARRIER_CONFIG_CHANGED);
        intent.putExtra(CarrierConfigManager.EXTRA_SLOT_INDEX, 0);
        mContext.sendBroadcast(intent);
        waitForMs(100);

        // Override SPN related resource
        mContextFixture.putResource(
                com.android.internal.R.string.lockscreen_carrier_default,
                CARRIER_NAME_DISPLAY_NO_SERVICE);
        mContextFixture.putResource(
                com.android.internal.R.string.emergency_calls_only,
                CARRIER_NAME_DISPLAY_EMERGENCY_CALL);
        mContextFixture.putStringArrayResource(
                com.android.internal.R.array.wfcSpnFormats,
                WIFI_CALLING_FORMATTERS);

        mBundle.putBoolean(
                CarrierConfigManager.KEY_ENABLE_CARRIER_DISPLAY_NAME_RESOLVER_BOOL, true);
        mBundle.putInt(CarrierConfigManager.KEY_WFC_SPN_FORMAT_IDX_INT, 0);
        mBundle.putInt(CarrierConfigManager.KEY_WFC_DATA_SPN_FORMAT_IDX_INT, 1);
        mBundle.putInt(CarrierConfigManager.KEY_WFC_FLIGHT_MODE_SPN_FORMAT_IDX_INT, 2);

        // Show SPN is required when roaming
        // Show PLMN is required when non-roaming.
        doReturn(IccRecords.CARRIER_NAME_DISPLAY_CONDITION_BITMASK_PLMN
                | IccRecords.CARRIER_NAME_DISPLAY_CONDITION_BITMASK_SPN)
                .when(mSimRecords).getCarrierNameDisplayCondition();

        mBundle.putString(CarrierConfigManager.KEY_CARRIER_NAME_STRING, "SPN from carrier config");
        mBundle.putInt(CarrierConfigManager.KEY_SPN_DISPLAY_CONDITION_OVERRIDE_INT,
                IccRecords.CARRIER_NAME_DISPLAY_CONDITION_BITMASK_PLMN
                        | IccRecords.CARRIER_NAME_DISPLAY_CONDITION_BITMASK_SPN);
        mBundle.putStringArray(CarrierConfigManager.KEY_SPDI_OVERRIDE_STRING_ARRAY,
                CARRIER_CONFIG_SPDI);
        mBundle.putStringArray(CarrierConfigManager.KEY_EHPLMN_OVERRIDE_STRING_ARRAY,
                CARRIER_CONFIG_EHPLMN);
        mBundle.putStringArray(CarrierConfigManager.KEY_PNN_OVERRIDE_STRING_ARRAY,
                CARRIER_CONFIG_PNN);

        // Do not force display "No service" when sim is not ready in any locales
        mContextFixture.putStringArrayResource(
                com.android.internal.R.array.config_display_no_service_when_sim_unready,
                new String[0]);

        logd("ServiceStateTrackerTest -Setup!");
    }

    @After
    public void tearDown() throws Exception {
        sst = null;
        mSSTTestHandler.quit();
        mSSTTestHandler.join();
        super.tearDown();
    }

    @Test
    @MediumTest
    public void testSetRadioPower() {
        boolean oldState = (mSimulatedCommands.getRadioState() == TelephonyManager.RADIO_POWER_ON);
        sst.setRadioPower(!oldState);
        waitForMs(100);
        assertTrue(oldState
                != (mSimulatedCommands.getRadioState() == TelephonyManager.RADIO_POWER_ON));
    }

    @Test
    @MediumTest
    public void testSetRadioPowerFromCarrier() {
        // Carrier disable radio power
        sst.setRadioPowerFromCarrier(false);
        waitForMs(100);
        assertFalse(mSimulatedCommands.getRadioState()
                == TelephonyManager.RADIO_POWER_ON);
        assertTrue(sst.getDesiredPowerState());
        assertFalse(sst.getPowerStateFromCarrier());

        // User toggle radio power will not overrides carrier settings
        sst.setRadioPower(true);
        waitForMs(100);
        assertFalse(mSimulatedCommands.getRadioState()
                == TelephonyManager.RADIO_POWER_ON);
        assertTrue(sst.getDesiredPowerState());
        assertFalse(sst.getPowerStateFromCarrier());

        // Carrier re-enable radio power
        sst.setRadioPowerFromCarrier(true);
        waitForMs(100);
        assertTrue(mSimulatedCommands.getRadioState() == TelephonyManager.RADIO_POWER_ON);
        assertTrue(sst.getDesiredPowerState());
        assertTrue(sst.getPowerStateFromCarrier());

        // User toggle radio power off (airplane mode) and set carrier on
        sst.setRadioPower(false);
        sst.setRadioPowerFromCarrier(true);
        waitForMs(100);
        assertFalse(mSimulatedCommands.getRadioState()
                == TelephonyManager.RADIO_POWER_ON);
        assertFalse(sst.getDesiredPowerState());
        assertTrue(sst.getPowerStateFromCarrier());
    }

    @Test
    @MediumTest
    public void testRilTrafficAfterSetRadioPower() {
        sst.setRadioPower(true);
        final int getOperatorCallCount = mSimulatedCommands.getGetOperatorCallCount();
        final int getDataRegistrationStateCallCount =
                mSimulatedCommands.getGetDataRegistrationStateCallCount();
        final int getVoiceRegistrationStateCallCount =
                mSimulatedCommands.getGetVoiceRegistrationStateCallCount();
        final int getNetworkSelectionModeCallCount =
                mSimulatedCommands.getGetNetworkSelectionModeCallCount();
        sst.setRadioPower(false);

        waitForMs(500);
        sst.pollState();
        waitForMs(250);

        // This test was meant to be for *no* ril traffic. However, RADIO_STATE_CHANGED is
        // considered a modem triggered action and that causes a pollState() to be done
        assertEquals(getOperatorCallCount + 1, mSimulatedCommands.getGetOperatorCallCount());
        assertEquals(getDataRegistrationStateCallCount + 1,
                mSimulatedCommands.getGetDataRegistrationStateCallCount());
        assertEquals(getVoiceRegistrationStateCallCount + 1,
                mSimulatedCommands.getGetVoiceRegistrationStateCallCount());
        assertEquals(getNetworkSelectionModeCallCount + 1,
                mSimulatedCommands.getGetNetworkSelectionModeCallCount());

        // Note that if the poll is triggered by a network change notification
        // and the modem is supposed to be off, we should still do the poll
        mSimulatedCommands.notifyNetworkStateChanged();
        waitForMs(250);

        assertEquals(getOperatorCallCount + 2 , mSimulatedCommands.getGetOperatorCallCount());
        assertEquals(getDataRegistrationStateCallCount + 2,
                mSimulatedCommands.getGetDataRegistrationStateCallCount());
        assertEquals(getVoiceRegistrationStateCallCount + 2,
                mSimulatedCommands.getGetVoiceRegistrationStateCallCount());
        assertEquals(getNetworkSelectionModeCallCount + 2,
                mSimulatedCommands.getGetNetworkSelectionModeCallCount());
    }

    @FlakyTest
    @Ignore
    @Test
    @MediumTest
    public void testSpnUpdateShowPlmnOnly() {
        doReturn(0).when(mSimRecords).getCarrierNameDisplayCondition();
        doReturn(IccCardApplicationStatus.AppState.APPSTATE_UNKNOWN).
                when(mUiccCardApplication3gpp).getState();

        sst.sendMessage(sst.obtainMessage(ServiceStateTracker.EVENT_NETWORK_STATE_CHANGED, null));

        waitForMs(750);

        ArgumentCaptor<Intent> intentArgumentCaptor = ArgumentCaptor.forClass(Intent.class);
        verify(mContextFixture.getTestDouble(), times(3))
                .sendStickyBroadcastAsUser(intentArgumentCaptor.capture(), eq(UserHandle.ALL));

        // We only want to verify the intent SPN_STRINGS_UPDATED_ACTION.
        List<Intent> intents = intentArgumentCaptor.getAllValues();
        logd("Total " + intents.size() + " intents");
        for (Intent intent : intents) {
            logd("  " + intent.getAction());
        }
        Intent intent = intents.get(2);
        assertEquals(TelephonyIntents.SPN_STRINGS_UPDATED_ACTION, intent.getAction());

        Bundle b = intent.getExtras();

        // For boolean we need to make sure the key exists first
        assertTrue(b.containsKey(TelephonyIntents.EXTRA_SHOW_SPN));
        assertFalse(b.getBoolean(TelephonyIntents.EXTRA_SHOW_SPN));

        assertEquals(null, b.getString(TelephonyIntents.EXTRA_SPN));
        assertEquals(null, b.getString(TelephonyIntents.EXTRA_DATA_SPN));

        // For boolean we need to make sure the key exists first
        assertTrue(b.containsKey(TelephonyIntents.EXTRA_SHOW_PLMN));
        assertTrue(b.getBoolean(TelephonyIntents.EXTRA_SHOW_PLMN));

        assertEquals(SimulatedCommands.FAKE_LONG_NAME, b.getString(TelephonyIntents.EXTRA_PLMN));

        ArgumentCaptor<Integer> intArgumentCaptor = ArgumentCaptor.forClass(Integer.class);
        verify(mTelephonyManager).setDataNetworkTypeForPhone(anyInt(), intArgumentCaptor.capture());
        assertEquals(ServiceState.RIL_RADIO_TECHNOLOGY_HSPA,
                intArgumentCaptor.getValue().intValue());
    }

    private CellInfoGsm getCellInfoGsm() {
        CellInfoGsm tmp = new CellInfoGsm();
        tmp.setCellIdentity(new CellIdentityGsm(0, 1, 900, 5, "001", "01", "test", "tst"));
        tmp.setCellSignalStrength(new CellSignalStrengthGsm(-85, 2, 3));
        return tmp;
    }

    @Test
    @MediumTest
    public void testCachedCellInfoList() {
        ArrayList<CellInfo> list = new ArrayList();
        list.add(getCellInfoGsm());
        mSimulatedCommands.setCellInfoList(list);

        WorkSource workSource = new WorkSource(Process.myUid(),
                mContext.getPackageName());

        // null worksource and no response message will update the writethrough cache
        sst.requestAllCellInfo(null, null);
        waitForMs(200);
        assertEquals(sst.getAllCellInfo(), list);
    }

    private static class CellInfoHandler extends Handler {
        // Need to define this here so that it's accessible
        public List<CellInfo> cellInfoResult;

        CellInfoHandler(Looper l) {
            super(l);
        }

        @Override
        public void handleMessage(Message msg) {
            synchronized (msg) {
                assertTrue("handler received null message", msg.obj != null);
                AsyncResult ar = (AsyncResult) msg.obj;
                cellInfoResult = (List<CellInfo>) ar.result;
                msg.notifyAll();
            }
        }
    }

    @Test
    @MediumTest
    public void testGetCellInfoResponse() throws InterruptedException {
        mSimulatedCommands.setCellInfoListBehavior(true);
        ArrayList<CellInfo> list = new ArrayList();
        list.add(getCellInfoGsm());
        mSimulatedCommands.setCellInfoList(list);
        CellInfoHandler cih = new CellInfoHandler(mSSTTestHandler.getLooper());

        Message rsp = cih.obtainMessage(0x7357);

        sst.requestAllCellInfo(null, rsp);

        synchronized (rsp) {
            if (cih.cellInfoResult == null) rsp.wait(5000);
        }

        AsyncResult ar = (AsyncResult) rsp.obj;
        assertTrue("CellInfo Response Not Received", cih.cellInfoResult != null);
        assertEquals(getCellInfoGsm(), cih.cellInfoResult.get(0));
    }

    @Test
    @MediumTest
    public void testGetCellInfoResponseTimeout() throws InterruptedException {
        mSimulatedCommands.setCellInfoListBehavior(false);
        CellInfoHandler cih = new CellInfoHandler(mSSTTestHandler.getLooper());

        Message rsp = cih.obtainMessage(0x7357);

        sst.requestAllCellInfo(null, rsp);

        synchronized (rsp) {
            if (cih.cellInfoResult == null) rsp.wait(5000);
        }

        assertTrue("Spurious CellInfo Response Received", cih.cellInfoResult == null);
    }

    @Test
    @MediumTest
    public void testImsRegState() {
        // Simulate IMS registered
        mSimulatedCommands.setImsRegistrationState(new int[]{1, PhoneConstants.PHONE_TYPE_GSM});

        sst.sendMessage(sst.obtainMessage(ServiceStateTracker.EVENT_IMS_STATE_CHANGED, null));
        waitForMs(200);

        assertTrue(sst.isImsRegistered());

        // Simulate IMS unregistered
        mSimulatedCommands.setImsRegistrationState(new int[]{0, PhoneConstants.PHONE_TYPE_GSM});

        sst.sendMessage(sst.obtainMessage(ServiceStateTracker.EVENT_IMS_STATE_CHANGED, null));
        waitForMs(200);

        assertFalse(sst.isImsRegistered());
    }

    @Test
    public void testOnImsServiceStateChanged() {
        // The service state of GsmCdmaPhone is STATE_OUT_OF_SERVICE, and IMS is unregistered.
        ServiceState ss = new ServiceState();
        ss.setVoiceRegState(ServiceState.STATE_OUT_OF_SERVICE);

        sst.mSS = ss;
        sst.sendMessage(sst.obtainMessage(ServiceStateTracker.EVENT_IMS_SERVICE_STATE_CHANGED));
        waitForMs(200);

        // The listener will be notified that the service state was changed.
        verify(mPhone).notifyServiceStateChanged(any(ServiceState.class));

        // The service state of GsmCdmaPhone is STATE_IN_SERVICE, and IMS is registered.
        ss = new ServiceState();
        ss.setVoiceRegState(ServiceState.STATE_IN_SERVICE);
        sst.mSS = ss;

        sst.sendMessage(sst.obtainMessage(ServiceStateTracker.EVENT_IMS_SERVICE_STATE_CHANGED));
        waitForMs(200);

        // Nothing happened because the IMS service state was not affected the merged service state.
        verify(mPhone, times(1)).notifyServiceStateChanged(any(ServiceState.class));
    }

    private void sendSignalStrength(SignalStrength ss) {
        mSimulatedCommands.setSignalStrength(ss);
        mSimulatedCommands.notifySignalStrength();
        waitForMs(300);
    }

    @Test
    @MediumTest
    public void testSignalStrength() {
        // Send in GSM Signal Strength Info and expect isGsm == true
        SignalStrength ss = new SignalStrength(
                new CellSignalStrengthCdma(),
                new CellSignalStrengthGsm(-53, 0, SignalStrength.INVALID),
                new CellSignalStrengthWcdma(),
                new CellSignalStrengthTdscdma(),
                new CellSignalStrengthLte(),
                new CellSignalStrengthNr());

        sendSignalStrength(ss);
        assertEquals(sst.getSignalStrength(), ss);
        assertEquals(sst.getSignalStrength().isGsm(), true);

        // Send in CDMA+LTE Signal Strength Info and expect isGsm == true
        ss = new SignalStrength(
                new CellSignalStrengthCdma(-90, -12,
                        SignalStrength.INVALID, SignalStrength.INVALID, SignalStrength.INVALID),
                new CellSignalStrengthGsm(),
                new CellSignalStrengthWcdma(),
                new CellSignalStrengthTdscdma(),
                new CellSignalStrengthLte(
                        -110, -114, -5, 0, SignalStrength.INVALID, SignalStrength.INVALID),
                new CellSignalStrengthNr());

        sendSignalStrength(ss);
        assertEquals(sst.getSignalStrength(), ss);
        assertEquals(sst.getSignalStrength().isGsm(), true);

        // Send in CDMA-only Signal Strength Info and expect isGsm == false
        ss = new SignalStrength(
                new CellSignalStrengthCdma(-90, -12,
                        SignalStrength.INVALID, SignalStrength.INVALID, SignalStrength.INVALID),
                new CellSignalStrengthGsm(),
                new CellSignalStrengthWcdma(),
                new CellSignalStrengthTdscdma(),
                new CellSignalStrengthLte(),
                new CellSignalStrengthNr());

        sendSignalStrength(ss);
        assertEquals(sst.getSignalStrength(), ss);
        assertEquals(sst.getSignalStrength().isGsm(), false);
    }

    private void sendCarrierConfigUpdate() {
        CarrierConfigManager mockConfigManager = Mockito.mock(CarrierConfigManager.class);
        when(mContext.getSystemService(Context.CARRIER_CONFIG_SERVICE))
                .thenReturn(mockConfigManager);
        when(mockConfigManager.getConfigForSubId(anyInt())).thenReturn(mBundle);

        Intent intent = new Intent().setAction(CarrierConfigManager.ACTION_CARRIER_CONFIG_CHANGED);
        intent.putExtra(CarrierConfigManager.EXTRA_SLOT_INDEX, PHONE_ID);
        mContext.sendBroadcast(intent);
        waitForMs(300);
    }

    @Test
    public void testLteSignalStrengthReportingCriteria() {
        SignalStrength ss = new SignalStrength(
                new CellSignalStrengthCdma(),
                new CellSignalStrengthGsm(),
                new CellSignalStrengthWcdma(),
                new CellSignalStrengthTdscdma(),
                new CellSignalStrengthLte(
                        -110, /* rssi */
                        -114, /* rsrp */
                        -5, /* rsrq */
                        0, /* rssnr */
                        SignalStrength.INVALID, /* cqi */
                        SignalStrength.INVALID /* ta */),
                new CellSignalStrengthNr());

        mBundle.putBoolean(CarrierConfigManager.KEY_USE_ONLY_RSRP_FOR_LTE_SIGNAL_BAR_BOOL,
                true);

        sendCarrierConfigUpdate();

        mSimulatedCommands.setSignalStrength(ss);
        mSimulatedCommands.notifySignalStrength();
        waitForMs(300);
        // Default thresholds are POOR=-115 MODERATE=-105 GOOD=-95 GREAT=-85
        assertEquals(CellSignalStrength.SIGNAL_STRENGTH_POOR, sst.getSignalStrength().getLevel());

        int[] lteThresholds = {
                -130, // SIGNAL_STRENGTH_POOR
                -120, // SIGNAL_STRENGTH_MODERATE
                -110, // SIGNAL_STRENGTH_GOOD
                -100,  // SIGNAL_STRENGTH_GREAT
        };
        mBundle.putIntArray(CarrierConfigManager.KEY_LTE_RSRP_THRESHOLDS_INT_ARRAY,
                lteThresholds);
        sendCarrierConfigUpdate();

        mSimulatedCommands.setSignalStrength(ss);
        mSimulatedCommands.notifySignalStrength();
        waitForMs(300);
        assertEquals(sst.getSignalStrength().getLevel(),
                CellSignalStrength.SIGNAL_STRENGTH_MODERATE);
    }

    @Test
    public void testWcdmaSignalStrengthReportingCriteria() {
        SignalStrength ss = new SignalStrength(
                new CellSignalStrengthCdma(),
                new CellSignalStrengthGsm(),
                new CellSignalStrengthWcdma(-79, 0, -85, -5),
                new CellSignalStrengthTdscdma(),
                new CellSignalStrengthLte(),
                new CellSignalStrengthNr());

        mSimulatedCommands.setSignalStrength(ss);
        mSimulatedCommands.notifySignalStrength();
        waitForMs(300);
        assertEquals(sst.getSignalStrength().getLevel(), CellSignalStrength.SIGNAL_STRENGTH_GOOD);

        int[] wcdmaThresholds = {
                -110, // SIGNAL_STRENGTH_POOR
                -100, // SIGNAL_STRENGTH_MODERATE
                -90, // SIGNAL_STRENGTH_GOOD
                -80  // SIGNAL_STRENGTH_GREAT
        };
        mBundle.putIntArray(CarrierConfigManager.KEY_WCDMA_RSCP_THRESHOLDS_INT_ARRAY,
                wcdmaThresholds);
        mBundle.putString(
                CarrierConfigManager.KEY_WCDMA_DEFAULT_SIGNAL_STRENGTH_MEASUREMENT_STRING,
                "rscp");
        sendCarrierConfigUpdate();
        mSimulatedCommands.setSignalStrength(ss);
        mSimulatedCommands.notifySignalStrength();
        waitForMs(300);
        assertEquals(sst.getSignalStrength().getLevel(), CellSignalStrength.SIGNAL_STRENGTH_GOOD);
    }

    @Test
    @MediumTest
    // TODO(nharold): we probably should remove support for this procedure (GET_LOC)
    public void testGsmCellLocation() {
        CellIdentityGsm cellIdentityGsm = new CellIdentityGsm(
                2, 3, 900, 5, "001", "01", "test", "tst");

        NetworkRegistrationInfo result = new NetworkRegistrationInfo.Builder()
                .setDomain(NetworkRegistrationInfo.DOMAIN_CS)
                .setTransportType(AccessNetworkConstants.TRANSPORT_TYPE_WWAN)
                .setRegistrationState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME)
                .setCellIdentity(cellIdentityGsm)
                .build();

        sst.sendMessage(sst.obtainMessage(ServiceStateTracker.EVENT_GET_LOC_DONE,
                new AsyncResult(null, result, null)));

        waitForMs(200);
        WorkSource workSource = new WorkSource(Process.myUid(), mContext.getPackageName());
        GsmCellLocation cl = (GsmCellLocation) sst.getCellLocation();
        assertEquals(2, cl.getLac());
        assertEquals(3, cl.getCid());
    }

    @FlakyTest /* flakes 0.86% of the time */
    @Test
    @MediumTest
    // TODO(nharold): we probably should remove support for this procedure (GET_LOC)
    public void testCdmaCellLocation() {
        CellIdentityCdma cellIdentityCdma = new CellIdentityCdma(1, 2, 3, 4, 5, "test", "tst");

        NetworkRegistrationInfo result = new NetworkRegistrationInfo.Builder()
                .setDomain(NetworkRegistrationInfo.DOMAIN_CS)
                .setTransportType(AccessNetworkConstants.TRANSPORT_TYPE_WWAN)
                .setRegistrationState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME)
                .setCellIdentity(cellIdentityCdma)
                .build();

        sst.sendMessage(sst.obtainMessage(ServiceStateTracker.EVENT_GET_LOC_DONE,
                new AsyncResult(null, result, null)));

        waitForMs(200);
        WorkSource workSource = new WorkSource(Process.myUid(), mContext.getPackageName());
        CdmaCellLocation cl = (CdmaCellLocation) sst.getCellLocation();
        assertEquals(5, cl.getBaseStationLatitude());
        assertEquals(4, cl.getBaseStationLongitude());
    }

    @Test
    @MediumTest
    public void testUpdatePhoneType() {
        doReturn(false).when(mPhone).isPhoneTypeGsm();
        doReturn(true).when(mPhone).isPhoneTypeCdmaLte();
        doReturn(CdmaSubscriptionSourceManager.SUBSCRIPTION_FROM_RUIM).when(mCdmaSSM).
                getCdmaSubscriptionSource();

        logd("Calling updatePhoneType");
        // switch to CDMA
        sst.updatePhoneType();

        ArgumentCaptor<Integer> integerArgumentCaptor = ArgumentCaptor.forClass(Integer.class);
        verify(mRuimRecords).registerForRecordsLoaded(eq(sst), integerArgumentCaptor.capture(),
                nullable(Object.class));

        // response for mRuimRecords.registerForRecordsLoaded()
        Message msg = Message.obtain();
        msg.what = integerArgumentCaptor.getValue();
        msg.obj = new AsyncResult(null, null, null);
        sst.sendMessage(msg);
        waitForMs(100);

        // on RUIM_RECORDS_LOADED, sst is expected to call following apis
        verify(mRuimRecords, times(1)).isProvisioned();

        // switch back to GSM
        doReturn(true).when(mPhone).isPhoneTypeGsm();
        doReturn(false).when(mPhone).isPhoneTypeCdmaLte();

        // response for mRuimRecords.registerForRecordsLoaded() can be sent after switching to GSM
        msg = Message.obtain();
        msg.what = integerArgumentCaptor.getValue();
        msg.obj = new AsyncResult(null, null, null);
        sst.sendMessage(msg);

        // There's no easy way to check if the msg was handled or discarded. Wait to make sure sst
        // did not crash, and then verify that the functions called records loaded are not called
        // again
        waitForMs(200);

        verify(mRuimRecords, times(1)).isProvisioned();
    }

    @Test
    @MediumTest
    public void testRegAndUnregForVoiceRoamingOn() throws Exception {
        sst.registerForVoiceRoamingOn(mTestHandler, EVENT_DATA_ROAMING_ON, null);

        // Enable roaming and trigger events to notify handler registered
        doReturn(true).when(mPhone).isPhoneTypeGsm();
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(200);

        // verify if registered handler has message posted to it
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler).sendMessageAtTime(messageArgumentCaptor.capture(), anyLong());
        assertEquals(EVENT_DATA_ROAMING_ON, messageArgumentCaptor.getValue().what);

        // Disable roaming
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        // Unregister registrant
        sst.unregisterForVoiceRoamingOn(mTestHandler);

        // Enable roaming
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(200);

        // verify that no new message posted to handler
        verify(mTestHandler, times(1)).sendMessageAtTime(any(Message.class), anyLong());
    }

    @Test
    @MediumTest
    public void testRegAndUnregForVoiceRoamingOff() throws Exception {
        // Enable roaming
        doReturn(true).when(mPhone).isPhoneTypeGsm();
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        sst.registerForVoiceRoamingOff(mTestHandler, EVENT_DATA_ROAMING_OFF, null);

        // Disable roaming
        doReturn(true).when(mPhone).isPhoneTypeGsm();
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(200);

        // verify if registered handler has message posted to it
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler).sendMessageAtTime(messageArgumentCaptor.capture(), anyLong());
        assertEquals(EVENT_DATA_ROAMING_OFF, messageArgumentCaptor.getValue().what);

        // Enable roaming
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        // Unregister registrant
        sst.unregisterForVoiceRoamingOff(mTestHandler);

        // Disable roaming
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        // verify that no new message posted to handler
        verify(mTestHandler, times(1)).sendMessageAtTime(any(Message.class), anyLong());
    }

    @Test
    @MediumTest
    public void testRegAndUnregForDataRoamingOn() throws Exception {
        sst.registerForDataRoamingOn(mTestHandler, EVENT_DATA_ROAMING_ON, null);

        // Enable roaming and trigger events to notify handler registered
        doReturn(true).when(mPhone).isPhoneTypeGsm();
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(200);

        // verify if registered handler has message posted to it
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler).sendMessageAtTime(messageArgumentCaptor.capture(), anyLong());
        assertEquals(EVENT_DATA_ROAMING_ON, messageArgumentCaptor.getValue().what);

        // Disable roaming
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        // Unregister registrant
        sst.unregisterForDataRoamingOn(mTestHandler);

        // Enable roaming
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(200);

        // verify that no new message posted to handler
        verify(mTestHandler, times(1)).sendMessageAtTime(any(Message.class), anyLong());
    }

    @Test
    @MediumTest
    public void testRegAndUnregForDataRoamingOff() throws Exception {
        // Enable roaming
        doReturn(true).when(mPhone).isPhoneTypeGsm();
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        sst.registerForDataRoamingOff(mTestHandler, EVENT_DATA_ROAMING_OFF, null, true);

        // Disable roaming
        doReturn(true).when(mPhone).isPhoneTypeGsm();
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        // verify if registered handler has message posted to it
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler).sendMessageAtTime(messageArgumentCaptor.capture(), anyLong());
        assertEquals(EVENT_DATA_ROAMING_OFF, messageArgumentCaptor.getValue().what);

        // Enable roaming
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        // Unregister registrant
        sst.unregisterForDataRoamingOff(mTestHandler);

        // Disable roaming
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        // verify that no new message posted to handler
        verify(mTestHandler, times(1)).sendMessageAtTime(any(Message.class), anyLong());
    }

    @Test
    @MediumTest
    public void testRegAndInvalidregForDataConnAttach() throws Exception {
        // Initially set service state out of service
        doReturn(true).when(mPhone).isPhoneTypeGsm();
        mSimulatedCommands.setVoiceRegState(23);
        mSimulatedCommands.setDataRegState(23);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        sst.registerForDataConnectionAttached(AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                mTestHandler, EVENT_DATA_CONNECTION_ATTACHED, null);

        // set service state in service and trigger events to post message on handler
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(200);

        // verify if registered handler has message posted to it
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler).sendMessageAtTime(messageArgumentCaptor.capture(), anyLong());
        assertEquals(EVENT_DATA_CONNECTION_ATTACHED, messageArgumentCaptor.getValue().what);

        // set service state out of service
        mSimulatedCommands.setVoiceRegState(-1);
        mSimulatedCommands.setDataRegState(-1);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        // Unregister registrant
        sst.unregisterForDataConnectionAttached(AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                mTestHandler);

        // set service state in service
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        // verify that no new message posted to handler
        verify(mTestHandler, times(1)).sendMessageAtTime(any(Message.class), anyLong());
    }

    @Test
    @MediumTest
    public void testRegAndUnregForDataConnAttach() throws Exception {
        // Initially set service state out of service
        doReturn(true).when(mPhone).isPhoneTypeGsm();
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_UNKNOWN);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_UNKNOWN);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        sst.registerForDataConnectionAttached(AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                mTestHandler, EVENT_DATA_CONNECTION_ATTACHED, null);

        // set service state in service and trigger events to post message on handler
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(200);

        // verify if registered handler has message posted to it
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler).sendMessageAtTime(messageArgumentCaptor.capture(), anyLong());
        assertEquals(EVENT_DATA_CONNECTION_ATTACHED, messageArgumentCaptor.getValue().what);

        // set service state out of service
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_UNKNOWN);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_UNKNOWN);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        // Unregister registrant
        sst.unregisterForDataConnectionAttached(AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                mTestHandler);

        // set service state in service
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        // verify that no new message posted to handler
        verify(mTestHandler, times(1)).sendMessageAtTime(any(Message.class), anyLong());
    }

    @Test
    @MediumTest
    public void testRegAndUnregForDataConnDetach() throws Exception {
        // Initially set service state in service
        doReturn(true).when(mPhone).isPhoneTypeGsm();
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.notifyNetworkStateChanged();

        sst.registerForDataConnectionDetached(AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                mTestHandler, EVENT_DATA_CONNECTION_DETACHED, null);

        // set service state out of service and trigger events to post message on handler
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_UNKNOWN);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_UNKNOWN);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(200);

        // verify if registered handler has message posted to it
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler).sendMessageAtTime(messageArgumentCaptor.capture(), anyLong());
        assertEquals(EVENT_DATA_CONNECTION_DETACHED, messageArgumentCaptor.getValue().what);

        // set service state in service
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        // Unregister registrant
        sst.unregisterForDataConnectionDetached(AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                mTestHandler);

        // set service state out of service
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_UNKNOWN);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_UNKNOWN);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        // verify that no new message posted to handler
        verify(mTestHandler, times(1)).sendMessageAtTime(any(Message.class), anyLong());
    }

    @Test
    @MediumTest
    public void testRegisterForVoiceRegStateOrRatChange() {
        NetworkRegistrationInfo nri = new NetworkRegistrationInfo.Builder()
                .setTransportType(AccessNetworkConstants.TRANSPORT_TYPE_WWAN)
                .setDomain(NetworkRegistrationInfo.DOMAIN_CS)
                .setAccessNetworkTechnology(TelephonyManager.NETWORK_TYPE_LTE)
                .build();
        sst.mSS.addNetworkRegistrationInfo(nri);

        sst.registerForVoiceRegStateOrRatChanged(mTestHandler, EVENT_VOICE_RAT_CHANGED, null);

        waitForMs(100);

        // Verify if message was posted to handler and value of result
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler).sendMessageAtTime(messageArgumentCaptor.capture(), anyLong());
        assertEquals(EVENT_VOICE_RAT_CHANGED, messageArgumentCaptor.getValue().what);
        assertEquals(new Pair<Integer, Integer>(ServiceState.STATE_IN_SERVICE,
                        ServiceState.RIL_RADIO_TECHNOLOGY_LTE),
                ((AsyncResult)messageArgumentCaptor.getValue().obj).result);
    }

    @Test
    @MediumTest
    public void testRegisterForDataRegStateOrRatChange() {
        NetworkRegistrationInfo nrs = new NetworkRegistrationInfo.Builder()
                .setDomain(NetworkRegistrationInfo.DOMAIN_PS)
                .setTransportType(AccessNetworkConstants.TRANSPORT_TYPE_WWAN)
                .setAccessNetworkTechnology(TelephonyManager.NETWORK_TYPE_LTE)
                .setRegistrationState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME)
                .build();
        sst.mSS.addNetworkRegistrationInfo(nrs);
        sst.registerForDataRegStateOrRatChanged(AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                mTestHandler, EVENT_DATA_RAT_CHANGED, null);

        waitForMs(100);

        // Verify if message was posted to handler and value of result
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler).sendMessageAtTime(messageArgumentCaptor.capture(), anyLong());
        assertEquals(EVENT_DATA_RAT_CHANGED, messageArgumentCaptor.getValue().what);
        assertEquals(new Pair<Integer, Integer>(ServiceState.STATE_IN_SERVICE,
                        ServiceState.RIL_RADIO_TECHNOLOGY_LTE),
                ((AsyncResult)messageArgumentCaptor.getValue().obj).result);
    }

    @FlakyTest /* flakes 0.43% of the time */
    @Test
    @MediumTest
    public void testRegAndUnregForNetworkAttached() throws Exception {
        // Initially set service state out of service
        doReturn(true).when(mPhone).isPhoneTypeGsm();
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_UNKNOWN);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_UNKNOWN);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        sst.registerForNetworkAttached(mTestHandler, EVENT_REGISTERED_TO_NETWORK, null);

        // set service state in service and trigger events to post message on handler
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        // verify if registered handler has message posted to it
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler).sendMessageAtTime(messageArgumentCaptor.capture(), anyLong());
        assertEquals(EVENT_REGISTERED_TO_NETWORK, messageArgumentCaptor.getValue().what);

        // set service state out of service
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_UNKNOWN);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_UNKNOWN);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        // Unregister registrant
        sst.unregisterForNetworkAttached(mTestHandler);

        // set service state in service
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        // verify that no new message posted to handler
        verify(mTestHandler, times(1)).sendMessageAtTime(any(Message.class), anyLong());
    }

    @Test
    @MediumTest
    public void testRegAndInvalidRegForNetworkAttached() throws Exception {
        // Initially set service state out of service
        doReturn(true).when(mPhone).isPhoneTypeGsm();
        mSimulatedCommands.setVoiceRegState(23);
        mSimulatedCommands.setDataRegState(23);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        sst.registerForNetworkAttached(mTestHandler, EVENT_REGISTERED_TO_NETWORK, null);

        // set service state in service and trigger events to post message on handler
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        // verify if registered handler has message posted to it
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler).sendMessageAtTime(messageArgumentCaptor.capture(), anyLong());
        assertEquals(EVENT_REGISTERED_TO_NETWORK, messageArgumentCaptor.getValue().what);

        // set service state out of service
        mSimulatedCommands.setVoiceRegState(-1);
        mSimulatedCommands.setDataRegState(-1);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        // Unregister registrant
        sst.unregisterForNetworkAttached(mTestHandler);


        waitForMs(100);

        sst.registerForNetworkAttached(mTestHandler, EVENT_REGISTERED_TO_NETWORK, null);

        // set service state in service
        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(100);

        // verify if registered handler has message posted to it
        messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler, times(2)).sendMessageAtTime(messageArgumentCaptor.capture(),
                anyLong());
        assertEquals(EVENT_REGISTERED_TO_NETWORK, messageArgumentCaptor.getValue().what);
    }

    @Test
    @MediumTest
    public void testRegisterForPsRestrictedEnabled() throws Exception {
        sst.mRestrictedState.setPsRestricted(true);
        // Since PsRestricted is set to true, registerForPsRestrictedEnabled will
        // also post message to handler
        sst.registerForPsRestrictedEnabled(mTestHandler, EVENT_PS_RESTRICT_ENABLED, null);

        waitForMs(100);

        // verify posted message
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler).sendMessageAtTime(messageArgumentCaptor.capture(), anyLong());
        assertEquals(EVENT_PS_RESTRICT_ENABLED, messageArgumentCaptor.getValue().what);
    }

    @Test
    @MediumTest
    public void testRegisterForPsRestrictedDisabled() throws Exception {
        sst.mRestrictedState.setPsRestricted(true);
        // Since PsRestricted is set to true, registerForPsRestrictedDisabled will
        // also post message to handler
        sst.registerForPsRestrictedDisabled(mTestHandler, EVENT_PS_RESTRICT_DISABLED, null);

        waitForMs(100);

        // verify posted message
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler).sendMessageAtTime(messageArgumentCaptor.capture(), anyLong());
        assertEquals(EVENT_PS_RESTRICT_DISABLED, messageArgumentCaptor.getValue().what);
    }

    @Test
    @MediumTest
    public void testOnRestrictedStateChanged() throws Exception {
        ServiceStateTracker spySst = spy(sst);
        doReturn(true).when(mPhone).isPhoneTypeGsm();
        doReturn(IccCardApplicationStatus.AppState.APPSTATE_READY).when(
                mUiccCardApplication3gpp).getState();

        ArgumentCaptor<Integer> intArgumentCaptor = ArgumentCaptor.forClass(Integer.class);
        verify(mSimulatedCommandsVerifier).setOnRestrictedStateChanged(any(Handler.class),
                intArgumentCaptor.capture(), eq(null));
        // Since spy() creates a copy of sst object we need to call
        // setOnRestrictedStateChanged() explicitly.
        mSimulatedCommands.setOnRestrictedStateChanged(spySst,
                intArgumentCaptor.getValue().intValue(), null);

        // Combination of restricted state and expected notification type.
        final int CS_ALL[] = {RILConstants.RIL_RESTRICTED_STATE_CS_ALL,
                ServiceStateTracker.CS_ENABLED};
        final int CS_NOR[] = {RILConstants.RIL_RESTRICTED_STATE_CS_NORMAL,
                ServiceStateTracker.CS_NORMAL_ENABLED};
        final int CS_EME[] = {RILConstants.RIL_RESTRICTED_STATE_CS_EMERGENCY,
                ServiceStateTracker.CS_EMERGENCY_ENABLED};
        final int CS_NON[] = {RILConstants.RIL_RESTRICTED_STATE_NONE,
                ServiceStateTracker.CS_DISABLED};
        final int PS_ALL[] = {RILConstants.RIL_RESTRICTED_STATE_PS_ALL,
                ServiceStateTracker.PS_ENABLED};
        final int PS_NON[] = {RILConstants.RIL_RESTRICTED_STATE_NONE,
                ServiceStateTracker.PS_DISABLED};

        int notifyCount = 0;
        // cs not restricted -> cs emergency/normal restricted
        internalCheckForRestrictedStateChange(spySst, ++notifyCount, CS_ALL);
        // cs emergency/normal restricted -> cs normal restricted
        internalCheckForRestrictedStateChange(spySst, ++notifyCount, CS_NOR);
        // cs normal restricted -> cs emergency restricted
        internalCheckForRestrictedStateChange(spySst, ++notifyCount, CS_EME);
        // cs emergency restricted -> cs not restricted
        internalCheckForRestrictedStateChange(spySst, ++notifyCount, CS_NON);
        // cs not restricted -> cs normal restricted
        internalCheckForRestrictedStateChange(spySst, ++notifyCount, CS_NOR);
        // cs normal restricted -> cs emergency/normal restricted
        internalCheckForRestrictedStateChange(spySst, ++notifyCount, CS_ALL);
        // cs emergency/normal restricted -> cs emergency restricted
        internalCheckForRestrictedStateChange(spySst, ++notifyCount, CS_EME);
        // cs emergency restricted -> cs emergency/normal restricted
        internalCheckForRestrictedStateChange(spySst, ++notifyCount, CS_ALL);
        // cs emergency/normal restricted -> cs not restricted
        internalCheckForRestrictedStateChange(spySst, ++notifyCount, CS_NON);
        // cs not restricted -> cs emergency restricted
        internalCheckForRestrictedStateChange(spySst, ++notifyCount, CS_EME);
        // cs emergency restricted -> cs normal restricted
        internalCheckForRestrictedStateChange(spySst, ++notifyCount, CS_NOR);
        // cs normal restricted -> cs not restricted
        internalCheckForRestrictedStateChange(spySst, ++notifyCount, CS_NON);

        // ps not restricted -> ps restricted
        internalCheckForRestrictedStateChange(spySst, ++notifyCount, PS_ALL);
        // ps restricted -> ps not restricted
        internalCheckForRestrictedStateChange(spySst, ++notifyCount, PS_NON);
    }

    private void internalCheckForRestrictedStateChange(ServiceStateTracker serviceStateTracker,
                int times, int[] restrictedState) {
        mSimulatedCommands.triggerRestrictedStateChanged(restrictedState[0]);
        waitForMs(100);
        ArgumentCaptor<Integer> intArgumentCaptor = ArgumentCaptor.forClass(Integer.class);
        verify(serviceStateTracker, times(times)).setNotification(intArgumentCaptor.capture());
        assertEquals(intArgumentCaptor.getValue().intValue(), restrictedState[1]);
    }

    private boolean notificationHasTitleSet(Notification n) {
        // Notification has no methods to check the actual title, but #toString() includes the
        // word "tick" if the title is set so we check this as a workaround
        return n.toString().contains("tick");
    }

    private String getNotificationTitle(Notification n) {
        return n.extras.getString(Notification.EXTRA_TITLE);
    }

    @Test
    @SmallTest
    public void testSetPsNotifications() {
        int subId = 1;
        sst.mSubId = subId;
        doReturn(subId).when(mSubInfo).getSubscriptionId();

        doReturn(mSubInfo).when(mSubscriptionController).getActiveSubscriptionInfo(
                anyInt(), anyString());

        final NotificationManager nm = (NotificationManager)
                mContext.getSystemService(Context.NOTIFICATION_SERVICE);
        mContextFixture.putBooleanResource(
                R.bool.config_user_notification_of_restrictied_mobile_access, true);
        doReturn(new ApplicationInfo()).when(mContext).getApplicationInfo();
        Drawable mockDrawable = mock(Drawable.class);
        Resources mockResources = mContext.getResources();
        when(mockResources.getDrawable(anyInt(), any())).thenReturn(mockDrawable);

        mContextFixture.putResource(com.android.internal.R.string.RestrictedOnDataTitle, "test1");
        sst.setNotification(ServiceStateTracker.PS_ENABLED);
        ArgumentCaptor<Notification> notificationArgumentCaptor =
                ArgumentCaptor.forClass(Notification.class);
        verify(nm).notify(anyString(), anyInt(), notificationArgumentCaptor.capture());
        // if the postedNotification has title set then it must have been the correct notification
        Notification postedNotification = notificationArgumentCaptor.getValue();
        assertTrue(notificationHasTitleSet(postedNotification));
        assertEquals("test1", getNotificationTitle(postedNotification));

        sst.setNotification(ServiceStateTracker.PS_DISABLED);
        verify(nm).cancel(Integer.toString(sst.mSubId), ServiceStateTracker.PS_NOTIFICATION);
    }

    @Test
    @SmallTest
    public void testSetCsNotifications() {
        int subId = 1;
        sst.mSubId = subId;
        doReturn(subId).when(mSubInfo).getSubscriptionId();
        doReturn(mSubInfo).when(mSubscriptionController)
                .getActiveSubscriptionInfo(anyInt(), anyString());

        final NotificationManager nm = (NotificationManager)
                mContext.getSystemService(Context.NOTIFICATION_SERVICE);
        mContextFixture.putBooleanResource(
                R.bool.config_user_notification_of_restrictied_mobile_access, true);
        doReturn(new ApplicationInfo()).when(mContext).getApplicationInfo();
        Drawable mockDrawable = mock(Drawable.class);
        Resources mockResources = mContext.getResources();
        when(mockResources.getDrawable(anyInt(), any())).thenReturn(mockDrawable);

        mContextFixture.putResource(com.android.internal.R.string.RestrictedOnAllVoiceTitle,
                "test2");
        sst.setNotification(ServiceStateTracker.CS_ENABLED);
        ArgumentCaptor<Notification> notificationArgumentCaptor =
                ArgumentCaptor.forClass(Notification.class);
        verify(nm).notify(anyString(), anyInt(), notificationArgumentCaptor.capture());
        // if the postedNotification has title set then it must have been the correct notification
        Notification postedNotification = notificationArgumentCaptor.getValue();
        assertTrue(notificationHasTitleSet(postedNotification));
        assertEquals("test2", getNotificationTitle(postedNotification));

        sst.setNotification(ServiceStateTracker.CS_DISABLED);
        verify(nm).cancel(Integer.toString(sst.mSubId), ServiceStateTracker.CS_NOTIFICATION);
    }

    @Test
    @SmallTest
    public void testSetCsNormalNotifications() {
        int subId = 1;
        sst.mSubId = subId;
        doReturn(subId).when(mSubInfo).getSubscriptionId();
        doReturn(mSubInfo).when(mSubscriptionController)
                .getActiveSubscriptionInfo(anyInt(), anyString());

        final NotificationManager nm = (NotificationManager)
                mContext.getSystemService(Context.NOTIFICATION_SERVICE);
        mContextFixture.putBooleanResource(
                R.bool.config_user_notification_of_restrictied_mobile_access, true);
        doReturn(new ApplicationInfo()).when(mContext).getApplicationInfo();
        Drawable mockDrawable = mock(Drawable.class);
        Resources mockResources = mContext.getResources();
        when(mockResources.getDrawable(anyInt(), any())).thenReturn(mockDrawable);

        mContextFixture.putResource(com.android.internal.R.string.RestrictedOnNormalTitle, "test3");
        sst.setNotification(ServiceStateTracker.CS_NORMAL_ENABLED);
        ArgumentCaptor<Notification> notificationArgumentCaptor =
                ArgumentCaptor.forClass(Notification.class);
        verify(nm).notify(anyString(), anyInt(), notificationArgumentCaptor.capture());
        // if the postedNotification has title set then it must have been the correct notification
        Notification postedNotification = notificationArgumentCaptor.getValue();
        assertTrue(notificationHasTitleSet(postedNotification));
        assertEquals("test3", getNotificationTitle(postedNotification));

        sst.setNotification(ServiceStateTracker.CS_DISABLED);
        verify(nm).cancel(Integer.toString(sst.mSubId), ServiceStateTracker.CS_NOTIFICATION);
    }

    @Test
    @SmallTest
    public void testSetCsEmergencyNotifications() {
        int subId = 1;
        sst.mSubId = subId;
        doReturn(subId).when(mSubInfo).getSubscriptionId();
        doReturn(mSubInfo).when(mSubscriptionController)
                .getActiveSubscriptionInfo(anyInt(), anyString());

        final NotificationManager nm = (NotificationManager)
                mContext.getSystemService(Context.NOTIFICATION_SERVICE);
        mContextFixture.putBooleanResource(
                R.bool.config_user_notification_of_restrictied_mobile_access, true);
        doReturn(new ApplicationInfo()).when(mContext).getApplicationInfo();
        Drawable mockDrawable = mock(Drawable.class);
        Resources mockResources = mContext.getResources();
        when(mockResources.getDrawable(anyInt(), any())).thenReturn(mockDrawable);

        mContextFixture.putResource(com.android.internal.R.string.RestrictedOnEmergencyTitle,
                "test4");
        sst.setNotification(ServiceStateTracker.CS_EMERGENCY_ENABLED);
        ArgumentCaptor<Notification> notificationArgumentCaptor =
                ArgumentCaptor.forClass(Notification.class);
        verify(nm).notify(anyString(), anyInt(), notificationArgumentCaptor.capture());
        // if the postedNotification has title set then it must have been the correct notification
        Notification postedNotification = notificationArgumentCaptor.getValue();
        assertTrue(notificationHasTitleSet(postedNotification));
        assertEquals("test4", getNotificationTitle(postedNotification));

        sst.setNotification(ServiceStateTracker.CS_DISABLED);
        verify(nm).cancel(Integer.toString(sst.mSubId), ServiceStateTracker.CS_NOTIFICATION);
        sst.setNotification(ServiceStateTracker.CS_REJECT_CAUSE_ENABLED);
    }

    @Test
    @SmallTest
    public void testSetNotificationsForGroupedSubs() {
        //if subscription is grouped, no notification should be set whatsoever
        int subId = 1;
        int otherSubId = 2;
        sst.mSubId = otherSubId;
        doReturn(subId).when(mSubInfo).getSubscriptionId();

        final NotificationManager nm = (NotificationManager)
                mContext.getSystemService(Context.NOTIFICATION_SERVICE);
        mContextFixture.putBooleanResource(
                R.bool.config_user_notification_of_restrictied_mobile_access, true);
        doReturn(new ApplicationInfo()).when(mContext).getApplicationInfo();
        Drawable mockDrawable = mock(Drawable.class);
        Resources mockResources = mContext.getResources();
        when(mockResources.getDrawable(anyInt(), any())).thenReturn(mockDrawable);

        mContextFixture.putResource(com.android.internal.R.string.RestrictedOnDataTitle, "test1");

        sst.setNotification(ServiceStateTracker.EVENT_NETWORK_STATE_CHANGED);
        ArgumentCaptor<Notification> notificationArgumentCaptor =
                ArgumentCaptor.forClass(Notification.class);
        verify(nm, never()).notify(anyString(), anyInt(), notificationArgumentCaptor.capture());

        sst.setNotification(ServiceStateTracker.PS_DISABLED);
        verify(nm, never()).cancel(Integer.toString(sst.mSubId),
                ServiceStateTracker.PS_NOTIFICATION);
    }

    @Test
    @MediumTest
    public void testRegisterForSubscriptionInfoReady() {
        sst.registerForSubscriptionInfoReady(mTestHandler, EVENT_SUBSCRIPTION_INFO_READY, null);

        // Call functions which would trigger posting of message on test handler
        doReturn(false).when(mPhone).isPhoneTypeGsm();
        sst.updatePhoneType();
        mSimulatedCommands.notifyOtaProvisionStatusChanged();

        waitForMs(200);

        // verify posted message
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler).sendMessageAtTime(messageArgumentCaptor.capture(), anyLong());
        assertEquals(EVENT_SUBSCRIPTION_INFO_READY, messageArgumentCaptor.getValue().what);
    }

    @Test
    @MediumTest
    public void testRoamingPhoneTypeSwitch() {
        // Enable roaming
        doReturn(true).when(mPhone).isPhoneTypeGsm();

        mSimulatedCommands.setVoiceRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.setDataRegState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING);
        mSimulatedCommands.notifyNetworkStateChanged();

        waitForMs(200);

        sst.registerForDataRoamingOff(mTestHandler, EVENT_DATA_ROAMING_OFF, null, true);
        sst.registerForVoiceRoamingOff(mTestHandler, EVENT_VOICE_ROAMING_OFF, null);
        sst.registerForDataConnectionDetached(AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                mTestHandler, EVENT_DATA_CONNECTION_DETACHED, null);

        // Call functions which would trigger posting of message on test handler
        doReturn(false).when(mPhone).isPhoneTypeGsm();
        sst.updatePhoneType();

        // verify if registered handler has message posted to it
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler, atLeast(3)).sendMessageAtTime(
                messageArgumentCaptor.capture(), anyLong());
        HashSet<Integer> messageSet = new HashSet<>();
        for (Message m : messageArgumentCaptor.getAllValues()) {
            messageSet.add(m.what);
        }

        assertTrue(messageSet.contains(EVENT_DATA_ROAMING_OFF));
        assertTrue(messageSet.contains(EVENT_VOICE_ROAMING_OFF));
        assertTrue(messageSet.contains(EVENT_DATA_CONNECTION_DETACHED));
    }

    @Test
    @SmallTest
    public void testGetDesiredPowerState() {
        sst.setRadioPower(true);
        assertEquals(sst.getDesiredPowerState(), true);
    }

    @Test
    @MediumTest
    public void testEnableLocationUpdates() throws Exception {
        sst.enableLocationUpdates();
        verify(mSimulatedCommandsVerifier, times(1)).setLocationUpdates(eq(true),
                any(Message.class));
    }

    @Test
    @SmallTest
    public void testDisableLocationUpdates() throws Exception {
        sst.disableLocationUpdates();
        verify(mSimulatedCommandsVerifier, times(1)).setLocationUpdates(eq(false),
                nullable(Message.class));
    }

    @Test
    @SmallTest
    public void testGetCurrentDataRegState() throws Exception {
        sst.mSS.setDataRegState(ServiceState.STATE_OUT_OF_SERVICE);
        assertEquals(sst.getCurrentDataConnectionState(), ServiceState.STATE_OUT_OF_SERVICE);
    }

    @Test
    @SmallTest
    public void testIsConcurrentVoiceAndDataAllowed() {
        doReturn(false).when(mPhone).isPhoneTypeGsm();
        sst.mSS.setCssIndicator(1);
        assertEquals(true, sst.isConcurrentVoiceAndDataAllowed());
        sst.mSS.setCssIndicator(0);
        assertEquals(false, sst.isConcurrentVoiceAndDataAllowed());

        doReturn(true).when(mPhone).isPhoneTypeGsm();
        NetworkRegistrationInfo nri = new NetworkRegistrationInfo.Builder()
                .setTransportType(AccessNetworkConstants.TRANSPORT_TYPE_WWAN)
                .setAccessNetworkTechnology(TelephonyManager.NETWORK_TYPE_HSPA)
                .setDomain(NetworkRegistrationInfo.DOMAIN_PS)
                .build();
        sst.mSS.addNetworkRegistrationInfo(nri);
        assertEquals(true, sst.isConcurrentVoiceAndDataAllowed());
        nri = new NetworkRegistrationInfo.Builder()
                .setTransportType(AccessNetworkConstants.TRANSPORT_TYPE_WWAN)
                .setAccessNetworkTechnology(TelephonyManager.NETWORK_TYPE_GPRS)
                .setDomain(NetworkRegistrationInfo.DOMAIN_PS)
                .build();
        sst.mSS.addNetworkRegistrationInfo(nri);
        assertEquals(false, sst.isConcurrentVoiceAndDataAllowed());
        sst.mSS.setCssIndicator(1);
        assertEquals(true, sst.isConcurrentVoiceAndDataAllowed());
    }

    @Test
    @MediumTest
    public void testIsImsRegistered() throws Exception {
        mSimulatedCommands.setImsRegistrationState(new int[]{1, PhoneConstants.PHONE_TYPE_GSM});
        mSimulatedCommands.notifyImsNetworkStateChanged();
        waitForMs(200);
        assertEquals(sst.isImsRegistered(), true);
    }

    @Test
    @SmallTest
    public void testIsDeviceShuttingDown() throws Exception {
        sst.requestShutdown();
        assertEquals(true, sst.isDeviceShuttingDown());
    }

    @Test
    @SmallTest
    public void testShuttingDownRequest() throws Exception {
        sst.setRadioPower(true);
        waitForMs(100);

        sst.requestShutdown();
        waitForMs(100);
        assertFalse(mSimulatedCommands.getRadioState()
                != TelephonyManager.RADIO_POWER_UNAVAILABLE);
    }

    @Test
    @SmallTest
    public void testShuttingDownRequestWithRadioPowerFailResponse() throws Exception {
        sst.setRadioPower(true);
        waitForMs(100);

        // Simulate RIL fails the radio power settings.
        mSimulatedCommands.setRadioPowerFailResponse(true);
        sst.setRadioPower(false);
        waitForMs(100);
        assertTrue(mSimulatedCommands.getRadioState() == TelephonyManager.RADIO_POWER_ON);
        sst.requestShutdown();
        waitForMs(100);
        assertFalse(mSimulatedCommands.getRadioState()
                != TelephonyManager.RADIO_POWER_UNAVAILABLE);
    }

    @Test
    @SmallTest
    public void testSetTimeFromNITZStr() throws Exception {
        {
            // Mock sending incorrect nitz str from RIL
            mSimulatedCommands.triggerNITZupdate("38/06/20,00:00:00+0");
            waitForMs(200);
            verify(mNitzStateMachine, times(0)).handleNitzReceived(any());
        }
        {
            // Mock sending correct nitz str from RIL
            String nitzStr = "15/06/20,00:00:00+0";
            NitzData expectedNitzData = NitzData.parse(nitzStr);
            mSimulatedCommands.triggerNITZupdate(nitzStr);
            waitForMs(200);

            ArgumentCaptor<TimestampedValue<NitzData>> argumentsCaptor =
                    ArgumentCaptor.forClass(TimestampedValue.class);
            verify(mNitzStateMachine, times(1))
                    .handleNitzReceived(argumentsCaptor.capture());

            // Confirm the argument was what we expected.
            TimestampedValue<NitzData> actualNitzSignal = argumentsCaptor.getValue();
            assertEquals(expectedNitzData, actualNitzSignal.getValue());
            assertTrue(actualNitzSignal.getReferenceTimeMillis() <= SystemClock.elapsedRealtime());
        }
    }

    private void changeRegState(int state, CellIdentity cid, int rat) {
        changeRegState(state, cid, rat, rat);
    }

    private void changeRegState(int state, CellIdentity cid, int voiceRat, int dataRat) {
        LteVopsSupportInfo lteVopsSupportInfo =
                new LteVopsSupportInfo(LteVopsSupportInfo.LTE_STATUS_NOT_AVAILABLE,
                    LteVopsSupportInfo.LTE_STATUS_NOT_AVAILABLE);
        NetworkRegistrationInfo dataResult = new NetworkRegistrationInfo(
                NetworkRegistrationInfo.DOMAIN_PS, AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                state, dataRat, 0, false,
                null, cid, 1, false, false, false, lteVopsSupportInfo, false);
        sst.mPollingContext[0] = 2;
        // update data reg state to be in service
        sst.sendMessage(sst.obtainMessage(
                ServiceStateTracker.EVENT_POLL_STATE_PS_CELLULAR_REGISTRATION,
                new AsyncResult(sst.mPollingContext, dataResult, null)));
        waitForMs(200);
        NetworkRegistrationInfo voiceResult = new NetworkRegistrationInfo(
                NetworkRegistrationInfo.DOMAIN_CS, AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                state, voiceRat, 0, false,
                null, cid, false, 0, 0, 0);
        sst.sendMessage(sst.obtainMessage(
                ServiceStateTracker.EVENT_POLL_STATE_CS_CELLULAR_REGISTRATION,
                new AsyncResult(sst.mPollingContext, voiceResult, null)));
        waitForMs(200);
    }

    private void changeRegStateWithIwlan(int state, CellIdentity cid, int voiceRat, int dataRat,
            int iwlanState, int iwlanDataRat) {
        LteVopsSupportInfo lteVopsSupportInfo =
                new LteVopsSupportInfo(LteVopsSupportInfo.LTE_STATUS_NOT_AVAILABLE,
                        LteVopsSupportInfo.LTE_STATUS_NOT_AVAILABLE);
        sst.mPollingContext[0] = 3;

        // PS WWAN
        NetworkRegistrationInfo dataResult = new NetworkRegistrationInfo(
                NetworkRegistrationInfo.DOMAIN_PS, AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                state, dataRat, 0, false,
                null, cid, 1, false, false, false, lteVopsSupportInfo, false);
        sst.sendMessage(sst.obtainMessage(
                ServiceStateTracker.EVENT_POLL_STATE_PS_CELLULAR_REGISTRATION,
                new AsyncResult(sst.mPollingContext, dataResult, null)));
        waitForMs(200);

        // CS WWAN
        NetworkRegistrationInfo voiceResult = new NetworkRegistrationInfo(
                NetworkRegistrationInfo.DOMAIN_CS, AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                state, voiceRat, 0, false,
                null, cid, false, 0, 0, 0);
        sst.sendMessage(sst.obtainMessage(
                ServiceStateTracker.EVENT_POLL_STATE_CS_CELLULAR_REGISTRATION,
                new AsyncResult(sst.mPollingContext, voiceResult, null)));
        waitForMs(200);

        // PS WLAN
        NetworkRegistrationInfo dataIwlanResult = new NetworkRegistrationInfo(
                NetworkRegistrationInfo.DOMAIN_PS, AccessNetworkConstants.TRANSPORT_TYPE_WLAN,
                iwlanState, iwlanDataRat, 0, false,
                null, null, 1, false, false, false, lteVopsSupportInfo, false);
        sst.sendMessage(sst.obtainMessage(
                ServiceStateTracker.EVENT_POLL_STATE_PS_IWLAN_REGISTRATION,
                new AsyncResult(sst.mPollingContext, dataIwlanResult, null)));
        waitForMs(200);
    }

    // Edge and GPRS are grouped under the same family and Edge has higher rate than GPRS.
    // Expect no rat update when move from E to G.
    @Test
    public void testRatRatchet() throws Exception {
        CellIdentityGsm cellIdentity =
                new CellIdentityGsm(0, 1, 900, 5, "001", "01", "test", "tst");
        // start on GPRS
        changeRegState(1, cellIdentity, 16, 1);
        assertEquals(ServiceState.STATE_IN_SERVICE, sst.getCurrentDataConnectionState());
        assertEquals(ServiceState.RIL_RADIO_TECHNOLOGY_GPRS, sst.mSS.getRilDataRadioTechnology());
        // upgrade to EDGE
        changeRegState(1, cellIdentity, 16, 2);
        assertEquals(ServiceState.RIL_RADIO_TECHNOLOGY_EDGE, sst.mSS.getRilDataRadioTechnology());
        // drop back to GPRS and expect a ratchet
        changeRegState(1, cellIdentity, 16, 1);
        assertEquals(ServiceState.RIL_RADIO_TECHNOLOGY_EDGE, sst.mSS.getRilDataRadioTechnology());
    }

    // Edge and GPRS are grouped under the same family and Edge has higher rate than GPRS.
    // Bypass rat rachet when cell id changed. Expect rat update from E to G
    @Test
    public void testRatRatchetWithCellChange() throws Exception {
        CellIdentityGsm cellIdentity =
                new CellIdentityGsm(0, 1, 900, 5, "001", "01", "test", "tst");
        // update data reg state to be in service
        changeRegState(1, cellIdentity, 16, 2);
        assertEquals(ServiceState.STATE_IN_SERVICE, sst.getCurrentDataConnectionState());
        assertEquals(ServiceState.RIL_RADIO_TECHNOLOGY_GSM, sst.mSS.getRilVoiceRadioTechnology());
        assertEquals(ServiceState.RIL_RADIO_TECHNOLOGY_EDGE, sst.mSS.getRilDataRadioTechnology());
        // RAT: EDGE -> GPRS cell ID: 1 -> 2
        cellIdentity = new CellIdentityGsm(0, 2, 900, 5, "001", "01", "test", "tst");
        changeRegState(1, cellIdentity, 16, 1);
        assertEquals(ServiceState.RIL_RADIO_TECHNOLOGY_GPRS, sst.mSS.getRilDataRadioTechnology());

    }

    // TODO(nharold): This actually seems like broken behavior; rather than preserve it, we should
    // probably remove it.
    // GSM, Edge, GPRS are grouped under the same family where Edge > GPRS > GSM.
    // Expect no rat update from E to G immediately following cell id change.
    // Expect ratratchet (from G to E) for the following rat update within the cell location.
    @Test
    public void testRatRatchetWithCellChangeBeforeRatChange() throws Exception {
        // cell ID update
        CellIdentityGsm cellIdentity =
                new CellIdentityGsm(0, 1, 900, 5, "001", "01", "test", "tst");
        changeRegState(1, cellIdentity, 16, 2);
        assertEquals(ServiceState.STATE_IN_SERVICE, sst.getCurrentDataConnectionState());
        assertEquals(ServiceState.RIL_RADIO_TECHNOLOGY_EDGE, sst.mSS.getRilDataRadioTechnology());

        // RAT: EDGE -> GPRS, cell ID unchanged. Expect no rat ratchet following cell Id change.
        changeRegState(1, cellIdentity, 16, 1);
        assertEquals(ServiceState.RIL_RADIO_TECHNOLOGY_GPRS, sst.mSS.getRilDataRadioTechnology());

        // RAT: GPRS -> EDGE should ratchet.
        changeRegState(1, cellIdentity, 16, 2);
        assertEquals(ServiceState.RIL_RADIO_TECHNOLOGY_EDGE, sst.mSS.getRilDataRadioTechnology());
    }

    private void sendPhyChanConfigChange(int[] bandwidths) {
        ArrayList<PhysicalChannelConfig> pc = new ArrayList<>();
        int ssType = PhysicalChannelConfig.CONNECTION_PRIMARY_SERVING;
        for (int bw : bandwidths) {
            pc.add(new PhysicalChannelConfig.Builder()
                    .setCellConnectionStatus(ssType)
                    .setCellBandwidthDownlinkKhz(bw)
                    .build());

            // All cells after the first are secondary serving cells.
            ssType = PhysicalChannelConfig.CONNECTION_SECONDARY_SERVING;
        }
        sst.sendMessage(sst.obtainMessage(ServiceStateTracker.EVENT_PHYSICAL_CHANNEL_CONFIG,
                new AsyncResult(null, pc, null)));
        waitForMs(100);
    }

    private void sendRegStateUpdateForLteCellId(CellIdentityLte cellId) {
        LteVopsSupportInfo lteVopsSupportInfo =
                new LteVopsSupportInfo(LteVopsSupportInfo.LTE_STATUS_NOT_AVAILABLE,
                    LteVopsSupportInfo.LTE_STATUS_NOT_AVAILABLE);
        NetworkRegistrationInfo dataResult = new NetworkRegistrationInfo(
                NetworkRegistrationInfo.DOMAIN_PS, AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                NetworkRegistrationInfo.REGISTRATION_STATE_HOME, TelephonyManager.NETWORK_TYPE_LTE,
                0, false, null, cellId, 1, false, false, false, lteVopsSupportInfo, false);
        NetworkRegistrationInfo voiceResult = new NetworkRegistrationInfo(
                NetworkRegistrationInfo.DOMAIN_CS, AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                NetworkRegistrationInfo.REGISTRATION_STATE_HOME, TelephonyManager.NETWORK_TYPE_LTE,
                0, false, null, cellId, false, 0, 0, 0);
        sst.mPollingContext[0] = 2;
        // update data reg state to be in service
        sst.sendMessage(sst.obtainMessage(
                ServiceStateTracker.EVENT_POLL_STATE_PS_CELLULAR_REGISTRATION,
                new AsyncResult(sst.mPollingContext, dataResult, null)));
        waitForMs(200);
        sst.sendMessage(sst.obtainMessage(
                ServiceStateTracker.EVENT_POLL_STATE_CS_CELLULAR_REGISTRATION,
                new AsyncResult(sst.mPollingContext, voiceResult, null)));
        waitForMs(200);
    }

    @Test
    public void testPhyChanBandwidthUpdatedOnDataRegState() throws Exception {
        // Cell ID change should trigger hasLocationChanged.
        CellIdentityLte cellIdentity5 =
                new CellIdentityLte(1, 1, 5, 1, 5000, "001", "01", "test", "tst");

        sendPhyChanConfigChange(new int[] {10000});
        sendRegStateUpdateForLteCellId(cellIdentity5);
        assertTrue(Arrays.equals(new int[] {5000}, sst.mSS.getCellBandwidths()));
    }

    @Test
    public void testPhyChanBandwidthNotUpdatedWhenInvalidInCellIdentity() throws Exception {
        // Cell ID change should trigger hasLocationChanged.
        CellIdentityLte cellIdentityInv =
                new CellIdentityLte(1, 1, 5, 1, 12345, "001", "01", "test", "tst");

        sendPhyChanConfigChange(new int[] {10000});
        sendRegStateUpdateForLteCellId(cellIdentityInv);
        assertTrue(Arrays.equals(new int[] {10000}, sst.mSS.getCellBandwidths()));
    }

    @Test
    public void testPhyChanBandwidthPrefersCarrierAggregationReport() throws Exception {
        // Cell ID change should trigger hasLocationChanged.
        CellIdentityLte cellIdentity10 =
                new CellIdentityLte(1, 1, 5, 1, 10000, "001", "01", "test", "tst");

        sendPhyChanConfigChange(new int[] {10000, 5000});
        sendRegStateUpdateForLteCellId(cellIdentity10);
        assertTrue(Arrays.equals(new int[] {10000, 5000}, sst.mSS.getCellBandwidths()));
    }

    @Test
    public void testPhyChanBandwidthRatchetedOnPhyChanBandwidth() throws Exception {
        // LTE Cell with bandwidth = 10000
        CellIdentityLte cellIdentity10 =
                new CellIdentityLte(1, 1, 1, 1, 10000, "1", "1", "test", "tst");

        sendRegStateUpdateForLteCellId(cellIdentity10);
        assertTrue(Arrays.equals(new int[] {10000}, sst.mSS.getCellBandwidths()));
        sendPhyChanConfigChange(new int[] {10000, 5000});
        assertTrue(Arrays.equals(new int[] {10000, 5000}, sst.mSS.getCellBandwidths()));
    }

    @Test
    public void testPhyChanBandwidthResetsOnOos() throws Exception {
        testPhyChanBandwidthRatchetedOnPhyChanBandwidth();
        LteVopsSupportInfo lteVopsSupportInfo =
                new LteVopsSupportInfo(LteVopsSupportInfo.LTE_STATUS_NOT_AVAILABLE,
                    LteVopsSupportInfo.LTE_STATUS_NOT_AVAILABLE);
        NetworkRegistrationInfo dataResult = new NetworkRegistrationInfo(
                NetworkRegistrationInfo.DOMAIN_PS, AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                NetworkRegistrationInfo.REGISTRATION_STATE_NOT_REGISTERED_OR_SEARCHING,
                TelephonyManager.NETWORK_TYPE_UNKNOWN, 0, false, null, null, 1, false, false,
                false, lteVopsSupportInfo, false);
        NetworkRegistrationInfo voiceResult = new NetworkRegistrationInfo(
                NetworkRegistrationInfo.DOMAIN_CS, AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                NetworkRegistrationInfo.REGISTRATION_STATE_NOT_REGISTERED_OR_SEARCHING,
                TelephonyManager.NETWORK_TYPE_UNKNOWN, 0, false, null, null, false, 0, 0, 0);
        sst.mPollingContext[0] = 2;
        sst.sendMessage(sst.obtainMessage(
                ServiceStateTracker.EVENT_POLL_STATE_PS_CELLULAR_REGISTRATION,
                new AsyncResult(sst.mPollingContext, dataResult, null)));
        waitForMs(200);
        sst.sendMessage(sst.obtainMessage(
                ServiceStateTracker.EVENT_POLL_STATE_CS_CELLULAR_REGISTRATION,
                new AsyncResult(sst.mPollingContext, voiceResult, null)));
        waitForMs(200);
        assertTrue(Arrays.equals(new int[0], sst.mSS.getCellBandwidths()));
    }

    /**
     * Ensure that TransportManager changes due to transport preference changes are picked up in the
     * new ServiceState when a poll event occurs. This causes ServiceState#getRilDataRadioTechnology
     * to change even though the underlying transports have not changed state.
     */
    @SmallTest
    @Test
    public void testRilDataTechnologyChangeTransportPreference() {
        when(mTransportManager.isAnyApnPreferredOnIwlan()).thenReturn(false);

        // Start state: Cell data only LTE + IWLAN
        CellIdentityLte cellIdentity =
                new CellIdentityLte(1, 1, 5, 1, 5000, "001", "01", "test", "tst");
        changeRegStateWithIwlan(
                // WWAN
                NetworkRegistrationInfo.REGISTRATION_STATE_HOME, cellIdentity,
                TelephonyManager.NETWORK_TYPE_UNKNOWN, TelephonyManager.NETWORK_TYPE_LTE,
                // WLAN
                NetworkRegistrationInfo.REGISTRATION_STATE_HOME,
                TelephonyManager.NETWORK_TYPE_IWLAN);
        assertEquals(ServiceState.RIL_RADIO_TECHNOLOGY_LTE, sst.mSS.getRilDataRadioTechnology());

        sst.registerForDataRegStateOrRatChanged(AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                mTestHandler, EVENT_DATA_RAT_CHANGED, null);
        // transport preference change for a PDN for IWLAN occurred, no registration change, but
        // trigger unrelated poll to pick up transport preference.
        when(mTransportManager.isAnyApnPreferredOnIwlan()).thenReturn(true);
        changeRegStateWithIwlan(
                // WWAN
                NetworkRegistrationInfo.REGISTRATION_STATE_HOME, cellIdentity,
                TelephonyManager.NETWORK_TYPE_UNKNOWN, TelephonyManager.NETWORK_TYPE_LTE,
                // WLAN
                NetworkRegistrationInfo.REGISTRATION_STATE_HOME,
                TelephonyManager.NETWORK_TYPE_IWLAN);
        // Now check to make sure a transport independent notification occurred for the registrants.
        // There will be two, one when the registration happened and another when the transport
        // preference changed.
        ArgumentCaptor<Message> messageArgumentCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mTestHandler, times(2)).sendMessageAtTime(messageArgumentCaptor.capture(),
                anyLong());
        assertEquals(ServiceState.RIL_RADIO_TECHNOLOGY_IWLAN, sst.mSS.getRilDataRadioTechnology());
    }

    @Test
    public void testGetServiceProviderNameWithBrandOverride() {
        String brandOverride = "spn from brand override";
        doReturn(brandOverride).when(mUiccProfile).getOperatorBrandOverride();

        assertThat(sst.getServiceProviderName()).isEqualTo(brandOverride);
    }

    @Test
    public void testGetServiceProviderNameWithCarrierConfigOverride() {
        String carrierOverride = "spn from carrier override";
        mBundle.putBoolean(CarrierConfigManager.KEY_CARRIER_NAME_OVERRIDE_BOOL, true);
        mBundle.putString(CarrierConfigManager.KEY_CARRIER_NAME_STRING, carrierOverride);

        assertThat(sst.getServiceProviderName()).isEqualTo(carrierOverride);
    }

    @Test
    public void testGetServiceProviderNameWithSimRecord() {
        String spn = "spn from sim record";
        doReturn(spn).when(mSimRecords).getServiceProviderName();

        assertThat(sst.getServiceProviderName()).isEqualTo(spn);
    }

    @Test
    public void testGetServiceProviderNameWithAllSource() {
        String brandOverride = "spn from brand override";
        doReturn(brandOverride).when(mUiccProfile).getOperatorBrandOverride();

        String carrierOverride = "spn from carrier override";
        mBundle.putBoolean(CarrierConfigManager.KEY_CARRIER_NAME_OVERRIDE_BOOL, true);
        mBundle.putString(CarrierConfigManager.KEY_CARRIER_NAME_STRING, carrierOverride);

        String spn = "spn from sim record";
        doReturn(spn).when(mSimRecords).getServiceProviderName();

        // Operator brand override has highest priority
        assertThat(sst.getServiceProviderName()).isEqualTo(brandOverride);

        // Remove the brand override
        doReturn(null).when(mUiccProfile).getOperatorBrandOverride();

        // Carrier config override has 2nd priority
        assertThat(sst.getServiceProviderName()).isEqualTo(carrierOverride);

        // Remove the carrier config override
        mBundle.putBoolean(CarrierConfigManager.KEY_CARRIER_NAME_OVERRIDE_BOOL, false);

        // SPN from sim has lowest priority
        assertThat(sst.getServiceProviderName()).isEqualTo(spn);
    }

    @Test
    public void testGetCarrierNameDisplayConditionWithBrandOverride() {
        String brandOverride = "spn from brand override";
        doReturn(brandOverride).when(mUiccProfile).getOperatorBrandOverride();

        // Only show spn because all PLMNs will be considered HOME PLMNs.
        assertThat(sst.getCarrierNameDisplayBitmask(new ServiceState())).isEqualTo(
                ServiceStateTracker.CARRIER_NAME_DISPLAY_BITMASK_SHOW_SPN);
    }

    @Test
    @SmallTest
    public void testGetMdn() throws Exception {
        doReturn(false).when(mPhone).isPhoneTypeGsm();
        doReturn(false).when(mPhone).isPhoneTypeCdma();
        doReturn(true).when(mPhone).isPhoneTypeCdmaLte();
        doReturn(CdmaSubscriptionSourceManager.SUBSCRIPTION_FROM_RUIM).when(mCdmaSSM)
                .getCdmaSubscriptionSource();

        logd("Calling updatePhoneType");
        // switch to CDMA
        sst.updatePhoneType();

        // trigger RUIM_RECORDS_LOADED
        ArgumentCaptor<Integer> integerArgumentCaptor = ArgumentCaptor.forClass(Integer.class);
        verify(mRuimRecords).registerForRecordsLoaded(eq(sst), integerArgumentCaptor.capture(),
                nullable(Object.class));

        // response for mRuimRecords.registerForRecordsLoaded()
        Message msg = Message.obtain();
        msg.what = integerArgumentCaptor.getValue();
        msg.obj = new AsyncResult(null, null, null);
        sst.sendMessage(msg);

        // wait for RUIM_RECORDS_LOADED to be handled
        waitForHandlerAction(sst, 5000);

        // mdn should be null as nothing populated it
        assertEquals(null, sst.getMdnNumber());

        // if ruim is provisioned, mdn should still be null
        doReturn(true).when(mRuimRecords).isProvisioned();
        assertEquals(null, sst.getMdnNumber());

        // if ruim is not provisioned, and mdn is non null, sst should still return the correct
        // value
        doReturn(false).when(mRuimRecords).isProvisioned();
        String mockMdn = "mockMdn";
        doReturn(mockMdn).when(mRuimRecords).getMdn();

        // trigger RUIM_RECORDS_LOADED
        Message msg1 = Message.obtain();
        msg1.what = integerArgumentCaptor.getValue();
        msg1.obj = new AsyncResult(null, null, null);
        sst.sendMessage(msg1);

        // wait for RUIM_RECORDS_LOADED to be handled
        waitForHandlerAction(sst, 5000);

        assertEquals(mockMdn, sst.getMdnNumber());
    }

    @Test
    @SmallTest
    public void testOnVopsInfoChanged() {
        ServiceState ss = new ServiceState();
        ss.setVoiceRegState(ServiceState.STATE_IN_SERVICE);
        ss.setDataRegState(ServiceState.STATE_IN_SERVICE);
        sst.mSS = ss;

        CellIdentityLte cellId =
                new CellIdentityLte(1, 1, 5, 1, 5000, "001", "01", "test", "tst");
        LteVopsSupportInfo lteVopsSupportInfo =
                new LteVopsSupportInfo(LteVopsSupportInfo.LTE_STATUS_NOT_SUPPORTED,
                    LteVopsSupportInfo.LTE_STATUS_NOT_SUPPORTED);

        NetworkRegistrationInfo dataResult = new NetworkRegistrationInfo(
                NetworkRegistrationInfo.DOMAIN_PS, AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                NetworkRegistrationInfo.REGISTRATION_STATE_HOME, TelephonyManager.NETWORK_TYPE_LTE,
                0, false, null, cellId, 1, false, false, false, lteVopsSupportInfo, false);
        sst.mPollingContext[0] = 2;

        sst.sendMessage(sst.obtainMessage(
                ServiceStateTracker.EVENT_POLL_STATE_PS_CELLULAR_REGISTRATION,
                new AsyncResult(sst.mPollingContext, dataResult, null)));
        NetworkRegistrationInfo voiceResult = new NetworkRegistrationInfo(
                NetworkRegistrationInfo.DOMAIN_CS, AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                NetworkRegistrationInfo.REGISTRATION_STATE_HOME,
                TelephonyManager.NETWORK_TYPE_LTE, 0,
                false, null, cellId, false, 0, 0, 0);
        sst.sendMessage(sst.obtainMessage(
                ServiceStateTracker.EVENT_POLL_STATE_CS_CELLULAR_REGISTRATION,
                new AsyncResult(sst.mPollingContext, voiceResult, null)));

        waitForMs(200);
        assertEquals(ServiceState.STATE_IN_SERVICE, sst.getCurrentDataConnectionState());
        NetworkRegistrationInfo sSnetworkRegistrationInfo =
                sst.mSS.getNetworkRegistrationInfo(NetworkRegistrationInfo.DOMAIN_PS,
                        AccessNetworkConstants.TRANSPORT_TYPE_WWAN);
        assertEquals(lteVopsSupportInfo,
                sSnetworkRegistrationInfo.getDataSpecificInfo().getLteVopsSupportInfo());

        lteVopsSupportInfo =
                new LteVopsSupportInfo(LteVopsSupportInfo.LTE_STATUS_SUPPORTED,
                    LteVopsSupportInfo.LTE_STATUS_NOT_SUPPORTED);
        dataResult = new NetworkRegistrationInfo(NetworkRegistrationInfo.DOMAIN_PS,
                AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                NetworkRegistrationInfo.REGISTRATION_STATE_HOME,
                TelephonyManager.NETWORK_TYPE_LTE, 0, false, null, cellId, 1, false, false, false,
                lteVopsSupportInfo, false);
        sst.mPollingContext[0] = 1;
        sst.sendMessage(sst.obtainMessage(
                ServiceStateTracker.EVENT_POLL_STATE_PS_CELLULAR_REGISTRATION,
                new AsyncResult(sst.mPollingContext, dataResult, null)));
        waitForMs(200);

        sSnetworkRegistrationInfo =
                sst.mSS.getNetworkRegistrationInfo(2, 1);
        assertEquals(lteVopsSupportInfo,
                sSnetworkRegistrationInfo.getDataSpecificInfo().getLteVopsSupportInfo());
    }

    @Test
    @SmallTest
    public void testEriLoading() {
        sst.obtainMessage(GsmCdmaPhone.EVENT_CARRIER_CONFIG_CHANGED, null).sendToTarget();
        waitForMs(100);
        verify(mEriManager, times(1)).loadEriFile();
    }

    private void enableCdnr() {
        mBundle.putBoolean(
                CarrierConfigManager.KEY_ENABLE_CARRIER_DISPLAY_NAME_RESOLVER_BOOL, true);
        sendCarrierConfigUpdate();
    }

    @Test
    public void testUpdateSpnDisplay_noService_displayEmergencyCallOnly() {
        enableCdnr();

        // GSM phone
        doReturn(true).when(mPhone).isPhoneTypeGsm();

        // Emergency call only
        ServiceState ss = new ServiceState();
        ss.setVoiceRegState(ServiceState.STATE_EMERGENCY_ONLY);
        ss.setDataRegState(ServiceState.STATE_OUT_OF_SERVICE);
        ss.setEmergencyOnly(true);
        doReturn(ss).when(mSST).getServiceState();

        // update the spn
        sst.updateSpnDisplay();

        // Plmn should be shown, and the string is "Emergency call only"
        Bundle b = getExtrasFromLastSpnUpdateIntent();
        assertThat(b.getString(TelephonyIntents.EXTRA_PLMN))
                .isEqualTo(CARRIER_NAME_DISPLAY_EMERGENCY_CALL);
        assertThat(b.getBoolean(TelephonyIntents.EXTRA_SHOW_PLMN)).isTrue();
    }

    @Test
    public void testUpdateSpnDisplay_noServiceAndEmergencyCallNotAvailable_displayOOS() {
        enableCdnr();

        // GSM phone
        doReturn(true).when(mPhone).isPhoneTypeGsm();

        // Completely out of service
        ServiceState ss = new ServiceState();
        ss.setVoiceRegState(ServiceState.STATE_OUT_OF_SERVICE);
        ss.setDataRegState(ServiceState.STATE_OUT_OF_SERVICE);
        ss.setEmergencyOnly(false);
        doReturn(ss).when(mSST).getServiceState();

        // update the spn
        sst.updateSpnDisplay();

        // Plmn should be shown, and the string is "No service"
        Bundle b = getExtrasFromLastSpnUpdateIntent();
        assertThat(b.getString(TelephonyIntents.EXTRA_PLMN))
                .isEqualTo(CARRIER_NAME_DISPLAY_NO_SERVICE);
        assertThat(b.getBoolean(TelephonyIntents.EXTRA_SHOW_PLMN)).isTrue();
    }

    @Test
    public void testUpdateSpnDisplay_flightMode_displayOOS() {
        enableCdnr();

        // GSM phone
        doReturn(true).when(mPhone).isPhoneTypeGsm();

        // Flight mode
        ServiceState ss = new ServiceState();
        ss.setVoiceRegState(ServiceState.STATE_POWER_OFF);
        ss.setDataRegState(ServiceState.STATE_POWER_OFF);
        doReturn(ss).when(mSST).getServiceState();

        // update the spn
        sst.updateSpnDisplay();

        // Plmn should be shown, and the string is "No service"
        Bundle b = getExtrasFromLastSpnUpdateIntent();
        assertThat(b.getString(TelephonyIntents.EXTRA_PLMN))
                .isEqualTo(CARRIER_NAME_DISPLAY_NO_SERVICE);
        assertThat(b.getBoolean(TelephonyIntents.EXTRA_SHOW_PLMN)).isTrue();
    }

    @Test
    public void testUpdateSpnDisplay_spnNotEmptyAndWifiCallingEnabled_showSpnOnly() {
        enableCdnr();

        // GSM phone
        doReturn(true).when(mPhone).isPhoneTypeGsm();

        // In Service
        ServiceState ss = new ServiceState();
        ss.setVoiceRegState(ServiceState.STATE_IN_SERVICE);
        ss.setDataRegState(ServiceState.STATE_IN_SERVICE);
        sst.mSS = ss;

        // wifi-calling is enabled
        doReturn(true).when(mPhone).isWifiCallingEnabled();

        // update the spn
        sst.updateSpnDisplay();

        // Only spn should be shown
        String spn = mBundle.getString(CarrierConfigManager.KEY_CARRIER_NAME_STRING);
        Bundle b = getExtrasFromLastSpnUpdateIntent();
        assertThat(b.getString(TelephonyIntents.EXTRA_SPN))
                .isEqualTo(String.format(WIFI_CALLING_VOICE_FORMAT, spn));
        assertThat(b.getBoolean(TelephonyIntents.EXTRA_SHOW_SPN)).isTrue();
        assertThat(b.getString(TelephonyIntents.EXTRA_DATA_SPN))
                .isEqualTo(String.format(WIFI_CALLING_DATA_FORMAT, spn));
        assertThat(b.getBoolean(TelephonyIntents.EXTRA_SHOW_PLMN)).isFalse();
    }

    @Test
    public void testUpdateSpnDisplay_spnEmptyAndWifiCallingEnabled_showPlmnOnly() {
        // set empty service provider name
        mBundle.putString(CarrierConfigManager.KEY_CARRIER_NAME_STRING, "");

        enableCdnr();

        // GSM phone
        doReturn(true).when(mPhone).isPhoneTypeGsm();

        // In Service
        ServiceState ss = new ServiceState();
        ss.setVoiceRegState(ServiceState.STATE_IN_SERVICE);
        ss.setDataRegState(ServiceState.STATE_IN_SERVICE);
        sst.mSS = ss;

        // wifi-calling is enabled
        doReturn(true).when(mPhone).isWifiCallingEnabled();

        // update the spn
        sst.updateSpnDisplay();

        // Only plmn should be shown
        String plmn = mBundle.getStringArray(CarrierConfigManager.KEY_PNN_OVERRIDE_STRING_ARRAY)[0];
        plmn = plmn.split("\\s*,\\s*")[0];
        Bundle b = getExtrasFromLastSpnUpdateIntent();
        assertThat(b.getString(TelephonyIntents.EXTRA_PLMN))
                .isEqualTo(String.format(WIFI_CALLING_VOICE_FORMAT, plmn));
        assertThat(b.getBoolean(TelephonyIntents.EXTRA_SHOW_PLMN)).isTrue();
        assertThat(b.getBoolean(TelephonyIntents.EXTRA_SHOW_SPN)).isFalse();
    }

    @Test
    public void testUpdateSpnDisplay_inServiceNoWifiCalling_showSpnAndPlmn() {
        enableCdnr();

        // GSM phone
        doReturn(true).when(mPhone).isPhoneTypeGsm();

        ServiceState ss = new ServiceState();
        ss.setVoiceRegState(ServiceState.STATE_IN_SERVICE);
        ss.setDataRegState(ServiceState.STATE_IN_SERVICE);
        sst.mSS = ss;

        // wifi-calling is disable
        doReturn(false).when(mPhone).isWifiCallingEnabled();

        // update the spn
        sst.updateSpnDisplay();

        // Show both spn & plmn
        String spn = mBundle.getString(CarrierConfigManager.KEY_CARRIER_NAME_STRING);
        String plmn = mBundle.getStringArray(CarrierConfigManager.KEY_PNN_OVERRIDE_STRING_ARRAY)[0];
        plmn = plmn.split("\\s*,\\s*")[0];
        Bundle b = getExtrasFromLastSpnUpdateIntent();
        assertThat(b.getString(TelephonyIntents.EXTRA_SPN)).isEqualTo(spn);
        assertThat(b.getBoolean(TelephonyIntents.EXTRA_SHOW_SPN)).isTrue();
        assertThat(b.getString(TelephonyIntents.EXTRA_PLMN)).isEqualTo(plmn);
        assertThat(b.getBoolean(TelephonyIntents.EXTRA_SHOW_PLMN)).isTrue();
    }

    @Test
    public void testShouldForceDisplayNoService_forceBasedOnLocale() {
        // set up unaffected locale (US) and clear the resource
        doReturn("us").when(mLocaleTracker).getCurrentCountry();
        mContextFixture.putStringArrayResource(
                com.android.internal.R.array.config_display_no_service_when_sim_unready,
                new String[0]);
        assertFalse(sst.shouldForceDisplayNoService());

        // set up the resource to include Germany
        mContextFixture.putStringArrayResource(
                com.android.internal.R.array.config_display_no_service_when_sim_unready,
                new String[]{"de"});
        doReturn("us").when(mLocaleTracker).getCurrentCountry();
        assertFalse(sst.shouldForceDisplayNoService());

        // mock the locale to Germany
        doReturn("de").when(mLocaleTracker).getCurrentCountry();
        assertTrue(sst.shouldForceDisplayNoService());
    }

    private Bundle getExtrasFromLastSpnUpdateIntent() {
        // Verify the spn update notification was sent
        ArgumentCaptor<Intent> intentArgumentCaptor = ArgumentCaptor.forClass(Intent.class);
        verify(mContextFixture.getTestDouble(), atLeast(1))
                .sendStickyBroadcastAsUser(intentArgumentCaptor.capture(), eq(UserHandle.ALL));

        List<Intent> intents = intentArgumentCaptor.getAllValues();
        return intents.get(intents.size() - 1).getExtras();
    }
}
