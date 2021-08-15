/*
 * Copyright 2019 The Android Open Source Project
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

#pragma once

#include <array>

/* Helper class generating N unique ids, from 0 to N-1 */
template <std::size_t N>
class IdGenerator {
 public:
  static int ALL_USED;

  IdGenerator() : in_use_{} {}

  /* Returns next free id, or ALL_USED if no ids left */
  int GetNext() {
    for (std::size_t i = 0; i < N; i++) {
      if (!in_use_[i]) {
        in_use_[i] = true;
        return i;
      }
    }
    return ALL_USED;
  }

  /* Release given ID */
  void Release(int id) { in_use_[id] = false; }

 private:
  std::array<bool, N> in_use_;
};

template <std::size_t N>
int IdGenerator<N>::ALL_USED = -1;