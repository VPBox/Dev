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

#include <stdint.h>

#include <chrono>

#include "NanoTime.h"

namespace android {
namespace gtest_extras {

uint64_t NanoTime() {
  std::chrono::nanoseconds duration(std::chrono::steady_clock::now().time_since_epoch());
  return static_cast<uint64_t>(duration.count());
}

}  // namespace gtest_extras
}  // namespace android
