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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;
import static org.mockito.MockitoAnnotations.initMocks;

import android.net.wifi.EAPConstants;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiEnterpriseConfig;
import android.net.wifi.hotspot2.PasspointConfiguration;
import android.net.wifi.hotspot2.pps.Credential;
import android.net.wifi.hotspot2.pps.HomeSp;
import android.net.wifi.hotspot2.pps.UpdateParameter;
import android.text.TextUtils;
import android.util.Base64;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.FakeKeys;
import com.android.server.wifi.IMSIParameter;
import com.android.server.wifi.SIMAccessor;
import com.android.server.wifi.WifiKeyStore;
import com.android.server.wifi.hotspot2.anqp.ANQPElement;
import com.android.server.wifi.hotspot2.anqp.CellularNetwork;
import com.android.server.wifi.hotspot2.anqp.Constants.ANQPElementType;
import com.android.server.wifi.hotspot2.anqp.DomainNameElement;
import com.android.server.wifi.hotspot2.anqp.NAIRealmData;
import com.android.server.wifi.hotspot2.anqp.NAIRealmElement;
import com.android.server.wifi.hotspot2.anqp.RoamingConsortiumElement;
import com.android.server.wifi.hotspot2.anqp.ThreeGPPNetworkElement;
import com.android.server.wifi.hotspot2.anqp.eap.AuthParam;
import com.android.server.wifi.hotspot2.anqp.eap.EAPMethod;
import com.android.server.wifi.hotspot2.anqp.eap.NonEAPInnerAuth;
import com.android.server.wifi.util.InformationElementUtil.RoamingConsortium;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.mockito.Mock;

import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.cert.X509Certificate;
import java.util.Arrays;
import java.util.BitSet;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.PasspointProvider}.
 */
@SmallTest
@RunWith(Parameterized.class)
public class PasspointProviderTest {
    private static final long PROVIDER_ID = 12L;
    private static final int CREATOR_UID = 1234;
    private static final String CREATOR_PACKAGE = "com.android.test";
    private static final String CA_CERTIFICATE_NAME = "CACERT_HS2_12_0";
    private static final String CA_CERTIFICATE_NAME_2 = "CACERT_HS2_12_1";
    private static final String CLIENT_CERTIFICATE_NAME = "USRCERT_HS2_12";
    private static final String CLIENT_PRIVATE_KEY_NAME = "USRPKEY_HS2_12";
    private static final String REMEDIATION_CA_CERTIFICATE_NAME = "CACERT_HS2_REMEDIATION_12";
    private static final String CA_CERTIFICATE_ALIAS = "HS2_12_0";
    private static final String CA_CERTIFICATE_ALIAS_2 = "HS2_12_1";
    private static final String CLIENT_CERTIFICATE_ALIAS = "HS2_12";
    private static final String CLIENT_PRIVATE_KEY_ALIAS = "HS2_12";
    private static final String REMEDIATION_CA_CERTIFICATE_ALIAS = "HS2_REMEDIATION_12";

    @Mock WifiKeyStore mKeyStore;
    @Mock SIMAccessor mSimAccessor;
    @Mock RoamingConsortium mRoamingConsortium;
    PasspointProvider mProvider;
    X509Certificate mRemediationCaCertificate;
    String mExpectedResult;

    @Parameterized.Parameters
    public static Collection rootCAConfigsForRemediation() {
        return Arrays.asList(
                new Object[][]{
                        {FakeKeys.CA_CERT0, REMEDIATION_CA_CERTIFICATE_ALIAS}, // For R2 config
                        {null, null}, // For R1 config
                });
    }

    public PasspointProviderTest(X509Certificate remediationCaCertificate, String expectedResult) {
        mRemediationCaCertificate = remediationCaCertificate;
        mExpectedResult = expectedResult;
    }

    /** Sets up test. */
    @Before
    public void setUp() throws Exception {
        initMocks(this);
        when(mRoamingConsortium.getRoamingConsortiums()).thenReturn(null);
    }

    /**
     * Helper function for creating a provider instance for testing.
     *
     * @param config The configuration associated with the provider
     * @return {@link com.android.server.wifi.hotspot2.PasspointProvider}
     */
    private PasspointProvider createProvider(PasspointConfiguration config) {
        return new PasspointProvider(config, mKeyStore, mSimAccessor, PROVIDER_ID, CREATOR_UID,
                CREATOR_PACKAGE);
    }

    /**
     * Verify that the configuration associated with the provider is the same or not the same
     * as the expected configuration.
     *
     * @param expectedConfig The expected configuration
     * @param equals Flag indicating equality or inequality check
     */
    private void verifyInstalledConfig(PasspointConfiguration expectedConfig, boolean equals) {
        PasspointConfiguration actualConfig = mProvider.getConfig();
        if (equals) {
            assertTrue(actualConfig.equals(expectedConfig));
        } else {
            assertFalse(actualConfig.equals(expectedConfig));
        }
    }

    /**
     * Helper function for creating a Domain Name ANQP element.
     *
     * @param domains List of domain names
     * @return {@link DomainNameElement}
     */
    private DomainNameElement createDomainNameElement(String[] domains) {
        return new DomainNameElement(Arrays.asList(domains));
    }

    /**
     * Helper function for creating a NAI Realm ANQP element.
     *
     * @param realm The realm of the network
     * @param eapMethodID EAP Method ID
     * @param authParam Authentication parameter
     * @return {@link NAIRealmElement}
     */
    private NAIRealmElement createNAIRealmElement(String realm, int eapMethodID,
            AuthParam authParam) {
        Map<Integer, Set<AuthParam>> authParamMap = new HashMap<>();
        if (authParam != null) {
            Set<AuthParam> authSet = new HashSet<>();
            authSet.add(authParam);
            authParamMap.put(authParam.getAuthTypeID(), authSet);
        }
        EAPMethod eapMethod = new EAPMethod(eapMethodID, authParamMap);
        NAIRealmData realmData = new NAIRealmData(Arrays.asList(new String[] {realm}),
                Arrays.asList(new EAPMethod[] {eapMethod}));
        return new NAIRealmElement(Arrays.asList(new NAIRealmData[] {realmData}));
    }

    /**
     * Helper function for creating a Roaming Consortium ANQP element.
     *
     * @param rcOIs Roaming consortium OIs
     * @return {@link RoamingConsortiumElement}
     */
    private RoamingConsortiumElement createRoamingConsortiumElement(Long[] rcOIs) {
        return new RoamingConsortiumElement(Arrays.asList(rcOIs));
    }

    /**
     * Helper function for creating a 3GPP Network ANQP element.
     *
     * @param imsiList List of IMSI to be included in a 3GPP Network
     * @return {@link ThreeGPPNetworkElement}
     */
    private ThreeGPPNetworkElement createThreeGPPNetworkElement(String[] imsiList) {
        CellularNetwork network = new CellularNetwork(Arrays.asList(imsiList));
        return new ThreeGPPNetworkElement(Arrays.asList(new CellularNetwork[] {network}));
    }

    /**
     * Verify that modification to the configuration used for creating PasspointProvider
     * will not change the configuration stored inside the PasspointProvider.
     *
     * @throws Exception
     */
    @Test
    public void verifyModifyOriginalConfig() throws Exception {
        // Create a dummy PasspointConfiguration.
        PasspointConfiguration config = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setFqdn("test1");
        config.setHomeSp(homeSp);
        Credential credential = new Credential();
        credential.setUserCredential(new Credential.UserCredential());
        config.setCredential(credential);
        mProvider = createProvider(config);
        verifyInstalledConfig(config, true);

        // Modify the original configuration, the configuration maintained by the provider
        // should be unchanged.
        config.getHomeSp().setFqdn("test2");
        verifyInstalledConfig(config, false);
    }

    /**
     * Verify that modification to the configuration retrieved from the PasspointProvider
     * will not change the configuration stored inside the PasspointProvider.
     *
     * @throws Exception
     */
    @Test
    public void verifyModifyRetrievedConfig() throws Exception {
        // Create a dummy PasspointConfiguration.
        PasspointConfiguration config = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setFqdn("test1");
        config.setHomeSp(homeSp);
        Credential credential = new Credential();
        credential.setUserCredential(new Credential.UserCredential());
        config.setCredential(credential);
        mProvider = createProvider(config);
        verifyInstalledConfig(config, true);

        // Modify the retrieved configuration, verify the configuration maintained by the
        // provider should be unchanged.
        PasspointConfiguration retrievedConfig = mProvider.getConfig();
        retrievedConfig.getHomeSp().setFqdn("test2");
        verifyInstalledConfig(retrievedConfig, false);
    }

    /**
     * Verify a successful installation of certificates and key.
     *
     * @throws Exception
     */
    @Test
    public void installCertsAndKeysSuccess() throws Exception {
        // Create a dummy configuration with certificate credential.
        PasspointConfiguration config = new PasspointConfiguration();
        Credential credential = new Credential();
        Credential.CertificateCredential certCredential = new Credential.CertificateCredential();
        certCredential.setCertSha256Fingerprint(
                MessageDigest.getInstance("SHA-256").digest(FakeKeys.CLIENT_CERT.getEncoded()));
        credential.setCertCredential(certCredential);
        credential.setCaCertificates(new X509Certificate[]{FakeKeys.CA_CERT0, FakeKeys.CA_CERT1});
        credential.setClientPrivateKey(FakeKeys.RSA_KEY1);
        credential.setClientCertificateChain(new X509Certificate[] {FakeKeys.CLIENT_CERT});
        config.setCredential(credential);
        if (mRemediationCaCertificate != null) {
            UpdateParameter updateParameter = new UpdateParameter();
            updateParameter.setCaCertificate(mRemediationCaCertificate);
            config.setSubscriptionUpdate(updateParameter);
        }
        mProvider = createProvider(config);

        // Install client certificate and key to the keystore successfully.
        when(mKeyStore.putCertInKeyStore(CA_CERTIFICATE_NAME, FakeKeys.CA_CERT0))
                .thenReturn(true);
        when(mKeyStore.putCertInKeyStore(CA_CERTIFICATE_NAME_2, FakeKeys.CA_CERT1))
                .thenReturn(true);
        when(mKeyStore.putKeyInKeyStore(CLIENT_PRIVATE_KEY_NAME, FakeKeys.RSA_KEY1))
                .thenReturn(true);
        when(mKeyStore.putCertInKeyStore(CLIENT_CERTIFICATE_NAME, FakeKeys.CLIENT_CERT))
                .thenReturn(true);
        when(mKeyStore.putCertInKeyStore(REMEDIATION_CA_CERTIFICATE_NAME, FakeKeys.CA_CERT0))
                .thenReturn(true);
        assertTrue(mProvider.installCertsAndKeys());

        // Verify client certificate and key in the configuration gets cleared and aliases
        // are set correctly.
        PasspointConfiguration curConfig = mProvider.getConfig();
        assertTrue(curConfig.getCredential().getCaCertificates() == null);
        assertTrue(curConfig.getCredential().getClientPrivateKey() == null);
        assertTrue(curConfig.getCredential().getClientCertificateChain() == null);
        if (mRemediationCaCertificate != null) {
            assertTrue(curConfig.getSubscriptionUpdate().getCaCertificate() == null);
        }
        assertTrue(mProvider.getCaCertificateAliases().equals(
                Arrays.asList(CA_CERTIFICATE_ALIAS, CA_CERTIFICATE_ALIAS_2)));
        assertTrue(mProvider.getClientPrivateKeyAlias().equals(CLIENT_PRIVATE_KEY_ALIAS));
        assertTrue(mProvider.getClientCertificateAlias().equals(CLIENT_CERTIFICATE_ALIAS));
        assertTrue(TextUtils.equals(mProvider.getRemediationCaCertificateAlias(), mExpectedResult));
    }

    /**
     * Verify a failure installation of certificates and key.
     */
    @Test
    public void installCertsAndKeysFailure() throws Exception {
        // Create a dummy configuration with certificate credential.
        PasspointConfiguration config = new PasspointConfiguration();
        Credential credential = new Credential();
        Credential.CertificateCredential certCredential = new Credential.CertificateCredential();
        certCredential.setCertSha256Fingerprint(
                MessageDigest.getInstance("SHA-256").digest(FakeKeys.CLIENT_CERT.getEncoded()));
        credential.setCertCredential(certCredential);
        credential.setCaCertificates(new X509Certificate[]{FakeKeys.CA_CERT0, FakeKeys.CA_CERT1});
        credential.setClientPrivateKey(FakeKeys.RSA_KEY1);
        credential.setClientCertificateChain(new X509Certificate[] {FakeKeys.CLIENT_CERT});
        config.setCredential(credential);

        UpdateParameter updateParameter = new UpdateParameter();
        updateParameter.setCaCertificate(mRemediationCaCertificate);
        config.setSubscriptionUpdate(updateParameter);
        mProvider = createProvider(config);

        // Failed to install client certificate to the keystore.
        when(mKeyStore.putCertInKeyStore(CA_CERTIFICATE_NAME, FakeKeys.CA_CERT0))
                .thenReturn(true);
        when(mKeyStore.putCertInKeyStore(CA_CERTIFICATE_NAME_2, FakeKeys.CA_CERT1))
                .thenReturn(false);
        when(mKeyStore.putKeyInKeyStore(CLIENT_PRIVATE_KEY_NAME, FakeKeys.RSA_KEY1))
                .thenReturn(true);
        when(mKeyStore.putCertInKeyStore(CLIENT_CERTIFICATE_NAME, FakeKeys.CLIENT_CERT))
                .thenReturn(true);
        when(mKeyStore.putCertInKeyStore(REMEDIATION_CA_CERTIFICATE_NAME, FakeKeys.CA_CERT0))
                .thenReturn(true);
        assertFalse(mProvider.installCertsAndKeys());

        // Verify certificates and key in the configuration are not cleared and aliases
        // are not set.
        PasspointConfiguration curConfig = mProvider.getConfig();
        assertTrue(curConfig.getCredential().getCaCertificates() != null);
        assertTrue(curConfig.getCredential().getClientCertificateChain() != null);
        assertTrue(curConfig.getCredential().getClientPrivateKey() != null);
        if (mRemediationCaCertificate != null) {
            assertTrue(curConfig.getSubscriptionUpdate().getCaCertificate() != null);
        }
        assertTrue(mProvider.getCaCertificateAliases() == null);
        assertTrue(mProvider.getClientPrivateKeyAlias() == null);
        assertTrue(mProvider.getClientCertificateAlias() == null);
        assertTrue(mProvider.getRemediationCaCertificateAlias() == null);
    }

    /**
     * Verify a successful uninstallation of certificates and key.
     */
    @Test
    public void uninstallCertsAndKeys() throws Exception {
        // Create a dummy configuration with certificate credential.
        PasspointConfiguration config = new PasspointConfiguration();
        Credential credential = new Credential();
        Credential.CertificateCredential certCredential = new Credential.CertificateCredential();
        certCredential.setCertSha256Fingerprint(
                MessageDigest.getInstance("SHA-256").digest(FakeKeys.CLIENT_CERT.getEncoded()));
        credential.setCertCredential(certCredential);
        credential.setCaCertificates(new X509Certificate[]{FakeKeys.CA_CERT0, FakeKeys.CA_CERT1});
        credential.setClientPrivateKey(FakeKeys.RSA_KEY1);
        credential.setClientCertificateChain(new X509Certificate[] {FakeKeys.CLIENT_CERT});
        config.setCredential(credential);
        if (mRemediationCaCertificate != null) {
            UpdateParameter updateParameter = new UpdateParameter();
            updateParameter.setCaCertificate(FakeKeys.CA_CERT0);
            config.setSubscriptionUpdate(updateParameter);
        }
        mProvider = createProvider(config);

        // Install client certificate and key to the keystore successfully.
        when(mKeyStore.putCertInKeyStore(CA_CERTIFICATE_NAME, FakeKeys.CA_CERT0))
                .thenReturn(true);
        when(mKeyStore.putCertInKeyStore(CA_CERTIFICATE_NAME_2, FakeKeys.CA_CERT1))
                .thenReturn(true);
        when(mKeyStore.putKeyInKeyStore(CLIENT_PRIVATE_KEY_NAME, FakeKeys.RSA_KEY1))
                .thenReturn(true);
        when(mKeyStore.putCertInKeyStore(CLIENT_CERTIFICATE_NAME, FakeKeys.CLIENT_CERT))
                .thenReturn(true);
        when(mKeyStore.putCertInKeyStore(REMEDIATION_CA_CERTIFICATE_NAME, FakeKeys.CA_CERT0))
                .thenReturn(true);
        assertTrue(mProvider.installCertsAndKeys());
        assertTrue(mProvider.getCaCertificateAliases().equals(
                Arrays.asList(CA_CERTIFICATE_ALIAS, CA_CERTIFICATE_ALIAS_2)));
        assertTrue(mProvider.getClientPrivateKeyAlias().equals(CLIENT_PRIVATE_KEY_ALIAS));
        assertTrue(mProvider.getClientCertificateAlias().equals(CLIENT_CERTIFICATE_ALIAS));
        assertTrue(TextUtils.equals(mProvider.getRemediationCaCertificateAlias(), mExpectedResult));

        // Uninstall certificates and key from the keystore.
        mProvider.uninstallCertsAndKeys();
        verify(mKeyStore).removeEntryFromKeyStore(CA_CERTIFICATE_NAME);
        verify(mKeyStore).removeEntryFromKeyStore(CA_CERTIFICATE_NAME_2);
        verify(mKeyStore).removeEntryFromKeyStore(CLIENT_CERTIFICATE_NAME);
        verify(mKeyStore).removeEntryFromKeyStore(CLIENT_PRIVATE_KEY_NAME);
        if (mRemediationCaCertificate != null) {
            verify(mKeyStore).removeEntryFromKeyStore(REMEDIATION_CA_CERTIFICATE_NAME);
        }

        assertTrue(mProvider.getCaCertificateAliases() == null);
        assertTrue(mProvider.getClientPrivateKeyAlias() == null);
        assertTrue(mProvider.getClientCertificateAlias() == null);
        assertTrue(mProvider.getRemediationCaCertificateAlias() == null);
    }

    /**
     * Verify that a provider is a home provider when its FQDN matches a domain name in the
     * Domain Name ANQP element and no NAI realm is provided.
     *
     * @throws Exception
     */
    @Test
    public void matchFQDNWithoutNAIRealm() throws Exception {
        String testDomain = "test.com";

        // Setup test provider.
        PasspointConfiguration config = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setFqdn(testDomain);
        config.setHomeSp(homeSp);
        Credential credential = new Credential();
        Credential.UserCredential userCredential = new Credential.UserCredential();
        userCredential.setNonEapInnerMethod(Credential.UserCredential.AUTH_METHOD_MSCHAPV2);
        credential.setUserCredential(userCredential);
        config.setCredential(credential);
        mProvider = createProvider(config);

        // Setup ANQP elements.
        Map<ANQPElementType, ANQPElement> anqpElementMap = new HashMap<>();
        anqpElementMap.put(ANQPElementType.ANQPDomName,
                createDomainNameElement(new String[] {testDomain}));

        assertEquals(PasspointMatch.HomeProvider,
            mProvider.match(anqpElementMap, mRoamingConsortium));
    }

    /**
     * Verify that a provider is a home provider when its FQDN matches a domain name in the
     * Domain Name ANQP element and the provider's credential matches the NAI realm provided.
     *
     * @throws Exception
     */
    @Test
    public void matchFQDNWithNAIRealmMatch() throws Exception {
        String testDomain = "test.com";
        String testRealm = "realm.com";

        // Setup test provider.
        PasspointConfiguration config = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setFqdn(testDomain);
        config.setHomeSp(homeSp);
        Credential credential = new Credential();
        credential.setRealm(testRealm);
        Credential.UserCredential userCredential = new Credential.UserCredential();
        userCredential.setNonEapInnerMethod(Credential.UserCredential.AUTH_METHOD_MSCHAPV2);
        credential.setUserCredential(userCredential);
        config.setCredential(credential);
        mProvider = createProvider(config);

        // Setup Domain Name ANQP element.
        Map<ANQPElementType, ANQPElement> anqpElementMap = new HashMap<>();
        anqpElementMap.put(ANQPElementType.ANQPDomName,
                createDomainNameElement(new String[] {testDomain}));
        anqpElementMap.put(ANQPElementType.ANQPNAIRealm,
                createNAIRealmElement(testRealm, EAPConstants.EAP_TTLS,
                        new NonEAPInnerAuth(NonEAPInnerAuth.AUTH_TYPE_MSCHAPV2)));

        assertEquals(PasspointMatch.HomeProvider,
            mProvider.match(anqpElementMap, mRoamingConsortium));
    }

    /**
     * Verify that there is no match when the provider's FQDN matches a domain name in the
     * Domain Name ANQP element but the provider's credential doesn't match the authentication
     * method provided in the NAI realm.
     *
     * @throws Exception
     */
    @Test
    public void matchFQDNWithNAIRealmMismatch() throws Exception {
        String testDomain = "test.com";
        String testRealm = "realm.com";

        // Setup test provider.
        PasspointConfiguration config = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setFqdn(testDomain);
        config.setHomeSp(homeSp);
        Credential credential = new Credential();
        credential.setRealm(testRealm);
        Credential.UserCredential userCredential = new Credential.UserCredential();
        userCredential.setNonEapInnerMethod(Credential.UserCredential.AUTH_METHOD_MSCHAPV2);
        credential.setUserCredential(userCredential);
        config.setCredential(credential);
        mProvider = createProvider(config);

        // Setup Domain Name ANQP element.
        Map<ANQPElementType, ANQPElement> anqpElementMap = new HashMap<>();
        anqpElementMap.put(ANQPElementType.ANQPDomName,
                createDomainNameElement(new String[] {testDomain}));
        anqpElementMap.put(ANQPElementType.ANQPNAIRealm,
                createNAIRealmElement(testRealm, EAPConstants.EAP_TLS, null));

        assertEquals(PasspointMatch.None, mProvider.match(anqpElementMap, mRoamingConsortium));
    }

    /**
     * Verify that a provider is a home provider when its SIM credential matches an 3GPP network
     * domain name in the Domain Name ANQP element.
     *
     * @throws Exception
     */
    @Test
    public void matchFQDNWith3GPPNetworkDomainName() throws Exception {
        String testImsi = "1234567890";

        // Setup test provider.
        PasspointConfiguration config = new PasspointConfiguration();
        config.setHomeSp(new HomeSp());
        Credential credential = new Credential();
        Credential.SimCredential simCredential = new Credential.SimCredential();
        simCredential.setImsi(testImsi);
        credential.setSimCredential(simCredential);
        config.setCredential(credential);
        when(mSimAccessor.getMatchingImsis(new IMSIParameter(testImsi, false)))
                .thenReturn(Arrays.asList(new String[] {testImsi}));
        mProvider = createProvider(config);

        // Setup Domain Name ANQP element.
        Map<ANQPElementType, ANQPElement> anqpElementMap = new HashMap<>();
        anqpElementMap.put(ANQPElementType.ANQPDomName,
                createDomainNameElement(new String[] {"wlan.mnc456.mcc123.3gppnetwork.org"}));

        assertEquals(PasspointMatch.HomeProvider,
            mProvider.match(anqpElementMap, mRoamingConsortium));
    }

    /**
     * Verify that a provider is a home provider when its FQDN, roaming consortium OI, and
     * IMSI all matched against the ANQP elements, since we prefer matching home provider over
     * roaming provider.
     *
     * @throws Exception
     */
    @Test
    public void matchFQDNOverRoamingProvider() throws Exception {
        // Setup test data.
        String testDomain = "test.com";
        String testImsi = "1234567890";
        long[] providerRCOIs = new long[] {0x1234L, 0x2345L};
        Long[] anqpRCOIs = new Long[] {0x1234L, 0x2133L};

        // Setup test provider.
        PasspointConfiguration config = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setFqdn(testDomain);
        homeSp.setRoamingConsortiumOis(providerRCOIs);
        config.setHomeSp(homeSp);
        Credential credential = new Credential();
        Credential.SimCredential simCredential = new Credential.SimCredential();
        simCredential.setImsi(testImsi);
        credential.setSimCredential(simCredential);
        config.setCredential(credential);
        when(mSimAccessor.getMatchingImsis(new IMSIParameter(testImsi, false)))
                .thenReturn(Arrays.asList(new String[] {testImsi}));
        mProvider = createProvider(config);

        // Setup ANQP elements.
        Map<ANQPElementType, ANQPElement> anqpElementMap = new HashMap<>();
        anqpElementMap.put(ANQPElementType.ANQPDomName,
                createDomainNameElement(new String[] {testDomain}));
        anqpElementMap.put(ANQPElementType.ANQPRoamingConsortium,
                createRoamingConsortiumElement(anqpRCOIs));
        anqpElementMap.put(ANQPElementType.ANQP3GPPNetwork,
                createThreeGPPNetworkElement(new String[] {"123456"}));

        assertEquals(PasspointMatch.HomeProvider,
                mProvider.match(anqpElementMap, mRoamingConsortium));
    }

    /**
     * Verify that a provider is a roaming provider when a roaming consortium OI matches an OI
     * in the roaming consortium ANQP element and no NAI realm is provided.
     *
     * @throws Exception
     */
    @Test
    public void matchRoamingConsortiumWithoutNAIRealm() throws Exception {
        long[] providerRCOIs = new long[] {0x1234L, 0x2345L};
        Long[] anqpRCOIs = new Long[] {0x1234L, 0x2133L};

        // Setup test provider.
        PasspointConfiguration config = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setRoamingConsortiumOis(providerRCOIs);
        config.setHomeSp(homeSp);
        Credential credential = new Credential();
        Credential.UserCredential userCredential = new Credential.UserCredential();
        userCredential.setNonEapInnerMethod(Credential.UserCredential.AUTH_METHOD_MSCHAPV2);
        credential.setUserCredential(userCredential);
        config.setCredential(credential);
        mProvider = createProvider(config);

        // Setup Roaming Consortium ANQP element.
        Map<ANQPElementType, ANQPElement> anqpElementMap = new HashMap<>();
        anqpElementMap.put(ANQPElementType.ANQPRoamingConsortium,
                createRoamingConsortiumElement(anqpRCOIs));

        assertEquals(PasspointMatch.RoamingProvider,
            mProvider.match(anqpElementMap, mRoamingConsortium));
    }

    /**
     * Verify that a provider is a roaming provider when a roaming consortium OI matches an OI
     * in the roaming consortium ANQP element and the provider's credential matches the
     * NAI realm provided.
     *
     * @throws Exception
     */
    @Test
    public void matchRoamingConsortiumWithNAIRealmMatch() throws Exception {
        long[] providerRCOIs = new long[] {0x1234L, 0x2345L};
        Long[] anqpRCOIs = new Long[] {0x1234L, 0x2133L};
        String testRealm = "realm.com";

        // Setup test provider.
        PasspointConfiguration config = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setRoamingConsortiumOis(providerRCOIs);
        config.setHomeSp(homeSp);
        Credential credential = new Credential();
        credential.setRealm(testRealm);
        Credential.UserCredential userCredential = new Credential.UserCredential();
        userCredential.setNonEapInnerMethod(Credential.UserCredential.AUTH_METHOD_MSCHAPV2);
        credential.setUserCredential(userCredential);
        config.setCredential(credential);
        mProvider = createProvider(config);

        // Setup Roaming Consortium ANQP element.
        Map<ANQPElementType, ANQPElement> anqpElementMap = new HashMap<>();
        anqpElementMap.put(ANQPElementType.ANQPRoamingConsortium,
                createRoamingConsortiumElement(anqpRCOIs));
        anqpElementMap.put(ANQPElementType.ANQPNAIRealm,
                createNAIRealmElement(testRealm, EAPConstants.EAP_TTLS,
                        new NonEAPInnerAuth(NonEAPInnerAuth.AUTH_TYPE_MSCHAPV2)));

        assertEquals(PasspointMatch.RoamingProvider,
                mProvider.match(anqpElementMap, mRoamingConsortium));
    }

    /**
     * Verify that there is no match when a roaming consortium OI matches an OI
     * in the roaming consortium ANQP element and but NAI realm is not matched.
     *
     * @throws Exception
     */
    @Test
    public void matchRoamingConsortiumWithNAIRealmMisMatch() throws Exception {
        long[] providerRCOIs = new long[] {0x1234L, 0x2345L};
        Long[] anqpRCOIs = new Long[] {0x1234L, 0x2133L};
        String testRealm = "realm.com";

        // Setup test provider.
        PasspointConfiguration config = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setRoamingConsortiumOis(providerRCOIs);
        config.setHomeSp(homeSp);
        Credential credential = new Credential();
        credential.setRealm(testRealm);
        Credential.UserCredential userCredential = new Credential.UserCredential();
        userCredential.setNonEapInnerMethod(Credential.UserCredential.AUTH_METHOD_MSCHAPV2);
        credential.setUserCredential(userCredential);
        config.setCredential(credential);
        mProvider = createProvider(config);

        // Setup Roaming Consortium ANQP element.
        Map<ANQPElementType, ANQPElement> anqpElementMap = new HashMap<>();
        anqpElementMap.put(ANQPElementType.ANQPRoamingConsortium,
                createRoamingConsortiumElement(anqpRCOIs));
        // Set up NAI with different EAP method
        anqpElementMap.put(ANQPElementType.ANQPNAIRealm,
                createNAIRealmElement(testRealm, EAPConstants.EAP_TLS, null));

        assertEquals(PasspointMatch.None,
                mProvider.match(anqpElementMap, mRoamingConsortium));
    }

    /**
     * Verify that a provider is a roaming provider when a roaming consortium OI matches an OI
     * in the roaming consortium information element and no NAI realm is provided.
     *
     * @throws Exception
     */
    @Test
    public void matchRoamingConsortiumIeWithoutNAIRealm() throws Exception {
        long[] providerRCOIs = new long[] {0x1234L, 0x2345L};
        long[] ieRCOIs = new long[] {0x1234L, 0x2133L};

        // Setup test provider.
        PasspointConfiguration config = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setRoamingConsortiumOis(providerRCOIs);
        config.setHomeSp(homeSp);
        Credential credential = new Credential();
        Credential.UserCredential userCredential = new Credential.UserCredential();
        userCredential.setNonEapInnerMethod(Credential.UserCredential.AUTH_METHOD_MSCHAPV2);
        credential.setUserCredential(userCredential);
        config.setCredential(credential);
        mProvider = createProvider(config);

        // Setup Roaming Consortium ANQP element.
        Map<ANQPElementType, ANQPElement> anqpElementMap = new HashMap<>();

        // Setup Roaming Consortium Information element.
        when(mRoamingConsortium.getRoamingConsortiums()).thenReturn(ieRCOIs);

        assertEquals(PasspointMatch.RoamingProvider,
            mProvider.match(anqpElementMap, mRoamingConsortium));
    }

    /**
     * Verify that a provider is a roaming provider when a roaming consortium OI matches an OI
     * in the roaming consortium information element and the provider's credential matches the
     * NAI realm provided.
     *
     * @throws Exception
     */
    @Test
    public void matchRoamingConsortiumIeWithNAIRealmMatch() throws Exception {
        long[] providerRCOIs = new long[] {0x1234L, 0x2345L};
        long[] ieRCOIs = new long[] {0x1234L, 0x2133L};
        String testRealm = "realm.com";

        // Setup test provider.
        PasspointConfiguration config = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setRoamingConsortiumOis(providerRCOIs);
        config.setHomeSp(homeSp);
        Credential credential = new Credential();
        credential.setRealm(testRealm);
        Credential.UserCredential userCredential = new Credential.UserCredential();
        userCredential.setNonEapInnerMethod(Credential.UserCredential.AUTH_METHOD_MSCHAPV2);
        credential.setUserCredential(userCredential);
        config.setCredential(credential);
        mProvider = createProvider(config);

        // Setup Roaming Consortium ANQP element.
        Map<ANQPElementType, ANQPElement> anqpElementMap = new HashMap<>();

        // Setup Roaming Consortium Information element.
        when(mRoamingConsortium.getRoamingConsortiums()).thenReturn(ieRCOIs);
        anqpElementMap.put(ANQPElementType.ANQPNAIRealm,
                createNAIRealmElement(testRealm, EAPConstants.EAP_TTLS,
                        new NonEAPInnerAuth(NonEAPInnerAuth.AUTH_TYPE_MSCHAPV2)));

        assertEquals(PasspointMatch.RoamingProvider,
                mProvider.match(anqpElementMap, mRoamingConsortium));
    }

    /**
     * Verify that there is no match when a roaming consortium OI matches an OI
     * in the roaming consortium information element, but NAI realm is not matched.
     *
     * @throws Exception
     */
    @Test
    public void matchRoamingConsortiumIeWithNAIRealmMismatch() throws Exception {
        long[] providerRCOIs = new long[] {0x1234L, 0x2345L};
        long[] ieRCOIs = new long[] {0x1234L, 0x2133L};
        String testRealm = "realm.com";

        // Setup test provider.
        PasspointConfiguration config = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setRoamingConsortiumOis(providerRCOIs);
        config.setHomeSp(homeSp);
        Credential credential = new Credential();
        credential.setRealm(testRealm);
        Credential.UserCredential userCredential = new Credential.UserCredential();
        userCredential.setNonEapInnerMethod(Credential.UserCredential.AUTH_METHOD_MSCHAPV2);
        credential.setUserCredential(userCredential);
        config.setCredential(credential);
        mProvider = createProvider(config);

        // Setup Roaming Consortium ANQP element.
        Map<ANQPElementType, ANQPElement> anqpElementMap = new HashMap<>();

        // Setup Roaming Consortium Information element.
        when(mRoamingConsortium.getRoamingConsortiums()).thenReturn(ieRCOIs);
        // Set up NAI with different EAP method
        anqpElementMap.put(ANQPElementType.ANQPNAIRealm,
                createNAIRealmElement(testRealm, EAPConstants.EAP_TLS, null));

        assertEquals(PasspointMatch.None,
                mProvider.match(anqpElementMap, mRoamingConsortium));
    }

    /**
     * Verify that none of matched providers are found when a roaming consortium OI doesn't
     * matches an OI in the roaming consortium information element and
     * none of NAI realms match each other.
     *
     * @throws Exception
     */
    @Test
    public void misMatchForRoamingConsortiumIeAndNAIRealm() throws Exception {
        long[] providerRCOIs = new long[] {0x1234L, 0x2345L};
        long[] ieRCOIs = new long[] {0x2255L, 0x2133L};

        // Setup test provider.
        PasspointConfiguration config = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setRoamingConsortiumOis(providerRCOIs);
        config.setHomeSp(homeSp);
        Credential credential = new Credential();
        Credential.UserCredential userCredential = new Credential.UserCredential();
        userCredential.setNonEapInnerMethod(Credential.UserCredential.AUTH_METHOD_MSCHAPV2);
        credential.setUserCredential(userCredential);
        config.setCredential(credential);
        mProvider = createProvider(config);

        // Setup Roaming Consortium ANQP element.
        Map<ANQPElementType, ANQPElement> anqpElementMap = new HashMap<>();

        // Setup Roaming Consortium Information element.
        when(mRoamingConsortium.getRoamingConsortiums()).thenReturn(ieRCOIs);

        assertEquals(PasspointMatch.None,
            mProvider.match(anqpElementMap, mRoamingConsortium));
    }

    /**
     * Verify that a provider is a roaming provider when the provider's IMSI parameter and an IMSI
     * from the SIM card matches a MCC-MNC in the 3GPP Network ANQP element regardless of NAI realm
     * mismatch.
     *
     * @throws Exception
     */
    @Test
    public void matchThreeGPPNetworkWithNAIRealmMismatch() throws Exception {
        String testImsi = "1234567890";
        String testRealm = "realm.com";
        // Setup test provider.
        PasspointConfiguration config = new PasspointConfiguration();
        config.setHomeSp(new HomeSp());
        Credential credential = new Credential();
        Credential.SimCredential simCredential = new Credential.SimCredential();
        simCredential.setImsi(testImsi);
        credential.setSimCredential(simCredential);
        config.setCredential(credential);
        when(mSimAccessor.getMatchingImsis(new IMSIParameter(testImsi, false)))
                .thenReturn(Arrays.asList(new String[] {testImsi}));
        mProvider = createProvider(config);

        // Setup 3GPP Network ANQP element.
        Map<ANQPElementType, ANQPElement> anqpElementMap = new HashMap<>();
        anqpElementMap.put(ANQPElementType.ANQP3GPPNetwork,
                createThreeGPPNetworkElement(new String[] {"123456"}));

        // Setup NAI Realm ANQP element with different realm.
        anqpElementMap.put(ANQPElementType.ANQPNAIRealm,
                createNAIRealmElement(testRealm, EAPConstants.EAP_TTLS,
                new NonEAPInnerAuth(NonEAPInnerAuth.AUTH_TYPE_MSCHAPV2)));

        assertEquals(PasspointMatch.RoamingProvider,
            mProvider.match(anqpElementMap, mRoamingConsortium));
    }

    /**
     * Verify that a provider is a roaming provider when the provider's IMSI parameter and an IMSI
     * from the SIM card matches a MCC-MNC in the 3GPP Network ANQP element regardless of NAI realm
     * match.
     *
     * @throws Exception
     */
    @Test
    public void matchThreeGPPNetworkWithNAIRealmMatch() throws Exception {
        String testImsi = "1234567890";
        String testRealm = "realm.com";
        // Setup test provider.
        PasspointConfiguration config = new PasspointConfiguration();
        config.setHomeSp(new HomeSp());
        Credential credential = new Credential();
        Credential.SimCredential simCredential = new Credential.SimCredential();
        simCredential.setImsi(testImsi);
        credential.setSimCredential(simCredential);
        config.setCredential(credential);
        credential.setRealm(testRealm);
        when(mSimAccessor.getMatchingImsis(new IMSIParameter(testImsi, false)))
                .thenReturn(Arrays.asList(new String[] {testImsi}));
        mProvider = createProvider(config);

        // Setup 3GPP Network ANQP element.
        Map<ANQPElementType, ANQPElement> anqpElementMap = new HashMap<>();
        anqpElementMap.put(ANQPElementType.ANQP3GPPNetwork,
                createThreeGPPNetworkElement(new String[] {"123456"}));

        // Setup NAI Realm ANQP element with same realm.
        anqpElementMap.put(ANQPElementType.ANQPNAIRealm,
                createNAIRealmElement(testRealm, EAPConstants.EAP_AKA, null));

        assertEquals(PasspointMatch.RoamingProvider,
                mProvider.match(anqpElementMap, mRoamingConsortium));
    }

    /**
     * Verify that a provider is a roaming provider when its credential only matches a NAI realm in
     * the NAI Realm ANQP element and not match for Domain Name, RoamingConsortium and 3GPP.
     *
     * @throws Exception
     */
    @Test
    public void matchOnlyNAIRealmWithOtherInformationMismatch() throws Exception {
        String testRealm = "realm.com";

        // Setup test provider.
        PasspointConfiguration config = new PasspointConfiguration();
        config.setHomeSp(new HomeSp());
        Credential credential = new Credential();
        credential.setRealm(testRealm);
        Credential.UserCredential userCredential = new Credential.UserCredential();
        userCredential.setNonEapInnerMethod(Credential.UserCredential.AUTH_METHOD_MSCHAPV2);
        credential.setUserCredential(userCredential);
        config.setCredential(credential);
        mProvider = createProvider(config);

        // Setup NAI Realm ANQP element.
        Map<ANQPElementType, ANQPElement> anqpElementMap = new HashMap<>();
        anqpElementMap.put(ANQPElementType.ANQPNAIRealm,
                createNAIRealmElement(testRealm, EAPConstants.EAP_TTLS,
                        new NonEAPInnerAuth(NonEAPInnerAuth.AUTH_TYPE_MSCHAPV2)));

        assertEquals(PasspointMatch.RoamingProvider,
            mProvider.match(anqpElementMap, mRoamingConsortium));
    }


    /**
     * Verify that an expected WifiConfiguration will be returned for a Passpoint provider
     * with an user credential.
     *
     * @throws Exception
     */
    @Test
    public void getWifiConfigWithUserCredential() throws Exception {
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
        BitSet allowedProtocols = new BitSet();
        allowedProtocols.set(WifiConfiguration.Protocol.RSN);

        // Create provider for R2.
        PasspointConfiguration config = new PasspointConfiguration();
        config.setUpdateIdentifier(1234);
        config.setUsageLimitDataLimit(100);
        HomeSp homeSp = new HomeSp();
        homeSp.setFqdn(fqdn);
        homeSp.setFriendlyName(friendlyName);
        homeSp.setRoamingConsortiumOis(rcOIs);
        config.setHomeSp(homeSp);
        Credential credential = new Credential();
        credential.setRealm(realm);
        Credential.UserCredential userCredential = new Credential.UserCredential();
        userCredential.setUsername(username);
        userCredential.setPassword(encodedPasswordStr);
        userCredential.setNonEapInnerMethod(Credential.UserCredential.AUTH_METHOD_MSCHAPV2);
        credential.setUserCredential(userCredential);
        credential.setCaCertificates(new X509Certificate[] {FakeKeys.CA_CERT0});
        config.setCredential(credential);
        mProvider = createProvider(config);

        // Install certificate.
        when(mKeyStore.putCertInKeyStore(CA_CERTIFICATE_NAME, FakeKeys.CA_CERT0))
                .thenReturn(true);
        assertTrue(mProvider.installCertsAndKeys());

        // Retrieve the WifiConfiguration associated with the provider, and verify the content of
        // the configuration.  Need to verify field by field since WifiConfiguration doesn't
        // override equals() function.
        WifiConfiguration wifiConfig = mProvider.getWifiConfig();
        WifiEnterpriseConfig wifiEnterpriseConfig = wifiConfig.enterpriseConfig;
        assertEquals(fqdn, wifiConfig.FQDN);
        assertEquals(friendlyName, wifiConfig.providerFriendlyName);
        assertTrue(Arrays.equals(rcOIs, wifiConfig.roamingConsortiumIds));
        assertTrue(wifiConfig.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.WPA_EAP));
        assertTrue(wifiConfig.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.IEEE8021X));
        assertEquals(wifiConfig.updateIdentifier, Integer.toString(config.getUpdateIdentifier()));
        assertEquals(allowedProtocols, wifiConfig.allowedProtocols);
        assertEquals("1234", wifiConfig.updateIdentifier);
        assertFalse(wifiConfig.shared);
        assertEquals(realm, wifiEnterpriseConfig.getRealm());
        assertEquals(fqdn, wifiEnterpriseConfig.getDomainSuffixMatch());
        assertEquals("anonymous@" + realm, wifiEnterpriseConfig.getAnonymousIdentity());
        assertEquals(WifiEnterpriseConfig.Eap.TTLS, wifiEnterpriseConfig.getEapMethod());
        assertEquals(WifiEnterpriseConfig.Phase2.MSCHAPV2, wifiEnterpriseConfig.getPhase2Method());
        assertEquals(username, wifiEnterpriseConfig.getIdentity());
        assertEquals(password, wifiEnterpriseConfig.getPassword());
        assertEquals(WifiConfiguration.METERED_OVERRIDE_METERED, wifiConfig.meteredOverride);
        assertEquals(CA_CERTIFICATE_ALIAS, wifiEnterpriseConfig.getCaCertificateAlias());
    }

    /**
     * Verify that an expected WifiConfiguration will be returned for a Passpoint provider
     * with a certificate credential.
     *
     * @throws Exception
     */
    @Test
    public void getWifiConfigWithCertCredential() throws Exception {
        // Test data.
        String fqdn = "test.com";
        String friendlyName = "Friendly Name";
        long[] rcOIs = new long[] {0x1234L, 0x2345L};
        String realm = "realm.com";
        BitSet allowedProtocols = new BitSet();
        allowedProtocols.set(WifiConfiguration.Protocol.RSN);

        // Create provider.
        PasspointConfiguration config = new PasspointConfiguration();
        config.setUpdateIdentifier(1234);
        config.setUsageLimitTimeLimitInMinutes(100);
        HomeSp homeSp = new HomeSp();
        homeSp.setFqdn(fqdn);
        homeSp.setFriendlyName(friendlyName);
        homeSp.setRoamingConsortiumOis(rcOIs);
        config.setHomeSp(homeSp);
        Credential credential = new Credential();
        credential.setRealm(realm);
        Credential.CertificateCredential certCredential = new Credential.CertificateCredential();
        certCredential.setCertSha256Fingerprint(
                MessageDigest.getInstance("SHA-256").digest(FakeKeys.CLIENT_CERT.getEncoded()));
        credential.setCertCredential(certCredential);
        credential.setCaCertificates(new X509Certificate[] {FakeKeys.CA_CERT0});
        credential.setClientPrivateKey(FakeKeys.RSA_KEY1);
        credential.setClientCertificateChain(new X509Certificate[] {FakeKeys.CLIENT_CERT});
        config.setCredential(credential);
        mProvider = createProvider(config);

        // Install certificate.
        when(mKeyStore.putCertInKeyStore(CA_CERTIFICATE_NAME, FakeKeys.CA_CERT0))
                .thenReturn(true);
        when(mKeyStore.putKeyInKeyStore(CLIENT_PRIVATE_KEY_NAME, FakeKeys.RSA_KEY1))
                .thenReturn(true);
        when(mKeyStore.putCertInKeyStore(CLIENT_CERTIFICATE_NAME, FakeKeys.CLIENT_CERT))
                .thenReturn(true);
        assertTrue(mProvider.installCertsAndKeys());

        // Retrieve the WifiConfiguration associated with the provider, and verify the content of
        // the configuration.  Need to verify field by field since WifiConfiguration doesn't
        // override equals() function.
        WifiConfiguration wifiConfig = mProvider.getWifiConfig();
        WifiEnterpriseConfig wifiEnterpriseConfig = wifiConfig.enterpriseConfig;
        assertEquals(fqdn, wifiConfig.FQDN);
        assertEquals(friendlyName, wifiConfig.providerFriendlyName);
        assertTrue(Arrays.equals(rcOIs, wifiConfig.roamingConsortiumIds));
        assertTrue(wifiConfig.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.WPA_EAP));
        assertTrue(wifiConfig.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.IEEE8021X));
        assertEquals(allowedProtocols, wifiConfig.allowedProtocols);
        assertEquals("1234", wifiConfig.updateIdentifier);
        assertFalse(wifiConfig.shared);
        assertEquals(realm, wifiEnterpriseConfig.getRealm());
        assertEquals(fqdn, wifiEnterpriseConfig.getDomainSuffixMatch());
        assertEquals("anonymous@" + realm, wifiEnterpriseConfig.getAnonymousIdentity());
        assertEquals(WifiEnterpriseConfig.Eap.TLS, wifiEnterpriseConfig.getEapMethod());
        assertEquals(CLIENT_CERTIFICATE_ALIAS, wifiEnterpriseConfig.getClientCertificateAlias());
        assertEquals(CA_CERTIFICATE_ALIAS, wifiEnterpriseConfig.getCaCertificateAlias());
        assertEquals(WifiConfiguration.METERED_OVERRIDE_METERED, wifiConfig.meteredOverride);
    }

    /**
     * Verify that an expected WifiConfiguration will be returned for a Passpoint provider
     * with a SIM credential.
     *
     * @throws Exception
     */
    @Test
    public void getWifiConfigWithSimCredential() throws Exception {
        // Test data.
        String fqdn = "test.com";
        String friendlyName = "Friendly Name";
        long[] rcOIs = new long[] {0x1234L, 0x2345L};
        String realm = "realm.com";
        String imsi = "1234*";
        BitSet allowedProtocols = new BitSet();
        allowedProtocols.set(WifiConfiguration.Protocol.RSN);

        // Create provider.
        PasspointConfiguration config = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setFqdn(fqdn);
        homeSp.setFriendlyName(friendlyName);
        homeSp.setRoamingConsortiumOis(rcOIs);
        config.setHomeSp(homeSp);
        Credential credential = new Credential();
        credential.setRealm(realm);
        Credential.SimCredential simCredential = new Credential.SimCredential();
        simCredential.setImsi(imsi);
        simCredential.setEapType(EAPConstants.EAP_SIM);
        credential.setSimCredential(simCredential);
        config.setCredential(credential);
        mProvider = createProvider(config);

        // Retrieve the WifiConfiguration associated with the provider, and verify the content of
        // the configuration.  Need to verify field by field since WifiConfiguration doesn't
        // override equals() function.
        WifiConfiguration wifiConfig = mProvider.getWifiConfig();
        WifiEnterpriseConfig wifiEnterpriseConfig = wifiConfig.enterpriseConfig;
        assertEquals(fqdn, wifiConfig.FQDN);
        assertEquals(friendlyName, wifiConfig.providerFriendlyName);
        assertTrue(Arrays.equals(rcOIs, wifiConfig.roamingConsortiumIds));
        assertTrue(wifiConfig.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.WPA_EAP));
        assertTrue(wifiConfig.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.IEEE8021X));
        assertEquals(allowedProtocols, wifiConfig.allowedProtocols);
        assertNull(wifiConfig.updateIdentifier);
        assertFalse(wifiConfig.shared);
        assertEquals(realm, wifiEnterpriseConfig.getRealm());
        assertEquals(fqdn, wifiEnterpriseConfig.getDomainSuffixMatch());
        assertEquals(WifiEnterpriseConfig.Eap.SIM, wifiEnterpriseConfig.getEapMethod());
        assertEquals(imsi, wifiEnterpriseConfig.getPlmn());
        assertEquals(WifiConfiguration.METERED_OVERRIDE_NONE, wifiConfig.meteredOverride);
    }

    /**
     * Verify that an expected {@link PasspointConfiguration} will be returned when converting
     * from a {@link WifiConfiguration} containing an user credential.
     *
     * @throws Exception
     */
    @Test
    public void convertFromWifiConfigWithUserCredential() throws Exception {
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

        assertEquals(passpointConfig, PasspointProvider.convertFromWifiConfig(wifiConfig));
    }

    /**
     * Verify that an expected {@link PasspointConfiguration} will be returned when converting
     * from a {@link WifiConfiguration} containing a SIM credential.
     *
     * @throws Exception
     */
    @Test
    public void convertFromWifiConfigWithSimCredential() throws Exception {
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

        assertEquals(passpointConfig, PasspointProvider.convertFromWifiConfig(wifiConfig));
    }

    /**
     * Verify that an expected {@link PasspointConfiguration} will be returned when converting
     * from a {@link WifiConfiguration} containing a certificate credential.
     *
     * @throws Exception
     */
    @Test
    public void convertFromWifiConfigWithCertCredential() throws Exception {
        // Test data.
        String fqdn = "test.com";
        String friendlyName = "Friendly Name";
        long[] rcOIs = new long[] {0x1234L, 0x2345L};
        String realm = "realm.com";

        // Setup WifiConfiguration for legacy Passpoint configuraiton.
        WifiConfiguration wifiConfig = new WifiConfiguration();
        wifiConfig.FQDN = fqdn;
        wifiConfig.providerFriendlyName = friendlyName;
        wifiConfig.roamingConsortiumIds = rcOIs;
        wifiConfig.enterpriseConfig.setRealm(realm);
        wifiConfig.enterpriseConfig.setEapMethod(WifiEnterpriseConfig.Eap.TLS);

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

        assertEquals(passpointConfig, PasspointProvider.convertFromWifiConfig(wifiConfig));
    }

    /**
     * Verify that {@link PasspointProvider#isSimCredential} will return true for provider that's
     * backed by a SIM credential.
     *
     * @throws Exception
     */
    @Test
    public void providerBackedBySimCredential() throws Exception {
        // Test data.
        String fqdn = "test.com";
        String friendlyName = "Friendly Name";
        long[] rcOIs = new long[] {0x1234L, 0x2345L};
        String realm = "realm.com";
        String imsi = "1234*";

        // Create provider with SIM credential.
        PasspointConfiguration config = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setFqdn(fqdn);
        homeSp.setFriendlyName(friendlyName);
        homeSp.setRoamingConsortiumOis(rcOIs);
        config.setHomeSp(homeSp);
        Credential credential = new Credential();
        credential.setRealm(realm);
        Credential.SimCredential simCredential = new Credential.SimCredential();
        simCredential.setImsi(imsi);
        simCredential.setEapType(EAPConstants.EAP_SIM);
        credential.setSimCredential(simCredential);
        config.setCredential(credential);
        mProvider = createProvider(config);

        assertTrue(mProvider.isSimCredential());
    }

    /**
     * Verify that {@link PasspointProvider#isSimCredential} will return false for provider that's
     * not backed by a SIM credential.
     *
     * @throws Exception
     */
    @Test
    public void providerNotBackedBySimCredential() throws Exception {
        // Test data.
        String fqdn = "test.com";
        String friendlyName = "Friendly Name";
        long[] rcOIs = new long[] {0x1234L, 0x2345L};
        String realm = "realm.com";

        // Create provider with certificate credential.
        PasspointConfiguration config = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setFqdn(fqdn);
        homeSp.setFriendlyName(friendlyName);
        homeSp.setRoamingConsortiumOis(rcOIs);
        config.setHomeSp(homeSp);
        Credential credential = new Credential();
        Credential.CertificateCredential certCredential = new Credential.CertificateCredential();
        certCredential.setCertType(Credential.CertificateCredential.CERT_TYPE_X509V3);
        credential.setCertCredential(certCredential);
        credential.setRealm(realm);
        config.setCredential(credential);
        mProvider = createProvider(config);

        assertFalse(mProvider.isSimCredential());
    }

    /**
     * Verify that hasEverConnected flag is set correctly using
     * {@link PasspointProvider#setHasEverConnected}.
     *
     * @throws Exception
     */
    @Test
    public void setHasEverConnected() throws Exception {
        PasspointConfiguration config = new PasspointConfiguration();
        HomeSp homeSp = new HomeSp();
        homeSp.setFqdn("test1");
        config.setHomeSp(homeSp);
        Credential credential = new Credential();
        credential.setUserCredential(new Credential.UserCredential());
        config.setCredential(credential);
        mProvider = createProvider(config);
        verifyInstalledConfig(config, true);

        assertFalse(mProvider.getHasEverConnected());
        mProvider.setHasEverConnected(true);
        assertTrue(mProvider.getHasEverConnected());
    }
}
