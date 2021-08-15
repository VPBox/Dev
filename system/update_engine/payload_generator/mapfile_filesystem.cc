//
// Copyright (C) 2016 The Android Open Source Project
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

#include "update_engine/payload_generator/mapfile_filesystem.h"

#include <algorithm>
#include <map>

#include <base/files/file_util.h>
#include <base/logging.h>
#include <base/memory/ptr_util.h>
#include <base/strings/string_number_conversions.h>
#include <base/strings/string_split.h>

#include "update_engine/common/utils.h"
#include "update_engine/payload_generator/extent_ranges.h"
#include "update_engine/payload_generator/extent_utils.h"
#include "update_engine/update_metadata.pb.h"

using std::string;
using std::vector;

namespace {
// The .map file is defined in terms of 4K blocks.
size_t kMapfileBlockSize = 4096;
}  // namespace

namespace chromeos_update_engine {

std::unique_ptr<MapfileFilesystem> MapfileFilesystem::CreateFromFile(
    const string& filename, const string& mapfile_filename) {
  if (filename.empty() || mapfile_filename.empty())
    return nullptr;

  off_t file_size = utils::FileSize(filename);
  if (file_size < 0)
    return nullptr;

  if (file_size % kMapfileBlockSize) {
    LOG(ERROR) << "Image file " << filename << " has a size of " << file_size
               << " which is not multiple of " << kMapfileBlockSize;
    return nullptr;
  }
  off_t num_blocks = file_size / kMapfileBlockSize;

  if (!utils::FileExists(mapfile_filename.c_str())) {
    LOG(ERROR) << "File " << mapfile_filename << " doesn't exist";
    return nullptr;
  }

  return base::WrapUnique(new MapfileFilesystem(mapfile_filename, num_blocks));
}

MapfileFilesystem::MapfileFilesystem(const string& mapfile_filename,
                                     off_t num_blocks)
    : mapfile_filename_(mapfile_filename), num_blocks_(num_blocks) {}

size_t MapfileFilesystem::GetBlockSize() const {
  return kMapfileBlockSize;
}

size_t MapfileFilesystem::GetBlockCount() const {
  return num_blocks_;
}

bool MapfileFilesystem::GetFiles(vector<File>* files) const {
  files->clear();

  string file_data;
  if (!base::ReadFileToString(base::FilePath(mapfile_filename_), &file_data)) {
    LOG(ERROR) << "Unable to read .map file: " << mapfile_filename_;
    return false;
  }

  // Iterate over all the lines in the file and generate one File entry per
  // line.
  vector<base::StringPiece> lines = base::SplitStringPiece(
      file_data, "\n", base::KEEP_WHITESPACE, base::SPLIT_WANT_ALL);
  for (const base::StringPiece& line : lines) {
    File mapped_file;

    mapped_file.extents = {};
    size_t delim, last_delim = line.size();
    while ((delim = line.rfind(' ', last_delim - 1)) != string::npos) {
      string blocks =
          line.substr(delim + 1, last_delim - (delim + 1)).as_string();
      size_t dash = blocks.find('-', 0);
      uint64_t block_start, block_end;
      if (dash == string::npos && base::StringToUint64(blocks, &block_start)) {
        mapped_file.extents.push_back(ExtentForRange(block_start, 1));
      } else if (dash != string::npos &&
                 base::StringToUint64(blocks.substr(0, dash), &block_start) &&
                 base::StringToUint64(blocks.substr(dash + 1), &block_end)) {
        if (block_end < block_start) {
          LOG(ERROR) << "End block " << block_end
                     << " is smaller than start block " << block_start
                     << std::endl
                     << line;
          return false;
        }
        if (block_end > static_cast<uint64_t>(num_blocks_)) {
          LOG(ERROR) << "The end block " << block_end
                     << " is past the end of the file of " << num_blocks_
                     << " blocks" << std::endl
                     << line;
          return false;
        }
        mapped_file.extents.push_back(
            ExtentForRange(block_start, block_end - block_start + 1));
      } else {
        // If we can't parse N or N-M, we assume the block is actually part of
        // the name of the file.
        break;
      }
      last_delim = delim;
    }
    // We parsed the blocks from the end of the line, so we need to reverse
    // the Extents in the file.
    std::reverse(mapped_file.extents.begin(), mapped_file.extents.end());

    if (last_delim == string::npos)
      continue;
    mapped_file.name = line.substr(0, last_delim).as_string();

    files->push_back(mapped_file);
  }

  return true;
}

bool MapfileFilesystem::LoadSettings(brillo::KeyValueStore* store) const {
  // Settings not supported in mapfile since the storage format is unknown.
  LOG(ERROR) << "mapfile doesn't support LoadSettings().";
  return false;
}

}  // namespace chromeos_update_engine
