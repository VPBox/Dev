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

#include "update_engine/payload_consumer/delta_performer.h"

#include <errno.h>
#include <linux/fs.h>

#include <algorithm>
#include <cstring>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <base/files/file_util.h>
#include <base/format_macros.h>
#include <base/metrics/histogram_macros.h>
#include <base/strings/string_number_conversions.h>
#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>
#include <base/time/time.h>
#include <brillo/data_encoding.h>
#include <bsdiff/bspatch.h>
#include <google/protobuf/repeated_field.h>
#include <puffin/puffpatch.h>

#include "update_engine/common/constants.h"
#include "update_engine/common/hardware_interface.h"
#include "update_engine/common/prefs_interface.h"
#include "update_engine/common/subprocess.h"
#include "update_engine/common/terminator.h"
#include "update_engine/payload_consumer/bzip_extent_writer.h"
#include "update_engine/payload_consumer/cached_file_descriptor.h"
#include "update_engine/payload_consumer/download_action.h"
#include "update_engine/payload_consumer/extent_reader.h"
#include "update_engine/payload_consumer/extent_writer.h"
#if USE_FEC
#include "update_engine/payload_consumer/fec_file_descriptor.h"
#endif  // USE_FEC
#include "update_engine/payload_consumer/file_descriptor_utils.h"
#include "update_engine/payload_consumer/mount_history.h"
#if USE_MTD
#include "update_engine/payload_consumer/mtd_file_descriptor.h"
#endif  // USE_MTD
#include "update_engine/payload_consumer/payload_constants.h"
#include "update_engine/payload_consumer/payload_verifier.h"
#include "update_engine/payload_consumer/xz_extent_writer.h"

using google::protobuf::RepeatedPtrField;
using std::min;
using std::string;
using std::vector;

namespace chromeos_update_engine {
const unsigned DeltaPerformer::kProgressLogMaxChunks = 10;
const unsigned DeltaPerformer::kProgressLogTimeoutSeconds = 30;
const unsigned DeltaPerformer::kProgressDownloadWeight = 50;
const unsigned DeltaPerformer::kProgressOperationsWeight = 50;
const uint64_t DeltaPerformer::kCheckpointFrequencySeconds = 1;

namespace {
const int kUpdateStateOperationInvalid = -1;
const int kMaxResumedUpdateFailures = 10;
#if USE_MTD
const int kUbiVolumeAttachTimeout = 5 * 60;
#endif

const uint64_t kCacheSize = 1024 * 1024;  // 1MB

FileDescriptorPtr CreateFileDescriptor(const char* path) {
  FileDescriptorPtr ret;
#if USE_MTD
  if (strstr(path, "/dev/ubi") == path) {
    if (!UbiFileDescriptor::IsUbi(path)) {
      // The volume might not have been attached at boot time.
      int volume_no;
      if (utils::SplitPartitionName(path, nullptr, &volume_no)) {
        utils::TryAttachingUbiVolume(volume_no, kUbiVolumeAttachTimeout);
      }
    }
    if (UbiFileDescriptor::IsUbi(path)) {
      LOG(INFO) << path << " is a UBI device.";
      ret.reset(new UbiFileDescriptor);
    }
  } else if (MtdFileDescriptor::IsMtd(path)) {
    LOG(INFO) << path << " is an MTD device.";
    ret.reset(new MtdFileDescriptor);
  } else {
    LOG(INFO) << path << " is not an MTD nor a UBI device.";
#endif
    ret.reset(new EintrSafeFileDescriptor);
#if USE_MTD
  }
#endif
  return ret;
}

// Opens path for read/write. On success returns an open FileDescriptor
// and sets *err to 0. On failure, sets *err to errno and returns nullptr.
FileDescriptorPtr OpenFile(const char* path,
                           int mode,
                           bool cache_writes,
                           int* err) {
  // Try to mark the block device read-only based on the mode. Ignore any
  // failure since this won't work when passing regular files.
  bool read_only = (mode & O_ACCMODE) == O_RDONLY;
  utils::SetBlockDeviceReadOnly(path, read_only);

  FileDescriptorPtr fd = CreateFileDescriptor(path);
  if (cache_writes && !read_only) {
    fd = FileDescriptorPtr(new CachedFileDescriptor(fd, kCacheSize));
    LOG(INFO) << "Caching writes.";
  }
#if USE_MTD
  // On NAND devices, we can either read, or write, but not both. So here we
  // use O_WRONLY.
  if (UbiFileDescriptor::IsUbi(path) || MtdFileDescriptor::IsMtd(path)) {
    mode = O_WRONLY;
  }
#endif
  if (!fd->Open(path, mode, 000)) {
    *err = errno;
    PLOG(ERROR) << "Unable to open file " << path;
    return nullptr;
  }
  *err = 0;
  return fd;
}

// Discard the tail of the block device referenced by |fd|, from the offset
// |data_size| until the end of the block device. Returns whether the data was
// discarded.
bool DiscardPartitionTail(const FileDescriptorPtr& fd, uint64_t data_size) {
  uint64_t part_size = fd->BlockDevSize();
  if (!part_size || part_size <= data_size)
    return false;

  struct blkioctl_request {
    int number;
    const char* name;
  };
  const vector<blkioctl_request> blkioctl_requests = {
      {BLKDISCARD, "BLKDISCARD"},
      {BLKSECDISCARD, "BLKSECDISCARD"},
#ifdef BLKZEROOUT
      {BLKZEROOUT, "BLKZEROOUT"},
#endif
  };
  for (const auto& req : blkioctl_requests) {
    int error = 0;
    if (fd->BlkIoctl(req.number, data_size, part_size - data_size, &error) &&
        error == 0) {
      return true;
    }
    LOG(WARNING) << "Error discarding the last "
                 << (part_size - data_size) / 1024 << " KiB using ioctl("
                 << req.name << ")";
  }
  return false;
}

}  // namespace

// Computes the ratio of |part| and |total|, scaled to |norm|, using integer
// arithmetic.
static uint64_t IntRatio(uint64_t part, uint64_t total, uint64_t norm) {
  return part * norm / total;
}

void DeltaPerformer::LogProgress(const char* message_prefix) {
  // Format operations total count and percentage.
  string total_operations_str("?");
  string completed_percentage_str("");
  if (num_total_operations_) {
    total_operations_str = std::to_string(num_total_operations_);
    // Upcasting to 64-bit to avoid overflow, back to size_t for formatting.
    completed_percentage_str = base::StringPrintf(
        " (%" PRIu64 "%%)",
        IntRatio(next_operation_num_, num_total_operations_, 100));
  }

  // Format download total count and percentage.
  size_t payload_size = payload_->size;
  string payload_size_str("?");
  string downloaded_percentage_str("");
  if (payload_size) {
    payload_size_str = std::to_string(payload_size);
    // Upcasting to 64-bit to avoid overflow, back to size_t for formatting.
    downloaded_percentage_str = base::StringPrintf(
        " (%" PRIu64 "%%)", IntRatio(total_bytes_received_, payload_size, 100));
  }

  LOG(INFO) << (message_prefix ? message_prefix : "") << next_operation_num_
            << "/" << total_operations_str << " operations"
            << completed_percentage_str << ", " << total_bytes_received_ << "/"
            << payload_size_str << " bytes downloaded"
            << downloaded_percentage_str << ", overall progress "
            << overall_progress_ << "%";
}

void DeltaPerformer::UpdateOverallProgress(bool force_log,
                                           const char* message_prefix) {
  // Compute our download and overall progress.
  unsigned new_overall_progress = 0;
  static_assert(kProgressDownloadWeight + kProgressOperationsWeight == 100,
                "Progress weights don't add up");
  // Only consider download progress if its total size is known; otherwise
  // adjust the operations weight to compensate for the absence of download
  // progress. Also, make sure to cap the download portion at
  // kProgressDownloadWeight, in case we end up downloading more than we
  // initially expected (this indicates a problem, but could generally happen).
  // TODO(garnold) the correction of operations weight when we do not have the
  // total payload size, as well as the conditional guard below, should both be
  // eliminated once we ensure that the payload_size in the install plan is
  // always given and is non-zero. This currently isn't the case during unit
  // tests (see chromium-os:37969).
  size_t payload_size = payload_->size;
  unsigned actual_operations_weight = kProgressOperationsWeight;
  if (payload_size)
    new_overall_progress +=
        min(static_cast<unsigned>(IntRatio(
                total_bytes_received_, payload_size, kProgressDownloadWeight)),
            kProgressDownloadWeight);
  else
    actual_operations_weight += kProgressDownloadWeight;

  // Only add completed operations if their total number is known; we definitely
  // expect an update to have at least one operation, so the expectation is that
  // this will eventually reach |actual_operations_weight|.
  if (num_total_operations_)
    new_overall_progress += IntRatio(
        next_operation_num_, num_total_operations_, actual_operations_weight);

  // Progress ratio cannot recede, unless our assumptions about the total
  // payload size, total number of operations, or the monotonicity of progress
  // is breached.
  if (new_overall_progress < overall_progress_) {
    LOG(WARNING) << "progress counter receded from " << overall_progress_
                 << "% down to " << new_overall_progress << "%; this is a bug";
    force_log = true;
  }
  overall_progress_ = new_overall_progress;

  // Update chunk index, log as needed: if forced by called, or we completed a
  // progress chunk, or a timeout has expired.
  base::TimeTicks curr_time = base::TimeTicks::Now();
  unsigned curr_progress_chunk =
      overall_progress_ * kProgressLogMaxChunks / 100;
  if (force_log || curr_progress_chunk > last_progress_chunk_ ||
      curr_time > forced_progress_log_time_) {
    forced_progress_log_time_ = curr_time + forced_progress_log_wait_;
    LogProgress(message_prefix);
  }
  last_progress_chunk_ = curr_progress_chunk;
}

size_t DeltaPerformer::CopyDataToBuffer(const char** bytes_p,
                                        size_t* count_p,
                                        size_t max) {
  const size_t count = *count_p;
  if (!count)
    return 0;  // Special case shortcut.
  size_t read_len = min(count, max - buffer_.size());
  const char* bytes_start = *bytes_p;
  const char* bytes_end = bytes_start + read_len;
  buffer_.reserve(max);
  buffer_.insert(buffer_.end(), bytes_start, bytes_end);
  *bytes_p = bytes_end;
  *count_p = count - read_len;
  return read_len;
}

bool DeltaPerformer::HandleOpResult(bool op_result,
                                    const char* op_type_name,
                                    ErrorCode* error) {
  if (op_result)
    return true;

  size_t partition_first_op_num =
      current_partition_ ? acc_num_operations_[current_partition_ - 1] : 0;
  LOG(ERROR) << "Failed to perform " << op_type_name << " operation "
             << next_operation_num_ << ", which is the operation "
             << next_operation_num_ - partition_first_op_num
             << " in partition \""
             << partitions_[current_partition_].partition_name() << "\"";
  if (*error == ErrorCode::kSuccess)
    *error = ErrorCode::kDownloadOperationExecutionError;
  return false;
}

int DeltaPerformer::Close() {
  int err = -CloseCurrentPartition();
  LOG_IF(ERROR,
         !payload_hash_calculator_.Finalize() ||
             !signed_hash_calculator_.Finalize())
      << "Unable to finalize the hash.";
  if (!buffer_.empty()) {
    LOG(INFO) << "Discarding " << buffer_.size() << " unused downloaded bytes";
    if (err >= 0)
      err = 1;
  }
  return -err;
}

int DeltaPerformer::CloseCurrentPartition() {
  int err = 0;
  if (source_fd_ && !source_fd_->Close()) {
    err = errno;
    PLOG(ERROR) << "Error closing source partition";
    if (!err)
      err = 1;
  }
  source_fd_.reset();
  if (source_ecc_fd_ && !source_ecc_fd_->Close()) {
    err = errno;
    PLOG(ERROR) << "Error closing ECC source partition";
    if (!err)
      err = 1;
  }
  source_ecc_fd_.reset();
  source_ecc_open_failure_ = false;
  source_path_.clear();

  if (target_fd_ && !target_fd_->Close()) {
    err = errno;
    PLOG(ERROR) << "Error closing target partition";
    if (!err)
      err = 1;
  }
  target_fd_.reset();
  target_path_.clear();
  return -err;
}

bool DeltaPerformer::OpenCurrentPartition() {
  if (current_partition_ >= partitions_.size())
    return false;

  const PartitionUpdate& partition = partitions_[current_partition_];
  size_t num_previous_partitions =
      install_plan_->partitions.size() - partitions_.size();
  const InstallPlan::Partition& install_part =
      install_plan_->partitions[num_previous_partitions + current_partition_];
  // Open source fds if we have a delta payload with minor version >= 2.
  if (payload_->type == InstallPayloadType::kDelta &&
      GetMinorVersion() != kInPlaceMinorPayloadVersion &&
      // With dynamic partitions we could create a new partition in a
      // delta payload, and we shouldn't open source partition in that case.
      install_part.source_size > 0) {
    source_path_ = install_part.source_path;
    int err;
    source_fd_ = OpenFile(source_path_.c_str(), O_RDONLY, false, &err);
    if (!source_fd_) {
      LOG(ERROR) << "Unable to open source partition "
                 << partition.partition_name() << " on slot "
                 << BootControlInterface::SlotName(install_plan_->source_slot)
                 << ", file " << source_path_;
      return false;
    }
  }

  target_path_ = install_part.target_path;
  int err;

  int flags = O_RDWR;
  if (!interactive_)
    flags |= O_DSYNC;

  LOG(INFO) << "Opening " << target_path_ << " partition with"
            << (interactive_ ? "out" : "") << " O_DSYNC";

  target_fd_ = OpenFile(target_path_.c_str(), flags, true, &err);
  if (!target_fd_) {
    LOG(ERROR) << "Unable to open target partition "
               << partition.partition_name() << " on slot "
               << BootControlInterface::SlotName(install_plan_->target_slot)
               << ", file " << target_path_;
    return false;
  }

  LOG(INFO) << "Applying " << partition.operations().size()
            << " operations to partition \"" << partition.partition_name()
            << "\"";

  // Discard the end of the partition, but ignore failures.
  DiscardPartitionTail(target_fd_, install_part.target_size);

  return true;
}

bool DeltaPerformer::OpenCurrentECCPartition() {
  if (source_ecc_fd_)
    return true;

  if (source_ecc_open_failure_)
    return false;

  if (current_partition_ >= partitions_.size())
    return false;

  // No support for ECC in minor version 1 or full payloads.
  if (payload_->type == InstallPayloadType::kFull ||
      GetMinorVersion() == kInPlaceMinorPayloadVersion)
    return false;

#if USE_FEC
  const PartitionUpdate& partition = partitions_[current_partition_];
  size_t num_previous_partitions =
      install_plan_->partitions.size() - partitions_.size();
  const InstallPlan::Partition& install_part =
      install_plan_->partitions[num_previous_partitions + current_partition_];
  string path = install_part.source_path;
  FileDescriptorPtr fd(new FecFileDescriptor());
  if (!fd->Open(path.c_str(), O_RDONLY, 0)) {
    PLOG(ERROR) << "Unable to open ECC source partition "
                << partition.partition_name() << " on slot "
                << BootControlInterface::SlotName(install_plan_->source_slot)
                << ", file " << path;
    source_ecc_open_failure_ = true;
    return false;
  }
  source_ecc_fd_ = fd;
#else
  // No support for ECC compiled.
  source_ecc_open_failure_ = true;
#endif  // USE_FEC

  return !source_ecc_open_failure_;
}

namespace {

void LogPartitionInfoHash(const PartitionInfo& info, const string& tag) {
  string sha256 = brillo::data_encoding::Base64Encode(info.hash());
  LOG(INFO) << "PartitionInfo " << tag << " sha256: " << sha256
            << " size: " << info.size();
}

void LogPartitionInfo(const vector<PartitionUpdate>& partitions) {
  for (const PartitionUpdate& partition : partitions) {
    if (partition.has_old_partition_info()) {
      LogPartitionInfoHash(partition.old_partition_info(),
                           "old " + partition.partition_name());
    }
    LogPartitionInfoHash(partition.new_partition_info(),
                         "new " + partition.partition_name());
  }
}

}  // namespace

uint32_t DeltaPerformer::GetMinorVersion() const {
  if (manifest_.has_minor_version()) {
    return manifest_.minor_version();
  }
  return payload_->type == InstallPayloadType::kDelta
             ? kMaxSupportedMinorPayloadVersion
             : kFullPayloadMinorVersion;
}

bool DeltaPerformer::IsHeaderParsed() const {
  return metadata_size_ != 0;
}

MetadataParseResult DeltaPerformer::ParsePayloadMetadata(
    const brillo::Blob& payload, ErrorCode* error) {
  *error = ErrorCode::kSuccess;

  if (!IsHeaderParsed()) {
    MetadataParseResult result =
        payload_metadata_.ParsePayloadHeader(payload, error);
    if (result != MetadataParseResult::kSuccess)
      return result;

    metadata_size_ = payload_metadata_.GetMetadataSize();
    metadata_signature_size_ = payload_metadata_.GetMetadataSignatureSize();
    major_payload_version_ = payload_metadata_.GetMajorVersion();

    // If the metadata size is present in install plan, check for it immediately
    // even before waiting for that many number of bytes to be downloaded in the
    // payload. This will prevent any attack which relies on us downloading data
    // beyond the expected metadata size.
    if (install_plan_->hash_checks_mandatory) {
      if (payload_->metadata_size != metadata_size_) {
        LOG(ERROR) << "Mandatory metadata size in Omaha response ("
                   << payload_->metadata_size
                   << ") is missing/incorrect, actual = " << metadata_size_;
        *error = ErrorCode::kDownloadInvalidMetadataSize;
        return MetadataParseResult::kError;
      }
    }
  }

  // Now that we have validated the metadata size, we should wait for the full
  // metadata and its signature (if exist) to be read in before we can parse it.
  if (payload.size() < metadata_size_ + metadata_signature_size_)
    return MetadataParseResult::kInsufficientData;

  // Log whether we validated the size or simply trusting what's in the payload
  // here. This is logged here (after we received the full metadata data) so
  // that we just log once (instead of logging n times) if it takes n
  // DeltaPerformer::Write calls to download the full manifest.
  if (payload_->metadata_size == metadata_size_) {
    LOG(INFO) << "Manifest size in payload matches expected value from Omaha";
  } else {
    // For mandatory-cases, we'd have already returned a kMetadataParseError
    // above. We'll be here only for non-mandatory cases. Just send a UMA stat.
    LOG(WARNING) << "Ignoring missing/incorrect metadata size ("
                 << payload_->metadata_size
                 << ") in Omaha response as validation is not mandatory. "
                 << "Trusting metadata size in payload = " << metadata_size_;
  }

  string public_key;
  if (!GetPublicKey(&public_key)) {
    LOG(ERROR) << "Failed to get public key.";
    *error = ErrorCode::kDownloadMetadataSignatureVerificationError;
    return MetadataParseResult::kError;
  }

  // We have the full metadata in |payload|. Verify its integrity
  // and authenticity based on the information we have in Omaha response.
  *error = payload_metadata_.ValidateMetadataSignature(
      payload, payload_->metadata_signature, public_key);
  if (*error != ErrorCode::kSuccess) {
    if (install_plan_->hash_checks_mandatory) {
      // The autoupdate_CatchBadSignatures test checks for this string
      // in log-files. Keep in sync.
      LOG(ERROR) << "Mandatory metadata signature validation failed";
      return MetadataParseResult::kError;
    }

    // For non-mandatory cases, just send a UMA stat.
    LOG(WARNING) << "Ignoring metadata signature validation failures";
    *error = ErrorCode::kSuccess;
  }

  // The payload metadata is deemed valid, it's safe to parse the protobuf.
  if (!payload_metadata_.GetManifest(payload, &manifest_)) {
    LOG(ERROR) << "Unable to parse manifest in update file.";
    *error = ErrorCode::kDownloadManifestParseError;
    return MetadataParseResult::kError;
  }

  manifest_parsed_ = true;
  return MetadataParseResult::kSuccess;
}

#define OP_DURATION_HISTOGRAM(_op_name, _start_time)                         \
  LOCAL_HISTOGRAM_CUSTOM_TIMES(                                              \
      "UpdateEngine.DownloadAction.InstallOperation::" _op_name ".Duration", \
      base::TimeTicks::Now() - _start_time,                                  \
      base::TimeDelta::FromMilliseconds(10),                                 \
      base::TimeDelta::FromMinutes(5),                                       \
      20);

// Wrapper around write. Returns true if all requested bytes
// were written, or false on any error, regardless of progress
// and stores an action exit code in |error|.
bool DeltaPerformer::Write(const void* bytes, size_t count, ErrorCode* error) {
  *error = ErrorCode::kSuccess;
  const char* c_bytes = reinterpret_cast<const char*>(bytes);

  // Update the total byte downloaded count and the progress logs.
  total_bytes_received_ += count;
  UpdateOverallProgress(false, "Completed ");

  while (!manifest_valid_) {
    // Read data up to the needed limit; this is either maximium payload header
    // size, or the full metadata size (once it becomes known).
    const bool do_read_header = !IsHeaderParsed();
    CopyDataToBuffer(
        &c_bytes,
        &count,
        (do_read_header ? kMaxPayloadHeaderSize
                        : metadata_size_ + metadata_signature_size_));

    MetadataParseResult result = ParsePayloadMetadata(buffer_, error);
    if (result == MetadataParseResult::kError)
      return false;
    if (result == MetadataParseResult::kInsufficientData) {
      // If we just processed the header, make an attempt on the manifest.
      if (do_read_header && IsHeaderParsed())
        continue;

      return true;
    }

    // Checks the integrity of the payload manifest.
    if ((*error = ValidateManifest()) != ErrorCode::kSuccess)
      return false;
    manifest_valid_ = true;

    // Clear the download buffer.
    DiscardBuffer(false, metadata_size_);

    block_size_ = manifest_.block_size();

    // This populates |partitions_| and the |install_plan.partitions| with the
    // list of partitions from the manifest.
    if (!ParseManifestPartitions(error))
      return false;

    // |install_plan.partitions| was filled in, nothing need to be done here if
    // the payload was already applied, returns false to terminate http fetcher,
    // but keep |error| as ErrorCode::kSuccess.
    if (payload_->already_applied)
      return false;

    num_total_operations_ = 0;
    for (const auto& partition : partitions_) {
      num_total_operations_ += partition.operations_size();
      acc_num_operations_.push_back(num_total_operations_);
    }

    LOG_IF(WARNING,
           !prefs_->SetInt64(kPrefsManifestMetadataSize, metadata_size_))
        << "Unable to save the manifest metadata size.";
    LOG_IF(WARNING,
           !prefs_->SetInt64(kPrefsManifestSignatureSize,
                             metadata_signature_size_))
        << "Unable to save the manifest signature size.";

    if (!PrimeUpdateState()) {
      *error = ErrorCode::kDownloadStateInitializationError;
      LOG(ERROR) << "Unable to prime the update state.";
      return false;
    }

    if (next_operation_num_ < acc_num_operations_[current_partition_]) {
      if (!OpenCurrentPartition()) {
        *error = ErrorCode::kInstallDeviceOpenError;
        return false;
      }
    }

    if (next_operation_num_ > 0)
      UpdateOverallProgress(true, "Resuming after ");
    LOG(INFO) << "Starting to apply update payload operations";
  }

  while (next_operation_num_ < num_total_operations_) {
    // Check if we should cancel the current attempt for any reason.
    // In this case, *error will have already been populated with the reason
    // why we're canceling.
    if (download_delegate_ && download_delegate_->ShouldCancel(error))
      return false;

    // We know there are more operations to perform because we didn't reach the
    // |num_total_operations_| limit yet.
    if (next_operation_num_ >= acc_num_operations_[current_partition_]) {
      CloseCurrentPartition();
      // Skip until there are operations for current_partition_.
      while (next_operation_num_ >= acc_num_operations_[current_partition_]) {
        current_partition_++;
      }
      if (!OpenCurrentPartition()) {
        *error = ErrorCode::kInstallDeviceOpenError;
        return false;
      }
    }
    const size_t partition_operation_num =
        next_operation_num_ -
        (current_partition_ ? acc_num_operations_[current_partition_ - 1] : 0);

    const InstallOperation& op =
        partitions_[current_partition_].operations(partition_operation_num);

    CopyDataToBuffer(&c_bytes, &count, op.data_length());

    // Check whether we received all of the next operation's data payload.
    if (!CanPerformInstallOperation(op))
      return true;

    // Validate the operation only if the metadata signature is present.
    // Otherwise, keep the old behavior. This serves as a knob to disable
    // the validation logic in case we find some regression after rollout.
    // NOTE: If hash checks are mandatory and if metadata_signature is empty,
    // we would have already failed in ParsePayloadMetadata method and thus not
    // even be here. So no need to handle that case again here.
    if (!payload_->metadata_signature.empty()) {
      // Note: Validate must be called only if CanPerformInstallOperation is
      // called. Otherwise, we might be failing operations before even if there
      // isn't sufficient data to compute the proper hash.
      *error = ValidateOperationHash(op);
      if (*error != ErrorCode::kSuccess) {
        if (install_plan_->hash_checks_mandatory) {
          LOG(ERROR) << "Mandatory operation hash check failed";
          return false;
        }

        // For non-mandatory cases, just send a UMA stat.
        LOG(WARNING) << "Ignoring operation validation errors";
        *error = ErrorCode::kSuccess;
      }
    }

    // Makes sure we unblock exit when this operation completes.
    ScopedTerminatorExitUnblocker exit_unblocker =
        ScopedTerminatorExitUnblocker();  // Avoids a compiler unused var bug.

    base::TimeTicks op_start_time = base::TimeTicks::Now();

    bool op_result;
    switch (op.type()) {
      case InstallOperation::REPLACE:
      case InstallOperation::REPLACE_BZ:
      case InstallOperation::REPLACE_XZ:
        op_result = PerformReplaceOperation(op);
        OP_DURATION_HISTOGRAM("REPLACE", op_start_time);
        break;
      case InstallOperation::ZERO:
      case InstallOperation::DISCARD:
        op_result = PerformZeroOrDiscardOperation(op);
        OP_DURATION_HISTOGRAM("ZERO_OR_DISCARD", op_start_time);
        break;
      case InstallOperation::MOVE:
        op_result = PerformMoveOperation(op);
        OP_DURATION_HISTOGRAM("MOVE", op_start_time);
        break;
      case InstallOperation::BSDIFF:
        op_result = PerformBsdiffOperation(op);
        OP_DURATION_HISTOGRAM("BSDIFF", op_start_time);
        break;
      case InstallOperation::SOURCE_COPY:
        op_result = PerformSourceCopyOperation(op, error);
        OP_DURATION_HISTOGRAM("SOURCE_COPY", op_start_time);
        break;
      case InstallOperation::SOURCE_BSDIFF:
      case InstallOperation::BROTLI_BSDIFF:
        op_result = PerformSourceBsdiffOperation(op, error);
        OP_DURATION_HISTOGRAM("SOURCE_BSDIFF", op_start_time);
        break;
      case InstallOperation::PUFFDIFF:
        op_result = PerformPuffDiffOperation(op, error);
        OP_DURATION_HISTOGRAM("PUFFDIFF", op_start_time);
        break;
      default:
        op_result = false;
    }
    if (!HandleOpResult(op_result, InstallOperationTypeName(op.type()), error))
      return false;

    if (!target_fd_->Flush()) {
      return false;
    }

    next_operation_num_++;
    UpdateOverallProgress(false, "Completed ");
    CheckpointUpdateProgress(false);
  }

  // In major version 2, we don't add dummy operation to the payload.
  // If we already extracted the signature we should skip this step.
  if (major_payload_version_ == kBrilloMajorPayloadVersion &&
      manifest_.has_signatures_offset() && manifest_.has_signatures_size() &&
      signatures_message_data_.empty()) {
    if (manifest_.signatures_offset() != buffer_offset_) {
      LOG(ERROR) << "Payload signatures offset points to blob offset "
                 << manifest_.signatures_offset()
                 << " but signatures are expected at offset " << buffer_offset_;
      *error = ErrorCode::kDownloadPayloadVerificationError;
      return false;
    }
    CopyDataToBuffer(&c_bytes, &count, manifest_.signatures_size());
    // Needs more data to cover entire signature.
    if (buffer_.size() < manifest_.signatures_size())
      return true;
    if (!ExtractSignatureMessage()) {
      LOG(ERROR) << "Extract payload signature failed.";
      *error = ErrorCode::kDownloadPayloadVerificationError;
      return false;
    }
    DiscardBuffer(true, 0);
    // Since we extracted the SignatureMessage we need to advance the
    // checkpoint, otherwise we would reload the signature and try to extract
    // it again.
    // This is the last checkpoint for an update, force this checkpoint to be
    // saved.
    CheckpointUpdateProgress(true);
  }

  return true;
}

bool DeltaPerformer::IsManifestValid() {
  return manifest_valid_;
}

bool DeltaPerformer::ParseManifestPartitions(ErrorCode* error) {
  if (major_payload_version_ == kBrilloMajorPayloadVersion) {
    partitions_.clear();
    for (const PartitionUpdate& partition : manifest_.partitions()) {
      partitions_.push_back(partition);
    }
    manifest_.clear_partitions();
  } else if (major_payload_version_ == kChromeOSMajorPayloadVersion) {
    LOG(INFO) << "Converting update information from old format.";
    PartitionUpdate root_part;
    root_part.set_partition_name(kPartitionNameRoot);
#ifdef __ANDROID__
    LOG(WARNING) << "Legacy payload major version provided to an Android "
                    "build. Assuming no post-install. Please use major version "
                    "2 or newer.";
    root_part.set_run_postinstall(false);
#else
    root_part.set_run_postinstall(true);
#endif  // __ANDROID__
    if (manifest_.has_old_rootfs_info()) {
      *root_part.mutable_old_partition_info() = manifest_.old_rootfs_info();
      manifest_.clear_old_rootfs_info();
    }
    if (manifest_.has_new_rootfs_info()) {
      *root_part.mutable_new_partition_info() = manifest_.new_rootfs_info();
      manifest_.clear_new_rootfs_info();
    }
    *root_part.mutable_operations() = manifest_.install_operations();
    manifest_.clear_install_operations();
    partitions_.push_back(std::move(root_part));

    PartitionUpdate kern_part;
    kern_part.set_partition_name(kPartitionNameKernel);
    kern_part.set_run_postinstall(false);
    if (manifest_.has_old_kernel_info()) {
      *kern_part.mutable_old_partition_info() = manifest_.old_kernel_info();
      manifest_.clear_old_kernel_info();
    }
    if (manifest_.has_new_kernel_info()) {
      *kern_part.mutable_new_partition_info() = manifest_.new_kernel_info();
      manifest_.clear_new_kernel_info();
    }
    *kern_part.mutable_operations() = manifest_.kernel_install_operations();
    manifest_.clear_kernel_install_operations();
    partitions_.push_back(std::move(kern_part));
  }

  // Fill in the InstallPlan::partitions based on the partitions from the
  // payload.
  for (const auto& partition : partitions_) {
    InstallPlan::Partition install_part;
    install_part.name = partition.partition_name();
    install_part.run_postinstall =
        partition.has_run_postinstall() && partition.run_postinstall();
    if (install_part.run_postinstall) {
      install_part.postinstall_path =
          (partition.has_postinstall_path() ? partition.postinstall_path()
                                            : kPostinstallDefaultScript);
      install_part.filesystem_type = partition.filesystem_type();
      install_part.postinstall_optional = partition.postinstall_optional();
    }

    if (partition.has_old_partition_info()) {
      const PartitionInfo& info = partition.old_partition_info();
      install_part.source_size = info.size();
      install_part.source_hash.assign(info.hash().begin(), info.hash().end());
    }

    if (!partition.has_new_partition_info()) {
      LOG(ERROR) << "Unable to get new partition hash info on partition "
                 << install_part.name << ".";
      *error = ErrorCode::kDownloadNewPartitionInfoError;
      return false;
    }
    const PartitionInfo& info = partition.new_partition_info();
    install_part.target_size = info.size();
    install_part.target_hash.assign(info.hash().begin(), info.hash().end());

    install_part.block_size = block_size_;
    if (partition.has_hash_tree_extent()) {
      Extent extent = partition.hash_tree_data_extent();
      install_part.hash_tree_data_offset = extent.start_block() * block_size_;
      install_part.hash_tree_data_size = extent.num_blocks() * block_size_;
      extent = partition.hash_tree_extent();
      install_part.hash_tree_offset = extent.start_block() * block_size_;
      install_part.hash_tree_size = extent.num_blocks() * block_size_;
      uint64_t hash_tree_data_end =
          install_part.hash_tree_data_offset + install_part.hash_tree_data_size;
      if (install_part.hash_tree_offset < hash_tree_data_end) {
        LOG(ERROR) << "Invalid hash tree extents, hash tree data ends at "
                   << hash_tree_data_end << ", but hash tree starts at "
                   << install_part.hash_tree_offset;
        *error = ErrorCode::kDownloadNewPartitionInfoError;
        return false;
      }
      install_part.hash_tree_algorithm = partition.hash_tree_algorithm();
      install_part.hash_tree_salt.assign(partition.hash_tree_salt().begin(),
                                         partition.hash_tree_salt().end());
    }
    if (partition.has_fec_extent()) {
      Extent extent = partition.fec_data_extent();
      install_part.fec_data_offset = extent.start_block() * block_size_;
      install_part.fec_data_size = extent.num_blocks() * block_size_;
      extent = partition.fec_extent();
      install_part.fec_offset = extent.start_block() * block_size_;
      install_part.fec_size = extent.num_blocks() * block_size_;
      uint64_t fec_data_end =
          install_part.fec_data_offset + install_part.fec_data_size;
      if (install_part.fec_offset < fec_data_end) {
        LOG(ERROR) << "Invalid fec extents, fec data ends at " << fec_data_end
                   << ", but fec starts at " << install_part.fec_offset;
        *error = ErrorCode::kDownloadNewPartitionInfoError;
        return false;
      }
      install_part.fec_roots = partition.fec_roots();
    }

    install_plan_->partitions.push_back(install_part);
  }

  if (install_plan_->target_slot != BootControlInterface::kInvalidSlot) {
    if (!InitPartitionMetadata()) {
      *error = ErrorCode::kInstallDeviceOpenError;
      return false;
    }
  }

  if (!install_plan_->LoadPartitionsFromSlots(boot_control_)) {
    LOG(ERROR) << "Unable to determine all the partition devices.";
    *error = ErrorCode::kInstallDeviceOpenError;
    return false;
  }
  LogPartitionInfo(partitions_);
  return true;
}

bool DeltaPerformer::InitPartitionMetadata() {
  BootControlInterface::PartitionMetadata partition_metadata;
  if (manifest_.has_dynamic_partition_metadata()) {
    std::map<string, uint64_t> partition_sizes;
    for (const auto& partition : install_plan_->partitions) {
      partition_sizes.emplace(partition.name, partition.target_size);
    }
    for (const auto& group : manifest_.dynamic_partition_metadata().groups()) {
      BootControlInterface::PartitionMetadata::Group e;
      e.name = group.name();
      e.size = group.size();
      for (const auto& partition_name : group.partition_names()) {
        auto it = partition_sizes.find(partition_name);
        if (it == partition_sizes.end()) {
          // TODO(tbao): Support auto-filling partition info for framework-only
          // OTA.
          LOG(ERROR) << "dynamic_partition_metadata contains partition "
                     << partition_name
                     << " but it is not part of the manifest. "
                     << "This is not supported.";
          return false;
        }
        e.partitions.push_back({partition_name, it->second});
      }
      partition_metadata.groups.push_back(std::move(e));
    }
  }

  bool metadata_updated = false;
  prefs_->GetBoolean(kPrefsDynamicPartitionMetadataUpdated, &metadata_updated);
  if (!boot_control_->InitPartitionMetadata(
          install_plan_->target_slot, partition_metadata, !metadata_updated)) {
    LOG(ERROR) << "Unable to initialize partition metadata for slot "
               << BootControlInterface::SlotName(install_plan_->target_slot);
    return false;
  }
  TEST_AND_RETURN_FALSE(
      prefs_->SetBoolean(kPrefsDynamicPartitionMetadataUpdated, true));
  LOG(INFO) << "InitPartitionMetadata done.";

  return true;
}

bool DeltaPerformer::CanPerformInstallOperation(
    const chromeos_update_engine::InstallOperation& operation) {
  // If we don't have a data blob we can apply it right away.
  if (!operation.has_data_offset() && !operation.has_data_length())
    return true;

  // See if we have the entire data blob in the buffer
  if (operation.data_offset() < buffer_offset_) {
    LOG(ERROR) << "we threw away data it seems?";
    return false;
  }

  return (operation.data_offset() + operation.data_length() <=
          buffer_offset_ + buffer_.size());
}

bool DeltaPerformer::PerformReplaceOperation(
    const InstallOperation& operation) {
  CHECK(operation.type() == InstallOperation::REPLACE ||
        operation.type() == InstallOperation::REPLACE_BZ ||
        operation.type() == InstallOperation::REPLACE_XZ);

  // Since we delete data off the beginning of the buffer as we use it,
  // the data we need should be exactly at the beginning of the buffer.
  TEST_AND_RETURN_FALSE(buffer_offset_ == operation.data_offset());
  TEST_AND_RETURN_FALSE(buffer_.size() >= operation.data_length());

  // Extract the signature message if it's in this operation.
  if (ExtractSignatureMessageFromOperation(operation)) {
    // If this is dummy replace operation, we ignore it after extracting the
    // signature.
    DiscardBuffer(true, 0);
    return true;
  }

  // Setup the ExtentWriter stack based on the operation type.
  std::unique_ptr<ExtentWriter> writer = std::make_unique<DirectExtentWriter>();

  if (operation.type() == InstallOperation::REPLACE_BZ) {
    writer.reset(new BzipExtentWriter(std::move(writer)));
  } else if (operation.type() == InstallOperation::REPLACE_XZ) {
    writer.reset(new XzExtentWriter(std::move(writer)));
  }

  TEST_AND_RETURN_FALSE(
      writer->Init(target_fd_, operation.dst_extents(), block_size_));
  TEST_AND_RETURN_FALSE(writer->Write(buffer_.data(), operation.data_length()));

  // Update buffer
  DiscardBuffer(true, buffer_.size());
  return true;
}

bool DeltaPerformer::PerformZeroOrDiscardOperation(
    const InstallOperation& operation) {
  CHECK(operation.type() == InstallOperation::DISCARD ||
        operation.type() == InstallOperation::ZERO);

  // These operations have no blob.
  TEST_AND_RETURN_FALSE(!operation.has_data_offset());
  TEST_AND_RETURN_FALSE(!operation.has_data_length());

#ifdef BLKZEROOUT
  bool attempt_ioctl = true;
  int request =
      (operation.type() == InstallOperation::ZERO ? BLKZEROOUT : BLKDISCARD);
#else   // !defined(BLKZEROOUT)
  bool attempt_ioctl = false;
  int request = 0;
#endif  // !defined(BLKZEROOUT)

  brillo::Blob zeros;
  for (const Extent& extent : operation.dst_extents()) {
    const uint64_t start = extent.start_block() * block_size_;
    const uint64_t length = extent.num_blocks() * block_size_;
    if (attempt_ioctl) {
      int result = 0;
      if (target_fd_->BlkIoctl(request, start, length, &result) && result == 0)
        continue;
      attempt_ioctl = false;
    }
    // In case of failure, we fall back to writing 0 to the selected region.
    zeros.resize(16 * block_size_);
    for (uint64_t offset = 0; offset < length; offset += zeros.size()) {
      uint64_t chunk_length =
          min(length - offset, static_cast<uint64_t>(zeros.size()));
      TEST_AND_RETURN_FALSE(utils::PWriteAll(
          target_fd_, zeros.data(), chunk_length, start + offset));
    }
  }
  return true;
}

bool DeltaPerformer::PerformMoveOperation(const InstallOperation& operation) {
  // Calculate buffer size. Note, this function doesn't do a sliding
  // window to copy in case the source and destination blocks overlap.
  // If we wanted to do a sliding window, we could program the server
  // to generate deltas that effectively did a sliding window.

  uint64_t blocks_to_read = 0;
  for (int i = 0; i < operation.src_extents_size(); i++)
    blocks_to_read += operation.src_extents(i).num_blocks();

  uint64_t blocks_to_write = 0;
  for (int i = 0; i < operation.dst_extents_size(); i++)
    blocks_to_write += operation.dst_extents(i).num_blocks();

  DCHECK_EQ(blocks_to_write, blocks_to_read);
  brillo::Blob buf(blocks_to_write * block_size_);

  // Read in bytes.
  ssize_t bytes_read = 0;
  for (int i = 0; i < operation.src_extents_size(); i++) {
    ssize_t bytes_read_this_iteration = 0;
    const Extent& extent = operation.src_extents(i);
    const size_t bytes = extent.num_blocks() * block_size_;
    TEST_AND_RETURN_FALSE(extent.start_block() != kSparseHole);
    TEST_AND_RETURN_FALSE(utils::PReadAll(target_fd_,
                                          &buf[bytes_read],
                                          bytes,
                                          extent.start_block() * block_size_,
                                          &bytes_read_this_iteration));
    TEST_AND_RETURN_FALSE(bytes_read_this_iteration ==
                          static_cast<ssize_t>(bytes));
    bytes_read += bytes_read_this_iteration;
  }

  // Write bytes out.
  ssize_t bytes_written = 0;
  for (int i = 0; i < operation.dst_extents_size(); i++) {
    const Extent& extent = operation.dst_extents(i);
    const size_t bytes = extent.num_blocks() * block_size_;
    TEST_AND_RETURN_FALSE(extent.start_block() != kSparseHole);
    TEST_AND_RETURN_FALSE(utils::PWriteAll(target_fd_,
                                           &buf[bytes_written],
                                           bytes,
                                           extent.start_block() * block_size_));
    bytes_written += bytes;
  }
  DCHECK_EQ(bytes_written, bytes_read);
  DCHECK_EQ(bytes_written, static_cast<ssize_t>(buf.size()));
  return true;
}

bool DeltaPerformer::ValidateSourceHash(const brillo::Blob& calculated_hash,
                                        const InstallOperation& operation,
                                        const FileDescriptorPtr source_fd,
                                        ErrorCode* error) {
  brillo::Blob expected_source_hash(operation.src_sha256_hash().begin(),
                                    operation.src_sha256_hash().end());
  if (calculated_hash != expected_source_hash) {
    LOG(ERROR) << "The hash of the source data on disk for this operation "
               << "doesn't match the expected value. This could mean that the "
               << "delta update payload was targeted for another version, or "
               << "that the source partition was modified after it was "
               << "installed, for example, by mounting a filesystem.";
    LOG(ERROR) << "Expected:   sha256|hex = "
               << base::HexEncode(expected_source_hash.data(),
                                  expected_source_hash.size());
    LOG(ERROR) << "Calculated: sha256|hex = "
               << base::HexEncode(calculated_hash.data(),
                                  calculated_hash.size());

    vector<string> source_extents;
    for (const Extent& ext : operation.src_extents()) {
      source_extents.push_back(
          base::StringPrintf("%" PRIu64 ":%" PRIu64,
                             static_cast<uint64_t>(ext.start_block()),
                             static_cast<uint64_t>(ext.num_blocks())));
    }
    LOG(ERROR) << "Operation source (offset:size) in blocks: "
               << base::JoinString(source_extents, ",");

    // Log remount history if this device is an ext4 partition.
    LogMountHistory(source_fd);

    *error = ErrorCode::kDownloadStateInitializationError;
    return false;
  }
  return true;
}

bool DeltaPerformer::PerformSourceCopyOperation(
    const InstallOperation& operation, ErrorCode* error) {
  if (operation.has_src_length())
    TEST_AND_RETURN_FALSE(operation.src_length() % block_size_ == 0);
  if (operation.has_dst_length())
    TEST_AND_RETURN_FALSE(operation.dst_length() % block_size_ == 0);

  TEST_AND_RETURN_FALSE(source_fd_ != nullptr);

  if (operation.has_src_sha256_hash()) {
    brillo::Blob source_hash;
    brillo::Blob expected_source_hash(operation.src_sha256_hash().begin(),
                                      operation.src_sha256_hash().end());

    // We fall back to use the error corrected device if the hash of the raw
    // device doesn't match or there was an error reading the source partition.
    // Note that this code will also fall back if writing the target partition
    // fails.
    bool read_ok = fd_utils::CopyAndHashExtents(source_fd_,
                                                operation.src_extents(),
                                                target_fd_,
                                                operation.dst_extents(),
                                                block_size_,
                                                &source_hash);
    if (read_ok && expected_source_hash == source_hash)
      return true;

    if (!OpenCurrentECCPartition()) {
      // The following function call will return false since the source hash
      // mismatches, but we still want to call it so it prints the appropriate
      // log message.
      return ValidateSourceHash(source_hash, operation, source_fd_, error);
    }

    LOG(WARNING) << "Source hash from RAW device mismatched: found "
                 << base::HexEncode(source_hash.data(), source_hash.size())
                 << ", expected "
                 << base::HexEncode(expected_source_hash.data(),
                                    expected_source_hash.size());

    TEST_AND_RETURN_FALSE(fd_utils::CopyAndHashExtents(source_ecc_fd_,
                                                       operation.src_extents(),
                                                       target_fd_,
                                                       operation.dst_extents(),
                                                       block_size_,
                                                       &source_hash));
    TEST_AND_RETURN_FALSE(
        ValidateSourceHash(source_hash, operation, source_ecc_fd_, error));
    // At this point reading from the the error corrected device worked, but
    // reading from the raw device failed, so this is considered a recovered
    // failure.
    source_ecc_recovered_failures_++;
  } else {
    // When the operation doesn't include a source hash, we attempt the error
    // corrected device first since we can't verify the block in the raw device
    // at this point, but we fall back to the raw device since the error
    // corrected device can be shorter or not available.
    if (OpenCurrentECCPartition() &&
        fd_utils::CopyAndHashExtents(source_ecc_fd_,
                                     operation.src_extents(),
                                     target_fd_,
                                     operation.dst_extents(),
                                     block_size_,
                                     nullptr)) {
      return true;
    }
    TEST_AND_RETURN_FALSE(fd_utils::CopyAndHashExtents(source_fd_,
                                                       operation.src_extents(),
                                                       target_fd_,
                                                       operation.dst_extents(),
                                                       block_size_,
                                                       nullptr));
  }
  return true;
}

FileDescriptorPtr DeltaPerformer::ChooseSourceFD(
    const InstallOperation& operation, ErrorCode* error) {
  if (source_fd_ == nullptr) {
    LOG(ERROR) << "ChooseSourceFD fail: source_fd_ == nullptr";
    return nullptr;
  }

  if (!operation.has_src_sha256_hash()) {
    // When the operation doesn't include a source hash, we attempt the error
    // corrected device first since we can't verify the block in the raw device
    // at this point, but we first need to make sure all extents are readable
    // since the error corrected device can be shorter or not available.
    if (OpenCurrentECCPartition() &&
        fd_utils::ReadAndHashExtents(
            source_ecc_fd_, operation.src_extents(), block_size_, nullptr)) {
      return source_ecc_fd_;
    }
    return source_fd_;
  }

  brillo::Blob source_hash;
  brillo::Blob expected_source_hash(operation.src_sha256_hash().begin(),
                                    operation.src_sha256_hash().end());
  if (fd_utils::ReadAndHashExtents(
          source_fd_, operation.src_extents(), block_size_, &source_hash) &&
      source_hash == expected_source_hash) {
    return source_fd_;
  }
  // We fall back to use the error corrected device if the hash of the raw
  // device doesn't match or there was an error reading the source partition.
  if (!OpenCurrentECCPartition()) {
    // The following function call will return false since the source hash
    // mismatches, but we still want to call it so it prints the appropriate
    // log message.
    ValidateSourceHash(source_hash, operation, source_fd_, error);
    return nullptr;
  }
  LOG(WARNING) << "Source hash from RAW device mismatched: found "
               << base::HexEncode(source_hash.data(), source_hash.size())
               << ", expected "
               << base::HexEncode(expected_source_hash.data(),
                                  expected_source_hash.size());

  if (fd_utils::ReadAndHashExtents(
          source_ecc_fd_, operation.src_extents(), block_size_, &source_hash) &&
      ValidateSourceHash(source_hash, operation, source_ecc_fd_, error)) {
    // At this point reading from the the error corrected device worked, but
    // reading from the raw device failed, so this is considered a recovered
    // failure.
    source_ecc_recovered_failures_++;
    return source_ecc_fd_;
  }
  return nullptr;
}

bool DeltaPerformer::ExtentsToBsdiffPositionsString(
    const RepeatedPtrField<Extent>& extents,
    uint64_t block_size,
    uint64_t full_length,
    string* positions_string) {
  string ret;
  uint64_t length = 0;
  for (const Extent& extent : extents) {
    int64_t start = extent.start_block() * block_size;
    uint64_t this_length =
        min(full_length - length,
            static_cast<uint64_t>(extent.num_blocks()) * block_size);
    ret += base::StringPrintf("%" PRIi64 ":%" PRIu64 ",", start, this_length);
    length += this_length;
  }
  TEST_AND_RETURN_FALSE(length == full_length);
  if (!ret.empty())
    ret.resize(ret.size() - 1);  // Strip trailing comma off
  *positions_string = ret;
  return true;
}

bool DeltaPerformer::PerformBsdiffOperation(const InstallOperation& operation) {
  // Since we delete data off the beginning of the buffer as we use it,
  // the data we need should be exactly at the beginning of the buffer.
  TEST_AND_RETURN_FALSE(buffer_offset_ == operation.data_offset());
  TEST_AND_RETURN_FALSE(buffer_.size() >= operation.data_length());

  string input_positions;
  TEST_AND_RETURN_FALSE(ExtentsToBsdiffPositionsString(operation.src_extents(),
                                                       block_size_,
                                                       operation.src_length(),
                                                       &input_positions));
  string output_positions;
  TEST_AND_RETURN_FALSE(ExtentsToBsdiffPositionsString(operation.dst_extents(),
                                                       block_size_,
                                                       operation.dst_length(),
                                                       &output_positions));

  TEST_AND_RETURN_FALSE(bsdiff::bspatch(target_path_.c_str(),
                                        target_path_.c_str(),
                                        buffer_.data(),
                                        buffer_.size(),
                                        input_positions.c_str(),
                                        output_positions.c_str()) == 0);
  DiscardBuffer(true, buffer_.size());

  if (operation.dst_length() % block_size_) {
    // Zero out rest of final block.
    // TODO(adlr): build this into bspatch; it's more efficient that way.
    const Extent& last_extent =
        operation.dst_extents(operation.dst_extents_size() - 1);
    const uint64_t end_byte =
        (last_extent.start_block() + last_extent.num_blocks()) * block_size_;
    const uint64_t begin_byte =
        end_byte - (block_size_ - operation.dst_length() % block_size_);
    brillo::Blob zeros(end_byte - begin_byte);
    TEST_AND_RETURN_FALSE(utils::PWriteAll(
        target_fd_, zeros.data(), end_byte - begin_byte, begin_byte));
  }
  return true;
}

namespace {

class BsdiffExtentFile : public bsdiff::FileInterface {
 public:
  BsdiffExtentFile(std::unique_ptr<ExtentReader> reader, size_t size)
      : BsdiffExtentFile(std::move(reader), nullptr, size) {}
  BsdiffExtentFile(std::unique_ptr<ExtentWriter> writer, size_t size)
      : BsdiffExtentFile(nullptr, std::move(writer), size) {}

  ~BsdiffExtentFile() override = default;

  bool Read(void* buf, size_t count, size_t* bytes_read) override {
    TEST_AND_RETURN_FALSE(reader_->Read(buf, count));
    *bytes_read = count;
    offset_ += count;
    return true;
  }

  bool Write(const void* buf, size_t count, size_t* bytes_written) override {
    TEST_AND_RETURN_FALSE(writer_->Write(buf, count));
    *bytes_written = count;
    offset_ += count;
    return true;
  }

  bool Seek(off_t pos) override {
    if (reader_ != nullptr) {
      TEST_AND_RETURN_FALSE(reader_->Seek(pos));
      offset_ = pos;
    } else {
      // For writes technically there should be no change of position, or it
      // should be equivalent of current offset.
      TEST_AND_RETURN_FALSE(offset_ == static_cast<uint64_t>(pos));
    }
    return true;
  }

  bool Close() override { return true; }

  bool GetSize(uint64_t* size) override {
    *size = size_;
    return true;
  }

 private:
  BsdiffExtentFile(std::unique_ptr<ExtentReader> reader,
                   std::unique_ptr<ExtentWriter> writer,
                   size_t size)
      : reader_(std::move(reader)),
        writer_(std::move(writer)),
        size_(size),
        offset_(0) {}

  std::unique_ptr<ExtentReader> reader_;
  std::unique_ptr<ExtentWriter> writer_;
  uint64_t size_;
  uint64_t offset_;

  DISALLOW_COPY_AND_ASSIGN(BsdiffExtentFile);
};

}  // namespace

bool DeltaPerformer::PerformSourceBsdiffOperation(
    const InstallOperation& operation, ErrorCode* error) {
  // Since we delete data off the beginning of the buffer as we use it,
  // the data we need should be exactly at the beginning of the buffer.
  TEST_AND_RETURN_FALSE(buffer_offset_ == operation.data_offset());
  TEST_AND_RETURN_FALSE(buffer_.size() >= operation.data_length());
  if (operation.has_src_length())
    TEST_AND_RETURN_FALSE(operation.src_length() % block_size_ == 0);
  if (operation.has_dst_length())
    TEST_AND_RETURN_FALSE(operation.dst_length() % block_size_ == 0);

  FileDescriptorPtr source_fd = ChooseSourceFD(operation, error);
  TEST_AND_RETURN_FALSE(source_fd != nullptr);

  auto reader = std::make_unique<DirectExtentReader>();
  TEST_AND_RETURN_FALSE(
      reader->Init(source_fd, operation.src_extents(), block_size_));
  auto src_file = std::make_unique<BsdiffExtentFile>(
      std::move(reader),
      utils::BlocksInExtents(operation.src_extents()) * block_size_);

  auto writer = std::make_unique<DirectExtentWriter>();
  TEST_AND_RETURN_FALSE(
      writer->Init(target_fd_, operation.dst_extents(), block_size_));
  auto dst_file = std::make_unique<BsdiffExtentFile>(
      std::move(writer),
      utils::BlocksInExtents(operation.dst_extents()) * block_size_);

  TEST_AND_RETURN_FALSE(bsdiff::bspatch(std::move(src_file),
                                        std::move(dst_file),
                                        buffer_.data(),
                                        buffer_.size()) == 0);
  DiscardBuffer(true, buffer_.size());
  return true;
}

namespace {

// A class to be passed to |puffpatch| for reading from |source_fd_| and writing
// into |target_fd_|.
class PuffinExtentStream : public puffin::StreamInterface {
 public:
  // Constructor for creating a stream for reading from an |ExtentReader|.
  PuffinExtentStream(std::unique_ptr<ExtentReader> reader, uint64_t size)
      : PuffinExtentStream(std::move(reader), nullptr, size) {}

  // Constructor for creating a stream for writing to an |ExtentWriter|.
  PuffinExtentStream(std::unique_ptr<ExtentWriter> writer, uint64_t size)
      : PuffinExtentStream(nullptr, std::move(writer), size) {}

  ~PuffinExtentStream() override = default;

  bool GetSize(uint64_t* size) const override {
    *size = size_;
    return true;
  }

  bool GetOffset(uint64_t* offset) const override {
    *offset = offset_;
    return true;
  }

  bool Seek(uint64_t offset) override {
    if (is_read_) {
      TEST_AND_RETURN_FALSE(reader_->Seek(offset));
      offset_ = offset;
    } else {
      // For writes technically there should be no change of position, or it
      // should equivalent of current offset.
      TEST_AND_RETURN_FALSE(offset_ == offset);
    }
    return true;
  }

  bool Read(void* buffer, size_t count) override {
    TEST_AND_RETURN_FALSE(is_read_);
    TEST_AND_RETURN_FALSE(reader_->Read(buffer, count));
    offset_ += count;
    return true;
  }

  bool Write(const void* buffer, size_t count) override {
    TEST_AND_RETURN_FALSE(!is_read_);
    TEST_AND_RETURN_FALSE(writer_->Write(buffer, count));
    offset_ += count;
    return true;
  }

  bool Close() override { return true; }

 private:
  PuffinExtentStream(std::unique_ptr<ExtentReader> reader,
                     std::unique_ptr<ExtentWriter> writer,
                     uint64_t size)
      : reader_(std::move(reader)),
        writer_(std::move(writer)),
        size_(size),
        offset_(0),
        is_read_(reader_ ? true : false) {}

  std::unique_ptr<ExtentReader> reader_;
  std::unique_ptr<ExtentWriter> writer_;
  uint64_t size_;
  uint64_t offset_;
  bool is_read_;

  DISALLOW_COPY_AND_ASSIGN(PuffinExtentStream);
};

}  // namespace

bool DeltaPerformer::PerformPuffDiffOperation(const InstallOperation& operation,
                                              ErrorCode* error) {
  // Since we delete data off the beginning of the buffer as we use it,
  // the data we need should be exactly at the beginning of the buffer.
  TEST_AND_RETURN_FALSE(buffer_offset_ == operation.data_offset());
  TEST_AND_RETURN_FALSE(buffer_.size() >= operation.data_length());

  FileDescriptorPtr source_fd = ChooseSourceFD(operation, error);
  TEST_AND_RETURN_FALSE(source_fd != nullptr);

  auto reader = std::make_unique<DirectExtentReader>();
  TEST_AND_RETURN_FALSE(
      reader->Init(source_fd, operation.src_extents(), block_size_));
  puffin::UniqueStreamPtr src_stream(new PuffinExtentStream(
      std::move(reader),
      utils::BlocksInExtents(operation.src_extents()) * block_size_));

  auto writer = std::make_unique<DirectExtentWriter>();
  TEST_AND_RETURN_FALSE(
      writer->Init(target_fd_, operation.dst_extents(), block_size_));
  puffin::UniqueStreamPtr dst_stream(new PuffinExtentStream(
      std::move(writer),
      utils::BlocksInExtents(operation.dst_extents()) * block_size_));

  const size_t kMaxCacheSize = 5 * 1024 * 1024;  // Total 5MB cache.
  TEST_AND_RETURN_FALSE(puffin::PuffPatch(std::move(src_stream),
                                          std::move(dst_stream),
                                          buffer_.data(),
                                          buffer_.size(),
                                          kMaxCacheSize));
  DiscardBuffer(true, buffer_.size());
  return true;
}

bool DeltaPerformer::ExtractSignatureMessageFromOperation(
    const InstallOperation& operation) {
  if (operation.type() != InstallOperation::REPLACE ||
      !manifest_.has_signatures_offset() ||
      manifest_.signatures_offset() != operation.data_offset()) {
    return false;
  }
  TEST_AND_RETURN_FALSE(manifest_.has_signatures_size() &&
                        manifest_.signatures_size() == operation.data_length());
  TEST_AND_RETURN_FALSE(ExtractSignatureMessage());
  return true;
}

bool DeltaPerformer::ExtractSignatureMessage() {
  TEST_AND_RETURN_FALSE(signatures_message_data_.empty());
  TEST_AND_RETURN_FALSE(buffer_offset_ == manifest_.signatures_offset());
  TEST_AND_RETURN_FALSE(buffer_.size() >= manifest_.signatures_size());
  signatures_message_data_.assign(
      buffer_.begin(), buffer_.begin() + manifest_.signatures_size());

  // Save the signature blob because if the update is interrupted after the
  // download phase we don't go through this path anymore. Some alternatives to
  // consider:
  //
  // 1. On resume, re-download the signature blob from the server and re-verify
  // it.
  //
  // 2. Verify the signature as soon as it's received and don't checkpoint the
  // blob and the signed sha-256 context.
  LOG_IF(WARNING,
         !prefs_->SetString(kPrefsUpdateStateSignatureBlob,
                            signatures_message_data_))
      << "Unable to store the signature blob.";

  LOG(INFO) << "Extracted signature data of size "
            << manifest_.signatures_size() << " at "
            << manifest_.signatures_offset();
  return true;
}

bool DeltaPerformer::GetPublicKey(string* out_public_key) {
  out_public_key->clear();

  if (utils::FileExists(public_key_path_.c_str())) {
    LOG(INFO) << "Verifying using public key: " << public_key_path_;
    return utils::ReadFile(public_key_path_, out_public_key);
  }

  // If this is an official build then we are not allowed to use public key from
  // Omaha response.
  if (!hardware_->IsOfficialBuild() && !install_plan_->public_key_rsa.empty()) {
    LOG(INFO) << "Verifying using public key from Omaha response.";
    return brillo::data_encoding::Base64Decode(install_plan_->public_key_rsa,
                                               out_public_key);
  }

  return true;
}

ErrorCode DeltaPerformer::ValidateManifest() {
  // Perform assorted checks to sanity check the manifest, make sure it
  // matches data from other sources, and that it is a supported version.

  bool has_old_fields =
      (manifest_.has_old_kernel_info() || manifest_.has_old_rootfs_info());
  for (const PartitionUpdate& partition : manifest_.partitions()) {
    has_old_fields = has_old_fields || partition.has_old_partition_info();
  }

  // The presence of an old partition hash is the sole indicator for a delta
  // update.
  InstallPayloadType actual_payload_type =
      has_old_fields ? InstallPayloadType::kDelta : InstallPayloadType::kFull;

  if (payload_->type == InstallPayloadType::kUnknown) {
    LOG(INFO) << "Detected a '"
              << InstallPayloadTypeToString(actual_payload_type)
              << "' payload.";
    payload_->type = actual_payload_type;
  } else if (payload_->type != actual_payload_type) {
    LOG(ERROR) << "InstallPlan expected a '"
               << InstallPayloadTypeToString(payload_->type)
               << "' payload but the downloaded manifest contains a '"
               << InstallPayloadTypeToString(actual_payload_type)
               << "' payload.";
    return ErrorCode::kPayloadMismatchedType;
  }

  // Check that the minor version is compatible.
  if (actual_payload_type == InstallPayloadType::kFull) {
    if (manifest_.minor_version() != kFullPayloadMinorVersion) {
      LOG(ERROR) << "Manifest contains minor version "
                 << manifest_.minor_version()
                 << ", but all full payloads should have version "
                 << kFullPayloadMinorVersion << ".";
      return ErrorCode::kUnsupportedMinorPayloadVersion;
    }
  } else {
    if (manifest_.minor_version() < kMinSupportedMinorPayloadVersion ||
        manifest_.minor_version() > kMaxSupportedMinorPayloadVersion) {
      LOG(ERROR) << "Manifest contains minor version "
                 << manifest_.minor_version()
                 << " not in the range of supported minor versions ["
                 << kMinSupportedMinorPayloadVersion << ", "
                 << kMaxSupportedMinorPayloadVersion << "].";
      return ErrorCode::kUnsupportedMinorPayloadVersion;
    }
  }

  if (major_payload_version_ != kChromeOSMajorPayloadVersion) {
    if (manifest_.has_old_rootfs_info() || manifest_.has_new_rootfs_info() ||
        manifest_.has_old_kernel_info() || manifest_.has_new_kernel_info() ||
        manifest_.install_operations_size() != 0 ||
        manifest_.kernel_install_operations_size() != 0) {
      LOG(ERROR) << "Manifest contains deprecated field only supported in "
                 << "major payload version 1, but the payload major version is "
                 << major_payload_version_;
      return ErrorCode::kPayloadMismatchedType;
    }
  }

  if (manifest_.max_timestamp() < hardware_->GetBuildTimestamp()) {
    LOG(ERROR) << "The current OS build timestamp ("
               << hardware_->GetBuildTimestamp()
               << ") is newer than the maximum timestamp in the manifest ("
               << manifest_.max_timestamp() << ")";
    return ErrorCode::kPayloadTimestampError;
  }

  if (major_payload_version_ == kChromeOSMajorPayloadVersion) {
    if (manifest_.has_dynamic_partition_metadata()) {
      LOG(ERROR)
          << "Should not contain dynamic_partition_metadata for major version "
          << kChromeOSMajorPayloadVersion
          << ". Please use major version 2 or above.";
      return ErrorCode::kPayloadMismatchedType;
    }
  }

  // TODO(garnold) we should be adding more and more manifest checks, such as
  // partition boundaries etc (see chromium-os:37661).

  return ErrorCode::kSuccess;
}

ErrorCode DeltaPerformer::ValidateOperationHash(
    const InstallOperation& operation) {
  if (!operation.data_sha256_hash().size()) {
    if (!operation.data_length()) {
      // Operations that do not have any data blob won't have any operation hash
      // either. So, these operations are always considered validated since the
      // metadata that contains all the non-data-blob portions of the operation
      // has already been validated. This is true for both HTTP and HTTPS cases.
      return ErrorCode::kSuccess;
    }

    // No hash is present for an operation that has data blobs. This shouldn't
    // happen normally for any client that has this code, because the
    // corresponding update should have been produced with the operation
    // hashes. So if it happens it means either we've turned operation hash
    // generation off in DeltaDiffGenerator or it's a regression of some sort.
    // One caveat though: The last operation is a dummy signature operation
    // that doesn't have a hash at the time the manifest is created. So we
    // should not complaint about that operation. This operation can be
    // recognized by the fact that it's offset is mentioned in the manifest.
    if (manifest_.signatures_offset() &&
        manifest_.signatures_offset() == operation.data_offset()) {
      LOG(INFO) << "Skipping hash verification for signature operation "
                << next_operation_num_ + 1;
    } else {
      if (install_plan_->hash_checks_mandatory) {
        LOG(ERROR) << "Missing mandatory operation hash for operation "
                   << next_operation_num_ + 1;
        return ErrorCode::kDownloadOperationHashMissingError;
      }

      LOG(WARNING) << "Cannot validate operation " << next_operation_num_ + 1
                   << " as there's no operation hash in manifest";
    }
    return ErrorCode::kSuccess;
  }

  brillo::Blob expected_op_hash;
  expected_op_hash.assign(operation.data_sha256_hash().data(),
                          (operation.data_sha256_hash().data() +
                           operation.data_sha256_hash().size()));

  brillo::Blob calculated_op_hash;
  if (!HashCalculator::RawHashOfBytes(
          buffer_.data(), operation.data_length(), &calculated_op_hash)) {
    LOG(ERROR) << "Unable to compute actual hash of operation "
               << next_operation_num_;
    return ErrorCode::kDownloadOperationHashVerificationError;
  }

  if (calculated_op_hash != expected_op_hash) {
    LOG(ERROR) << "Hash verification failed for operation "
               << next_operation_num_ << ". Expected hash = ";
    utils::HexDumpVector(expected_op_hash);
    LOG(ERROR) << "Calculated hash over " << operation.data_length()
               << " bytes at offset: " << operation.data_offset() << " = ";
    utils::HexDumpVector(calculated_op_hash);
    return ErrorCode::kDownloadOperationHashMismatch;
  }

  return ErrorCode::kSuccess;
}

#define TEST_AND_RETURN_VAL(_retval, _condition)              \
  do {                                                        \
    if (!(_condition)) {                                      \
      LOG(ERROR) << "VerifyPayload failure: " << #_condition; \
      return _retval;                                         \
    }                                                         \
  } while (0);

ErrorCode DeltaPerformer::VerifyPayload(
    const brillo::Blob& update_check_response_hash,
    const uint64_t update_check_response_size) {
  string public_key;
  if (!GetPublicKey(&public_key)) {
    LOG(ERROR) << "Failed to get public key.";
    return ErrorCode::kDownloadPayloadPubKeyVerificationError;
  }

  // Verifies the download size.
  if (update_check_response_size !=
      metadata_size_ + metadata_signature_size_ + buffer_offset_) {
    LOG(ERROR) << "update_check_response_size (" << update_check_response_size
               << ") doesn't match metadata_size (" << metadata_size_
               << ") + metadata_signature_size (" << metadata_signature_size_
               << ") + buffer_offset (" << buffer_offset_ << ").";
    return ErrorCode::kPayloadSizeMismatchError;
  }

  // Verifies the payload hash.
  TEST_AND_RETURN_VAL(ErrorCode::kDownloadPayloadVerificationError,
                      !payload_hash_calculator_.raw_hash().empty());
  TEST_AND_RETURN_VAL(
      ErrorCode::kPayloadHashMismatchError,
      payload_hash_calculator_.raw_hash() == update_check_response_hash);

  // Verifies the signed payload hash.
  if (public_key.empty()) {
    LOG(WARNING) << "Not verifying signed delta payload -- missing public key.";
    return ErrorCode::kSuccess;
  }
  TEST_AND_RETURN_VAL(ErrorCode::kSignedDeltaPayloadExpectedError,
                      !signatures_message_data_.empty());
  brillo::Blob hash_data = signed_hash_calculator_.raw_hash();
  TEST_AND_RETURN_VAL(ErrorCode::kDownloadPayloadPubKeyVerificationError,
                      hash_data.size() == kSHA256Size);

  if (!PayloadVerifier::VerifySignature(
          signatures_message_data_, public_key, hash_data)) {
    // The autoupdate_CatchBadSignatures test checks for this string
    // in log-files. Keep in sync.
    LOG(ERROR) << "Public key verification failed, thus update failed.";
    return ErrorCode::kDownloadPayloadPubKeyVerificationError;
  }

  LOG(INFO) << "Payload hash matches value in payload.";
  return ErrorCode::kSuccess;
}

void DeltaPerformer::DiscardBuffer(bool do_advance_offset,
                                   size_t signed_hash_buffer_size) {
  // Update the buffer offset.
  if (do_advance_offset)
    buffer_offset_ += buffer_.size();

  // Hash the content.
  payload_hash_calculator_.Update(buffer_.data(), buffer_.size());
  signed_hash_calculator_.Update(buffer_.data(), signed_hash_buffer_size);

  // Swap content with an empty vector to ensure that all memory is released.
  brillo::Blob().swap(buffer_);
}

bool DeltaPerformer::CanResumeUpdate(PrefsInterface* prefs,
                                     const string& update_check_response_hash) {
  int64_t next_operation = kUpdateStateOperationInvalid;
  if (!(prefs->GetInt64(kPrefsUpdateStateNextOperation, &next_operation) &&
        next_operation != kUpdateStateOperationInvalid && next_operation > 0))
    return false;

  string interrupted_hash;
  if (!(prefs->GetString(kPrefsUpdateCheckResponseHash, &interrupted_hash) &&
        !interrupted_hash.empty() &&
        interrupted_hash == update_check_response_hash))
    return false;

  int64_t resumed_update_failures;
  // Note that storing this value is optional, but if it is there it should not
  // be more than the limit.
  if (prefs->GetInt64(kPrefsResumedUpdateFailures, &resumed_update_failures) &&
      resumed_update_failures > kMaxResumedUpdateFailures)
    return false;

  // Sanity check the rest.
  int64_t next_data_offset = -1;
  if (!(prefs->GetInt64(kPrefsUpdateStateNextDataOffset, &next_data_offset) &&
        next_data_offset >= 0))
    return false;

  string sha256_context;
  if (!(prefs->GetString(kPrefsUpdateStateSHA256Context, &sha256_context) &&
        !sha256_context.empty()))
    return false;

  int64_t manifest_metadata_size = 0;
  if (!(prefs->GetInt64(kPrefsManifestMetadataSize, &manifest_metadata_size) &&
        manifest_metadata_size > 0))
    return false;

  int64_t manifest_signature_size = 0;
  if (!(prefs->GetInt64(kPrefsManifestSignatureSize,
                        &manifest_signature_size) &&
        manifest_signature_size >= 0))
    return false;

  return true;
}

bool DeltaPerformer::ResetUpdateProgress(PrefsInterface* prefs, bool quick) {
  TEST_AND_RETURN_FALSE(prefs->SetInt64(kPrefsUpdateStateNextOperation,
                                        kUpdateStateOperationInvalid));
  if (!quick) {
    prefs->SetInt64(kPrefsUpdateStateNextDataOffset, -1);
    prefs->SetInt64(kPrefsUpdateStateNextDataLength, 0);
    prefs->SetString(kPrefsUpdateStateSHA256Context, "");
    prefs->SetString(kPrefsUpdateStateSignedSHA256Context, "");
    prefs->SetString(kPrefsUpdateStateSignatureBlob, "");
    prefs->SetInt64(kPrefsManifestMetadataSize, -1);
    prefs->SetInt64(kPrefsManifestSignatureSize, -1);
    prefs->SetInt64(kPrefsResumedUpdateFailures, 0);
    prefs->Delete(kPrefsPostInstallSucceeded);
    prefs->Delete(kPrefsVerityWritten);
    prefs->Delete(kPrefsDynamicPartitionMetadataUpdated);
  }
  return true;
}

bool DeltaPerformer::CheckpointUpdateProgress(bool force) {
  base::TimeTicks curr_time = base::TimeTicks::Now();
  if (force || curr_time > update_checkpoint_time_) {
    update_checkpoint_time_ = curr_time + update_checkpoint_wait_;
  } else {
    return false;
  }

  Terminator::set_exit_blocked(true);
  if (last_updated_buffer_offset_ != buffer_offset_) {
    // Resets the progress in case we die in the middle of the state update.
    ResetUpdateProgress(prefs_, true);
    TEST_AND_RETURN_FALSE(prefs_->SetString(
        kPrefsUpdateStateSHA256Context, payload_hash_calculator_.GetContext()));
    TEST_AND_RETURN_FALSE(
        prefs_->SetString(kPrefsUpdateStateSignedSHA256Context,
                          signed_hash_calculator_.GetContext()));
    TEST_AND_RETURN_FALSE(
        prefs_->SetInt64(kPrefsUpdateStateNextDataOffset, buffer_offset_));
    last_updated_buffer_offset_ = buffer_offset_;

    if (next_operation_num_ < num_total_operations_) {
      size_t partition_index = current_partition_;
      while (next_operation_num_ >= acc_num_operations_[partition_index])
        partition_index++;
      const size_t partition_operation_num =
          next_operation_num_ -
          (partition_index ? acc_num_operations_[partition_index - 1] : 0);
      const InstallOperation& op =
          partitions_[partition_index].operations(partition_operation_num);
      TEST_AND_RETURN_FALSE(
          prefs_->SetInt64(kPrefsUpdateStateNextDataLength, op.data_length()));
    } else {
      TEST_AND_RETURN_FALSE(
          prefs_->SetInt64(kPrefsUpdateStateNextDataLength, 0));
    }
  }
  TEST_AND_RETURN_FALSE(
      prefs_->SetInt64(kPrefsUpdateStateNextOperation, next_operation_num_));
  return true;
}

bool DeltaPerformer::PrimeUpdateState() {
  CHECK(manifest_valid_);

  int64_t next_operation = kUpdateStateOperationInvalid;
  if (!prefs_->GetInt64(kPrefsUpdateStateNextOperation, &next_operation) ||
      next_operation == kUpdateStateOperationInvalid || next_operation <= 0) {
    // Initiating a new update, no more state needs to be initialized.
    return true;
  }
  next_operation_num_ = next_operation;

  // Resuming an update -- load the rest of the update state.
  int64_t next_data_offset = -1;
  TEST_AND_RETURN_FALSE(
      prefs_->GetInt64(kPrefsUpdateStateNextDataOffset, &next_data_offset) &&
      next_data_offset >= 0);
  buffer_offset_ = next_data_offset;

  // The signed hash context and the signature blob may be empty if the
  // interrupted update didn't reach the signature.
  string signed_hash_context;
  if (prefs_->GetString(kPrefsUpdateStateSignedSHA256Context,
                        &signed_hash_context)) {
    TEST_AND_RETURN_FALSE(
        signed_hash_calculator_.SetContext(signed_hash_context));
  }

  prefs_->GetString(kPrefsUpdateStateSignatureBlob, &signatures_message_data_);

  string hash_context;
  TEST_AND_RETURN_FALSE(
      prefs_->GetString(kPrefsUpdateStateSHA256Context, &hash_context) &&
      payload_hash_calculator_.SetContext(hash_context));

  int64_t manifest_metadata_size = 0;
  TEST_AND_RETURN_FALSE(
      prefs_->GetInt64(kPrefsManifestMetadataSize, &manifest_metadata_size) &&
      manifest_metadata_size > 0);
  metadata_size_ = manifest_metadata_size;

  int64_t manifest_signature_size = 0;
  TEST_AND_RETURN_FALSE(
      prefs_->GetInt64(kPrefsManifestSignatureSize, &manifest_signature_size) &&
      manifest_signature_size >= 0);
  metadata_signature_size_ = manifest_signature_size;

  // Advance the download progress to reflect what doesn't need to be
  // re-downloaded.
  total_bytes_received_ += buffer_offset_;

  // Speculatively count the resume as a failure.
  int64_t resumed_update_failures;
  if (prefs_->GetInt64(kPrefsResumedUpdateFailures, &resumed_update_failures)) {
    resumed_update_failures++;
  } else {
    resumed_update_failures = 1;
  }
  prefs_->SetInt64(kPrefsResumedUpdateFailures, resumed_update_failures);
  return true;
}

}  // namespace chromeos_update_engine
