//
// Copyright (C) 2009 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef UPDATE_ENGINE_FAKE_FILE_WRITER_H_
#define UPDATE_ENGINE_FAKE_FILE_WRITER_H_

#include <vector>

#include <base/macros.h>
#include <brillo/secure_blob.h>

#include "update_engine/payload_consumer/file_writer.h"

// FakeFileWriter is an implementation of FileWriter. It will succeed
// calls to Open(), Close(), but not do any work. All calls to Write()
// will append the passed data to an internal vector.

namespace chromeos_update_engine {

class FakeFileWriter : public FileWriter {
 public:
  FakeFileWriter() : was_opened_(false), was_closed_(false) {}

  virtual int Open(const char* path, int flags, mode_t mode) {
    CHECK(!was_opened_);
    CHECK(!was_closed_);
    was_opened_ = true;
    return 0;
  }

  virtual ssize_t Write(const void* bytes, size_t count) {
    CHECK(was_opened_);
    CHECK(!was_closed_);
    const char* char_bytes = reinterpret_cast<const char*>(bytes);
    bytes_.insert(bytes_.end(), char_bytes, char_bytes + count);
    return count;
  }

  virtual int Close() {
    CHECK(was_opened_);
    CHECK(!was_closed_);
    was_closed_ = true;
    return 0;
  }

  const brillo::Blob& bytes() { return bytes_; }

 private:
  // The internal store of all bytes that have been written
  brillo::Blob bytes_;

  // These are just to ensure FileWriter methods are called properly.
  bool was_opened_;
  bool was_closed_;

  DISALLOW_COPY_AND_ASSIGN(FakeFileWriter);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_FAKE_FILE_WRITER_H_
