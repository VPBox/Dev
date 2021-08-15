/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.xsdc.tag;

import com.android.xsdc.XsdParserException;

import javax.xml.namespace.QName;

public class XsdElement extends XsdTag {
    final private XsdType type;
    private boolean multiple;

    public XsdElement(String name, QName ref, XsdType type, boolean multiple)
            throws XsdParserException {
        super(name, ref);
        if (name == null && ref == null) {
            throw new XsdParserException("name and ref cannot be both null");
        }
        if (name != null && type == null) {
            throw new XsdParserException(
                    String.format("In element '%s', type definition should exist", name));
        }
        this.type = type;
        this.multiple = multiple;
    }

    public XsdType getType() {
        return type;
    }

    public void setMultiple(boolean multiple) {
        this.multiple = multiple;
    }

    public boolean isMultiple() {
        return multiple;
    }
}
