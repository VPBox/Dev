/*
 * Copyright (C) 2017 The Android Open Source Project
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

package com.android.libcore.timezone.tzlookup;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

/**
 * Stores context, errors and error severity for logging and flow control.
 */
final class Errors {

    private final static int LEVEL_WARNING = 1;
    private final static int LEVEL_ERROR = 2;
    private final static int LEVEL_FATAL = 3;

    private int level = 0;

    private final LinkedList<String> scopes = new LinkedList<>();
    private final List<String> messages = new ArrayList<>();

    Errors() {
    }

    void pushScope(String name) {
        scopes.add(name);
    }

    String popScope() {
        return scopes.removeLast();
    }

    void addFatal(String msg) {
        level = Math.max(level, LEVEL_FATAL);
        add(msg);
    }

    void addError(String msg) {
        level = Math.max(level, LEVEL_ERROR);
        add(msg);
    }

    void addWarning(String msg) {
        level = Math.max(level, LEVEL_WARNING);
        add(msg);
    }

    String asString() {
        StringBuilder sb = new StringBuilder();
        for (String message : messages) {
            sb.append(message);
            sb.append("\n");
        }
        return sb.toString();
    }

    boolean isEmpty() {
        return messages.isEmpty();
    }

    boolean hasError() {
        return level >= LEVEL_ERROR;
    }

    boolean hasFatal() {
        return level >= LEVEL_FATAL;
    }

    private void add(String msg) {
        messages.add(scopes.toString() + ": " + msg);
    }
}
