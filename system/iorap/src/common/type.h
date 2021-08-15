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

#ifndef IORAP_COMMON_TYPE_H
#define IORAP_COMMON_TYPE_H

#include <cstdint>
#include <tuple>

namespace iorap {
namespace introspect {

/*
 * Simple types-as-value abstractions.
 *
 * Allow types to be passed as regular function parameters instead of using 'template' type
 * parameters.
 *
 * This enables the following, more concise, pattern:
 * ----------------------------
 *
 * Traditional metaprogramming with template parameters:
 *
 *   template <typename ... Args>
 *   struct get_num_params {
 *     static constexpr size_t value = sizeof...(Args);
 *   };
 *
 *  typename get_num_params<decltype("hello"), decltype("world")>::value == 2
 *  typename get_num_params<decltype(int), decltype(int), decltype(int), decltype(int)>::value == 4
 *
 * Alternative metaprogramming with values:
 *
 *   constexpr auto get_num_params = [](auto&&... val) { return sizeof...(val); };
 *
 *   get_num_params("hello", "world") == 2
 *   get_num_params(0,0,0,0) == 4
 */

/*
 * A fully instantiated type wrapper.
 *
 * basic_type<T> is intended for overloading functions between different basic_types.
 * type_c<T> is intended for instantiating new type wrappers as a short-hand (and not requiring
 * typename).
 *
 * For basic_type<T> in particular it allows one to overload on basic_type<T> to handle specific
 * types, and there's no requirement that T be constexpr, be default constructible, and no
 * template specializations is necessary.
 *
 *   void foo(basic_type<int>) {
 *     printf("int");
 *   }
 *
 *   template <typename T>
 *   void foo(basic_type<T>) {
 *     printf("everything else");
 *   }
 *
 * as opposed to this verbosity
 *
 *   template <typename T>
 *   struct foo {
 *     void operator() {
 *       printf("everything else");
 *     }
 *   };
 *
 *   template <>
 *   struct foo<int> {
 *     void operator() {
 *       printf("int");
 *     }
 *   };
 *
 * OR this super-hack which works in rare situations
 *
 *   void foo(int) {
 *     printf("int");
 *   }
 *
 *   template <typename T>
 *   void foo(T&&) {
 *     printf("everything else")
 *   }
 *
 * Note that invoking the last foo(T&&) is particularly challenging. declval<T> fails at compilation
 * with a static_assert, so a real value has to be constructed that is immediately discarded.
 */
template <typename T>
struct basic_type {
  using type = T;
};

template <typename T>
struct type_impl {
  struct _ : basic_type<T> { };
};

template <typename T>
using type = basic_type<T>;  // typename type_impl<T>::_;  // subclass of basic_type<T>
// TODO: why doesn't using type_impl::_ work with ADL?

template <typename T>
using type_t = type<T>;

template <typename T>
constexpr auto type_c = type<T>{};

template <auto X>
struct value_constant {
  static constexpr auto value = X;
};

template <int X>
constexpr auto int_c = value_constant<X>{};

template <typename T>
constexpr bool dependent_false_v = false;

// Emit a static_assert(false) if the else branch in an 'if constexpr' is taken.
// Needs a type as the first parameter.
#define STATIC_FAIL(T, msg) static_assert(::iorap::introspect::dependent_false_v<T>, msg)
// Emit a static_assert(false) if an else branch in an 'if constexpr' is taken, used with
// (e.g. auto) values instead of types.
#define STATIC_FAIL_DT(var, msg) STATIC_FAIL(decltype(var), msg)

template <size_t i, typename Tuple, typename F>
static constexpr void for_each_impl(Tuple&& t, F&& f) {
  if constexpr (i == std::tuple_size<std::decay_t<Tuple>>::value) {
    return;
  } else {
    f(std::get<i>(std::forward<Tuple>(t)));
    for_each_impl<i+1>(std::forward<Tuple>(t), std::forward<F>(f));
  }
}

// for each Tuple<a1,a2,...,aN> invoke { f(a1); f(a2); ... ; f(aN); }
template <typename Tuple, typename F>
static constexpr void for_each(Tuple&& t, F&& f) {
  return for_each_impl<0u>(std::forward<Tuple>(t), std::forward<F>(f));
}

// Perfect forwarding for structured binding.
//
// Example:
//   auto&& [a,b] = whatever;
//   return aliasing_forward<T>(a);
template <typename T, typename U>
constexpr decltype(auto) aliasing_forward(U&& val) noexcept {
  if constexpr (std::is_lvalue_reference_v<T>) {
    return val;
  } else {
    return std::move(val);
  }
}


}  // namespace introspect
}  // namespace iorap

#endif  // IORAP_COMMON_TYPE_H
