//
// Copyright (C) 2015 The Android Open Source Project
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

#include "update_engine/payload_generator/fake_filesystem.h"

#include <gtest/gtest.h>

namespace chromeos_update_engine {

FakeFilesystem::FakeFilesystem(uint64_t block_size, uint64_t block_count)
    : block_size_(block_size), block_count_(block_count) {}

size_t FakeFilesystem::GetBlockSize() const {
  return block_size_;
}

size_t FakeFilesystem::GetBlockCount() const {
  return block_count_;
}

bool FakeFilesystem::GetFiles(std::vector<File>* files) const {
  *files = files_;
  return true;
}

void FakeFilesystem::AddFile(const std::string& filename,
                             const std::vector<Extent>& extents) {
  File file;
  file.name = filename;
  file.extents = extents;
  for (const Extent& extent : extents) {
    EXPECT_LE(0U, extent.start_block());
    EXPECT_LE(extent.start_block() + extent.num_blocks(), block_count_);
  }
  files_.push_back(file);
}

bool FakeFilesystem::LoadSettings(brillo::KeyValueStore* store) const {
  if (minor_version_ < 0)
    return false;
  store->SetString("PAYLOAD_MINOR_VERSION", std::to_string(minor_version_));
  return true;
}

}  // namespace chromeos_update_engine
