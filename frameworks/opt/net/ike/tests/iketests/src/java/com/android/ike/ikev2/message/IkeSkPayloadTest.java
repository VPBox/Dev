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

import org.junit.Before;
import org.junit.Test;

import java.nio.ByteBuffer;
import java.util.Arrays;

import javax.crypto.Cipher;
import javax.crypto.Mac;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;

public final class IkeSkPayloadTest {

    private static final String IKE_AUTH_INIT_REQUEST_HEX_STRING =
            "5f54bf6d8b48e6e1909232b3d1edcb5c2e20230800000001000000ec"
                    + "230000d0b9132b7bb9f658dfdc648e5017a6322a030c316c"
                    + "e55f365760d46426ce5cfc78bd1ed9abff63eb9594c1bd58"
                    + "46de333ecd3ea2b705d18293b130395300ba92a351041345"
                    + "0a10525cea51b2753b4e92b081fd78d995659a98f742278f"
                    + "f9b8fd3e21554865c15c79a5134d66b2744966089e416c60"
                    + "a274e44a9a3f084eb02f3bdce1e7de9de8d9a62773ab563b"
                    + "9a69ba1db03c752acb6136452b8a86c41addb4210d68c423"
                    + "efed80e26edca5fa3fe5d0a5ca9375ce332c474b93fb1fa3"
                    + "59eb4e81ae6e0f22abdad69ba8007d50";

    private static final String IKE_AUTH_INIT_REQUEST_DECRYPTED_BODY_HEX_STRING =
            "2400000c010000000a50500d2700000c010000000a505050"
                    + "2100001c02000000df7c038aefaaa32d3f44b228b52a3327"
                    + "44dfb2c12c00002c00000028010304032ad4c0a20300000c"
                    + "0100000c800e008003000008030000020000000805000000"
                    + "2d00001801000000070000100000ffff00000000ffffffff"
                    + "2900001801000000070000100000ffff00000000ffffffff"
                    + "29000008000040000000000c0000400100000001";

    private static final String ENCR_KEY_FROM_INIT_TO_RESP = "5cbfd33f75796c0188c4a3a546aec4a1";
    private static final String INTE_KEY_FROM_INIT_TO_RESP =
            "554fbf5a05b7f511e05a30ce23d874db9ef55e51";

    private static final String ENCR_ALGO_AES_CBC = "AES/CBC/NoPadding";
    private static final String INTE_ALGO_HMAC_SHA1 = "HmacSHA1";

    private static final int CHECKSUM_LEN = 12;

    private Cipher mAesCbcDecryptCipher;
    private SecretKey mAesCbcDecryptKey;
    private Mac mHmacSha1IntegrityMac;

    @Before
    public void setUp() throws Exception {
        mAesCbcDecryptCipher =
                Cipher.getInstance(ENCR_ALGO_AES_CBC, IkeMessage.getSecurityProvider());
        byte[] decryptKeyBytes = TestUtils.hexStringToByteArray(ENCR_KEY_FROM_INIT_TO_RESP);
        mAesCbcDecryptKey = new SecretKeySpec(decryptKeyBytes, ENCR_ALGO_AES_CBC);

        mHmacSha1IntegrityMac =
                Mac.getInstance(INTE_ALGO_HMAC_SHA1, IkeMessage.getSecurityProvider());
        byte[] integrityKeyBytes = TestUtils.hexStringToByteArray(INTE_KEY_FROM_INIT_TO_RESP);
        SecretKeySpec integrityKey = new SecretKeySpec(integrityKeyBytes, INTE_ALGO_HMAC_SHA1);
        mHmacSha1IntegrityMac.init(integrityKey);
    }

    @Test
    public void testEncode() throws Exception {
        byte[] message = TestUtils.hexStringToByteArray(IKE_AUTH_INIT_REQUEST_HEX_STRING);
        byte[] payloadBytes =
                Arrays.copyOfRange(message, IkeHeader.IKE_HEADER_LENGTH, message.length);

        IkeSkPayload payload =
                IkePayloadFactory.getIkeSkPayload(
                                message,
                                mHmacSha1IntegrityMac,
                                CHECKSUM_LEN,
                                mAesCbcDecryptCipher,
                                mAesCbcDecryptKey)
                        .first;
        int payloadLength = payload.getPayloadLength();
        ByteBuffer buffer = ByteBuffer.allocate(payloadLength);
        payload.encodeToByteBuffer(IkePayload.PAYLOAD_TYPE_ID_INITIATOR, buffer);
        assertArrayEquals(payloadBytes, buffer.array());
    }
}
