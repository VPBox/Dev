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

import java.util.Arrays;
import java.util.HashSet;

class Utils {
    private static final String[] keywords = {
        "alignas", "alignof", "and", "and_eq", "asm", "atomic_cancel", "atomic_commit",
        "atomic_noexcept", "auto", "bitand", "bitor", "bool", "break", "case", "catch",
        "char", "char16_t", "char32_t", "class", "compl", "concept", "const", "constexpr",
        "const_cast", "continue", "decltype", "default", "delete", "do", "double",
        "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float",
        "for", "friend", "goto", "if", "inline", "int", "import", "long", "module", "mutable",
        "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq",
        "private", "protected", "public", "register", "reinterpret_cast", "requires", "return",
        "short", "signed", "sizeof", "static", "static_assert", "static_cast", "struct",
        "switch", "synchronized", "template", "this", "thread_local", "throw", "true", "try",
        "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void",
        "volatile", "wchar_t", "while", "xor", "xor_eq",
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

    static String lowerize(String input) {
        return input.substring(0, 1).toLowerCase() + input.substring(1);
    }

    static String toVariableName(String name) throws CppCodeGeneratorException {
        String trimmed = toCamelCase(name.replaceAll("[^A-Za-z0-9_-]", "").split("-"));
        if (trimmed.isEmpty()) {
            throw new CppCodeGeneratorException(
                    String.format("cannot convert to a variable name : %s", name));
        }
        String lowered = Character.isDigit(trimmed.charAt(0)) ? "_" + trimmed
                : lowerize(trimmed);
        // always starts with a lowercase or underscore character.
        return (keywordSet.contains(trimmed)) ? "_" + lowered : lowered;
    }

    static String toClassName(String name) throws CppCodeGeneratorException {
        String trimmed = toCamelCase(
                name.replaceAll("[^A-Za-z0-9_-]", "").replaceAll("-","_").split("_"));
        if (trimmed.isEmpty() || Character.isDigit(trimmed.charAt(0))) {
            throw new CppCodeGeneratorException(
                    String.format("cannot convert to a class name : %s", name));
        }
        return capitalize(trimmed);
    }

    static String toEnumName(String name) throws CppCodeGeneratorException {
        String trimmed = name.replace(".", "_").replaceAll("[^A-Za-z0-9_]", "");
        if (trimmed.isEmpty()) {
            throw new CppCodeGeneratorException(
                    String.format("cannot convert to a variable name : %s", name));
        }
        String enumName = Character.isDigit(trimmed.charAt(0)) ? "_" + trimmed : trimmed;
        return (keywordSet.contains(trimmed)) ? "_" + enumName : enumName;
    }
}
