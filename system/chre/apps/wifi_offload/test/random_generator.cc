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

#include "include/random_generator.h"

namespace wifi_offload_test {
namespace {
/* use default seed (vs. random seed) to reproduce the same sequence on
 * different runs */
constexpr bool kUseDefaultSeed = true;
}  // namespace

RandomGenerator::RandomGenerator() {
  if (kUseDefaultSeed) {
    initial_seed_ = random_engine_.default_seed;
  } else {
    /* Used to obtain a seed for the random number engine */
    std::random_device random_device;
    // save the seed for future resets
    initial_seed_ = random_device();
  }

  Reset();
}

void RandomGenerator::Reset() {
  random_engine_.seed(initial_seed_);
  uniform_distribution_.reset();
}

}  // wifi_offload_test namespace
