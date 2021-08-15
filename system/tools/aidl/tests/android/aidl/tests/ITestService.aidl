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

package android.aidl.tests;

import android.aidl.tests.INamedCallback;
import android.aidl.tests.SimpleParcelable;
import android.aidl.tests.StructuredParcelable;
import android.os.PersistableBundle;

interface ITestService {
  // Test that constants are accessible
  const int TEST_CONSTANT = 42;
  const int TEST_CONSTANT2 = -42;
  const int TEST_CONSTANT3 = +42;
  const int TEST_CONSTANT4 = +4;
  const int TEST_CONSTANT5 = -4;
  const int TEST_CONSTANT6 = -0;
  const int TEST_CONSTANT7 = +0;
  const int TEST_CONSTANT8 = 0;
  const int TEST_CONSTANT9 = 0x56;
  const int TEST_CONSTANT10 = 0xa5;
  const int TEST_CONSTANT11 = 0xFA;
  const int TEST_CONSTANT12 = 0xffffffff;

  const String STRING_TEST_CONSTANT = "foo";
  const String STRING_TEST_CONSTANT2 = "bar";

  const @utf8InCpp String STRING_TEST_CONSTANT_UTF8 = "baz";

  // Test that primitives work as parameters and return types.
  boolean RepeatBoolean(boolean token);
  byte RepeatByte(byte token);
  char RepeatChar(char token);
  int RepeatInt(int token);
  long RepeatLong(long token);
  float RepeatFloat(float token);
  double RepeatDouble(double token);
  String RepeatString(String token);
  Map RepeatMap(in Map token);

  SimpleParcelable RepeatSimpleParcelable(in SimpleParcelable input,
                                          out SimpleParcelable repeat);
  PersistableBundle RepeatPersistableBundle(in PersistableBundle input);

  // Test that arrays work as parameters and return types.
  boolean[] ReverseBoolean(in boolean[] input, out boolean[] repeated);
  byte[]    ReverseByte   (in byte[]    input, out byte[]    repeated);
  char[]    ReverseChar   (in char[]    input, out char[]    repeated);
  int[]     ReverseInt    (in int[]     input, out int[]     repeated);
  long[]    ReverseLong   (in long[]    input, out long[]    repeated);
  float[]   ReverseFloat  (in float[]   input, out float[]   repeated);
  double[]  ReverseDouble (in double[]  input, out double[]  repeated);
  String[]  ReverseString (in String[]  input, out String[]  repeated);

  SimpleParcelable[]  ReverseSimpleParcelables(in SimpleParcelable[] input,
                                               out SimpleParcelable[] repeated);
  PersistableBundle[] ReversePersistableBundles(
      in PersistableBundle[] input, out PersistableBundle[] repeated);

  // Test that clients can send and receive Binders.
  INamedCallback GetOtherTestService(String name);
  boolean VerifyName(INamedCallback service, String name);

  // Test that List<T> types work correctly.
  List<String> ReverseStringList(in List<String> input,
                                 out List<String> repeated);
  List<IBinder> ReverseNamedCallbackList(in List<IBinder> input,
                                         out List<IBinder> repeated);

  FileDescriptor RepeatFileDescriptor(in FileDescriptor read);
  FileDescriptor[] ReverseFileDescriptorArray(in FileDescriptor[] input,
                                              out FileDescriptor[] repeated);

  ParcelFileDescriptor RepeatParcelFileDescriptor(in ParcelFileDescriptor read);
  ParcelFileDescriptor[] ReverseParcelFileDescriptorArray(in ParcelFileDescriptor[] input,
                                              out ParcelFileDescriptor[] repeated);

  // Test that service specific exceptions work correctly.
  void ThrowServiceException(int code);

  // Test nullability
  @nullable int[] RepeatNullableIntArray(in @nullable int[] input);
  @nullable String RepeatNullableString(in @nullable String input);
  @nullable List<String> RepeatNullableStringList(in @nullable List<String> input);
  @nullable SimpleParcelable RepeatNullableParcelable(in @nullable SimpleParcelable input);

  void TakesAnIBinder(in IBinder input);
  void TakesAnIBinderList(in List<IBinder> input);
  void TakesANullableIBinder(in @nullable IBinder input);
  void TakesANullableIBinderList(in @nullable List<IBinder> input);

  // Test utf8 decoding from utf16 wire format
  @utf8InCpp String RepeatUtf8CppString(@utf8InCpp String token);
  @nullable @utf8InCpp String RepeatNullableUtf8CppString(
      @nullable @utf8InCpp String token);

  @utf8InCpp String[]  ReverseUtf8CppString (in @utf8InCpp String[] input,
                                             out @utf8InCpp String[] repeated);

  @nullable @utf8InCpp String[]  ReverseNullableUtf8CppString (
      in @nullable @utf8InCpp String[] input,
      out @nullable @utf8InCpp String[] repeated);

  @nullable @utf8InCpp List<String> ReverseUtf8CppStringList(
      in @nullable @utf8InCpp List<String> input,
      out @nullable @utf8InCpp List<String> repeated);

  @nullable INamedCallback GetCallback(boolean return_null);

  // Since this paracelable has clearly defined default values, it would be
  // inefficient to use an IPC to fill it out in practice.
  void FillOutStructuredParcelable(inout StructuredParcelable parcel);

  // This is to emulate a method that is added after the service is implemented.
  // So the client cannot assume that call to this method will be successful
  // or not. However, inside the test environment, we can't build client and
  // the server with different version of this AIDL file. So, we let the server
  // to actually implement this, but intercept the dispatch to the method
  // inside onTransact().
  int UnimplementedMethod(int arg);
}
