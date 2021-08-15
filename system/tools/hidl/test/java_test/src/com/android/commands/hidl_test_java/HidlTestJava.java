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

package com.android.commands.hidl_test_java;

import android.hidl.manager.V1_0.IServiceManager;
import android.hardware.tests.baz.V1_0.IBase;
import android.hardware.tests.baz.V1_0.IBaz;
import android.hardware.tests.baz.V1_0.IQuux;
import android.hardware.tests.baz.V1_0.IBaz.MyHandle;
import android.hardware.tests.baz.V1_0.IBaz.NestedStruct;
import android.hardware.tests.baz.V1_0.IBazCallback;
import android.hardware.tests.safeunion.V1_0.IOtherInterface;
import android.hardware.tests.safeunion.V1_0.ISafeUnion;
import android.hardware.tests.safeunion.V1_0.ISafeUnion.HandleTypeSafeUnion;
import android.hardware.tests.safeunion.V1_0.ISafeUnion.InterfaceTypeSafeUnion;
import android.hardware.tests.safeunion.V1_0.ISafeUnion.LargeSafeUnion;
import android.hardware.tests.safeunion.V1_0.ISafeUnion.SmallSafeUnion;
import android.os.HwBinder;
import android.os.NativeHandle;
import android.os.RemoteException;
import android.os.HidlSupport;
import android.util.Log;

import java.io.File;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.NoSuchElementException;
import java.util.Objects;

public final class HidlTestJava {
    private static final String TAG = "HidlTestJava";

    public static void main(String[] args) {
        int exitCode = 1;
        try {
            exitCode = new HidlTestJava().run(args);
        } catch (Exception e) {
            e.printStackTrace();
            Log.e(TAG, "Error ", e);
        }
        System.exit(exitCode);
    }

    public int run(String[] args) throws RemoteException, IOException {
        if (args[0].equals("-c")) {
            client();
        } else if (args[0].equals("-s")) {
            server();
        } else {
            Log.e(TAG, "Usage: HidlTestJava  -c(lient) | -s(erver)");
            System.err.printf("Usage: HidlTestJava  -c(lient) | -s(erver)\n");
            return 1;
        }

        return 0;
    }

    final class HidlDeathRecipient implements HwBinder.DeathRecipient {
        final Object mLock = new Object();
        boolean mCalled = false;
        long mCookie = 0;

        @Override
        public void serviceDied(long cookie) {
            synchronized (mLock) {
                mCalled = true;
                mCookie = cookie;
                mLock.notify();
            }
        }

        public boolean cookieMatches(long cookie) {
            synchronized (mLock) {
                return mCookie == cookie;
            }
        }

        public boolean waitUntilServiceDied(long timeoutMillis) {
            synchronized(mLock) {
                while (!mCalled) {
                    try {
                        mLock.wait(timeoutMillis);
                    } catch (InterruptedException e) {
                        continue; // Spin for another loop
                    }
                    break; // got notified or timeout hit
                }
                return mCalled;
            }
        }
    };

    private void ExpectTrue(boolean x) {
        if (x) {
            return;
        }

        throw new RuntimeException();
    }

    private void ExpectFalse(boolean x) {
        ExpectTrue(!x);
    }

    private void Expect(String result, String s) {
        if (result.equals(s)) {
            return;
        }

        System.err.printf("Expected '%s', got '%s'\n", s, result);
        Log.e(TAG, "Expected '" + s + "', got '" + result + "'");
        throw new RuntimeException();
    }

    // .equals and HidlSupport.interfacesEqual should have the same behavior.
    private void ExpectEqual(android.hidl.base.V1_0.IBase l, android.hidl.base.V1_0.IBase r) {
        ExpectTrue(Objects.equals(l, r));
        ExpectTrue(Objects.equals(r, l));
        ExpectTrue(HidlSupport.interfacesEqual(l, r));
        ExpectTrue(HidlSupport.interfacesEqual(r, l));
    }
    private void ExpectNotEqual(android.hidl.base.V1_0.IBase l, android.hidl.base.V1_0.IBase r) {
        ExpectFalse(Objects.equals(l, r));
        ExpectFalse(Objects.equals(r, l));
        ExpectFalse(HidlSupport.interfacesEqual(l, r));
        ExpectFalse(HidlSupport.interfacesEqual(r, l));
    }

    class BazCallback extends IBazCallback.Stub {
        private boolean mCalled;

        public BazCallback() {
            mCalled = false;
        }

        boolean wasCalled() {
            return mCalled;
        }

        public void heyItsMe(IBazCallback cb) throws RemoteException {
            mCalled = true;

            cb.heyItsMe(null);
        }

        public void hey() {
            mCalled = true;
        }

        @Override public boolean equals(Object other) {
            return other != null && other.getClass() == BazCallback.class &&
                ((BazCallback) other).mCalled == mCalled;
        }
        @Override public int hashCode() { return mCalled ? 1 : 0; }
    }

    private String numberToEnglish(int x) {
        final String[] kDigits = {
            "zero",
            "one",
            "two",
            "three",
            "four",
            "five",
            "six",
            "seven",
            "eight",
            "nine",
        };

        if (x < 0) {
            return "negative " + numberToEnglish(-x);
        }

        if (x < 10) {
            return kDigits[x];
        }

        if (x <= 15) {
            final String[] kSpecialTens = {
                "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen",
            };

            return kSpecialTens[x - 10];
        }

        if (x < 20) {
            return kDigits[x % 10] + "teen";
        }

        if (x < 100) {
            final String[] kDecades = {
                "twenty", "thirty", "forty", "fifty", "sixty", "seventy",
                "eighty", "ninety",
            };

            return kDecades[x / 10 - 2] + kDigits[x % 10];
        }

        return "positively huge!";
    }

    private void ExpectDeepEq(Object l, Object r) {
        ExpectTrue(HidlSupport.deepEquals(l, r));
        ExpectTrue(HidlSupport.deepHashCode(l) == HidlSupport.deepHashCode(r));
    }

    private void ExpectDeepNe(Object l, Object r) {
        ExpectTrue(!HidlSupport.deepEquals(l, r));
    }

    private void runClientSafeUnionTests() throws RemoteException, IOException {
        ISafeUnion safeunionInterface = ISafeUnion.getService();

        {
            // SafeUnionNoInitTest
            LargeSafeUnion safeUnion = safeunionInterface.newLargeSafeUnion();
            ExpectTrue(safeUnion.getDiscriminator() == LargeSafeUnion.hidl_discriminator.noinit);
        }
        {
            // SafeUnionSimpleTest
            LargeSafeUnion safeUnion = safeunionInterface.newLargeSafeUnion();

            safeUnion = safeunionInterface.setA(safeUnion, (byte) -5);
            ExpectTrue(safeUnion.getDiscriminator() == LargeSafeUnion.hidl_discriminator.a);
            ExpectTrue(safeUnion.a() == (byte) -5);

            safeUnion = safeunionInterface.setD(safeUnion, Long.MAX_VALUE);
            ExpectTrue(safeUnion.getDiscriminator() == LargeSafeUnion.hidl_discriminator.d);
            ExpectTrue(safeUnion.d() == Long.MAX_VALUE);
        }
        {
            // SafeUnionArrayLikeTypesTest
            long[] testArray = new long[] {1, -2, 3, -4, 5};
            ArrayList<Long> testVector = new ArrayList<Long>(Arrays.asList(Long.MAX_VALUE));

            LargeSafeUnion safeUnion = safeunionInterface.newLargeSafeUnion();
            safeUnion = safeunionInterface.setF(safeUnion, testArray);
            ExpectTrue(safeUnion.getDiscriminator() == LargeSafeUnion.hidl_discriminator.f);
            ExpectDeepEq(testArray, safeUnion.f());

            safeUnion = safeunionInterface.newLargeSafeUnion();
            safeUnion = safeunionInterface.setI(safeUnion, testVector);
            ExpectTrue(safeUnion.getDiscriminator() == LargeSafeUnion.hidl_discriminator.i);
            ExpectDeepEq(testVector, safeUnion.i());
        }
        {
            // SafeUnionStringTypeTest
            String testString = "This is an inordinately long test string.";

            LargeSafeUnion safeUnion = safeunionInterface.newLargeSafeUnion();
            safeUnion = safeunionInterface.setG(safeUnion, testString);
            ExpectTrue(safeUnion.getDiscriminator() == LargeSafeUnion.hidl_discriminator.g);
            ExpectDeepEq(testString, safeUnion.g());
        }
        {
            // SafeUnionNestedTest
            SmallSafeUnion smallSafeUnion = new SmallSafeUnion();
            smallSafeUnion.a((byte) 1);

            LargeSafeUnion safeUnion = safeunionInterface.newLargeSafeUnion();
            safeUnion = safeunionInterface.setL(safeUnion, smallSafeUnion);
            ExpectTrue(safeUnion.getDiscriminator() == LargeSafeUnion.hidl_discriminator.l);
            ExpectTrue(safeUnion.l().getDiscriminator() == SmallSafeUnion.hidl_discriminator.a);
            ExpectTrue(safeUnion.l().a() == (byte) 1);
        }
        {
            // SafeUnionEnumTest
            LargeSafeUnion safeUnion = safeunionInterface.newLargeSafeUnion();
            safeUnion = safeunionInterface.setM(safeUnion, ISafeUnion.BitField.V1);
            ExpectTrue(safeUnion.getDiscriminator() == LargeSafeUnion.hidl_discriminator.m);
            ExpectTrue(safeUnion.m() == ISafeUnion.BitField.V1);
        }
        {
            // SafeUnionBitFieldTest
            LargeSafeUnion safeUnion = safeunionInterface.newLargeSafeUnion();
            safeUnion = safeunionInterface.setN(safeUnion, ISafeUnion.BitField.V1);
            ExpectTrue(safeUnion.getDiscriminator() == LargeSafeUnion.hidl_discriminator.n);
            ExpectTrue(safeUnion.n() == ISafeUnion.BitField.V1);
        }
        {
            // SafeUnionInterfaceNullNativeHandleTest
            InterfaceTypeSafeUnion safeUnion = new InterfaceTypeSafeUnion();

            safeUnion = safeunionInterface.setInterfaceF(safeUnion, null);
            ExpectTrue(safeUnion.getDiscriminator() == InterfaceTypeSafeUnion.hidl_discriminator.f);
            ExpectTrue(safeUnion.f() == null);
        }
        {
            // SafeUnionInterfaceTest
            byte[] testArray = new byte[] {-1, -2, -3, 0, 1, 2, 3};
            ArrayList<String> testVector = new ArrayList(Arrays.asList("So", "Many", "Words"));
            String testStringA = "Hello";
            String testStringB = "World";

            IOtherInterface otherInterface = IOtherInterface.getService();

            ArrayList<NativeHandle> testHandlesVector = new ArrayList<>();
            for (int i = 0; i < 128; i++) {
                testHandlesVector.add(new NativeHandle());
            }

            InterfaceTypeSafeUnion safeUnion = safeunionInterface.newInterfaceTypeSafeUnion();
            safeUnion = safeunionInterface.setInterfaceB(safeUnion, testArray);
            ExpectTrue(safeUnion.getDiscriminator() == InterfaceTypeSafeUnion.hidl_discriminator.b);
            ExpectDeepEq(testArray, safeUnion.b());

            safeUnion.c(otherInterface);
            ExpectTrue(safeUnion.getDiscriminator() == InterfaceTypeSafeUnion.hidl_discriminator.c);
            ExpectTrue(HidlSupport.interfacesEqual(otherInterface, safeUnion.c()));
            String result = safeUnion.c().concatTwoStrings(testStringA, testStringB);
            Expect(result, testStringA + testStringB);

            safeUnion = safeunionInterface.setInterfaceD(safeUnion, testStringA);
            ExpectTrue(safeUnion.getDiscriminator() == InterfaceTypeSafeUnion.hidl_discriminator.d);
            Expect(testStringA, safeUnion.d());

            safeUnion = safeunionInterface.setInterfaceE(safeUnion, testVector);
            ExpectTrue(safeUnion.getDiscriminator() == InterfaceTypeSafeUnion.hidl_discriminator.e);
            ExpectDeepEq(testVector, safeUnion.e());

            safeUnion = safeunionInterface.setInterfaceG(safeUnion, testHandlesVector);
            ExpectTrue(safeUnion.getDiscriminator() == InterfaceTypeSafeUnion.hidl_discriminator.g);
            ExpectTrue(safeUnion.g().size() == testHandlesVector.size());

            for (int i = 0; i < testHandlesVector.size(); i++) {
                ExpectFalse(safeUnion.g().get(i).hasSingleFileDescriptor());
            }
        }
        {
            // SafeUnionNullNativeHandleTest
            HandleTypeSafeUnion safeUnion = new HandleTypeSafeUnion();

            safeUnion = safeunionInterface.setHandleA(safeUnion, null);
            ExpectTrue(safeUnion.getDiscriminator() == HandleTypeSafeUnion.hidl_discriminator.a);
            ExpectTrue(safeUnion.a() == null);
        }
        {
            // SafeUnionDefaultNativeHandleTest
            NativeHandle[] testHandlesArray = new NativeHandle[5];
            for (int i = 0; i < testHandlesArray.length; i++) {
                testHandlesArray[i] = new NativeHandle();
            }

            ArrayList<NativeHandle> testHandlesList = new ArrayList<NativeHandle>(
                Arrays.asList(testHandlesArray));

            HandleTypeSafeUnion safeUnion = safeunionInterface.newHandleTypeSafeUnion();
            safeUnion = safeunionInterface.setHandleA(safeUnion, new NativeHandle());
            ExpectTrue(safeUnion.getDiscriminator() == HandleTypeSafeUnion.hidl_discriminator.a);
            ExpectFalse(safeUnion.a().hasSingleFileDescriptor());

            safeUnion = safeunionInterface.setHandleB(safeUnion, testHandlesArray);
            ExpectTrue(safeUnion.getDiscriminator() == HandleTypeSafeUnion.hidl_discriminator.b);
            ExpectTrue(safeUnion.b().length == testHandlesArray.length);

            for (int i = 0; i < testHandlesArray.length; i++) {
                ExpectFalse(safeUnion.b()[i].hasSingleFileDescriptor());
            }

            safeUnion = safeunionInterface.setHandleC(safeUnion, testHandlesList);
            ExpectTrue(safeUnion.getDiscriminator() == HandleTypeSafeUnion.hidl_discriminator.c);
            ExpectTrue(safeUnion.c().size() == testHandlesList.size());

            for (int i = 0; i < testHandlesList.size(); i++) {
                ExpectFalse(safeUnion.c().get(i).hasSingleFileDescriptor());
            }
        }
        {
            // SafeUnionNativeHandleWithFdTest
            final String testFileName = "/data/local/tmp/SafeUnionNativeHandleWithFdTest";
            final String[] testStrings = {"This ", "is ", "so ", "much ", "data!\n"};
            File file = new File(testFileName);

            if (file.exists()) { ExpectTrue(file.delete()); }
            ExpectTrue(file.createNewFile());

            StringBuilder builder = new StringBuilder();
            for (String testString : testStrings) {
                builder.append(testString);
            }
            final String goldenResult = builder.toString();

            ArrayList<NativeHandle> testHandlesList = new ArrayList<NativeHandle>();
            FileOutputStream fos = new FileOutputStream(file);
            for (int i = 0; i < testStrings.length; i++) {
                testHandlesList.add(new NativeHandle(fos.getFD(), false /*own*/));
            }

            HandleTypeSafeUnion safeUnion = safeunionInterface.newHandleTypeSafeUnion();
            safeUnion = safeunionInterface.setHandleC(safeUnion, testHandlesList);
            for (int i = 0; i < safeUnion.c().size(); i++) {
                ExpectTrue(safeUnion.c().get(i).hasSingleFileDescriptor());

                // If you want to copy it out of the binder buffer or save it, it needs to be duped.
                // This isn't necessary for the test since it is kept open for the binder window.
                NativeHandle handle = safeUnion.c().get(i);
                if (i%2 == 0) handle = handle.dup();

                // Original fd is duped if not dup'd above
                FileDescriptor resultFd = handle.getFileDescriptor();
                ExpectTrue(resultFd.getInt$() != fos.getFD().getInt$());

                FileOutputStream otherFos = new FileOutputStream(resultFd);
                otherFos.write(testStrings[i].getBytes());
                otherFos.flush();

                otherFos.close();

                if (i%2 == 0) handle.close();
            }

            byte[] resultData = new byte[(int) file.length()];
            FileInputStream fis = new FileInputStream(file);
            fis.read(resultData);

            String result = new String(resultData);
            Expect(result, goldenResult);

            fis.close();
            fos.close();
            ExpectTrue(file.delete());
        }
        {
            // SafeUnionEqualityTest
            LargeSafeUnion one = safeunionInterface.newLargeSafeUnion();
            LargeSafeUnion two = safeunionInterface.newLargeSafeUnion();
            ExpectTrue(one.equals(two));

            one = safeunionInterface.setA(one, (byte) 1);
            ExpectFalse(one.equals(two));

            two = safeunionInterface.setB(two, (byte) 1);
            ExpectFalse(one.equals(two));

            two = safeunionInterface.setA(two, (byte) 2);
            ExpectFalse(one.equals(two));

            two = safeunionInterface.setA(two, (byte) 1);
            ExpectTrue(one.equals(two));
        }
        {
            // SafeUnionDeepEqualityTest
            ArrayList<Long> testVectorA = new ArrayList(Arrays.asList(1L, 2L, 3L));
            ArrayList<Long> testVectorB = new ArrayList(Arrays.asList(2L, 1L, 3L));

            LargeSafeUnion one = safeunionInterface.newLargeSafeUnion();
            LargeSafeUnion two = safeunionInterface.newLargeSafeUnion();

            one = safeunionInterface.setI(one, testVectorA);
            two = safeunionInterface.setI(two, testVectorB);
            ExpectFalse(one.equals(two));

            two = safeunionInterface.setI(two, (ArrayList<Long>) testVectorA.clone());
            ExpectTrue(one.equals(two));
        }
        {
            // SafeUnionHashCodeTest
            ArrayList<Boolean> testVector =
                new ArrayList(Arrays.asList(true, false, false, true, true));

            LargeSafeUnion one = safeunionInterface.newLargeSafeUnion();
            LargeSafeUnion two = safeunionInterface.newLargeSafeUnion();

            one = safeunionInterface.setH(one, testVector);
            two = safeunionInterface.setA(two, (byte) -5);
            ExpectFalse(one.hashCode() == two.hashCode());

            two = safeunionInterface.setH(two, (ArrayList<Boolean>) testVector.clone());
            ExpectTrue(one.hashCode() == two.hashCode());
        }
    }

    private void client() throws RemoteException, IOException {

        ExpectDeepEq(null, null);
        ExpectDeepNe(null, new String());
        ExpectDeepNe(new String(), null);
        ExpectDeepEq(new String(), new String());
        ExpectDeepEq("hey", "hey");

        ExpectDeepEq(new int[]{1,2}, new int[]{1,2});
        ExpectDeepNe(new int[]{1,2}, new int[]{1,3});
        ExpectDeepNe(new int[]{1,2}, new int[]{1,2,3});
        ExpectDeepEq(new int[][]{{1,2},{3,4}}, new int[][]{{1,2},{3,4}});
        ExpectDeepNe(new int[][]{{1,2},{3,4}}, new int[][]{{1,2},{3,5}});
        ExpectDeepNe(new int[][]{{1,2},{3,4}}, new int[][]{{1,2,3},{4,5,6}});
        ExpectDeepNe(new int[][]{{1,2},{3,4}}, new int[][]{{1,2},{3,4,5}});

        ExpectDeepEq(new Integer[]{1,2}, new Integer[]{1,2});
        ExpectDeepNe(new Integer[]{1,2}, new Integer[]{1,3});
        ExpectDeepNe(new Integer[]{1,2}, new Integer[]{1,2,3});
        ExpectDeepEq(new Integer[][]{{1,2},{3,4}}, new Integer[][]{{1,2},{3,4}});
        ExpectDeepNe(new Integer[][]{{1,2},{3,4}}, new Integer[][]{{1,2},{3,5}});
        ExpectDeepNe(new Integer[][]{{1,2},{3,4}}, new Integer[][]{{1,2,3},{4,5,6}});
        ExpectDeepNe(new Integer[][]{{1,2},{3,4}}, new Integer[][]{{1,2},{3,4,5}});

        ExpectDeepEq(new ArrayList(Arrays.asList(1, 2)),
                     new ArrayList(Arrays.asList(1, 2)));
        ExpectDeepNe(new ArrayList(Arrays.asList(1, 2)),
                     new ArrayList(Arrays.asList(1, 2, 3)));

        ExpectDeepEq(new ArrayList(Arrays.asList(new int[]{1,2}, new int[]{3,4})),
                     new ArrayList(Arrays.asList(new int[]{1,2}, new int[]{3,4})));
        ExpectDeepNe(new ArrayList(Arrays.asList(new int[]{1,2}, new int[]{3,4})),
                     new ArrayList(Arrays.asList(new int[]{1,2}, new int[]{3,5})));

        ExpectDeepEq(new ArrayList(Arrays.asList(new Integer[]{1,2}, new Integer[]{3,4})),
                     new ArrayList(Arrays.asList(new Integer[]{1,2}, new Integer[]{3,4})));
        ExpectDeepNe(new ArrayList(Arrays.asList(new Integer[]{1,2}, new Integer[]{3,4})),
                     new ArrayList(Arrays.asList(new Integer[]{1,2}, new Integer[]{3,5})));

        ExpectDeepEq(new ArrayList[]{new ArrayList(Arrays.asList(1,2)),
                                     new ArrayList(Arrays.asList(3,4))},
                     new ArrayList[]{new ArrayList(Arrays.asList(1,2)),
                                     new ArrayList(Arrays.asList(3,4))});

        {
            // Test proper exceptions are thrown
            try {
                IBase proxy = IBase.getService("this-doesn't-exist");
                // this should never run
                ExpectTrue(false);
            } catch (Exception e) {
                ExpectTrue(e instanceof NoSuchElementException);
            }
        }

        {
            // Test access through base interface binder.
            IBase baseProxy = IBase.getService();
            baseProxy.someBaseMethod();

            IBaz bazProxy = IBaz.castFrom(baseProxy);
            ExpectTrue(bazProxy != null);

            // IQuux is completely unrelated to IBase/IBaz, so the following
            // should fail, i.e. return null.
            IQuux quuxProxy = IQuux.castFrom(baseProxy);
            ExpectTrue(quuxProxy == null);
        }

        {
            // Test waiting API
            IBase baseProxyA = IBaz.getService(true /* retry */);
            ExpectTrue(baseProxyA != null);
            IBase baseProxyB = IBaz.getService(false /* retry */);
            ExpectTrue(baseProxyB != null);
        }

        IBaz proxy = IBaz.getService();

        proxy.ping();

        proxy.someBaseMethod();

        {
            Expect(proxy.interfaceDescriptor(), IBaz.kInterfaceName);
        }

        {
            IBase.Foo foo = new IBase.Foo();
            foo.x = 1;

            for (int i = 0; i < 5; ++i) {
                IBase.Foo.Bar bar = new IBase.Foo.Bar();
                bar.z = 1.0f + (float)i * 0.01f;
                bar.s = "Hello, world " + i;
                foo.aaa.add(bar);
            }

            foo.y.z = 3.14f;
            foo.y.s = "Lorem ipsum...";

            IBase.Foo result = proxy.someOtherBaseMethod(foo);
            ExpectTrue(result.equals(foo));
        }

        {
            IBase.Foo[] inputArray = new IBase.Foo[2];

            IBase.Foo foo = new IBase.Foo();
            foo.x = 1;

            for (int i = 0; i < 5; ++i) {
                IBase.Foo.Bar bar = new IBase.Foo.Bar();
                bar.z = 1.0f + (float)i * 0.01f;
                bar.s = "Hello, world " + i;
                foo.aaa.add(bar);
            }

            foo.y.z = 3.14f;
            foo.y.s = "Lorem ipsum...";

            inputArray[0] = foo;

            foo = new IBase.Foo();
            foo.x = 2;

            for (int i = 0; i < 3; ++i) {
                IBase.Foo.Bar bar = new IBase.Foo.Bar();
                bar.z = 2.0f - (float)i * 0.01f;
                bar.s = "Lorem ipsum " + i;
                foo.aaa.add(bar);
            }

            foo.y.z = 1.1414f;
            foo.y.s = "Et tu brute?";

            inputArray[1] = foo;

            IBase.Foo[] expectedOutputArray = new IBase.Foo[2];
            expectedOutputArray[0] = inputArray[1];
            expectedOutputArray[1] = inputArray[0];

            IBase.Foo[] outputArray = proxy.someMethodWithFooArrays(inputArray);

            ExpectTrue(java.util.Objects.deepEquals(outputArray, expectedOutputArray));
        }

        {
            ArrayList<IBase.Foo> inputVec = new ArrayList<IBase.Foo>();

            IBase.Foo foo = new IBase.Foo();
            foo.x = 1;

            for (int i = 0; i < 5; ++i) {
                IBase.Foo.Bar bar = new IBase.Foo.Bar();
                bar.z = 1.0f + (float)i * 0.01f;
                bar.s = "Hello, world " + i;
                foo.aaa.add(bar);
            }

            foo.y.z = 3.14f;
            foo.y.s = "Lorem ipsum...";

            inputVec.add(foo);

            foo = new IBase.Foo();
            foo.x = 2;

            for (int i = 0; i < 3; ++i) {
                IBase.Foo.Bar bar = new IBase.Foo.Bar();
                bar.z = 2.0f - (float)i * 0.01f;
                bar.s = "Lorem ipsum " + i;
                foo.aaa.add(bar);
            }

            foo.y.z = 1.1414f;
            foo.y.s = "Et tu brute?";

            inputVec.add(foo);

            ArrayList<IBase.Foo> expectedOutputVec = new ArrayList<IBase.Foo>();
            expectedOutputVec.add(inputVec.get(1));
            expectedOutputVec.add(inputVec.get(0));

            ArrayList<IBase.Foo> outputVec =
                proxy.someMethodWithFooVectors(inputVec);

            ExpectTrue(java.util.Objects.deepEquals(outputVec, expectedOutputVec));
        }

        {
            IBase.VectorOfArray in = new IBase.VectorOfArray();

            int k = 0;
            for (int i = 0; i < 3; ++i) {
                byte[] mac = new byte[6];
                for (int j = 0; j < 6; ++j, ++k) {
                    mac[j] = (byte)k;
                }

                in.addresses.add(mac);
            }

            IBase.VectorOfArray expectedOut = new IBase.VectorOfArray();
            int n = in.addresses.size();

            for (int i = 0; i < n; ++i) {
                expectedOut.addresses.add(in.addresses.get(n - 1 - i));
            }

            IBase.VectorOfArray out = proxy.someMethodWithVectorOfArray(in);
            ExpectTrue(out.equals(expectedOut));
        }

        {
            ArrayList<byte[]> in = new ArrayList<byte[]>();

            int k = 0;
            for (int i = 0; i < 3; ++i) {
                byte[] mac = new byte[6];
                for (int j = 0; j < 6; ++j, ++k) {
                    mac[j] = (byte)k;
                }

                in.add(mac);
            }

            ArrayList<byte[]> expectedOut = new ArrayList<byte[]>();

            int n = in.size();
            for (int i = 0; i < n; ++i) {
                expectedOut.add(in.get(n - 1 - i));
            }

            ArrayList<byte[]> out = proxy.someMethodTakingAVectorOfArray(in);

            ExpectTrue(out.size() == expectedOut.size());
            for  (int i = 0; i < n; ++i) {
                ExpectTrue(java.util.Objects.deepEquals(out.get(i), expectedOut.get(i)));
            }
        }

        {
            IBase.StringMatrix5x3 in = new IBase.StringMatrix5x3();
            IBase.StringMatrix3x5 expectedOut = new IBase.StringMatrix3x5();

            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < 3; ++j) {
                    in.s[i][j] = numberToEnglish(3 * i + j + 1);
                    expectedOut.s[j][i] = in.s[i][j];
                }
            }

            IBase.StringMatrix3x5 out = proxy.transpose(in);

            // [[1 2 3] [4 5 6] [7 8 9] [10 11 12] [13 14 15]]^T
            // = [[1 4 7 10 13] [2 5 8 11 14] [3 6 9 12 15]]
            ExpectTrue(out.equals(expectedOut));
        }

        {
            String[][] in = new String[5][3];
            String[][] expectedOut = new String[3][5];
            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < 3; ++j) {
                    in[i][j] = numberToEnglish(3 * i + j + 1);
                    expectedOut[j][i] = in[i][j];
                }
            }

            String[][] out = proxy.transpose2(in);

            // [[1 2 3] [4 5 6] [7 8 9] [10 11 12] [13 14 15]]^T
            // = [[1 4 7 10 13] [2 5 8 11 14] [3 6 9 12 15]]
            ExpectTrue(java.util.Arrays.deepEquals(out, expectedOut));
        }

        ExpectTrue(proxy.someBoolMethod(true) == false);

        {
            boolean[] someBoolArray = new boolean[3];
            someBoolArray[0] = true;
            someBoolArray[1] = false;
            someBoolArray[2] = true;

            boolean[] resultArray = proxy.someBoolArrayMethod(someBoolArray);
            ExpectTrue(resultArray[0] == false);
            ExpectTrue(resultArray[1] == true);
            ExpectTrue(resultArray[2] == false);

            ArrayList<Boolean> someBoolVec = new ArrayList<Boolean>();
            someBoolVec.add(true);
            someBoolVec.add(false);
            someBoolVec.add(true);

            ArrayList<Boolean> resultVec = proxy.someBoolVectorMethod(someBoolVec);
            ExpectTrue(resultVec.get(0) == false);
            ExpectTrue(resultVec.get(1) == true);
            ExpectTrue(resultVec.get(2) == false);
        }

        proxy.doThis(1.0f);

        ExpectTrue(proxy.doThatAndReturnSomething(1) == 666);
        ExpectTrue(proxy.doQuiteABit(1, 2L, 3.0f, 4.0) == 666.5);

        {
            int[] paramArray = new int[15];
            int[] expectedOutArray = new int[32];
            ArrayList<Integer> paramVec = new ArrayList<Integer>();
            ArrayList<Integer> expectedOutVec = new ArrayList<Integer>();

            for (int i = 0; i < paramArray.length; ++i) {
                paramArray[i] = i;
                paramVec.add(i);

                expectedOutArray[i] = 2 * i;
                expectedOutArray[15 + i] = i;

                expectedOutVec.add(2 * i);
            }

            expectedOutArray[30] = 1;
            expectedOutArray[31] = 2;


            int[] outArray = proxy.doSomethingElse(paramArray);
            ExpectTrue(java.util.Objects.deepEquals(outArray, expectedOutArray));

            ArrayList<Integer> outVec = proxy.mapThisVector(paramVec);
            java.util.Objects.equals(outVec, expectedOutVec);

        }

        Expect(proxy.doStuffAndReturnAString(), "Hello, world!");

        BazCallback cb = new BazCallback();
        ExpectTrue(!cb.wasCalled());
        proxy.callMe(cb);
        ExpectTrue(cb.wasCalled());

        ExpectTrue(proxy.useAnEnum(IBaz.SomeEnum.goober) == -64);

        {
            String[] stringArray = new String[3];
            stringArray[0] = "one";
            stringArray[1] = "two";
            stringArray[2] = "three";

            String[] expectedOutArray = new String[2];
            expectedOutArray[0] = "Hello";
            expectedOutArray[1] = "World";

            String[] outArray = proxy.haveSomeStrings(stringArray);
            ExpectTrue(java.util.Arrays.deepEquals(outArray, expectedOutArray));

            ArrayList<String> stringVec = new ArrayList<String>();
            stringVec.add("one");
            stringVec.add("two");
            stringVec.add("three");

            ArrayList<String> expectedOutVec = new ArrayList<String>();
            expectedOutVec.add("Hello");
            expectedOutVec.add("World");

            ExpectTrue(expectedOutVec.equals(proxy.haveAStringVec(stringVec)));
        }

        proxy.returnABunchOfStrings(
                new IBaz.returnABunchOfStringsCallback() {
                    @Override
                    public void onValues(String a, String b, String c) {
                        Expect(a, "Eins");
                        Expect(b, "Zwei");
                        Expect(c, "Drei");
                    }
                });

        proxy.returnABunchOfStrings((a,b,c) -> Expect(a + b + c, "EinsZweiDrei"));

        proxy.callMeLater(new BazCallback());
        System.gc();
        proxy.iAmFreeNow();

        {
            IBaz.T t1 = new IBaz.T();
            IBaz.T t2 = new IBaz.T();
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 3; j++) {
                    t1.matrix5x3[i][j] = t2.matrix5x3[i][j] = (i + 1) * (j + 1);
                }
            }
            ExpectTrue(t1.equals(t2));
            ExpectTrue(t1.hashCode() == t2.hashCode());
            t2.matrix5x3[4][2] = -60;
            ExpectTrue(!t1.equals(t2));
        }

        ArrayList<NestedStruct> structs = proxy.getNestedStructs();
        ExpectTrue(structs.size() == 5);
        ExpectTrue(structs.get(1).matrices.size() == 6);

        {
            IBaz.Everything e = new IBaz.Everything();
            Expect(e.toString(),
                "{.number = 0, .anotherNumber = 0, .s = , " +
                ".vs = [], .multidimArray = [[null, null], [null, null]], " +
                ".sArray = [null, null, null], .anotherStruct = {.first = , .last = }, .bf = }");
            e.s = "string!";
            e.number = 127;
            e.anotherNumber = 100;
            e.vs.addAll(Arrays.asList("One", "Two", "Three"));
            for (int i = 0; i < e.multidimArray.length; i++)
                for (int j = 0; j < e.multidimArray[i].length; j++)
                    e.multidimArray[i][j] = Integer.toString(i) + Integer.toString(j);
            e.bf = IBaz.BitField.VALL;
            e.anotherStruct.first = "James";
            e.anotherStruct.last = "Bond";
            Expect(e.toString(),
                "{.number = 127, .anotherNumber = 100, .s = string!, " +
                ".vs = [One, Two, Three], .multidimArray = [[00, 01], [10, 11]], " +
                ".sArray = [null, null, null], .anotherStruct = {.first = James, .last = Bond}, " +
                ".bf = V0 | V1 | V2 | V3 | VALL}");
            Expect(IBaz.BitField.toString(IBaz.BitField.VALL), "VALL");
            Expect(IBaz.BitField.toString((byte)(IBaz.BitField.V0 | IBaz.BitField.V2)), "0x5");
            Expect(IBaz.BitField.dumpBitfield(IBaz.BitField.VALL), "V0 | V1 | V2 | V3 | VALL");
            Expect(IBaz.BitField.dumpBitfield((byte)(IBaz.BitField.V1 | IBaz.BitField.V3 | 0xF0)),
                "V1 | V3 | 0xf0");

            Expect(proxy.toString(), IBaz.kInterfaceName + "@Proxy");
        }

        {
            // Ensure that native parcel is cleared even if the corresponding
            // Java object isn't GC'd.
            ArrayList<Integer> data4K = new ArrayList<>(1024);
            for (int i = 0; i < 1024; i++) {
                data4K.add(i);
            }

            for (int i = 0; i < 1024; i++) {
                // If they are not properly cleaned up, these calls will put 4MB of data in
                // kernel binder buffer, and will fail.
                try {
                    proxy.mapThisVector(data4K);
                } catch (RemoteException ex) {
                    throw new RuntimeException("Failed at call #" + Integer.toString(i), ex);
                }
            }
        }

        {
            // TestArrays
            IBase.LotsOfPrimitiveArrays in = new IBase.LotsOfPrimitiveArrays();

            for (int i = 0; i < 128; ++i) {
                in.byte1[i] = (byte)i;
                in.boolean1[i] = (i & 4) != 0;
                in.double1[i] = i;
            }

            int m = 0;
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 128; ++j, ++m) {
                    in.byte2[i][j] = (byte)m;
                    in.boolean2[i][j] = (m & 4) != 0;
                    in.double2[i][j] = m;
                }
            }

            m = 0;
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 16; ++j) {
                    for (int k = 0; k < 128; ++k, ++m) {
                        in.byte3[i][j][k] = (byte)m;
                        in.boolean3[i][j][k] = (m & 4) != 0;
                        in.double3[i][j][k] = m;
                    }
                }
            }

            IBase.LotsOfPrimitiveArrays out = proxy.testArrays(in);
            ExpectTrue(in.equals(out));
        }

        {
            // testByteVecs

            ArrayList<byte[]> in = new ArrayList<byte[]>();

            int k = 0;
            for (int i = 0; i < 8; ++i) {
                byte[] elem = new byte[128];
                for (int j = 0; j < 128; ++j, ++k) {
                    elem[j] = (byte)k;
                }
                in.add(elem);
            }

            ArrayList<byte[]> out = proxy.testByteVecs(in);

            ExpectDeepEq(in, out);
        }

        {
            // testByteVecs w/ mismatched element lengths.

            ArrayList<byte[]> in = new ArrayList<byte[]>();

            int k = 0;
            for (int i = 0; i < 8; ++i) {
                byte[] elem = new byte[128 - i];
                for (int j = 0; j < (128 - i); ++j, ++k) {
                    elem[j] = (byte)k;
                }
                in.add(elem);
            }

            boolean failedAsItShould = false;

            try {
                ArrayList<byte[]> out = proxy.testByteVecs(in);
            }
            catch (IllegalArgumentException e) {
                failedAsItShould = true;
            }

            ExpectTrue(failedAsItShould);
        }

        {
            // testBooleanVecs

            ArrayList<boolean[]> in = new ArrayList<boolean[]>();

            int k = 0;
            for (int i = 0; i < 8; ++i) {
                boolean[] elem = new boolean[128];
                for (int j = 0; j < 128; ++j, ++k) {
                    elem[j] = (k & 4) != 0;
                }
                in.add(elem);
            }

            ArrayList<boolean[]> out = proxy.testBooleanVecs(in);
            ExpectDeepEq(in, out);
        }

        {
            // testDoubleVecs

            ArrayList<double[]> in = new ArrayList<double[]>();

            int k = 0;
            for (int i = 0; i < 8; ++i) {
                double[] elem = new double[128];
                for (int j = 0; j < 128; ++j, ++k) {
                    elem[j] = k;
                }
                in.add(elem);
            }

            ArrayList<double[]> out = proxy.testDoubleVecs(in);
            ExpectDeepEq(in, out);
        }
        {
            // testProxyEquals
            // TODO(b/68727931): test passthrough services as well.

            IBase proxy1 = IBase.getService();
            IBase proxy2 = IBase.getService();
            IBaz proxy3 = IBaz.getService();
            IBazCallback callback1 = new BazCallback();
            IBazCallback callback2 = new BazCallback();
            IServiceManager manager = IServiceManager.getService();

            // test hwbinder proxies
            ExpectEqual(proxy1, proxy2); // same proxy class
            ExpectEqual(proxy1, proxy3); // different proxy class

            // negative tests
            ExpectNotEqual(proxy1, null);
            ExpectNotEqual(proxy1, callback1); // proxy != stub
            ExpectNotEqual(proxy1, manager);

            // HidlSupport.interfacesEqual use overridden .equals for stubs
            ExpectEqual(callback1, callback1);
            ExpectEqual(callback1, callback2);
            callback1.hey();
            ExpectNotEqual(callback1, callback2);
            callback2.hey();
            ExpectEqual(callback1, callback2);

            // test hash for proxies
            java.util.HashSet<IBase> set = new java.util.HashSet<>();
            set.add(proxy1);
            ExpectTrue(set.contains(proxy1)); // hash is stable
            ExpectTrue(set.contains(proxy2));
            ExpectFalse(set.contains(manager));
        }
        {
            IBaz baz = IBaz.getService();
            ExpectTrue(baz != null);
            IBaz.StructWithInterface swi = new IBaz.StructWithInterface();
            swi.dummy = baz;
            swi.number = 12345678;
            IBaz.StructWithInterface swi_back = baz.haveSomeStructWithInterface(swi);
            ExpectTrue(swi_back != null);
            ExpectTrue(swi_back.dummy != null);
            ExpectTrue(HidlSupport.interfacesEqual(baz, swi_back.dummy));
            ExpectTrue(swi_back.number == 12345678);
        }

        runClientSafeUnionTests();

        // --- DEATH RECIPIENT TESTING ---
        // This must always be done last, since it will kill the native server process
        HidlDeathRecipient recipient1 = new HidlDeathRecipient();
        HidlDeathRecipient recipient2 = new HidlDeathRecipient();

        final int cookie1 = 0x1481;
        final int cookie2 = 0x1482;
        final int cookie3 = 0x1483;
        ExpectTrue(proxy.linkToDeath(recipient1, cookie1));

        ExpectTrue(proxy.linkToDeath(recipient1, cookie3));
        ExpectTrue(proxy.unlinkToDeath(recipient1));

        ExpectTrue(proxy.linkToDeath(recipient2, cookie2));
        ExpectTrue(proxy.unlinkToDeath(recipient2));
        try {
            proxy.dieNow();
        } catch (RemoteException e) {
            // Expected
        }
        ExpectTrue(recipient1.waitUntilServiceDied(2000 /*timeoutMillis*/));
        ExpectTrue(!recipient2.waitUntilServiceDied(2000 /*timeoutMillis*/));
        ExpectTrue(recipient1.cookieMatches(cookie1));
        Log.d(TAG, "OK, exiting");

    }

    class Baz extends IBaz.Stub {
        // from IBase
        public void someBaseMethod() {
            Log.d(TAG, "Baz someBaseMethod");
        }

        public IBase.Foo someOtherBaseMethod(IBase.Foo foo) {
            Log.d(TAG, "Baz someOtherBaseMethod " + foo.toString());
            return foo;
        }

        public IBase.Foo[] someMethodWithFooArrays(IBase.Foo[] fooInput) {
            Log.d(TAG, "Baz someMethodWithFooArrays " + fooInput.toString());

            IBase.Foo[] fooOutput = new IBase.Foo[2];
            fooOutput[0] = fooInput[1];
            fooOutput[1] = fooInput[0];

            return fooOutput;
        }

        public ArrayList<IBase.Foo> someMethodWithFooVectors(
                ArrayList<IBase.Foo> fooInput) {
            Log.d(TAG, "Baz someMethodWithFooVectors " + fooInput.toString());

            ArrayList<IBase.Foo> fooOutput = new ArrayList<IBase.Foo>();
            fooOutput.add(fooInput.get(1));
            fooOutput.add(fooInput.get(0));

            return fooOutput;
        }

        public IBase.VectorOfArray someMethodWithVectorOfArray(
                IBase.VectorOfArray in) {
            Log.d(TAG, "Baz someMethodWithVectorOfArray " + in.toString());

            IBase.VectorOfArray out = new IBase.VectorOfArray();
            int n = in.addresses.size();
            for (int i = 0; i < n; ++i) {
                out.addresses.add(in.addresses.get(n - i - 1));
            }

            return out;
        }

        public ArrayList<byte[/* 6 */]> someMethodTakingAVectorOfArray(
                ArrayList<byte[/* 6 */]> in) {
            Log.d(TAG, "Baz someMethodTakingAVectorOfArray");

            int n = in.size();
            ArrayList<byte[]> out = new ArrayList<byte[]>();
            for (int i = 0; i < n; ++i) {
                out.add(in.get(n - i - 1));
            }

            return out;
        }

        public IBase.StringMatrix3x5 transpose(IBase.StringMatrix5x3 in) {
            Log.d(TAG, "Baz transpose " + in.toString());

            IBase.StringMatrix3x5 out = new IBase.StringMatrix3x5();
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 5; ++j) {
                    out.s[i][j] = in.s[j][i];
                }
            }

            return out;
        }

        public String[][] transpose2(String[][] in) {
            Log.d(TAG, "Baz transpose2 " + in.toString());

            String[][] out = new String[3][5];
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 5; ++j) {
                    out[i][j] = in[j][i];
                }
            }

            return out;
        }

        public boolean someBoolMethod(boolean x) {
            Log.d(TAG, "Baz someBoolMethod(" + x + ")");

            return !x;
        }

        public boolean[] someBoolArrayMethod(boolean[] x) {
            Log.d(TAG, "Baz someBoolArrayMethod("
                    + x.toString() + ")");

            boolean[] out = new boolean[4];
            out[0] = !x[0];
            out[1] = !x[1];
            out[2] = !x[2];
            out[3] = true;

            return out;
        }

        public ArrayList<Boolean> someBoolVectorMethod(ArrayList<Boolean> x) {
            Log.d(TAG, "Baz someBoolVectorMethod(" + x.toString() + ")");

            ArrayList<Boolean> out = new ArrayList<Boolean>();
            for (int i = 0; i < x.size(); ++i) {
                out.add(!x.get(i));
            }

            return out;
        }

        public void doThis(float param) {
            Log.d(TAG, "Baz doThis " + param);
        }

        public int doThatAndReturnSomething(long param) {
            Log.d(TAG, "Baz doThatAndReturnSomething " + param);
            return 666;
        }

        public double doQuiteABit(int a, long b, float c, double d) {
            Log.d(TAG, "Baz doQuiteABit " + a + ", " + b + ", " + c + ", " + d);
            return 666.5;
        }

        public int[] doSomethingElse(int[] param) {
            Log.d(TAG, "Baz doSomethingElse " + param.toString());

            int[] something = new int[32];
            for (int i = 0; i < 15; ++i) {
                something[i] = 2 * param[i];
                something[15 + i] = param[i];
            }
            something[30] = 1;
            something[31] = 2;

            return something;
        }

        public String doStuffAndReturnAString() {
            Log.d(TAG, "doStuffAndReturnAString");
            return "Hello, world!";
        }

        public ArrayList<Integer> mapThisVector(ArrayList<Integer> param) {
            Log.d(TAG, "mapThisVector " + param.toString());

            ArrayList<Integer> out = new ArrayList<Integer>();

            for (int i = 0; i < param.size(); ++i) {
                out.add(2 * param.get(i));
            }

            return out;
        }

        public void takeAMask(byte bf, byte first, IBase.MyMask second, byte third,
                takeAMaskCallback cb) {
            cb.onValues(bf, (byte)(bf | first),
                    (byte)(second.value & bf), (byte)((bf | bf) & third));
        }

        public LotsOfPrimitiveArrays testArrays(LotsOfPrimitiveArrays in) {
            return in;
        }

        public ArrayList<byte[]> testByteVecs(ArrayList<byte[]> in) {
            return in;
        }

        public ArrayList<boolean[]> testBooleanVecs(ArrayList<boolean[]> in) {
            return in;
        }

        public ArrayList<double[]> testDoubleVecs(ArrayList<double[]> in) {
            return in;
        }

        public byte returnABitField() {
            return 0;
        }

        public int size(int size) {
            return size;
        }

        @Override
        public ArrayList<NestedStruct> getNestedStructs() throws RemoteException {
            return new ArrayList<>();
        }

        class BazCallback extends IBazCallback.Stub {
            public void heyItsMe(IBazCallback cb) {
                Log.d(TAG, "SERVER: heyItsMe");
            }

            public void hey() {
                Log.d(TAG, "SERVER: hey");
            }
        }

        public void callMe(IBazCallback cb) throws RemoteException {
            Log.d(TAG, "callMe");
            cb.heyItsMe(new BazCallback());
        }

        private IBazCallback mStoredCallback;
        public void callMeLater(IBazCallback cb) {
            mStoredCallback = cb;
        }

        public void iAmFreeNow() throws RemoteException {
            if (mStoredCallback != null) {
                mStoredCallback.hey();
            }
        }

        public void dieNow() {
            // Not tested in Java
        }

        public byte useAnEnum(byte zzz) {
            Log.d(TAG, "useAnEnum " + zzz);
            return SomeEnum.quux;
        }

        public String[] haveSomeStrings(String[] array) {
            Log.d(TAG, "haveSomeStrings ["
                        + "\"" + array[0] + "\", "
                        + "\"" + array[1] + "\", "
                        + "\"" + array[2] + "\"]");

            String[] result = new String[2];
            result[0] = "Hello";
            result[1] = "World";

            return result;
        }

        public ArrayList<String> haveAStringVec(ArrayList<String> vector) {
            Log.d(TAG, "haveAStringVec ["
                        + "\"" + vector.get(0) + "\", "
                        + "\"" + vector.get(1) + "\", "
                        + "\"" + vector.get(2) + "\"]");

            ArrayList<String> result = new ArrayList<String>();
            result.add("Hello");
            result.add("World");

            return result;
        }

        public void returnABunchOfStrings(returnABunchOfStringsCallback cb) {
            cb.onValues("Eins", "Zwei", "Drei");
        }

        public StructWithInterface haveSomeStructWithInterface(StructWithInterface swi) {
            return swi;
        }
    }

    class SafeUnion extends ISafeUnion.Stub {
        @Override
        public LargeSafeUnion newLargeSafeUnion() {
            Log.d(TAG, "SERVER: newLargeSafeUnion");
            return new LargeSafeUnion();
        }

        @Override
        public LargeSafeUnion setA(LargeSafeUnion safeUnion, byte a) {
            Log.d(TAG, "SERVER: setA(" + a + ")");
            safeUnion.a(a);

            return safeUnion;
        }

        @Override
        public LargeSafeUnion setB(LargeSafeUnion safeUnion, short b) {
            Log.d(TAG, "SERVER: setB(" + b + ")");
            safeUnion.b(b);

            return safeUnion;
        }

        @Override
        public LargeSafeUnion setC(LargeSafeUnion safeUnion, int c) {
            Log.d(TAG, "SERVER: setC(" + c + ")");
            safeUnion.c(c);

            return safeUnion;
        }

        @Override
        public LargeSafeUnion setD(LargeSafeUnion safeUnion, long d) {
            Log.d(TAG, "SERVER: setD(" + d + ")");
            safeUnion.d(d);

            return safeUnion;
        }

        @Override
        public LargeSafeUnion setE(LargeSafeUnion safeUnion, byte[/* 13 */] e) {
            Log.d(TAG, "SERVER: setE(" + e + ")");
            safeUnion.e(e);

            return safeUnion;
        }

        @Override
        public LargeSafeUnion setF(LargeSafeUnion safeUnion, long[/* 5 */] f) {
            Log.d(TAG, "SERVER: setF(" + f + ")");
            safeUnion.f(f);

            return safeUnion;
        }

        @Override
        public LargeSafeUnion setG(LargeSafeUnion safeUnion, String g) {
            Log.d(TAG, "SERVER: setG(" + g + ")");
            safeUnion.g(g);

            return safeUnion;
        }

        @Override
        public LargeSafeUnion setH(LargeSafeUnion safeUnion, ArrayList<Boolean> h) {
            Log.d(TAG, "SERVER: setH(" + h + ")");
            safeUnion.h(h);

            return safeUnion;
        }

        @Override
        public LargeSafeUnion setI(LargeSafeUnion safeUnion, ArrayList<Long> i) {
            Log.d(TAG, "SERVER: setI(" + i + ")");
            safeUnion.i(i);

            return safeUnion;
        }

        @Override
        public LargeSafeUnion setJ(LargeSafeUnion safeUnion, ISafeUnion.J j) {
            Log.d(TAG, "SERVER: setJ(" + j + ")");
            safeUnion.j(j);

            return safeUnion;
        }

        @Override
        public LargeSafeUnion setK(LargeSafeUnion safeUnion, LargeSafeUnion.K k) {
            Log.d(TAG, "SERVER: setK(" + k + ")");
            safeUnion.k(k);

            return safeUnion;
        }

        @Override
        public LargeSafeUnion setL(LargeSafeUnion safeUnion, SmallSafeUnion l) {
            Log.d(TAG, "SERVER: setL(" + l + ")");
            safeUnion.l(l);

            return safeUnion;
        }

        @Override
        public LargeSafeUnion setM(LargeSafeUnion safeUnion, byte m) {
            Log.d(TAG, "SERVER: setM(" + m + ")");
            safeUnion.m(m);

            return safeUnion;
        }

        @Override
        public LargeSafeUnion setN(LargeSafeUnion safeUnion, byte n) {
            Log.d(TAG, "SERVER: setN(" + n + ")");
            safeUnion.n(n);

            return safeUnion;
        }

        @Override
        public InterfaceTypeSafeUnion newInterfaceTypeSafeUnion() {
            Log.d(TAG, "SERVER: newInterfaceTypeSafeUnion");
            return new InterfaceTypeSafeUnion();
        }

        @Override
        public InterfaceTypeSafeUnion setInterfaceA(InterfaceTypeSafeUnion safeUnion, int a) {
            Log.d(TAG, "SERVER: setInterfaceA(" + a + ")");
            safeUnion.a(a);

            return safeUnion;
        }

        @Override
        public InterfaceTypeSafeUnion setInterfaceB(
            InterfaceTypeSafeUnion safeUnion, byte[/* 7 */] b) {
            Log.d(TAG, "SERVER: setInterfaceB(" + b + ")");
            safeUnion.b(b);

            return safeUnion;
        }

        @Override
        public InterfaceTypeSafeUnion setInterfaceC(
            InterfaceTypeSafeUnion safeUnion, IOtherInterface c) {
            Log.d(TAG, "SERVER: setInterfaceC(" + c + ")");
            safeUnion.c(c);

            return safeUnion;
        }

        @Override
        public InterfaceTypeSafeUnion setInterfaceD(InterfaceTypeSafeUnion safeUnion, String d) {
            Log.d(TAG, "SERVER: setInterfaceD(" + d + ")");
            safeUnion.d(d);

            return safeUnion;
        }

        @Override
        public InterfaceTypeSafeUnion setInterfaceE(
            InterfaceTypeSafeUnion safeUnion, ArrayList<String> e) {
            Log.d(TAG, "SERVER: setInterfaceE(" + e + ")");
            safeUnion.e(e);

            return safeUnion;
        }

        @Override
        public InterfaceTypeSafeUnion setInterfaceF(
            InterfaceTypeSafeUnion safeUnion, NativeHandle f) {
            Log.d(TAG, "SERVER: setInterfaceF(" + f + ")");
            safeUnion.f(f);

            return safeUnion;
        }

        @Override
        public InterfaceTypeSafeUnion setInterfaceG(
            InterfaceTypeSafeUnion safeUnion, ArrayList<NativeHandle> g) {
            Log.d(TAG, "SERVER: setInterfaceG(" + g + ")");
            safeUnion.g(g);

            return safeUnion;
        }

        @Override
        public HandleTypeSafeUnion newHandleTypeSafeUnion() {
            Log.d(TAG, "SERVER: newHandleTypeSafeUnion");
            return new HandleTypeSafeUnion();
        }

        @Override
        public HandleTypeSafeUnion setHandleA(HandleTypeSafeUnion safeUnion, NativeHandle a) {
            Log.d(TAG, "SERVER: setHandleA(" + a + ")");
            safeUnion.a(a);

            return safeUnion;
        }

        @Override
        public HandleTypeSafeUnion setHandleB(HandleTypeSafeUnion safeUnion, NativeHandle[] b) {
            Log.d(TAG, "SERVER: setHandleB(" + b + ")");
            safeUnion.b(b);

            return safeUnion;
        }

        @Override
        public HandleTypeSafeUnion setHandleC(HandleTypeSafeUnion safeUnion,
                                              ArrayList<NativeHandle> c) {
            Log.d(TAG, "SERVER: setHandleC(" + c + ")");
            safeUnion.c(c);

            return safeUnion;
        }
    }

    class OtherInterface extends IOtherInterface.Stub {
        @Override
        public String concatTwoStrings(String a, String b) {
            return a.concat(b);
        }
    }

    private void server() throws RemoteException {
        HwBinder.configureRpcThreadpool(1, true);

        Baz baz = new Baz();
        baz.registerAsService("default");

        SafeUnion safeunionInterface = new SafeUnion();
        safeunionInterface.registerAsService("default");

        OtherInterface otherInterface = new OtherInterface();
        otherInterface.registerAsService("default");

        HwBinder.joinRpcThreadpool();
    }
}
