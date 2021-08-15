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

#include "aidl_test_client_primitives.h"

#include <iostream>
#include <vector>

#include <utils/String16.h>
#include <utils/String8.h>
#include <binder/Value.h>
#include <binder/Map.h>

#include "android/aidl/tests/INamedCallback.h"

#include "test_helpers.h"

// libutils:
using android::sp;
using android::String16;
using android::String8;

// libbinder:
using android::binder::Status;
using android::binder::Value;
using android::binder::Map;

// generated
using android::aidl::tests::ITestService;
using android::aidl::tests::INamedCallback;

using std::cerr;
using std::cout;
using std::endl;
using std::vector;

namespace android {
namespace aidl {
namespace tests {
namespace client {

bool ConfirmPrimitiveRepeat(const sp<ITestService>& s) {
  cout << "Confirming passing and returning primitives works." << endl;

  Map test_map;
  test_map["first_val"] = int8_t{-128};
  test_map["second_val"] = int32_t{1 << 30};
  test_map["third_val"] = String16("OHAI");

  if (!RepeatPrimitive(s, &ITestService::RepeatBoolean, true) ||
      !RepeatPrimitive(s, &ITestService::RepeatByte, int8_t{-128}) ||
      !RepeatPrimitive(s, &ITestService::RepeatChar, char16_t{'A'}) ||
      !RepeatPrimitive(s, &ITestService::RepeatInt, int32_t{1 << 30}) ||
      !RepeatPrimitive(s, &ITestService::RepeatLong, int64_t{1LL << 60}) ||
      !RepeatPrimitive(s, &ITestService::RepeatFloat, float{1.0f/3.0f}) ||
      !RepeatPrimitive(s, &ITestService::RepeatDouble, double{1.0/3.0}) ||
      !RepeatPrimitive(s, &ITestService::RepeatMap, test_map) ||
      !RepeatPrimitive(
          s, &ITestService::RepeatInt, ITestService::TEST_CONSTANT)  ||
      !RepeatPrimitive(
          s, &ITestService::RepeatInt, ITestService::TEST_CONSTANT2) ||
      !RepeatPrimitive(
          s, &ITestService::RepeatInt, ITestService::TEST_CONSTANT3) ||
      !RepeatPrimitive(
          s, &ITestService::RepeatInt, ITestService::TEST_CONSTANT4) ||
      !RepeatPrimitive(
          s, &ITestService::RepeatInt, ITestService::TEST_CONSTANT5) ||
      !RepeatPrimitive(
          s, &ITestService::RepeatInt, ITestService::TEST_CONSTANT6) ||
      !RepeatPrimitive(
          s, &ITestService::RepeatInt, ITestService::TEST_CONSTANT7) ||
      !RepeatPrimitive(
          s, &ITestService::RepeatInt, ITestService::TEST_CONSTANT8) ||
      !RepeatPrimitive(
          s, &ITestService::RepeatInt, ITestService::TEST_CONSTANT9) ||
      !RepeatPrimitive(
          s, &ITestService::RepeatInt, ITestService::TEST_CONSTANT10) ||
      !RepeatPrimitive(
          s, &ITestService::RepeatInt, ITestService::TEST_CONSTANT11) ||
      !RepeatPrimitive(
          s, &ITestService::RepeatInt, ITestService::TEST_CONSTANT12)
      ) {
    return false;
  }

  vector<String16> inputs = {
      String16("Deliver us from evil."),
      String16(),
      String16("\0\0", 2),
      // This is actually two unicode code points:
      //   U+10437: The 'small letter yee' character in the deseret alphabet
      //   U+20AC: A euro sign
      String16("\xD8\x01\xDC\x37\x20\xAC"),
      ITestService::STRING_TEST_CONSTANT(),
      ITestService::STRING_TEST_CONSTANT2(),
  };
  for (const auto& input : inputs) {
    String16 reply;
    Status status = s->RepeatString(input, &reply);
    if (!status.isOk() || input != reply) {
      cerr << "Failed while requesting service to repeat String16=\""
           << String8(input).string()
           << "\". Got status=" << status.toString8() << endl;
      return false;
    }
  }
  return true;
}

bool ConfirmReverseArrays(const sp<ITestService>& s) {
  cout << "Confirming passing and returning arrays works." << endl;

  if (!ReverseArray(s, &ITestService::ReverseBoolean,
                    {true, false, false}) ||
      !ReverseArray(s, &ITestService::ReverseByte,
                    {uint8_t{255}, uint8_t{0}, uint8_t{127}}) ||
      !ReverseArray(s, &ITestService::ReverseChar,
                    {char16_t{'A'}, char16_t{'B'}, char16_t{'C'}}) ||
      !ReverseArray(s, &ITestService::ReverseInt,
                    {1, 2, 3}) ||
      !ReverseArray(s, &ITestService::ReverseLong,
                    {-1LL, 0LL, int64_t{1LL << 60}}) ||
      !ReverseArray(s, &ITestService::ReverseFloat,
                    {-0.3f, -0.7f, 8.0f}) ||
      !ReverseArray(s, &ITestService::ReverseDouble,
                    {1.0/3.0, 1.0/7.0, 42.0}) ||
      !ReverseArray(s, &ITestService::ReverseString,
                    {String16{"f"}, String16{"a"}, String16{"b"}})) {
    return false;
  }

  return true;
}

bool ConfirmReverseLists(const sp<ITestService>& s) {
  cout << "Confirming passing and returning List<T> works." << endl;

  if (!ReverseArray(s, &ITestService::ReverseStringList,
                    {String16{"f"}, String16{"a"}, String16{"b"}})) {
    return false;
  }

  return true;
}

bool ConfirmReverseBinderLists(const sp<ITestService>& s) {
  Status status;
  cout << "Confirming passing and returning List<T> works with binders." << endl;

  vector<String16> names = {
    String16{"Larry"},
    String16{"Curly"},
    String16{"Moe"}
  };

  vector<sp<IBinder>> input;

  for (int i = 0; i < 3; i++) {
    sp<INamedCallback> got;

    status = s->GetOtherTestService(names[i], &got);
    if (!status.isOk()) {
      cerr << "Could not retrieve service for test." << endl;
      return false;
    }

    input.push_back(INamedCallback::asBinder(got));
  }

  vector<sp<IBinder>> output;
  vector<sp<IBinder>> reversed;

  status = s->ReverseNamedCallbackList(input, &output, &reversed);
  if (!status.isOk()) {
    cerr << "Failed to reverse named callback list." << endl;
  }

  if (output.size() != 3) {
    cerr << "ReverseNamedCallbackList gave repetition with wrong length." << endl;
    return false;
  }

  if (reversed.size() != 3) {
    cerr << "ReverseNamedCallbackList gave reversal with wrong length." << endl;
    return false;
  }

  for (int i = 0; i < 3; i++) {
    String16 ret;
    sp<INamedCallback> named_callback =
        android::interface_cast<INamedCallback>(output[i]);
    status = named_callback->GetName(&ret);

    if (!status.isOk()) {
      cerr << "Could not query INamedCallback from output" << endl;
      return false;
    }

    if (ret != names[i]) {
      cerr << "Output had wrong INamedCallback" << endl;
      return false;
    }
  }

  for (int i = 0; i < 3; i++) {
    String16 ret;
    sp<INamedCallback> named_callback =
        android::interface_cast<INamedCallback>(reversed[i]);
    status = named_callback->GetName(&ret);

    if (!status.isOk()) {
      cerr << "Could not query INamedCallback from reversed output" << endl;
      return false;
    }

    if (ret != names[2 - i]) {
      cerr << "Reversed output had wrong INamedCallback" << endl;
      return false;
    }
  }

  return true;
}

}  // namespace client
}  // namespace tests
}  // namespace aidl
}  // namespace android
