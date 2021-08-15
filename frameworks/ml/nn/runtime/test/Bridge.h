/*
 * Copyright (C) 2017 The Android Open Source Project
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

// There are name clashes between NeuralNetworksWrapper.h and
// HalInterfaces.h.  Many tests include the former; many internal
// header files (nn/runtime/*.h) include the latter.  This file
// contains a few utilities for tests to call that trampoline to the
// internal headers.

#ifndef ANDROID_ML_NN_RUNTIME_TEST_BRIDGE_H
#define ANDROID_ML_NN_RUNTIME_TEST_BRIDGE_H

#include <iostream>

namespace android {
namespace nn {

class ModelBuilder;

namespace bridge_tests {

void graphDump(const char* name, const ModelBuilder* model, std::ostream* outStream = &std::cout);

}  // namespace bridge_tests

}  // namespace nn
}  // namespace android

#endif  // ANDROID_ML_NN_RUNTIME_TEST_BRIDGE_H
