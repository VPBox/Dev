/*
 * Copyright 2018 The Android Open Source Project
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

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.ArgumentMatchers.isNull;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.reset;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.net.Network;
import android.os.test.TestLooper;
import android.util.Pair;

import androidx.test.filters.SmallTest;

import com.android.dx.mockito.inline.extended.ExtendedMockito;
import com.android.org.conscrypt.TrustManagerImpl;
import com.android.server.wifi.hotspot2.soap.HttpsServiceConnection;
import com.android.server.wifi.hotspot2.soap.HttpsTransport;
import com.android.server.wifi.hotspot2.soap.SoapParser;
import com.android.server.wifi.hotspot2.soap.SppResponseMessage;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.ksoap2.HeaderProperty;
import org.ksoap2.SoapEnvelope;
import org.ksoap2.serialization.SoapObject;
import org.ksoap2.serialization.SoapSerializationEnvelope;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.mockito.MockitoAnnotations;
import org.mockito.MockitoSession;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.Socket;
import java.net.URL;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;

import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;

/**
 * Unit tests for {@link OsuServerConnection}.
 */
@SmallTest
public class OsuServerConnectionTest {
    private static final String TEST_VALID_URL = "https://www.google.com";
    private static final String TEST_INVALID_URL = "http://www.google.com";
    private static final String AUTH_TYPE = "ECDHE_RSA";
    private static final String PROVIDER_NAME_VALID = "Boingo";
    private static final String PROVIDER_NAME_INVALID = "Boingo1";
    private static final String TEST_PROVIDER_CHINESE_NAME = "宝音阁";
    private static final int ENABLE_VERBOSE_LOGGING = 1;
    private static final int TEST_SESSION_ID = 1;

    private TestLooper mLooper = new TestLooper();
    private OsuServerConnection mOsuServerConnection;
    private URL mServerUrl;
    private List<Pair<Locale, String>> mProviderIdentities = new ArrayList<>();
    private ArgumentCaptor<TrustManager[]> mTrustManagerCaptor =
            ArgumentCaptor.forClass(TrustManager[].class);
    private Map<Integer, Map<String, byte[]>> mTrustCertsInfo = new HashMap<>();

    @Mock PasspointProvisioner.OsuServerCallbacks mOsuServerCallbacks;
    @Mock Network mNetwork;
    @Mock HttpsURLConnection mUrlConnection;
    @Mock WfaKeyStore mWfaKeyStore;
    @Mock SSLContext mTlsContext;
    @Mock KeyStore mKeyStore;
    @Mock TrustManagerImpl mDelegate;
    @Mock HttpsTransport mHttpsTransport;
    @Mock HttpsServiceConnection mHttpsServiceConnection;
    @Mock SppResponseMessage mSppResponseMessage;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mOsuServerConnection = new OsuServerConnection(mLooper.getLooper());
        mOsuServerConnection.enableVerboseLogging(ENABLE_VERBOSE_LOGGING);
        mProviderIdentities.add(Pair.create(Locale.US, PROVIDER_NAME_VALID));
        mServerUrl = new URL(TEST_VALID_URL);
        when(mWfaKeyStore.get()).thenReturn(mKeyStore);
        when(mOsuServerCallbacks.getSessionId()).thenReturn(TEST_SESSION_ID);
        when(mNetwork.openConnection(any(URL.class))).thenReturn(mUrlConnection);
        when(mHttpsTransport.getServiceConnection()).thenReturn(mHttpsServiceConnection);
        when(mDelegate.getTrustedChainForServer(any(X509Certificate[].class), anyString(),
                (Socket) isNull()))
                .thenReturn(PasspointProvisioningTestUtil.getOsuCertsForTest());
    }

    /**
     * Verifies initialization and connecting to the OSU server
     */
    @Test
    public void verifyInitAndConnect() throws Exception {
        // static mocking
        MockitoSession session = ExtendedMockito.mockitoSession().mockStatic(
                ServiceProviderVerifier.class).startMocking();
        try {
            when(ServiceProviderVerifier.getProviderNames(any(X509Certificate.class))).thenReturn(
                    mProviderIdentities);

            establishServerConnection();

            TrustManager[] trustManagers = mTrustManagerCaptor.getValue();
            X509TrustManager trustManager = (X509TrustManager) trustManagers[0];

            trustManager.checkServerTrusted(new X509Certificate[1], AUTH_TYPE);

            verify(mOsuServerCallbacks).onServerValidationStatus(anyInt(), eq(true));
            Map<String, String> providerNames = new HashMap<>();
            providerNames.put(Locale.US.getISO3Language(), PROVIDER_NAME_VALID);
            assertTrue(mOsuServerConnection.validateProvider(providerNames));
        } finally {
            session.finishMocking();
        }
    }

    /**
     * Verifies multiple languages of OsuProvider names are matched with cert
     */
    @Test
    public void verifyValidateProviderWithMultipleProviderLangs() throws Exception {
        // static mocking
        MockitoSession session = ExtendedMockito.mockitoSession().mockStatic(
                ServiceProviderVerifier.class).startMocking();
        try {
            when(ServiceProviderVerifier.getProviderNames(any(X509Certificate.class))).thenReturn(
                    mProviderIdentities);
            establishServerConnection();
            TrustManager[] trustManagers = mTrustManagerCaptor.getValue();
            X509TrustManager trustManager = (X509TrustManager) trustManagers[0];
            trustManager.checkServerTrusted(new X509Certificate[1], AUTH_TYPE);
            Map<String, String> friendlyNames = new HashMap<>();
            friendlyNames.put(
                    Locale.SIMPLIFIED_CHINESE.getISO3Language(), TEST_PROVIDER_CHINESE_NAME);
            friendlyNames.put(Locale.US.getISO3Language(), PROVIDER_NAME_VALID);

            assertTrue(mOsuServerConnection.validateProvider(friendlyNames));
        } finally {
            session.finishMocking();
        }
    }

    /**
     * Verifies wrong language of OsuProvider name is mismatched with cert
     */
    @Test
    public void verifyValidateProviderWithMismatchedProviderLang() throws Exception {
        // static mocking
        MockitoSession session = ExtendedMockito.mockitoSession().mockStatic(
                ServiceProviderVerifier.class).startMocking();
        try {
            when(ServiceProviderVerifier.getProviderNames(any(X509Certificate.class))).thenReturn(
                    mProviderIdentities);
            establishServerConnection();
            TrustManager[] trustManagers = mTrustManagerCaptor.getValue();
            X509TrustManager trustManager = (X509TrustManager) trustManagers[0];
            trustManager.checkServerTrusted(new X509Certificate[1], AUTH_TYPE);
            Map<String, String> friendlyNames = new HashMap<>();
            friendlyNames.put(
                    Locale.SIMPLIFIED_CHINESE.getISO3Language(), TEST_PROVIDER_CHINESE_NAME);

            assertFalse(mOsuServerConnection.validateProvider(friendlyNames));
        } finally {
            session.finishMocking();
        }
    }

    /**
     * Verifies same language from different regions.
     */
    @Test
    public void verifyValidateProviderWithSameLangButDifferentRegion() throws Exception {
        // static mocking
        MockitoSession session = ExtendedMockito.mockitoSession().mockStatic(
                ServiceProviderVerifier.class).startMocking();
        try {
            when(ServiceProviderVerifier.getProviderNames(any(X509Certificate.class))).thenReturn(
                    mProviderIdentities);
            establishServerConnection();
            TrustManager[] trustManagers = mTrustManagerCaptor.getValue();
            X509TrustManager trustManager = (X509TrustManager) trustManagers[0];
            trustManager.checkServerTrusted(new X509Certificate[1], AUTH_TYPE);
            Map<String, String> friendlyNames = new HashMap<>();
            friendlyNames.put(
                    Locale.CANADA.getISO3Language(), PROVIDER_NAME_VALID);

            assertTrue(mOsuServerConnection.validateProvider(friendlyNames));
        } finally {
            session.finishMocking();
        }
    }

    /**
     * Verifies initialization of the HTTPS connection with invalid TLSContext
     */
    @Test
    public void verifyInvalidTlsContext() {
        mOsuServerConnection.init(null, mDelegate);
        mOsuServerConnection.setEventCallback(mOsuServerCallbacks);

        assertFalse(mOsuServerConnection.canValidateServer());
    }

    /**
     * Verifies initialization of the HTTPS connection when TlsContext init fails
     */
    @Test
    public void verifyTlsContextInitFailure() throws Exception {
        doThrow(new KeyManagementException()).when(mTlsContext).init(any(), any(), any());

        mOsuServerConnection.init(mTlsContext, mDelegate);
        mOsuServerConnection.setEventCallback(mOsuServerCallbacks);

        assertFalse(mOsuServerConnection.canValidateServer());
    }

    /**
     * Verifies initialization and opening URL connection failed on the network
     */
    @Test
    public void verifyInitAndNetworkOpenURLConnectionFailed() throws Exception {
        doThrow(new IOException()).when(mNetwork).openConnection(any(URL.class));

        mOsuServerConnection.init(mTlsContext, mDelegate);
        mOsuServerConnection.setEventCallback(mOsuServerCallbacks);

        assertTrue(mOsuServerConnection.canValidateServer());
        assertTrue(mOsuServerConnection.connect(mServerUrl, mNetwork));

        mLooper.dispatchAll();

        verify(mOsuServerCallbacks).onServerConnectionStatus(anyInt(), eq(false));
    }

    /**
     * Verifies initialization and connection failure to OSU server
     */
    @Test
    public void verifyInitAndServerConnectFailure() throws Exception {
        doThrow(new IOException()).when(mUrlConnection).connect();

        mOsuServerConnection.init(mTlsContext, mDelegate);
        mOsuServerConnection.setEventCallback(mOsuServerCallbacks);

        assertTrue(mOsuServerConnection.canValidateServer());
        assertTrue(mOsuServerConnection.connect(mServerUrl, mNetwork));

        mLooper.dispatchAll();

        verify(mOsuServerCallbacks).onServerConnectionStatus(anyInt(), eq(false));
    }

    /**
     * Verifies initialization and connecting to the OSU server, cert validation failure
     */
    @Test
    public void verifyInitAndConnectCertValidationFailure() throws Exception {
        establishServerConnection();
        List<X509Certificate> certificateList = PasspointProvisioningTestUtil.getOsuCertsForTest();
        X509Certificate[] certificates = new X509Certificate[1];
        certificates[0] = certificateList.get(0);
        TrustManager[] trustManagers = mTrustManagerCaptor.getValue();
        X509TrustManager trustManager = (X509TrustManager) trustManagers[0];
        doThrow(new CertificateException()).when(mDelegate)
                .getTrustedChainForServer(any(X509Certificate[].class), anyString(),
                        (Socket) isNull());

        trustManager.checkServerTrusted(certificates, AUTH_TYPE);

        verify(mOsuServerCallbacks).onServerValidationStatus(anyInt(), eq(false));
    }

    /**
     * Verifies initialization and connecting to the OSU server, friendly name mismatch
     */
    @Test
    public void verifyInitAndConnectInvalidProviderIdentity()throws Exception  {
        // static mocking
        MockitoSession session = ExtendedMockito.mockitoSession().mockStatic(
                ServiceProviderVerifier.class).startMocking();
        try {
            when(ServiceProviderVerifier.getProviderNames(any(X509Certificate.class))).thenReturn(
                    mProviderIdentities);

            establishServerConnection();

            TrustManager[] trustManagers = mTrustManagerCaptor.getValue();
            X509TrustManager trustManager = (X509TrustManager) trustManagers[0];

            trustManager.checkServerTrusted(new X509Certificate[1], AUTH_TYPE);

            verify(mOsuServerCallbacks).onServerValidationStatus(anyInt(), eq(true));
            Map<String, String> providerNames = new HashMap<>();
            providerNames.put(Locale.US.getISO3Language(), PROVIDER_NAME_INVALID);
            assertFalse(mOsuServerConnection.validateProvider(providerNames));
        } finally {
            session.finishMocking();
        }
    }

    /**
     * Verifies {@code ExchangeSoapMessage} should return {@code false} if there is no connection.
     */
    @Test
    public void verifyExchangeSoapMessageWithoutConnection() {
        assertFalse(mOsuServerConnection.exchangeSoapMessage(
                new SoapSerializationEnvelope(SoapEnvelope.VER12)));
    }

    /**
     * Verifies {@code ExchangeSoapMessage} should return {@code false} if {@code soapEnvelope} is
     * {@code null}
     */
    @Test
    public void verifyExchangeSoapMessageWithInvalidArgument() throws Exception {
        establishServerConnection();
        assertFalse(mOsuServerConnection.exchangeSoapMessage(null));
    }

    /**
     * Verifies {@code ExchangeSoapMessage} should get {@code null} message if exception occurs
     * during soap exchange.
     */
    @Test
    public void verifyExchangeSoapMessageWithException() throws Exception {
        // static mocking
        MockitoSession session = ExtendedMockito.mockitoSession().mockStatic(
                HttpsTransport.class).startMocking();
        try {
            establishServerConnection();
            when(HttpsTransport.createInstance(any(Network.class), any(URL.class))).thenReturn(
                    mHttpsTransport);
            doThrow(new IOException()).when(mHttpsTransport).call(any(String.class),
                    any(SoapSerializationEnvelope.class));

            assertTrue(mOsuServerConnection.exchangeSoapMessage(
                    new SoapSerializationEnvelope(SoapEnvelope.VER12)));

            mLooper.dispatchAll();

            verify(mOsuServerCallbacks).onReceivedSoapMessage(anyInt(), isNull());
        } finally {
            session.finishMocking();
        }
    }

    /**
     * Verifies {@code ExchangeSoapMessage} should return an instance of {@link SppResponseMessage}.
     */
    @Test
    public void verifyExchangeSoapMessage() throws Exception {
        // static mocking
        MockitoSession session = ExtendedMockito.mockitoSession().mockStatic(
                HttpsTransport.class).mockStatic(SoapParser.class).startMocking();
        try {
            establishServerConnection();

            SoapSerializationEnvelope envelope = new SoapSerializationEnvelope(SoapEnvelope.VER12);
            envelope.bodyIn = new SoapObject();
            when(HttpsTransport.createInstance(any(Network.class), any(URL.class))).thenReturn(
                    mHttpsTransport);
            when(SoapParser.getResponse(any(SoapObject.class))).thenReturn(mSppResponseMessage);

            assertTrue(mOsuServerConnection.exchangeSoapMessage(envelope));

            mLooper.dispatchAll();

            verify(mOsuServerCallbacks).onReceivedSoapMessage(anyInt(), eq(mSppResponseMessage));
        } finally {
            session.finishMocking();
        }
    }

    /**
     * Verifies {@code retrieveTrustRootCerts} should return {@code false} if there is no
     * connection.
     */
    @Test
    public void verifyRetrieveTrustRootCertsWithoutConnection() {
        assertFalse(mOsuServerConnection.retrieveTrustRootCerts(mTrustCertsInfo));
    }

    /**
     * Verifies {@code retrieveTrustRootCerts} should return {@code false} if {@code
     * mTrustCertsInfo} is empty.
     */
    @Test
    public void verifyRetrieveTrustRootCertsWithEmptyOfTrustCertsInfo() {
        mOsuServerConnection.init(mTlsContext, mDelegate);
        mOsuServerConnection.setEventCallback(mOsuServerCallbacks);
        assertFalse(mOsuServerConnection.retrieveTrustRootCerts(mTrustCertsInfo));
    }

    /**
     * Verifies it should return an empty collection of CA certificates if HTTPS response from
     * server to get root CA certificate is not HTTP OK.
     */
    @Test
    public void verifyRetrieveTrustRootCertsWithErrorInHTTPSResponse() throws Exception {
        // static mocking
        MockitoSession session = ExtendedMockito.mockitoSession().mockStatic(
                HttpsTransport.class).startMocking();
        try {
            when(HttpsTransport.createInstance(any(Network.class), any(URL.class))).thenReturn(
                    mHttpsTransport);
            when(mHttpsServiceConnection.getResponseCode()).thenReturn(
                    HttpURLConnection.HTTP_NO_CONTENT);
            ArgumentCaptor<Map<Integer, List<X509Certificate>>> argumentCaptor =
                    ArgumentCaptor.forClass(Map.class);

            // Test Data
            Map<String, byte[]> certInfo = new HashMap<>();
            certInfo.put("https://test.com/trustroot", "testData".getBytes());
            certInfo.put("https://test2.com/trustroot", "testData2".getBytes());
            mTrustCertsInfo.put(OsuServerConnection.TRUST_CERT_TYPE_AAA, certInfo);

            establishServerConnection();

            assertTrue(mOsuServerConnection.retrieveTrustRootCerts(mTrustCertsInfo));

            mLooper.dispatchAll();

            verify(mOsuServerCallbacks).onReceivedTrustRootCertificates(anyInt(),
                    argumentCaptor.capture());
            assertTrue(argumentCaptor.getValue().isEmpty());
        } finally {
            session.finishMocking();
        }
    }

    /**
     * Verifies it should return a collection of CA certificates if there is no error while
     * downloading root CA certificate from each {@code URL} provided
     */
    @Test
    public void verifyRetrieveTrustRootCertsWithoutError() throws Exception {
        // static mocking
        MockitoSession session = ExtendedMockito.mockitoSession().mockStatic(
                HttpsTransport.class).mockStatic(CertificateFactory.class).mockStatic(
                ServiceProviderVerifier.class).startMocking();
        try {
            X509Certificate certificate = Mockito.mock(X509Certificate.class);
            InputStream inputStream = Mockito.mock(InputStream.class);

            // To avoid infinite loop in OsuServerConnection.getCert.
            when(inputStream.read(any(byte[].class), anyInt(), anyInt())).thenReturn(-1);

            CertificateFactory certificateFactory = Mockito.mock(CertificateFactory.class);
            when(certificateFactory.generateCertificate(any(InputStream.class))).thenReturn(
                    certificate);
            when(CertificateFactory.getInstance(anyString())).thenReturn(certificateFactory);
            when(HttpsTransport.createInstance(any(Network.class), any(URL.class))).thenReturn(
                    mHttpsTransport);
            when(mHttpsServiceConnection.getResponseCode()).thenReturn(
                    HttpURLConnection.HTTP_OK);
            when(mHttpsServiceConnection.openInputStream()).thenReturn(inputStream);
            ArgumentCaptor<Map<Integer, List<X509Certificate>>> argumentCaptor =
                    ArgumentCaptor.forClass(Map.class);
            when(ServiceProviderVerifier.verifyCertFingerprint(any(X509Certificate.class),
                    any(byte[].class))).thenReturn(true);

            // Test Data
            Map<String, byte[]> certInfo = new HashMap<>();
            certInfo.put("https://test.com/trustroot", "testData".getBytes());
            mTrustCertsInfo.put(OsuServerConnection.TRUST_CERT_TYPE_AAA, certInfo);

            List<HeaderProperty> properties = new ArrayList<>();

            // Indicates that X.509 CA certificate is included.
            properties.add(new HeaderProperty("Content-Type", "application/x-x509-ca-cert"));
            when(mHttpsServiceConnection.getResponseProperties()).thenReturn(properties);

            establishServerConnection();

            assertTrue(mOsuServerConnection.retrieveTrustRootCerts(mTrustCertsInfo));

            mLooper.dispatchAll();

            verify(mOsuServerCallbacks).onReceivedTrustRootCertificates(anyInt(),
                    argumentCaptor.capture());
            Assert.assertEquals(1, argumentCaptor.getValue().size());
            Assert.assertEquals(certificate,
                    argumentCaptor.getValue().get(OsuServerConnection.TRUST_CERT_TYPE_AAA).get(0));
        } finally {
            session.finishMocking();
        }
    }

    /**
     * Verifies that cleanup is properly called on the OsuServerHandlerThread.
     */
    @Test
    public void verifyCleanup() throws Exception {
        // static mocking
        MockitoSession session = ExtendedMockito.mockitoSession().mockStatic(
                HttpsTransport.class).mockStatic(SoapParser.class).startMocking();
        try {
            establishServerConnection();

            SoapSerializationEnvelope envelope = new SoapSerializationEnvelope(SoapEnvelope.VER12);
            envelope.bodyIn = new SoapObject();
            when(HttpsTransport.createInstance(any(Network.class), any(URL.class))).thenReturn(
                    mHttpsTransport);
            when(SoapParser.getResponse(any(SoapObject.class))).thenReturn(mSppResponseMessage);

            assertTrue(mOsuServerConnection.exchangeSoapMessage(envelope));

            mLooper.dispatchAll();

            verify(mHttpsServiceConnection).disconnect();
            reset(mHttpsServiceConnection);

            mOsuServerConnection.cleanup();
            mLooper.dispatchAll();

            verify(mUrlConnection).disconnect();
            verify(mHttpsServiceConnection, never()).disconnect();
        } finally {
            session.finishMocking();
        }
    }

    /**
     * Verifies initialization and opening URL connection failure for an HTTP URL (not HTTPS)
     */
    @Test
    public void verifyInitAndNetworkOpenURLConnectionFailedWithHttpUrl() throws Exception {
        mServerUrl = new URL(TEST_INVALID_URL);
        mOsuServerConnection.init(mTlsContext, mDelegate);
        mOsuServerConnection.setEventCallback(mOsuServerCallbacks);

        assertTrue(mOsuServerConnection.canValidateServer());
        assertFalse(mOsuServerConnection.connect(mServerUrl, mNetwork));
    }

    private void establishServerConnection() throws Exception {
        mOsuServerConnection.init(mTlsContext, mDelegate);
        mOsuServerConnection.setEventCallback(mOsuServerCallbacks);
        verify(mTlsContext).init(isNull(), mTrustManagerCaptor.capture(), isNull());

        assertTrue(mOsuServerConnection.canValidateServer());
        assertTrue(mOsuServerConnection.connect(mServerUrl, mNetwork));
        mLooper.dispatchAll();

        verify(mOsuServerCallbacks).onServerConnectionStatus(anyInt(), eq(true));
    }
}
