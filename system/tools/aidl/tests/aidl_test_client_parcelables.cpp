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

#include "aidl_test_client_parcelables.h"

#include <iostream>
#include <vector>

// libutils:
using android::sp;

// libbinder:
using android::binder::Status;

// generated
using android::aidl::tests::ITestService;
using android::aidl::tests::SimpleParcelable;
using android::os::PersistableBundle;

using std::cout;
using std::endl;
using std::vector;

namespace android {
namespace aidl {
namespace tests {
namespace client {

bool ConfirmSimpleParcelables(const sp<ITestService>& s) {
  cout << "Confirming passing and returning SimpleParcelable objects works."
       << endl;

  SimpleParcelable input("Booya", 42);
  SimpleParcelable out_param, returned;
  Status status = s->RepeatSimpleParcelable(input, &out_param, &returned);
  if (!status.isOk()) {
    cout << "Binder call failed." << endl;
    return false;
  }
  if (input != out_param || input != returned) {
    cout << "Failed to repeat SimpleParcelable objects." << endl;
    return false;
  }

  cout << "Attempting to reverse an array of SimpleParcelable objects." << endl;
  const vector<SimpleParcelable> original{SimpleParcelable("first", 0),
                                          SimpleParcelable("second", 1),
                                          SimpleParcelable("third", 2)};
  vector<SimpleParcelable> repeated;
  vector<SimpleParcelable> reversed;
  status = s->ReverseSimpleParcelables(original, &repeated, &reversed);
  if (!status.isOk()) {
    cout << "Binder call failed." << endl;
    return false;
  }
  std::reverse(reversed.begin(), reversed.end());
  if (repeated != original || reversed != original) {
    cout << "Failed to reverse an array of SimpleParcelable objects." << endl;
    return false;
  }

  return true;
}

bool ConfirmPersistableBundles(const sp<ITestService>& s) {
  cout << "Confirming passing and returning PersistableBundle objects works."
       << endl;

  PersistableBundle empty_bundle, returned;
  Status status = s->RepeatPersistableBundle(empty_bundle, &returned);
  if (!status.isOk()) {
    cout << "Binder call failed for empty PersistableBundle." << endl;
    return false;
  }
  if (empty_bundle != returned) {
    cout << "Failed to repeat empty PersistableBundle." << endl;
    return false;
  }

  PersistableBundle non_empty_bundle;
  non_empty_bundle.putBoolean(String16("test_bool"), false);
  non_empty_bundle.putInt(String16("test_int"), 33);
  non_empty_bundle.putLong(String16("test_long"), 34359738368L);
  non_empty_bundle.putDouble(String16("test_double"), 1.1);
  non_empty_bundle.putString(String16("test_string"), String16("Woot!"));
  non_empty_bundle.putBooleanVector(String16("test_bool_vector"),
                                    {true, false, true});
  non_empty_bundle.putIntVector(String16("test_int_vector"), {33, 44, 55, 142});
  non_empty_bundle.putLongVector(String16("test_long_vector"),
                                 {34L, 8371L, 34359738375L});
  non_empty_bundle.putDoubleVector(String16("test_double_vector"), {2.2, 5.4});
  non_empty_bundle.putStringVector(String16("test_string_vector"),
                                   {String16("hello"), String16("world!")});
  PersistableBundle nested_bundle;
  nested_bundle.putInt(String16("test_nested_int"), 345);
  non_empty_bundle.putPersistableBundle(String16("test_persistable_bundle"),
                                        nested_bundle);

  status = s->RepeatPersistableBundle(non_empty_bundle, &returned);
  if (!status.isOk()) {
    cout << "Binder call failed. " << endl;
    return false;
  }
  if (non_empty_bundle != returned) {
    cout << "Failed to repeat PersistableBundle object." << endl;
    return false;
  }

  cout << "Attempting to reverse an array of PersistableBundle objects."
       << endl;
  PersistableBundle first;
  PersistableBundle second;
  PersistableBundle third;
  first.putInt(String16("test_int"), 1231);
  second.putLong(String16("test_long"), 222222L);
  third.putDouble(String16("test_double"), 10.8);
  const vector<PersistableBundle> original{first, second, third};

  vector<PersistableBundle> repeated;
  vector<PersistableBundle> reversed;
  status = s->ReversePersistableBundles(original, &repeated, &reversed);
  if (!status.isOk()) {
    cout << "Binder call failed." << endl;
    return false;
  }
  std::reverse(reversed.begin(), reversed.end());
  if (repeated != original || reversed != original) {
    cout << "Failed to reverse an array of PersistableBundle objects." << endl;
    return false;
  }

  return true;
}

bool ConfirmStructuredParcelables(const sp<ITestService>& s) {
  constexpr int kDesiredValue = 23;

  StructuredParcelable parcelable;
  parcelable.f = kDesiredValue;

  if (parcelable.stringDefaultsToFoo != String16("foo")) {
    cout << "stringDefaultsToFoo should be 'foo' but is " << parcelable.stringDefaultsToFoo << endl;
    return false;
  }
  if (parcelable.byteDefaultsToFour != 4) {
    cout << "byteDefaultsToFour should be 4 but is " << parcelable.byteDefaultsToFour << endl;
    return false;
  }
  if (parcelable.intDefaultsToFive != 5) {
    cout << "intDefaultsToFive should be 5 but is " << parcelable.intDefaultsToFive << endl;
    return false;
  }
  if (parcelable.longDefaultsToNegativeSeven != -7) {
    cout << "longDefaultsToNegativeSeven should be -7 but is "
         << parcelable.longDefaultsToNegativeSeven << endl;
    return false;
  }
  if (!parcelable.booleanDefaultsToTrue) {
    cout << "booleanDefaultsToTrue isn't true" << endl;
    return false;
  }
  if (parcelable.charDefaultsToC != 'C') {
    cout << "charDefaultsToC is " << parcelable.charDefaultsToC << endl;
    return false;
  }
  if (parcelable.floatDefaultsToPi != 3.14f) {
    cout << "floatDefaultsToPi is " << parcelable.floatDefaultsToPi << endl;
    return false;
  }
  if (parcelable.doubleWithDefault != -3.14e17) {
    cout << "doubleWithDefault is " << parcelable.doubleWithDefault << " but should be -3.14e17"
         << endl;
    return false;
  }
  if (parcelable.arrayDefaultsTo123.size() != 3) {
    cout << "arrayDefaultsTo123 is of length " << parcelable.arrayDefaultsTo123.size() << endl;
    return false;
  }
  for (int i = 0; i < 3; i++) {
    if (parcelable.arrayDefaultsTo123[i] != i + 1) {
      cout << "arrayDefaultsTo123[" << i << "] is " << parcelable.arrayDefaultsTo123[i]
           << " but should be " << i + 1 << endl;
      return false;
    }
  }
  if (!parcelable.arrayDefaultsToEmpty.empty()) {
    cout << "arrayDefaultsToEmpty is not empty " << parcelable.arrayDefaultsToEmpty.size() << endl;
    return false;
  }

  s->FillOutStructuredParcelable(&parcelable);

  if (parcelable.shouldContainThreeFs.size() != 3) {
    cout << "shouldContainThreeFs is of length " << parcelable.shouldContainThreeFs.size() << endl;
    return false;
  }

  for (int i = 0; i < 3; i++) {
    if (parcelable.shouldContainThreeFs[i] != kDesiredValue) {
      cout << "shouldContainThreeFs[" << i << "] is " << parcelable.shouldContainThreeFs[i]
           << " but should be " << kDesiredValue << endl;
      return false;
    }
  }

  if (parcelable.shouldBeJerry != "Jerry") {
    cout << "shouldBeJerry should be 'Jerry' but is " << parcelable.shouldBeJerry << endl;
    return false;
  }

  return true;
}

}  // namespace client
}  // namespace tests
}  // namespace aidl
}  // namespace android
