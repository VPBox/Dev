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

#ifndef NVRAM_MESSAGES_TYPE_TRAITS_H_
#define NVRAM_MESSAGES_TYPE_TRAITS_H_

extern "C" {
#include <stddef.h>
}

// A subset of utilities similar to what is available in <type_traits>. We have
// our own standalone version here since not all target platforms have a full
// C++ standard library.

namespace nvram {

template <typename T>
struct remove_const {
  using Type = T;
};
template <typename T>
struct remove_const<const T> {
  using Type = T;
};

template <typename T>
struct remove_reference {
  using Type = T;
};
template <typename T>
struct remove_reference<T&> {
  using Type = T;
};
template <typename T>
struct remove_reference<T&&> {
  using Type = T;
};

template <bool value, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> {
  using Type = T;
};

template <typename T, T const_value>
struct integral_constant {
  static constexpr T value = const_value;
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template <typename T>
T declval();

template <size_t... index>
struct index_sequence {};

template <size_t size, size_t... indices>
struct make_index_sequence_builder {
  using Type = typename make_index_sequence_builder<size - 1,
                                                    size - 1,
                                                    indices...>::Type;
};

template <size_t... indices>
struct make_index_sequence_builder<0, indices...> {
  using Type = index_sequence<indices...>;
};

template <size_t size>
constexpr typename make_index_sequence_builder<size>::Type
make_index_sequence(){
  return typename make_index_sequence_builder<size>::Type();
};

}  // namespace nvram

#endif  // NVRAM_MESSAGES_TYPE_TRAITS_H_
