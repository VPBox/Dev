//
// Copyright (C) 2017 The Android Open Source Project
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

// This class implements a FilesystemInterface, which lets the caller obtain
// basic information about what files are in the filesystem and where are they
// located in the disk, but not full access to the uncompressed contents of
// these files. This class uses the definitions found in
// fs/squashfs/squashfs_fs.h in the kernel header tree. This class supports
// squashfs version 4 in little-endian format only.

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_SQUASHFS_FILESYSTEM_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_SQUASHFS_FILESYSTEM_H_

#include <memory>
#include <string>
#include <vector>

#include <brillo/secure_blob.h>

#include "update_engine/payload_consumer/file_descriptor.h"
#include "update_engine/payload_generator/extent_utils.h"
#include "update_engine/payload_generator/filesystem_interface.h"

namespace chromeos_update_engine {

class SquashfsFilesystem : public FilesystemInterface {
 public:
  // From an squashfs image we need: (offset, bytes)
  // - magic: (0, 4)
  //   * Acceptable value is: 0x73717368
  // - block size: (12, 4)
  // - compression type: (20, 2)
  //   * 1 is for zlib, gzip
  // - major number: (28, 2)
  //   * We only support version 4 for now.
  struct SquashfsHeader {
    uint32_t magic;
    uint32_t block_size;
    uint16_t compression_type;
    uint16_t major_version;
  };

  ~SquashfsFilesystem() override = default;

  // Creates the file system from the Squashfs file itself. If
  // |extract_deflates| is true, it will process files to find location of all
  // deflate streams.
  static std::unique_ptr<SquashfsFilesystem> CreateFromFile(
      const std::string& sqfs_path, bool extract_deflates);

  // Creates the file system from a file map |filemap| which is a multi-line
  // string with each line with the following format:
  //
  // file_path start_byte compressed_size_1 ... compressed_size_2
  //
  // file_path: The name of the file inside the Squashfs File.
  // start_byte: The byte address of the start of the file.
  // compressed_size_i: The compressed size of the ith block of the file.
  //
  // The 25th bit of compressed_size_i is set if the block is uncompressed.
  // |size| is the size of the Squashfs image.
  static std::unique_ptr<SquashfsFilesystem> CreateFromFileMap(
      const std::string& filemap, size_t size, const SquashfsHeader& header);

  // FilesystemInterface overrides.
  size_t GetBlockSize() const override;
  size_t GetBlockCount() const override;

  // Returns one FilesystemInterface::File for every file (that is not added to
  // fragments) in the squashfs image.
  //
  // It also returns the following metadata files:
  //  <fragment-i>: The ith fragment in the Sqauashfs file.
  //  <metadata-i>: The part of the file system that does not belong to any
  //                file. Normally, there is only two: one for superblock and
  //                one for the metadata at the end.
  bool GetFiles(std::vector<File>* files) const override;

  // Squashfs image does not support this yet.
  bool LoadSettings(brillo::KeyValueStore* store) const override;

  // Returns true if the first few bytes of a file indicates a valid Squashfs
  // image. The size of the |blob| should be at least
  // sizeof(SquashfsHeader) or for now 96 bytes.
  static bool IsSquashfsImage(const brillo::Blob& blob);

 private:
  SquashfsFilesystem() = default;

  // Initialize and populates the files in the file system.
  bool Init(const std::string& map,
            const std::string& sqfs_path,
            size_t size,
            const SquashfsHeader& header,
            bool extract_deflates);

  // The size of the image in bytes.
  size_t size_;

  // All the files in the filesystem.
  std::vector<File> files_;

  DISALLOW_COPY_AND_ASSIGN(SquashfsFilesystem);
};

}  // namespace chromeos_update_engine
#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_SQUASHFS_FILESYSTEM_H_
