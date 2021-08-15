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

import java.util.ArrayList;
import java.util.List;

/**
 * An abstract base class for a general tree node. Each tree node has an id, a link to its parent
 * and zero or more child nodes.
 *
 * @param <V> the type of the TreeNode subclass
 */
abstract class TreeNode<V extends TreeNode<V>> {

    private V parent;
    private final String id;
    private final List<V> children = new ArrayList<>();

    public TreeNode(String id) {
        this.id = id;
    }

    public final String getId() {
        return id;
    }

    @SuppressWarnings("unchecked")
    protected final V getThis() {
        return (V) this;
    }

    public final V getParent() {
        return parent;
    }

    /** For use by {@link #addChild(TreeNode)} and {@link #removeChild(TreeNode)} only. */
    protected final void setParent(V newParent) {
        parent = newParent;
    }

    /**
     * Adds the child and sets the parent of the child. The child must not already have a parent.
     */
    public final void addChild(V e) {
        if (e.getParent() != null) {
            throw new IllegalStateException("Node already attached");
        }
        children.add(e);
        e.setParent(getThis());
    }

    public final List<V> getChildren() {
        return new ArrayList<>(children);
    }

    public final int getChildrenCount() {
        return children.size();
    }

    /**
     * Recursively visit this node and then all children.
     */
    public final void visitSelfThenChildrenRecursive(Visitor<V> visitor) {
        visitor.visit(getThis());
        for (V child : getChildren()) {
            child.visitSelfThenChildrenRecursive(visitor);
        }
    }

    /**
     * Remove a single child. {@link Object#equals(Object)} is used to
     * identify the child node to remove. The parent of the node to be removed is set to null.
     *
     * <p>Returns the node removed, or {@code null} if the node could not be removed.
     */
    public final V removeChild(V toRemove) {
        for (int i = 0; i < children.size(); i++) {
            V candidate = children.get(i);
            if (toRemove.equals(candidate)) {
                toRemove.setParent(null);
                children.remove(i);
                return toRemove;
            }
        }
        return null;
    }

    public final boolean isRoot() {
        return getParent() == null;
    }

    public final boolean isLeaf() {
        return getChildrenCount() == 0;
    }

    /**
     * A visitor of {@link TreeNode}.
     * @param <N> the type of tree node
     */
    public interface Visitor<N extends TreeNode<N>> {
        void visit(N node);
    }
}
