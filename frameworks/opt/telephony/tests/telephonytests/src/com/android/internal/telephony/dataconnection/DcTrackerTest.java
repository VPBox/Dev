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

package com.android.internal.telephony.dataconnection;

import static com.android.internal.telephony.TelephonyTestUtils.waitForMs;
import static com.android.internal.telephony.dataconnection.ApnSettingTest.createApnSetting;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyInt;
import static org.mockito.Matchers.anyLong;
import static org.mockito.Matchers.anyString;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.atLeastOnce;
import static org.mockito.Mockito.clearInvocations;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ServiceInfo;
import android.database.Cursor;
import android.database.MatrixCursor;
import android.hardware.radio.V1_0.SetupDataCallResult;
import android.net.LinkProperties;
import android.net.NetworkAgent;
import android.net.NetworkCapabilities;
import android.net.NetworkRequest;
import android.net.Uri;
import android.os.AsyncResult;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Message;
import android.os.PersistableBundle;
import android.provider.Settings;
import android.provider.Telephony;
import android.telephony.AccessNetworkConstants;
import android.telephony.AccessNetworkConstants.AccessNetworkType;
import android.telephony.CarrierConfigManager;
import android.telephony.NetworkRegistrationInfo;
import android.telephony.ServiceState;
import android.telephony.SignalStrength;
import android.telephony.SubscriptionInfo;
import android.telephony.SubscriptionManager;
import android.telephony.TelephonyManager;
import android.telephony.data.ApnSetting;
import android.telephony.data.DataProfile;
import android.telephony.data.DataService;
import android.test.mock.MockContentProvider;
import android.test.mock.MockContentResolver;
import android.test.suitebuilder.annotation.MediumTest;
import android.test.suitebuilder.annotation.SmallTest;
import android.text.TextUtils;
import android.util.Pair;

import androidx.test.filters.FlakyTest;

import com.android.internal.R;
import com.android.internal.telephony.DctConstants;
import com.android.internal.telephony.ISub;
import com.android.internal.telephony.PhoneConstants;
import com.android.internal.telephony.TelephonyTest;
import com.android.server.pm.PackageManagerService;

import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;

public class DcTrackerTest extends TelephonyTest {

    private final static String[] sNetworkAttributes = new String[]{
            "mobile,0,0,0,-1,true", "mobile_mms,2,0,2,60000,true",
            "mobile_supl,3,0,2,60000,true", "mobile_dun,4,0,2,60000,true",
            "mobile_hipri,5,0,3,60000,true", "mobile_fota,10,0,2,60000,true",
            "mobile_ims,11,0,2,60000,true", "mobile_cbs,12,0,2,60000,true",
            "mobile_ia,14,0,2,-1,true", "mobile_emergency,15,0,2,-1,true"};

    public static final String FAKE_APN1 = "FAKE APN 1";
    public static final String FAKE_APN2 = "FAKE APN 2";
    public static final String FAKE_APN3 = "FAKE APN 3";
    public static final String FAKE_APN4 = "FAKE APN 4";
    public static final String FAKE_APN5 = "FAKE APN 5";
    public static final String FAKE_APN6 = "FAKE APN 6";
    public static final String FAKE_IFNAME = "FAKE IFNAME";
    public static final String FAKE_PCSCF_ADDRESS = "22.33.44.55";
    public static final String FAKE_GATEWAY = "11.22.33.44";
    public static final String FAKE_DNS = "55.66.77.88";
    public static final String FAKE_ADDRESS = "99.88.77.66";
    private static final int NETWORK_TYPE_LTE_BITMASK =
            1 << (TelephonyManager.NETWORK_TYPE_LTE - 1);
    private static final int NETWORK_TYPE_EHRPD_BITMASK =
            1 << (TelephonyManager.NETWORK_TYPE_EHRPD - 1);
    private static final Uri PREFERAPN_URI = Uri.parse(
            Telephony.Carriers.CONTENT_URI + "/preferapn");
    private static final int DATA_ENABLED_CHANGED = 0;
    private static final String FAKE_PLMN = "44010";
    private static final long TEST_TIMEOUT = 1000;

    @Mock
    ISub mIsub;
    @Mock
    IBinder mBinder;
    @Mock
    SubscriptionInfo mSubscriptionInfo;
    @Mock
    ApnContext mApnContext;
    @Mock
    DataConnection mDataConnection;
    @Mock
    PackageManagerService mMockPackageManagerInternal;
    @Mock
    Handler mHandler;

    private DcTracker mDct;
    private DcTrackerTestHandler mDcTrackerTestHandler;

    private AlarmManager mAlarmManager;

    private PersistableBundle mBundle;

    private SubscriptionManager.OnSubscriptionsChangedListener mOnSubscriptionsChangedListener;

    private final ApnSettingContentProvider mApnSettingContentProvider =
            new ApnSettingContentProvider();

    private Message mMessage;

    private void addDataService() {
        CellularDataService cellularDataService = new CellularDataService();
        ServiceInfo serviceInfo = new ServiceInfo();
        serviceInfo.packageName = "com.android.phone";
        serviceInfo.permission = "android.permission.BIND_TELEPHONY_DATA_SERVICE";
        IntentFilter filter = new IntentFilter();
        mContextFixture.addService(
                DataService.SERVICE_INTERFACE,
                null,
                "com.android.phone",
                cellularDataService.mBinder,
                serviceInfo,
                filter);
    }

    private class DcTrackerTestHandler extends HandlerThread {

        private DcTrackerTestHandler(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            mDct = new DcTracker(mPhone, AccessNetworkConstants.TRANSPORT_TYPE_WWAN);
            setReady(true);
        }
    }

    private class ApnSettingContentProvider extends MockContentProvider {
        private int mPreferredApnSet = 0;

        @Override
        public Cursor query(Uri uri, String[] projection, String selection, String[] selectionArgs,
                            String sortOrder) {
            logd("ApnSettingContentProvider: query");
            logd("   uri = " + uri);
            logd("   projection = " + Arrays.toString(projection));
            logd("   selection = " + selection);
            logd("   selectionArgs = " + Arrays.toString(selectionArgs));
            logd("   sortOrder = " + sortOrder);

            if (uri.compareTo(Telephony.Carriers.CONTENT_URI) == 0
                    || uri.toString().startsWith(Uri.withAppendedPath(
                            Telephony.Carriers.CONTENT_URI, "filtered").toString())
                    || uri.toString().startsWith(Uri.withAppendedPath(
                            Telephony.Carriers.SIM_APN_URI, "filtered").toString())) {
                if (projection == null) {

                    logd("Query '" + FAKE_PLMN + "' APN settings");
                    MatrixCursor mc = new MatrixCursor(
                            new String[]{Telephony.Carriers._ID, Telephony.Carriers.NUMERIC,
                                    Telephony.Carriers.NAME, Telephony.Carriers.APN,
                                    Telephony.Carriers.PROXY, Telephony.Carriers.PORT,
                                    Telephony.Carriers.MMSC, Telephony.Carriers.MMSPROXY,
                                    Telephony.Carriers.MMSPORT, Telephony.Carriers.USER,
                                    Telephony.Carriers.PASSWORD, Telephony.Carriers.AUTH_TYPE,
                                    Telephony.Carriers.TYPE,
                                    Telephony.Carriers.PROTOCOL,
                                    Telephony.Carriers.ROAMING_PROTOCOL,
                                    Telephony.Carriers.CARRIER_ENABLED, Telephony.Carriers.BEARER,
                                    Telephony.Carriers.BEARER_BITMASK,
                                    Telephony.Carriers.PROFILE_ID,
                                    Telephony.Carriers.MODEM_PERSIST,
                                    Telephony.Carriers.MAX_CONNECTIONS,
                                    Telephony.Carriers.WAIT_TIME_RETRY,
                                    Telephony.Carriers.TIME_LIMIT_FOR_MAX_CONNECTIONS,
                                    Telephony.Carriers.MTU,
                                    Telephony.Carriers.MVNO_TYPE,
                                    Telephony.Carriers.MVNO_MATCH_DATA,
                                    Telephony.Carriers.NETWORK_TYPE_BITMASK,
                                    Telephony.Carriers.APN_SET_ID,
                                    Telephony.Carriers.CARRIER_ID,
                                    Telephony.Carriers.SKIP_464XLAT});

                    mc.addRow(new Object[]{
                            2163,                   // id
                            FAKE_PLMN,              // numeric
                            "sp-mode",              // name
                            FAKE_APN1,              // apn
                            "",                     // proxy
                            "",                     // port
                            "",                     // mmsc
                            "",                     // mmsproxy
                            "",                     // mmsport
                            "",                     // user
                            "",                     // password
                            -1,                     // authtype
                            "default,supl",         // types
                            "IP",                   // protocol
                            "IP",                   // roaming_protocol
                            1,                      // carrier_enabled
                            ServiceState.RIL_RADIO_TECHNOLOGY_LTE, // bearer
                            0,                      // bearer_bitmask
                            0,                      // profile_id
                            1,                      // modem_cognitive
                            0,                      // max_conns
                            0,                      // wait_time
                            0,                      // max_conns_time
                            0,                      // mtu
                            "",                     // mvno_type
                            "",                     // mnvo_match_data
                            NETWORK_TYPE_LTE_BITMASK, // network_type_bitmask
                            0,                      // apn_set_id
                            -1,                     // carrier_id
                            -1                      // skip_464xlat
                    });

                    mc.addRow(new Object[]{
                            2164,                   // id
                            FAKE_PLMN,              // numeric
                            "mopera U",             // name
                            FAKE_APN2,              // apn
                            "",                     // proxy
                            "",                     // port
                            "",                     // mmsc
                            "",                     // mmsproxy
                            "",                     // mmsport
                            "",                     // user
                            "",                     // password
                            -1,                     // authtype
                            "default,supl",         // types
                            "IP",                   // protocol
                            "IP",                   // roaming_protocol
                            1,                      // carrier_enabled
                            ServiceState.RIL_RADIO_TECHNOLOGY_LTE, // bearer,
                            0,                      // bearer_bitmask
                            0,                      // profile_id
                            1,                      // modem_cognitive
                            0,                      // max_conns
                            0,                      // wait_time
                            0,                      // max_conns_time
                            0,                      // mtu
                            "",                     // mvno_type
                            "",                     // mnvo_match_data
                            NETWORK_TYPE_LTE_BITMASK, // network_type_bitmask
                            0,                      // apn_set_id
                            -1,                     // carrier_id
                            -1                      // skip_464xlat
                    });

                    mc.addRow(new Object[]{
                            2165,                   // id
                            FAKE_PLMN,              // numeric
                            "b-mobile for Nexus",   // name
                            FAKE_APN3,              // apn
                            "",                     // proxy
                            "",                     // port
                            "",                     // mmsc
                            "",                     // mmsproxy
                            "",                     // mmsport
                            "",                     // user
                            "",                     // password
                            -1,                     // authtype
                            "ims",                  // types
                            "IP",                   // protocol
                            "IP",                   // roaming_protocol
                            1,                      // carrier_enabled
                            0,                      // bearer
                            0,                      // bearer_bitmask
                            0,                      // profile_id
                            1,                      // modem_cognitive
                            0,                      // max_conns
                            0,                      // wait_time
                            0,                      // max_conns_time
                            0,                      // mtu
                            "",                     // mvno_type
                            "",                     // mnvo_match_data
                            0,                      // network_type_bitmask
                            0,                      // apn_set_id
                            -1,                     // carrier_id
                            -1                      // skip_464xlat
                    });

                    mc.addRow(new Object[]{
                            2166,                   // id
                            FAKE_PLMN,              // numeric
                            "sp-mode ehrpd",        // name
                            FAKE_APN4,              // apn
                            "",                     // proxy
                            "",                     // port
                            "",                     // mmsc
                            "",                     // mmsproxy
                            "",                     // mmsport
                            "",                     // user
                            "",                     // password
                            -1,                     // authtype
                            "default,supl",         // types
                            "IP",                   // protocol
                            "IP",                   // roaming_protocol
                            1,                      // carrier_enabled
                            ServiceState.RIL_RADIO_TECHNOLOGY_EHRPD, // bearer
                            0,                      // bearer_bitmask
                            0,                      // profile_id
                            1,                      // modem_cognitive
                            0,                      // max_conns
                            0,                      // wait_time
                            0,                      // max_conns_time
                            0,                      // mtu
                            "",                     // mvno_type
                            "",                     // mnvo_match_data
                            NETWORK_TYPE_EHRPD_BITMASK, // network_type_bitmask
                            0,                      // apn_set_id
                            -1,                     // carrier_id
                            -1                      // skip_464xlat
                    });

                    mc.addRow(new Object[]{
                            2167,                   // id
                            FAKE_PLMN,              // numeric
                            "b-mobile for Nexus",   // name
                            FAKE_APN5,              // apn
                            "",                     // proxy
                            "",                     // port
                            "",                     // mmsc
                            "",                     // mmsproxy
                            "",                     // mmsport
                            "",                     // user
                            "",                     // password
                            -1,                     // authtype
                            "dun",                  // types
                            "IP",                   // protocol
                            "IP",                   // roaming_protocol
                            1,                      // carrier_enabled
                            0,                      // bearer
                            0,                      // bearer_bitmask
                            0,                      // profile_id
                            1,                      // modem_cognitive
                            0,                      // max_conns
                            0,                      // wait_time
                            0,                      // max_conns_time
                            0,                      // mtu
                            "",                     // mvno_type
                            "",                     // mnvo_match_data
                            0,                      // network_type_bitmask
                            0,                      // apn_set_id
                            -1,                     // carrier_id
                            -1                      // skip_464xlat
                    });

                    mc.addRow(new Object[]{
                            2168,                   // id
                            FAKE_PLMN,              // numeric
                            "sp-mode",              // name
                            FAKE_APN6,              // apn
                            "",                     // proxy
                            "",                     // port
                            "",                     // mmsc
                            "",                     // mmsproxy
                            "",                     // mmsport
                            "",                     // user
                            "",                     // password
                            -1,                     // authtype
                            "mms",                  // types
                            "IP",                   // protocol
                            "IP",                   // roaming_protocol
                            1,                      // carrier_enabled
                            ServiceState.RIL_RADIO_TECHNOLOGY_LTE, // bearer
                            0,                      // bearer_bitmask
                            0,                      // profile_id
                            1,                      // modem_cognitive
                            0,                      // max_conns
                            0,                      // wait_time
                            0,                      // max_conns_time
                            0,                      // mtu
                            "",                     // mvno_type
                            "",                     // mnvo_match_data
                            NETWORK_TYPE_LTE_BITMASK, // network_type_bitmask
                            0,                      // apn_set_id
                            -1,                     // carrier_id
                            -1                      // skip_464xlat
                    });

                    return mc;
                }
            } else if (uri.isPathPrefixMatch(
                    Uri.withAppendedPath(Telephony.Carriers.CONTENT_URI, "preferapnset"))) {
                MatrixCursor mc = new MatrixCursor(
                        new String[]{Telephony.Carriers.APN_SET_ID});
                // apn_set_id is the only field used with this URL
                mc.addRow(new Object[]{ mPreferredApnSet });
                mc.addRow(new Object[]{ 0 });
                return mc;
            }

            return null;
        }

        @Override
        public int update(Uri url, ContentValues values, String where, String[] whereArgs) {
            mPreferredApnSet = values.getAsInteger(Telephony.Carriers.APN_SET_ID);
            return 1;
        }
    }

    @Before
    public void setUp() throws Exception {
        logd("DcTrackerTest +Setup!");
        super.setUp(getClass().getSimpleName());

        doReturn("fake.action_detached").when(mPhone).getActionDetached();
        doReturn("fake.action_attached").when(mPhone).getActionAttached();
        doReturn(ServiceState.RIL_RADIO_TECHNOLOGY_LTE).when(mServiceState)
                .getRilDataRadioTechnology();

        mContextFixture.putStringArrayResource(com.android.internal.R.array.networkAttributes,
                sNetworkAttributes);
        mContextFixture.putStringArrayResource(com.android.internal.R.array.
                config_mobile_tcp_buffers, new String[]{
                "umts:131072,262144,1452032,4096,16384,399360",
                "hspa:131072,262144,2441216,4096,16384,399360",
                "hsupa:131072,262144,2441216,4096,16384,399360",
                "hsdpa:131072,262144,2441216,4096,16384,399360",
                "hspap:131072,262144,2441216,4096,16384,399360",
                "edge:16384,32768,131072,4096,16384,65536",
                "gprs:4096,8192,24576,4096,8192,24576",
                "1xrtt:16384,32768,131070,4096,16384,102400",
                "evdo:131072,262144,1048576,4096,16384,524288",
                "lte:524288,1048576,8388608,262144,524288,4194304"});

        mContextFixture.putResource(R.string.config_wwan_data_service_package,
                "com.android.phone");

        ((MockContentResolver) mContext.getContentResolver()).addProvider(
                Telephony.Carriers.CONTENT_URI.getAuthority(), mApnSettingContentProvider);
        Settings.Global.putInt(mContext.getContentResolver(),
                Settings.Global.DATA_STALL_RECOVERY_ON_BAD_NETWORK, 0);

        doReturn(true).when(mSimRecords).getRecordsLoaded();
        doReturn(PhoneConstants.State.IDLE).when(mCT).getState();
        doReturn(true).when(mSST).getDesiredPowerState();
        doReturn(true).when(mSST).getPowerStateFromCarrier();
        doAnswer(
                new Answer<Void>() {
                    @Override
                    public Void answer(InvocationOnMock invocation) throws Throwable {
                        mOnSubscriptionsChangedListener =
                                (SubscriptionManager.OnSubscriptionsChangedListener)
                                        invocation.getArguments()[0];
                        return null;
                    }
                }
        ).when(mSubscriptionManager).addOnSubscriptionsChangedListener(any());
        doReturn(mSubscriptionInfo).when(mSubscriptionManager).getActiveSubscriptionInfo(anyInt());

        doReturn(1).when(mIsub).getDefaultDataSubId();
        doReturn(mIsub).when(mBinder).queryLocalInterface(anyString());
        mServiceManagerMockedServices.put("isub", mBinder);
        mServiceManagerMockedServices.put("package", mMockPackageManagerInternal);

        mContextFixture.putStringArrayResource(
                com.android.internal.R.array.config_cell_retries_per_error_code,
                new String[]{"36,2"});

        mAlarmManager = (AlarmManager) mContext.getSystemService(Context.ALARM_SERVICE);
        mBundle = mContextFixture.getCarrierConfigBundle();

        mSimulatedCommands.setDataCallResult(true, createSetupDataCallResult());
        addDataService();

        mDcTrackerTestHandler = new DcTrackerTestHandler(getClass().getSimpleName());
        mDcTrackerTestHandler.start();
        waitUntilReady();

        Intent intent = new Intent(CarrierConfigManager.ACTION_CARRIER_CONFIG_CHANGED);
        intent.putExtra(CarrierConfigManager.EXTRA_SLOT_INDEX, 0);
        mContext.sendBroadcast(intent);

        waitForMs(600);
        logd("DcTrackerTest -Setup!");
    }

    @After
    public void tearDown() throws Exception {
        logd("DcTrackerTest -tearDown");
        mDct.removeCallbacksAndMessages(null);
        mDct = null;
        mDcTrackerTestHandler.quit();
        super.tearDown();
    }

    // Create a successful data response
    private static SetupDataCallResult createSetupDataCallResult() throws Exception {
        SetupDataCallResult result = new SetupDataCallResult();
        result.status = 0;
        result.suggestedRetryTime = -1;
        result.cid = 1;
        result.active = 2;
        result.type = "IP";
        result.ifname = FAKE_IFNAME;
        result.addresses = FAKE_ADDRESS;
        result.dnses = FAKE_DNS;
        result.gateways = FAKE_GATEWAY;
        result.pcscf = FAKE_PCSCF_ADDRESS;
        result.mtu = 1440;
        return result;
    }

    private void verifyDataProfile(DataProfile dp, String apn, int profileId,
                                   int supportedApnTypesBitmap, int type, int bearerBitmask) {
        assertEquals(profileId, dp.getProfileId());
        assertEquals(apn, dp.getApn());
        assertEquals(ApnSetting.PROTOCOL_IP, dp.getProtocolType());
        assertEquals(0, dp.getAuthType());
        assertEquals("", dp.getUserName());
        assertEquals("", dp.getPassword());
        assertEquals(type, dp.getType());
        assertEquals(0, dp.getWaitTime());
        assertTrue(dp.isEnabled());
        assertEquals(supportedApnTypesBitmap, dp.getSupportedApnTypesBitmask());
        assertEquals(ApnSetting.PROTOCOL_IP, dp.getRoamingProtocolType());
        assertEquals(bearerBitmask, dp.getBearerBitmask());
        assertEquals(0, dp.getMtu());
        assertTrue(dp.isPersistent());
        assertFalse(dp.isPreferred());
    }

    private void verifyDataConnected(final String apnSetting) {
        verify(mPhone, atLeastOnce()).notifyDataConnection(
                eq(PhoneConstants.APN_TYPE_DEFAULT));

        verify(mAlarmManager, times(1)).set(eq(AlarmManager.ELAPSED_REALTIME), anyLong(),
                any(PendingIntent.class));

        assertEquals(apnSetting, mDct.getActiveApnString(PhoneConstants.APN_TYPE_DEFAULT));
        assertArrayEquals(new String[]{PhoneConstants.APN_TYPE_DEFAULT}, mDct.getActiveApnTypes());

        assertEquals(DctConstants.State.CONNECTED, mDct.getOverallState());
        assertEquals(DctConstants.State.CONNECTED, mDct.getState(PhoneConstants.APN_TYPE_DEFAULT));

        LinkProperties linkProperties = mDct.getLinkProperties(PhoneConstants.APN_TYPE_DEFAULT);
        assertEquals(FAKE_IFNAME, linkProperties.getInterfaceName());
        assertEquals(1, linkProperties.getAddresses().size());
        assertEquals(FAKE_ADDRESS, linkProperties.getAddresses().get(0).getHostAddress());
        assertEquals(1, linkProperties.getDnsServers().size());
        assertEquals(FAKE_DNS, linkProperties.getDnsServers().get(0).getHostAddress());
        assertEquals(FAKE_GATEWAY, linkProperties.getRoutes().get(0).getGateway().getHostAddress());
    }

    private boolean isDataAllowed(DataConnectionReasons dataConnectionReasons) {
        try {
            Method method = DcTracker.class.getDeclaredMethod("isDataAllowed",
                    DataConnectionReasons.class);
            method.setAccessible(true);
            return (boolean) method.invoke(mDct, dataConnectionReasons);
        } catch (Exception e) {
            fail(e.toString());
            return false;
        }
    }

    // Test the unmetered APN setup when data is disabled.
    @Test
    @SmallTest
    public void testTrySetupDataUnmeteredDefaultNotSelected() throws Exception {
        initApns(PhoneConstants.APN_TYPE_FOTA, new String[]{PhoneConstants.APN_TYPE_ALL});
        doReturn(SubscriptionManager.INVALID_SUBSCRIPTION_ID).when(mIsub).getDefaultDataSubId();

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT});

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForMs(200);

        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_TRY_SETUP_DATA, mApnContext));
        waitForMs(200);

        verify(mSimulatedCommandsVerifier, times(1)).setupDataCall(
                eq(AccessNetworkType.EUTRAN), any(DataProfile.class),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
    }

    // Test the normal data call setup scenario.
    @Test
    @MediumTest
    public void testDataSetup() throws Exception {
        mSimulatedCommands.setDataCallResult(true, createSetupDataCallResult());

        DataConnectionReasons dataConnectionReasons = new DataConnectionReasons();
        boolean allowed = isDataAllowed(dataConnectionReasons);
        assertFalse(dataConnectionReasons.toString(), allowed);

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForMs(200);

        logd("Sending EVENT_ENABLE_APN");
        // APN id 0 is APN_TYPE_DEFAULT
        mDct.enableApn(ApnSetting.TYPE_DEFAULT, DcTracker.REQUEST_TYPE_NORMAL, null);
        waitForMs(200);

        dataConnectionReasons = new DataConnectionReasons();
        allowed = isDataAllowed(dataConnectionReasons);
        assertTrue(dataConnectionReasons.toString(), allowed);

        ArgumentCaptor<DataProfile> dpCaptor = ArgumentCaptor.forClass(DataProfile.class);
        // Verify if RIL command was sent properly.
        verify(mSimulatedCommandsVerifier, times(1)).setupDataCall(
                eq(AccessNetworkType.EUTRAN), dpCaptor.capture(),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
        verifyDataProfile(dpCaptor.getValue(), FAKE_APN1, 0, 21, 1, NETWORK_TYPE_LTE_BITMASK);

        verifyDataConnected(FAKE_APN1);
    }

    // Test the scenario where the first data call setup is failed, and then retry the setup later.
    @Test
    @MediumTest
    public void testDataRetry() throws Exception {
        AsyncResult ar = new AsyncResult(null,
                new Pair<>(true, DataEnabledSettings.REASON_USER_DATA_ENABLED), null);
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_ENABLED_CHANGED, ar));
        waitForMs(200);

        // LOST_CONNECTION(0x10004) is a non-permanent failure, so we'll retry data setup later.
        SetupDataCallResult result = createSetupDataCallResult();
        result.status = 0x10004;

        // Simulate RIL fails the data call setup
        mSimulatedCommands.setDataCallResult(true, result);

        DataConnectionReasons dataConnectionReasons = new DataConnectionReasons();
        boolean allowed = isDataAllowed(dataConnectionReasons);
        assertFalse(dataConnectionReasons.toString(), allowed);

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForMs(200);

        logd("Sending EVENT_ENABLE_APN");
        // APN id 0 is APN_TYPE_DEFAULT
        mDct.enableApn(ApnSetting.TYPE_DEFAULT, DcTracker.REQUEST_TYPE_NORMAL, null);
        waitForMs(200);

        dataConnectionReasons = new DataConnectionReasons();
        allowed = isDataAllowed(dataConnectionReasons);
        assertTrue(dataConnectionReasons.toString(), allowed);

        ArgumentCaptor<DataProfile> dpCaptor = ArgumentCaptor.forClass(DataProfile.class);
        // Verify if RIL command was sent properly.
        verify(mSimulatedCommandsVerifier, times(1)).setupDataCall(
                eq(AccessNetworkType.EUTRAN), dpCaptor.capture(),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
        verifyDataProfile(dpCaptor.getValue(), FAKE_APN1, 0, 21, 1, NETWORK_TYPE_LTE_BITMASK);

        // Verify the retry manger schedule another data call setup.
        verify(mAlarmManager, times(1)).setExact(eq(AlarmManager.ELAPSED_REALTIME_WAKEUP),
                anyLong(), any(PendingIntent.class));

        // This time we'll let RIL command succeed.
        mSimulatedCommands.setDataCallResult(true, createSetupDataCallResult());

        // Simulate the timer expires.
        Intent intent = new Intent("com.android.internal.telephony.data-reconnect.default");
        intent.putExtra("reconnect_alarm_extra_type", PhoneConstants.APN_TYPE_DEFAULT);
        intent.putExtra("reconnect_alarm_extra_transport_type",
                AccessNetworkConstants.TRANSPORT_TYPE_WWAN);
        intent.putExtra(PhoneConstants.SUBSCRIPTION_KEY, 0);
        intent.addFlags(Intent.FLAG_RECEIVER_FOREGROUND);
        mContext.sendBroadcast(intent);
        waitForMs(200);

        dpCaptor = ArgumentCaptor.forClass(DataProfile.class);
        // Verify if RIL command was sent properly.
        verify(mSimulatedCommandsVerifier, times(2)).setupDataCall(
                eq(AccessNetworkType.EUTRAN), dpCaptor.capture(),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
        verifyDataProfile(dpCaptor.getValue(), FAKE_APN2, 0, 21, 1, NETWORK_TYPE_LTE_BITMASK);

        // Verify connected with APN2 setting.
        verifyDataConnected(FAKE_APN2);
    }

    @Test
    @MediumTest
    @Ignore
    @FlakyTest
    public void testUserDisableData() throws Exception {
        //step 1: setup two DataCalls one for Metered: default, another one for Non-metered: IMS
        //set Default and MMS to be metered in the CarrierConfigManager
        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT, PhoneConstants.APN_TYPE_MMS});
        mDct.enableApn(ApnSetting.TYPE_IMS, DcTracker.REQUEST_TYPE_NORMAL, null);
        mDct.enableApn(ApnSetting.TYPE_DEFAULT, DcTracker.REQUEST_TYPE_NORMAL, null);

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForMs(200);

        ArgumentCaptor<DataProfile> dpCaptor = ArgumentCaptor.forClass(DataProfile.class);
        verify(mSimulatedCommandsVerifier, times(2)).setupDataCall(
                eq(AccessNetworkType.EUTRAN), dpCaptor.capture(),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
        verifyDataProfile(dpCaptor.getValue(), FAKE_APN1, 0, 5, 1, NETWORK_TYPE_LTE_BITMASK);

        logd("Sending DATA_DISABLED_CMD");
        doReturn(false).when(mDataEnabledSettings).isDataEnabled();
        doReturn(false).when(mDataEnabledSettings).isDataEnabled(anyInt());
        AsyncResult ar = new AsyncResult(null,
                new Pair<>(false, DataEnabledSettings.REASON_USER_DATA_ENABLED), null);
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_ENABLED_CHANGED, ar));
        waitForMs(200);

        // expected tear down all metered DataConnections
        verify(mSimulatedCommandsVerifier, times(1)).deactivateDataCall(
                eq(DataService.REQUEST_REASON_NORMAL), anyInt(),
                any(Message.class));
        assertEquals(DctConstants.State.CONNECTED, mDct.getOverallState());
        assertEquals(DctConstants.State.IDLE, mDct.getState(PhoneConstants.APN_TYPE_DEFAULT));
        assertEquals(DctConstants.State.CONNECTED, mDct.getState(PhoneConstants.APN_TYPE_IMS));
    }

    @Test
    @MediumTest
    public void testTrySetupDataMmsAllowedDataDisabled() throws Exception {
        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT, PhoneConstants.APN_TYPE_MMS});
        mDct.enableApn(ApnSetting.TYPE_MMS, DcTracker.REQUEST_TYPE_NORMAL, null);
        mDct.enableApn(ApnSetting.TYPE_DEFAULT, DcTracker.REQUEST_TYPE_NORMAL, null);

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForMs(500);

        ArgumentCaptor<DataProfile> dpCaptor = ArgumentCaptor.forClass(DataProfile.class);
        verify(mSimulatedCommandsVerifier, times(2)).setupDataCall(
                eq(AccessNetworkType.EUTRAN), dpCaptor.capture(),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
        verifyDataProfile(dpCaptor.getValue(), FAKE_APN1, 0, 21, 1, NETWORK_TYPE_LTE_BITMASK);

        logd("Sending DATA_DISABLED_CMD for default data");
        doReturn(false).when(mDataEnabledSettings).isDataEnabled();
        doReturn(false).when(mDataEnabledSettings).isDataEnabled(anyInt());
        mDct.obtainMessage(DctConstants.EVENT_DATA_ENABLED_OVERRIDE_RULES_CHANGED).sendToTarget();
        waitForMs(200);

        // expected tear down all metered DataConnections
        verify(mSimulatedCommandsVerifier, times(2)).deactivateDataCall(
                eq(DataService.REQUEST_REASON_NORMAL), anyInt(),
                any(Message.class));
        assertEquals(DctConstants.State.IDLE, mDct.getState(PhoneConstants.APN_TYPE_DEFAULT));
        assertEquals(DctConstants.State.IDLE, mDct.getState(PhoneConstants.APN_TYPE_MMS));

        clearInvocations(mSimulatedCommandsVerifier);
        doReturn(true).when(mDataEnabledSettings).isDataEnabled(ApnSetting.TYPE_MMS);
        mDct.obtainMessage(DctConstants.EVENT_DATA_ENABLED_OVERRIDE_RULES_CHANGED).sendToTarget();
        waitForMs(200);

        verify(mSimulatedCommandsVerifier, times(1)).setupDataCall(
                eq(AccessNetworkType.EUTRAN), dpCaptor.capture(),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
        assertEquals(DctConstants.State.IDLE, mDct.getState(PhoneConstants.APN_TYPE_DEFAULT));
        assertEquals(DctConstants.State.CONNECTED, mDct.getState(PhoneConstants.APN_TYPE_MMS));
    }

    @Test
    @MediumTest
    public void testUserDisableRoaming() throws Exception {
        //step 1: setup two DataCalls one for Metered: default, another one for Non-metered: IMS
        //step 2: set roaming disabled, data is enabled
        //step 3: under roaming service
        //step 4: only tear down metered data connections.

        //set Default and MMS to be metered in the CarrierConfigManager
        boolean roamingEnabled = mDct.getDataRoamingEnabled();

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_ROAMING_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT, PhoneConstants.APN_TYPE_MMS});

        mDct.enableApn(ApnSetting.TYPE_IMS, DcTracker.REQUEST_TYPE_NORMAL, null);
        waitForHandlerAction(mDct, 1000);
        mDct.enableApn(ApnSetting.TYPE_DEFAULT, DcTracker.REQUEST_TYPE_NORMAL, null);
        waitForHandlerAction(mDct, 1000);

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForHandlerAction(mDct, 1000);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForHandlerAction(mDct, 1000);
        logd("Handling EVENT_DATA_CONNECTION_ATTACHED complete");
        // dataconnection is on a different handler
        waitForMs(200);

        ArgumentCaptor<DataProfile> dpCaptor = ArgumentCaptor.forClass(DataProfile.class);
        verify(mSimulatedCommandsVerifier, times(2)).setupDataCall(
                eq(AccessNetworkType.EUTRAN), dpCaptor.capture(),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
        verifyDataProfile(dpCaptor.getValue(), FAKE_APN1, 0, 21, 1, NETWORK_TYPE_LTE_BITMASK);

        //user is in roaming
        doReturn(true).when(mServiceState).getDataRoaming();
        logd("Sending DISABLE_ROAMING_CMD");
        mDct.setDataRoamingEnabledByUser(false);
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_ROAMING_ON));
        waitForMs(200);

        // expected tear down all metered DataConnections
        verify(mSimulatedCommandsVerifier, times(1)).deactivateDataCall(
                eq(DataService.REQUEST_REASON_NORMAL), anyInt(),
                any(Message.class));
        assertEquals(DctConstants.State.CONNECTED, mDct.getOverallState());
        assertEquals(DctConstants.State.IDLE, mDct.getState(PhoneConstants.APN_TYPE_DEFAULT));
        assertEquals(DctConstants.State.CONNECTED, mDct.getState(PhoneConstants.APN_TYPE_IMS));

        // reset roaming settings / data enabled settings at end of this test
        mDct.setDataRoamingEnabledByUser(roamingEnabled);
        waitForMs(200);
    }

    @Test
    @MediumTest
    public void testDataCallOnUserDisableRoaming() throws Exception {
        //step 1: mock under roaming service and user disabled roaming from settings.
        //step 2: user toggled data settings on
        //step 3: only non-metered data call is established

        boolean roamingEnabled = mDct.getDataRoamingEnabled();
        doReturn(true).when(mServiceState).getDataRoaming();

        //set Default and MMS to be metered in the CarrierConfigManager
        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_ROAMING_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT, PhoneConstants.APN_TYPE_MMS});
        mDct.enableApn(ApnSetting.TYPE_IMS, DcTracker.REQUEST_TYPE_NORMAL, null);
        mDct.enableApn(ApnSetting.TYPE_DEFAULT, DcTracker.REQUEST_TYPE_NORMAL, null);

        logd("Sending DISABLE_ROAMING_CMD");
        mDct.setDataRoamingEnabledByUser(false);

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForMs(200);

        waitForMs(200);
        ArgumentCaptor<DataProfile> dpCaptor = ArgumentCaptor.forClass(DataProfile.class);
        verify(mSimulatedCommandsVerifier, times(1)).setupDataCall(
                eq(AccessNetworkType.EUTRAN), dpCaptor.capture(),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
        verifyDataProfile(dpCaptor.getValue(), FAKE_APN3, 2, 64, 0, 0);

        assertEquals(DctConstants.State.CONNECTED, mDct.getOverallState());
        assertEquals(DctConstants.State.IDLE, mDct.getState(PhoneConstants.APN_TYPE_DEFAULT));
        assertEquals(DctConstants.State.CONNECTED, mDct.getState(PhoneConstants.APN_TYPE_IMS));

        // reset roaming settings / data enabled settings at end of this test
        mDct.setDataRoamingEnabledByUser(roamingEnabled);
        waitForMs(200);
    }

    // Test the default data switch scenario.
    @FlakyTest /* flakes 1.57% of the time */
    @Test
    @MediumTest
    public void testDDSResetAutoAttach() throws Exception {

        ContentResolver resolver = mContext.getContentResolver();
        Settings.Global.putInt(resolver, Settings.Global.DEVICE_PROVISIONED, 1);

        mContextFixture.putBooleanResource(
                com.android.internal.R.bool.config_auto_attach_data_on_creation, true);

        mSimulatedCommands.setDataCallResult(true, createSetupDataCallResult());

        AsyncResult ar = new AsyncResult(null,
                new Pair<>(true, DataEnabledSettings.REASON_USER_DATA_ENABLED), null);
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_ENABLED_CHANGED, ar));
        waitForMs(200);

        DataConnectionReasons dataConnectionReasons = new DataConnectionReasons();
        boolean allowed = isDataAllowed(dataConnectionReasons);
        assertFalse(dataConnectionReasons.toString(), allowed);

        ArgumentCaptor<Integer> intArgumentCaptor = ArgumentCaptor.forClass(Integer.class);
        verify(mUiccController, times(1)).registerForIccChanged(eq(mDct),
                intArgumentCaptor.capture(), eq(null));
        // Ideally this should send EVENT_ICC_CHANGED.
        mDct.sendMessage(mDct.obtainMessage(intArgumentCaptor.getValue(), null));
        waitForMs(100);

        verify(mSimRecords, times(1)).registerForRecordsLoaded(eq(mDct),
                intArgumentCaptor.capture(), eq(null));
        // Ideally this should send EVENT_RECORDS_LOADED.
        mDct.sendMessage(mDct.obtainMessage(intArgumentCaptor.getValue(), null));
        waitForMs(100);

        verify(mSST, times(1)).registerForDataConnectionAttached(
                eq(AccessNetworkConstants.TRANSPORT_TYPE_WWAN), eq(mDct),
                intArgumentCaptor.capture(), eq(null));
        // Ideally this should send EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(intArgumentCaptor.getValue(), null));
        waitForMs(200);

        NetworkRequest nr = new NetworkRequest.Builder()
                .addCapability(NetworkCapabilities.NET_CAPABILITY_INTERNET).build();
        mDct.requestNetwork(nr, DcTracker.REQUEST_TYPE_NORMAL, null);
        waitForMs(200);

        verifyDataConnected(FAKE_APN1);

        assertTrue(mDct.shouldAutoAttach());
        mDct.update();
        // The auto attach flag should be reset after update
        assertFalse(mDct.shouldAutoAttach());

        verify(mSST, times(1)).registerForDataConnectionDetached(
                eq(AccessNetworkConstants.TRANSPORT_TYPE_WWAN), eq(mDct),
                intArgumentCaptor.capture(), eq(null));
        // Ideally this should send EVENT_DATA_CONNECTION_DETACHED
        mDct.sendMessage(mDct.obtainMessage(intArgumentCaptor.getValue(), null));
        waitForMs(200);

        // Data should not be allowed since auto attach flag has been reset.
        dataConnectionReasons = new DataConnectionReasons();
        allowed = isDataAllowed(dataConnectionReasons);
        assertFalse(dataConnectionReasons.toString(), allowed);
    }

    // Test for API carrierActionSetMeteredApnsEnabled.
    @FlakyTest
    @Ignore
    @Test
    @MediumTest
    public void testCarrierActionSetMeteredApnsEnabled() throws Exception {
        //step 1: setup two DataCalls one for Internet and IMS
        //step 2: set data is enabled
        //step 3: cold sim is detected
        //step 4: all data connection is torn down
        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT, PhoneConstants.APN_TYPE_MMS});

        mDct.enableApn(ApnSetting.TYPE_IMS, DcTracker.REQUEST_TYPE_NORMAL, null);
        mDct.enableApn(ApnSetting.TYPE_DEFAULT, DcTracker.REQUEST_TYPE_NORMAL, null);

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForMs(200);

        ArgumentCaptor<DataProfile> dpCaptor = ArgumentCaptor.forClass(DataProfile.class);
        verify(mSimulatedCommandsVerifier, times(2)).setupDataCall(
                eq(AccessNetworkType.EUTRAN), dpCaptor.capture(),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
        verifyDataProfile(dpCaptor.getValue(), FAKE_APN1, 0, 5, 1, NETWORK_TYPE_LTE_BITMASK);
        assertEquals(DctConstants.State.CONNECTED, mDct.getOverallState());

        AsyncResult ar = new AsyncResult(null,
                new Pair<>(false, DataEnabledSettings.REASON_DATA_ENABLED_BY_CARRIER), null);
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_ENABLED_CHANGED, ar));
        waitForMs(200);

        // Validate all metered data connections have been torn down
        verify(mSimulatedCommandsVerifier, times(1)).deactivateDataCall(
                eq(DataService.REQUEST_REASON_NORMAL), anyInt(),
                any(Message.class));
        assertEquals(DctConstants.State.CONNECTED, mDct.getOverallState());
        assertEquals(DctConstants.State.IDLE, mDct.getState(PhoneConstants.APN_TYPE_DEFAULT));
    }

    private void initApns(String targetApn, String[] canHandleTypes) {
        doReturn(targetApn).when(mApnContext).getApnType();
        doReturn(true).when(mApnContext).isConnectable();
        ApnSetting apnSetting = createApnSetting(ApnSetting.getApnTypesBitmaskFromString(
                TextUtils.join(",", canHandleTypes)));
        doReturn(apnSetting).when(mApnContext).getNextApnSetting();
        doReturn(apnSetting).when(mApnContext).getApnSetting();
        doReturn(mDataConnection).when(mApnContext).getDataConnection();
        doReturn(true).when(mApnContext).isEnabled();
        doReturn(true).when(mApnContext).isDependencyMet();
        doReturn(true).when(mApnContext).isReady();
        doReturn(false).when(mApnContext).hasRestrictedRequests(eq(true));
    }

    // Test the emergency APN setup.
    @Test
    @SmallTest
    public void testTrySetupDataEmergencyApn() throws Exception {
        initApns(PhoneConstants.APN_TYPE_EMERGENCY, new String[]{PhoneConstants.APN_TYPE_ALL});
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_TRY_SETUP_DATA, mApnContext));
        waitForMs(200);

        verify(mSimulatedCommandsVerifier, times(1)).setupDataCall(
                eq(AccessNetworkType.EUTRAN), any(DataProfile.class),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
    }

    @Test
    @SmallTest
    public void testGetDataConnectionState() throws Exception {
        initApns(PhoneConstants.APN_TYPE_SUPL,
                new String[]{PhoneConstants.APN_TYPE_SUPL, PhoneConstants.APN_TYPE_DEFAULT});
        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT});

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForMs(200);

        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_TRY_SETUP_DATA, mApnContext));
        waitForMs(200);

        // Assert that both APN_TYPE_SUPL & APN_TYPE_DEFAULT are connected even we only setup data
        // for APN_TYPE_SUPL
        assertEquals(DctConstants.State.CONNECTED, mDct.getState(PhoneConstants.APN_TYPE_SUPL));
        assertEquals(DctConstants.State.CONNECTED, mDct.getState(PhoneConstants.APN_TYPE_DEFAULT));
    }

    // Test the unmetered APN setup when data is disabled.
    @Test
    @SmallTest
    public void testTrySetupDataUnmeteredDataDisabled() throws Exception {
        initApns(PhoneConstants.APN_TYPE_FOTA, new String[]{PhoneConstants.APN_TYPE_ALL});
        //mDct.setUserDataEnabled(false);
        doReturn(false).when(mDataEnabledSettings).isDataEnabled();
        doReturn(false).when(mDataEnabledSettings).isDataEnabled(anyInt());

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT});

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForMs(200);

        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_TRY_SETUP_DATA, mApnContext));
        waitForMs(200);

        verify(mSimulatedCommandsVerifier, times(1)).setupDataCall(
                eq(AccessNetworkType.EUTRAN), any(DataProfile.class),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
    }

    // Test the metered APN setup when data is disabled.
    @Test
    @SmallTest
    public void testTrySetupMeteredDataDisabled() throws Exception {
        initApns(PhoneConstants.APN_TYPE_DEFAULT, new String[]{PhoneConstants.APN_TYPE_ALL});
        //mDct.setUserDataEnabled(false);
        doReturn(false).when(mDataEnabledSettings).isDataEnabled();
        doReturn(false).when(mDataEnabledSettings).isDataEnabled(anyInt());

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT});

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForMs(200);

        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_TRY_SETUP_DATA, mApnContext));
        waitForMs(200);

        verify(mSimulatedCommandsVerifier, times(0)).setupDataCall(anyInt(), any(DataProfile.class),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
    }

    // Test the restricted data request when data is disabled.
    @Test
    @SmallTest
    public void testTrySetupRestrictedDataDisabled() throws Exception {
        initApns(PhoneConstants.APN_TYPE_DEFAULT, new String[]{PhoneConstants.APN_TYPE_ALL});
        doReturn(true).when(mApnContext).hasRestrictedRequests(eq(true));

        //mDct.setUserDataEnabled(false);
        doReturn(false).when(mDataEnabledSettings).isDataEnabled();
        doReturn(false).when(mDataEnabledSettings).isDataEnabled(anyInt());

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT});

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForMs(200);

        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_TRY_SETUP_DATA, mApnContext));
        waitForMs(200);

        verify(mSimulatedCommandsVerifier, times(1)).setupDataCall(anyInt(), any(DataProfile.class),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
    }

    // Test the restricted data request when roaming is disabled.
    @Test
    @SmallTest
    public void testTrySetupRestrictedRoamingDisabled() throws Exception {
        initApns(PhoneConstants.APN_TYPE_DEFAULT, new String[]{PhoneConstants.APN_TYPE_ALL});
        doReturn(true).when(mApnContext).hasRestrictedRequests(eq(true));

        mDct.setDataRoamingEnabledByUser(false);
        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT});
        //user is in roaming
        doReturn(true).when(mServiceState).getDataRoaming();

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForMs(200);

        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_TRY_SETUP_DATA, mApnContext));
        waitForMs(200);

        verify(mSimulatedCommandsVerifier, times(1)).setupDataCall(anyInt(), any(DataProfile.class),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
    }

    // Test the default data when data is not connectable.
    @Test
    @SmallTest
    public void testTrySetupNotConnectable() throws Exception {
        initApns(PhoneConstants.APN_TYPE_DEFAULT, new String[]{PhoneConstants.APN_TYPE_ALL});
        doReturn(false).when(mApnContext).isConnectable();

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT});

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForMs(200);

        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_TRY_SETUP_DATA, mApnContext));
        waitForMs(200);

        verify(mSimulatedCommandsVerifier, times(0)).setupDataCall(anyInt(), any(DataProfile.class),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
    }

    // Test the default data on IWLAN.
    @Test
    @SmallTest
    public void testTrySetupDefaultOnIWLAN() throws Exception {
        doReturn(true).when(mTransportManager).isInLegacyMode();
        initApns(PhoneConstants.APN_TYPE_DEFAULT, new String[]{PhoneConstants.APN_TYPE_ALL});
        mNetworkRegistrationInfo = new NetworkRegistrationInfo.Builder()
                .setAccessNetworkTechnology(TelephonyManager.NETWORK_TYPE_IWLAN)
                .setRegistrationState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME)
                .build();
        doReturn(mNetworkRegistrationInfo).when(mServiceState).getNetworkRegistrationInfo(
                anyInt(), anyInt());

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT});

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForMs(200);

        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_TRY_SETUP_DATA, mApnContext));
        waitForMs(200);

        verify(mSimulatedCommandsVerifier, times(0)).setupDataCall(anyInt(), any(DataProfile.class),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
    }

    // Test the default data when the phone is in ECBM.
    @Test
    @SmallTest
    public void testTrySetupDefaultInECBM() throws Exception {
        initApns(PhoneConstants.APN_TYPE_DEFAULT, new String[]{PhoneConstants.APN_TYPE_ALL});
        doReturn(true).when(mPhone).isInEcm();

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT});

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForMs(200);

        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_TRY_SETUP_DATA, mApnContext));
        waitForMs(200);

        verify(mSimulatedCommandsVerifier, times(0)).setupDataCall(anyInt(), any(DataProfile.class),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
    }

    // Test update waiting apn list when on data rat change
    @FlakyTest /* flakes 0.86% of the time */
    @Test
    @SmallTest
    public void testUpdateWaitingApnListOnDataRatChange() throws Exception {
        mNetworkRegistrationInfo = new NetworkRegistrationInfo.Builder()
                .setAccessNetworkTechnology(TelephonyManager.NETWORK_TYPE_EHRPD)
                .setRegistrationState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME)
                .build();
        doReturn(mNetworkRegistrationInfo).when(mServiceState).getNetworkRegistrationInfo(
                anyInt(), anyInt());
        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT});
        mDct.enableApn(ApnSetting.TYPE_DEFAULT, DcTracker.REQUEST_TYPE_NORMAL, null);
        initApns(PhoneConstants.APN_TYPE_DEFAULT, new String[]{PhoneConstants.APN_TYPE_ALL});

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForMs(200);

        ArgumentCaptor<DataProfile> dpCaptor = ArgumentCaptor.forClass(DataProfile.class);
        // Verify if RIL command was sent properly.
        verify(mSimulatedCommandsVerifier).setupDataCall(
                eq(AccessNetworkType.CDMA2000), dpCaptor.capture(),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
        verifyDataProfile(dpCaptor.getValue(), FAKE_APN4, 0, 21, 2, NETWORK_TYPE_EHRPD_BITMASK);
        assertEquals(DctConstants.State.CONNECTED, mDct.getOverallState());

        //data rat change from ehrpd to lte
        logd("Sending EVENT_DATA_RAT_CHANGED");
        mNetworkRegistrationInfo = new NetworkRegistrationInfo.Builder()
                .setAccessNetworkTechnology(TelephonyManager.NETWORK_TYPE_LTE)
                .setRegistrationState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME)
                .build();
        doReturn(mNetworkRegistrationInfo).when(mServiceState).getNetworkRegistrationInfo(
                anyInt(), anyInt());
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_RAT_CHANGED, null));
        waitForMs(200);

        // Verify the disconnected data call due to rat change and retry manger schedule another
        // data call setup
        verify(mSimulatedCommandsVerifier, times(1)).deactivateDataCall(
                eq(DataService.REQUEST_REASON_NORMAL), anyInt(),
                any(Message.class));
        verify(mAlarmManager, times(1)).setExact(eq(AlarmManager.ELAPSED_REALTIME_WAKEUP),
                anyLong(), any(PendingIntent.class));

        // Simulate the timer expires.
        Intent intent = new Intent("com.android.internal.telephony.data-reconnect.default");
        intent.putExtra("reconnect_alarm_extra_type", PhoneConstants.APN_TYPE_DEFAULT);
        intent.putExtra(PhoneConstants.SUBSCRIPTION_KEY, 0);
        intent.putExtra("reconnect_alarm_extra_transport_type",
                AccessNetworkConstants.TRANSPORT_TYPE_WWAN);
        intent.addFlags(Intent.FLAG_RECEIVER_FOREGROUND);
        mContext.sendBroadcast(intent);
        waitForMs(200);

        // Verify if RIL command was sent properly.
        verify(mSimulatedCommandsVerifier).setupDataCall(
                eq(AccessNetworkType.EUTRAN), dpCaptor.capture(),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
        verifyDataProfile(dpCaptor.getValue(), FAKE_APN1, 0, 21, 1, NETWORK_TYPE_LTE_BITMASK);
        assertEquals(DctConstants.State.CONNECTED, mDct.getOverallState());
    }

    // Test for fetchDunApns()
    @Test
    @SmallTest
    public void testFetchDunApn() {
        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        String dunApnString = "[ApnSettingV3]HOT mobile PC,pc.hotm,,,,,,,,,440,10,,DUN,,,true,"
                + "0,,,,,,,,";
        ApnSetting dunApnExpected = ApnSetting.fromString(dunApnString);

        Settings.Global.putString(mContext.getContentResolver(),
                Settings.Global.TETHER_DUN_APN, dunApnString);
        // should return APN from Setting
        ApnSetting dunApn = mDct.fetchDunApns().get(0);
        assertTrue(dunApnExpected.equals(dunApn));

        Settings.Global.putString(mContext.getContentResolver(),
                Settings.Global.TETHER_DUN_APN, null);
        // should return APN from db
        dunApn = mDct.fetchDunApns().get(0);
        assertEquals(FAKE_APN5, dunApn.getApnName());
    }

    // Test for fetchDunApns() with apn set id
    @Test
    @SmallTest
    public void testFetchDunApnWithPreferredApnSet() {
        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        // apnSetId=1
        String dunApnString1 = "[ApnSettingV5]HOT mobile PC,pc.hotm,,,,,,,,,440,10,,DUN,,,true,"
                + "0,,,,,,,,,,1";
        // apnSetId=0
        String dunApnString2 = "[ApnSettingV5]HOT mobile PC,pc.coldm,,,,,,,,,440,10,,DUN,,,true,"
                + "0,,,,,,,,,,0";

        ApnSetting dunApnExpected = ApnSetting.fromString(dunApnString1);

        ContentResolver cr = mContext.getContentResolver();
        Settings.Global.putString(cr, Settings.Global.TETHER_DUN_APN,
                dunApnString1 + ";" + dunApnString2);

        // set that we prefer apn set 1
        ContentValues values = new ContentValues();
        values.put(Telephony.Carriers.APN_SET_ID, 1);
        cr.update(PREFERAPN_URI, values, null, null);

        // return APN from Setting with apnSetId=1
        ArrayList<ApnSetting> dunApns = mDct.sortApnListByPreferred(mDct.fetchDunApns());
        assertEquals(2, dunApns.size());
        assertTrue(dunApnExpected.equals(dunApns.get(0)));
    }

    // Test oos
    @Test
    @SmallTest
    public void testDataRatChangeOOS() throws Exception {
        mNetworkRegistrationInfo = new NetworkRegistrationInfo.Builder()
                .setAccessNetworkTechnology(TelephonyManager.NETWORK_TYPE_EHRPD)
                .setRegistrationState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME)
                .build();
        doReturn(mNetworkRegistrationInfo).when(mServiceState).getNetworkRegistrationInfo(
                anyInt(), anyInt());

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT});
        mDct.enableApn(ApnSetting.TYPE_DEFAULT, DcTracker.REQUEST_TYPE_NORMAL, null);
        initApns(PhoneConstants.APN_TYPE_DEFAULT, new String[]{PhoneConstants.APN_TYPE_ALL});

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForMs(200);

        ArgumentCaptor<DataProfile> dpCaptor = ArgumentCaptor.forClass(DataProfile.class);
        // Verify if RIL command was sent properly.
        verify(mSimulatedCommandsVerifier).setupDataCall(
                eq(AccessNetworkType.CDMA2000), dpCaptor.capture(),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
        verifyDataProfile(dpCaptor.getValue(), FAKE_APN4, 0, 21, 2, NETWORK_TYPE_EHRPD_BITMASK);
        assertEquals(DctConstants.State.CONNECTED, mDct.getOverallState());

        // Data rat change from ehrpd to unknown due to OOS
        logd("Sending EVENT_DATA_RAT_CHANGED");
        mNetworkRegistrationInfo = new NetworkRegistrationInfo.Builder()
                .setAccessNetworkTechnology(TelephonyManager.NETWORK_TYPE_UNKNOWN)
                .setRegistrationState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME)
                .build();
        doReturn(mNetworkRegistrationInfo).when(mServiceState).getNetworkRegistrationInfo(
                anyInt(), anyInt());
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_RAT_CHANGED, null));
        waitForMs(200);

        // Verify data connection is on
        verify(mSimulatedCommandsVerifier, times(0)).deactivateDataCall(
                eq(DataService.REQUEST_REASON_NORMAL), anyInt(),
                any(Message.class));

        // Data rat resume from unknown to ehrpd
        mNetworkRegistrationInfo = new NetworkRegistrationInfo.Builder()
                .setAccessNetworkTechnology(TelephonyManager.NETWORK_TYPE_EHRPD)
                .setRegistrationState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME)
                .build();
        doReturn(mNetworkRegistrationInfo).when(mServiceState).getNetworkRegistrationInfo(
                anyInt(), anyInt());
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_RAT_CHANGED, null));
        waitForMs(200);

        // Verify the same data connection
        assertEquals(FAKE_APN4, mDct.getActiveApnString(PhoneConstants.APN_TYPE_DEFAULT));
        assertEquals(DctConstants.State.CONNECTED, mDct.getOverallState());
    }

    // Test provisioning
    /*@Test
    @SmallTest
    public void testDataEnableInProvisioning() throws Exception {
        ContentResolver resolver = mContext.getContentResolver();

        assertEquals(1, Settings.Global.getInt(resolver, Settings.Global.MOBILE_DATA));
        assertTrue(mDct.isDataEnabled());
        assertTrue(mDct.isUserDataEnabled());

        mDct.setUserDataEnabled(false);
        waitForMs(200);

        assertEquals(0, Settings.Global.getInt(resolver, Settings.Global.MOBILE_DATA));
        assertFalse(mDct.isDataEnabled());
        assertFalse(mDct.isUserDataEnabled());

        // Changing provisioned to 0.
        Settings.Global.putInt(resolver, Settings.Global.DEVICE_PROVISIONED, 0);
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DEVICE_PROVISIONED_CHANGE, null));
        waitForMs(200);

        assertTrue(mDct.isDataEnabled());
        assertTrue(mDct.isUserDataEnabled());

        // Enable user data during provisioning. It should write to
        // Settings.Global.MOBILE_DATA and keep data enabled when provisioned.
        mDct.setUserDataEnabled(true);
        Settings.Global.putInt(resolver, Settings.Global.DEVICE_PROVISIONED, 1);
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DEVICE_PROVISIONED_CHANGE, null));
        waitForMs(200);

        assertTrue(mDct.isDataEnabled());
        assertTrue(mDct.isUserDataEnabled());
        assertEquals(1, Settings.Global.getInt(resolver, Settings.Global.MOBILE_DATA));
    }*/

    /*
    @Test
    @SmallTest
    public void testNotifyDataEnabledChanged() throws Exception {
        doAnswer(invocation -> {
            mMessage = (Message) invocation.getArguments()[0];
            return true;
        }).when(mHandler).sendMessageDelayed(any(), anyLong());

        // Test registration.
        mDct.registerForDataEnabledChanged(mHandler, DATA_ENABLED_CHANGED, null);
        verifyDataEnabledChangedMessage(true, DataEnabledSettings.REASON_REGISTERED);

        // Disable user data. Should receive data enabled change to false.
        mDct.setUserDataEnabled(false);
        waitForMs(200);
        verifyDataEnabledChangedMessage(false, DataEnabledSettings.REASON_USER_DATA_ENABLED);

        // Changing provisioned to 0. Shouldn't receive any message, as data enabled remains false.
        ContentResolver resolver = mContext.getContentResolver();
        Settings.Global.putInt(resolver, Settings.Global.DEVICE_PROVISIONED, 0);
        Settings.Global.putInt(resolver, Settings.Global.DEVICE_PROVISIONING_MOBILE_DATA_ENABLED,
                0);
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DEVICE_PROVISIONED_CHANGE, null));
        waitForMs(200);
        assertFalse(mDct.isDataEnabled());
        verify(mHandler, never()).sendMessageDelayed(any(), anyLong());

        // Changing provisioningDataEnabled to 1. It should trigger data enabled change to true.
        Settings.Global.putInt(resolver,
                Settings.Global.DEVICE_PROVISIONING_MOBILE_DATA_ENABLED, 1);
        mDct.sendMessage(mDct.obtainMessage(
                DctConstants.EVENT_DEVICE_PROVISIONING_DATA_SETTING_CHANGE, null));
        waitForMs(200);
        verifyDataEnabledChangedMessage(
                true, DataEnabledSettings.REASON_PROVISIONING_DATA_ENABLED_CHANGED);
    }*/

    @Test
    @SmallTest
    public void testNetworkStatusChangedRecoveryOFF() throws Exception {
        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT, PhoneConstants.APN_TYPE_MMS});
        mDct.enableApn(ApnSetting.TYPE_IMS, DcTracker.REQUEST_TYPE_NORMAL, null);
        mDct.enableApn(ApnSetting.TYPE_DEFAULT, DcTracker.REQUEST_TYPE_NORMAL, null);

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForMs(200);

        ArgumentCaptor<DataProfile> dpCaptor = ArgumentCaptor.forClass(DataProfile.class);
        verify(mSimulatedCommandsVerifier, times(2)).setupDataCall(
                eq(AccessNetworkType.EUTRAN), dpCaptor.capture(),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
        verifyDataProfile(dpCaptor.getValue(), FAKE_APN1, 0, 21, 1, NETWORK_TYPE_LTE_BITMASK);

        logd("Sending EVENT_NETWORK_STATUS_CHANGED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_NETWORK_STATUS_CHANGED,
                NetworkAgent.VALID_NETWORK, 0, null));
        waitForMs(200);

        logd("Sending EVENT_NETWORK_STATUS_CHANGED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_NETWORK_STATUS_CHANGED,
                NetworkAgent.INVALID_NETWORK, 0, null));
        waitForMs(200);

        // Verify that its no-op when the new data stall detection feature is disabled
        verify(mSimulatedCommandsVerifier, times(0)).getDataCallList(any(Message.class));
    }

    @FlakyTest
    @Test
    @SmallTest
    public void testNetworkStatusChangedRecoveryON() throws Exception {
        ContentResolver resolver = mContext.getContentResolver();
        Settings.Global.putInt(resolver, Settings.Global.DATA_STALL_RECOVERY_ON_BAD_NETWORK, 1);
        Settings.System.putInt(resolver, "radio.data.stall.recovery.action", 0);
        doReturn(new SignalStrength()).when(mPhone).getSignalStrength();

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT, PhoneConstants.APN_TYPE_MMS});
        mDct.enableApn(ApnSetting.TYPE_IMS, DcTracker.REQUEST_TYPE_NORMAL, null);
        mDct.enableApn(ApnSetting.TYPE_DEFAULT, DcTracker.REQUEST_TYPE_NORMAL, null);

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));

        ArgumentCaptor<DataProfile> dpCaptor = ArgumentCaptor.forClass(DataProfile.class);
        verify(mSimulatedCommandsVerifier, timeout(TEST_TIMEOUT).times(2)).setupDataCall(
                eq(AccessNetworkType.EUTRAN), dpCaptor.capture(),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
        verifyDataProfile(dpCaptor.getValue(), FAKE_APN1, 0, 21, 1, NETWORK_TYPE_LTE_BITMASK);

        logd("Sending EVENT_NETWORK_STATUS_CHANGED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_NETWORK_STATUS_CHANGED,
                NetworkAgent.VALID_NETWORK, 0, null));
        waitForMs(200);

        logd("Sending EVENT_NETWORK_STATUS_CHANGED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_NETWORK_STATUS_CHANGED,
                NetworkAgent.INVALID_NETWORK, 0, null));
        waitForMs(200);

        verify(mSimulatedCommandsVerifier, times(1)).getDataCallList(any(Message.class));
    }

    @FlakyTest
    @Test
    @SmallTest
    public void testRecoveryStepPDPReset() throws Exception {
        ContentResolver resolver = mContext.getContentResolver();
        Settings.Global.putInt(resolver, Settings.Global.DATA_STALL_RECOVERY_ON_BAD_NETWORK, 1);
        Settings.Global.putLong(resolver,
                Settings.Global.MIN_DURATION_BETWEEN_RECOVERY_STEPS_IN_MS, 100);
        Settings.System.putInt(resolver, "radio.data.stall.recovery.action", 1);
        doReturn(new SignalStrength()).when(mPhone).getSignalStrength();

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT, PhoneConstants.APN_TYPE_MMS});
        mDct.enableApn(ApnSetting.TYPE_IMS, DcTracker.REQUEST_TYPE_NORMAL, null);
        mDct.enableApn(ApnSetting.TYPE_DEFAULT, DcTracker.REQUEST_TYPE_NORMAL, null);

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));

        ArgumentCaptor<DataProfile> dpCaptor = ArgumentCaptor.forClass(DataProfile.class);
        verify(mSimulatedCommandsVerifier, timeout(TEST_TIMEOUT).times(2)).setupDataCall(
                eq(AccessNetworkType.EUTRAN), dpCaptor.capture(),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
        verifyDataProfile(dpCaptor.getValue(), FAKE_APN1, 0, 21, 1, NETWORK_TYPE_LTE_BITMASK);

        logd("Sending EVENT_NETWORK_STATUS_CHANGED false");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_NETWORK_STATUS_CHANGED,
                NetworkAgent.INVALID_NETWORK, 0, null));
        waitForMs(200);

        // expected tear down all DataConnections
        verify(mSimulatedCommandsVerifier, times(1)).deactivateDataCall(
                eq(DataService.REQUEST_REASON_NORMAL), anyInt(),
                any(Message.class));
    }


    @Test
    @SmallTest
    public void testRecoveryStepReRegister() throws Exception {
        ContentResolver resolver = mContext.getContentResolver();
        Settings.Global.putInt(resolver, Settings.Global.DATA_STALL_RECOVERY_ON_BAD_NETWORK, 1);
        Settings.Global.putLong(resolver,
                Settings.Global.MIN_DURATION_BETWEEN_RECOVERY_STEPS_IN_MS, 100);
        Settings.System.putInt(resolver, "radio.data.stall.recovery.action", 2);
        doReturn(new SignalStrength()).when(mPhone).getSignalStrength();

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT, PhoneConstants.APN_TYPE_MMS});
        mDct.enableApn(ApnSetting.TYPE_DEFAULT, DcTracker.REQUEST_TYPE_NORMAL, null);

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForMs(200);

        ArgumentCaptor<DataProfile> dpCaptor = ArgumentCaptor.forClass(DataProfile.class);
        verify(mSimulatedCommandsVerifier, times(1)).setupDataCall(
                eq(AccessNetworkType.EUTRAN), dpCaptor.capture(),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
        verifyDataProfile(dpCaptor.getValue(), FAKE_APN1, 0, 21, 1, NETWORK_TYPE_LTE_BITMASK);

        logd("Sending EVENT_NETWORK_STATUS_CHANGED false");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_NETWORK_STATUS_CHANGED,
                NetworkAgent.INVALID_NETWORK, 0, null));
        waitForMs(200);

        // expected to get preferred network type
        verify(mSST, times(1)).reRegisterNetwork(eq(null));
    }

    @Test
    @SmallTest
    public void testRecoveryStepRestartRadio() throws Exception {
        ContentResolver resolver = mContext.getContentResolver();
        Settings.Global.putInt(resolver, Settings.Global.DATA_STALL_RECOVERY_ON_BAD_NETWORK, 1);
        Settings.Global.putLong(resolver,
                Settings.Global.MIN_DURATION_BETWEEN_RECOVERY_STEPS_IN_MS, 100);
        Settings.System.putInt(resolver, "radio.data.stall.recovery.action", 3);
        doReturn(new SignalStrength()).when(mPhone).getSignalStrength();

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT, PhoneConstants.APN_TYPE_MMS});
        mDct.enableApn(ApnSetting.TYPE_DEFAULT, DcTracker.REQUEST_TYPE_NORMAL, null);

        logd("Sending EVENT_RECORDS_LOADED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_RECORDS_LOADED, null));
        waitForMs(200);

        logd("Sending EVENT_DATA_CONNECTION_ATTACHED");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_DATA_CONNECTION_ATTACHED, null));
        waitForMs(200);

        ArgumentCaptor<DataProfile> dpCaptor = ArgumentCaptor.forClass(DataProfile.class);
        verify(mSimulatedCommandsVerifier, times(1)).setupDataCall(
                eq(AccessNetworkType.EUTRAN), dpCaptor.capture(),
                eq(false), eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(),
                any(Message.class));
        verifyDataProfile(dpCaptor.getValue(), FAKE_APN1, 0, 21, 1, NETWORK_TYPE_LTE_BITMASK);

        logd("Sending EVENT_NETWORK_STATUS_CHANGED false");
        mDct.sendMessage(mDct.obtainMessage(DctConstants.EVENT_NETWORK_STATUS_CHANGED,
                NetworkAgent.INVALID_NETWORK, 0, null));
        waitForMs(200);

        // expected to get preferred network type
        verify(mSST, times(1)).powerOffRadioSafely();
    }

    private void verifyDataEnabledChangedMessage(boolean enabled, int reason) {
        verify(mHandler, times(1)).sendMessageDelayed(any(), anyLong());
        Pair<Boolean, Integer> result = (Pair) ((AsyncResult) mMessage.obj).result;
        assertEquals(DATA_ENABLED_CHANGED, mMessage.what);
        assertEquals(enabled, result.first);
        assertEquals(reason, (int) result.second);
        clearInvocations(mHandler);
    }
}
