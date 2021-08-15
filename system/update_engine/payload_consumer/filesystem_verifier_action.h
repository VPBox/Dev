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

#ifndef UPDATE_ENGINE_PAYLOAD_CONSUMER_FILESYSTEM_VERIFIER_ACTION_H_
#define UPDATE_ENGINE_PAYLOAD_CONSUMER_FILESYSTEM_VERIFIER_ACTION_H_

#include <sys/stat.h>
#include <sys/types.h>

#include <memory>
#include <string>
#include <vector>

#include <brillo/streams/stream.h>

#include "update_engine/common/action.h"
#include "update_engine/common/hash_calculator.h"
#include "update_engine/payload_consumer/install_plan.h"
#include "update_engine/payload_consumer/verity_writer_interface.h"

// This action will hash all the partitions of the target slot involved in the
// update. The hashes are then verified against the ones in the InstallPlan.
// If the target hash does not match, the action will fail. In case of failure,
// the error code will depend on whether the source slot hashes are provided and
// match.

namespace chromeos_update_engine {

// The step FilesystemVerifier is on. On kVerifyTargetHash it computes the hash
// on the target partitions based on the already populated size and verifies it
// matches the one in the target_hash in the InstallPlan.
// If the hash matches, then we skip the kVerifySourceHash step, otherwise we
// need to check if the source is the root cause of the mismatch.
enum class VerifierStep {
  kVerifyTargetHash,
  kVerifySourceHash,
};

class FilesystemVerifierAction : public InstallPlanAction {
 public:
  FilesystemVerifierAction()
      : verity_writer_(verity_writer::CreateVerityWriter()) {}
  ~FilesystemVerifierAction() override = default;

  void PerformAction() override;
  void TerminateProcessing() override;

  // Debugging/logging
  static std::string StaticType() { return "FilesystemVerifierAction"; }
  std::string Type() const override { return StaticType(); }

 private:
  friend class FilesystemVerifierActionTestDelegate;
  // Starts the hashing of the current partition. If there aren't any partitions
  // remaining to be hashed, it finishes the action.
  void StartPartitionHashing();

  // Schedules the asynchronous read of the filesystem.
  void ScheduleRead();

  // Called from the main loop when a single read from |src_stream_| succeeds or
  // fails, calling OnReadDoneCallback() and OnReadErrorCallback() respectively.
  void OnReadDoneCallback(size_t bytes_read);
  void OnReadErrorCallback(const brillo::Error* error);

  // When the read is done, finalize the hash checking of the current partition
  // and continue checking the next one.
  void FinishPartitionHashing();

  // Cleans up all the variables we use for async operations and tells the
  // ActionProcessor we're done w/ |code| as passed in. |cancelled_| should be
  // true if TerminateProcessing() was called.
  void Cleanup(ErrorCode code);

  // The type of the partition that we are verifying.
  VerifierStep verifier_step_ = VerifierStep::kVerifyTargetHash;

  // The index in the install_plan_.partitions vector of the partition currently
  // being hashed.
  size_t partition_index_{0};

  // If not null, the FileStream used to read from the device.
  brillo::StreamPtr src_stream_;

  // Buffer for storing data we read.
  brillo::Blob buffer_;

  bool cancelled_{false};  // true if the action has been cancelled.

  // The install plan we're passed in via the input pipe.
  InstallPlan install_plan_;

  // Calculates the hash of the data.
  std::unique_ptr<HashCalculator> hasher_;

  // Write verity data of the current partition.
  std::unique_ptr<VerityWriterInterface> verity_writer_;

  // Reads and hashes this many bytes from the head of the input stream. When
  // the partition starts to be hashed, this field is initialized from the
  // corresponding InstallPlan::Partition size which is the total size
  // update_engine is expected to write, and may be smaller than the size of the
  // partition in gpt.
  uint64_t partition_size_{0};

  // The byte offset that we are reading in the current partition.
  uint64_t offset_{0};

  DISALLOW_COPY_AND_ASSIGN(FilesystemVerifierAction);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_CONSUMER_FILESYSTEM_VERIFIER_ACTION_H_
