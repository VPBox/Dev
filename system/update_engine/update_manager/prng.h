//
// Copyright (C) 2014 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_PRNG_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_PRNG_H_

#include <random>

#include <base/logging.h>

namespace chromeos_update_manager {

// A thread-safe, unsecure, 32-bit pseudo-random number generator based on
// std::mt19937.
class PRNG {
 public:
  // Initializes the generator with the passed |seed| value.
  explicit PRNG(uint32_t seed) : gen_(seed) {}

  // Returns a random unsigned 32-bit integer.
  uint32_t Rand() { return gen_(); }

  // Returns a random integer uniformly distributed in the range [min, max].
  int RandMinMax(int min, int max) {
    DCHECK_LE(min, max);
    return std::uniform_int_distribution<>(min, max)(gen_);
  }

 private:
  // A pseudo-random number generator.
  std::mt19937 gen_;

  DISALLOW_COPY_AND_ASSIGN(PRNG);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_PRNG_H_
