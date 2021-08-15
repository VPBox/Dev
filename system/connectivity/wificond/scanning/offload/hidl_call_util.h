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

// This file is modified from
// hardware/interfaces/wifi/1.0/vts/functional/wifi_hidl_call_util.h

#pragma once

#include <android-base/logging.h>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace {
namespace detail {
template <typename>
struct functionArgSaver;

// Provides a std::function that takes one argument, and a buffer
// wherein the function will store its argument. The buffer has
// the same type as the argument, but with const and reference
// modifiers removed.
template <typename ArgT>
struct functionArgSaver<std::function<void(ArgT)>> final {
  using StorageT = typename std::remove_const<
      typename std::remove_reference<ArgT>::type>::type;

  std::function<void(ArgT)> saveArgs = [this](ArgT arg) {
    this->saved_values = arg;
  };

  StorageT saved_values;
};

// Provides a std::function that takes two arguments, and a buffer
// wherein the function will store its arguments. The buffer is a
// std::pair, whose elements have the same types as the arguments
// (but with const and reference modifiers removed).
template <typename Arg1T, typename Arg2T>
struct functionArgSaver<std::function<void(Arg1T, Arg2T)>> final {
  using StorageT =
      std::pair<typename std::remove_const<
                    typename std::remove_reference<Arg1T>::type>::type,
                typename std::remove_const<
                    typename std::remove_reference<Arg2T>::type>::type>;

  std::function<void(Arg1T, Arg2T)> saveArgs = [this](Arg1T arg1, Arg2T arg2) {
    this->saved_values = {arg1, arg2};
  };

  StorageT saved_values;
};

// Provides a std::function that takes three or more arguments, and a
// buffer wherein the function will store its arguments. The buffer is a
// std::tuple whose elements have the same types as the arguments (but
// with const and reference modifiers removed).
template <typename... ArgT>
struct functionArgSaver<std::function<void(ArgT...)>> final {
  using StorageT = std::tuple<typename std::remove_const<
      typename std::remove_reference<ArgT>::type>::type...>;

  std::function<void(ArgT...)> saveArgs = [this](ArgT... arg) {
    this->saved_values = {arg...};
  };

  StorageT saved_values;
};

// Invokes |method| on |object|, providing |method| a CallbackT as the
// final argument. Returns a copy of the parameters that |method| provided
// to CallbackT. (The parameters are returned by value.)
template <typename CallbackT, typename MethodT, typename ObjectT,
          typename... ArgT>
std::pair<typename functionArgSaver<CallbackT>::StorageT, bool> invokeMethod(
    MethodT method, ObjectT object, ArgT&&... methodArg) {
  functionArgSaver<CallbackT> result_buffer;
  const auto& res = ((*object).*method)(std::forward<ArgT>(methodArg)...,
                                        result_buffer.saveArgs);
  bool transportStatus = true;
  if (!res.isOk()) {
    LOG(ERROR) << " Transport failed " << res.description();
    transportStatus = false;
  }
  return std::make_pair(result_buffer.saved_values, transportStatus);
}
}  // namespace detail
}  // namespace

// Invokes |method| on |strong_pointer|, passing provided arguments through to
// |method|.
//
// Returns either:
// - A copy of the result callback parameter (for callbacks with a single
//   parameter), OR
// - A pair containing a copy of the result callback parameters (for callbacks
//   with two parameters), OR
// - A tuple containing a copy of the result callback paramters (for callbacks
//   with three or more parameters).
//
// Example usage:
//   EXPECT_EQ(WifiStatusCode::SUCCESS,
//       HIDL_INVOKE(strong_pointer, methodReturningWifiStatus).code);
//   EXPECT_EQ(WifiStatusCode::SUCCESS,
//       HIDL_INVOKE(strong_pointer, methodReturningWifiStatusAndOneMore)
//         .first.code);
//   EXPECT_EQ(WifiStatusCode::SUCCESS, std::get<0>(
//       HIDL_INVOKE(strong_pointer, methodReturningWifiStatusAndTwoMore))
//         .code);
#define HIDL_INVOKE(strong_pointer, method, ...)                            \
  (detail::invokeMethod<                                                    \
      std::remove_reference<decltype(*strong_pointer)>::type::method##_cb>( \
      &std::remove_reference<decltype(*strong_pointer)>::type::method,      \
      strong_pointer, ##__VA_ARGS__))
