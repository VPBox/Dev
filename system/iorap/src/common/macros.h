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

#ifndef IORAP_COMMON_MACROS_H_
#define IORAP_COMMON_MACROS_H_

// Expands to the # of arguments passed to the macro.
// For example `IORAP_PP_NARG(a,b,c)` -> 3
// For example `IORAP_PP_NARG(x)` -> 1
//
// The # of arguments must be >0 and <64.
#define IORAP_PP_NARG(...) \
  IORAP_PP_NARG_IMPL(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,)  // NOLINT

// Implementation explanation: Equivalent of:
//   lst = __VA_ARGS__ + 64..1
//   return lst[64]
// The variadic arguments logically shift-off the hardcoded 64..1 list, revealing
// the # of arguments that were shifted.
#define IORAP_PP_NARG_IMPL(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63, N, ...) N

#define IORAP_PP_CONCAT(lhs, rhs) \
  IORAP_PP_CONCAT_IMPL(lhs, rhs)

#define IORAP_PP_CONCAT_IMPL(lhs, rhs) \
  lhs ## rhs

// Expands to call FN around every argument passed it.
//   MAP(f, a1, a2, .., an) -> f(a1) f(a2) f(a3) ... f(an)
#define IORAP_PP_MAP(FN, ...) IORAP_PP_MAP_IMPL(IORAP_PP_NARG(__VA_ARGS__), /*sep=blank*/, FN, __VA_ARGS__)
// Expands to call FN around every argument passed it.
// Every non-first/non-last function call also has a separator prefixed before it.
//   MAP_SEP(f, sep, a1, a2, .., an) -> f(a1) sep() f(a2) sep() f(a3) sep() ... sep() f(an-1) f(an)
#define IORAP_PP_MAP_SEP(FN, sep, ...) IORAP_PP_MAP_IMPL(IORAP_PP_NARG(__VA_ARGS__), FN, sep, __VA_ARGS__)
#define IORAP_PP_MAP_IMPL(N, FN, sep, ...) IORAP_PP_CONCAT(IORAP_PP_MAP_IMPL_, N)(FN, sep, __VA_ARGS__)
#define IORAP_PP_MAP_IMPL_1(FN, sep, a1) FN(a1)
#define IORAP_PP_MAP_IMPL_2(FN, sep, a1, a2) FN(a1) sep() FN(a2)
#define IORAP_PP_MAP_IMPL_3(FN, sep, a1, a2, a3) FN(a1) sep() FN(a2) sep() FN(a3)
#define IORAP_PP_MAP_IMPL_4(FN, sep, a1, a2, a3, a4) FN(a1) sep() FN(a2) sep() FN(a3) sep() FN(a4)

// Deferred comma. Useful with the above MAP function when you need a comma separator.
#define IORAP_PP_COMMA() ,
// All arguments are ignored.
#define IORAP_PP_NOP(...)

#endif  // IORAP_COMMON_MACROS_H_