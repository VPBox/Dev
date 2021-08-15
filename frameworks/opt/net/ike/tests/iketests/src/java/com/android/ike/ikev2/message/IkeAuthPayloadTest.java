/*
 * Copyright (C) 2018 The Android Open Source Project
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

package com.android.ike.ikev2.message;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import org.junit.Test;

import javax.crypto.Mac;

public final class IkeAuthPayloadTest {
    private static final String PSK_AUTH_PAYLOAD_HEX_STRING =
            "02000000df7c038aefaaa32d3f44b228b52a332744dfb2c1";
    private static final String PSK_AUTH_PAYLOAD_SIGNATURE_HEX_STRING =
            "df7c038aefaaa32d3f44b228b52a332744dfb2c1";
    private static final String PSK_ID_PAYLOAD_HEX_STRING = "010000000a50500d";
    private static final String PSK_SKP_HEX_STRING = "094787780EE466E2CB049FA327B43908BC57E485";
    private static final String PSK_SIGNED_OCTETS_APPENDIX_HEX_STRING =
            "D83B20CC6A0932B2A7CEF26E4020ABAAB64F0C6A";
    private static final String PSK_IKE_INIT_REQUEST_HEX_STRING =
            "5f54bf6d8b48e6e1000000000000000021202208"
                    + "0000000000000150220000300000002c01010004"
                    + "0300000c0100000c800e00800300000803000002"
                    + "0300000804000002000000080200000228000088"
                    + "00020000b4a2faf4bb54878ae21d638512ece55d"
                    + "9236fc5046ab6cef82220f421f3ce6361faf3656"
                    + "4ecb6d28798a94aad7b2b4b603ddeaaa5630adb9"
                    + "ece8ac37534036040610ebdd92f46bef84f0be7d"
                    + "b860351843858f8acf87056e272377f70c9f2d81"
                    + "e29c7b0ce4f291a3a72476bb0b278fd4b7b0a4c2"
                    + "6bbeb08214c707137607958729000024c39b7f36"
                    + "8f4681b89fa9b7be6465abd7c5f68b6ed5d3b4c7"
                    + "2cb4240eb5c464122900001c00004004e54f73b7"
                    + "d83f6beb881eab2051d8663f421d10b02b00001c"
                    + "00004005d915368ca036004cb578ae3e3fb26850"
                    + "9aeab19000000020699369228741c6d4ca094c93"
                    + "e242c9de19e7b7c60000000500000500";
    private static final String PSK_NONCE_RESP_HEX_STRING =
            "9756112ca539f5c25abacc7ee92b73091942a9c06950f98848f1af1694c4ddff";
    private static final String PSK_INIT_SIGNED_OCTETS =
            "5F54BF6D8B48E6E1000000000000000021202208"
                    + "0000000000000150220000300000002C01010004"
                    + "0300000C0100000C800E00800300000803000002"
                    + "0300000804000002000000080200000228000088"
                    + "00020000B4A2FAF4BB54878AE21D638512ECE55D"
                    + "9236FC5046AB6CEF82220F421F3CE6361FAF3656"
                    + "4ECB6D28798A94AAD7B2B4B603DDEAAA5630ADB9"
                    + "ECE8AC37534036040610EBDD92F46BEF84F0BE7D"
                    + "B860351843858F8ACF87056E272377F70C9F2D81"
                    + "E29C7B0CE4F291A3A72476BB0B278FD4B7B0A4C2"
                    + "6BBEB08214C707137607958729000024C39B7F36"
                    + "8F4681B89FA9B7BE6465ABD7C5F68B6ED5D3B4C7"
                    + "2CB4240EB5C464122900001C00004004E54F73B7"
                    + "D83F6BEB881EAB2051D8663F421D10B02B00001C"
                    + "00004005D915368CA036004CB578AE3E3FB26850"
                    + "9AEAB19000000020699369228741C6D4CA094C93"
                    + "E242C9DE19E7B7C600000005000005009756112C"
                    + "A539F5C25ABACC7EE92B73091942A9C06950F988"
                    + "48F1AF1694C4DDFFD83B20CC6A0932B2A7CEF26E"
                    + "4020ABAAB64F0C6A";

    private static final int AUTH_METHOD_POSITION = 0;

    private static final String PRF_HMAC_SHA1_ALGO_NAME = "HmacSHA1";

    @Test
    public void testDecodeIkeAuthPayload() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(PSK_AUTH_PAYLOAD_HEX_STRING);
        IkeAuthPayload payload = IkeAuthPayload.getIkeAuthPayload(false, inputPacket);

        assertEquals(IkeAuthPayload.AUTH_METHOD_PRE_SHARED_KEY, payload.authMethod);
        assertTrue(payload instanceof IkeAuthPskPayload);

        byte[] expectedSignature =
                TestUtils.hexStringToByteArray(PSK_AUTH_PAYLOAD_SIGNATURE_HEX_STRING);
        assertArrayEquals(expectedSignature, ((IkeAuthPskPayload) payload).signature);
    }

    @Test
    public void testDecodeIkeAuthPayloadWithUnsupportedMethod() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(PSK_AUTH_PAYLOAD_HEX_STRING);
        inputPacket[AUTH_METHOD_POSITION] = 0;
        try {
            IkeAuthPayload payload = IkeAuthPayload.getIkeAuthPayload(false, inputPacket);
            fail("Expected Exception: authentication method is not supported");
        } catch (UnsupportedOperationException e) {
            // TODO: Catch AuthenticationFailedException after it is implemented.
        }
    }

    @Test
    public void testSignWithPrf() throws Exception {
        Mac prfMac = Mac.getInstance(PRF_HMAC_SHA1_ALGO_NAME, IkeMessage.getSecurityProvider());
        byte[] skpBytes = TestUtils.hexStringToByteArray(PSK_SKP_HEX_STRING);
        byte[] idBytes = TestUtils.hexStringToByteArray(PSK_ID_PAYLOAD_HEX_STRING);
        byte[] calculatedBytes = IkeAuthPayload.signWithPrf(prfMac, skpBytes, idBytes);

        byte[] expectedBytes =
                TestUtils.hexStringToByteArray(PSK_SIGNED_OCTETS_APPENDIX_HEX_STRING);
        assertArrayEquals(expectedBytes, calculatedBytes);
    }

    @Test
    public void testGetSignedOctets() throws Exception {
        Mac prfMac = Mac.getInstance(PRF_HMAC_SHA1_ALGO_NAME, IkeMessage.getSecurityProvider());
        byte[] skpBytes = TestUtils.hexStringToByteArray(PSK_SKP_HEX_STRING);
        byte[] idBytes = TestUtils.hexStringToByteArray(PSK_ID_PAYLOAD_HEX_STRING);
        byte[] ikeInitRequest = TestUtils.hexStringToByteArray(PSK_IKE_INIT_REQUEST_HEX_STRING);
        byte[] nonceResp = TestUtils.hexStringToByteArray(PSK_NONCE_RESP_HEX_STRING);

        byte[] calculatedBytes =
                IkeAuthPayload.getSignedOctets(
                        ikeInitRequest, nonceResp, idBytes, prfMac, skpBytes);
        byte[] expectedBytes = TestUtils.hexStringToByteArray(PSK_INIT_SIGNED_OCTETS);
    }
}
