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

#ifndef ANDROID_AIDL_TESTS_CLIENT_UTF8_STRINGS_H
#define ANDROID_AIDL_TESTS_CLIENT_UTF8_STRINGS_H

#include <utils/StrongPointer.h>

#include "android/aidl/tests/ITestService.h"

// Tests for passing and returning utf8 strings.
namespace android {
namespace aidl {
namespace tests {
namespace client {

bool ConfirmUtf8InCppStringRepeat(
    const android::sp<android::aidl::tests::ITestService>& s);
bool ConfirmUtf8InCppStringArrayReverse(
    const android::sp<android::aidl::tests::ITestService>& s);
bool ConfirmUtf8InCppStringListReverse(
    const android::sp<android::aidl::tests::ITestService>& s);

}  // namespace client
}  // namespace tests
}  // namespace aidl
}  // namespace android

#endif  // ANDROID_AIDL_TESTS_CLIENT_UTF8_STRINGS_H

