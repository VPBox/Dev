/*
 * Copyright (C) 2008 The Android Open Source Project
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


package com.android.tools.layoutlib.create;

import com.android.tools.layoutlib.create.AsmAnalyzer.DependencyVisitor;
import com.android.tools.layoutlib.create.AsmAnalyzer.Result;

import org.junit.Test;
import org.objectweb.asm.ClassReader;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

/**
 * Unit tests for some methods of {@link AsmAnalyzer}.
 */
public class AsmAnalyzerTest {
    private static final List<String> MOCK_ANDROID_JAR;
    private static final String[] DEFAULT_EXCLUDES = new String[]{"notjava.lang.JavaClass"};
    private static final String[] DEFAULT_INCLUDE_FILES = new String[]{"mock_android/data/data*"};

    static {
        List<String> mockJar = new ArrayList<>();
        URL url = AsmAnalyzerTest.class.getClassLoader().getResource("data/mock_android.jar");
        assert url != null : "Unable to locate mock_android.jar";
        mockJar.add(url.getFile());
        MOCK_ANDROID_JAR = Collections.unmodifiableList(mockJar);
    }

    private static AsmAnalyzer getDefaultAnalyzer() {
        MockLog log = new MockLog();
        return new AsmAnalyzer(log, MOCK_ANDROID_JAR, null ,
                null /* includeGlobs */, DEFAULT_EXCLUDES, DEFAULT_INCLUDE_FILES);
    }

    @Test
    public void testParseZip() throws IOException {
        Map<String, ClassReader> map = new TreeMap<>();
        Map<String, InputStream> filesFound = new TreeMap<>();

        getDefaultAnalyzer().parseZip(MOCK_ANDROID_JAR, map, filesFound);

        assertArrayEquals(new String[] {
                "mock_android.dummy.DummyClass",
                "mock_android.dummy.InnerTest",
                "mock_android.dummy.InnerTest$1",
                "mock_android.dummy.InnerTest$DerivingClass",
                "mock_android.dummy.InnerTest$MyGenerics1",
                "mock_android.dummy.InnerTest$MyIntEnum",
                "mock_android.dummy.InnerTest$MyStaticInnerClass",
                "mock_android.dummy.InnerTest$NotStaticInner1",
                "mock_android.dummy.InnerTest$NotStaticInner2",
                "mock_android.dummy.subpackage.SubpackageClassA",
                "mock_android.dummy.subpackage.SubpackageClassB",
                "mock_android.dummy.subpackage.SubpackageClassC",
                "mock_android.dummy.subpackage.SubpackageClassC$InnerClass",
                "mock_android.dummy.subpackage.SubpackageClassC$StaticInnerClass",
                "mock_android.dummy2.DummyClass",
                "mock_android.dummy2.keep.DoNotRemove",
                "mock_android.util.EmptyArray",
                "mock_android.util.NotNeeded",
                "mock_android.view.View",
                "mock_android.view.ViewGroup",
                "mock_android.view.ViewGroup$LayoutParams",
                "mock_android.view.ViewGroup$MarginLayoutParams",
                "mock_android.widget.LinearLayout",
                "mock_android.widget.LinearLayout$LayoutParams",
                "mock_android.widget.TableLayout",
                "mock_android.widget.TableLayout$LayoutParams",
                "notjava.lang.JavaClass",
            },
            map.keySet().toArray());
        assertArrayEquals(new String[] {"mock_android/data/dataFile"},
            filesFound.keySet().toArray());
    }

    @Test
    public void testFindClass() throws IOException {
        Map<String, ClassReader> zipClasses = new TreeMap<>();
        Map<String, InputStream> filesFound = new TreeMap<>();

        getDefaultAnalyzer().parseZip(MOCK_ANDROID_JAR, zipClasses, filesFound);
        TreeMap<String, ClassReader> found = new TreeMap<>();

        ClassReader cr = AsmAnalyzer.findClass("mock_android.view.ViewGroup$LayoutParams",
                zipClasses, found);

        assertNotNull(cr);
        assertEquals("mock_android/view/ViewGroup$LayoutParams", cr.getClassName());
        assertArrayEquals(new String[] { "mock_android.view.ViewGroup$LayoutParams" },
                found.keySet().toArray());
        assertArrayEquals(new ClassReader[] { cr }, found.values().toArray());
    }

    @Test
    public void testInclude() throws IOException {
        AsmAnalyzer analyzer = new AsmAnalyzer(new MockLog(), MOCK_ANDROID_JAR, null,
                new String[] {
                    "mock_android.util.EmptyArray", // Single class select
                    "mock_android.dummy.**", // Multi package select
                    "mock_android.dummy2.*", // Exclude subpackages select
                },
                DEFAULT_EXCLUDES,
                DEFAULT_INCLUDE_FILES);
        Result result = analyzer.analyze();
        assertArrayEquals(new String[] {
                        "mock_android.dummy.DummyClass",
                        "mock_android.dummy.InnerTest$MyIntEnum",
                        "mock_android.util.EmptyArray",
                        "mock_android.dummy.InnerTest$DerivingClass",
                        "mock_android.dummy2.DummyClass",
                        "mock_android.dummy.subpackage.SubpackageClassC$InnerClass",
                        "mock_android.dummy.InnerTest$MyGenerics1",
                        "mock_android.dummy.subpackage.SubpackageClassC$StaticInnerClass",
                        "mock_android.dummy.InnerTest$MyStaticInnerClass",
                        "mock_android.dummy.InnerTest$NotStaticInner1",
                        "mock_android.dummy.InnerTest$NotStaticInner2",
                        "mock_android.dummy.subpackage.SubpackageClassA",
                        "mock_android.dummy.InnerTest",
                        "mock_android.dummy.InnerTest$1",
                        "mock_android.dummy.subpackage.SubpackageClassC",
                        "mock_android.dummy.subpackage.SubpackageClassB",
                },
                result.getFound().keySet().toArray());
    }

    @Test
    public void testFindClassesDerivingFrom() throws IOException {
        Map<String, ClassReader> zipClasses = new TreeMap<>();
        Map<String, InputStream> filesFound = new TreeMap<>();

        getDefaultAnalyzer().parseZip(MOCK_ANDROID_JAR, zipClasses, filesFound);
        TreeMap<String, ClassReader> found = new TreeMap<>();

        AsmAnalyzer.findClassesDerivingFrom("mock_android.view.View", zipClasses, found);

        assertArrayEquals(new String[] {
                "mock_android.view.View",
                "mock_android.view.ViewGroup",
                "mock_android.widget.LinearLayout",
                "mock_android.widget.TableLayout",
            },
            found.keySet().toArray());

        for (String key : found.keySet()) {
            ClassReader value = found.get(key);
            assertNotNull("No value for " + key, value);
            assertEquals(key, AsmAnalyzer.classReaderToClassName(value));
        }
    }

    @Test
    public void testDependencyVisitor() throws IOException {
        Map<String, ClassReader> zipClasses = new TreeMap<>();
        Map<String, InputStream> filesFound = new TreeMap<>();

        getDefaultAnalyzer().parseZip(MOCK_ANDROID_JAR, zipClasses, filesFound);
        TreeMap<String, ClassReader> keep = new TreeMap<>();
        TreeMap<String, ClassReader> new_keep = new TreeMap<>();
        TreeMap<String, ClassReader> in_deps = new TreeMap<>();
        TreeMap<String, ClassReader> out_deps = new TreeMap<>();

        ClassReader cr = AsmAnalyzer.findClass("mock_android.widget.LinearLayout", zipClasses, keep);
        DependencyVisitor visitor = getDefaultAnalyzer().getVisitor(zipClasses, keep, new_keep, in_deps, out_deps);

        // get first level dependencies
        cr.accept(visitor, 0 /* flags */);

        assertArrayEquals(new String[] {
                "mock_android.util.EmptyArray",
                "mock_android.view.ViewGroup",
                "mock_android.widget.LinearLayout$LayoutParams",
            },
            out_deps.keySet().toArray());

        in_deps.putAll(out_deps);
        out_deps.clear();

        // get second level dependencies
        for (ClassReader cr2 : in_deps.values()) {
            cr2.accept(visitor, 0 /* flags */);
        }

        assertArrayEquals(new String[] {
                "mock_android.view.View",
                "mock_android.view.ViewGroup$LayoutParams",
                "mock_android.view.ViewGroup$MarginLayoutParams",
            },
            out_deps.keySet().toArray());

        in_deps.putAll(out_deps);
        out_deps.clear();

        // get third level dependencies (there are none)
        for (ClassReader cr2 : in_deps.values()) {
            cr2.accept(visitor, 0 /* flags */);
        }
        keep.putAll(new_keep);

        assertArrayEquals(new String[] { }, out_deps.keySet().toArray());
        assertArrayEquals(new String[] {
                "mock_android.widget.LinearLayout",
                "notjava.lang.JavaClass",
        }, keep.keySet().toArray());
    }
}
