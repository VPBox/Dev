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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import java.nio.ByteBuffer;

public final class IkeTsPayloadTest {
    private static final String TS_INITIATOR_PAYLOAD_HEX_STRING =
            "2d00002802000000070000100000ffff00000000ffffffff070000100000ffff00000001fffffffe";
    private static final int NUMBER_OF_TS = 2;

    @Test
    public void testDecodeTsInitiatorPayload() throws Exception {
        ByteBuffer inputBuffer =
                ByteBuffer.wrap(TestUtils.hexStringToByteArray(TS_INITIATOR_PAYLOAD_HEX_STRING));

        IkePayload payload =
                IkePayloadFactory.getIkePayload(
                                IkePayload.PAYLOAD_TYPE_TS_INITIATOR, false, inputBuffer)
                        .first;
        assertTrue(payload instanceof IkeTsPayload);

        IkeTsPayload tsPayload = (IkeTsPayload) payload;
        assertEquals(IkePayload.PAYLOAD_TYPE_TS_INITIATOR, tsPayload.payloadType);
        assertEquals(NUMBER_OF_TS, tsPayload.numTs);
    }
}
