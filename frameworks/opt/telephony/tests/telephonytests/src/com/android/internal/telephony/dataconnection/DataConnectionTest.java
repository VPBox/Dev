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

import static android.net.NetworkCapabilities.NET_CAPABILITY_NOT_CONGESTED;
import static android.net.NetworkCapabilities.NET_CAPABILITY_NOT_METERED;
import static android.net.NetworkPolicyManager.OVERRIDE_CONGESTED;
import static android.net.NetworkPolicyManager.OVERRIDE_UNMETERED;

import static com.android.internal.telephony.TelephonyTestUtils.waitForMs;
import static com.android.internal.telephony.dataconnection.DcTrackerTest.FAKE_ADDRESS;
import static com.android.internal.telephony.dataconnection.DcTrackerTest.FAKE_DNS;
import static com.android.internal.telephony.dataconnection.DcTrackerTest.FAKE_GATEWAY;
import static com.android.internal.telephony.dataconnection.DcTrackerTest.FAKE_IFNAME;
import static com.android.internal.telephony.dataconnection.DcTrackerTest.FAKE_PCSCF_ADDRESS;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.content.IntentFilter;
import android.content.pm.ServiceInfo;
import android.net.KeepalivePacketData;
import android.net.LinkAddress;
import android.net.LinkProperties;
import android.net.NattKeepalivePacketData;
import android.net.NetworkCapabilities;
import android.net.NetworkInfo;
import android.net.NetworkUtils;
import android.os.AsyncResult;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.telephony.AccessNetworkConstants;
import android.telephony.AccessNetworkConstants.AccessNetworkType;
import android.telephony.CarrierConfigManager;
import android.telephony.ServiceState;
import android.telephony.data.ApnSetting;
import android.telephony.data.DataCallResponse;
import android.telephony.data.DataProfile;
import android.telephony.data.DataService;
import android.test.suitebuilder.annotation.MediumTest;
import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.R;
import com.android.internal.telephony.PhoneConstants;
import com.android.internal.telephony.RetryManager;
import com.android.internal.telephony.TelephonyTest;
import com.android.internal.telephony.dataconnection.DataConnection.ConnectionParams;
import com.android.internal.telephony.dataconnection.DataConnection.DisconnectParams;
import com.android.internal.telephony.dataconnection.DataConnection.SetupResult;
import com.android.internal.util.IState;
import com.android.internal.util.StateMachine;
import com.android.server.pm.PackageManagerService;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.Arrays;

public class DataConnectionTest extends TelephonyTest {

    @Mock
    DcTesterFailBringUpAll mDcTesterFailBringUpAll;
    @Mock
    ConnectionParams mCp;
    @Mock
    DisconnectParams mDcp;
    @Mock
    ApnContext mApnContext;
    @Mock
    DcFailBringUp mDcFailBringUp;
    @Mock
    PackageManagerService mMockPackageManagerInternal;

    private DataConnection mDc;
    private DataConnectionTestHandler mDataConnectionTestHandler;
    private DcController mDcc;

    private ApnSetting mApn1 = ApnSetting.makeApnSetting(
            2163,                   // id
            "44010",                // numeric
            "sp-mode",              // name
            "spmode.ne.jp",         // apn
            null,                     // proxy
            -1,                     // port
            null,                     // mmsc
            null,                     // mmsproxy
            -1,                     // mmsport
            "",                     // user
            "",                     // password
            -1,                     // authtype
            ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_SUPL,     // types
            ApnSetting.PROTOCOL_IP,                   // protocol
            ApnSetting.PROTOCOL_IP,                   // roaming_protocol
            true,                   // carrier_enabled
            0,                      // networktype_bitmask
            0,                      // profile_id
            false,                  // modem_cognitive
            0,                      // max_conns
            0,                      // wait_time
            0,                      // max_conns_time
            0,                      // mtu
            -1,                     // mvno_type
            "");                    // mnvo_match_data

    private ApnSetting mApn2 = ApnSetting.makeApnSetting(
            2164,                   // id
            "44010",                // numeric
            "sp-mode",              // name
            "spmode.ne.jp",         // apn
            null,                     // proxy
            -1,                     // port
            null,                     // mmsc
            null,                     // mmsproxy
            -1,                     // mmsport
            "",                     // user
            "",                     // password
            -1,                     // authtype
            ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_DUN,     // types
            ApnSetting.PROTOCOL_IP,                   // protocol
            ApnSetting.PROTOCOL_IP,                   // roaming_protocol
            true,                   // carrier_enabled
            0,                      // networktype_bitmask
            0,                      // profile_id
            false,                  // modem_cognitive
            0,                      // max_conns
            0,                      // wait_time
            0,                      // max_conns_time
            0,                      // mtu
            -1,                     // mvno_type
            "");                    // mnvo_match_data

    private ApnSetting mApn3 = ApnSetting.makeApnSetting(
            2164,                   // id
            "44010",                // numeric
            "sp-mode",              // name
            "spmode.ne.jp",         // apn
            null,                   // proxy
            -1,                     // port
            null,                   // mmsc
            null,                   // mmsproxy
            -1,                     // mmsport
            "",                     // user
            "",                     // password
            -1,                     // authtype
            ApnSetting.TYPE_DEFAULT, // types
            ApnSetting.PROTOCOL_IPV6, // protocol
            ApnSetting.PROTOCOL_IP, // roaming_protocol
            true,                   // carrier_enabled
            0,                      // networktype_bitmask
            0,                      // profile_id
            false,                  // modem_cognitive
            0,                      // max_conns
            0,                      // wait_time
            0,                      // max_conns_time
            0,                      // mtu
            -1,                     // mvno_type
            "",                     // mnvo_match_data
            0,                      // apn_set_id
            -1,                     // carrier_id
            1);                     // skip_464xlat

    private ApnSetting mApn4 = ApnSetting.makeApnSetting(
            2164,                   // id
            "44010",                // numeric
            "sp-mode",              // name
            "spmode.ne.jp",         // apn
            null,                   // proxy
            -1,                     // port
            null,                   // mmsc
            null,                   // mmsproxy
            -1,                     // mmsport
            "",                     // user
            "",                     // password
            -1,                     // authtype
            ApnSetting.TYPE_IMS,    // types
            ApnSetting.PROTOCOL_IPV6, // protocol
            ApnSetting.PROTOCOL_IP, // roaming_protocol
            true,                   // carrier_enabled
            0,                      // networktype_bitmask
            0,                      // profile_id
            false,                  // modem_cognitive
            0,                      // max_conns
            0,                      // wait_time
            0,                      // max_conns_time
            0,                      // mtu
            -1,                     // mvno_type
            "");                    // mnvo_match_data

    private ApnSetting mApn5 = ApnSetting.makeApnSetting(
            2164,                   // id
            "44010",                // numeric
            "sp-mode",              // name
            "spmode.ne.jp",         // apn
            null,                   // proxy
            -1,                     // port
            null,                   // mmsc
            null,                   // mmsproxy
            -1,                     // mmsport
            "",                     // user
            "",                     // password
            -1,                     // authtype
            ApnSetting.TYPE_IMS,    // types
            ApnSetting.PROTOCOL_IPV6, // protocol
            ApnSetting.PROTOCOL_IP, // roaming_protocol
            true,                   // carrier_enabled
            0,                      // networktype_bitmask
            0,                      // profile_id
            false,                  // modem_cognitive
            0,                      // max_conns
            0,                      // wait_time
            0,                      // max_conns_time
            0,                      // mtu
            -1,                     // mvno_type
            "",                     // mnvo_match_data
            0,                      // apn_set_id
            -1,                     // carrier_id
            0);                     // skip_464xlat

    private class DataConnectionTestHandler extends HandlerThread {

        private DataConnectionTestHandler(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            Handler h = new Handler();

            DataServiceManager manager = new DataServiceManager(mPhone,
                    AccessNetworkConstants.TRANSPORT_TYPE_WWAN, "");
            mDcc = DcController.makeDcc(mPhone, mDcTracker, manager, h, "");
            mDcc.start();
            mDc = DataConnection.makeDataConnection(mPhone, 0, mDcTracker, manager,
                    mDcTesterFailBringUpAll, mDcc);
        }
    }

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


    @Before
    public void setUp() throws Exception {
        logd("+Setup!");
        super.setUp(getClass().getSimpleName());
        mServiceManagerMockedServices.put("package", mMockPackageManagerInternal);
        doReturn("fake.action_detached").when(mPhone).getActionDetached();
        replaceInstance(ConnectionParams.class, "mApnContext", mCp, mApnContext);
        replaceInstance(ConnectionParams.class, "mRilRat", mCp,
                ServiceState.RIL_RADIO_TECHNOLOGY_UMTS);
        doReturn(mApn1).when(mApnContext).getApnSetting();
        doReturn(PhoneConstants.APN_TYPE_DEFAULT).when(mApnContext).getApnType();

        mDcFailBringUp.saveParameters(0, 0, -2);
        doReturn(mDcFailBringUp).when(mDcTesterFailBringUpAll).getDcFailBringUp();

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

        mDcp.mApnContext = mApnContext;

        addDataService();

        mDataConnectionTestHandler = new DataConnectionTestHandler(getClass().getSimpleName());
        mDataConnectionTestHandler.start();

        waitForMs(200);
        logd("-Setup!");
    }

    @After
    public void tearDown() throws Exception {
        logd("tearDown");
        mDc = null;
        mDcc = null;
        mDataConnectionTestHandler.quit();
        super.tearDown();
    }

    private IState getCurrentState() throws Exception {
        Method method = StateMachine.class.getDeclaredMethod("getCurrentState");
        method.setAccessible(true);
        return (IState) method.invoke(mDc);
    }

    private long getSuggestedRetryDelay(DataCallResponse response) throws Exception {
        Class[] cArgs = new Class[1];
        cArgs[0] = DataCallResponse.class;
        Method method = DataConnection.class.getDeclaredMethod("getSuggestedRetryDelay", cArgs);
        method.setAccessible(true);
        return (long) method.invoke(mDc, response);
    }

    private boolean isUnmeteredUseOnly() throws Exception {
        Method method = DataConnection.class.getDeclaredMethod("isUnmeteredUseOnly");
        method.setAccessible(true);
        return (boolean) method.invoke(mDc);
    }

    private SetupResult setLinkProperties(DataCallResponse response,
                                                         LinkProperties linkProperties)
            throws Exception {
        Class[] cArgs = new Class[2];
        cArgs[0] = DataCallResponse.class;
        cArgs[1] = LinkProperties.class;
        Method method = DataConnection.class.getDeclaredMethod("setLinkProperties", cArgs);
        method.setAccessible(true);
        return (SetupResult) method.invoke(mDc, response, linkProperties);
    }

    @Test
    @SmallTest
    public void testSanity() throws Exception {
        assertEquals("DcInactiveState", getCurrentState().getName());
    }

    @Test
    @SmallTest
    public void testConnectEvent() throws Exception {
        testSanity();

        mDc.sendMessage(DataConnection.EVENT_CONNECT, mCp);
        waitForMs(200);

        verify(mCT, times(1)).registerForVoiceCallStarted(any(Handler.class),
                eq(DataConnection.EVENT_DATA_CONNECTION_VOICE_CALL_STARTED), eq(null));
        verify(mCT, times(1)).registerForVoiceCallEnded(any(Handler.class),
                eq(DataConnection.EVENT_DATA_CONNECTION_VOICE_CALL_ENDED), eq(null));
        verify(mSimulatedCommandsVerifier, times(1))
                .registerForNattKeepaliveStatus(any(Handler.class),
                        eq(DataConnection.EVENT_KEEPALIVE_STATUS), eq(null));
        verify(mSimulatedCommandsVerifier, times(1))
                .registerForLceInfo(any(Handler.class),
                        eq(DataConnection.EVENT_LINK_CAPACITY_CHANGED), eq(null));

        ArgumentCaptor<DataProfile> dpCaptor = ArgumentCaptor.forClass(DataProfile.class);
        verify(mSimulatedCommandsVerifier, times(1)).setupDataCall(
                eq(AccessNetworkType.UTRAN), dpCaptor.capture(), eq(false),
                eq(false), eq(DataService.REQUEST_REASON_NORMAL), any(), any(Message.class));

        assertEquals("spmode.ne.jp", dpCaptor.getValue().getApn());

        assertEquals("DcActiveState", getCurrentState().getName());

        assertEquals(3, mDc.getPcscfAddresses().length);
        assertTrue(Arrays.stream(mDc.getPcscfAddresses()).anyMatch("fd00:976a:c305:1d::8"::equals));
        assertTrue(Arrays.stream(mDc.getPcscfAddresses()).anyMatch("fd00:976a:c202:1d::7"::equals));
        assertTrue(Arrays.stream(mDc.getPcscfAddresses()).anyMatch("fd00:976a:c305:1d::5"::equals));
    }

    @Test
    @SmallTest
    public void testDisconnectEvent() throws Exception {
        testConnectEvent();

        mDc.sendMessage(DataConnection.EVENT_DISCONNECT, mDcp);
        waitForMs(100);

        verify(mSimulatedCommandsVerifier, times(1)).unregisterForLceInfo(any(Handler.class));
        verify(mSimulatedCommandsVerifier, times(1))
                .unregisterForNattKeepaliveStatus(any(Handler.class));
        verify(mSimulatedCommandsVerifier, times(1)).deactivateDataCall(eq(1),
                eq(DataService.REQUEST_REASON_NORMAL), any(Message.class));

        assertEquals("DcInactiveState", getCurrentState().getName());
    }

    @Test
    @SmallTest
    public void testModemSuggestRetry() throws Exception {
        DataCallResponse response = new DataCallResponse(0, 0, 1, 2,
                ApnSetting.PROTOCOL_IP, FAKE_IFNAME,
                Arrays.asList(new LinkAddress(NetworkUtils.numericToInetAddress(FAKE_ADDRESS), 0)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_DNS)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_GATEWAY)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_PCSCF_ADDRESS)),
                1440);

        assertEquals(response.getSuggestedRetryTime(), getSuggestedRetryDelay(response));

        response = new DataCallResponse(0, 1000, 1, 2,
                ApnSetting.PROTOCOL_IP, FAKE_IFNAME,
                Arrays.asList(new LinkAddress(NetworkUtils.numericToInetAddress(FAKE_ADDRESS), 0)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_DNS)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_GATEWAY)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_PCSCF_ADDRESS)),
                1440);
        assertEquals(response.getSuggestedRetryTime(), getSuggestedRetryDelay(response));

        response = new DataCallResponse(0, 9999, 1, 2,
                ApnSetting.PROTOCOL_IP, FAKE_IFNAME,
                Arrays.asList(new LinkAddress(NetworkUtils.numericToInetAddress(FAKE_ADDRESS), 0)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_DNS)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_GATEWAY)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_PCSCF_ADDRESS)),
                1440);
        assertEquals(response.getSuggestedRetryTime(), getSuggestedRetryDelay(response));
    }

    @Test
    @SmallTest
    public void testModemNotSuggestRetry() throws Exception {
        DataCallResponse response = new DataCallResponse(0, -1, 1, 2,
                ApnSetting.PROTOCOL_IP, FAKE_IFNAME,
                Arrays.asList(new LinkAddress(NetworkUtils.numericToInetAddress(FAKE_ADDRESS), 0)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_DNS)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_GATEWAY)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_PCSCF_ADDRESS)),
                1440);

        assertEquals(RetryManager.NO_SUGGESTED_RETRY_DELAY, getSuggestedRetryDelay(response));

        response = new DataCallResponse(0, -5, 1, 2,
                ApnSetting.PROTOCOL_IP, FAKE_IFNAME,
                Arrays.asList(new LinkAddress(NetworkUtils.numericToInetAddress(FAKE_ADDRESS), 0)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_DNS)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_GATEWAY)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_PCSCF_ADDRESS)),
                1440);
        assertEquals(RetryManager.NO_SUGGESTED_RETRY_DELAY, getSuggestedRetryDelay(response));

        response = new DataCallResponse(0, Integer.MIN_VALUE, 1, 2,
                ApnSetting.PROTOCOL_IP, FAKE_IFNAME,
                Arrays.asList(new LinkAddress(NetworkUtils.numericToInetAddress(FAKE_ADDRESS), 0)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_DNS)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_GATEWAY)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_PCSCF_ADDRESS)),
                1440);
        assertEquals(RetryManager.NO_SUGGESTED_RETRY_DELAY, getSuggestedRetryDelay(response));
    }

    @Test
    @SmallTest
    public void testModemSuggestNoRetry() throws Exception {
        DataCallResponse response = new DataCallResponse(0, Integer.MAX_VALUE, 1, 2,
                ApnSetting.PROTOCOL_IP, FAKE_IFNAME,
                Arrays.asList(new LinkAddress(NetworkUtils.numericToInetAddress(FAKE_ADDRESS), 0)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_DNS)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_GATEWAY)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_PCSCF_ADDRESS)),
                1440);
        assertEquals(RetryManager.NO_RETRY, getSuggestedRetryDelay(response));
    }

    private NetworkInfo getNetworkInfo() throws Exception {
        Field f = DataConnection.class.getDeclaredField("mNetworkInfo");
        f.setAccessible(true);
        return (NetworkInfo) f.get(mDc);
    }

    private NetworkCapabilities getNetworkCapabilities() throws Exception {
        Method method = DataConnection.class.getDeclaredMethod("getNetworkCapabilities");
        method.setAccessible(true);
        return (NetworkCapabilities) method.invoke(mDc);
    }

    private int getDisallowedApnTypes() throws Exception {
        Method method = DataConnection.class.getDeclaredMethod("getDisallowedApnTypes");
        method.setAccessible(true);
        return (int) method.invoke(mDc);
    }

    @Test
    @SmallTest
    public void testNetworkCapability() throws Exception {
        mContextFixture.getCarrierConfigBundle().putStringArray(
                CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[] { "default" });
        doReturn(mApn2).when(mApnContext).getApnSetting();
        testConnectEvent();

        assertTrue("capabilities: " + getNetworkCapabilities(), getNetworkCapabilities()
                .hasCapability(NetworkCapabilities.NET_CAPABILITY_DUN));
        assertTrue("capabilities: " + getNetworkCapabilities(), getNetworkCapabilities()
                .hasCapability(NetworkCapabilities.NET_CAPABILITY_INTERNET));
        assertFalse("capabilities: " + getNetworkCapabilities(), getNetworkCapabilities()
                .hasCapability(NetworkCapabilities.NET_CAPABILITY_MMS));

        mContextFixture.getCarrierConfigBundle().putStringArray(
                CarrierConfigManager.KEY_CARRIER_WWAN_DISALLOWED_APN_TYPES_STRING_ARRAY,
                new String[] {"supl"});

        mDc.sendMessage(DataConnection.EVENT_DISCONNECT, mDcp);
        waitForMs(100);
        doReturn(mApn1).when(mApnContext).getApnSetting();
        mDc.sendMessage(DataConnection.EVENT_CONNECT, mCp);
        waitForMs(200);

        assertFalse("capabilities: " + getNetworkCapabilities(), getNetworkCapabilities()
                .hasCapability(NetworkCapabilities.NET_CAPABILITY_DUN));
        assertTrue("capabilities: " + getNetworkCapabilities(), getNetworkCapabilities()
                .hasCapability(NetworkCapabilities.NET_CAPABILITY_INTERNET));
        assertFalse("capabilities: " + getNetworkCapabilities(), getNetworkCapabilities()
                .hasCapability(NetworkCapabilities.NET_CAPABILITY_SUPL));
    }

    @Test
    @SmallTest
    public void testMeteredCapability() throws Exception {

        mContextFixture.getCarrierConfigBundle().
                putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[] {"default"});

        testConnectEvent();

        assertFalse(getNetworkCapabilities()
                .hasCapability(NetworkCapabilities.NET_CAPABILITY_NOT_METERED));
    }

    @Test
    @SmallTest
    public void testNonMeteredCapability() throws Exception {

        doReturn(2819).when(mPhone).getSubId();
        mContextFixture.getCarrierConfigBundle().
                putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                        new String[] {"mms"});

        testConnectEvent();

        assertTrue(getNetworkCapabilities()
                .hasCapability(NetworkCapabilities.NET_CAPABILITY_NOT_METERED));
    }

    @Test
    public void testOverrideUnmetered() throws Exception {
        mContextFixture.getCarrierConfigBundle().putStringArray(
                CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[] { "default" });
        testConnectEvent();

        assertFalse(getNetworkCapabilities().hasCapability(NET_CAPABILITY_NOT_METERED));
        assertTrue(getNetworkCapabilities().hasCapability(NET_CAPABILITY_NOT_CONGESTED));

        mDc.onSubscriptionOverride(OVERRIDE_UNMETERED, OVERRIDE_UNMETERED);

        assertTrue(getNetworkCapabilities().hasCapability(NET_CAPABILITY_NOT_METERED));
        assertTrue(getNetworkCapabilities().hasCapability(NET_CAPABILITY_NOT_CONGESTED));

        mDc.onSubscriptionOverride(OVERRIDE_UNMETERED, 0);

        assertFalse(getNetworkCapabilities().hasCapability(NET_CAPABILITY_NOT_METERED));
        assertTrue(getNetworkCapabilities().hasCapability(NET_CAPABILITY_NOT_CONGESTED));
    }

    @Test
    public void testOverrideCongested() throws Exception {
        mContextFixture.getCarrierConfigBundle().putStringArray(
                CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[] { "default" });
        testConnectEvent();

        assertFalse(getNetworkCapabilities().hasCapability(NET_CAPABILITY_NOT_METERED));
        assertTrue(getNetworkCapabilities().hasCapability(NET_CAPABILITY_NOT_CONGESTED));

        mDc.onSubscriptionOverride(OVERRIDE_CONGESTED, OVERRIDE_CONGESTED);

        assertFalse(getNetworkCapabilities().hasCapability(NET_CAPABILITY_NOT_METERED));
        assertFalse(getNetworkCapabilities().hasCapability(NET_CAPABILITY_NOT_CONGESTED));

        mDc.onSubscriptionOverride(OVERRIDE_CONGESTED, 0);

        assertFalse(getNetworkCapabilities().hasCapability(NET_CAPABILITY_NOT_METERED));
        assertTrue(getNetworkCapabilities().hasCapability(NET_CAPABILITY_NOT_CONGESTED));
    }

    @Test
    public void testShouldSkip464Xlat() throws Exception {
        assertFalse(testShouldSkip464XlatEvent(mApn1));
        disconnectEvent();

        assertTrue(testShouldSkip464XlatEvent(mApn3));
        disconnectEvent();

        assertTrue(testShouldSkip464XlatEvent(mApn4));
        disconnectEvent();

        assertFalse(testShouldSkip464XlatEvent(mApn5));
        disconnectEvent();
    }

    private boolean testShouldSkip464XlatEvent(ApnSetting apn) throws Exception {
        Method method = DataConnection.class.getDeclaredMethod("shouldSkip464Xlat");
        method.setAccessible(true);

        doReturn(apn).when(mApnContext).getApnSetting();
        connectEvent();
        logd(getNetworkCapabilities().toString());

        return (Boolean) method.invoke(mDc);
    }

    private void connectEvent() throws Exception {
        mDc.sendMessage(DataConnection.EVENT_CONNECT, mCp);
        waitForMs(200);
        assertEquals("DcActiveState", getCurrentState().getName());
    }

    private void disconnectEvent() throws Exception {
        mDc.sendMessage(DataConnection.EVENT_DISCONNECT, mDcp);
        waitForMs(100);
        assertEquals("DcInactiveState", getCurrentState().getName());
    }

    @Test
    @SmallTest
    public void testIsIpAddress() throws Exception {
        // IPv4
        assertTrue(DataConnection.isIpAddress("1.2.3.4"));
        assertTrue(DataConnection.isIpAddress("127.0.0.1"));

        // IPv6
        assertTrue(DataConnection.isIpAddress("::1"));
        assertTrue(DataConnection.isIpAddress("2001:4860:800d::68"));
    }

    @Test
    @SmallTest
    public void testSetLinkProperties() throws Exception {

        DataCallResponse response = new DataCallResponse(0, -1, 1, 2,
                ApnSetting.PROTOCOL_IP, FAKE_IFNAME,
                Arrays.asList(new LinkAddress(NetworkUtils.numericToInetAddress(FAKE_ADDRESS), 0)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_DNS)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_GATEWAY)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_PCSCF_ADDRESS)),
                1440);

        LinkProperties linkProperties = new LinkProperties();
        assertEquals(SetupResult.SUCCESS, setLinkProperties(response, linkProperties));
        logd(linkProperties.toString());
        assertEquals(response.getInterfaceName(), linkProperties.getInterfaceName());
        assertEquals(response.getAddresses().size(), linkProperties.getAddresses().size());
        for (int i = 0; i < response.getAddresses().size(); ++i) {
            assertEquals(response.getAddresses().get(i).getAddress(),
                    NetworkUtils.numericToInetAddress(linkProperties.getLinkAddresses().get(i)
                            .getAddress().getHostAddress()));
        }

        assertEquals(response.getDnsAddresses().size(), linkProperties.getDnsServers().size());
        for (int i = 0; i < response.getDnsAddresses().size(); ++i) {
            assertEquals("i = " + i, response.getDnsAddresses().get(i),
                    NetworkUtils.numericToInetAddress(
                            linkProperties.getDnsServers().get(i).getHostAddress()));
        }

        assertEquals(response.getGatewayAddresses().size(), linkProperties.getRoutes().size());
        for (int i = 0; i < response.getGatewayAddresses().size(); ++i) {
            assertEquals("i = " + i, response.getGatewayAddresses().get(i),
                    NetworkUtils.numericToInetAddress(linkProperties.getRoutes().get(i)
                            .getGateway().getHostAddress()));
        }

        assertEquals(response.getPcscfAddresses().size(), linkProperties.getPcscfServers().size());
        for (int i = 0; i < response.getPcscfAddresses().size(); ++i) {
            assertEquals("i = " + i, response.getPcscfAddresses().get(i),
                    NetworkUtils.numericToInetAddress(linkProperties.getPcscfServers().get(i)
                            .getHostAddress()));
        }

        assertEquals(response.getMtu(), linkProperties.getMtu());
    }

    @Test
    @SmallTest
    public void testSetLinkPropertiesEmptyAddress() throws Exception {

        // 224.224.224.224 is an invalid address.
        DataCallResponse response = new DataCallResponse(0, -1, 1, 2,
                ApnSetting.PROTOCOL_IP, FAKE_IFNAME,
                null,
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_DNS)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_GATEWAY)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_PCSCF_ADDRESS)),
                1440);

        LinkProperties linkProperties = new LinkProperties();
        assertEquals(SetupResult.ERROR_INVALID_ARG,
                setLinkProperties(response, linkProperties));
    }

    @Test
    @SmallTest
    public void testSetLinkPropertiesEmptyDns() throws Exception {

        // Empty dns entry.
        DataCallResponse response = new DataCallResponse(0, -1, 1, 2,
                ApnSetting.PROTOCOL_IP, FAKE_IFNAME,
                Arrays.asList(new LinkAddress(NetworkUtils.numericToInetAddress(FAKE_ADDRESS), 0)),
                null,
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_GATEWAY)),
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_PCSCF_ADDRESS)),
                1440);

        // Make sure no exception was thrown
        LinkProperties linkProperties = new LinkProperties();
        assertEquals(SetupResult.SUCCESS, setLinkProperties(response, linkProperties));
    }

    @Test
    @SmallTest
    public void testStartKeepaliveWLAN() throws Exception {
        testConnectEvent();
        waitForMs(200);

        Field field = DataConnection.class.getDeclaredField("mTransportType");
        field.setAccessible(true);
        field.setInt(mDc, AccessNetworkConstants.TRANSPORT_TYPE_WLAN);

        final int sessionHandle = 0xF00;
        final int slotId = 3;
        final int interval = 10; // seconds
        // Construct a new KeepalivePacketData request as we would receive from a Network Agent,
        // and check that the packet is sent to the RIL.
        KeepalivePacketData kd = NattKeepalivePacketData.nattKeepalivePacket(
                NetworkUtils.numericToInetAddress("1.2.3.4"),
                1234,
                NetworkUtils.numericToInetAddress("8.8.8.8"),
                4500);
        mDc.obtainMessage(
                DataConnection.EVENT_KEEPALIVE_START_REQUEST, slotId, interval, kd).sendToTarget();
        waitForMs(100);
        // testStartStopNattKeepalive() verifies that this request is passed with WWAN.
        // Thus, even though we can't see the response in NetworkAgent, we can verify that the
        // CommandsInterface never receives a request and infer that it was dropped due to WLAN.
        verify(mSimulatedCommandsVerifier, times(0))
                .startNattKeepalive(anyInt(), eq(kd), eq(interval * 1000), any(Message.class));
    }

    public void checkStartStopNattKeepalive(boolean useCondensedFlow) throws Exception {
        testConnectEvent();
        waitForMs(200);

        final int sessionHandle = 0xF00;
        final int slotId = 3;
        final int interval = 10; // seconds
        // Construct a new KeepalivePacketData request as we would receive from a Network Agent,
        // and check that the packet is sent to the RIL.
        KeepalivePacketData kd = NattKeepalivePacketData.nattKeepalivePacket(
                NetworkUtils.numericToInetAddress("1.2.3.4"),
                1234,
                NetworkUtils.numericToInetAddress("8.8.8.8"),
                4500);
        mDc.obtainMessage(
                DataConnection.EVENT_KEEPALIVE_START_REQUEST, slotId, interval, kd).sendToTarget();
        waitForMs(100);
        verify(mSimulatedCommandsVerifier, times(1))
                .startNattKeepalive(anyInt(), eq(kd), eq(interval * 1000), any(Message.class));

        Message kaStarted = mDc.obtainMessage(DataConnection.EVENT_KEEPALIVE_STARTED, slotId, 0);
        if (useCondensedFlow) {
            // Send a singled condensed response that a keepalive have been requested and the
            // activation is completed. This flow should be used if the keepalive offload request
            // is handled by a high-priority signalling path.
            AsyncResult.forMessage(
                    kaStarted, new KeepaliveStatus(
                            sessionHandle, KeepaliveStatus.STATUS_ACTIVE), null);
            kaStarted.sendToTarget();
        } else {
            // Send the sequential responses indicating first that the request was received and
            // then that the keepalive is running. This should create an active record of the
            // keepalive in DataConnection while permitting the status from a low priority or other
            // high-latency handler to activate the keepalive without blocking a request.
            AsyncResult.forMessage(
                    kaStarted, new KeepaliveStatus(
                            sessionHandle, KeepaliveStatus.STATUS_PENDING), null);
            kaStarted.sendToTarget();
            Message kaRunning = mDc.obtainMessage(DataConnection.EVENT_KEEPALIVE_STATUS);
            AsyncResult.forMessage(
                    kaRunning, new KeepaliveStatus(
                            sessionHandle, KeepaliveStatus.STATUS_ACTIVE), null);
            kaRunning.sendToTarget();
        }
        waitForMs(100);

        // Verify that we can stop the connection, which checks that the record in DataConnection
        // has a valid mapping between slotId (from network agent) to sessionHandle (from Radio).
        mDc.obtainMessage(DataConnection.EVENT_KEEPALIVE_STOP_REQUEST, slotId).sendToTarget();
        waitForMs(100);
        verify(mSimulatedCommandsVerifier, times(1))
                .stopNattKeepalive(eq(sessionHandle), any(Message.class));

        Message kaStopped = mDc.obtainMessage(
                DataConnection.EVENT_KEEPALIVE_STOPPED, sessionHandle, slotId);
        AsyncResult.forMessage(kaStopped);
        kaStopped.sendToTarget();
        // Verify that after the connection is stopped, the mapping for a Keepalive Session is
        // removed. Thus, subsequent calls to stop the same keepalive are ignored.
        mDc.obtainMessage(DataConnection.EVENT_KEEPALIVE_STOP_REQUEST, slotId).sendToTarget();
        waitForMs(100);
        // Check that the mock has not been called subsequent to the previous invocation
        // while avoiding the use of reset()
        verify(mSimulatedCommandsVerifier, times(1))
                .stopNattKeepalive(anyInt(), any(Message.class));
    }

    @Test
    @MediumTest
    public void testStartStopNattKeepalive() throws Exception {
        checkStartStopNattKeepalive(false);
    }

    @Test
    @MediumTest
    public void testStartStopNattKeepaliveCondensed() throws Exception {
        checkStartStopNattKeepalive(true);
    }

    public void checkStartNattKeepaliveFail(boolean useCondensedFlow) throws Exception {
        testConnectEvent();
        waitForMs(200);

        final int sessionHandle = 0xF00;
        final int slotId = 3;
        final int interval = 10; // seconds
        // Construct a new KeepalivePacketData request as we would receive from a Network Agent,
        // and check that the packet is sent to the RIL.
        KeepalivePacketData kd = NattKeepalivePacketData.nattKeepalivePacket(
                NetworkUtils.numericToInetAddress("1.2.3.4"),
                1234,
                NetworkUtils.numericToInetAddress("8.8.8.8"),
                4500);
        mDc.obtainMessage(
                DataConnection.EVENT_KEEPALIVE_START_REQUEST, slotId, interval, kd).sendToTarget();
        waitForMs(100);
        verify(mSimulatedCommandsVerifier, times(1))
                .startNattKeepalive(anyInt(), eq(kd), eq(interval * 1000), any(Message.class));

        Message kaStarted = mDc.obtainMessage(DataConnection.EVENT_KEEPALIVE_STARTED, slotId, 0);
        if (useCondensedFlow) {
            // Indicate in the response that the keepalive has failed.
            AsyncResult.forMessage(
                    kaStarted, new KeepaliveStatus(KeepaliveStatus.ERROR_UNSUPPORTED), null);
            kaStarted.sendToTarget();
        } else {
            // Indicate that the keepalive is queued, and then signal a failure from the modem
            // such that a pending keepalive fails to activate.
            AsyncResult.forMessage(
                    kaStarted, new KeepaliveStatus(
                            sessionHandle, KeepaliveStatus.STATUS_PENDING), null);
            kaStarted.sendToTarget();
            Message kaRunning = mDc.obtainMessage(DataConnection.EVENT_KEEPALIVE_STATUS);
            AsyncResult.forMessage(
                    kaRunning, new KeepaliveStatus(
                            sessionHandle, KeepaliveStatus.STATUS_INACTIVE), null);
            kaRunning.sendToTarget();
        }
        waitForMs(100);
        // Verify that a failed connection request cannot be stopped due to no record in
        // the DataConnection.
        mDc.obtainMessage(DataConnection.EVENT_KEEPALIVE_STOP_REQUEST, slotId).sendToTarget();
        waitForMs(100);
        verify(mSimulatedCommandsVerifier, times(0))
                .stopNattKeepalive(anyInt(), any(Message.class));
    }

    @Test
    @SmallTest
    public void testStartNattKeepaliveFail() throws Exception {
        checkStartNattKeepaliveFail(false);
    }

    @Test
    @SmallTest
    public void testStartNattKeepaliveFailCondensed() throws Exception {
        checkStartNattKeepaliveFail(true);
    }

    @Test
    @SmallTest
    public void testIsUnmeteredUseOnly() throws Exception {
        Field field = DataConnection.class.getDeclaredField("mTransportType");
        field.setAccessible(true);
        field.setInt(mDc, AccessNetworkConstants.TRANSPORT_TYPE_WLAN);

        assertFalse(isUnmeteredUseOnly());

        field = DataConnection.class.getDeclaredField("mTransportType");
        field.setAccessible(true);
        field.setInt(mDc, AccessNetworkConstants.TRANSPORT_TYPE_WWAN);

        doReturn(false).when(mDataEnabledSettings).isDataEnabled();
        doReturn(false).when(mServiceState).getDataRoaming();
        doReturn(ApnSetting.TYPE_MMS).when(mApnContext).getApnTypeBitmask();

        mContextFixture.getCarrierConfigBundle().putStringArray(
                CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[] { "default" });

        assertTrue(isUnmeteredUseOnly());
    }

    @Test
    @SmallTest
    public void testGetDisallowedApnTypes() throws Exception {
        mContextFixture.getCarrierConfigBundle().putStringArray(
                CarrierConfigManager.KEY_CARRIER_WWAN_DISALLOWED_APN_TYPES_STRING_ARRAY,
                new String[] { "mms", "supl", "fota" });
        testConnectEvent();

        assertEquals(ApnSetting.TYPE_MMS | ApnSetting.TYPE_SUPL | ApnSetting.TYPE_FOTA,
                getDisallowedApnTypes());
    }
}
