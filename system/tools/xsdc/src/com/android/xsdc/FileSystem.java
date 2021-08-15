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

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.Map;

public class FileSystem {
    private File rootDirectory;
    private Map<String, StringBuffer> fileOutputMap;

    public FileSystem(File rootDirectory) {
        this.rootDirectory = rootDirectory;
    }

    public FileSystem(Map<String, StringBuffer> fileOutputMap) {
        this.fileOutputMap = fileOutputMap;
    }

    public PrintWriter getPrintWriter(String fileName) throws IOException {
        if (rootDirectory != null) {
            return new PrintWriter(new File(rootDirectory, fileName));
        } else {
            StringWriter sw = new StringWriter();
            fileOutputMap.put(fileName, sw.getBuffer());
            return new PrintWriter(sw);
        }
    }
}
