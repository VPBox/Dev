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

#ifndef CHRE_UTIL_TOOLCHAIN_H_
#define CHRE_UTIL_TOOLCHAIN_H_

/**
 * @file
 * Compiler/build toolchain-specific macros used by nanoapps and the CHRE system
 */

#if defined(__GNUC__) || defined(__clang__)

// Promoting float to double is a necessary part of vararg-based logging, so
// ignore those warnings when logging via chre/util/nanoapp/log.h
#define CHRE_LOG_PREAMBLE \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Wdouble-promotion\"")

#define CHRE_LOG_EPILOGUE \
    _Pragma("GCC diagnostic pop")

// Enable printf-style compiler warnings for mismatched format string and args
#define CHRE_PRINTF_ATTR(formatPos, argStart) \
    __attribute__((format(printf, formatPos, argStart)))

#else  // if !defined(__GNUC__) && !defined(__clang__)

#error Need to add support for new compiler

#endif

#endif  // CHRE_UTIL_TOOLCHAIN_H_
