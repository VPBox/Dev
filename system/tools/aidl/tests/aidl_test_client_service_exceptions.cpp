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

#include "aidl_test_client_service_exceptions.h"

#include <iostream>

#include "binder/Status.h"

using android::binder::Status;
using std::cout;
using std::endl;

namespace android {
namespace aidl {
namespace tests {
namespace client {

bool ConfirmServiceSpecificExceptions(const sp<ITestService>& s) {
  cout << "Confirming application exceptions work" << endl;

  for (int32_t i = -1; i < 2; ++i) {
    Status status = s->ThrowServiceException(i);
    if (status.exceptionCode() != Status::EX_SERVICE_SPECIFIC ||
        status.serviceSpecificErrorCode() != i) {
      return false;
    }
  }

  return true;
}

}  // namespace client
}  // namespace tests
}  // namespace aidl
}  // namespace android
