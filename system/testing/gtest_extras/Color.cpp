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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <gtest/gtest.h>

#include "Color.h"

namespace android {
namespace gtest_extras {

static bool ShouldUseColor() {
  const auto& gtest_color = ::testing::GTEST_FLAG(color);
  if (gtest_color == "yes" || gtest_color == "true" || gtest_color == "t") {
    return true;
  }
  if (gtest_color != "auto") {
    return false;
  }

  bool stdout_is_tty = isatty(STDOUT_FILENO) != 0;
  if (!stdout_is_tty) {
    return false;
  }

  std::string color_term;
  const char* const color_term_env = getenv("COLORTERM");
  if (color_term_env != nullptr) {
    color_term = color_term_env;
  }
  std::string term;
  const char* const term_env = getenv("TERM");
  if (term_env != nullptr) {
    term = term_env;
  }
  return !color_term.empty() || term == "xterm" || term == "xterm-color" ||
         term == "xterm-256color" || term == "screen" || term == "screen-256color" ||
         term == "tmux" || term == "tmux-256color" || term == "rxvt-unicode" ||
         term == "rxvt-unicode-256color" || term == "linux" || term == "cygwing";
}

void ColoredPrintf(const char* color, const char* fmt, ...) {
  static const bool use_color = ShouldUseColor();

  va_list args;
  va_start(args, fmt);

  if (!use_color) {
    vprintf(fmt, args);
  } else {
    printf("%s", color);
    vprintf(fmt, args);
    printf("%s", COLOR_RESET);
  }

  va_end(args);
}

}  // namespace gtest_extras
}  // namespace android
