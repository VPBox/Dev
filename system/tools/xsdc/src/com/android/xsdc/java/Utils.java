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

package com.android.xsdc.java;

import java.util.Arrays;
import java.util.HashSet;

class Utils {
    private static final String[] keywords = {
            "abstract", "assert", "boolean", "break", "byte", "case",
            "catch", "char", "class", "const", "continue", "default",
            "double", "do", "else", "enum", "extends", "false",
            "final", "finally", "float", "for", "goto", "if",
            "implements", "import", "instanceof", "int", "interface", "long",
            "native", "new", "null", "package", "private", "protected",
            "public", "return", "short", "static", "strictfp", "super",
            "switch", "synchronized", "this", "throw", "throws", "transient",
            "true", "try", "void", "volatile", "while"
    };
    private static final HashSet<String> keywordSet = new HashSet<>(Arrays.asList(keywords));

    private static String toCamelCase(String[] words) {
        String res = words[0];
        for (int idx = 1; idx < words.length; ++idx) {
            res += capitalize(words[idx]);
        }
        return res;
    }

    static String capitalize(String input) {
        return input.substring(0, 1).toUpperCase() + input.substring(1);
    }

    private static String lowerize(String input) {
        return input.substring(0, 1).toLowerCase() + input.substring(1);
    }

    static String toVariableName(String name) throws JavaCodeGeneratorException {
        String trimmed = toCamelCase(name.replaceAll("[^A-Za-z0-9_-]", "").split("-"));
        if (trimmed.isEmpty()) {
            throw new JavaCodeGeneratorException(
                    String.format("cannot convert to a variable name : %s", name));
        }
        String lowered = Character.isDigit(trimmed.charAt(0)) ? "_" + trimmed
                : lowerize(trimmed);
        // always starts with a lowercase or underscore character.
        return (keywordSet.contains(trimmed)) ? "_" + lowered : lowered;
    }

    static String toClassName(String name) throws JavaCodeGeneratorException {
        String trimmed = toCamelCase(
                name.replaceAll("[^A-Za-z0-9_-]", "").replaceAll("-","_").split("_"));
        if (trimmed.isEmpty() || Character.isDigit(trimmed.charAt(0))) {
            throw new JavaCodeGeneratorException(
                    String.format("cannot convert to a class name : %s", name));
        }
        return capitalize(trimmed);
    }

    static String toEnumName(String name) throws JavaCodeGeneratorException {
        String trimmed = name.replace(".", "_").replaceAll("[^A-Za-z0-9_]", "");
        if (trimmed.isEmpty()) {
            throw new JavaCodeGeneratorException(
                    String.format("cannot convert to a variable name : %s", name));
        }
        String enumName = Character.isDigit(trimmed.charAt(0)) ? "_" + trimmed
                : trimmed;
        return (keywordSet.contains(trimmed)) ? "_" + enumName : enumName;
    }
}

