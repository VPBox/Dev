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

#include "aidl_test_client_nullables.h"

#include <utils/String16.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// libutils:
using android::sp;
using android::String16;

// libbinder:
using android::binder::Status;

// generated
using android::aidl::tests::ITestService;
using android::aidl::tests::SimpleParcelable;

using std::string;
using std::unique_ptr;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;

namespace android {
namespace aidl {
namespace tests {
namespace client {

namespace {
template<typename T>
bool ValuesEqual(const unique_ptr<T>& in, const unique_ptr<T>& out) {
  return *in == *out;
}

template<>
bool ValuesEqual<vector<unique_ptr<String16>>>(
    const unique_ptr<vector<unique_ptr<String16>>>& in,
    const unique_ptr<vector<unique_ptr<String16>>>& out) {
  if (!in) {
    return !out;
  }

  if (!out) {
    return false;
  }

  if (in->size() != out->size()) {
    return false;
  }

  for (size_t i = 0; i < in->size(); i++) {
    const unique_ptr<String16>& a = (*in)[i];
    const unique_ptr<String16>& b = (*out)[i];

    if (!(a || b)) {
      continue;
    }

    if (!(a && b)) {
      return false;
    }

    if (*a != *b) {
      return false;
    }
  }

  return true;
}

template<typename T>
bool ConfirmNullableType(const sp<ITestService>& s, const string& type_name,
                         unique_ptr<T> in,
                         Status(ITestService::*func)(const unique_ptr<T>&,
                                                     unique_ptr<T>*)) {
  cout << "... Confirming nullables for " << type_name << " ..." << endl;
  Status status;
  unique_ptr<T> out;

  status = (*s.*func)(in, &out);

  if (!status.isOk()) {
    cerr << "Could not repeat nullable " << type_name << "." << endl;
    return false;
  }

  if (!out) {
    cerr << "Got back null when repeating " << type_name << "." << endl;
    return false;
  }

  if (!ValuesEqual(in, out)) {
    cerr << "Got back a non-matching value when repeating " << type_name
         << "." << endl;
    return false;
  }

  in.reset();
  status = (*s.*func)(in, &out);

  if (!status.isOk()) {
    cerr << "Could not repeat null as " << type_name << "." << endl;
    return false;
  }

  if (out) {
    cerr << "Got back a value when sent null for " << type_name << "."
         << endl;
    return false;
  }

  return true;
}

bool CheckAppropriateIBinderHandling(const sp<ITestService>& s) {

  Status status;
  sp<IBinder> binder = new BBinder();
  sp<IBinder> null_binder = nullptr;
  unique_ptr<vector<sp<IBinder>>> list_with_nulls(
      new vector<sp<IBinder>>{binder, null_binder});
  unique_ptr<vector<sp<IBinder>>> list_without_nulls(
      new vector<sp<IBinder>>{binder, binder});

  // Methods without @nullable throw up when given null binders
  if (s->TakesAnIBinder(null_binder).exceptionCode() !=
      binder::Status::EX_NULL_POINTER) {
    cerr << "Did not receive expected null exception on line: "
         << __LINE__ << endl;
    return false;
  }
  if (s->TakesAnIBinderList(*list_with_nulls).exceptionCode() !=
      binder::Status::EX_NULL_POINTER) {
    cerr << "Did not receive expected null exception on line: "
         << __LINE__ << endl;
    return false;
  }

  // But those same methods are fine with valid binders
  if (!s->TakesAnIBinder(binder).isOk()) {
    cerr << "Received unexpected exception on line "
         << __LINE__ << endl;
    return false;
  }
  if (!s->TakesAnIBinderList(*list_without_nulls).isOk()) {
    cerr << "Received unexpected exception on line "
         << __LINE__ << endl;
    return false;
  }

  // And methods with @nullable don't care.
  if (!s->TakesANullableIBinder(null_binder).isOk()) {
    cerr << "Received unexpected exception on line "
         << __LINE__ << endl;
    return false;
  }
  if (!s->TakesANullableIBinder(binder).isOk()) {
    cerr << "Received unexpected exception on line "
         << __LINE__ << endl;
    return false;
  }
  if (!s->TakesANullableIBinderList(list_with_nulls).isOk()) {
    cerr << "Received unexpected exception on line "
         << __LINE__ << endl;
    return false;
  }
  if (!s->TakesANullableIBinderList(list_without_nulls).isOk()) {
    cerr << "Received unexpected exception on line "
         << __LINE__ << endl;
    return false;
  }

  return true;
}

bool CheckAppropriateIInterfaceHandling(const sp<ITestService>& s) {

  sp<INamedCallback> callback;
  if (!s->GetCallback(false, &callback).isOk()) {
    cerr << "Received unexpected exception on line "
         << __LINE__ << endl;
    return false;
  }
  if (callback.get() == nullptr) {
    cerr << "Expected to receive a non-null binder on line: "
         << __LINE__ << endl;
    return false;
  }
  if (!s->GetCallback(true, &callback).isOk()) {
    cerr << "Received unexpected exception on line "
         << __LINE__ << endl;
    return false;
  }
  if (callback.get() != nullptr) {
    cerr << "Expected to receive a null binder on line: "
         << __LINE__ << endl;
    return false;
  }
  return true;
}

}  // namespace

bool ConfirmNullables(const sp<ITestService>& s) {
  Status status;
  cout << "Confirming passing and returning nullable values works." << endl;

  if (!ConfirmNullableType(s, "integer array",
                           unique_ptr<vector<int32_t>>(
                               new vector<int32_t>({1,2,3})),
                           &ITestService::RepeatNullableIntArray)) {
    return false;
  }

  if (!ConfirmNullableType(s, "string",
                           unique_ptr<String16>(new String16("Blooob")),
                           &ITestService::RepeatNullableString)) {
    return false;
  }

  unique_ptr<vector<unique_ptr<String16>>> test_string_array(
      new vector<unique_ptr<String16>>());
  test_string_array->push_back(unique_ptr<String16>(new String16("Wat")));
  test_string_array->push_back(unique_ptr<String16>(
      new String16("Blooob")));
  test_string_array->push_back(unique_ptr<String16>(new String16("Wat")));
  test_string_array->push_back(unique_ptr<String16>(nullptr));
  test_string_array->push_back(unique_ptr<String16>(new String16("YEAH")));
  test_string_array->push_back(unique_ptr<String16>(
      new String16("OKAAAAY")));

  if (!ConfirmNullableType(s, "string array", std::move(test_string_array),
                           &ITestService::RepeatNullableStringList)) {
    return false;
  }

  if (!ConfirmNullableType(s, "parcelable",
                           unique_ptr<SimpleParcelable>(
                               new SimpleParcelable("Booya", 42)),
                           &ITestService::RepeatNullableParcelable)) {
    return false;
  }

  if (!CheckAppropriateIBinderHandling(s)) {
    cerr << "Handled null IBinders poorly." << endl;
    return false;
  }

  if (!CheckAppropriateIInterfaceHandling(s)) {
    cerr << "Handled nullable IInterface instances poorly." << endl;
    return false;
  }

  return true;
}

}  // namespace client
}  // namespace tests
}  // namespace aidl
}  // namespace android
