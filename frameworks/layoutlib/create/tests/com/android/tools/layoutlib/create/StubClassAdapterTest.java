/*
 * Copyright (C) 2018 The Android Open Source Project
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


import com.android.tools.layoutlib.create.dataclass.StubClass;

import org.junit.Test;

import java.io.IOException;

import com.google.common.io.ByteStreams;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

public class StubClassAdapterTest {
    private static final String STUB_CLASS_NAME = StubClass.class.getName();

    @Test
    public void testStubbedClass()
            throws ClassNotFoundException, IOException, IllegalAccessException,
            InstantiationException {
        // Always rename the class to avoid conflict with the original class.
        byte[] classContent = ByteStreams.toByteArray(ClassLoader.getSystemResourceAsStream(
                STUB_CLASS_NAME.replace('.', '/') + ".class"));

        String newClassName = STUB_CLASS_NAME + '_';
        classContent =
                StubClassAdapter.stubClass(new Log(), classContent, newClassName.replace('.', '/'));
        TestClassLoader myClassLoader = new TestClassLoader(newClassName, classContent);
        Class<?> aClass = myClassLoader.loadClass(newClassName);
        assertTrue("StubClass not loaded by the classloader. Likely a bug in the test.",
                myClassLoader.wasClassLoaded(newClassName));
        try {
            aClass.newInstance();
            fail("Method should throw a RuntimeException");
        } catch (RuntimeException e) {
            assertEquals("Stub!", e.getMessage());
        }
    }
}
