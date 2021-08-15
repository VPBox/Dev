/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include "aidl_test_client_defaultimpl.h"

#include <iostream>

namespace android {
namespace aidl {
namespace tests {
namespace client {

static const int32_t kExpectedArgValue = 100;
static const int32_t kExpectedReturnValue = 200;

bool ConfirmDefaultImpl(const sp<ITestService>& s) {
  class Def : public android::aidl::tests::ITestServiceDefault {
    android::binder::Status UnimplementedMethod(int32_t arg, int32_t* _aidl_return) override {
      if (arg != kExpectedArgValue) {
        std::cerr << "Argument to UnimplementedMethod is expected to be " << kExpectedArgValue
                  << ", "
                  << "but got " << arg << std::endl;
        return android::binder::Status::fromStatusT(android::FAILED_TRANSACTION);
      }
      *_aidl_return = kExpectedReturnValue;
      return android::binder::Status::ok();
    }
  };

  bool success = android::aidl::tests::ITestService::setDefaultImpl(std::make_unique<Def>());
  if (!success) {
    std::cerr << "Failed to set default impl for ITestService" << std::endl;
    return false;
  }

  int32_t ret;
  android::binder::Status status = s->UnimplementedMethod(kExpectedArgValue, &ret);
  if (!status.isOk()) {
    std::cerr << "Call to UnimplementedMethod() has failed. status=" << status.toString8()
              << std::endl;
    return false;
  }

  if (ret != kExpectedReturnValue) {
    std::cerr << "Return value from UnimplementedMethod is expected to be " << kExpectedReturnValue
              << ", "
              << "but got " << ret << std::endl;
    return false;
  }

  return true;
}

}  // namespace client
}  // namespace tests
}  // namespace aidl
}  // namespace android
