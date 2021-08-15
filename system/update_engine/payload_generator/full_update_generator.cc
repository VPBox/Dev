//
// Copyright (C) 2012 The Android Open Source Project
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

#include "update_engine/payload_generator/full_update_generator.h"

#include <fcntl.h>
#include <inttypes.h>

#include <algorithm>
#include <deque>
#include <memory>

#include <base/format_macros.h>
#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>
#include <base/synchronization/lock.h>
#include <base/threading/simple_thread.h>
#include <brillo/secure_blob.h>

#include "update_engine/common/utils.h"
#include "update_engine/payload_generator/delta_diff_utils.h"

using std::vector;

namespace chromeos_update_engine {

namespace {

const size_t kDefaultFullChunkSize = 1024 * 1024;  // 1 MiB

// This class encapsulates a full update chunk processing thread work. The
// processor reads a chunk of data from the input file descriptor and compresses
// it. The processor will destroy itself when the work is done.
class ChunkProcessor : public base::DelegateSimpleThread::Delegate {
 public:
  // Read a chunk of |size| bytes from |fd| starting at offset |offset|.
  ChunkProcessor(const PayloadVersion& version,
                 int fd,
                 off_t offset,
                 size_t size,
                 BlobFileWriter* blob_file,
                 AnnotatedOperation* aop)
      : version_(version),
        fd_(fd),
        offset_(offset),
        size_(size),
        blob_file_(blob_file),
        aop_(aop) {}
  // We use a default move constructor since all the data members are POD types.
  ChunkProcessor(ChunkProcessor&&) = default;
  ~ChunkProcessor() override = default;

  // Overrides DelegateSimpleThread::Delegate.
  // Run() handles the read from |fd| in a thread-safe way, and stores the
  // new operation to generate the region starting at |offset| of size |size|
  // in the output operation |aop|. The associated blob data is stored in
  // |blob_fd| and |blob_file_size| is updated.
  void Run() override;

 private:
  bool ProcessChunk();

  // Work parameters.
  const PayloadVersion& version_;
  int fd_;
  off_t offset_;
  size_t size_;
  BlobFileWriter* blob_file_;
  AnnotatedOperation* aop_;

  DISALLOW_COPY_AND_ASSIGN(ChunkProcessor);
};

void ChunkProcessor::Run() {
  if (!ProcessChunk()) {
    LOG(ERROR) << "Error processing region at " << offset_ << " of size "
               << size_;
  }
}

bool ChunkProcessor::ProcessChunk() {
  brillo::Blob buffer_in_(size_);
  brillo::Blob op_blob;
  ssize_t bytes_read = -1;
  TEST_AND_RETURN_FALSE(utils::PReadAll(
      fd_, buffer_in_.data(), buffer_in_.size(), offset_, &bytes_read));
  TEST_AND_RETURN_FALSE(bytes_read == static_cast<ssize_t>(size_));

  InstallOperation::Type op_type;
  TEST_AND_RETURN_FALSE(diff_utils::GenerateBestFullOperation(
      buffer_in_, version_, &op_blob, &op_type));

  aop_->op.set_type(op_type);
  TEST_AND_RETURN_FALSE(aop_->SetOperationBlob(op_blob, blob_file_));
  return true;
}

}  // namespace

bool FullUpdateGenerator::GenerateOperations(
    const PayloadGenerationConfig& config,
    const PartitionConfig& old_part,
    const PartitionConfig& new_part,
    BlobFileWriter* blob_file,
    vector<AnnotatedOperation>* aops) {
  TEST_AND_RETURN_FALSE(new_part.ValidateExists());

  // FullUpdateGenerator requires a positive chunk_size, otherwise there will
  // be only one operation with the whole partition which should not be allowed.
  // For performance reasons, we force a small default hard limit of 1 MiB. This
  // limit can be changed in the config, and we will use the smaller of the two
  // soft/hard limits.
  size_t full_chunk_size;
  if (config.hard_chunk_size >= 0) {
    full_chunk_size = std::min(static_cast<size_t>(config.hard_chunk_size),
                               config.soft_chunk_size);
  } else {
    full_chunk_size = std::min(kDefaultFullChunkSize, config.soft_chunk_size);
    LOG(INFO) << "No chunk_size provided, using the default chunk_size for the "
              << "full operations: " << full_chunk_size << " bytes.";
  }
  TEST_AND_RETURN_FALSE(full_chunk_size > 0);
  TEST_AND_RETURN_FALSE(full_chunk_size % config.block_size == 0);

  size_t chunk_blocks = full_chunk_size / config.block_size;
  size_t max_threads = diff_utils::GetMaxThreads();
  LOG(INFO) << "Compressing partition " << new_part.name << " from "
            << new_part.path << " splitting in chunks of " << chunk_blocks
            << " blocks (" << config.block_size << " bytes each) using "
            << max_threads << " threads";

  int in_fd = open(new_part.path.c_str(), O_RDONLY, 0);
  TEST_AND_RETURN_FALSE(in_fd >= 0);
  ScopedFdCloser in_fd_closer(&in_fd);

  // We potentially have all the ChunkProcessors in memory but only
  // |max_threads| will actually hold a block in memory while we process.
  size_t partition_blocks = new_part.size / config.block_size;
  size_t num_chunks = utils::DivRoundUp(partition_blocks, chunk_blocks);
  aops->resize(num_chunks);
  vector<ChunkProcessor> chunk_processors;
  chunk_processors.reserve(num_chunks);
  blob_file->SetTotalBlobs(num_chunks);

  for (size_t i = 0; i < num_chunks; ++i) {
    size_t start_block = i * chunk_blocks;
    // The last chunk could be smaller.
    size_t num_blocks =
        std::min(chunk_blocks, partition_blocks - i * chunk_blocks);

    // Preset all the static information about the operations. The
    // ChunkProcessor will set the rest.
    AnnotatedOperation* aop = aops->data() + i;
    aop->name = base::StringPrintf(
        "<%s-operation-%" PRIuS ">", new_part.name.c_str(), i);
    Extent* dst_extent = aop->op.add_dst_extents();
    dst_extent->set_start_block(start_block);
    dst_extent->set_num_blocks(num_blocks);

    chunk_processors.emplace_back(
        config.version,
        in_fd,
        static_cast<off_t>(start_block) * config.block_size,
        num_blocks * config.block_size,
        blob_file,
        aop);
  }

  // Thread pool used for worker threads.
  base::DelegateSimpleThreadPool thread_pool("full-update-generator",
                                             max_threads);
  thread_pool.Start();
  for (ChunkProcessor& processor : chunk_processors)
    thread_pool.AddWork(&processor);
  thread_pool.JoinAll();

  // All the work done, disable logging.
  blob_file->SetTotalBlobs(0);

  // All the operations must have a type set at this point. Otherwise, a
  // ChunkProcessor failed to complete.
  for (const AnnotatedOperation& aop : *aops) {
    if (!aop.op.has_type())
      return false;
  }
  return true;
}

}  // namespace chromeos_update_engine
