/*
 * Copyright 2018 The Android Open Source Project
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

#ifndef ANDROID_AUDIO_UTILS_VARIADIC_UTILS_H
#define ANDROID_AUDIO_UTILS_VARIADIC_UTILS_H

#include <array>
#include <cmath> // for std::sqrt
#include <ostream>
#include <tuple>
#include <utility>

namespace android {
namespace audio_utils {

/**
 * We provide operator overloading for variadic math and printing.
 *
 * A object allowed for variadic operation requires the following:
 *   1) variadic constructor
 *   2) support std::get<>
 *   3) support std::tuple_size<>
 *   4) support std::tuple_element<>
 *
 * Examples of common variadic classes: std::pair, std::tuple, std::array.
 *
 * User defined variadic classes will need to create overloaded functions for
 * std::get, std::tuple_size, std::tuple_element.
 *
 * Overloads and functions always check whether the type of the argument is
 * variadic to prevent false application, unless parameters include a variadic index sequence.
 * This makes shorter function names safe from name collision as well.
 */

template <typename T, template <typename...> class C>
struct is_template : std::false_type {};
template <template <typename...> class C, typename... args>
struct is_template<C<args...>, C> : std::true_type {};

template <typename T> using is_tuple = is_template<std::decay_t<T>, std::tuple>;
template <typename T> using is_pair = is_template<std::decay_t<T>, std::pair>;

/* is_array<T>::value , different than std::is_array<T>::value */
template <typename T>
struct is_array_impl : std::false_type {};
template <typename T, size_t N>
struct is_array_impl<std::array<T, N>> : std::true_type {};
template <typename T>
struct is_array : is_array_impl<std::decay_t<T>> {};

/* is_variadic<T>::value is true if T supports std::tuple_size<T> */
struct is_variadic_impl {
    // SFINAE test(0) prefers this if std::tuple_size<T>::value exists
    template <typename T> static int test(int, int[std::tuple_size<T>::value] = nullptr);
    template <typename T> static bool test(...);
};

template <typename T>
struct is_variadic : std::integral_constant<bool,
    std::is_same<decltype(is_variadic_impl::test<std::decay_t<T>>(0)), int>::value> {};

/**
 * We allow variadic OP variadic or variadic OP scalar or scalar OP variadic
 *
 * where OP is +, -, *, /.
 *
 * Deep operations are possible on nested variadics, for example:
 *
 * std::cout << std::make_pair(0, std::make_pair(1 , 2)) + 2;
 * -> (2, (3, 4))
 *
 */

#define MAKE_VARIADIC_BINARY_OPERATOR(OPERATOR, OPERATOR_NAME) \
template <typename T1, typename T2, std::size_t... I> \
constexpr auto OPERATOR_NAME##_VS(const T1& t1, const T2& t2, std::index_sequence<I...>); \
template <typename T1, typename T2, std::size_t... I> \
constexpr auto OPERATOR_NAME##_VV(const T1& t1, const T2& t2, std::index_sequence<I...>); \
template <typename T1, typename T2, \
         std::enable_if_t<is_variadic<T1>::value && !is_variadic<T2>::value, int> = 0> \
constexpr auto operator OPERATOR(const T1& t1, const T2& t2) { \
    return OPERATOR_NAME##_VS(t1, t2, std::make_index_sequence<std::tuple_size<T1>::value>{}); \
} \
template <typename T1, typename T2, \
         std::enable_if_t<!is_variadic<T1>::value && is_variadic<T2>::value, int> = 0> \
constexpr auto operator OPERATOR(const T1& t1, const T2& t2) { \
    return OPERATOR_NAME##_VS( \
            t2, t1, std::make_index_sequence<std::tuple_size<T2>::value>{}); \
} \
template <typename T1, typename T2, \
         std::enable_if_t<is_variadic<T1>::value && is_variadic<T2>::value, int> = 0> \
constexpr auto operator OPERATOR(const T1& t1,  const T2& t2) { \
    static_assert(std::tuple_size<T1>::value == std::tuple_size<T2>::value, \
                  #OPERATOR_NAME " size must match"); \
    return OPERATOR_NAME##_VV(t1, t2, std::make_index_sequence<std::tuple_size<T1>::value>{}); \
} \
template <typename T1, typename T2, \
         std::enable_if_t<is_variadic<T1>::value && !is_variadic<T2>::value, int> = 0> \
constexpr auto operator OPERATOR##=(T1& t1, const T2& t2) { \
    t1 = OPERATOR_NAME##_VS(t1, t2, std::make_index_sequence<std::tuple_size<T1>::value>{}); \
    return t1; \
} \
template <typename T1, typename T2, \
         std::enable_if_t<is_variadic<T1>::value && is_variadic<T2>::value, int> = 0> \
constexpr auto operator OPERATOR##=(T1& t1,  const T2& t2) { \
    static_assert(std::tuple_size<T1>::value == std::tuple_size<T2>::value, \
                  #OPERATOR_NAME " size must match"); \
    t1 = OPERATOR_NAME##_VV(t1, t2, std::make_index_sequence<std::tuple_size<T1>::value>{}); \
    return t1; \
} \
template <typename T1, typename T2, std::size_t... I> \
constexpr auto OPERATOR_NAME##_VS(const T1& t1, const T2& t2, std::index_sequence<I...>) { \
    return T1{std::get<I>(t1) OPERATOR t2...}; \
} \
template <typename T1, typename T2, std::size_t... I> \
constexpr auto OPERATOR_NAME##_VV(const T1& t1, const T2& t2, std::index_sequence<I...>) { \
    return T1{std::get<I>(t1) OPERATOR std::get<I>(t2)...}; \
} \

MAKE_VARIADIC_BINARY_OPERATOR(+, plus)
MAKE_VARIADIC_BINARY_OPERATOR(-, minus)
MAKE_VARIADIC_BINARY_OPERATOR(*, multiplies)
MAKE_VARIADIC_BINARY_OPERATOR(/, divides)

#undef MAKE_VARIADIC_BINARY_OPERATOR

/**
 * We overload ostream operators for stringification or printing.
 *
 * Nested variadics are properly printed.
 *
 * std::cout << std::make_pair(1, 2) << std::make_tuple(1., 2., 3.)
 *           << std::make_pair(0, std::make_pair(3, 4));
 */

// forward declaration of helper
template <class charT, class traits, class T, std::size_t... I>
auto& ostream_variadic(
        std::basic_ostream<charT, traits>& os,
        const T& t,
        std::index_sequence<I...>);

// operator overload
template <class charT, class traits, class T, \
         std::enable_if_t<is_variadic<T>::value, int> = 0> \
auto& operator<<(std::basic_ostream<charT, traits>& os, const T& t) { \
    return ostream_variadic(os, t, std::make_index_sequence<std::tuple_size<T>::value>{}); \
}

// helper function (recursively calls <<)
template <class charT, class traits, class T, std::size_t... I>
auto& ostream_variadic(
        std::basic_ostream<charT, traits>& os,
        const T& t,
        std::index_sequence<I...>) {
    os << "(";
    // ((os << (I == 0 ? "" : ", ") << std::get<I>(t)), ...); is C++17
    int dummy[] __unused = { (os << (I == 0 ? "" : ", ") << std::get<I>(t), 0) ... };
    return os << ")";
}

/**
 * We have a fold operator which converts a variadic to a scalar using
 * a binary operator.
 *
 * Following standard binary operator convention, it is a left-associative fold.
 *
 * Example:
 *
 * fold(std::plus<>(), std::make_pair(1, 2));
 *
 * This is a shallow operation - does not recurse through nested variadics.
 */

// helper
template <size_t index, typename Op, typename T,
          std::enable_if_t<index == 0 && is_variadic<T>::value, int> = 0>
constexpr auto fold(Op&& op __unused, T&& t) {
    return std::get<index>(std::forward<T>(t));
}

// helper
template <size_t index, typename Op, typename T,
          std::enable_if_t<(index > 0) && is_variadic<T>::value, int> = 0>
constexpr auto fold(Op&& op, T&& t) {
    return op(fold<index - 1>(std::forward<Op>(op), t), std::get<index>(std::forward<T>(t)));
}

// variadic
template <typename Op, typename T,
          std::enable_if_t<is_variadic<T>::value, int> = 0>
constexpr auto fold(Op&& op, T&& t)  {
    return fold<std::tuple_size<T>::value - 1>(std::forward<Op>(op), std::forward<T>(t));
}


/**
 * tupleOp returns a tuple resulting from an element-wise operation on two variadics.
 *
 * the type of each tuple element depends on the return value of the op.
 *
 * This is a shallow operation - does not recurse through nested variadics.
 */
// helper
template <typename Op, typename T1, typename T2, std::size_t... I,
         std::enable_if_t<is_variadic<T1>::value && is_variadic<T2>::value, int> = 0>
constexpr auto tupleOp(Op&& op, T1&& t1, T2&& t2, std::index_sequence<I...>) {
    return std::make_tuple(
            op(std::get<I>(std::forward<T1>(t1)), std::get<I>(std::forward<T2>(t2)))...);
}

// variadic
template <typename Op, typename T1, typename T2,
         std::enable_if_t<is_variadic<T1>::value && is_variadic<T2>::value, int> = 0>
constexpr auto tupleOp(Op&& op, T1&& t1, T2&& t2) {
    static_assert(std::tuple_size<std::remove_reference_t<T1>>::value
            == std::tuple_size<std::remove_reference_t<T2>>::value,
            "tuple size must match");
    return tupleOp(std::forward<Op>(op),
                   std::forward<T1>(t1),
                   std::forward<T2>(t2),
                   std::make_index_sequence<
                           std::tuple_size<std::remove_reference_t<T1>>::value>());
}

/**
 *  equivalent compares two variadics OR scalars
 *
 * equivalent(std::make_pair(1, 2), std::make_tuple(1, 2)) == true
 *
 * Does a deep compare through nested variadics.
 *
 * Does not do short-circuit evaluation.
 * C++17 will allow for a better implementation of this.
 */

// scalar
template <typename T1, typename T2,
          std::enable_if_t<!is_variadic<T1>::value && !is_variadic<T2>::value, int> = 0>
constexpr bool equivalent(const T1& t1, const T2& t2) {
    return t1 == t2;
}

// variadic / scalar mismatch overload
template <typename T1, typename T2,
          std::enable_if_t<is_variadic<T1>::value != is_variadic<T2>::value, int> = 0>
constexpr bool equivalent(const T1& t1 __unused, const T2& t2 __unused) {
    return false;
}

// variadic
template <typename T1, typename T2,
          std::enable_if_t<is_variadic<T1>::value && is_variadic<T2>::value, int> = 0>
constexpr bool equivalent(const T1& t1, const T2& t2) {
    return std::tuple_size<T1>::value == std::tuple_size<T2>::value
        && fold([](const bool& v1, const bool& v2) { return v1 && v2; },
                tupleOp([](const auto &v1, const auto &v2) { return equivalent(v1, v2); },
                          t1, t2));
}

/**
 *  The innerProduct is the dot product of two 1D variadics.
 *
 * innerProduct(std::make_pair(1, 2), std::make_pair(3, 4)) == 11
 */

template <typename T1, typename T2,
          std::enable_if_t<is_variadic<T1>::value && is_variadic<T2>::value, int> = 0>
constexpr auto innerProduct(const T1& t1, const T2& t2) {
    return fold(std::plus<>{}, t1 * t2);
}

/**
 * The outerProduct is the tensor product of two 1D variadics.
 *
 * This only returns tuples, regardless of the input.
 *
 * outerProduct(std::make_tuple(1, 2), std::make_tuple(1, 2)) ==
 * std::make_tuple(1, 2, 2, 4);
 *
 */

// helper
template <typename T1, typename T2, std::size_t... I>
constexpr auto outerProduct(const T1& t1, const T2& t2, std::index_sequence<I...>)  {
    return std::tuple_cat(std::get<I>(t1) * t2 ...);
}

// variadic
template <typename T1, typename T2,
          std::enable_if_t<is_variadic<T1>::value && is_variadic<T2>::value, int> = 0>
constexpr auto outerProduct(const T1& t1, const T2& t2) {
    return outerProduct(t1, t2, std::make_index_sequence<std::tuple_size<T1>::value>());
}

/**
 * tail_variadic returns the tail offset by a template size_t Offset
 * of a variadic object. It always returns a tuple.
 */

// helper
template <size_t Offset, typename T, std::size_t... I>
constexpr auto tail_variadic(T&& t, std::index_sequence<I...>) {
    return std::make_tuple(std::get<I + Offset>(std::forward<T>(t))...);  // force a tuple here
}

// variadic
template <size_t Offset, typename T,
          std::enable_if_t<is_variadic<T>::value, int> = 0>
constexpr auto tail_variadic(T&& t) {
    return tail_variadic<Offset>(
           std::forward<T>(t),
           std::make_index_sequence<std::tuple_size<
                   std::remove_reference_t<T>>::value - Offset>());
}

/**
 * The outerProduct_UT is the tensor product of two identical length 1D variadics,
 * but only the upper triangular portion, eliminating the symmetric lower triangular
 * half.  This is useful for the outerProduct of two parallel variadics.
 *
 * This only returns tuples, regardless of the input.
 *
 * outerProduct_UT(std::make_tuple(1, 2, 3), std::make_tuple(1, 2, 3)) ==
 * std::make_tuple(1, 2, 3, 4, 6, 9);
 */

// helper
template <typename T1, typename T2, std::size_t... I>
constexpr auto outerProduct_UT(const T1& t1, const T2& t2, std::index_sequence<I...>)  {
    return std::tuple_cat(std::get<I>(t1) * tail_variadic<I>(t2) ...);
}

// variadic
template <typename T1, typename T2,
          std::enable_if_t<is_variadic<T1>::value && is_variadic<T2>::value, int> = 0>
constexpr auto outerProduct_UT(const T1& t1, const T2& t2) {
    static_assert(std::tuple_size<T1>::value == std::tuple_size<T2>::value,
                  "tuple size must match");
    return outerProduct_UT(t1, t2, std::make_index_sequence<std::tuple_size<T1>::value>());
}

/**
 * to_array does a conversion of any variadic to a std::array whose element type is
 * the input variadic's first tuple element and whose size is the tuple size.
 * This is a shallow operation and does not work on nested variadics.
 */

 // helper
template <typename T, std::size_t...I>
constexpr auto to_array(const T &t, std::index_sequence<I...>) {
    return std::array<std::tuple_element_t<0, T>, std::tuple_size<T>::value>{std::get<I>(t)...};
}

// variadic
template <typename T>
constexpr auto to_array(const T &t) {
     return to_array(t, std::make_index_sequence<std::tuple_size<T>::value>());
}

/**
 * We create functor versions of the inner and outer products to
 * pass in as a type argument to a template.  A tuple and an array
 * return variant are provided.
 *
 * See related: std::function<>.
 */

template <typename T>
struct innerProduct_scalar {
    constexpr auto operator()(const T &lhs, const T &rhs) const {
        return innerProduct(lhs, rhs);
    }
};

template <typename T>
struct outerProduct_tuple {
    constexpr auto operator()(const T &lhs, const T &rhs) const {
        return outerProduct(lhs, rhs);
    }
};

template <typename T>
struct outerProduct_UT_tuple {
    constexpr auto operator()(const T &lhs, const T &rhs) const {
        return outerProduct_UT(lhs, rhs);
    }
};

template <typename T>
struct outerProduct_array {
    constexpr auto operator()(const T &lhs, const T &rhs) const {
        return to_array(outerProduct(lhs, rhs));
    }
};

template <typename T>
struct outerProduct_UT_array {
    constexpr auto operator()(const T &lhs, const T &rhs) const {
        return to_array(outerProduct_UT(lhs, rhs));
    }
};

/**
 * for_each is used to apply an operation to each element of a variadic OR scalar object.
 *
 * auto t = std:make_tuple(1, 2);
 * for_each([](int &x) { ++x; }, t);
 *
 * Related: std::for_each<>
 * Note difference from std::apply, which forwards tuple elements as arguments to a function.
 */

// helper
template <typename T, typename Op, std::size_t... I >
constexpr void for_each(T& t, Op op, std::index_sequence<I...>) {
    int dummy[] __unused = {(op(std::get<I>(t)), 0)...};
}

// variadic
template <typename T, typename Op,
          std::enable_if_t<is_variadic<T>::value, int> = 0>
constexpr void for_each(T& t, Op op) {
    for_each(t, op, std::make_index_sequence<std::tuple_size<T>::value>());
}

// scalar version applies if not a class, rather than not a variadic
template <typename T, typename Op,
          std::enable_if_t<!std::is_class<T>::value, int> = 0>
constexpr void for_each(T& t, Op op) {
    op(t);
}

/**
 * We make variants of the unary function std::sqrt()
 * and the binary std::min(), std::max() to work on variadics.
 *
 * These are shallow operations and do not work on nested variadics.
 *
 * TODO: update to variadic function application for C++17
 * with built-in std::apply, std::invoke, and constexpr lambdas.
 *
 */

#define MAKE_VARIADIC_STD_UNARY_FUNCTION(FUNCTION) \
template <typename T, \
          std::enable_if_t<!is_variadic<T>::value, int> = 0> \
constexpr auto FUNCTION(const T &t) { \
    return std::FUNCTION(t); \
} \
template <typename T, std::size_t... I > \
constexpr auto FUNCTION(const T &t, std::index_sequence<I...>) { \
    return T{audio_utils::FUNCTION(std::get<I>(t))...}; \
} \
template <typename T, \
          std::enable_if_t<is_variadic<T>::value, int> = 0> \
constexpr auto FUNCTION(const T& t) { \
    return audio_utils::FUNCTION(t, std::make_index_sequence<std::tuple_size<T>::value>()); \
}

MAKE_VARIADIC_STD_UNARY_FUNCTION(sqrt);

#undef MAKE_VARIADIC_STD_UNARY_FUNCTION

#define MAKE_VARIADIC_STD_BINARY_FUNCTION(FUNCTION) \
template <typename T1, typename T2, \
          std::enable_if_t<!is_variadic<T1>::value && !is_variadic<T2>::value, int> = 0> \
constexpr auto FUNCTION(const T1 &t1, const T2 &t2) { \
    return std::FUNCTION(t1, t2); \
} \
template <typename T1, typename T2, std::size_t... I > \
constexpr auto FUNCTION(const T1 &t1, const T2 &t2, std::index_sequence<I...>) { \
    return T1{audio_utils::FUNCTION(std::get<I>(t1), std::get<I>(t2))...}; \
} \
template <typename T1, typename T2, \
          std::enable_if_t<is_variadic<T1>::value && is_variadic<T2>::value, int> = 0> \
constexpr auto FUNCTION(const T1 &t1, const T2 &t2) { \
    static_assert(std::tuple_size<T1>::value == std::tuple_size<T2>::value, \
                  #FUNCTION " size must match"); \
    return audio_utils::FUNCTION( \
            t1, t2, std::make_index_sequence<std::tuple_size<T1>::value>()); \
}

MAKE_VARIADIC_STD_BINARY_FUNCTION(min);
MAKE_VARIADIC_STD_BINARY_FUNCTION(max);

/* is_iterator<T>::value is true if T supports std::iterator_traits<T>

   TODO: poor resolution on iterator type, prefer emulating hidden STL templates
   __is_input_iterator<>
   __is_forward_iterator<>
   ...
 */
 // helper
struct is_iterator_impl {
    // SFINAE test(0) preferred if iterator traits
    template <typename T,
              typename = typename std::iterator_traits<T>::difference_type,
              typename = typename std::iterator_traits<T>::value_type,
              typename = typename std::iterator_traits<T>::pointer,
              typename = typename std::iterator_traits<T>::iterator_category>
              static int test(int);
    template <typename T> static bool test(...);
};

template <typename T>
struct is_iterator : std::integral_constant<bool,
    std::is_same<decltype(is_iterator_impl::test<std::decay_t<T>>(0)), int>::value> {};


} // namespace audio_utils
} // namespace android

#endif // !ANDROID_AUDIO_UTILS_VARIADIC_UTILS_H
