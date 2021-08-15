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

package com.android.server.wifi.hotspot2.omadm;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Test;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

import java.util.Arrays;
import java.util.List;

/**
 * Unit tests for {@link MoSerializer}.
 */
@SmallTest
public class MoSerializerTest {
    MoSerializer mMoSerializer;
    Document mDocument;
    static final String TEST_NODE = "test_node";
    static final String TEST_VALUE = "test_value";
    static final String TEST_URN = "urn:oma:mo:oma-dm-devinfo:1.0";

    @Before
    public void setUp() throws Exception {
        mMoSerializer = new MoSerializer();
        mDocument = mMoSerializer.createNewDocument();
    }

    /**
     * Verify that the returned element should be valid element.
     *
     * Expected output: <MgmtTree></MgmtTree>
     */
    @Test
    public void testShouldReturnValidElementForCreateMgmtTree() {
        Element element = mMoSerializer.createMgmtTree(mDocument);

        assertEquals(element.getTagName(), MoSerializer.TAG_MGMT_TREE);
        assertTrue(mDocument.hasChildNodes());
    }

    /**
     * Verify that the returned element should be valid element.
     *
     * Expected output: <VerDTD>[value]</VerDTD>
     */
    @Test
    public void testShouldReturnValidElementForWriteVersion() {
        Element element = mMoSerializer.writeVersion(mDocument);

        assertEquals(element.getTagName(), MoSerializer.TAG_VERSION);
        Node childNode = element.getFirstChild();
        assertNotNull(childNode);
        assertEquals(childNode.getNodeValue(), MoSerializer.DM_VERSION);
    }

    /**
     * Verify that the returned element should be valid element.
     *
     * Expected output: <RTProperties><Type><DDFName>[urn]</DDFName></Type></RTProperites>
     */
    @Test
    public void testShouldReturnValidElementForCreateNodeForUrn() {
        Element element = mMoSerializer.createNodeForUrn(mDocument, TEST_URN);

        assertEquals(element.getTagName(), MoSerializer.TAG_RTPROPERTIES);
        assertEquals(element.getChildNodes().getLength(), 1);
        Node childNode = element.getFirstChild();
        assertTrue(childNode instanceof Element);
        assertEquals(((Element)childNode).getTagName(), MoSerializer.TAG_TYPE);

        Node childNode2 = childNode.getFirstChild();
        assertTrue(childNode2 instanceof Element);
        assertEquals(((Element)childNode2).getTagName(), MoSerializer.TAG_DDF_NAME);
        Node textNode = childNode2.getFirstChild();
        assertNotNull(textNode);
        assertEquals(textNode.getNodeValue(), TEST_URN);
    }

    /**
     * Verify that the returned element should be valid element.
     *
     * Expected output: <Node><NodeName>[nodeName]</NodeName></Node>
     */
    @Test
    public void testShouldReturnValidElementForCreateNode() {
        Element element = mMoSerializer.createNode(mDocument, TEST_NODE);

        assertEquals(element.getTagName(), MoSerializer.TAG_NODE);
        assertEquals(element.getChildNodes().getLength(), 1);
        Node childNode = element.getFirstChild();
        assertTrue(childNode instanceof Element);
        Node textNode = childNode.getFirstChild();
        assertNotNull(textNode);
        assertEquals(textNode.getNodeValue(), TEST_NODE);
    }

    /**
     * Verify that the returned element should be valid element.
     *
     * Expected output:
     * <Node><NodeName>[name]</NodeName><Value>[value]</Value></Node>
     */
    @Test
    public void testShouldReturnValidElementForCreateNodeForValue() {
        Element element = mMoSerializer.createNodeForValue(mDocument, TEST_NODE, TEST_VALUE);

        assertEquals(element.getTagName(), MoSerializer.TAG_NODE);
        assertEquals(element.getChildNodes().getLength(), 2);
        List<String> values = Arrays.asList(TEST_NODE, TEST_VALUE);
        Node childNode;
        Node textNode;
        for (String value : values) {
            childNode = element.getFirstChild();
            assertTrue(childNode instanceof Element);
            textNode = childNode.getFirstChild();
            assertNotNull(textNode);
            assertEquals(textNode.getNodeValue(), value);
            element.removeChild(childNode);
        }
    }
}
