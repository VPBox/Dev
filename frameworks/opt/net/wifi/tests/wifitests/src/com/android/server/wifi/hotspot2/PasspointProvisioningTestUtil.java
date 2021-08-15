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

package com.android.server.wifi.hotspot2;

import android.graphics.drawable.Icon;
import android.net.Uri;
import android.net.wifi.WifiSsid;
import android.net.wifi.hotspot2.OsuProvider;
import android.util.Base64;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Helper for creating and populating WifiConfigurations in unit tests.
 */
public class PasspointProvisioningTestUtil {
    /**
     * These are constants used to generate predefined OsuProvider.
     */
    public static final WifiSsid TEST_SSID =
            WifiSsid.createFromByteArray("TEST SSID".getBytes(StandardCharsets.UTF_8));
    public static final String TEST_FRIENDLY_NAME = "Friendly Name";
    public static final String TEST_SERVICE_DESCRIPTION = "Dummy Service";
    public static final Uri TEST_SERVER_URI = Uri.parse("https://test.com");
    public static final Uri INVALID_SERVER_URI = Uri.parse("abcd");
    public static final String TEST_NAI = "test.access.com";
    public static final List<Integer> TEST_METHOD_LIST =
            Arrays.asList(OsuProvider.METHOD_SOAP_XML_SPP);
    public static final Icon TEST_ICON = Icon.createWithData(new byte[10], 0, 10);

    /**
     * Construct a {@link android.net.wifi.hotspot2.OsuProvider}.
     *
     * @param openOsuAP indicates if the OSU AP belongs to an open or OSEN network
     * @return the constructed {@link android.net.wifi.hotspot2.OsuProvider}
     */
    public static OsuProvider generateOsuProvider(boolean openOsuAP) {
        Map<String, String> friendlyNames = new HashMap<>();
        friendlyNames.put("en", TEST_FRIENDLY_NAME);
        return generateOsuProviderWithFriendlyName(openOsuAP, friendlyNames);
    }

    /**
     * Construct a {@link android.net.wifi.hotspot2.OsuProvider} with given friendlyName.
     *
     * @param openOsuAP     indicates if the OSU AP belongs to an open or OSEN network
     * @param friendlyNames map of friendly names with language and friendly name in the language
     * @return the constructed {@link android.net.wifi.hotspot2.OsuProvider}
     */
    public static OsuProvider generateOsuProviderWithFriendlyName(boolean openOsuAP,
            Map<String, String> friendlyNames) {
        if (openOsuAP) {
            return new OsuProvider(TEST_SSID, friendlyNames,
                    TEST_SERVICE_DESCRIPTION,
                    TEST_SERVER_URI, null, TEST_METHOD_LIST, TEST_ICON);
        } else {
            return new OsuProvider(TEST_SSID, friendlyNames,
                    TEST_SERVICE_DESCRIPTION,
                    TEST_SERVER_URI, TEST_NAI, TEST_METHOD_LIST, TEST_ICON);
        }
    }

    /**
     * Construct a {@link android.net.wifi.hotspot2.OsuProvider} with invalid server URL
     * @return the constructed {@link android.net.wifi.hotspot2.OsuProvider}
     */
    public static OsuProvider generateInvalidServerUrlOsuProvider() {
        HashMap<String, String> friendlyNameMap = new HashMap<>();
        friendlyNameMap.put("en", TEST_FRIENDLY_NAME);
        return new OsuProvider(TEST_SSID, friendlyNameMap,
                TEST_SERVICE_DESCRIPTION, INVALID_SERVER_URI, null, TEST_METHOD_LIST, TEST_ICON);
    }

    /**
     * Create the fake keyStore for the test.
     *
     * @return KeyStore that has the TEST_CERTs, null if not processed
     */
    public static KeyStore createFakeKeyStore() {
        KeyStore keyStore = null;
        int index = 0;
        try {
            keyStore = KeyStore.getInstance(KeyStore.getDefaultType());
            keyStore.load(null, null);
            List<X509Certificate> certs = getOsuCertsForTest();
            for (X509Certificate cert : certs) {
                keyStore.setCertificateEntry(String.format("%d", index), cert);
                index++;
            }
        } catch (KeyStoreException | CertificateException | NoSuchAlgorithmException
                | IOException e) {
            e.printStackTrace();
        }
        return keyStore;
    }

    /**
     * Return a set of OSU server certs to test with
     * @return List<X509Certificate> an array of OSU certs
     */
    /* package */ static List<X509Certificate> getOsuCertsForTest() {
        List<X509Certificate> osuCerts = new ArrayList<X509Certificate>();
        try {
            CertificateFactory certFactory = CertificateFactory.getInstance("X.509");
            for (String certString : TEST_CERTS) {
                ByteArrayInputStream bis = new ByteArrayInputStream(
                        Base64.decode(certString, Base64.DEFAULT));
                Certificate cert = certFactory.generateCertificate(bis);
                osuCerts.add((X509Certificate) cert);
            }
        } catch (CertificateException e) {
            e.printStackTrace();
        }
        return osuCerts;
    }

    private static final String[] TEST_CERTS = {
            "MIID8zCCAtugAwIBAgIJAIY3yjv1B5eWMA0GCSqGSIb3DQEBCwUAMIGPMQswCQYD"
                + "VQQGEwJVUzELMAkGA1UECAwCQ0ExFjAUBgNVBAcMDU1vdW50YWluIFZpZXcxDzAN"
                + "BgNVBAoMBkdvb2dsZTEQMA4GA1UECwwHQW5kcm9pZDETMBEGA1UEAwwKZ29vZ2xl"
                + "LmNvbTEjMCEGCSqGSIb3DQEJARYUc29oYW5pcmFvQGdvb2dsZS5jb20wHhcNMTgw"
                + "MTEyMDM0NjI3WhcNMTkwMTEyMDM0NjI3WjCBjzELMAkGA1UEBhMCVVMxCzAJBgNV"
                + "BAgMAkNBMRYwFAYDVQQHDA1Nb3VudGFpbiBWaWV3MQ8wDQYDVQQKDAZHb29nbGUx"
                + "EDAOBgNVBAsMB0FuZHJvaWQxEzARBgNVBAMMCmdvb2dsZS5jb20xIzAhBgkqhkiG"
                + "9w0BCQEWFHNvaGFuaXJhb0Bnb29nbGUuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOC"
                + "AQ8AMIIBCgKCAQEA41IIl6NpaN60XbS/fb0bW8+nWGh8OrHjFTyEOSXAE9W+sxFi"
                + "x2ygJxJvOpN0Nj+UFXMDnwIkvq7l0L0kUTAljQh1C1Gm4a2rDYKkMhTNab5uSp1/"
                + "X3VDUztIJVn4EKIt5jT9vGTqwvI4Xxd3fmM1/eRHpzR8gRLLs89itulpoHrXiN29"
                + "dlD72J2877vo3QktFMo/MiHyWPne5WiU5+qiAw70MflnptzaOCeisCVSYHyvhDtK"
                + "xWPgQ9eqG1jHgxOgdOEB6nzJvWY26GzjpEVZlUOBeEvcznmTqrFoM4Q4ANX//F1+"
                + "zdNfQPes5fPShMgLp65MAdVJFQKq7o8jVcX+2wIDAQABo1AwTjAdBgNVHQ4EFgQU"
                + "467KG5Zzpr4nnURUbOwdr5hoQVkwHwYDVR0jBBgwFoAU467KG5Zzpr4nnURUbOwd"
                + "r5hoQVkwDAYDVR0TBAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEAvI9DKIXTrJQi"
                + "Gaobr0wnQ1JfpP3oezWHmhFDSwdq3xhFLlxlZOLi3Ay/GwaW3cQfQ8c6xzpbjVNm"
                + "uMyNzfVHZjzRoX7Ae9S0uzsYmXuAm3LbD4EDflDDHlYKz97J4nlzPvi/EUxoOmk0"
                + "CF2LS9CAsI/AVCOqXne89EK4TwsZ8fSzNv0I2YdA2VFHUqqW2h1uZCG9Mb4gmwGj"
                + "bjxrdFAKFlSKxUfrus+BGwjOKwfvvkWLmU4TPqBAPgVLHUBrcdDoIoYW6XAA4Dnd"
                + "4eMFTUDmMaUew8K5aGuuj+2fNcOuz+/wy2xL8TkGF/m1Xi1JUbk0Ny/pG3qaznzB"
                + "FeODMMYDCg==",

        "MIIC7jCCAdagAwIBAgIJAI0gOVoxJv+kMA0GCSqGSIb3DQEBCwUAMBYxFDASBgNV"
                + "BAMTC2RvbWFpbjEuY29tMB4XDTE4MDExMzAwMzczOFoXDTE4MDIxMjAwMzczOFow"
                + "FjEUMBIGA1UEAxMLZG9tYWluMS5jb20wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAw"
                + "ggEKAoIBAQDsIvx1z9iLMuXXjP2/9YEBAMCQ6qC75t5YmqZeCfl2SIjt3Ho2qCRA"
                + "Wq5y5ZTSqTTBTMMiDHGAYe+qcP7+4UuyCx60CbhJVn1Maxcrhs3tUEKJlQNqEk4t"
                + "I4DONvZjsOX1UxYNvbjMlWf3KZ0rXLusvMjk4VEOxZVvyO4EyJ+K4tMse7OkHBGt"
                + "eBATgOqFvOZxy3SaicH36bN5eL6KWHOe0IIGoQ4VLcMrW9hBwSBaO9m95I0ZNAqt"
                + "0+84hgJNna6KtPRGfPaHdJfYZ6yqWMtpUr9yz8GRlSY67vieFkB+Qx2WO/En2K25"
                + "PM/zVGqmNFpDeATbUmCUDWegNb4XPkJBAgMBAAGjPzA9MAsGA1UdDwQEAwIEMDAT"
                + "BgNVHSUEDDAKBggrBgEFBQcDATAZBgNVHREEEjAQgg53d3cuZ29vZ2xlLmNvbTAN"
                + "BgkqhkiG9w0BAQsFAAOCAQEAQX+HT2JlCnZ/Zb9hCe8139poDxqkj4ZhYu94OpLH"
                + "Y+eRsMY8SA/rHp0fbCiqA4H/IL9GI79HGdcjNChqVH304fvdic97dwXZLOFC/JcO"
                + "XXIZs/j1ir6WfezhTdrLHXB+ZYahST2L8fEHeyS+goZFK0XNgdvTe1h2W7qfdn3Z"
                + "G4DkdYhFlzXnvSq+YI2SvSArrbw02wg4xZzHwXHsV8sIXuve8KvZzKw0+zP2oj5b"
                + "XgkNgi75VBf+dEvA9D6GSJ9uNaUmhlglpiwyeMBhDGYibje1/e4aQYclaarWMkf2"
                + "IYatpc/KqB9TWGIAVGLsCcfDbdbIil2AlaoA006SXa1fHA=="
    };
}
