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

#include "utils.rsh"

#define DEFINE_TEST(T)                                      \
  T g_##T=7, notUsed_##T;                                   \
  /* Test Allocations in between other global variables */  \
  rs_allocation allocNotUsed_##T;                           \
  /* Test statics in between other global variables */      \
  static T static_##T;                                      \
  /* Test taking addresses of globals */                    \
  static T *ptr1_##T;                                       \
  static T *ptr2_##T;                                       \
  T __attribute__((kernel)) T##_kernel1(T in) {             \
    return g_##T;                                           \
  }                                                         \
  T __attribute__((kernel)) T##_kernel2(T in) {             \
    return *ptr1_##T + *ptr2_##T;                           \
  }                                                         \
  void T##_test_init(rs_allocation a) {                     \
    /* Test static local variable */                        \
    rsForEach(T##_kernel1, a, a);                           \
    T actual = get_##T(a, 19);                              \
    if (!checkEq((T)(7), actual)) {                         \
      rsDebug("failed "#T, actual);                         \
      rsSendToClientBlocking(RS_MSG_TEST_FAILED);           \
      return;                                               \
    }                                                       \
    rsDebug("succeeded "#T, actual);                        \
    rsSendToClientBlocking(RS_MSG_TEST_PASSED);             \
  }                                                         \
  void T##_test(rs_allocation a, T expected) {              \
    /* Test static local variable */                        \
    static T localStatic;                                   \
    localStatic = g_##T;                                    \
    static_##T = localStatic;                               \
    ptr1_##T = &g_##T;                                      \
    ptr2_##T = &static_##T;                                 \
    rsForEach(T##_kernel2, a, a);                           \
    T actual = get_##T(a, 19);                              \
    if (!checkEq(expected * 2, actual)) {                   \
      rsDebug("failed "#T, actual);                         \
      rsSendToClientBlocking(RS_MSG_TEST_FAILED);           \
      return;                                               \
    }                                                       \
    rsDebug("succeeded "#T, actual);                        \
    rsSendToClientBlocking(RS_MSG_TEST_PASSED);             \
  }

