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

#include "update_engine/payload_generator/raw_filesystem.h"

#include <memory>

#include "update_engine/common/utils.h"
#include "update_engine/payload_generator/extent_ranges.h"
#include "update_engine/update_metadata.pb.h"

using std::unique_ptr;

namespace chromeos_update_engine {

unique_ptr<RawFilesystem> RawFilesystem::Create(const std::string& filename,
                                                uint64_t block_size,
                                                uint64_t block_count) {
  unique_ptr<RawFilesystem> result(new RawFilesystem());
  result->filename_ = filename;
  result->block_size_ = block_size;
  result->block_count_ = block_count;
  return result;
}

size_t RawFilesystem::GetBlockSize() const {
  return block_size_;
}

size_t RawFilesystem::GetBlockCount() const {
  return block_count_;
}

bool RawFilesystem::GetFiles(std::vector<File>* files) const {
  files->clear();
  File file;
  file.name = filename_;
  file.extents = {ExtentForRange(0, block_count_)};
  files->push_back(file);
  return true;
}

}  // namespace chromeos_update_engine
