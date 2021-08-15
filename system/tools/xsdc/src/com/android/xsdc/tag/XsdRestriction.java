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

import java.util.Collections;
import java.util.List;
import com.android.xsdc.XsdParserException;

public class XsdRestriction extends XsdSimpleType {
    final private XsdType base;
    final private List<XsdEnumeration> enums;

    public XsdRestriction(String name, XsdType base, List<XsdEnumeration> enums)
            throws XsdParserException {
        super(name);
        if (base == null) {
            throw new XsdParserException("restriction base should exist in simpleType");
        }
        this.base = base;
        this.enums = enums;
    }

    public XsdType getBase() {
        return base;
    }

    public List<XsdEnumeration> getEnums() {
        return enums;
    }
}
