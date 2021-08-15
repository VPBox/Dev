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

package com.android.server.wifi.hotspot2.anqp.eap;

import static org.junit.Assert.assertEquals;

import android.net.wifi.EAPConstants;

import androidx.test.filters.SmallTest;

import org.junit.Test;

import java.net.ProtocolException;
import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.eap.EAPMethod}.
 */
@SmallTest
public class EAPMethodTest {
    /**
     * Setup basic test data - contained multiple parameters of the same type.
     */
    private static final byte[] TEST_DATA1_BYTES =
            new byte[] {0x0B /* length */, 0x0D /* EAP_TLS */, 0x03 /* Auth Param Count */,
                        0x05 /* CredentialType */, 0x01, 0x02 /* USIM */,
                        0x05 /* CredentialType */, 0x01, 0x06 /* Certificate */,
                        0x06 /* Tunneled CredentialType */, 0x01, 0x03 /* NFC */};
    private static final Map<Integer, Set<AuthParam>> TEST_DATA1_AUTH_PARAMS =
            new HashMap<>();
    private static final Set<AuthParam> TEST_DATA1_CREDENTIAL_TYPE_PARAMS = new HashSet<>();
    private static final Set<AuthParam> TEST_DATA1_TUNNELED_CREDENTIAL_TYPE_PARAMS =
            new HashSet<>();
    static {
        TEST_DATA1_CREDENTIAL_TYPE_PARAMS.add(new CredentialType(
                AuthParam.PARAM_TYPE_CREDENTIAL_TYPE, CredentialType.CREDENTIAL_TYPE_USIM));
        TEST_DATA1_CREDENTIAL_TYPE_PARAMS.add(new CredentialType(
                AuthParam.PARAM_TYPE_CREDENTIAL_TYPE, CredentialType.CREDENTIAL_TYPE_CERTIFICATE));

        TEST_DATA1_TUNNELED_CREDENTIAL_TYPE_PARAMS.add(new CredentialType(
                AuthParam.PARAM_TYPE_TUNNELED_EAP_METHOD_CREDENTIAL_TYPE,
                CredentialType.CREDENTIAL_TYPE_NFC));

        TEST_DATA1_AUTH_PARAMS.put(AuthParam.PARAM_TYPE_CREDENTIAL_TYPE,
                TEST_DATA1_CREDENTIAL_TYPE_PARAMS);
        TEST_DATA1_AUTH_PARAMS.put(AuthParam.PARAM_TYPE_TUNNELED_EAP_METHOD_CREDENTIAL_TYPE,
                TEST_DATA1_TUNNELED_CREDENTIAL_TYPE_PARAMS);
    }
    private static final EAPMethod TEST_DATA1_EAP_METHOD = new EAPMethod(
            EAPConstants.EAP_TLS, TEST_DATA1_AUTH_PARAMS);

    /**
     * Setup test data for testing an EAP Method containing all types of authentication parameters.
     */
    private static final byte[] TEST_DATA2_BYTES =
            new byte[] {0x26 /* length */, 0x0D /* EAP_TLS */, 0x07,
                        // Expanded EAP Method
                        0x01, 0x07, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78,
                        // Non-EAP Inner Auth Type
                        0x02, 0x01, 0x03 /* AUTH_TYPE_MSCHAP */,
                        // Inner Auth EAP Method
                        0x03, 0x01, 0x1D /* EAP_PEAP */,
                        // Expanded Inner EAP Method
                        0x04, 0x07, 0x01, 0x23, 0x45, 0x56, 0x78, 0x56, 0x12,
                        // Credential Type
                        0x05, 0x01, 0x02 /* USIM */,
                        // Tunneled Credential Type
                        0x06, 0x01, 0x03 /* NFC */,
                        // Vendor Specific
                        (byte) 0xDD, 0x04, 0x12, 0x23, 0x45, 0x56};
    private static final Map<Integer, Set<AuthParam>> TEST_DATA2_AUTH_PARAMS =
            new HashMap<>();
    private static final Set<AuthParam> TEST_DATA2_EXPANDED_EAP_METHOD_PARAMS = new HashSet<>();
    private static final Set<AuthParam> TEST_DATA2_NON_EAP_INNER_AUTH_PARAMS = new HashSet<>();
    private static final Set<AuthParam> TEST_DATA2_INNER_AUTH_EAP_PARAMS = new HashSet<>();
    private static final Set<AuthParam> TEST_DATA2_EXPANDED_INNER_EAP_PARAMS = new HashSet<>();
    private static final Set<AuthParam> TEST_DATA2_CREDENTIAL_TYPE_PARAMS = new HashSet<>();
    private static final Set<AuthParam> TEST_DATA2_TUNNELED_CREDENTIAL_TYPE_PARAMS =
            new HashSet<>();
    private static final Set<AuthParam> TEST_DATA2_VENDOR_SPECIFIC_PARAMS = new HashSet<>();
    static {
        TEST_DATA2_EXPANDED_EAP_METHOD_PARAMS.add(new ExpandedEAPMethod(
                AuthParam.PARAM_TYPE_EXPANDED_EAP_METHOD, 0x122334, 0x45566778L));
        TEST_DATA2_NON_EAP_INNER_AUTH_PARAMS.add(new NonEAPInnerAuth(
                NonEAPInnerAuth.AUTH_TYPE_MSCHAP));
        TEST_DATA2_INNER_AUTH_EAP_PARAMS.add(new InnerAuthEAP(EAPConstants.EAP_PEAP));
        TEST_DATA2_EXPANDED_INNER_EAP_PARAMS.add(new ExpandedEAPMethod(
                AuthParam.PARAM_TYPE_EXPANDED_INNER_EAP_METHOD, 0x012345, 0x56785612L));
        TEST_DATA2_CREDENTIAL_TYPE_PARAMS.add(new CredentialType(
                AuthParam.PARAM_TYPE_CREDENTIAL_TYPE, CredentialType.CREDENTIAL_TYPE_USIM));
        TEST_DATA2_TUNNELED_CREDENTIAL_TYPE_PARAMS.add(new CredentialType(
                AuthParam.PARAM_TYPE_TUNNELED_EAP_METHOD_CREDENTIAL_TYPE,
                CredentialType.CREDENTIAL_TYPE_NFC));
        TEST_DATA2_VENDOR_SPECIFIC_PARAMS.add(new VendorSpecificAuth(
                new byte[] {0x12, 0x23, 0x45, 0x56}));

        TEST_DATA2_AUTH_PARAMS.put(AuthParam.PARAM_TYPE_EXPANDED_EAP_METHOD,
                TEST_DATA2_EXPANDED_EAP_METHOD_PARAMS);
        TEST_DATA2_AUTH_PARAMS.put(AuthParam.PARAM_TYPE_NON_EAP_INNER_AUTH_TYPE,
                TEST_DATA2_NON_EAP_INNER_AUTH_PARAMS);
        TEST_DATA2_AUTH_PARAMS.put(AuthParam.PARAM_TYPE_INNER_AUTH_EAP_METHOD_TYPE,
                TEST_DATA2_INNER_AUTH_EAP_PARAMS);
        TEST_DATA2_AUTH_PARAMS.put(AuthParam.PARAM_TYPE_EXPANDED_INNER_EAP_METHOD,
                TEST_DATA2_EXPANDED_INNER_EAP_PARAMS);
        TEST_DATA2_AUTH_PARAMS.put(AuthParam.PARAM_TYPE_CREDENTIAL_TYPE,
                TEST_DATA2_CREDENTIAL_TYPE_PARAMS);
        TEST_DATA2_AUTH_PARAMS.put(AuthParam.PARAM_TYPE_TUNNELED_EAP_METHOD_CREDENTIAL_TYPE,
                TEST_DATA2_TUNNELED_CREDENTIAL_TYPE_PARAMS);
        TEST_DATA2_AUTH_PARAMS.put(AuthParam.PARAM_TYPE_VENDOR_SPECIFIC,
                TEST_DATA2_VENDOR_SPECIFIC_PARAMS);
    }
    private static final EAPMethod TEST_DATA2_EAP_METHOD = new EAPMethod(
            EAPConstants.EAP_TLS, TEST_DATA2_AUTH_PARAMS);

    /**
     * Verify that BufferUnderflowException will be thrown when parsing from an empty buffer.
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseEmptyBuffer() throws Exception {
        EAPMethod.parse(ByteBuffer.wrap(new byte[0]));
    }

    /**
     * Verify that ProtocolException will be thrown when parsing a truncated buffer
     * (missing a byte at the end).
     *
     * @throws Exception
     */
    @Test(expected = ProtocolException.class)
    public void parseTruncatedBuffer() throws Exception {
        ByteBuffer buffer = ByteBuffer.wrap(TEST_DATA1_BYTES, 0, TEST_DATA1_BYTES.length - 1);
        EAPMethod.parse(buffer);
    }

    /**
     * Verify that the expected EAPMethod is return when parsing a buffer contained
     * {@link #TEST_DATA1_BYTES}.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithTestData1() throws Exception {
        assertEquals(TEST_DATA1_EAP_METHOD, EAPMethod.parse(ByteBuffer.wrap(TEST_DATA1_BYTES)));
    }

    /**
     * Verify that the expected EAPMethod is return when parsing a buffer contained
     * {@link #TEST_DATA2_BYTES}.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithTestData2() throws Exception {
        assertEquals(TEST_DATA2_EAP_METHOD, EAPMethod.parse(ByteBuffer.wrap(TEST_DATA2_BYTES)));
    }
}
