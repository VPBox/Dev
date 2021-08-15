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

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_EXT2_FILESYSTEM_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_EXT2_FILESYSTEM_H_

#include "update_engine/payload_generator/filesystem_interface.h"

#include <memory>
#include <string>
#include <vector>

#if defined(__clang__)
// TODO: Remove these pragmas when b/35721782 is fixed.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmacro-redefined"
#endif
#include <ext2fs/ext2fs.h>
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

namespace chromeos_update_engine {

class Ext2Filesystem : public FilesystemInterface {
 public:
  // Creates an Ext2Filesystem from a ext2 formatted filesystem stored in a
  // file. The file doesn't need to be loop-back mounted.
  static std::unique_ptr<Ext2Filesystem> CreateFromFile(
      const std::string& filename);
  virtual ~Ext2Filesystem();

  // FilesystemInterface overrides.
  size_t GetBlockSize() const override;
  size_t GetBlockCount() const override;

  // GetFiles will return one FilesystemInterface::File for every file and every
  // directory in the filesystem. Hard-linked files will appear in the list
  // several times with the same list of blocks.
  // On addition to actual files, it also returns these pseudo-files:
  //  <free-space>: With all the unallocated data-blocks.
  //  <inode-blocks>: Will all the data-blocks for second and third level inodes
  //    of all the files.
  //  <group-descriptors>: With the block group descriptor and their reserved
  //    space.
  //  <metadata>: With the rest of ext2 metadata blocks, such as superblocks
  //    and bitmap tables.
  bool GetFiles(std::vector<File>* files) const override;

  bool LoadSettings(brillo::KeyValueStore* store) const override;

 private:
  Ext2Filesystem() = default;

  // The ext2 main data structure holding the filesystem.
  ext2_filsys filsys_ = nullptr;

  // The file where the filesystem is stored.
  std::string filename_;

  DISALLOW_COPY_AND_ASSIGN(Ext2Filesystem);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_EXT2_FILESYSTEM_H_
