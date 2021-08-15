/*
 * Copyright (C) 2019 The Android Open Source Project
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
import static org.junit.Assert.fail;

import com.android.ike.ikev2.exceptions.AuthenticationFailedException;

import org.junit.Test;

import java.nio.ByteBuffer;
import java.util.Arrays;

import javax.crypto.Mac;

public final class IkeAuthPskPayloadTest {
    private static final String PSK_AUTH_PAYLOAD_HEX_STRING =
            "2100001c02000000df7c038aefaaa32d3f44b228b52a332744dfb2c1";
    private static final String PSK_AUTH_PAYLOAD_BODY_HEX_STRING =
            "02000000df7c038aefaaa32d3f44b228b52a332744dfb2c1";
    private static final String PSK_AUTH_PAYLOAD_SIGNATURE_HEX_STRING =
            "df7c038aefaaa32d3f44b228b52a332744dfb2c1";

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
    private static final String PSK_ID_INITIATOR_PAYLOAD_HEX_STRING = "010000000a50500d";

    private static final String PSK_HEX_STRING = "6A756E69706572313233";
    private static final String PSK_SKP_HEX_STRING = "094787780EE466E2CB049FA327B43908BC57E485";

    private static final String PRF_HMAC_SHA1_ALGO_NAME = "HmacSHA1";

    private static final byte[] PSK = TestUtils.hexStringToByteArray(PSK_HEX_STRING);
    private static final byte[] IKE_INIT_REQUEST =
            TestUtils.hexStringToByteArray(PSK_IKE_INIT_REQUEST_HEX_STRING);
    private static final byte[] NONCE = TestUtils.hexStringToByteArray(PSK_NONCE_RESP_HEX_STRING);
    private static final byte[] ID_PAYLOAD_BODY =
            TestUtils.hexStringToByteArray(PSK_ID_INITIATOR_PAYLOAD_HEX_STRING);
    private static final byte[] PRF_KEY = TestUtils.hexStringToByteArray(PSK_SKP_HEX_STRING);
    private static final byte[] SIGNATURE =
            TestUtils.hexStringToByteArray(PSK_AUTH_PAYLOAD_SIGNATURE_HEX_STRING);

    @Test
    public void testBuildOutboundIkeAuthPskPayload() throws Exception {
        Mac prfMac = Mac.getInstance(PRF_HMAC_SHA1_ALGO_NAME, IkeMessage.getSecurityProvider());

        IkeAuthPskPayload payload =
                new IkeAuthPskPayload(
                        PSK, IKE_INIT_REQUEST, NONCE, ID_PAYLOAD_BODY, prfMac, PRF_KEY);

        assertEquals(IkeAuthPayload.AUTH_METHOD_PRE_SHARED_KEY, payload.authMethod);
        assertArrayEquals(SIGNATURE, payload.signature);

        // Verify payload length
        int payloadLength = payload.getPayloadLength();
        byte[] expectedPayload = TestUtils.hexStringToByteArray(PSK_AUTH_PAYLOAD_HEX_STRING);
        assertEquals(expectedPayload.length, payloadLength);

        // Verify encoding
        ByteBuffer byteBuffer = ByteBuffer.allocate(payloadLength);
        payload.encodeToByteBuffer(IkePayload.PAYLOAD_TYPE_SA, byteBuffer);
        assertArrayEquals(expectedPayload, byteBuffer.array());
    }

    private IkeAuthPskPayload buildPskPayload() throws Exception {
        byte[] payloadBody = TestUtils.hexStringToByteArray(PSK_AUTH_PAYLOAD_BODY_HEX_STRING);
        IkeAuthPskPayload pskPayload =
                (IkeAuthPskPayload) IkeAuthPayload.getIkeAuthPayload(false, payloadBody);
        return pskPayload;
    }

    @Test
    public void testDecodeIkeAuthPskPayload() throws Exception {
        IkeAuthPskPayload pskPayload = buildPskPayload();

        assertArrayEquals(SIGNATURE, pskPayload.signature);
    }

    @Test
    public void testVerifyReceivedSignature() throws Exception {
        Mac prfMac = Mac.getInstance(PRF_HMAC_SHA1_ALGO_NAME, IkeMessage.getSecurityProvider());
        IkeAuthPskPayload pskPayload = buildPskPayload();

        pskPayload.verifyInboundSignature(
                PSK, IKE_INIT_REQUEST, NONCE, ID_PAYLOAD_BODY, prfMac, PRF_KEY);
    }

    @Test
    public void testVerifyReceivedSignatureFailure() throws Exception {
        Mac prfMac = Mac.getInstance(PRF_HMAC_SHA1_ALGO_NAME, IkeMessage.getSecurityProvider());
        IkeAuthPskPayload pskPayload = buildPskPayload();
        byte[] nonce = Arrays.copyOf(NONCE, NONCE.length);
        nonce[0]++;

        try {
            pskPayload.verifyInboundSignature(
                    PSK, IKE_INIT_REQUEST, nonce, ID_PAYLOAD_BODY, prfMac, PRF_KEY);
            fail("Expected signature verification to have failed due to mismatched signatures.");
        } catch (AuthenticationFailedException expected) {
        }
    }
}
