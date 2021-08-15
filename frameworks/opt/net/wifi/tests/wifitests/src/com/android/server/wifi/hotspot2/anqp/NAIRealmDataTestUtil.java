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

package com.android.server.wifi.hotspot2.anqp;

import android.net.wifi.EAPConstants;

import com.android.server.wifi.hotspot2.anqp.eap.AuthParam;
import com.android.server.wifi.hotspot2.anqp.eap.CredentialType;
import com.android.server.wifi.hotspot2.anqp.eap.EAPMethod;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

/**
 * Utility class containing test data for NAI Realm Data.
 */
public class NAIRealmDataTestUtil {
    /**
     * Raw bytes for EAP Method.
     */
    private static final byte[] TEST_EAP_METHOD_BYTES =
            new byte[] {0x05 /* length */, 0x0D /* EAP_TLS */, 0x01 /* Auth Param Count */,
                        0x05 /* CredentialType */, 0x01, 0x02 /* USIM */};

    /**
     * NAI Realm strings.
     */
    private static final String[] TEST_REALMS = new String[] {"test1", "test2"};

    /**
     * Setup expected EAPMethod list.
     */
    private static final Map<Integer, Set<AuthParam>> TEST_EAP_METHOD_AUTH_PARAMS =
            new HashMap<>();
    private static final Set<AuthParam> TEST_EAP_METHOD_CREDENTIAL_TYPE_PARAMS = new HashSet<>();
    private static final List<EAPMethod> TEST_EAP_METHOD_LIST = new ArrayList<>();
    static {
        TEST_EAP_METHOD_CREDENTIAL_TYPE_PARAMS.add(new CredentialType(
                AuthParam.PARAM_TYPE_CREDENTIAL_TYPE, CredentialType.CREDENTIAL_TYPE_USIM));
        TEST_EAP_METHOD_AUTH_PARAMS.put(AuthParam.PARAM_TYPE_CREDENTIAL_TYPE,
                TEST_EAP_METHOD_CREDENTIAL_TYPE_PARAMS);

        TEST_EAP_METHOD_LIST.add(new EAPMethod(EAPConstants.EAP_TLS, TEST_EAP_METHOD_AUTH_PARAMS));
    }

    /**
     * Setup expected NAIRealmData.
     */
    public static final NAIRealmData TEST_REALM_DATA =
            new NAIRealmData(Arrays.asList(TEST_REALMS), TEST_EAP_METHOD_LIST);

    public static byte[] TEST_REAML_WITH_UTF8_DATA_BYTES = formatNAIRealmData(true);
    public static byte[] TEST_REAML_WITH_NON_UTF8_DATA_BYTES = formatNAIRealmData(false);

    /**
     * Helper function for returning raw bytes of NAI Realm Data (including the length field) for
     * testing.
     *
     * @param utfEncoding Flag indicating the UTF encoding of the realm string
     * @return byte[]
     */
    private static byte[] formatNAIRealmData(boolean utfEncoding) {
        try {
            byte[] realmData = getNAIRealmData(utfEncoding);
            ByteArrayOutputStream stream = new ByteArrayOutputStream();
            // Realm Data length in Little-Endian.
            stream.write((byte) realmData.length);
            stream.write((byte) realmData.length >> 8);
            stream.write(realmData);
            return stream.toByteArray();
        } catch (IOException e) {
            return null;
        }
    }

    /**
     * Helper function for returning raw bytes of NAI Realm Data payload.
     *
     * @param utfEncoding Flag indicating the encoding of NAI Realm string
     * @return byte[]
     * @throws IOException
     */
    private static byte[] getNAIRealmData(boolean utfEncoding) throws IOException {
        String realmsStr = String.join(NAIRealmData.NAI_REALM_STRING_SEPARATOR, TEST_REALMS);
        byte[] realmStrData = realmsStr.getBytes(
                utfEncoding ? StandardCharsets.UTF_8 : StandardCharsets.US_ASCII);

        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        // NAI Realm Encoding byte.
        stream.write((byte) (utfEncoding ? NAIRealmData.NAI_ENCODING_UTF8_MASK : 0));
        stream.write((byte) realmStrData.length);
        stream.write(realmStrData);
        stream.write((byte) 1);    // EAP Method count
        stream.write(TEST_EAP_METHOD_BYTES);
        return stream.toByteArray();
    }
}
