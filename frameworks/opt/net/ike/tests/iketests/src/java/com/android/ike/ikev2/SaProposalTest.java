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

package com.android.ike.ikev2;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import com.android.ike.ikev2.SaProposal.Builder;
import com.android.ike.ikev2.message.IkePayload;
import com.android.ike.ikev2.message.IkeSaPayload.DhGroupTransform;
import com.android.ike.ikev2.message.IkeSaPayload.EncryptionTransform;
import com.android.ike.ikev2.message.IkeSaPayload.IntegrityTransform;
import com.android.ike.ikev2.message.IkeSaPayload.PrfTransform;
import com.android.ike.ikev2.message.IkeSaPayload.Transform;

import org.junit.Test;

public final class SaProposalTest {
    private final EncryptionTransform mEncryption3DesTransform;
    private final EncryptionTransform mEncryptionAesGcm8Transform;
    private final EncryptionTransform mEncryptionAesGcm12Transform;
    private final IntegrityTransform mIntegrityHmacSha1Transform;
    private final IntegrityTransform mIntegrityNoneTransform;
    private final PrfTransform mPrfAes128XCbcTransform;
    private final DhGroupTransform mDhGroup1024Transform;

    public SaProposalTest() {
        mEncryption3DesTransform = new EncryptionTransform(SaProposal.ENCRYPTION_ALGORITHM_3DES);
        mEncryptionAesGcm8Transform =
                new EncryptionTransform(
                        SaProposal.ENCRYPTION_ALGORITHM_AES_GCM_8, SaProposal.KEY_LEN_AES_128);
        mEncryptionAesGcm12Transform =
                new EncryptionTransform(
                        SaProposal.ENCRYPTION_ALGORITHM_AES_GCM_12, SaProposal.KEY_LEN_AES_128);
        mIntegrityHmacSha1Transform =
                new IntegrityTransform(SaProposal.INTEGRITY_ALGORITHM_HMAC_SHA1_96);
        mIntegrityNoneTransform = new IntegrityTransform(SaProposal.INTEGRITY_ALGORITHM_NONE);
        mPrfAes128XCbcTransform = new PrfTransform(SaProposal.PSEUDORANDOM_FUNCTION_AES128_XCBC);
        mDhGroup1024Transform = new DhGroupTransform(SaProposal.DH_GROUP_1024_BIT_MODP);
    }

    @Test
    public void testBuildIkeSaProposalWithNormalModeCipher() throws Exception {
        Builder builder = Builder.newIkeSaProposalBuilder();
        SaProposal proposal =
                builder.addEncryptionAlgorithm(SaProposal.ENCRYPTION_ALGORITHM_3DES)
                        .addIntegrityAlgorithm(SaProposal.INTEGRITY_ALGORITHM_HMAC_SHA1_96)
                        .addPseudorandomFunction(SaProposal.PSEUDORANDOM_FUNCTION_AES128_XCBC)
                        .addDhGroup(SaProposal.DH_GROUP_1024_BIT_MODP)
                        .build();

        assertEquals(IkePayload.PROTOCOL_ID_IKE, proposal.getProtocolId());
        assertArrayEquals(
                new EncryptionTransform[] {mEncryption3DesTransform},
                proposal.getEncryptionTransforms());
        assertArrayEquals(
                new IntegrityTransform[] {mIntegrityHmacSha1Transform},
                proposal.getIntegrityTransforms());
        assertArrayEquals(
                new PrfTransform[] {mPrfAes128XCbcTransform}, proposal.getPrfTransforms());
        assertArrayEquals(
                new DhGroupTransform[] {mDhGroup1024Transform}, proposal.getDhGroupTransforms());
    }

    @Test
    public void testBuildIkeSaProposalWithCombinedModeCipher() throws Exception {
        Builder builder = Builder.newIkeSaProposalBuilder();
        SaProposal proposal =
                builder.addEncryptionAlgorithm(
                                SaProposal.ENCRYPTION_ALGORITHM_AES_GCM_8,
                                SaProposal.KEY_LEN_AES_128)
                        .addPseudorandomFunction(SaProposal.PSEUDORANDOM_FUNCTION_AES128_XCBC)
                        .addDhGroup(SaProposal.DH_GROUP_1024_BIT_MODP)
                        .build();

        assertEquals(IkePayload.PROTOCOL_ID_IKE, proposal.getProtocolId());
        assertArrayEquals(
                new EncryptionTransform[] {mEncryptionAesGcm8Transform},
                proposal.getEncryptionTransforms());
        assertArrayEquals(
                new PrfTransform[] {mPrfAes128XCbcTransform}, proposal.getPrfTransforms());
        assertArrayEquals(
                new DhGroupTransform[] {mDhGroup1024Transform}, proposal.getDhGroupTransforms());
        assertTrue(proposal.getIntegrityTransforms().length == 0);
    }

    @Test
    public void testBuildFirstChildSaProposalWithCombinedCipher() throws Exception {
        Builder builder = Builder.newChildSaProposalBuilder(true);
        SaProposal proposal =
                builder.addEncryptionAlgorithm(
                                SaProposal.ENCRYPTION_ALGORITHM_AES_GCM_8,
                                SaProposal.KEY_LEN_AES_128)
                        .addIntegrityAlgorithm(SaProposal.INTEGRITY_ALGORITHM_NONE)
                        .build();

        assertEquals(IkePayload.PROTOCOL_ID_ESP, proposal.getProtocolId());
        assertArrayEquals(
                new EncryptionTransform[] {mEncryptionAesGcm8Transform},
                proposal.getEncryptionTransforms());
        assertArrayEquals(
                new IntegrityTransform[] {mIntegrityNoneTransform},
                proposal.getIntegrityTransforms());
        assertTrue(proposal.getPrfTransforms().length == 0);
        assertTrue(proposal.getDhGroupTransforms().length == 0);
    }

    @Test
    public void testBuildAdditionalChildSaProposalWithNormalCipher() throws Exception {
        Builder builder = Builder.newChildSaProposalBuilder(false);

        SaProposal proposal =
                builder.addEncryptionAlgorithm(SaProposal.ENCRYPTION_ALGORITHM_3DES)
                        .addIntegrityAlgorithm(SaProposal.INTEGRITY_ALGORITHM_NONE)
                        .addDhGroup(SaProposal.DH_GROUP_1024_BIT_MODP)
                        .build();

        assertEquals(IkePayload.PROTOCOL_ID_ESP, proposal.getProtocolId());
        assertArrayEquals(
                new EncryptionTransform[] {mEncryption3DesTransform},
                proposal.getEncryptionTransforms());
        assertArrayEquals(
                new IntegrityTransform[] {mIntegrityNoneTransform},
                proposal.getIntegrityTransforms());
        assertArrayEquals(
                new DhGroupTransform[] {mDhGroup1024Transform}, proposal.getDhGroupTransforms());
        assertTrue(proposal.getPrfTransforms().length == 0);
    }

    @Test
    public void testBuildEncryptAlgosWithNoAlgorithm() throws Exception {
        Builder builder = Builder.newIkeSaProposalBuilder();
        try {
            builder.build();
            fail("Expected to fail when no encryption algorithm is proposed.");
        } catch (IllegalArgumentException expected) {

        }
    }

    @Test
    public void testBuildEncryptAlgosWithUnrecognizedAlgorithm() throws Exception {
        Builder builder = Builder.newIkeSaProposalBuilder();
        try {
            builder.addEncryptionAlgorithm(-1);
            fail("Expected to fail when unrecognized encryption algorithm is proposed.");
        } catch (IllegalArgumentException expected) {

        }
    }

    @Test
    public void testBuildEncryptAlgosWithTwoModes() throws Exception {
        Builder builder = Builder.newIkeSaProposalBuilder();
        try {
            builder.addEncryptionAlgorithm(SaProposal.ENCRYPTION_ALGORITHM_3DES)
                    .addEncryptionAlgorithm(SaProposal.ENCRYPTION_ALGORITHM_AES_GCM_12);
            fail(
                    "Expected to fail when "
                            + "normal and combined-mode ciphers are proposed together.");
        } catch (IllegalArgumentException expected) {

        }
    }

    @Test
    public void testBuildIkeProposalWithoutPrf() throws Exception {
        Builder builder = Builder.newIkeSaProposalBuilder();
        try {
            builder.addEncryptionAlgorithm(SaProposal.ENCRYPTION_ALGORITHM_3DES).build();
            fail("Expected to fail when PRF is not provided in IKE SA proposal.");
        } catch (IllegalArgumentException expected) {

        }
    }

    @Test
    public void testBuildChildProposalWithPrf() throws Exception {
        Builder builder = Builder.newChildSaProposalBuilder(false);
        try {
            builder.addEncryptionAlgorithm(SaProposal.ENCRYPTION_ALGORITHM_3DES)
                    .addPseudorandomFunction(SaProposal.PSEUDORANDOM_FUNCTION_HMAC_SHA1)
                    .build();

            fail("Expected to fail when PRF is provided in Child SA proposal.");
        } catch (IllegalArgumentException expected) {

        }
    }

    // Test throwing exception when building IKE SA Proposal with AEAD and not-none integrity
    // algorithm.
    @Test
    public void testBuildAeadWithIntegrityAlgo() throws Exception {
        Builder builder = Builder.newChildSaProposalBuilder(false);
        try {
            builder.addEncryptionAlgorithm(SaProposal.ENCRYPTION_ALGORITHM_AES_GCM_12)
                    .addIntegrityAlgorithm(SaProposal.INTEGRITY_ALGORITHM_NONE)
                    .addIntegrityAlgorithm(SaProposal.INTEGRITY_ALGORITHM_HMAC_SHA1_96)
                    .build();

            fail("Expected to fail when not-none integrity algorithm is proposed with AEAD");
        } catch (IllegalArgumentException expected) {

        }
    }

    // Test throwing exception when building IKE SA Proposal with normal mode cipher and without
    // integrity algorithm.
    @Test
    public void testBuildIkeProposalNormalCipherWithoutIntegrityAlgo() throws Exception {
        Builder builder = Builder.newChildSaProposalBuilder(false);
        try {
            builder.addEncryptionAlgorithm(SaProposal.ENCRYPTION_ALGORITHM_3DES)
                    .addPseudorandomFunction(SaProposal.PSEUDORANDOM_FUNCTION_HMAC_SHA1)
                    .build();

            fail(
                    "Expected to fail when"
                            + " no integrity algorithm is proposed with non-combined cipher");
        } catch (IllegalArgumentException expected) {

        }
    }

    // Test throwing exception when building IKE SA Proposal with normal mode cipher and none-value
    // integrity algorithm.
    @Test
    public void testBuildIkeProposalNormalCipherWithNoneValueIntegrityAlgo() throws Exception {
        Builder builder = Builder.newChildSaProposalBuilder(false);
        try {
            builder.addEncryptionAlgorithm(SaProposal.ENCRYPTION_ALGORITHM_3DES)
                    .addPseudorandomFunction(SaProposal.PSEUDORANDOM_FUNCTION_HMAC_SHA1)
                    .addIntegrityAlgorithm(SaProposal.INTEGRITY_ALGORITHM_NONE)
                    .addIntegrityAlgorithm(SaProposal.INTEGRITY_ALGORITHM_HMAC_SHA1_96)
                    .build();

            fail(
                    "Expected to fail when none-value integrity algorithm is proposed"
                            + " with non-combined cipher");
        } catch (IllegalArgumentException expected) {

        }
    }

    @Test
    public void testBuildIkeProposalWithoutDhGroup() throws Exception {
        Builder builder = Builder.newIkeSaProposalBuilder();
        try {
            builder.addEncryptionAlgorithm(SaProposal.ENCRYPTION_ALGORITHM_3DES)
                    .addIntegrityAlgorithm(SaProposal.INTEGRITY_ALGORITHM_HMAC_SHA1_96)
                    .addPseudorandomFunction(SaProposal.PSEUDORANDOM_FUNCTION_AES128_XCBC)
                    .build();

            fail("Expected to fail when no DH Group is proposed in IKE SA proposal.");
        } catch (IllegalArgumentException expected) {

        }
    }

    @Test
    public void testBuildIkeProposalWithNoneValueDhGroup() throws Exception {
        Builder builder = Builder.newIkeSaProposalBuilder();
        try {
            builder.addEncryptionAlgorithm(SaProposal.ENCRYPTION_ALGORITHM_3DES)
                    .addIntegrityAlgorithm(SaProposal.INTEGRITY_ALGORITHM_HMAC_SHA1_96)
                    .addPseudorandomFunction(SaProposal.PSEUDORANDOM_FUNCTION_AES128_XCBC)
                    .addDhGroup(SaProposal.DH_GROUP_1024_BIT_MODP)
                    .addDhGroup(SaProposal.DH_GROUP_NONE)
                    .build();

            fail("Expected to fail when none-value DH Group is proposed in IKE SA proposal.");
        } catch (IllegalArgumentException expected) {

        }
    }

    // Test throwing exception when building first Child SA Proposal with not-none-value DH Group.
    @Test
    public void testBuildFirstChildProposalWithNotNoneValueDhGroup() throws Exception {
        Builder builder = Builder.newChildSaProposalBuilder(true);
        try {
            builder.addEncryptionAlgorithm(SaProposal.ENCRYPTION_ALGORITHM_3DES)
                    .addIntegrityAlgorithm(SaProposal.INTEGRITY_ALGORITHM_HMAC_SHA1_96)
                    .addDhGroup(SaProposal.DH_GROUP_1024_BIT_MODP)
                    .build();

            fail(
                    "Expected to fail when"
                            + " not-none-value DH Group is proposed in first Child SA proposal.");
        } catch (IllegalArgumentException expected) {

        }
    }

    @Test
    public void testIsTransformSelectedFrom() throws Exception {
        assertTrue(SaProposal.isTransformSelectedFrom(new Transform[0], new Transform[0]));
        assertTrue(
                SaProposal.isTransformSelectedFrom(
                        new Transform[] {mEncryptionAesGcm8Transform},
                        new Transform[] {
                            mEncryptionAesGcm8Transform, mEncryptionAesGcm12Transform
                        }));
        assertTrue(
                SaProposal.isTransformSelectedFrom(
                        new Transform[] {mIntegrityNoneTransform},
                        new Transform[] {mIntegrityNoneTransform}));

        // No transform selected.
        assertFalse(
                SaProposal.isTransformSelectedFrom(
                        new Transform[0], new Transform[] {mEncryptionAesGcm8Transform}));

        // Selected transform was not part of original proposal.
        assertFalse(
                SaProposal.isTransformSelectedFrom(
                        new Transform[] {mPrfAes128XCbcTransform}, new Transform[0]));

        // More than one transform returned.
        assertFalse(
                SaProposal.isTransformSelectedFrom(
                        new Transform[] {mEncryptionAesGcm8Transform, mEncryptionAesGcm12Transform},
                        new Transform[] {
                            mEncryptionAesGcm8Transform, mEncryptionAesGcm12Transform
                        }));

        // Selected transform was not part of original proposal.
        assertFalse(
                SaProposal.isTransformSelectedFrom(
                        new Transform[] {mIntegrityNoneTransform},
                        new Transform[] {mIntegrityHmacSha1Transform}));
    }
}
