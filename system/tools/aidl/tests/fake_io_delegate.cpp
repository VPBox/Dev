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

#include "fake_io_delegate.h"

#include <android-base/stringprintf.h>
#include <android-base/strings.h>

#include "logging.h"
#include "os.h"
#include "tests/test_util.h"

using android::base::StringAppendF;
using android::base::StringPrintf;
using std::string;
using std::unique_ptr;
using std::vector;

namespace android {
namespace aidl {
namespace test {

// Claims to always write successfully, but can't close the file.
class BrokenCodeWriter : public CodeWriter {
  bool Write(const char* /* format */, ...) override {  return true; }
  bool Close() override { return false; }
  ~BrokenCodeWriter() override = default;
};  // class BrokenCodeWriter

unique_ptr<string> FakeIoDelegate::GetFileContents(
    const string& relative_filename,
    const string& content_suffix) const {
  string filename = CleanPath(relative_filename);
  unique_ptr<string> contents;
  auto it = file_contents_.find(filename);
  if (it == file_contents_.end()) {
    return contents;
  }
  contents.reset(new string);
  *contents = it->second;
  contents->append(content_suffix);

  return contents;
}

unique_ptr<LineReader> FakeIoDelegate::GetLineReader(
    const string& file_path) const {
  unique_ptr<LineReader> ret;
  const auto& it = file_contents_.find(CleanPath(file_path));
  if (it != file_contents_.cend()) {
    ret = LineReader::ReadFromMemory(it->second);
  }
  return ret;
}

bool FakeIoDelegate::FileIsReadable(const string& path) const {
  return file_contents_.find(CleanPath(path)) != file_contents_.end();
}

std::unique_ptr<CodeWriter> FakeIoDelegate::GetCodeWriter(
    const std::string& file_path) const {
  if (broken_files_.count(file_path) > 0) {
    return unique_ptr<CodeWriter>(new BrokenCodeWriter);
  }
  removed_files_.erase(file_path);
  written_file_contents_[file_path] = "";
  return CodeWriter::ForString(&written_file_contents_[file_path]);
}

void FakeIoDelegate::RemovePath(const std::string& file_path) const {
  removed_files_.insert(file_path);
}

void FakeIoDelegate::SetFileContents(const string& filename,
                                     const string& contents) {
  file_contents_[filename] = contents;
}

vector<string> FakeIoDelegate::ListFiles(const string& dir) const {
  const string dir_name = dir.back() == OS_PATH_SEPARATOR ? dir : dir + OS_PATH_SEPARATOR;
  vector<string> files;
  for (auto it = file_contents_.begin(); it != file_contents_.end(); it++) {
    if (android::base::StartsWith(it->first, dir_name) && !it->second.empty()) {
      files.emplace_back(it->first);
    }
  }
  return files;
}

void FakeIoDelegate::AddStubParcelable(const string& canonical_name,
                                       const string& cpp_header) {
  string package, class_name, rel_path;
  SplitPackageClass(canonical_name, &rel_path, &package, &class_name);
  string contents;
  if (cpp_header.empty()) {
    contents = StringPrintf("package %s;\nparcelable %s;",
                            package.c_str(), class_name.c_str());
  } else {
    contents = StringPrintf("package %s;\nparcelable %s cpp_header \"%s\";",
                            package.c_str(), class_name.c_str(),
                            cpp_header.c_str());
  }
  SetFileContents(rel_path, contents);
}

void FakeIoDelegate::AddStubInterface(const string& canonical_name) {
  string package, class_name, rel_path;
  SplitPackageClass(canonical_name, &rel_path, &package, &class_name);
  string contents = StringPrintf("package %s;\ninterface %s { }",
                                 package.c_str(), class_name.c_str());
  SetFileContents(rel_path, contents);
}

void FakeIoDelegate::AddCompoundParcelable(const string& canonical_name,
                                           const vector<string>& subclasses) {
  string package, class_name, rel_path;
  SplitPackageClass(canonical_name, &rel_path, &package, &class_name);
  string contents = StringPrintf("package %s;\n", package.c_str());
  for (const string& subclass : subclasses) {
    StringAppendF(&contents, "parcelable %s.%s;\n",
                  class_name.c_str(), subclass.c_str());
  }
  SetFileContents(rel_path, contents);
}

void FakeIoDelegate::AddBrokenFilePath(const std::string& path) {
  broken_files_.insert(path);
}

bool FakeIoDelegate::GetWrittenContents(const string& path, string* content) {
  const auto it = written_file_contents_.find(path);
  if (it == written_file_contents_.end()) {
    return false;
  }
  if (content) {
    *content = it->second;
  }
  return true;
}

bool FakeIoDelegate::PathWasRemoved(const std::string& path) {
  if (removed_files_.count(path) > 0) {
    return true;
  }
  return false;
}

string FakeIoDelegate::CleanPath(const string& path) const {
  string clean_path = path;
  while (clean_path.length() >= 2 &&
         clean_path[0] == '.' &&
         clean_path[1] == OS_PATH_SEPARATOR) {
    clean_path = clean_path.substr(2);
  }
  return clean_path;
}

}  // namespace test
}  // namespace android
}  // namespace aidl
