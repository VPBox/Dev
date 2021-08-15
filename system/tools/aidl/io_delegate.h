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

#include <android-base/macros.h>

#include <memory>
#include <string>
#include <vector>

#include "code_writer.h"
#include "line_reader.h"

namespace android {
namespace aidl {

class IoDelegate {
 public:
  IoDelegate() = default;
  virtual ~IoDelegate() = default;

  // Stores an absolute version of |path| to |*absolute_path|,
  // possibly prefixing it with the current working directory.
  // Returns false and does not set |*absolute_path| on error.
  static bool GetAbsolutePath(const std::string& path,
                              std::string* absolute_path);

  // Returns a unique_ptr to the contents of |filename|.
  // Will append the optional |content_suffix| to the returned contents.
  virtual std::unique_ptr<std::string> GetFileContents(
      const std::string& filename,
      const std::string& content_suffix = "") const;

  virtual std::unique_ptr<LineReader> GetLineReader(
      const std::string& file_path) const;

  virtual bool FileIsReadable(const std::string& path) const;

  virtual std::unique_ptr<CodeWriter> GetCodeWriter(
      const std::string& file_path) const;

  virtual void RemovePath(const std::string& file_path) const;

  virtual std::vector<std::string> ListFiles(const std::string& dir) const;

 private:
  // Create the directory when path is a dir or the parent directory when
  // path is a file. Path is a dir if it ends with the path separator.
  bool CreateDirForPath(const std::string& path) const;

  DISALLOW_COPY_AND_ASSIGN(IoDelegate);
};  // class IoDelegate

}  // namespace android
}  // namespace aidl
