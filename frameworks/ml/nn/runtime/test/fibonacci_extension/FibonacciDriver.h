/*
 * Copyright (C) 2019 The Android Open Source Project
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

#ifndef ANDROID_FRAMEWORKS_ML_NN_RUNTIME_TEST_FIBONACCI_EXTENSION_FIBONACCI_DRIVER_H
#define ANDROID_FRAMEWORKS_ML_NN_RUNTIME_TEST_FIBONACCI_EXTENSION_FIBONACCI_DRIVER_H

#include "OperationResolver.h"
#include "SampleDriver.h"

namespace android {
namespace nn {
namespace sample_driver {

class FibonacciOperationResolver : public IOperationResolver {
    DISALLOW_COPY_AND_ASSIGN(FibonacciOperationResolver);

   public:
    static const FibonacciOperationResolver* get() {
        static FibonacciOperationResolver instance;
        return &instance;
    }

    const OperationRegistration* findOperation(OperationType operationType) const override;

   private:
    FibonacciOperationResolver() {}
};

// A driver that implements FibonacciExtension.
// The only supported operation is TEST_VENDOR_FIBONACCI.
class FibonacciDriver : public SampleDriver {
   public:
    FibonacciDriver() : SampleDriver(kDriverName, FibonacciOperationResolver::get()) {}
    Return<void> getSupportedExtensions(getSupportedExtensions_cb cb) override;
    Return<void> getCapabilities_1_2(getCapabilities_1_2_cb cb) override;
    Return<void> getSupportedOperations_1_2(const V1_2::Model& model,
                                            getSupportedOperations_1_2_cb cb) override;

    static constexpr char kDriverName[] = "sample-driver-fibonacci-extension";
};

}  // namespace sample_driver
}  // namespace nn
}  // namespace android

#endif  // ANDROID_FRAMEWORKS_ML_NN_RUNTIME_TEST_FIBONACCI_EXTENSION_FIBONACCI_DRIVER_H
