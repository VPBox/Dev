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

import java.util.HashMap;
import java.util.List;
import java.util.Map;

class TestCompilationResult {
    private static class ByteArrayClassLoader extends ClassLoader {
        private Map<String, byte[]> codeMap;

        ByteArrayClassLoader(List<TestHelper.InMemoryJavaClassObject> objects) {
            super();
            codeMap = new HashMap<>();
            for (TestHelper.InMemoryJavaClassObject object : objects) {
                codeMap.put(object.getClassName(), object.getBytes());
            }
        }

        @Override
        protected Class findClass(String name) throws ClassNotFoundException {
            byte[] code = codeMap.get(name);
            return defineClass(name, code, 0, code.length);
        }
    }

    private Map<String, Class<?>> classes;

    TestCompilationResult(List<TestHelper.InMemoryJavaClassObject> objects)
            throws ClassNotFoundException {
        ByteArrayClassLoader loader = new ByteArrayClassLoader(objects);
        classes = new HashMap<>();

        for (TestHelper.InMemoryJavaClassObject object : objects) {
            Class<?> cls = loader.loadClass(object.getClassName());
            classes.put(object.getClassName(), cls);
        }
    }

    Class<?> loadClass(String name) {
        return classes.get(TestHelper.packageName + "." + name);
    }
}
