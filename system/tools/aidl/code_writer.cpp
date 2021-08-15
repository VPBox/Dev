/*
 * Copyright (C) 2015, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "code_writer.h"

#include <stdarg.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <android-base/logging.h>
#include <android-base/stringprintf.h>

namespace android {
namespace aidl {

CodeWriter::CodeWriter(std::unique_ptr<std::ostream> ostream) : ostream_(std::move(ostream)) {}

std::string CodeWriter::ApplyIndent(const std::string& str) {
  std::string output;
  if (!start_of_line_ || str == "\n") {
    output = str;
  } else {
    output = std::string(indent_level_ * 2, ' ') + str;
  }
  start_of_line_ = !output.empty() && output.back() == '\n';
  return output;
}

bool CodeWriter::Write(const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  std::string formatted;
  android::base::StringAppendV(&formatted, format, ap);
  va_end(ap);

  // extract lines. empty line is preserved.
  std::vector<std::string> lines;
  size_t pos = 0;
  while (pos < formatted.size()) {
    size_t line_end = formatted.find('\n', pos);
    if (line_end != std::string::npos) {
      lines.push_back(formatted.substr(pos, (line_end - pos) + 1));
      pos = line_end + 1;
    } else {
      lines.push_back(formatted.substr(pos));
      break;
    }
  }

  std::string indented;
  for (const auto& line : lines) {
    indented.append(ApplyIndent(line));
  }

  (*ostream_) << indented;
  return !ostream_->fail();
}

void CodeWriter::Indent() {
  indent_level_++;
}
void CodeWriter::Dedent() {
  CHECK(indent_level_ > 0);

  indent_level_--;
}

bool CodeWriter::Close() {
  if (ostream_.get()->rdbuf() != std::cout.rdbuf()) {
    // if the steam is for file (not stdout), do the close.
    static_cast<std::fstream*>(ostream_.get())->close();
    return !ostream_->fail();
  }
  return true;
}

CodeWriter& CodeWriter::operator<<(const char* s) {
  Write(s);
  return *this;
}

CodeWriter& CodeWriter::operator<<(const std::string& str) {
  Write(str.c_str());
  return *this;
}

CodeWriterPtr CodeWriter::ForFile(const std::string& filename) {
  std::unique_ptr<std::ostream> stream;
  if (filename == "-") {
    stream = std::unique_ptr<std::ostream>(new std::ostream(std::cout.rdbuf()));
  } else {
    stream = std::unique_ptr<std::ostream>(
        new std::fstream(filename, std::fstream::out | std::fstream::binary));
  }
  return CodeWriterPtr(new CodeWriter(std::move(stream)));
}

CodeWriterPtr CodeWriter::ForString(std::string* buf) {
  // This class is defined inside this static function of CodeWriter
  // in order to have access to private constructor and private member
  // ostream_.
  class StringCodeWriter : public CodeWriter {
   public:
    StringCodeWriter(std::string* buf)
        : CodeWriter(std::unique_ptr<std::ostream>(new std::stringstream())), buf_(buf) {}
    ~StringCodeWriter() override { Close(); }
    bool Close() override {
      // extract whats written to the stringstream to the external buffer.
      // we are sure that ostream_ is indeed stringstream.
      *buf_ = static_cast<std::stringstream*>(ostream_.get())->str();
      return true;
    }

   private:
    std::string* buf_;
  };
  return CodeWriterPtr(new StringCodeWriter(buf));
}

}  // namespace aidl
}  // namespace android
