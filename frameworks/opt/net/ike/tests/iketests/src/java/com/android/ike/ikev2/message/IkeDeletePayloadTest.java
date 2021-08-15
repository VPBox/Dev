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
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import com.android.ike.ikev2.exceptions.InvalidSyntaxException;

import org.junit.Test;

import java.nio.ByteBuffer;

public final class IkeDeletePayloadTest {
    private static final String DELETE_IKE_PAYLOAD_HEX_STRING = "0000000801000000";
    private static final String DELETE_CHILD_PAYLOAD_HEX_STRING = "0000000c030400012ad4c0a2";
    private static final String CHILD_SPI = "2ad4c0a2";

    private static final int NUM_CHILD_SPI = 1;

    private static final int PROTOCOL_ID_OFFSET = 4;
    private static final int SPI_SIZE_OFFSET = 5;
    private static final int NUM_OF_SPI_OFFSET = 6;

    @Test
    public void testDecodeDeleteIkePayload() throws Exception {
        ByteBuffer inputBuffer =
                ByteBuffer.wrap(TestUtils.hexStringToByteArray(DELETE_IKE_PAYLOAD_HEX_STRING));

        IkePayload payload =
                IkePayloadFactory.getIkePayload(
                                IkePayload.PAYLOAD_TYPE_DELETE, false /*is request*/, inputBuffer)
                        .first;

        assertTrue(payload instanceof IkeDeletePayload);

        IkeDeletePayload deletePayload = (IkeDeletePayload) payload;
        assertEquals(IkePayload.PROTOCOL_ID_IKE, deletePayload.protocolId);
        assertEquals(IkePayload.SPI_LEN_NOT_INCLUDED, deletePayload.spiSize);
        assertEquals(0, deletePayload.numSpi);
        assertArrayEquals(new int[0], deletePayload.spisToDelete);
    }

    @Test
    public void testDecodeDeleteChildPayload() throws Exception {
        ByteBuffer inputBuffer =
                ByteBuffer.wrap(TestUtils.hexStringToByteArray(DELETE_CHILD_PAYLOAD_HEX_STRING));

        IkePayload payload =
                IkePayloadFactory.getIkePayload(
                                IkePayload.PAYLOAD_TYPE_DELETE, false /*is request*/, inputBuffer)
                        .first;

        assertTrue(payload instanceof IkeDeletePayload);

        IkeDeletePayload deletePayload = (IkeDeletePayload) payload;
        assertEquals(IkePayload.PROTOCOL_ID_ESP, deletePayload.protocolId);
        assertEquals(IkePayload.SPI_LEN_IPSEC, deletePayload.spiSize);
        assertEquals(NUM_CHILD_SPI, deletePayload.numSpi);

        byte[] childSpiBytes = TestUtils.hexStringToByteArray(CHILD_SPI);
        ByteBuffer buffer = ByteBuffer.wrap(childSpiBytes);
        int expectedChildSpi = buffer.getInt();

        assertArrayEquals(new int[] {expectedChildSpi}, deletePayload.spisToDelete);
    }

    @Test
    public void testDecodeWithInvalidProtocol() throws Exception {
        byte[] deletePayloadBytes = TestUtils.hexStringToByteArray(DELETE_IKE_PAYLOAD_HEX_STRING);
        deletePayloadBytes[PROTOCOL_ID_OFFSET] = -1;
        ByteBuffer inputBuffer = ByteBuffer.wrap(deletePayloadBytes);

        try {
            IkePayloadFactory.getIkePayload(
                    IkePayload.PAYLOAD_TYPE_DELETE, false /*is request*/, inputBuffer);
            fail("Expected to fail due to unrecognized protocol ID.");
        } catch (InvalidSyntaxException expected) {

        }
    }

    @Test
    public void testDecodeWithInvalidSpiSize() throws Exception {
        byte[] deletePayloadBytes = TestUtils.hexStringToByteArray(DELETE_IKE_PAYLOAD_HEX_STRING);
        deletePayloadBytes[SPI_SIZE_OFFSET] = IkePayload.SPI_LEN_IPSEC;
        ByteBuffer inputBuffer = ByteBuffer.wrap(deletePayloadBytes);

        try {
            IkePayloadFactory.getIkePayload(
                    IkePayload.PAYLOAD_TYPE_DELETE, false /*is request*/, inputBuffer);
            fail("Expected to fail due to invalid SPI size in Delete IKE Payload.");
        } catch (InvalidSyntaxException expected) {

        }
    }

    @Test
    public void testDecodeWithInvalidNumSpi() throws Exception {
        byte[] deletePayloadBytes = TestUtils.hexStringToByteArray(DELETE_IKE_PAYLOAD_HEX_STRING);
        deletePayloadBytes[NUM_OF_SPI_OFFSET] = 1;
        ByteBuffer inputBuffer = ByteBuffer.wrap(deletePayloadBytes);

        try {
            IkePayloadFactory.getIkePayload(
                    IkePayload.PAYLOAD_TYPE_DELETE, false /*is request*/, inputBuffer);
            fail("Expected to fail because number of SPI is not zero in Delete IKE Payload.");
        } catch (InvalidSyntaxException expected) {

        }
    }

    @Test
    public void testDecodeWithInvalidNumSpiAndSpiSize() throws Exception {
        byte[] deletePayloadBytes = TestUtils.hexStringToByteArray(DELETE_IKE_PAYLOAD_HEX_STRING);
        deletePayloadBytes[SPI_SIZE_OFFSET] = 1;
        deletePayloadBytes[NUM_CHILD_SPI] = 4;
        ByteBuffer inputBuffer = ByteBuffer.wrap(deletePayloadBytes);

        try {
            IkePayloadFactory.getIkePayload(
                    IkePayload.PAYLOAD_TYPE_DELETE, false /*is request*/, inputBuffer);
            fail("Expected to fail due to invalid SPI size in Delete IKE Payload.");
        } catch (InvalidSyntaxException expected) {

        }
    }
}
