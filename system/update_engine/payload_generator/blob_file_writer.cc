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

#include "update_engine/payload_generator/blob_file_writer.h"

#include "update_engine/common/utils.h"

namespace chromeos_update_engine {

off_t BlobFileWriter::StoreBlob(const brillo::Blob& blob) {
  base::AutoLock auto_lock(blob_mutex_);
  if (!utils::PWriteAll(blob_fd_, blob.data(), blob.size(), *blob_file_size_))
    return -1;

  off_t result = *blob_file_size_;
  *blob_file_size_ += blob.size();

  stored_blobs_++;
  if (total_blobs_ > 0 && (10 * (stored_blobs_ - 1) / total_blobs_) !=
                              (10 * stored_blobs_ / total_blobs_)) {
    LOG(INFO) << (100 * stored_blobs_ / total_blobs_) << "% complete "
              << stored_blobs_ << "/" << total_blobs_
              << " ops (output size: " << *blob_file_size_ << ")";
  }
  return result;
}

void BlobFileWriter::SetTotalBlobs(size_t total_blobs) {
  total_blobs_ = total_blobs;
  stored_blobs_ = 0;
}

}  // namespace chromeos_update_engine
