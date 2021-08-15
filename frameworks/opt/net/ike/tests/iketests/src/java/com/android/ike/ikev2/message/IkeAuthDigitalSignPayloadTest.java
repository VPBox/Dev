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

public final class IkeAuthDigitalSignPayloadTest {

    private static final String AUTH_PAYLOAD_BODY_GENERIC_DIGITAL_SIGN_HEX_STRING =
            "0e0000000f300d06092a864886f70d01010b05007b2f4456878b1344e803f094"
                    + "159a59361bc639071b69de41915452c478b77a46ce4a2c96ddc7ba2c18d08406"
                    + "50ce51c77124605423a2f75d8ed4b5a1ec5944c3396221a39e25def09abe5c9f"
                    + "6d9cd70e8f6254d4c835015256c9d6c26f0c6d31ac96a2ed802ccb16e48e7ff3"
                    + "daf736221b18c2a972130a69edb197a505a312882baed95d38a47bf6784533f2"
                    + "ffee671d742b5ae463216e46ef970ee6a335ffb3fc9c170a680fb802bb950cb0"
                    + "5601339be8869a73f8f85254d792b6e91697d8893ccd34b5fb6aad6268c4ab0f"
                    + "9ead7b3f8a4a255e1b2eabfa3da0de284f3954cf49271918dd2d2db95c8e7812"
                    + "9aea77e5761ac5683a0b5af300ceb52f5e8d8168";
    // TODO: Build a RSA_SHA1 signature and add tests for it.

    @Test
    public void testDecodeGenericDigitalSignPayload() throws Exception {
        byte[] inputPacket =
                TestUtils.hexStringToByteArray(AUTH_PAYLOAD_BODY_GENERIC_DIGITAL_SIGN_HEX_STRING);
        IkeAuthPayload payload = IkeAuthPayload.getIkeAuthPayload(false, inputPacket);

        assertTrue(payload instanceof IkeAuthDigitalSignPayload);
        IkeAuthDigitalSignPayload dsPayload = (IkeAuthDigitalSignPayload) payload;
        assertEquals(
                IkeAuthDigitalSignPayload.SIGNATURE_ALGO_RSA_SHA2_256,
                dsPayload.signatureAlgoAndHash);
    }
}
