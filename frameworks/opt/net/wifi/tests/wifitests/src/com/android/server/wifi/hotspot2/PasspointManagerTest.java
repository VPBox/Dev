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

package com.android.server.wifi.hotspot2;

import static android.app.AppOpsManager.MODE_IGNORED;
import static android.app.AppOpsManager.OPSTR_CHANGE_WIFI_STATE;
import static android.net.wifi.WifiManager.ACTION_PASSPOINT_DEAUTH_IMMINENT;
import static android.net.wifi.WifiManager.ACTION_PASSPOINT_ICON;
import static android.net.wifi.WifiManager.ACTION_PASSPOINT_SUBSCRIPTION_REMEDIATION;
import static android.net.wifi.WifiManager.EXTRA_BSSID_LONG;
import static android.net.wifi.WifiManager.EXTRA_DELAY;
import static android.net.wifi.WifiManager.EXTRA_ESS;
import static android.net.wifi.WifiManager.EXTRA_FILENAME;
import static android.net.wifi.WifiManager.EXTRA_ICON;
import static android.net.wifi.WifiManager.EXTRA_SUBSCRIPTION_REMEDIATION_METHOD;
import static android.net.wifi.WifiManager.EXTRA_URL;

import static org.hamcrest.collection.IsIterableContainingInAnyOrder.containsInAnyOrder;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertThat;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.isNull;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyBoolean;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.anyLong;
import static org.mockito.Mockito.anyMap;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.lenient;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.reset;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;
import static org.mockito.MockitoAnnotations.initMocks;

import android.app.AppOpsManager;
import android.content.Context;
import android.content.Intent;
import android.graphics.drawable.Icon;
import android.net.Uri;
import android.net.wifi.EAPConstants;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiEnterpriseConfig;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiSsid;
import android.net.wifi.hotspot2.IProvisioningCallback;
import android.net.wifi.hotspot2.OsuProvider;
import android.net.wifi.hotspot2.PasspointConfiguration;
import android.net.wifi.hotspot2.pps.Credential;
import android.net.wifi.hotspot2.pps.HomeSp;
import android.os.Handler;
import android.os.Looper;
import android.os.UserHandle;
import android.os.test.TestLooper;
import android.telephony.SubscriptionManager;
import android.telephony.TelephonyManager;
import android.util.Base64;
import android.util.Pair;

import androidx.test.filters.SmallTest;

import com.android.dx.mockito.inline.extended.ExtendedMockito;
import com.android.server.wifi.ClientModeImpl;
import com.android.server.wifi.Clock;
import com.android.server.wifi.FakeKeys;
import com.android.server.wifi.IMSIParameter;
import com.android.server.wifi.SIMAccessor;
import com.android.server.wifi.ScanDetail;
import com.android.server.wifi.WifiConfigManager;
import com.android.server.wifi.WifiConfigStore;
import com.android.server.wifi.WifiConfigurationTestUtil;
import com.android.server.wifi.WifiInjector;
import com.android.server.wifi.WifiKeyStore;
import com.android.server.wifi.WifiMetrics;
import com.android.server.wifi.WifiNative;
import com.android.server.wifi.hotspot2.anqp.ANQPElement;
import com.android.server.wifi.hotspot2.anqp.Constants.ANQPElementType;
import com.android.server.wifi.hotspot2.anqp.DomainNameElement;
import com.android.server.wifi.hotspot2.anqp.HSOsuProvidersElement;
import com.android.server.wifi.hotspot2.anqp.I18Name;
import com.android.server.wifi.hotspot2.anqp.NAIRealmData;
import com.android.server.wifi.hotspot2.anqp.NAIRealmElement;
import com.android.server.wifi.hotspot2.anqp.OsuProviderInfo;
import com.android.server.wifi.hotspot2.anqp.eap.EAPMethod;
import com.android.server.wifi.util.InformationElementUtil;
import com.android.server.wifi.util.InformationElementUtil.RoamingConsortium;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoSession;

import java.nio.charset.StandardCharsets;
import java.security.GeneralSecurityException;
import java.security.KeyStore;
import java.security.cert.X509Certificate;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

/**
 * Unit tests for {@link PasspointManager}.
 */
@SmallTest
public class PasspointManagerTest {
    private static final long BSSID = 0x112233445566L;
    private static final String TEST_PACKAGE = "com.android.test";
    private static final String ICON_FILENAME = "test";
    private static final String TEST_FQDN = "test1.test.com";
    private static final String TEST_FQDN2 = "test2.test.com";
    private static final String TEST_FRIENDLY_NAME = "friendly name";
    private static final String TEST_FRIENDLY_NAME2 = "second friendly name";
    private static final String TEST_REALM = "realm.test.com";
    private static final String TEST_IMSI = "123456*";
    private static final IMSIParameter TEST_IMSI_PARAM = IMSIParameter.build(TEST_IMSI);

    private static final long TEST_BSSID = 0x112233445566L;
    private static final String TEST_SSID = "TestSSID";
    private static final String TEST_BSSID_STRING = "11:22:33:44:55:66";
    private static final String TEST_SSID2 = "TestSSID2";
    private static final String TEST_BSSID_STRING2 = "11:22:33:44:55:77";
    private static final String TEST_SSID3 = "TestSSID3";
    private static final String TEST_BSSID_STRING3 = "11:22:33:44:55:88";
    private static final String TEST_MCC_MNC = "123456";
    private static final String TEST_3GPP_FQDN = String.format("wlan.mnc%s.mcc%s.3gppnetwork.org",
            TEST_MCC_MNC.substring(3), TEST_MCC_MNC.substring(0, 3));

    private static final long TEST_HESSID = 0x5678L;
    private static final int TEST_ANQP_DOMAIN_ID = 0;
    private static final int TEST_ANQP_DOMAIN_ID2 = 1;
    private static final ANQPNetworkKey TEST_ANQP_KEY = ANQPNetworkKey.buildKey(
            TEST_SSID, TEST_BSSID, TEST_HESSID, TEST_ANQP_DOMAIN_ID);
    private static final ANQPNetworkKey TEST_ANQP_KEY2 = ANQPNetworkKey.buildKey(
            TEST_SSID, TEST_BSSID, TEST_HESSID, TEST_ANQP_DOMAIN_ID2);
    private static final int TEST_CREATOR_UID = 1234;
    private static final int TEST_UID = 1500;

    @Mock Context mContext;
    @Mock WifiNative mWifiNative;
    @Mock WifiKeyStore mWifiKeyStore;
    @Mock Clock mClock;
    @Mock SIMAccessor mSimAccessor;
    @Mock PasspointObjectFactory mObjectFactory;
    @Mock PasspointEventHandler.Callbacks mCallbacks;
    @Mock AnqpCache mAnqpCache;
    @Mock ANQPRequestManager mAnqpRequestManager;
    @Mock CertificateVerifier mCertVerifier;
    @Mock WifiConfigManager mWifiConfigManager;
    @Mock WifiConfigStore mWifiConfigStore;
    @Mock PasspointConfigSharedStoreData.DataSource mSharedDataSource;
    @Mock PasspointConfigUserStoreData.DataSource mUserDataSource;
    @Mock WifiMetrics mWifiMetrics;
    @Mock OsuNetworkConnection mOsuNetworkConnection;
    @Mock OsuServerConnection mOsuServerConnection;
    @Mock PasspointProvisioner mPasspointProvisioner;
    @Mock IProvisioningCallback mCallback;
    @Mock WfaKeyStore mWfaKeyStore;
    @Mock KeyStore mKeyStore;
    @Mock AppOpsManager mAppOpsManager;
    @Mock WifiInjector mWifiInjector;
    @Mock ClientModeImpl mClientModeImpl;
    @Mock TelephonyManager mTelephonyManager;
    @Mock TelephonyManager mDataTelephonyManager;
    @Mock SubscriptionManager mSubscriptionManager;

    Handler mHandler;
    TestLooper mLooper;
    PasspointManager mManager;
    ArgumentCaptor<AppOpsManager.OnOpChangedListener> mAppOpChangedListenerCaptor =
            ArgumentCaptor.forClass(AppOpsManager.OnOpChangedListener.class);

    /** Sets up test. */
    @Before
    public void setUp() throws Exception {
        initMocks(this);
        when(mObjectFactory.makeAnqpCache(mClock)).thenReturn(mAnqpCache);
        when(mObjectFactory.makeANQPRequestManager(any(), eq(mClock)))
                .thenReturn(mAnqpRequestManager);
        when(mObjectFactory.makeCertificateVerifier()).thenReturn(mCertVerifier);
        when(mObjectFactory.makeOsuNetworkConnection(any(Context.class)))
                .thenReturn(mOsuNetworkConnection);
        when(mObjectFactory.makeOsuServerConnection())
                .thenReturn(mOsuServerConnection);
        when(mObjectFactory.makeWfaKeyStore()).thenReturn(mWfaKeyStore);
        when(mWfaKeyStore.get()).thenReturn(mKeyStore);
        when(mObjectFactory.makePasspointProvisioner(any(Context.class), any(WifiNative.class),
                any(PasspointManager.class), any(WifiMetrics.class)))
                .thenReturn(mPasspointProvisioner);
        when(mContext.getSystemService(Context.APP_OPS_SERVICE)).thenReturn(mAppOpsManager);
        when(mWifiInjector.getClientModeImpl()).thenReturn(mClientModeImpl);
        mLooper = new TestLooper();
        mHandler = new Handler(mLooper.getLooper());
        mManager = new PasspointManager(mContext, mWifiInjector, mHandler, mWifiNative,
                mWifiKeyStore, mClock, mSimAccessor, mObjectFactory, mWifiConfigManager,
                mWifiConfigStore, mWifiMetrics, mTelephonyManager, mSubscriptionManager);
        ArgumentCaptor<PasspointEventHandler.Callbacks> callbacks =
                ArgumentCaptor.forClass(PasspointEventHandler.Callbacks.class);
        verify(mObjectFactory).makePasspointEventHandler(any(WifiNative.class),
                                                         callbacks.capture());
        ArgumentCaptor<PasspointConfigSharedStoreData.DataSource> sharedDataSource =
                ArgumentCaptor.forClass(PasspointConfigSharedStoreData.DataSource.class);
        verify(mObjectFactory).makePasspointConfigSharedStoreData(sharedDataSource.capture());
        ArgumentCaptor<PasspointConfigUserStoreData.DataSource> userDataSource =
                ArgumentCaptor.forClass(PasspointConfigUserStoreData.DataSource.class);
        verify(mObjectFactory).makePasspointConfigUserStoreData(
                any(WifiKeyStore.class), any(SIMAccessor.class), userDataSource.capture());
        mCallbacks = callbacks.getValue();
        mSharedDataSource = sharedDataSource.getValue();
        mUserDataSource = userDataSource.getValue();
        // SIM is absent
        when(mSubscriptionManager.getActiveSubscriptionIdList()).thenReturn(new int[0]);
    }

    /**
     * Verify {@link WifiManager#ACTION_PASSPOINT_ICON} broadcast intent.
     * @param bssid BSSID of the AP
     * @param fileName Name of the icon file
     * @param data icon data byte array
     */
    private void verifyIconIntent(long bssid, String fileName, byte[] data) {
        ArgumentCaptor<Intent> intent = ArgumentCaptor.forClass(Intent.class);
        verify(mContext).sendBroadcastAsUser(intent.capture(), eq(UserHandle.ALL),
                eq(android.Manifest.permission.ACCESS_WIFI_STATE));
        assertEquals(ACTION_PASSPOINT_ICON, intent.getValue().getAction());
        assertTrue(intent.getValue().getExtras().containsKey(EXTRA_BSSID_LONG));
        assertEquals(bssid, intent.getValue().getExtras().getLong(EXTRA_BSSID_LONG));
        assertTrue(intent.getValue().getExtras().containsKey(EXTRA_FILENAME));
        assertEquals(fileName, intent.getValue().getExtras().getString(EXTRA_FILENAME));
        if (data != null) {
            assertTrue(intent.getValue().getExtras().containsKey(EXTRA_ICON));
            Icon icon = (Icon) intent.getValue().getExtras().getParcelable(EXTRA_ICON);
            assertTrue(Arrays.equals(data, icon.getDataBytes()));
        } else {
            assertFalse(intent.getValue().getExtras().containsKey(EXTRA_ICON));
        }
    }

    /**
     * Verify that the given Passpoint configuration matches the one that's added to
     * the PasspointManager.
     *
     * @param expectedConfig The expected installed Passpoint configuration
     */
    private void verifyInstalledConfig(PasspointConfiguration expectedConfig) {
        List<PasspointConfiguration> installedConfigs =
                mManager.getProviderConfigs(TEST_CREATOR_UID, true);
        assertEquals(1, installedConfigs.size());
        assertEquals(expectedConfig, installedConfigs.get(0));
    }

    /**
     * Create a mock PasspointProvider with default expectations.
     *
     * @param config The configuration associated with the provider
     * @return {@link com.android.server.wifi.hotspot2.PasspointProvider}
     */
    private PasspointProvider createMockProvider(PasspointConfiguration config) {
        PasspointProvider provider = mock(PasspointProvider.class);
        when(provider.installCertsAndKeys()).thenReturn(true);
        lenient().when(provider.getConfig()).thenReturn(config);
        lenient().when(provider.getCreatorUid()).thenReturn(TEST_CREATOR_UID);
        return provider;
    }

    /**
     * Helper function for creating a test configuration with user credential.
     *
     * @return {@link PasspointConfiguration}
     */
    private PasspointConfiguration createTestConfigWithUserCredential(String fqdn,
            String friendlyName) {
        PasspointConfiguration config = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setFqdn(fqdn);
        homeSp.setFriendlyName(friendlyName);
        config.setHomeSp(homeSp);
        Map<String, String> friendlyNames = new HashMap<>();
        friendlyNames.put("en", friendlyName);
        friendlyNames.put("kr", friendlyName + 1);
        friendlyNames.put("jp", friendlyName + 2);
        config.setServiceFriendlyNames(friendlyNames);
        Credential credential = new Credential();
        credential.setRealm(TEST_REALM);
        credential.setCaCertificate(FakeKeys.CA_CERT0);
        Credential.UserCredential userCredential = new Credential.UserCredential();
        userCredential.setUsername("username");
        userCredential.setPassword("password");
        userCredential.setEapType(EAPConstants.EAP_TTLS);
        userCredential.setNonEapInnerMethod(Credential.UserCredential.AUTH_METHOD_MSCHAP);
        credential.setUserCredential(userCredential);
        config.setCredential(credential);
        return config;
    }

    /**
     * Helper function for creating a test configuration with SIM credential.
     *
     * @return {@link PasspointConfiguration}
     */
    private PasspointConfiguration createTestConfigWithSimCredential(String fqdn, String imsi,
            String realm) {
        PasspointConfiguration config = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setFqdn(fqdn);
        homeSp.setFriendlyName(TEST_FRIENDLY_NAME);
        config.setHomeSp(homeSp);
        Credential credential = new Credential();
        credential.setRealm(TEST_REALM);
        Credential.SimCredential simCredential = new Credential.SimCredential();
        simCredential.setImsi(imsi);
        simCredential.setEapType(EAPConstants.EAP_SIM);
        credential.setSimCredential(simCredential);
        config.setCredential(credential);
        return config;
    }

    /**
     * Helper function for adding a test provider to the manager.  Return the mock
     * provider that's added to the manager.
     *
     * @return {@link PasspointProvider}
     */
    private PasspointProvider addTestProvider(String fqdn, String friendlyName,
            String packageName) {
        PasspointConfiguration config = createTestConfigWithUserCredential(fqdn, friendlyName);
        PasspointProvider provider = createMockProvider(config);
        when(mObjectFactory.makePasspointProvider(eq(config), eq(mWifiKeyStore),
                eq(mSimAccessor), anyLong(), eq(TEST_CREATOR_UID), eq(TEST_PACKAGE))).thenReturn(
                provider);
        when(provider.getPackageName()).thenReturn(packageName);
        assertTrue(mManager.addOrUpdateProvider(config, TEST_CREATOR_UID, TEST_PACKAGE));
        return provider;
    }

    /**
     * Helper function for adding a test provider with SIM credentials to the manager.  Return the
     * mock provider that's added to the manager.
     *
     * @return {@link PasspointProvider}
     */
    private PasspointProvider addTestCarrierProvider(String fqdn, String imsi, String realm) {
        PasspointConfiguration config = createTestConfigWithSimCredential(fqdn, imsi, realm);
        PasspointProvider provider = createMockProvider(config);
        when(mObjectFactory.makePasspointProvider(eq(config), eq(mWifiKeyStore),
                eq(mSimAccessor), anyLong(), eq(TEST_CREATOR_UID), eq(TEST_PACKAGE))).thenReturn(
                provider);

        assertTrue(mManager.addOrUpdateProvider(config, TEST_CREATOR_UID, TEST_PACKAGE));

        return provider;
    }

    /**
     * Helper function for creating a ScanResult for testing.
     *
     * @return {@link ScanResult}
     */
    private ScanResult createTestScanResult() {
        ScanResult scanResult = new ScanResult();
        scanResult.SSID = TEST_SSID;
        scanResult.BSSID = TEST_BSSID_STRING;
        scanResult.hessid = TEST_HESSID;
        scanResult.anqpDomainId = TEST_ANQP_DOMAIN_ID;
        scanResult.flags = ScanResult.FLAG_PASSPOINT_NETWORK;
        return scanResult;
    }

    /**
     * Helper function for creating a ScanResult for testing.
     *
     * @return {@link ScanResult}
     */
    private List<ScanResult> createTestScanResults() {
        List<ScanResult> scanResults = new ArrayList<>();

        // Passpoint AP
        ScanResult scanResult = new ScanResult();
        scanResult.SSID = TEST_SSID;
        scanResult.BSSID = TEST_BSSID_STRING;
        scanResult.hessid = TEST_HESSID;
        scanResult.flags = ScanResult.FLAG_PASSPOINT_NETWORK;
        scanResult.anqpDomainId = TEST_ANQP_DOMAIN_ID2;
        scanResults.add(scanResult);

        // Non-Passpoint AP
        ScanResult scanResult2 = new ScanResult();
        scanResult2.SSID = TEST_SSID2;
        scanResult2.BSSID = TEST_BSSID_STRING2;
        scanResult2.hessid = TEST_HESSID;
        scanResult2.flags = 0;
        scanResults.add(scanResult2);

        // Passpoint AP
        ScanResult scanResult3 = new ScanResult();
        scanResult3.SSID = TEST_SSID3;
        scanResult3.BSSID = TEST_BSSID_STRING3;
        scanResult3.hessid = TEST_HESSID;
        scanResult3.flags = ScanResult.FLAG_PASSPOINT_NETWORK;
        scanResult3.anqpDomainId = TEST_ANQP_DOMAIN_ID2;
        scanResults.add(scanResult3);

        return scanResults;
    }

    /**
     * Helper function for generating {@link ScanDetail} for testing.
     */
    private ScanDetail generateScanDetail(String ssid, String bssid, long hessid, int anqpDomaiId,
            boolean isPasspoint) {
        NetworkDetail networkDetail = mock(NetworkDetail.class);

        ScanDetail scanDetail = mock(ScanDetail.class);
        ScanResult scanResult = new ScanResult();
        scanResult.SSID = ssid;
        scanResult.BSSID = bssid;
        scanResult.hessid = hessid;
        scanResult.anqpDomainId = anqpDomaiId;
        if (isPasspoint) {
            lenient().when(networkDetail.isInterworking()).thenReturn(true);
            scanResult.flags = ScanResult.FLAG_PASSPOINT_NETWORK;
        } else {
            lenient().when(networkDetail.isInterworking()).thenReturn(false);
        }

        lenient().when(scanDetail.getScanResult()).thenReturn(scanResult);
        lenient().when(scanDetail.getNetworkDetail()).thenReturn(networkDetail);
        return scanDetail;
    }

    /**
     * Verify that the ANQP elements will be added to the ANQP cache on receiving a successful
     * response.
     *
     * @throws Exception
     */
    @Test
    public void anqpResponseSuccess() throws Exception {
        Map<ANQPElementType, ANQPElement> anqpElementMap = new HashMap<>();
        anqpElementMap.put(ANQPElementType.ANQPDomName,
                new DomainNameElement(Arrays.asList(new String[] {"test.com"})));

        when(mAnqpRequestManager.onRequestCompleted(TEST_BSSID, true)).thenReturn(TEST_ANQP_KEY);
        mCallbacks.onANQPResponse(TEST_BSSID, anqpElementMap);
        verify(mAnqpCache).addEntry(TEST_ANQP_KEY, anqpElementMap);
        verify(mContext, never()).sendBroadcastAsUser(any(Intent.class), any(UserHandle.class),
                any(String.class));
    }

    /**
     * Verify that no ANQP elements will be added to the ANQP cache on receiving a successful
     * response for a request that's not sent by us.
     *
     * @throws Exception
     */
    @Test
    public void anqpResponseSuccessWithUnknownRequest() throws Exception {
        Map<ANQPElementType, ANQPElement> anqpElementMap = new HashMap<>();
        anqpElementMap.put(ANQPElementType.ANQPDomName,
                new DomainNameElement(Arrays.asList(new String[] {"test.com"})));

        when(mAnqpRequestManager.onRequestCompleted(TEST_BSSID, true)).thenReturn(null);
        mCallbacks.onANQPResponse(TEST_BSSID, anqpElementMap);
        verify(mAnqpCache, never()).addEntry(any(ANQPNetworkKey.class), anyMap());
    }

    /**
     * Verify that no ANQP elements will be added to the ANQP cache on receiving a failure response.
     *
     * @throws Exception
     */
    @Test
    public void anqpResponseFailure() throws Exception {
        when(mAnqpRequestManager.onRequestCompleted(TEST_BSSID, false)).thenReturn(TEST_ANQP_KEY);
        mCallbacks.onANQPResponse(TEST_BSSID, null);
        verify(mAnqpCache, never()).addEntry(any(ANQPNetworkKey.class), anyMap());

    }

    /**
     * Validate the broadcast intent when icon file retrieval succeeded.
     *
     * @throws Exception
     */
    @Test
    public void iconResponseSuccess() throws Exception {
        byte[] iconData = new byte[] {0x00, 0x11};
        mCallbacks.onIconResponse(BSSID, ICON_FILENAME, iconData);
        verifyIconIntent(BSSID, ICON_FILENAME, iconData);
    }

    /**
     * Validate the broadcast intent when icon file retrieval failed.
     *
     * @throws Exception
     */
    @Test
    public void iconResponseFailure() throws Exception {
        mCallbacks.onIconResponse(BSSID, ICON_FILENAME, null);
        verifyIconIntent(BSSID, ICON_FILENAME, null);
    }

    /**
     * Validate the broadcast intent {@link WifiManager#ACTION_PASSPOINT_DEAUTH_IMMINENT} when
     * Deauth Imminent WNM frame is received.
     *
     * @throws Exception
     */
    @Test
    public void onDeauthImminentReceived() throws Exception {
        String reasonUrl = "test.com";
        int delay = 123;
        boolean ess = true;

        mCallbacks.onWnmFrameReceived(new WnmData(BSSID, reasonUrl, ess, delay));
        // Verify the broadcast intent.
        ArgumentCaptor<Intent> intent = ArgumentCaptor.forClass(Intent.class);
        verify(mContext).sendBroadcastAsUser(intent.capture(), eq(UserHandle.ALL),
                eq(android.Manifest.permission.ACCESS_WIFI_STATE));
        assertEquals(ACTION_PASSPOINT_DEAUTH_IMMINENT, intent.getValue().getAction());
        assertTrue(intent.getValue().getExtras().containsKey(EXTRA_BSSID_LONG));
        assertEquals(BSSID, intent.getValue().getExtras().getLong(EXTRA_BSSID_LONG));
        assertTrue(intent.getValue().getExtras().containsKey(EXTRA_ESS));
        assertEquals(ess, intent.getValue().getExtras().getBoolean(EXTRA_ESS));
        assertTrue(intent.getValue().getExtras().containsKey(EXTRA_DELAY));
        assertEquals(delay, intent.getValue().getExtras().getInt(EXTRA_DELAY));
        assertTrue(intent.getValue().getExtras().containsKey(EXTRA_URL));
        assertEquals(reasonUrl, intent.getValue().getExtras().getString(EXTRA_URL));
    }

    /**
     * Validate the broadcast intent {@link WifiManager#ACTION_PASSPOINT_SUBSCRIPTION_REMEDIATION}
     * when Subscription Remediation WNM frame is received.
     *
     * @throws Exception
     */
    @Test
    public void onSubscriptionRemediationReceived() throws Exception {
        int serverMethod = 1;
        String serverUrl = "testUrl";

        mCallbacks.onWnmFrameReceived(new WnmData(BSSID, serverUrl, serverMethod));
        // Verify the broadcast intent.
        ArgumentCaptor<Intent> intent = ArgumentCaptor.forClass(Intent.class);
        verify(mContext).sendBroadcastAsUser(intent.capture(), eq(UserHandle.ALL),
                eq(android.Manifest.permission.ACCESS_WIFI_STATE));
        assertEquals(ACTION_PASSPOINT_SUBSCRIPTION_REMEDIATION, intent.getValue().getAction());
        assertTrue(intent.getValue().getExtras().containsKey(EXTRA_BSSID_LONG));
        assertEquals(BSSID, intent.getValue().getExtras().getLong(EXTRA_BSSID_LONG));
        assertTrue(intent.getValue().getExtras().containsKey(
                EXTRA_SUBSCRIPTION_REMEDIATION_METHOD));
        assertEquals(serverMethod, intent.getValue().getExtras().getInt(
                EXTRA_SUBSCRIPTION_REMEDIATION_METHOD));
        assertTrue(intent.getValue().getExtras().containsKey(EXTRA_URL));
        assertEquals(serverUrl, intent.getValue().getExtras().getString(EXTRA_URL));
    }

    /**
     * Verify that adding a provider with a null configuration will fail.
     *
     * @throws Exception
     */
    @Test
    public void addProviderWithNullConfig() throws Exception {
        assertFalse(mManager.addOrUpdateProvider(null, TEST_CREATOR_UID, TEST_PACKAGE));
        verify(mWifiMetrics).incrementNumPasspointProviderInstallation();
        verify(mWifiMetrics, never()).incrementNumPasspointProviderInstallSuccess();
    }

    /**
     * Verify that adding a provider with a empty configuration will fail.
     *
     * @throws Exception
     */
    @Test
    public void addProviderWithEmptyConfig() throws Exception {
        assertFalse(mManager.addOrUpdateProvider(new PasspointConfiguration(), TEST_CREATOR_UID,
                TEST_PACKAGE));
        verify(mWifiMetrics).incrementNumPasspointProviderInstallation();
        verify(mWifiMetrics, never()).incrementNumPasspointProviderInstallSuccess();
    }

    /**
     * Verify taht adding a provider with an invalid credential will fail (using EAP-TLS
     * for user credential).
     *
     * @throws Exception
     */
    @Test
    public void addProviderWithInvalidCredential() throws Exception {
        PasspointConfiguration config = createTestConfigWithUserCredential(TEST_FQDN,
                TEST_FRIENDLY_NAME);
        // EAP-TLS not allowed for user credential.
        config.getCredential().getUserCredential().setEapType(EAPConstants.EAP_TLS);
        assertFalse(mManager.addOrUpdateProvider(config, TEST_CREATOR_UID, TEST_PACKAGE));
        verify(mWifiMetrics).incrementNumPasspointProviderInstallation();
        verify(mWifiMetrics, never()).incrementNumPasspointProviderInstallSuccess();
    }

    /**
     * Verify that adding a provider with a valid configuration and user credential will succeed.
     *
     * @throws Exception
     */
    @Test
    public void addRemoveProviderWithValidUserCredential() throws Exception {
        PasspointConfiguration config = createTestConfigWithUserCredential(TEST_FQDN,
                TEST_FRIENDLY_NAME);
        PasspointProvider provider = createMockProvider(config);
        when(provider.getPackageName()).thenReturn(TEST_PACKAGE);
        when(mObjectFactory.makePasspointProvider(eq(config), eq(mWifiKeyStore),
                eq(mSimAccessor), anyLong(), eq(TEST_CREATOR_UID), eq(TEST_PACKAGE))).thenReturn(
                provider);
        assertTrue(mManager.addOrUpdateProvider(config, TEST_CREATOR_UID, TEST_PACKAGE));
        verifyInstalledConfig(config);
        verify(mWifiConfigManager).saveToStore(true);
        verify(mWifiMetrics).incrementNumPasspointProviderInstallation();
        verify(mWifiMetrics).incrementNumPasspointProviderInstallSuccess();
        verify(mAppOpsManager).startWatchingMode(eq(OPSTR_CHANGE_WIFI_STATE), eq(TEST_PACKAGE), any(
                AppOpsManager.OnOpChangedListener.class));
        reset(mWifiMetrics);
        reset(mWifiConfigManager);

        // Verify content in the data source.
        List<PasspointProvider> providers = mUserDataSource.getProviders();
        assertEquals(1, providers.size());
        assertEquals(config, providers.get(0).getConfig());
        // Provider index start with 0, should be 1 after adding a provider.
        assertEquals(1, mSharedDataSource.getProviderIndex());

        // Remove the provider as the creator app.
        assertTrue(mManager.removeProvider(TEST_CREATOR_UID, false, TEST_FQDN));
        verify(provider).uninstallCertsAndKeys();
        verify(mWifiConfigManager).saveToStore(true);
        verify(mWifiMetrics).incrementNumPasspointProviderUninstallation();
        verify(mWifiMetrics).incrementNumPasspointProviderUninstallSuccess();
        verify(mAppOpsManager).stopWatchingMode(any(AppOpsManager.OnOpChangedListener.class));
        assertTrue(mManager.getProviderConfigs(TEST_CREATOR_UID, false).isEmpty());

        // Verify content in the data source.
        assertTrue(mUserDataSource.getProviders().isEmpty());
        // Removing a provider should not change the provider index.
        assertEquals(1, mSharedDataSource.getProviderIndex());
    }

    /**
     * Verify that adding a provider with a valid configuration and SIM credential will succeed.
     *
     * @throws Exception
     */
    @Test
    public void addRemoveProviderWithValidSimCredential() throws Exception {
        PasspointConfiguration config = createTestConfigWithSimCredential(TEST_FQDN, TEST_IMSI,
                TEST_REALM);
        PasspointProvider provider = createMockProvider(config);
        when(mObjectFactory.makePasspointProvider(eq(config), eq(mWifiKeyStore),
                eq(mSimAccessor), anyLong(), eq(TEST_CREATOR_UID), eq(TEST_PACKAGE))).thenReturn(
                provider);
        assertTrue(mManager.addOrUpdateProvider(config, TEST_CREATOR_UID, TEST_PACKAGE));
        verifyInstalledConfig(config);
        verify(mWifiConfigManager).saveToStore(true);
        verify(mWifiMetrics).incrementNumPasspointProviderInstallation();
        verify(mWifiMetrics).incrementNumPasspointProviderInstallSuccess();
        reset(mWifiMetrics);
        reset(mWifiConfigManager);

        // Verify content in the data source.
        List<PasspointProvider> providers = mUserDataSource.getProviders();
        assertEquals(1, providers.size());
        assertEquals(config, providers.get(0).getConfig());
        // Provider index start with 0, should be 1 after adding a provider.
        assertEquals(1, mSharedDataSource.getProviderIndex());

        // Remove the provider as a privileged non-creator app.
        assertTrue(mManager.removeProvider(TEST_UID, true, TEST_FQDN));
        verify(provider).uninstallCertsAndKeys();
        verify(mWifiConfigManager).saveToStore(true);
        verify(mWifiMetrics).incrementNumPasspointProviderUninstallation();
        verify(mWifiMetrics).incrementNumPasspointProviderUninstallSuccess();
        assertTrue(mManager.getProviderConfigs(TEST_UID, true).isEmpty());

        // Verify content in the data source.
        assertTrue(mUserDataSource.getProviders().isEmpty());
        // Removing a provider should not change the provider index.
        assertEquals(1, mSharedDataSource.getProviderIndex());
    }

    /**
     * Verify that adding a provider with the same base domain as the existing provider will
     * succeed, and verify that the existing provider is replaced by the new provider with
     * the new configuration.
     *
     * @throws Exception
     */
    @Test
    public void addProviderWithExistingConfig() throws Exception {
        // Add a provider with the original configuration.
        PasspointConfiguration origConfig = createTestConfigWithSimCredential(TEST_FQDN, TEST_IMSI,
                TEST_REALM);
        PasspointProvider origProvider = createMockProvider(origConfig);
        when(mObjectFactory.makePasspointProvider(eq(origConfig), eq(mWifiKeyStore),
                eq(mSimAccessor), anyLong(), eq(TEST_CREATOR_UID), eq(TEST_PACKAGE))).thenReturn(
                origProvider);
        assertTrue(mManager.addOrUpdateProvider(origConfig, TEST_CREATOR_UID, TEST_PACKAGE));
        verifyInstalledConfig(origConfig);
        verify(mWifiConfigManager).saveToStore(true);
        verify(mWifiMetrics).incrementNumPasspointProviderInstallation();
        verify(mWifiMetrics).incrementNumPasspointProviderInstallSuccess();
        reset(mWifiMetrics);
        reset(mWifiConfigManager);

        // Verify data source content.
        List<PasspointProvider> origProviders = mUserDataSource.getProviders();
        assertEquals(1, origProviders.size());
        assertEquals(origConfig, origProviders.get(0).getConfig());
        assertEquals(1, mSharedDataSource.getProviderIndex());

        // Add another provider with the same base domain as the existing provider.
        // This should replace the existing provider with the new configuration.
        PasspointConfiguration newConfig = createTestConfigWithUserCredential(TEST_FQDN,
                TEST_FRIENDLY_NAME);
        PasspointProvider newProvider = createMockProvider(newConfig);
        when(mObjectFactory.makePasspointProvider(eq(newConfig), eq(mWifiKeyStore),
                eq(mSimAccessor), anyLong(), eq(TEST_CREATOR_UID), eq(TEST_PACKAGE))).thenReturn(
                newProvider);
        assertTrue(mManager.addOrUpdateProvider(newConfig, TEST_CREATOR_UID, TEST_PACKAGE));
        verifyInstalledConfig(newConfig);
        verify(mWifiConfigManager).saveToStore(true);
        verify(mWifiMetrics).incrementNumPasspointProviderInstallation();
        verify(mWifiMetrics).incrementNumPasspointProviderInstallSuccess();

        // Verify data source content.
        List<PasspointProvider> newProviders = mUserDataSource.getProviders();
        assertEquals(1, newProviders.size());
        assertEquals(newConfig, newProviders.get(0).getConfig());
        assertEquals(2, mSharedDataSource.getProviderIndex());
    }

    /**
     * Verify that adding a provider will fail when failing to install certificates and
     * key to the keystore.
     *
     * @throws Exception
     */
    @Test
    public void addProviderOnKeyInstallationFailiure() throws Exception {
        PasspointConfiguration config = createTestConfigWithUserCredential(TEST_FQDN,
                TEST_FRIENDLY_NAME);
        PasspointProvider provider = mock(PasspointProvider.class);
        when(provider.installCertsAndKeys()).thenReturn(false);
        when(mObjectFactory.makePasspointProvider(eq(config), eq(mWifiKeyStore),
                eq(mSimAccessor), anyLong(), eq(TEST_CREATOR_UID), eq(TEST_PACKAGE))).thenReturn(
                provider);
        assertFalse(mManager.addOrUpdateProvider(config, TEST_CREATOR_UID, TEST_PACKAGE));
        verify(mWifiMetrics).incrementNumPasspointProviderInstallation();
        verify(mWifiMetrics, never()).incrementNumPasspointProviderInstallSuccess();
    }

    /**
     * Verify that adding a provider with an invalid CA certificate will fail.
     *
     * @throws Exception
     */
    @Test
    public void addProviderWithInvalidCaCert() throws Exception {
        PasspointConfiguration config = createTestConfigWithUserCredential(TEST_FQDN,
                TEST_FRIENDLY_NAME);
        doThrow(new GeneralSecurityException())
                .when(mCertVerifier).verifyCaCert(any(X509Certificate.class));
        assertFalse(mManager.addOrUpdateProvider(config, TEST_CREATOR_UID, TEST_PACKAGE));
        verify(mWifiMetrics).incrementNumPasspointProviderInstallation();
        verify(mWifiMetrics, never()).incrementNumPasspointProviderInstallSuccess();
    }

    /**
     * Verify that adding a provider with R2 configuration will not perform CA certificate
     * verification.
     *
     * @throws Exception
     */
    @Test
    public void addProviderWithR2Config() throws Exception {
        PasspointConfiguration config = createTestConfigWithUserCredential(TEST_FQDN,
                TEST_FRIENDLY_NAME);
        config.setUpdateIdentifier(1);
        PasspointProvider provider = createMockProvider(config);
        when(mObjectFactory.makePasspointProvider(eq(config), eq(mWifiKeyStore),
                eq(mSimAccessor), anyLong(), eq(TEST_CREATOR_UID), eq(TEST_PACKAGE))).thenReturn(
                provider);
        assertTrue(mManager.addOrUpdateProvider(config, TEST_CREATOR_UID, TEST_PACKAGE));
        verify(mCertVerifier, never()).verifyCaCert(any(X509Certificate.class));
        verifyInstalledConfig(config);
        verify(mWifiMetrics).incrementNumPasspointProviderInstallation();
        verify(mWifiMetrics).incrementNumPasspointProviderInstallSuccess();
    }

    /**
     * Verify that removing a non-existing provider will fail.
     *
     * @throws Exception
     */
    @Test
    public void removeNonExistingProvider() throws Exception {
        assertFalse(mManager.removeProvider(TEST_CREATOR_UID, true, TEST_FQDN));
        verify(mWifiMetrics).incrementNumPasspointProviderUninstallation();
        verify(mWifiMetrics, never()).incrementNumPasspointProviderUninstallSuccess();
    }

    /**
     * Verify that a {code null} will be returned when no providers are installed.
     *
     * @throws Exception
     */
    @Test
    public void matchProviderWithNoProvidersInstalled() throws Exception {
        assertNull(mManager.matchProvider(createTestScanResult()));
    }

    /**
     * Verify that a {code null} be returned when ANQP entry doesn't exist in the cache.
     *
     * @throws Exception
     */
    @Test
    public void matchProviderWithAnqpCacheMissed() throws Exception {
        addTestProvider(TEST_FQDN, TEST_FRIENDLY_NAME, TEST_PACKAGE);

        when(mAnqpCache.getEntry(TEST_ANQP_KEY)).thenReturn(null);
        assertNull(mManager.matchProvider(createTestScanResult()));
        // Verify that a request for ANQP elements is initiated.
        verify(mAnqpRequestManager).requestANQPElements(eq(TEST_BSSID), any(ANQPNetworkKey.class),
                anyBoolean(), anyBoolean());
    }

    /**
     * Verify that the expected provider will be returned when a HomeProvider is matched.
     *
     * @throws Exception
     */
    @Test
    public void matchProviderAsHomeProvider() throws Exception {
        PasspointProvider provider = addTestProvider(TEST_FQDN, TEST_FRIENDLY_NAME, TEST_PACKAGE);
        ANQPData entry = new ANQPData(mClock, null);

        when(mAnqpCache.getEntry(TEST_ANQP_KEY)).thenReturn(entry);
        when(provider.match(anyMap(), any(RoamingConsortium.class)))
            .thenReturn(PasspointMatch.HomeProvider);
        Pair<PasspointProvider, PasspointMatch> result =
                mManager.matchProvider(createTestScanResult());
        assertEquals(PasspointMatch.HomeProvider, result.second);
        assertEquals(TEST_FQDN, result.first.getConfig().getHomeSp().getFqdn());
    }

    /**
     * Verify that the expected provider will be returned when a RoamingProvider is matched.
     *
     * @throws Exception
     */
    @Test
    public void matchProviderAsRoamingProvider() throws Exception {
        PasspointProvider provider = addTestProvider(TEST_FQDN, TEST_FRIENDLY_NAME, TEST_PACKAGE);
        ANQPData entry = new ANQPData(mClock, null);

        when(mAnqpCache.getEntry(TEST_ANQP_KEY)).thenReturn(entry);
        when(provider.match(anyMap(), any(RoamingConsortium.class)))
            .thenReturn(PasspointMatch.RoamingProvider);
        Pair<PasspointProvider, PasspointMatch> result =
                mManager.matchProvider(createTestScanResult());
        assertEquals(PasspointMatch.RoamingProvider, result.second);
        assertEquals(TEST_FQDN, result.first.getConfig().getHomeSp().getFqdn());
    }

    /**
     * Verify that a {code null} will be returned when there is no matching provider.
     *
     * @throws Exception
     */
    @Test
    public void matchProviderWithNoMatch() throws Exception {
        PasspointProvider provider = addTestProvider(TEST_FQDN, TEST_FRIENDLY_NAME, TEST_PACKAGE);
        ANQPData entry = new ANQPData(mClock, null);

        when(mAnqpCache.getEntry(TEST_ANQP_KEY)).thenReturn(entry);
        when(provider.match(anyMap(), any(RoamingConsortium.class)))
            .thenReturn(PasspointMatch.None);
        assertNull(mManager.matchProvider(createTestScanResult()));
    }

    /**
     * Verify the expectations for sweepCache.
     *
     * @throws Exception
     */
    @Test
    public void sweepCache() throws Exception {
        mManager.sweepCache();
        verify(mAnqpCache).sweep();
    }

    /**
     * Verify that an empty map will be returned if ANQP elements are not cached for the given AP.
     *
     * @throws Exception
     */
    @Test
    public void getANQPElementsWithNoMatchFound() throws Exception {
        when(mAnqpCache.getEntry(TEST_ANQP_KEY)).thenReturn(null);
        assertTrue(mManager.getANQPElements(createTestScanResult()).isEmpty());
    }

    /**
     * Verify that an expected ANQP elements will be returned if ANQP elements are cached for the
     * given AP.
     *
     * @throws Exception
     */
    @Test
    public void getANQPElementsWithMatchFound() throws Exception {
        Map<ANQPElementType, ANQPElement> anqpElementMap = new HashMap<>();
        anqpElementMap.put(ANQPElementType.ANQPDomName,
                new DomainNameElement(Arrays.asList(new String[] {"test.com"})));
        ANQPData entry = new ANQPData(mClock, anqpElementMap);

        when(mAnqpCache.getEntry(TEST_ANQP_KEY)).thenReturn(entry);
        assertEquals(anqpElementMap, mManager.getANQPElements(createTestScanResult()));
    }

    /**
     * Verify that an expected map of FQDN and a list of ScanResult will be returned when provided
     * scanResults are matched to installed Passpoint profiles.
     */
    @Test
    public void getAllMatchingFqdnsForScanResults() {
        // static mocking
        MockitoSession session =
                com.android.dx.mockito.inline.extended.ExtendedMockito.mockitoSession().mockStatic(
                        InformationElementUtil.class).startMocking();
        try {
            PasspointProvider providerHome = addTestProvider(TEST_FQDN + 0, TEST_FRIENDLY_NAME,
                    TEST_PACKAGE);
            WifiConfiguration homeWifiConfiguration = new WifiConfiguration();
            homeWifiConfiguration.FQDN = TEST_FQDN + 0;
            homeWifiConfiguration.isHomeProviderNetwork = true;
            PasspointProvider providerRoaming = addTestProvider(TEST_FQDN + 1, TEST_FRIENDLY_NAME,
                    TEST_PACKAGE);
            WifiConfiguration roamingWifiConfiguration = new WifiConfiguration();
            roamingWifiConfiguration.FQDN = TEST_FQDN + 1;
            PasspointProvider providerNone = addTestProvider(TEST_FQDN + 2, TEST_FRIENDLY_NAME,
                    TEST_PACKAGE);
            ANQPData entry = new ANQPData(mClock, null);
            InformationElementUtil.Vsa vsa = new InformationElementUtil.Vsa();
            vsa.anqpDomainID = TEST_ANQP_DOMAIN_ID2;

            when(mAnqpCache.getEntry(TEST_ANQP_KEY2)).thenReturn(entry);
            when(InformationElementUtil.getHS2VendorSpecificIE(isNull())).thenReturn(vsa);
            when(providerHome.match(anyMap(), isNull()))
                    .thenReturn(PasspointMatch.HomeProvider);
            when(providerRoaming.match(anyMap(), isNull()))
                    .thenReturn(PasspointMatch.RoamingProvider);
            when(providerNone.match(anyMap(), isNull()))
                    .thenReturn(PasspointMatch.None);

            lenient().when(providerHome.getWifiConfig()).thenReturn(homeWifiConfiguration);
            lenient().when(providerRoaming.getWifiConfig()).thenReturn(roamingWifiConfiguration);
            lenient().when(providerNone.getWifiConfig()).thenReturn(new WifiConfiguration());

            Map<String, Map<Integer, List<ScanResult>>> configs =
                    mManager.getAllMatchingFqdnsForScanResults(
                            createTestScanResults());

            // Expects to be matched with home Provider for each AP (two APs).
            assertEquals(2, configs.get(TEST_FQDN + 0).get(
                    WifiManager.PASSPOINT_HOME_NETWORK).size());
            assertFalse(
                    configs.get(TEST_FQDN + 0).containsKey(WifiManager.PASSPOINT_ROAMING_NETWORK));

            // Expects to be matched with roaming Provider for each AP (two APs).
            assertEquals(2, configs.get(TEST_FQDN + 1).get(
                    WifiManager.PASSPOINT_ROAMING_NETWORK).size());
            assertFalse(configs.get(TEST_FQDN + 1).containsKey(WifiManager.PASSPOINT_HOME_NETWORK));

        } finally {
            session.finishMocking();
        }
    }

    /**
     * Verify that an expected list of {@link WifiConfiguration} will be returned when provided
     * a list of FQDN is matched to installed Passpoint profiles.
     */
    @Test
    public void getWifiConfigsForPasspointProfiles() {
        PasspointProvider provider1 = addTestProvider(TEST_FQDN, TEST_FRIENDLY_NAME, TEST_PACKAGE);
        WifiConfiguration wifiConfiguration1 = new WifiConfiguration();
        wifiConfiguration1.FQDN = TEST_FQDN;
        PasspointProvider provider2 = addTestProvider(TEST_FQDN + 1, TEST_FRIENDLY_NAME,
                TEST_PACKAGE);
        WifiConfiguration wifiConfiguration2 = new WifiConfiguration();
        wifiConfiguration2.FQDN = TEST_FQDN + 1;
        PasspointProvider provider3 = addTestProvider(TEST_FQDN + 2, TEST_FRIENDLY_NAME,
                TEST_PACKAGE);
        WifiConfiguration wifiConfiguration3 = new WifiConfiguration();
        wifiConfiguration3.FQDN = TEST_FQDN + 2;
        lenient().when(provider1.getWifiConfig()).thenReturn(wifiConfiguration1);
        lenient().when(provider2.getWifiConfig()).thenReturn(wifiConfiguration2);
        lenient().when(provider3.getWifiConfig()).thenReturn(wifiConfiguration3);

        assertEquals(3, mManager.getWifiConfigsForPasspointProfiles(
                Arrays.asList(TEST_FQDN, TEST_FQDN + 1, TEST_FQDN + 2)).size());
    }

    /**
     * Verify that an empty map will be returned when trying to get all matching FQDN for a {@code
     * null} {@link ScanResult}.
     */
    @Test
    public void getAllMatchingFqdnsForScanResultsWithNullScanResult() throws Exception {
        assertEquals(0, mManager.getAllMatchingFqdnsForScanResults(null).size());
    }

    /**
     * Verify that an empty map will be returned when trying to get a all matching FQDN for a {@link
     * ScanResult} with a {@code null} BSSID.
     */
    @Test
    public void getAllMatchingFqdnsForScanResultsWithNullBSSID() throws Exception {
        ScanResult scanResult = createTestScanResult();
        scanResult.BSSID = null;

        assertEquals(0,
                mManager.getAllMatchingFqdnsForScanResults(Arrays.asList(scanResult)).size());
    }

    /**
     * Verify that an empty map will be returned when trying to get all matching FQDN for a {@link
     * ScanResult} with an invalid BSSID.
     */
    @Test
    public void ggetAllMatchingFqdnsForScanResultsWithInvalidBSSID() throws Exception {
        ScanResult scanResult = createTestScanResult();
        scanResult.BSSID = "asdfdasfas";

        assertEquals(0,
                mManager.getAllMatchingFqdnsForScanResults(Arrays.asList(scanResult)).size());
    }

    /**
     * Verify that an empty map will be returned when trying to get all matching FQDN for a
     * non-Passpoint AP.
     */
    @Test
    public void getAllMatchingFqdnsForScanResultsForNonPasspointAP() throws Exception {
        ScanResult scanResult = createTestScanResult();
        scanResult.flags = 0;
        assertEquals(0,
                mManager.getAllMatchingFqdnsForScanResults(Arrays.asList(scanResult)).size());
    }

    /**
     * Verify that an empty list will be returned when retrieving OSU providers for an AP with
     * null scan result.
     *
     * @throws Exception
     */
    @Test
    public void getMatchingOsuProvidersForNullScanResult() throws Exception {
        assertTrue(mManager.getMatchingOsuProviders(null).isEmpty());
    }

    /**
     * Verify that an empty list will be returned when retrieving OSU providers for an AP with
     * invalid BSSID.
     *
     * @throws Exception
     */
    @Test
    public void getMatchingOsuProvidersForInvalidBSSID() throws Exception {
        ScanResult scanResult = createTestScanResult();
        scanResult.BSSID = "asdfdasfas";
        assertTrue(mManager.getMatchingOsuProviders(Arrays.asList(scanResult)).isEmpty());
    }

    /**
     * Verify that an empty list will be returned when retrieving OSU providers for a
     * non-Passpoint AP.
     *
     * @throws Exception
     */
    @Test
    public void getMatchingOsuProvidersForNonPasspointAP() throws Exception {
        ScanResult scanResult = createTestScanResult();
        scanResult.flags = 0;
        assertTrue(mManager.getMatchingOsuProviders(Arrays.asList(scanResult)).isEmpty());
    }

    /**
     * Verify that an empty list will be returned when no match is found from the ANQP cache.
     *
     * @throws Exception
     */
    @Test
    public void getMatchingOsuProviderWithNoMatch() throws Exception {
        when(mAnqpCache.getEntry(TEST_ANQP_KEY)).thenReturn(null);
        assertTrue(
                mManager.getMatchingOsuProviders(Arrays.asList(createTestScanResult())).isEmpty());
    }

    /**
     * Verify that an expected provider list will be returned when a match is found from
     * the ANQP cache with a given list of scanResult.
     *
     * @throws Exception
     */
    @Test
    public void getMatchingOsuProvidersWithMatch() throws Exception {
        // Setup OSU providers ANQP element for AP1.
        List<OsuProviderInfo> providerInfoListOfAp1 = new ArrayList<>();
        Map<ANQPElementType, ANQPElement> anqpElementMapOfAp1 = new HashMap<>();
        Set<OsuProvider> expectedOsuProvidersForDomainId = new HashSet<>();

        // Setup OSU providers ANQP element for AP2.
        List<OsuProviderInfo> providerInfoListOfAp2 = new ArrayList<>();
        Map<ANQPElementType, ANQPElement> anqpElementMapOfAp2 = new HashMap<>();
        Set<OsuProvider> expectedOsuProvidersForDomainId2 = new HashSet<>();
        int osuProviderCount = 4;

        // static mocking
        MockitoSession session =
                com.android.dx.mockito.inline.extended.ExtendedMockito.mockitoSession().mockStatic(
                        InformationElementUtil.class).startMocking();
        try {
            for (int i = 0; i < osuProviderCount; i++) {
                // Test data.
                String friendlyName = "Test Provider" + i;
                String serviceDescription = "Dummy Service" + i;
                Uri serverUri = Uri.parse("https://" + "test" + i + ".com");
                String nai = "access.test.com";
                List<Integer> methodList = Arrays.asList(1);
                List<I18Name> friendlyNames = Arrays.asList(
                        new I18Name(Locale.ENGLISH.getLanguage(), Locale.ENGLISH, friendlyName));
                List<I18Name> serviceDescriptions = Arrays.asList(
                        new I18Name(Locale.ENGLISH.getLanguage(), Locale.ENGLISH,
                                serviceDescription));
                Map<String, String> friendlyNameMap = new HashMap<>();
                friendlyNames.forEach(e -> friendlyNameMap.put(e.getLanguage(), e.getText()));

                expectedOsuProvidersForDomainId.add(new OsuProvider(
                        null, friendlyNameMap, serviceDescription,
                        serverUri, nai, methodList, null));

                // add All OSU Providers for AP1.
                providerInfoListOfAp1.add(new OsuProviderInfo(
                        friendlyNames, serverUri, methodList, null, nai, serviceDescriptions));

                // add only half of All OSU Providers for AP2.
                if (i >= osuProviderCount / 2) {
                    providerInfoListOfAp2.add(new OsuProviderInfo(
                            friendlyNames, serverUri, methodList, null, nai, serviceDescriptions));
                    expectedOsuProvidersForDomainId2.add(new OsuProvider(
                            null, friendlyNameMap, serviceDescription,
                            serverUri, nai, methodList,
                            null));
                }
            }
            anqpElementMapOfAp1.put(ANQPElementType.HSOSUProviders,
                    new HSOsuProvidersElement(WifiSsid.createFromAsciiEncoded("Test SSID"),
                            providerInfoListOfAp1));
            ANQPData anqpData = new ANQPData(mClock, anqpElementMapOfAp1);
            when(mAnqpCache.getEntry(TEST_ANQP_KEY)).thenReturn(anqpData);

            anqpElementMapOfAp2.put(ANQPElementType.HSOSUProviders,
                    new HSOsuProvidersElement(WifiSsid.createFromAsciiEncoded("Test SSID2"),
                            providerInfoListOfAp2));
            ANQPData anqpData2 = new ANQPData(mClock, anqpElementMapOfAp2);
            when(mAnqpCache.getEntry(TEST_ANQP_KEY2)).thenReturn(anqpData2);

            InformationElementUtil.Vsa vsa = new InformationElementUtil.Vsa();

            // ANQP_DOMAIN_ID(TEST_ANQP_KEY)
            vsa.anqpDomainID = TEST_ANQP_DOMAIN_ID;
            when(InformationElementUtil.getHS2VendorSpecificIE(isNull())).thenReturn(vsa);
            assertEquals(mManager.getMatchingOsuProviders(
                    Arrays.asList(createTestScanResult())).keySet(),
                    expectedOsuProvidersForDomainId);

            // ANQP_DOMAIN_ID2(TEST_ANQP_KEY2)
            vsa.anqpDomainID = TEST_ANQP_DOMAIN_ID2;
            when(InformationElementUtil.getHS2VendorSpecificIE(isNull())).thenReturn(vsa);
            assertEquals(mManager.getMatchingOsuProviders(
                    createTestScanResults()).keySet(), expectedOsuProvidersForDomainId2);
        } finally {
            session.finishMocking();
        }
    }

    /**
     * Verify that matching Passpoint configurations will be returned as map with corresponding
     * OSU providers.
     */
    @Test
    public void getMatchingPasspointConfigsForOsuProvidersWithMatch() {
        PasspointProvider provider1 = addTestProvider(TEST_FQDN, TEST_FRIENDLY_NAME, TEST_PACKAGE);
        PasspointProvider provider2 = addTestProvider(TEST_FQDN2, TEST_FRIENDLY_NAME2,
                TEST_PACKAGE);

        List<OsuProvider> osuProviders = new ArrayList<>();
        Map<String, String> friendlyNames = new HashMap<>();
        friendlyNames.put("en", "NO-MATCH-NAME");
        friendlyNames.put("kr", TEST_FRIENDLY_NAME + 1);

        osuProviders.add(PasspointProvisioningTestUtil.generateOsuProviderWithFriendlyName(true,
                friendlyNames));
        friendlyNames = new HashMap<>();
        friendlyNames.put("en", TEST_FRIENDLY_NAME2);
        osuProviders.add(PasspointProvisioningTestUtil.generateOsuProviderWithFriendlyName(true,
                friendlyNames));

        Map<OsuProvider, PasspointConfiguration> results =
                mManager.getMatchingPasspointConfigsForOsuProviders(osuProviders);

        assertEquals(2, results.size());
        assertThat(Arrays.asList(provider1.getConfig(), provider2.getConfig()),
                containsInAnyOrder(results.values().toArray()));
    }

    /**
     * Verify that empty map will be returned when there is no matching Passpoint configuration.
     */
    @Test
    public void getMatchingPasspointConfigsForOsuProvidersWitNoMatch() {
        addTestProvider(TEST_FQDN, TEST_FRIENDLY_NAME, TEST_PACKAGE);
        addTestProvider(TEST_FQDN2, TEST_FRIENDLY_NAME2, TEST_PACKAGE);

        List<OsuProvider> osuProviders = new ArrayList<>();

        Map<String, String> friendlyNames = new HashMap<>();
        friendlyNames.put("en", "NO-MATCH-NAME");
        osuProviders.add(PasspointProvisioningTestUtil.generateOsuProviderWithFriendlyName(true,
                friendlyNames));
        friendlyNames = new HashMap<>();
        friendlyNames.put("en", "NO-MATCH-NAME-2");
        osuProviders.add(PasspointProvisioningTestUtil.generateOsuProviderWithFriendlyName(true,
                friendlyNames));

        assertEquals(0, mManager.getMatchingPasspointConfigsForOsuProviders(osuProviders).size());
    }

    /**
     * Verify that the provider list maintained by the PasspointManager after the list is updated
     * in the data source.
     *
     * @throws Exception
     */
    @Test
    public void verifyProvidersAfterDataSourceUpdate() throws Exception {
        // Update the provider list in the data source.
        PasspointConfiguration config = createTestConfigWithUserCredential(TEST_FQDN,
                TEST_FRIENDLY_NAME);
        PasspointProvider provider = createMockProvider(config);
        List<PasspointProvider> providers = new ArrayList<>();
        providers.add(provider);
        mUserDataSource.setProviders(providers);

        // Verify the providers maintained by PasspointManager.
        assertEquals(1, mManager.getProviderConfigs(TEST_CREATOR_UID, true).size());
        assertEquals(config, mManager.getProviderConfigs(TEST_CREATOR_UID, true).get(0));
    }

    /**
     * Verify that the provider index used by PasspointManager is updated after it is updated in
     * the data source.
     *
     * @throws Exception
     */
    @Test
    public void verifyProviderIndexAfterDataSourceUpdate() throws Exception {
        long providerIndex = 9;
        mSharedDataSource.setProviderIndex(providerIndex);
        assertEquals(providerIndex, mSharedDataSource.getProviderIndex());

        // Add a provider.
        PasspointConfiguration config = createTestConfigWithUserCredential(TEST_FQDN,
                TEST_FRIENDLY_NAME);
        PasspointProvider provider = createMockProvider(config);
        // Verify the provider ID used to create the new provider.
        when(mObjectFactory.makePasspointProvider(eq(config), eq(mWifiKeyStore),
                eq(mSimAccessor), eq(providerIndex), eq(TEST_CREATOR_UID),
                eq(TEST_PACKAGE))).thenReturn(provider);

        assertTrue(mManager.addOrUpdateProvider(config, TEST_CREATOR_UID, TEST_PACKAGE));
        verifyInstalledConfig(config);
        verify(mWifiConfigManager).saveToStore(true);
        reset(mWifiConfigManager);
    }

    /**
     * Verify that a PasspointProvider with expected PasspointConfiguration will be installed when
     * adding a legacy Passpoint configuration containing a valid user credential.
     *
     * @throws Exception
     */
    @Test
    public void addLegacyPasspointConfigWithUserCredential() throws Exception {
        // Test data.
        String fqdn = "test.com";
        String friendlyName = "Friendly Name";
        long[] rcOIs = new long[] {0x1234L, 0x2345L};
        String realm = "realm.com";
        String username = "username";
        String password = "password";
        byte[] base64EncodedPw =
                Base64.encode(password.getBytes(StandardCharsets.UTF_8), Base64.DEFAULT);
        String encodedPasswordStr = new String(base64EncodedPw, StandardCharsets.UTF_8);
        String caCertificateAlias = "CaCert";

        // Setup WifiConfiguration for legacy Passpoint configuraiton.
        WifiConfiguration wifiConfig = new WifiConfiguration();
        wifiConfig.FQDN = fqdn;
        wifiConfig.providerFriendlyName = friendlyName;
        wifiConfig.roamingConsortiumIds = rcOIs;
        wifiConfig.enterpriseConfig.setIdentity(username);
        wifiConfig.enterpriseConfig.setPassword(password);
        wifiConfig.enterpriseConfig.setRealm(realm);
        wifiConfig.enterpriseConfig.setEapMethod(WifiEnterpriseConfig.Eap.TTLS);
        wifiConfig.enterpriseConfig.setPhase2Method(WifiEnterpriseConfig.Phase2.PAP);
        wifiConfig.enterpriseConfig.setCaCertificateAlias(caCertificateAlias);

        // Setup expected {@link PasspointConfiguration}
        PasspointConfiguration passpointConfig = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setFqdn(fqdn);
        homeSp.setFriendlyName(friendlyName);
        homeSp.setRoamingConsortiumOis(rcOIs);
        passpointConfig.setHomeSp(homeSp);
        Credential credential = new Credential();
        Credential.UserCredential userCredential = new Credential.UserCredential();
        userCredential.setUsername(username);
        userCredential.setPassword(encodedPasswordStr);
        userCredential.setEapType(EAPConstants.EAP_TTLS);
        userCredential.setNonEapInnerMethod("PAP");
        credential.setUserCredential(userCredential);
        credential.setRealm(realm);
        passpointConfig.setCredential(credential);

        assertTrue(PasspointManager.addLegacyPasspointConfig(wifiConfig));
        verifyInstalledConfig(passpointConfig);
    }

    /**
     * Verify that adding a legacy Passpoint configuration containing user credential will
     * fail when client certificate is not provided.
     *
     * @throws Exception
     */
    @Test
    public void addLegacyPasspointConfigWithUserCredentialWithoutCaCert() throws Exception {
        // Test data.
        String fqdn = "test.com";
        String friendlyName = "Friendly Name";
        long[] rcOIs = new long[] {0x1234L, 0x2345L};
        String realm = "realm.com";
        String username = "username";
        String password = "password";
        byte[] base64EncodedPw =
                Base64.encode(password.getBytes(StandardCharsets.UTF_8), Base64.DEFAULT);
        String encodedPasswordStr = new String(base64EncodedPw, StandardCharsets.UTF_8);

        // Setup WifiConfiguration for legacy Passpoint configuraiton.
        WifiConfiguration wifiConfig = new WifiConfiguration();
        wifiConfig.FQDN = fqdn;
        wifiConfig.providerFriendlyName = friendlyName;
        wifiConfig.roamingConsortiumIds = rcOIs;
        wifiConfig.enterpriseConfig.setIdentity(username);
        wifiConfig.enterpriseConfig.setPassword(password);
        wifiConfig.enterpriseConfig.setRealm(realm);
        wifiConfig.enterpriseConfig.setEapMethod(WifiEnterpriseConfig.Eap.TTLS);
        wifiConfig.enterpriseConfig.setPhase2Method(WifiEnterpriseConfig.Phase2.PAP);

        assertFalse(PasspointManager.addLegacyPasspointConfig(wifiConfig));
    }

    /**
     * Verify that a PasspointProvider with expected PasspointConfiguration will be installed when
     * adding a legacy Passpoint configuration containing a valid SIM credential.
     *
     * @throws Exception
     */
    @Test
    public void addLegacyPasspointConfigWithSimCredential() throws Exception {
        // Test data.
        String fqdn = "test.com";
        String friendlyName = "Friendly Name";
        long[] rcOIs = new long[] {0x1234L, 0x2345L};
        String realm = "realm.com";
        String imsi = "1234";

        // Setup WifiConfiguration for legacy Passpoint configuraiton.
        WifiConfiguration wifiConfig = new WifiConfiguration();
        wifiConfig.FQDN = fqdn;
        wifiConfig.providerFriendlyName = friendlyName;
        wifiConfig.roamingConsortiumIds = rcOIs;
        wifiConfig.enterpriseConfig.setRealm(realm);
        wifiConfig.enterpriseConfig.setEapMethod(WifiEnterpriseConfig.Eap.SIM);
        wifiConfig.enterpriseConfig.setPlmn(imsi);

        // Setup expected {@link PasspointConfiguration}
        PasspointConfiguration passpointConfig = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setFqdn(fqdn);
        homeSp.setFriendlyName(friendlyName);
        homeSp.setRoamingConsortiumOis(rcOIs);
        passpointConfig.setHomeSp(homeSp);
        Credential credential = new Credential();
        Credential.SimCredential simCredential = new Credential.SimCredential();
        simCredential.setEapType(EAPConstants.EAP_SIM);
        simCredential.setImsi(imsi);
        credential.setSimCredential(simCredential);
        credential.setRealm(realm);
        passpointConfig.setCredential(credential);

        assertTrue(PasspointManager.addLegacyPasspointConfig(wifiConfig));
        verifyInstalledConfig(passpointConfig);
    }

    /**
     * Verify that a PasspointProvider with expected PasspointConfiguration will be installed when
     * adding a legacy Passpoint configuration containing a valid certificate credential.
     *
     * @throws Exception
     */
    @Test
    public void addLegacyPasspointConfigWithCertCredential() throws Exception {
        // Test data.
        String fqdn = "test.com";
        String friendlyName = "Friendly Name";
        long[] rcOIs = new long[] {0x1234L, 0x2345L};
        String realm = "realm.com";
        String caCertificateAlias = "CaCert";
        String clientCertificateAlias = "ClientCert";

        // Setup WifiConfiguration for legacy Passpoint configuraiton.
        WifiConfiguration wifiConfig = new WifiConfiguration();
        wifiConfig.FQDN = fqdn;
        wifiConfig.providerFriendlyName = friendlyName;
        wifiConfig.roamingConsortiumIds = rcOIs;
        wifiConfig.enterpriseConfig.setRealm(realm);
        wifiConfig.enterpriseConfig.setEapMethod(WifiEnterpriseConfig.Eap.TLS);
        wifiConfig.enterpriseConfig.setCaCertificateAlias(caCertificateAlias);
        wifiConfig.enterpriseConfig.setClientCertificateAlias(clientCertificateAlias);

        // Setup expected {@link PasspointConfiguration}
        PasspointConfiguration passpointConfig = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setFqdn(fqdn);
        homeSp.setFriendlyName(friendlyName);
        homeSp.setRoamingConsortiumOis(rcOIs);
        passpointConfig.setHomeSp(homeSp);
        Credential credential = new Credential();
        Credential.CertificateCredential certCredential = new Credential.CertificateCredential();
        certCredential.setCertType(Credential.CertificateCredential.CERT_TYPE_X509V3);
        credential.setCertCredential(certCredential);
        credential.setRealm(realm);
        passpointConfig.setCredential(credential);

        assertTrue(PasspointManager.addLegacyPasspointConfig(wifiConfig));
        verifyInstalledConfig(passpointConfig);
    }

    /**
     * Verify that adding a legacy Passpoint configuration containing certificate credential will
     * fail when CA certificate is not provided.
     *
     * @throws Exception
     */
    @Test
    public void addLegacyPasspointConfigWithCertCredentialWithoutCaCert() throws Exception {
        // Test data.
        String fqdn = "test.com";
        String friendlyName = "Friendly Name";
        long[] rcOIs = new long[] {0x1234L, 0x2345L};
        String realm = "realm.com";
        String clientCertificateAlias = "ClientCert";

        // Setup WifiConfiguration for legacy Passpoint configuraiton.
        WifiConfiguration wifiConfig = new WifiConfiguration();
        wifiConfig.FQDN = fqdn;
        wifiConfig.providerFriendlyName = friendlyName;
        wifiConfig.roamingConsortiumIds = rcOIs;
        wifiConfig.enterpriseConfig.setRealm(realm);
        wifiConfig.enterpriseConfig.setEapMethod(WifiEnterpriseConfig.Eap.TLS);
        wifiConfig.enterpriseConfig.setClientCertificateAlias(clientCertificateAlias);

        assertFalse(PasspointManager.addLegacyPasspointConfig(wifiConfig));
    }

    /**
     * Verify that adding a legacy Passpoint configuration containing certificate credential will
     * fail when client certificate is not provided.
     *
     * @throws Exception
     */
    @Test
    public void addLegacyPasspointConfigWithCertCredentialWithoutClientCert() throws Exception {
        // Test data.
        String fqdn = "test.com";
        String friendlyName = "Friendly Name";
        long[] rcOIs = new long[] {0x1234L, 0x2345L};
        String realm = "realm.com";
        String caCertificateAlias = "CaCert";

        // Setup WifiConfiguration for legacy Passpoint configuraiton.
        WifiConfiguration wifiConfig = new WifiConfiguration();
        wifiConfig.FQDN = fqdn;
        wifiConfig.providerFriendlyName = friendlyName;
        wifiConfig.roamingConsortiumIds = rcOIs;
        wifiConfig.enterpriseConfig.setRealm(realm);
        wifiConfig.enterpriseConfig.setEapMethod(WifiEnterpriseConfig.Eap.TLS);
        wifiConfig.enterpriseConfig.setCaCertificateAlias(caCertificateAlias);

        assertFalse(PasspointManager.addLegacyPasspointConfig(wifiConfig));
    }

    /**
     * Verify that the provider's "hasEverConnected" flag will be set to true and the associated
     * metric is updated after the provider was used to successfully connect to a Passpoint
     * network for the first time.
     *
     * @throws Exception
     */
    @Test
    public void providerNetworkConnectedFirstTime() throws Exception {
        PasspointProvider provider = addTestProvider(TEST_FQDN, TEST_FRIENDLY_NAME, TEST_PACKAGE);
        when(provider.getHasEverConnected()).thenReturn(false);
        mManager.onPasspointNetworkConnected(TEST_FQDN);
        verify(provider).setHasEverConnected(eq(true));
    }

    /**
     * Verify that the provider's "hasEverConnected" flag the associated metric is not updated
     * after the provider was used to successfully connect to a Passpoint network for non-first
     * time.
     *
     * @throws Exception
     */
    @Test
    public void providerNetworkConnectedNotFirstTime() throws Exception {
        PasspointProvider provider = addTestProvider(TEST_FQDN, TEST_FRIENDLY_NAME, TEST_PACKAGE);
        when(provider.getHasEverConnected()).thenReturn(true);
        mManager.onPasspointNetworkConnected(TEST_FQDN);
        verify(provider, never()).setHasEverConnected(anyBoolean());
    }

    /**
     * Verify that the expected Passpoint metrics are updated when
     * {@link PasspointManager#updateMetrics} is invoked.
     *
     * @throws Exception
     */
    @Test
    public void updateMetrics() {
        PasspointProvider provider = addTestProvider(TEST_FQDN, TEST_FRIENDLY_NAME, TEST_PACKAGE);
        ArgumentCaptor<Map<String, PasspointProvider>> argCaptor = ArgumentCaptor.forClass(
                Map.class);
        // Provider have not provided a successful network connection.
        int expectedInstalledProviders = 1;
        int expectedConnectedProviders = 0;
        when(provider.getHasEverConnected()).thenReturn(false);
        mManager.updateMetrics();
        verify(mWifiMetrics).updateSavedPasspointProfiles(
                eq(expectedInstalledProviders), eq(expectedConnectedProviders));

        verify(mWifiMetrics).updateSavedPasspointProfilesInfo(argCaptor.capture());
        assertEquals(expectedInstalledProviders, argCaptor.getValue().size());
        assertEquals(provider, argCaptor.getValue().get(TEST_FQDN));
        reset(mWifiMetrics);

        // Provider have provided a successful network connection.
        expectedConnectedProviders = 1;
        when(provider.getHasEverConnected()).thenReturn(true);
        mManager.updateMetrics();
        verify(mWifiMetrics).updateSavedPasspointProfiles(
                eq(expectedInstalledProviders), eq(expectedConnectedProviders));
    }

    /**
     * Verify Passpoint Manager's provisioning APIs by invoking methods in PasspointProvisioner for
     * initiailization and provisioning a provider.
     */
    @Test
    public void verifyPasspointProvisioner() {
        mManager.initializeProvisioner(mLooper.getLooper());
        verify(mPasspointProvisioner).init(any(Looper.class));
        when(mPasspointProvisioner.startSubscriptionProvisioning(anyInt(), any(OsuProvider.class),
                any(IProvisioningCallback.class))).thenReturn(true);
        OsuProvider osuProvider = PasspointProvisioningTestUtil.generateOsuProvider(true);
        assertEquals(true,
                mManager.startSubscriptionProvisioning(TEST_UID, osuProvider, mCallback));
    }

    /**
     * Verify that it will return {@code null} if the EAP-Method provided is not a carrier
     * EAP-Method.
     */
    @Test
    public void verifyCreateEphemeralPasspointConfigurationWithNonCarrierEapMethod() {
        when(mTelephonyManager.createForSubscriptionId(anyInt())).thenReturn(mDataTelephonyManager);
        when(mDataTelephonyManager.getSimOperator()).thenReturn("123456");
        PasspointManager passpointManager = new PasspointManager(mContext, mWifiInjector,
                mHandler, mWifiNative, mWifiKeyStore, mClock, mSimAccessor, mObjectFactory,
                mWifiConfigManager, mWifiConfigStore, mWifiMetrics, mTelephonyManager,
                mSubscriptionManager);

        assertNull(passpointManager.createEphemeralPasspointConfigForCarrier(
                EAPConstants.EAP_TLS));
    }

    /**
     * Verify that it creates an ephemeral Passpoint configuration for the carrier.
     */
    @Test
    public void verifyCreateEphemeralPasspointConfigurationWithCarrierEapMethod() {
        String mccmnc = "123456";
        when(mTelephonyManager.createForSubscriptionId(anyInt())).thenReturn(mDataTelephonyManager);
        when(mDataTelephonyManager.getSimOperator()).thenReturn(mccmnc);
        when(mDataTelephonyManager.getSimOperatorName()).thenReturn("test");
        PasspointManager passpointManager = new PasspointManager(mContext, mWifiInjector,
                mHandler, mWifiNative, mWifiKeyStore, mClock, mSimAccessor, mObjectFactory,
                mWifiConfigManager, mWifiConfigStore, mWifiMetrics, mTelephonyManager,
                mSubscriptionManager);

        PasspointConfiguration result =
                passpointManager.createEphemeralPasspointConfigForCarrier(
                        EAPConstants.EAP_AKA);

        assertNotNull(result);
        assertTrue(result.validate());
        assertEquals(Utils.getRealmForMccMnc(mccmnc), result.getHomeSp().getFqdn());
        assertEquals(mccmnc + "*", result.getCredential().getSimCredential().getImsi());
    }

    /**
     * Verify that it will not install the Passpoint configuration with Non-Carrier EAP method.
     */
    @Test
    public void verifyInstallEphemeralPasspointConfigurationWithNonCarrierEapMethod() {
        // SIM is present
        when(mSubscriptionManager.getActiveSubscriptionIdList()).thenReturn(new int[1]);
        PasspointConfiguration config = createTestConfigWithUserCredential("abc.com", "test");
        PasspointProvider provider = createMockProvider(config);
        when(mObjectFactory.makePasspointProvider(eq(config), eq(mWifiKeyStore),
                eq(mSimAccessor), anyLong(), anyInt(), isNull())).thenReturn(provider);

        assertFalse(mManager.installEphemeralPasspointConfigForCarrier(config));
    }

    /**
     * Verify that it installs the carrier Passpoint configuration successfully.
     */
    @Test
    public void verifyInstallEphemeralPasspointConfiguration() {
        // SIM is present
        when(mSubscriptionManager.getActiveSubscriptionIdList()).thenReturn(new int[1]);
        PasspointConfiguration config = createTestConfigWithSimCredential(TEST_FQDN, TEST_IMSI,
                TEST_REALM);
        PasspointProvider provider = createMockProvider(config);
        when(mObjectFactory.makePasspointProvider(eq(config), eq(mWifiKeyStore),
                eq(mSimAccessor), anyLong(), anyInt(), isNull())).thenReturn(provider);

        assertTrue(mManager.installEphemeralPasspointConfigForCarrier(config));
        verify(mAppOpsManager, never()).startWatchingMode(eq(OPSTR_CHANGE_WIFI_STATE),
                eq(TEST_PACKAGE), any(AppOpsManager.OnOpChangedListener.class));
    }

    /**
     * Verify that it returns {@code true} when it has Carrier Provider.
     */
    @Test
    public void verifyHasProviderForCarrierWithMatch() {
        addTestCarrierProvider(TEST_3GPP_FQDN, TEST_MCC_MNC, TEST_3GPP_FQDN);

        assertTrue(mManager.hasCarrierProvider(TEST_MCC_MNC));
    }

    /**
     * Verify that it returns {@code false} when it does not have Carrier Provider.
     */
    @Test
    public void verifyHasProviderForCarrierWithNoMatch() {
        addTestProvider(TEST_FQDN, TEST_FRIENDLY_NAME, TEST_PACKAGE);

        assertFalse(mManager.hasCarrierProvider(TEST_MCC_MNC));
    }

    /**
     * Verify that it returns a carrier EAP-method from NAI-Realm matched with the carrier.
     */
    @Test
    public void verifyFindEapMethodFromNAIRealmMatchedWithCarrierWithMatch() {
        // static mocking
        MockitoSession session = ExtendedMockito.mockitoSession()
                .mockStatic(InformationElementUtil.class)
                .startMocking();
        try {
            when(mTelephonyManager.createForSubscriptionId(anyInt()))
                    .thenReturn(mDataTelephonyManager);
            when(mDataTelephonyManager.getSimOperator()).thenReturn(TEST_MCC_MNC);
            // SIM is present
            when(mSubscriptionManager.getActiveSubscriptionIdList()).thenReturn(new int[1]);
            List<ScanDetail> scanDetails = new ArrayList<>();
            scanDetails.add(generateScanDetail(TEST_SSID, TEST_BSSID_STRING, TEST_HESSID,
                    TEST_ANQP_DOMAIN_ID, true));
            InformationElementUtil.Vsa vsa = new InformationElementUtil.Vsa();

            // ANQP_DOMAIN_ID(TEST_ANQP_KEY)
            vsa.anqpDomainID = TEST_ANQP_DOMAIN_ID;
            when(InformationElementUtil.getHS2VendorSpecificIE(isNull())).thenReturn(vsa);
            Map<ANQPElementType, ANQPElement> anqpElementMapOfAp1 = new HashMap<>();
            List<NAIRealmData> realmDataList = new ArrayList<>();
            realmDataList.add(new NAIRealmData(Arrays.asList(TEST_3GPP_FQDN),
                    Arrays.asList(new EAPMethod(EAPConstants.EAP_AKA, null))));
            anqpElementMapOfAp1.put(ANQPElementType.ANQPNAIRealm,
                    new NAIRealmElement(realmDataList));

            ANQPData anqpData = new ANQPData(mClock, anqpElementMapOfAp1);
            when(mAnqpCache.getEntry(TEST_ANQP_KEY)).thenReturn(anqpData);

            PasspointManager passpointManager = new PasspointManager(mContext, mWifiInjector,
                    mHandler, mWifiNative, mWifiKeyStore, mClock, mSimAccessor, mObjectFactory,
                    mWifiConfigManager, mWifiConfigStore, mWifiMetrics, mTelephonyManager,
                    mSubscriptionManager);
            assertEquals(EAPConstants.EAP_AKA,
                    passpointManager.findEapMethodFromNAIRealmMatchedWithCarrier(scanDetails));
        } finally {
            session.finishMocking();
        }
    }

    /**
     * Verify that it returns -1 when there is no NAI-Realm matched with the carrier.
     */
    @Test
    public void verifyFindEapMethodFromNAIRealmMatchedWithCarrierWithNoMatch() {
        // static mocking
        MockitoSession session = ExtendedMockito.mockitoSession()
                .mockStatic(InformationElementUtil.class)
                .startMocking();
        try {
            when(mTelephonyManager.createForSubscriptionId(anyInt()))
                    .thenReturn(mDataTelephonyManager);
            when(mDataTelephonyManager.getSimOperator()).thenReturn(TEST_MCC_MNC);
            // SIM is present
            when(mSubscriptionManager.getActiveSubscriptionIdList()).thenReturn(new int[1]);
            List<ScanDetail> scanDetails = new ArrayList<>();
            scanDetails.add(generateScanDetail(TEST_SSID, TEST_BSSID_STRING, 0, 0, false));

            PasspointManager passpointManager = new PasspointManager(mContext, mWifiInjector,
                    mHandler, mWifiNative, mWifiKeyStore, mClock, mSimAccessor, mObjectFactory,
                    mWifiConfigManager, mWifiConfigStore, mWifiMetrics, mTelephonyManager,
                    mSubscriptionManager);

            assertEquals(-1,
                    passpointManager.findEapMethodFromNAIRealmMatchedWithCarrier(scanDetails));
        } finally {
            session.finishMocking();
        }
    }

    /**
     * Verify that the corresponding Passpoint provider is removed when the app is disabled.
     */
    @Test
    public void verifyRemovingPasspointProfilesWhenAppIsDisabled() {
        WifiConfiguration currentConfiguration = WifiConfigurationTestUtil.createPasspointNetwork();
        currentConfiguration.FQDN = TEST_FQDN;
        when(mClientModeImpl.getCurrentWifiConfiguration()).thenReturn(currentConfiguration);
        addTestProvider(TEST_FQDN, TEST_FRIENDLY_NAME, TEST_PACKAGE);

        verify(mAppOpsManager).startWatchingMode(eq(OPSTR_CHANGE_WIFI_STATE), eq(TEST_PACKAGE),
                mAppOpChangedListenerCaptor.capture());
        assertEquals(1, mManager.getProviderConfigs(TEST_CREATOR_UID, true).size());
        AppOpsManager.OnOpChangedListener listener = mAppOpChangedListenerCaptor.getValue();
        assertNotNull(listener);

        // Disallow change wifi state & ensure we remove the profiles from database.
        when(mAppOpsManager.unsafeCheckOpNoThrow(
                OPSTR_CHANGE_WIFI_STATE, TEST_CREATOR_UID,
                TEST_PACKAGE))
                .thenReturn(MODE_IGNORED);
        listener.onOpChanged(OPSTR_CHANGE_WIFI_STATE, TEST_PACKAGE);
        mLooper.dispatchAll();

        verify(mAppOpsManager).stopWatchingMode(mAppOpChangedListenerCaptor.getValue());
        verify(mClientModeImpl).disconnectCommand();
        assertTrue(mManager.getProviderConfigs(TEST_CREATOR_UID, true).isEmpty());
    }

    /**
     * Verify that removing a provider with a different UID will not succeed.
     *
     * @throws Exception
     */
    @Test
    public void removeGetProviderWithDifferentUid() throws Exception {
        PasspointConfiguration config = createTestConfigWithSimCredential(TEST_FQDN, TEST_IMSI,
                TEST_REALM);
        PasspointProvider provider = createMockProvider(config);
        when(mObjectFactory.makePasspointProvider(eq(config), eq(mWifiKeyStore),
                eq(mSimAccessor), anyLong(), eq(TEST_CREATOR_UID), eq(TEST_PACKAGE))).thenReturn(
                provider);
        assertTrue(mManager.addOrUpdateProvider(config, TEST_CREATOR_UID, TEST_PACKAGE));
        verifyInstalledConfig(config);
        verify(mWifiConfigManager).saveToStore(true);
        verify(mWifiMetrics).incrementNumPasspointProviderInstallation();
        verify(mWifiMetrics).incrementNumPasspointProviderInstallSuccess();
        reset(mWifiMetrics);
        reset(mWifiConfigManager);

        // no profiles available for TEST_UID
        assertTrue(mManager.getProviderConfigs(TEST_UID, false).isEmpty());
        // 1 profile available for TEST_CREATOR_UID
        assertFalse(mManager.getProviderConfigs(TEST_CREATOR_UID, false).isEmpty());

        // Remove the provider as a non-privileged non-creator app.
        assertFalse(mManager.removeProvider(TEST_UID, false, TEST_FQDN));
        verify(provider, never()).uninstallCertsAndKeys();
        verify(mWifiConfigManager, never()).saveToStore(true);
        verify(mWifiMetrics).incrementNumPasspointProviderUninstallation();
        verify(mWifiMetrics, never()).incrementNumPasspointProviderUninstallSuccess();

        // no profiles available for TEST_UID
        assertTrue(mManager.getProviderConfigs(TEST_UID, false).isEmpty());
        // 1 profile available for TEST_CREATOR_UID
        assertFalse(mManager.getProviderConfigs(TEST_CREATOR_UID, false).isEmpty());
    }
}
