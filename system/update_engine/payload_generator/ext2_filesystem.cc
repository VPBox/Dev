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

#include "update_engine/payload_generator/ext2_filesystem.h"

#include <et/com_err.h>
#if defined(__clang__)
// TODO(*): Remove these pragmas when b/35721782 is fixed.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmacro-redefined"
#endif
#include <ext2fs/ext2_io.h>
#include <ext2fs/ext2fs.h>
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#include <map>
#include <set>

#include <base/logging.h>
#include <base/strings/stringprintf.h>

#include "update_engine/common/utils.h"
#include "update_engine/payload_generator/extent_ranges.h"
#include "update_engine/payload_generator/extent_utils.h"
#include "update_engine/update_metadata.pb.h"

using std::set;
using std::string;
using std::unique_ptr;
using std::vector;

namespace chromeos_update_engine {

namespace {
// Processes all blocks belonging to an inode and adds them to the extent list.
// This function should match the prototype expected by ext2fs_block_iterate2().
int ProcessInodeAllBlocks(ext2_filsys fs,
                          blk_t* blocknr,
                          e2_blkcnt_t blockcnt,
                          blk_t ref_blk,
                          int ref_offset,
                          void* priv) {
  vector<Extent>* extents = static_cast<vector<Extent>*>(priv);
  AppendBlockToExtents(extents, *blocknr);
  return 0;
}

// Processes only indirect, double indirect or triple indirect metadata
// blocks belonging to an inode. This function should match the prototype of
// ext2fs_block_iterate2().
int AddMetadataBlocks(ext2_filsys fs,
                      blk_t* blocknr,
                      e2_blkcnt_t blockcnt,
                      blk_t ref_blk,
                      int ref_offset,
                      void* priv) {
  set<uint64_t>* blocks = static_cast<set<uint64_t>*>(priv);
  // If |blockcnt| is non-negative, |blocknr| points to the physical block
  // number.
  // If |blockcnt| is negative, it is one of the values: BLOCK_COUNT_IND,
  // BLOCK_COUNT_DIND, BLOCK_COUNT_TIND or BLOCK_COUNT_TRANSLATOR and
  // |blocknr| points to a block in the first three cases. The last case is
  // only used by GNU Hurd, so we shouldn't see those cases here.
  if (blockcnt == BLOCK_COUNT_IND || blockcnt == BLOCK_COUNT_DIND ||
      blockcnt == BLOCK_COUNT_TIND) {
    blocks->insert(*blocknr);
  }
  return 0;
}

struct UpdateFileAndAppendState {
  std::map<ext2_ino_t, FilesystemInterface::File>* inodes = nullptr;
  set<ext2_ino_t>* used_inodes = nullptr;
  vector<FilesystemInterface::File>* files = nullptr;
  ext2_filsys filsys;
};

int UpdateFileAndAppend(ext2_ino_t dir,
                        int entry,
                        struct ext2_dir_entry* dirent,
                        int offset,
                        int blocksize,
                        char* buf,
                        void* priv_data) {
  UpdateFileAndAppendState* state =
      static_cast<UpdateFileAndAppendState*>(priv_data);
  uint32_t file_type = dirent->name_len >> 8;
  // Directories can't have hard links, and they are added from the outer loop.
  if (file_type == EXT2_FT_DIR)
    return 0;

  auto ino_file = state->inodes->find(dirent->inode);
  if (ino_file == state->inodes->end())
    return 0;
  auto dir_file = state->inodes->find(dir);
  if (dir_file == state->inodes->end())
    return 0;
  string basename(dirent->name, dirent->name_len & 0xff);
  ino_file->second.name = dir_file->second.name;
  if (dir_file->second.name != "/")
    ino_file->second.name += "/";
  ino_file->second.name += basename;

  // Append this file to the output. If the file has a hard link, it will be
  // added twice to the output, but with different names, which is ok. That will
  // help identify all the versions of the same file.
  state->files->push_back(ino_file->second);
  state->used_inodes->insert(dirent->inode);
  return 0;
}

}  // namespace

unique_ptr<Ext2Filesystem> Ext2Filesystem::CreateFromFile(
    const string& filename) {
  if (filename.empty())
    return nullptr;
  unique_ptr<Ext2Filesystem> result(new Ext2Filesystem());
  result->filename_ = filename;

  errcode_t err = ext2fs_open(filename.c_str(),
                              0,  // flags (read only)
                              0,  // superblock block number
                              0,  // block_size (autodetect)
                              unix_io_manager,
                              &result->filsys_);
  if (err) {
    LOG(ERROR) << "Opening ext2fs " << filename;
    return nullptr;
  }
  return result;
}

Ext2Filesystem::~Ext2Filesystem() {
  ext2fs_free(filsys_);
}

size_t Ext2Filesystem::GetBlockSize() const {
  return filsys_->blocksize;
}

size_t Ext2Filesystem::GetBlockCount() const {
  return ext2fs_blocks_count(filsys_->super);
}

bool Ext2Filesystem::GetFiles(vector<File>* files) const {
  TEST_AND_RETURN_FALSE_ERRCODE(ext2fs_read_inode_bitmap(filsys_));

  ext2_inode_scan iscan;
  TEST_AND_RETURN_FALSE_ERRCODE(
      ext2fs_open_inode_scan(filsys_, 0 /* buffer_blocks */, &iscan));

  std::map<ext2_ino_t, File> inodes;

  // List of directories. We need to first parse all the files in a directory
  // to later fix the absolute paths.
  vector<ext2_ino_t> directories;

  set<uint64_t> inode_blocks;

  // Iterator
  ext2_ino_t it_ino;
  ext2_inode it_inode;

  bool ok = true;
  while (true) {
    errcode_t error = ext2fs_get_next_inode(iscan, &it_ino, &it_inode);
    if (error) {
      LOG(ERROR) << "Failed to retrieve next inode (" << error << ")";
      ok = false;
      break;
    }
    if (it_ino == 0)
      break;

    // Skip inodes that are not in use.
    if (!ext2fs_test_inode_bitmap(filsys_->inode_map, it_ino))
      continue;

    File& file = inodes[it_ino];
    if (it_ino == EXT2_RESIZE_INO) {
      file.name = "<group-descriptors>";
    } else {
      file.name = base::StringPrintf("<inode-%u>", it_ino);
    }

    memset(&file.file_stat, 0, sizeof(file.file_stat));
    file.file_stat.st_ino = it_ino;
    file.file_stat.st_mode = it_inode.i_mode;
    file.file_stat.st_nlink = it_inode.i_links_count;
    file.file_stat.st_uid = it_inode.i_uid;
    file.file_stat.st_gid = it_inode.i_gid;
    file.file_stat.st_size = it_inode.i_size;
    file.file_stat.st_blksize = filsys_->blocksize;
    file.file_stat.st_blocks = it_inode.i_blocks;
    file.file_stat.st_atime = it_inode.i_atime;
    file.file_stat.st_mtime = it_inode.i_mtime;
    file.file_stat.st_ctime = it_inode.i_ctime;

    bool is_dir = (ext2fs_check_directory(filsys_, it_ino) == 0);
    if (is_dir)
      directories.push_back(it_ino);

    if (!ext2fs_inode_has_valid_blocks(&it_inode))
      continue;

    // Process the inode data and metadata blocks.
    // For normal files, inode blocks are indirect, double indirect
    // and triple indirect blocks (no data blocks). For directories and
    // the journal, all blocks are considered metadata blocks.
    int flags = it_ino < EXT2_GOOD_OLD_FIRST_INO ? 0 : BLOCK_FLAG_DATA_ONLY;
    error = ext2fs_block_iterate2(filsys_,
                                  it_ino,
                                  flags,
                                  nullptr,  // block_buf
                                  ProcessInodeAllBlocks,
                                  &file.extents);

    if (error) {
      LOG(ERROR) << "Failed to enumerate inode " << it_ino << " blocks ("
                 << error << ")";
      continue;
    }
    if (it_ino >= EXT2_GOOD_OLD_FIRST_INO) {
      ext2fs_block_iterate2(
          filsys_, it_ino, 0, nullptr, AddMetadataBlocks, &inode_blocks);
    }
  }
  ext2fs_close_inode_scan(iscan);
  if (!ok)
    return false;

  // The set of inodes already added to the output. There can be less elements
  // here than in files since the later can contain repeated inodes due to
  // hardlink files.
  set<ext2_ino_t> used_inodes;

  UpdateFileAndAppendState priv_data;
  priv_data.inodes = &inodes;
  priv_data.used_inodes = &used_inodes;
  priv_data.files = files;
  priv_data.filsys = filsys_;

  files->clear();
  // Iterate over all the files of each directory to update the name and add it.
  for (ext2_ino_t dir_ino : directories) {
    char* dir_name = nullptr;
    errcode_t error = ext2fs_get_pathname(filsys_, dir_ino, 0, &dir_name);
    if (error) {
      // Not being able to read a directory name is not a fatal error, it is
      // just skiped.
      LOG(WARNING) << "Reading directory name on inode " << dir_ino
                   << " (error " << error << ")";
      inodes[dir_ino].name = base::StringPrintf("<dir-%u>", dir_ino);
    } else {
      inodes[dir_ino].name = dir_name;
      files->push_back(inodes[dir_ino]);
      used_inodes.insert(dir_ino);
    }
    ext2fs_free_mem(&dir_name);

    error = ext2fs_dir_iterate2(filsys_,
                                dir_ino,
                                0,
                                nullptr /* block_buf */,
                                UpdateFileAndAppend,
                                &priv_data);
    if (error) {
      LOG(WARNING) << "Failed to enumerate files in directory "
                   << inodes[dir_ino].name << " (error " << error << ")";
    }
  }

  // Add <inode-blocks> file with the blocks that hold inodes.
  File inode_file;
  inode_file.name = "<inode-blocks>";
  for (uint64_t block : inode_blocks) {
    AppendBlockToExtents(&inode_file.extents, block);
  }
  files->push_back(inode_file);

  // Add <free-spacce> blocs.
  errcode_t error = ext2fs_read_block_bitmap(filsys_);
  if (error) {
    LOG(ERROR) << "Reading the blocks bitmap (error " << error << ")";
  } else {
    File free_space;
    free_space.name = "<free-space>";
    blk64_t blk_start = ext2fs_get_block_bitmap_start2(filsys_->block_map);
    blk64_t blk_end = ext2fs_get_block_bitmap_end2(filsys_->block_map);
    for (blk64_t block = blk_start; block < blk_end; block++) {
      if (!ext2fs_test_block_bitmap2(filsys_->block_map, block))
        AppendBlockToExtents(&free_space.extents, block);
    }
    files->push_back(free_space);
  }

  // Add all the unreachable files plus the pseudo-files with an inode. Since
  // these inodes aren't files in the filesystem, ignore the empty ones.
  for (const auto& ino_file : inodes) {
    if (used_inodes.find(ino_file.first) != used_inodes.end())
      continue;
    if (ino_file.second.extents.empty())
      continue;

    File file = ino_file.second;
    ExtentRanges ranges;
    ranges.AddExtents(file.extents);
    file.extents = ranges.GetExtentsForBlockCount(ranges.blocks());

    files->push_back(file);
  }

  return true;
}

bool Ext2Filesystem::LoadSettings(brillo::KeyValueStore* store) const {
  // First search for the settings inode following symlinks if we find some.
  ext2_ino_t ino_num = 0;
  errcode_t err = ext2fs_namei_follow(filsys_,
                                      EXT2_ROOT_INO /* root */,
                                      EXT2_ROOT_INO /* cwd */,
                                      "/etc/update_engine.conf",
                                      &ino_num);
  if (err != 0)
    return false;

  ext2_inode ino_data;
  if (ext2fs_read_inode(filsys_, ino_num, &ino_data) != 0)
    return false;

  // Load the list of blocks and then the contents of the inodes.
  vector<Extent> extents;
  err = ext2fs_block_iterate2(filsys_,
                              ino_num,
                              BLOCK_FLAG_DATA_ONLY,
                              nullptr,  // block_buf
                              ProcessInodeAllBlocks,
                              &extents);
  if (err != 0)
    return false;

  brillo::Blob blob;
  uint64_t physical_size = utils::BlocksInExtents(extents) * filsys_->blocksize;
  // Sparse holes in the settings file are not supported.
  if (EXT2_I_SIZE(&ino_data) > physical_size)
    return false;
  if (!utils::ReadExtents(
          filename_, extents, &blob, physical_size, filsys_->blocksize))
    return false;

  string text(blob.begin(), blob.begin() + EXT2_I_SIZE(&ino_data));
  return store->LoadFromString(text);
}

}  // namespace chromeos_update_engine
