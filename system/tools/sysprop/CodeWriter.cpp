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

#include "CodeWriter.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "sysprop_gen"

#include <cstdarg>
#include <cstdio>
#include <vector>

#include <android-base/logging.h>

CodeWriter::CodeWriter(std::string indent) : indent_(std::move(indent)) {
}

void CodeWriter::Write(const char* format, ...) {
  va_list ap, apc;
  va_start(ap, format);
  va_copy(apc, ap);

  int size = std::vsnprintf(nullptr, 0, format, ap);
  va_end(ap);

  if (size < 0) {
    va_end(apc);
    PLOG(FATAL) << "vsnprintf failed";
  }

  std::vector<char> buf(size + 1);
  if (std::vsnprintf(buf.data(), size + 1, format, apc) < 0) {
    va_end(apc);
    PLOG(FATAL) << "vsnprintf failed";
  }
  va_end(apc);

  for (int i = 0; i < size; ++i) {
    char ch = buf[i];
    if (ch == '\n') {
      start_of_line_ = true;
    } else {
      if (start_of_line_) {
        for (int j = 0; j < indent_level_; ++j) {
          code_ += indent_;
        }
        start_of_line_ = false;
      }
    }
    code_.push_back(ch);
  }
}

void CodeWriter::Indent() {
  ++indent_level_;
}

void CodeWriter::Dedent() {
  if (indent_level_ == 0) {
    LOG(FATAL) << "Dedent failed: indent level is already 0";
  }
  --indent_level_;
}
