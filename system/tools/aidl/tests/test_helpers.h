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

#include <algorithm>
#include <vector>

#include <android-base/logging.h>
#include <binder/Status.h>
#include <utils/StrongPointer.h>

#include "android/aidl/tests/ITestService.h"

namespace android {
namespace aidl {
namespace tests {
namespace client {

template <typename T, typename U, typename V>
bool RepeatPrimitive(
    const android::sp<android::aidl::tests::ITestService>& service,
    android::binder::Status(android::aidl::tests::ITestService::*func)(T, V*),
    U input) {
  V reply;
  android::binder::Status status = (*service.*func)(input, &reply);
  if (!status.isOk() || input != reply) {
    LOG(ERROR) << "Failed to repeat primitive. status=" << status.toString8()
               << ".";
    return false;
  }
  return true;
}

template <typename T>
bool ReverseArray(
    const android::sp<android::aidl::tests::ITestService>& service,
    android::binder::Status(android::aidl::tests::ITestService::*func)(
        const std::vector<T>&, std::vector<T>*, std::vector<T>*),
        std::vector<T> input) {
  std::vector<T> actual_reversed;
  std::vector<T> actual_repeated;
  android::binder::Status status = (*service.*func)(
      input, &actual_repeated, &actual_reversed);
  if (!status.isOk()) {
    LOG(ERROR) << "Failed to repeat array. status=" << status.toString8()
               << ".";
    return false;
  }
  if (input != actual_repeated) {
    LOG(ERROR) << "Repeated version of array did not match";
    LOG(ERROR) << "input.size()=" << input.size()
               << " repeated.size()=" << actual_repeated.size();
    return false;
  }
  std::reverse(input.begin(), input.end());
  if (input != actual_reversed) {
    LOG(ERROR) << "Reversed version of array did not match";
    return false;
  }
  return true;
}

}  // namespace client
}  // namespace tests
}  // namespace aidl
}  // namespace android
