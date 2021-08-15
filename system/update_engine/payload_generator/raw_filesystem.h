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

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_RAW_FILESYSTEM_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_RAW_FILESYSTEM_H_

// A simple filesystem interface implementation used for unknown filesystem
// format such as the kernel.

#include "update_engine/payload_generator/filesystem_interface.h"

#include <memory>
#include <string>
#include <vector>

namespace chromeos_update_engine {

class RawFilesystem : public FilesystemInterface {
 public:
  static std::unique_ptr<RawFilesystem> Create(const std::string& filename,
                                               uint64_t block_size,
                                               uint64_t block_count);
  virtual ~RawFilesystem() = default;

  // FilesystemInterface overrides.
  size_t GetBlockSize() const override;
  size_t GetBlockCount() const override;

  // GetFiles will return only one file with all the blocks of the filesystem
  // with the name passed during construction.
  bool GetFiles(std::vector<File>* files) const override;

  bool LoadSettings(brillo::KeyValueStore* store) const override {
    return false;
  }

 private:
  RawFilesystem() = default;

  std::string filename_;
  uint64_t block_count_;
  uint64_t block_size_;

  DISALLOW_COPY_AND_ASSIGN(RawFilesystem);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_RAW_FILESYSTEM_H_
