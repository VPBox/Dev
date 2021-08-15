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

#ifndef SYSTEM_TOOLS_SYSPROP_CODE_WRITER_H_
#define SYSTEM_TOOLS_SYSPROP_CODE_WRITER_H_

#include <string>

class CodeWriter {
 public:
  explicit CodeWriter(std::string indent);

  void Write(const char* format, ...) __attribute__((format(__printf__, 2, 3)));

  void Indent();
  void Dedent();

  const std::string& Code() const {
    return code_;
  }

 private:
  CodeWriter(const CodeWriter&) = delete;
  CodeWriter& operator=(const CodeWriter&) = delete;

  int indent_level_ = 0;
  bool start_of_line_ = true;
  std::string code_;
  const std::string indent_;
};

#endif  // SYSTEM_TOOLS_SYSPROP_CODE_WRITER_H_
