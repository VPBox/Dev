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

import com.android.ike.ikev2.exceptions.InvalidSyntaxException;

import org.junit.Test;

import java.net.InetAddress;
import java.nio.ByteBuffer;

public final class IkeNotifyPayloadTest {
    private static final String NOTIFY_PAYLOAD_GENERIC_HEADER = "2900001c";
    private static final String NOTIFY_PAYLOAD_BODY_RAW_PACKET =
            "00004004e54f73b7d83f6beb881eab2051d8663f421d10b0";

    private static final String NAT_DETECTION_SOURCE_IP_DATA_HEX_STRING =
            "e54f73b7d83f6beb881eab2051d8663f421d10b0";
    private static final String IKE_INITIATOR_SPI_HEX_STRING = "5f54bf6d8b48e6e1";
    private static final String IKE_RESPODNER_SPI_HEX_STRING = "0000000000000000";
    private static final String IP_ADDR = "10.80.80.13";
    private static final int PORT = 500;

    private static final int EXPECTED_PROTOCOL_ID = IkePayload.PROTOCOL_ID_UNSET;
    private static final int EXPECTED_SPI_SIZE = IkePayload.SPI_LEN_NOT_INCLUDED;

    @IkePayload.PayloadType
    private static final int NEXT_PAYLOAD_TYPE = IkePayload.PAYLOAD_TYPE_NOTIFY;

    @IkeNotifyPayload.NotifyType
    private static final int EXPECTED_NOTIFY_TYPE =
            IkeNotifyPayload.NOTIFY_TYPE_NAT_DETECTION_SOURCE_IP;

    private static final int EXPECTED_NOTIFY_DATA_LEN = 20;

    private static final int POS_PROTOCOL_ID = 0;

    @Test
    public void testDecodeNotifyPayload() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(NOTIFY_PAYLOAD_BODY_RAW_PACKET);
        IkeNotifyPayload payload = new IkeNotifyPayload(false, inputPacket);
        assertEquals(EXPECTED_PROTOCOL_ID, payload.protocolId);
        assertEquals(EXPECTED_SPI_SIZE, payload.spiSize);
        assertEquals(EXPECTED_NOTIFY_TYPE, payload.notifyType);
        assertEquals(EXPECTED_SPI_SIZE, payload.spi);
        assertEquals(EXPECTED_NOTIFY_DATA_LEN, payload.notifyData.length);
    }

    @Test
    public void testDecodeNotifyPayloadThrowException() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(NOTIFY_PAYLOAD_BODY_RAW_PACKET);
        // Change Protocol ID to ESP
        inputPacket[POS_PROTOCOL_ID] = (byte) (IkePayload.PROTOCOL_ID_ESP & 0xFF);
        try {
            IkeNotifyPayload payload = new IkeNotifyPayload(false, inputPacket);
            fail("Expected InvalidSyntaxException: Protocol ID should not be ESP");
        } catch (InvalidSyntaxException expected) {
        }
    }

    @Test
    public void testGenerateNatDetectionData() throws Exception {
        long initiatorIkeSpi = Long.parseLong(IKE_INITIATOR_SPI_HEX_STRING, 16);
        long responderIkespi = Long.parseLong(IKE_RESPODNER_SPI_HEX_STRING, 16);
        InetAddress inetAddress = InetAddress.getByName(IP_ADDR);

        byte[] netDetectionData =
                IkeNotifyPayload.generateNatDetectionData(
                        initiatorIkeSpi, responderIkespi, inetAddress, PORT);

        byte[] expectedBytes =
                TestUtils.hexStringToByteArray(NAT_DETECTION_SOURCE_IP_DATA_HEX_STRING);
        assertArrayEquals(expectedBytes, netDetectionData);
    }

    @Test
    public void testEncodeNotifyPayload() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(NOTIFY_PAYLOAD_BODY_RAW_PACKET);
        IkeNotifyPayload payload = new IkeNotifyPayload(false, inputPacket);

        ByteBuffer byteBuffer = ByteBuffer.allocate(payload.getPayloadLength());
        payload.encodeToByteBuffer(NEXT_PAYLOAD_TYPE, byteBuffer);

        byte[] expectedNoncePayload =
                TestUtils.hexStringToByteArray(
                        NOTIFY_PAYLOAD_GENERIC_HEADER + NOTIFY_PAYLOAD_BODY_RAW_PACKET);
        assertArrayEquals(expectedNoncePayload, byteBuffer.array());
    }
}
