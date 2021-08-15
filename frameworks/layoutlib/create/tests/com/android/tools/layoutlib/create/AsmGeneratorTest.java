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


import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.objectweb.asm.ClassReader;
import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.FieldVisitor;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Type;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.URL;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

/**
 * Unit tests for some methods of {@link AsmGenerator}.
 */
public class AsmGeneratorTest {
    private MockLog mLog;
    private ArrayList<String> mOsJarPath;
    private String mOsDestJar;
    private File mTempFile;

    // ASM internal name for the the class in java package that should be refactored.
    private static final String JAVA_CLASS_NAME = "notjava.lang.JavaClass";

    @Before
    public void setUp() throws Exception {
        mLog = new MockLog();
        URL url = this.getClass().getClassLoader().getResource("data/mock_android.jar");

        mOsJarPath = new ArrayList<>();
        mOsJarPath.add(url.getFile());

        mTempFile = File.createTempFile("mock", ".jar");
        mOsDestJar = mTempFile.getAbsolutePath();
        mTempFile.deleteOnExit();
    }

    @After
    public void tearDown() {
        if (mTempFile != null) {
            //noinspection ResultOfMethodCallIgnored
            mTempFile.delete();
            mTempFile = null;
        }
    }

    @Test
    public void testClassRenaming() throws IOException {

        ICreateInfo ci = new CreateInfoAdapter() {
            @Override
            public String[] getRenamedClasses() {
                // classes to rename (so that we can replace them)
                return new String[] {
                        "mock_android.view.View", "mock_android.view._Original_View",
                        "not.an.actual.ClassName", "another.fake.NewClassName",
                };
            }
        };

        AsmGenerator agen = new AsmGenerator(mLog, ci);

        AsmAnalyzer aa = new AsmAnalyzer(mLog, mOsJarPath,
                null,                 // derived from
                new String[] {        // include classes
                    "**"
                },
                new String[]{}  /* excluded classes */,
                new String[]{} /* include files */);
        agen.setAnalysisResult(aa.analyze());
        agen.generate();

        Set<String> notRenamed = agen.getClassesNotRenamed();
        assertArrayEquals(new String[] { "not/an/actual/ClassName" }, notRenamed.toArray());

    }

    @Test
    public void testJavaClassRefactoring() throws IOException {
        ICreateInfo ci = new CreateInfoAdapter() {
            @Override
            public Class<?>[] getInjectedClasses() {
                // classes to inject in the final JAR
                return new Class<?>[] {
                        com.android.tools.layoutlib.create.dataclass.JavaClass.class
                };
            }

            @Override
            public String[] getJavaPkgClasses() {
             // classes to refactor (so that we can replace them)
                return new String[] {
                        JAVA_CLASS_NAME, "com.android.tools.layoutlib.create.dataclass.JavaClass",
                };
            }

            @Override
            public String[] getExcludedClasses() {
                return new String[]{JAVA_CLASS_NAME};
            }
        };

        AsmGenerator agen = new AsmGenerator(mLog, ci);

        AsmAnalyzer aa = new AsmAnalyzer(mLog, mOsJarPath,
                null,                 // derived from
                new String[] {        // include classes
                    "**"
                },
                new String[]{},
                new String[] {        /* include files */
                    "mock_android/data/data*"
                });
        agen.setAnalysisResult(aa.analyze());
        Map<String, byte[]> output = agen.generate();
        RecordingClassVisitor cv = new RecordingClassVisitor();
        for (Map.Entry<String, byte[]> entry: output.entrySet()) {
            if (!entry.getKey().endsWith(".class")) continue;
            ClassReader cr = new ClassReader(entry.getValue());
            cr.accept(cv, 0);
        }
        assertTrue(cv.mVisitedClasses.contains(
                "com/android/tools/layoutlib/create/dataclass/JavaClass"));
        assertFalse(cv.mVisitedClasses.contains(
                JAVA_CLASS_NAME));
        assertArrayEquals(new String[] {"mock_android/data/dataFile"},
                findFileNames(output));
    }

    @Test
    public void testClassRefactoring() throws IOException {
        ICreateInfo ci = new CreateInfoAdapter() {
            @Override
            public Class<?>[] getInjectedClasses() {
                // classes to inject in the final JAR
                return new Class<?>[] {
                        com.android.tools.layoutlib.create.dataclass.JavaClass.class
                };
            }

            @Override
            public String[] getRefactoredClasses() {
                // classes to refactor (so that we can replace them)
                return new String[] {
                        "mock_android.view.View", "mock_android.view._Original_View",
                };
            }
        };

        AsmGenerator agen = new AsmGenerator(mLog, ci);

        AsmAnalyzer aa = new AsmAnalyzer(mLog, mOsJarPath,
                null,                 // derived from
                new String[] {        // include classes
                        "**"
                },
                new String[]{},
                new String[] {});
        agen.setAnalysisResult(aa.analyze());
        Map<String, byte[]> output = agen.generate();
        RecordingClassVisitor cv = new RecordingClassVisitor();
        for (byte[] classContent: output.values()) {
            ClassReader cr = new ClassReader(classContent);
            cr.accept(cv, 0);
        }
        assertTrue(cv.mVisitedClasses.contains(
                "mock_android/view/_Original_View"));
        assertFalse(cv.mVisitedClasses.contains(
                "mock_android/view/View"));
    }

    @Test
    public void testClassExclusion() throws IOException {
        ICreateInfo ci = new CreateInfoAdapter() {
            @Override
            public String[] getExcludedClasses() {
                return new String[] {
                        "mock_android.dummy2.*",
                        "mock_android.dummy.**",
                        "mock_android.util.NotNeeded",
                        JAVA_CLASS_NAME
                };
            }
        };

        AsmGenerator agen = new AsmGenerator(mLog, ci);
        AsmAnalyzer aa = new AsmAnalyzer(mLog, mOsJarPath,
                null,                 // derived from
                new String[] {        // include classes
                        "**"
                },
                ci.getExcludedClasses(),
                new String[] {        /* include files */
                        "mock_android/data/data*"
                });
        agen.setAnalysisResult(aa.analyze());
        Map<String, byte[]> output = agen.generate();
        // Everything in .dummy.** should be filtered
        // Only things is .dummy2.* should be filtered
        assertArrayEquals(new String[] {
                "mock_android.dummy2.keep.DoNotRemove",
                "mock_android.util.EmptyArray",
                "mock_android.view.View",
                "mock_android.view.ViewGroup",
                "mock_android.view.ViewGroup$LayoutParams",
                "mock_android.view.ViewGroup$MarginLayoutParams",
                "mock_android.widget.LinearLayout",
                "mock_android.widget.LinearLayout$LayoutParams",
                "mock_android.widget.TableLayout",
                "mock_android.widget.TableLayout$LayoutParams"},
                findClassNames(output)
        );
        assertArrayEquals(new String[] {"mock_android/data/dataFile"},
                findFileNames(output));
    }

    @Test
    public void testMethodInjection() throws IOException, ClassNotFoundException,
            IllegalAccessException, InstantiationException,
            NoSuchMethodException, InvocationTargetException {
        ICreateInfo ci = new CreateInfoAdapter() {
            @Override
            public Map<String, InjectMethodRunnable> getInjectedMethodsMap() {
                return Collections.singletonMap("mock_android.util.EmptyArray",
                        InjectMethodRunnables.CONTEXT_GET_FRAMEWORK_CLASS_LOADER);
            }
        };

        AsmGenerator agen = new AsmGenerator(mLog, ci);
        AsmAnalyzer aa = new AsmAnalyzer(mLog, mOsJarPath,
                null,                 // derived from
                new String[] {        // include classes
                        "**"
                },
                ci.getExcludedClasses(),
                new String[] {        /* include files */
                        "mock_android/data/data*"
                });
        agen.setAnalysisResult(aa.analyze());
        JarUtil.createJar(new FileOutputStream(mOsDestJar), agen.generate());

        final String modifiedClass = "mock_android.util.EmptyArray";
        final String modifiedClassPath = modifiedClass.replace('.', '/').concat(".class");
        ZipFile zipFile = new ZipFile(mOsDestJar);
        ZipEntry entry = zipFile.getEntry(modifiedClassPath);
        assertNotNull(entry);
        final byte[] bytes;
        try (InputStream inputStream = zipFile.getInputStream(entry)) {
            bytes = getByteArray(inputStream);
        }
        ClassLoader classLoader = new ClassLoader(getClass().getClassLoader()) {
            @Override
            protected Class<?> findClass(String name) throws ClassNotFoundException {
                if (name.equals(modifiedClass)) {
                    return defineClass(null, bytes, 0, bytes.length);
                }
                throw new ClassNotFoundException(name + " not found.");
            }
        };
        Class<?> emptyArrayClass = classLoader.loadClass(modifiedClass);
        Object emptyArrayInstance = emptyArrayClass.newInstance();
        Method method = emptyArrayClass.getMethod("getFrameworkClassLoader");
        Object cl = method.invoke(emptyArrayInstance);
        assertEquals(classLoader, cl);
    }

    private static byte[] getByteArray(InputStream stream) throws IOException {
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        byte[] buffer = new byte[1024];
        int read;
        while ((read = stream.read(buffer, 0, buffer.length)) > -1) {
            bos.write(buffer, 0, read);
        }
        return bos.toByteArray();
    }


    private static String[] findClassNames(Map<String, byte[]> content) {
        return content.entrySet().stream()
                .filter(entry -> entry.getKey().endsWith(".class"))
                .map(entry -> classReaderToClassName(new ClassReader(entry.getValue())))
                .sorted()
                .toArray(String[]::new);
    }

    private static String[] findFileNames(Map<String, byte[]> content) {
        return content.keySet().stream()
                .filter(entry -> !entry.endsWith(".class"))
                .sorted()
                .toArray(String[]::new);
    }

    private static String classReaderToClassName(ClassReader classReader) {
        if (classReader == null) {
            return null;
        } else {
            return classReader.getClassName().replace('/', '.');
        }
    }

    /**
     * {@link ClassVisitor} that records every class that sees.
     */
    private static class RecordingClassVisitor extends ClassVisitor {
        private Set<String> mVisitedClasses = new HashSet<>();

        private RecordingClassVisitor() {
            super(Main.ASM_VERSION);
        }

        private void addClass(String className) {
            if (className == null) {
                return;
            }

            int pos = className.indexOf('$');
            if (pos > 0) {
                // For inner classes, add also the base class
                mVisitedClasses.add(className.substring(0, pos));
            }
            mVisitedClasses.add(className);
        }

        @Override
        public void visit(int version, int access, String name, String signature, String superName,
                String[] interfaces) {
            addClass(superName);
            Arrays.stream(interfaces).forEach(this::addClass);
        }

        private void processType(Type type) {
            switch (type.getSort()) {
                case Type.OBJECT:
                    addClass(type.getInternalName());
                    break;
                case Type.ARRAY:
                    addClass(type.getElementType().getInternalName());
                    break;
                case Type.METHOD:
                    processType(type.getReturnType());
                    Arrays.stream(type.getArgumentTypes()).forEach(this::processType);
                    break;
            }
        }

        @Override
        public FieldVisitor visitField(int access, String name, String desc, String signature,
                Object value) {
            processType(Type.getType(desc));
            return super.visitField(access, name, desc, signature, value);
        }

        @Override
        public MethodVisitor visitMethod(int access, String name, String desc, String signature,
                String[] exceptions) {
            MethodVisitor mv = super.visitMethod(access, name, desc, signature, exceptions);
            return new MethodVisitor(Main.ASM_VERSION, mv) {

                @Override
                public void visitFieldInsn(int opcode, String owner, String name, String desc) {
                    addClass(owner);
                    processType(Type.getType(desc));
                    super.visitFieldInsn(opcode, owner, name, desc);
                }

                @Override
                public void visitLdcInsn(Object cst) {
                    if (cst instanceof Type) {
                        processType((Type) cst);
                    }
                    super.visitLdcInsn(cst);
                }

                @Override
                public void visitTypeInsn(int opcode, String type) {
                    addClass(type);
                    super.visitTypeInsn(opcode, type);
                }

                @Override
                public void visitMethodInsn(int opcode, String owner, String name, String desc,
                        boolean itf) {
                    addClass(owner);
                    processType(Type.getType(desc));
                    super.visitMethodInsn(opcode, owner, name, desc, itf);
                }

            };
        }
    }
}
