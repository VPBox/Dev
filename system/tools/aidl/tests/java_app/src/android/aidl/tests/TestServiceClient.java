/*
 * Copyright (C) 2015, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package android.aidl.tests;

import android.aidl.tests.SimpleParcelable;
import android.aidl.tests.StructuredParcelable;
import android.aidl.tests.TestFailException;
import android.aidl.tests.TestLogger;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.ServiceSpecificException;
import android.os.Bundle;
import android.os.IBinder;
import android.os.ParcelFileDescriptor;
import android.os.PersistableBundle;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.util.Log;
import java.io.File;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.HashMap;

// Generated
import android.aidl.tests.INamedCallback;
import android.aidl.tests.ITestService;

public class TestServiceClient extends Activity {
    private static final String TAG = "TestServiceClient";

    private TestLogger mLog;
    private String mSuccessSentinel;
    private String mFailureSentinel;

    private void init() {
        Intent intent = getIntent();
        mLog = new TestLogger(this);
        mLog.log("Reading sentinels from intent...");
        mSuccessSentinel = intent.getStringExtra("sentinel.success");
        mFailureSentinel = intent.getStringExtra("sentinel.failure");
        if (mSuccessSentinel == null || mFailureSentinel == null) {
            String message = "Failed to read intent extra input.";
            Log.e(TAG, message);
            mLog.close();
            throw new RuntimeException(message);
        }
    }

    private ITestService getService() throws TestFailException {
        IBinder service = ServiceManager.getService(
                ITestService.class.getName());
        if (service == null) {
            mLog.logAndThrow("Failed to obtain binder...");
        }
        ITestService ret = ITestService.Stub.asInterface(service);
        if (ret == null) {
            mLog.logAndThrow("Failed to cast IBinder instance.");
        }
        return ret;
    }

    private void checkPrimitiveRepeat(ITestService service)
            throws TestFailException {
        mLog.log("Checking that service can repeat primitives back...");
        try {
            {
                boolean query = true;
                boolean response = service.RepeatBoolean(query);
                if (query != response) {
                    mLog.logAndThrow("Repeat with " + query +
                                     " responded " + response);
                }
            }
            {
                char query = 'A';
                char response = service.RepeatChar(query);
                if (query != response) {
                    mLog.logAndThrow("Repeat with " + query +
                                     " responded " + response);
                }
            }
            {
                byte query = -128;
                byte response = service.RepeatByte(query);
                if (query != response) {
                    mLog.logAndThrow("Repeat with " + query +
                                     " responded " + response);
                }
            }
            {
                int query = 1 << 30;
                int response = service.RepeatInt(query);
                if (query != response) {
                    mLog.logAndThrow("Repeat with " + query +
                                     " responded " + response);
                }
            }
            {
                int query[] = {ITestService.TEST_CONSTANT,
                               ITestService.TEST_CONSTANT2,
                               ITestService.TEST_CONSTANT3,
                               ITestService.TEST_CONSTANT4,
                               ITestService.TEST_CONSTANT5,
                               ITestService.TEST_CONSTANT6,
                               ITestService.TEST_CONSTANT7,
                               ITestService.TEST_CONSTANT8};
                for (int i = 0; i < query.length; i++) {
                    int response = service.RepeatInt(query[i]);
                    if (query[i] != response) {
                        mLog.logAndThrow("Repeat with " + query[i] +
                                " responded " + response);
                    }
                }
            }
            {
                long query = 1L << 60;
                long response = service.RepeatLong(query);
                if (query != response) {
                    mLog.logAndThrow("Repeat with " + query +
                                     " responded " + response);
                }
            }
            {
                float query = 1.0f/3.0f;
                float response = service.RepeatFloat(query);
                if (query != response) {
                    mLog.logAndThrow("Repeat with " + query +
                                     " responded " + response);
                }
            }
            {
                double query = 1.0/3.0;
                double response = service.RepeatDouble(query);
                if (query != response) {
                    mLog.logAndThrow("Repeat with " + query +
                                     " responded " + response);
                }
            }
            {
                Map<String, Object> query = new HashMap<String, Object>();
                query.put("first_val", new Byte((byte)-128));
                query.put("second_val", new Integer(1<<30));
                query.put("third_val", "OHAI");
                Object response = service.RepeatMap(query);
                if (!query.equals(response)) {
                    mLog.logAndThrow("Repeat with " + query +
                                     " responded " + response);
                }
            }

            List<String> queries = Arrays.asList(
                "not empty", "", "\0",
                ITestService.STRING_TEST_CONSTANT,
                ITestService.STRING_TEST_CONSTANT2);
            for (String query : queries) {
                String response = service.RepeatString(query);
                if (!query.equals(response)) {
                    mLog.logAndThrow("Repeat request with '" + query + "'" +
                                     " of length " + query.length() +
                                     " responded with '" + response + "'" +
                                     " of length " + response.length());
                }
            }
        } catch (RemoteException ex) {
            mLog.log(ex.toString());
            mLog.logAndThrow("Service failed to repeat a primitive back.");
        }
        mLog.log("...Basic primitive repeating works.");
    }

    private void checkArrayReversal(ITestService service)
            throws TestFailException {
        mLog.log("Checking that service can reverse and return arrays...");
        try {
            {
                boolean[] input = {true, false, false, false};
                boolean echoed[] = new boolean[input.length];
                boolean[] reversed = service.ReverseBoolean(input, echoed);
                if (!Arrays.equals(input, echoed)) {
                    mLog.logAndThrow("Failed to echo input array back.");
                }
                if (input.length != reversed.length) {
                    mLog.logAndThrow("Reversed array is the wrong size.");
                }
                for (int i = 0; i < input.length; ++i) {
                    int j = reversed.length - (1 + i);
                    if (input[i] != reversed[j]) {
                        mLog.logAndThrow(
                                "input[" + i + "] = " + input[i] +
                                " but reversed value = " + reversed[j]);
                    }
                }
            }
            {
                byte[] input = {0, 1, 2};
                byte echoed[] = new byte[input.length];
                byte[] reversed = service.ReverseByte(input, echoed);
                if (!Arrays.equals(input, echoed)) {
                    mLog.logAndThrow("Failed to echo input array back.");
                }
                if (input.length != reversed.length) {
                    mLog.logAndThrow("Reversed array is the wrong size.");
                }
                for (int i = 0; i < input.length; ++i) {
                    int j = reversed.length - (1 + i);
                    if (input[i] != reversed[j]) {
                        mLog.logAndThrow(
                                "input[" + i + "] = " + input[i] +
                                " but reversed value = " + reversed[j]);
                    }
                }
            }
            {
                char[] input = {'A', 'B', 'C', 'D', 'E'};
                char echoed[] = new char[input.length];
                char[] reversed = service.ReverseChar(input, echoed);
                if (!Arrays.equals(input, echoed)) {
                    mLog.logAndThrow("Failed to echo input array back.");
                }
                if (input.length != reversed.length) {
                    mLog.logAndThrow("Reversed array is the wrong size.");
                }
                for (int i = 0; i < input.length; ++i) {
                    int j = reversed.length - (1 + i);
                    if (input[i] != reversed[j]) {
                        mLog.logAndThrow(
                                "input[" + i + "] = " + input[i] +
                                " but reversed value = " + reversed[j]);
                    }
                }
            }
            {
                int[] input = {-1, 0, 1, 2, 3, 4, 5, 6};
                int echoed[] = new int[input.length];
                int[] reversed = service.ReverseInt(input, echoed);
                if (!Arrays.equals(input, echoed)) {
                    mLog.logAndThrow("Failed to echo input array back.");
                }
                if (input.length != reversed.length) {
                    mLog.logAndThrow("Reversed array is the wrong size.");
                }
                for (int i = 0; i < input.length; ++i) {
                    int j = reversed.length - (1 + i);
                    if (input[i] != reversed[j]) {
                        mLog.logAndThrow(
                                "input[" + i + "] = " + input[i] +
                                " but reversed value = " + reversed[j]);
                    }
                }
            }
            {
                long[] input = {-2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8};
                long echoed[] = new long[input.length];
                long[] reversed = service.ReverseLong(input, echoed);
                if (!Arrays.equals(input, echoed)) {
                    mLog.logAndThrow("Failed to echo input array back.");
                }
                if (input.length != reversed.length) {
                    mLog.logAndThrow("Reversed array is the wrong size.");
                }
                for (int i = 0; i < input.length; ++i) {
                    int j = reversed.length - (1 + i);
                    if (input[i] != reversed[j]) {
                        mLog.logAndThrow(
                                "input[" + i + "] = " + input[i] +
                                " but reversed value = " + reversed[j]);
                    }
                }
            }
            {
                float[] input = {0.0f, 1.0f, -0.3f};
                float echoed[] = new float[input.length];
                float[] reversed = service.ReverseFloat(input, echoed);
                if (!Arrays.equals(input, echoed)) {
                    mLog.logAndThrow("Failed to echo input array back.");
                }
                if (input.length != reversed.length) {
                    mLog.logAndThrow("Reversed array is the wrong size.");
                }
                for (int i = 0; i < input.length; ++i) {
                    int j = reversed.length - (1 + i);
                    if (input[i] != reversed[j]) {
                        mLog.logAndThrow(
                                "input[" + i + "] = " + input[i] +
                                " but reversed value = " + reversed[j]);
                    }
                }
            }
            {
                double[] input = {-1.0, -4.0, -2.0};
                double echoed[] = new double[input.length];
                double[] reversed = service.ReverseDouble(input, echoed);
                if (!Arrays.equals(input, echoed)) {
                    mLog.logAndThrow("Failed to echo input array back.");
                }
                if (input.length != reversed.length) {
                    mLog.logAndThrow("Reversed array is the wrong size.");
                }
                for (int i = 0; i < input.length; ++i) {
                    int j = reversed.length - (1 + i);
                    if (input[i] != reversed[j]) {
                        mLog.logAndThrow(
                                "input[" + i + "] = " + input[i] +
                                " but reversed value = " + reversed[j]);
                    }
                }
            }
            {
                String[] input = {"For", "relaxing", "times"};
                String echoed[] = new String[input.length];
                String[] reversed = service.ReverseString(input, echoed);
                if (!Arrays.equals(input, echoed)) {
                    mLog.logAndThrow("Failed to echo input array back.");
                }
                if (input.length != reversed.length) {
                    mLog.logAndThrow("Reversed array is the wrong size.");
                }
                for (int i = 0; i < input.length; ++i) {
                    int j = reversed.length - (1 + i);
                    if (!input[i].equals(reversed[j])) {
                        mLog.logAndThrow(
                                "input[" + i + "] = " + input[i] +
                                " but reversed value = " + reversed[j]);
                    }
                }
            }
        } catch (RemoteException ex) {
            mLog.log(ex.toString());
            mLog.logAndThrow("Service failed to reverse an array.");
        }
        mLog.log("...service can reverse and return arrays.");
    }

    private void checkBinderExchange(
                ITestService service) throws TestFailException {
      mLog.log("Checking exchange of binders...");
      try {
          INamedCallback got = service.GetOtherTestService("Smythe");
          mLog.log("Received test service");
          String name = got.GetName();

          if (!name.equals("Smythe")) {
              mLog.logAndThrow("Tried to get service with name 'Smythe'" +
                               " and found service with name '" + name + "'");
          }

          if (!service.VerifyName(got, "Smythe")) {
              mLog.logAndThrow("Test service could not verify name of 'Smythe'");
          }
      } catch (RemoteException ex) {
          mLog.log(ex.toString());
          mLog.logAndThrow("Service failed to exchange binders.");
      }
      mLog.log("...Exchange of binders works");
    }

    private void checkListReversal(ITestService service)
            throws TestFailException {
        mLog.log("Checking that service can reverse and return lists...");
        try {
            {
                List<String> input = Arrays.asList("Walk", "into", "Córdoba");
                List<String> echoed = new ArrayList<String>();
                List<String> reversed = service.ReverseStringList(input, echoed);
                if (!input.equals(echoed)) {
                    mLog.logAndThrow("Failed to echo input List<String> back.");
                }
                Collections.reverse(input);
                if (!input.equals(reversed)) {
                    mLog.logAndThrow("Reversed list is not correct.");
                }
            }
        } catch (RemoteException ex) {
            mLog.log(ex.toString());
            mLog.logAndThrow("Service failed to reverse an List<String>.");
        }
        mLog.log("...service can reverse and return lists.");
    }

    private void checkSimpleParcelables(ITestService service)
            throws TestFailException {
        mLog.log("Checking that service can repeat and reverse SimpleParcelable objects...");
        try {
            {
                SimpleParcelable input = new SimpleParcelable("foo", 42);
                SimpleParcelable out_param = new SimpleParcelable();
                SimpleParcelable returned =
                        service.RepeatSimpleParcelable(input, out_param);
                if (!input.equals(out_param)) {
                    mLog.log(input.toString() + " != " + out_param.toString());
                    mLog.logAndThrow("out param SimpleParcelable was not equivalent");
                }
                if (!input.equals(returned)) {
                    mLog.log(input.toString() + " != " + returned.toString());
                    mLog.logAndThrow("returned SimpleParcelable was not equivalent");
                }
            }
            {
                SimpleParcelable[] input = new SimpleParcelable[3];
                input[0] = new SimpleParcelable("a", 1);
                input[1] = new SimpleParcelable("b", 2);
                input[2] = new SimpleParcelable("c", 3);
                SimpleParcelable[] repeated = new SimpleParcelable[3];
                SimpleParcelable[] reversed = service.ReverseSimpleParcelables(
                        input, repeated);
                if (!Arrays.equals(input, repeated)) {
                    mLog.logAndThrow(
                            "Repeated list of SimpleParcelable objects did not match.");
                }
                if (input.length != reversed.length) {
                    mLog.logAndThrow(
                            "Reversed list of SimpleParcelable objects had wrong length.");
                }
                for (int i = 0, k = input.length - 1;
                     i < input.length;
                     ++i, --k) {
                    if (!input[i].equals(reversed[k])) {
                        mLog.log(input[i].toString() + " != " +
                                 reversed[k].toString());
                        mLog.logAndThrow("reversed SimpleParcelable was not equivalent");
                    }
                }
            }
        } catch (Exception ex) {
            mLog.log(ex.toString());
            mLog.logAndThrow("Service failed to handle SimpleParcelable objects.");
        }
        mLog.log("...service can manipulate SimpleParcelable objects.");
    }

    private void checkPersistableBundles(ITestService service)
            throws TestFailException {
        mLog.log("Checking that service can repeat and reverse PersistableBundle objects...");
        try {
            {
                PersistableBundle emptyBundle = new PersistableBundle();
                PersistableBundle returned = service.RepeatPersistableBundle(emptyBundle);
                if (emptyBundle.size() != 0 || returned.size() != 0) {
                    mLog.log(emptyBundle.toString() + " != " + returned.toString());
                    mLog.logAndThrow("returned empty PersistableBundle object was not equivalent");
                }
                mLog.log("...service can repeat and reverse empty PersistableBundle objects...");
            }
            {
                final String testBoolKey = new String("testBool");
                final String testIntKey = new String("testInt");
                final String testNestedIntKey = new String("testNestedInt");
                final String testLongKey = new String("testLong");
                final String testDoubleKey = new String("testDouble");
                final String testStringKey = new String("testString");
                final String testBoolArrayKey = new String("testBoolArray");
                final String testIntArrayKey = new String("testIntArray");
                final String testLongArrayKey = new String("testLongArray");
                final String testDoubleArrayKey = new String("testDoubleArray");
                final String testStringArrayKey = new String("testStringArray");
                final String testPersistableBundleKey = new String("testPersistableBundle");
                PersistableBundle nonEmptyBundle = new PersistableBundle();
                nonEmptyBundle.putBoolean(testBoolKey, false);
                nonEmptyBundle.putInt(testIntKey, 33);
                nonEmptyBundle.putLong(testLongKey, 34359738368L);
                nonEmptyBundle.putDouble(testDoubleKey, 1.1);
                nonEmptyBundle.putString(testStringKey, new String("Woot!"));
                nonEmptyBundle.putBooleanArray(testBoolArrayKey, new boolean[] {true, false, true});
                nonEmptyBundle.putIntArray(testIntArrayKey, new int[] {33, 44, 55, 142});
                nonEmptyBundle.putLongArray(
                    testLongArrayKey, new long[] {34L, 8371L, 34359738375L});
                nonEmptyBundle.putDoubleArray(testDoubleArrayKey, new double[] {2.2, 5.4});
                nonEmptyBundle.putStringArray(testStringArrayKey, new String[] {"hello", "world!"});
                PersistableBundle testNestedPersistableBundle = new PersistableBundle();
                testNestedPersistableBundle.putInt(testNestedIntKey, 345);
                nonEmptyBundle.putPersistableBundle(
                    testPersistableBundleKey, testNestedPersistableBundle);
                PersistableBundle returned = service.RepeatPersistableBundle(nonEmptyBundle);
                if (returned.size() != nonEmptyBundle.size()
                    || returned.getBoolean(testBoolKey) != nonEmptyBundle.getBoolean(testBoolKey)
                    || returned.getInt(testIntKey) != nonEmptyBundle.getInt(testIntKey)
                    || returned.getLong(testLongKey) != nonEmptyBundle.getLong(testLongKey)
                    || returned.getDouble(testDoubleKey) != nonEmptyBundle.getDouble(testDoubleKey)
                    || !returned.getString(testStringKey)
                                .equals(nonEmptyBundle.getString(testStringKey))
                    || !Arrays.equals(nonEmptyBundle.getBooleanArray(testBoolArrayKey),
                                      returned.getBooleanArray(testBoolArrayKey))
                    || !Arrays.equals(nonEmptyBundle.getIntArray(testIntArrayKey),
                                      returned.getIntArray(testIntArrayKey))
                    || !Arrays.equals(nonEmptyBundle.getLongArray(testLongArrayKey),
                                      returned.getLongArray(testLongArrayKey))
                    || !Arrays.equals(nonEmptyBundle.getDoubleArray(testDoubleArrayKey),
                                      returned.getDoubleArray(testDoubleArrayKey))
                    || !Arrays.equals(nonEmptyBundle.getStringArray(testStringArrayKey),
                                      returned.getStringArray(testStringArrayKey))) {
                    PersistableBundle temp =
                        returned.getPersistableBundle(testPersistableBundleKey);
                    if (temp == null
                        || temp.getInt(testNestedIntKey)
                            != testNestedPersistableBundle.getInt(testNestedIntKey)) {
                        mLog.log(nonEmptyBundle.toString() + " != " + returned.toString());
                        mLog.logAndThrow("returned non-empty PersistableBundle " +
                                         "object was not equivalent");
                    }
                }
                mLog.log("...service can repeat and reverse non-empty " +
                         "PersistableBundle objects...");
            }
            {
                PersistableBundle[] input = new PersistableBundle[3];
                PersistableBundle first = new PersistableBundle();
                PersistableBundle second = new PersistableBundle();
                PersistableBundle third = new PersistableBundle();
                final String testIntKey = new String("testInt");
                final String testLongKey = new String("testLong");
                final String testDoubleKey = new String("testDouble");
                first.putInt(testIntKey, 1231);
                second.putLong(testLongKey, 222222L);
                third.putDouble(testDoubleKey, 10.8);
                input[0] = first;
                input[1] = second;
                input[2] = third;
                final int original_input_size = input.length;
                PersistableBundle[] repeated = new PersistableBundle[input.length];
                PersistableBundle[] reversed = service.ReversePersistableBundles(input, repeated);
                if (input.length != repeated.length || input.length != original_input_size) {
                    mLog.logAndThrow("Repeated list of PersistableBundle objects had " +
                                     "wrong length.");
                }
                if (input[0].getInt(testIntKey) != repeated[0].getInt(testIntKey)
                    || input[1].getLong(testLongKey) != repeated[1].getLong(testLongKey)
                    || input[2].getDouble(testDoubleKey) != repeated[2].getDouble(testDoubleKey)) {
                    mLog.logAndThrow("Repeated list of PersistableBundle objects did not match.");
                }
                if (input.length != reversed.length || input.length != original_input_size) {
                    mLog.logAndThrow("Reversed list of PersistableBundle objects had " +
                                     "wrong length.");
                }
                if (input[0].getInt(testIntKey) != reversed[2].getInt(testIntKey)
                    || input[1].getLong(testLongKey) != reversed[1].getLong(testLongKey)
                    || input[2].getDouble(testDoubleKey) != reversed[0].getDouble(testDoubleKey)) {
                    mLog.logAndThrow("reversed PersistableBundle objects were not equivalent");
                }
                mLog.log("...service can repeat and reverse arrays of " +
                         "non-empty PersistableBundle objects...");
            }
        } catch (Exception ex) {
            mLog.log(ex.toString());
            mLog.logAndThrow("Service failed to handle PersistableBundle objects.");
        }
        mLog.log("...service can manipulate PersistableBundle objects.");
    }

    private void checkFileDescriptorPassing(ITestService service)
            throws TestFailException {
        mLog.log("Checking that service can receive and return file descriptors...");
        try {
            FileOutputStream fileOutputStream =
                    openFileOutput("test-dummy", Context.MODE_PRIVATE);

            FileDescriptor descriptor = fileOutputStream.getFD();
            FileDescriptor journeyed = service.RepeatFileDescriptor(descriptor);
            fileOutputStream.close();

            FileOutputStream journeyedStream = new FileOutputStream(journeyed);

            String testData = "FrazzleSnazzleFlimFlamFlibbityGumboChops";
            byte[] output = testData.getBytes();
            journeyedStream.write(output);
            journeyedStream.close();

            FileInputStream fileInputStream = openFileInput("test-dummy");
            byte[] input = new byte[output.length];
            if (fileInputStream.read(input) != input.length) {
                mLog.logAndThrow("Read short count from file");
            }

            if (!Arrays.equals(input, output)) {
                mLog.logAndThrow("Read incorrect data");
            }
        } catch (RemoteException ex) {
            mLog.log(ex.toString());
            mLog.logAndThrow("Service failed to repeat a file descriptor.");
        } catch (IOException ex) {
            mLog.log(ex.toString());
            mLog.logAndThrow("Exception while operating on temporary file");
        }
        mLog.log("...service can receive and return file descriptors.");
    }

    private void checkParcelFileDescriptorPassing(ITestService service)
            throws TestFailException {
        mLog.log("Checking that service can receive and return parcel file descriptors...");
        try {
            ParcelFileDescriptor descriptor = ParcelFileDescriptor.open(
                    getFileStreamPath("test-dummy"), ParcelFileDescriptor.MODE_CREATE |
                        ParcelFileDescriptor.MODE_WRITE_ONLY);
            ParcelFileDescriptor journeyed = service.RepeatParcelFileDescriptor(descriptor);

            FileOutputStream journeyedStream = new ParcelFileDescriptor.AutoCloseOutputStream(journeyed);

            String testData = "FrazzleSnazzleFlimFlamFlibbityGumboChops";
            byte[] output = testData.getBytes();
            journeyedStream.write(output);
            journeyedStream.close();

            FileInputStream fileInputStream = openFileInput("test-dummy");
            byte[] input = new byte[output.length];
            if (fileInputStream.read(input) != input.length) {
                mLog.logAndThrow("Read short count from file");
            }

            if (!Arrays.equals(input, output)) {
                mLog.logAndThrow("Read incorrect data");
            }
        } catch (RemoteException ex) {
            mLog.log(ex.toString());
            mLog.logAndThrow("Service failed to repeat a file descriptor.");
        } catch (IOException ex) {
            mLog.log(ex.toString());
            mLog.logAndThrow("Exception while operating on temporary file");
        }
        mLog.log("...service can receive and return file descriptors.");
    }

    private void checkServiceSpecificExceptions(
                ITestService service) throws TestFailException {
        mLog.log("Checking application exceptions...");
        for (int i = -1; i < 2; ++i) {
            try {
                service.ThrowServiceException(i);
            } catch (RemoteException ex) {
                mLog.logAndThrow("Service threw RemoteException: " +
                                 ex.toString());
            } catch (ServiceSpecificException ex) {
                if (ex.errorCode != i) {
                    mLog.logAndThrow("Service threw wrong error code: " + i);
                }
            }
        }
        mLog.log("...application exceptions work");
    }

    private void checkUtf8Strings(ITestService service)
            throws TestFailException {
        mLog.log("Checking that service can work with UTF8 strings...");
        // Note that Java's underlying encoding is UTF16.
        final List<String> utf8_queries = Arrays.asList(
              "typical string",
              "",
              "\0\0\0",
              // Java doesn't handle unicode code points above U+FFFF well.
              new String(Character.toChars(0x1F701)) + "\u03A9");
        final List<String> utf8_queries_and_nulls = Arrays.asList(
              "typical string",
              null,
              "",
              "\0\0\0",
              null,
              // Java doesn't handle unicode code points above U+FFFF well.
              new String(Character.toChars(0x1F701)) + "\u03A9");
        try {
            for (String query : utf8_queries) {
                String response = service.RepeatUtf8CppString(query);
                if (!query.equals(response)) {
                    mLog.logAndThrow("Repeat request with '" + query + "'" +
                                     " of length " + query.length() +
                                     " responded with '" + response + "'" +
                                     " of length " + response.length());
                }
            }
            {
                String[] input = (String[])utf8_queries.toArray();
                String echoed[] = new String[input.length];
                String[] reversed = service.ReverseUtf8CppString(input, echoed);
                if (!Arrays.equals(input, echoed)) {
                    mLog.logAndThrow("Failed to echo utf8 input array back.");
                }
                if (input.length != reversed.length) {
                    mLog.logAndThrow("Reversed utf8 array is the wrong size.");
                }
                for (int i = 0; i < input.length; ++i) {
                    int j = reversed.length - (1 + i);
                    if (!input[i].equals(reversed[j])) {
                        mLog.logAndThrow(
                                "input[" + i + "] = " + input[i] +
                                " but reversed value = " + reversed[j]);
                    }
                }
            }
            {
                String[] input = (String[])utf8_queries_and_nulls.toArray();
                String echoed[] = new String[input.length];
                String[] reversed = service.ReverseNullableUtf8CppString(input,
                    echoed);
                if (!Arrays.equals(input, echoed)) {
                    mLog.logAndThrow("Failed to echo utf8 input array back.");
                }
                if (input.length != reversed.length) {
                    mLog.logAndThrow("Reversed utf8 array is the wrong size.");
                }
                for (int i = 0; i < input.length; ++i) {
                    int j = reversed.length - (1 + i);
                    if (input[i] == null && reversed[j] == null) {
                        continue;
                    }

                    if (!input[i].equals(reversed[j])) {
                        mLog.logAndThrow(
                                "input[" + i + "] = " + input[i] +
                                " but reversed value = " + reversed[j]);
                    }
                }
            }
        } catch (RemoteException ex) {
            mLog.log(ex.toString());
            mLog.logAndThrow("Service failed to handle utf8 strings.");
        }
        mLog.log("...UTF8 annotations work.");
    }

    private void checkStructuredParcelable(ITestService service) throws TestFailException {
      final int kDesiredFValue = 17;

      StructuredParcelable parcelable = new StructuredParcelable();
      parcelable.shouldContainThreeFs = new int[0];
      parcelable.f = kDesiredFValue;
      parcelable.shouldBeJerry = "";

      if (!parcelable.stringDefaultsToFoo.equals("foo")) {
        mLog.logAndThrow(
            "stringDefaultsToFoo should be 'foo' but is " + parcelable.stringDefaultsToFoo);
      }
      if (parcelable.byteDefaultsToFour != 4) {
        mLog.logAndThrow("byteDefaultsToFour should be 4 but is " + parcelable.byteDefaultsToFour);
      }
      if (parcelable.intDefaultsToFive != 5) {
        mLog.logAndThrow("intDefaultsToFive should be 5 but is " + parcelable.intDefaultsToFive);
      }
      if (parcelable.longDefaultsToNegativeSeven != -7) {
        mLog.logAndThrow("longDefaultsToNegativeSeven should be -7 but is "
            + parcelable.longDefaultsToNegativeSeven);
      }
      if (!parcelable.booleanDefaultsToTrue) {
        mLog.logAndThrow("booleanDefaultsToTrue should be true");
      }
      if (parcelable.charDefaultsToC != 'C') {
        mLog.logAndThrow("charDefaultsToC is " + parcelable.charDefaultsToC);
      }
      if (parcelable.floatDefaultsToPi != 3.14f) {
        mLog.logAndThrow("floatDefaultsToPi is " + parcelable.floatDefaultsToPi);
      }
      if (parcelable.doubleWithDefault != -3.14e17) {
        mLog.logAndThrow(
            "doubleWithDefault is " + parcelable.doubleWithDefault + " but should be -3.14e17");
      }
      if (!Arrays.equals(parcelable.arrayDefaultsTo123, new int[] {1, 2, 3})) {
        mLog.logAndThrow("arrayDefaultsTo123 should be [1,2,3] but is "
            + Arrays.toString(parcelable.arrayDefaultsTo123));
      }
      if (parcelable.arrayDefaultsToEmpty.length != 0) {
        mLog.logAndThrow("arrayDefaultsToEmpty should be empty but is "
            + Arrays.toString(parcelable.arrayDefaultsToEmpty));
      }

      try {
        service.FillOutStructuredParcelable(parcelable);
      } catch (RemoteException ex) {
        mLog.log(ex.toString());
        mLog.logAndThrow("Service failed to handle structured parcelable.");
      }

      if (!Arrays.equals(parcelable.shouldContainThreeFs,
              new int[] {kDesiredFValue, kDesiredFValue, kDesiredFValue})) {
        mLog.logAndThrow(
            "shouldContainThreeFs is " + Arrays.toString(parcelable.shouldContainThreeFs));
      }

      if (!parcelable.shouldBeJerry.equals("Jerry")) {
        mLog.logAndThrow("shouldBeJerry should be 'Jerry' but is " + parcelable.shouldBeJerry);
      }
    }

    private void checkDefaultImpl(ITestService service) throws TestFailException {
      final int expectedArg = 100;
      final int expectedReturnValue = 200;

      boolean success = ITestService.Stub.setDefaultImpl(new ITestService.Default() {
        @Override
        public int UnimplementedMethod(int arg) throws RemoteException {
          if (arg != expectedArg) {
            throw new RemoteException("Argument for UnimplementedMethod is expected "
                + " to be " + expectedArg + ", but got " + arg);
          }
          return expectedReturnValue;
        }
      });
      if (!success) {
        mLog.logAndThrow("Failed to set default impl for ITestService");
      }

      try {
        int ret = service.UnimplementedMethod(expectedArg);
        if (ret != expectedReturnValue) {
          mLog.logAndThrow("Return value from UnimplementedMethod is expected "
              + " to be " + expectedReturnValue + ", but got " + ret);
        }
      } catch (RemoteException ex) {
        mLog.log(ex.toString());
        mLog.logAndThrow("Failed to call UnimplementedMethod");
      }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.i(TAG, "Starting!");
        try {
          init();
          ITestService service = getService();
          checkPrimitiveRepeat(service);
          checkArrayReversal(service);
          checkBinderExchange(service);
          checkListReversal(service);
          checkSimpleParcelables(service);
          checkPersistableBundles(service);
          checkFileDescriptorPassing(service);
          checkParcelFileDescriptorPassing(service);
          checkServiceSpecificExceptions(service);
          checkUtf8Strings(service);
          checkStructuredParcelable(service);
          new NullableTests(service, mLog).runTests();
          checkDefaultImpl(service);

          mLog.log(mSuccessSentinel);
        } catch (TestFailException e) {
            mLog.log(mFailureSentinel);
            throw new RuntimeException(e);
        } finally {
            if (mLog != null) {
                mLog.close();
            }
        }
    }
}
