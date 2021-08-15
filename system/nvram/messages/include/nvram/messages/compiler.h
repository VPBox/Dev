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

#ifndef NVRAM_MESSAGES_COMPILER_H_
#define NVRAM_MESSAGES_COMPILER_H_

extern "C" {
#include <stdlib.h>
}

#if defined(__GNUC__) || defined(__clang__)
#define NVRAM_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define NVRAM_WARN_UNUSED_RESULT
#endif

// Check a condition, abort if it doesn't hold.
#define NVRAM_CHECK(cond) if (!(cond)) abort()

// The noinline attribute is understood both by clang and GCC.
#define NVRAM_NOINLINE __attribute__((noinline))

// The visibility attribute works both with clang and GCC.
#define NVRAM_EXPORT __attribute__((visibility("default")))

#endif  // NVRAM_MESSAGES_COMPILER_H_
