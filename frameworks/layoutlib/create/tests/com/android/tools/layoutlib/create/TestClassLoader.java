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

import java.util.HashSet;
import java.util.Map;

import com.google.common.collect.ImmutableMap;

class TestClassLoader extends ClassLoader {
    private final Map<String, byte[]> mClassDefinitions;
    private final HashSet<String> mLoadedClasses = new HashSet<>();

    private TestClassLoader(Map<String, byte[]> classDefinitions) {
        mClassDefinitions = classDefinitions;
    }

    TestClassLoader(String name, byte[] bytes) {
        this(ImmutableMap.of(name, bytes));
    }

    public boolean wasClassLoaded(String name) {
        return mLoadedClasses.contains(name);
    }

    @Override
    protected Class<?> findClass(String name) throws ClassNotFoundException {
        byte[] classContent = mClassDefinitions.get(name);
        if (classContent != null) {
            mLoadedClasses.add(name);
            return defineClass(name, classContent, 0, classContent.length);
        }
        return super.findClass(name);
    }
}
