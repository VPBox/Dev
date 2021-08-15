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

package com.android.xsdc;

import java.io.Closeable;
import java.io.PrintWriter;

public class CodeWriter implements Closeable {
    private PrintWriter out;
    private int indent;
    private boolean startLine;

    public CodeWriter(PrintWriter printWriter) {
        out = printWriter;
        indent = 0;
        startLine = true;
    }

    private void printIndent() {
        assert startLine;
        for (int i = 0; i < indent; ++i) {
            out.print("    ");
        }
        startLine = false;
    }

    public void println() {
        out.println();
        startLine = true;
    }

    public void println(String code) {
        print(code + "\n");
    }

    public void print(String code) {
        String[] lines = code.split("\n", -1);
        for (int i = 0; i < lines.length; ++i) {
            // trim only start of line for more flexibility
            String line = lines[i].replaceAll("^\\s+", "");
            if (line.startsWith("}")) {
                --indent;
            }
            if (startLine && !line.isEmpty()) {
                printIndent();
            }
            out.print(line);
            if (line.endsWith("{")) {
                ++indent;
            }
            if (i + 1 < lines.length) {
                out.println();
                startLine = true;
            }
        }
    }

    public void printf(String code, Object... arguments) {
        print(String.format(code, arguments));
    }

    @Override
    public void close() {
        if (out != null) {
            out.close();
        }
    }
}
