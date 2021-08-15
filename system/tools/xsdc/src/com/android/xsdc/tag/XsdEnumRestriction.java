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

public class XsdEnumRestriction extends XsdTag {
    final private XsdType base;
    final private List<XsdEnumeration> enums;

    public XsdEnumRestriction(XsdType base, List<XsdEnumeration> enums) {
        super(null, null);
        this.base = base;
        this.enums = Collections.unmodifiableList(enums);
    }

    public XsdType getBase() {
        return base;
    }

    public List<XsdEnumeration> getEnums() {
        return enums;
    }
}
