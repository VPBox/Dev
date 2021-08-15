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

#ifndef AIDL_TESTS_FAKE_IO_DELEGATE_H_
#define AIDL_TESTS_FAKE_IO_DELEGATE_H_

#include <android-base/macros.h>

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "io_delegate.h"

namespace android {
namespace aidl {
namespace test {

class FakeIoDelegate : public IoDelegate {
 public:
  FakeIoDelegate() = default;
  virtual ~FakeIoDelegate() = default;

  // Overrides from the real IoDelegate
  std::unique_ptr<std::string> GetFileContents(
      const std::string& filename,
      const std::string& append_content_suffix = "") const override;
  std::unique_ptr<LineReader> GetLineReader(
      const std::string& file_path) const override;
  bool FileIsReadable(const std::string& path) const override;
  std::unique_ptr<CodeWriter> GetCodeWriter(
      const std::string& file_path) const override;
  void RemovePath(const std::string& file_path) const override;
  std::vector<std::string> ListFiles(const std::string& dir) const override;

  // Methods added to facilitate testing.
  void SetFileContents(const std::string& filename,
                       const std::string& contents);
  void AddStubParcelable(const std::string& canonical_name,
                         const std::string& cpp_header);
  void AddStubInterface(const std::string& canonical_name);
  void AddCompoundParcelable(const std::string& canonical_name,
                             const std::vector<std::string>& subclasses);
  void AddBrokenFilePath(const std::string& path);
  // Returns true iff we've previously written to |path|.
  // When we return true, we'll set *contents to the written string.
  bool GetWrittenContents(const std::string& path, std::string* content);

  bool PathWasRemoved(const std::string& path);

 private:
  // Remove leading "./" from |path|.
  std::string CleanPath(const std::string& path) const;

  std::map<std::string, std::string> file_contents_;
  // Normally, writing to files leaves the IoDelegate unchanged, so
  // GetCodeWriter is a const method.  However, for tests, we break this
  // intentionally by storing the written strings.
  mutable std::map<std::string, std::string> written_file_contents_;

  // We normally just write to strings in |written_file_contents_| but for
  // files in this list, we simulate I/O errors.
  std::set<std::string> broken_files_;
  mutable std::set<std::string> removed_files_;

  DISALLOW_COPY_AND_ASSIGN(FakeIoDelegate);
};  // class FakeIoDelegate

}  // namespace test
}  // namespace android
}  // namespace aidl

#endif // AIDL_TESTS_FAKE_IO_DELEGATE_H_
