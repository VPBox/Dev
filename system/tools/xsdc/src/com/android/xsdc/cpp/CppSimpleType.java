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

package com.android.xsdc.cpp;

class CppSimpleType implements CppType {
    final private String name;
    final private String fullName;
    final private String rawParsingExpression;
    final private boolean list;

    CppSimpleType(String name, String rawParsingExpression, boolean list) {
        this.rawParsingExpression = rawParsingExpression;
        this.list = list;
        this.name = name;
        this.fullName = list ? String.format("std::vector<%s>", name) : name;
    }

    boolean isList() {
        return list;
    }

    CppSimpleType newListType() throws CppCodeGeneratorException {
        if (list) throw new CppCodeGeneratorException("list of list is not supported");
        return new CppSimpleType(name, rawParsingExpression, true);
    }

    public String getTypeName() {
        return name;
    }

    @Override
    public String getName() {
        return fullName;
    }

    @Override
    public String getParsingExpression() {
        StringBuilder expression = new StringBuilder();
        if (list) {
            expression.append(
                    String.format("%s value;\n", getName()));
            expression.append("for (auto& token : android::base::Split(raw, \" \")) {\n");
            expression.append(String.format("value.push_back(std::move(%s));\n",
                    String.format(rawParsingExpression, "token")));
            expression.append("}\n");
        } else {
            expression.append(
                    String.format("%s value = %s;\n", getName(),
                            String.format(rawParsingExpression, "raw")));
        }
        return expression.toString();
    }
}
