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

import org.junit.Test;

import java.nio.ByteBuffer;

public final class IkeNoncePayloadTest {

    private static final String NONCE_PAYLOAD_RAW_HEX_STRING =
            "29000024c39b7f368f4681b89fa9b7be6465abd7c5f68b6ed5d3b4c72cb4240eb5c46412";
    private static final String NONCE_DATA_RAW_HEX_STRING =
            "c39b7f368f4681b89fa9b7be6465abd7c5f68b6ed5d3b4c72cb4240eb5c46412";

    @IkePayload.PayloadType
    private static final int NEXT_PAYLOAD_TYPE = IkePayload.PAYLOAD_TYPE_NOTIFY;

    @Test
    public void testEncode() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(NONCE_DATA_RAW_HEX_STRING);
        IkeNoncePayload payload = new IkeNoncePayload(false, inputPacket);

        ByteBuffer byteBuffer = ByteBuffer.allocate(payload.getPayloadLength());
        payload.encodeToByteBuffer(NEXT_PAYLOAD_TYPE, byteBuffer);

        byte[] expectedNoncePayload =
                TestUtils.hexStringToByteArray(NONCE_PAYLOAD_RAW_HEX_STRING);
        assertArrayEquals(expectedNoncePayload, byteBuffer.array());
    }
}
