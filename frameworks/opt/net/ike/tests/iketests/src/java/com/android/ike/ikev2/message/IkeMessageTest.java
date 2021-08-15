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
import static org.junit.Assert.fail;

import com.android.ike.ikev2.exceptions.IkeException;
import com.android.ike.ikev2.exceptions.InvalidSyntaxException;
import com.android.ike.ikev2.exceptions.UnsupportedCriticalPayloadException;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.nio.ByteBuffer;

public final class IkeMessageTest {
    private static final String IKE_SA_INIT_HEADER_RAW_PACKET =
            "8f54bf6d8b48e6e10000000000000000212022080000000000000150";
    private static final String IKE_SA_INIT_BODY_RAW_PACKET =
            "220000300000002c010100040300000c0100000c"
                    + "800e00800300000803000002030000080400000200000008"
                    + "020000022800008800020000b4a2faf4bb54878ae21d6385"
                    + "12ece55d9236fc5046ab6cef82220f421f3ce6361faf3656"
                    + "4ecb6d28798a94aad7b2b4b603ddeaaa5630adb9ece8ac37"
                    + "534036040610ebdd92f46bef84f0be7db860351843858f8a"
                    + "cf87056e272377f70c9f2d81e29c7b0ce4f291a3a72476bb"
                    + "0b278fd4b7b0a4c26bbeb08214c707137607958729000024"
                    + "c39b7f368f4681b89fa9b7be6465abd7c5f68b6ed5d3b4c7"
                    + "2cb4240eb5c464122900001c00004004e54f73b7d83f6beb"
                    + "881eab2051d8663f421d10b02b00001c00004005d915368c"
                    + "a036004cb578ae3e3fb268509aeab1900000002069936922"
                    + "8741c6d4ca094c93e242c9de19e7b7c60000000500000500";
    private static final String IKE_SA_INIT_RAW_PACKET =
            IKE_SA_INIT_HEADER_RAW_PACKET + IKE_SA_INIT_BODY_RAW_PACKET;

    // Byte offsets of first payload type in IKE message header.
    private static final int FIRST_PAYLOAD_TYPE_OFFSET = 16;
    // Byte offsets of first payload's critical bit in IKE message body.
    private static final int PAYLOAD_CRITICAL_BIT_OFFSET = 1;
    // Byte offsets of first payload length in IKE message body.
    private static final int FIRST_PAYLOAD_LENGTH_OFFSET = 2;
    // Byte offsets of last payload length in IKE message body.
    private static final int LAST_PAYLOAD_LENGTH_OFFSET = 278;

    private static final int[] SUPPORTED_PAYLOAD_LIST = {
        IkePayload.PAYLOAD_TYPE_SA,
        IkePayload.PAYLOAD_TYPE_KE,
        IkePayload.PAYLOAD_TYPE_NONCE,
        IkePayload.PAYLOAD_TYPE_NOTIFY,
        IkePayload.PAYLOAD_TYPE_NOTIFY,
        IkePayload.PAYLOAD_TYPE_VENDOR
    };

    class TestIkeSupportedPayload extends IkePayload {
        TestIkeSupportedPayload(int payload, boolean critical) {
            super(payload, critical);
        }

        @Override
        protected void encodeToByteBuffer(@PayloadType int nextPayload, ByteBuffer byteBuffer) {
            throw new UnsupportedOperationException(
                    "It is not supported to encode " + getTypeString());
        }

        @Override
        protected int getPayloadLength() {
            throw new UnsupportedOperationException(
                    "It is not supported to get payload length of " + getTypeString());
        }

        @Override
        public String getTypeString() {
            return "Test Payload";
        }
    }

    @Before
    public void setUp() {
        IkePayloadFactory.sDecoderInstance =
                new IkePayloadFactory.IIkePayloadDecoder() {

                    @Override
                    public IkePayload decodeIkePayload(
                            int payloadType, boolean isCritical, boolean isResp, byte[] payloadBody)
                            throws IkeException {
                        if (support(payloadType)) {
                            return new TestIkeSupportedPayload(payloadType, isCritical);
                        } else {
                            return new IkeUnsupportedPayload(payloadType, isCritical);
                        }
                    }
                };
    }

    @After
    public void tearDown() {
        IkePayloadFactory.sDecoderInstance = new IkePayloadFactory.IkePayloadDecoder();
    }

    @Test
    public void testDecodeIkeMessage() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(IKE_SA_INIT_RAW_PACKET);
        IkeHeader header = new IkeHeader(inputPacket);
        IkeMessage message = IkeMessage.decode(header, inputPacket);
        assertEquals(SUPPORTED_PAYLOAD_LIST.length, message.ikePayloadList.size());
        for (int i = 0; i < SUPPORTED_PAYLOAD_LIST.length; i++) {
            assertEquals(SUPPORTED_PAYLOAD_LIST[i], message.ikePayloadList.get(i).payloadType);
        }
    }

    @Test
    public void testDecodeMessageWithUnsupportedUncriticalPayload() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(IKE_SA_INIT_RAW_PACKET);
        // Set first payload unsupported uncritical
        inputPacket[FIRST_PAYLOAD_TYPE_OFFSET] = (byte) 0xff;
        IkeHeader header = new IkeHeader(inputPacket);
        IkeMessage message = IkeMessage.decode(header, inputPacket);
        assertEquals(SUPPORTED_PAYLOAD_LIST.length - 1, message.ikePayloadList.size());
        for (int i = 0; i < SUPPORTED_PAYLOAD_LIST.length - 1; i++) {
            assertEquals(SUPPORTED_PAYLOAD_LIST[i + 1], message.ikePayloadList.get(i).payloadType);
        }
    }

    @Test
    public void testThrowUnsupportedCriticalPayloadException() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(IKE_SA_INIT_RAW_PACKET);
        // Set first payload unsupported critical
        inputPacket[FIRST_PAYLOAD_TYPE_OFFSET] = (byte) 0xff;
        inputPacket[IkeHeader.IKE_HEADER_LENGTH + PAYLOAD_CRITICAL_BIT_OFFSET] = (byte) 0x80;

        IkeHeader header = new IkeHeader(inputPacket);
        try {
            IkeMessage.decode(header, inputPacket);
            fail(
                    "Expected UnsupportedCriticalPayloadException: first"
                            + "payload is unsupported critical.");
        } catch (UnsupportedCriticalPayloadException expected) {
            assertEquals(1, expected.payloadTypeList.size());
        }
    }

    @Test
    public void testDecodeMessageWithTooShortPayloadLength() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(IKE_SA_INIT_RAW_PACKET);
        // Set first payload length to 0
        inputPacket[IkeHeader.IKE_HEADER_LENGTH + FIRST_PAYLOAD_LENGTH_OFFSET] = (byte) 0;
        inputPacket[IkeHeader.IKE_HEADER_LENGTH + FIRST_PAYLOAD_LENGTH_OFFSET + 1] = (byte) 0;
        IkeHeader header = new IkeHeader(inputPacket);
        try {
            IkeMessage message = IkeMessage.decode(header, inputPacket);
            fail("Expected InvalidSyntaxException: Payload length is too short.");
        } catch (InvalidSyntaxException expected) {
        }
    }

    @Test
    public void testDecodeMessageWithTooLongPayloadLength() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(IKE_SA_INIT_RAW_PACKET);
        // Increase last payload length by one byte
        inputPacket[IkeHeader.IKE_HEADER_LENGTH + LAST_PAYLOAD_LENGTH_OFFSET]++;
        IkeHeader header = new IkeHeader(inputPacket);
        try {
            IkeMessage message = IkeMessage.decode(header, inputPacket);
            fail("Expected InvalidSyntaxException: Payload length is too long.");
        } catch (InvalidSyntaxException expected) {
        }
    }

    @Test
    public void testDecodeMessageWithExpectedBytesInTheEnd() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(IKE_SA_INIT_RAW_PACKET + "0000");
        IkeHeader header = new IkeHeader(inputPacket);
        try {
            IkeMessage message = IkeMessage.decode(header, inputPacket);
            fail("Expected InvalidSyntaxException: Unexpected bytes in the end of packet.");
        } catch (InvalidSyntaxException expected) {
        }
    }

    private boolean support(int payloadType) {
        return (payloadType == IkePayload.PAYLOAD_TYPE_SA
                || payloadType == IkePayload.PAYLOAD_TYPE_KE
                || payloadType == IkePayload.PAYLOAD_TYPE_NONCE
                || payloadType == IkePayload.PAYLOAD_TYPE_NOTIFY
                || payloadType == IkePayload.PAYLOAD_TYPE_VENDOR
                || payloadType == IkePayload.PAYLOAD_TYPE_SK);
    }

    @Test
    public void testAttachEncodedHeader() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(IKE_SA_INIT_RAW_PACKET);
        byte[] ikeBodyBytes = TestUtils.hexStringToByteArray(IKE_SA_INIT_BODY_RAW_PACKET);
        IkeHeader header = new IkeHeader(inputPacket);
        IkeMessage message = IkeMessage.decode(header, inputPacket);

        byte[] encodedIkeMessage = message.attachEncodedHeader(ikeBodyBytes);
        assertArrayEquals(inputPacket, encodedIkeMessage);
    }

    // TODO: Implement encodeToByteBuffer() of each payload and add test for encoding message
}
