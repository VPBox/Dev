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

import java.util.Collections;
import java.util.List;

public class XsdUnion extends XsdSimpleType {
    final private List<XsdType> memberTypes;

    public XsdUnion(String name, List<XsdType> memberTypes) throws XsdParserException {
        super(name);
        if (memberTypes == null || memberTypes.isEmpty()) {
            throw new XsdParserException("union memberTypes should exist in simpleType");
        }
        this.memberTypes = Collections.unmodifiableList(memberTypes);
    }

    public List<XsdType> getMemberTypes() {
        return memberTypes;
    }
}
