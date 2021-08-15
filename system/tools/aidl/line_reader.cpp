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

#include "line_reader.h"

#include <fstream>
#include <sstream>

using std::istringstream;
using std::ifstream;
using std::string;
using std::unique_ptr;

namespace android {
namespace aidl {

class FileLineReader : public LineReader {
 public:
  FileLineReader() = default;
  ~FileLineReader() override {
    input_stream_.close();
  }

  bool Init(const std::string& file_path) {
    input_stream_.open(file_path, ifstream::in | ifstream::binary);
    return input_stream_.is_open() && input_stream_.good();
  }

  bool ReadLine(string* line) override {
    if (!input_stream_.good()) {
      return false;
    }
    line->clear();
    std::getline(input_stream_, *line);
    return true;
  }

 private:
  ifstream input_stream_;

  DISALLOW_COPY_AND_ASSIGN(FileLineReader);
};  // class FileLineReader

class MemoryLineReader : public LineReader {
 public:
  explicit MemoryLineReader(const string& contents) : input_stream_(contents) {}
  ~MemoryLineReader() override = default;

  bool ReadLine(string* line) override {
    if (!input_stream_.good()) {
      return false;
    }
    line->clear();
    std::getline(input_stream_, *line);
    return true;
  }

 private:
  istringstream input_stream_;

  DISALLOW_COPY_AND_ASSIGN(MemoryLineReader);
};  // class MemoryLineReader

unique_ptr<LineReader> LineReader::ReadFromFile(const string& file_path) {
  unique_ptr<FileLineReader> file_reader(new FileLineReader());
  unique_ptr<LineReader> ret;
  if (file_reader->Init(file_path)) {
    ret.reset(file_reader.release());
  }
  return ret;
}

unique_ptr<LineReader> LineReader::ReadFromMemory(const string& contents) {
  return unique_ptr<LineReader>(new MemoryLineReader(contents));
}

}  // namespace android
}  // namespace aidl
