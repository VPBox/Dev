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

package com.android.libcore.timezone.tzlookup.zonetree;

import org.junit.Test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static java.util.Arrays.asList;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertSame;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

public class TreeNodeTest {

    private static class TestTreeVisitor implements TreeNode.Visitor<TestTreeNode> {

        private List<String> ids = new ArrayList<>();

        @Override
        public void visit(TestTreeNode node) {
            ids.add(node.getId());
        }

        public void reset() {
            ids.clear();
        }
    }

    private static class TestTreeNode extends TreeNode<TestTreeNode> {
        public TestTreeNode(String id) {
            super(id);
        }
    }

    @Test
    public void testTreeNode_single() {
        TestTreeNode testTreeNode = new TestTreeNode("id");
        assertEquals("id", testTreeNode.getId());
        assertEquals(0, testTreeNode.getChildrenCount());
        assertTrue(testTreeNode.getChildren().isEmpty());
        assertNull(testTreeNode.getParent());
        assertTrue(testTreeNode.isRoot());
        assertTrue(testTreeNode.isLeaf());

        TestTreeVisitor testVisitor = new TestTreeVisitor();
        testTreeNode.visitSelfThenChildrenRecursive(testVisitor);
        assertEquals(testVisitor.ids, asList("id"));
    }

    @Test
    public void testTreeNode_many() {
        TestTreeNode node1 = new TestTreeNode("1");
        TestTreeNode node11 = new TestTreeNode("1.1");
        TestTreeNode node12 = new TestTreeNode("1.2");
        TestTreeNode node111 = new TestTreeNode("1.1.1");
        TestTreeNode node112 = new TestTreeNode("1.1.2");
        TestTreeNode node113 = new TestTreeNode("1.1.3");
        TestTreeNode node121 = new TestTreeNode("1.2.1");
        TestTreeNode node122 = new TestTreeNode("1.2.2");
        TestTreeNode node123 = new TestTreeNode("1.2.3");

        // Build the tree.
        node1.addChild(node11);
        node1.addChild(node12);
        node11.addChild(node111);
        node11.addChild(node112);
        node11.addChild(node113);
        node12.addChild(node121);
        node12.addChild(node122);
        node12.addChild(node123);

        assertTrue(node1.isRoot());
        assertFalse(node1.isLeaf());
        assertFalse(node11.isRoot());
        assertFalse(node11.isLeaf());
        assertFalse(node111.isRoot());
        assertTrue(node111.isLeaf());

        // Visit the tree.
        TestTreeVisitor testVisitor = new TestTreeVisitor();
        node1.visitSelfThenChildrenRecursive(testVisitor);
        assertEquals(
                asList("1", "1.1", "1.1.1", "1.1.2", "1.1.3", "1.2", "1.2.1", "1.2.2", "1.2.3"),
                testVisitor.ids);

        // Remove a node from the tree.
        node1.removeChild(node11);
        assertNull(node11.getParent());
        assertTrue(node11.isRoot());
        assertFalse(node11.isLeaf());

        // Visit the tree again.
        testVisitor.reset();
        node1.visitSelfThenChildrenRecursive(testVisitor);
        assertEquals(asList("1", "1.2", "1.2.1", "1.2.2", "1.2.3"), testVisitor.ids);

        // Visit the removed node.
        testVisitor.reset();
        node11.visitSelfThenChildrenRecursive(testVisitor);
        assertEquals(
                asList("1.1", "1.1.1", "1.1.2", "1.1.3"),
                testVisitor.ids);
    }

    @Test
    public void testTreeNode_cannotAddTwice() {
        TestTreeNode node1 = new TestTreeNode("1");
        TestTreeNode node11 = new TestTreeNode("1.1");
        node1.addChild(node11);

        try {
            node1.addChild(node11);
            fail();
        } catch (IllegalStateException expected) {
        }

        assertSame(node11, node1.removeChild(node11));
        node1.addChild(node11);

        TestTreeNode unattachedNode = new TestTreeNode("Unattached");
        assertNull(node1.removeChild(unattachedNode));
    }
}
