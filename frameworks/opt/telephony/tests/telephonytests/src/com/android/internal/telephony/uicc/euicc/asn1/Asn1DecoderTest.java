/*
 * Copyright (C) 2016 The Android Open Source Project
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

package com.android.internal.telephony.uicc.euicc.asn1;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.telephony.uicc.asn1.Asn1Decoder;
import com.android.internal.telephony.uicc.asn1.Asn1Node;
import com.android.internal.telephony.uicc.asn1.InvalidAsn1DataException;
import com.android.internal.telephony.uicc.asn1.TagNotFoundException;

import org.junit.Test;

public class Asn1DecoderTest {

    @SmallTest
    @Test
    public void testNormalOperation() throws TagNotFoundException, InvalidAsn1DataException {
        String hex = "e30c5a0a98102100000000000000";
        Asn1Decoder decoder = new Asn1Decoder(hex);
        assertEquals(0, decoder.getPosition());

        assertTrue(decoder.hasNextNode());
        Asn1Node root = decoder.nextNode();
        assertEquals(hex.length() / 2, decoder.getPosition());
        assertTrue(root.isConstructed());
        assertEquals(0xE3, root.getTag());
        assertEquals(0x0E, root.getEncodedLength());
        assertFalse(root.hasValue());

        Asn1Node node = root.getChild(0x5A);
        assertEquals(1, root.getChildren().size());
        assertEquals(1, root.getChildren(0x5A).size());
        assertEquals(node, root.getChildren().get(0));
        assertEquals(node, root.getChildren(0x5A).get(0));
        assertFalse(node.isConstructed());
        assertEquals(0x5A, node.getTag());
        assertEquals(0x0C, node.getEncodedLength());
        assertTrue(node.hasValue());
        assertArrayEquals(
                new byte[] {(byte) 0x98, 0x10, 0x21, 0, 0, 0, 0, 0, 0, 0}, node.asBytes());

        assertFalse(decoder.hasNextNode());
        assertEquals(hex.length() / 2, decoder.getPosition());
    }

    @SmallTest
    @Test
    public void testHighTagNumberForm() throws TagNotFoundException, InvalidAsn1DataException {
        String hex = "bf370c5a0a98102100000000000000";
        Asn1Decoder decoder = new Asn1Decoder(hex);

        assertTrue(decoder.hasNextNode());
        Asn1Node root = decoder.nextNode();
        assertTrue(root.isConstructed());
        assertEquals(0xBF37, root.getTag());
        assertEquals(0x0F, root.getEncodedLength());
        assertTrue(root.isConstructed());

        Asn1Node node = root.getChild(0x5A);
        assertEquals(0x0C, node.getEncodedLength());
    }

    @SmallTest
    @Test
    public void testNodeList() throws TagNotFoundException, InvalidAsn1DataException {
        String hex = "e30c5a0a98102100000000000000e30c5a0a98102100000000000001";

        Asn1Decoder decoder = new Asn1Decoder(hex);
        assertEquals(0, decoder.getPosition());

        assertTrue(decoder.hasNextNode());
        Asn1Node firstRoot = decoder.nextNode();
        assertEquals(hex.length() / 4, decoder.getPosition());
        assertTrue(firstRoot.isConstructed());
        assertEquals(0xE3, firstRoot.getTag());
        Asn1Node childOfFirst = firstRoot.getChild(0x5A);
        assertNotNull(childOfFirst);
        assertFalse(childOfFirst.isConstructed());
        assertEquals(0x5A, childOfFirst.getTag());
        assertTrue(childOfFirst.hasValue());
        assertArrayEquals(
                new byte[] {(byte) 0x98, 0x10, 0x21, 0, 0, 0, 0, 0, 0, 0}, childOfFirst.asBytes());

        assertTrue(decoder.hasNextNode());
        Asn1Node secondRoot = decoder.nextNode();
        assertEquals(hex.length() / 2, decoder.getPosition());
        assertTrue(secondRoot.isConstructed());
        assertEquals(0xE3, secondRoot.getTag());
        Asn1Node childOfSecond = secondRoot.getChild(0x5A);
        assertNotNull(childOfSecond);
        assertFalse(childOfSecond.isConstructed());
        assertEquals(0x5A, childOfSecond.getTag());
        assertTrue(childOfSecond.hasValue());
        assertArrayEquals(
                new byte[] {(byte) 0x98, 0x10, 0x21, 0, 0, 0, 0, 0, 0, 1}, childOfSecond.asBytes());

        assertFalse(decoder.hasNextNode());
        assertEquals(hex.length() / 2, decoder.getPosition());
    }

    @SmallTest
    @Test
    public void testMissingData() throws TagNotFoundException, InvalidAsn1DataException {
        String hex = "e30c5a0a98102100000000000000e30c5a0a98102100000000";
        Asn1Decoder decoder = new Asn1Decoder(hex);
        assertEquals(0, decoder.getPosition());

        assertTrue(decoder.hasNextNode());
        Asn1Node firstRoot = decoder.nextNode();
        assertEquals(0x0E, decoder.getPosition());
        assertTrue(firstRoot.isConstructed());
        assertEquals(0xE3, firstRoot.getTag());
        Asn1Node childOfFirst = firstRoot.getChild(0x5A);
        assertNotNull(childOfFirst);
        assertFalse(childOfFirst.isConstructed());
        assertEquals(0x5A, childOfFirst.getTag());
        assertTrue(childOfFirst.hasValue());
        assertArrayEquals(
                new byte[] {(byte) 0x98, 0x10, 0x21, 0, 0, 0, 0, 0, 0, 0}, childOfFirst.asBytes());

        assertTrue(decoder.hasNextNode());
        try {
            decoder.nextNode();
            fail("Bytes should not be parsed.");
        } catch (InvalidAsn1DataException e) {
            assertEquals(0xE3, e.getTag());
        }
    }

    @SmallTest
    @Test
    public void testEmptyData() throws TagNotFoundException, InvalidAsn1DataException {
        String hex = "e3025a00e3025a00";
        Asn1Decoder decoder = new Asn1Decoder(hex);
        assertEquals(0, decoder.getPosition());

        assertTrue(decoder.hasNextNode());
        Asn1Node firstRoot = decoder.nextNode();
        assertEquals(hex.length() / 4, decoder.getPosition());
        assertTrue(firstRoot.isConstructed());
        assertEquals(0xE3, firstRoot.getTag());
        Asn1Node childOfFirst = firstRoot.getChild(0x5A);
        assertNotNull(childOfFirst);
        assertFalse(childOfFirst.isConstructed());
        assertEquals(0x5A, childOfFirst.getTag());
        assertTrue(childOfFirst.hasValue());
        assertArrayEquals(new byte[] {}, childOfFirst.asBytes());

        assertTrue(decoder.hasNextNode());
        Asn1Node secondRoot = decoder.nextNode();
        assertEquals(hex.length() / 2, decoder.getPosition());
        assertTrue(secondRoot.isConstructed());
        assertEquals(0xE3, secondRoot.getTag());
        Asn1Node childOfSecond = secondRoot.getChild(0x5A);
        assertNotNull(childOfSecond);
        assertFalse(childOfSecond.isConstructed());
        assertEquals(0x5A, childOfSecond.getTag());
        assertTrue(childOfSecond.hasValue());
        assertArrayEquals(new byte[] {}, childOfSecond.asBytes());

        assertFalse(decoder.hasNextNode());
        assertEquals(hex.length() / 2, decoder.getPosition());
    }

    @SmallTest
    @Test
    public void testLongFormLength() throws TagNotFoundException, InvalidAsn1DataException {
        String hex = "bf37075a820003010203";
        Asn1Decoder decoder = new Asn1Decoder(hex);

        assertTrue(decoder.hasNextNode());
        Asn1Node root = decoder.nextNode();
        assertTrue(root.isConstructed());
        assertEquals(0xBF37, root.getTag());
        assertEquals(10, root.getEncodedLength());
        assertTrue(root.isConstructed());

        Asn1Node node = root.getChild(0x5A);
        assertEquals(5, node.getEncodedLength());
        assertArrayEquals(new byte[] {1, 2, 3}, node.asBytes());
    }

    @SmallTest
    @Test(expected = IllegalStateException.class)
    public void testDecodeEmptyByteArray() throws TagNotFoundException, InvalidAsn1DataException {
        new Asn1Decoder("").nextNode();
    }

    @SmallTest
    @Test(expected = IndexOutOfBoundsException.class)
    public void testDecodeIndexOutOfBounds() throws TagNotFoundException, InvalidAsn1DataException {
        new Asn1Decoder(new byte[] {1, 2}, 0, 3);
    }

    @SmallTest
    @Test(expected = InvalidAsn1DataException.class)
    public void testIncompleteLength() throws TagNotFoundException, InvalidAsn1DataException {
        try {
            new Asn1Decoder("BF37").nextNode();
        } catch (InvalidAsn1DataException e) {
            assertEquals(0, e.getTag());
            assertEquals("Invalid length at position: 2 (tag=0)", e.getMessage());
            throw e;
        }
    }

    @SmallTest
    @Test(expected = InvalidAsn1DataException.class)
    public void testInvalidTag() throws TagNotFoundException, InvalidAsn1DataException {
        try {
            new Asn1Decoder("BF818283840100").nextNode();
        } catch (InvalidAsn1DataException e) {
            assertEquals(0, e.getTag());
            assertEquals("Cannot parse tag at position: 0 (tag=0)", e.getMessage());
            throw e;
        }
    }

    @SmallTest
    @Test(expected = InvalidAsn1DataException.class)
    public void testIncompleteData() throws TagNotFoundException, InvalidAsn1DataException {
        try {
            new Asn1Decoder("BF81010200").nextNode();
        } catch (InvalidAsn1DataException e) {
            assertEquals(0xBF8101, e.getTag());
            assertEquals(
                    "Incomplete data at position: 4, expected bytes: 2, "
                            + "actual bytes: 1 (tag=12550401)",
                    e.getMessage());
            throw e;
        }
    }

    @SmallTest
    @Test(expected = InvalidAsn1DataException.class)
    public void testIncompleteLongFormLength()
            throws TagNotFoundException, InvalidAsn1DataException {
        try {
            new Asn1Decoder("BF81018200").nextNode();
        } catch (InvalidAsn1DataException e) {
            assertEquals(0xBF8101, e.getTag());
            assertEquals("Cannot parse length at position: 4 (tag=12550401)", e.getMessage());
            throw e;
        }
    }

    @SmallTest
    @Test(expected = InvalidAsn1DataException.class)
    public void testInvalidLongFormLength() throws TagNotFoundException, InvalidAsn1DataException {
        try {
            new Asn1Decoder("BF8101851234567890").nextNode();
        } catch (InvalidAsn1DataException e) {
            assertEquals(0xBF8101, e.getTag());
            assertEquals("Cannot parse length at position: 4 (tag=12550401)", e.getMessage());
            throw e;
        }
    }
}
