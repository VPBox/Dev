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
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.telephony.uicc.IccUtils;
import com.android.internal.telephony.uicc.asn1.Asn1Decoder;
import com.android.internal.telephony.uicc.asn1.Asn1Node;
import com.android.internal.telephony.uicc.asn1.InvalidAsn1DataException;
import com.android.internal.telephony.uicc.asn1.TagNotFoundException;

import org.junit.Test;

public class Asn1NodeTest {

    @SmallTest
    @Test
    public void testCreateConstructedNodeWithShortTag() {
        Asn1Node node = Asn1Node.newBuilder(0x65).build();
        assertEquals(0x65, node.getTag());
        assertTrue(node.isConstructed());
        assertEquals(2, node.getEncodedLength());
        assertEquals(0, node.getDataLength());
        assertArrayEquals(new byte[] {0x65, 0x00}, node.toBytes());
    }

    @SmallTest
    @Test
    public void testCreateConstructedNodeWithLongTag() {
        Asn1Node node = Asn1Node.newBuilder(0x3F34).build();
        assertEquals(0x3F34, node.getTag());
        assertTrue(node.isConstructed());
        assertEquals(3, node.getEncodedLength());
        assertEquals(0, node.getDataLength());
        assertArrayEquals(new byte[] {0x3F, 0x34, 0x00}, node.toBytes());
    }

    @SmallTest
    @Test
    public void testChildren() throws TagNotFoundException, InvalidAsn1DataException {
        Asn1Node node =
                Asn1Node.newBuilder(0x3F34)
                        .addChild(Asn1Node.newBuilder(0x35).addChild(Asn1Node.newBuilder(0x36)))
                        .addChild(Asn1Node.newBuilder(0x35))
                        .addChild(Asn1Node.newBuilder(0x37))
                        .build();

        assertEquals(0x3F34, node.getTag());
        assertEquals(3, node.getChildren().size());
        assertEquals(0x35, node.getChildren().get(0).getTag());
        assertEquals(0x35, node.getChildren().get(1).getTag());
        assertEquals(0x37, node.getChildren().get(2).getTag());
        assertEquals(2, node.getChildren(0x35).size());
        assertEquals(0x35, node.getChildren(0x35).get(0).getTag());
        assertEquals(0x35, node.getChildren(0x35).get(1).getTag());
        assertEquals(1, node.getChild(0x35).getChildren().size());
        assertEquals(0, node.getChildren().get(1).getChildren().size());
        assertEquals(0x36, node.getChild(0x35).getChildren().get(0).getTag());

        assertTrue(node.hasChild(0x35, 0x36));
        assertFalse(node.hasChild(0x38, 0x39));
        assertTrue(node.getChild(0x35).hasChild(0x36));
        assertFalse(node.getChild(0x35).hasChild(0x39));
    }

    @SmallTest
    @Test
    public void testNonExitingChildren() throws TagNotFoundException, InvalidAsn1DataException {
        Asn1Node node =
                Asn1Node.newBuilder(0x3F34)
                        .addChild(Asn1Node.newBuilder(0x34))
                        .addChild(Asn1Node.newBuilder(0x35).addChild(Asn1Node.newBuilder(0x36)))
                        .addChildAsInteger(0x11, 123)
                        .build();

        assertEquals(0, node.getChild(0x34).getChildren(0x37).size());
        assertEquals(0, node.getChildren(0x37).size());
        assertEquals(0, node.getChild(0x11).getChildren(0x37).size());
        assertEquals(0, node.getChild(0x11).getChildren().size());
        try {
            node.getChild(0x35).getChild(0x37);
            fail("0x37 should not exist.");
        } catch (TagNotFoundException e) {
            assertEquals(0x37, e.getTag());
        }
        try {
            node.getChild(0x35, 0x37);
            fail("0x37 should not exist.");
        } catch (TagNotFoundException e) {
            assertEquals(0x37, e.getTag());
        }
        try {
            node.getChild(0x11).getChild(0x37);
            fail("0x37 should not exist.");
        } catch (TagNotFoundException e) {
            assertEquals(0x37, e.getTag());
        }
    }

    @SmallTest
    @Test
    public void testBuilderAndGetters() throws TagNotFoundException, InvalidAsn1DataException {
        Asn1Node node =
                Asn1Node.newBuilder(0x30)
                        .addChildAsInteger(0x11, 1234567)
                        .addChildAsString(0x12, "This is a test.")
                        .addChild(
                                Asn1Node.newBuilder(0x31)
                                        .addChildAsBits(0x13, 0xF2344)
                                        .addChildAsBytes(0x14, new byte[] {-1, 0, -1}))
                        .addChildAsBoolean(0x15, true)
                        .addChildAsBoolean(0x16, false)
                        .addChildren(IccUtils.hexStringToBytes("13040422C4F01403FF00FF"))
                        .build();

        assertEquals(54, node.getEncodedLength());
        assertEquals(52, node.getDataLength());

        final String nodeHex =
                "3034110312D687120F54686973206973206120746573742E310B13040422C4F0"
                        + "1403FF00FF1501FF16010013040422C4F01403FF00FF";
        assertEquals(nodeHex, node.toHex());
        assertEquals("3034", node.getHeadAsHex());
        assertEquals(1234567, node.getChild(0x11).asInteger());
        assertEquals("This is a test.", node.getChild(0x12).asString());
        assertEquals(0xF2344, node.getChild(0x31).getChild(0x13).asBits());
        assertArrayEquals(new byte[] {-1, 0, -1}, node.getChild(0x31).getChild(0x14).asBytes());
        assertTrue(node.getChild(0x15).asBoolean());
        assertFalse(node.getChild(0x16).asBoolean());
        assertEquals(0xF2344, node.getChild(0x13).asBits());
        assertArrayEquals(new byte[] {-1, 0, -1}, node.getChild(0x14).asBytes());
    }

    @SmallTest
    @Test
    public void testGettersInvalidData() throws TagNotFoundException, InvalidAsn1DataException {
        Asn1Node node = Asn1Node.newBuilder(0x30).addChildAsString(0x12, "string").build();
        try {
            node.getChild(0x12).asInteger();
            fail("Should not be parsed.");
        } catch (InvalidAsn1DataException e) {
            assertEquals(0x12, e.getTag());
        }
        try {
            node.getChild(0x12).asBits();
            fail("Should not be parsed.");
        } catch (InvalidAsn1DataException e) {
            assertEquals(0x12, e.getTag());
        }
        try {
            node.getChild(0x12).asBoolean();
            fail("Should not be parsed.");
        } catch (InvalidAsn1DataException e) {
            assertEquals(0x12, e.getTag());
        }
    }

    @SmallTest
    @Test
    public void testWriteToBytes() throws InvalidAsn1DataException {
        final String nodeHex =
                "3031110312D687120F54686973206973206120746573742E310B13040422C4F0"
                        + "1403FF00FF1501FF13040422C4F01403FF00FF";

        Asn1Node node = new Asn1Decoder(nodeHex).nextNode();
        // Writes the bytes before children accessed.
        assertEquals(nodeHex, node.toHex());
        // Writes the bytes after children accessed.
        node.getChildren();
        assertEquals(nodeHex, node.toHex());
    }

    @SmallTest
    @Test
    public void testLongFormLength() throws TagNotFoundException, InvalidAsn1DataException {
        Asn1Node node = Asn1Node.newBuilder(0xBF37).addChildAsBytes(0x5A, new byte[257]).build();
        assertEquals(266, node.getEncodedLength());
        assertEquals(261, node.getDataLength());
        assertEquals(257, node.getChild(0x5A).getDataLength());
        assertEquals("BF37820105", node.getHeadAsHex());

        byte[] expected = new byte[266];
        System.arraycopy(IccUtils.hexStringToBytes("BF378201055A820101"), 0, expected, 0, 9);
        byte[] actual = new byte[266];
        node.writeToBytes(actual, 0);
        assertArrayEquals(expected, actual);
    }

    @SmallTest
    @Test(expected = IllegalArgumentException.class)
    public void testBuilder_NonConstructedTag() {
        Asn1Node.newBuilder(0x87);
    }

    @SmallTest
    @Test(expected = IllegalStateException.class)
    public void testBuilderAddChildAsInteger_ConstructedTag() {
        Asn1Node.newBuilder(0xBF37).addChildAsInteger(0xBF37, 1);
    }

    @SmallTest
    @Test(expected = IllegalStateException.class)
    public void testBuilderAddChildAsString_ConstructedTag() {
        Asn1Node.newBuilder(0xBF37).addChildAsString(0xBF37, "");
    }

    @SmallTest
    @Test(expected = IllegalStateException.class)
    public void testBuilderAddChildAsBytes_ConstructedTag() {
        Asn1Node.newBuilder(0xBF37).addChildAsBytesFromHex(0xBF37, "00");
    }

    @SmallTest
    @Test(expected = IllegalStateException.class)
    public void testBuilderAddChildAsBits_ConstructedTag() {
        Asn1Node.newBuilder(0xBF37).addChildAsBits(0xBF37, 1);
    }

    @SmallTest
    @Test(expected = IllegalStateException.class)
    public void testBuilderAddChildAsBoolean_ConstructedTag() {
        Asn1Node.newBuilder(0xBF37).addChildAsBoolean(0xBF37, true);
    }

    @SmallTest
    @Test(expected = IllegalStateException.class)
    public void testAsInteger_ConstructedTag() throws InvalidAsn1DataException {
        Asn1Node node = Asn1Node.newBuilder(0xBF37).build();
        node.asInteger();
    }

    @SmallTest
    @Test(expected = IllegalStateException.class)
    public void testAsLong_ConstructedTag() throws InvalidAsn1DataException {
        Asn1Node node = Asn1Node.newBuilder(0xBF37).build();
        node.asRawLong();
    }

    @SmallTest
    @Test(expected = IllegalStateException.class)
    public void testAsString_ConstructedTag() throws InvalidAsn1DataException {
        Asn1Node node = Asn1Node.newBuilder(0xBF37).build();
        node.asString();
    }

    @SmallTest
    @Test(expected = IllegalStateException.class)
    public void testAsBytes_ConstructedTag() throws InvalidAsn1DataException {
        Asn1Node node = Asn1Node.newBuilder(0xBF37).build();
        node.asBytes();
    }

    @SmallTest
    @Test(expected = IllegalStateException.class)
    public void testAsBits_ConstructedTag() throws InvalidAsn1DataException {
        Asn1Node node = Asn1Node.newBuilder(0xBF37).build();
        node.asBits();
    }

    @SmallTest
    @Test(expected = IllegalStateException.class)
    public void testAsBoolean_ConstructedTag() throws InvalidAsn1DataException {
        Asn1Node node = Asn1Node.newBuilder(0xBF37).build();
        node.asBoolean();
    }

    @SmallTest
    @Test(expected = InvalidAsn1DataException.class)
    public void testAsBoolean_InvalidData() throws TagNotFoundException, InvalidAsn1DataException {
        Asn1Node node = Asn1Node.newBuilder(0xBF37).addChildAsString(1, "1").build();
        node.getChild(1).asBoolean();
    }

    @SmallTest
    @Test(expected = IndexOutOfBoundsException.class)
    public void testWriteToBytes_IndexOutOfBounds()
            throws TagNotFoundException, InvalidAsn1DataException {
        Asn1Node node = Asn1Node.newBuilder(0xBF37).addChildAsString(1, "1").build();
        byte[] bytes = new byte[1];
        node.writeToBytes(bytes, 0);
    }
}
