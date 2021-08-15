/*
 * Copyright (C) 2016 The Android Open Source Project
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

package com.android.internal.telephony;

import android.telephony.SmsCbMessage;
import android.util.Log;

import com.android.internal.telephony.cdma.SmsMessage;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import junit.framework.Assert;

/**
 * This class provides reflection for classes/methods that are not accessible from tests.
 * Convention for helper function naming is: classNameFunctionName()
 */
public class TelephonyTestUtils {
    private static final String TAG = "TelephonyTestUtils";

    /**
     * This function calls constructor that takes in params.
     * This function does not work if any of the parameter passed in is null because
     * params[i].getClass() fails. In that case it needs to be handled by the caller (it knows the
     * param types so getClass() is not needed)
     */
    public static Object createNewInstance(String className, Object... params) {
        try {
            Class clazz = Class.forName(className);
            int numParam = params.length;
            Class<?>[] paramType = new Class[numParam];
            for (int i = 0; i < numParam; i++) {
                paramType[i] = params[i].getClass();
            }
            Constructor constructor = clazz.getDeclaredConstructor(paramType);
            constructor.setAccessible(true);

            return constructor.newInstance(params);
        } catch (Exception e) {
            Assert.fail(e.toString());
            return null;
        }
    }

    /**
     * This function does not work if any of the parameter passed in is null because
     * params[i].getClass() fails. In that case it needs to be handled by the caller (it knows the
     * param types so getClass() is not needed)
     */
    public static Object invokeStaticMethod(Class<?> clazz, String method, Object... params) {
        try {
            int numParam = params.length;
            Class<?>[] paramType = new Class[numParam];
            for (int i = 0; i < numParam; i++) {
                paramType[i] = params[i].getClass();
            }
            Method methodReflection = clazz.getDeclaredMethod(method, paramType);
            methodReflection.setAccessible(true);
            return methodReflection.invoke(null, params);
        } catch (Exception e) {
            Assert.fail(e.toString());
            return null;
        }
    }

    /**
     * This is needed when the test expects the method in source being called to throw an exception.
     * Throwable will be an instanceof the expected exception.
     * This function does not work if any of the parameter passed in is null because
     * params[i].getClass() fails. In that case it needs to be handled by the caller (it knows the
     * param types so getClass() is not needed)
     */
    public static Object invokeStaticMethodThrowsException(Class<?> clazz, String method,
                                                           Object... params) throws Throwable {
        try {
            int numParam = params.length;
            Class<?>[] paramType = new Class[numParam];
            for (int i = 0; i < numParam; i++) {
                paramType[i] = params[i].getClass();
            }
            Method methodReflection = clazz.getDeclaredMethod(method, paramType);
            methodReflection.setAccessible(true);
            return methodReflection.invoke(null, params);
        } catch (InvocationTargetException e) {
            throw e.getTargetException();
        } catch (Exception e) {
            Assert.fail(e.toString());
            return null;
        }
    }

    /**
     * This function does not work if any of the parameter passed in is null because
     * params[i].getClass() fails. In that case it needs to be handled by the caller (it knows the
     * param types so getClass() is not needed)
     */
    public static Object invokeNonStaticMethod(Class<?> clazz, Object caller, String method,
                                               Object... params) {
        try {
            int numParam = params.length;
            Class<?>[] paramType = new Class[numParam];
            for (int i = 0; i < numParam; i++) {
                paramType[i] = params[i].getClass();
            }
            Method methodReflection = clazz.getDeclaredMethod(method, paramType);
            methodReflection.setAccessible(true);
            return methodReflection.invoke(caller, params);
        } catch (Exception e) {
            Assert.fail(e.toString());
            return null;
        }
    }

    /**
     * This is needed when the test expects the method in source being called to throw an exception.
     * Throwable will be an instanceof the expected exception.
     * This function does not work if any of the parameter passed in is null because
     * params[i].getClass() fails. In that case it needs to be handled by the caller (it knows the
     * param types so getClass() is not needed)
     */
    public static Object invokeNonStaticMethodThrowsException(Class<?> clazz, Object caller,
                                                              String method, Object... params)
            throws Throwable {
        try {
            int numParam = params.length;
            Class<?>[] paramType = new Class[numParam];
            for (int i = 0; i < numParam; i++) {
                paramType[i] = params[i].getClass();
            }
            Method methodReflection = clazz.getDeclaredMethod(method, paramType);
            methodReflection.setAccessible(true);
            return methodReflection.invoke(caller, params);
        } catch (InvocationTargetException e) {
            throw e.getTargetException();
        } catch (Exception e) {
            Assert.fail(e.toString());
            return null;
        }
    }

    public static void waitForMs(long ms) {
        try {
            Thread.sleep(ms);
        } catch (InterruptedException e) {
            logd("InterruptedException while waiting: " + e);
        }
    }

    private static void logd(String s) {
        Log.d(TAG, s);
    }
}
