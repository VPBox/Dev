/*
 * Copyright (C) 2015 The Android Open Source Project
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

package com.android.server.wifi;

import androidx.test.filters.SmallTest;

import java.lang.annotation.Annotation;
import java.lang.reflect.Method;

/**
 * Generial Utilities for Wifi tests
 */
@SmallTest
public class WifiTestUtil {

    /**
     * Walk up the stack and find the first method annotated with @Test
     * Note: this will evaluate all overloads with the method name for the @Test annotation
     */
    public static String getTestMethod() {
        StackTraceElement[] stack = Thread.currentThread().getStackTrace();
        for (StackTraceElement e : stack) {
            if (e.isNativeMethod()) {
                continue;
            }
            Class clazz;
            try {
                clazz = Class.forName(e.getClassName());
            } catch (ClassNotFoundException ex) {
                throw new RuntimeException("Could not find class from stack", ex);
            }
            Method[] methods = clazz.getDeclaredMethods();
            for (Method method : methods) {
                if (method.getName().equals(e.getMethodName())) {
                    Annotation[] annotations = method.getDeclaredAnnotations();
                    for (Annotation annotation : annotations) {
                        if (annotation.annotationType().equals(org.junit.Test.class)) {
                            return e.getClassName() + "#" + e.getMethodName();
                        }
                    }
                }
            }
        }
        throw new RuntimeException("Could not find a test method in the stack");
    }
}
