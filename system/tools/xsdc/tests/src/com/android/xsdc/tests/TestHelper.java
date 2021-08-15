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

package com.android.xsdc.tests;

import com.android.xsdc.FileSystem;
import com.android.xsdc.XmlSchema;
import com.android.xsdc.XsdHandler;
import com.android.xsdc.java.JavaCodeGenerator;

import javax.tools.*;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import java.io.*;
import java.net.URI;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;

import static org.junit.Assert.fail;

class TestHelper {
    static class InMemoryJavaFileObject extends SimpleJavaFileObject {
        private final String contents;

        InMemoryJavaFileObject(String className, String contents) {
            super(URI.create("string:///" + className.replace('.', '/') + Kind.SOURCE.extension),
                    Kind.SOURCE);
            this.contents = contents;
        }

        @Override
        public CharSequence getCharContent(boolean ignoreEncodingErrors) {
            return contents;
        }
    }

    static class InMemoryJavaClassObject extends SimpleJavaFileObject {
        private ByteArrayOutputStream baos;
        private String name;

        InMemoryJavaClassObject(String name, Kind kind) {
            super(URI.create("string:///" + name.replace('.', '/') + kind.extension), kind);
            baos = new ByteArrayOutputStream();
            this.name = name;
        }

        byte[] getBytes() {
            return baos.toByteArray();
        }

        @Override
        public OutputStream openOutputStream() {
            return baos;
        }

        String getClassName() {
            return name;
        }
    }

    static class InMemoryClassManager extends ForwardingJavaFileManager<JavaFileManager> {
        private List<InMemoryJavaClassObject> classObjects;

        InMemoryClassManager(JavaFileManager fileManager) {
            super(fileManager);
            classObjects = new ArrayList<>();
        }

        @Override
        public JavaFileObject getJavaFileForOutput(Location location, String name,
                JavaFileObject.Kind kind, FileObject sibling) {
            InMemoryJavaClassObject object = new InMemoryJavaClassObject(name, kind);
            classObjects.add(object);
            return object;
        }

        List<InMemoryJavaClassObject> getAllClasses() {
            return classObjects;
        }
    }

    final static String packageName = "test";

    static TestCompilationResult parseXsdAndCompile(InputStream in) throws Exception {
        SAXParserFactory factory = SAXParserFactory.newInstance();
        factory.setNamespaceAware(true);
        SAXParser parser = factory.newSAXParser();
        XsdHandler xsdHandler = new XsdHandler();
        parser.parse(in, xsdHandler);
        XmlSchema xmlSchema = xsdHandler.getSchema();
        Map<String, StringBuffer> fileOutputMap = new HashMap<>();
        FileSystem fs = new FileSystem(fileOutputMap);
        JavaCodeGenerator javaCodeGenerator = new JavaCodeGenerator(xmlSchema, packageName);
        javaCodeGenerator.print(fs);
        List<JavaFileObject> javaFileObjects = new ArrayList<>();
        for (Map.Entry<String, StringBuffer> entry : fileOutputMap.entrySet()) {
            String className = entry.getKey().split("\\.")[0];
            javaFileObjects.add(
                    new InMemoryJavaFileObject(className, entry.getValue().toString()));
        }
        return new TestCompilationResult(compile(javaFileObjects));
    }

    private static List<InMemoryJavaClassObject> compile(List<JavaFileObject> javaFileObjects)
            throws IOException {
        JavaCompiler compiler = ToolProvider.getSystemJavaCompiler();
        DiagnosticCollector<JavaFileObject> diagnostics = new DiagnosticCollector<>();
        List<InMemoryJavaClassObject> ret = null;

        try (InMemoryClassManager fileManager = new InMemoryClassManager(
                compiler.getStandardFileManager(diagnostics, null, null))) {
            JavaCompiler.CompilationTask task = compiler.getTask(null, fileManager, diagnostics,
                    null, null, javaFileObjects);
            boolean success = task.call();

            if (!success) {
                StringBuilder log = new StringBuilder();
                log.append("Compilation failed!\n\n");
                for (Diagnostic diagnostic : diagnostics.getDiagnostics()) {
                    log.append("Code: ").append(diagnostic.getCode()).append("\n");
                    log.append("Kind: " + diagnostic.getKind() + "\n");
                    log.append("Line: " + diagnostic.getLineNumber() + "\n");
                    log.append("Column: " + diagnostic.getColumnNumber() + "\n");
                    log.append("Source: " + diagnostic.getSource() + "\n");
                    log.append("Message: " + diagnostic.getMessage(Locale.getDefault()) + "\n");
                }
                fail(log.toString());
            }
            ret = fileManager.getAllClasses();
        }
        return ret;
    }
}