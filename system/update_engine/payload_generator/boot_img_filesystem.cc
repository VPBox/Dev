//
// Copyright (C) 2018 The Android Open Source Project
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

#include "update_engine/payload_generator/boot_img_filesystem.h"

#include <base/logging.h>
#include <brillo/secure_blob.h>
#include <puffin/utils.h>

#include "update_engine/common/utils.h"
#include "update_engine/payload_generator/delta_diff_generator.h"
#include "update_engine/payload_generator/extent_ranges.h"

using std::string;
using std::unique_ptr;
using std::vector;

namespace chromeos_update_engine {

unique_ptr<BootImgFilesystem> BootImgFilesystem::CreateFromFile(
    const string& filename) {
  if (filename.empty())
    return nullptr;

  brillo::Blob header;
  if (!utils::ReadFileChunk(filename, 0, sizeof(boot_img_hdr), &header) ||
      header.size() != sizeof(boot_img_hdr) ||
      memcmp(header.data(), BOOT_MAGIC, BOOT_MAGIC_SIZE) != 0) {
    return nullptr;
  }

  unique_ptr<BootImgFilesystem> result(new BootImgFilesystem());
  result->filename_ = filename;
  memcpy(&result->hdr_, header.data(), header.size());
  return result;
}

size_t BootImgFilesystem::GetBlockSize() const {
  // Page size may not be 4K, but we currently only support 4K block size.
  return kBlockSize;
}

size_t BootImgFilesystem::GetBlockCount() const {
  return utils::DivRoundUp(utils::FileSize(filename_), kBlockSize);
}

FilesystemInterface::File BootImgFilesystem::GetFile(const string& name,
                                                     uint64_t offset,
                                                     uint64_t size) const {
  File file;
  file.name = name;
  file.extents = {ExtentForBytes(kBlockSize, offset, size)};

  brillo::Blob data;
  if (utils::ReadFileChunk(filename_, offset, size, &data)) {
    constexpr size_t kGZipHeaderSize = 10;
    // Check GZip header magic.
    if (data.size() > kGZipHeaderSize && data[0] == 0x1F && data[1] == 0x8B) {
      if (!puffin::LocateDeflatesInGzip(data, &file.deflates)) {
        LOG(ERROR) << "Error occurred parsing gzip " << name << " at offset "
                   << offset << " of " << filename_ << ", found "
                   << file.deflates.size() << " deflates.";
        return file;
      }
      for (auto& deflate : file.deflates) {
        deflate.offset += offset * 8;
      }
    }
  }
  return file;
}

bool BootImgFilesystem::GetFiles(vector<File>* files) const {
  files->clear();
  const uint64_t file_size = utils::FileSize(filename_);
  // The first page is header.
  uint64_t offset = hdr_.page_size;
  if (hdr_.kernel_size > 0 && offset + hdr_.kernel_size <= file_size) {
    files->emplace_back(GetFile("<kernel>", offset, hdr_.kernel_size));
  }
  offset += utils::RoundUp(hdr_.kernel_size, hdr_.page_size);
  if (hdr_.ramdisk_size > 0 && offset + hdr_.ramdisk_size <= file_size) {
    files->emplace_back(GetFile("<ramdisk>", offset, hdr_.ramdisk_size));
  }
  return true;
}

bool BootImgFilesystem::LoadSettings(brillo::KeyValueStore* store) const {
  return false;
}

}  // namespace chromeos_update_engine
