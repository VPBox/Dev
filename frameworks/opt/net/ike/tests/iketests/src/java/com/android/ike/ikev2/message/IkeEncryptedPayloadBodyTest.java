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

import org.junit.Before;
import org.junit.Test;

import java.security.GeneralSecurityException;
import java.util.Arrays;

import javax.crypto.Cipher;
import javax.crypto.Mac;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;

public final class IkeEncryptedPayloadBodyTest {

    private static final String IKE_AUTH_INIT_REQUEST_HEADER =
            "5f54bf6d8b48e6e1909232b3d1edcb5c2e20230800000001000000ec";
    private static final String IKE_AUTH_INIT_REQUEST_SK_HEADER = "230000d0";
    private static final String IKE_AUTH_INIT_REQUEST_IV = "b9132b7bb9f658dfdc648e5017a6322a";
    private static final String IKE_AUTH_INIT_REQUEST_ENCRYPT_PADDED_DATA =
            "030c316ce55f365760d46426ce5cfc78bd1ed9abff63eb9594c1bd58"
                    + "46de333ecd3ea2b705d18293b130395300ba92a351041345"
                    + "0a10525cea51b2753b4e92b081fd78d995659a98f742278f"
                    + "f9b8fd3e21554865c15c79a5134d66b2744966089e416c60"
                    + "a274e44a9a3f084eb02f3bdce1e7de9de8d9a62773ab563b"
                    + "9a69ba1db03c752acb6136452b8a86c41addb4210d68c423"
                    + "efed80e26edca5fa3fe5d0a5ca9375ce332c474b93fb1fa3"
                    + "59eb4e81";
    private static final String IKE_AUTH_INIT_REQUEST_CHECKSUM = "ae6e0f22abdad69ba8007d50";

    private static final String IKE_AUTH_INIT_REQUEST_UNENCRYPTED_DATA =
            "2400000c010000000a50500d2700000c010000000a505050"
                    + "2100001c02000000df7c038aefaaa32d3f44b228b52a3327"
                    + "44dfb2c12c00002c00000028010304032ad4c0a20300000c"
                    + "0100000c800e008003000008030000020000000805000000"
                    + "2d00001801000000070000100000ffff00000000ffffffff"
                    + "2900001801000000070000100000ffff00000000ffffffff"
                    + "29000008000040000000000c0000400100000001";
    private static final String IKE_AUTH_INIT_REQUEST_PADDING = "0000000000000000000000";
    private static final int HMAC_SHA1_CHECKSUM_LEN = 12;

    private static final String ENCR_KEY_FROM_INIT_TO_RESP = "5cbfd33f75796c0188c4a3a546aec4a1";
    private static final String INTE_KEY_FROM_INIT_TO_RESP =
            "554fbf5a05b7f511e05a30ce23d874db9ef55e51";

    private static final String ENCR_ALGO_AES_CBC = "AES/CBC/NoPadding";
    private static final String INTE_ALGO_HMAC_SHA1 = "HmacSHA1";

    private Cipher mAesCbcCipher;
    private SecretKey mAesCbcKey;
    private Mac mHmacSha1IntegrityMac;

    private byte[] mDataToPadAndEncrypt;
    private byte[] mDataToAuthenticate;
    private byte[] mEncryptedPaddedData;
    private byte[] mIkeMessage;

    private byte[] mChecksum;
    private byte[] mIv;
    private byte[] mPadding;

    // TODO: Add tests for authenticating and decrypting received message.
    @Before
    public void setUp() throws Exception {
        mDataToPadAndEncrypt =
                TestUtils.hexStringToByteArray(IKE_AUTH_INIT_REQUEST_UNENCRYPTED_DATA);
        String hexStringToAuthenticate =
                IKE_AUTH_INIT_REQUEST_HEADER
                        + IKE_AUTH_INIT_REQUEST_SK_HEADER
                        + IKE_AUTH_INIT_REQUEST_IV
                        + IKE_AUTH_INIT_REQUEST_ENCRYPT_PADDED_DATA;
        mDataToAuthenticate = TestUtils.hexStringToByteArray(hexStringToAuthenticate);
        mEncryptedPaddedData =
                TestUtils.hexStringToByteArray(IKE_AUTH_INIT_REQUEST_ENCRYPT_PADDED_DATA);
        mIkeMessage =
                TestUtils.hexStringToByteArray(
                        IKE_AUTH_INIT_REQUEST_HEADER
                                + IKE_AUTH_INIT_REQUEST_SK_HEADER
                                + IKE_AUTH_INIT_REQUEST_IV
                                + IKE_AUTH_INIT_REQUEST_ENCRYPT_PADDED_DATA
                                + IKE_AUTH_INIT_REQUEST_CHECKSUM);

        mChecksum = TestUtils.hexStringToByteArray(IKE_AUTH_INIT_REQUEST_CHECKSUM);
        mIv = TestUtils.hexStringToByteArray(IKE_AUTH_INIT_REQUEST_IV);
        mPadding = TestUtils.hexStringToByteArray(IKE_AUTH_INIT_REQUEST_PADDING);

        mAesCbcCipher = Cipher.getInstance(ENCR_ALGO_AES_CBC, IkeMessage.getSecurityProvider());
        byte[] encryptKeyBytes = TestUtils.hexStringToByteArray(ENCR_KEY_FROM_INIT_TO_RESP);
        mAesCbcKey = new SecretKeySpec(encryptKeyBytes, ENCR_ALGO_AES_CBC);

        mHmacSha1IntegrityMac =
                Mac.getInstance(INTE_ALGO_HMAC_SHA1, IkeMessage.getSecurityProvider());
        byte[] integrityKeyBytes = TestUtils.hexStringToByteArray(INTE_KEY_FROM_INIT_TO_RESP);
        SecretKeySpec integrityKey = new SecretKeySpec(integrityKeyBytes, INTE_ALGO_HMAC_SHA1);
        mHmacSha1IntegrityMac.init(integrityKey);
    }

    @Test
    public void testCalculateChecksum() throws Exception {
        byte[] calculatedChecksum =
                IkeEncryptedPayloadBody.calculateChecksum(
                        mDataToAuthenticate, mHmacSha1IntegrityMac, HMAC_SHA1_CHECKSUM_LEN);

        assertArrayEquals(mChecksum, calculatedChecksum);
    }

    @Test
    public void testValidateChecksum() throws Exception {
        IkeEncryptedPayloadBody.validateChecksumOrThrow(
                mDataToAuthenticate, mHmacSha1IntegrityMac, mChecksum);
    }

    @Test
    public void testThrowForInvalidChecksum() throws Exception {
        byte[] dataToAuthenticate = Arrays.copyOf(mDataToAuthenticate, mDataToAuthenticate.length);
        dataToAuthenticate[0]++;

        try {
            IkeEncryptedPayloadBody.validateChecksumOrThrow(
                    dataToAuthenticate, mHmacSha1IntegrityMac, mChecksum);
            fail("Expected GeneralSecurityException due to mismatched checksum.");
        } catch (GeneralSecurityException expected) {
        }
    }

    @Test
    public void testCalculatePaddingPlaintextShorterThanBlockSize() throws Exception {
        int blockSize = 16;
        int plainTextLength = 15;
        int expectedPadLength = 0;

        byte[] calculatedPadding =
                IkeEncryptedPayloadBody.calculatePadding(plainTextLength, blockSize);
        assertEquals(expectedPadLength, calculatedPadding.length);
    }

    @Test
    public void testCalculatePaddingPlaintextInBlockSize() throws Exception {
        int blockSize = 16;
        int plainTextLength = 16;
        int expectedPadLength = 15;

        byte[] calculatedPadding =
                IkeEncryptedPayloadBody.calculatePadding(plainTextLength, blockSize);
        assertEquals(expectedPadLength, calculatedPadding.length);
    }

    @Test
    public void testCalculatePaddingPlaintextLongerThanBlockSize() throws Exception {
        int blockSize = 16;
        int plainTextLength = 17;
        int expectedPadLength = 14;

        byte[] calculatedPadding =
                IkeEncryptedPayloadBody.calculatePadding(plainTextLength, blockSize);
        assertEquals(expectedPadLength, calculatedPadding.length);
    }

    @Test
    public void testEncrypt() throws Exception {
        byte[] calculatedData =
                IkeEncryptedPayloadBody.encrypt(
                        mDataToPadAndEncrypt, mAesCbcCipher, mAesCbcKey, mIv, mPadding);

        assertArrayEquals(mEncryptedPaddedData, calculatedData);
    }

    @Test
    public void testDecrypt() throws Exception {
        byte[] calculatedPlainText =
                IkeEncryptedPayloadBody.decrypt(
                        mEncryptedPaddedData, mAesCbcCipher, mAesCbcKey, mIv);

        assertArrayEquals(mDataToPadAndEncrypt, calculatedPlainText);
    }

    @Test
    public void testBuildAndEncodeOutboundIkeEncryptedPayloadBody() throws Exception {
        IkeHeader ikeHeader = new IkeHeader(mIkeMessage);

        IkeEncryptedPayloadBody paylaodBody =
                new IkeEncryptedPayloadBody(
                        ikeHeader,
                        IkePayload.PAYLOAD_TYPE_ID_INITIATOR,
                        mDataToPadAndEncrypt,
                        mHmacSha1IntegrityMac,
                        HMAC_SHA1_CHECKSUM_LEN,
                        mAesCbcCipher,
                        mAesCbcKey,
                        mIv,
                        mPadding);

        byte[] expectedEncodedData =
                TestUtils.hexStringToByteArray(
                        IKE_AUTH_INIT_REQUEST_IV
                                + IKE_AUTH_INIT_REQUEST_ENCRYPT_PADDED_DATA
                                + IKE_AUTH_INIT_REQUEST_CHECKSUM);
        assertArrayEquals(expectedEncodedData, paylaodBody.encode());
    }

    @Test
    public void testAuthenticateAndDecryptInboundIkeEncryptedPayloadBody() throws Exception {
        IkeEncryptedPayloadBody paylaodBody =
                new IkeEncryptedPayloadBody(
                        mIkeMessage,
                        mHmacSha1IntegrityMac,
                        HMAC_SHA1_CHECKSUM_LEN,
                        mAesCbcCipher,
                        mAesCbcKey);

        assertArrayEquals(mDataToPadAndEncrypt, paylaodBody.getUnencryptedData());
    }
}
