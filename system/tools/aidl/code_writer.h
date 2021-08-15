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

#pragma once

#include <memory>
#include <ostream>
#include <string>

#include <stdio.h>

#include <android-base/macros.h>

namespace android {
namespace aidl {

class CodeWriter;
using CodeWriterPtr = std::unique_ptr<CodeWriter>;

class CodeWriter {
 public:
  // Get a CodeWriter that writes to a file. When filename is "-",
  // it is written to stdout.
  static CodeWriterPtr ForFile(const std::string& filename);
  // Get a CodeWriter that writes to a string buffer.
  // The buffer gets updated only after Close() is called or the CodeWriter
  // is deleted -- much like a real file.
  static CodeWriterPtr ForString(std::string* buf);
  // Write a formatted string to this writer in the usual printf sense.
  // Returns false on error.
  virtual bool Write(const char* format, ...);
  void Indent();
  void Dedent();
  virtual bool Close();
  virtual ~CodeWriter() = default;
  CodeWriter() = default;

  CodeWriter& operator<<(const char* s);
  CodeWriter& operator<<(const std::string& str);

 private:
  CodeWriter(std::unique_ptr<std::ostream> ostream);
  std::string ApplyIndent(const std::string& str);
  const std::unique_ptr<std::ostream> ostream_;
  int indent_level_ {0};
  bool start_of_line_ {true};
};

}  // namespace aidl
}  // namespace android
