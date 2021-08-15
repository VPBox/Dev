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

package com.android.server.wifi.hotspot2;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

import android.net.wifi.hotspot2.PasspointConfiguration;
import android.net.wifi.hotspot2.pps.Credential;
import android.net.wifi.hotspot2.pps.HomeSp;
import android.net.wifi.hotspot2.pps.Policy;
import android.net.wifi.hotspot2.pps.UpdateParameter;
import android.util.Xml;

import androidx.test.filters.SmallTest;

import com.android.internal.util.FastXmlSerializer;
import com.android.server.wifi.SIMAccessor;
import com.android.server.wifi.WifiConfigStore;
import com.android.server.wifi.WifiKeyStore;
import com.android.server.wifi.util.WifiConfigStoreEncryptionUtil;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlSerializer;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.nio.charset.StandardCharsets;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.PasspointConfigUserStoreData}.
 */
@SmallTest
public class PasspointConfigUserStoreDataTest {
    private static final String TEST_CA_CERTIFICATE_ALIAS = "CaCert";
    private static final String TEST_CA_CERTIFICATE_ALIAS_2 = "CaCert_2";
    private static final String TEST_CLIENT_CERTIFICATE_ALIAS = "ClientCert";
    private static final String TEST_CLIENT_PRIVATE_KEY_ALIAS = "ClientPrivateKey";
    private static final String TEST_REMEDIATION_CA_CERTIFICATE_ALIAS = "CaCert_3";
    private static final String TEST_CREATOR_PACKAGE = "com.android.test";
    private static final long TEST_PROVIDER_ID = 1;
    private static final long TEST_PROVIDER_ID_2 = 2;
    private static final int TEST_CREATOR_UID = 1234;
    private static final boolean TEST_HAS_EVER_CONNECTED = true;
    private static final boolean TEST_SHARED = false;

    @Mock WifiKeyStore mKeyStore;
    @Mock SIMAccessor mSimAccessor;
    @Mock PasspointConfigUserStoreData.DataSource mDataSource;
    PasspointConfigUserStoreData mConfigStoreData;

    /** Sets up test. */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mConfigStoreData = new PasspointConfigUserStoreData(mKeyStore, mSimAccessor, mDataSource);
    }

    /**
     * Helper function for generating a {@link PasspointConfiguration} for testing the XML
     * serialization/deserialization logic.
     *
     * @return {@link PasspointConfiguration}
     * @throws Exception
     */
    private PasspointConfiguration createFullPasspointConfiguration() throws Exception {
        DateFormat format = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss'Z'");
        byte[] certFingerprint = new byte[32];
        Arrays.fill(certFingerprint, (byte) 0x1f);

        PasspointConfiguration config = new PasspointConfiguration();
        config.setUpdateIdentifier(12);
        config.setCredentialPriority(99);

        // AAA Server trust root.
        Map<String, byte[]> trustRootCertList = new HashMap<>();
        trustRootCertList.put("server1.trust.root.com", certFingerprint);
        config.setTrustRootCertList(trustRootCertList);

        // Subscription update.
        UpdateParameter subscriptionUpdate = new UpdateParameter();
        subscriptionUpdate.setUpdateIntervalInMinutes(120);
        subscriptionUpdate.setUpdateMethod(UpdateParameter.UPDATE_METHOD_SSP);
        subscriptionUpdate.setRestriction(UpdateParameter.UPDATE_RESTRICTION_ROAMING_PARTNER);
        subscriptionUpdate.setServerUri("subscription.update.com");
        subscriptionUpdate.setUsername("subscriptionUser");
        subscriptionUpdate.setBase64EncodedPassword("subscriptionPass");
        subscriptionUpdate.setTrustRootCertUrl("subscription.update.cert.com");
        subscriptionUpdate.setTrustRootCertSha256Fingerprint(certFingerprint);
        config.setSubscriptionUpdate(subscriptionUpdate);

        // Subscription parameters.
        config.setSubscriptionCreationTimeInMillis(format.parse("2016-02-01T10:00:00Z").getTime());
        config.setSubscriptionExpirationTimeInMillis(
                format.parse("2016-03-01T10:00:00Z").getTime());
        config.setSubscriptionType("Gold");
        config.setUsageLimitDataLimit(921890);
        config.setUsageLimitStartTimeInMillis(format.parse("2016-12-01T10:00:00Z").getTime());
        config.setUsageLimitTimeLimitInMinutes(120);
        config.setUsageLimitUsageTimePeriodInMinutes(99910);

        // HomeSP configuration.
        HomeSp homeSp = new HomeSp();
        homeSp.setFriendlyName("Century House");
        homeSp.setFqdn("mi6.co.uk");
        homeSp.setRoamingConsortiumOis(new long[] {0x112233L, 0x445566L});
        homeSp.setIconUrl("icon.test.com");
        Map<String, Long> homeNetworkIds = new HashMap<>();
        homeNetworkIds.put("TestSSID", 0x12345678L);
        homeNetworkIds.put("NullHESSID", null);
        homeSp.setHomeNetworkIds(homeNetworkIds);
        homeSp.setMatchAllOis(new long[] {0x11223344});
        homeSp.setMatchAnyOis(new long[] {0x55667788});
        homeSp.setOtherHomePartners(new String[] {"other.fqdn.com"});
        config.setHomeSp(homeSp);

        // Credential configuration.
        Credential credential = new Credential();
        credential.setCreationTimeInMillis(format.parse("2016-01-01T10:00:00Z").getTime());
        credential.setExpirationTimeInMillis(format.parse("2016-02-01T10:00:00Z").getTime());
        credential.setRealm("shaken.stirred.com");
        credential.setCheckAaaServerCertStatus(true);
        Credential.UserCredential userCredential = new Credential.UserCredential();
        userCredential.setUsername("james");
        userCredential.setPassword("Ym9uZDAwNw==");
        userCredential.setMachineManaged(true);
        userCredential.setSoftTokenApp("TestApp");
        userCredential.setAbleToShare(true);
        userCredential.setEapType(21);
        userCredential.setNonEapInnerMethod("MS-CHAP-V2");
        credential.setUserCredential(userCredential);
        Credential.CertificateCredential certCredential = new Credential.CertificateCredential();
        certCredential.setCertType("x509v3");
        certCredential.setCertSha256Fingerprint(certFingerprint);
        credential.setCertCredential(certCredential);
        Credential.SimCredential simCredential = new Credential.SimCredential();
        simCredential.setImsi("imsi");
        simCredential.setEapType(24);
        credential.setSimCredential(simCredential);
        config.setCredential(credential);

        // Policy configuration.
        Policy policy = new Policy();
        List<Policy.RoamingPartner> preferredRoamingPartnerList = new ArrayList<>();
        Policy.RoamingPartner partner1 = new Policy.RoamingPartner();
        partner1.setFqdn("test1.fqdn.com");
        partner1.setFqdnExactMatch(true);
        partner1.setPriority(127);
        partner1.setCountries("us,fr");
        Policy.RoamingPartner partner2 = new Policy.RoamingPartner();
        partner2.setFqdn("test2.fqdn.com");
        partner2.setFqdnExactMatch(false);
        partner2.setPriority(200);
        partner2.setCountries("*");
        preferredRoamingPartnerList.add(partner1);
        preferredRoamingPartnerList.add(partner2);
        policy.setPreferredRoamingPartnerList(preferredRoamingPartnerList);
        policy.setMinHomeDownlinkBandwidth(23412);
        policy.setMinHomeUplinkBandwidth(9823);
        policy.setMinRoamingDownlinkBandwidth(9271);
        policy.setMinRoamingUplinkBandwidth(2315);
        policy.setExcludedSsidList(new String[] {"excludeSSID"});
        Map<Integer, String> requiredProtoPortMap = new HashMap<>();
        requiredProtoPortMap.put(12, "34,92,234");
        policy.setRequiredProtoPortMap(requiredProtoPortMap);
        policy.setMaximumBssLoadValue(23);
        UpdateParameter policyUpdate = new UpdateParameter();
        policyUpdate.setUpdateIntervalInMinutes(120);
        policyUpdate.setUpdateMethod(UpdateParameter.UPDATE_METHOD_OMADM);
        policyUpdate.setRestriction(UpdateParameter.UPDATE_RESTRICTION_HOMESP);
        policyUpdate.setServerUri("policy.update.com");
        policyUpdate.setUsername("updateUser");
        policyUpdate.setBase64EncodedPassword("updatePass");
        policyUpdate.setTrustRootCertUrl("update.cert.com");
        policyUpdate.setTrustRootCertSha256Fingerprint(certFingerprint);
        policy.setPolicyUpdate(policyUpdate);
        config.setPolicy(policy);
        return config;
    }

    /**
     * Helper function for serializing store data to a XML block.
     *
     * @return byte[]
     * @throws Exception
     */
    private byte[] serializeData() throws Exception {
        final XmlSerializer out = new FastXmlSerializer();
        final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        out.setOutput(outputStream, StandardCharsets.UTF_8.name());
        mConfigStoreData.serializeData(out, mock(WifiConfigStoreEncryptionUtil.class));
        out.flush();
        return outputStream.toByteArray();
    }

    /**
     * Helper function for deserializing store data from a XML block.
     *
     * @param data The XML block data bytes
     * @throws Exception
     */
    private void deserializeData(byte[] data) throws Exception {
        final XmlPullParser in = Xml.newPullParser();
        final ByteArrayInputStream inputStream = new ByteArrayInputStream(data);
        in.setInput(inputStream, StandardCharsets.UTF_8.name());
        mConfigStoreData.deserializeData(in, in.getDepth(),
                WifiConfigStore.ENCRYPT_CREDENTIALS_CONFIG_STORE_DATA_VERSION,
                mock(WifiConfigStoreEncryptionUtil.class));
    }

    /**
     * Verify that the serialization and deserialization of user store data works as expected.
     * The data used for serialization matches the result of the deserialization.
     *
     * @throws Exception
     */
    @Test
    public void serializeAndDeserializeUserStoreData() throws Exception {
        // Setup expected data.
        List<PasspointProvider> providerList = new ArrayList<>();
        providerList.add(new PasspointProvider(createFullPasspointConfiguration(),
                mKeyStore, mSimAccessor, TEST_PROVIDER_ID, TEST_CREATOR_UID, TEST_CREATOR_PACKAGE,
                Arrays.asList(TEST_CA_CERTIFICATE_ALIAS), TEST_CLIENT_CERTIFICATE_ALIAS,
                TEST_CLIENT_PRIVATE_KEY_ALIAS, null, TEST_HAS_EVER_CONNECTED, TEST_SHARED));
        providerList.add(new PasspointProvider(createFullPasspointConfiguration(),
                mKeyStore, mSimAccessor, TEST_PROVIDER_ID_2, TEST_CREATOR_UID, TEST_CREATOR_PACKAGE,
                Arrays.asList(TEST_CA_CERTIFICATE_ALIAS, TEST_CA_CERTIFICATE_ALIAS_2),
                TEST_CLIENT_CERTIFICATE_ALIAS,
                TEST_CLIENT_PRIVATE_KEY_ALIAS, TEST_REMEDIATION_CA_CERTIFICATE_ALIAS,
                TEST_HAS_EVER_CONNECTED, TEST_SHARED));

        // Serialize data for user store.
        when(mDataSource.getProviders()).thenReturn(providerList);
        byte[] data = serializeData();

        // Deserialize data for user store and verify the content.
        ArgumentCaptor<ArrayList> providersCaptor = ArgumentCaptor.forClass(ArrayList.class);
        deserializeData(data);
        verify(mDataSource).setProviders(providersCaptor.capture());
        assertEquals(providerList, providersCaptor.getValue());
    }

    /**
     * Verify that deserialization of an empty user store data doesn't cause any exception and
     * the corresponding data source should not be updated.
     *
     * @throws Exception
     */
    @Test
    public void deserializeEmptyUserStoreData() throws Exception {
        deserializeData(new byte[0]);
        verify(mDataSource, never()).setProviders(any(ArrayList.class));
    }

    /**
     * Verify that PasspointConfigUserStoreData is written to
     * {@link WifiConfigStore#STORE_FILE_USER_GENERAL}.
     *
     * @throws Exception
     */
    @Test
    public void getUserStoreFileId() throws Exception {
        assertEquals(WifiConfigStore.STORE_FILE_USER_GENERAL,
                mConfigStoreData.getStoreFileId());
    }
}
