// Copyright (C) 2018 The Android Open Source Project
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

#include <ostream>

namespace iorap {

// kIsDebugBuild is special.
// It gets to be in the 'iorap' namespace
// so that different modules don't need to qualify it.
#ifndef NDEBUG
static constexpr bool kIsDebugBuild = true;
#else
static constexpr bool kIsDebugBuild = false;
#endif

namespace common {

// TODO: move below code to helpers.
template <typename T, bool>
struct base_if_condition {};

template <typename T>
struct base_if_condition<T, true> : public T {};

template <typename T>
using base_if_debug = base_if_condition<T, kIsDebugBuild>;

namespace detail {
// "if constexpr" doesn't allow us to exclude fields from a struct/class,
// and also "if constexpr" doesn't allow us to reference a field that does not
// exist.
// so we must move everything into a separate base class.
template <bool kIsDebug = kIsDebugBuild>
struct DebugCounterBase {
  constexpr size_t value() const {
    return counter;
  }

  constexpr void set_value(size_t value) {
    counter = value;
  }

  size_t counter{1};  // Don't start with 0.
};

template <>
struct DebugCounterBase<false /*kIsDebug*/> {
  constexpr size_t value() const {
    return 0;
  }

  constexpr void set_value(size_t value) {
  }
};
}  // namespace detail

// This counter does absolutely nothing, the code compiles to no-ops
// when debugging is disabled.
struct DebugCounter : detail::DebugCounterBase<> {
  constexpr DebugCounter& operator++() {
    set_value(value() + 1);
    return *this;
  }

  constexpr DebugCounter operator++(int) {
    DebugCounter now = *this;
    set_value(value() + 1);
    return now;
  }

  constexpr operator size_t() const {
    return value();
  }

  friend std::ostream& operator<<(std::ostream& os, DebugCounter counter);
};

inline std::ostream& operator<<(std::ostream& os, DebugCounter counter) {
  os << counter.value();
  return os;
}

// TODO: refactor DebugCounter and base traits into their own files?

}  // namespace common
}  // namespace iorap
