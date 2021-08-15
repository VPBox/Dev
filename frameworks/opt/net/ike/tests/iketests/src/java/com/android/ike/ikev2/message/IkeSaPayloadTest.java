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
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyInt;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

import android.util.Pair;

import com.android.ike.ikev2.SaProposal;
import com.android.ike.ikev2.exceptions.IkeException;
import com.android.ike.ikev2.exceptions.InvalidSyntaxException;
import com.android.ike.ikev2.exceptions.NoValidProposalChosenException;
import com.android.ike.ikev2.message.IkeSaPayload.Attribute;
import com.android.ike.ikev2.message.IkeSaPayload.AttributeDecoder;
import com.android.ike.ikev2.message.IkeSaPayload.DhGroupTransform;
import com.android.ike.ikev2.message.IkeSaPayload.EncryptionTransform;
import com.android.ike.ikev2.message.IkeSaPayload.EsnTransform;
import com.android.ike.ikev2.message.IkeSaPayload.IntegrityTransform;
import com.android.ike.ikev2.message.IkeSaPayload.KeyLengthAttribute;
import com.android.ike.ikev2.message.IkeSaPayload.PrfTransform;
import com.android.ike.ikev2.message.IkeSaPayload.Proposal;
import com.android.ike.ikev2.message.IkeSaPayload.Transform;
import com.android.ike.ikev2.message.IkeSaPayload.TransformDecoder;
import com.android.ike.ikev2.message.IkeSaPayload.UnrecognizedAttribute;
import com.android.ike.ikev2.message.IkeSaPayload.UnrecognizedTransform;

import org.junit.Before;
import org.junit.Test;

import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.Random;

public final class IkeSaPayloadTest {
    private static final String OUTBOUND_SA_PAYLOAD_HEADER = "22000030";
    private static final String OUTBOUND_PROPOSAL_RAW_PACKET =
            "0000002C010100040300000C0100000C800E0080030000080200000203000008030"
                    + "000020000000804000002";
    private static final String INBOUND_PROPOSAL_RAW_PACKET =
            "0000002c010100040300000c0100000c800e0080030000080300000203000008040"
                    + "000020000000802000002";
    private static final String INBOUND_TWO_PROPOSAL_RAW_PACKET =
            "020000dc010100190300000c0100000c800e00800300000c0100000c800e00c0030"
                    + "0000c0100000c800e01000300000801000003030000080300000c0300"
                    + "00080300000d030000080300000e03000008030000020300000803000"
                    + "005030000080200000503000008020000060300000802000007030000"
                    + "080200000403000008020000020300000804000013030000080400001"
                    + "40300000804000015030000080400001c030000080400001d03000008"
                    + "0400001e030000080400001f030000080400000f03000008040000100"
                    + "300000804000012000000080400000e000001000201001a0300000c01"
                    + "000014800e00800300000c01000014800e00c00300000c01000014800"
                    + "e01000300000c0100001c800e01000300000c01000013800e00800300"
                    + "000c01000013800e00c00300000c01000013800e01000300000c01000"
                    + "012800e00800300000c01000012800e00c00300000c01000012800e01"
                    + "000300000802000005030000080200000603000008020000070300000"
                    + "802000004030000080200000203000008040000130300000804000014"
                    + "0300000804000015030000080400001c030000080400001d030000080"
                    + "400001e030000080400001f030000080400000f030000080400001003"
                    + "00000804000012000000080400000e";
    private static final String ENCR_TRANSFORM_RAW_PACKET = "0300000c0100000c800e0080";
    private static final String PRF_TRANSFORM_RAW_PACKET = "0000000802000002";
    private static final String INTEG_TRANSFORM_RAW_PACKET = "0300000803000002";
    private static final String DH_GROUP_TRANSFORM_RAW_PACKET = "0300000804000002";
    private static final String ESN_TRANSFORM_RAW_PACKET = "0000000805000000";

    private static final int TRANSFORM_TYPE_OFFSET = 4;
    private static final int TRANSFORM_ID_OFFSET = 7;

    private static final String ATTRIBUTE_RAW_PACKET = "800e0080";

    private static final int PROPOSAL_NUMBER = 1;

    private static final int PROPOSAL_NUMBER_OFFSET = 4;
    private static final int PROTOCOL_ID_OFFSET = 5;

    // Constants for multiple proposals test
    private static final byte[] PROPOSAL_NUMBER_LIST = {1, 2};

    private AttributeDecoder mMockedAttributeDecoder;
    private KeyLengthAttribute mAttributeKeyLength128;
    private List<Attribute> mAttributeListWithKeyLength128;

    private EncryptionTransform mEncrAesCbc128Transform;
    private EncryptionTransform mEncrAesGcm8Key128Transform;
    private IntegrityTransform mIntegHmacSha1Transform;
    private PrfTransform mPrfHmacSha1Transform;
    private DhGroupTransform mDhGroup1024Transform;

    private Transform[] mValidNegotiatedTransformSet;

    private SaProposal mSaProposalOne;
    private SaProposal mSaProposalTwo;
    private SaProposal[] mTwoSaProposalsArray;

    @Before
    public void setUp() throws Exception {
        mMockedAttributeDecoder = mock(AttributeDecoder.class);
        mAttributeKeyLength128 = new KeyLengthAttribute(SaProposal.KEY_LEN_AES_128);
        mAttributeListWithKeyLength128 = new LinkedList<>();
        mAttributeListWithKeyLength128.add(mAttributeKeyLength128);

        mEncrAesCbc128Transform =
                new EncryptionTransform(
                        SaProposal.ENCRYPTION_ALGORITHM_AES_CBC, SaProposal.KEY_LEN_AES_128);
        mEncrAesGcm8Key128Transform =
                new EncryptionTransform(
                        SaProposal.ENCRYPTION_ALGORITHM_AES_GCM_8, SaProposal.KEY_LEN_AES_128);
        mIntegHmacSha1Transform =
                new IntegrityTransform(SaProposal.INTEGRITY_ALGORITHM_HMAC_SHA1_96);
        mPrfHmacSha1Transform = new PrfTransform(SaProposal.PSEUDORANDOM_FUNCTION_HMAC_SHA1);
        mDhGroup1024Transform = new DhGroupTransform(SaProposal.DH_GROUP_1024_BIT_MODP);

        mValidNegotiatedTransformSet =
                new Transform[] {
                    mEncrAesCbc128Transform,
                    mIntegHmacSha1Transform,
                    mPrfHmacSha1Transform,
                    mDhGroup1024Transform
                };

        mSaProposalOne =
                SaProposal.Builder.newIkeSaProposalBuilder()
                        .addEncryptionAlgorithm(
                                SaProposal.ENCRYPTION_ALGORITHM_AES_CBC, SaProposal.KEY_LEN_AES_128)
                        .addIntegrityAlgorithm(SaProposal.INTEGRITY_ALGORITHM_HMAC_SHA1_96)
                        .addDhGroup(SaProposal.DH_GROUP_1024_BIT_MODP)
                        .addPseudorandomFunction(SaProposal.PSEUDORANDOM_FUNCTION_HMAC_SHA1)
                        .build();

        mSaProposalTwo =
                SaProposal.Builder.newIkeSaProposalBuilder()
                        .addEncryptionAlgorithm(
                                SaProposal.ENCRYPTION_ALGORITHM_AES_GCM_8,
                                SaProposal.KEY_LEN_AES_128)
                        .addEncryptionAlgorithm(
                                SaProposal.ENCRYPTION_ALGORITHM_AES_GCM_12,
                                SaProposal.KEY_LEN_AES_128)
                        .addPseudorandomFunction(SaProposal.PSEUDORANDOM_FUNCTION_AES128_XCBC)
                        .addDhGroup(SaProposal.DH_GROUP_1024_BIT_MODP)
                        .addDhGroup(SaProposal.DH_GROUP_2048_BIT_MODP)
                        .build();
        mTwoSaProposalsArray = new SaProposal[] {mSaProposalOne, mSaProposalTwo};
    }

    // TODO: Add tearDown() to reset Proposal.sTransformDecoder and Transform.sAttributeDecoder.

    @Test
    public void testDecodeAttribute() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(ATTRIBUTE_RAW_PACKET);
        ByteBuffer inputBuffer = ByteBuffer.wrap(inputPacket);

        Pair<Attribute, Integer> pair = Attribute.readFrom(inputBuffer);
        Attribute attribute = pair.first;

        assertTrue(attribute instanceof KeyLengthAttribute);
        assertEquals(Attribute.ATTRIBUTE_TYPE_KEY_LENGTH, attribute.type);
        assertEquals(SaProposal.KEY_LEN_AES_128, ((KeyLengthAttribute) attribute).keyLength);
    }

    @Test
    public void testEncodeAttribute() throws Exception {
        ByteBuffer byteBuffer = ByteBuffer.allocate(mAttributeKeyLength128.getAttributeLength());
        mAttributeKeyLength128.encodeToByteBuffer(byteBuffer);

        byte[] expectedBytes = TestUtils.hexStringToByteArray(ATTRIBUTE_RAW_PACKET);

        assertArrayEquals(expectedBytes, byteBuffer.array());
    }

    @Test
    public void testDecodeEncryptionTransform() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(ENCR_TRANSFORM_RAW_PACKET);
        ByteBuffer inputBuffer = ByteBuffer.wrap(inputPacket);

        when(mMockedAttributeDecoder.decodeAttributes(anyInt(), any()))
                .thenReturn(mAttributeListWithKeyLength128);
        Transform.sAttributeDecoder = mMockedAttributeDecoder;

        Transform transform = Transform.readFrom(inputBuffer);
        assertTrue(transform instanceof EncryptionTransform);
        assertEquals(Transform.TRANSFORM_TYPE_ENCR, transform.type);
        assertEquals(SaProposal.ENCRYPTION_ALGORITHM_AES_CBC, transform.id);
        assertTrue(transform.isSupported);
    }

    @Test
    public void testDecodeEncryptionTransformWithInvalidKeyLength() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(ENCR_TRANSFORM_RAW_PACKET);
        ByteBuffer inputBuffer = ByteBuffer.wrap(inputPacket);

        List<Attribute> attributeList = new LinkedList<>();
        Attribute keyLengAttr = new KeyLengthAttribute(SaProposal.KEY_LEN_AES_128 + 1);
        attributeList.add(keyLengAttr);

        when(mMockedAttributeDecoder.decodeAttributes(anyInt(), any())).thenReturn(attributeList);
        Transform.sAttributeDecoder = mMockedAttributeDecoder;

        try {
            Transform.readFrom(inputBuffer);
            fail("Expected InvalidSyntaxException for invalid key length.");
        } catch (InvalidSyntaxException expected) {
        }
    }

    @Test
    public void testEncodeEncryptionTransform() throws Exception {
        ByteBuffer byteBuffer = ByteBuffer.allocate(mEncrAesCbc128Transform.getTransformLength());
        mEncrAesCbc128Transform.encodeToByteBuffer(false, byteBuffer);

        byte[] expectedBytes = TestUtils.hexStringToByteArray(ENCR_TRANSFORM_RAW_PACKET);

        assertArrayEquals(expectedBytes, byteBuffer.array());
    }

    @Test
    public void testConstructEncryptionTransformWithUnsupportedId() throws Exception {
        try {
            new EncryptionTransform(-1);
            fail("Expected IllegalArgumentException for unsupported Transform ID");
        } catch (IllegalArgumentException expected) {
        }
    }

    @Test
    public void testConstructEncryptionTransformWithInvalidKeyLength() throws Exception {
        try {
            new EncryptionTransform(SaProposal.ENCRYPTION_ALGORITHM_3DES, 129);
            fail("Expected IllegalArgumentException for invalid key length.");
        } catch (IllegalArgumentException expected) {
        }
    }

    @Test
    public void testDecodePrfTransform() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(PRF_TRANSFORM_RAW_PACKET);
        ByteBuffer inputBuffer = ByteBuffer.wrap(inputPacket);

        when(mMockedAttributeDecoder.decodeAttributes(anyInt(), any()))
                .thenReturn(new LinkedList<Attribute>());
        Transform.sAttributeDecoder = mMockedAttributeDecoder;

        Transform transform = Transform.readFrom(inputBuffer);
        assertTrue(transform instanceof PrfTransform);
        assertEquals(Transform.TRANSFORM_TYPE_PRF, transform.type);
        assertEquals(SaProposal.PSEUDORANDOM_FUNCTION_HMAC_SHA1, transform.id);
        assertTrue(transform.isSupported);
    }

    @Test
    public void testEncodePrfTransform() throws Exception {
        ByteBuffer byteBuffer = ByteBuffer.allocate(mPrfHmacSha1Transform.getTransformLength());
        mPrfHmacSha1Transform.encodeToByteBuffer(true, byteBuffer);

        byte[] expectedBytes = TestUtils.hexStringToByteArray(PRF_TRANSFORM_RAW_PACKET);

        assertArrayEquals(expectedBytes, byteBuffer.array());
    }

    @Test
    public void testConstructPrfTransformWithUnsupportedId() throws Exception {
        try {
            new PrfTransform(-1);
            fail("Expected IllegalArgumentException for unsupported Transform ID");
        } catch (IllegalArgumentException expected) {
        }
    }

    @Test
    public void testDecodeIntegrityTransform() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(INTEG_TRANSFORM_RAW_PACKET);
        ByteBuffer inputBuffer = ByteBuffer.wrap(inputPacket);

        when(mMockedAttributeDecoder.decodeAttributes(anyInt(), any()))
                .thenReturn(new LinkedList<Attribute>());
        Transform.sAttributeDecoder = mMockedAttributeDecoder;

        Transform transform = Transform.readFrom(inputBuffer);
        assertTrue(transform instanceof IntegrityTransform);
        assertEquals(Transform.TRANSFORM_TYPE_INTEG, transform.type);
        assertEquals(SaProposal.INTEGRITY_ALGORITHM_HMAC_SHA1_96, transform.id);
        assertTrue(transform.isSupported);
    }

    @Test
    public void testDecodeIntegrityTransformWithUnrecognizedAttribute() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(INTEG_TRANSFORM_RAW_PACKET);
        ByteBuffer inputBuffer = ByteBuffer.wrap(inputPacket);

        when(mMockedAttributeDecoder.decodeAttributes(anyInt(), any()))
                .thenReturn(mAttributeListWithKeyLength128);
        Transform.sAttributeDecoder = mMockedAttributeDecoder;

        Transform transform = Transform.readFrom(inputBuffer);
        assertTrue(transform instanceof IntegrityTransform);
        assertEquals(Transform.TRANSFORM_TYPE_INTEG, transform.type);
        assertEquals(SaProposal.INTEGRITY_ALGORITHM_HMAC_SHA1_96, transform.id);
        assertFalse(transform.isSupported);
    }

    @Test
    public void testEncodeIntegrityTransform() throws Exception {
        ByteBuffer byteBuffer = ByteBuffer.allocate(mIntegHmacSha1Transform.getTransformLength());
        mIntegHmacSha1Transform.encodeToByteBuffer(false, byteBuffer);

        byte[] expectedBytes = TestUtils.hexStringToByteArray(INTEG_TRANSFORM_RAW_PACKET);

        assertArrayEquals(expectedBytes, byteBuffer.array());
    }

    @Test
    public void testConstructIntegrityTransformWithUnsupportedId() throws Exception {
        try {
            new IntegrityTransform(-1);
            fail("Expected IllegalArgumentException for unsupported Transform ID");
        } catch (IllegalArgumentException expected) {
        }
    }

    @Test
    public void testDecodeDhGroupTransform() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(DH_GROUP_TRANSFORM_RAW_PACKET);
        ByteBuffer inputBuffer = ByteBuffer.wrap(inputPacket);

        when(mMockedAttributeDecoder.decodeAttributes(anyInt(), any()))
                .thenReturn(new LinkedList<Attribute>());
        Transform.sAttributeDecoder = mMockedAttributeDecoder;

        Transform transform = Transform.readFrom(inputBuffer);
        assertTrue(transform instanceof DhGroupTransform);
        assertEquals(Transform.TRANSFORM_TYPE_DH, transform.type);
        assertEquals(SaProposal.DH_GROUP_1024_BIT_MODP, transform.id);
        assertTrue(transform.isSupported);
    }

    @Test
    public void testEncodeDhGroupTransform() throws Exception {
        ByteBuffer byteBuffer = ByteBuffer.allocate(mDhGroup1024Transform.getTransformLength());
        mDhGroup1024Transform.encodeToByteBuffer(false, byteBuffer);

        byte[] expectedBytes = TestUtils.hexStringToByteArray(DH_GROUP_TRANSFORM_RAW_PACKET);

        assertArrayEquals(expectedBytes, byteBuffer.array());
    }

    @Test
    public void testConstructDhGroupTransformWithUnsupportedId() throws Exception {
        try {
            new DhGroupTransform(-1);
            fail("Expected IllegalArgumentException for unsupported Transform ID");
        } catch (IllegalArgumentException expected) {
        }
    }

    @Test
    public void testDecodeEsnTransform() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(ESN_TRANSFORM_RAW_PACKET);
        ByteBuffer inputBuffer = ByteBuffer.wrap(inputPacket);

        when(mMockedAttributeDecoder.decodeAttributes(anyInt(), any()))
                .thenReturn(new LinkedList<Attribute>());
        Transform.sAttributeDecoder = mMockedAttributeDecoder;

        Transform transform = Transform.readFrom(inputBuffer);
        assertTrue(transform instanceof EsnTransform);
        assertEquals(Transform.TRANSFORM_TYPE_ESN, transform.type);
        assertEquals(EsnTransform.ESN_POLICY_NO_EXTENDED, transform.id);
        assertTrue(transform.isSupported);
    }

    @Test
    public void testDecodeEsnTransformWithUnsupportedId() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(ESN_TRANSFORM_RAW_PACKET);
        inputPacket[TRANSFORM_ID_OFFSET] = -1;
        ByteBuffer inputBuffer = ByteBuffer.wrap(inputPacket);

        when(mMockedAttributeDecoder.decodeAttributes(anyInt(), any()))
                .thenReturn(new LinkedList<Attribute>());
        Transform.sAttributeDecoder = mMockedAttributeDecoder;

        Transform transform = Transform.readFrom(inputBuffer);
        assertTrue(transform instanceof EsnTransform);
        assertEquals(Transform.TRANSFORM_TYPE_ESN, transform.type);
        assertFalse(transform.isSupported);
    }

    @Test
    public void testDecodeEsnTransformWithUnrecognizedAttribute() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(ESN_TRANSFORM_RAW_PACKET);
        ByteBuffer inputBuffer = ByteBuffer.wrap(inputPacket);

        when(mMockedAttributeDecoder.decodeAttributes(anyInt(), any()))
                .thenReturn(mAttributeListWithKeyLength128);
        Transform.sAttributeDecoder = mMockedAttributeDecoder;

        Transform transform = Transform.readFrom(inputBuffer);
        assertTrue(transform instanceof EsnTransform);
        assertEquals(Transform.TRANSFORM_TYPE_ESN, transform.type);
        assertEquals(EsnTransform.ESN_POLICY_NO_EXTENDED, transform.id);
        assertFalse(transform.isSupported);
    }

    @Test
    public void testEncodeEsnTransform() throws Exception {
        EsnTransform mEsnTransform = new EsnTransform();
        ByteBuffer byteBuffer = ByteBuffer.allocate(mEsnTransform.getTransformLength());
        mEsnTransform.encodeToByteBuffer(true, byteBuffer);

        byte[] expectedBytes = TestUtils.hexStringToByteArray(ESN_TRANSFORM_RAW_PACKET);

        assertArrayEquals(expectedBytes, byteBuffer.array());
    }

    @Test
    public void testDecodeUnrecognizedTransform() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(ENCR_TRANSFORM_RAW_PACKET);
        inputPacket[TRANSFORM_TYPE_OFFSET] = 6;
        ByteBuffer inputBuffer = ByteBuffer.wrap(inputPacket);

        when(mMockedAttributeDecoder.decodeAttributes(anyInt(), any()))
                .thenReturn(mAttributeListWithKeyLength128);
        Transform.sAttributeDecoder = mMockedAttributeDecoder;

        Transform transform = Transform.readFrom(inputBuffer);

        assertTrue(transform instanceof UnrecognizedTransform);
    }

    @Test
    public void testDecodeTransformWithRepeatedAttribute() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(ENCR_TRANSFORM_RAW_PACKET);
        ByteBuffer inputBuffer = ByteBuffer.wrap(inputPacket);

        List<Attribute> attributeList = new LinkedList<>();
        attributeList.add(mAttributeKeyLength128);
        attributeList.add(mAttributeKeyLength128);

        when(mMockedAttributeDecoder.decodeAttributes(anyInt(), any())).thenReturn(attributeList);
        Transform.sAttributeDecoder = mMockedAttributeDecoder;

        try {
            Transform.readFrom(inputBuffer);
            fail("Expected InvalidSyntaxException for repeated Attribute Type Key Length.");
        } catch (InvalidSyntaxException expected) {
        }
    }

    @Test
    public void testDecodeTransformWithUnrecognizedTransformId() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(ENCR_TRANSFORM_RAW_PACKET);
        inputPacket[TRANSFORM_ID_OFFSET] = 1;
        ByteBuffer inputBuffer = ByteBuffer.wrap(inputPacket);

        when(mMockedAttributeDecoder.decodeAttributes(anyInt(), any()))
                .thenReturn(mAttributeListWithKeyLength128);
        Transform.sAttributeDecoder = mMockedAttributeDecoder;

        Transform transform = Transform.readFrom(inputBuffer);

        assertFalse(transform.isSupported);
    }

    @Test
    public void testDecodeTransformWithUnrecogniedAttributeType() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(ENCR_TRANSFORM_RAW_PACKET);
        ByteBuffer inputBuffer = ByteBuffer.wrap(inputPacket);

        List<Attribute> attributeList = new LinkedList<>();
        attributeList.add(mAttributeKeyLength128);
        Attribute attributeUnrecognized = new UnrecognizedAttribute(1, new byte[0]);
        attributeList.add(attributeUnrecognized);

        when(mMockedAttributeDecoder.decodeAttributes(anyInt(), any())).thenReturn(attributeList);
        Transform.sAttributeDecoder = mMockedAttributeDecoder;

        Transform transform = Transform.readFrom(inputBuffer);

        assertFalse(transform.isSupported);
    }

    @Test
    public void testTransformEquals() throws Exception {
        EncryptionTransform mEncrAesGcm8Key128TransformOther =
                new EncryptionTransform(
                        SaProposal.ENCRYPTION_ALGORITHM_AES_GCM_8, SaProposal.KEY_LEN_AES_128);

        assertEquals(mEncrAesGcm8Key128Transform, mEncrAesGcm8Key128TransformOther);

        EncryptionTransform mEncrAesGcm8Key192Transform =
                new EncryptionTransform(
                        SaProposal.ENCRYPTION_ALGORITHM_AES_GCM_8, SaProposal.KEY_LEN_AES_192);

        assertNotEquals(mEncrAesGcm8Key128Transform, mEncrAesGcm8Key192Transform);

        IntegrityTransform mIntegHmacSha1TransformOther =
                new IntegrityTransform(SaProposal.INTEGRITY_ALGORITHM_HMAC_SHA1_96);

        assertNotEquals(mEncrAesGcm8Key128Transform, mIntegHmacSha1Transform);
        assertEquals(mIntegHmacSha1Transform, mIntegHmacSha1TransformOther);
    }

    private TransformDecoder getDummyTransformDecoder(Transform[] decodedTransforms) {
        return new TransformDecoder() {
            @Override
            public Transform[] decodeTransforms(int count, ByteBuffer inputBuffer)
                    throws IkeException {
                for (int i = 0; i < count; i++) {
                    // Read length field and move position
                    inputBuffer.getShort();
                    int length = Short.toUnsignedInt(inputBuffer.getShort());
                    byte[] temp = new byte[length - 4];
                    inputBuffer.get(temp);
                }
                return decodedTransforms;
            }
        };
    }

    @Test
    public void testDecodeSingleProposal() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(INBOUND_PROPOSAL_RAW_PACKET);
        ByteBuffer inputBuffer = ByteBuffer.wrap(inputPacket);
        Proposal.sTransformDecoder = getDummyTransformDecoder(new Transform[0]);

        Proposal proposal = Proposal.readFrom(inputBuffer);

        assertEquals(PROPOSAL_NUMBER, proposal.number);
        assertEquals(IkePayload.PROTOCOL_ID_IKE, proposal.protocolId);
        assertEquals(IkePayload.SPI_LEN_NOT_INCLUDED, proposal.spiSize);
        assertEquals(IkePayload.SPI_NOT_INCLUDED, proposal.spi);
        assertFalse(proposal.hasUnrecognizedTransform);
        assertNotNull(proposal.saProposal);
    }

    @Test
    public void testDecodeSaRequestWithMultipleProposal() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(INBOUND_TWO_PROPOSAL_RAW_PACKET);
        Proposal.sTransformDecoder = getDummyTransformDecoder(new Transform[0]);

        IkeSaPayload payload = new IkeSaPayload(false, false, inputPacket);

        assertEquals(PROPOSAL_NUMBER_LIST.length, payload.proposalList.size());
        for (int i = 0; i < payload.proposalList.size(); i++) {
            Proposal proposal = payload.proposalList.get(i);
            assertEquals(PROPOSAL_NUMBER_LIST[i], proposal.number);
            assertEquals(IkePayload.PROTOCOL_ID_IKE, proposal.protocolId);
            assertEquals(0, proposal.spiSize);
        }
    }

    @Test
    public void testEncodeProposal() throws Exception {
        Proposal proposal =
                new Proposal(
                        (byte) PROPOSAL_NUMBER,
                        IkePayload.PROTOCOL_ID_IKE,
                        IkePayload.SPI_LEN_NOT_INCLUDED,
                        IkePayload.SPI_NOT_INCLUDED,
                        mSaProposalOne,
                        false /*has no unrecognized Tramsform*/);

        ByteBuffer byteBuffer = ByteBuffer.allocate(proposal.getProposalLength());
        proposal.encodeToByteBuffer(true /*is the last*/, byteBuffer);

        byte[] expectedBytes = TestUtils.hexStringToByteArray(OUTBOUND_PROPOSAL_RAW_PACKET);
        assertArrayEquals(expectedBytes, byteBuffer.array());
    }

    @Test
    public void testDecodeSaResponseWithMultipleProposal() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(INBOUND_TWO_PROPOSAL_RAW_PACKET);
        Proposal.sTransformDecoder = getDummyTransformDecoder(new Transform[0]);

        try {
            new IkeSaPayload(false, true, inputPacket);
            fail("Expected to fail due to more than one proposal in response SA payload.");
        } catch (InvalidSyntaxException expected) {

        }
    }

    @Test
    public void testBuildIkeSaResponsePayload() throws Exception {
        final long ikeSpi = new Random().nextLong();
        final SaProposal[] saProposals = new SaProposal[] {mSaProposalOne};
        IkeSaPayload saPayload =
                new IkeSaPayload(
                        true, true, IkePayload.SPI_LEN_IKE, new long[] {ikeSpi}, saProposals);

        assertTrue(saPayload.isSaResponse);
        assertEquals(saProposals.length, saPayload.proposalList.size());

        Proposal proposal = saPayload.proposalList.get(0);
        assertEquals(IkePayload.PROTOCOL_ID_IKE, proposal.protocolId);
        assertEquals(IkePayload.SPI_LEN_IKE, proposal.spiSize);
        assertEquals(ikeSpi, proposal.spi);
        assertEquals(mSaProposalOne, proposal.saProposal);
    }

    @Test
    public void testBuildInitialIkeSaRequestPayload() throws Exception {
        IkeSaPayload saPayload = new IkeSaPayload(mTwoSaProposalsArray);

        assertFalse(saPayload.isSaResponse);
        assertEquals(PROPOSAL_NUMBER_LIST.length, saPayload.proposalList.size());

        for (int i = 0; i < saPayload.proposalList.size(); i++) {
            Proposal proposal = saPayload.proposalList.get(i);
            assertEquals(PROPOSAL_NUMBER_LIST[i], proposal.number);
            assertEquals(IkePayload.PROTOCOL_ID_IKE, proposal.protocolId);
            assertEquals(IkePayload.SPI_LEN_NOT_INCLUDED, proposal.spiSize);
            assertEquals(mTwoSaProposalsArray[i], proposal.saProposal);
        }
    }

    @Test
    public void testEncodeIkeSaPayload() throws Exception {
        IkeSaPayload saPayload = new IkeSaPayload(new SaProposal[] {mSaProposalOne});

        ByteBuffer byteBuffer = ByteBuffer.allocate(saPayload.getPayloadLength());
        saPayload.encodeToByteBuffer(IkePayload.PAYLOAD_TYPE_KE, byteBuffer);

        byte[] expectedBytes =
                TestUtils.hexStringToByteArray(
                        OUTBOUND_SA_PAYLOAD_HEADER + OUTBOUND_PROPOSAL_RAW_PACKET);
        assertArrayEquals(expectedBytes, byteBuffer.array());
    }

    private void buildAndVerifySaRespProposal(byte[] saResponseBytes, Transform[] decodedTransforms)
            throws Exception {
        // Build response SA payload from decoding bytes.
        Proposal.sTransformDecoder = getDummyTransformDecoder(decodedTransforms);
        IkeSaPayload respPayload = new IkeSaPayload(false, true, saResponseBytes);

        // Build request SA payload from SaProposal.
        IkeSaPayload reqPayload = new IkeSaPayload(mTwoSaProposalsArray);

        SaProposal saProposal = respPayload.getVerifiedNegotiatedProposal(reqPayload);

        assertEquals(respPayload.proposalList.get(0).saProposal, saProposal);
    }

    @Test
    public void testGetVerifiedNegotiatedProposal() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(INBOUND_PROPOSAL_RAW_PACKET);

        buildAndVerifySaRespProposal(inputPacket, mValidNegotiatedTransformSet);
    }

    // Test throwing when negotiated proposal in SA response payload has unrecognized Transform.
    @Test
    public void testGetVerifiedNegotiatedProposalWithUnrecogTransform() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(INBOUND_PROPOSAL_RAW_PACKET);

        Transform[] negotiatedTransformSet =
                Arrays.copyOfRange(
                        mValidNegotiatedTransformSet, 0, mValidNegotiatedTransformSet.length);
        negotiatedTransformSet[0] = new UnrecognizedTransform(-1, 1, new LinkedList<>());

        try {
            buildAndVerifySaRespProposal(inputPacket, negotiatedTransformSet);
            fail("Expected to fail because negotiated proposal has unrecognized Transform.");
        } catch (NoValidProposalChosenException expected) {
        }
    }

    // Test throwing when negotiated proposal has invalid proposal number.
    @Test
    public void testGetVerifiedNegotiatedProposalWithInvalidNumber() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(INBOUND_PROPOSAL_RAW_PACKET);
        inputPacket[PROPOSAL_NUMBER_OFFSET] = (byte) 10;

        try {
            buildAndVerifySaRespProposal(inputPacket, mValidNegotiatedTransformSet);
            fail("Expected to fail due to invalid proposal number.");
        } catch (NoValidProposalChosenException expected) {
        }
    }

    // Test throwing when negotiated proposal has mismatched protocol ID.
    @Test
    public void testGetVerifiedNegotiatedProposalWithMisMatchedProtocol() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(INBOUND_PROPOSAL_RAW_PACKET);
        inputPacket[PROTOCOL_ID_OFFSET] = IkePayload.PROTOCOL_ID_ESP;

        try {
            buildAndVerifySaRespProposal(inputPacket, mValidNegotiatedTransformSet);
            fail("Expected to fail due to mismatched protocol ID.");
        } catch (NoValidProposalChosenException expected) {
        }
    }

    // Test throwing when negotiated proposal has Transform that was not proposed in request.
    @Test
    public void testGetVerifiedNegotiatedProposalWithMismatchedTransform() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(INBOUND_PROPOSAL_RAW_PACKET);

        Transform[] negotiatedTransformSet =
                Arrays.copyOfRange(
                        mValidNegotiatedTransformSet, 0, mValidNegotiatedTransformSet.length);
        negotiatedTransformSet[0] = mEncrAesGcm8Key128Transform;

        try {
            buildAndVerifySaRespProposal(inputPacket, negotiatedTransformSet);
            fail("Expected to fail due to mismatched Transform.");
        } catch (NoValidProposalChosenException expected) {
        }
    }

    // Test throwing when negotiated proposal is lack of a certain type Transform.
    @Test
    public void testGetVerifiedNegotiatedProposalWithoutTransform() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(INBOUND_PROPOSAL_RAW_PACKET);

        try {
            buildAndVerifySaRespProposal(inputPacket, new Transform[0]);
            fail("Expected to fail due to absence of Transform.");
        } catch (NoValidProposalChosenException expected) {
        }
    }
}
