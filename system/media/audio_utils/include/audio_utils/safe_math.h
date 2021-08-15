/*
 * Copyright (C) 2019 The Android Open Source Project
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

namespace android::audio_utils {

// safe_sub_overflow is used ensure that subtraction occurs in the same native
// type with proper 2's complement overflow.  Without calling this function, it
// is possible, for example, that optimizing compilers may elect to treat 32 bit
// subtraction as 64 bit subtraction when storing into a 64 bit destination as
// integer overflow is technically undefined.
template <typename T, typename U,
          typename = std::enable_if_t<
              std::is_same<std::decay_t<T>, std::decay_t<U>>{}>>
// ensure arguments are same type (ignoring volatile, which is used in cblk
// variables).
auto safe_sub_overflow(const T& a, const U& b) {
  std::decay_t<T> result;
  (void)__builtin_sub_overflow(a, b, &result);
  // note if __builtin_sub_overflow returns true, an overflow occurred.
  return result;
}

// similar to safe_sub_overflow but for add operator.
template <typename T, typename U,
          typename = std::enable_if_t<
              std::is_same<std::decay_t<T>, std::decay_t<U>>{}>>
// ensure arguments are same type (ignoring volatile, which is used in cblk
// variables).
auto safe_add_overflow(const T& a, const U& b) {
  std::decay_t<T> result;
  (void)__builtin_add_overflow(a, b, &result);
  // note if __builtin_add_overflow returns true, an overflow occurred.
  return result;
}

} // namespace android::audio_utils
