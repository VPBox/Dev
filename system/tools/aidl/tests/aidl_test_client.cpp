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

#include <iostream>

#include <android-base/logging.h>
#include <binder/IServiceManager.h>
#include <utils/String16.h>
#include <utils/StrongPointer.h>

#include "android/aidl/tests/ITestService.h"

#include "aidl_test_client_defaultimpl.h"
#include "aidl_test_client_file_descriptors.h"
#include "aidl_test_client_nullables.h"
#include "aidl_test_client_parcelables.h"
#include "aidl_test_client_primitives.h"
#include "aidl_test_client_service_exceptions.h"
#include "aidl_test_client_utf8_strings.h"

// libutils:
using android::OK;
using android::sp;
using android::status_t;
using android::String16;

// libbinder:
using android::getService;

// generated
using android::aidl::tests::ITestService;

using std::cerr;
using std::cout;
using std::endl;

namespace android {
namespace aidl {
namespace tests {
namespace client {

const char kServiceName[] = "android.aidl.tests.ITestService";

bool GetService(sp<ITestService>* service) {
  cout << "Retrieving test service binder" << endl;
  status_t status = getService(String16(kServiceName), service);
  if (status != OK) {
    cerr << "Failed to get service binder: '" << kServiceName
         << "' status=" << status << endl;
    return false;
  }
  return true;
}

}  // namespace client
}  // namespace tests
}  // namespace aidl
}  // namespace android

/* Runs all the test cases in aidl_test_client_*.cpp files. */
int main(int /* argc */, char * argv []) {
  android::base::InitLogging(argv, android::base::StderrLogger);
  sp<ITestService> service;
  namespace client_tests = android::aidl::tests::client;

  if (!client_tests::GetService(&service)) return 1;

  if (!client_tests::ConfirmPrimitiveRepeat(service)) return 1;

  if (!client_tests::ConfirmReverseArrays(service)) return 1;

  if (!client_tests::ConfirmReverseLists(service)) return 1;

  if (!client_tests::ConfirmReverseBinderLists(service)) return 1;

  if (!client_tests::ConfirmSimpleParcelables(service)) return 1;

  if (!client_tests::ConfirmPersistableBundles(service)) return 1;

  if (!client_tests::ConfirmStructuredParcelables(service)) return 1;

  if (!client_tests::ConfirmFileDescriptors(service)) return 1;

  if (!client_tests::ConfirmFileDescriptorArrays(service)) return 1;

  if (!client_tests::ConfirmParcelFileDescriptors(service)) return 1;

  if (!client_tests::ConfirmParcelFileDescriptorArrays(service)) return 1;

  if (!client_tests::ConfirmServiceSpecificExceptions(service)) return 1;

  if (!client_tests::ConfirmNullables(service)) return 1;

  if (!client_tests::ConfirmUtf8InCppStringRepeat(service)) return 1;

  if (!client_tests::ConfirmUtf8InCppStringArrayReverse(service)) return 1;

  if (!client_tests::ConfirmUtf8InCppStringListReverse(service)) return 1;

  if (!client_tests::ConfirmDefaultImpl(service)) return 1;

  return 0;
}
